import { Module } from '@nestjs/common';
import { TypeOrmModule } from '@nestjs/typeorm';
import { UserModule } from './users/user/user.module';
import { AuthModule } from './auth/auth.module';
import { ConfigModule } from '@nestjs/config';
import { ServeStaticModule } from '@nestjs/serve-static';
import { join } from 'path';
import { PongGameModule } from './pong-game/pong-game.module';
import { SettingsModule } from './users/settings/settings.module';
import { ChatUserModule } from './chat/chat-user/chat-user.module';
import { ChatRoomDirectMessageModule } from './chat/chat-room-direct-message/chat-room-direct-message.module';
import { ChatRoomModule } from './chat/chat-room/chat-room.module';
import { ScheduleModule } from '@nestjs/schedule';


@Module({
  imports: [
    ...(process.env.NODE_ENV === 'production'
      ? [
          ServeStaticModule.forRoot({
            rootPath: join(__dirname, '..', 'client'),
          }),
        ]
      : []),
    ConfigModule.forRoot({
      isGlobal: true,
    }),
    TypeOrmModule.forRoot({
      type: 'postgres',
      host: process.env.POSTGRES_HOST,
      port: parseInt(process.env.POSTGRES_HOST_PORT),
      username: process.env.POSTGRES_USER,
      password: process.env.POSTGRES_PASSWORD,
      database: process.env.POSTGRES_DB,
      autoLoadEntities: true,
      synchronize: true,
    }),
    ScheduleModule.forRoot(),
    UserModule,
    AuthModule,
    PongGameModule,
    SettingsModule,
    ChatUserModule,
    ChatRoomDirectMessageModule,
    ChatRoomModule,
  ],
  controllers: [],
  providers: [],
})
export class AppModule {}
