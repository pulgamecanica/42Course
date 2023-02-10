<template>
  <el-container class="fields-holder">
    <h2 class="hidden-xs-only">Profile Details</h2>
    <el-form :model="formData" ref="form" class="profile-form" :rules="rules">
      <el-form-item prop="username42" :label-width="formLabelWidth" label="Username 42">
        <div class="read u-ml--8">{{ profile.username42 }}</div>
      </el-form-item>
      <el-form-item prop="nickname" :label-width="formLabelWidth" label="Nickname" :error="getError('nickname')">
        <div v-if="!editing" class="read u-ml--8">{{ profile.nickname }}</div>
        <el-input v-else v-model="formData.nickname" clearable autofocus/>
      </el-form-item>
      <el-form-item prop="firstName" :label-width="formLabelWidth" label="First Name">
        <div v-if="!editing" class="read u-ml--8">{{ profile.firstName }}</div>
        <el-input v-else v-model="formData.firstName" clearable/>
      </el-form-item>
      <el-form-item prop="lastName" :label-width="formLabelWidth" label="Last Name">
        <div v-if="!editing" class="read u-ml--8">{{ profile.lastName }}</div>
        <el-input v-else v-model="formData.lastName" clearable/>
      </el-form-item>
      <el-form-item prop="email" :label-width="formLabelWidth" label="Email">
        <div v-if="!editing" class="read u-ml--8">{{ profile.email }}</div>
        <el-input v-else v-model="formData.email" clearable/>
      </el-form-item>
      <el-form-item prop="bio" :label-width="formLabelWidth" label="Bio">
        <div v-if="!editing" class="read u-ml--8 bio-box">{{ profile.bio }}</div>
        <el-input class="bio-box" v-else v-model="formData.bio" type="textarea"/>
      </el-form-item>
      <template v-if="editing">
        <el-button type="success" @click="saveProfile" :disabled="disabled"><SaveIcon/></el-button>
        <el-button type="danger" @click="cancelEditing"><Trash/></el-button>
      </template>
      <el-button type="primary" v-if="!editing && isMe" @click="startEditing"><Pencil/></el-button>
    </el-form>
  </el-container>
</template>

<script setup lang="ts">
import SaveIcon from "@/components/Icons/SaveIcon.vue"
import Pencil from "@/components/Icons/Pencil.vue"
import Trash from "@/components/Icons/Trash.vue"
import { computed, toRefs, ref, reactive } from "vue";
import type { User as Profile, ProfileStruct } from "@/composables/profile/types";
import { useProfile } from "@/composables/profile/useProfile";
import type { FormInstance, FormRules } from 'element-plus';
import { ElMessage } from 'element-plus';
import { useMe } from "@/composables/user/useMe";

// props
type Props = {
  profile: Profile,
  isMe: boolean,
}
const props = defineProps<Props>()
const { profile, isMe } = toRefs(props)
const { first, firstOff } = useMe()

// composables
const { updateProfile, errors, clearErrors } = useProfile();

// Constants
const EMAIL = /^(([^<>()\[\]\\.,;:\s@"]+(\.[^<>()\[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,24}))$/
const formLabelWidth = '213px'

const validateEmail = (_rule: any, value: string, cb: any) => {
  if (value && !isEmail(value)) {
    cb(new Error('Not a valid Email!'))
  } else {
    cb()
  }
}

const prefill = () => {
  if (!profile.value) {
    return
  }
  const p = profile.value
  formData.email = p.email
  formData.firstName = p.firstName
  formData.lastName = p.lastName
  formData.nickname = p.nickname
  formData.bio = p.bio
}

const editing = ref(first.value || errors.value);
const form = ref<FormInstance>()
const formData = reactive<ProfileStruct>({})

const rules = reactive<FormRules>({
  email: [
    { validator: validateEmail },
    { max: 200, message: 'Length should be below 200', trigger: 'blur' },
  ],
  firstName: [
    { required:true, message: 'First name is required' },
    { max: 200, message: 'Length should be below 200', trigger: 'blur' },
  ],
  lastName: [
    { required:true, message: 'Last name is required' },
    { max: 200, message: 'Length should be below 200', trigger: 'blur' },
  ],
})

// computed
const disabled = computed(() => !formData.email || !isEmail(formData.email))

const getError = (key: string) => {
  if (!errors.value) {
    return ''
  }
  if (key in errors.value) {
    return errors.value[key][0]
  }
  return ''
}

// lifecycle hooks
prefill()
if (first.value) {

  ElMessage({
    message: 'Welcome! Start by setting your nickname.',
    type: 'success',
  })
  firstOff()
}

// methods
const isEmail = (str: string) => EMAIL.test(str)

const startEditing = () => {
  clearErrors();
  prefill()
  editing.value = true;
}

const saveProfile = async () => {
  if (!form.value) {
    return
  }
  try {
    await form.value.validate(async (valid) => {
      if (valid) {
        await updateProfile(formData)
      }
    })
  } catch (e) {}
}

const cancelEditing = () => { 
  editing.value = false; 
  clearErrors(); 
}
</script>

<style scoped lang="scss">
.fields-holder {
  flex-direction: column;
  max-width: 425px;
  display: flex;
  margin: auto;
  row-gap: 15px;
  height: 100%;
  justify-content: center;
}

.read {
  text-align: left;
  font-weight: 900;
}
.bio-box {
  max-height: 200px;
  overflow-Y: scroll;
}

.el-form-item:nth-child(odd) {
  background: rgba(0, 0, 0, 0.25)
}
</style>