export function buildSnapshots(events, count) {
  const snapshots = new Map();
  const philosophers = Array.from({ length: count }, () => ({
    action: 'thinking',
    leftFork: null,
    rightFork: null,
  }));
  const forks = Array(count).fill(null);

  for (const event of events) {
    const { timestamp, philosopher, action } = event;
    const state = JSON.parse(JSON.stringify(philosophers));

    switch (action) {
      case 'taken_fork': {
        const leftId = philosopher;
        const rightId = (philosopher + 1) % count;
        const target = forks[leftId] === null ? leftId : rightId;
        forks[target] = philosopher;

        if (state[philosopher].leftFork === null) {
          state[philosopher].leftFork = target;
        } else {
          state[philosopher].rightFork = target;
        }

        // 🧠 Do NOT update action — just fork data
        break;
      }

      case 'eating':
      case 'sleeping':
      case 'thinking':
      case 'died':
        state[philosopher].action = action;
        break;
    }

    snapshots.set(timestamp, deepClone(state));
  }

  return snapshots;
}

function deepClone(obj) {
  return JSON.parse(JSON.stringify(obj));
}
