import {
  Entity,
  PrimaryGeneratedColumn,
  Column,
  JoinTable,
  ManyToMany,
} from 'typeorm';
import { User } from 'src/users/user/entities/user.entity';

@Entity()
export class Achievement {
  @PrimaryGeneratedColumn()
  id: number;

  @ManyToMany(() => User)
  @JoinTable()
  users: User[];

  @Column({ unique: true })
  title: string;

  @Column('text')
  description: string;

  @Column({
    default: 'achievementDefault.jpg',
  })
  imgUrl: string;
}
