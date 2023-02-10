<template>
  <div
    v-loading="imLoading"
  >
    <el-card
      v-if="!imLoading"   
    >
      <h1>{{ username42 }} Settings</h1>
      <hr>
      <h4>Total Settings {{ totalSettings }}</h4>
      <el-row
        :gutter="12"
      >
        <el-col
          style="margin-bottom: 1rem;"
          :sm="12"
          :md="6"
          v-for="setting in settings"
        >
          <SettingCard
            :setting="setting"
            :makeDefault="makeDefault"
            :isDefaultSetting="setting.id === __defaultSetting"
            :deleteSetting="deleteSetting"
          />
        </el-col>
        <el-col
          class="centerPlus"
          :sm="12"
          :md="6"
        >
          <el-card
            class="el-button"
            style="min-height: 100px; min-width: 100px;"
            @click="createSetting"
          >
            <h1>+</h1>
          </el-card>
        </el-col>
      </el-row>
    </el-card>
  </div>
</template>

<script setup lang="ts">
import SettingCard from '@/components/SettingCard.vue';
import { computed, ref, onBeforeMount, watch } from "vue";
import { useMe } from "@/composables/user/useMe";
import type { Setting } from '@/components/pongTypes'
import { useProfileSettings } from "@/composables/profile/useProfileSettings";

const {
  loading,
  settings,
  fetchSettings,
  createSetting,
  updateSetting,
  deleteSetting: serverDeleteSetting,
} = useProfileSettings();
fetchSettings();

const {
  me,
  fetchMe,
  loading: imLoading,
  updateDefaultSetting
} = useMe();
const username42 = computed(() => me.value.username42);
const defaultSetting = computed(() => me.value.defaultSettingId);
const __defaultSetting = ref<number>(defaultSetting);
const makeDefault = (settingId) => {
  __defaultSetting.value = settingId;
  updateDefaultSetting(settingId);
};
const deleteSetting = (itemToRemove) => {
  if (itemToRemove.id === defaultSetting.value) {
    alert("You can't remove the default setting");
  } else {
    settings.value.forEach((item, index) => {
      if(item === itemToRemove) settings.value.splice(index,1);
    });
    serverDeleteSetting(itemToRemove.id);
  }
};
const totalSettings = computed(() => settings.value.length);
</script>

<style lang="scss" scoped="">
.centerPlus {
  margin: auto 0;
}
</style>

