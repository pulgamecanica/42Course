# 💘 Matcha

Welcome to **Matcha**, a real-time, fully-featured dating platform built with **Ruby**, **React**, and **Love** 💎  
Swipe, connect, chat — all backed by a beautiful API and blazing-fast frontend.

---

<div align="center">

🎥 Demo Preview (Login)

![Screencastfrom2025-05-0611-48-52-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/7348ffc8-25d3-4781-aef4-a75a07a00808)

***

🎥 Demo Preview (Profile & Matching)

![Screencastfrom2025-05-0611-49-11-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/060adffc-3435-4936-8d11-f5f3679d6739)


</div>

---

## 🌐 Live Preview

- 🔗 [API Docs Viewer](https://pulgamecanica.github.io/matcha-docs/)
- 🔗 [Matcha Frontend](https://www.42matcha.com/)

---

## 🧱 The Architecture

| Layer      | Stack                                      |
|------------|--------------------------------------------|
| **Backend** | Sinatra + PostgreSQL + RSpec + Docker       |
| **Frontend** | React 18 + TypeScript + Vite + Tailwind     |
| **Live Docs** | Static Viewer with Simulated API Calls     |
| **Auth**     | JWT + OAuth (Google, Intra42)               |
| **Realtime** | WebSockets for Chat, Typing Indicators and audio with [Faye](https://faye.jcoglan.com/ruby/websockets.html)   |

---

🧭 **Architecture Diagram**  

For the backend we decided to implement an API with Sinatra, following M~V~C (model-~view~-controller)
And we created a Domain-specific language (DSL) for the API documentation.

```txt
├── api
│   ├── app
│   │   ├── controllers
│   │   ├── helpers
│   │   ├── lib
│   │   └── models
│   ├── config
│   ├── db
│   │   └── migrate
│   ├── docs
│   └── spec
│       ├── controllers
│       ├── helpers
│       └── models
├── docs
└── web
    ├── public
    └── src
        ├── api
        ├── components
        │   ├── chat
        │   ├── forms
        │   ├── location
        │   ├── match
        │   └── profile
        ├── context
        ├── hooks
        ├── pages
        ├── types
        └── utils
```

---

## 🔥 Core Features

- 🧪 **TDD API** with RSpec [100% endpoint coverage]
- 🔐 **JWT Auth** with confirmation, ban, and session validation
- 💬 **Live Messaging** with typing detection
- 💑 **Matching System** with filters and geolocation
- 📞 **Voice Chat**
- 📷 **Picture Uploads & Profile Management**
- 📦 **Fully Dockerized Setup**
- 📘 **API Docs Generator** with simulation UI

---

<div align="center">


### 🖼️ Screenshots

<details>
  <summary><strong>🧭 Dashboard</strong> — Real-time feed and quick actions</summary>

|                                                                                    |                                                                                               |
| ---------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- |
| 💬 Real-time feed showing new matches, profile activity, and shortcuts to actions. | ![Dashboard](https://github.com/user-attachments/assets/886b169b-5f92-4481-a54c-b856d90bd3a7) |

</details>

<details>
  <summary><strong>💬 Chat Interface</strong> — WebSocket-powered messaging + voice</summary>

|                                                                                        |                                                                                          |
| -------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------- |
| Messaging UI with live typing indicators, full message history, and voice call button. | ![Chat](https://github.com/user-attachments/assets/d5754b88-0c8c-4f92-b277-8c88485b03b7) |

</details>

<details>
  <summary><strong>👤 Profile Page</strong> — View user bios, tags, and photos</summary>

|                                                                                        |                                                                                             |
| -------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- |
| Full user profile with block/report actions, pictures, online status, and shared tags. | ![Profile](https://github.com/user-attachments/assets/79b80c13-ac83-4aec-8fca-5598fb226015) |

</details>

<details>
  <summary><strong>🗺️ Matches (Map View)</strong> — Interactive exploration</summary>

|                                                                                         |                                                                                             |
| --------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- |
| Discover matches based on location. Interactive map with card popups and quick connect. | ![Matches](https://github.com/user-attachments/assets/cac31f7e-bfc8-48e4-aec5-6bb0275c4de1) |

</details>

<details>
  <summary><strong>💘 Matches (List)</strong> — Customizable filter for matches</summary>

|                                                                                              |                                                                                   |
| -------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------- |
| Search people to fall in love! | ![Matches](https://github.com/user-attachments/assets/2c9d006b-fbc3-4a3e-ae5b-96e16ddb7a6b) |

</details>

</div>

---


## 💡 Philosophy

> **📜 Everything documented**
> 
> **🧪 Everything tested**
> 
> **🧼 Everything clean**
> 
> **💥 Everything fails gracefully**
> 
> **❤️ Everything feels human**

---

Made with 💘 by [@Jsarda42](https://github.com/Jsarda42) & [@pulgamecanica](https://github.com/pulgamecanica)
