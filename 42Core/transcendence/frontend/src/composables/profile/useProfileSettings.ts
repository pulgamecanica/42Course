import { reactive, toRefs } from "vue";
import server from "@/server";
import type { ApiErrors, Setting, Skin } from "@/composables/types";
import { useProfile } from "@/composables/profile/useProfile";
import { useMe } from "@/composables/user/useMe";
import { parseAxiosError } from "@/composables/axios";
import type { AxiosError } from 'axios';

const state = reactive<{
  currentSetting: Setting | null,
  lastCreatedSetting: Setting | null,
  settings: Setting[],
  skins: Skin[],
  loading: boolean,
  createLoading: boolean,
  errors: ApiErrors | null;
}>({
  currentSetting: null,
  lastCreatedSetting: null,
  settings: [],
  skins: [],
  loading: false,
  createLoading: false,
  errors: null,
});

const { me } = useMe();

const randomColor = () => {
  const letters = '0123456789ABCDEF';
  let color = '#';
  for (var i = 0; i < 6; i++) {
    color += letters[Math.floor(Math.random() * 16)];
  }
  return color;
};

export const useProfileSettings = () => {
  const { achievements, fetchProfile } = useProfile();
  
  const changeCurrentSetting = async (newSetting: Setting) => {
    if (newSetting) {
      state.currentSetting = newSetting;
    }
  }

  const fetchSettings = async () => {
    state.loading = true;
    try {
      const { data: dataSettings } = await server.get('/users/me/settings');
      state.settings = dataSettings;
    } catch (error) {
      state.errors = { settings: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };

  const createSetting = async () => {
    state.loading = true;
    try {
      const { data: dataSettings } = await server.post('/settings', {puck: randomColor(), bg: randomColor(), paddleL: randomColor(), paddleR: randomColor()});
      state.settings.push(dataSettings);
      state.lastCreatedSetting = dataSettings;
    } catch (error) {
      state.errors = { setting: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  }

  const deleteSetting = async (settingId: number) => {
    state.loading = true;
    try {
      await server.delete('/settings/' + settingId);
    } catch (error) {
      state.errors = { setting: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  }

  const updateSetting = async (settingId: number, newSetting: Setting) => {
    state.loading = true;
    try {
      await server.patch('/settings/' + settingId, { ...newSetting });
    } catch (error) {
      state.errors = { setting: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  }

  const fetchSkins = async () => {
    if (me.value) {
      fetchProfile(me.value.username42);
      state.skins = achievements.value;
    }
  }

  return {
    fetchSettings,
    createSetting,
    deleteSetting,
    updateSetting,
    changeCurrentSetting,
    fetchSkins,
    ...toRefs(state)
  };
};
