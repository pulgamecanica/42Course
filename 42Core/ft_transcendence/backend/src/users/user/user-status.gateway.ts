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
import { UserService } from './user.service';
import { User, UserStatus } from './entities/user.entity';
import { AuthService } from 'src/auth/auth.service'

@WebSocketGateway({ namespace: '/status', cors: true })
export class UserStatusGateway implements OnGatewayInit, OnGatewayConnection, OnGatewayDisconnect {
  /*******************************************/
  /***               General               ***/
  /*******************************************/
  @WebSocketServer()
  server: Server;

  constructor(
    private userService: UserService,
    private authService: AuthService,
  ) {}

  /*******************************************/
  /***           Lifecycle Hooks           ***/
  /*******************************************/
  async afterInit(server: Server) {
    console.log("[Status Socket] Initializing");
    // EMIT USER STATUSES 10 seconds
    const interval = setInterval(async () => {
      this.server.emit('usersStatus', { users: await this.userService.findAllUsersStatus() })
    }, 10000);
  }

  async handleConnection(client: Socket) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    console.log("[Status Socket] Connected", client.handshake.query.username42);
    this.userService.updateUserStatus(client.handshake.query.username42 + "", UserStatus.ONLINE);
    this.server.emit('usersStatus', { users: await this.userService.findAllUsersStatus() })
  }


  async handleDisconnect(client: Socket) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    console.log("[Status Socket] Disconnected", client.handshake.query.username42);
    this.userService.updateUserStatus(client.handshake.query.username42 + "", UserStatus.OFFLINE);
      this.server.emit('usersStatus', { users: await this.userService.findAllUsersStatus() })
  }

  /*******************************************/
  /***            Event Helpers            ***/
  /*******************************************/
  @SubscribeMessage('changeStatus')
  async changeStatus(
    @MessageBody() status: { status: UserStatus },
    @ConnectedSocket() client: Socket,
  ) {
    if (!this.authService.verify(<string>client.handshake.query.token)) {
      return;
    }
    console.log("[Status Socket] Change", client.handshake.query.username42, " <(=-=)>", status.status);
    this.userService.updateUserStatus(client.handshake.query.username42 + "", status.status);
    this.server.emit('usersStatus', { users: await this.userService.findAllUsersStatus() })
  }
}
