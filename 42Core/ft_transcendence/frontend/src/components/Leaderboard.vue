<template>
  <el-col :span="8">
    <div class="leaderboard-grid__item leaderboard-grid__item--sticky leaderboard-scroll">
      <div class="c-card">
        <div class="c-card__body">
          <div class="u-display--flex u-justify--space-between">
            <div class="u-text--left">
              <div class="u-text--small u-mt--8 u-pb--8">My Rank</div>
              <h1>{{ userStats?.rank !== null ? userStats?.rank : '🤷‍♂️' }}</h1>
            </div>
            <div class="u-text--right">
              <div class="u-text--small u-mt--8 u-pb--8">My Ranking</div>
              <h1>{{ userStats?.ranking }}</h1>
            </div>
          </div>
        </div>
      </div>
      <div v-if="!imLoading" class="c-card" >
        <div class="c-card__body">
          <div class="u-text--center">
            <el-avatar :size="100" :src="me!.avatarImg" />
            <h3 class="u-mt--16">{{ me?.nickname ? me.nickname : `${me?.firstName} ${me?.lastName}` }}</h3>
            <router-link
              class="c-media__link u-text--big"
              :to="{ name: 'profile', params: { username42: me!.username42 }}"
            >
              @{{ me!.username42 }}
            </router-link>
          </div>
        </div>
      </div>
    </div>
  </el-col>
  <el-col :span="16">
    <div class="leaderboard-grid__item leaderboard-grid__players">
      <div class="c-card">
        <div class="c-card__body">
          <ul class="c-list" v-if="players.length">
            <li
              v-for="player, i in players"
              :key="player.username42"
              class="c-list__item"
            >
              <div class="c-list__leaderboard leaderboard-row">
                <div class="c-flag c-place u-bg--transparent" :class="rankClass(i)">{{ i + 1 }}</div>
                <div class="c-media">
                  <el-avatar :size="60" :src="player.avatarImg" />
                  <div class="c-media__content">
                    <div class="c-media__title">{{ player.nickname ? player.nickname : `${player.firstName} ${player.lastName}` }}</div>
                    <router-link
                      class="c-media__link u-text--small"
                      :to="{ name: 'profile', params: { username42: player.username42 }}"
                    >
                      @{{ player.username42 }}
                    </router-link>
                  </div>
                </div>
                <div class="u-text--right c-kudos">
                  <div class="u-mt--8">
                    <h1><strong>{{ player.ranking }}</strong></h1>
                  </div>
                </div>
              </div>
            </li>
          </ul>
          <el-empty v-else description="There are no ranked players yet! Play a game to be the first!" :image="randomRickAndMortyImg()" />
        </div>
      </div>
    </div>
  </el-col>
  <!-- <div v-else class="leaderboard__empty-message">No data available</div> -->
</template>

<script setup lang="ts">
import { ref, computed} from 'vue';
import { useMe } from '@/composables/user/useMe';
import { useStats } from "@/composables/user/useStats";
import { useGame } from '@/composables/game/useGame';
import { randomRickAndMortyImg } from "@/composables/images.ts";

const { me, loading: imLoading } = useMe();
const {
  loadingRankings,
  rankings: players,
  fetchRankings,
} = useGame();
fetchRankings();

const { fetchStats, userStats } = useStats();
fetchStats(null, true);

interface Player {
  username42: string;
  avatarImg: string;
  ranking: number;
  firstName: string;
  lastName: string;
}

const rankClass = (rank: number) => {
  if (rank === 0) return 'u-text--dark u-bg--gold';
  if (rank === 1) return 'u-text--dark u-bg--silver';
  if (rank === 2) return 'u-text--dark u-bg--bronze';
  return '';
}
</script>