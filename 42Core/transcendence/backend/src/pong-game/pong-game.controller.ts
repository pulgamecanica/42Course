import {
  Controller,
  Get,
  Post,
  Body,
  Patch,
  Param,
  Delete,
  Req,
  UseGuards,
} from '@nestjs/common';
import { PongGameService } from './pong-game.service';
import { CreatePongGameDto } from './dto/create-pong-game.dto';
import { AdminGuard } from 'src/auth/guards/roles.guard';
import { UpdatePongGameDto } from './dto/update-pong-game.dto';
import { Jwt2faAuthGuard } from 'src/auth/guards/jwt-2fa-auth.guard';

@UseGuards(Jwt2faAuthGuard)
@Controller('pong-game')
export class PongGameController {
  constructor(private readonly pongGameService: PongGameService) {}

  /*****************************************/
  /***              Ranking              ***/
  /*****************************************/
  @Get('rankings')
  async playersRanking(@Req() req) {
    return await this.pongGameService.playersRanking();
  }

  /*******************************************/
  /***               Standard              ***/
  /*******************************************/

  @Get()
  findAll() {
    return this.pongGameService.findAll();
  }

  @Get(':id')
  findOne(@Param('id') id: string) {
    return this.pongGameService.findOne(id);
  }

  @UseGuards(AdminGuard)
  @Post()
  create(@Body() createPongGameDto: CreatePongGameDto) {
    return this.pongGameService.create(createPongGameDto);
  }

  @UseGuards(AdminGuard)
  @Patch(':id')
  update(
    @Param('id') id: string,
    @Body() updatePongGameDto: UpdatePongGameDto,
  ) {
    return this.pongGameService.update(id, updatePongGameDto);
  }

  @UseGuards(AdminGuard)
  @Delete(':id')
  remove(@Param('id') id: string) {
    return this.pongGameService.remove(id);
  }
}
