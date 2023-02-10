<template>
  <el-container class="center-children">
    <el-button class="login-button" @click="login"
      >Login with 42</el-button
    >
    <particles
      id="tsparticles"
      :options="particleOptions"
      :particlesInit="pInit"
    />
  </el-container>
</template>

<script setup lang="ts">
  import { useAxios } from "@/composables/axios";
  import { loadFull } from "tsparticles";
  import { useMe } from "@/composables/user/useMe";
  import { onMounted } from "vue";
  import { useRouter, useRoute } from "vue-router";

  const { setToken, login } = useAxios();
  const { fetchMe, me, firstOn } = useMe();
  const router = useRouter();
  const route = useRoute();

  onMounted(async () => {
    let token = route.query.access_token;
    let firstLogin = route.query.first_login; // TODO: add first_login boolean to backend callback

    if (token) {
      // This will happen for users without 2fa
      setToken(token as string);
      await fetchMe();

      if (firstLogin && me.value) {
        firstOn()
        router.push({ name: "profile", params: { username42: me.value.username42 } });
      } else {
        router.push({ name: "home" });
      }
    }
  });

  const particleOptions = {
    fullScreen: {
      enable: true,
      zIndex: -1,
    },
    background: {
      color: "#111",
    },
    particles: {
      links: {
        distance: 125,
        enable: true,
        triangles: {
          enable: true,
          opacity: 0.1,
        },
      },
      move: {
        enable: true,
        speed: 5,
      },
      size: {
        value: 0,
      },
      // shape: {
      //   type: "circle",
      // },
    },
  };

  async function pInit(engine: any) {
    await loadFull(engine);
  }
</script>

<style scoped>
  .center-children {
    height: 100vh;
    align-items: center;
    justify-content: center;
    display: flex;
  }

  .login-button {
    width: 250px;
    height: 50px;
    font-size: 18px;
    backdrop-filter: blur(10px);
  }
</style>
