<template>
  <el-badge :value="profileStatus" :type="profileStatus === 'offline' ? 'danger' : 'success'">
    <el-avatar
      v-loading="uploading"
      :src="profile.avatarImg"
      class="animate__animated animate__headShake margin-bottom avatar"
    />
  </el-badge>
  <el-container class="margin-bottom justify-center">
    <template v-if="isMe">
      <el-upload
        name="image"
        :action="`${IMAGE_URL}?key=${IMAGE_KEY}`"
        :show-file-list="false"
        :on-success="onUploadSuccess"
        :on-error="onUploadError"
        :before-upload="onBeforeUpload"
        :on-change="onChangeUpload"
        :disabled="uploading"
      >
        <el-button type="primary" :disabled="uploading"><Pencil/></el-button>
      </el-upload>
      <el-button :type="has2fa ? 'danger' : 'success'" @click="open2fa(has2fa)" class="u-ml--8">
        {{ has2fa ? "Disable" : "Enable" }} 2FA
      </el-button>
    </template>
  </el-container>
  <el-container v-if="!isMe && !relationsLoading" class="margin-bottom justify-center main-button-container">
    <div
      v-if="!profileLoading"
      v-for="option in relationOptions"
      :key="option"
      class="button-container"
    >
      <el-button
        v-if="option === 'block'"
        type="danger"
        @click="fetchRelation(Relation.Block)"
      >
        Block
      </el-button>
      <el-button
        v-else-if="option === 'sendRelation'"
        type="primary"
        @click="fetchRelation(Relation.Send)"
      >
        Send Friend Request
      </el-button>
      <el-button
        v-else-if="option === 'acceptRelation'"
        type="secondary"
        @click="fetchRelation(Relation.Accept)"
      >
        Accept Request
      </el-button>
      <el-button
        v-else-if="option === 'declineRelation'"
        type="secondary"
        @click="fetchRelation(Relation.Reject)"
      >
        Reject Request
      </el-button>
      <div v-else-if="option === 'sent'">
        <el-button type="success"> pending </el-button>
        <el-button type="danger" @click="fetchRelation(Relation.Reject)">
          Cancel Request
        </el-button>
      </div>
      <el-button
        v-else-if="option === 'unfriend'"
        type="danger"
        @click="fetchRelation(Relation.Remove)"
      >
        Remove Friend
      </el-button>
    </div>
  </el-container>
</template>

<script setup lang="ts">
import Pencil from "@/components/Icons/Pencil.vue"
import { useRoute, useRouter } from "vue-router";
import { computed, toRefs, ref } from "vue";
import type { User as Profile } from "@/composables/profile/types";
import { useMe } from "@/composables/user/useMe";
import { useStatus } from "@/composables/profile/useStatus";
import { useProfile } from "@/composables/profile/useProfile";
import { useRelations } from "@/composables/relations/useRelations";
import { Relation } from "@/components/Profile/types";
import type { UploadProps } from 'element-plus'
import { ElMessage } from 'element-plus'

// constants
const IMAGE_TYPES = ['image/jpeg', 'image/png']
const IMAGE_URL = import.meta.env.VITE_IMAGE_URL;
const IMAGE_KEY = import.meta.env.VITE_IMAGE_KEY;

// props
type Props = {
  profile: Profile,
  isMe: boolean,
}
const props = defineProps<Props>()
const { profile, isMe } = toRefs(props)

// composables
const { me } = useMe()
const router = useRouter();
const route = useRoute();
const { usersStatus } = useStatus();
const { relationOptions, updateProfile, fetchProfile, loading: profileLoading } = useProfile();
const {
  blockUser,
  sendFriendRequest,
  acceptFriendRequest,
  rejectFriendRequest,
  removeFriend,
  unblockUser,
  loading: relationsLoading,
} = useRelations();

// refs
const uploading = ref(false)

// computed
const username = computed(() => route.params.username42 as string);
const has2fa = computed(() => !!me.value?.isTwoFactorAuthenticationEnabled);

const profileStatus = computed(() => {
  if (usersStatus.value && usersStatus.value.users && username.value) {
    return usersStatus.value.users[username.value];
  }
  return "offline";
});

// event handlers
const open2fa = (disable: boolean) => router
  .push({ name: "2fa", ...(disable ? { params: { action: "disable" } } : {}) });

const fetchRelation = async (
  relation: Relation,
  username42: string = username.value + ""
) => {

  switch (relation) {
    case Relation.Send: await sendFriendRequest(username42); break;
    case Relation.Accept: await acceptFriendRequest(username42); break;
    case Relation.Reject: await rejectFriendRequest(username42); break;
    case Relation.Remove: await removeFriend(username42); break;
    case Relation.Unblock: await unblockUser(username42); break;
    case Relation.Block:
      await blockUser(username42);
      router.push({ name: "home" });
      break;
  }
  if (relation !== Relation.Block) {
    await fetchProfile(username42);
  }
}

const onChangeUpload = () => uploading.value = true

const onUploadError: UploadProps['onError'] = () => {
  uploading.value = false
  ElMessage.error('Error occured uploading image!')
}

const onUploadSuccess: UploadProps['onSuccess'] = async (response) => {
  await updateProfile({ avatarImg: response.data.url })
  uploading.value = false
}

const onBeforeUpload: UploadProps['beforeUpload'] = (rawFile) => {
  if (!IMAGE_TYPES.includes(rawFile.type)) {
    ElMessage.error('Avatar picture must be JPG / PNG format!')
    return false
  } else if (rawFile.size / 1024 / 1024 > 32) {
    ElMessage.error('Avatar picture size can not exceed 32MB!')
    return false
  }
  return true
}
</script>

<style scoped>
.col-padding {
  padding: 10px;
}

.avatar {
  width: 20vw;
  height: 20vw;
  min-width: 150px;
  min-height: 150px;
  max-width: 400px;
  max-height: 400px;
}

.margin-bottom {
  margin-bottom: 10px;
}

.justify-center {
  justify-content: center;
}

.main-button-container {
  column-gap: 10px;
}
</style>