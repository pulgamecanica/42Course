import { IsString } from 'class-validator';

export class UserAndSocketIdDto {
  @IsString()
  name: string;

  @IsString()
  clientId: string;
}
