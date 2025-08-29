import { renderVisualizer } from "./renderer/visualizer.js";
import { renderTimelineChart } from './renderer/timelineChart.js';
import { buildActionSpans } from './utils/buildActionSpans.js';
import { buildSnapshots } from './utils/buildSnapshots.js';
import { runSimulation } from './utils/simulation.js';

const configPanel = document.getElementById('configPanel');
const toggleBtn = document.getElementById('togglePanel');
const backToVisBtn = document.getElementById('backToVis');
const visualizerPanel = document.getElementById('visualizerPanel');
const clearBtn = document.getElementById('clearStorage');
const chartContainer = document.getElementById('timeChart');
const seekBar = document.getElementById("seekBar");
const playerContainer = document.getElementById("playerContainer");
const seekProgress = document.getElementById("seekProgress");
const seekHoverLabel = document.getElementById("seekHoverLabel");
let isSeeking = false;

let spans = null;
let forkEvents = [];
let timestamps = [];
let zoomLevel = 1;
let isDown = false;
let startX = 0;
let bonusMode = false;
let playbackInterval = null;
let isPlaying = true;
let playbackSpeed = 1;
let currentTimeMs = 0;

let frames = new Map();
let philosopherCount = 0;

// File + Text input
const logFileInput = document.getElementById('logFile');
const logTextArea = document.getElementById('logText');

logFileInput.addEventListener('change', async (e) => {
  const file = e.target.files[0];
  const text = await file.text();
  logTextArea.value = text;
  processLog(text);
});

logTextArea.addEventListener('input', (e) => {
  processLog(e.target.value);
});

seekBar.addEventListener("mousemove", (e) => {
  const rect = seekBar.getBoundingClientRect();
  const percent = Math.max(0, Math.min(1, (e.clientX - rect.left) / rect.width));
  const maxTime = timestamps.at(-1) || 1;
  const hoverTime = Math.floor(percent * maxTime);

  seekHoverLabel.style.left = `${e.clientX}px`;
  seekHoverLabel.style.display = "block";
  seekHoverLabel.textContent = `${hoverTime} ms`;
});

seekBar.addEventListener("mouseleave", () => {
  seekHoverLabel.style.display = "none";
});

seekBar.addEventListener("mousedown", (e) => {
  const rect = seekBar.getBoundingClientRect();
  const percent = Math.max(0, Math.min(1, (e.clientX - rect.left) / rect.width));
  const maxTime = timestamps.at(-1) || 1;
  currentTimeMs = Math.floor(percent * maxTime);

  isPlaying = false;
  updatePlayPauseIcon();

  const frame = frames.get(currentTimeMs) || findClosestFrame(currentTimeMs);
  if (frame) {
    renderVisualizer(philosopherCount, frame, bonusMode);
    updateTimeline();
    updateScrubber();
  }
});

document.getElementById('zoomSlider').addEventListener('input', (e) => {
  zoomLevel = parseFloat(e.target.value);
  updateTimeline();
});

toggleBtn.addEventListener('click', () => collapseVisualizerPanel());

backToVisBtn.addEventListener('click', () => {
  const text = logTextArea.value;
  if (text) {
    collapseConfigPanel();
    processLog(text);
  } else {
    showToast('Invalid logs');
  }
});

clearBtn.addEventListener('click', () => {
  localStorage.removeItem('logText');
  localStorage.removeItem('config');
  logTextArea.value = '';
  document.getElementById('timeToDie').value = '';
  document.getElementById('timeToEat').value = '';
  document.getElementById('timeToSleep').value = '';
  document.getElementById('bonusToggle').checked = false;
  spans = null;
  forkEvents = [];
  timestamps = [];
  showToast('Local data cleared');
  collapseVisualizerPanel();
});

chartContainer.addEventListener('mousedown', e => {
  isDown = true;
  chartContainer.classList.add('active');
  startX = e.pageX - chartContainer.offsetLeft;
  scrollLeft = chartContainer.scrollLeft;
});
chartContainer.addEventListener('mouseleave', () => isDown = false);
chartContainer.addEventListener('mouseup', () => isDown = false);
chartContainer.addEventListener('mousemove', e => {
  if (!isDown) return;
  const x = e.pageX - chartContainer.offsetLeft;
  chartContainer.scrollLeft = scrollLeft - (x - startX);
});

function showToast(message, duration = 3000) {
  const toast = document.getElementById('toast');
  toast.textContent = message;
  toast.classList.add('visible');
  toast.classList.remove('hidden');
  setTimeout(() => toast.classList.remove('visible'), duration);
}

function collapseConfigPanel() {
  configPanel.classList.add('closed');
  toggleBtn.classList.remove('hidden');
  visualizerPanel.classList.add('visible');
  backToVisBtn.classList.add('hidden');
  toggleBtn.classList.remove('hidden');
  playerContainer.classList.remove('hidden');
}

function collapseVisualizerPanel() {
  configPanel.classList.remove('closed');
  toggleBtn.classList.add('hidden');
  playerContainer.classList.add('hidden');
  visualizerPanel.classList.remove('visible');
  if (frames && frames.size) backToVisBtn.classList.remove('hidden');
}

async function tryLoadLogFromURL() {
  const qs = new URLSearchParams(window.location.search);
  const hash = new URLSearchParams(window.location.hash.slice(1));

  // Helper decoders
  const fromBase64 = (b64) => {
    const bin = atob(b64);
    const bytes = new Uint8Array(bin.length);
    for (let i = 0; i < bin.length; i++) bytes[i] = bin.charCodeAt(i);
    return new TextDecoder().decode(bytes);
  };

  const maybe = (...keys) => {
    for (const k of keys) {
      if (qs.has(k)) return qs.get(k);
      if (hash.has(k)) return hash.get(k);
    }
    return null;
  };

  // 1) gzip+base64 → requires pako if present (optional)
  const gzB64 = maybe('log_gz_b64');
  if (gzB64) {
    try {
      if (!window.pako) throw new Error("pako not loaded");
      const bin = Uint8Array.from(atob(gzB64), c => c.charCodeAt(0));
      const ungz = window.pako.ungzip(bin);
      const text = new TextDecoder().decode(ungz);
      logTextArea.value = text;
      processLog(text);
      return true;
    } catch (e) {
      console.warn("Failed to decode log_gz_b64:", e);
      showToast("Failed to decode compressed log in URL");
    }
  }

  // 2) base64
  const b64 = maybe('log_b64');
  if (b64) {
    try {
      const text = fromBase64(b64);
      logTextArea.value = text;
      processLog(text);
      return true;
    } catch (e) {
      console.warn("Failed to decode log_b64:", e);
      showToast("Failed to decode base64 log in URL");
    }
  }

  // 3) plain URI-encoded text
  const raw = maybe('log');
  if (raw) {
    try {
      const text = decodeURIComponent(raw);
      logTextArea.value = text;
      processLog(text);
      return true;
    } catch {
      // If it wasn’t encoded, use as-is
      logTextArea.value = raw;
      processLog(raw);
      return true;
    }
  }

  return false;
}

// Ensure this runs before restoreFromLocalStorage replays old data
window.addEventListener('DOMContentLoaded', async () => {
  const loaded = await tryLoadLogFromURL();
  if (!loaded) restoreFromLocalStorage();
});


function getConfig() {
  const die = parseInt(document.getElementById('timeToDie').value);
  const eat = parseInt(document.getElementById('timeToEat').value);
  const sleep = parseInt(document.getElementById('timeToSleep').value);
  const bonus = document.getElementById('bonusToggle')?.checked ?? false;

  bonusMode = bonus;

  return {
    time_to_die: isNaN(die) ? null : die,
    time_to_eat: isNaN(eat) ? null : eat,
    time_to_sleep: isNaN(sleep) ? null : sleep,
    bonus_mode: bonus
  };
}

function saveToLocalStorage(logText, config) {
  localStorage.setItem('logText', logText);
  localStorage.setItem('config', JSON.stringify(config));
}

function restoreFromLocalStorage() {
  const logText = localStorage.getItem('logText');
  const config = JSON.parse(localStorage.getItem('config') || '{}');

  if (config.time_to_die !== undefined) {
    document.getElementById('timeToDie').value = config.time_to_die ?? '';
    document.getElementById('timeToEat').value = config.time_to_eat ?? '';
    document.getElementById('timeToSleep').value = config.time_to_sleep ?? '';
  }
  if (config.bonus_mode !== undefined) {
    document.getElementById('bonusToggle').checked = config.bonus_mode;
    bonusMode = config.bonus_mode;
  }

  if (logText) {
    logTextArea.value = logText;
    processLog(logText);
  }
}

function processLog(text) {
  try {
    if (text.trim().length === 0) return;
    const config = getConfig();
    const sim = runSimulation(text, config);
    currentTimeMs = 0;
    frames = sim.frames;
    philosopherCount = sim.philosopherCount;
    forkEvents = sim.forkEvents;
    timestamps = [...frames.keys()].sort((a, b) => a - b);
    spans = buildActionSpans(frames, philosopherCount);

    collapseConfigPanel();
    startPlayback();
    renderVisualizer(philosopherCount, findLatestFrame(currentTimeMs), bonusMode);
    updateTimeline();
    updateScrubber()
    saveToLocalStorage(text, config);
  } catch (err) {
    showToast("Log error: " + err.message);
  }
}

function updateTimeline() {
  renderTimelineChart(spans, forkEvents, {
    currentTime: currentTimeMs,
    zoom: zoomLevel
  });
}

function startPlayback() {
  if (playbackInterval) clearInterval(playbackInterval);
  isPlaying = true;
  updatePlayPauseIcon();

  const maxTime = timestamps.at(-1) || 0;

  playbackInterval = setInterval(() => {
    if (!isPlaying) return;

    if (currentTimeMs > maxTime) {
      clearInterval(playbackInterval);
      isPlaying = false;
      updatePlayPauseIcon();
      return;
    }

    renderVisualizer(philosopherCount, findLatestFrame(currentTimeMs), bonusMode);
    updateTimeline();
    updateScrubber();

    currentTimeMs += playbackSpeed;

  }, 1);
}

function updatePlayPauseIcon() {
  document.getElementById('btnPlayPause').textContent = isPlaying ? '⏸️' : '▶️';
}

function updateScrubber() {
  const maxTime = timestamps.at(-1) || 1;
  const percent = Math.min(1, currentTimeMs / maxTime);
  seekProgress.style.width = `${percent * 100}%`;
  document.getElementById("currentTimeLabel").textContent = `${parseInt(currentTimeMs)} ms`;
}

document.getElementById('btnPlayPause').addEventListener('click', () => {
  isPlaying = !isPlaying;
  updatePlayPauseIcon();
  if (isPlaying) startPlayback();
});

document.getElementById('btnToStart').addEventListener('click', () => {
  currentTimeMs = 0;
  isPlaying = false;
  updatePlayPauseIcon();
  const closest = timestamps.find(t => t >= currentTimeMs);
  if (closest !== undefined) {
    renderVisualizer(philosopherCount, frames.get(closest), bonusMode);
  }
  updateTimeline();
  updateScrubber();
});

document.getElementById('btnToEnd').addEventListener('click', () => {
  currentTimeMs = timestamps.at(-1) || 0;
  isPlaying = false;
  updatePlayPauseIcon();
  renderVisualizer(philosopherCount, frames.get(currentTimeMs), bonusMode);
  updateTimeline();
  updateScrubber();
});

document.getElementById('btnPrev').addEventListener('click', () => {
  const prev = [...timestamps].reverse().find(t => t < currentTimeMs);
  if (prev !== undefined) {
    currentTimeMs = prev;
    isPlaying = false;
    updatePlayPauseIcon();
    renderVisualizer(philosopherCount, frames.get(currentTimeMs), bonusMode);
    updateTimeline();
    updateScrubber();
  }
});

document.getElementById('btnNext').addEventListener('click', () => {
  const next = timestamps.find(t => t > currentTimeMs);
  if (next !== undefined) {
    currentTimeMs = next;
    isPlaying = false;
    updatePlayPauseIcon();
    renderVisualizer(philosopherCount, frames.get(currentTimeMs), bonusMode);
    updateTimeline();
    updateScrubber();
  }
});

document.getElementById('speedSelect').addEventListener('change', (e) => {
  playbackSpeed = parseFloat(e.target.value);
  startPlayback();
});

function findClosestFrame(time) {
  return frames.get(timestamps.find(t => t >= time));
}

function findLatestFrame(time) {
  const latest = [...timestamps].reverse().find(t => t <= time);
  return frames.get(latest);
}


window.addEventListener('DOMContentLoaded', restoreFromLocalStorage);
