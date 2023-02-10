import { io } from 'socket.io-client';
import {
  ref, reactive, toRefs, computed,
} from 'vue';
import type { ApiErrors } from "@/composables/types";
import { useMe } from "@/composables/user/useMe";
import { useNavbar } from "@/composables/navbar/useNavbar";
import { useRouter } from "vue-router";

const BACKEND_URL = import.meta.env.VITE_BACKEND_URL.replace(/\/+$/, "");

type QueueState = {
  joinedQueue: boolean;
  loading: boolean;
  errors: ApiErrors | null;
}

const state = reactive<QueueState>({
  joinedQueue: false,
  loading: false,
  errors: null,
});

const { me } = useMe()
const { closeQueue } = useNavbar()

const socket = computed(() => {
  if (me.value) {
    return io(BACKEND_URL + '/pong-queue', { query: { username: me.value.username42 } });
  }
  return null;
})

export const useQueue = () => {
  const router = useRouter();
  const init = () => {
    if (!socket.value) {
      return
    }
    state.loading = true
    socket.value.on('gameCreated', (gameId: number) => {
      closeQueue()
      state.joinedQueue = false;
      router.push({ name: "game", params: { gameId: gameId } });
    });
    state.loading = false
  }

  const joinQueue = () => {
    state.joinedQueue = true;
    setTimeout(() => {
      socket.value?.emit('joinQueue');
    }, 1500);
  }

  const abandonQueue = () => {
    state.joinedQueue = false;
    socket.value?.emit('abandonQueue');
  }

  return {
    socket,
    init,
    joinQueue,
    abandonQueue,
    ...toRefs(state)
  };
};
