import { IsArray, IsBoolean, IsString } from 'class-validator';

export class CreateChatUserDto {
  @IsString()
  socketId?: string;

  @IsString()
  color?: string;

  @IsBoolean()
  showDate?: boolean;

  @IsBoolean()
  showTime?: boolean;

  @IsArray()
  blockedUsers?: string[];

  @IsArray()
  connectedRooms?: string[];

  @IsArray()
  connectedRoomsDirectMessage?: string[];
}
