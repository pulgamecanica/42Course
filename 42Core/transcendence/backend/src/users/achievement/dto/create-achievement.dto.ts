import { IsString } from 'class-validator';

export class CreateAchievementDto {
  @IsString()
  readonly title: string;

  @IsString()
  readonly description: string;

  @IsString()
  readonly imgUrl: string;
}
