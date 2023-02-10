<template>
  <div
    class="curtain"
  >
    <el-card
      class="card-lg"
      ref="cardLg"
    >
      <button
        class="close-btn"
        @click="toggleSearch"
      >
        <svg viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg"><path d="M12,23A11,11,0,1,0,1,12,11.013,11.013,0,0,0,12,23ZM12,3a9,9,0,1,1-9,9A9.01,9.01,0,0,1,12,3ZM8.293,14.293,10.586,12,8.293,9.707A1,1,0,0,1,9.707,8.293L12,10.586l2.293-2.293a1,1,0,0,1,1.414,1.414L13.414,12l2.293,2.293a1,1,0,1,1-1.414,1.414L12,13.414,9.707,15.707a1,1,0,0,1-1.414-1.414Z"/></svg>
      </button>
      <el-row
        class="el-card card-center-start light-card"
        :gutter="20"
      >
        <el-col
          class="card-col"
          :md="10"
          :sm="24"
        >
          <h2 class="title">Search Players</h2>
          <el-input
            v-model="input"
            class="w-50 m-2"
            size="large"
            placeholder="Please Input"
            :suffix-icon="Search"
            v-on:keyup="autoSearchUsers"
          />
          <span
            v-if="numberOfResults"
          >
            <h4>{{ numberOfResults }} results for {{ input }}</h4>
            <hr>
          </span>
          <span
            v-else-if="finishedTyping && input !== ''"
          >
            <el-image
              style="margin-top: 2rem;"
              :src="'https://www.icegif.com/wp-content/uploads/2022/06/icegif-525.gif'"
              :fit="'cover'"
            />
            <h4>No Results for {{ input }}</h4>
          </span>
          <span
            v-if="loadingResults || !finishedTyping"
          >
            <h3
              v-loading="true"
              element-loading-text="Loading..."
              element-loading-background="rgba(122, 122, 122, 0.8)"
              style="height: 100px;"
              class=""
            >
            </h3>
          </span>
          <el-row
            class="result-card el-card"
            v-else
            v-for="result in foundedResults"
            :key="result"
            :gutter="10"
            >
            <el-col
              class="card-center dark-card "
              style="height: 35px;"
              :md="6"
            >
              <el-tooltip
                class="box-item"
                effect="light"
                :raw-content="true"
                :content="result.email +  '<br>' + result.username42 +  '<br>' + result.nickname +  '<br>' + result.firstName +  '<br>' + result.lastName"
                placement="left-start"
              >
                <el-avatar
                  class="animate__animated animate__headShake"
                  :src="result.avatarImg"
                  :size="25"
                />
              </el-tooltip>
            </el-col>
            <el-col
              class="card-center"
              style="min-height: 25px;"
              :md="18"
            >
              <RouterLink
                :to="{ name: 'profile', params: { username42: result.username42 }}"
                @click="toggleSearch"
              >
                @{{ result.username42 }}
              </RouterLink>
            </el-col>
          </el-row>
        </el-col>
        <el-col
          class="card-col"
          :md="10"
          :sm="24"
        >
          <div
            class="card-content"
            v-if="!loadingGames"
          >
            <h2 class="title">Pong Games</h2>
            <RouterLink
              v-if="routeName != 'game'"
              :to="{ name: 'game', params: { gameId: game.id }}"
              class="result-card el-card btn el-row"
              v-for="game in games"
              :key="game.id"
              @click="toggleSearch"
              >
              <button
                class="game-status"
              >
                <svg
                  class="pause"
                  viewBox="0 0 512 512"
                  xml:space="preserve"
                  v-if="game.status === 'suspended'"
                >
                  <g>
                    <path class="st0" d="M256,0C114.625,0,0,114.625,0,256c0,141.374,114.625,256,256,256s256-114.626,256-256
                      C512,114.625,397.375,0,256,0z M224,336h-64V176h64V336z M352,336h-64V176h64V336z"/>
                  </g>
                </svg>
                <svg
                  class="playing"
                  v-if="game.status === 'playing' || game.status === 'starting'"
                  viewBox="0 0 24 24"
                >
                  <path d="M21.261,2.739A9.836,9.836,0,0,0,8.3,2.114,4.489,4.489,0,1,0,4.134,8.963a9.415,9.415,0,0,0,.842,5.668.5.5,0,0,1-.07.564L2,16.945A3.743,3.743,0,0,0,3.735,24a3.891,3.891,0,0,0,.457-.027,3.705,3.705,0,0,0,2.725-1.735l2.068-3.127a.5.5,0,0,1,.575-.089,9.663,9.663,0,0,0,11.315-2.147A10.5,10.5,0,0,0,24,9.758,9.409,9.409,0,0,0,21.261,2.739ZM2,4.5A2.5,2.5,0,1,1,4.5,7,2.5,2.5,0,0,1,2,4.5Zm8.44,12.726a2.494,2.494,0,0,0-3.017.632c-.024.029-.046.059-.067.09L5.229,21.166A1.742,1.742,0,0,1,2.02,20a1.76,1.76,0,0,1,.961-1.312l3.041-1.831a.956.956,0,0,0,.126-.09,2.49,2.49,0,0,0,.623-3.016,7.331,7.331,0,0,1-.693-4.259l8.433,8.433A7.31,7.31,0,0,1,10.44,17.226Zm9.021-1.765a8.871,8.871,0,0,1-2.732,1.865c-.009-.01-.012-.023-.022-.033L7.36,7.945A4.473,4.473,0,0,0,9,4.5c0-.119-.026-.231-.035-.347a8.01,8.01,0,0,1,10.882,0A7.423,7.423,0,0,1,22,9.7,8.506,8.506,0,0,1,19.461,15.461Z"/>
                </svg>
                <svg
                  class="finished"
                  viewBox="0 0 512 512"
                  xml:space="preserve"
                  v-if="game.status === 'finished'"
                >
                  <g>
                    <path class="st0" d="M493.379,55.416c-11.458-11.479-27.441-18.63-44.952-18.62h-29.049h-14.736H107.358H92.632H63.573
                      c-17.502-0.01-33.494,7.142-44.952,18.62C7.142,66.874-0.01,82.866,0,100.368v33.297c0,40.979,24.949,77.828,62.994,93.045
                      l0.569,0.225l66.065,20.24c3.286,4.258,6.75,8.427,10.615,12.41c24.537,25.233,48.386,42.177,64.82,55.931
                      c8.221,6.848,14.481,12.881,18.15,18.013c1.854,2.561,3.08,4.846,3.856,6.956c0.766,2.119,1.128,4.061,1.128,6.327
                      c0,31.983,0,13.5,0,45.483c0,6.612-0.932,12.292-2.482,16.982c-2.364,7.054-5.954,11.831-10.37,15.089
                      c-4.455,3.218-10.007,5.141-17.424,5.17c-26.449,0-26.704,0-26.724,0h-14.736v45.669h199.078v-45.669h-14.736c0,0-0.265,0-26.715,0
                      c-4.945,0-9.075-0.893-12.577-2.403c-5.23-2.306-9.282-5.916-12.47-11.528c-3.139-5.602-5.238-13.372-5.238-23.31
                      c0-31.983,0-13.5,0-45.483c0.009-2.266,0.373-4.208,1.128-6.327c1.324-3.66,4.179-8.016,9.046-13.107
                      c7.23-7.653,18.65-16.541,32.267-27.382c13.616-10.88,29.402-23.84,45.511-40.41c3.866-3.983,7.338-8.152,10.616-12.41
                      l66.074-20.24l0.56-0.225C487.061,211.494,512,174.644,512,133.665v-33.297C512.01,82.866,504.867,66.874,493.379,55.416z
                       M73.55,199.191c-26.646-10.841-44.089-36.732-44.089-65.526v-33.297c0.01-9.458,3.797-17.904,9.987-24.115
                      c6.23-6.201,14.657-9.987,24.125-9.997h29.01c-0.03,6.858-0.07,13.725-0.07,20.554c0.02,31.59,0.736,62.749,6.956,92.22
                      c2.237,10.556,5.229,20.887,9.154,30.903L73.55,199.191z M181.733,225.101c0,0-42.696-40.185-42.696-80.369s0-77.858,0-77.858
                      h42.696V225.101z M482.548,133.665c0,28.794-17.453,54.685-44.099,65.526l-35.093,10.742c6.672-16.992,10.596-34.916,12.882-53.301
                      c2.815-22.682,3.237-46.13,3.247-69.823c0-6.828-0.04-13.696-0.069-20.554h29.01c9.468,0.01,17.904,3.796,24.124,9.997
                      c6.191,6.21,9.988,14.647,9.998,24.115V133.665z"/>
                  </g>
                </svg>
              </button>
              <el-col
                class="card-center dark-card"
                style="height: 50px;"
                :md="10"
              >
                <el-avatar
                  class="animate__animated animate__headShake"
                  :src="game.userOne.avatarImg"
                  :size="25"
                />
              </el-col>
              <el-col
                class="card-center"
                :md="4"
              >
                <h4>vs</h4>
              </el-col>
              <el-col
                class="card-center dark-card"
                style="height: 50px;"
                :md="10"
              >
                <el-avatar
                  class="animate__animated animate__headShake"
                  :src="game.userTwo.avatarImg"
                  :size="25"
                />
              </el-col>
            </RouterLink>
            <el-row
              v-else-if="currentGame"
            >
              <el-col
                :span="24"
                >
                <h4>You are currently watching a game</h4>
                <h4>Game {{ currentGame.status }}</h4>
              </el-col>
              <el-col
                class="card-center dark-light"
                style="height: 50px;"
                :span="10"
              >
                <el-avatar
                  class="animate__animated animate__headShake"
                  :src="currentGame.userOne.avatarImg"
                  :size="50"
                />
              </el-col>
              <el-col
                class="card-center"
                :span="4"
              >
                <h4>vs</h4>
              </el-col>
              <el-col
                class="card-center dark-light"
                style="height: 50px;"
                :span="10"
              >
                <el-avatar
                  class="animate__animated animate__headShake"
                  :src="currentGame.userTwo.avatarImg"
                  :size="50"
                />
              </el-col>
              <el-col
                :span="10"
              >
                <h3>{{ currentGame.scoreOne }}</h3>
              </el-col>
              <el-col
                :span="4"
              >
              <h3>-</h3>
              </el-col>
              <el-col
                :span="10"
              >
                <h3>{{ currentGame.scoreTwo }}</h3>
              </el-col>
              <!--{{currentGame}}-->
            </el-row>
          </div>
        </el-col>
      </el-row>
    </el-card>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from "vue";
import { onClickOutside } from '@vueuse/core'
import { Search } from '@element-plus/icons-vue'
import { useGame } from "@/composables/game/useGame";
import { useSearch } from "@/composables/user/useSearch";
import { useNavbar } from "@/composables/navbar/useNavbar";
import { useRoute } from 'vue-router';

interface User {
  id?: number
  intraId?: number
  email?: string
  username42?: string
  firstName?: string
  lastName?: string
  nickname?: string
  avatarImg?: string
  bio?: string
  status?: string
}

const { toggleSearch } = useNavbar();

const route = useRoute()
const routeName = computed(() => route.name)
const input = ref<string>('');
const cardLg = ref(null);
const url = 'www.icegif.com/wp-content/uploads/2022/06/icegif-525.gif'
const {
  searchUsers,
  users: foundedResults,
  loading: loadingResults,
  errors,
} = useSearch();
searchUsers();
const {
  fetchAllGames,
  game: currentGame,
  gameDuration,
  games,
  loading: loadingGames,
} = useGame();
fetchAllGames();
const numberOfResults = computed(() => {
  if (!foundedResults.value) {
    return 0;
  }
  return foundedResults.value.length;
  }
);
let timeout = null;
let finishedTyping = true;
const autoSearchUsers = () => {
  finishedTyping = false;
  clearTimeout(timeout);
  timeout = setTimeout(function () {
    console.log("Finished Typing", input.value);
    searchUsers(input.value);
    finishedTyping = true;
  }, 1000);
};
onClickOutside(cardLg, (event) => toggleSearch());

</script>

<style lang="scss" scoped>
.curtain {
  z-index: 2015;
  position: fixed;
  top: 0;
  left: 0;
  width: 100vw;
  height: 100vh;
  background-color: rgba(42, 42, 42, 0.9);
  display: flex;
  justify-content: center;
  align-items: center;
  cursor: zoom-out;
}
.card-lg {
  background-image: url('https://cdn.intra.42.fr/coalition/cover/109/assembly_background.jpg');
  background-position: center;
  background-repeat: no-repeat;
  background-size: cover;
  position: relative;
  max-width: 800px;
  min-height: 400px;
  height: 90%;
  width: 95%;
  border-radius: 1rem;
  padding-top: 2rem;
  overflow: scroll;
  box-shadow: 0 0 10px 1px var(--dark) !important;
  cursor: auto;
  .game-starting .circular {
    width: 20px !important;
  }
  .btn {
    cursor: pointer;
    &:hover {
      opacity: 0.75;
    }
  }
  .card-col {
    min-height: 400px;
    .title {
      border-bottom: 1px var(--light) solid;
    }
    &:hover {
      background-color: rgba(0, 0, 0, 0.5);
    }
  }
  .result-card {
    overflow: inherit;
    margin: 1rem auto;
    .el-button {
      background-color: transparent;
      border: none;
    }
  }
  .close-btn, .game-status {
    position: absolute;
    top: 0;
    right: 0;
    border: none;
    background-color: transparent;
  }
  .game-status {
    display: flex;
    justify-content: center;
    align-items: center;
    flex-flow: row;
    right: -15px;
    top: -10px;
    svg {
      width: 20px;
      height: auto;
    }
    svg.finished {
      * {
        fill: var(--yellow);
      }
    }
    svg.pause {
      * {
        fill: var(--light);
      }
    }
    svg.playing {
      * {
        fill: var(--orange);
      }
    }
    svg.suspende {
      * {
        fill: var(--dark);
      }
    }
  }
  .close-btn {
    padding: 0.5rem;
    border: none;
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
.dark-card {
  background-color: rgba(0, 0, 0, 0.2) !important;
}
.light-card {
  background-color: rgba(0, 0, 0, 0.1) !important;
}
.card-center-start {
  display: flex;
  justify-content: center;
  gap: 0.5rem;
  background-color: transparent;
  border: none;
}
.card-center {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 0.5rem;
  background-color: transparent;
  border: none;
}
</style>