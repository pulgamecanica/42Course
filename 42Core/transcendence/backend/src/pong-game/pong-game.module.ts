import { Module } from '@nestjs/common';
import { TypeOrmModule } from '@nestjs/typeorm';
import { PongGameService } from './pong-game.service';
import { PongGameController } from './pong-game.controller';
import { User } from 'src/users/user/entities/user.entity';
import { PongGame } from './entities/pong-game.entity';
import { Jwt2faAuthStrategy } from 'src/auth/strategies/jwt-2fa-auth.strategy';
import { UserModule } from 'src/users/user/user.module';
import { PongGameGateway } from './pong-game.gateway';
import { PongQueueGateway } from './pong-queue.gateway';
import { AuthModule } from 'src/auth/auth.module';

@Module({
  exports: [PongGameService],
  imports: [TypeOrmModule.forFeature([User, PongGame]), UserModule, AuthModule],
  controllers: [PongGameController],
  providers: [
    PongGameService,
    Jwt2faAuthStrategy,
    PongGameGateway,
    PongQueueGateway,
  ],
})
export class PongGameModule {}
