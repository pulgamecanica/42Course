import { IsEnum, IsNumber, IsString } from 'class-validator';
import { GameStatus } from 'src/pong-game/entities/pong-game.entity';

export class PongGameChat {
  @IsEnum(GameStatus)
  status: GameStatus;

  @IsString()
  usernameOne: string;

  @IsString()
  usernameTwo: string;

  @IsNumber()
  id: number;

  @IsString()
  winnerUsername: string;
}
