import { IsEnum, IsJSON, IsString, ValidateNested } from 'class-validator';
import {
  TypeOfMessage,
  ChatRoomEnum,
  MsgDtoPrivilege,
} from '../enums/chat-room.enums';
import { Type } from 'class-transformer';
import { PongGameChat } from './pong-game-chat.dto';

export class MsgDto {
  @IsEnum(TypeOfMessage)
  typeOfMessage?: TypeOfMessage;

  @IsEnum(ChatRoomEnum)
  typeOfRoom?: ChatRoomEnum;

  @IsString()
  userColor?: string;

  @IsString()
  theDate?: string;

  @IsString()
  theTime?: string;

  @IsString()
  name?: string;

  @IsString()
  text?: string;

  @IsString()
  fromRoom?: string;

  // If it's owner, admin, or user
  @IsEnum(MsgDtoPrivilege)
  privilege?: MsgDtoPrivilege;

  @IsJSON()
  @ValidateNested({ each: true })
  @Type(() => PongGameChat)
  pongGameChat?: PongGameChat;
}
