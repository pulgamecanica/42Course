import { PartialType } from '@nestjs/mapped-types';
import { CreatePongGameDto } from './create-pong-game.dto';

export class UpdatePongGameDto extends PartialType(CreatePongGameDto) {}
