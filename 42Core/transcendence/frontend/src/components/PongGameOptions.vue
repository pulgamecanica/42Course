<template>
  <div
    v-if="isPlayer"
    v-show="showRules"
    class="fixed-card"
  >
    <el-card
      ref="cardLg"
      class="cardLg"
    >
      <h1>Pong-Game</h1>
      <hr>
      <el-button
        type="success"
        style="height: 50px;"
        @click="toggleRules"
        >
        <h1>Start Playing</h1>
      </el-button>
      <hr>
      <h3>RULES</h3>
      <hr>
      <h5>Use the keys to move the paddle Up and Down<br>[ArrowUp ↑ & ArrowDown ↓]</h5>
      <hr>
      <h5>Each Player must be connected and ready to start the game</h5>
      <el-button
        :type="'success'"
        disabled
      >
        Ready/Hold
      </el-button>
      <hr>
      <h5>You can see the if the player is connected next to it's image
      </h5>
      <OnlineConnection/>
      <OfflineConnection/>
      <h5>This icons will indicate if the user is ready</h5>
      <img
        style="height: 50px; width: auto;"
        src="@/assets/tux2.gif"
      />
      <img
        style="height: 50px; width: auto;"
        src="@/assets/catWaiting.gif"
      />
      <hr>
      <h5>The winner is the player who scores 10 points</h5>
      <h2>10 | X</h2>
      <hr>
      <h5>You only get poits for your ranking when you play online and using the queue</h5>
      <h5>Invitation Games don't count for the ladder</h5>
      <hr>
      <details>
        <summary>=> Read More &lt= </summary>
        <h5>Settings</h5>
        <h5>Open the NevBar on the bottom of the screen</h5>
        <Logo42/>
        <h5>Then open The Game Pannel</h5>
        <Controller/>
        <h5>Now click on the Gear icon</h5>
        <GearIcon/>
        <h5>Create as many settings as you want</h5>
        <h5>You will unluck new skins if you win achievements</h5>
      </details>
    </el-card>
  </div>
  <el-row
    class="waitingRow"
  >
    <el-col
      class="waitingCol"
      v-loading="!playerLeftReady"
      :xs="24"
      :sm="24"
      :md="12"
    >
      <img
        class="mirrorImg"
        v-show="playerLeftReady"
        src="@/assets/tux2.gif"
      />
      <img
        class="mirrorImg"
        v-show="!playerLeftReady"
        src="@/assets/catWaiting.gif"
      />
    </el-col>
    <el-col
      class="waitingCol"
      v-loading="!playerRightReady"
      :xs="24"
      :sm="24"
      :md="12"
    >
      <img
        v-show="playerRightReady"
        src="@/assets/tux2.gif"
      />
      <img
        v-show="!playerRightReady"
        src="@/assets/catWaiting.gif"
      />
    </el-col>
    <el-col :span="24">
      <el-button
        v-if="isPlayer && gameStatus !== 'playing' && gameState.playerLeft && gameState.playerRight"
        style="margin: 1rem;"
        @click="readyToStart"
        :type="'success'"
      >
        Ready/Hold
      </el-button>
      <el-button
        v-else-if="gameStatus !== 'playing'"
        style="margin: 1rem;"
        :type="'warning'"
      >
        Wait for oponent
      </el-button>
      <el-button
        style="margin: 1rem;"
        @click="openMenu"
      >
        <GearIcon/>
      </el-button>
    </el-col>
  </el-row>
  <div
    class="pongMenu"
    v-show="showMenu">
    <el-button
      type="info"
      disabled
    >
      {{ gameStatus }}
    </el-button>
    <span>
      <h4>Setting: &nbsp;</h4>
      <el-select
        class="el-card"
        style="border: none !important;"
        v-model="valueSetting"
        placeholder="Default"
        @change="changeCurrentSetting"
      >
        <el-option
          v-for="item, i in settings"
          :key="i"
          :label="item.name"
          :value="item"
        />
      </el-select>
    </span>
    <el-button
      type="danger"
      @click="abandonGame"
    >
      Abandon
    </el-button>
    <el-button
      @click="toggleRules"
    >
      <Book/>
    </el-button>
    <audio
      style="margin-bottom: 1rem;"
      ref="audio"
      loop="true"
      controls
    >
      <source  src="../assets/music/tloz_ww_symphonic_movement.mp3" type="audio/mpeg">
        Not supported
    </audio>
  </div>
</template>

<script setup lang="ts">
import OnlineConnection from '@/components/Icons/OnlineConnection.vue';
import OfflineConnection from '@/components/Icons/OfflineConnection.vue';
import Logo42 from '@/components/Icons/Logo42.vue';
import Controller from '@/components/Icons/Controller.vue';
import Book from '@/components/Icons/Book.vue';
import GearIcon from "@/components/Icons/GearIcon.vue"
import { onClickOutside } from '@vueuse/core'
import { ref, computed, onMounted } from 'vue';
import { useMe } from '@/composables/user/useMe';
import { useProfileSettings } from '@/composables/profile/useProfileSettings';
import { useGameSocket } from "@/composables/game/useGame";
import { ElMessage, ElNotification } from 'element-plus'

/*************************************/
/*          Volume & Audio           */
/*************************************/
const audio = ref<HTMLAudioElement>();

onMounted(async () => {
  try {
    if (audio.value) {
      await audio.value.play();
      console.log("Playing audio");
    }
    console.log(audio.value);
  } catch (error) {
    ElNotification.warning({
      title: 'Auto Play',
      message: 'You probably need to enable audio on the permissions to play it!',
      duration: 0,
    });
    console.log(error);
  }
});

const valueSetting = ref('');
const { settings, fetchSettings, changeCurrentSetting: changeCurrentSettingProfile } = useProfileSettings()
const showMenu = ref<boolean>(false);
const { me } = useMe();
const {
  readyToStart,
  pongGameState: gameState,
  abandonGame,
} = useGameSocket();
const isPlayer = computed(() => {
  if (me.value && gameState.value) {
    if (gameState.value.playerRight) {
      if (me.value.username42 === gameState.value.playerRight.username42) {
        return true;
      }
    }
    if (gameState.value.playerLeft) {
      if (me.value.username42 === gameState.value.playerLeft.username42) {
        return true;
      }
    }
  }
  return false;
});
const gameStatus = computed(() => {
  if (gameState.value) {
    return gameState.value.gameStatus;
  }
  return "No Game";
});
const playerRightReady = computed(() => {
  if (gameState.value && gameState.value.playerRight) {
    return gameState.value.playerRight.ready;
  }
  return false;
});
const playerLeftReady = computed(() => {
  if (gameState.value && gameState.value.playerLeft) {
    return gameState.value.playerLeft.ready;
  }
  return false;
});
const openMenu = (event) => {
  showMenu.value = !showMenu.value;
};
const changeCurrentSetting = () => {
  changeCurrentSettingProfile(valueSetting.value);
}
fetchSettings();
const cardLg = ref(null);
const showRules = ref<boolean>(true);
const toggleRules = () => {
  showRules.value = !showRules.value;
  showMenu.value = false;
}
onClickOutside(cardLg, (event) => showRules.value = false);
</script>

<style lang="scss">
@use '../assets/foundation/vars';

details {
  cursor: pointer;
  padding: 0.5rem;
  &:hover {
    background: rgba(0, 0, 0, 0.5);
  }
  svg {
    width: 50px;
    height: 50px;
    * {
      fill: white;
    }
  }
}
.fixed-card {
  z-index: 2015;
  position: fixed;
  top: 0;
  width: 100vw;
  height: 100vh;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  background-color: rgba(42, 42, 42, 0.9);
  cursor: zoom-out;
  .cardLg {
    background: none;
    cursor: help;
    z-index: 2016;
    width: 90vw;
    max-width: 600px;
    height: 90vh;
    max-height: 800px;
    overflow: scroll;
    position: relative;
    background-image: url('https://cdn.intra.42.fr/coalition/cover/109/assembly_background.jpg');
    background-position: center;
    background-repeat: no-repeat;
    background-size: cover;
  }
}
.waitingRow {
  width: 100%;
  margin: 0 !important;
  .waitingCol {
    display: flex;
    align-items: center;
    justify-content: center;
    img {
      height: 45px;
      width: auto;
    }
    .mirrorImg {
      -webkit-transform: scaleX(-1);
      transform: scaleX(-1);
    }
  }
}

.pongMenu {

  background-image: url('https://cdn.intra.42.fr/coalition/cover/109/assembly_background.jpg');
  background-position: center;
  background-repeat: no-repeat;
  background-size: cover;
  border: 0.25rem var(--darker) solid;
  border-radius: 1rem;
  max-width: 500px;
  min-height: 300px;
  display: flex;
  flex-flow: column;
  flex-wrap: no-wrap;
  justify-content: center;
  margin: 2rem auto;
  align-items: center;
  * {
    z-index: 10 !important;
  }
  &:hover {
    border-color: var(--orange);
  }
}

.slider {
  width: 90%;
  display: flex;
  align-items: center;
  margin: auto;
  background-color: var(--darker);
  border-radius: 2rem;
}

.slider .el-slider {
  width: 80%;
  margin: 0 12px 0 12px;
}

.slider .volume {
  font-size: 14px;
  color: var(--secondary);
  line-height: 44px;
  display: flex;
  align-items: center;
  flex: 1;
  overflow: hidden;
  text-overflow: ellipsis;
  white-space: nowrap;
  margin-left: 5px;
  svg {
    height: 25px;
    width: 25px;
  }
}
.pongMenu .el-button {
  margin: auto !important;
}
</style>
