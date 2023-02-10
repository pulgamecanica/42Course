import { Rect } from "./Rect"
import { Puck } from "./index"
import { Size, Side, RectOptions, Direction, Point } from "../types"

/**
 * Represents a Paddle in the Pong game.
 * Paddles can bounce Pucks.
 * @public
 */
export class Paddle extends Rect {
  /**
   * Refers to the player, left or right.
   * @member Rect#side
   * @public
   * @type {Side}
   */
  side: Side

  /**
   * The vertical speed of the paddle.
   * @member Rect#speed
   * @public
   * @type {number}
   * @defaultValue 5
   */
  speed: number

  /**
   * Check if the player is human or the computer.
   * @member Rect#isHuman
   * @public
   * @type {number}
   * @defaultValue true
   */
  isHuman: boolean

  constructor(options: RectOptions, side: Side, isHuman: boolean, speed?: number) {
    super(options)
    this.side = side
    this.speed = speed || 5
    this.isHuman = isHuman
  }

  input(direction: Direction, puck: Puck, size: Size) {
    if (this.isHuman && direction) {
      this.movePaddle(direction, size);
    } else if (!this.isHuman){
      this.moveAI(puck, size);
    }
  }

  movePaddle(direction: Direction, size: Size) {
    if (direction === Direction.UP) {
      this.moveUp()
    } else if (direction === Direction.DOWN) {
      this.moveDown(size.h)
    }
  }

  ballTrajectory(puckMidd: Point, puckState: Point, size: Size): Point {
    const nextCord : Point = {x: 0, y: 0};
    nextCord.x = puckMidd.x + (puckState.x * (this.trajectoryLength(puckMidd, puckState, size)))
    nextCord.y = puckMidd.y + (puckState.y * (this.trajectoryLength(puckMidd, puckState, size)))
    return nextCord;
  }

  trajectoryLength(puckMidd: Point, puckState: Point, size: Size): number {
    let lengthY: number
    let lengthX: number
    if (puckState.x > 0) {
      lengthX = (size.w - puckMidd.x) / puckState.x
    } else {
      lengthX = puckMidd.x / -puckState.x
    }
    if (puckState.y > 0) {
      lengthY = (size.h - puckMidd.y) / puckState.y
    } else {
      lengthY = puckMidd.y / -puckState.y
    }
    if (lengthX < lengthY) {
      return lengthX;
    } else {
      return lengthY;
    }
  }

  puckLanding(puck: Puck, size: Size): number {
    /*
    ----------------------------
    [            |             ]  * How to calculate
    [ | .        |             ]    where is it going to land?
    [ |   .      |             ]
    [ |     .    |           | ]  |\       * The trajectory is the hipotenusse                       /|
    [         o  |           | ]  | \        The long side size is unknown                          / |
    [           .|          .| ]  |  \       The short side is:                                    /  |
    [            |.       .    ]  |   \      - Right pov; the x coordinate of the puck - width    /   |
    [            |  .   .      ]  |____\     - Left pov; the x coordinate of the puck            /____|
    [            |    .        ]
    ----------------------------           * The landing is going to be the current y position - the long side
    */
    const finalPos: Point = this.ballTrajectory({x: puck.x + (puck.width / 2), y: puck.y + (puck.height / 2)}, {x: puck.speed, y: puck.gravity}, size)
    if (finalPos.x > finalPos.y) {
      return this.ballTrajectory({x: finalPos.x, y: finalPos.y}, {x: puck.speed, y: -puck.gravity}, size).y
    } else {
      return finalPos.y;
    }
  }

  moveAI(puck: Puck, size: Size) {
    if (this.side == Side.Left && puck.speed > 0 ||
      this.side == Side.Right && puck.speed < 0) {
      if ((this.y + this.height / 2) > size.h / 2 - 5 && (this.y + this.height / 2) > size.h / 2 + 5) {
        this.moveUp()
      } else if ((this.y + this.height / 2) < size.h / 2) {
        this.moveDown(size.h)
      }
    } else {
      if (this.puckLanding(puck, size) > this.y + (this.height / 2)){
        this.moveDown(size.h)
      } else {
        this.moveUp()
      }
    }
  }

  moveUp() {
    if (this.y - this.speed > 0) {
      this.y -= this.speed
    }
  }

  moveDown(h: Size["h"]) {
    if (this.y + this.height + this.speed < h) {
      this.y += this.speed
    }
  }
}
