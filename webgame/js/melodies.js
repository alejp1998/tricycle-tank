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

  // --- Despacito (160 notes, from piTankGo_1.c) --------------------------
  const despacitoFreq = [
    0, 1175, 1109, 988, 740, 740, 740, 740, 740, 740, 988, 988, 988, 988, 880,
    988, 784, 0, 784, 784, 784, 784, 784, 988, 988, 988, 988, 1109, 1175, 880,
    0, 880, 880, 880, 880, 880, 1175, 1175, 1175, 1175, 1318, 1318, 1109, 0,
    1175, 1109, 988, 740, 740, 740, 740, 740, 740, 988, 988, 988, 988, 880, 988,
    784, 0, 784, 784, 784, 784, 784, 988, 988, 988, 988, 1109, 1175, 880, 0,
    880, 880, 880, 880, 880, 1175, 1175, 1175, 1175, 1318, 1318, 1109, 0, 1480,
    1318, 1480, 1318, 1480, 1318, 1480, 1318, 1480, 1318, 1480, 1568, 1568,
    1175, 0, 1175, 1568, 1568, 1568, 0, 1568, 1760, 1568, 1480, 0, 1480, 1480,
    1480, 1760, 1568, 1480, 1318, 659, 659, 659, 659, 659, 659, 659, 659, 554,
    587, 1480, 1318, 1480, 1318, 1480, 1318, 1480, 1318, 1480, 1318, 1480, 1568,
    1568, 1175, 0, 1175, 1568, 1568, 1568, 1568, 1760, 1568, 1480, 0, 1480,
    1480, 1480, 1760, 1568, 1480, 1318,
  ];
  const despacitoTime = [
    1200, 600, 600, 300, 300, 150, 150, 150, 150, 150, 150, 150, 150, 300, 150,
    300, 343, 112, 150, 150, 150, 150, 150, 150, 150, 150, 300, 150, 300, 300,
    150, 150, 150, 150, 150, 150, 150, 150, 150, 300, 150, 300, 800, 300, 600,
    600, 300, 300, 150, 150, 150, 150, 150, 150, 150, 150, 300, 150, 300, 343,
    112, 150, 150, 150, 150, 150, 150, 150, 150, 300, 150, 300, 300, 150, 150,
    150, 150, 150, 150, 150, 150, 150, 300, 150, 300, 450, 1800, 150, 150, 150,
    150, 300, 150, 300, 150, 150, 150, 300, 150, 300, 450, 450, 300, 150, 150,
    225, 75, 150, 150, 300, 450, 800, 150, 150, 300, 150, 150, 300, 450, 150,
    150, 150, 150, 150, 150, 150, 150, 300, 300, 150, 150, 150, 150, 150, 150,
    450, 150, 150, 150, 300, 150, 300, 450, 450, 300, 150, 150, 150, 300, 150,
    300, 450, 800, 150, 150, 300, 150, 150, 300, 450,
  ];

  const MELODIES = [
    { name: "TETRIS", freq: tetrisFreq, time: tetrisTime },
    { name: "STAR WARS", freq: starwarsFreq, time: starwarsTime },
    { name: "DESPACITO", freq: despacitoFreq, time: despacitoTime },
  ];

  // Nodes of the melody currently scheduled (so a new melody can stop it)
  let activeNodes = [];

  /** Stop the melody currently playing/scheduled (if any). */
  function stopMelody() {
    activeNodes.forEach(function (n) {
      try {
        n.osc.stop();
      } catch (e) {
        /* already stopped */
      }
    });
    activeNodes = [];
  }

  /** Schedule a melody on the given AudioContext (returns total duration s).
   *  Any melody already playing is stopped first. */
  function playMelody(ctx, melodyIndex, gainScale) {
    if (!ctx) return 0;
    stopMelody();
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
        activeNodes.push({ osc: osc, gain: gain });
      }
      t += dur;
      total += dur;
    }
    // Drop finished nodes after the last scheduled stop has fired
    const endTime = total + 0.5;
    window.setTimeout(function () {
      activeNodes = [];
    }, endTime * 1000);
    return total;
  }

  return { MELODIES: MELODIES, playMelody: playMelody, stopMelody: stopMelody };
});
