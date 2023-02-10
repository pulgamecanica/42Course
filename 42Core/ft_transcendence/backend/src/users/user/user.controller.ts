import {
  Body,
  Controller,
  Delete,
  Get,
  Param,
  Patch,
  Post,
  Put,
  Req,
  UseGuards,
  Query,
  ClassSerializerInterceptor,
  UseInterceptors,
} from '@nestjs/common';
import { Jwt2faAuthGuard } from 'src/auth/guards/jwt-2fa-auth.guard';
import { CreateUserDto } from './dto/create-user.dto';
import { UpdateUserDto } from './dto/update-user.dto';
import { SearchPostsQuery } from './dto/search-users.dto';
import { UserService } from './user.service';
import { AdminGuard } from 'src/auth/guards/roles.guard';
import { SettingsService } from 'src/users/settings/settings.service';
import { UploadImageDto } from './dto/upload-image.dto';

@UseGuards(Jwt2faAuthGuard)
@UseInterceptors(ClassSerializerInterceptor)
@Controller('users')
export class UserController {
  constructor(
    private readonly userService: UserService,
    private readonly settingsService: SettingsService,
  ) {}

  /*******************************************/
  /***           Search User               ***/
  /*******************************************/

  @Get('searchUsers')
  async searchUsers(@Req() req, @Query() { search }: SearchPostsQuery) {
    console.log('Searching by query', search);
    return await this.userService.searchUsers(search);
    // Concider searching for all users exept the ones you've blocked or the ones who blocked you...
  }

  /*******************************************/
  /***                 Me                  ***/
  /*******************************************/

  @Get('me')
  async findMe(@Req() req) {
    return await this.userService.findMe(req.user.id);
  }

  @Patch('me')
  async updateMe(@Req() req, @Body() updateUserDto: UpdateUserDto) {
    console.log('updateMe', updateUserDto);
    return await this.userService.update(req.user.id, updateUserDto);
  }

  @Put('me/image')
  async updateMeImage(@Req() req, @Body() uploadImage: UploadImageDto) {
    console.log('updateMeImage', uploadImage);
    // call service to upload image
    return await this.userService.updateImage(req.user.id, uploadImage.img);
  }

  @Get('me/achievements') // == @Get(':userName42/achievements')
  async findMyAchievements(@Req() req) {
    return await this.userService.findMyAchievements(req.user.id);
  }

  @Get('me/friends') // == @Get(':userName42/friends')
  async findMyFriends(@Req() req) {
    return await this.userService.findMyFriends(req.user.id);
  }

  @Get('me/history') // == @Get(':userName42/history')
  async findMyHistory(@Req() req) {
    return await this.userService.findMyHistory(req.user.id);
  }

  @Get('me/friend-requests-sent')
  async findMyFriendRequestsSent(@Req() req) {
    return await this.userService.findMyFriendRequestsSent(req.user.id);
  }

  @Get('me/friend-requests-received')
  async findMyFriendRequestsReceived(@Req() req) {
    return await this.userService.findMyFriendRequestsReceived(req.user.id);
  }

  @Get('me/blocked-users')
  async findMyBlockedUsers(@Req() req) {
    return await this.userService.findMyBlockedUsers(req.user.id);
  }

  @Get('me/settings')
  async findSettings(@Req() req) {
    return await this.settingsService.findUserSettings(req.user.id);
  }

  @Patch('me/defaultSetting/:settingId')
  async setDefaultSetting(@Req() req, @Param('settingId') settingId: number) {
    return await this.userService.setDefaultSetting(req.user.id, settingId);
  }

  /*******************************************/
  /***                Find                 ***/
  /*******************************************/

  @Get(':userName42/find')
  async findUserByUserName42(@Param('userName42') username42: string) {
    return await this.userService.findUserByUserName42(username42);
  }

  @Get(':userName42')
  async findProfileByUserName42(
    @Req() req,
    @Param('userName42') username42: string,
  ) {
    return await this.userService.findProfileByUserName42(
      username42,
      req.user.id,
    );
  }

  @Get(':userName42/achievements')
  async findAchievementsByUserName(
    @Req() req,
    @Param('userName42') username42: string,
  ) {
    return await this.userService.findAchievementsByUserName(
      username42,
      req.user.id,
    );
  }

  @Get(':userName42/friends')
  async findFriendsByUserName(
    @Req() req,
    @Param('userName42') username42: string,
  ) {
    return await this.userService.findFriendsByUserName(
      username42,
      req.user.id,
    );
  }

  @Get(':userName42/history')
  async findHistoryByUserName(
    @Req() req,
    @Param('userName42') username42: string,
  ) {
    return await this.userService.findHistoryByUserName(
      username42,
      req.user.id,
    );
  }

  /*******************************************/
  /***               Standard              ***/
  /*******************************************/

  @Get()
  async findAll(@Req() req: any) {
    // TOOD => Return All users where you don't have [relation : blocked]
    return await this.userService.findAll();
  }

  @UseGuards(AdminGuard)
  @Post()
  async create(@Body() createUserDto: CreateUserDto) {
    return await this.userService.create(createUserDto);
  }

  @UseGuards(AdminGuard)
  @Patch(':id')
  async update(@Param('id') id: string, @Body() updateUserDto: UpdateUserDto) {
    return await this.userService.update(id, updateUserDto);
  }

  @UseGuards(AdminGuard)
  @Delete(':id')
  async remove(@Param('id') id: string) {
    return await this.userService.remove(id);
  }

  @Get(':userName42/pong-stats')
  async findPongStats(
    @Req() req,
    @Query('simple') simple: boolean,
    @Param('userName42') username42: string,
  ) {
    // Defaults to isSimple == true. If the query param 'simple' is provided, it's value will be used.
    const isSimple = simple === undefined ? true : simple;

    if (username42 === 'me') {
      return await this.userService.findPongStats(+req.user.id, isSimple);
    }

    const user = await this.userService.findUserByUserName42(username42);

    if (user) {
      return await this.userService.findPongStats(user.id, isSimple);
    }
  }

  @Get(':username42/games')
  findGames(@Param('username42') username42: string) {
    return this.userService.findGames(username42 + '');
  }

  /*
  @Get(':id/history')
  findHistory(@Param('id') id: number) {
    return this.userService.findHistory('' + id);
  }

  @Get(':id/friends')
  findFriends(@Param('id') id: number) {
    return this.userService.findFriends('' + id);
  }

  @Get(':id/requests')
  findFriendRequests(@Param('id') id: number) {
    return this.userService.findFriendRequests('' + id);
  }

  @Get(':id/matches')
  findMatches(@Param('id') id: number) {
    return this.userService.findMatches('' + id);
  }

  @Get(':id/games')
  findGames(@Param('id') id: number) {
    return this.userService.findGames('' + id);
  }

  @Get(':id/blocked')
  findBlocked(@Param('id') id: number) {
    return this.userService.findBlocked('' + id);
  }

  @Get(':id/findDMS')
  findDirectMessages(
    @Param('id') id: number,
    @Query('other_id') other_id: number,
  ) {
    return this.userService.findDirectMessages(id, other_id);
  }
*/
}
