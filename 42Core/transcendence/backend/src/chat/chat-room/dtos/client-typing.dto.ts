import { IsBoolean, IsString } from 'class-validator';

export class ClientTypingDto {
  @IsString()
  name: string;

  @IsString()
  fromRoom: string;

  @IsBoolean()
  isTyping: boolean;
}
