import { IsString, IsNotEmpty, IsOptional } from 'class-validator';
 
export class SearchPostsQuery {
  @IsString()
  @IsNotEmpty()
  @IsOptional()
  search?: string;
}