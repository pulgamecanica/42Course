import { IsEnum, IsNumber, IsBoolean } from 'class-validator';
import { GameStatus } from '../entities/pong-game.entity';

export class CreatePongGameDto {
  @IsEnum(GameStatus)
  readonly status: GameStatus;

  @IsNumber()
  readonly userOneId: number;

  @IsNumber()
  readonly userTwoId: number;

  @IsNumber()
  readonly scoreOne: number;

  @IsNumber()
  readonly scoreTwo: number;

  @IsBoolean()
  readonly isLadder?: boolean;
}
