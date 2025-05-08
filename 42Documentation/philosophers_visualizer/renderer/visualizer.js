import * as d3 from "https://cdn.jsdelivr.net/npm/d3@7/+esm";
import { getActionColor } from "../utils/colors.js";

export function renderVisualizer(philosopherCount, frame, bonusMode) {
  const svg = d3.select('#philosopherCanvas');

  // Preserve zoom state
  const previousTransform = d3.zoomTransform(svg.node());

  svg.selectAll('*').remove();

  const mainGroup = svg.append('g')
    .attr('id', 'zoomGroup')
    .attr('transform', previousTransform); // Apply previous transform

  // Ensure tooltip exists
  let tooltip = document.getElementById('philo-tooltip');
  if (!tooltip) {
    tooltip = document.createElement('div');
    tooltip.id = 'philo-tooltip';
    tooltip.style.position = 'absolute';
    tooltip.style.pointerEvents = 'none';
    tooltip.style.padding = '6px 10px';
    tooltip.style.background = 'rgba(0,0,0,0.75)';
    tooltip.style.borderRadius = '6px';
    tooltip.style.color = 'white';
    tooltip.style.fontSize = '12px';
    tooltip.style.visibility = 'hidden';
    document.body.appendChild(tooltip);
  }

  const width = svg.node().clientWidth;
  const height = svg.node().clientHeight;
  const centerX = width / 2;
  const centerY = height / 2;

  const minSpacing = 80;
  const maxRadius = Math.min(centerX, centerY) - 50;
  const angleStep = (2 * Math.PI) / philosopherCount;
  const radius = Math.max(Math.min(maxRadius, philosopherCount * minSpacing / (2 * Math.PI)), 100);
  const nodeRadius = Math.max(10, Math.min(30, (radius * Math.PI) / philosopherCount / 2));

  // Draw table
  mainGroup.append('circle')
    .attr('cx', centerX)
    .attr('cy', centerY)
    .attr('r', radius - 40)
    .attr('fill', '#111')
    .attr('stroke', '#555')
    .attr('stroke-width', 4);

  // Determine locked forks
  const lockedForks = new Set();
  for (let i = 0; i < philosopherCount; i++) {
    if (frame.philosophers[i].action === 'eating') {
      const left = (i - 1 + philosopherCount) % philosopherCount;
      const right = i;
      lockedForks.add(left);
      lockedForks.add(right);
    }
  }

  // Draw philosophers
  for (let i = 0; i < philosopherCount; i++) {
    const angle = i * angleStep;
    const x = centerX + radius * Math.cos(angle);
    const y = centerY + radius * Math.sin(angle);
    const p = frame.philosophers[i];

    const group = mainGroup.append('g')
      .attr('transform', `translate(${x}, ${y})`)
      .on('mouseover', () => {
        tooltip.style.visibility = 'visible';
        tooltip.innerHTML = `
          <strong>P${i}</strong><br/>
          Action: ${p.action}<br/>
          Forks: ${p.forks}<br/>
          Alive: ${p.alive ? '✅' : '💀'}
        `;
      })
      .on('mousemove', (event) => {
        tooltip.style.left = `${event.pageX + 10}px`;
        tooltip.style.top = `${event.pageY + 10}px`;
      })
      .on('mouseout', () => {
        tooltip.style.visibility = 'hidden';
      });

    group.append('circle')
      .attr('r', nodeRadius + 4)
      .attr('fill', 'none')
      .attr('stroke', getActionColor(p.action) || '#777')
      .attr('stroke-width', 3);

    group.append('circle')
      .attr('r', nodeRadius)
      .attr('fill', '#333')
      .attr('stroke', p.alive ? '#fff' : '#f00')
      .attr('stroke-width', 2);

    group.append('text')
      .attr('text-anchor', 'middle')
      .attr('dy', '0.35em')
      .attr('fill', '#fff')
      .attr('font-size', nodeRadius * 0.8)
      .text(`P${i}`);
  }

  // Forks logic
  if (bonusMode) {
    const eaters = Object.entries(frame.philosophers)
      .filter(([_, p]) => p.action === 'eating')
      .map(([id]) => parseInt(id));

    const totalForks = Object.values(frame.philosophers).reduce((sum, p) => sum + p.forks, 0);
    const takenForks = eaters.length * 2;
    const centerForks = totalForks - takenForks;
    const forkRadius = 30;

    for (let i = 0; i < centerForks; i++) {
      const angle = (2 * Math.PI / centerForks) * i;
      const fx = centerX + Math.cos(angle) * forkRadius;
      const fy = centerY + Math.sin(angle) * forkRadius;

      mainGroup.append('text')
        .attr('x', fx)
        .attr('y', fy + 4)
        .attr('text-anchor', 'middle')
        .attr('alignment-baseline', 'middle')
        .attr('fill', 'orange')
        .attr('font-size', '14px')
        .text('🍴');
    }

    for (const i of eaters) {
      const angle = i * angleStep;
      const offset = Math.PI / 60;
      const forkDist = radius - 40;
      const angles = [angle - offset, angle + offset];

      angles.forEach(a => {
        const fx = centerX + Math.cos(a) * forkDist;
        const fy = centerY + Math.sin(a) * forkDist;

        mainGroup.append('text')
          .attr('x', fx)
          .attr('y', fy + 4)
          .attr('text-anchor', 'middle')
          .attr('alignment-baseline', 'middle')
          .attr('fill', 'red')
          .attr('font-size', '16px')
          .text('🍴');
      });
    }
  } else {
    for (let i = 0; i < philosopherCount; i++) {
      const angle = (i + 0.5) * angleStep;
      const forkRadius = radius - 20;
      const baseX = centerX + forkRadius * Math.cos(angle);
      const baseY = centerY + forkRadius * Math.sin(angle);

      const leftPhilo = i;
      const rightPhilo = (i + 1) % philosopherCount;

      const leftHas = frame.philosophers[leftPhilo]?.action === 'eating';
      const rightHas = frame.philosophers[rightPhilo]?.action === 'eating';

      let forkX = baseX;
      let forkY = baseY;

      if (leftHas && !rightHas) {
        forkX = centerX + (forkRadius - 10) * Math.cos(angle - angleStep * 0.15);
        forkY = centerY + (forkRadius - 10) * Math.sin(angle - angleStep * 0.15);
      } else if (rightHas && !leftHas) {
        forkX = centerX + (forkRadius - 10) * Math.cos(angle + angleStep * 0.15);
        forkY = centerY + (forkRadius - 10) * Math.sin(angle + angleStep * 0.15);
      }

      const locked = leftHas || rightHas;

      mainGroup.append('text')
        .attr('x', forkX)
        .attr('y', forkY + 4)
        .attr('text-anchor', 'middle')
        .attr('alignment-baseline', 'middle')
        .attr('fill', locked ? '#f44336' : '#888')
        .attr('font-size', '14px')
        .text(locked ? '🔒' : '🍴');
    }
  }

  // Enable zoom and pan
  svg.call(
    d3.zoom()
      .scaleExtent([0.5, 4])
      .on("zoom", (event) => {
        mainGroup.attr("transform", event.transform);
      })
  );

  if (!svg.node().__zoomInitialized) {
    svg.call(
      d3.zoom()
        .scaleExtent([0.5, 4])
        .on("zoom", (event) => {
          svg.select('#zoomGroup').attr("transform", event.transform);
        })
    );
    svg.node().__zoomInitialized = true;
  }

}
