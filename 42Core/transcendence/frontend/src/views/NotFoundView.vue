<template>
  <component v-bind:is="components[Math.floor(Math.random() * components.length)]" />
</template>

<script setup lang="ts">
  import Portal from "@/components/NotFound/Portal.vue";
  import Whaaat from "@/components/NotFound/Whaaat.vue";
  import PortalTop from "@/components/NotFound/PortalTop.vue";
  import Falling from "@/components/NotFound/Falling.vue";
  import PortalWalk from "@/components/NotFound/PortalWalk.vue";
  import Surprised from "@/components/NotFound/Surprised.vue";

  const components = [Portal, Whaaat, PortalTop, Falling, PortalWalk, Surprised];
</script>
