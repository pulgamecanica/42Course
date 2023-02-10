<template>
<svg
  x="0px"
  y="0px"
  viewBox="0 -200 960 960"
  enable-background="new 0 -200 960 960"
  xml:space="preserve"
  v-if="!isOpen"
>
  <polygon
    points="32,412.6 362.1,412.6 362.1,578 526.8,578 526.8,279.1 197.3,279.1 526.8,-51.1 362.1,-51.1 32,279.1 "
  />
  <polygon
    points="597.9,114.2 762.7,-51.1 597.9,-51.1 "
  />
  <polygon
    points="762.7,114.2 597.9,279.1 597.9,443.9 762.7,443.9 762.7,279.1 928,114.2 928,-51.1 762.7,-51.1 "
  />
  <polygon
    points="928,279.1 762.7,443.9 928,443.9 "
  />
</svg>
<svg
  viewBox="0 0 24 24"
  fill="none"
  v-else
>
  <path
    d="M16.95 8.464a1 1 0 0 0-1.414-1.414L12 10.586 8.464 7.05A1 1 0 1 0 7.05 8.464L10.586 12 7.05 15.536a1 1 0 1 0 1.414 1.414L12 13.414l3.536 3.536a1 1 0 0 0 1.414-1.414L13.414 12l3.536-3.536Z"
    fill="black"
  />
</svg>
</template>

<script setup lang="ts">
defineProps<{
  isOpen?: boolean;
}>();
</script>