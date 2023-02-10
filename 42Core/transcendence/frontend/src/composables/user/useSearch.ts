import { reactive, toRefs } from "vue";
import server from "@/server";
import type { ApiErrors } from "@/composables/types";
import { parseAxiosError } from "@/composables/axios";
import type { AxiosError } from 'axios';

interface User {
  id: number;
  intraId: number;
  email: string;
  username42: string;
  firstName: string;
  lastName: string;
  avatarImg: string;
  status: string;
}

const state = reactive<{
  users: User[];
  loading: boolean;
  errors: ApiErrors | null;
}>({
  users: [],
  loading: false,
  errors: null,
});

export const useSearch = () => {

  const searchUsers = async (searchQuery: string) => {
    state.loading = true;
    try {
      if (searchQuery !== "") {
        const { data } = await server.get<User[]>('/users/searchUsers', { params: { search: searchQuery } });
        state.users = data;
      } else {
        const { data } = await server.get<User[]>('/users/searchUsers');
        state.users = data;
      }
    } catch (error) {
      state.errors = { search: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  }

  return {
    searchUsers,
    ...toRefs(state)
  };
};
