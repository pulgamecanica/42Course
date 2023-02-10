import { Controller, Get, Post, Body, Patch, Param, Delete, Req, UseGuards } from '@nestjs/common';
import { SettingsService } from './settings.service';
import { CreateSettingDto } from './dto/create-setting.dto';
import { UpdateSettingDto } from './dto/update-setting.dto';
import { Jwt2faAuthGuard } from 'src/auth/guards/jwt-2fa-auth.guard';

@UseGuards(Jwt2faAuthGuard)
@Controller('settings')
export class SettingsController {
  constructor(private readonly settingsService: SettingsService) {}

  @Post()
  async create(@Req() req, @Body() createSettingDto: CreateSettingDto) {
    return await this.settingsService.create(req.user.id, createSettingDto);
  }

  @Get()
  async findAll() {
    return await this.settingsService.findAll();
  }

  @Get(':id')
  async findOne(@Param('id') id: string) {
    return await this.settingsService.findOne(+id);
  }

  @Patch(':id')
  async update(@Req() req, @Param('id') id: string, @Body() updateSettingDto: UpdateSettingDto) {
    return await this.settingsService.update(req.user.id, +id, updateSettingDto);
  }

  @Delete(':id')
  async remove(@Req() req, @Param('id') id: string) {
    return await this.settingsService.remove(req.user.id, +id);
  }
}
