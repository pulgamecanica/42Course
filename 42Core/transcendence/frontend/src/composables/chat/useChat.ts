import { Socket, io } from 'socket.io-client';
import { ref, reactive, toRefs, computed, nextTick } from 'vue';
import type {
  AbstractChatRoom, ChatRoom, ChatUser, ClientMessage, ClientTyping,
  Message, RoomStruct,
} from "@/composables/chat/types";
import { MessagePrivilege, MessageType, ChatRoomStatus } from "@/composables/chat/types";
import type { ApiErrors } from "@/composables/types";
import { useMe } from "@/composables/user/useMe";
import { useAxios } from "@/composables/axios"

// debug
const c = console.log;
const log = (msg: any) => {
  c(msg);
}

const BACKEND_URL = import.meta.env.VITE_BACKEND_URL.replace(/\/+$/, "");

type ChatState = {
  socket: Socket | null;
  user: ChatUser | null;
  currentRoomName: string | null;
  rooms: {
    [name: string]: AbstractChatRoom;
  };
  publicRooms: string[];
  typing: boolean;
  loading: boolean;
  errors: ApiErrors | null;
}

const state = reactive<ChatState>({
  socket: null,
  user: null,
  currentRoomName: null,
  rooms: {},
  publicRooms: [],
  typing: false,
  loading: false,
  errors: null,
});

const messagesRef = ref<{ [name: string ]: Element }>({})

const scrollToBottom = () => {
  nextTick(() => {
    if (state.currentRoomName && messagesRef.value[state.currentRoomName]) {
      const $el = messagesRef.value[state.currentRoomName];
      $el.scrollTop = $el.scrollHeight;
    }
  })
}

const { me } = useMe()
const { getToken } = useAxios()

const currentRoom = computed(() => {
  if (state.currentRoomName) {
    return state.rooms[state.currentRoomName]
  }
  return null;
})

// Helpers
const isDirectMessage = (room: AbstractChatRoom) => room?.status === ChatRoomStatus.DirectMessage

export const useChat = () => {

  const socketCreate = () => {
    if (!state.socket && me.value) {
      state.socket = io(BACKEND_URL + '/chat', { query: { username: me.value.username42, token: getToken() }});
      c('[Socket] chat client created')
    }
  }

  const socketDisconnect = () => {
    if (state.socket) {
      state.socket.disconnect()
      c('[Socket] chat client disconnected')
      clear()
    }
  }

  const socketInit = () => {
    if (!state.socket) {
      return
    }
    state.loading = true
    const s = state.socket;
    s.on('connect', onConnect)
    s.on('disconnect', onDisconnect)
    s.on('joinedRoom', onJoinedRoom)
    s.on('leftRoom', onLeftRoom)
    s.on('joinedRoomDirectMessage', onJoinedRoom)
    s.on('leftRoomDirectMessage', onLeftRoom)
    s.on('msgToClient', onMsgToClient)
    s.on('getPublicRooms', onGetPublicRooms)
    s.on('getChatUser', onGetChatUser)
    s.on('chatRoomUpdate', onChatRoomUpdate)
    s.on('typing', onTyping)
    state.loading = false
  }

  const socketReset = () => {
    socketDisconnect()
    socketCreate()
    socketInit()
  }

  // Helpers
  const clear = () => {
    state.rooms = {}
    state.socket = null
  }

  // Socket On callbacks
  const onConnect = () => {
    c('[Socket] chat server connected')
  }

  const onDisconnect = () => {
    c('[Socket] chat server disconnected')
    if (state.currentRoomName && state.rooms[state.currentRoomName]) {
      const c = currentRoom.value as ChatRoom

      state.rooms[state.currentRoomName].messagesObj.push({
        typeOfRoom: c.status,
        theDate: '',
        theTime: '',
        name: '',
        fromRoom: c.roomName,
        privilege: MessagePrivilege.User,
        text: 'You\'ve been disconnected from the server',
        typeOfMessage: MessageType.Disconnected,
      })
    }
  }

  const onJoinedRoom = (room: AbstractChatRoom) => {
    state.loading = true;
    if (!state.rooms[room.roomName]) {
      state.rooms[room.roomName] = room;
    }
    state.loading = false;
    // maybe find way to not go to tab only if got msg invite
    if (!isDirectMessage(room)) {
      state.currentRoomName = room.roomName;
      scrollToBottom()
    }
  }

  const onLeftRoom = (name: string) => {
    state.loading = true;
    const roomNames = Object.keys(state.rooms)
    const index = roomNames.findIndex(roomName => roomName === name)

    if (index !== -1) {
      if (roomNames[index - 1]) {
        state.currentRoomName = roomNames[index - 1];
      } else if (roomNames[roomNames.length - 1]) {
        state.currentRoomName = roomNames[roomNames.length - 1];
      }
      delete state.rooms[name];
    }
    state.loading = false;
  }

  const onMsgToClient = (message: Message) => {
    // In case there is no roomName (it's to send to the active room)
    let roomName = message.fromRoom
    if (!roomName && state.currentRoomName) {
      roomName = state.currentRoomName;
    }
    state.rooms[roomName]?.messagesObj.push(message);
    if (state.currentRoomName === roomName) {
      scrollToBottom();
    }
  }

  const onGetPublicRooms = (rooms: string[]) => state.publicRooms = rooms

  const onGetChatUser = (user: ChatUser) => {
    state.user = user;
    scrollToBottom(); // to show unblocked messages
  }

  const onChatRoomUpdate = (newRoom: ChatRoom) => {
    if (!state.rooms[newRoom.roomName]) {
      return
    }
    // state.rooms[newRoom.roomName] = newRoom
    const room = state.rooms[newRoom.roomName] as ChatRoom
    state.rooms[newRoom.roomName] = {
      ...newRoom,
      messagesObj: room.messagesObj,
    }
  }

  const onTyping = (info: ClientTyping) => {
    state.typing = info.isTyping && state.currentRoomName === info.fromRoom
  }

  // Socket Emits
  const emitTyping = (info: ClientTyping) => state.socket?.emit('typing', info)
  const emitMessage = (message: ClientMessage) => state.socket?.emit('msgFromClient', message)

  const _sendMessage = (message: string, type?: MessageType) => {
    if (message === '' || !currentRoom.value || !me.value) {
      return ;
    }
    const c = currentRoom.value
    emitMessage({
      roomName: c.roomName,
      name: me.value.username42,
      text: message,
      typeOfRoom: c.status,
      ...(type ? { typeOfMessage: type } : {}),
    })
  }

  const sendMessage = (message: string) => _sendMessage(message)
  const sendCommand = (message: string) => _sendMessage(message, MessageType.Command)

  const createRoom = ({ status, name, password }: RoomStruct) => {
    sendCommand(`/create ${status} ${name.trim()} ${password || ''}`.trim())
  }

  const joinRoom = ({ name, password }: RoomStruct) => {
    sendCommand(`/join ${name} ${password || ''}`.trim())
  }

  const leaveRoom = () => sendCommand(`/leave`)
  const directMessageUser = (name: string) => sendCommand(`/msg ${name}`)
  const toggleDate = () => sendCommand('/showDate')
  const toggleTime = () => sendCommand('/showTime')
  const setRoomPassword = (password: string) => sendCommand(`/password ${password}`.trim())

  const inviteChatGame = (name: string) => {
    if (!currentRoom.value) {
      return
    }
    if (isDirectMessage(currentRoom.value)) {
      state.currentRoomName = 'lounge';
    }
    sendCommand(`/invite ${name}`.trim())
  }

  return {
    socketInit,
    socketCreate,
    socketDisconnect,
    socketReset,
    clear,
    emitMessage,
    emitTyping,
    currentRoom,
    messagesRef,
    scrollToBottom,
    sendMessage,
    createRoom,
    joinRoom,
    leaveRoom,
    directMessageUser,
    isDirectMessage,
    toggleDate,
    toggleTime,
    setRoomPassword,
    sendCommand,
    inviteChatGame,
    ...toRefs(state)
  };
};
