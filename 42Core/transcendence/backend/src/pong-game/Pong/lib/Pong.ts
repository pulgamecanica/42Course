import { Side, Context, Size, Point, Direction } from "../types"
import { Paddle, Rect, Puck } from "./index"
import { GameStatus } from "../../entities/pong-game.entity"
/**
 * Represents a PongGame
 * @public
 */
export class Pong {
  WIDTH = 800
  HEIGHT = 600
  FPS = 64
  SCORETOWIN = 10
  /**
   * Left and Right Score
   * How many points each player has earned.
   * @member Pong#scoreLeft or Pong#scoreRight
   * @public
   * @type {number}
   * @defaultValue 0
   */
  scoreLeft: number
  scoreRight: number
  /**
   * The game Paddles
   * Right and Left Paddle.
   * @member Pong#paddleLeft or Pong#paddleRight
   * @public
   * @type {Paddle}
   */
  paddleLeft: Paddle
  paddleRight: Paddle
  /**
   * The Last pressed Key of each Side
   * @member Pong#directionLeft or Pong#directionRight
   * @public
   * @type {Direction} 
   */
  directionLeft: Direction
  directionRight: Direction
  /**
   * The PongGame Puck
   * @member Pong#puck
   * @public
   * @type {Puck}
   */
  puck: Puck
  /**
   * Keeps trak of the current frame
   * @member Pong#frameCount
   * @public
   * @type {number}
   * @defaultValue 0
   */
  frameCount: number
  /**
   * Saves when the game was created it's constant readonly
   * @member Pong#gameCreatedAt
   * @public
   * @readonly
   * @type {Date}
   * @defaultValue new Date()
   */
  readonly gameCreatedAt: Date = new Date()

  /**
   * The current game status
   * @member Pong#gameStatus
   * @public
   * @readonly
   * @type {GameStatus}
   * @defaultValue GameStatus.Playing
   */
  gameStatus: GameStatus

  constructor() {
    this.paddleLeft = new Paddle({
      x: 10,
      y: 100,
      width: 20,
      height: 100,
    }, Side.Left, true);

    this.paddleRight = new Paddle({
      x: this.WIDTH - 30,
      y: 100,
      width: 20,
      height: 100,
    }, Side.Right, true);

    this.puck = new Puck({
      x: this.WIDTH / 2,
      y: this.HEIGHT / 2,
      width: 20,
      height: 20,
    }, {speed: 5, gravity: 5});

    this.scoreLeft = 0;
    this.scoreRight = 0;
    this.frameCount = 0;
    this.directionLeft = null;
    this.directionRight = null;
    this.gameStatus = GameStatus.PLAYING;
    //this.update();
  }

  input(side: Side, direction: Direction) {
    if (side === Side.Left) {
      this.directionLeft = direction;
    } else if (side === Side.Right) {
      this.directionRight = direction;
    }
  }

  setGameStatus(status: GameStatus) {
    this.gameStatus = status;
  }

  getTimestamp(): number {
    return ((new Date().getTime() - this.gameCreatedAt.getTime()) / 1000);
  }

  update() {
    if (this.gameStatus === GameStatus.PLAYING) {
      if (this.getTimestamp() > (this.frameCount / this.FPS)) {
        this.puck.bounce({w: this.WIDTH, h: this.HEIGHT})
        this.puck.collision({w: this.WIDTH, h: this.HEIGHT}, { paddleLeft: this.paddleLeft, paddleRight: this.paddleRight })
        this.puck.move()
        this.paddleRight.input(this.directionRight, this.puck, {w: this.WIDTH, h: this.HEIGHT})
        this.paddleLeft.input(this.directionLeft, this.puck, {w: this.WIDTH, h: this.HEIGHT})
        let score = this.puck.checkScore({w: this.WIDTH, h: this.HEIGHT})
        if (score === -1) {
          this.scoreLeft += 1
        } else if (score === 1) {
          this.scoreRight += 1
        }
        this.frameCount += 1;
      }
      if (this.scoreRight >= this.SCORETOWIN ||this.scoreLeft >= this.SCORETOWIN) {
        this.setGameStatus(GameStatus.FINISHED);
      }
    } // else if (this.gameStatus === GameStatus.Paused) {
  }
}
