import {
  Controller,
  Get,
  Post,
  Body,
  Param,
  Delete,
  UseGuards,
} from '@nestjs/common';
import { UserHistoryService } from './user-history.service';
import { CreateUserHistoryDto } from './dto/create-user-history.dto';
import { Jwt2faAuthGuard } from 'src/auth/guards/jwt-2fa-auth.guard';
//import { UpdateUserHistoryDto } from './dto/update-user-history.dto';

@UseGuards(Jwt2faAuthGuard)
@Controller('user-history')
export class UserHistoryController {
  constructor(private readonly userHistoryService: UserHistoryService) {}

  /*******************************************/
  /***               Standard              ***/
  /*******************************************/

  @Get()
  findAll() {
    return this.userHistoryService.findAll();
  }

  @Get(':id')
  findOne(@Param('id') id: string) {
    return this.userHistoryService.findOne(id);
  }

  @Post()
  create(@Body() createUserHistoryDto: CreateUserHistoryDto) {
    return this.userHistoryService.create(createUserHistoryDto);
  }

  @Delete(':id')
  remove(@Param('id') id: string) {
    return this.userHistoryService.remove(id);
  }
}
