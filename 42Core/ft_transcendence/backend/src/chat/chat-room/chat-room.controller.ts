import {
  Body,
  Controller,
  Delete,
  Get,
  NotFoundException,
  Param,
  Patch,
  Post,
  UseGuards,
} from '@nestjs/common';
import { ChatRoomService } from './chat-room.service';
import { CreateChatRoomDto } from './dtos/create-chat-room.dto';
import { UpdateChatRoomDto } from './dtos/update-chat-room.dto';
import { MsgFromClientDto } from './dtos/msg-from-client.dto';
import { Jwt2faAuthGuard } from 'src/auth/guards/jwt-2fa-auth.guard';

@UseGuards(Jwt2faAuthGuard)
@Controller('chat-room')
export class ChatRoomController {
  constructor(private chatRoomService: ChatRoomService) {}

  /*******************************************/
  /***               Standard              ***/
  /*******************************************/
  @Get()
  async findAll() {
    return this.chatRoomService.findAll();
  }

  @Get('/:roomName')
  async findByRoomName(@Param('roomName') roomName: string) {
    if (!roomName) {
      throw new NotFoundException(`No value inserted`);
    }
    return this.chatRoomService.findByRoomName(roomName);
  }

  @Post()
  async create(@Body() chatRoomDto: CreateChatRoomDto) {
    if (!chatRoomDto) {
      throw new NotFoundException(`No value inserted`);
    }

    return this.chatRoomService.create(chatRoomDto);
  }

  @Patch('/:roomName')
  async update(
    @Param('roomName') roomName: string,
    @Body() updateChatRoomDto: UpdateChatRoomDto,
  ) {
    return await this.chatRoomService.update(roomName, updateChatRoomDto);
  }

  @Delete('/:roomName')
  async delete(@Param('roomName') roomName: string) {
    if (!roomName) {
      throw new NotFoundException(`No value inserted`);
    }
    return this.chatRoomService.delete(roomName);
  }

  /*******************************************/
  /***               Message               ***/
  /*******************************************/
  @Post('/addmessage')
  async addMessageToRoomName(@Body() msgFromClientDto: MsgFromClientDto) {
    if (!msgFromClientDto.roomName) {
      throw new NotFoundException(`No value inserted`);
    }
    return await this.chatRoomService.addMessageToChatRoom(msgFromClientDto);
  }

  @Delete('/messages/:roomName')
  async cleanMessagesFromRoomName(@Param('roomName') roomName: string) {
    if (!roomName) {
      throw new NotFoundException(`No value inserted`);
    }
    return this.chatRoomService.cleanMessagesFromRoomName(roomName);
  }

  /*******************************************/
  /***              loggedUsers            ***/
  /*******************************************/
  @Delete('/loggedusers/delete')
  async removeAllLoggedUsersFromAllRooms() {
    return this.chatRoomService.removeAllLoggedUsersFromAllRooms();
  }
}
