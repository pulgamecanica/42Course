import { ChatRoomDirectMessage } from 'src/chat/chat-room-direct-message/entities/chat-room-direct-message.entity';
import { User } from 'src/users/user/entities/user.entity';
import {
  Column,
  Entity,
  JoinTable,
  ManyToMany,
  OneToOne,
  PrimaryGeneratedColumn,
} from 'typeorm';

@Entity()
export class ChatUser {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({ default: '#FFFFFF' })
  userColor: string;

  @Column({ nullable: true })
  socketId: string;

  @Column({ default: false })
  showDate: boolean;

  @Column({ default: false })
  showTime: boolean;

  @Column('character varying', { array: true, default: {} })
  blockedUsers: string[];

  @Column('character varying', { array: true, default: {} })
  connectedRooms: string[];

  @Column('character varying', { array: true, default: {} })
  connectedRoomsDirectMessage: string[];

  @OneToOne(() => User, (user) => user.chatUser, {
    onDelete: 'CASCADE',
  })
  user: User;

  @ManyToMany(
    () => ChatRoomDirectMessage,
    (chatRoomDirectMessage) => chatRoomDirectMessage.chatUser,
    { cascade: true },
  )
  @JoinTable({
    name: 'chat_user-chat_room_direct_message',
    joinColumn: {
      name: 'chatUserId',
      referencedColumnName: 'id',
    },
    inverseJoinColumn: {
      name: 'chatRoomDirectMessageId',
      referencedColumnName: 'id',
    },
  })
  chatRoomDirectMessage: ChatRoomDirectMessage[];
}
