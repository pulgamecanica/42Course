# 💘 Matcha Frontend (React + TypeScript + Vite)

[![Vite](https://img.shields.io/badge/Vite-%23646CFF?logo=vite&logoColor=white)](https://vitejs.dev/)
[![React](https://img.shields.io/badge/React-18-blue?logo=react)](https://react.dev/)
[![TypeScript](https://img.shields.io/badge/TypeScript-4.x-blue?logo=typescript)](https://www.typescriptlang.org/)
[![TailwindCSS](https://img.shields.io/badge/TailwindCSS-2.x-06B6D4?logo=tailwindcss)](https://tailwindcss.com/)
[![Linting](https://img.shields.io/badge/ESLint-TypeChecked-blueviolet?logo=eslint)](https://eslint.org/)

This is the official frontend for **Matcha**, a modern, real-time dating web application. It’s built using **React**, **TypeScript**, and **Vite**, and integrates with the Matcha API to provide a fast, interactive user experience — including messaging, profiles, filtering, and real-time updates via WebSockets.

---

## 🧩 Tech Stack

- ⚛️ React 18
- 🛠️ TypeScript
- ⚡ Vite for lightning-fast builds and HMR
- 🌬️ Tailwind CSS for styling
- 📡 Axios for HTTP requests
- 🧠 JWT Authentication
- 🔌 WebSockets (Faye, Socket.IO, or native)
- ✅ ESLint + Type-aware rules
- 🧰 Reusable components and utilities

---

## 📁 Project Structure

```
.
├── public/               # Static files
├── src/
│   ├── api/              # Reusable API calls
│   ├── components/       # Reusable UI components
│   │   ├── chat
│   │   ├── forms
│   │   ├── location
│   │   ├── match
│   │   └── profile
│   ├── context/         # Context providers (e.g. Auth, WebSocket)
│   ├── pages/            # Route-based pages (protected/public)
│   ├── hooks/            # Custom React hooks
│   ├── utils/            # Utility functions
│   ├── types/            # TypeScript types/interfaces
│   ├── services/         # API abstraction layer (Axios)
│   └── App.tsx
├── index.html            # API documentation viewer (see below)
├── tsconfig.json
├── vite.config.ts
└── README.md

```

---

## 🚀 Getting Started

### 1. Install dependencies

```bash
npm install
````

### 2. Start development server

```bash
npm run dev
```

Runs the app at: [http://localhost:5173](http://localhost:5173)

---

## 🔐 API Integration

* Connects to the Matcha backend (`/auth`, `/users`, `/me`, `/TAGS`, etc.)
* Handles JWT authentication with protected routes
* Mulitple OAuthe external implementation to login (intra & google)
* Uses `localStorage` for session tokens
* Includes advanced features like:
  * Real-time chat and typing indicators via WebSockets
  * Matching with geolocation and filters tailored by the user
  * Voice chat

---

## 🧹 Linting & Code Quality

We use strict type-aware ESLint rules.

For React-specific linting:

```bash
npm run lint
```
---

