<template>
  <div v-loading="loading" class="friends-panel__item friends-panel__players">
    <div class="c-card">
      <div class="c-card__body">
        <ul class="c-list" v-if="friends.length">
          <li
            v-for="friend in friends"
            :key="friend.username42"
            class="c-list__item"
          >
            <div class="c-list__friends">
              <div class="c-media" :style="'opacity: ' +  (friend.status === 'offline' ? '0.5' : '1')">
                <el-avatar :size="60" :src="friend.avatarImg" />
                <div class="c-media__content">
                  <RouterLink
                    class="c-media__title u-text--light"
                    :to="{ name: 'profile', params: { username42: friend.username42 }}"
                  >
                    @{{ friend.username42 }}
                  </RouterLink>
                  <div class="c-media__status" style="color: var(--light)">
                    <em>{{ friend.status }}</em>
                  </div>
                </div>
              </div>
              <div class="u-text--right c-kudos">
                <div class="u-mt--8">
                  <el-dropdown>
                    <span class="el-dropdown-link">
                      <el-icon class="el-icon--right">
                        <arrow-down />
                      </el-icon>
                    </span>
                    <template #dropdown>
                      <el-dropdown-menu>
                        <el-dropdown-item @click="friend.username42 && inviteChatGame(friend.username42)">Invite</el-dropdown-item>
                        <el-dropdown-item @click="friend.username42 && directMessageUser(friend.username42)">Chat</el-dropdown-item>
                      </el-dropdown-menu>
                    </template>
                  </el-dropdown>
                </div>
              </div>
            </div>
          </li>
        </ul>
        <el-empty v-else description="You have no friends yet! Search for some in the profile page!" :image="randomRickAndMortyImg()" />
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { watch, computed } from 'vue';
import { useChat } from "@/composables/chat/useChat";
import { useProfile } from "@/composables/profile/useProfile";
import { useMe } from "@/composables/user/useMe";
import { randomRickAndMortyImg } from "@/composables/images.ts";

const {
  directMessageUser,
  inviteChatGame,
} = useChat()

const { me, loading } = useMe();
const username42 = computed(() => me.value?.username42);
const {
  friends,
  fetchProfile,
} = useProfile();

if (username42.value) {
  fetchProfile(username42.value);
}

watch(loading, () => {
  if (username42.value) {
    fetchProfile(username42.value);
  }
});
</script>

<style scoped>

</style>