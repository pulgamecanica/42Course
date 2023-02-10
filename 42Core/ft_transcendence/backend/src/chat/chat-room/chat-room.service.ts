/* eslint-disable prettier/prettier */
import { Injectable, Logger, UnprocessableEntityException } from '@nestjs/common';
import { Server, Socket } from 'socket.io';
import { MsgDto } from './dtos/msg.dto';
import { ChatRoomEnum, FlaggedStatusEnum, MsgDtoPrivilege, TypeOfMessage } from './enums/chat-room.enums';
import { InjectRepository } from '@nestjs/typeorm';
import { ChatRoom } from './entities/chat-room.entity';
import { Repository } from 'typeorm';
import { ChatUserService } from '../chat-user/chat-user.service';
import { UpdateChatUserDto } from '../chat-user/dto/update-chat-user.dto';
import { SchedulerRegistry } from '@nestjs/schedule';
import { promisify } from 'util';
import { scrypt as _scrypt } from 'crypto';
import * as moment from 'moment';
import { CreateChatRoomDto } from './dtos/create-chat-room.dto';
import { UpdateChatRoomDto } from './dtos/update-chat-room.dto';
import { MsgFromClientDto } from './dtos/msg-from-client.dto';
import { UserService } from 'src/users/user/user.service';
import { UserAndSocketIdDto } from './dtos/user-and-socketId.dto';
import { FlaggedUserDto } from './dtos/flagged-user.dto';
import { ChatRoomDirectMessageService } from '../chat-room-direct-message/chat-room-direct-message.service';
import { User } from 'src/users/user/entities/user.entity';
import { UpdateChatRoomDirectMessageDto } from '../chat-room-direct-message/dtos/update-chat-room-direct-message.dto';
import { FriendRelationService } from '../../users/friend-relation/friend-relation.service';
import { PongGameService } from 'src/pong-game/pong-game.service';
import { GameStatus } from 'src/pong-game/entities/pong-game.entity';

/*******************************************/
/***               Helpers               ***/
/*******************************************/
export const StaticRoom = 'lounge';

export enum CommandOptions {
  // User commands
  help = '/help',
  create = '/create',
  join = '/join',
  leave = '/leave',
  showdate = '/showdate',
  showtime = '/showtime',
  block = '/block',
  unblock = '/unblock',
  blocklist = '/blocklist',
  msg = '/msg',
  invite = '/invite',

  // Room commands
  password = '/password',
  owner = '/owner',
  admin = '/admin',
  remove = '/remove',
  ownerlist = '/ownerlist',
  adminlist = '/adminlist',
  flaglist = '/flaglist',
  kick = '/kick',
  ban = '/ban',
  unban = '/unban',
  mute = '/mute',
  unmute = '/unmute',
}

export enum DateOptions {
  onlyTime = 'ONLYTIME',
  onlyDate = 'ONLYDATE',
  timeAndDate = 'TIMEANDDATE',
  none = 'NONE',
}

export class ColorScheme {
  reset = '\x1b[0m';

  fgBlack = '\x1b[30m';
  fgRed = '\x1b[31m';
  fgGreen = '\x1b[32m';
  fgYellow = '\x1b[33m';
  fgBlue = '\x1b[34m';
  fgMagenta = '\x1b[35m';
  fgCyan = '\x1b[36m';
  fgWhite = '\x1b[37m';

  bgBlack = '\x1b[40m';
  bgRed = '\x1b[41m';
  bgGreen = '\x1b[42m';
  bgYellow = '\x1b[43m';
  bgBlue = '\x1b[44m';
  bgMagenta = '\x1b[45m';
  bgCyan = '\x1b[46m';
  bgWhite = '\x1b[47m';
}

@Injectable()
export class ChatRoomService {
  constructor(
    @InjectRepository(ChatRoom) private repo: Repository<ChatRoom>,
    private friendRelationService: FriendRelationService,
    private userService: UserService,
    private chatUserService: ChatUserService,
    private chatRoomDirectMessageService: ChatRoomDirectMessageService,
    private pongGameService: PongGameService,
    private schedulerRegistry: SchedulerRegistry,
  ) {}

  private logger: Logger = new Logger('ChatRoomService');
  colorScheme = new ColorScheme();
  scrypt = promisify(_scrypt);

  /*******************************************/
  /***            Controller Logic         ***/
  /*******************************************/
  async findAll() {
    return await this.repo.find();
  }

  async findByRoomName(roomName: string) {
    const chatRoom = await this.repo.findOne({
      where: {
        roomName,
      },
    });
    if (!chatRoom) {
      return null;
    }

    return chatRoom;
  }

  async findAllPublicRooms() {
    const chatRooms = await this.repo.find({
      where: {
        status: ChatRoomEnum.public,
      },
    });
    if (!chatRooms) {
      return null;
    }

    return chatRooms;
  }

  async create(chatRoomDto: CreateChatRoomDto) {
    if (await this.findByRoomName(chatRoomDto.roomName)) {
      return `${chatRoomDto.roomName} already exists!`;
    }

    const newChatRoom = this.repo.create(chatRoomDto);

    return await this.repo.save(newChatRoom).catch((err: any) => {
      const message =
        typeof err === 'string' ? err : err.message ? err.message : '';

      if (message) {
        throw new UnprocessableEntityException(message);
      } else {
        console.log('message not found');
      }
    });
  }

  async update(roomName: string, updateChatRoomDto: UpdateChatRoomDto) {
    const chatRoom = await this.findByRoomName(roomName);
    if (!chatRoom) {
      return `${roomName} not found.`;
    }
    await this.repo.update(chatRoom.id, updateChatRoomDto);

    return await this.findByRoomName(roomName);
  }

  async delete(roomName: string) {
    const chatRoom = await this.findByRoomName(roomName);
    if (!chatRoom) {
      return `${roomName} not found.`;
    }

    return await this.repo.remove(chatRoom);
  }

  /*******************************************/
  /***             Gateway Logic           ***/
  /*******************************************/
  /*******************************************/
  /***             Utility Logic           ***/
  /*******************************************/
  getDate(dateOptions: DateOptions): string {
    const date = new Date(Date.now());

    const dateDay = date.toLocaleString('pt-PT', { day: 'numeric' });
    const dateMonth = date
      .toLocaleString('pt-PT', { month: 'short' })
      .replace('.', '');
    const dateYear = date.toLocaleString('pt-PT', { year: 'numeric' });

    const dateTime = date.toLocaleString('pt-PT', {
      hour: 'numeric',
      minute: 'numeric',
      second: 'numeric',
    });

    const upperMonth = dateMonth[0].toUpperCase() + dateMonth.slice(1);
    const dateFormated =
      '[' + dateDay + '-' + upperMonth + '-' + dateYear + ']';

    const timeFormated = '[' + dateTime + ']';

    if (dateOptions === DateOptions.onlyTime) {
      return timeFormated;
    } else if (dateOptions === DateOptions.onlyDate) {
      return dateFormated;
    } else {
      return dateFormated + timeFormated;
    }
  }

  async createLoungeRoom() {
    const loungeChatRoom: CreateChatRoomDto = {
      roomName: StaticRoom,
      password: '',
      status: ChatRoomEnum.public,
      owner: '',
      admins: [],
      loggedUsers: {} as UserAndSocketIdDto,
      messagesObj: [],
    };

    await this.create(loungeChatRoom);
  }

  /*******************************************/
  /***               Log Logic             ***/
  /*******************************************/
  logClientMessage(
    color: string,
    timeAndDate: string,
    name: string,
    text: string,
  ) {
    this.logger.log(
      color + timeAndDate + ' ' + name + ' ' + text + this.colorScheme.reset,
    );
  }

  logConnection(color: string, name: string, connected: boolean) {
    let text: string;

    if (connected) {
      text = 'has connected.';
    } else {
      text = 'has disconnected.';
    }

    this.logger.log(color + name + ' ' + text + this.colorScheme.reset);
  }

  logMessage(color: string, text: string) {
    this.logger.log(color + text + this.colorScheme.reset);
  }

  /*******************************************/
  /***             Message Logic           ***/
  /*******************************************/
  async handleMsgClientToClient(server: Server, client: Socket, msgFromClientDto: MsgFromClientDto) {
    const theDate = this.getDate(DateOptions.onlyDate);
    const theTime = this.getDate(DateOptions.onlyTime);

    this.logClientMessage(
      this.colorScheme.fgWhite,
      theDate + theTime,
      msgFromClientDto.name,
      msgFromClientDto.text,
    );

    const chatUser = await this.chatUserService.findByUsername42(msgFromClientDto.name);
    if (chatUser) {
      msgFromClientDto.userColor = chatUser.userColor;
    }

    const msgDto: MsgDto = {
      typeOfMessage: msgFromClientDto.typeOfMessage,
      typeOfRoom: msgFromClientDto.typeOfRoom,
      userColor: msgFromClientDto.userColor,
      theDate,
      theTime,
      name: msgFromClientDto.name,
      text: msgFromClientDto.text,
      fromRoom: msgFromClientDto.roomName,
    };

    server.to(msgFromClientDto.roomName).emit('msgToClient', msgDto);
    await this.addMessageToChatRoomDirectMessage(msgFromClientDto);
    await this.handleJoinRoomDirectMessageOtherClient(server, msgFromClientDto);
  }

  async handleMsgClientToRoom(server: Server, client: Socket, msgFromClientDto: MsgFromClientDto) {
    const theDate = this.getDate(DateOptions.onlyDate);
    const theTime = this.getDate(DateOptions.onlyTime);

    this.logClientMessage(
      this.colorScheme.fgWhite,
      theDate + theTime,
      msgFromClientDto.name,
      msgFromClientDto.text,
    );

    // Check user privilege
    const chatRoom = await this.findByRoomName(msgFromClientDto.roomName);
    let privilege;
    if (chatRoom.owner === msgFromClientDto.name) {
      privilege = MsgDtoPrivilege.owner;
    } else if (chatRoom.admins.includes(msgFromClientDto.name)) {
      privilege = MsgDtoPrivilege.admin;
    } else {
      privilege = MsgDtoPrivilege.user;
    }

    const chatUser = await this.chatUserService.findByUsername42(msgFromClientDto.name);
    if (chatUser) {
      msgFromClientDto.userColor = chatUser.userColor;
    }

    const msgDto: MsgDto = {
      typeOfMessage: msgFromClientDto.typeOfMessage,
      typeOfRoom: msgFromClientDto.typeOfRoom,
      userColor: msgFromClientDto.userColor,
      theDate,
      theTime,
      name: msgFromClientDto.name,
      text: msgFromClientDto.text,
      fromRoom: msgFromClientDto.roomName,
      privilege: privilege,
    };

    if (msgFromClientDto.typeOfMessage === TypeOfMessage.gameInvite) {
      msgDto.pongGameChat = msgFromClientDto.pongGameChat;
    }

    // Checks if user is muted from room.
    if (await this.checkFlaggedUser(server, client, msgFromClientDto, FlaggedStatusEnum.muted) === true) {
      return;
    }

    server.to(msgFromClientDto.roomName).emit('msgToClient', msgDto);
    await this.addMessageToChatRoom(msgFromClientDto);
  }

  async handleMsgServerToRoom(server: Server, msgFromClientDto: MsgFromClientDto) {
    const theDate = this.getDate(DateOptions.onlyDate);
    const theTime = this.getDate(DateOptions.onlyTime);

    this.logClientMessage(
      this.colorScheme.fgWhite,
      theDate + theTime,
      msgFromClientDto.name,
      msgFromClientDto.text,
    );

    const msgDto: MsgDto = {
      typeOfMessage: msgFromClientDto.typeOfMessage,
      typeOfRoom: msgFromClientDto.typeOfRoom,
      theDate,
      theTime,
      name: msgFromClientDto.name,
      text: msgFromClientDto.text,
      fromRoom: msgFromClientDto.roomName,
    };

    server.to(msgFromClientDto.roomName).emit('msgToClient', msgDto);
    await this.addMessageToChatRoom(msgFromClientDto);
  }

  handleMsgServerToClient(client: Socket, msgFromClientDto: MsgFromClientDto) {
    const theDate = this.getDate(DateOptions.onlyDate);
    const theTime = this.getDate(DateOptions.onlyTime);

    if (msgFromClientDto.typeOfMessage !== TypeOfMessage.welcome &&
        msgFromClientDto.typeOfMessage !== TypeOfMessage.command) {
      this.logClientMessage(
        this.colorScheme.fgWhite,
        theDate + theTime,
        msgFromClientDto.name,
        msgFromClientDto.text,
      );
    }

    const msgDto: MsgDto = {
      typeOfMessage: msgFromClientDto.typeOfMessage,
      typeOfRoom: msgFromClientDto.typeOfRoom,
      theDate,
      theTime,
      name: msgFromClientDto.name,
      text: msgFromClientDto.text,
    };

    client.emit('msgToClient', msgDto);
  }

  /*******************************************/
  /***           Chat-Room Logic           ***/
  /***         Connect / Disconnect        ***/
  /***        Join / Leave / Welcome       ***/
  /*******************************************/
  async handleConnection(server: Server, client: Socket) {
    const username42 = <string>client.handshake.query.username;

    this.logConnection(
      this.colorScheme.fgCyan,
      username42,
      true,
    );

    // Disconnect previous clientId (in case of duplicated tabs)
    const oldChatUser = await this.chatUserService.findByUsername42(username42);
    if (!oldChatUser) {
      return ;
    }
    if (oldChatUser && oldChatUser.socketId) {
      const userSockets = await server.in(oldChatUser.socketId).fetchSockets();
      const userSocket = userSockets.find(socket => socket.id.toString() === oldChatUser.socketId);
      if (userSocket) {
        userSocket.disconnect();
      }
    }

    // Add socketId to client
    await this.chatUserService.update(oldChatUser.id, {
      socketId: client.id,
    });

    // Send the Chat-User to the Client (frontend).
    const user = await this.userService.findUserByUserName42(username42);
    if (user) {
      client.emit('getChatUser', user.chatUser);
    }

    // Send the list of Public Rooms to the Client (frontend).
    const publicChatRooms = await this.findAllPublicRooms();
    if (publicChatRooms) {
      const publicRooms: string[] = [];
      for (const index in publicChatRooms) {
        publicRooms.push(publicChatRooms[index].roomName);
      }
      client.emit('getPublicRooms', publicRooms);
    }

    // Join main room ('Lounge').
    let msgFromClientDto: MsgFromClientDto = { name: username42, roomName: StaticRoom, text: '' }
    await this.handleJoinRoom(server, client, msgFromClientDto);

    // Join other rooms that the client previously joined
    for (const connectedRoom in user.chatUser.connectedRooms) {
      if (user.chatUser.connectedRooms[connectedRoom] !== StaticRoom) {
        msgFromClientDto = { name: username42, roomName: user.chatUser.connectedRooms[connectedRoom], text: '' }
        await this.handleJoinRoom(server, client, msgFromClientDto);
      }
    }

    // Join direct messages rooms that the client previously started
    for (const connectedRoomDirectMessage in user.chatUser.connectedRoomsDirectMessage) {
        await this.handleJoinRoomDirectMessage(client, user, user.chatUser.connectedRoomsDirectMessage[connectedRoomDirectMessage]);
    }
  }

  async handleDisconnect(server: Server, client: Socket) {
    const username = <string>client.handshake.query.username;

    this.logConnection(
      this.colorScheme.fgRed,
      username,
      false,
    );

    // Send disconnect message to the rooms user was logged.
    // Removes user from ChatRoom loggedUsers.
    // Updates Chat-Room to all users.
    const user = await this.userService.findUserByUserName42(username);
    for (const connectedRoom in user.chatUser.connectedRooms) {
      const msgFromClientDto: MsgFromClientDto = {
        roomName: user.chatUser.connectedRooms[connectedRoom],
        name: username,
        text: 'has disconnected',
        typeOfMessage: TypeOfMessage.disconnected,
      };

      await this.handleMsgServerToRoom(server, msgFromClientDto);

      await this.removeLoggedUserFromChatRoom(client, msgFromClientDto.roomName, msgFromClientDto.name);

      const chatRoom = await this.findByRoomName(msgFromClientDto.roomName);
      server.to(msgFromClientDto.roomName).emit('chatRoomUpdate', chatRoom);
    }
  }

  async handleJoinRoom(server: Server, client: Socket, msgFromClientDto: MsgFromClientDto) {
    const roomLowerCase = msgFromClientDto.roomName.toLowerCase();

    // Checks if user is banned from room.
    if (await this.checkFlaggedUser(server, client, msgFromClientDto, FlaggedStatusEnum.banned) === true) {
      return;
    }

    // Join room, add user to loggedUser of Chat-Room and to Chat-User connected rooms.
    client.join(roomLowerCase);
    await this.addLoggedUserToChatRoom(client, roomLowerCase, msgFromClientDto.name);
    await this.chatUserService.addConnectedRoomToChatUser(msgFromClientDto.name, roomLowerCase);

    // Send the Chat-Room to the user.
    const chatRoom = await this.findByRoomName(roomLowerCase);
    client.emit('joinedRoom', chatRoom);

    // Send the Chat-User updated to the client.
    const user = await this.userService.findUserByUserName42(msgFromClientDto.name);
    client.emit('getChatUser', user.chatUser);

    // Send the Chat-Room updated to the entire room except the client.
    client.to(roomLowerCase).emit('chatRoomUpdate', chatRoom);

    // Welcome message.
    await this.handleMsgWelcome(server, client, msgFromClientDto);

    // Connected or Joined msg
    if (msgFromClientDto.roomName.toLowerCase() === StaticRoom) {
      msgFromClientDto.text = 'has connected';
      msgFromClientDto.typeOfMessage = TypeOfMessage.connected;
      await this.handleMsgServerToRoom(server, msgFromClientDto);
    } else {
      msgFromClientDto.text = `has joined #${roomLowerCase}`;
      msgFromClientDto.typeOfMessage = TypeOfMessage.joined;
      await this.handleMsgServerToRoom(server, msgFromClientDto);
    }
  }

  async handleLeaveRoom(server: Server, client: Socket, msgFromClientDto: MsgFromClientDto) {
    const roomLowerCase = msgFromClientDto.roomName.toLowerCase()

    // Sends room for frontend to remove from there, remove user from Chat-room and from
    // Chat-User connected rooms. client.leave has to be the last, because if it's first
    // client.emit('leftRoom') won't work on client
    client.emit('leftRoom', roomLowerCase);
    await this.removeLoggedUserFromChatRoom(client, msgFromClientDto.roomName, msgFromClientDto.name);
    await this.chatUserService.removeConnectedRoomFromChatUser(msgFromClientDto.name, roomLowerCase);
    client.leave(roomLowerCase);

    // Sends message to all clients in the room the client just left with left room message.
    msgFromClientDto.text = `has left #${roomLowerCase}`;
    msgFromClientDto.typeOfMessage = TypeOfMessage.left;
    await this.handleMsgServerToRoom(server, msgFromClientDto);

    // Send the Chat-Room updated to the entire room the client just left.
    const chatRoom = await this.findByRoomName(roomLowerCase);
    server.to(roomLowerCase).emit('chatRoomUpdate', chatRoom);

    // Send the Chat-User updated to the client.
    const user = await this.userService.findUserByUserName42(msgFromClientDto.name);
    client.emit('getChatUser', user.chatUser);
  }

  async handleMsgWelcome(server: Server, client: Socket, msgFromClientDto: MsgFromClientDto) {
    const roomNameCapitalized = msgFromClientDto.roomName.charAt(0).toUpperCase() + msgFromClientDto.roomName.slice(1);
    const typeOfMessage = TypeOfMessage.welcome;
    const name = msgFromClientDto.name;
    const roomName = msgFromClientDto.roomName;

    this.handleMsgServerToClient(client, { typeOfMessage, name, roomName, text: '-- -- --- -- - ---' });
    this.handleMsgServerToClient(client, { typeOfMessage, name, roomName, text: '-- - -- -- -- --' });
    this.handleMsgServerToClient(client, { typeOfMessage, name, roomName, text: '-- -- --- --- --- --- --- --- --- -- -- ---' });
    this.handleMsgServerToClient(client, { typeOfMessage, name, roomName, text: `-- -- Welcome to the #${roomNameCapitalized} Channel -- ---` });
    this.handleMsgServerToClient(client, { typeOfMessage, name, roomName, text: '-- Type /help to get a list of commands --' });
    this.handleMsgServerToClient(client, { typeOfMessage, name, roomName, text: '--- -- -- -- -- -- -- -- -- -- -- -- -- ---' });
    this.handleMsgServerToClient(client, { typeOfMessage, name, roomName, text: '-- - -- --' });
    this.handleMsgServerToClient(client, { typeOfMessage, name, roomName, text: '-- -' });
    this.handleMsgServerToClient(client, { typeOfMessage, name, roomName, text: '-' });
  }

  /*******************************************/
  /***    Chat-Room-Direct-Message Logic   ***/
  /*******************************************/
  async handleJoinRoomDirectMessage(client: Socket, user: User, roomNameHashed: string) {
    // Join room, add user to Chat-User ConnectedRoomDirectMessageToChatUser.
    client.join(roomNameHashed);
    await this.chatUserService.addConnectedRoomDirectMessageToChatUser(user.username42, roomNameHashed);

    // Send the Chat-Room-Direct-Message to the user.
    const chatRoomDirectMessage = await this.chatRoomDirectMessageService.findByRoomName(roomNameHashed);
    client.emit('joinedRoomDirectMessage', chatRoomDirectMessage);
  }

  async handleJoinRoomDirectMessageOtherClient(server: Server, msgFromClientDto: MsgFromClientDto) {
    const chatRoomDirectMessage = await this.chatRoomDirectMessageService.findByRoomName(msgFromClientDto.roomName);
    for (const index in chatRoomDirectMessage.participants) {
      if (chatRoomDirectMessage.participants[index] !== msgFromClientDto.name) {
        const otherUser = await this.userService.findUserByUserName42(chatRoomDirectMessage.participants[index]);

        // Check if user is blocked
        if (otherUser.chatUser.blockedUsers.includes(msgFromClientDto.name)) {
          return ;
        }

        // Get the user socket.
        const userSockets = await server.in(otherUser.chatUser.socketId).fetchSockets();
        const userSocket = userSockets.find(socket => socket.id.toString() === otherUser.chatUser.socketId);

        if (userSocket) {
          // Join room, add user to loggedUser of Chat-Room-Direct-Message and to Chat-User ConnectedRoomDirectMessageToChatUser.
          userSocket.join(msgFromClientDto.roomName);
          await this.chatUserService.addConnectedRoomDirectMessageToChatUser(otherUser.username42, msgFromClientDto.roomName);

          // Send the Chat-Room-Direct-Message to the user.
          userSocket.emit('joinedRoomDirectMessage', chatRoomDirectMessage);
        } else {
          await this.chatUserService.addConnectedRoomDirectMessageToChatUser(otherUser.username42, msgFromClientDto.roomName);
        }
        return ;
      }
    }

    for (const index in chatRoomDirectMessage.chatUser) {
      // Look for the chatUser that isn't the person sending the message.
      if (chatRoomDirectMessage.chatUser[index].user.username42 !== msgFromClientDto.name) {

        // Check if he already has the room open.
        if (!chatRoomDirectMessage.chatUser[index].user.chatUser.connectedRoomsDirectMessage.includes(msgFromClientDto.roomName)) {
          // Get the user socket.
          const userSockets = await server.in(chatRoomDirectMessage.chatUser[index].user.chatUser.socketId).fetchSockets();
          const userSocket = userSockets.find(socket => socket.id.toString() === chatRoomDirectMessage.chatUser[index].user.chatUser.socketId);

          // Join room, add user to loggedUser of Chat-Room-Direct-Message and to Chat-User ConnectedRoomDirectMessageToChatUser.
          userSocket.join(msgFromClientDto.roomName);
          await this.chatUserService.addConnectedRoomDirectMessageToChatUser(chatRoomDirectMessage.chatUser[index].user.username42, msgFromClientDto.roomName);

          // Send the Chat-Room-Direct-Message to the user.
          userSocket.emit('joinedRoomDirectMessage', chatRoomDirectMessage);
        }
        break;
      }
    }
  }

  async handleLeaveRoomDirectMessage(client: Socket, username: string, roomName: string) {
    await this.chatUserService.removeConnectedRoomDirectMessageFromChatUser(username, roomName);

    client.emit('leftRoomDirectMessage', roomName);
  }

  /*******************************************/
  /***            Commands Logic           ***/
  /*******************************************/
  handleMsgCommand(client: Socket, typeOfRoom: ChatRoomEnum, text: string) {
    const msgFromClientDto: MsgFromClientDto = {
      typeOfMessage: TypeOfMessage.command,
      typeOfRoom: typeOfRoom,
      text,
      roomName: '',
      name: '',
    }
    this.handleMsgServerToClient(client, msgFromClientDto);
  }

  async handleCommands(
    server: Server,
    client: Socket,
    msgFromClientDto: MsgFromClientDto,
  ) : Promise<boolean> {
    const msgTrimmed = msgFromClientDto.text.trim();
    if (msgTrimmed[0] !== '/') {
      return false;
    }

    const splitCommand: string[] = msgTrimmed.split(' ');
    const command = splitCommand[0];

    switch (command.toLowerCase()) {
      case CommandOptions.help:
        this.handleCommandHelp(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.create:
        await this.handleCommandCreate(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.join:
        await this.handleCommandJoin(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.leave:
        await this.handleCommandLeave(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.showdate:
        await this.handleCommandShowdate(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.showtime:
        await this.handleCommandShowtime(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.block:
        await this.handleCommandBlock(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.unblock:
        await this.handleCommandUnblock(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.blocklist:
        await this.handleCommandBlocklist(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.msg:
        await this.handleCommandMsg(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.invite:
        await this.handleCommandInvite(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.password:
        await this.handleCommandPassword(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.owner:
        await this.handleCommandOwner(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.admin:
        await this.handleCommandAdmin(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.remove:
        await this.handleCommandRemove(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.ownerlist:
        await this.handleCommandOwnerlist(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.adminlist:
        await this.handleCommandAdminlist(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.flaglist:
        await this.handleCommandFlaglist(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.kick:
        await this.handleCommandKick(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.ban:
        await this.handleCommandBan(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.unban:
        await this.handleCommandUnban(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.mute:
        await this.handleCommandMute(server, client, splitCommand, msgFromClientDto);
        break;
      case CommandOptions.unmute:
        await this.handleCommandUnmute(server, client, splitCommand, msgFromClientDto);
        break;
      default:
        const text = `${command.toLowerCase()} command doesn't exist`;
        this.handleMsgCommand(client, msgFromClientDto.typeOfRoom, text);

      return true;
    }
  }

  handleCommandHelp(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command] = splitCommand;

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- Command: /help');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- It shows a list of avaliable commands.' );
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- Client commands can be used anywhere.');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- Channel commands should be used in the channel you want.');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 1) {
      const text = `${command.toLowerCase()} format is incorrect - type '/help --help' for more info`
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- User commands:');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- /help - Shows this list');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /create 'status' 'roomname' 'password' - Creates a new channel.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /join 'roomname' 'password' - Joins a channel.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /leave - Leaves a channel.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /block 'username'- Stops receiving messages from a user and hides all their messages.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /unblock 'username'- Unblocks a user and shows all their messages again.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- /blocklist - Shows a list of blocked users.');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /msg 'username'- Starts a conversation with the user.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /showDate'- Turns on/off the date on the messages.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /showTime'- Turns on/off the time on the messages.");

    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- Room commands:');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /invite 'username' - Sends an invitation to a game.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /password 'password' - Sets a password to the channel.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /owner 'username' - Gives owner to user.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /admin 'username' - Gives administrator to user.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /remove 'username' - Removes administrator status.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /ban 'username' 'reason' 'time' - Bans user for x seconds.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /unban 'username' 'time' - Unbans user.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /mute 'username' 'time' - Mutes user for x seconds.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /unmute 'username' 'time' - Unmutes user.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- /kick 'username' - Kicks user.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- /ownerlist - Name of the owner.');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- /adminlist - List of admins.');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- /flaglist - list of flagged users.');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "- For more detailed help on commands type '--help' after command.");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "- Example: '/password --help'");
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
  }

  async handleCommandCreate(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, status, roomToCreate, password] = splitCommand;

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /create 'status' 'roomname' 'password'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- It creates a new channel if it doesn't exist and sets you as the owner.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- When you have ownership of a channel you can use commands that only owners and admins can use.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- The command has 3 possible params, 'status', 'room-name' and 'password'.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Status: param can be either 'public' or 'private', private rooms won't be shown in the channel list.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Roomname: has to be unique.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Password: field can be empty.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length < 3 || splitCommand.length > 4) {
      const text = `${command.toLowerCase()} format is incorrect - type '/create --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (status.toLowerCase() !== 'private' && status.toLowerCase() !== 'public') {
      const text = `${command.toLowerCase()} should be either private or public - type '/create --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (roomToCreate === '') {
      const text = `roomname should not be empty or contain empty spaces`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomToCreate);
    if (chatRoom) {
      const text = `${roomToCreate.toLowerCase()} room already exists`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    let hashedPassword: string;
    if (password !== undefined) {
      hashedPassword = await this.getHashString(password);
    }

    const chatRoomDto: CreateChatRoomDto = {
      roomName: roomToCreate.toLowerCase(),
      password: password === undefined ? '' : hashedPassword,
      status: status.toLowerCase() === 'public' ? ChatRoomEnum.public : ChatRoomEnum.private,
      owner: msgFromClientDto.name,
    }

    await this.create(chatRoomDto);
    await this.handleJoinRoom(server, client, {name: msgFromClientDto.name, roomName: roomToCreate, text: ''});

    if (chatRoomDto.status === ChatRoomEnum.public) {
      const publicChatRooms = await this.findAllPublicRooms();
      if (publicChatRooms) {
        const publicRooms: string[] = [];
        for (const index in publicChatRooms) {
          publicRooms.push(publicChatRooms[index].roomName);
        }
        server.emit('getPublicRooms', publicRooms);
      }
    }
  }

  async handleCommandJoin(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, roomToJoin, password] = splitCommand;

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /join 'roomname' 'password'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- It joins a channel if it exists.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You can leave the password field empty if the room doesn't have a password.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- If you are the owner or an admin of the channel you don't need to input a password.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length < 2 || splitCommand.length > 3) {
      const text = `${command.toLowerCase()} format is incorrect - type '/join --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomToJoin.toLowerCase());
    if (!chatRoom) {
      const text = `#${roomToJoin} room doesn't exist`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    let hashedPassword: string;
    if (password !== undefined) {
      hashedPassword = await this.getHashString(password);
    }
    // Checks if password matches.
    if (chatRoom.password && chatRoom.password !== hashedPassword) {
      // Check if it's owner or admin, if it is, it doesn't need password.
      if (chatRoom.owner !== msgFromClientDto.name &&
          !chatRoom.admins.includes(msgFromClientDto.name)) {
            const text = `incorrect password for room #${roomToJoin}`;
            this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
            return;
      }
    }

    for (const index in chatRoom.loggedUsers) {
      if (chatRoom.loggedUsers[index].name === msgFromClientDto.name) {
        const text = `you're already in #${roomToJoin}`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        return;
      }
    }

    await this.handleJoinRoom(server, client, {name: msgFromClientDto.name, roomName: roomToJoin.toLowerCase(), text: ''});
  }

  async handleCommandShowdate(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command] = splitCommand;

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /showdate");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- It turns on and off the date you see in the messages.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 1) {
      const text = `${command.toLowerCase()} format is incorrect - type '/showdate --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const user = await this.userService.findUserByUserName42(msgFromClientDto.name);
    if (user) {
      const createChatUserDto: UpdateChatUserDto = {
        showDate: user.chatUser.showDate === true ? false : true,
      }
      await this.chatUserService.update(user.chatUserId, createChatUserDto);

      const updatedUser = await this.userService.findUserByUserName42(msgFromClientDto.name);
      client.emit('getChatUser', updatedUser.chatUser);
    }
  }

  async handleCommandShowtime(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command] = splitCommand;

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /showtime");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- It turns on and off the time you see in the messages.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 1) {
      const text = `${command.toLowerCase()} format is incorrect - type '/showtime --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const user = await this.userService.findUserByUserName42(msgFromClientDto.name);
    if (user) {
      const createChatUserDto: UpdateChatUserDto = {
        showTime: user.chatUser.showTime === true ? false : true,
      }
      await this.chatUserService.update(user.chatUserId, createChatUserDto);

      const updatedUser = await this.userService.findUserByUserName42(msgFromClientDto.name);
      client.emit('getChatUser', updatedUser.chatUser);
    }
  }

  async handleCommandBlock(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, userToBlock] = splitCommand;

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /block 'username'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Blocks user.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You will not be able to see their messages again.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- They will keep seeing your messages..");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/block --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (!await this.userService.findUserByUserName42(userToBlock)) {
      const text = `there is no user named ${userToBlock}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (msgFromClientDto.name === userToBlock) {
      const text = `you can't block yourself`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const user = await this.userService.findUserByUserName42(msgFromClientDto.name);
    if (!user.chatUser.blockedUsers.includes(userToBlock)) {
      this.friendRelationService.blockByUserName42(user.id, userToBlock);

      user.chatUser.blockedUsers.push(userToBlock);
      await this.chatUserService.update(user.chatUser.id, user.chatUser);

      const text = `user ${userToBlock} is now blocked and you won't be able to see his messages`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);

      const updatedUser = await this.userService.findUserByUserName42(msgFromClientDto.name);
      client.emit('getChatUser', updatedUser.chatUser);
      return;
    } else {
      const text = `user ${userToBlock} is already blocked`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }
  }

  async handleCommandUnblock(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, userToUnblock] = splitCommand;

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /unblock 'username'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Unblocks user.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You will be able to see their messages again.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/unblock --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (!await this.userService.findUserByUserName42(userToUnblock)) {
      const text = `there is no user named ${userToUnblock}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (msgFromClientDto.name === userToUnblock) {
      const text = `you can't block yourself`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const user = await this.userService.findUserByUserName42(msgFromClientDto.name);
    if (!user.chatUser.blockedUsers.includes(userToUnblock)) {
      const text = `user ${userToUnblock} is not blocked`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;

    } else {
      this.friendRelationService.unblockByUserName42(user.id, userToUnblock);

      const index = user.chatUser.blockedUsers.indexOf(userToUnblock, 0);
      if (index > -1) {
        user.chatUser.blockedUsers.splice(index, 1);
      }
      await this.chatUserService.update(user.chatUser.id, user.chatUser);

      const text = `user ${userToUnblock} is now unblocked and you will be able to see his messages again`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);

      const updatedUser = await this.userService.findUserByUserName42(msgFromClientDto.name);
      client.emit('getChatUser', updatedUser.chatUser);
      return;
    }
  }

  async handleCommandBlocklist(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command] = splitCommand;

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /blocklist");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Shows a list of blocked users.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 1) {
      const text = `${command.toLowerCase()} format is incorrect - type '/blocklist --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const user = await this.userService.findUserByUserName42(msgFromClientDto.name);

    if (user.chatUser.blockedUsers.length === 0) {
      const text = `you have no users blocked`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return ;
    } else {
      const text = `the users blocked are ${user.chatUser.blockedUsers.join(', ')}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }
  }

  async handleCommandMsg(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, userToMsg] = splitCommand;

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /msg 'username'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Starts a private conversation withe the username.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- User needs to exist before you can start a conversation.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You can't message yourself.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/msg --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (!await this.userService.findUserByUserName42(userToMsg)) {
      const text = `there is no user named ${userToMsg}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (msgFromClientDto.name === userToMsg) {
      const text = `you can't start a conversation with yourself`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const user1 = await this.userService.findByUsername42Relations(msgFromClientDto.name);
    if (user1.chatUser.blockedUsers.includes(userToMsg)) {
      const text = `you can't start a conversation with someone you've blocked`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    // Get the hash of the roomname.
    // RoomName should be hashed because it's a room, and users shouldn't '/join' it.
    // RoomName is 'user1.user2', but char compared.
    // Should be 'daniela.olbrien' and not 'olbrien.daniela'.
    const roomNameLocale = msgFromClientDto.name.localeCompare(userToMsg) < 0 ? msgFromClientDto.name + '.' + userToMsg :
                                                                               userToMsg + '.' + msgFromClientDto.name;
    const roomNameHashed = await this.getHashString(roomNameLocale);

    // Check if there is already an active conversation between these two
    // by checking if the roomNameHashed exists.
    const chatRoomDirectMessage = await this.chatRoomDirectMessageService.findByRoomName(roomNameHashed);
    if (chatRoomDirectMessage) {
      await this.handleJoinRoomDirectMessage(client, user1, roomNameHashed);
      return ;
    } else {
      const chatRoomDirectMessage = await this.chatRoomDirectMessageService.create({
        roomName: roomNameHashed,
        participants: [msgFromClientDto.name, userToMsg],
      });

      if (typeof chatRoomDirectMessage === 'object') {
        const user2 = await this.userService.findByUsername42Relations(userToMsg);

        user1.chatUser.chatRoomDirectMessage.push(chatRoomDirectMessage);
        user2.chatUser.chatRoomDirectMessage.push(chatRoomDirectMessage);

        await this.chatUserService.save(user1.chatUser);
        await this.chatUserService.save(user2.chatUser);

        const userUpdated1 = await this.userService.findByUsername42Relations(msgFromClientDto.name);
        const userUpdated2 = await this.userService.findByUsername42Relations(userToMsg);

        chatRoomDirectMessage.chatUser = [userUpdated1.chatUser, userUpdated2.chatUser];

        await this.chatRoomDirectMessageService.save(chatRoomDirectMessage);

        await this.handleJoinRoomDirectMessage(client, userUpdated1, roomNameHashed);
        return ;
      }
    }
  }

  async handleCommandInvite(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, userToInvite] = splitCommand;

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /invite 'username'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Starts a public game with the username.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- User needs to exist before you can start a game.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You can't start a game on private message.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/invite --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (!await this.userService.findUserByUserName42(userToInvite)) {
      const text = `there is no user named ${userToInvite}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (msgFromClientDto.name === userToInvite) {
      const text = `you can't game with yourself`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const user1: User = await this.userService.findUserByUserName42(msgFromClientDto.name);
    const user2: User = await this.userService.findUserByUserName42(userToInvite);
    if (user1.chatUser.blockedUsers.includes(userToInvite)) {
      const text = `you can't start a game with someone you've blocked`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const pongGame = await this.pongGameService.create({
      status: GameStatus.STARTING,
      scoreOne: 0,
      scoreTwo: 0,
      userOneId: user1.id,
      userTwoId: user2.id,
      isLadder: false,
    })
    if (!pongGame) {
      const text = `pong game could not be created`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    msgFromClientDto.typeOfMessage = TypeOfMessage.gameInvite;
    msgFromClientDto.pongGameChat = {
      id: pongGame.id,
      status: pongGame.status,
      usernameOne: user1.username42,
      usernameTwo: user2.username42,
      winnerUsername: '',
    }

    this.handleMsgClientToRoom(server, client, msgFromClientDto);
  }

  async handleCommandLeave(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command] = splitCommand;


    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /leave");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- It leaves the channel you are in.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You don't lose your status in that channel, whether it's a flag or status.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You cannot leave the 'Lounge' channel.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length != 1) {
      const text = `${command.toLowerCase()} format is incorrect - type '/leave --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      await this.handleLeaveRoomDirectMessage(client, msgFromClientDto.name, msgFromClientDto.roomName);
      return;
    }

    if (msgFromClientDto.roomName === StaticRoom) {
      const text = `you cannot leave #${StaticRoom}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    await this.handleLeaveRoom(server, client, msgFromClientDto);
  }

  async handleCommandPassword(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, password] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /password 'password'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- It changes the password of the channel.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You need to be inside the channel.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- It will only work if you are the server owner or admin.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- If you leave the 'password' field empty, the password will be removed from the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length < 1 || splitCommand.length > 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/password --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.owner !== msgFromClientDto.name &&
      !chatRoom.admins.includes(msgFromClientDto.name)) {
        const text = `you need to be the owner or an admin to change the password of #${roomName}`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        return;
    }

    let hashedPassword: string;
    if (password !== undefined) {
      hashedPassword = await this.getHashString(password);
    }
    const updateChatRoomDto: UpdateChatRoomDto = { password: password === undefined ? '' : hashedPassword };
    await this.update(msgFromClientDto.roomName, updateChatRoomDto);

    msgFromClientDto.typeOfMessage = TypeOfMessage.info;
    msgFromClientDto.text = `room password has been changed by ${msgFromClientDto.name}`;
    msgFromClientDto.name = '';
    this.handleMsgServerToRoom(server, msgFromClientDto);
  }

  async handleCommandOwner(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, username] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /owner 'username'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Gives ownership to the username.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You need to be the owner of the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- After giving ownership you will become admin for that room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/owner --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.owner !== msgFromClientDto.name) {
      const text = `you need to be the owner to give ownership to another user`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (chatRoom.owner === username) {
      const text = `you are already the owner of #${roomName}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (!await this.userService.findUserByUserName42(username)) {
      const text = `there is no user named ${username}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const newAdmins: string[] = [msgFromClientDto.name];
    for (const index in chatRoom.admins) {
      if (chatRoom.admins[index] !== username) {
        newAdmins.push(chatRoom.admins[index]);
      }
    }
    const updateChatRoomDto: UpdateChatRoomDto = {
      owner: username,
      admins: newAdmins,
    };

    const newChatRoom = await this.update(msgFromClientDto.roomName, updateChatRoomDto);
    server.to(msgFromClientDto.roomName).emit('chatRoomUpdate', newChatRoom);

    msgFromClientDto.name = '';
    msgFromClientDto.typeOfMessage = TypeOfMessage.info;
    msgFromClientDto.text = `the owner of this room has been changed to ${username}`;
    this.handleMsgServerToRoom(server, msgFromClientDto);
  }

  async handleCommandAdmin(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, username] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /admin 'username'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Gives administrator rights to the username.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You need to be the owner or an admin of the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/admin --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.owner !== msgFromClientDto.name &&
      !chatRoom.admins.includes(msgFromClientDto.name)) {
        const text = `you need to be the owner or an admin to give admin to another user`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        return;
    }

    if (!await this.userService.findUserByUserName42(username)) {
      const text = `there is no user named ${username}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (msgFromClientDto.name === username) {
      const text = `you can't give yourself admin status`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (chatRoom.owner === username || chatRoom.admins.includes(username)) {
      const text = `that user is already the owner or a admin`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    chatRoom.admins.push(username);
    const updateChatRoomDto: UpdateChatRoomDto = {
      admins: chatRoom.admins,
    };

    const newChatRoom = await this.update(msgFromClientDto.roomName, updateChatRoomDto);
    server.to(msgFromClientDto.roomName).emit('chatRoomUpdate', newChatRoom);

    msgFromClientDto.name = '';
    msgFromClientDto.typeOfMessage = TypeOfMessage.info;
    msgFromClientDto.text = `the user ${username} has been added as an admin`;
    this.handleMsgServerToRoom(server, msgFromClientDto);
  }

  async handleCommandRemove(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, userToRemove] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /remove 'username'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Removes the user from being an administrator.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You need to be the owner or an admin of the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/remove --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.owner !== msgFromClientDto.name &&
      !chatRoom.admins.includes(msgFromClientDto.name)) {
        const text = `you need to be the owner or an admin to remove admin from another user`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        return;
    }

    if (!await this.userService.findUserByUserName42(userToRemove)) {
      const text = `there is no user named ${userToRemove}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (chatRoom.owner === userToRemove) {
      const text = `you can't remove the owner`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (!chatRoom.admins.includes(userToRemove)) {
      const text = `there is no user named ${userToRemove} in the admin list`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const newAdmins: string[] = [];
    for (const index in chatRoom.admins) {
      if (chatRoom.admins[index] !== userToRemove) {
        newAdmins.push(chatRoom.admins[index]);
      }
    }
    const updateChatRoomDto: UpdateChatRoomDto = {
      admins: newAdmins,
    };

    const newChatRoom = await this.update(msgFromClientDto.roomName, updateChatRoomDto);
    server.to(msgFromClientDto.roomName).emit('chatRoomUpdate', newChatRoom);

    msgFromClientDto.name = '';
    msgFromClientDto.typeOfMessage = TypeOfMessage.info;
    msgFromClientDto.text = `the user ${userToRemove} has been removed as an admin`;
    this.handleMsgServerToRoom(server, msgFromClientDto);
  }

  async handleCommandOwnerlist(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /ownerlist");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Gives the name of the owner of the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 1) {
      const text = `${command.toLowerCase()} format is incorrect - type '/ownerlist --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomName);
    if (!chatRoom.owner) {
      const text = `this room has no owner`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const text = `the owner of this room is ${chatRoom.owner}`;
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
  }

  async handleCommandAdminlist(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /adminlist");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Gives the list of the admins of this room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 1) {
      const text = `${command.toLowerCase()} format is incorrect - type '/adminlist --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.admins.length === 0 ) {
      const text = `this room has no admins`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const text = `the admins of this room are ${chatRoom.admins.join(', ')}`;
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
  }

  async handleCommandFlaglist(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /flaglist");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Gives a list of the flagged users of the channel.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You need to be the owner or an admin of the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 1) {
      const text = `${command.toLowerCase()} format is incorrect - type '/flaglist --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.owner !== msgFromClientDto.name &&
      !chatRoom.admins.includes(msgFromClientDto.name)) {
        const text = `you need to be the owner or an admin to see the list of flagged users`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        return;
    }

    if (Object.values(chatRoom.flaggedUsers).length !== 0) {
      for (const index in chatRoom.flaggedUsers) {
        let text = '[' + index + ']';
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        text = `- ${chatRoom.flaggedUsers[index].name} has been ${chatRoom.flaggedUsers[index].status} by ${chatRoom.flaggedUsers[index].flaggedBy}`
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        if (chatRoom.flaggedUsers[index].status === FlaggedStatusEnum.banned) {
          text = `- for the reason: ${chatRoom.flaggedUsers[index].reason}`;
          this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        }
        text = `- start time: "${chatRoom.flaggedUsers[index].timeStart}" end time: "${chatRoom.flaggedUsers[index].timeEnd}"`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      }
      return;
    }

    const text = `there are no flagged users in this room`;
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
    return;
  }

  async handleCommandKick(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, userToKick] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /kick 'username'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Kicks user of the channel.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Must be owner or admin.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You can't kick the owner or yourself.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You can kick other admins.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/kick --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    let chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.owner !== msgFromClientDto.name &&
      !chatRoom.admins.includes(msgFromClientDto.name)) {
        const text = `you need to be the owner or an admin to kick another user`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        return;
    }

    if (!await this.userService.findUserByUserName42(userToKick)) {
      const text = `there is no user named ${userToKick}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (msgFromClientDto.name === userToKick) {
      const text = `you can't kick yourself out of the room`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (chatRoom.owner === userToKick) {
      const text = `you can't kick the owner of the room`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    let isUserInRoom = false;
    for (const index in chatRoom.loggedUsers) {
      if (chatRoom.loggedUsers[index].name === userToKick) {
        isUserInRoom = true;
        break;
      }
    }
    if (isUserInRoom === false) {
      const text = `${userToKick} is not in the room`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const userObj: UserAndSocketIdDto = Object.values(chatRoom.loggedUsers).find((obj) => { return obj.name === userToKick });
    if (userObj) {
      const userSockets = await server.in(userObj.clientId).fetchSockets();
      const userSocket = userSockets.find(socket => socket.id.toString() === userObj.clientId);

      // Similar to handleLeaveRoom
      userSocket.emit('leftRoom', roomName);
      await this.removeLoggedUserFromChatRoom(client, roomName, userObj.name);
      await this.chatUserService.removeConnectedRoomFromChatUser(userObj.name, roomName);
      userSocket.leave(roomName);

      // Sends message to the client that got kicked.
      const msgDto: MsgDto = {
        theDate: this.getDate(DateOptions.onlyDate),
        theTime: this.getDate(DateOptions.onlyTime),
        text: `you've been kicked from #${roomName}`,
        typeOfMessage: TypeOfMessage.kicked,
        fromRoom: StaticRoom,
      };
      userSocket.emit('msgToClient', msgDto);

      // Sends message to all clients in the room the client was kicked of the room.
      msgFromClientDto.name = '';
      msgFromClientDto.roomName = roomName;
      msgFromClientDto.text = `${userToKick} has been kicked`;
      msgFromClientDto.typeOfMessage = TypeOfMessage.kicked;
      await this.handleMsgServerToRoom(server, msgFromClientDto);

      // Send the Chat-Room updated to the entire room the client just left.
      chatRoom = await this.findByRoomName(roomName);
      server.to(roomName).emit('chatRoomUpdate', chatRoom);

      // Send the Chat-User updated to the client.
      const user = await this.userService.findUserByUserName42(userObj.name);
      userSocket.emit('getChatUser', user.chatUser);
    }
  }

  async handleCommandBan(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, userToBan, ...rest] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    // This separates the commands correctly with quotes and all.
    let reason = undefined
    let time = undefined;
    if (rest.length === 1) {
      reason = rest[0];
      splitCommand = [command, userToBan, reason];
    } else if (rest.length > 1) {
      if (rest[rest.length-1][rest[rest.length-1].length - 1] !== '"') {
        time = rest[rest.length - 1];
        rest.pop();
      }
      reason = rest.join(' ');
      splitCommand = [command, userToBan, reason, time];
    }

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /ban 'username' 'reason' 'time'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Bans a user from the channel for a determined time.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- If the user is in the channel, he will get kicked.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You can ban anyone even if it's not on the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You can't ban the owner or self.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Reason has to be filled.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Reason needs to be in quotes \"insert reason here\".");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- If Time is left empty the user will be banned for 42 years.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Time is in seconds, and has to be in form of number.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You need to be the owner or an admin to ban people.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length < 3 || splitCommand.length > 4) {
      const text = `${command.toLowerCase()} format is incorrect - type '/ban --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (reason[0] !== '"' || reason[reason.length-1] !== '"') {
      const text = `${command.toLowerCase()} reason needs to be in quotes \"insert reason here\"`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    // Removes the Quotes from the reason.
    reason = reason.replace(/\"/g, '')

    const chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.owner !== msgFromClientDto.name &&
      !chatRoom.admins.includes(msgFromClientDto.name)) {
        const text = `you need to be the owner or an admin to ban another user`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        return;
    }

    const user = await this.userService.findUserByUserName42(userToBan);
    if (!user) {
      const text = `there is no user named ${userToBan}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (msgFromClientDto.name === userToBan) {
      const text = `you can't ban yourself out of the room`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (chatRoom.owner === userToBan) {
      const text = `you can't ban the owner of the room`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (time !== undefined && Number.isNaN(+time)) {
      const text = `time is not correct. Insert a valid number`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (+time < 0) {
      const text = `can't ban for negative seconds`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    // Add userToBan to the flaggedUsers.
    const flaggedUserDto: FlaggedUserDto = {
      name: userToBan,
      flaggedBy: msgFromClientDto.name,
      status: FlaggedStatusEnum.banned,
      reason: reason,
    }
    time = time === undefined ? -1 : Math.round(+time)
    await this.addFlaggedUserToChatRoom(server, chatRoom, flaggedUserDto, +time);

    // Kicks user of the room if he is there.
    for (const index in chatRoom.loggedUsers) {
      if (chatRoom.loggedUsers[index].name === userToBan) {
        const newSplitCommand: string[] = ['/kick', userToBan];
        await this.handleCommandKick(server, client, newSplitCommand, msgFromClientDto);
        break;
      }
    }

    // Sends message to all clients in the room the client was banned of the room.
    const timeText = `${String(time === -1 ? ' for 42 years' : ' for ' + Math.round(+time) + ' seconds')}`;
    const newMsgFromClientDto: MsgFromClientDto = {
      name: userToBan,
      roomName: roomName,
      text: `has been banned ${timeText} for the reason: ${reason}`,
      typeOfMessage: TypeOfMessage.banned,
    }
    await this.handleMsgServerToRoom(server, newMsgFromClientDto);

    // Send message to client that he was banned
    const userSockets = await server.in(user.chatUser.socketId).fetchSockets();
    const userSocket = userSockets.find(socket => socket.id.toString() === user.chatUser.socketId);
    if (userSocket) {
      const msgDto: MsgDto = {
        theDate: this.getDate(DateOptions.onlyDate),
        theTime: this.getDate(DateOptions.onlyTime),
        text: `you've been banned from #${chatRoom.roomName} by ${msgFromClientDto.name} for the reason: ${reason}`,
        typeOfMessage: TypeOfMessage.banned,
        fromRoom: StaticRoom,
      };
      userSocket.emit('msgToClient', msgDto);
    }
  }

  async handleCommandUnban(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, userToUnban] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /unban 'username'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Removes the user from being banned.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You need to be the owner or an admin of the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/unban --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.owner !== msgFromClientDto.name &&
      !chatRoom.admins.includes(msgFromClientDto.name)) {
        const text = `you need to be the owner or an admin to unban another user`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        return;
    }

    if (!await this.userService.findUserByUserName42(userToUnban)) {
      const text = `there is no user named ${userToUnban}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    // Check if user is banned, and if it is it removes the flag.
    // If not, moves along.
    for (const index in chatRoom.flaggedUsers) {
      if (chatRoom.flaggedUsers[index].name === userToUnban &&
          chatRoom.flaggedUsers[index].status === FlaggedStatusEnum.banned) {
            await this.removeFlaggedUserFromChatRoom(server, chatRoom, userToUnban, msgFromClientDto.name, FlaggedStatusEnum.banned);
            return;
      }
    }

    const text = `the user ${userToUnban} is not banned`;
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
    return;
  }

  async handleCommandMute(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, userToMute, time] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /mute 'username' 'time'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Mutes the user from speaking in the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You need to provide the username.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Time is in seconds.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- If no time is provided, the user is muted for 42 years.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You need to be the owner or an admin of the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length < 2 || splitCommand.length > 3) {
      const text = `${command.toLowerCase()} format is incorrect - type '/mute --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.owner !== msgFromClientDto.name &&
      !chatRoom.admins.includes(msgFromClientDto.name)) {
        const text = `you need to be the owner or an admin to ban another user`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        return;
    }

    if (!await this.userService.findUserByUserName42(userToMute)) {
      const text = `there is no user named ${userToMute}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (msgFromClientDto.name === userToMute) {
      const text = `you can't mute yourself`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (chatRoom.owner === userToMute) {
      const text = `you can't mute the owner of the room`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (time !== undefined && Number.isNaN(+time)) {
      const text = `time is not correct. Insert a valid number`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (+time < 0) {
      const text = `can't mute for negative seconds`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    // Add userToMute to the flaggedUsers.
    const flaggedUserDto: FlaggedUserDto = {
      name: userToMute,
      flaggedBy: msgFromClientDto.name,
      status: FlaggedStatusEnum.muted,
      reason: '',
    }
    const newTime = time === undefined ? -1 : Math.round(+time)
    await this.addFlaggedUserToChatRoom(server, chatRoom, flaggedUserDto, +newTime);

    // Sends message to all clients in the room the client was muted.
    const timeText = `${String(newTime === -1 ? ' for 42 years' : ' for ' + Math.round(+time) + ' seconds')}`;
    const newMsgFromClientDto: MsgFromClientDto = {
      name: userToMute,
      roomName: roomName,
      text: `has been muted ${timeText}`,
      typeOfMessage: TypeOfMessage.muted,
    }
    await this.handleMsgServerToRoom(server, newMsgFromClientDto);
  }

  async handleCommandUnmute(
    server: Server,
    client: Socket,
    splitCommand: string[],
    msgFromClientDto: MsgFromClientDto,
  ) {
    const [command, userToUnmute] = splitCommand;
    const roomName = msgFromClientDto.roomName.toLowerCase();

    if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
      const text = `you can only use User commands here - type '/help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    if (splitCommand.length === 2 && splitCommand[1].toLowerCase() === '--help') {
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- -- -- ---');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- -- Command: /unmute 'username'");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-- -- ');
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- Removes the user from being muted.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, "-- You need to be the owner or an admin of the room.");
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, '-');
      return;
    }

    if (splitCommand.length !== 2) {
      const text = `${command.toLowerCase()} format is incorrect - type '/unmute --help' for more info`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    const chatRoom = await this.findByRoomName(roomName);
    if (chatRoom.owner !== msgFromClientDto.name &&
      !chatRoom.admins.includes(msgFromClientDto.name)) {
        const text = `you need to be the owner or an admin to unban another user`;
        this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
        return;
    }

    if (!await this.userService.findUserByUserName42(userToUnmute)) {
      const text = `there is no user named ${userToUnmute}`;
      this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
      return;
    }

    // Check if user is muted, and if it is it removes the flag.
    // If not, moves along.
    for (const index in chatRoom.flaggedUsers) {
      if (chatRoom.flaggedUsers[index].name === userToUnmute &&
          chatRoom.flaggedUsers[index].status === FlaggedStatusEnum.muted) {
            await this.removeFlaggedUserFromChatRoom(server, chatRoom, userToUnmute, msgFromClientDto.name, FlaggedStatusEnum.muted);
            return;
      }
    }

    const text = `the user ${userToUnmute} is not muted`;
    this.handleMsgCommand(client,  msgFromClientDto.typeOfRoom, text);
    return;
  }

  /*******************************************/
  /***           messagesObj Logic         ***/
  /*******************************************/
  async addMessageToChatRoom(msgFromClientDto: MsgFromClientDto) {
    const chatRoom = await this.findByRoomName(msgFromClientDto.roomName);
    if (!chatRoom) {
      return `${msgFromClientDto.roomName} not found.`;
    }

    const theDate = this.getDate(DateOptions.onlyDate);
    const theTime = this.getDate(DateOptions.onlyTime);

    // Check user privilege
    let privilege;
    if (chatRoom.owner === msgFromClientDto.name) {
      privilege = MsgDtoPrivilege.owner;
    } else if (chatRoom.admins.includes(msgFromClientDto.name)) {
      privilege = MsgDtoPrivilege.admin;
    } else {
      privilege = MsgDtoPrivilege.user;
    }

    const msgDto: MsgDto = {
      typeOfMessage: msgFromClientDto.typeOfMessage,
      userColor: msgFromClientDto.userColor,
      theDate,
      theTime,
      name: msgFromClientDto.name,
      text: msgFromClientDto.text,
      privilege: privilege,
    };

    if (msgFromClientDto.typeOfMessage === TypeOfMessage.gameInvite) {
      msgDto.pongGameChat = msgFromClientDto.pongGameChat;
    }

    chatRoom.messagesObj.push(msgDto);
    await this.repo.update(chatRoom.id, chatRoom);
  }

  async addMessageToChatRoomDirectMessage(msgFromClientDto: MsgFromClientDto) {
    const chatRoomDirectMessage = await this.chatRoomDirectMessageService.findByRoomName(msgFromClientDto.roomName);
    if (!chatRoomDirectMessage) {
      return `${msgFromClientDto.roomName} not found.`;
    }

    const theDate = this.getDate(DateOptions.onlyDate);
    const theTime = this.getDate(DateOptions.onlyTime);

    const msgDto: MsgDto = {
      typeOfMessage: msgFromClientDto.typeOfMessage,
      userColor: msgFromClientDto.userColor,
      theDate,
      theTime,
      name: msgFromClientDto.name,
      text: msgFromClientDto.text,
    };

    chatRoomDirectMessage.messagesObj.push(msgDto);

    const updateChatRoomDirectMessageDto: UpdateChatRoomDirectMessageDto = {
      messagesObj: chatRoomDirectMessage.messagesObj,
    }
    await this.chatRoomDirectMessageService.update(chatRoomDirectMessage.roomName, updateChatRoomDirectMessageDto);
  }

  async cleanMessagesFromRoomName(roomName: string) {
    const chatRoom = await this.findByRoomName(roomName);
    if (!chatRoom) {
      return `${roomName} not found.`;
    }

    chatRoom.messagesObj = [];

    await this.repo.update(chatRoom.id, chatRoom);

    await this.findByRoomName(roomName);
  }

  /*******************************************/
  /***           loggedUsers Logic         ***/
  /*******************************************/
  async addLoggedUserToChatRoom(client: Socket, roomName: string, name: string) {
    const chatRoom = await this.findByRoomName(roomName);
    if (!chatRoom) {
      return `${roomName} not found.`;
    }

    const loggedUserDto: UserAndSocketIdDto = {
      name: name,
      clientId: client.id,
    };

    const length = Object.keys(chatRoom.loggedUsers).length;
    if (length === 0) {
      chatRoom.loggedUsers[0] = loggedUserDto;
      await this.repo.update(chatRoom.id, chatRoom);
      return;
    }

    for (const index in chatRoom.loggedUsers) {
      if (chatRoom.loggedUsers[index].name === name) {
        chatRoom.loggedUsers[index] = loggedUserDto;
        await this.repo.update(chatRoom.id, chatRoom);
        return;
      }
    }

    chatRoom.loggedUsers[length] = loggedUserDto;

    await this.repo.update(chatRoom.id, chatRoom);
  }

  async removeLoggedUserFromChatRoom(client: Socket, roomName: string, name: string) {
    const chatRoom = await this.findByRoomName(roomName);
    if (!chatRoom) {
      return `${roomName} not found.`;
    }

    const newLoggedUsers: UserAndSocketIdDto = {} as UserAndSocketIdDto;
    let i = 0;
    for (const index in chatRoom.loggedUsers) {
      if (chatRoom.loggedUsers[index].name !== name) {
        newLoggedUsers[i] = chatRoom.loggedUsers[index];
        i++;
      }
    }

    chatRoom.loggedUsers = newLoggedUsers;
    await this.repo.update(chatRoom.id, chatRoom);
  }

  async removeAllLoggedUsersFromAllRooms() {
    const chatRooms = await this.findAll();

    for (const chatRoomIndex in chatRooms) {
      for (const loggedUsersIndex in chatRooms[chatRoomIndex].loggedUsers) {
        delete chatRooms[chatRoomIndex].loggedUsers[loggedUsersIndex];
      }
      await this.repo.update(chatRooms[chatRoomIndex].id, chatRooms[chatRoomIndex]);
    }
  }

  /*******************************************/
  /***          flaggedUsers Logic         ***/
  /*******************************************/
  async scheduleFlaggedUserToRemove(server: Server, chatRoom: ChatRoom, flaggedUserDto: FlaggedUserDto, time: number) {
    // This function servers as a schedule, it adds a timeout for every flagged added.

    // Checks if the schedule exists, deletes if it exists.
    const scheduleName: string = chatRoom.roomName + flaggedUserDto.name + flaggedUserDto.status;
    try {
      this.schedulerRegistry.deleteTimeout(scheduleName);
    } catch {}

    // Creates a new schedule and removes the flag after x time.
    const callback = async () => {
      const newChatRoom = await this.findByRoomName(chatRoom.roomName);
      await this.removeFlaggedUserFromChatRoom(server, newChatRoom, flaggedUserDto.name, 'The Server', flaggedUserDto.status);
    };

    // Max setTimeout is max_int 32bit, meaning it can only setTimeout for 24 days.
    if (time * 1000 < 2147483647) {
      const timeout = setTimeout(callback, time * 1000);
      this.schedulerRegistry.addTimeout(scheduleName, timeout);
    }
  }

  async addFlaggedUserToChatRoom(server: Server, chatRoom: ChatRoom, flaggedUserDto: FlaggedUserDto, time: number) {
    flaggedUserDto.timeStart = moment().format("DD-MM-YYYY - kk:mm:ss");
    if (time === -1) {
      flaggedUserDto.timeEnd = moment().add(42, 'years').format("DD-MM-YYYY - kk:mm:ss");
    } else {
      flaggedUserDto.timeEnd = moment(flaggedUserDto.timeStart, "DD-MM-YYYY - kk:mm:ss").add(time, 'seconds').format("DD-MM-YYYY - kk:mm:ss");
    }

    const length = Object.keys(chatRoom.flaggedUsers).length;
    for (const index in chatRoom.flaggedUsers) {
      if (chatRoom.flaggedUsers[index].name === flaggedUserDto.name &&
          chatRoom.flaggedUsers[index].status === flaggedUserDto.status) {
            chatRoom.flaggedUsers[index] = flaggedUserDto;
            await this.repo.update(chatRoom.id, chatRoom);
            // Add user to a schedule so it gets deleted automatically after x time.
            if (time !== -1) {
              await this.scheduleFlaggedUserToRemove(server, chatRoom, flaggedUserDto, time);
            }
            return;
          }
    }

    chatRoom.flaggedUsers[length] = flaggedUserDto;
    await this.repo.update(chatRoom.id, chatRoom);
    // Add user to a schedule so it gets deleted automatically after x time.
    if (time !== -1) {
      await this.scheduleFlaggedUserToRemove(server, chatRoom, flaggedUserDto, time);
    }
  }

  async checkFlaggedUser(server: Server, client: Socket, msgFromClientDto: MsgFromClientDto, status: FlaggedStatusEnum) : Promise<boolean> {
    const roomLowerCase = msgFromClientDto.roomName.toLowerCase();

    // Check if the timeEnd of the flag has ended, if it has ended
    // it will return false, otherwise, it will send a message to the client.
    const chatRoom = await this.findByRoomName(roomLowerCase);
    for (const index in chatRoom.flaggedUsers) {
      if (chatRoom.flaggedUsers[index].name === msgFromClientDto.name &&
          chatRoom.flaggedUsers[index].status === status) {
            const timeNow = moment().format("DD-MM-YYYY - kk:mm:ss");
            if (moment(timeNow, 'DD-MM-YYYY - kk:mm:ss').isBefore(
                moment(chatRoom.flaggedUsers[index].timeEnd, 'DD-MM-YYYY - kk:mm:ss'))) {
              const timeLeft = moment(timeNow, 'DD-MM-YYYY - kk:mm:ss').to(moment(chatRoom.flaggedUsers[index].timeEnd, 'DD-MM-YYYY - kk:mm:ss'), true);

              if (status === FlaggedStatusEnum.banned) {
                msgFromClientDto.name = '';
                msgFromClientDto.typeOfMessage = TypeOfMessage.banned;
                msgFromClientDto.text = `you're banned from #${roomLowerCase} it will expire in ${timeLeft} for the reason: ${chatRoom.flaggedUsers[index].reason}`
                this.handleMsgServerToClient(client, msgFromClientDto);
                return true;
              } else if (status === FlaggedStatusEnum.muted) {
                msgFromClientDto.name = '';
                msgFromClientDto.typeOfMessage = TypeOfMessage.muted;
                msgFromClientDto.text = `you're muted from #${roomLowerCase} it will expire in ${timeLeft}`
                this.handleMsgServerToClient(client, msgFromClientDto);
                return true;
              }
            } else {
              return false;
            }
          }
    }
    return false;
  }

  async removeFlaggedUserFromChatRoom(server: Server, chatRoom: ChatRoom, userToRemoveFlag: string, username: string, status: FlaggedStatusEnum) {
    // Checks if the schedule exists, deletes if it exists.
    const scheduleName: string = chatRoom.roomName + userToRemoveFlag + status;
    try {
      this.schedulerRegistry.deleteTimeout(scheduleName);
    } catch {}

    const newFlaggedUsers: FlaggedUserDto = {} as FlaggedUserDto;
    let i = 0;
    for (const index in chatRoom.flaggedUsers) {
      if (chatRoom.flaggedUsers[index].name === userToRemoveFlag) {
        if (chatRoom.flaggedUsers[index].status === status) {
          continue;
        }
      }
      newFlaggedUsers[i] = chatRoom.flaggedUsers[index];
      i++;
    }

    chatRoom.flaggedUsers = newFlaggedUsers;
    await this.repo.update(chatRoom.id, chatRoom);

    // Send message to channel the client was unmuted / unbanned
    const newMsgFromClientDto: MsgFromClientDto = {
      name: userToRemoveFlag,
      roomName: chatRoom.roomName,
      text: status === FlaggedStatusEnum.muted ? `has been unmuted by ${username}` :
                                                 `has been unbanned by ${username}`,
      typeOfMessage: TypeOfMessage.info,
    }
    await this.handleMsgServerToRoom(server, newMsgFromClientDto);

    // Send message to client that he was unmuted / unbanned
    const user = await this.userService.findUserByUserName42(userToRemoveFlag);
    const userSockets = await server.in(user.chatUser.socketId).fetchSockets();
    const userSocket = userSockets.find(socket => socket.id.toString() === user.chatUser.socketId);

    // Check if he is in the room, if he is, sends the message to the room he's in
    // If not, send message to the 'lounge'.
    let fromRoom = StaticRoom;
    for (const count in chatRoom.loggedUsers) {
      if (chatRoom.loggedUsers[count].name === userToRemoveFlag) {
        fromRoom = chatRoom.roomName;
        break;
      }
    }

    if (userSocket) {
      const msgDto: MsgDto = {
        theDate: this.getDate(DateOptions.onlyDate),
        theTime: this.getDate(DateOptions.onlyTime),
        text: status === FlaggedStatusEnum.muted ? `you've been unmuted from #${chatRoom.roomName} by ${username}` :
                                                   `you've been unbanned from #${chatRoom.roomName} by ${username}`,
        typeOfMessage: TypeOfMessage.info,
        fromRoom: fromRoom,
      };
      userSocket.emit('msgToClient', msgDto);
    }
  }

  async addAllSchedulesBack(server: Server) {
    const chatRooms = await this.findAll();
    for (const index in chatRooms) {
      for (const i in chatRooms[index].flaggedUsers) {
        const flaggedUserDto: FlaggedUserDto = {
          name: chatRooms[index].flaggedUsers[i].name,
          status: chatRooms[index].flaggedUsers[i].status,
          flaggedBy: '',
          reason: '',
        }
        const timeNow = moment().format("DD-MM-YYYY - kk:mm:ss");
        let time: number = moment(chatRooms[index].flaggedUsers[i].timeEnd, 'DD-MM-YYYY - kk:mm:ss').diff(
          moment(timeNow, 'DD-MM-YYYY - kk:mm:ss'));
        time = time < 0 ? time = 0 : time / 1000;

        await this.scheduleFlaggedUserToRemove(server, chatRooms[index], flaggedUserDto, time);
      }
    }
  }

  /*******************************************/
  /***              hash Logic             ***/
  /*******************************************/
  async getHashString(unhashedString: string): Promise<string> {
    // Generate a Salt
    const salt = process.env.SALT;
    const hashSalt = (await this.scrypt(salt, salt, 8)) as Buffer;
    const hashSaltedToString = hashSalt.toString('hex');

    // Hash the salt and the unhashedString together
    const hash = (await this.scrypt(unhashedString, hashSaltedToString, 32)) as Buffer;

    // Join the hashed result and salt together
    const result = hashSaltedToString + '.' + hash.toString('hex');

    return result;
  }

  async compareStringToHash(unhashedString: string, hashedString: string): Promise<boolean> {
    // Split the hashedString in two
    const [salt, storedHash] = hashedString.split('.');

    // Hash the salt and the unashedString together
    const hash = (await this.scrypt(unhashedString, salt, 32)) as Buffer;

    // After converting unhashedString to hashed, now we compare both
    if (storedHash !== hash.toString('hex')) {
      return false;
    }
    return true;
  }
}
