import { io, Socket } from 'socket.io-client';
import { reactive, toRefs, computed, ref } from "vue";
import server from "../../server";
import type { PongGameState } from '@/components/pongTypes'
import type { ApiErrors, Setting } from "@/composables/types";
import { useMe } from "@/composables/user/useMe";
import { useProfileSettings } from '@/composables/profile/useProfileSettings';
import { useRoute, useRouter } from "vue-router";
import { useStatus } from "@/composables/profile/useStatus";
import { parseAxiosError } from "@/composables/axios";
import type { AxiosError } from 'axios';
import { useAxios } from "@/composables/axios"

/******************************/
/*           AXIOS            */
/******************************/
interface Player {
  username42: string;
  avatarImg: string;
  ranking: number;
  firstName: string;
  lastName: string;
}

interface User {
  id?: number;
  intraId?: number;
  email?: string;
  username42?: string;
  firstName?: string;
  lastName?: string;
  birthday?: string;
  nickname?: string;
  avatarImg?: string;
  bio?: string;
  status?: string;
  isTwoFactorAuthenticationEnabled?: boolean;
  createdAt?: string;
  updatedAt?: string;
}

interface Game {
  id?: number
  status?: string
  userOneId?: number
  userTwoId?: number
  scoreOne?: number
  scoreTwo?: number
  isLadder?: boolean
  createdAt?: Date
  userOne?: User
  userTwo?: User
}

const state = reactive<{
  game: Game,
  games: Game[],
  rankings: Player[],
  loading: boolean,
  loadingRankings: boolean,
  errors: ApiErrors | null,
}>({
  game: {},
  games: [],
  rankings: [],
  loading: false,
  loadingRankings: false,
  errors: null,
});

export const useGame = () => {
  const fetchAllGames = async () => {
    state.loading = true;
    /* Check that authenticatedUser42 is in fact authenticated... it's not safe to rely on the localStorage */
    try {
      const { data: gamesData } = await server.get('/pong-game/');
      state.games = gamesData;
    } catch (error) {
      state.errors = { games: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };
  const fetchGameInfo = async (gameId: number) => {
    state.loading = true;
    /* Check that authenticatedUser42 is in fact authenticated... it's not safe to rely on the localStorage */
    try {
      const { data: gameData } = await server.get('/pong-game/' + gameId);
      state.game = gameData;
    } catch (error) {
      state.errors = { game: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };
  const fetchRankings = async () => {
    state.loadingRankings = true;
    try {
      const { data: rankings } = await server.get('/pong-game/rankings');
      state.rankings = rankings;
    } catch (error) {
      state.errors = { game: [parseAxiosError(error as AxiosError)] };
    }
    state.loadingRankings = false;
  }
  return {
    fetchGameInfo,
    fetchAllGames,
    fetchRankings,
    ...toRefs(state)
  };
};

/******************************/
/*         WebSocket         */
/******************************/
const {
  changeStatus,
} = useStatus();

const BACKEND_URL = import.meta.env.VITE_BACKEND_URL.replace(/\/+$/, "");
//const gameId = computed( () => +route.params.gameId);

//const routeName = route.value.name
const pongGameState = ref<PongGameState>({
  puck: {},
  paddleLeft: {},
  paddleRight: {},
  scoreLeft: 0,
  scoreRight: 0,
  frameCount: 0,
  gameStatus: {},
  spectators: 0,
  playerRight: {},
  playerLeft: {},
});

const socketState = reactive<{
  socket: Socket | null
}>({
  socket: null,
})

const { me } = useMe()
const { getToken } = useAxios()

export const useGameSocket = () => {
  const { currentSetting } = useProfileSettings();
  const route = useRoute();
  const router = useRouter()
  const gameId = computed( () => +route.params.gameId);

  const socketCreate = () => {
    if (!socketState.socket && me.value && gameId.value) {
      socketState.socket = io(BACKEND_URL + '/pong-game', {forceNew: true, query: {gameId: gameId.value, username: me.value.username42, token: getToken()}});
      console.log('[Game Socket] game client created');
    }
  }

  const socketDisconnect = () => {
    if (socketState.socket) {
      socketState.socket.disconnect()
      console.log('[Game Socket] game client disconnected')
      socketState.socket = null
      pongGameState.value = {
        puck: {},
        paddleLeft: {},
        paddleRight: {},
        scoreLeft: 0,
        scoreRight: 0,
        frameCount: 0,
        gameStatus: {},
        spectators: 0,
        playerRight: {},
        playerLeft: {},
      };
      changeStatus('online');
    }
  }

  const init = () => {
    if (!socketState.socket) {
      return
    }
    state.loading = true
    socketState.socket.on('gameCreated', (gameId: number) => {
      state.joinedQueue = false;
      router.push({ name: "game", params: { gameId: state.game.id } });
    });
    socketState.socket.on('connect', () => {
      socketState.socket.emit('logUser', {name: me.value.username42});
    });
    socketState.socket.on('logUser', () => {
      socketState.socket.emit('getGameState', {name: me.value.username42});
    });
    socketState.socket.on('getGameState', (state: any ) => {
      if (currentSetting.value === null) {
        if (me.value && me.value.defaultSetting !== null) {
          currentSetting.value = me.value.defaultSetting
        } else {
          currentSetting.value = {
            puck: "#424242",
            paddleR: "#424242",
            paddleL: "#424242",
            bg: "#000000",
          }
        }
      }
      pongGameState.value = {
        puck: {
          ...state.state.pong.puck,
          skin: currentSetting.value.puck
        },
        paddleLeft: {
          ...state.state.pong.paddleLeft,
          skin: currentSetting.value.paddleL
        },
        paddleRight: {
          ...state.state.pong.paddleRight,
          skin: currentSetting.value.paddleR
        },
        background: currentSetting.value.bg,
        scoreLeft: state.state.pong.scoreLeft,
        scoreRight: state.state.pong.scoreRight,
        frameCount: state.state.pong.frameCount,
        gameStatus: state.state.pong.gameStatus,
        spectators: state.state.spectators,
        playerRight: state.state.playerRight,
        playerLeft: state.state.playerLeft,
      };
    });
    state.loading = false

    if (state.game) {
      if (state.game.isLadder) {
        changeStatus('playingLader');
      } else {
        changeStatus('playingFriendly');
      }
    }
  };

  const socketReset = () => {
    socketDisconnect()
    socketCreate()
    init()
  }

  const readyToStart = () => {
    if (socketState.socket) {
      socketState.socket.emit('readyToStart');
    }
  };

  const abandonGame = () => {
    if (socketState.socket) {
      socketState.socket.emit('abandonGame', {username42: me.value.username42});
    }
  };

  const sendKeyPressed = (event) => {
    if (socketState.socket) {
      socketState.socket.emit('keyPressed', {username42: me.value.username42, keyPressed: (event as KeyboardEvent).code});
    }
  };

  const sendkeyReleased = (event) => {
    if (socketState.socket) {
      socketState.socket.emit('keyReleased', {username42: me.value.username42, keyPressed: (event as KeyboardEvent).code});
    }
  };

  return {
    init,
    pongGameState,
    sendkeyReleased,
    sendKeyPressed,
    readyToStart,
    socketCreate,
    socketReset,
    socketDisconnect,
    abandonGame,
    ...toRefs(socketState),
  };
};