<template>
  <div>
    <div id="coolIntraBackground"></div>
    <el-row v-if="!imLoading" class="main-container" :gutter="20">
      <el-col :xs="24" :sm="12" :md="12" class="col-padding center-all">
        <ProfileAvatar
          v-if="!profileLoading"
          :profile="user"
          :is-me="userIsMe"
        />
      </el-col>
      <el-col :xs="24" :sm="12" :md="12" class="col-padding">
        <ProfileDetails
          v-if="!profileLoading"
          :profile="user"
          :is-me="userIsMe"
        />
      </el-col>
      <el-col
        v-if="!profileLoading"
        :xs="24"
        :sm="12"
        :md="8"
        class="animate__animated animate__bounceInDown col-padding"
      >
        <el-card class="full-height">
          <p>Achievements ({{ achievements.length }})</p>
          <span v-if="achievements.length">
            <el-tooltip
              v-for="achievement in achievements"
              :key="achievement"
              effect="dark"
              :content="achievement.description"
              placement="top-start"
            >
              <el-button style="min-height: 75px; margin: 1rem">
                <el-avatar
                  :src="achievement.imgUrl"
                  :size="50"
                  class="animate__animated animate__headShake"
                  style="margin: 0.25rem"
                />
                <el-link>{{ achievement.title }}</el-link>
              </el-button>
            </el-tooltip>
          </span>
          <el-empty v-else description="No achievements yet. Go conquer!" :image="randomRickAndMortyImg()" />
        </el-card>
      </el-col>
      <el-col
        v-if="!profileLoading"
        :xs="24"
        :sm="12"
        :md="8"
        class="animate__animated animate__bounceInDown col-padding"
      >
        <el-card class="full-height">
          <el-tabs v-model="activeTab">
            <el-tab-pane label="Stats" name="first">
              <el-card class="no-border full-height" v-if="!statsLoading">
                <div class="space-around">
                  <el-tooltip
                    class="box-item"
                    effect="dark"
                    :content="rankMessage"
                    placement="top"
                  >
                    <h1>
                      {{ (userStats as Stats).rank !== null ? `#${(userStats as Stats).rank}` : '🤷‍♂️' }}
                    </h1>
                  </el-tooltip>

                  <el-tooltip
                    class="box-item"
                    effect="dark"
                    content="Your ranking."
                    placement="top"
                  >
                    <h1>
                      {{ (userStats as Stats).ranking ? (userStats as Stats).ranking : 0 }}
                    </h1>
                  </el-tooltip>
                </div>

                <el-divider></el-divider>
                <h2>{{ ratio }}</h2>
                <el-tooltip
                  class="box-item"
                  effect="dark"
                  content="Win / Loss Ratio"
                  placement="top"
                >
                  <span>w/l ratio</span>
                </el-tooltip>
                <el-divider></el-divider>
                <el-row>
                  <el-col :span="12">
                    <el-tooltip
                      class="box-item"
                      effect="dark"
                      content="Wins"
                      placement="top"
                    >
                      <h2>🏆 {{ (userStats as Stats).won }}</h2>
                    </el-tooltip>
                  </el-col>
                  <el-col :span="12">
                    <el-tooltip
                      class="box-item"
                      effect="dark"
                      content="Losses"
                      placement="top"
                    >
                      <h2>❌ {{ (userStats as Stats).lost }}</h2>
                    </el-tooltip>
                  </el-col>
                </el-row>
              </el-card>
            </el-tab-pane>
            <el-tab-pane label="Match History" name="second">
              <el-card class="no-border full-height limit-height">
                <el-container v-if="matches?.length > 0" class="flex-col">
                  <el-container
                    class="result-card el-card btn el-row full-width"
                    v-for="match in matches"
                    :key="match"
                  >
                    <RouterLink
                      :to="{ name: 'game', params: { gameId: match.id }}"
                      class="result-card el-card btn el-row full-width"
                      style="margin: 0;"
                    >
                      <button class="game-status">
                        <svg
                          class="pause"
                          viewBox="0 0 512 512"
                          xml:space="preserve"
                          v-if="match.status === 'suspended'"
                        >
                          <g>
                            <path
                              class="st0"
                              d="M256,0C114.625,0,0,114.625,0,256c0,141.374,114.625,256,256,256s256-114.626,256-256
                        C512,114.625,397.375,0,256,0z M224,336h-64V176h64V336z M352,336h-64V176h64V336z"
                            />
                          </g>
                        </svg>
                        <svg
                          class="playing"
                          v-if="
                            match.status === 'playing' ||
                            match.status === 'starting'
                          "
                          viewBox="0 0 24 24"
                        >
                          <path
                            d="M21.261,2.739A9.836,9.836,0,0,0,8.3,2.114,4.489,4.489,0,1,0,4.134,8.963a9.415,9.415,0,0,0,.842,5.668.5.5,0,0,1-.07.564L2,16.945A3.743,3.743,0,0,0,3.735,24a3.891,3.891,0,0,0,.457-.027,3.705,3.705,0,0,0,2.725-1.735l2.068-3.127a.5.5,0,0,1,.575-.089,9.663,9.663,0,0,0,11.315-2.147A10.5,10.5,0,0,0,24,9.758,9.409,9.409,0,0,0,21.261,2.739ZM2,4.5A2.5,2.5,0,1,1,4.5,7,2.5,2.5,0,0,1,2,4.5Zm8.44,12.726a2.494,2.494,0,0,0-3.017.632c-.024.029-.046.059-.067.09L5.229,21.166A1.742,1.742,0,0,1,2.02,20a1.76,1.76,0,0,1,.961-1.312l3.041-1.831a.956.956,0,0,0,.126-.09,2.49,2.49,0,0,0,.623-3.016,7.331,7.331,0,0,1-.693-4.259l8.433,8.433A7.31,7.31,0,0,1,10.44,17.226Zm9.021-1.765a8.871,8.871,0,0,1-2.732,1.865c-.009-.01-.012-.023-.022-.033L7.36,7.945A4.473,4.473,0,0,0,9,4.5c0-.119-.026-.231-.035-.347a8.01,8.01,0,0,1,10.882,0A7.423,7.423,0,0,1,22,9.7,8.506,8.506,0,0,1,19.461,15.461Z"
                          />
                        </svg>
                        <svg
                          class="finished"
                          viewBox="0 0 512 512"
                          xml:space="preserve"
                          v-if="match.status === 'finished'"
                        >
                          <g>
                            <path
                              class="st0"
                              d="M493.379,55.416c-11.458-11.479-27.441-18.63-44.952-18.62h-29.049h-14.736H107.358H92.632H63.573
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
                        c6.191,6.21,9.988,14.647,9.998,24.115V133.665z"
                            />
                          </g>
                        </svg>
                      </button>
                      <el-row class="dark-card full-width bottom-border">
                        <el-col style="padding-top: 5px;">
                          <span>{{
                            match.createdAt
                              .replace("T", " ")
                              .substr(0, match.createdAt.lastIndexOf(":"))
                          }}</span>
                        </el-col>
                      </el-row>
                      <el-row class="dark-card full-width">
                        <el-col class="card-center" style="height: 50px" :md="10">
                          <el-tooltip
                            :content="`Score: ${match.scoreOne}`"
                            placement="top"
                          >
                            <el-avatar
                              class="
                                cursor-pointer
                                animate__animated animate__headShake
                              "
                              :src="match.userOne.avatarImg"
                              :size="36"
                              @click="
                                router.push(
                                  `/profile/${match.userOne.username42}`
                                )
                              "
                            />
                          </el-tooltip>
                        </el-col>
                        <el-col class="card-center light-card" :md="4">
                          <h4>vs</h4>
                        </el-col>
                        <el-col class="card-center" style="height: 50px" :md="10">
                          <el-tooltip
                            :content="`Score: ${match.scoreTwo}`"
                            placement="top"
                          >
                            <el-avatar
                              class="
                                cursor-pointer
                                animate__animated animate__headShake
                              "
                              :src="match.userTwo.avatarImg"
                              :size="36"
                              @click="
                                router.push(
                                  `/profile/${match.userTwo.username42}`
                                )
                              "
                            />
                          </el-tooltip>
                        </el-col>
                      </el-row>
                    </RouterLink>
                  </el-container>
                </el-container>
                <el-empty
                  v-else
                  description="No matches in the history. Play your first game to see it here!"
                  :image="randomRickAndMortyImg()"
                />
              </el-card>
            </el-tab-pane>
          </el-tabs>
        </el-card>
      </el-col>
      <el-col
        :xs="24"
        :sm="24"
        :md="8"
        class="animate__animated animate__bounceInDown col-padding"
      >
        <el-card style="width: 100%" class="full-height">
          <p>Friends ({{ friends.length }})</p>
          <el-container
            v-if="
              friends?.length > 0 ||
              (userIsMe &&
                (friendRequestsRecieved?.length > 0 ||
                  friendRequestsSent?.length > 0 ||
                  blockedUsers?.length > 0))
            "
            class="flex-col"
          >
            <el-badge
              style="display: block; margin: auto; width: fit-content"
              v-for="friend in friends"
              :key="friend"
              value=" "
              class="item"
              :type="
                usersStatus.users &&
                friend.username42 &&
                usersStatus.users[friend.username42] === 'offline'
                  ? 'danger'
                  : 'success'
              "
            >
              <el-button
                style="border: none"
                @click="changeProfile(friend.username42)"
              >
                <el-avatar
                  :src="friend.avatarImg"
                  :size="20"
                  class="animate__animated animate__headShake"
                  style="margin: 0.25rem"
                />
                {{ friend.username42 }}
              </el-button>
            </el-badge>
            <span v-if="userIsMe && !relationsLoading && !profileLoading" class="full-width">
              <span v-if="friendRequestsRecieved.length">
                <p>Friend Requests ({{ friendRequestsRecieved.length }})</p>
                <p
                  style="
                    display: block;
                    margin: 0.25rem auto;
                    width: fit-content;
                  "
                  v-for="sender in friendRequestsRecieved"
                  :key="sender.id"
                >
                  <el-button
                    style="border: none"
                    @click="changeProfile(sender.username42)"
                  >
                    <el-avatar
                      :src="sender.avatarImg"
                      :size="20"
                      class="animate__animated animate__headShake"
                      style="margin: 0.25rem"
                    />
                    {{ sender.username42 }}
                  </el-button>
                  <el-button
                    style="border: none"
                    type="success"
                    @click="fetchRelation(Relation.Accept, sender.username42)"
                  >
                    Accept
                  </el-button>
                  <el-button
                    style="border: none"
                    type="danger"
                    @click="fetchRelation(Relation.Reject, sender.username42)"
                  >
                    Reject
                  </el-button>
                </p>
              </span>
              <span v-if="friendRequestsSent.length">
                <p>Pending Requests ({{ friendRequestsSent.length }})</p>
                <p
                  style="
                    display: block;
                    margin: 0.25rem auto;
                    width: fit-content;
                  "
                  v-for="reciever in friendRequestsSent"
                  :key="reciever.id"
                >
                  <el-button
                    style="border: none"
                    @click="changeProfile(reciever.username42)"
                  >
                    <el-avatar
                      :src="reciever.avatarImg"
                      :size="20"
                      class="animate__animated animate__headShake"
                      style="margin: 0.25rem"
                    />
                    {{ reciever.username42 }}
                  </el-button>
                  <el-button
                    style="border: none"
                    type="danger"
                    @click="fetchRelation(Relation.Reject, reciever.username42)"
                  >
                    Unsend
                  </el-button>
                </p>
              </span>
              <span v-if="blockedUsers.length">
                <p>Blocked Users ({{ blockedUsers.length }})</p>
                <p
                  style="
                    display: block;
                    margin: 0.25rem auto;
                    width: fit-content;
                  "
                  v-for="blockedUser in blockedUsers"
                  :key="blockedUser.id"
                >
                  <el-button>{{ blockedUser.username42 }}</el-button>
                  <el-button
                    type="danger"
                    @click="
                      fetchRelation(Relation.Unblock, blockedUser.username42)
                    "
                  >
                    Unblock
                  </el-button>
                </p>
              </span>
            </span>
          </el-container>
          <el-empty
            v-else
            description="No friend activity yet. Meet someone to start!"
            :image="randomRickAndMortyImg()"
          />
        </el-card>
      </el-col>
    </el-row>
  </div>
</template>

<script setup lang="ts">
import ProfileDetails from "@/components/Profile/ProfileDetails.vue";
import ProfileAvatar from "@/components/Profile/ProfileAvatar.vue";
import { computed, watch, ref } from "vue";
import { useRoute, useRouter } from "vue-router";
import { useMe } from "@/composables/user/useMe";
import { useProfile } from "@/composables/profile/useProfile";
import { useRelations } from "@/composables/relations/useRelations";
import { useStatus } from "@/composables/profile/useStatus";
import { useStats } from "@/composables/user/useStats";
import type { Stats } from "@/composables/user/useStats";
import { randomRickAndMortyImg } from "@/composables/images.ts";
import { Relation } from "@/components/Profile/types";

/******************************/
/*   User Status Web Socket   */
/******************************/

const { usersStatus } = useStatus();

/*
  * The following Arrays will only be fetched from the API when the username42 === route.params.username42 (my own  profile)
  * - friendRequestsSent
  * - friendRequestsRecieved
  * - blockedUsers
  */
const route = useRoute();
const router = useRouter();
const { me, loading: imLoading } = useMe();
const myUsername = computed(() => me.value!.username42);
const username = computed(() => route.params.username42 as string);
const {
  user, // maybe move this to useUser, now that useMe subbed it
  friends,
  achievements,
  friendRequestsSent,
  friendRequestsRecieved,
  blockedUsers,
  matches,
  fetchProfile,
  loading: profileLoading,
} = useProfile();

const { fetchStats, userStats, loading: statsLoading } = useStats();
fetchStats(username.value + "", false);

const ratio = computed(() => {
  if (!userStats.value) {
    return;
  }

  const stats = userStats.value as Stats;
  if (stats.lost) {
    return Math.round(stats.won / stats.lost);
  } else {
    return stats.won;
  }
});

const rankMessage = computed(() => {
  if (userStats.value?.rank === null) {
    return userIsMe.value
      ? "Play a ladder game to get ranked!"
      : "No ladder games played yet.";
  } else {
    return `${userIsMe.value ? "You're ranked " : ""}#${
      userStats.value?.rank
    } in the world!`;
  }
});

if (username.value) {
  fetchProfile(username.value);
}

watch(imLoading, () => {
  if (username.value) {
    fetchProfile(username.value);
  }
});

const userIsMe = computed(() => username.value === myUsername.value);

const activeTab = ref("first");

/* Setup the relations options for your Friends Profile */
const {
  blockUser,
  sendFriendRequest,
  acceptFriendRequest,
  rejectFriendRequest,
  removeFriend,
  unblockUser,
  loading: relationsLoading,
} = useRelations();

async function changeProfile(username42: string | undefined) {
  if (!username42) {
    return;
  }
  user.value = {};
  router.push({ name: "profile", params: { username42: username42 } });
}

async function fetchRelation(
  relation: Relation,
  username42: string = username.value + ""
) {
  switch (relation) {
    case Relation.Send:
      await sendFriendRequest(username42);
      break;
    case Relation.Accept:
      await acceptFriendRequest(username42);
      break;
    case Relation.Reject:
      await rejectFriendRequest(username42);
      break;
    case Relation.Remove:
      await removeFriend(username42);
      break;
    case Relation.Unblock:
      await unblockUser(username42);
      break;
    case Relation.Block:
      await blockUser(username42);
      router.push({ name: "home" });
      break;
  }
  if (relation !== Relation.Block && myUsername.value) {
    await fetchProfile(myUsername.value);
  }
}
</script>

<style scoped lang="scss">
#coolIntraBackground {
  background-image: url("https://cdn.intra.42.fr/coalition/cover/109/assembly_background.jpg");
  background-position: center;
  background-repeat: no-repeat;
  background-size: cover;
  display: block;
  position: fixed;
  opacity: 0.15;
  width: 100vw;
  height: 100vh;
  overflow: hidden;
  margin: 0;
  padding: 0;
}

.main-container {
  height: 100vh;
  width: 100%;
  margin: auto !important;
  /*padding-top: 0.5rem;*/
  padding: 15px;
}

.col-padding {
  padding: 10px;
}

.full-height {
  height: 100%;
}

.button-container {
  display: inline;
  flex: none;
}

.main-button-container {
  column-gap: 10px;
}

.space-around {
  display: flex;
  justify-content: space-around;
  align-items: center;
}

.result-card {
  overflow: inherit;
  margin: 1rem auto;
  position: relative;
  .el-button {
    background-color: transparent;
    border: none;
  }
}

.game-status {
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

.dark-card {
  background-color: rgba(0, 0, 0, 0.2) !important;
}

.light-card {
  background-color: rgba(255, 255, 255, 0.1) !important;
}

.card-center {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 0.5rem;
  background-color: transparent;
  border: none;
}

.full-width {
  width: 100%;
}

.bottom-border {
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
}

.limit-height {
  max-height: 50vh;
  overflow-y: scroll;
}

.no-border {
  border: 0;
}

.cursor-pointer {
  cursor: pointer;
}

.flex-col {
  flex-direction: column;
}
</style>
