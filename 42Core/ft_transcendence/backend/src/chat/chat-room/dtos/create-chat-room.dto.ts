import {
  IsArray,
  IsEnum,
  IsJSON,
  IsString,
  ValidateNested,
} from 'class-validator';
import { Type } from 'class-transformer';
import { ChatRoomEnum } from '../enums/chat-room.enums';
import { FlaggedUserDto } from './flagged-user.dto';
import { UserAndSocketIdDto } from './user-and-socketId.dto';
import { MsgDto } from './msg.dto';

export class CreateChatRoomDto {
  @IsString()
  roomName: string;

  @IsString()
  password: string;

  @IsEnum(ChatRoomEnum)
  status: ChatRoomEnum;

  @IsString()
  owner: string;

  @IsArray()
  admins?: string[];

  @IsJSON()
  @ValidateNested({ each: true })
  @Type(() => FlaggedUserDto)
  flaggedUsers?: FlaggedUserDto;

  @IsJSON()
  @ValidateNested({ each: true })
  @Type(() => UserAndSocketIdDto)
  loggedUsers?: UserAndSocketIdDto;

  @IsArray()
  messagesObj?: MsgDto[];
}
