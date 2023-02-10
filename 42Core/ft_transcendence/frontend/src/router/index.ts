import { createRouter, createWebHistory } from "vue-router"
import type { RouteRecordRaw } from "vue-router"
import HomeView from "@/views/HomeView.vue"
import GameView from "@/views/GameView.vue"
import LoginView from "@/views/LoginView.vue"
import ProfileView from "@/views/ProfileView.vue"
import SettingsView from "@/views/SettingsView.vue"
import TwoFactorView from "@/views/TwoFactorView.vue"
import NotFoundView from "@/views/NotFoundView.vue"
import ChatView from '@/views/ChatView.vue'
import { useAxios } from "@/composables/axios"

const { authenticated, clearToken } = useAxios();

// auth: TRUE means users will only access this route if authenticated
// noAuth: TRUE means users will only access this route if NOT authenticated
const routes: Array<RouteRecordRaw> = [
  {
    path: "/login",
    name: "login",
    component: LoginView,
    meta: { auth: false, noAuth: true },
  },
  {
    path: "/logout",
    name: "logout",
    component: {
      beforeRouteEnter(to, from, next) {
        clearToken(); // Should we do something else for logout?

        next("/login");
      }
    },
    meta: { auth: true },
  },
  {
    path: "/2fa/:action?", // Only action that can be provided is "disable"
    name: "2fa",
    component: TwoFactorView,
    meta: { auth: false },
  },
  {
    path: "/",
    name: "home",
    component: HomeView,
    meta: { auth: true },
  },
  {
    path: "/game/:gameId",
    name: "game",
    component: GameView,
    meta: { auth: true },
  },
  {
    path: "/profile/:username42",
    name: "profile",
    component: ProfileView,
    meta: { auth: true },
  },
  {
    path: "/settings",
    name: "settings",
    component: SettingsView,
    meta: { auth: true },
  },
  {
    path: "/:pathMatch(.*)*",
    name: "not-found",
    component: NotFoundView,
    meta: { auth: false },
  },
]

const router = createRouter({
  history: createWebHistory(),
  routes,
});

// auth / noAuth guard
router.beforeEach((to, from, next) => {
  if (to.meta.auth === true && !authenticated()) {
    return next("/login");
  }

  if (to.meta.noAuth === true && authenticated()) {
    return next("/");
  }

  next();
});

export default router;
