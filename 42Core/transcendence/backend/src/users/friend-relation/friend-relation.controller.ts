import {
  Controller,
  Get,
  Post,
  Body,
  Patch,
  Param,
  Delete,
  Req,
  UseGuards,
} from '@nestjs/common';
import { FriendRelationService } from './friend-relation.service';
import { CreateFriendRelationDto } from './dto/create-friend-relation.dto';
import { UpdateFriendRelationDto } from './dto/update-friend-relation.dto';
import { Jwt2faAuthGuard } from 'src/auth/guards/jwt-2fa-auth.guard';
import { ChatUserService } from 'src/chat/chat-user/chat-user.service';

@UseGuards(Jwt2faAuthGuard)
@Controller('friend-relation')
export class FriendRelationController {
  constructor(
    private readonly friendRelationService: FriendRelationService,
    private readonly chatUserService: ChatUserService,
  ) {}

  /*******************************************/
  /***               Standard              ***/
  /*******************************************/

  @Get()
  findAll() {
    return this.friendRelationService.findAll();
  }

  /*******************************************/
  /***                Friend               ***/
  /*******************************************/

  @Post(':userName42/block')
  blockByUserName42(@Param('userName42') username42: string, @Req() req: any) {
    this.chatUserService.blockUser(req.user.id, username42);
    return this.friendRelationService.blockByUserName42(
      req.user.id,
      username42,
    );
  }

  @Post(':userName42/send-friend-request')
  sendRequestByUserName42(
    @Param('userName42') username42: string,
    @Req() req: any,
  ) {
    return this.friendRelationService.sendRequestByUserName42(
      req.user.id,
      username42,
    );
  }

  @Patch(':userName42/accept-friend-request')
  acceptRequestUserByUserName42(
    @Param('userName42') username42: string,
    @Req() req: any,
  ) {
    return this.friendRelationService.acceptRequestUserByUserName42(
      req.user.id,
      username42,
    );
  }

  @Patch(':userName42/reject-friend-request')
  rejectRequestUserByUserName42(
    @Param('userName42') username42: string,
    @Req() req: any,
  ) {
    return this.friendRelationService.rejectRequestUserByUserName42(
      req.user.id,
      username42,
    );
  }

  @Delete(':userName42/remove-friend')
  removeFriendByUserName42(
    @Param('userName42') username42: string,
    @Req() req: any,
  ) {
    return this.friendRelationService.removeFriendByUserName42(
      req.user.id,
      username42,
    );
  }

  @Delete(':userName42/unblock')
  unblockByUserName42(
    @Param('userName42') username42: string,
    @Req() req: any,
  ) {
    this.chatUserService.unblockUser(req.user.id, username42);
    return this.friendRelationService.unblockByUserName42(
      req.user.id,
      username42,
    );
  }
}
