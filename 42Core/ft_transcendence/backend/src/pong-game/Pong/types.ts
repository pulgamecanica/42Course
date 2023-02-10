export enum Side {
  Left = "left",
  Right = "right"
}

export interface Size {
  w: number
  h: number
}

export interface Point {
  x: number
  y: number
}

export enum Direction {
  UP = "up",
  DOWN = "down"
}

export type Context = CanvasRenderingContext2D | null | undefined

export interface RectOptions {
  x?: number
  y?: number
  width: number
  height: number
  color?: string
}

export interface PuckOptions {
  speed?: number
  gravity?: number
}
