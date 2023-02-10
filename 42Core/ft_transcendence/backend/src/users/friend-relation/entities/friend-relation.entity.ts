import {
  Column,
  CreateDateColumn,
  Entity,
  ManyToOne,
  PrimaryGeneratedColumn,
  JoinColumn,
} from 'typeorm';
import { User } from 'src/users/user/entities/user.entity';

export enum FriendStatus {
  REQUEST = 'friend_request',
  FRIENDS = 'friends',
  BLOCKED = 'blocked',
}

export enum RelationOptions {
  SendRelation = "sendRelation",
  AcceptRelation = "acceptRelation",
  DeclineRelation = "declineRelation",
  Sent = "sent",
  Unfriend = "unfriend",
  Block = "block"
}

@Entity()
export class FriendRelation {
  @PrimaryGeneratedColumn()
  id: number;

  @Column()
  userOneId!: number;

  @Column()
  userTwoId!: number;

  @ManyToOne(() => User, (user) => user.relationsOne, { nullable: false })
  @JoinColumn({ name: 'userOneId', referencedColumnName: 'id' })
  userOne: User;

  @ManyToOne(() => User, (user) => user.relationsTwo, { nullable: false })
  @JoinColumn({ name: 'userTwoId', referencedColumnName: 'id' })
  userTwo: User;

  @Column({
    type: 'enum',
    enum: FriendStatus,
  })
  status: FriendStatus;

  @CreateDateColumn()
  createdAt: Date;
}
