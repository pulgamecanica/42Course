import { IsEnum, IsJSON, IsString, ValidateNested } from 'class-validator';
import { TypeOfMessage, ChatRoomEnum } from '../enums/chat-room.enums';
import { PongGameChat } from './pong-game-chat.dto';
import { Type } from 'class-transformer';

export class MsgFromClientDto {
  @IsEnum(TypeOfMessage)
  typeOfMessage?: TypeOfMessage;

  @IsEnum(ChatRoomEnum)
  typeOfRoom?: ChatRoomEnum;

  @IsString()
  userColor?: string;

  @IsString()
  roomName: string;

  @IsString()
  name: string;

  @IsString()
  text: string;

  @IsJSON()
  @ValidateNested({ each: true })
  @Type(() => PongGameChat)
  pongGameChat?: PongGameChat;
}
