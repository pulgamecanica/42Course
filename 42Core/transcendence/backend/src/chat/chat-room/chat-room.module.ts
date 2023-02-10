import { Module } from '@nestjs/common';
import { ChatRoomService } from './chat-room.service';
import { ChatRoomGateway } from './chat-room.gateway';
import { ChatRoomController } from './chat-room.controller';
import { ChatRoom } from './entities/chat-room.entity';
import { TypeOrmModule } from '@nestjs/typeorm';
import { ChatUserModule } from '../chat-user/chat-user.module';
import { UserModule } from 'src/users/user/user.module';
import { ChatRoomDirectMessageModule } from '../chat-room-direct-message/chat-room-direct-message.module';
import { FriendRelationModule } from 'src/users/friend-relation/friend-relation.module';
import { PongGameModule } from 'src/pong-game/pong-game.module';
import { AuthModule } from 'src/auth/auth.module';

@Module({
  imports: [
    TypeOrmModule.forFeature([ChatRoom]),
    UserModule,
    ChatUserModule,
    ChatRoomDirectMessageModule,
    FriendRelationModule,
    PongGameModule,
    AuthModule,
  ],
  providers: [ChatRoomGateway, ChatRoomService],
  exports: [ChatRoomService],
  controllers: [ChatRoomController],
})
export class ChatRoomModule {}
