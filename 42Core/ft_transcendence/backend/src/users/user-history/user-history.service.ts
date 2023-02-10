import { Injectable } from '@nestjs/common';
import { InjectRepository } from '@nestjs/typeorm';
import { Repository } from 'typeorm';
import { CreateUserHistoryDto } from './dto/create-user-history.dto';
import { UserHistory } from './entities/user-history.entity';

@Injectable()
export class UserHistoryService {
  constructor(
    @InjectRepository(UserHistory)
    private readonly userHistoryRepository: Repository<UserHistory>,
  ) {}

  /*******************************************/
  /***            Standard Logic           ***/
  /*******************************************/

  findAll() {
    return this.userHistoryRepository.find();
  }

  async findOne(id: string) {
    const userHistory = await this.userHistoryRepository.findOne({
      where: {
        id: +id,
      },
      relations: ['user'],
    });
    return userHistory;
  }

  async create(createUserHistoryDto: CreateUserHistoryDto) {
    const userHistory = this.userHistoryRepository.create(createUserHistoryDto);
    console.log(userHistory);
    return this.userHistoryRepository.save(userHistory);
  }

  async remove(id: string) {
    const userHistory = await this.userHistoryRepository.findOne({
      where: { id: +id },
    });
    return this.userHistoryRepository.remove(userHistory);
  }
}
