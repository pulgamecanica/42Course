<template>
  <div class="center">
    <h1>404</h1>
    <img src="@/assets/portal.gif" />
    <p>Portal to a box. Great idea, Rick!</p>
  </div>
</template>

<style scoped>
  .center {
    position: absolute;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    width: 100%;
    max-height: 100%;
  }

  img {
    width: 25%;
    filter: drop-shadow(0px 0px 10px #444);
  }
</style>
