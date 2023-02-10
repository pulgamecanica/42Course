import { IsEnum, IsNumber } from 'class-validator';
import { FriendStatus } from '../entities/friend-relation.entity';

export class CreateFriendRelationDto {
  @IsEnum(FriendStatus)
  readonly status: FriendStatus;

  @IsNumber()
  readonly userOneId: number;

  @IsNumber()
  readonly userTwoId: number;
}
