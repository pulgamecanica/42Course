import {
  Column,
  CreateDateColumn,
  Entity,
  OneToMany,
  PrimaryGeneratedColumn,
  UpdateDateColumn,
  ManyToMany,
  JoinTable,
  OneToOne,
  JoinColumn,
} from 'typeorm';
import { FriendRelation } from 'src/users/friend-relation/entities/friend-relation.entity';
import { PongGame } from 'src/pong-game/entities/pong-game.entity';
import { UserHistory } from 'src/users/user-history/entities/user-history.entity';
import { Achievement } from 'src/users/achievement/entities/achievement.entity';
import { Setting } from 'src/users/settings/entities/setting.entity';
import { Exclude } from 'class-transformer';
import { ChatUser } from 'src/chat/chat-user/entities/chat-user.entity';

export enum UserStatus {
  ONLINE = 'online',
  OFFLINE = 'offline',
  PLAYINGFRIENDLY = 'playingFriendly',
  PLAYINGLADDER = 'playingLader',
}

@Entity()
export class User {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({ default: true })
  firstLogin: boolean;

  @Column({ unique: true })
  intraId: number;

  @Column('varchar', { length: 200 })
  @Column({ nullable: true })
  email: string;

  @Column({ unique: true })
  username42: string;

  @Column({ default: false })
  isAdmin: boolean;

  @Column('varchar', { length: 200 })
  firstName: string;

  @Column('varchar', { length: 200 })
  lastName: string;

  @Column({ unique: true, nullable: true })
  nickname: string;

  @Column()
  avatarImg: string;

  @Column('text', { default: '' })
  bio: string;

  @Column({
    type: 'enum',
    enum: UserStatus,
    default: UserStatus.OFFLINE,
  })
  status: UserStatus;

  @Exclude()
  @Column({ default: '' })
  twoFactorAuthenticationSecret: string;

  @Column({ default: false })
  isTwoFactorAuthenticationEnabled: boolean;

  @Column({ nullable: true })
  defaultSettingId!: number;

  @OneToOne(() => Setting)
  @JoinColumn()
  defaultSetting: Setting;

  @OneToMany(() => UserHistory, (userHistory) => userHistory.user)
  history: UserHistory[];

  @OneToMany(() => Setting, (setting) => setting.user)
  settings: Setting[];

  @OneToMany(() => FriendRelation, (userRelations) => userRelations.userOne)
  relationsOne: FriendRelation[];

  @OneToMany(() => FriendRelation, (userRelations) => userRelations.userTwo)
  relationsTwo: FriendRelation[];

  @OneToMany(() => PongGame, (pongGame) => pongGame.userOne)
  pongGamePlayerOne: PongGame[];

  @OneToMany(() => PongGame, (pongGame) => pongGame.userTwo)
  pongGamePlayerTwo: PongGame[];

  @ManyToMany(() => Achievement)
  @JoinTable()
  achievements: Achievement[];

  @CreateDateColumn()
  createdAt: Date;

  @UpdateDateColumn()
  updatedAt: Date;

  @OneToOne(() => ChatUser, (chatUser) => chatUser.user, {
    onDelete: 'CASCADE',
  })
  @JoinColumn()
  chatUser: ChatUser;

  @Column()
  chatUserId: number;
}
