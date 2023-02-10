import { io, Socket } from 'socket.io-client';
import { reactive, toRefs } from 'vue';
import { useMe } from "@/composables/user/useMe";
import { useAxios } from "@/composables/axios"

/******************************/
/*         WebSocket         */
/******************************/
const BACKEND_URL = import.meta.env.VITE_BACKEND_URL.replace(/\/+$/, "");

const state = reactive<{
  usersStatus: {
    users?: {
      [username42: string]: string;
    }
  },
  socket: Socket | null,
  loading: boolean,
}>({
  usersStatus: {},
  socket: null,
  loading: false,
})

const { me } = useMe()
const { getToken } = useAxios()

export enum UserStatus {
  ONLINE = 'online',
  OFFLINE = 'offline',
  PLAYINGFRIENDLY = 'playingFriendly',
  PLAYINGLADDER = 'playingLader'
}

export const useStatus = () => {

  const socketCreate = () => {
    if (!state.socket && me.value) {
      state.socket = io(BACKEND_URL + '/status', {forceNew: true, query: {username42: me.value.username42, token: getToken()}});
      console.log('[Status Socket] status client connected');
    }
  }

  const socketDisconnect = () => {
    if (state.socket) {
      state.socket.disconnect()
      console.log('[Status Socket] status client disconnected')
      state.socket = null;
    }
  }

  const init = () => {
    if (!state.socket) {
      return
    }
    state.socket.on('connect', () => {
      state.socket.emit('logUser', {name: me.value.username42});
    });

    state.socket.on('usersStatus', (users: any ) => {
      state.usersStatus = users;
    });
    state.loading = false
  };

  const socketReset = () => {
    socketDisconnect()
    socketCreate()
    init()
  }

  const changeStatus = (status: UserStatus) => {
    if (!state.socket) {
      return
    }
    state.socket.emit('changeStatus', {status: status});
  }

  return {
    init,
    socketCreate,
    socketReset,
    socketDisconnect,
    changeStatus,
    ...toRefs(state),
  };
};