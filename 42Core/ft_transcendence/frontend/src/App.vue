<template>
  <el-main id="main">
    <router-view :key="$route.fullPath"/>
    <nav-bar
      v-if="($route.name === 'not-found' && authenticated()) || $route.meta.auth === true"
    />
    <template v-if="me">
      <queue-view
        v-if="me && isQueueOpen"
      />
      <search-container
        v-if="me && isSearchOpen"
      />
    </template>
  </el-main>
</template>

<script setup lang="ts">
import NavBar from "@/components/NavBarSemiCircle.vue"
import QueueView from "@/components/QueueView.vue"
import SearchContainer from "@/components/SearchContainer.vue"
import { useAxios } from "@/composables/axios";
import { useMe } from "@/composables/user/useMe";
import { onMounted, onUnmounted, watch } from "vue";
import { useNavbar } from "@/composables/navbar/useNavbar";
import { useStatus } from "@/composables/profile/useStatus";

const { authenticated } = useAxios();
const { fetchMe, me } = useMe();

onMounted(async () => {
  if (authenticated() && !me.value) {
    await fetchMe();
  }
});

const {
  isQueueOpen,
  isSearchOpen,
} = useNavbar();

const {
  init,
  socketCreate,
  socketReset,
  socketDisconnect,
} = useStatus();
socketCreate();
init();
onUnmounted(socketDisconnect);

watch(
  me,
  (newValue, oldValue) => {
    if (newValue && (!oldValue || newValue.username42 !== oldValue.username42)) {
      socketReset()
    }
  },
  { deep: true }
);

</script>

<style>
#main {
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  padding: 0;
  box-sizing: border-box;
  /* color: #2c3e50; */
}
</style>
