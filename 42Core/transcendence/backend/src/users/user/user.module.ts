import { Module } from '@nestjs/common';
import { TypeOrmModule } from '@nestjs/typeorm';
import { UserController } from './user.controller';
import { UserService } from './user.service';
// Entities
import { User } from './entities/user.entity';
import { Setting } from '../settings/entities/setting.entity';
import { UserHistory } from '../user-history/entities/user-history.entity';
import { Achievement } from '../achievement/entities/achievement.entity';
import { FriendRelation } from '../friend-relation/entities/friend-relation.entity';
import { PongGame } from 'src/pong-game/entities/pong-game.entity';
// Modules
// import { FriendRelationModule } from './friend-relation/friend-relation.module';
// import { PongGameModule } from './pong-game/pong-game.module';
// import { MatchMakerModule } from './match-maker/match-maker.module';
// import { AchievementModule } from './achievement/achievement.module';
// import { UserHistoryModule } from './user-history/user-history.module';
// Controllers
import { SettingsController } from '../settings/settings.controller';
import { UserHistoryController } from '../user-history/user-history.controller';
import { FriendRelationController } from '../friend-relation/friend-relation.controller';
import { AchievementController } from '../achievement/achievement.controller';
import { PongGameController } from 'src/pong-game/pong-game.controller';
// Services
import { SettingsService } from '../settings/settings.service';
import { UserHistoryService } from '../user-history/user-history.service';
import { FriendRelationService } from '../friend-relation/friend-relation.service';
import { PongGameService } from 'src/pong-game/pong-game.service';
import { AchievementService } from '../achievement/achievement.service';
import { Jwt2faAuthStrategy } from 'src/auth/strategies/jwt-2fa-auth.strategy';
import { ChatUserModule } from 'src/chat/chat-user/chat-user.module';
import { UserStatusGateway } from './user-status.gateway';
import { AuthService } from 'src/auth/auth.service';
import { JwtService } from '@nestjs/jwt';

@Module({
  imports: [
    TypeOrmModule.forFeature([
      User,
      Setting,
      UserHistory,
      Achievement,
      FriendRelation,
      PongGame,
    ]),
    UserModule,
    ChatUserModule,
  ],
  controllers: [
    UserController,
    SettingsController,
    UserHistoryController,
    FriendRelationController,
    PongGameController,
    AchievementController,
  ],
  providers: [
    UserService,
    AuthService,
    JwtService,
    SettingsService,
    UserHistoryService,
    FriendRelationService,
    PongGameService,
    AchievementService,
    Jwt2faAuthStrategy,
    UserStatusGateway,
  ],
  exports: [UserService],
})
export class UserModule {}
