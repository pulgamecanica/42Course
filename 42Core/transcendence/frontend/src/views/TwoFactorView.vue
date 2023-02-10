<template>
  <el-container class="center-children">
    <p v-if="!isLogin && !isDisable" class="info">
      Please scan the QRCode and input the generated code.
    </p>
    <el-image v-if="!isLogin && !isDisable" class="image" :src="qrcode">
      <template #placeholder>
        <div class="image-slot">
          <el-icon><icon-picture /></el-icon>
        </div>
      </template>
    </el-image>
    <p v-if="isLogin" class="info">Please input the generated code.</p>
    <p v-if="isDisable" class="info">Please input the code to disable 2FA.</p>
    <el-input
      v-model="code"
      @input="checkDigit()"
      class="code"
      placeholder="2FA Code"
      label="2FA Code"
      inputmode="numeric"
      autocomplete="one-time-code"
      :disabled="loading"
    />
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
  import { ref, onMounted } from "vue";
  const { authenticated } = useAxios();
  import server from "@/server";
  import { Picture as IconPicture } from "@element-plus/icons-vue";
  const { setToken } = useAxios();
  const { fetchMe } = useMe();
  import { useRouter, useRoute } from "vue-router";
  import axios from "axios";
  import { ElMessage } from 'element-plus';

  const router = useRouter();
  const route = useRoute();

  const code = ref(null);
  const qrcode = ref(null);
  const loading = ref(null);
  const isLogin = ref(null);

  const isDisable = route.params.action === "disable";

  onMounted(async () => {
    if (authenticated() && !isDisable) {
      loading.value = true;
      isLogin.value = false;

      server.post("/auth/2fa/generate").then((r) => {
        qrcode.value = r.data;
        loading.value = false;
      })
      .catch(() => {
        ElMessage.error('Error generating QR code!')
      });
    } else if (!isDisable) {
      let token = route.query.access_token;

      if (token) {
        // This will happen for users with 2fa
        isLogin.value = true;
      } else {
        router.push({ name: "logout" });
      }
    } else if (isDisable) {
      isLogin.value = false;
    }
  });

  function next() {
    loading.value = true;

    const noSpaceCode = code.value.replace(/\D/g, ""); // Could use a computed, but I see no need.

    if (isLogin.value === false) {
      server
        .post("/auth/2fa/turn-on-off", {
          twoFactorAuthenticationCode: noSpaceCode,
        })
        .then(async (r) => {
          if (isDisable) {
            router.push({ name: "home" });
            return;
          }
          login(r.data.access_token);
        })
        .catch(() => {
          loading.value = false;
          code.value = "";
          ElMessage.error('Error toggling 2FA!')
        });
    } else {
      // We must specify the token here cuz if it's in localStorage, the system thinks the user is authenticated and will generate a new 2FA secret.
      server
        .post(
          "/auth/2fa/authenticate",
          { twoFactorAuthenticationCode: noSpaceCode },
          { headers: { Authorization: `Bearer ${route.query.access_token}` } }
        )
        .then(async (r) => {
          login(r.data.access_token);
        })
        .catch(() => {
          loading.value = false;
          code.value = "";
          ElMessage.error('Error authenticating 2FA!')
        });
    }
  }

  async function login(token) {
    setToken(token as string);
    await fetchMe();
    router.push({ name: "home" });
  }

  async function pInit(engine: any) {
    await loadFull(engine);
  }

  function checkDigit() {
    const len = 6 + 1; // 6 digits and 1 space at the middle

    if (code.value.length > len) {
      code.value = code.value.slice(0, len);
    } else if (code.value.length === len) {
      next();
    } else if (code.value.length < len) {
      code.value = code.value.replace(/\D/g, "");

      if (code.value.length >= 4) {
        code.value = code.value.slice(0, 3) + " " + code.value.slice(3, len);
      }
    }
  }

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
</script>

<style scoped>
  .center-children {
    height: 100vh;
    align-items: center;
    justify-content: center;
    display: flex;
    flex-direction: column;
  }

  .code {
    width: 42%;
    height: 50px;
    font-size: 18px;
  }

  .code :deep(.el-input__wrapper) {
    backdrop-filter: blur(10px);
  }

  .code :deep(.el-input__inner) {
    text-align: center;
    caret-color: rgba(255, 255, 255, 0.5);
  }

  .image {
    width: 200px;
    height: 200px;
    margin-bottom: 20px;
    border-radius: 5px;
  }

  .info {
    color: #fff;
    font-size: 18px;
    margin-bottom: 20px;
    font-weight: 600;
    max-width: 300px;
  }

  .image .el-icon {
    width: 200px;
    height: 200px;
    backdrop-filter: blur(10px);
    font-size: 42px;
  }
</style>
