import { PartialType } from '@nestjs/mapped-types';
import { CreateFriendRelationDto } from './create-friend-relation.dto';

export class UpdateFriendRelationDto extends PartialType(
  CreateFriendRelationDto,
) {}
