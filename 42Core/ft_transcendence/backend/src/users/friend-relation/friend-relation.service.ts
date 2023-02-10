import {
  Injectable,
  NotFoundException,
  ConflictException,
  NotImplementedException,
  Inject,
} from '@nestjs/common';
import { InjectRepository } from '@nestjs/typeorm';
import { Repository, Connection } from 'typeorm';
import { CreateFriendRelationDto } from './dto/create-friend-relation.dto';
import { UpdateFriendRelationDto } from './dto/update-friend-relation.dto';
import { FriendStatus } from './entities/friend-relation.entity';
import { FriendRelation } from './entities/friend-relation.entity';
import { UserHistory } from 'src/users/user-history/entities/user-history.entity';
import { UserHistorySubject } from 'src/users/user-history/entities/user-history.entity';
import { UserService } from 'src/users/user/user.service';

@Injectable()
export class FriendRelationService {
  @Inject(UserService)
  private readonly userService: UserService;
  constructor(
    @InjectRepository(FriendRelation)
    private readonly friendRelationRepository: Repository<FriendRelation>,
    private readonly connection: Connection,
  ) {}

  /*******************************************/
  /***            Standard Logic           ***/
  /*******************************************/

  findAll() {
    return this.friendRelationRepository.find({});
  }

  /*******************************************/
  /***             Friend Logic            ***/
  /*******************************************/

  async findOne(id: string) {
    const relation = await this.friendRelationRepository.findOne({
      where: {
        id: +id,
      },
      relations: ['userOne', 'userTwo'],
    });
    if (!relation) {
      throw new NotFoundException(`Relation #${id} not found`);
    }
    return relation;
  }

  async findPreviousRelationsWithUserName42(
    current_user: any,
    user42: any,
  ): Promise<FriendRelation> {
    const previous_relation = await this.friendRelationRepository.find({
      where: [
        { userOneId: current_user.id, userTwoId: user42.id },
        { userOneId: user42.id, userTwoId: current_user.id },
      ],
    });
    if (!current_user || !user42) {
      throw new NotFoundException(`User not found`);
    } else if (user42.id === current_user.id) {
      throw new ConflictException(`Cannot have a relationship with yourselve`);
    } else if (previous_relation.length == 1) {
      return previous_relation[0];
    }
    return null;
  }

  async blockByUserName42(
    current_user: number,
    requested_userName42: string,
  ): Promise<any> {
    const user = await this.userService.findMe('' + current_user);
    const blocked_user = await this.userService.findUserByUserName42(requested_userName42);
    const relation = await this.findPreviousRelationsWithUserName42(
      user,
      blocked_user,
    );
    if (relation) {
      if (relation.status === FriendStatus.BLOCKED && relation.userTwoId === user.id) {
        throw new ConflictException(
          `${blocked_user} blocked you, so you can't block them`,
        );
      }
      if (user.id === relation.userTwoId) {
        relation.userOne = user;
        relation.userOneId = user.id;
        relation.userTwo = blocked_user;
        relation.userTwoId = blocked_user.id;
      }
      relation.status = FriendStatus.BLOCKED;
      return this.friendRelationRepository.save(relation);
    } else {
      const new_relation = new FriendRelation();
      new_relation.userOne = user;
      new_relation.userTwo = blocked_user;
      new_relation.status = FriendStatus.BLOCKED;
      return this.friendRelationRepository.save(new_relation);
    }
  }

  async sendRequestByUserName42(
    current_user: number,
    requested_userName42: string,
  ): Promise<any> {
    const requesting_user = await this.userService.findMe('' + current_user);
    const requested_user = await this.userService.findUserByUserName42(
      requested_userName42,
    );
    const previous_relation = await this.findPreviousRelationsWithUserName42(
      requesting_user,
      requested_user,
    );
    if (previous_relation) {
      throw new ConflictException(`A relationship already exists`);
    }
    const queryRunner = this.connection.createQueryRunner();
    await queryRunner.connect();
    await queryRunner.startTransaction();
    try {
      const relation = new FriendRelation();
      relation.userOne = requesting_user;
      relation.userTwo = requested_user;
      relation.status = FriendStatus.REQUEST;
      // If there is a notification in the future, here it would be sent...
      await queryRunner.manager.save(relation);
      await queryRunner.commitTransaction();
      return relation;
    } catch (err) {
      await queryRunner.rollbackTransaction();
    } finally {
      await queryRunner.release();
    }
  }

  async acceptRequestUserByUserName42(
    current_user: number,
    requested_userName42: string,
  ): Promise<any> {
    const user_accepting = await this.userService.findMe('' + current_user);
    const user_requested = await this.userService.findUserByUserName42(
      requested_userName42,
    );
    const previous_relation = await this.findPreviousRelationsWithUserName42(
      user_accepting,
      user_requested,
    );
    if (!previous_relation) {
      throw new ConflictException(
        `${requested_userName42} has not requested friendship`,
      );
    } else if (previous_relation.status != FriendStatus.REQUEST) {
      throw new ConflictException(
        `${requested_userName42} has not requested friendship, has another status`,
      );
    } else if (
      previous_relation.status === FriendStatus.REQUEST &&
      previous_relation.userTwoId != user_accepting.id
    ) {
      throw new ConflictException(
        `${requested_userName42} must be the one accepting the request`,
      );
    }
    return this.update('' + previous_relation.id, {
      status: FriendStatus.FRIENDS,
    });
  }

  async rejectRequestUserByUserName42(
    current_user: number,
    requested_userName42: string,
  ): Promise<any> {
    // Reject or Destroy a request you sent
    const requesting_user = await this.userService.findMe('' + current_user);
    const requested_user = await this.userService.findUserByUserName42(
      requested_userName42,
    );
    const previous_relation = await this.findPreviousRelationsWithUserName42(
      requesting_user,
      requested_user,
    );
    if (
      !previous_relation ||
      previous_relation.status != FriendStatus.REQUEST
    ) {
      throw new ConflictException(
        `${requested_userName42} has not requested friendship or has another status`,
      );
    }
    return this.remove('' + previous_relation.id);
  }

  async removeFriendByUserName42(
    current_user: number,
    requested_userName42: string,
  ): Promise<any> {
    const user_removing = await this.userService.findMe('' + current_user);
    const user_removed = await this.userService.findUserByUserName42(
      requested_userName42,
    );
    const previous_relation = await this.findPreviousRelationsWithUserName42(
      user_removing,
      user_removed,
    );
    if (
      !previous_relation ||
      previous_relation.status != FriendStatus.FRIENDS
    ) {
      throw new ConflictException(
        `${requested_userName42} and you are not friends`,
      );
    }
    return this.remove('' + previous_relation.id);
  }

  async unblockByUserName42(
    current_user: number,
    requested_userName42: string,
  ): Promise<any> {
    const user_unblocking = await this.userService.findMe('' + current_user);
    const user_blocked = await this.userService.findUserByUserName42(
      requested_userName42,
    );
    const previous_relation = await this.findPreviousRelationsWithUserName42(
      user_unblocking,
      user_blocked,
    );
    if (
      !previous_relation ||
      previous_relation.status != FriendStatus.BLOCKED
    ) {
      throw new ConflictException(
        `${requested_userName42} doesn't have you blocked`,
      );
    } else if (
      previous_relation.status === FriendStatus.BLOCKED &&
      previous_relation.userTwoId === user_unblocking.id
    ) {
      throw new ConflictException(
        `${requested_userName42} blocked you, only they can unblock you`,
      );
    }
    return this.remove('' + previous_relation.id);
  }

  async normal_update(
    id: string,
    updateFriendRelationDto: UpdateFriendRelationDto,
  ) {
    const relation = await this.friendRelationRepository.preload({
      id: +id,
      ...updateFriendRelationDto,
    });
    if (!relation) {
      throw new NotFoundException(`UserRelation #${id} not found`);
    }
    return this.friendRelationRepository.save(relation);
  }

  async update(id: string, updateFriendRelationDto: UpdateFriendRelationDto) {
    if (updateFriendRelationDto.status != FriendStatus.FRIENDS) {
      return this.normal_update(id, updateFriendRelationDto);
    }
    const queryRunner = this.connection.createQueryRunner();

    await queryRunner.connect();
    await queryRunner.startTransaction();
    try {
      const relation = await this.friendRelationRepository.preload({
        id: +id,
        ...updateFriendRelationDto,
      });

      const history1 = new UserHistory();
      const history2 = new UserHistory();
      history1.user = (await this.findOne('' + relation.id)).userOne;
      history2.user = (await this.findOne('' + relation.id)).userTwo;
      history1.description = `Congrats! Now you're friends with ${history2.user.firstName} ${history1.user.lastName}`;
      history2.description = `Congrats! Now you're friends with ${history1.user.firstName} ${history1.user.lastName}`;
      history1.subject = UserHistorySubject.NEW_FRIEND;
      history2.subject = UserHistorySubject.NEW_FRIEND;

      await queryRunner.manager.save(relation);
      await queryRunner.manager.save(history1);
      await queryRunner.manager.save(history2);

      await queryRunner.commitTransaction();
    } catch (err) {
      await queryRunner.rollbackTransaction();
    } finally {
      await queryRunner.release();
    }
  }

  async remove(id: string) {
    const relation = await this.friendRelationRepository.findOne({
      where: { id: +id },
    });
    return this.friendRelationRepository.remove(relation);
  }
}
