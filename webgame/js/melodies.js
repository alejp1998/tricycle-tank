/**
 * melodies.js — buzzer melodies ported straight from piTankGo_1.c
 * (frecuencia and tiempo tables) + a small WebAudio player.
 *
 * The real tank played these on a piezo buzzer via wiringPi softTone;
 * here they play through an oscillator with the same note table.
 */
(function (root, factory) {
  if (typeof module === "object" && module.exports) module.exports = factory();
  else root.TricycleMelodies = factory();
})(typeof self !== "undefined" ? self : this, function () {
  "use strict";

  // --- Tetris (55 notes) -------------------------------------------------
  const tetrisFreq = [
    1319, 988, 1047, 1175, 1047, 988, 880, 880, 1047, 1319, 1175, 1047, 988,
    988, 1047, 1175, 1319, 1047, 880, 880, 0, 1175, 1397, 1760, 1568, 1397,
    1319, 1047, 1319, 1175, 1047, 988, 988, 1047, 1175, 1319, 1047, 880, 880, 0,
    659, 523, 587, 494, 523, 440, 415, 659, 523, 587, 494, 523, 659, 880, 831,
  ];
  const tetrisTime = [
    450, 225, 225, 450, 225, 225, 450, 225, 225, 450, 225, 225, 450, 225, 225,
    450, 450, 450, 450, 450, 675, 450, 225, 450, 225, 225, 675, 225, 450, 225,
    225, 450, 225, 225, 450, 450, 450, 450, 450, 450, 900, 900, 900, 900, 900,
    900, 1800, 900, 900, 900, 900, 450, 450, 900, 1800,
  ];

  // --- Star Wars main theme (59 notes) ------------------------------------
  const starwarsFreq = [
    523, 0, 523, 0, 523, 0, 698, 0, 1046, 0, 0, 880, 0, 784, 0, 1397, 0, 523, 0,
    1760, 0, 0, 880, 0, 784, 0, 1397, 0, 523, 0, 1760, 0, 0, 880, 0, 784, 0,
    1397, 0, 523, 0, 1760, 0, 0, 880, 0, 1760, 0, 0, 784, 0, 523, 0, 0, 523, 0,
    0, 523, 0,
  ];
  const starwarsTime = [
    134, 134, 134, 134, 134, 134, 536, 134, 536, 134, 134, 134, 134, 134, 134,
    536, 134, 402, 134, 134, 429, 357, 134, 134, 134, 134, 536, 134, 402, 134,
    134, 429, 357, 134, 134, 134, 134, 536, 134, 402, 134, 134, 429, 357, 134,
    134, 134, 429, 357, 1071, 268, 67, 67, 268, 67, 67, 67, 67, 67,
  ];

  const MELODIES = [
    { name: "TETRIS", freq: tetrisFreq, time: tetrisTime },
    { name: "STAR WARS", freq: starwarsFreq, time: starwarsTime },
  ];

  /** Schedule a melody on the given AudioContext (returns total duration s). */
  function playMelody(ctx, melodyIndex, gainScale) {
    if (!ctx) return 0;
    const m = MELODIES[melodyIndex] || MELODIES[0];
    const gainScaleV = gainScale || 0.12;
    let t = ctx.currentTime + 0.05;
    let total = 0;
    for (let i = 0; i < m.freq.length; i++) {
      const dur = m.time[i] / 1000;
      if (m.freq[i] > 0) {
        const osc = ctx.createOscillator();
        const gain = ctx.createGain();
        osc.type = "square"; // buzzer-like
        osc.frequency.setValueAtTime(m.freq[i], t);
        gain.gain.setValueAtTime(gainScaleV, t);
        gain.gain.exponentialRampToValueAtTime(0.001, t + dur);
        osc.connect(gain);
        gain.connect(ctx.destination);
        osc.start(t);
        osc.stop(t + dur + 0.02);
      }
      t += dur;
      total += dur;
    }
    return total;
  }

  return { MELODIES: MELODIES, playMelody: playMelody };
});
