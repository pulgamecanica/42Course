import { Module } from '@nestjs/common';
import { TypeOrmModule } from '@nestjs/typeorm';
import { AchievementService } from './achievement.service';
import { AchievementController } from './achievement.controller';
import { Achievement } from './entities/achievement.entity';
import { User } from 'src/users/user/entities/user.entity';
import { UserModule } from 'src/users/user/user.module';
import { Jwt2faAuthStrategy } from 'src/auth/strategies/jwt-2fa-auth.strategy';

@Module({
  imports: [
  	TypeOrmModule.forFeature([
  		User,
  		Achievement
  	]),
  ],
  controllers: [
  	AchievementController,
  ],
  providers: [
  	AchievementService,
  	Jwt2faAuthStrategy
  ],
  exports: [AchievementService],
})
export class AchievementModule {}
