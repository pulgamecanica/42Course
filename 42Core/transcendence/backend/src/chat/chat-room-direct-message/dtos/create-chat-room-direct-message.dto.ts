import { IsArray, IsJSON, IsString, ValidateNested } from 'class-validator';
import { Type } from 'class-transformer';
import { ChatUser } from 'src/chat/chat-user/entities/chat-user.entity';
import { MsgDto } from 'src/chat/chat-room/dtos/msg.dto';

export class CreateChatRoomDirectMessageDto {
  @IsString()
  roomName?: string;

  @IsArray()
  participants?: string[];

  @IsJSON()
  @ValidateNested({ each: true })
  @Type(() => ChatUser)
  chatUser?: ChatUser[];

  @IsArray()
  messagesObj?: MsgDto[];
}
