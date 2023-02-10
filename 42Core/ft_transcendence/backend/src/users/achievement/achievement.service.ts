import { Injectable, OnModuleInit, NotFoundException } from '@nestjs/common';
import { InjectRepository } from '@nestjs/typeorm';
import { Repository, Connection } from 'typeorm';
import { CreateAchievementDto } from './dto/create-achievement.dto';
import { Achievement } from './entities/achievement.entity';
import { User } from '../user/entities/user.entity';
import { FriendStatus } from '../friend-relation/entities/friend-relation.entity';
import { GameStatus } from '../../pong-game/entities/pong-game.entity';

@Injectable()
export class AchievementService implements OnModuleInit {
  constructor(
    @InjectRepository(Achievement)
    private readonly achievementRepository: Repository<Achievement>,
    @InjectRepository(User)
    private readonly userRepository: Repository<User>,
    private readonly connection: Connection,
  ) {}

  /*******************************************/
  /* Initialize the acheivements on startup  */
  /*******************************************/
  async onModuleInit() {
    const achievements = [
      {
        title: "NewUser",
        description: "You joined  more than one day ago!",
        imgUrl: "https://images.pexels.com/photos/235985/pexels-photo-235985.jpeg"
      },
      {
        title: "WeeklyUser",
        description: "Joined one Week ago, uffff you are olddd",
        imgUrl: "https://images.pexels.com/photos/1939485/pexels-photo-1939485.jpeg"
      },
      {
        title: "MonthlyUser",
        description: "You joined one month ago... WHAT? thats so long ago!",
        imgUrl: "https://images.pexels.com/photos/3343801/pexels-photo-3343801.jpeg"
      },
      {
        title: "GoldenUser",
        description: "Joined 42 days ago. You are so loyal omg!",
        imgUrl: "https://images.pexels.com/photos/2860804/pexels-photo-2860804.jpeg"
      },
      {
        title: "FirstWin",
        description: "You won your first Pong Game, Congratulations!",
        imgUrl: "https://images.pexels.com/photos/2340254/pexels-photo-2340254.jpeg"
      },
      {
        title: "1000",
        description: "You have reached ranking 1000 WoW",
        imgUrl: "https://images.pexels.com/photos/129731/pexels-photo-129731.jpeg"
      },
      {
        title: "Loser",
        description: "Lost 5 games... RIP",
        imgUrl: "https://images.pexels.com/photos/207300/pexels-photo-207300.jpeg"
      },
      {
        title: "Winner",
        description: "You won 5 games",
        imgUrl: "https://images.pexels.com/photos/235525/pexels-photo-235525.jpeg"
      },
      {
        title: "Addict",
        description: "You have played 10 games, you are such an addict",
        imgUrl: "https://images.pexels.com/photos/2457284/pexels-photo-2457284.jpeg"
      },
      {
        title: "PongKing",
        description: "You won 42 games, you are the King!",
        imgUrl: "https://images.pexels.com/photos/268966/pexels-photo-268966.jpeg"
      },
      {
        title: "Friendly",
        description: "You made 1 friend!",
        imgUrl: "https://images.pexels.com/photos/132204/pexels-photo-132204.jpeg"
      },
      {
        title: "Haters",
        description: "You blocked or were blocked (by) someone",
        imgUrl: "https://images.pexels.com/photos/695207/pexels-photo-695207.jpeg"
      },
      {
        title: "Popular",
        description: "You made 5 friends, you are so popular!",
        imgUrl: "https://images.pexels.com/photos/3144221/pexels-photo-3144221.jpeg"
      },
      {
        title: "TopUser",
        description: "You made 42 friends, you are the top popular player, w8 does this website even has 42 users???",
        imgUrl: "https://images.pexels.com/photos/317333/pexels-photo-317333.jpeg"
      }
    ]
    const currentAchievements = await this.findAll();
    for(let achievement of achievements) {
      if (currentAchievements.filter((currentAchievement) => currentAchievement.title === achievement.title).length === 0) {
        this.create(achievement)
          .then((item) => console.log("Created", item.title))
          .catch((error) => console.log(achievement.title, "was not created now"))
      }
    }
  }

  /*******************************************/
  /***            Standard Logic           ***/
  /*******************************************/
  findAll() {
    return this.achievementRepository.find();
  }

  async findOne(id: string) {
    const achievement = await this.achievementRepository.findOne({
      where: {
        id: +id,
      },
      relations: ['user'],
    });
    if (!achievement) {
      throw new NotFoundException(`Achievement #${id} not found`);
    }
    return achievement;
  }

  async findByTitle(title: string) {
    const achievement = await this.achievementRepository.findOne({
      where: {
        title: title,
      }
    });
    if (!achievement) {
      throw new NotFoundException(`Achievement #${title} not found`);
    }
    return achievement;
  }

  async checkUserAchievements(user: User) {
    const achievements = await this.findAll();
    const userAchievements = user.achievements.map((achievement) => achievement.title);
    const createdDaysAgo = Math.floor(((new Date()).getTime() - user.createdAt.getTime()) / (1000 * 3600 * 24));
    const userInfo = (await this.userRepository.find({
      where: {
        id: user.id,
      },
      relations: ['relationsOne', 'relationsTwo', 'pongGamePlayerOne', 'pongGamePlayerTwo'],
      select: {
        relationsOne: {
          status: true,
        },
        relationsTwo: {
          status: true,
        },
        pongGamePlayerOne: {
          scoreOne: true,
          scoreTwo: true,
          status: true,
        },
        pongGamePlayerTwo: {
          scoreOne: true,
          scoreTwo: true,
          status: true,
        },
      }
    }))[0];
    const userPongGamesWon = [
    ...userInfo.pongGamePlayerOne.filter((game) => game.status === GameStatus.FINISHED && game.scoreOne > game.scoreTwo),
    ...userInfo.pongGamePlayerTwo.filter((game) => game.status === GameStatus.FINISHED && game.scoreOne < game.scoreTwo),
    ];
    const userPongGamesLost = [
    ...userInfo.pongGamePlayerOne.filter((game) => game.status === GameStatus.FINISHED && game.scoreOne < game.scoreTwo),
    ...userInfo.pongGamePlayerTwo.filter((game) => game.status === GameStatus.FINISHED && game.scoreOne > game.scoreTwo),
    ];
    const userFriends = [...userInfo.relationsOne.filter((relation) => relation.status === FriendStatus.FRIENDS)];
    const userBlocked = [...userInfo.relationsOne.filter((relation) => relation.status === FriendStatus.BLOCKED)];
    const opt = {
      time: ["NewUser", "WeeklyUser", "MonthlyUser", "GoldenUser"],
      pongGame: ["FirstWin", "1000", "Loser", "Winner", "Addict", "PongKing"],
      relations: ["Friendly", "Haters", "Popular", "TopUser"],
    }
    for (let achievement of achievements) {
      if (userAchievements.includes(achievement.title)) {
        continue;
      }
      if (opt.time.includes(achievement.title)) {
        if (createdDaysAgo >= 1 && achievement.title === "NewUser") {
          this.addUserAchievement(achievement, user);
        } else if (createdDaysAgo >= 7 && achievement.title === "WeeklyUser") {
          this.addUserAchievement(achievement, user);
        } else if (createdDaysAgo >= 30 && achievement.title === "MonthlyUser") {
          this.addUserAchievement(achievement, user);
        } else if (createdDaysAgo >= 42 && achievement.title === "GoldenUser") {
          this.addUserAchievement(achievement, user);
        }
      } else if (opt.pongGame.includes(achievement.title)) {
        if (userPongGamesWon.length >= 1 && achievement.title === "FirstWin") {
          this.addUserAchievement(achievement, user);
        } else if (userPongGamesWon.length >= 10 && achievement.title === "1000") {
          this.addUserAchievement(achievement, user);
        } else if (userPongGamesLost.length >= 5 && achievement.title === "Loser") {
          this.addUserAchievement(achievement, user);
        } else if (userPongGamesWon.length >= 5 && achievement.title === "Winner") {
          this.addUserAchievement(achievement, user);
        } else if (userPongGamesWon.length + userPongGamesLost.length >= 10 && achievement.title === "Addict") {
          this.addUserAchievement(achievement, user);
        } else if (userPongGamesWon.length >= 42 && achievement.title === "PongKing") {
          this.addUserAchievement(achievement, user);
        }
      } else if (opt.relations.includes(achievement.title)) {
        if (userFriends.length >= 1 && achievement.title === "Friendly") {
          this.addUserAchievement(achievement, user);
        } else if (userBlocked.length >= 1 && achievement.title === "Haters") {
          this.addUserAchievement(achievement, user);
        } else if (userFriends.length >= 10 && achievement.title === "Popular") {
          this.addUserAchievement(achievement, user);
        } else if (userFriends.length >= 42 && achievement.title === "TopUser") {
          this.addUserAchievement(achievement, user);
        }
      }
    }
  }

  async addUserAchievement(achievement: Achievement, user: User) {
    if (!achievement || !user) {
      throw new NotFoundException(`User or Achievement not found`);
    }
    const queryRunner = this.connection.createQueryRunner();

    await queryRunner.connect();
    await queryRunner.startTransaction();
    try {
      user.achievements.push(achievement);
      achievement.users.push(user);
      await queryRunner.manager.save(user);
      await queryRunner.manager.save(achievement);
      await queryRunner.commitTransaction();
    } catch (err) {
      await queryRunner.rollbackTransaction();
    } finally {
      await queryRunner.release();
    }
  }

  async create(createAchievementDto: CreateAchievementDto) {
    const achievement = this.achievementRepository.create(createAchievementDto);
    return this.achievementRepository.save(achievement);
  }

  async remove(id: string) {
    const achievement = await this.achievementRepository.findOne({
      where: { id: +id },
    });
    if (!achievement) {
      throw new NotFoundException(`Achievement #${id} not found`);
    }
    return this.achievementRepository.remove(achievement);
  }
}
