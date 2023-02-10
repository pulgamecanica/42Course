import type { Ref } from "vue";

export type Context = CanvasRenderingContext2D | null | undefined

export enum Side {
	Left = "left",
	Right = "right"
}

export enum GameStatus {
	STARTING = 'starting',
	PLAYING = 'playing',
	FINISHED = 'finished',
	SUSPENDED = 'suspended',
}

export interface Rect {
	x: number,
	y: number,
	width: number,
	height: number,
	color: string,
	speed: number,
	gravity: number,
	skin: string,
}

export interface Player {
  side: Side,
  clientId: string,
  username42: string,
  ready: boolean
}

export interface PongGameState {
	playerRightConnection: Ref<boolean>,
	playerLeftConnection: Ref<boolean>,
	puck: Ref<Rect>,
	paddleLeft: Ref<Rect>,
	paddleRight: Ref<Rect>,
	scoreLeft: Ref<number>,
	scoreRight: Ref<number>,
	frameCount: Ref<number>,
	gameStatus: Ref<GameStatus>,
	spectators: Ref<number>,
	playerRight: Player,
	playerLeft: Player,
	background: Ref<string>,
}

export interface Setting {
	puck?: ref<string>,
	paddleR?: ref<string>,
	paddleL?: ref<string>,
	bg?: ref<string>,
	name?: ref<string>,
	id?: ref<number>
}