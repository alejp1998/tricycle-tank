/**
 * Tricycle Tank — pure game logic (faithful to the Raspberry Pi original).
 * No DOM / Pixi dependencies: works in the browser and under node:test.
 *
 * The real game: a tricycle tank with a dual-servo turret and an IR laser,
 * driven by an Xbox 360 controller through four parallel FSMs. This port
 * keeps the same rules: 10-bullet magazine with manual reload, 10 hits to
 * win, HP to lose, AI opponents, and the arena obstacles.
 */
(function (root, factory) {
  if (typeof module === "object" && module.exports) module.exports = factory();
  else root.TricycleTank = factory();
})(typeof self !== "undefined" ? self : this, function () {
  "use strict";

  const ARENA_W = 1280;
  const ARENA_H = 800;

  const PLAYER = {
    maxHp: 5,
    maxMagazine: 10,
    reloadMs: 1500,
    fireCooldownMs: 320,
    speed: 160,
    turnSpeed: 2.6,
    radius: 20,
  };

  const ENEMY = {
    speed: 95,
    turnSpeed: 1.8,
    fireRange: 520,
    fireCooldownMs: 1600,
    inaccuracy: 0.11, // half-spread (radians) of enemy fire
    radius: 20,
  };

  const BULLET = { speed: 420, life: 2.0 };

  // Difficulty presets: map layout, enemy roster, and AI tuning.
  // The hit economy always allows 10 hits (win condition):
  //   easy   2 enemies x 5 HP  = 10 hits available
  //   medium 3 enemies x 4 HP  = 12
  //   hard   4 enemies x 3 HP  = 12
  const MAPS = {
    easy: {
      name: "Training Grounds",
      desc: "Open arena, one slow target tank",
      obstacles: [
        { x: 560, y: 330, w: 160, h: 60 },
        { x: 240, y: 180, w: 130, h: 60 },
      ],
      enemies: [
        { x: 640, y: 110, hp: 5, name: "PRAXIS" },
        { x: 260, y: 520, hp: 5, name: "SCRAP" },
      ],
      enemyCfg: {
        speed: 70,
        fireRange: 380,
        fireCooldownMs: 2400,
        inaccuracy: 0.3,
      },
    },
    medium: {
      name: "Industrial Zone",
      desc: "Crates everywhere, three hunter tanks",
      obstacles: [
        { x: 240, y: 180, w: 130, h: 60 },
        { x: 910, y: 180, w: 130, h: 60 },
        { x: 120, y: 420, w: 60, h: 140 },
        { x: 1100, y: 420, w: 60, h: 140 },
        { x: 560, y: 330, w: 160, h: 60 },
        { x: 540, y: 150, w: 60, h: 120 },
      ],
      enemies: [
        { x: 220, y: 120, hp: 4, name: "BRIGADIER" },
        { x: 640, y: 90, hp: 4, name: "KORVUS" },
        { x: 1060, y: 120, hp: 4, name: "VANDAL" },
      ],
      enemyCfg: {
        speed: 95,
        fireRange: 520,
        fireCooldownMs: 1600,
        inaccuracy: 0.11,
      },
    },
    hard: {
      name: "Fortress Siege",
      desc: "Dense walls, four relentless tanks",
      obstacles: [
        { x: 240, y: 180, w: 130, h: 60 },
        { x: 910, y: 180, w: 130, h: 60 },
        { x: 120, y: 420, w: 60, h: 140 },
        { x: 1100, y: 420, w: 60, h: 140 },
        { x: 560, y: 330, w: 160, h: 60 },
        { x: 540, y: 150, w: 60, h: 120 },
        { x: 480, y: 430, w: 320, h: 46 },
        { x: 170, y: 300, w: 60, h: 130 },
        { x: 1050, y: 300, w: 60, h: 130 },
      ],
      enemies: [
        { x: 160, y: 110, hp: 3, name: "BRIGADIER" },
        { x: 640, y: 80, hp: 3, name: "KORVUS" },
        { x: 1120, y: 110, hp: 3, name: "VANDAL" },
        { x: 380, y: 170, hp: 3, name: "MANTIS" },
      ],
      enemyCfg: {
        speed: 120,
        fireRange: 620,
        fireCooldownMs: 1100,
        inaccuracy: 0.05,
      },
    },
  };

  /** Deterministic PRNG (mulberry32) so tests are reproducible. */
  function mulberry32(seed) {
    let a = seed >>> 0;
    return function () {
      a |= 0;
      a = (a + 0x6d2b79f5) | 0;
      let t = Math.imul(a ^ (a >>> 15), 1 | a);
      t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t;
      return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
    };
  }

  function clamp(v, lo, hi) {
    return v < lo ? lo : v > hi ? hi : v;
  }

  function dist(a, b) {
    const dx = a.x - b.x;
    const dy = a.y - b.y;
    return Math.sqrt(dx * dx + dy * dy);
  }

  function pointInRect(px, py, r) {
    return px >= r.x && px <= r.x + r.w && py >= r.y && py <= r.y + r.h;
  }

  function circleHitsRect(cx, cy, r, rect) {
    const nx = clamp(cx, rect.x, rect.x + rect.w);
    const ny = clamp(cy, rect.y, rect.y + rect.h);
    const dx = cx - nx;
    const dy = cy - ny;
    return dx * dx + dy * dy <= r * r;
  }

  function spawnEnemies(map, rng) {
    return map.enemies.map(function (s, i) {
      return {
        x: s.x,
        y: s.y,
        angle: Math.PI / 2, // facing down (toward the player spawn)
        turret: Math.PI / 2,
        hp: s.hp,
        name: s.name,
        fireCd: rng() * 1.5,
        turnDir: rng() > 0.5 ? 1 : -1,
      };
    });
  }

  function createGame(seed, difficulty) {
    const diff = MAPS[difficulty] ? difficulty : "medium";
    const map = MAPS[diff];
    const rng = mulberry32(seed === undefined ? 1 : seed);
    return {
      t: 0,
      difficulty: diff,
      map: map,
      enemyCfg: map.enemyCfg,
      over: false,
      won: false,
      player: {
        x: ARENA_W / 2,
        y: ARENA_H - 90,
        angle: -Math.PI / 2,
        turret: -Math.PI / 2,
        hp: PLAYER.maxHp,
        magazine: PLAYER.maxMagazine,
        reloading: false,
        reloadT: 0,
        fireCd: 0,
        hits: 0,
      },
      enemies: spawnEnemies(map, rng),
      bullets: [],
      enemyBullets: [],
      sparks: [],
      rng: rng,
      lastEvent: null,
    };
  }

  function reset(game, seed, difficulty) {
    const fresh = createGame(seed, difficulty || game.difficulty);
    game.t = fresh.t;
    game.difficulty = fresh.difficulty;
    game.map = fresh.map;
    game.enemyCfg = fresh.enemyCfg;
    game.over = fresh.over;
    game.won = fresh.won;
    game.player = fresh.player;
    game.enemies = fresh.enemies;
    game.bullets = fresh.bullets;
    game.enemyBullets = fresh.enemyBullets;
    game.sparks = fresh.sparks;
    game.rng = fresh.rng;
    game.lastEvent = fresh.lastEvent;
  }

  function fireBullet(game, owner, x, y, angle) {
    const list = owner === "player" ? game.bullets : game.enemyBullets;
    list.push({
      x: x,
      y: y,
      vx: Math.cos(angle) * BULLET.speed,
      vy: Math.sin(angle) * BULLET.speed,
      angle: angle,
      owner: owner,
      life: BULLET.life,
    });
  }

  function playerFire(game) {
    const p = game.player;
    if (p.fireCd > 0) return;
    if (p.magazine <= 0) {
      // Empty magazine: auto-start a reload instead of firing
      if (!p.reloading) {
        p.reloading = true;
        p.reloadT = PLAYER.reloadMs / 1000;
        game.lastEvent = { type: "empty" };
      }
      return;
    }
    if (p.reloading) return;
    p.magazine--;
    p.fireCd = PLAYER.fireCooldownMs / 1000;
    fireBullet(game, "player", p.x, p.y, p.turret);
    game.lastEvent = { type: "fire", x: p.x, y: p.y, angle: p.turret };
    if (p.magazine === 0) {
      p.reloading = true;
      p.reloadT = PLAYER.reloadMs / 1000;
      game.lastEvent = { type: "empty" };
    }
  }

  function startReload(game) {
    const p = game.player;
    if (p.reloading || p.magazine === PLAYER.maxMagazine) return;
    p.reloading = true;
    p.reloadT = PLAYER.reloadMs / 1000;
    game.lastEvent = { type: "reload" };
  }

  function enemyUpdate(game, e, dt) {
    const p = game.player;
    // Steer toward the player, dodging obstacles with a simple side-step
    const desired = Math.atan2(p.y - e.y, p.x - e.x);
    let dAng = desired - e.angle;
    while (dAng > Math.PI) dAng -= 2 * Math.PI;
    while (dAng < -Math.PI) dAng += 2 * Math.PI;

    let blocked = false;
    for (const r of game.map.obstacles) {
      if (
        circleHitsRect(
          e.x + Math.cos(e.angle) * 40,
          e.y + Math.sin(e.angle) * 40,
          14,
          r,
        )
      ) {
        blocked = true;
        break;
      }
    }
    const turn = blocked ? e.turnDir : clamp(dAng, -1, 1);
    e.angle += turn * ENEMY.turnSpeed * dt;
    e.turret = e.angle;

    const cfg = game.enemyCfg || ENEMY;
    const d = dist(e, p);
    if (!blocked && d > 130) {
      e.x += Math.cos(e.angle) * cfg.speed * dt;
      e.y += Math.sin(e.angle) * cfg.speed * dt;
    }

    // Keep inside the arena
    e.x = clamp(e.x, 40, ARENA_W - 40);
    e.y = clamp(e.y, 40, ARENA_H - 40);

    // Shoot when in range (inaccuracy scales with difficulty)
    e.fireCd -= dt;
    if (e.fireCd <= 0 && d < cfg.fireRange) {
      e.fireCd = cfg.fireCooldownMs / 1000;
      const aim = e.angle + (game.rng() - 0.5) * 2 * cfg.inaccuracy;
      fireBullet(game, "enemy", e.x, e.y, aim);
      game.lastEvent = { type: "enemyFire", x: e.x, y: e.y };
    }
  }

  function updateBullets(game, list, dt) {
    const p = game.player;
    const STEP_PX = 8; // sub-step so fast bullets can't tunnel through tanks
    for (let i = list.length - 1; i >= 0; i--) {
      const b = list[i];
      const travel = Math.hypot(b.vx * dt, b.vy * dt);
      const steps = Math.max(1, Math.ceil(travel / STEP_PX));
      const sdt = dt / steps;
      let dead = false;

      for (let s = 0; s < steps && !dead; s++) {
        b.x += b.vx * sdt;
        b.y += b.vy * sdt;
        b.life -= sdt;

        if (
          b.life <= 0 ||
          b.x < 0 ||
          b.x > ARENA_W ||
          b.y < 0 ||
          b.y > ARENA_H
        ) {
          dead = true;
          break;
        }

        // Obstacles absorb bullets
        for (const r of game.map.obstacles) {
          if (pointInRect(b.x, b.y, r)) {
            dead = true;
            game.lastEvent = { type: "spark", x: b.x, y: b.y };
            break;
          }
        }
        if (dead) break;

        if (b.owner === "player") {
          for (const e of game.enemies) {
            if (e.hp > 0 && dist(b, e) < ENEMY.radius + 6) {
              e.hp--;
              p.hits++;
              dead = true;
              game.lastEvent = {
                type: "hit",
                x: b.x,
                y: b.y,
                target: e.name,
                hits: p.hits,
              };
              if (p.hits >= 10) {
                game.over = true;
                game.won = true;
                game.lastEvent = { type: "win" };
              }
              break;
            }
          }
        } else if (b.owner === "enemy") {
          if (dist(b, p) < PLAYER.radius + 6) {
            p.hp--;
            dead = true;
            game.lastEvent = { type: "hurt", x: b.x, y: b.y, hp: p.hp };
            if (p.hp <= 0) {
              game.over = true;
              game.won = false;
              game.lastEvent = { type: "lose" };
            }
          }
        }
      }

      if (dead) list.splice(i, 1);
    }
  }

  /**
   * Advance the simulation by dt seconds.
   * input: { throttle: -1..1, steer: -1..1, aim: angle|null, fire: bool,
   *          reload: bool, turretDir: -1|0|1 }
   */
  function update(game, input, dt) {
    if (game.over) return;
    game.t += dt;

    const p = game.player;
    const inp = input || {};

    // Drive: throttle forward along the body angle, steer rotates the body
    const throttle = clamp(inp.throttle || 0, -1, 1);
    const steer = clamp(inp.steer || 0, -1, 1);
    p.angle += steer * PLAYER.turnSpeed * dt;
    p.x += Math.cos(p.angle) * PLAYER.speed * throttle * dt;
    p.y += Math.sin(p.angle) * PLAYER.speed * throttle * dt;

    // Obstacles block the body
    for (const r of game.map.obstacles) {
      if (circleHitsRect(p.x, p.y, PLAYER.radius - 4, r)) {
        p.x -= Math.cos(p.angle) * PLAYER.speed * throttle * dt;
        p.y -= Math.sin(p.angle) * PLAYER.speed * throttle * dt;
      }
    }
    p.x = clamp(p.x, PLAYER.radius, ARENA_W - PLAYER.radius);
    p.y = clamp(p.y, PLAYER.radius, ARENA_H - PLAYER.radius);

    // Turret: absolute aim (mouse) or relative rotation
    if (typeof inp.aim === "number" && !isNaN(inp.aim)) {
      p.turret = inp.aim;
    } else if (inp.turretDir) {
      p.turret += inp.turretDir * 2.2 * dt;
    }

    // Timers
    if (p.fireCd > 0) p.fireCd -= dt;
    if (p.reloading) {
      p.reloadT -= dt;
      if (p.reloadT <= 0) {
        p.reloading = false;
        p.magazine = PLAYER.maxMagazine;
        game.lastEvent = { type: "reloaded" };
      }
    }

    if (inp.fire) playerFire(game);
    if (inp.reload) startReload(game);

    for (const e of game.enemies) {
      if (e.hp > 0) enemyUpdate(game, e, dt);
    }

    updateBullets(game, game.bullets, dt);
    updateBullets(game, game.enemyBullets, dt);
  }

  /** Damage a specific enemy (used by the demo/tests). */
  function damageEnemy(game, index) {
    const e = game.enemies[index];
    if (!e || e.hp <= 0) return;
    e.hp--;
    game.player.hits++;
    if (game.player.hits >= 10) {
      game.over = true;
      game.won = true;
      game.lastEvent = { type: "win" };
    }
  }

  return {
    ARENA_W: ARENA_W,
    ARENA_H: ARENA_H,
    MAPS: MAPS,
    OBSTACLES: MAPS.medium.obstacles, // alias for compatibility
    PLAYER: PLAYER,
    ENEMY: ENEMY,
    createGame: createGame,
    reset: reset,
    update: update,
    playerFire: playerFire,
    startReload: startReload,
    damageEnemy: damageEnemy,
    dist: dist,
  };
});
