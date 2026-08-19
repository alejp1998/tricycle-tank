/**
 * Tricycle Tank — PixiJS v8 renderer + cockpit wiring.
 * Procedural tank sprites (no external assets), DOM HUD, corner toast,
 * last-key indicator, and the guide modal.
 */
(function () {
  "use strict";

  var S = window.TricycleTank;
  var M = window.TricycleMelodies;
  if (!S || !PIXI) return;

  // ------------------------------------------------------------------ state
  var game = S.createGame(Math.floor(Math.random() * 1e9));
  var stage;
  var cell = 1;
  var racers = {};
  var sounds = { shoot: [], hit: [], spark: [] };
  var audioCtx = null;
  var guideOpen = false;
  var lastKeyLabel = "—";

  var palette = {
    arena: 0x0f1526,
    grid: 0x232e45,
    gridDark: 0x16203a,
    border: 0x3b4a63,
    crate: 0x1c2a44,
    crateBorder: 0x3b4a63,
    playerBody: 0x14b8a6,
    playerTrack: 0x0b3b36,
    playerBarrel: 0x0f766e,
    enemyBody: 0xf43f5e,
    enemyTrack: 0x4c1220,
    enemyBarrel: 0x9f1239,
    bulletPlayer: 0x2dd4bf,
    bulletEnemy: 0xfb7185,
    wedge: 0xf59e0b,
    plate: 0x94a3b8,
  };

  function groundColors() {
    return document.documentElement.getAttribute("data-theme") === "dark"
      ? palette
      : {
          arena: 0xf3f6fb,
          grid: 0xe3eaf5,
          gridDark: 0xeef3fa,
          border: 0xc7d2e4,
          crate: 0xe2e9f4,
          crateBorder: 0xb7c4da,
          playerBody: 0x0d9488,
          playerTrack: 0x134e4a,
          playerBarrel: 0x115e59,
          enemyBody: 0xe11d48,
          enemyTrack: 0x881337,
          enemyBarrel: 0xbe123c,
          bulletPlayer: 0x0d9488,
          bulletEnemy: 0xe11d48,
          wedge: 0xf59e0b,
        };
  }

  function $id(id) {
    return document.getElementById(id);
  }

  // ------------------------------------------------------------------ HUD
  function updateHud() {
    var p = game.player;
    $id("hud-hits").textContent = p.hits + " / 10";
    $id("hud-mag").textContent = p.reloading
      ? "⟳ reloading…"
      : p.magazine + " / " + S.PLAYER.maxMagazine;
    $id("hud-mag").classList.toggle("warn", p.reloading || p.magazine === 0);
    $id("hud-hp").textContent =
      "♥".repeat(Math.max(0, p.hp)) +
      "♡".repeat(Math.max(0, S.PLAYER.maxHp - p.hp));

    var enemies = game.enemies.filter(function (e) {
      return e.hp > 0;
    }).length;
    $id("hud-enemies").textContent = enemies + " / " + game.enemies.length;

    var toast = $id("game-banner");
    var result = $id("hud-result");
    if (game.over) {
      var win = game.won;
      toast.classList.remove("hidden");
      toast.classList.toggle("fail", !win);
      toast.classList.toggle("win", win);
      $id("banner-emoji").textContent = win ? "🏆" : "💥";
      $id("banner-title").textContent = win ? "VICTORY!" : "TANK DESTROYED";
      $id("banner-sub").textContent = win
        ? "10 hits on the enemy tanks — the arena is yours. Press R for a rematch."
        : "Your tank took 5 hits. Press R for a rematch.";
      result.classList.remove("hidden");
      result.classList.toggle("fail", !win);
      result.classList.toggle("win", win);
      result.innerHTML =
        '<div class="hud-result-title">' +
        (win ? "🏆 VICTORY!" : "💥 TANK DESTROYED") +
        "</div>" +
        '<div class="hud-result-sub">' +
        (win
          ? "10 hits landed — the arena is yours!"
          : "Your tank took 5 hits — hull breach.") +
        "</div>" +
        '<div class="hud-result-key">press <b>R</b> to restart</div>';
    } else {
      toast.classList.add("hidden");
      result.classList.add("hidden");
    }

    $id("last-key").textContent = lastKeyLabel;
  }

  function log(msg) {
    var el = $id("log");
    var row = document.createElement("div");
    row.textContent = "› " + msg;
    el.appendChild(row);
    while (el.children.length > 40) el.removeChild(el.firstChild);
    el.scrollTop = el.scrollHeight;
  }

  function markKey(label, action) {
    lastKeyLabel = label;
    $id("last-key").textContent = label;
    document.querySelectorAll(".ctrl[data-action]").forEach(function (b) {
      b.classList.toggle(
        "key-pressed",
        b.getAttribute("data-action") === action,
      );
    });
    updateHud();
  }

  function handleEvent(ev) {
    if (!ev) return;
    if (ev.type === "fire") {
      sounds.shoot.push({ x: ev.x, y: ev.y, t: 0 });
      playShoot();
    } else if (ev.type === "hit") {
      sounds.hit.push({ x: ev.x, y: ev.y, t: 0 });
      log("🎯 Direct hit on " + ev.target + "! Hits: " + ev.hits + "/10");
      playHit();
    } else if (ev.type === "hurt") {
      sounds.hit.push({ x: ev.x, y: ev.y, t: 0 });
      log("💔 Hit taken! HP: " + ev.hp);
      playHit();
    } else if (ev.type === "spark") {
      sounds.hit.push({ x: ev.x, y: ev.y, t: 0 });
    } else if (ev.type === "enemyFire") {
      sounds.shoot.push({ x: ev.x, y: ev.y, t: 0 });
    } else if (ev.type === "empty") {
      log("🔫 Magazine empty — reloading…");
    } else if (ev.type === "reload") {
      log("🔁 Reloading…");
    } else if (ev.type === "reloaded") {
      log("🔫 Magazine full (" + S.PLAYER.maxMagazine + ")");
    } else if (ev.type === "win") {
      log("🏆 VICTORY! 10 hits — the arena is yours!");
      playMelody(0);
    } else if (ev.type === "lose") {
      log("💥 Tank destroyed…");
      playMelody(1);
    }
  }

  // ------------------------------------------------------------------ audio
  function ensureAudio() {
    if (!audioCtx) {
      try {
        var AC = window.AudioContext || window.webkitAudioContext;
        if (AC) audioCtx = new AC();
      } catch (e) {
        audioCtx = null;
      }
    }
    if (audioCtx && audioCtx.state === "suspended") audioCtx.resume();
    return audioCtx;
  }

  function playShoot() {
    if (!ensureAudio() || $id("btn-sound").classList.contains("muted")) return;
    var ctx = audioCtx;
    var osc = ctx.createOscillator();
    var gain = ctx.createGain();
    osc.type = "sawtooth";
    osc.frequency.setValueAtTime(1600, ctx.currentTime);
    osc.frequency.exponentialRampToValueAtTime(400, ctx.currentTime + 0.08);
    gain.gain.setValueAtTime(0.06, ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, ctx.currentTime + 0.09);
    osc.connect(gain);
    gain.connect(ctx.destination);
    osc.start();
    osc.stop(ctx.currentTime + 0.1);
  }

  function playHit() {
    if (!ensureAudio() || $id("btn-sound").classList.contains("muted")) return;
    var ctx = audioCtx;
    var osc = ctx.createOscillator();
    var gain = ctx.createGain();
    osc.type = "square";
    osc.frequency.setValueAtTime(220, ctx.currentTime);
    osc.frequency.exponentialRampToValueAtTime(80, ctx.currentTime + 0.12);
    gain.gain.setValueAtTime(0.09, ctx.currentTime);
    gain.gain.exponentialRampToValueAtTime(0.001, ctx.currentTime + 0.13);
    osc.connect(gain);
    gain.connect(ctx.destination);
    osc.start();
    osc.stop(ctx.currentTime + 0.14);
  }

  function playMelody(idx) {
    if (!ensureAudio() || $id("btn-sound").classList.contains("muted")) return;
    M.playMelody(audioCtx, idx, 0.05);
  }

  // --------------------------------------------------------------- rendering
  function buildTank(g, kind) {
    var colors =
      kind === "player"
        ? { body: g.playerBody, track: g.playerTrack, barrel: g.playerBarrel }
        : { body: g.enemyBody, track: g.enemyTrack, barrel: g.enemyBarrel };
    var tank = new PIXI.Container();

    // tracks (two dark wheels)
    var trackL = new PIXI.Graphics();
    trackL.roundRect(-22, -11, 12, 22, 5).fill(colors.track);
    var trackR = new PIXI.Graphics();
    trackR.roundRect(10, -11, 12, 22, 5).fill(colors.track);

    // body
    var body = new PIXI.Graphics();
    body.roundRect(-12, -9, 24, 18, 5).fill(colors.body);

    // turret group (rotates independently)
    var turret = new PIXI.Container();
    var base = new PIXI.Graphics();
    base.circle(0, 0, 7).fill(colors.body);
    var barrel = new PIXI.Graphics();
    barrel.roundRect(0, -3.5, 20, 7, 2).fill(colors.barrel);
    turret.addChild(base, barrel);

    tank.addChild(trackL, trackR, body, turret);
    tank.turret = turret;
    return tank;
  }

  function buildArena() {
    var c = groundColors();
    var cont = new PIXI.Container();
    var bg = new PIXI.Graphics();
    bg.rect(0, 0, S.ARENA_W, S.ARENA_H).fill(c.arena);
    bg.roundRect(0, 0, S.ARENA_W, S.ARENA_H, 18).stroke({
      width: 3,
      color: c.border,
    });
    cont.addChild(bg);

    // grid
    var grid = new PIXI.Graphics();
    for (var x = 0; x <= S.ARENA_W; x += 64) {
      grid.moveTo(x, 0).lineTo(x, S.ARENA_H);
    }
    for (var y = 0; y <= S.ARENA_H; y += 64) {
      grid.moveTo(0, y).lineTo(S.ARENA_W, y);
    }
    grid.stroke({ width: 1, color: c.grid });
    cont.addChild(grid);

    // obstacles (from the current difficulty's map)
    game.map.obstacles.forEach(function (r) {
      var o = new PIXI.Graphics();
      o.roundRect(r.x, r.y, r.w, r.h, 8)
        .fill(c.crate)
        .stroke({ width: 2, color: c.crateBorder });
      o.moveTo(r.x + 14, r.y).lineTo(r.x, r.y + 14);
      o.moveTo(r.x + r.w - 14, r.y).lineTo(r.x + r.w, r.y + 14);
      o.moveTo(r.x + 14, r.y + r.h).lineTo(r.x, r.y + r.h - 14);
      o.moveTo(r.x + r.w - 14, r.y + r.h).lineTo(r.x + r.w, r.y + r.h - 14);
      o.stroke({ width: 2, color: c.crateBorder });
      cont.addChild(o);
    });

    // spawn pads
    var pad = new PIXI.Graphics();
    pad
      .circle(S.ARENA_W / 2, S.ARENA_H - 90, 30)
      .fill({ color: c.border, alpha: 0.35 });
    pad.stroke({ width: 2, color: c.border });
    cont.addChild(pad);

    return cont;
  }

  function drawFacingIndicator(cont, x, y, angle, colors) {
    // amber wedge in front of the turret — unmistakable facing
    var wedge = new PIXI.Graphics();
    var spread = 0.5;
    var len = 34;
    var a1 = angle - spread;
    var a2 = angle + spread;
    wedge.moveTo(x + Math.cos(a1) * 18, y + Math.sin(a1) * 18);
    wedge.lineTo(x + Math.cos(angle) * len, y + Math.sin(angle) * len);
    wedge.lineTo(x + Math.cos(a2) * 18, y + Math.sin(a2) * 18);
    wedge.closePath();
    wedge.fill({ color: colors.wedge, alpha: 0.55 });
    wedge.stroke({ width: 1.5, color: colors.wedge });
    cont.addChild(wedge);
  }

  function render() {
    var c = groundColors();
    // Clear the whole stage every frame (board, tanks, bullets, effects)
    while (stage.children.length) {
      stage.removeChildAt(0).destroy({ children: true });
    }
    stage.addChild(buildArena());

    var p = game.player;

    // facing wedge + tank
    drawFacingIndicator(stage, p.x, p.y, p.turret, c);
    var tank = buildTank(c, "player");
    tank.x = p.x;
    tank.y = p.y;
    tank.rotation = p.angle;
    tank.turret.rotation = p.turret - p.angle;
    stage.addChild(tank);

    // enemies
    game.enemies.forEach(function (e) {
      if (e.hp <= 0) return;
      var et = buildTank(c, "enemy");
      et.x = e.x;
      et.y = e.y;
      et.rotation = e.angle;
      et.turret.rotation = e.turret - e.angle;
      stage.addChild(et);

      // hp bar
      var bar = new PIXI.Graphics();
      bar.roundRect(e.x - 22, e.y - 34, 44, 6, 3).fill(0x00000066);
      bar
        .roundRect(e.x - 22, e.y - 34, (44 * e.hp) / 3, 6, 3)
        .fill(e.hp === 1 ? 0xef4444 : 0xf59e0b);
      stage.addChild(bar);

      // name
      var label = new PIXI.Text({
        text: e.name,
        style: { fontSize: 11, fill: 0x94a3b8, fontWeight: "700" },
      });
      label.anchor.set(0.5, 0);
      label.x = e.x;
      label.y = e.y - 40;
      stage.addChild(label);
    });

    // bullets
    game.bullets.forEach(function (b) {
      var dot = new PIXI.Graphics();
      dot.circle(0, 0, 4).fill(c.bulletPlayer);
      dot.circle(0, 0, 8).fill({ color: c.bulletPlayer, alpha: 0.25 });
      dot.x = b.x;
      dot.y = b.y;
      stage.addChild(dot);
    });
    game.enemyBullets.forEach(function (b) {
      var dot = new PIXI.Graphics();
      dot.circle(0, 0, 4).fill(c.bulletEnemy);
      dot.circle(0, 0, 8).fill({ color: c.bulletEnemy, alpha: 0.25 });
      dot.x = b.x;
      dot.y = b.y;
      stage.addChild(dot);
    });

    // sparks / flashes
    for (var i = sounds.hit.length - 1; i >= 0; i--) {
      var s = sounds.hit[i];
      s.t += 0.016;
      if (s.t > 0.25) {
        sounds.hit.splice(i, 1);
        continue;
      }
      var ring = new PIXI.Graphics();
      ring
        .circle(s.x, s.y, 6 + s.t * 60)
        .stroke({ width: 2, color: c.wedge, alpha: 1 - s.t * 3 });
      stage.addChild(ring);
    }
    for (var j = sounds.shoot.length - 1; j >= 0; j--) {
      var f = sounds.shoot[j];
      f.t += 0.016;
      if (f.t > 0.12) {
        sounds.shoot.splice(j, 1);
        continue;
      }
      var flash = new PIXI.Graphics();
      flash.circle(f.x, f.y, 10).fill({ color: 0xfbbf24, alpha: 0.5 });
      stage.addChild(flash);
    }
  }

  // ------------------------------------------------------------------ input
  var input = {
    throttle: 0,
    steer: 0,
    aim: null,
    fire: false,
    reload: false,
    turretDir: 0,
  };
  var keys = {};

  function updateInput() {
    input.throttle =
      (keys["ArrowUp"] || keys["KeyW"] ? 1 : 0) -
      (keys["ArrowDown"] || keys["KeyS"] ? 1 : 0);
    input.steer =
      (keys["ArrowRight"] || keys["KeyD"] ? 1 : 0) -
      (keys["ArrowLeft"] || keys["KeyA"] ? 1 : 0);
  }

  function wireInput() {
    document.addEventListener("keydown", function (e) {
      if (guideOpen) return;
      var k = e.code;
      if (
        ["ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight", "Space"].indexOf(
          k,
        ) !== -1
      )
        e.preventDefault();
      if (k === "KeyR" || k === "KeyQ" || k === "KeyG" || k === "KeyP")
        e.preventDefault();

      if (k === "KeyR") {
        restart();
        markKey("R — restart", "restart");
        return;
      }
      if (k === "KeyQ") {
        log("🛑 Match abandoned.");
        game.over = true;
        game.won = false;
        updateHud();
        markKey("Q — quit", "quit");
        return;
      }
      if (k === "KeyG") {
        toggleGuide();
        return;
      }
      if (k === "KeyM") {
        toggleSound();
        return;
      }
      if (k === "Digit1") {
        playMelody(0);
        log("🎵 Buzzer: Tetris");
        return;
      }
      if (k === "Digit2") {
        playMelody(1);
        log("🎵 Buzzer: Star Wars");
        return;
      }
      if (k === "Digit3") {
        playMelody(2);
        log("🎵 Buzzer: Despacito");
        return;
      }

      keys[k] = true;
      updateInput();

      if (k === "Space") {
        input.fire = true;
        markKey("Space — fire", "fire");
      } else if (k === "KeyR" || k === "KeyP") {
        input.reload = true;
        markKey("R — reload", "reload");
      } else if (k === "KeyQ") {
        input.turretDir = -1;
        markKey("Q — turret ←", "turret-left");
      } else if (k === "KeyE") {
        input.turretDir = 1;
        markKey("E — turret →", "turret-right");
      } else if (k === "ArrowUp" || k === "KeyW") {
        markKey("↑ / W — drive", "drive");
      } else if (k === "ArrowDown" || k === "KeyS") {
        markKey("↓ / S — reverse", "drive");
      } else if (k === "ArrowLeft" || k === "KeyA") {
        markKey("← / A — turn left", "turn");
      } else if (k === "ArrowRight" || k === "KeyD") {
        markKey("→ / D — turn right", "turn");
      }
    });

    document.addEventListener("keyup", function (e) {
      keys[e.code] = false;
      if (e.code === "Space") input.fire = false;
      if (e.code === "KeyQ" || e.code === "KeyE") input.turretDir = 0;
      updateInput();
    });

    var stageEl = $id("pixi-stage");
    stageEl.addEventListener("mousemove", function (e) {
      // Map against the live canvas rect (scale-safe even if CSS constrains it)
      var cr = app.canvas.getBoundingClientRect();
      var ccell = cr.width / S.ARENA_W;
      input.aim = Math.atan2(
        e.clientY - cr.top - game.player.y * ccell,
        e.clientX - cr.left - game.player.x * ccell,
      );
    });
    stageEl.addEventListener("click", function () {
      input.fire = true;
      markKey("click — fire", "fire");
    });
    stageEl.addEventListener("contextmenu", function (e) {
      e.preventDefault();
      input.reload = true;
      markKey("right-click — reload", "reload");
    });

    document.querySelectorAll(".ctrl[data-action]").forEach(function (btn) {
      btn.addEventListener("click", function () {
        var a = btn.getAttribute("data-action");
        if (a === "fire") {
          input.fire = true;
          markKey("click — fire", "fire");
        } else if (a === "reload") {
          input.reload = true;
          markKey("click — reload", "reload");
        } else if (a === "turret-left") {
          input.turretDir = -1;
          markKey("◀ — turret", "turret-left");
        } else if (a === "turret-right") {
          input.turretDir = 1;
          markKey("▶ — turret", "turret-right");
        } else if (a === "drive") {
          input.throttle = 1;
          markKey("▲ — drive", "drive");
        } else if (a === "reverse") {
          input.throttle = -1;
          markKey("▼ — reverse", "drive");
        } else if (a === "turn") {
          input.steer = 1;
          markKey("◀ — turn", "turn");
        } else if (a === "turn-r") {
          input.steer = -1;
          markKey("▶ — turn", "turn");
        }
        window.setTimeout(function () {
          if (a === "fire") input.fire = false;
          if (a === "reload") input.reload = false;
          if (a === "turret-left" || a === "turret-right") input.turretDir = 0;
          if (a === "drive" || a === "reverse") input.throttle = 0;
          if (a === "turn" || a === "turn-r") input.steer = 0;
        }, 180);
      });
    });

    $id("btn-restart").addEventListener("click", function () {
      restart();
      markKey("click — restart", "restart");
    });
    $id("difficulty").addEventListener("change", function () {
      restart();
      log(
        "🎚️ Difficulty set to " + $id("difficulty").value.toUpperCase() + ".",
      );
    });
    $id("btn-sound").addEventListener("click", toggleSound);
    $id("btn-theme").addEventListener("click", toggleTheme);
    $id("btn-guide").addEventListener("click", toggleGuide);
    document.querySelectorAll("[data-close-guide]").forEach(function (el) {
      el.addEventListener("click", toggleGuide);
    });
  }

  function restart() {
    var diff = $id("difficulty").value;
    S.reset(game, Math.floor(Math.random() * 1e9), diff);
    input = {
      throttle: 0,
      steer: 0,
      aim: null,
      fire: false,
      reload: false,
      turretDir: 0,
    };
    sounds.hit = [];
    sounds.shoot = [];
    log(
      "🔄 " +
        game.map.name +
        " (" +
        diff.toUpperCase() +
        ") — 10 hits to win, 5 HP to lose.",
    );
    updateHud();
    render();
  }

  function toggleSound() {
    var btn = $id("btn-sound");
    btn.classList.toggle("muted");
    btn.textContent = btn.classList.contains("muted") ? "🔇 Sound" : "🔊 Sound";
  }

  function toggleTheme() {
    var root = document.documentElement;
    var next = root.getAttribute("data-theme") === "dark" ? "light" : "dark";
    root.setAttribute("data-theme", next);
    try {
      localStorage.setItem("theme", next);
    } catch (e) {
      /* ignore */
    }
    render();
  }

  function toggleGuide() {
    guideOpen = !guideOpen;
    $id("guide").classList.toggle("hidden", !guideOpen);
  }

  // ------------------------------------------------------------------ main
  function layout() {
    var wrap = $id("pixi-stage");
    var w = wrap.clientWidth;
    var h = wrap.clientHeight;
    cell = Math.min(w / S.ARENA_W, h / S.ARENA_H);
    var cw = Math.floor(S.ARENA_W * cell);
    var ch = Math.floor(S.ARENA_H * cell);
    // The renderer screen stays at WORLD size (1280x800) so the whole map is
    // always in view; the CSS size letterboxes the canvas to fit the stage.
    app.renderer.resize(S.ARENA_W, S.ARENA_H);
    app.canvas.style.width = cw + "px";
    app.canvas.style.height = ch + "px";
  }

  var app = new PIXI.Application();
  app
    .init({
      background: 0x0b0f19,
      antialias: true,
      resolution: Math.max(2, window.devicePixelRatio || 1),
    })
    .then(function () {
      window.__tankGame = game; // debug/test handle
      var wrap = $id("pixi-stage");
      wrap.appendChild(app.canvas);
      stage = new PIXI.Container();
      window.__tankStage = stage; // debug handle
      window.__tankApp = app;
      app.stage.addChild(stage);
      window.addEventListener("resize", layout);
      layout();

      // theme boot (inline head script already set data-theme)

      log(
        "🚁 Tricycle Tank ONLINE — " +
          game.map.name +
          " (" +
          game.difficulty.toUpperCase() +
          ")",
      );
      log(
        "🔫 10-bullet magazine · R reloads · 1/2/3 play Tetris / Star Wars / Despacito",
      );
      updateHud();
      render();
      wireInput();

      var last = performance.now();
      function loop(now) {
        var dt = Math.min(0.05, (now - last) / 1000);
        last = now;
        if (!game.over) {
          input.aim = input.aim; // mouse aim persists
          S.update(game, input, dt);
          if (game.lastEvent) {
            handleEvent(game.lastEvent);
            game.lastEvent = null;
          }
        }
        render();
        updateHud();
        requestAnimationFrame(loop);
      }
      requestAnimationFrame(loop);
    });
})();
