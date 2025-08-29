# 🧠🍴 Dining Philosophers Visualizer

[![Visualizer](https://img.shields.io/badge/Visualization-Cyber--Cool-8a2be2?style=flat-square)]()
[![Built with D3.js](https://img.shields.io/badge/Built%20with-D3.js-orange?style=flat-square)]()
[![Play It](https://img.shields.io/badge/Playback-ms%20by%20ms-green?style=flat-square)]()
[![No License](https://img.shields.io/badge/license-none-red?style=flat-square)]()

---

#### Try it: https://42-course.github.io/philosophers_visualizer/

---

You can execute your philo like this!
```bash
PHILO_ARGS="4 460 200 200 2" ruby -e 'args = ENV["PHILO_ARGS"].to_s.split; exec("./philo", *args)'
```

And then we can use this oneliner to open directly on the browser using my visualizer:

```bash
export PHILO_ARGS="8 1260 200 200 10"; \
\
xdg-open "https://42-course.github.io/philosophers_visualizer/?log_gz_b64=$(ruby -e 'args = ENV["PHILO_ARGS"].to_s.split; exec("./philo", *args)' | gzip -c | base64 -w0 | sed -e 's/+/%2B/g; s/\//%2F/g; s/=/%3D/g')" | bash 
```

---

Welcome to a **cyberpunk-infused, pastel-powered, nerd-approved** visualization of the legendary **Dining Philosophers Problem**. This project takes logs from a philosophers simulation and brings them to life — with motion, style, and lots of 🍴.

### 🔍 Features

- 🎞️ **Millisecond-by-millisecond** animation
- 🍝 **Interactive timeline** with zoom, pan & scrub
- 🌀 **Circular simulation view** (drag, zoom, reset)
- 🖍️ **Pastel color-coded actions** (to make it aesthetic)
- ✍️ Supports **log file upload** or copy-paste
- 🧠 Visualizes **fork states**, **philosopher actions**, and **deaths**
- 💾 **Auto-saves** last session with localStorage

## 🚀 How to Use

1. Clone or open the `index.html` in your browser (no server needed).
2. Paste or upload your philosopher log.
3. Watch them dine, think, sleep, and... sometimes die.
4. Use the scrubber, speed control, or jump buttons to navigate.
5. Zoom into a philosopher, drag around the table, or hit **🔄 eeset** if you get lost in thought.

> Warning: staring too long may lead to philosophical existentialism.

## 🖼️ Interface Overview

```

+------------------------+
\| 🧠 Dining Philosophers |
\| 🧪 Config Inputs       |
\| 📄 Log Input           |
+------------------------+
\| 🔁 Playback Controls   |
\| 🧭 Timeline Chart       |
\| 🍽️ Visual Simulation   |
+------------------------+

```

## 🎨 Philosophy of Style

- All philosophers are equal.  
- All forks are sacred.  
- All animations are smooth.  
- All colors are pastel.  
- All bugs are Schrödinger’s bugs (they exist until observed).  

## 💡 Contribution

Contributions welcome — especially:

- 🪲 Bug fixes
- ✨ UI enhancements
- 🧩 New view modes

Just fork, philosophize, and PR away!

## 🙅 License

This project has **no license** — use it, tweak it, remix it.

---

Made with ☕, 🍜, and a bit of ✨ while thinking deeply about concurrency by pulgamecanica
