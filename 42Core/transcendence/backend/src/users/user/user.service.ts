import {
  Injectable,
  NotFoundException,
  BadRequestException,
  UnprocessableEntityException,
} from '@nestjs/common';
import { InjectRepository } from '@nestjs/typeorm';
import { Repository, ILike, Raw, In, QueryFailedError } from 'typeorm';
import { CreateUserDto } from './dto/create-user.dto';
import { UpdateUserDto } from './dto/update-user.dto';
import { User, UserStatus } from './entities/user.entity';
import { UserHistory } from '../user-history/entities/user-history.entity';
import { Setting } from '../settings/entities/setting.entity';
import { FriendRelation } from '../friend-relation/entities/friend-relation.entity';
import { FriendStatus } from '../friend-relation/entities/friend-relation.entity';
import { Achievement } from '../achievement/entities/achievement.entity';
import { GameStatus, PongGame } from 'src/pong-game/entities/pong-game.entity';
import { RelationOptions } from '../friend-relation/entities/friend-relation.entity';
import { ChatUserService } from 'src/chat/chat-user/chat-user.service';
import { AchievementService } from 'src/users/achievement/achievement.service';
import axios from 'axios';

export type SimpleStats = {
  rank: number;
  ranking: number;
};

export type Stats = SimpleStats & {
  totalGames: number;
  finished: number;
  suspended: number;
  won: number;
  lost: number;
};

@Injectable()
export class UserService {
  constructor(
    @InjectRepository(User)
    private readonly userRepository: Repository<User>,
    @InjectRepository(UserHistory)
    private readonly userHistoryRepository: Repository<UserHistory>,
    @InjectRepository(FriendRelation)
    private readonly friendRelationRepository: Repository<FriendRelation>,
    @InjectRepository(PongGame)
    private readonly pongGameRepository: Repository<PongGame>,
    @InjectRepository(Achievement)
    private readonly achievementRepository: Repository<Achievement>,
    @InjectRepository(Setting)
    private readonly settingsRepository: Repository<Setting>,
    private readonly chatUserService: ChatUserService,
    private readonly achievementService: AchievementService,
  ) {}

  /*******************************************/
  /***            Standard Logic           ***/
  /*******************************************/
  async findAll() {
    return await this.userRepository.find({
      relations: ['chatUser', 'chatUser.chatRoomDirectMessage'],
    });
  }

  async findAllUsersStatus() {
    const users = await this.userRepository.find({
      select: ['username42', 'status'],
    });
    const usersMap: {
      [username42: string]: string;
    } = {};
    for (const user of users) {
      usersMap[user.username42] = user.status;
    }
    return usersMap;
  }

  async update(id: string, updateUserDto: UpdateUserDto) {
    const user = await this.userRepository.preload({
      id: +id,
      ...updateUserDto,
    });
    if (!user) {
      throw new NotFoundException(`User #${id} not found`);
    }
    return await this.userRepository.save(user).catch((err: any) => {
      const message =
        typeof err === 'string'
          ? err
          : err.detail
          ? err.detail
          : err.message
          ? err.message
          : '';
      if (message) {
        throw new UnprocessableEntityException(message);
      } else {
        console.log('message not found');
      }
    });
  }

  async create(createUserDto: CreateUserDto) {
    if (await this.findUserByUserName42(createUserDto.username42)) {
      throw new NotFoundException(
        `${createUserDto.username42} already exists!`,
      );
    }
    const chatUser = await this.chatUserService.create(
      createUserDto.username42,
    );

    const newUser = this.userRepository.create(createUserDto);
    newUser.chatUser = chatUser;

    return await this.userRepository.save(newUser).catch((err: any) => {
      const message =
        typeof err === 'string'
          ? err
          : err.detail
          ? err.detail
          : err.message
          ? err.message
          : '';
      if (message) {
        throw new UnprocessableEntityException(message);
      } else {
        console.log('message not found');
      }
    });
  }

  async remove(id: string) {
    const user = await this.userRepository.findOne({ where: { id: +id } });
    return await this.userRepository.remove(user);
  }

  /*******************************************/
  /***             Update Image            ***/
  /*******************************************/
  async updateImage(id: string, img: any) {
    const user = await this.userRepository.findOne({ where: { id: +id } });
    if (!user) {
      throw new NotFoundException(`User #${id} not found`);
    }
    // upload to external service via axios
    const response = await axios.post(process.env.IMAGE_UPLOAD_URL, {
      key: process.env.IMAGE_UPLOAD_KEY,
      image: img,
      name: user.username42,
    });

    const { status, data } = response;
    if (status !== 200) {
      throw new BadRequestException(`Image upload failed`);
    }

    const url = data.thumb.url;
    user.avatarImg = url;
    return await this.userRepository.save(user);
  }

  /*******************************************/
  /***               Me Logic              ***/
  /*******************************************/

  async findMe(userId: string) {
    const user = await this.userRepository.findOne({
      where: {
        id: +userId,
      },
      relations: ['defaultSetting', 'achievements'],
    });
    this.achievementService.checkUserAchievements(user);
    return user;
  }

  async findMyAchievements(userId: string) {
    const user = await this.findMe(userId);
    if (!user) {
      throw new NotFoundException(`User #${userId} not found`);
    }
    return user.achievements;
  }

  async findRelationsUsers(userId: string, friendStatus: FriendStatus) {
    const user = await this.findMe(userId);
    if (!user) {
      throw new NotFoundException(`User #${userId} not found`);
    }
    const friends = await this.friendRelationRepository.find({
      where: [
        { userOneId: user.id, status: friendStatus },
        { userTwoId: user.id, status: friendStatus },
      ],
    });

    return this.userRepository.find({
      where: [
        {
          id: In(
            friends.map((friend: FriendRelation) => {
              if (friend.userOneId === user.id) {
                return friend.userTwoId;
              } else if (friend.userTwoId === user.id) {
                return friend.userOneId;
              }
            }),
          ),
        },
      ],
      order: {
        status: 'ASC',
      },
    });
  }

  async findMyFriends(userId: string) {
    return this.findRelationsUsers(userId, FriendStatus.FRIENDS);
  }

  async findMyFriendRequestsSent(userId: string) {
    const user = await this.findMe(userId);
    if (!user) {
      throw new NotFoundException(`User #${userId} not found`);
    }
    const friends = await this.friendRelationRepository.find({
      where: [{ userOneId: user.id, status: FriendStatus.REQUEST }],
    });
    return this.userRepository.findByIds(
      friends.map((friend: FriendRelation) => {
        if (friend.userOneId === user.id) {
          return friend.userTwoId;
        } else if (friend.userTwoId === user.id) {
          return friend.userOneId;
        }
      }),
    );
  }

  async findMyFriendRequestsReceived(userId: string) {
    const user = await this.findMe(userId);
    if (!user) {
      throw new NotFoundException(`User #${userId} not found`);
    }
    const friends = await this.friendRelationRepository.find({
      where: [{ userTwoId: user.id, status: FriendStatus.REQUEST }],
    });
    return this.userRepository.findByIds(
      friends.map((friend: FriendRelation) => {
        if (friend.userOneId === user.id) {
          return friend.userTwoId;
        } else if (friend.userTwoId === user.id) {
          return friend.userOneId;
        }
      }),
    );
  }

  async findMyBlockedUsers(userId: string) {
    /* Blocked users should not return the users but the id of the users */
    // return this.findRelationsUsers(userId, FriendStatus.BLOCKED);
    const user = await this.findMe(userId);
    if (!user) {
      throw new NotFoundException(`User #${userId} not found`);
    }
    const blocked = await this.friendRelationRepository.find({
      where: [{ userOneId: user.id, status: FriendStatus.BLOCKED }],
    });
    return this.userRepository.findByIds(
      blocked.map((blockedUser: FriendRelation) => {
        return blockedUser.userTwoId;
      }),
    );
  }

  async findMyRelation(userId: string, friendId: string) {
    const user = await this.findMe(userId);
    if (!user) {
      throw new NotFoundException(`User #${userId} not found`);
    }
    const relation = await this.friendRelationRepository.find({
      where: [
        { userOneId: user.id, userTwoId: +friendId },
        { userOneId: +friendId, userTwoId: user.id },
      ],
    });
    if (relation.length == 0) {
      return null;
    }
    return relation[0];
  }

  async findMyHistory(userId: string) {
    const user = await this.findMe(userId);
    if (!user) {
      throw new NotFoundException(`User #${userId} not found`);
    }
    const userHistory = await this.userHistoryRepository.find({
      where: {
        userId: user.id,
      },
    });
    return userHistory;
  }

  async setDefaultSetting(userId: number, settingId: number) {
    if (Number.isNaN(settingId))
      throw new BadRequestException(`#${settingId} is not a valid number`);
    const setting = await this.settingsRepository.findOne({
      where: {
        id: settingId,
      },
    });
    const user = await this.findMe(userId + '');
    if (!setting || !user) {
      throw new NotFoundException(
        `Setting #${settingId} not found or user #${userId}`,
      );
    }
    if (setting.userId != userId) {
      throw new NotFoundException(
        `Setting #${settingId} doesn't belong to user #${userId}`,
      );
    }
    user.defaultSetting = setting;
    return await this.userRepository.save(user);
  }

  async findPongGamesByStatus(userId: number, status: GameStatus) {
    return this.pongGameRepository.find({
      where: [
        { userOneId: +userId, status: status },
        { userTwoId: +userId, status: status },
      ],
    });
  }

  async findPongStats(
    userId: number,
    simple: boolean,
  ): Promise<SimpleStats | Stats> {
    const rankInfo = await this.calculateRank(userId);

    if (simple) {
      return rankInfo;
    }

    const suspendedGames = await this.findPongGamesByStatus(
      userId,
      GameStatus.SUSPENDED,
    );

    const finishedGames = await this.findPongGamesByStatus(
      userId,
      GameStatus.FINISHED,
    );

    return {
      totalGames: suspendedGames.length + finishedGames.length,
      finished: finishedGames.length,
      suspended: suspendedGames.length,
      won: finishedGames.filter((game) => {
        return (
          (game.userOneId === userId && game.scoreOne > game.scoreTwo) ||
          (game.userTwoId === userId && game.scoreTwo > game.scoreOne)
        );
      }).length,
      lost: finishedGames.filter((game) => {
        return (
          (game.userOneId === userId && game.scoreOne < game.scoreTwo) ||
          (game.userTwoId === userId && game.scoreTwo < game.scoreOne)
        );
      }).length,
      rank: rankInfo.rank,
      ranking: rankInfo.ranking,
    };
  }

  /*******************************************/
  /***             OTHER STUFF             ***/
  /*******************************************/

  /*
  async findMatches(id: string) {
    const user = await this.findMe(id);
    const matches = await this.matchMackerRepository.find({
      where: [{ userOne: user }, { userTwo: user }],
      relations: ['userOne', 'userTwo'],
    });
    return matches;
  }

  async findGames(id: string) {
    const user = await this.findMe(id);
    if (!user) return 'User not found...';
    const games = await this.pongGameRepository.find({
      where: [{ userOne: user }, { userTwo: user }],
      relations: ['userOne', 'userTwo'],
    });
    return games;
  }

  async findDirectMessages(id: number, other_id: number) {
    if (!id || !other_id) return 'Invalid User...';
    const user1 = await this.findMe('' + id);
    const user2 = await this.findMe('' + other_id);
    if (!user1 || !user2) return 'Either User not found...';
    const dms = await this.chatDirectMessageRepository.find({
      where: [
        { userOne: user1 },
        { userTwo: user2 },
        { userOne: user2 },
        { userTwo: user1 },
      ],
      relations: ['userOne', 'userTwo'],
    });
    return dms;
  }
  */

  async findGames(username42: string) {
    const user = await this.findUserByUserName42(username42);

    const games = await this.pongGameRepository.find({
      where: [{ userOneId: user.id }, { userTwoId: user.id }],
      relations: ['userOne', 'userTwo'],
    });

    return games;
  }

  /*******************************************/
  /***        Find username42 Logic        ***/
  /*******************************************/

  async findFriendByUserName42(username42: string, userId: string) {
    const friend = await this.userRepository.findOneBy({
      username42: username42,
    });
    if (!friend) {
      throw new NotFoundException(`UserName42 #${username42} not found`);
    }
    const userBlocked = await this.findMyBlockedUsers(userId);
    const friendBlocked = await this.findMyBlockedUsers('' + friend.id);
    if (friend.id != +userId) {
      if (
        userBlocked.filter((data) => +data.id === +friend.id).map((res) => res)
          .length
      ) {
        throw new NotFoundException(`#You Blocked ${username42}`);
      } else if (
        friendBlocked.filter((data) => +data.id === +userId).map((res) => res)
          .length
      ) {
        throw new NotFoundException(`#${username42} Blocked You`);
      }
    }
    return friend;
  }

  async findProfileByUserName42(username42: string, userId: string) {
    const user = await this.findFriendByUserName42(username42, userId);
    const friends = await this.findMyFriends('' + user.id);
    const achievements = await this.findMyAchievements('' + user.id);
    const history = await this.findMyHistory('' + user.id);
    const relation = await this.findMyRelation(userId, '' + user.id);
    const relationOptions: RelationOptions[] = [];
    relationOptions.push(RelationOptions.Block);
    if (relation == null) {
      relationOptions.push(RelationOptions.SendRelation);
    } else if (
      relation.status == FriendStatus.REQUEST &&
      relation.userOneId == +userId
    ) {
      relationOptions.push(RelationOptions.Sent);
    } else if (
      relation.status == FriendStatus.REQUEST &&
      relation.userTwoId == +userId
    ) {
      relationOptions.push(RelationOptions.AcceptRelation);
      relationOptions.push(RelationOptions.DeclineRelation);
    } else if (relation.status == FriendStatus.FRIENDS) {
      relationOptions.push(RelationOptions.Unfriend);
    }
    return { ...user, friends, achievements, history, relationOptions };
  }

  async findAchievementsByUserName(username42: string, userId: string) {
    const user = await this.findFriendByUserName42(username42, userId);
    return await this.findMyAchievements('' + user.id);
  }

  async findFriendsByUserName(username42: string, userId: string) {
    const user = await this.findFriendByUserName42(username42, userId);
    return await this.findMyFriends('' + user.id);
  }

  async findHistoryByUserName(username42: string, userId: string) {
    const user = await this.findFriendByUserName42(username42, userId);
    return await this.findMyHistory('' + user.id);
  }

  /*******************************************/
  /***         Search Users Logic          ***/
  /*******************************************/

  async searchUsers(searchQuery?: string) {
    if (searchQuery) {
      console.log('Query', searchQuery);
      return this.userRepository.find({
        where: [
          { username42: ILike(`%${searchQuery}%`) },
          { firstName: ILike(`%${searchQuery}%`) },
          { lastName: ILike(`%${searchQuery}%`) },
          { nickname: ILike(`%${searchQuery}%`) },
        ],
      });
    }
    return await this.findAll();
    //blocked.map((blockedUser: FriendRelation) => {
    //    return blockedUser.userTwoId;
    //})
  }

  /*******************************************/
  /***      Find  Guard Logic              ***/
  /*******************************************/

  async findUserByUserName42(username42: string) {
    const user = await this.userRepository.findOne({
      where: {
        username42: username42,
      },
      relations: ['chatUser'],
    });
    if (!user) {
      return null;
    }

    return user;
  }

  async findUserByIntraId(intraId: number) {
    const user = await this.userRepository.findOne({
      where: {
        intraId,
      },
    });
    return user;
  }

  async findUserById(userId: string) {
    const user = await this.userRepository.findOne({
      where: {
        id: +userId,
      },
    });
    return user;
  }

  /*******************************************/
  /***   Two Factor Authentication Logic   ***/
  /*******************************************/

  async setTwoFactorAuthenticationSecret(secret: string, username42: string) {
    const user = await this.findUserByUserName42(username42);
    user.twoFactorAuthenticationSecret = secret;

    return await this.userRepository.save(user);
  }

  async setTwoFactorAuthenticationOnAndOff(username42: string) {
    const user = await this.findUserByUserName42(username42);

    if (user.isTwoFactorAuthenticationEnabled === true) {
      user.isTwoFactorAuthenticationEnabled = false;
      user.twoFactorAuthenticationSecret = '';
    } else {
      user.isTwoFactorAuthenticationEnabled = true;
    }

    return await this.userRepository.save(user);
  }

  /*******************************************/
  /***            Standard Logic           ***/
  /*******************************************/
  async findByUsername42Relations(username42: string) {
    const user = await this.userRepository.find({
      where: {
        username42: username42,
      },
      relations: ['chatUser', 'chatUser.chatRoomDirectMessage'],
    });
    if (!user) {
      return null;
    }

    return user[0];
  }

  async findById(id: number) {
    const user = await this.userRepository.findOne({
      where: {
        id: id,
      },
      relations: ['chatUser'],
    });
    if (!user) {
      return null;
    }

    return user;
  }

  async updateUser(username42: string, updateUserDto: UpdateUserDto) {
    const user = await this.findUserByUserName42(username42);
    if (!user) {
      return `${username42} not found.`;
    }
    await this.userRepository.update(user.id, updateUserDto);

    return await this.userRepository.findOne({ where: { id: user.id } });
  }

  async updateUserStatus(username42: string, status: UserStatus) {
    const user = await this.findUserByUserName42(username42);
    if (!user) {
      return `${username42} not found.`;
    }
    await this.userRepository.update(user.id, { status: status });
  }

  // It deletes the chat-user and the user
  async delete(username42: string) {
    const user = await this.findUserByUserName42(username42);
    if (!user) {
      return 'User not found';
    }
    await this.chatUserService.delete(user.chatUser);
    return await this.userRepository.remove(user);
  }

  async findLadderPongGames() {
    const games = await this.pongGameRepository.find({
      where: [{ isLadder: true, status: GameStatus.FINISHED }],
    });

    const ladderGames = games.map((game: PongGame) => {
      if (game.scoreOne === game.scoreTwo) {
        return {
          // It doesn't matter who is the winner or the loser, since it's a tie
          winner: game.userOneId,
          loser: game.userTwoId,
          winnerScore: game.scoreOne,
          loserScore: game.scoreTwo,
          tie: true,
        };
      }

      if (game.scoreOne > game.scoreTwo) {
        return {
          winner: game.userOneId,
          loser: game.userTwoId,
          winnerScore: game.scoreOne,
          loserScore: game.scoreTwo,
          tie: false,
        };
      } else {
        return {
          winner: game.userTwoId,
          loser: game.userOneId,
          winnerScore: game.scoreTwo,
          loserScore: game.scoreOne,
          tie: false,
        };
      }
    });

    return ladderGames;
  }

  async calculateRank(userId: number): Promise<SimpleStats> {
    const games = await this.findLadderPongGames();

    // Calculate ranking
    const players = [];
    games.forEach((game) => {
      const rankingScale = 100;

      if (game.tie === true) {
        const score = rankingScale / 2;

        const playerOne = players.find((p) => p.id == game.winner);
        const playerTwo = players.find((p) => p.id == game.loser);

        if (!playerOne) {
          players.push({
            id: game.winner,
            ranking: score,
            rank: null,
          });
        } else {
          playerOne.ranking += score;
        }

        if (!playerTwo) {
          players.push({
            id: game.loser,
            ranking: score,
            rank: null,
          });
        } else {
          playerTwo.ranking += score;
        }
      } else {
        const bonusRanking = rankingScale - (game.loserScore * rankingScale) / game.winnerScore;

        // Winner gets 'rankingScale' ranking + bonus ranking, which vary depending on the score difference.
        const score = rankingScale + bonusRanking;

        const winner = players.find((player) => player.id == game.winner);
        const loser = players.find((player) => player.id == game.loser);

        if (!winner) {
          // If the winner of this game is not yet on the list, add him.
          players.push({
            id: game.winner,
            ranking: score,
            rank: null,
          });
        } else {
          winner.ranking += score;
        }

        if (!loser) {
          players.push({
            id: game.loser,
            ranking: 0,
            rank: null,
          });
        }
      }
    });

    // Sort players by ranking
    players.sort((a, b) => b.ranking - a.ranking);

    let currentRank = 1;
    for (let i = 0; i < players.length; i++) {
      if (i !== 0 && players[i - 1].ranking === players[i].ranking) {
        players[i].rank = players[i - 1].rank;
      } else {
        players[i].rank = currentRank;
        currentRank++;
      }
    }

    // Zero rank for players with ranking 0
    players.forEach((player) => {
      if (player.ranking === 0) {
        player.rank = 0;
      }
    });

    // find userid in players
    const stats = players.find((player) => player.id == userId);

    if (!stats) {
      return {
        rank: null,
        ranking: 0,
      };
    }

    return {
      rank: stats.rank,
      ranking: Math.round(stats.ranking),
    };
  }
}
