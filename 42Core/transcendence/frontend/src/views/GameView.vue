<template>
  <el-container
    class="game"
    :class="fullScreen === true ? 'full-screen' : 'normal-screen'"
    v-if="!loading && game"
  >
    <el-button
      class="exit-full-screen"
      v-if="fullScreen"
      @click="toggleFullScreen"
      type="info"
    >
      <svg viewBox="0 0 24 24" fill="none">
        <path d="M9.00001 18.0001L9.00001 17.0001C9.00001 15.8956 8.10458 15.0001 7.00001 15.0001H6.00001M15 18.0001V17.0001C15 15.8956 15.8954 15.0001 17 15.0001L18 15.0001M9 6.00012L9 7.00012C9 8.10469 8.10457 9.00012 7 9.00012L6 9.00012M15 6.00014L15 7.00014C15 8.10471 15.8954 9.00014 17 9.00014L18 9.00014" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
      </svg>
    </el-button>
    <el-button
      class="enter-full-screen"
      v-else
      @click="toggleFullScreen"
      type="info"
    >
      <svg viewBox="0 0 24 24" fill="none">
        <path d="M6 15V16C6 17.1046 6.89543 18 8 18H9M18 15V16C18 17.1046 17.1046 18 16 18H15M6 9V8C6 6.89543 6.89543 6 8 6H9M18 9V8C18 6.89543 17.1046 6 16 6H15" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"/>
      </svg>
    </el-button>
    <span
      v-show="!fullScreen"
    >
      <PongGameInfo/>
      <PongGameStats
        v-if="['finished', 'suspended'].includes(gameState.gameStatus) || ['finished', 'suspended'].includes(game.status)"
      />
      <PongGameOptions
        v-if="['starting', 'playing'].includes(gameState.gameStatus)"
      />
    </span>
    <PongGameCanvas
      v-if="['starting', 'playing'].includes(gameState.gameStatus)"
    />
    <el-button
      class="spectators"
      type="info"
    >
      {{ gameState.spectators }} Spectators
    </el-button>
  </el-container>
</template>

<script setup lang="ts">
import PongGameInfo from '@/components/PongGameInfo.vue';
import PongGameOptions from '@/components/PongGameOptions.vue';
import PongGameCanvas from '@/components/PongGameCanvas.vue';
import PongGameStats from '@/components/PongGameStats.vue';
import { useRoute } from "vue-router";
import { onUnmounted, computed, watch, ref } from "vue";
import { useGame, useGameSocket } from "@/composables/game/useGame";
import { useMe } from '@/composables/user/useMe';

const fullScreen = ref<boolean>(false);
const toggleFullScreen = () => {
  fullScreen.value = !fullScreen.value;
}
const { me, loading: imLoading } = useMe();
const route = useRoute();
const gameId = computed( () => +route.params.gameId);
const {
  fetchGameInfo,
  game,
  loading,
} = useGame();
fetchGameInfo(gameId.value);
const {
  socketCreate,
  socketDisconnect,
  init: socketInit,
  socketReset,
  pongGameState: gameState,
} = useGameSocket();
socketCreate()
socketInit()
onUnmounted(socketDisconnect)

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

<style lang="scss" scoped>
.spectators {
  margin: 1rem;
  position: fixed;
  bottom: 0;
  right: 0;
}
.normal-screen {
  display: block;
}
.exit-full-screen {
  position: fixed;
  z-index: 3000;
  top: 5px;
  left: 50%;
  transform: translateX(-50%);
  opacity: 0.75;
  &:hover {
    opacity: 1;
  }
  svg {
    width: 30px;
    height: auto;
  }
}
.enter-full-screen {
  position: fixed;
  z-index: 3000;
  bottom: 10px;
  left: 10px;
  svg {
    width: 30px;
    height: auto;
  }
}
</style>
