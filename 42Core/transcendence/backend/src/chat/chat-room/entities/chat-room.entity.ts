import { Column, Entity, PrimaryGeneratedColumn } from 'typeorm';
import { MsgDto } from '../dtos/msg.dto';
import { UserAndSocketIdDto } from '../dtos/user-and-socketId.dto';
import { FlaggedUserDto } from '../dtos/flagged-user.dto';
import { ChatRoomEnum } from '../enums/chat-room.enums';

@Entity()
export class ChatRoom {
  @PrimaryGeneratedColumn()
  id: string;

  @Column({ unique: true })
  roomName: string;

  @Column({ nullable: true })
  password: string;

  @Column({ type: 'enum', enum: ChatRoomEnum })
  status: ChatRoomEnum;

  @Column({ nullable: true })
  owner: string;

  @Column('character varying', { array: true, nullable: true, default: {} })
  admins: string[];

  @Column({ type: 'jsonb', nullable: true, default: {} })
  loggedUsers: UserAndSocketIdDto;

  @Column({ type: 'jsonb', nullable: true, default: {} })
  flaggedUsers: FlaggedUserDto;

  @Column({ type: 'json', default: [] })
  messagesObj: MsgDto[];
}
