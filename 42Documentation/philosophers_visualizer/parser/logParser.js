export function parseLog(input) {
  const lines = input.trim().split("\n");

  const philosophers = new Map(); // id -> [{timestamp, action, ...}]
  const allEvents = [];

  const actionMap = {
    "has taken a fork": "taken_fork",
    "is eating": "eating",
    "is sleeping": "sleeping",
    "is thinking": "thinking",
    "died": "died"
  };

  for (let i = 0; i < lines.length; i++) {
    const line = lines[i].trim();

    // Skip CLI args line or empty lines
    if (
      !line ||
      line.startsWith("./philo") ||
      line.match(/^\d+\s+(\d+\s+){2,}/)
    ) continue;

    const match = line.match(/^(\d+)\s+(\d+)\s+(has taken a fork|is eating|is sleeping|is thinking|died)$/);
    if (!match) {
      // Last line may be incomplete (e.g., from Ctrl+C)
      if (i === lines.length - 1 && i > 2) {
        console.warn("Skipping malformed last line:", line);
        continue;
      }
      throw new Error(`Malformed line at ${i + 1}: "${line}"`);
    }

    const [_, timestampStr, philIdStr, actionText] = match;
    const timestamp = parseInt(timestampStr, 10);
    const philosopher = parseInt(philIdStr, 10);
    const action = actionMap[actionText];

    const event = { timestamp, philosopher, action };

    // Add forkId placeholder if relevant
    if (action === "taken_fork") {
      event.forkId = null; // will assign later during simulation if needed
    }

    // Store in global event list
    allEvents.push(event);

    // Group by philosopher
    if (!philosophers.has(philosopher)) {
      philosophers.set(philosopher, []);
    }
    philosophers.get(philosopher).push(event);
  }

  const philosopherCount = Math.max(...Array.from(philosophers.keys())) + 1;

  return {
    philosophers,
    allEvents: allEvents.sort((a, b) => a.timestamp - b.timestamp),
    philosopherCount,
  };
}

