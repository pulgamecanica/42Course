import { reactive, toRefs } from "vue";
import server from "@/server";
import type { ApiErrors, Setting } from "@/composables/types";
import { parseAxiosError } from "@/composables/axios";
import type { AxiosError } from 'axios';
import type { ProfileStruct as MeStruct } from "@/composables/profile/types"

interface Me {
  id: number;
  intraId: number;
  email: string;
  username42: string;
  firstName: string;
  lastName: string;
  nickname?: string;
  avatarImg: string;
  defaultSetting: Setting;
  defaultSettingId: number;
  isTwoFactorAuthenticationEnabled: boolean;
}

// decide what form this will take when getting several errors in one
// response from, for example a Database instance with several validation errors
// One option is to have an object of error messages, where the key is the field

const state = reactive<{
  me: Me | null;
  first: boolean;
  loading: boolean;
  errors: ApiErrors | null;
}>({
  me: null,
  first: false,
  loading: false,
  errors: null,
});

const firstOn = () => state.first = true
const firstOff = () => state.first = false

const toggleLoading = (loading: boolean) => state.loading = loading;
const setMe = (me: MeStruct) => state.me = { ...state.me, ...(me as Me) }

export const useMe = () => {

  const fetchMe = async () => {
    state.loading = true;
    try {
      const { data } = await server.get<Me>('/users/me');
      state.me = data;
    } catch (error) {
      state.errors = { me: [parseAxiosError(error as AxiosError)] };
    }
    state.loading= false;
  };

  const logout = async () => {
    state.loading = true;
    try {
      const { status } = await server.post('auth/logout');
      console.log("Logged out with status: " + status);
      window.location.href = "/"; // Avoid this...
      /* REMOVE ACCESS_TOKEN, Username42 and cookies */
    } catch (error) {
      state.errors = { me: [parseAxiosError(error as AxiosError)] };
    }
    state.loading= false;
  };

  const updateDefaultSetting = async (settingId: number) => {
    state.loading = true;
    try {
      console.log("Changing Default setting", settingId);
      await server.patch('users/me/defaultSetting/' + settingId)
      if (state.me) {
        state.me.defaultSettingId = settingId;
      }
    } catch (error) {
      state.errors = { setting: [parseAxiosError(error as AxiosError)] };
    }
    state.loading = false;
  };

  return {
    fetchMe,
    logout,
    updateDefaultSetting,
    firstOn,
    firstOff,
    toggleLoading,
    setMe,
    ...toRefs(state)
  };
};
