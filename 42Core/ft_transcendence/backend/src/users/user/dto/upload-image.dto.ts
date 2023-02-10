import { IsNotEmpty } from 'class-validator';

export class UploadImageDto {

  @IsNotEmpty()
  readonly img: any;
}
