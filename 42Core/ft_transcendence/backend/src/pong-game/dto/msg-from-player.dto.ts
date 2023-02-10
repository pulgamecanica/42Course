import { IsEnum } from 'class-validator';

export enum Direction {
  DOWN = 'down',
  UP = 'up',
  LEFT = 'left',
  RIGHT = 'right',
}

export class  MsgFromPlayer{
  @IsEnum(Direction)
  readonly direction: Direction;
}
