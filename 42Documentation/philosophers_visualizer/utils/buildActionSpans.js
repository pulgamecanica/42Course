/**
 * Builds action spans (start, end, type) for each philosopher
 * from a series of simulation frames.
 *
 * @param {Map<number, Frame>} frames - Map of timestamp -> frame
 * @param {number} philosopherCount
 * @returns {Map<number, Span[]>} - philosopherId -> Span[]
 */
export function buildActionSpans(frames, philosopherCount) {
  const result = new Map();
  for (let i = 1; i < philosopherCount; i++) {
    result.set(i, []);
  }

  const sortedTimestamps = [...frames.keys()].sort((a, b) => a - b);
  if (sortedTimestamps.length === 0) return result;

  for (let i = 1; i < philosopherCount; i++) {
    let currentType = null;
    let currentStart = null;

    for (let t of sortedTimestamps) {
      const frame = frames.get(t);
      const action = frame.philosophers[i]?.action;

      if (action !== currentType) {
        if (currentType !== null && currentStart !== null) {
          result.get(i).push({
            start: currentStart,
            end: t,
            type: currentType,
          });
        }
        currentType = action;
        currentStart = t;
      }
    }

    // Handle final span
    if (currentType !== null && currentStart !== null) {
      result.get(i).push({
        start: currentStart,
        end: sortedTimestamps.at(-1) + 1,
        type: currentType,
      });
    }
  }

  return result;
}
