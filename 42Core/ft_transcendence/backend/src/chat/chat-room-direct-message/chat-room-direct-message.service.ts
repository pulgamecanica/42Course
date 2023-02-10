import { Injectable, NotFoundException, UnprocessableEntityException } from '@nestjs/common';
import { InjectRepository } from '@nestjs/typeorm';
import { Repository } from 'typeorm';
import { ChatRoomDirectMessage } from './entities/chat-room-direct-message.entity';
import { CreateChatRoomDirectMessageDto } from './dtos/create-chat-room-direct-message.dto';
import { UpdateChatRoomDirectMessageDto } from './dtos/update-chat-room-direct-message.dto';

@Injectable()
export class ChatRoomDirectMessageService {
  constructor(
    @InjectRepository(ChatRoomDirectMessage)
    private repo: Repository<ChatRoomDirectMessage>,
  ) {}

  /*******************************************/
  /***            Controller Logic         ***/
  /*******************************************/
  async findAll() {
    return await this.repo.find({
      relations: ['chatUser'],
    });
  }

  async findByRoomName(roomName: string) {
    const chatRoom = await this.repo.findOne({
      where: {
        roomName,
      },
      relations: ['chatUser'],
    });
    if (!chatRoom) {
      return null;
    }

    return chatRoom;
  }

  async create(chatRoomDirectMessageDto: CreateChatRoomDirectMessageDto) {
    if (await this.findByRoomName(chatRoomDirectMessageDto.roomName)) {
      throw new NotFoundException(
        `${chatRoomDirectMessageDto.roomName} already exists!`,
      );
    }

    const newChatRoom = this.repo.create(chatRoomDirectMessageDto);

    return await this.repo.save(newChatRoom).catch((err: any) => {
      const message =
        typeof err === 'string'
          ? err
          : err.detail
          ? err.detail
          : err.message
          ? err.message
          : '';
      if (message) {
        throw new UnprocessableEntityException(message);
      } else {
        console.log('message not found');
      }
    });
  }

  async update(
    roomName: string,
    updateChatRoomDirectMessageDto: UpdateChatRoomDirectMessageDto,
  ) {
    const chatRoom = await this.findByRoomName(roomName);
    if (!chatRoom) {
      return `${roomName} not found.`;
    }

    await this.repo.update(chatRoom.id, updateChatRoomDirectMessageDto);

    return await this.findByRoomName(roomName);
  }

  async save(chatRoomDirectMessage: ChatRoomDirectMessage) {
    await this.repo.save(chatRoomDirectMessage).catch((err: any) => {
      const message =
        typeof err === 'string'
          ? err
          : err.detail
          ? err.detail
          : err.message
          ? err.message
          : '';
      if (message) {
        throw new UnprocessableEntityException(message);
      } else {
        console.log('message not found');
      }
    });
  }
}
