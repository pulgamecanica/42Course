/**
 * @typedef {Object} PhilosopherState
 * @property {string} action
 * @property {number} forks
 * @property {boolean} alive
 */

/**
 * @typedef {Object} Frame
 * @property {number} time
 * @property {Object.<number, PhilosopherState>} philosophers
 */

/**
 * @typedef {Object} Simulation
 * @property {number} philosopherCount
 * @property {Map<number, Frame>} frames
 * @property {Array<Object>} forkEvents
 */


import { parseLog } from '../parser/logParser.js';

/**
 * Build the simulation from log + config
 * @param {string} logText
 * @param {Object} config
 * @returns {Simulation}
 */
export function runSimulation(logText, config) {
  const { philosophers, allEvents, philosopherCount } = parseLog(logText);
  const frames = new Map();

  const state = {}; // philosopherId -> current state
  const forkCounts = {}; // philosopherId -> forks held

  for (let i = 1; i < philosopherCount; i++) {
    state[i] = { action: 'thinking', forks: 0, alive: true };
    forkCounts[i] = 0;
  }

  for (const event of allEvents) {
    const time = event.timestamp;
    const pid = event.philosopher;
    const action = event.action;

    if (!frames.has(time)) {
      // Deep copy previous state
      const prevTime = Math.max(...Array.from(frames.keys()).filter(t => t < time), -1);
      const newFrame = {
        time,
        philosophers: {}
      };

      for (let i = 1; i < philosopherCount; i++) {
        newFrame.philosophers[i] = {
          action: prevTime >= 0 ? frames.get(prevTime).philosophers[i].action : 'thinking',
          forks: prevTime >= 0 ? frames.get(prevTime).philosophers[i].forks : 0,
          alive: prevTime >= 0 ? frames.get(prevTime).philosophers[i].alive : true
        };
      }

      frames.set(time, newFrame);
    }

    const frame = frames.get(time);
    const philo = frame.philosophers[pid];

    // Update based on action
    if (action === 'taken_fork') {
      philo.forks++;
    } else if (action === 'sleeping') {
      philo.forks = 0;
      philo.action = action;
    } else if (action === 'eating' || action === 'thinking') {
      philo.action = action;
    } else if (action === 'died') {
      philo.action = 'died';
      philo.alive = false;
    }
  }

  // Sort frames by time (not necessary for Map but useful)
  return {
    philosopherCount,
    frames: new Map([...frames.entries()].sort((a, b) => a[0] - b[0])),
    forkEvents: [] // you can later collect them if needed
  };
}

