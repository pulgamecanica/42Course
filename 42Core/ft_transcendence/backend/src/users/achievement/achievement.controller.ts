import {
  Controller,
  Get,
  Post,
  Body,
  Param,
  Delete,
  UseGuards,
} from '@nestjs/common';
import { Jwt2faAuthGuard } from 'src/auth/guards/jwt-2fa-auth.guard';
import { AdminGuard } from 'src/auth/guards/roles.guard';
import { AchievementService } from './achievement.service';
import { CreateAchievementDto } from './dto/create-achievement.dto';
//import { UpdateAchievementDto } from './dto/update-achievement.dto';

@UseGuards(Jwt2faAuthGuard)
@Controller('achievement')
export class AchievementController {
  constructor(private readonly achievementService: AchievementService) {}

  /*******************************************/
  /***               Standard              ***/
  /*******************************************/

  @Get()
  findAll() {
    return this.achievementService.findAll();
  }

  @Get(':id')
  findOne(@Param('id') id: string) {
    return this.achievementService.findOne(id);
  }

  @UseGuards(AdminGuard)
  @Post()
  create(@Body() createAchievementDto: CreateAchievementDto) {
    return this.achievementService.create(createAchievementDto);
  }

  @UseGuards(AdminGuard)
  @Delete(':id')
  remove(@Param('id') id: string) {
    return this.achievementService.remove(id);
  }
}
