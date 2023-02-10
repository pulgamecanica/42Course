import { Injectable, NotFoundException, ConflictException, BadRequestException } from '@nestjs/common';
import { InjectRepository } from '@nestjs/typeorm';
import { Repository } from 'typeorm';
import { Setting } from './entities/setting.entity';
import { CreateSettingDto } from './dto/create-setting.dto';
import { UpdateSettingDto } from './dto/update-setting.dto';

@Injectable()
export class SettingsService {
  constructor(
    @InjectRepository(Setting)
    private readonly settingsRepository: Repository<Setting>,
  ) {}

  async create(userId: number, createSettingDto: CreateSettingDto) {
    if (Number.isNaN(userId)) throw new BadRequestException(`#${userId} is not a valid number`);
    const setting = this.settingsRepository.create({
      userId: userId,
      ...createSettingDto 
    });
    return await this.settingsRepository.save(setting);
  }

  async findAll() {
    return await this.settingsRepository.find();
  }

  async findOne(id: number) {
    if (Number.isNaN(id)) throw new BadRequestException(`#${id} is not a valid number`);
    const setting = await this.settingsRepository.findOne({
      where: {
        id: id,
      },
      relations: ['user'],
    });
    if (!setting) {
      throw new NotFoundException(`Setting #${id} not found`);
    }
    return setting;
  }

  async findUserSettings(userId: number) {
    if (Number.isNaN(userId)) throw new BadRequestException(`#${userId} is not a valid number`);
    const settings = await this.settingsRepository.find({
      where: [
        {userId: userId}
      ]
    });
    return settings;
  }

  async update(userId: number, id: number, updateSettingDto: UpdateSettingDto) {
    if (Number.isNaN(id) || Number.isNaN(userId) ) throw new BadRequestException(`#${id} or #${userId} is not a valid number`);
    const setting = await this.settingsRepository.preload({
      id: id,
      ...updateSettingDto,
    });
    if (!setting) {
      throw new NotFoundException(`Setting #${id} not found`);
    }
    if (setting.userId !== userId) {
      throw new ConflictException(`Setting #${id} doesn't belong to user #${userId}`);
    }
    return await this.settingsRepository.save(setting);
  }

  async remove(userId: number, id: number) {
    if (Number.isNaN(id) || Number.isNaN(userId) ) throw new BadRequestException(`#${id} or #${userId} is not a valid number`);
    const setting = await this.settingsRepository.findOne({
      where: { id: id },
    });
    if (!setting) {
      throw new NotFoundException(`Setting #${id} not found`);
    }
    if (setting.userId !== userId) {
      throw new ConflictException(`Setting #${id} doesn't belong to user #${userId}`);
    }
    return this.settingsRepository.remove(setting);
  }
}
