<template>
  <el-card v-if="editing">
    <h4>Editing</h4>
    <hr />
    <span class="edit-form">
      <span class="item" v-for="choice in choices">
        <component :is="choice.componentName" />
        <span v-if="choice.type === 'text'">
          <el-input v-model="choice.value" :placeholder="'Setting Name'" />
        </span>
        <span v-else>
          <el-color-picker
            v-if="isColor(choice.value)"
            v-model="choice.value"
            size="large"
          />
          <el-select
            v-else-if="choice.value"
            v-model="choice.value"
          >
            <el-option
              v-for="(skin, i) in choice.skins"
              :key="i"
              :label="skin.title"
              :value="skin.imgUrl"
            >
              <el-image
                style="width:100px; height: 50px;"
                :src="skin.imgUrl"
                :fit="'cover'"
              />
            </el-option>
          </el-select>

          <span v-if="!choice.value">
            <el-tooltip effect="light" content="Pick Color" placement="top">
              <el-button
                style="padding: 1rem auto !important"
                @click="reset(choice, '#424242')"
              >
                <Palette />
              </el-button>
            </el-tooltip>
            <el-tooltip effect="light" content="Pick Skin" placement="top">
              <el-button
                style="padding: 1rem auto !important"
                @click="
                  reset(
                    choice,
                    'https://as2.ftcdn.net/v2/jpg/04/44/01/33/1000_F_444013321_vzwRWzfmRQNlCFwYJmMaqssq6HuKENuf.jpg'
                  )
                "
              >
                <Texture />
              </el-button>
            </el-tooltip>
          </span>
        </span>
        <el-button v-if="choice.value" @click="reset(choice)">
          <Trash />
        </el-button>
      </span>
    </span>

    <hr />
    <el-button
      type="success"
      @click="
        saveSettings();
        toggleEdit();
      "
    >
      <p>Save</p>
      <SaveIcon />
    </el-button>
    <el-button @click="toggleEdit">
      <svg width="30px" height="30px" viewBox="0 0 32 32">
        <path
          d="M10.722 9.969l-0.754 0.754 5.278 5.278-5.253 5.253 0.754 0.754 5.253-5.253 5.253 5.253 0.754-0.754-5.253-5.253 5.278-5.278-0.754-0.754-5.278 5.278z"
          fill="var(--light)"
        />
      </svg>
    </el-button>
  </el-card>
  <el-card v-else>
    <h2>{{ setting.name }}</h2>
    <hr />
    <div class="config-container">
      <Bucket v-if="isColor(setting.puck)" :color="setting.puck" />
      <el-avatar
        v-else
        :src="setting.puck"
        :size="35"
        class="animate__animated animate__headShake margin-bottom"
      />
      <el-tooltip effect="light" content="Puck" placement="right">
        <PuckIcon />
      </el-tooltip>
    </div>

    <div class="config-container">
      <Bucket v-if="isColor(setting.paddleR)" :color="setting.paddleR" />
      <el-avatar
        :src="setting.paddleR"
        :size="35"
        v-else
        class="animate__animated animate__headShake margin-bottom"
      />
      <el-tooltip effect="light" content="Paddle Right" placement="right">
        <PaddleIcon />
      </el-tooltip>
    </div>

    <div class="config-container mirror-svg">
      <Bucket v-if="isColor(setting.paddleL)" :color="setting.paddleL" />
      <el-avatar
        :src="setting.paddleL"
        :size="35"
        v-else
        class="animate__animated animate__headShake margin-bottom"
      />
      <el-tooltip effect="light" content="Paddle Left" placement="right">
        <PaddleIcon />
      </el-tooltip>
    </div>
    <div class="config-container">
      <Bucket v-if="isColor(setting.bg)" :color="setting.bg" />
      <el-avatar
        :src="setting.bg"
        :size="35"
        v-else
        class="animate__animated animate__headShake margin-bottom"
      />
      <el-tooltip effect="light" content="Background" placement="right">
        <BackgroundIcon />
      </el-tooltip>
    </div>
    <hr />
    <el-button type="success" v-if="isDefaultSetting"> Default </el-button>
    <el-button @click="makeDefault(setting.id)" v-else>
      Make Default
    </el-button>
    <el-button type="primary" @click="toggleEdit">
      <Pencil />
    </el-button>
    <el-button
      v-if="!isDefaultSetting"
      type="warning"
      @click="deleteSetting(setting)"
    >
      <Trash />
    </el-button>
  </el-card>
</template>

<script setup lang="ts">
import { ref, computed } from "vue";
import Trash from "@/components/Icons/Trash.vue";
import SaveIcon from "@/components/Icons/SaveIcon.vue";
import PuckIcon from "@/components/Icons/PuckIcon.vue";
import Pencil from "@/components/Icons/Pencil.vue";
import Bucket from "@/components/Icons/Bucket.vue";
import PaddleIcon from "@/components/Icons/PaddleIcon.vue";
import Palette from "@/components/Icons/Palette.vue";
import Texture from "@/components/Icons/Texture.vue";
import BackgroundIcon from "@/components/Icons/BackgroundIcon.vue";
import { useProfileSettings } from "@/composables/profile/useProfileSettings";

const {
  updateSetting,
  fetchSkins,
  skins,
} = useProfileSettings();
fetchSkins();
interface Choices {
  puckChoice: { type: string; componentName: string; value: ref<string> };
  puckChoice: {
    type: string;
    componentName: string;
    value: ref<string>;
    skins: string[];
  };
  paddleRChoice: {
    type: string;
    componentName: string;
    value: ref<string>;
    skins: string[];
  };
  paddleLChoice: {
    type: string;
    componentName: string;
    value: ref<string>;
    skins: string[];
  };
  bgChoice: {
    type: string;
    componentName: string;
    value: ref<string>;
    skins: string[];
  };
}
const props = defineProps([
  "setting",
  "makeDefault",
  "isDefaultSetting",
  "deleteSetting",
]);
const setting = computed(() => props.setting);
const editing = ref<boolean>(false);
const isColor = (str) => {
  return /^#[0-9A-F]{6}$/i.test(str);
};
const reset = (element, choice = "") => {
  element.value = choice;
};
const toggleEdit = () => {
  editing.value = !editing.value;
};
const saveSettings = () => {
  setting.value.name = choices.value.nameChoice.value;
  setting.value.puck = choices.value.puckChoice.value;
  setting.value.paddleR = choices.value.paddleRChoice.value;
  setting.value.paddleL = choices.value.paddleLChoice.value;
  setting.value.bg = choices.value.bgChoice.value;
  updateSetting(setting.value.id, {
    name: choices.value.nameChoice.value,
    puck: choices.value.puckChoice.value,
    paddleR: choices.value.paddleRChoice.value,
    paddleL: choices.value.paddleLChoice.value,
    bg: choices.value.bgChoice.value,
  });
}

const choices = ref<Choices>({
  nameChoice: {type: "text", componentName: "TextIcon", value: setting.value.name},
  puckChoice: {type: "any", componentName: "PuckIcon", value: setting.value.puck, skins: skins.value},
  paddleRChoice: {type: "any", componentName: "PaddleIcon", value: setting.value.paddleL, skins: skins.value},
  paddleLChoice: {type: "any", componentName: "PaddleIcon", value: setting.value.paddleR, skins: skins.value},
  bgChoice: {type: "any", componentName: "BackgroundIcon", value: setting.value.bg, skins: skins.value},
});
</script>

<style lang="scss" scoped>
.config-container {
  display: flex;
  flex-direction: row;
  justify-content: space-around;
  align-items: center;
  min-height: 50px;
  p {
    margin: 0;
  }
  svg {
    height: 35px;
    width: 35px;
  }
}
.mirror-svg {
  .paddle {
    -webkit-transform: scaleX(-1);
    transform: scaleX(-1);
  }
}
.edit-form {
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 1rem;
  svg {
    height: 35px;
    width: 35px;
    fill: var(--light);
  }
  svg.sm {
    height: 20px !important;
    width: 20px !important;
  }
  .item {
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    align-items: center;
    gap: 1rem;
  }
}
.el-select-dropdown__item {
  text-align: center;
  padding: 0;
  overflow: hidden;
}
</style>