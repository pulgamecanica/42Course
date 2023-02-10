import { Module } from '@nestjs/common';
import { TypeOrmModule } from '@nestjs/typeorm';
import { UserHistoryService } from './user-history.service';
import { UserHistoryController } from './user-history.controller';
import { UserHistory } from './entities/user-history.entity';
import { User } from '../user/entities/user.entity';
import { Jwt2faAuthStrategy } from 'src/auth/strategies/jwt-2fa-auth.strategy';

@Module({
  imports: [TypeOrmModule.forFeature([User, UserHistory])],
  controllers: [UserHistoryController],
  providers: [UserHistoryService, Jwt2faAuthStrategy],
})
export class UserHistoryModule {}
