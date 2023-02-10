import type { PongGameState } from "@/components/pongTypes";

export enum MessageType {
  Info = 'INFO',
  Connected = 'CONNECTED',
  Disconnected = 'DISCONNECTED',
  Joined = 'JOINED',
  Left = 'LEFT',
  Kicked = 'KICKED',
  Banned = 'BANNED',
  Muted = 'MUTED',
  Command = 'COMMAND',
  Welcome = 'WELCOME',
  Game = 'GAMEINVITE',
}

export enum ChatRoomStatus {
  Public = 'PUBLIC',
  Private = 'PRIVATE',
  DirectMessage = 'DIRECTMESSAGE',
}

export enum MessagePrivilege {
  Owner = 'OWNER',
  Admin = 'ADMIN',
  User = 'USER',
}

export interface ChatUser {
  socketId: string;
  showDate: boolean;
  showTime: boolean;
  blockedUsers: string[];
  connectedRooms: string[];
}

type Users = {
  [index: number]: {
    name: string;
    clientId: string
  }
}

export interface FlaggedUser {
  flaggedBy: string;
  name: string;
  reason?: string;
  status: 'BANNED' | 'MUTED';
  timeEnd: string;
  timeStart: string;
}

export type FlaggedUsers = {
  [index: number]: FlaggedUser
}

export type RoomStruct = {
  name: string
  status?: 'private' | 'public'
  password?: string
}

export interface AbstractChatRoom {
  roomName: string;
  status: ChatRoomStatus;
  messagesObj: Message[];
}

export interface ChatRoom extends AbstractChatRoom {
  password: string;
  owner: string;
  admins: string[];
  loggedUsers: Users;
  flaggedUsers: FlaggedUsers;
}

export interface DirectMessageChatRoom extends AbstractChatRoom {
  participants: string[];
  chatUser: ChatUser[];
}

export interface ChatPongGame {
  id: number
  status: string
  usernameOne: string
  usernameTwo: string
  winnerUsername: string
}

export interface Message {
  typeOfMessage: MessageType,
  typeOfRoom: ChatRoomStatus,
  theDate: string,
  theTime: string,
  name: string,
  text: string,
  fromRoom: string,
  userColor?: string,
  privilege: MessagePrivilege;
  pongGameChat?: ChatPongGame;
}

export interface ClientMessage {
  typeOfMessage?: MessageType;
  typeOfRoom: ChatRoomStatus;
  roomName: string;
  name: string;
  text: string;
}

export interface ClientTyping {
  name: string;
  fromRoom: string;
  isTyping: boolean;
}
