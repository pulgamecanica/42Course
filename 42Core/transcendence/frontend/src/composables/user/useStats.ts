import { reactive, toRefs } from "vue";
import server from "@/server";
import type { ApiErrors } from "@/composables/types";
import { parseAxiosError } from "@/composables/axios";
import type { AxiosError } from 'axios';

export interface Stats {
  finished: number,
  lost: number,
  suspended: number,
  totalGames: number,
  won: number,
  rank: number,
  ranking: number,
};

export interface SimpleStats {
  rank: number,
  ranking: number,
};

const state = reactive<{
  userStats: Stats | SimpleStats | null;
  loading: boolean;
  errors: ApiErrors | null;
}>({
  userStats: null,
  loading: false,
  errors: null,
});

export const useStats = () => {

  const fetchStats = async (username42: string | null, simple: boolean) => {
    state.loading = true;
    if (username42 === null) {
      username42 = "me";
    }
    try {
      const { data } = await server.get<Stats>(`/users/${username42}/pong-stats?simple=${simple}`);
      state.userStats = data;
    } catch (error) {
      state.errors = { stats: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };

  return {
    fetchStats,
    ...toRefs(state)
  }
};
