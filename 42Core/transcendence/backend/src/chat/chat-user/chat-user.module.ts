import { Module } from '@nestjs/common';
import { ChatUserService } from './chat-user.service';
import { TypeOrmModule } from '@nestjs/typeorm';
import { ChatUser } from './entities/chat-user.entity';
import { ChatUserController } from './chat-user.controller';

@Module({
  imports: [TypeOrmModule.forFeature([ChatUser])],
  providers: [ChatUserService],
  exports: [ChatUserService],
  controllers: [ChatUserController],
})
export class ChatUserModule {}
