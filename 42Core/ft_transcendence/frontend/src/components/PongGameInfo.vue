<template>
  <el-row
    class="el-card"
    v-if="!loadingGame && gameState"
  >
    <el-col
      class="el-card-centered"
      :span="12"
      v-if="game.userOne"
    >
      <OnlineConnection
        v-if="!gameState.playerLeft"
      />
      <OfflineConnection
        v-if="gameState.playerLeft"
      />
      <RouterLink
        class="c-media__link u-text--medium"
        :to="{ name: 'profile', params: { username42: game.userOne.username42 }}"
      >
        @{{ game.userOne.username42 }}
      </RouterLink>
      <el-avatar
        class="animate__animated animate__headShake"
        :src="game.userOne.avatarImg"
        :size="100"
      />
    </el-col>
    <el-col
      class="el-card-centered"
      :span="12"
      v-if="game.userTwo"
    >
      <el-avatar
        class="animate__animated animate__headShake"
        :src="game.userTwo.avatarImg"
        :size="100"
      />
      <RouterLink
        class="c-media__link u-text--medium"
        :to="{ name: 'profile', params: { username42: game.userTwo.username42 }}"
      >
        @{{ game.userTwo.username42 }}
      </RouterLink>
      <OnlineConnection
        v-if="!gameState.playerRight"
      />
      <OfflineConnection
        v-if="gameState.playerRight"
      />
    </el-col>
  </el-row>
</template>

<script setup lang="ts">
import OnlineConnection from '@/components/Icons/OnlineConnection.vue';
import OfflineConnection from '@/components/Icons/OfflineConnection.vue';
import { computed } from "vue";
import { useGame, useGameSocket } from "@/composables/game/useGame";

const {
  pongGameState: gameState,
} = useGameSocket();
const {
  game,
  loading: loadingGame,
} = useGame();
</script>

<style lang="scss" scoped="">
.el-card {
  padding: 1rem 0;
}
.el-card-centered {
  display: flex;
  justify-content: center;
  gap: 1rem;
  align-items: center;
}
</style>
