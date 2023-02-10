import {
  SubscribeMessage,
  WebSocketGateway,
  OnGatewayInit,
  OnGatewayConnection,
  OnGatewayDisconnect,
  WebSocketServer,
  MessageBody,
  ConnectedSocket,
} from '@nestjs/websockets';
import { Socket, Server } from 'socket.io';
import { PongGameService } from './pong-game.service';
import { UserService } from 'src/users/user/user.service';
import { Direction } from './dto/msg-from-player.dto';
import { AuthService } from 'src/auth/auth.service';

@WebSocketGateway({ namespace: '/pong-game', cors: true })
export class PongGameGateway
  implements OnGatewayInit, OnGatewayConnection, OnGatewayDisconnect
{
  /*******************************************/
  /***               General               ***/
  /*******************************************/
  @WebSocketServer()
  server: Server;

  constructor(
    private pongGameService: PongGameService,
    private authService: AuthService, //private userService: UserService, //private games: {},
  ) {}

  /*******************************************/
  /***           Lifecycle Hooks           ***/
  /*******************************************/
  async afterInit(server: Server) {
    console.log('[PongGame Socket] Initializing');
    this.pongGameService.initializeGames(server);
  }

  async handleConnection(client: Socket) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    console.log('[PongGame Socket] Connected', client.handshake.query.gameId);
    client.join('pongGame' + client.handshake.query.gameId);
  }

  async handleDisconnect(client: Socket) {
    console.log(
      '[PongGame Socket] Disconnected',
      client.handshake.query.gameId,
    );
    client.leave('pongGame' + client.handshake.query.gameId);
    this.pongGameService.removeExpectator(client);
  }

  /*******************************************/
  /***            Event Helpers            ***/
  /*******************************************/
  @SubscribeMessage('logUser')
  async logUser(
    @MessageBody() username42: { name: string },
    @ConnectedSocket() client: Socket,
  ) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    await this.pongGameService.addExpectator(client, username42.name);
  }

  @SubscribeMessage('readyToStart')
  async readyToStartPongGame(@ConnectedSocket() client: Socket) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    this.pongGameService.readyToStartGame(client);
  }

  @SubscribeMessage('keyPressed')
  async handleKeyPressed(
    @MessageBody() message: { username42: string; keyPressed: string },
    @ConnectedSocket() client: Socket,
  ) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    await this.pongGameService.keyPressed(client, message);
  }

  @SubscribeMessage('keyReleased')
  async handleKeyRelesased(
    @MessageBody() message: { username42: string; keyReleased: string },
    @ConnectedSocket() client: Socket,
  ) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    await this.pongGameService.keyReleased(client, message);
  }

  @SubscribeMessage('abandonGame')
  async abandonGame(
    @MessageBody() message: { username42: string },
    @ConnectedSocket() client: Socket,
  ) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    await this.pongGameService.abandonGame(client, message);
  }
}
