<template>
  <el-tabs
    v-model="currentRoomName"
    type="border-card"
    editable
    class="chat-tabs"
    @tab-change="scrollToBottom"
    @tab-remove="onLeaveRoom"
    @tab-add="onClickCreateRoom"
    v-if="!loading"
  >
    <el-tab-pane
      v-for="room in rooms"
      :key="room.roomName"
      :label="roomLabel(room)"
      :name="room.roomName"
    >
      <div class="chat-panel" >
        <div class="chat-messages"
          v-if="room && !loading"
          :ref="$el => { messagesRef[room.roomName] = $el as Element }"
        >
          <template v-for="(message, i) in room.messagesObj" :key="i">
            <div class="message" v-if="!isBlocked(message.name)">
              <div class="date" v-if="user?.showDate">{{ message.theDate }}&nbsp;</div>
              <div class="time" v-if="user?.showTime">{{ message.theTime }}&nbsp;</div>
              <div
                class="username"
                :class="usernameClass(message)"
                :style="{ color: !message.typeOfMessage && message.userColor || '' }"
                v-if="message.name && message.typeOfMessage !== MessageType.Welcome && message.typeOfMessage !== MessageType.Game"
                @click.prevent="router.push({ name: 'profile', params: { username42: message.name } })"
              >
                {{ formatUsername(message) }}&nbsp;
              </div>
              <div class="text" :class="textClass(message)">
                {{ isGame(message) ? inviteText(message) : message.text }}
                <el-button
                  v-if="isGame(message)"
                  type="primary"
                  size="small"
                  class="u-ml--2"
                  @click="router.push({ name: 'game', params: { gameId: message.pongGameChat!.id } })"
                >
                  {{ isPlayer(message) ? 'Play' : 'Spectate' }}
                </el-button>
              </div>
            </div>
          </template>
        </div>
        <div class="chat-input">
          <el-input
            v-model="newMessage"
            placeholder="Type a message..."
            @keydown.enter="onSendMessage"
          />
          <el-tooltip content="Join" placement="top" >
            <el-button class="join-button" icon="chat-dot-round" @click="onClickJoinRoom"/>
          </el-tooltip>
          <el-tooltip v-if="hasUsersButton" content="Users" placement="top" :disabled="isMenuUsersVisible">
            <el-dropdown trigger="click" @command="onClickUser" @visible-change="(v: boolean) => isMenuUsersVisible = v">
              <el-button v-if="hasUsersButton" class="users-button" icon="user" />
              <template #dropdown>
                <el-dropdown-menu class="chat-users">
                  <template v-for="user in loggedUsers" :key="user.name">
                    <el-dropdown-item :command="user" :class="user.privilege.toLowerCase()"  >
                      {{ `${privilegePrefix(user.privilege)}${user.name}` }}
                    </el-dropdown-item>
                  </template>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </el-tooltip>
          <el-tooltip content="Settings" placement="top" :disabled="isMenuSettingsVisible">
            <el-dropdown :hide-on-click="false" trigger="click" @visible-change="(v: boolean) => isMenuSettingsVisible = v">
              <el-button class="settings-button" icon="setting" />
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item @click="toggleDate">{{ user?.showDate ? 'Hide' : 'Show' }} Date</el-dropdown-item>
                  <el-dropdown-item @click="toggleTime">{{ user?.showTime ? 'Hide' : 'Show' }} Time</el-dropdown-item>
                  <el-dropdown-item v-if="canSetPassword" @click="onClickSetPassword">Set Password</el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </el-tooltip>
        </div>
      </div>
    </el-tab-pane>
  </el-tabs>

  <!-- Form for Create or Join chat room -->
  <el-dialog v-model="isDialogAddVisible" :title="`${isCreateDialog ? 'Create' : 'Join'} Chat Room`" @close="formAddRef?.resetFields()">
    <el-form :model="formAdd" ref="formAddRef" :rules="formAddRules">
      <el-form-item :label-width="formLabelWidth">
        <el-radio-group v-if="isCreateDialog || filteredPublicRooms.length" v-model="formAdd.status" class="ml-4">
          <el-radio label="public" size="large">Public</el-radio>
          <el-radio label="private" size="large">Private</el-radio>
        </el-radio-group>
        <span v-else>Private</span>
      </el-form-item>
      <el-form-item label="Name" :label-width="formLabelWidth" prop="name">
        <el-select
          v-if="isJoinDialog && formAdd.status === 'public' && filteredPublicRooms.length"
          v-model="formAdd.name"
          placeholder="Select room"
          style="width: 100%;"
        >
          <el-option
            v-for="room in filteredPublicRooms"
            :key="room"
            :label="room"
            :value="room"
          />
        </el-select>
        <el-input v-else v-model="formAdd.name" placeholder="Enter room name" />
      </el-form-item>
      <el-form-item label="Password" :label-width="formLabelWidth">
        <el-input v-model="formAdd.password" type="password" show-password placeholder="Optional" />
      </el-form-item>
    </el-form>

    <template #footer>
      <span class="dialog-footer">
        <el-button @click="closeAddDialog">Cancel</el-button>
        <el-button type="primary" @click="onAddRoom" :disabled="isFormAddDisabled">
          {{ `${isCreateDialog ? 'Create' : 'Join'}` }}
        </el-button>
      </span>
    </template>
  </el-dialog>

  <!-- Form for User actions -->
  <el-dialog v-model="isDialogUserVisible" title="User actions">
    <el-form :model="formUser">
      <el-form-item label="Action" :label-width="formLabelWidth">
        <el-select
          v-model="formUser.command"
          placeholder="Select action"
          style="width: 100%;"
        >
          <el-option
            v-for="action in filteredActions"
            :key="action.label"
            :label="action.label"
            :value="action.command"
          />
        </el-select>
      </el-form-item>
      <el-form-item v-if="formUser.command === 'ban'" label="Reason" :label-width="formLabelWidth">
        <el-input v-model="formUser.reason" placeholder="Enter reason" />
      </el-form-item>
      <el-form-item v-if="['ban', 'mute'].includes(formUser.command)" label="Duration" :label-width="formLabelWidth">
        <el-input v-model="formUser.time" type="number" placeholder="Enter duration" />
      </el-form-item>
    </el-form>
    <template #footer>
      <span class="dialog-footer">
        <el-button @click="closeUserDialog">Cancel</el-button>
        <el-button type="primary" @click="onUserAction">Save</el-button>
      </span>
    </template>
  </el-dialog>
</template>

<script lang="ts" setup>
import { reactive, ref, watch, onUnmounted, computed } from 'vue'
import { useChat } from "@/composables/chat/useChat"
import { useMe } from "@/composables/user/useMe"
import type {
  Message, AbstractChatRoom, DirectMessageChatRoom, RoomStruct, ChatRoom, FlaggedUser,
} from "@/composables/chat/types";
import {
  MessageType, MessagePrivilege,
} from "@/composables/chat/types";
import { ElMessage, ElMessageBox } from 'element-plus'
import { useRouter } from 'vue-router';
import type { FormInstance, FormRules } from 'element-plus'


// debug
const c = console.log;
const log = (msg: any) => {
  c(msg);
}


// Types
type LoggedUser = {
  name: string,
  privilege: MessagePrivilege
}

// Constants
const ALPHANUMERIC = /^[a-zA-Z0-9]+$/;
const formLabelWidth = '140px'

const validateAlphanumeric = (_rule: any, value: string, cb: any) => {
  if (value && !isAlphanumeric(value)) {
    cb(new Error('Must be alphanumeric!'))
  } else {
    cb()
  }
}

// Refs
const newMessage = ref('')
const isDialogAddVisible = ref(false)
const isDialogUserVisible = ref(false)
const addRoomDialog = ref<'join' | 'create'>()
const isMenuSettingsVisible = ref(false)
const isMenuUsersVisible = ref(false)
const selectedUser = ref<LoggedUser>()
const formAddRef = ref<FormInstance>()

// Reactives
const formAdd = reactive<RoomStruct>({
  name: '',
  password: '',
  status: 'public',
})

const formUser = reactive({
  command: '',
  reason: '',
  time: '',
})

const formAddRules = reactive<FormRules>({
  name: [
    { required: true, message: 'Name is required', trigger: 'blur' },
    { validator: validateAlphanumeric },
  ],
})

const router = useRouter();
const { me } = useMe();
const {
  sendMessage, socketInit, isDirectMessage, currentRoom, sendCommand,
  createRoom, socketCreate, publicRooms, currentRoomName, loading,
  joinRoom, socketDisconnect, toggleDate, messagesRef, user, scrollToBottom,
  leaveRoom, socketReset, toggleTime, rooms, setRoomPassword,
} = useChat()

// Computed
const filteredPublicRooms = computed(() => publicRooms.value.filter(r => !Object.keys(rooms.value).includes(r)))
const isJoinDialog = computed(() => addRoomDialog.value === 'join')
const isCreateDialog = computed(() => addRoomDialog.value === 'create')

const filteredUsers = computed<string[]>(() => {
  if (!currentRoom.value || isDirectMessage(currentRoom.value)) {
    return []
  }
  const room = currentRoom.value as ChatRoom
  return Object
    .values(room.loggedUsers)
    .map(u => u.name)
    .filter(u => u !== me.value?.username42)
})

const loggedUsers = computed(() => {
  if (!currentRoom.value || isDirectMessage(currentRoom.value)) {
    return []
  }
  return filteredUsers.value.map(name => {
    const privilege = isOwner(name)
      ? MessagePrivilege.Owner : isAdmin(name)
        ? MessagePrivilege.Admin : MessagePrivilege.User;

    return {
      name,
      privilege,
    }
  }).sort((a, b) => { // can be extracted if used somewhere else
    const isOwner = (p: MessagePrivilege) => p === 'OWNER'
    const isAdmin = (p: MessagePrivilege) => p === 'ADMIN'
    const isUser = (p: MessagePrivilege) => p === 'USER'

    if (isOwner(a.privilege) && (isAdmin(b.privilege) || isUser(b.privilege))) return -1;
    if (isAdmin(a.privilege) && isUser(b.privilege)) return -1;
    if (isUser(a.privilege) && (isOwner(b.privilege) || isAdmin(b.privilege))) return 1;
    if (isAdmin(a.privilege) && isOwner(b.privilege)) return 1;

    if (a.name > b.name) return 1;
    if (a.name < b.name) return -1;
    return 0
  })
})

const isMeAdmin = computed(() => me.value && isAdmin(me.value.username42))
const isMeOwner = computed(() => me.value && isOwner(me.value.username42))

const canSetPassword = computed(() => isMeAdmin.value || isMeOwner.value)

const hasUsersButton = computed(() => {
  return !isDirectMessage(currentRoom.value!) && filteredUsers.value.length
})

const usersFlags = computed(() => {
  if (!currentRoom.value || isDirectMessage(currentRoom.value)) {
    return {}
  }
  const room = currentRoom.value as ChatRoom
  return Object.values(room.flaggedUsers).reduce<{ [name: string]: FlaggedUser[] }>((users, u) => {
    users[u.name] ||= []
    users[u.name].push(u)
    return users
  }, {});
})

const filteredActions = computed(() => {
  if (!selectedUser.value) {
    return []
  }
  const u = selectedUser.value
  const flags = (usersFlags.value[u.name] || []).map(f => f.status)
  const isBanned = flags.includes('BANNED')
  const isMuted = flags.includes('MUTED')
  const isOwner = u.privilege === 'OWNER'
  const isAdmin = u.privilege === 'ADMIN'
  const options = [
    { label: 'Game invite', command: 'invite' },
    { label: 'View profile', command: 'profile' },
  ]

  if (isBlocked(u.name)) {
    options.push({ label: 'Unblock', command: 'unblock' })
  } else {
    options.push({ label: 'Chat', command: 'msg' })
    options.push({ label: 'Block', command: 'block' })
  }

  if (isMeOwner.value) {
    options.push({ label: 'Make owner', command: 'owner' })
  }

  if (isMeOwner.value || (isMeAdmin.value && !isOwner)) {
    isAdmin
      ? options.push({ label: 'Remove admin', command: 'remove' })
      : options.push({ label: 'Make admin', command: 'admin' })
    isBanned
      ? options.push({ label: 'Unban', command: 'unban' })
      : options.push({ label: 'Ban', command: 'ban' })
    isMuted
      ? options.push({ label: 'Unmute', command: 'unmute' })
      : options.push({ label: 'Mute', command: 'mute' })
    options.push({ label: 'Kick', command: 'kick' })
  }

  return options
})

const isFormAddDisabled = computed(() => !formAdd.name || !isAlphanumeric(formAdd.name))

// Lifecycle hooks
socketCreate()
socketInit()
onUnmounted(socketDisconnect)

watch(
  me,
  (newValue, oldValue) => {
    if (newValue && (!oldValue || newValue.username42 !== oldValue.username42)) {
      socketReset()
    }
  },
  { deep: true }
);

// Methods
const usernameClass = ({ typeOfMessage: type }: Message): string => type && `message-${type.toLowerCase()}` || ''
const textClass = ({ typeOfMessage: type }: Message): string => type && `message-${type.toLowerCase()}` || ''
const _separator = (type: MessageType): string => !type ? ':' : type === MessageType.Command ? '' : '*'

const privilegePrefix = (privilege: MessagePrivilege): string => {
  return privilege === MessagePrivilege.Owner ? '@' : privilege === MessagePrivilege.Admin ? '+' : ''
}

const formatUsername = ({ typeOfMessage: type, name, privilege }: Message, withSeparator: boolean = true): string => {
  if (!currentRoom.value) {
    return ''
  }
  const n = isDirectMessage(currentRoom.value) ? name : `${privilegePrefix(privilege)}${name}`
  const separator = withSeparator ? _separator(type) : ''
  return !type ? `${n}${separator}` : `${separator} ${n}`
}

const isGame = ({ typeOfMessage }: Message) => typeOfMessage === MessageType.Game
const myMessage = ({ name }: Message) => name === me.value?.username42
const isChallenger = (message: Message) => isGame(message) && isPlayer(message) && myMessage(message)
const isOpponent = (message: Message) => isGame(message) && isPlayer(message) && !myMessage(message)

const isPlayer = ({ pongGameChat }: Message) => {
  return pongGameChat && me.value && [pongGameChat.usernameOne, pongGameChat.usernameTwo].includes(me.value.username42)
}

const inviteText = (message: Message) => {
  const challenger = message.name
  const opponent = message.text.split(' ')[1]

  if (isChallenger(message)) {
    return `You have invited ${opponent} to a game of Pong!`
  } else if (isOpponent(message)) {
    return `You have been invited to a game of Pong by ${challenger}!`
  } else {
    return `${challenger} has invited ${opponent} to a game of Pong!`
  }
}

const roomLabel = (room: AbstractChatRoom): string => {
  return isDirectMessage(room)
    ? `(dm) ${(room as DirectMessageChatRoom).participants.filter(p => p !== me.value?.username42)[0]}`
    : room.roomName;
}

const isOwner = (username: string) => {
  if (!currentRoom.value || isDirectMessage(currentRoom.value)) {
    return false
  }
  return (currentRoom.value as ChatRoom).owner === username
}

const isAdmin = (username: string) => {
  if (!currentRoom.value || isDirectMessage(currentRoom.value)) {
    return false
  }
  return (currentRoom.value as ChatRoom).admins.includes(username)
}

const isAlphanumeric = (str: string) => ALPHANUMERIC.test(str)
const isBlocked = (username: string) => user.value?.blockedUsers.includes(username)

// Event Handlers
const closeAddDialog = () => {
  isDialogAddVisible.value = false
  formAdd.password = ''
  formAdd.name = ''
  formAdd.status = 'public'
}

const closeUserDialog = () => {
  isDialogUserVisible.value = false
  selectedUser.value = undefined
  formUser.command = ''
  formUser.reason = ''
  formUser.time = ''
}

const onSendMessage = () => {
  sendMessage(newMessage.value);
  newMessage.value = ''
}

const onLeaveRoom = (name: string) => {
  currentRoomName.value = name
  leaveRoom()
}

const onClickJoinRoom = () => {
  addRoomDialog.value = 'join'
  // default to public unless no available public rooms
  formAdd.status = publicRooms.value.length ? 'public' : 'private'
  isDialogAddVisible.value = true
}

const onClickCreateRoom = () => {
  addRoomDialog.value = 'create'
  isDialogAddVisible.value = true
}

const onAddRoom = async () => {
  if (!formAddRef.value) {
    return
  }
  try {
    await formAddRef.value.validate((valid) => {
      if (valid) {
        (isCreateDialog.value ? createRoom : joinRoom)(formAdd)
        closeAddDialog()
      }
    })
  } catch (error) {}
}

const onClickSetPassword = () => {
  ElMessageBox.prompt('Set a password', 'Password Protection', {
    confirmButtonText: 'OK',
    cancelButtonText: 'Cancel',
    inputType: 'password'
  })
    .then(({ value }) => setRoomPassword(value))
    .catch(() => {}); // to not get unhandled promise error in console when closing prompt
}

const onClickUser = (user: LoggedUser) => {
  selectedUser.value = user
  if (filteredActions.value.length) {
    isDialogUserVisible.value = true
  }
}

const onUserAction = () => {
  if (!selectedUser.value) {
    return
  }
  const u = selectedUser.value
  const f = formUser
  switch (f.command) {
    case 'profile':
      router.push({ name: "profile", params: { username42: u.name } });
    case 'ban':
      sendCommand(`/${f.command} ${u.name} ${f.reason ? `"${f.reason}"` : ''} ${f.time || ''}`.trim());
      break;
    case 'mute':
      sendCommand(`/${f.command} ${u.name} ${f.time || ''}`.trim());
      break;
    default:
      sendCommand(`/${f.command} ${u.name}`.trim());
      break;
  }
  closeUserDialog()
}
</script>

<style lang="scss" scoped>

</style>