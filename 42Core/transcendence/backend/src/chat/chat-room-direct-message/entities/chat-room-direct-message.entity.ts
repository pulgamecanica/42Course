import { MsgDto } from 'src/chat/chat-room/dtos/msg.dto';
import { ChatRoomEnum } from 'src/chat/chat-room/enums/chat-room.enums';
import { ChatUser } from 'src/chat/chat-user/entities/chat-user.entity';
import { Column, Entity, ManyToMany, PrimaryGeneratedColumn } from 'typeorm';

@Entity()
export class ChatRoomDirectMessage {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({ unique: true })
  roomName: string;

  @Column('character varying', { array: true, default: {} })
  participants: string[];

  @Column({
    type: 'enum',
    enum: ChatRoomEnum,
    default: ChatRoomEnum.directMessage,
  })
  status: ChatRoomEnum;

  @ManyToMany(() => ChatUser, (chatUser) => chatUser.chatRoomDirectMessage)
  chatUser: ChatUser[];

  @Column({ type: 'json', default: [] })
  messagesObj: MsgDto[];
}
