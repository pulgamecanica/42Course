import {
  Column,
  Entity,
  ManyToOne,
  PrimaryGeneratedColumn,
  JoinColumn,
} from 'typeorm';
import { User } from 'src/users/user/entities/user.entity';


@Entity()
export class Setting {
  @PrimaryGeneratedColumn()
  id: number;

  @Column()
  userId!: number;

  @ManyToOne(() => User, (user) => user.history, { nullable: false })
  @JoinColumn({ name: 'userId', referencedColumnName: 'id' })
  user: User;

  @Column({
    default: "#424242"
  })
  puck: string;

  @Column({
    default: "#424242"
  })
  paddleR: string;

  @Column({
    default: "#424242"
  })
  paddleL: string;

  @Column({
    default: "#ffffff"
  })
  bg: string;

  @Column({
    default: "New Setting"
  })
  name: string;
}