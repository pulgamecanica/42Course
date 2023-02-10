import { IsEnum, IsString } from 'class-validator';
import { FlaggedStatusEnum } from '../enums/chat-room.enums';

export class FlaggedUserDto {
  @IsString()
  name: string;

  @IsString()
  flaggedBy: string;

  @IsEnum(FlaggedStatusEnum)
  status: FlaggedStatusEnum;

  @IsString()
  reason: string;

  @IsString()
  timeStart?: string;

  @IsString()
  timeEnd?: string;
}
