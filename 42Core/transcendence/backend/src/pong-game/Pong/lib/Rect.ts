import { RectOptions } from "../types"

/**
 * Represents a Rectangle in the Pong game.
 * @public
 */
export class Rect {
  /**
   * Top left corner x coordinate.
   * @member Rect#x
   * @public
   * @type {number}
   * @defaultValue 10
   */
  x: number
  /**
   * Top left corner y coordinate.
   * @member Rect#y
   * @public
   * @type {number}
   * @defaultValue 10
   */
  y: number
  /**
   * Represents how wide/narrow is the rectangle.
   * @member Rect#width
   * @public
   * @type {number}z
   */
  width: number
  /**
   * Represents how tall/short is the rectangle.
   * @member Rect#height
   * @public
   * @type {number}
   */
  height: number
  /**
   * Represents the color of the rectangle.
   * @member Rect#color
   * @public
   * @type {string}
   * @readonly
   */
  readonly color: string

  constructor(options: RectOptions) {
    this.x = options.x || 10
    this.y = options.y || 10
    this.width = options.width
    this.height = options.height
    this.color = options.color || "gray"
  }

  // draw(ctx: CanvasRenderingContext2D) {
  //   ctx.fillStyle = this.color
  //   ctx.fillRect(this.x.value, this.y.value, this.width.value, this.height.value)
  // }
}
