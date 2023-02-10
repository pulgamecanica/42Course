export enum ChatRoomEnum {
  public = 'PUBLIC',
  private = 'PRIVATE',
  directMessage = 'DIRECTMESSAGE',
}

export enum FlaggedStatusEnum {
  banned = 'BANNED',
  muted = 'MUTED',
}

export enum MsgDtoPrivilege {
  owner = 'OWNER',
  admin = 'ADMIN',
  user = 'USER',
}

export enum TypeOfMessage {
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
  gameInvite = 'GAMEINVITE',
}
