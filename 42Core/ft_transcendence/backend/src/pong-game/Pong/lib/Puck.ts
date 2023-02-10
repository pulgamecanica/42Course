import { Size, Side, PuckOptions, RectOptions } from "../types"
import { Paddle, Rect } from "./index"

/**
 * Represents a Ball/Puck in the Pong game.
 * Pucks are bounced by paddles.
 * @public
 */
export class Puck extends Rect {
  /**
   * The horizontal velocity of the Puck
   * I.e. how fast it goes side to side.
   * @member Rect#speed
   * @public
   * @type {number}
   * @defaultValue 5
   */
  speed: number
  /**
   * The vertical velocity of the Puck.
   * I.e. how fast it goes up/down.
   * @member Rect#gravity
   * @public
   * @type {number}
   * @defaultValue 5
   */
  gravity: number

  constructor(rectOptions: RectOptions, puckOptions: PuckOptions) {
    super(rectOptions)
    this.speed = puckOptions.speed || 5
    this.gravity = puckOptions.gravity || 5
  }
  /*
  * Check if the ball will hit the top or bottom of the canvas
  */
  bounce(size: Size) {
    if (
      this.y + this.gravity <= 0 ||
      this.y + this.gravity + this.height >= size.h
    ) {
      this.gravity = -this.gravity
    }
  }

  /*
  * Check if the ball will hit the left or right sides of the canvas
  * and check if the ball is missed by a paddle
  */
  collision(
    size: Size,
    { paddleLeft, paddleRight }: { paddleLeft: Paddle, paddleRight: Paddle }
  ) {
    if (this.speed < 0) {
      if (
        (this.x + this.speed <= paddleLeft.x + paddleLeft.width) &&
        (this.y + this.gravity > paddleLeft.y) &&
        (this.y + this.gravity <= paddleLeft.y + paddleLeft.height)
      ) {
        this.switch(this.angleDefect(paddleLeft))
      }
    } else {
      if (
        (this.x + this.width + this.speed >= paddleRight.x) &&
        (this.y + this.gravity > paddleRight.y) &&
        (this.y + this.gravity <= paddleRight.y + paddleRight.height)
      ) {
        this.switch(this.angleDefect(paddleRight))
      }
    }
  }

  move() {
    this.x += this.speed
    this.y += this.gravity
  }

  angleDefect(paddle: Paddle): number {
    const midBall = this.y + (this.height / 2)
    const midPaddle = paddle.y + (paddle.height / 2)
    return (((midBall - midPaddle) * 5) / paddle.height)
  }

  switch(defect: number) {
    this.gravity += defect;
    this.speed = -this.speed;
    if (this.speed < 0) {
      this.speed -= 0.25;
    } else {
      this.speed += 0.25;
    }
  }

  checkScore(size: Size): number {
    if (this.x > size.w) {
      this.reset(Side.Right, size)
      return -1
    } else if (this.x < 0) {
      this.reset(Side.Left, size)
      return 1
    }
    return 0
  }

  reset(side: Side, size: Size) {
    this.switch(0)
    this.x = size.w / 2
    this.y += this.gravity
  }
}

