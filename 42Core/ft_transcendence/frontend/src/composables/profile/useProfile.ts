import { reactive, toRefs } from "vue";
import server from "@/server";
import type { ProfileStruct, User } from "@/composables/profile/types"
import type { ApiErrors } from "@/composables/types";
import { useMe } from "@/composables/user/useMe";
import { parseAxiosError, ErrorNotification } from "@/composables/axios";
import type { AxiosError } from 'axios';
import { useRouter } from 'vue-router';

interface Achievement {
  id: number;
  description: string;
  imgUrl: string;
  title: string;
}

export enum RelationOptions {
  SendRelation = "sendRelation",
  AcceptRelation = "acceptRelation",
  DeclineRelation = "declineRelation",
  Sent = "sent",
  Unfriend = "unfriend",
  Block = "block"
}

type ProfileState = {
  user: User,
  friends: User[],
  friendRequestsSent: User[],
  friendRequestsRecieved: User[],
  blockedUsers: User[],
  achievements: Achievement[],
  relationOptions: RelationOptions[],
  matches: [],
  loading: boolean,
  errors: ApiErrors | null;
}

const state = reactive<ProfileState>({
  user: {},
  friends: [],
  friendRequestsSent: [],
  friendRequestsRecieved: [],
  blockedUsers: [],
  achievements: [],
  relationOptions: [],
  matches: [],
  loading: false,
  errors: null,
});

const { me, toggleLoading: toggleMeLoading, setMe } = useMe();

const clearErrors = () => state.errors = null;


export const useProfile = () => {
  const router = useRouter()

  const fetchProfile = async (username42: string) => {
    if (!username42) {
      return ;
    }

    state.loading = true;
    try {
      const { data: dataProfile } = await server.get(`/users/${username42}`);
      const { data: dataMatches } = await server.get(`/users/${username42}/games`);

      if (me.value && me.value.username42 === username42) {
        const { data: dataRequestsSent } = await server.get('/users/me/friend-requests-sent');
        const { data: dataRequestsReceived } = await server.get('/users/me/friend-requests-received');
        const { data: dataBlockedUsers } = await server.get('/users/me/blocked-users');

        state.friendRequestsSent = dataRequestsSent;
        state.friendRequestsRecieved = dataRequestsReceived;
        state.blockedUsers = dataBlockedUsers;
      }

      state.user = dataProfile;
      state.matches = dataMatches;

      state.friends = dataProfile.friends;
      state.achievements = dataProfile.achievements;
      state.relationOptions = dataProfile.relationOptions;
    } catch (error) {
      if (error?.response?.status === 404) {
        router.push({name: "not-found"})
      }
      parseAxiosError(error as AxiosError);
    }
    state.loading = false;
  };

  const updateProfile = async (params: ProfileStruct) => {
    toggleMeLoading(true)
    state.loading = true;
    try {
      const { data } = await server.patch<User>('/users/me/', params);
      state.user = { ...state.user, ...data };
      setMe(params)
    } catch (error) {
      const message = parseAxiosError(error as AxiosError, false)
      const errors: ApiErrors = {}
      if (message.includes('nickname')) {
        errors.nickname = ['Nickname already exists']
      } else {
        ErrorNotification(message, 'Validation Error')
      }
      state.errors = errors;
    }
    toggleMeLoading(false)
    state.loading = false;
  };

  return {
    fetchProfile,
    updateProfile,
    clearErrors,
    ...toRefs(state)
  };
};
