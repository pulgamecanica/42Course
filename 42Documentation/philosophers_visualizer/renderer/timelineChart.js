import * as d3 from "https://cdn.jsdelivr.net/npm/d3@7/+esm";
import { getActionColor } from '../utils/colors.js';

/**
 * Renders a draggable, zoomable timeline chart (Gantt-style)
 * @param {Map<number, Span[]>} spans - Map of philosopherId -> array of spans
 * @param {Array} forkEvents - not used yet, reserved for future
 * @param {Object} options - { currentTime: number, zoom: number }
 */
export function renderTimelineChart(spans, forkEvents, options = {}) {
  const svg = d3.select("#timelineCanvas");
  svg.selectAll("*").remove();

  const currentTime = options.currentTime || 0;
  const zoom = options.zoom || 1; // ms → px

  const philosopherIds = [...spans.keys()].sort((a, b) => a - b);
  const allSpans = [...spans.values()].flat();
  const maxTime = Math.max(...allSpans.map(s => s.end));

  const rowHeight = 28;
  const labelWidth = 30;
  const msToPx = zoom;
  const tickStep = 50;

  const totalWidth = maxTime * msToPx + labelWidth + 100;
  const totalHeight = rowHeight * philosopherIds.length + 20;

  const svgElem = svg
    .attr("width", totalWidth)
    .attr("height", totalHeight)
    .style("overflow", "visible");

  const g = svg.append("g").attr("class", "timeline-group");

  // Timestamp ticks
  const tickY = 20;
  for (let t = 0; t <= maxTime; t += tickStep) {
    const x = labelWidth + t * msToPx;
    g.append("line")
      .attr("x1", x)
      .attr("y1", tickY)
      .attr("x2", x)
      .attr("y2", totalHeight)
      .attr("stroke", "#444")
      .attr("stroke-width", 1)
      .attr("opacity", 0.3);

    g.append("text")
      .attr("x", x + 2)
      .attr("y", tickY - 4)
      .attr("font-size", "10px")
      .attr("fill", "#aaa")
      .text(t);
  }

  // Rows and spans
  philosopherIds.forEach((pid, i) => {
    const y = tickY + i * rowHeight + 10;
    const rowGroup = g.append("g");

    // Background row
    rowGroup.append("rect")
      .attr("x", 0)
      .attr("y", y - rowHeight / 2)
      .attr("width", totalWidth)
      .attr("height", rowHeight)
      .attr("fill", i % 2 === 0 ? "#222" : "#1a1a1a");

    // Philosopher label
    rowGroup.append("text")
      .attr("x", 4)
      .attr("y", y + 4)
      .attr("font-size", "12px")
      .attr("fill", "#ccc")
      .text(`P${pid}`);

    // Action spans
    const spansList = spans.get(pid);
    spansList.forEach(span => {
      const x = labelWidth + span.start * msToPx;
      const width = (span.end - span.start) * msToPx;

      const rect = rowGroup.append("rect")
        .attr("x", x)
        .attr("y", y - 8)
        .attr("width", width)
        .attr("height", 16)
        .attr("fill", getActionColor(span.type))
        .attr("rx", 3)
        .attr("ry", 3);

      rect.append("title")
        .text(`${span.type} [${span.start}-${span.end}]`);

      if (width > 40) {
        rowGroup.append("text")
          .attr("x", x + width / 2)
          .attr("y", y + 4)
          .attr("text-anchor", "middle")
          .attr("fill", "#111")
          .attr("font-size", "10px")
          .attr("pointer-events", "none")
          .text(span.type);
      }
    });
  });

  // Current time indicator
  const currentX = labelWidth + currentTime * msToPx;
  g.append("line")
    .attr("x1", currentX)
    .attr("y1", tickY)
    .attr("x2", currentX)
    .attr("y2", totalHeight)
    .attr("stroke", "lime")
    .attr("stroke-width", 2);

  // Auto-scroll to current time
  const parent = svg.node().parentElement;
  if (parent) {
    const scrollTarget = currentX - parent.clientWidth / 2;
    parent.scrollTo({ left: scrollTarget, behavior: 'smooth' });
  }

  // Drag to scroll manually
  const container = svg.node().parentElement;
  if (container) {
    let isDragging = false;
    let startX = 0;
    let scrollLeft = 0;

    container.addEventListener("mousedown", (e) => {
      isDragging = true;
      startX = e.pageX;
      scrollLeft = container.scrollLeft;
      container.style.cursor = "grabbing";
    });

    container.addEventListener("mousemove", (e) => {
      if (!isDragging) return;
      const dx = e.pageX - startX;
      container.scrollLeft = scrollLeft - dx;
    });

    container.addEventListener("mouseup", () => {
      isDragging = false;
      container.style.cursor = "grab";
    });

    container.addEventListener("mouseleave", () => {
      isDragging = false;
      container.style.cursor = "grab";
    });
  }

  const chapterTimestamps = new Set();
  for (const list of spans.values()) {
    for (const span of list) {
      chapterTimestamps.add(span.start);
    }
  }

  [...chapterTimestamps].forEach(ts => {
    const x = labelWidth + ts * msToPx;

    g.append('line')
      .attr('x1', x)
      .attr('y1', tickY)
      .attr('x2', x)
      .attr('y2', totalHeight)
      .attr('stroke', 'orange')
      .attr('stroke-dasharray', '2,2')
      .attr('stroke-width', 1)
      .attr('opacity', 0.3)
      .append('title')
      .text(`Action at ${ts} ms`);
  });

}
