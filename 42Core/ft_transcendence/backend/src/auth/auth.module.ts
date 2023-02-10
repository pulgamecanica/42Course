import { Module } from '@nestjs/common';
import { AuthController } from './auth.controller';
import { AuthService } from './auth.service';
import { UserModule } from '../users/user/user.module';
import { PassportModule } from '@nestjs/passport';
import { SessionSerializer } from './session.serializer';
import { FortyTwoAuthStrategy } from './strategies/42-auth.strategy';
import { JwtModule } from '@nestjs/jwt';
import { JwtAuthStrategy } from './strategies/jwt-auth.strategy';
import { Jwt2faAuthStrategy } from './strategies/jwt-2fa-auth.strategy';
import { ConfigModule } from '@nestjs/config';

@Module({
  exports: [AuthService],
  controllers: [AuthController],
  providers: [
    AuthService,
    JwtAuthStrategy,
    Jwt2faAuthStrategy,
    FortyTwoAuthStrategy,
    SessionSerializer,
  ],
  imports: [
    UserModule,
    ConfigModule.forRoot({
      isGlobal: true,
      envFilePath: '.env',
    }),
    JwtModule.register({
      secret: process.env.JWT_SECRET,
      signOptions: { expiresIn: '1d' },
    }),
    PassportModule.register({ session: true }),
  ],
})
export class AuthModule {}
