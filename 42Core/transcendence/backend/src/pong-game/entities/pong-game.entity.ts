import { User } from 'src/users/user/entities/user.entity';
import {
  Column,
  CreateDateColumn,
  Entity,
  ManyToOne,
  PrimaryGeneratedColumn,
  JoinColumn,
} from 'typeorm';

export enum GameStatus {
  STARTING = 'starting',
  PLAYING = 'playing',
  FINISHED = 'finished',
  SUSPENDED = 'suspended',
}

@Entity()
export class PongGame {
  @PrimaryGeneratedColumn()
  id: number;

  @Column({
    type: 'enum',
    enum: GameStatus,
  })
  status: GameStatus;

  @Column()
  userOneId!: number;

  @Column()
  userTwoId!: number;

  @ManyToOne(() => User, (user) => user.pongGamePlayerOne)
  @JoinColumn({ name: 'userOneId', referencedColumnName: 'id' })
  userOne: User;

  @ManyToOne(() => User, (user) => user.pongGamePlayerTwo)
  @JoinColumn({ name: 'userTwoId', referencedColumnName: 'id' })
  userTwo: User;

  @Column()
  scoreOne: number;

  @Column()
  scoreTwo: number;

  @Column({ default: true })
  isLadder: boolean;

  @CreateDateColumn()
  createdAt: Date;
}
