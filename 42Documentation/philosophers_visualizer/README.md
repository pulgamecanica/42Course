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

You can take the example bellow to test it:
<details>
<summary>Example output</summary>

```
0000 01 has taken a fork
0000 01 has taken a fork
0000 01 is eating
0000 03 has taken a fork
0000 03 has taken a fork
0000 03 is eating
0000 05 has taken a fork
0000 05 has taken a fork
0000 05 is eating
0000 07 has taken a fork
0000 07 has taken a fork
0000 07 is eating
0200 01 is sleeping
0200 05 is sleeping
0200 02 has taken a fork
0200 06 has taken a fork
0200 09 has taken a fork
0200 09 has taken a fork
0200 09 is eating
0200 03 is sleeping
0200 04 has taken a fork
0200 04 has taken a fork
0200 04 is eating
0200 07 is sleeping
0200 08 has taken a fork
0200 06 has taken a fork
0200 06 is eating
0200 02 has taken a fork
0200 02 is eating
0400 05 is thinking
0400 01 is thinking
0400 09 is sleeping
0400 08 has taken a fork
0400 08 is eating
0400 04 is sleeping
0400 07 is thinking
0400 03 is thinking
0400 03 has taken a fork
0400 06 is sleeping
0400 05 has taken a fork
0400 05 has taken a fork
0400 05 is eating
0400 02 is sleeping
0400 03 has taken a fork
0400 03 is eating
0400 01 has taken a fork
0400 01 has taken a fork
0400 01 is eating
0600 09 is thinking
0600 08 is sleeping
0600 07 has taken a fork
0600 07 has taken a fork
0600 07 is eating
0600 06 is thinking
0600 05 is sleeping
0600 02 is thinking
0600 06 has taken a fork
0600 04 is thinking
0600 01 is sleeping
0600 09 has taken a fork
0600 09 has taken a fork
0600 09 is eating
0600 02 has taken a fork
0600 03 is sleeping
0601 04 has taken a fork
0601 04 has taken a fork
0601 04 is eating
0601 02 has taken a fork
0601 02 is eating
0800 08 is thinking
0800 07 is sleeping
0800 08 has taken a fork
0800 06 has taken a fork
0800 06 is eating
0800 05 is thinking
0800 01 is thinking
0801 09 is sleeping
0801 08 has taken a fork
0801 08 is eating
0801 04 is sleeping
0801 03 is thinking
0801 03 has taken a fork
0801 02 is sleeping
0801 03 has taken a fork
0801 03 is eating
0801 01 has taken a fork
0801 01 has taken a fork
0801 01 is eating
1000 07 is thinking
1000 06 is sleeping
1000 05 has taken a fork
1000 05 has taken a fork
1000 05 is eating
1001 09 is thinking
1001 04 is thinking
1001 08 is sleeping
1001 07 has taken a fork
1001 07 has taken a fork
1001 07 is eating
1001 01 is sleeping
1001 09 has taken a fork
1001 09 has taken a fork
1001 09 is eating
1001 02 is thinking
1001 02 has taken a fork
1001 03 is sleeping
1001 04 has taken a fork
1001 02 has taken a fork
1001 02 is eating
1201 06 is thinking
1201 08 is thinking
1201 05 is sleeping
1201 06 has taken a fork
1201 04 has taken a fork
1201 04 is eating
1201 07 is sleeping
1201 06 has taken a fork
1201 08 has taken a fork
1201 06 is eating
1201 01 is thinking
1201 09 is sleeping
1201 08 has taken a fork
1201 08 is eating
1201 03 is thinking
1201 02 is sleeping
1201 01 has taken a fork
1201 01 has taken a fork
1201 01 is eating
1401 04 is sleeping
1401 05 is thinking
1401 03 has taken a fork
1401 03 has taken a fork
1401 03 is eating
1401 07 is thinking
1401 06 is sleeping
1401 05 has taken a fork
1401 05 has taken a fork
1401 05 is eating
1401 09 is thinking
1401 08 is sleeping
1401 07 has taken a fork
1401 07 has taken a fork
1401 07 is eating
1401 02 is thinking
1401 01 is sleeping
1401 02 has taken a fork
1402 09 has taken a fork
1402 09 has taken a fork
1402 09 is eating
1601 04 is thinking
1601 03 is sleeping
1601 04 has taken a fork
1601 05 is sleeping
1601 06 is thinking
1601 06 has taken a fork
1601 04 has taken a fork
1601 04 is eating
1601 02 has taken a fork
1601 02 is eating
1601 08 is thinking
1601 07 is sleeping
1601 06 has taken a fork
1601 06 is eating
1601 08 has taken a fork
1601 01 is thinking
1602 09 is sleeping
1602 08 has taken a fork
1602 08 is eating
1801 04 is sleeping
1801 02 is sleeping
1801 01 has taken a fork
1801 01 has taken a fork
1801 01 is eating
1801 03 is thinking
1801 03 has taken a fork
1801 03 has taken a fork
1801 03 is eating
1801 05 is thinking
1801 06 is sleeping
1801 07 is thinking
1801 05 has taken a fork
1801 05 has taken a fork
1801 05 is eating
1802 08 is sleeping
1802 09 is thinking
1802 07 has taken a fork
1802 07 has taken a fork
1802 07 is eating
2001 04 is thinking
2001 02 is thinking
2001 01 is sleeping
2001 02 has taken a fork
2001 03 is sleeping
2001 04 has taken a fork
2001 02 has taken a fork
2001 02 is eating
2001 06 is thinking
2001 09 has taken a fork
2001 09 has taken a fork
2001 09 is eating
2001 05 is sleeping
2001 06 has taken a fork
2001 04 has taken a fork
2001 04 is eating
2002 08 is thinking
2002 07 is sleeping
2002 06 has taken a fork
2002 06 is eating
2002 08 has taken a fork
2201 01 is thinking
2201 02 is sleeping
2201 05 is thinking
2201 01 has taken a fork
2201 09 is sleeping
2201 03 is thinking
2201 04 is sleeping
2201 03 has taken a fork
2201 03 has taken a fork
2201 03 is eating
2202 08 has taken a fork
2202 08 is eating
2202 01 has taken a fork
2202 01 is eating
2202 07 is thinking
2202 06 is sleeping
2202 05 has taken a fork
2202 05 has taken a fork
2202 05 is eating
2401 02 is thinking
2402 09 is thinking
2402 03 is sleeping
2402 04 is thinking
2402 04 has taken a fork
2402 08 is sleeping
2402 07 has taken a fork
2402 07 has taken a fork
2402 07 is eating
2402 01 is sleeping
2402 09 has taken a fork
2402 09 has taken a fork
2402 09 is eating
2402 02 has taken a fork
2402 02 has taken a fork
2402 02 is eating
2402 06 is thinking
2402 05 is sleeping
2402 06 has taken a fork
2402 04 has taken a fork
2402 04 is eating
2602 03 is thinking
2602 07 is sleeping
2602 08 is thinking
2602 08 has taken a fork
2602 01 is thinking
2602 09 is sleeping
2602 06 has taken a fork
2602 06 is eating
2602 08 has taken a fork
2602 08 is eating
2602 02 is sleeping
2602 01 has taken a fork
2602 01 has taken a fork
2602 01 is eating
2602 05 is thinking
2602 04 is sleeping
2602 03 has taken a fork
2602 03 has taken a fork
2602 03 is eating
2802 08 is sleeping
2802 06 is sleeping
2802 09 is thinking
2802 07 is thinking
2802 07 has taken a fork
2802 07 has taken a fork
2802 07 is eating
2802 05 has taken a fork
2802 05 has taken a fork
2802 05 is eating
2802 02 is thinking
2802 01 is sleeping
2802 02 has taken a fork
2802 09 has taken a fork
2802 09 has taken a fork
2802 09 is eating
2803 03 is sleeping
2803 02 has taken a fork
2803 02 is eating
2803 04 is thinking
2803 04 has taken a fork
3002 08 is thinking
3002 07 is sleeping
3002 08 has taken a fork
3002 01 is thinking
3002 09 is sleeping
3002 05 is sleeping
3002 06 is thinking
3002 06 has taken a fork
3002 06 has taken a fork
3002 06 is eating
3002 08 has taken a fork
3002 08 is eating
3002 04 has taken a fork
3002 04 is eating
3003 03 is thinking
3003 02 is sleeping
3003 01 has taken a fork
3003 01 has taken a fork
3003 01 is eating
3202 07 is thinking
3202 05 is thinking
3202 09 is thinking
3202 08 is sleeping
3202 07 has taken a fork
3202 04 is sleeping
3202 06 is sleeping
3202 03 has taken a fork
3202 03 has taken a fork
3202 03 is eating
3202 05 has taken a fork
3202 05 has taken a fork
3202 05 is eating
3202 07 has taken a fork
3202 07 is eating
3203 02 is thinking
3203 01 is sleeping
3203 02 has taken a fork
3203 09 has taken a fork
3203 09 has taken a fork
3203 09 is eating
3402 08 is thinking
3402 03 is sleeping
3402 05 is sleeping
3402 02 has taken a fork
3402 02 is eating
3403 04 is thinking
3403 04 has taken a fork
3403 04 has taken a fork
3403 04 is eating
3403 06 is thinking
3403 06 has taken a fork
3403 07 is sleeping
3403 06 has taken a fork
3403 06 is eating
3403 08 has taken a fork
3403 01 is thinking
3403 09 is sleeping
3403 08 has taken a fork
3403 08 is eating
3602 03 is thinking
3603 05 is thinking
3603 04 is sleeping
3603 03 has taken a fork
3603 02 is sleeping
3603 07 is thinking
3603 03 has taken a fork
3603 03 is eating
3603 06 is sleeping
3603 05 has taken a fork
3603 05 has taken a fork
3603 05 is eating
3603 01 has taken a fork
3603 01 has taken a fork
3603 01 is eating
3603 09 is thinking
3603 08 is sleeping
3604 07 has taken a fork
3604 07 has taken a fork
3604 07 is eating
3803 03 is sleeping
3803 04 is thinking
3803 04 has taken a fork
3803 02 is thinking
3803 05 is sleeping
3803 06 is thinking
3803 06 has taken a fork
3803 01 is sleeping
3803 04 has taken a fork
3803 04 is eating
3803 02 has taken a fork
3803 02 has taken a fork
3803 02 is eating
3803 09 has taken a fork
3803 09 has taken a fork
3803 09 is eating
3803 08 is thinking
3804 07 is sleeping
3804 08 has taken a fork
3804 06 has taken a fork
3804 06 is eating
4003 03 is thinking
4003 05 is thinking
4003 04 is sleeping
4003 03 has taken a fork
4003 01 is thinking
4003 02 is sleeping
4003 03 has taken a fork
4003 03 is eating
4003 01 has taken a fork
4003 09 is sleeping
4003 01 has taken a fork
4003 01 is eating
4003 08 has taken a fork
4003 08 is eating
4004 07 is thinking
4004 06 is sleeping
4004 05 has taken a fork
4004 05 has taken a fork
4004 05 is eating
```

</details>

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
