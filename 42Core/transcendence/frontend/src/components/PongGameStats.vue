<template>
	<hr>
	<el-row
		class="el-card"
		v-if="!loadingGame"
	>
		<hr>
		<el-col><h1>Pong Game #{{ game.id }}</h1></el-col>
		<hr>
		<el-col>
			<h2 v-if="!isGameSocketEmpty">{{ gameState.gameStatus.toUpperCase() }}</h2>
			<h2 v-else>{{ game.status.toUpperCase() }}</h2>
		</el-col>
		<hr>
		<el-col>
			<h3 v-if="!isGameSocketEmpty">Score: {{gameState.scoreLeft}} | {{gameState.scoreRight}}</h3>
			<h3 v-else>Score: {{game.scoreOne}} | {{game.scoreTwo}}</h3>
		</el-col>
		<hr>
		<el-col><h3>At: {{ game.createdAt.replace("T", " ").substr(0, game.createdAt.lastIndexOf(":")) }}</h3></el-col>
		<el-col v-if="winner" class="winner-card">
			<el-avatar
				class="animate__animated animate__headShake winner"
				:src="winner.avatarImg"
				:size="200"
			/>
		</el-col>
		<el-col v-else>
			<h1>No contest</h1>
		</el-col>
	</el-row>
	<!-- src="@/assets/catWaiting.gif" -->

</template>

<script setup lang="ts">
import { computed, ref } from "vue";
import { useGame, useGameSocket } from "@/composables/game/useGame";

const {
  game,
  loading: loadingGame,
} = useGame();
const {
  pongGameState: gameState,
} = useGameSocket();
const isGameSocketEmpty = computed(() => JSON.stringify(gameState.value?.gameStatus) === "{}");
const winner = computed(() => {
	if (isGameSocketEmpty && game.value) {
		if (game.value.scoreOne < game.value.scoreTwo) {
			return game.value.userTwo;
		} else if (game.value.scoreOne > game.value.scoreTwo) {
			return game.value.userOne;
		}
	}
	else if (gameState.value && gameState.value.gameStatus === 'finished') {
		if (gameState.value.scoreLeft != gameState.value.scoreRight) {
			if (gameState.value.scoreLeft < gameState.value.scoreRight) {
				return game.value.userOne;
			} else {
				return game.value.userTwo;
			}
		}
	}
	return undefined;
});

</script>


<style lang="scss">
	.winner-card {
		height: 300px;
		display: flex;
		justify-content: center;
		align-items: center;
		.winner {
			background-color: transparent;
			border: 10px solid;
			border-image-source: url(@/assets/goldenBorder.png);
			border-image-slice: 300;
			border-image-width: 600;
			border-image-outset: 4;
			border-image-repeat: round;
		}
	}
</style>