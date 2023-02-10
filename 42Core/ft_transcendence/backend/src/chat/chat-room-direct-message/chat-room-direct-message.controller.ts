import {
  Body,
  Controller,
  Get,
  NotFoundException,
  Param,
  Patch,
  Post,
} from '@nestjs/common';
import { ChatRoomDirectMessageService } from './chat-room-direct-message.service';
import { CreateChatRoomDirectMessageDto } from './dtos/create-chat-room-direct-message.dto';
import { UpdateChatRoomDirectMessageDto } from './dtos/update-chat-room-direct-message.dto';

@Controller('chat-room-direct-message')
export class ChatRoomDirectMessageController {
  constructor(
    private chatRoomDirectMessageService: ChatRoomDirectMessageService,
  ) {}

  /*******************************************/
  /***               Standard              ***/
  /*******************************************/
  @Get()
  async findAll() {
    return this.chatRoomDirectMessageService.findAll();
  }

  @Get('/:roomName')
  async findByRoomName(@Param('roomName') roomName: string) {
    if (!roomName) {
      throw new NotFoundException(`No value inserted`);
    }
    return this.chatRoomDirectMessageService.findByRoomName(roomName);
  }

  @Post()
  async create(
    @Body() chatRoomDirectMessageDto: CreateChatRoomDirectMessageDto,
  ) {
    if (!chatRoomDirectMessageDto) {
      throw new NotFoundException(`No value inserted`);
    }

    return this.chatRoomDirectMessageService.create(chatRoomDirectMessageDto);
  }

  @Patch('/:roomName')
  async update(
    @Param('roomName') roomName: string,
    @Body() updatedChatRoomDirectMessageDto: UpdateChatRoomDirectMessageDto,
  ) {
    return await this.chatRoomDirectMessageService.update(
      roomName,
      updatedChatRoomDirectMessageDto,
    );
  }
}
