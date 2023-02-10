import { PartialType } from '@nestjs/mapped-types';
import { CreateChatRoomDto } from './create-chat-room.dto';

export class UpdateChatRoomDto extends PartialType(CreateChatRoomDto) {}
