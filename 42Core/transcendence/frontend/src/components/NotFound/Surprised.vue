<template>
  <h1 class="big">404</h1>
  <img src="@/assets/surprised.png" />
</template>

<style scoped>
  .big {
    font-size: 10rem;
    z-index: 10;
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
  }

  img {
    height: 100vh;
    bottom: 0;
    position: absolute;
    left: 50%;
    transform: translateX(-50%) translateZ(0);
  }
</style>
