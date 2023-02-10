import {
  Body,
  Controller,
  Get,
  NotFoundException,
  Param,
  Patch,
  Put,
  Query,
} from '@nestjs/common';
import { ChatUserService } from './chat-user.service';
import { UpdateChatUserDto } from './dto/update-chat-user.dto';

@Controller('/chat-user')
export class ChatUserController {
  constructor(private chatUserService: ChatUserService) {}

  /*******************************************/
  /***               Standard              ***/
  /*******************************************/
  @Get()
  async findAll() {
    return this.chatUserService.findAll();
  }

  @Get('/:username42')
  async findByUsername42(@Param('username42') username42: string) {
    if (!username42) {
      throw new NotFoundException(`No value inserted`);
    }
    return this.chatUserService.findByUsername42(username42);
  }

  @Patch('/:id')
  async update(
    @Param('id') id: number,
    @Body() updateChatUserDto: UpdateChatUserDto,
  ) {
    await this.chatUserService.update(id, updateChatUserDto);
    return await this.chatUserService.findById(id);
  }
}
