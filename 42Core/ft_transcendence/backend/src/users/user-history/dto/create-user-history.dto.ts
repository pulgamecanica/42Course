import { IsEnum, IsNumber, IsString } from 'class-validator';
import { UserHistorySubject } from '../entities/user-history.entity';

export class CreateUserHistoryDto {
  @IsString()
  readonly description: string;

  @IsEnum(UserHistorySubject)
  readonly subject: UserHistorySubject;

  @IsNumber()
  readonly userId: number;
}
