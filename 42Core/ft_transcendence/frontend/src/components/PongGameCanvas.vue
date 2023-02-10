<template>
	<el-row>
		<el-col
			class="game--wrapper"
			:span="24"
		>
			<img
				id="tuxLoading"
				v-show="tuxLoading"
				src="@/assets/tux.gif"
			/>
			<canvas
				id="pongCanvas"
				ref="pongGameCanvas"
				:style="{ background: currentBG }"
				:width="800"
				:height="600"
			>
        Your browser doesn't support Canvas, try another or update
      </canvas>
    </el-col>
  </el-row>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, onBeforeMount, watch } from "vue";
import type { Context, PongGameState, Rect } from '@/components/pongTypes'
import { GameStatus, Side } from '@/components/pongTypes'
import { useEventListener } from "@/composables/event"
import { useGameSocket } from "@/composables/game/useGame";

const isColor = (str) => {
	return /^#[0-9A-F]{6}$/i.test(str)
}
const {
  sendKeyPressed,
  sendkeyReleased,
  pongGameState: gameState,
} = useGameSocket();
const ctx = ref<Context>(null)
const pongGameCanvas = ref<HTMLElement | null>(null)
const tuxLoading = computed(() => gameState.value.gameStatus != GameStatus.PLAYING);
const currentBG = computed(() => {
	if (isColor(gameState.value?.background)) {
		return gameState.value?.background
	} else {
		return `url(${gameState.value?.background})`
	}
});

let img: Image;
const drawRect = (rect: Rect) => {
	img = new Image();
	if (!ctx.value) {
		return
	}
	ctx.value.fillStyle = rect.color
	if (rect.skin) {
		if (isColor(rect.skin)) {
			ctx.value.fillStyle = rect.skin;
			ctx.value.fillRect(rect.x, rect.y, rect.width, rect.height)
		} else {
			img.src = rect.skin; 
	  		ctx.value.drawImage(img, rect.x, rect.y, rect.width, rect.height);
  		}
  	} else {
		ctx.value.fillRect(rect.x, rect.y, rect.width, rect.height)
  	}
}
const drawScore = (score: string, side: Side) => {
	if (!ctx.value) {
		return
	}
	const sideMultiplier = side === Side.Left ? -1 : 1
	const fontSize = 800 / 15
	ctx.value.font = `${fontSize}px Arial`
	ctx.value.fillStyle = "#FFFFFF"
	ctx.value.fillText(score, (800 / 2) + (sideMultiplier * fontSize) - fontSize / 4, fontSize)
}
const drawGame = () => {
	if (!ctx.value) {
		return
	}
	drawScore(gameState.value.scoreLeft.toString(), Side.Left)
	drawScore(gameState.value.scoreRight.toString(), Side.Right)
	for(let i = 10; i < 600; i+=24){
		drawRect({
      x: (800 / 2 - 2),
      y: i,
      width: 2,
      height: 8,
      color: "white",
		})
	}
	drawRect(gameState.value.puck)
	drawRect(gameState.value.paddleRight)
	drawRect(gameState.value.paddleLeft)
}
const drawLoadingGame = () => {
	if (!ctx.value) {
		return
	}
	progress = (progress + 0.1) % (2 * Math.PI)
	ctx.value.beginPath()
	ctx.value.arc(800 / 2, 600 / 2, 50, progress - Math.PI / 2, progress)
	ctx.value.lineWidth = 5;
	ctx.value.strokeStyle = "#fff"
	ctx.value.stroke();
}
	let progress = 0;

onMounted(() => {
	ctx.value = pongGameCanvas.value?.getContext("2d");
	if (ctx.value) {
		ctx.value.clearRect(0, 0, 800, 600)
		drawGame();
		if (gameState.value.gameStatus != GameStatus.PLAYING) {
			drawLoadingGame();
		}
	}
});


useEventListener(window, "keydown", event => {
  event.preventDefault();
  sendKeyPressed(event);
});

useEventListener(window, "keyup", event => {
  sendkeyReleased(event);
});

watch(gameState, (newGameState) => {
	if (ctx.value) {
		ctx.value.clearRect(0, 0, 800, 600)
		drawGame();
		if (tuxLoading.value) {
			drawLoadingGame();
		}
	}
});

</script>

<style lang="scss">
#tuxLoading {
	position: absolute;
	z-index: 9;
	width: 70px;
	height: auto;
}

.normal-screen #pongCanvas {
	background-color: #424242;
	background-repeat: no-repeat !important;
	background-size: cover !important;
	background-position: center;
	max-width: 900px;
	max-height: 700px;
	width: 100vw;
}

.fullScreen #pongCanvas {
	position: fixed;
	top: 0;
	left: 0;
	z-index: 3000;
	max-width: 100vw;
	max-height: 100vh;
	width: 100vw;
	height: 100vh;
}
.full-screen {
	.game {
		&--wrapper {
			height: 100vh;
			width: 100vw;
			align-items: center;
			justify-content: center;
			display: flex;
			canvas {
				width: 100vw;
				max-height: 100vh; 
			}
		}
	}
}
.normal-screen {
	.game {
		min-height: 100%;
		display: block;
		margin-bottom: 5rem;
		&--wrapper {
			align-items: center;
			justify-content: center;
			display: flex;
			canvas {
				max-height: 80vh;
			}
		}
	}
}
</style>