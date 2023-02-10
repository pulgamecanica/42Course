import { PartialType } from '@nestjs/mapped-types';
import { CreateUserHistoryDto } from './create-user-history.dto';

export class UpdateUserHistoryDto extends PartialType(CreateUserHistoryDto) {}
