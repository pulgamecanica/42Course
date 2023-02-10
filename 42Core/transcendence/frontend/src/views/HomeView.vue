<template>
  <el-row>
    <el-col :span="17">
      <el-row v-loading="loading">
        <leaderboard />
      </el-row>
      <el-row>
        <el-col v-loading="loading">
          <chat-panel />
        </el-col>
      </el-row>
    </el-col>
    <el-col :span="7" v-loading="loading">
      <friends-panel />
    </el-col>
  </el-row>
</template>

<script setup lang="ts">
import Leaderboard from "@/components/Leaderboard.vue";
import FriendsPanel from "@/components/FriendsPanel.vue";
import ChatPanel from "@/components/ChatPanel/index.vue";
import { useMe } from "@/composables/user/useMe";

const { loading } = useMe();
</script>