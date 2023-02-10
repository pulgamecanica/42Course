export interface ApiErrors {
  [field: string]: string[];
}

export interface Setting {
	id: number,
	userId: number,
    puck: string,
    paddleR: string,
    paddleL: string,
    bg: string,
    name: string,
}

export interface Skin {
	id: number,
	title: string,
	description: string,
	imgUrl: string,
}