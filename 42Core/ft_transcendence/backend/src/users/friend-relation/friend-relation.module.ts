import { Module } from '@nestjs/common';
import { FriendRelationService } from './friend-relation.service';
import { FriendRelationController } from './friend-relation.controller';
import { TypeOrmModule } from '@nestjs/typeorm';
import { FriendRelation } from './entities/friend-relation.entity';
import { User } from 'src/users/user/entities/user.entity';
import { Jwt2faAuthStrategy } from 'src/auth/strategies/jwt-2fa-auth.strategy';
import { UserModule } from '../user/user.module';
import { ChatUserModule } from 'src/chat/chat-user/chat-user.module';

@Module({
  imports: [
    TypeOrmModule.forFeature([User, FriendRelation]),
    UserModule,
    ChatUserModule,
  ],
  controllers: [FriendRelationController],
  providers: [FriendRelationService, Jwt2faAuthStrategy],
  exports: [FriendRelationService],
})
export class FriendRelationModule {}
