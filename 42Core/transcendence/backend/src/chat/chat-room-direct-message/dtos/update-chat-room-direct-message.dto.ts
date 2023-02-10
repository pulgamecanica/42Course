import { PartialType } from '@nestjs/mapped-types';
import { CreateChatRoomDirectMessageDto } from './create-chat-room-direct-message.dto';

export class UpdateChatRoomDirectMessageDto extends PartialType(
  CreateChatRoomDirectMessageDto,
) {}
