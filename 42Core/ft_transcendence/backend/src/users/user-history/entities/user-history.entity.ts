import {
  Column,
  CreateDateColumn,
  Entity,
  ManyToOne,
  PrimaryGeneratedColumn,
  JoinColumn,
} from 'typeorm';
import { User } from 'src/users/user/entities/user.entity';

export enum UserHistorySubject {
  PLAYED_GAME = 'played_game',
  WON_GAME = 'won_game',
  LOST_GAME = 'lost_game',
  NEW_FRIEND = 'new_friend',
  WON_ACHIEVEMENT = 'won_achievement',
}

@Entity()
export class UserHistory {
  @PrimaryGeneratedColumn()
  id: number;

  @Column()
  userId!: number;

  @ManyToOne(() => User, (user) => user.history, { nullable: false })
  @JoinColumn({ name: 'userId', referencedColumnName: 'id' })
  user: User;

  @Column({
    type: 'enum',
    enum: UserHistorySubject,
  })
  subject: UserHistorySubject;

  @Column('text')
  description: string;

  @CreateDateColumn()
  createdAt: Date;
}
