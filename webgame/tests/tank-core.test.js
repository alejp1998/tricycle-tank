/**
 * Node test suite for the Tricycle Tank web game core.
 * Runs with: node --test webgame/tests/tank-core.test.js
 */
const { test } = require("node:test");
const assert = require("node:assert");

const {
  ARENA_W,
  ARENA_H,
  OBSTACLES,
  PLAYER,
  createGame,
  reset,
  update,
  damageEnemy,
  dist,
} = require("../js/tank-core.js");

const idle = { throttle: 0, steer: 0, fire: false, reload: false };

test("game initializes with full magazine, HP, and 3 enemies", () => {
  const g = createGame(7);
  assert.strictEqual(g.player.magazine, PLAYER.maxMagazine);
  assert.strictEqual(g.player.hp, PLAYER.maxHp);
  assert.strictEqual(g.player.hits, 0);
  assert.strictEqual(g.enemies.length, 3);
  assert.strictEqual(g.over, false);
});

test("throttle moves the tank forward along its facing angle", () => {
  const g = createGame(1);
  const start = { x: g.player.x, y: g.player.y };
  update(g, { ...idle, throttle: 1 }, 0.5);
  const dx = g.player.x - start.x;
  const dy = g.player.y - start.y;
  assert.ok(Math.abs(dx) < 1, "north-facing tank barely moves on X");
  assert.ok(dy < -40, "tank moved forward (up the arena)");
});

test("steer rotates the tank body", () => {
  const g = createGame(1);
  const a0 = g.player.angle;
  update(g, { ...idle, steer: 1 }, 0.5);
  assert.ok(g.player.angle > a0, "turning right increases the angle");
});

test("firing consumes a bullet and spawns it along the turret angle", () => {
  const g = createGame(1);
  update(g, { ...idle, fire: true }, 0.01);
  assert.strictEqual(g.player.magazine, PLAYER.maxMagazine - 1);
  assert.strictEqual(g.bullets.length, 1);
  assert.strictEqual(g.lastEvent.type, "fire");
});

test("empty magazine blocks firing and auto-starts a reload", () => {
  const g = createGame(1);
  g.player.magazine = 0;
  update(g, { ...idle, fire: true }, 0.01);
  assert.strictEqual(g.bullets.length, 0, "no bullet with empty magazine");
  assert.strictEqual(g.player.reloading, true, "auto-reload starts");
  assert.strictEqual(g.lastEvent.type, "empty");
});

test("reload refills the magazine after reloadMs", () => {
  const g = createGame(1);
  g.player.magazine = 2;
  update(g, { ...idle, reload: true }, 0.01);
  assert.strictEqual(g.player.reloading, true);
  update(g, idle, PLAYER.reloadMs / 1000 + 0.01);
  assert.strictEqual(g.player.reloading, false);
  assert.strictEqual(g.player.magazine, PLAYER.maxMagazine);
});

test("player bullet damages an enemy and counts a hit", () => {
  const g = createGame(1);
  const e = g.enemies[0];
  e.hp = 3;
  // Park the player right below the enemy and fire straight up
  // (enemy stays still: it is already inside its 130px engagement range)
  g.player.x = e.x;
  g.player.y = e.y + 70;
  g.player.turret = -Math.PI / 2;
  update(g, { ...idle, fire: true }, 0.01);
  update(g, idle, 0.4); // 70px at 420px/s -> ~0.17s
  assert.strictEqual(e.hp, 2);
  assert.strictEqual(g.player.hits, 1);
  assert.strictEqual(g.lastEvent.type, "hit");
});

test("enemy bullets damage the player", () => {
  const g = createGame(1);
  const hp0 = g.player.hp;
  // Spawn an enemy bullet aimed directly at the player
  g.enemyBullets.push({
    x: g.player.x,
    y: g.player.y - 100,
    vx: 0,
    vy: 420,
    angle: Math.PI / 2,
    owner: "enemy",
    life: 2,
  });
  update(g, idle, 100 / 420 + 0.05);
  assert.strictEqual(g.player.hp, hp0 - 1);
  assert.strictEqual(g.lastEvent.type, "hurt");
});

test("obstacles absorb bullets", () => {
  const g = createGame(1);
  const o = OBSTACLES[0];
  g.bullets.push({
    x: o.x + o.w / 2,
    y: o.y - 50,
    vx: 0,
    vy: 420,
    angle: Math.PI / 2,
    owner: "player",
    life: 2,
  });
  update(g, idle, 0.3);
  assert.strictEqual(g.bullets.length, 0, "bullet died on the obstacle");
  assert.strictEqual(g.lastEvent.type, "spark");
});

test("ten hits win the match", () => {
  const g = createGame(1);
  for (let i = 0; i < 10; i++) {
    damageEnemy(g, 0);
    g.enemies[0].hp = 1; // keep the target hittable
  }
  assert.strictEqual(g.over, true);
  assert.strictEqual(g.won, true);
  assert.strictEqual(g.lastEvent.type, "win");
});

test("losing all HP loses the match", () => {
  const g = createGame(1);
  g.player.hp = 1;
  g.enemyBullets.push({
    x: g.player.x,
    y: g.player.y - 100,
    vx: 0,
    vy: 420,
    angle: Math.PI / 2,
    owner: "enemy",
    life: 2,
  });
  update(g, idle, 0.3);
  assert.strictEqual(g.over, true);
  assert.strictEqual(g.won, false);
  assert.strictEqual(g.lastEvent.type, "lose");
});

test("the game is frozen once over", () => {
  const g = createGame(1);
  damageEnemy(g, 0);
  g.over = true;
  const x0 = g.player.x;
  update(g, { ...idle, throttle: 1 }, 0.5);
  assert.strictEqual(g.player.x, x0, "no movement after game over");
});

test("reset restores a fresh match", () => {
  const g = createGame(1);
  g.player.hp = 1;
  g.player.hits = 9;
  reset(g, 1);
  assert.strictEqual(g.player.hp, PLAYER.maxHp);
  assert.strictEqual(g.player.hits, 0);
  assert.strictEqual(g.enemies.length, 3);
  assert.strictEqual(g.over, false);
});

test("tank stays inside the arena bounds", () => {
  const g = createGame(1);
  g.player.x = 10;
  g.player.y = 10;
  g.player.angle = Math.PI / 4;
  for (let i = 0; i < 60; i++) update(g, { ...idle, throttle: 1 }, 0.1);
  assert.ok(g.player.x >= PLAYER.radius && g.player.x <= ARENA_W - PLAYER.radius);
  assert.ok(g.player.y >= PLAYER.radius && g.player.y <= ARENA_H - PLAYER.radius);
});

test("enemies converge on the player", () => {
  const g = createGame(1);
  const startDist = dist(g.enemies[0], g.player);
  for (let i = 0; i < 60; i++) update(g, idle, 0.1);
  const endDist = dist(g.enemies[0], g.player);
  assert.ok(endDist < startDist, "enemy got closer: " + endDist + " < " + startDist);
});

test("same seed reproduces the same match", () => {
  const a = createGame(42);
  const b = createGame(42);
  assert.strictEqual(a.rng(), b.rng());
});
