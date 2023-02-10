import { Injectable, NotFoundException } from '@nestjs/common';
import { UpdateChatUserDto } from './dto/update-chat-user.dto';
import { InjectRepository } from '@nestjs/typeorm';
import { ChatUser } from './entities/chat-user.entity';
import { Repository } from 'typeorm';
import * as sha256 from 'crypto-js/sha256';
import * as tinycolor from 'tinycolor2';

@Injectable()
export class ChatUserService {
  constructor(@InjectRepository(ChatUser) private repo: Repository<ChatUser>) {}

  /*******************************************/
  /***             Standard Logic          ***/
  /*******************************************/
  async findAll() {
    return await this.repo.find({ relations: ['chatRoomDirectMessage'] });
  }

  async findById(id: number) {
    const chatUser = await this.repo.findOne({
      where: {
        id: id,
      },
      relations: ['user'],
    });
    if (!chatUser) {
      return null;
    }

    return chatUser;
  }

  async findByUsername42(username42: string) {
    const chatUser = await this.repo.findOne({
      where: {
        user: {
          username42: username42,
        },
      },
      relations: ['user'],
    });
    if (!chatUser) {
      return null;
    }

    return chatUser;
  }

  async create(username42: string) {
    const newChatUser = this.repo.create({
      userColor: this.calculateColor(username42),
    });

    return await this.repo.save(newChatUser);
  }

  async update(id: number, updateChatUserDto: UpdateChatUserDto) {
    await this.repo.update(id, updateChatUserDto);
  }

  async delete(chatUser: ChatUser) {
    return await this.repo.remove(chatUser);
  }

  async save(chatUser: ChatUser) {
    await this.repo.save(chatUser);
  }

  /*******************************************/
  /***             Chat-Room Logic         ***/
  /*******************************************/
  async blockUser(myUsername42Id: number, blockUsername42: string) {
    // Check if Logins are valid
    const myChatUser = await this.findById(myUsername42Id);
    if (!myChatUser) {
      throw new NotFoundException(`${myUsername42Id} not found.`);
    }

    const blockChatUser = await this.findByUsername42(blockUsername42);
    if (!blockChatUser) {
      throw new NotFoundException(`${blockUsername42} not found.`);
    }

    // Check if the id of the user to be blocked is self.
    if (myChatUser.user.username42 === blockUsername42) {
      throw new NotFoundException(`Can't add yourself to the blocked list.`);
    }

    // Check if the the username42 is already blocked.
    if (myChatUser.blockedUsers.includes(blockUsername42)) {
      throw new NotFoundException(`${blockUsername42} is already blocked.`);
    }

    // Updates blockedUsers by adding the username42.
    myChatUser.blockedUsers.push(blockUsername42);
    await this.update(myChatUser.id, {
      blockedUsers: myChatUser.blockedUsers,
    });
  }

  async unblockUser(myUsername42Id: number, unblockUsername42: string) {
    // Check if Logins are valid
    const myChatUser = await this.findById(myUsername42Id);
    if (!myChatUser) {
      throw new NotFoundException(`${myUsername42Id} not found.`);
    }

    const unblockChatUser = await this.findByUsername42(unblockUsername42);
    if (!unblockChatUser) {
      throw new NotFoundException(`${unblockUsername42} not found.`);
    }

    // Check if the id of the user to be blocked is self.
    if (myChatUser.user.username42 === unblockUsername42) {
      throw new NotFoundException(`Can't unblock yourself.`);
    }

    // Check if the the username42 is already blocked.
    const index = myChatUser.blockedUsers.indexOf(unblockUsername42);
    if (index === -1) {
      throw new NotFoundException(`${unblockUsername42} is not blocked.`);
    }

    // Updates blockedUsers by removing the username42.
    myChatUser.blockedUsers.splice(index, 1);
    await this.update(myChatUser.id, {
      blockedUsers: myChatUser.blockedUsers,
    });
  }

  async addConnectedRoomToChatUser(username42: string, roomName: string) {
    const chatUser = await this.findByUsername42(username42);
    if (!chatUser) {
      throw new NotFoundException(`${username42} not found.`);
    }

    if (!chatUser.connectedRooms.includes(roomName)) {
      chatUser.connectedRooms.push(roomName);
    }

    await this.update(chatUser.id, { connectedRooms: chatUser.connectedRooms });
  }

  async removeConnectedRoomFromChatUser(username42: string, roomName: string) {
    const chatUser = await this.findByUsername42(username42);
    if (!chatUser) {
      throw new NotFoundException(`${username42} not found.`);
    }

    chatUser.connectedRooms = chatUser.connectedRooms.filter(
      (filter) => filter !== roomName,
    );

    await this.update(chatUser.id, { connectedRooms: chatUser.connectedRooms });
  }

  async addConnectedRoomDirectMessageToChatUser(
    username42: string,
    roomName: string,
  ) {
    const chatUser = await this.findByUsername42(username42);
    if (!chatUser) {
      throw new NotFoundException(`${username42} not found.`);
    }

    if (!chatUser.connectedRoomsDirectMessage.includes(roomName)) {
      chatUser.connectedRoomsDirectMessage.push(roomName);
    }

    await this.update(chatUser.id, {
      connectedRoomsDirectMessage: chatUser.connectedRoomsDirectMessage,
    });
  }

  async removeConnectedRoomDirectMessageFromChatUser(
    username42: string,
    roomName: string,
  ) {
    const chatUser = await this.findByUsername42(username42);
    if (!chatUser) {
      throw new NotFoundException(`${username42} not found.`);
    }

    chatUser.connectedRoomsDirectMessage =
      chatUser.connectedRoomsDirectMessage.filter(
        (filter) => filter !== roomName,
      );

    await this.update(chatUser.id, {
      connectedRoomsDirectMessage: chatUser.connectedRoomsDirectMessage,
    });
  }

  /*******************************************/
  /***         Chat-User User Color        ***/
  /*******************************************/
  hashUsername(username42: string) {
    const hash = sha256(username42).toString();
    return hash.substring(hash.length - 6);
  }

  calculateColor(username42: string) {
    const BACKGROUND = tinycolor('#141414');
    const _360 = [...Array(360).keys()];
    const HUES = [..._360.reverse().map((h) => -h), ..._360];

    const hash = this.hashUsername(username42);
    let color = tinycolor(`#${hash}`);

    for (let i = 0; i < HUES.length; i++) {
      const hue = HUES[i];
      const spinned = color.spin(hue);

      if (tinycolor.isReadable(BACKGROUND, spinned)) {
        color = spinned;
        break;
      }
    }
    return color.toHexString();
  }
}
