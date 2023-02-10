<template>
  <div class="chat">
    <div class="chat-container">

      <div class="messages-chatRoomData">
        <div v-if="activeRoom">
          <div v-for="chatRoom in activeRoom.messagesObj" v-bind:key="chatRoom">

            <div v-if="chatRoom.typeOfMessage === TypeOfMessage.welcome">
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="welcome-color"> {{ chatRoom.text }}</span>
            </div>
            <div v-else-if="chatRoom.typeOfMessage === TypeOfMessage.connected">
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="connected-color">* {{ chatRoom.name }} {{ chatRoom.text }} </span>
            </div>
            <div v-else-if="chatRoom.typeOfMessage === TypeOfMessage.disconnected">
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="disconnected-color">* {{ chatRoom.name }} {{ chatRoom.text }} </span>
            </div>
            <div v-else-if="chatRoom.typeOfMessage === TypeOfMessage.joined">
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="joined-color">* {{ chatRoom.name }} {{ chatRoom.text }} </span>
            </div>
            <div v-else-if="chatRoom.typeOfMessage === TypeOfMessage.left">
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="left-color">* {{ chatRoom.name }} {{ chatRoom.text }} </span>
            </div>
            <div v-else-if="chatRoom.typeOfMessage === TypeOfMessage.kicked">
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="kicked-color">* {{ chatRoom.name }} {{ chatRoom.text }} </span>
            </div>
            <div v-else-if="chatRoom.typeOfMessage === TypeOfMessage.muted">
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="muted-color">* {{ chatRoom.name }} {{ chatRoom.text }} </span>
            </div>
            <div v-else-if="chatRoom.typeOfMessage === TypeOfMessage.banned">
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="banned-color">* {{ chatRoom.name }} {{ chatRoom.text }} </span>
            </div>
            <div v-else-if="chatRoom.typeOfMessage === TypeOfMessage.command">
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="command-color"> {{ chatRoom.name }} {{ chatRoom.text }} </span>
            </div>
            <div v-else-if="chatRoom.typeOfMessage === TypeOfMessage.info">
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="info-color">* {{ chatRoom.name }} {{ chatRoom.text }} </span>
            </div>
            <div v-else-if="chatUserData?.blockedUsers.includes(chatRoom.name)">
               <!-- This one is left blank to hide blocked users messages-->
            </div>
            <div v-else-if="activeRoom.status !== ChatRoomEnum.directMessage">
              <div v-if="chatRoom.privilege === MsgDtoPrivilege.owner">
                {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="chatColor-color"> &lt;@{{ chatRoom.name }}> {{ chatRoom.text }} </span>
              </div>
              <div v-else-if="chatRoom.privilege === MsgDtoPrivilege.admin">
                {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="chatColor-color"> &lt;+{{ chatRoom.name }}> {{ chatRoom.text }} </span>
              </div>
              <div v-else>
                {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="chatColor-color"> &lt;{{ chatRoom.name }}> {{ chatRoom.text }} </span>
              </div>
            </div>
            <div v-else>
              {{ chatUserData?.showDate ? chatRoom.theDate : '' }}{{ chatUserData?.showTime ? chatRoom.theTime : '' }} <span class="chatColor-color"> &lt;{{ chatRoom.name }}> {{ chatRoom.text }} </span>
            </div>

          </div>
        </div>
      </div>

      <div class="messages-typing">
        <div v-if="typingDisplay">{{ typingDisplay }}</div>
      </div>

      <div class="chat-room-tabs">
        <template v-for="chatRoom in chatRoomData" v-bind:key="chatRoom">
          <button class="tab-btn" :class="{ active: activeRoom?.roomName === chatRoom.roomName }" @click="activeRoom !== undefined ? activeRoom = chatRoom : ''">
            #{{ chatRoom.roomName }}
          </button>
        </template>

        <template v-for="chatRoomDirectMessage in chatRoomDirectMessageData" v-bind:key="chatRoomDirectMessage">
          <button class="tab-btn" :class="{ active: activeRoom?.roomName === chatRoomDirectMessage.roomName }" @click="activeRoom !== undefined ? activeRoom = chatRoomDirectMessage : ''">
            {{ chatRoomDirectMessage.participants[0] !== username ? chatRoomDirectMessage.participants[0] : chatRoomDirectMessage.participants[1] }}
          </button>
        </template>
      </div>


      <div class="message-input">
        <form>
          <label >Message: </label>
          <input v-model="messageText" type="message" @input="emitTyping"/>
          <button type="submit" @click.prevent="sendMessage">Send</button>
        </form>
      </div>

      <section class="public-logged-section">

        <div class="logged-users">
          <h2><u>Logged Users</u></h2>
          <div v-if="activeRoom !== undefined">
            <div v-for="loggedUsers in activeRoom.loggedUsers" v-bind:key="loggedUsers">
              <div v-if ="activeRoom.owner === loggedUsers.name" span class="owner-logged-users-color">
                @{{ loggedUsers.name }}
              </div>
            </div>
            <div v-for="loggedUsers in activeRoom.loggedUsers" v-bind:key="loggedUsers">
              <div v-if ="activeRoom.admins.includes(loggedUsers.name)" span class="admins-logged-users-color">
                +{{ loggedUsers.name }}
              </div>
            </div>
            <div v-for="loggedUsers in activeRoom.loggedUsers" v-bind:key="loggedUsers">
              <div v-if ="!activeRoom.admins.includes(loggedUsers.name) &&
                          activeRoom.owner !== loggedUsers.name" span class="users-logged-users-color">
                {{ loggedUsers.name }}
              </div>
            </div>
          </div>
        </div>

        <div class="public-rooms">
          <h2><u>Public Rooms</u></h2>
          <div v-if="publicRooms">
            <div v-for="publicRoom in publicRooms" v-bind:key="publicRoom">
              <span class="public-rooms-color">
                {{ publicRoom }}
              </span>
            </div>
          </div>
        </div>

      </section>

    </div>
  </div>
</template>

<script setup lang="ts">

import { io, Socket } from 'socket.io-client';
import { onBeforeMount, ref } from 'vue';

import { useMe } from "@/composables/user/useMe";

const BACKEND_URL = import.meta.env.VITE_BACKEND_URL.replace(/\/+$/, "");

const { me, fetchMe } = useMe();

const username = ref<string>('')
let socket: Socket;

const chatUserData = ref<ChatUser>();

const chatRoomData = ref<ChatRoom[]>([]);
const chatRoomDirectMessageData = ref<ChatRoomDirectMessage[]>([]);
const activeRoom = ref<ChatRoom | ChatRoomDirectMessage>();

const messageText = ref<string>('');
const typingDisplay = ref<string>('');

const publicRooms = ref<string[]>([]);

interface ChatUser {
  socketId: string;
  showDate: boolean;
  showTime: boolean;
  blockedUsers: string[];
  connectedRooms: string[];
}

interface ChatRoom {
  roomName: string;
  password: string;
  status: ChatRoomEnum;
  owner: string;
  admins: string[];
  loggedUsers: any;
  messagesObj: any;
  flaggedUsers: any;
}

interface ChatRoomDirectMessage extends ChatRoom {
  roomName: string;
  participants: string[];
  status: ChatRoomEnum;
  chatUser: ChatUser[];
  messagesObj: any;
}

interface MsgDto {
  typeOfMessage?: TypeOfMessage,
  typeOfRoom?: ChatRoomEnum,
  theDate?: string,
  theTime?: string,
  name?: string,
  text?: string,
  fromRoom?: string,
  privilege?: MsgDtoPrivilege;
}

interface MsgFromClientDto {
  typeOfMessage?: TypeOfMessage;
  typeOfRoom?: ChatRoomEnum;
  roomName?: string;
  name: string;
  text: string;
}

interface ClientTypingDto {
  name: string;
  fromRoom: string;
  isTyping: boolean;
}

enum TypeOfMessage {
  info = 'INFO',
  connected = 'CONNECTED',
  disconnected = 'DISCONNECTED',
  joined = 'JOINED',
  left = 'LEFT',
  kicked = 'KICKED',
  banned = 'BANNED',
  muted = 'MUTED',
  command = 'COMMAND',
  welcome = 'WELCOME',
}

enum ChatRoomEnum {
  public = 'PUBLIC',
  private = 'PRIVATE',
  directMessage = 'DIRECTMESSAGE',
}

enum MsgDtoPrivilege {
  owner = 'OWNER',
  admin = 'ADMIN',
  user = 'USER',
}

onBeforeMount(async () => {
  await fetchMe()
  socket = io(BACKEND_URL + '/chat', {query: {username: String(me.value?.username42)}});
  username.value = String(me.value?.username42);

  /*******************************************/
  /***            Connect Emits            ***/
  /*******************************************/
  socket.on('connect', () => {
    chatRoomData.value = [];
    chatRoomDirectMessageData.value = [];
  });

  socket.on('disconnect', () => {
    chatRoomData.value = [];
    chatRoomDirectMessageData.value = [];

    const length = Object.keys(activeRoom.value?.messagesObj).length;
    if (activeRoom.value) {
      const msgDto: MsgDto = {
        text: 'You\'ve been disconnected from the server',
        typeOfMessage: TypeOfMessage.disconnected,
      }
      activeRoom.value.messagesObj[length] = msgDto;
    }
  });

  /*******************************************/
  /***           Chat-Rooms Emits          ***/
  /*******************************************/
  socket.on('joinedRoom', (chatRoom: ChatRoom) => {
    activeRoom.value = chatRoom;

    chatRoomData.value.push(chatRoom);
  })

  socket.on('leftRoom', (room: string) => {
    // Remove Chat-Room and change Active Room to previous
    const newChatRoom = ref<ChatRoom[]>([]);
    let i = 0;
    let activeRoomIndex;
    for (const index in chatRoomData.value) {
      if (chatRoomData.value[index].roomName !== room) {
        newChatRoom.value[i] = chatRoomData.value[index];
        i++;
      } else {
        activeRoomIndex = i;
      }
    }
    chatRoomData.value = newChatRoom.value;

    if (activeRoomIndex) {
      activeRoom.value = chatRoomData.value[activeRoomIndex - 1];
    }
  })

  /*******************************************/
  /***   Chat-Rooms-Direct-Message Emits   ***/
  /*******************************************/
  socket.on('joinedRoomDirectMessage', (chatRoomDirectMessage: ChatRoomDirectMessage) => {
    // If chatRoomDirectMessage already exists, returns
    for (const index in chatRoomDirectMessageData.value) {
      if (chatRoomDirectMessageData.value[index].roomName === chatRoomDirectMessage.roomName) {
        activeRoom.value = chatRoomDirectMessageData.value[index];
        return ;
      }
    }
    chatRoomDirectMessageData.value.push(chatRoomDirectMessage);

    activeRoom.value = chatRoomDirectMessage;
  })

  socket.on('leftRoomDirectMessage', (room: string) => {
    // Remove Chat-Room and change Active Room to previous
    const newChatRoomDirectMessageData = ref<ChatRoomDirectMessage[]>([]);
    let i = 0;
    let activeRoomIndex;
    for (const index in chatRoomDirectMessageData.value) {
      if (chatRoomDirectMessageData.value[index].roomName !== room) {
        newChatRoomDirectMessageData.value[i] = chatRoomDirectMessageData.value[index];
        i++;
      } else {
        activeRoomIndex = i;
      }
    }
    chatRoomDirectMessageData.value = newChatRoomDirectMessageData.value;

    // If there is only 1 chatRoomDirectMessageData, it changes active room
    // to the last chatRoomData instead
    if (activeRoomIndex) {
      activeRoom.value = chatRoomDirectMessageData.value[activeRoomIndex - 1];
    } else {
      const lengthOfMessages = Object.keys(chatRoomData.value).length;
      activeRoom.value = chatRoomData.value[lengthOfMessages - 1];
    }
  })

  /*******************************************/
  /***            Messages Emits           ***/
  /*******************************************/
  socket.on('msgToClient', (msgDto: MsgDto) => {
    addMessageToChatRoomData(msgDto);
  });

  /*******************************************/
  /***            Updating Emits           ***/
  /*******************************************/
  // Get a list of public rooms in the server
  socket.on('getPublicRooms', (publicRoomsList: string[]) => {
    publicRooms.value = publicRoomsList;
  });

  // Gets the chat-user of this client
  socket.on('getChatUser', (chatUser: ChatUser) => {
    chatUserData.value = chatUser;
  });

  // Everytime a chat-room is updated (changed password, added admins etc...)
  socket.on('chatRoomUpdate', (chatRoom: ChatRoom) => {
    if (chatRoom && chatRoomData.value !== undefined) {

      // It updates everything except the messageObj.
      for (const index in chatRoomData.value) {
        if (chatRoomData.value[index].roomName === chatRoom.roomName) {
          chatRoomData.value[index].loggedUsers = chatRoom.loggedUsers;
          chatRoomData.value[index].password = chatRoom.password;
          chatRoomData.value[index].status = chatRoom.status;
          chatRoomData.value[index].owner = chatRoom.owner;
          chatRoomData.value[index].admins = chatRoom.admins;
          chatRoomData.value[index].flaggedUsers = chatRoom.flaggedUsers;

          break;
        }
      }
    }
  });

  /*******************************************/
  /***             Helpers Emit            ***/
  /*******************************************/
  socket.on('typing', (clientTypingDto: ClientTypingDto) => {
    if (clientTypingDto.isTyping && activeRoom.value?.roomName === clientTypingDto.fromRoom) {
      typingDisplay.value = `${clientTypingDto.name} is typing...`;
    } else {
      typingDisplay.value = '';
    }
  })
})

function addMessageToChatRoomData(msgDto: MsgDto) {
  // In case there is no roomName (it's to send to the active room)
  if (!msgDto.fromRoom && activeRoom.value) {
      msgDto.fromRoom = activeRoom.value?.roomName;
  }
  if (msgDto) {
    // Add message to the end of Chat Room Direct Message messagesObj.
    if (msgDto.typeOfRoom === ChatRoomEnum.directMessage) {
      for (const index in chatRoomDirectMessageData.value) {
        if (chatRoomDirectMessageData.value[index].roomName === msgDto.fromRoom) {
          const lengthOfMessages = Object.keys(chatRoomDirectMessageData.value[+index].messagesObj).length;
          chatRoomDirectMessageData.value[+index].messagesObj[lengthOfMessages] = msgDto;
          break;
        }
      }
    }
    // Add message to the end of Chat Room messagesObj.
    else {
      for (const index in chatRoomData.value) {
        if (chatRoomData.value[index].roomName === msgDto.fromRoom) {
          const lengthOfMessages = Object.keys(chatRoomData.value[+index].messagesObj).length;
          chatRoomData.value[+index].messagesObj[lengthOfMessages] = msgDto;
          break;
        }
      }
    }
  }
}

function sendMessage() {
  if (messageText.value === '') { return ; }

  const msgFromClientDto: MsgFromClientDto = {
    roomName: activeRoom.value?.roomName,
    name: username.value,
    text: messageText.value,
    typeOfRoom: activeRoom.value?.status,
  }

  socket.emit('msgFromClient', msgFromClientDto);
  messageText.value = '';
}

let timeout: number;
function emitTyping() {
  let clientTypingDto: ClientTypingDto = {
    name: username.value,
    isTyping: true,
    fromRoom: activeRoom.value ? activeRoom.value?.roomName : '',
  }
  socket.emit('typing', clientTypingDto);
  clearTimeout(timeout);

  timeout = setTimeout(() => {
    clientTypingDto.isTyping = false;
    socket.emit('typing', clientTypingDto);
  }, 750);
}

</script>

<style>
.welcome-color {
  font-style: italic;
  color: rgba(12, 81, 90, 0.822);
}

.info-color {
  font-style: italic;
  color: rgb(37, 150, 179);
}

.connected-color {
  font-style: italic;
  font-weight: bold;
  color: rgb(51, 126, 51);
}

.disconnected-color {
  font-style: italic;
  font-weight: bold;
  color: rgb(133, 59, 59);
}

.joined-color {
  font-style: italic;
  color: rgb(223, 59, 209);
}

.left-color {
  font-style: italic;
  color: rgb(196, 105, 188);
}

.kicked-color {
  font-style: italic;
  color: rgb(233, 46, 46);
}

.banned-color {
  font-style: italic;
  color: rgb(190, 0, 0);
}

.muted-color {
  font-style: italic;
  color: rgb(236, 109, 50);
}

.command-color {
  font-style: italic;
  color: rgb(72, 72, 190);
}

.chatColor-color {
  color: rgb(46, 38, 38)
}

.tab-btn.active {
  background: #0f7fb3;
  color: white;
}

.messages-chatRoomData {
  border: 1px solid black;
  max-height: 475px;
  max-width: 1000px;
  height: 475px;
  width: 1000px;

  position: relative;
  margin-right: 25px;

  left: 5px;
  top: 5px;
  bottom: 5px;
  right: 50%;

  text-indent: 5px;

  overflow-y: auto;
  scroll-snap-type: mandatory;

  display: flex;
  flex-direction: column-reverse;
}

.messages-typing {
  border: 1px solid rgb(252, 130, 130);
  max-height: 20px;
  max-width: 250px;
  height: 20px;
  width: 1000px;

  position: relative;
  margin-right: 25px;

  left: 5px;
  top: 10px;
  bottom: 5px;
  right: 50%;
}

.chat-room-tabs {
  position: relative;
  margin-right: 25px;

  left: 5px;
  top: 10px;
  bottom: 5px;
  right: 50%;
}

.message-input {
  position: relative;
  margin-right: 25px;

  left: 5px;
  top: 10px;
  bottom: 5px;
  right: 50%;
}

.public-logged-section {
  width: 100%;
  height: 200px;
  margin: auto;
  padding: 0px;
}

.logged-users {
  position: relative;
  margin-right: 25px;
  text-align: center;

  left: 5px;
  top: 20px;
  bottom: 5px;
  right: 50%;

  border: 1px solid black;

  height: 200px;
  width: 200px;
  max-height: 200px;
  max-width: 200px;

  float: left;
}

.owner-logged-users-color {
  color: #df29b7;
  font-weight: bold;
}

.admins-logged-users-color {
  color: #23a56f;
  font-weight: bold;
}

.users-logged-users-color {
  color: #2b3836;
  font-weight: bold;
}

.public-rooms {
  position: relative;
  margin-right: 25px;
  text-align: center;

  left: 5px;
  top: 20px;
  bottom: 5px;
  right: 50%;

  border: 1px solid black;

  height: 200px;
  width: 200px;
  max-height: 200px;
  max-width: 200px;

  float: left;
}

.public-rooms-color {
  color: rgb(77, 28, 155)
}
/*
#app {
  font-family: 'Courier New', Courier, monospace;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: left;
  color: #35083b;
  margin-top: 10px;
} */
</style>