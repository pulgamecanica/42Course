import { Injectable, NotFoundException, ConflictException, BadRequestException, UnprocessableEntityException } from '@nestjs/common';
import { InjectRepository } from '@nestjs/typeorm';
import { Repository } from 'typeorm';
import { CreatePongGameDto } from './dto/create-pong-game.dto';
import { UpdatePongGameDto } from './dto/update-pong-game.dto';
import { PongGame } from './entities/pong-game.entity';
import { UserService } from 'src/users/user/user.service';
import { GameStatus } from './entities/pong-game.entity';
import { Socket, Server } from 'socket.io';
import { Pong } from './Pong/lib/index';
import { Side, Direction } from './Pong/types';
import type { Stats, SimpleStats } from 'src/users/user/user.service';
import { User } from 'src/users/user/entities/user.entity';

type PlayerStats = User & (Stats | SimpleStats);

interface Player {
  side: Side;
  clientId: string;
  username42: string;
  ready: boolean;
}

interface PongGameState {
  pong: Pong;
  status: GameStatus;
  spectators: number;
  playerRight: Player;
  playerLeft: Player;
}

interface QueuePlayer {
  client: Socket;
  username42: string;
  userId: number;
}

@Injectable()
export class PongGameService {
  private games: Map<string, PongGameState> = new Map<string, PongGameState>();
  private pongQueue: Array<QueuePlayer> = new Array<QueuePlayer>();

  constructor(
    @InjectRepository(PongGame)
    private readonly pongGameRepository: Repository<PongGame>,
    private readonly userService: UserService,
  ) {}

  async initializeGames(server: Server) {
    const interval = setInterval(async () => {
      this.games.forEach((state, gameId) => {
        if (
          state &&
          (state.pong.gameStatus === GameStatus.FINISHED ||
            state.pong.gameStatus === GameStatus.SUSPENDED)
        ) {
          this.update(gameId, {
            scoreOne: state.pong.scoreLeft,
            scoreTwo: state.pong.scoreRight,
            status: state.pong.gameStatus,
          });
          if (state.pong.gameStatus === GameStatus.FINISHED) {
            state.status = GameStatus.FINISHED;
          }
        } else if (
          state &&
          (state.status === GameStatus.STARTING ||
            state.status === GameStatus.PLAYING)
        ) {
          state.pong.setGameStatus(state.status);
          state.pong.update();
        }
        server.to('pongGame' + gameId).emit('getGameState', { state: state });
      });
    }, (1.0 / 32) * 1000);
  }

  /*******************************************/
  /***            Standard Logic           ***/
  /*******************************************/

  findAll() {
    return this.pongGameRepository.find({
      relations: ['userOne', 'userTwo'],
    });
  }

  async findOne(id: string) {
    if (!+id) {
      throw new BadRequestException(`#${id} is invalid`);
    }
    const game = await this.pongGameRepository.findOne({
      where: {
        id: +id,
      },
      relations: ['userOne', 'userTwo'],
    });
    if (!game) {
      throw new NotFoundException(`Game #${id} not found`);
    }
    return game;
  }

  async create(createPongGameDto: CreatePongGameDto) {
    const game = this.pongGameRepository.create(createPongGameDto);
    return this.pongGameRepository.save(game);
  }

  async update(id: string, updatePongGameDto: UpdatePongGameDto) {
    if (!+id) {
      throw new BadRequestException(`#${id} is invalid`);
    }
    const game = await this.pongGameRepository.preload({
      id: +id,
      ...updatePongGameDto,
    });
    if (!game) {
      throw new NotFoundException(`Game #${id} not found`);
    }
    return this.pongGameRepository.save(game).catch((err: any) => {
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
    if (!+id) {
      throw new BadRequestException(`#${id} is invalid`);
    }
    const game = await this.pongGameRepository.findOne({ where: { id: +id } });
    if (!game) {
      throw new NotFoundException(`Game #${id} not found`);
    }
    return this.pongGameRepository.remove(game);
  }

  /*******************************************/
  /***            Standard Logic           ***/
  /*******************************************/

  async getPongGameById(id: string) {
    try {
      const game = await this.findOne(id);
      if (!game) {
        return null;
      }
      if (game.status === GameStatus.FINISHED) {
        return null;
      } else if (!this.games.has(id)) {
        this.games.set(id, {
          pong: new Pong(),
          status: GameStatus.STARTING,
          spectators: 0,
          playerRight: null,
          playerLeft: null,
        });
      }
      return this.games.get(id);
    } catch (error) {
      console.error(error);
    }
  }

  async readyToStartGame(client: Socket) {
    try {
      const game = await this.getPongGameById(
        client.handshake.query.gameId + '',
      );
      if (!game || game.status == GameStatus.PLAYING) return;
      if (game.pong.paddleRight.isHuman && game.pong.paddleLeft.isHuman) {
        if (
          !game.playerRight ||
          !game.playerLeft ||
          (game.playerRight.clientId != client.id &&
            game.playerLeft.clientId != client.id)
        ) {
          console.log('Waiting for all players to connect...');
          return;
        }
      } else if (
        game.pong.paddleRight.isHuman &&
        !game.pong.paddleLeft.isHuman
      ) {
        if (!game.playerRight || game.playerRight.clientId != client.id) {
          console.log('Waiting for all players to connect...');
          return;
        }
      } else if (
        game.pong.paddleLeft.isHuman &&
        !game.pong.paddleRight.isHuman
      ) {
        if (!game.playerLeft || game.playerLeft.clientId != client.id) {
          console.log('Waiting for all players to connect...');
          return;
        }
      }
      if (game.playerLeft.clientId === client.id) {
        game.playerLeft.ready = !game.playerLeft.ready;
      } else if (game.playerRight.clientId === client.id) {
        game.playerRight.ready = !game.playerRight.ready;
      }
      if (game.playerLeft.ready && game.playerRight.ready) {
        game.status = GameStatus.PLAYING;
        this.update(client.handshake.query.gameId + '', {
          status: game.status,
        });
      }
    } catch (error) {
      console.error(error);
    }
  }

  async addExpectator(client: Socket, username42: string) {
    try {
      if (client.handshake.query.gameId === undefined) {
        return;
      }
      console.log(
        'Add Expectator...',
        username42,
        'to game',
        client.handshake.query.gameId,
      );
      const game = await this.getPongGameById(
        client.handshake.query.gameId + '',
      );
      const pongGame = await this.findOne(client.handshake.query.gameId + '');
      const user = await this.userService.findUserByUserName42(username42);
      if (!game || !pongGame || !user) {
        return;
      }
      if (pongGame.userOneId === user.id && !game.playerLeft) {
        game.playerLeft = {
          side: Side.Left,
          clientId: client.id,
          username42: username42,
          ready: false,
        };
      } else if (pongGame.userTwoId === user.id && !game.playerRight) {
        game.playerRight = {
          side: Side.Right,
          clientId: client.id,
          username42: username42,
          ready: false,
        };
      } else {
        game.spectators += 1;
      }
    } catch (e) {
      console.log(e);
    }
  }

  async removeExpectator(client: Socket) {
    try {
      console.log('Removing Client: ' + client.id);
      const game = await this.getPongGameById(
        client.handshake.query.gameId + '',
      );
      if (!game) return;
      if (game.playerLeft && game.playerLeft.clientId == client.id) {
        game.playerLeft = null;
        game.status = GameStatus.STARTING;
      } else if (game.playerRight && game.playerRight.clientId == client.id) {
        game.playerRight = null;
        game.status = GameStatus.STARTING;
      } else {
        if (game.spectators > 0) {
          game.spectators -= 1;
        }
      }
    } catch (error) {
      console.error(error);
    }
  }

  getUsername42Side(game: PongGameState, username42: string): Side {
    if (game) {
      if (game.playerLeft && game.playerLeft.username42 == username42) {
        return Side.Left;
      } else if (
        game.playerRight &&
        game.playerRight.username42 == username42
      ) {
        return Side.Right;
      }
    }
  }

  async keyPressed(
    client: Socket,
    message: { username42: string; keyPressed: string },
  ) {
    const game = this.games.get(client.handshake.query.gameId + '');
    const side = this.getUsername42Side(game, message.username42);
    if (!game || !side) {
      return;
    }
    if (message.keyPressed === 'ArrowUp') {
      game.pong.input(side, Direction.UP);
    } else if (message.keyPressed === 'ArrowDown') {
      game.pong.input(side, Direction.DOWN);
    } else if (message.keyPressed === 'KeyP') {
      game.status = GameStatus.STARTING;
      if (game.playerRight) {
        game.playerRight.ready = false;
      }
      if (game.playerLeft) {
        game.playerLeft.ready = false;
      }
    }
  }

  async keyReleased(
    client: Socket,
    message: { username42: string; keyReleased: string },
  ) {
    const game = this.games.get(client.handshake.query.gameId + '');
    const side = this.getUsername42Side(game, message.username42);
    if (!game || !side) {
      return;
    }
    game.pong.input(side, null);
  }

  async abandonGame(client: Socket, message: { username42: string }) {
    const game = this.games.get(client.handshake.query.gameId + '');
    if (
      (game.playerLeft && game.playerLeft.username42 === message.username42) ||
      (game.playerRight && game.playerRight.username42 === message.username42)
    ) {
      game.status = GameStatus.SUSPENDED;
      game.pong.setGameStatus(GameStatus.FINISHED);
      this.update(client.handshake.query.gameId + '', {
        scoreOne: game.pong.scoreLeft,
        scoreTwo: game.pong.scoreRight,
        status: game.status,
      });
    }
  }
  /*******************************************/
  /***        Standard Logic Queue         ***/
  /*******************************************/

  async joinQueue(client: Socket, username42: string) {
    try {
      const user = await this.userService.findUserByUserName42(username42);
      if (user) {
        if (
          client &&
          !this.pongQueue.find((element) => element.client.id === client.id)
        ) {
          this.pongQueue.push({
            client: client,
            username42: username42,
            userId: user.id,
          });
        }
      }
    } catch (error) {
      console.error(error);
    }
  }

  matchPlayers() {
    while (this.pongQueue.length > 1) {
      const player1 = this.pongQueue[0];
      const playe2 = this.pongQueue[1];
      this.createMatch(player1, playe2);
      this.abandonQueue(player1.client);
      this.abandonQueue(playe2.client);
    }
  }

  abandonQueue(client: Socket) {
    if (client && client.id) {
      const playerIndex = this.pongQueue.findIndex(
        (element) => element.client.id === client.id,
      );
      const playerRemoved = this.pongQueue.splice(playerIndex, 1);
      // SEND REMOVED FROM QUEUE SUCCESSFULLY
    }
  }

  async createMatch(
    player1: QueuePlayer,
    player2: QueuePlayer,
  ): Promise<number> {
    if (!player1 || !player2) return;

    const game = await this.create({
      status: GameStatus.STARTING,
      userOneId: player1.userId,
      userTwoId: player2.userId,
      scoreOne: 0,
      scoreTwo: 0,
    });

    player1.client.emit('gameCreated', game.id);
    player2.client.emit('gameCreated', game.id);

    return game.id;
  }

  async playersRanking() {
    const players = await Promise.all(
      (
        await this.userService.findAll()
      ).map(async (player) => {
        const pongStats = await this.userService.findPongStats(
          +player.id,
          false,
        );

        return {
          username42: player.username42,
          firstName: player.firstName,
          lastName: player.lastName,
          avatarImg: player.avatarImg,
          ...pongStats,
        } as PlayerStats;
      }),
    );

    return players
      .filter((player) => player.ranking)
      .sort((p1, p2) => {
        if (p1.ranking === p2.ranking) {
          return p1.firstName < p2.firstName ? -1 : 1;
        }
        return p2.ranking - p1.ranking;
      });
  }

  /*
  removeExpectator(clientId: string) {
    console.log("Removing Client: " + clientId);
    this.spectators.delete(clientId);
  }

  addExpectator(clientId: string, username42: string) {
    console.log("Add Expectator...", username42, this.spectators);
    this.spectators.set(clientId, username42);
  }

  getAllExpectators(): Map<string, string> {
    console.log("Emit To channel...", this.spectators);
     return (this.spectators);
  }
*/
}
