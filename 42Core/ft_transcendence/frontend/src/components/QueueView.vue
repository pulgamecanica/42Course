<template>
  <div class="curtain">
    <div
      class="card-md"
      v-if="!joinedQueue"
    >
      <el-alert
        v-show="errors"
        :title="errors"
        type="error"
        center
        show-icon
      />
      <h2>Join a Pong Game</h2>
      <el-button
        :type="'primary'"
        @click="joinQueue"
      >
        Find Match
      </el-button>
      <el-button
        :type="'success'"
        @click="joinQueue"
        disabled
      >
        Play AI
      </el-button>
      <RouterLink
        class="el-button"
        style="padding: 0 2rem; margin: 0 auto;"
        :to="{ name: 'settings' }"
        @click="close"
      >
        <el-tooltip
          effect="light"
          content="Settings"
          placement="top"
        >
          <GearIcon/>
        </el-tooltip>
      </RouterLink>
      <el-button
        style="padding: 2rem;"
        :type="'info'"
      >
        <p>GameID:</p>
        &nbsp;
        <input type="number" ref="goToGameId" style="max-width: 75px;">
        &nbsp;
        <el-button
          :type="'success'"
          @click="goToGame"
        >
          Go
        </el-button>
      </el-button>
      <button
        class="close-btn"
        @click="toggleQueue"
      >
        <svg viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg"><path d="M12,23A11,11,0,1,0,1,12,11.013,11.013,0,0,0,12,23ZM12,3a9,9,0,1,1-9,9A9.01,9.01,0,0,1,12,3ZM8.293,14.293,10.586,12,8.293,9.707A1,1,0,0,1,9.707,8.293L12,10.586l2.293-2.293a1,1,0,0,1,1.414,1.414L13.414,12l2.293,2.293a1,1,0,1,1-1.414,1.414L12,13.414,9.707,15.707a1,1,0,0,1-1.414-1.414Z"/></svg>
      </button>
    </div>
    <div
      class="card-md"
      v-else
    >
      <h3
        class="card-loading"
        v-loading="true"
      >Waiting for another player</h3>
      <button
        class="close-btn"
        @click="close"
      >
        <svg viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg"><path d="M12,23A11,11,0,1,0,1,12,11.013,11.013,0,0,0,12,23ZM12,3a9,9,0,1,1-9,9A9.01,9.01,0,0,1,12,3ZM8.293,14.293,10.586,12,8.293,9.707A1,1,0,0,1,9.707,8.293L12,10.586l2.293-2.293a1,1,0,0,1,1.414,1.414L13.414,12l2.293,2.293a1,1,0,1,1-1.414,1.414L12,13.414,9.707,15.707a1,1,0,0,1-1.414-1.414Z"/></svg>
      </button>
      <el-button
        class="abandon-btn"
        :type="'warning'"
        @click="abandonQueue()"
      >
        Abandon
      </el-button>
    </div>
    <div
      class="matchingWarning"
      v-show="joinedQueue"
    >
      <h2>Matching</h2>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, watch } from "vue";
import { io, Socket } from 'socket.io-client';
import { useQueue } from "@/composables/game/useQueue"
import GearIcon from "@/components/Icons/GearIcon.vue"
import { useRouter } from "vue-router";
import { useNavbar } from "@/composables/navbar/useNavbar";

const { toggleQueue } = useNavbar();
const router = useRouter();
const goToGameId = ref<number>();
const errors = ref<string>();

const {
  socket,
  init: socketInit,
  joinQueue,
  abandonQueue,
  joinedQueue,
  loading,
  errors: queueErrors,
} = useQueue()

socketInit()

watch(
  socket,
  (newValue, oldValue) => {
    if (newValue && (!oldValue || newValue !== oldValue)) {
      socketInit()
    }
  },
  { deep: true }
);

const goToGame = () => {
  if (goToGameId.value && goToGameId.value.value > 0) {
    router.push({ name: "game", params: { gameId: goToGameId.value.value } });
  } else {
    errors.value = "Please enter a valid gameID"
  }
}

const close = () => {
  abandonQueue();
  toggleQueue();
}

</script>

<style lang="scss" scoped>
@keyframes blink {
  from {
    opacity: 1;
  }

  to {
    opacity: 0.25;
  }
}

.matchingWarning {
  position: fixed;
  top: 0;
  right: 0;
  color: var(--yellow);
  margin: 1rem;
  animation: blink 2.5s infinite;

}
.curtain {
  z-index: 2015;
  position: fixed;
  top: 0;
  left: 0;
  overflow: hidden;
  width: 100vw;
  height: 100vh;
  background-color: rgba(42, 42, 42, 0.9);
  display: flex;
  justify-content: center;
  align-items: center;
}
.card-md {
  background-image: url('https://cdn.intra.42.fr/coalition/cover/109/assembly_background.jpg');
  background-position: center;
  background-repeat: no-repeat;
  background-size: cover;
  position: relative;
  max-width: 400px;
  max-height: 400px;
  height: 100%;
  width: 100%;
  display: flex;
  gap: 1rem;
  align-items: center;
  flex-direction: column;
  flex-wrap: no-wrap;
  justify-content: center;
  border-radius: 1rem;
  box-shadow: 0 0 10px 1px var(--dark);
  .card-loading {
    display: flex;
    align-items: center;
    padding: 1rem;
    height: 80%;
  }
  button {
    margin: 0;
  }
  .abandon-btn {
    position: absolute;
    bottom: 0;
    left: 50%;
    transform: translateX(-50%);
    margin: 0.5rem;
  }
  .close-btn {
    position: absolute;
    top: 0;
    right: 0;
    padding: 0.5rem;
    border: none;
    background-color: transparent;
    svg {
      width: 25px;
      height: auto;
      opacity: 0.5;
      &:hover {
        opacity: 1 !important;
      }
      * {
        fill: var(--orange);
      }
    }
  }
}
</style>