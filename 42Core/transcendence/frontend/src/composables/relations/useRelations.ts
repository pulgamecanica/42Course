import { reactive, toRefs } from "vue";
import server from "@/server";
import { parseAxiosError } from "@/composables/axios";
import type { AxiosError } from 'axios';
import type { ApiErrors } from "@/composables/types";

const state = reactive<{
  status: number,
  loading: boolean,
  errors: ApiErrors | null;
}>({
  status: 0,
  loading: false,
  errors: null
});

export const useRelations = ( ) => {
  // Post friend-relation/:userName42/block
  const blockUser = async (username42: string) => {
    state.loading = true;
    try {
      const { status } = await server.post('friend-relation/' + username42 + '/block');
      state.status = status;
    } catch (error) {
      state.errors = { block: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };

  // Post /friend-relation/:userName42/send-friend-request
  const sendFriendRequest = async (username42: string) => {
    state.loading = true;
    try {
      const { status } = await server.post('/friend-relation/' + username42 + '/send-friend-request');
      state.status = status;
    } catch (error) {
      state.errors = { send: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };

  // Patch /friend-relation/:userName42/accept-friend-request
  const acceptFriendRequest = async (username42: string) => {
    state.loading = true;
    try {
      const { status } = await server.patch('/friend-relation/' + username42 + '/accept-friend-request');
      state.status = status;
    } catch (error) {
      state.errors = { accept: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };

  // Patch /friend-relation/:userName42/reject-friend-request
  const rejectFriendRequest = async (username42: string) => {
    state.loading = true;
    try {
      const { status } = await server.patch('/friend-relation/' + username42 + '/reject-friend-request ');
      state.status = status;
    } catch (error) {
      state.errors = { reject: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };

  // Delete /friend-relation/:userName42/remove-friend
  const removeFriend = async (username42: string) => {
    state.loading = true;
    try {
      const { status } = await server.delete('/friend-relation/' + username42 + '/remove-friend');
      state.status = status;
    } catch (error) {
      state.errors = { remove: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };

  // Delete /friend-relation/:userName42/unblock
  const unblockUser = async (username42: string) => {
    state.loading = true;
    try {
      const { status } = await server.delete('/friend-relation/' + username42 + '/unblock');
      state.status = status;
    } catch (error) {
      state.errors = { unblock: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };

  return {
    blockUser,
    sendFriendRequest,
    acceptFriendRequest,
    rejectFriendRequest,
    removeFriend,
    unblockUser,
    ...toRefs(state)
  };
};
