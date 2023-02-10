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

@WebSocketGateway({ namespace: '/pong-queue', cors: true })
export class PongQueueGateway implements OnGatewayInit, OnGatewayConnection, OnGatewayDisconnect {
  /*******************************************/
  /***               General               ***/
  /*******************************************/
  @WebSocketServer()
  server: Server;

  constructor(
    private pongGameService: PongGameService,
  ) {
  }

  /*******************************************/
  /***           Lifecycle Hooks           ***/
  /*******************************************/
  async afterInit(server: Server) {
    console.log("[Queue Socket] Initializing");
  }

  async handleConnection(client: Socket) {
    console.log("[Queue Socket] Connected",  client.handshake.query.username);
  }

  async handleDisconnect(client: Socket) {
    await this.pongGameService.abandonQueue(client);
    console.log("[Queue Socket] Dissconnected", client.handshake.query.username);
  }

  /*******************************************/
  /***            Event Helpers            ***/
  /*******************************************/
  @SubscribeMessage('joinQueue')
  async joinQueue(
    @ConnectedSocket() client: Socket,
  ) {
    console.log("[Queue Socket] Join Queue", client.handshake.query.username);
    await this.pongGameService.joinQueue(client, client.handshake.query.username + "");
    this.pongGameService.matchPlayers();
  }

  @SubscribeMessage('abandonQueue')
  async abandonQueue(
    @ConnectedSocket() client: Socket,
  ) {
    console.log("[Queue Socket] Abandon Queue", client.handshake.query.username);    
    await this.pongGameService.abandonQueue(client);
  }


}