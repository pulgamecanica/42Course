import { Module } from '@nestjs/common';
import { TypeOrmModule } from '@nestjs/typeorm';
import { ChatRoomDirectMessageService } from './chat-room-direct-message.service';
import { ChatRoomDirectMessageController } from './chat-room-direct-message.controller';
import { ChatRoomDirectMessage } from './entities/chat-room-direct-message.entity';

@Module({
  imports: [TypeOrmModule.forFeature([ChatRoomDirectMessage])],
  providers: [ChatRoomDirectMessageService],
  exports: [ChatRoomDirectMessageService],
  controllers: [ChatRoomDirectMessageController],
})
export class ChatRoomDirectMessageModule {}
