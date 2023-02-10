import {
  ConnectedSocket,
  MessageBody,
  OnGatewayConnection,
  OnGatewayDisconnect,
  OnGatewayInit,
  SubscribeMessage,
  WebSocketGateway,
  WebSocketServer,
} from '@nestjs/websockets';
import { Socket, Server } from 'socket.io';
import { ChatRoomService } from './chat-room.service';
import { MsgFromClientDto } from './dtos/msg-from-client.dto';
import { ClientTypingDto } from './dtos/client-typing.dto';
import { UserService } from 'src/users/user/user.service';
import { ChatRoomEnum } from './enums/chat-room.enums';
import { UseGuards } from '@nestjs/common';
import { Jwt2faAuthGuard } from 'src/auth/guards/jwt-2fa-auth.guard';
import { AuthService } from 'src/auth/auth.service';

@WebSocketGateway({
  namespace: '/chat',
  cors: {
    origin: '*',
    methods: ['GET', 'POST'],
    allowHeaders: ['content-type'],
  },
})
export class ChatRoomGateway
  implements OnGatewayInit, OnGatewayConnection, OnGatewayDisconnect
{
  /*******************************************/
  /***               General               ***/
  /*******************************************/
  @WebSocketServer()
  server: Server;

  constructor(
    private chatRoomService: ChatRoomService,
    private userService: UserService,
    private authService: AuthService,
  ) {}

  /*******************************************/
  /***           Lifecycle Hooks           ***/
  /*******************************************/
  async afterInit(server: Server) {
    this.chatRoomService.logMessage(
      this.chatRoomService.colorScheme.fgGreen,
      'ChatRoom Server Initialized',
    );

    // This is to create the main channel.
    await this.chatRoomService.createLoungeRoom();

    // This is case the server disconnects, it removes all users from all channels.
    await this.chatRoomService.removeAllLoggedUsersFromAllRooms();

    // This is case the server disconnects, it puts the schedules back of the flags.
    await this.chatRoomService.addAllSchedulesBack(server);
  }

  async handleConnection(client: Socket) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    this.chatRoomService.handleConnection(this.server, client);
  }

  async handleDisconnect(client: Socket) {
    this.chatRoomService.handleDisconnect(this.server, client);
  }

  /*******************************************/
  /***            Event Messages           ***/
  /*******************************************/
  @SubscribeMessage('msgFromClient')
  async handleMsgFromClient(
    @MessageBody() msgFromClientDto: MsgFromClientDto,
    @ConnectedSocket() client: Socket,
  ) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    const isCommand: boolean = await this.chatRoomService.handleCommands(
      this.server,
      client,
      msgFromClientDto,
    );

    if (isCommand === false) {
      if (
        msgFromClientDto.typeOfRoom === ChatRoomEnum.public ||
        msgFromClientDto.typeOfRoom === ChatRoomEnum.private
      ) {
        await this.chatRoomService.handleMsgClientToRoom(
          this.server,
          client,
          msgFromClientDto,
        );
      } else if (msgFromClientDto.typeOfRoom === ChatRoomEnum.directMessage) {
        await this.chatRoomService.handleMsgClientToClient(
          this.server,
          client,
          msgFromClientDto,
        );
      }
    }
  }

  /*******************************************/
  /***             Event Rooms             ***/
  /*******************************************/
  @SubscribeMessage('joinRoom')
  async handleJoinRoom(
    @MessageBody() msgFromClientDto: MsgFromClientDto,
    @ConnectedSocket() client: Socket,
  ) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    await this.chatRoomService.handleJoinRoom(
      this.server,
      client,
      msgFromClientDto,
    );
  }

  @SubscribeMessage('leaveRoom')
  async handleLeaveRoom(
    @MessageBody() msgFromClientDto: MsgFromClientDto,
    @ConnectedSocket() client: Socket,
  ) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    await this.chatRoomService.handleLeaveRoom(
      this.server,
      client,
      msgFromClientDto,
    );
  }

  /*******************************************/
  /***            Event Helpers            ***/
  /*******************************************/
  @SubscribeMessage('typing')
  handleTyping(
    @MessageBody() clientTypingDto: ClientTypingDto,
    @ConnectedSocket() client: Socket,
  ) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    client.broadcast.emit('typing', clientTypingDto);
  }
}
