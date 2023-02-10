import { IsNumber, IsString, IsBoolean } from 'class-validator';

export class CreateUserDto {
  @IsNumber()
  readonly intraId: number;

  @IsString()
  readonly email: string;

  @IsString()
  readonly username42: string;

  @IsString()
  readonly firstName: string;

  @IsString()
  readonly lastName: string;

  @IsString()
  readonly nickname?: string;

  @IsString()
  readonly avatarImg: string;

  @IsString()
  readonly bio?: string;

  @IsBoolean()
  readonly firstLogin?: boolean;
}
