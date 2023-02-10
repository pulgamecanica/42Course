import { IsString, IsOptional } from 'class-validator';

export class CreateSettingDto {
  @IsOptional()
  @IsString()
  readonly puck?: string;

  @IsOptional()
  @IsString()
  readonly paddleR?: string;

  @IsOptional()
  @IsString()
  readonly paddleL?: string;

  @IsOptional()
  @IsString()
  readonly bg?: string;

  @IsOptional()
  @IsString()
  readonly name?: string;
}
