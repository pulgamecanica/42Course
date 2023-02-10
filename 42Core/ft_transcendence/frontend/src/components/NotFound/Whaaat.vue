<template>
  <div class="bottom">
    <h1>404</h1>
    <p>Morty! Where's everything!</p>
    <img src="@/assets/whaaat.png" />
  </div>
</template>

<style scoped>
  .bottom {
    bottom: 0;
    position: absolute;
    left: 50%;
    transform: translateX(-50%);
    width: 100%;
    max-height: 100%;
  }

  img {
    width: 36%;
    filter: drop-shadow(0px 0px 5px #fff);
  }
</style>