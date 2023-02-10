import { createApp } from 'vue'
import './assets/style.scss'
import ElementPlus from 'element-plus'
import App from './App.vue'
import router from './router/index'
import Particles from 'vue3-particles';
import VueAxios from 'vue-axios'
import 'animate.css';
import { useAxios } from '@/composables/axios'
import PuckIcon from '@/components/Icons/PuckIcon.vue';
import PaddleIcon from '@/components/Icons/PaddleIcon.vue';
import BackgroundIcon from '@/components/Icons/BackgroundIcon.vue';
import TextIcon from '@/components/Icons/TextIcon.vue';
import * as ElementPlusIconsVue from '@element-plus/icons-vue'

const { axios } = useAxios()

const app = createApp(App)

// register all Element Icon components at once
for (const [key, component] of Object.entries(ElementPlusIconsVue)) {
  app.component(key, component)
}
app
	.component('PuckIcon', PuckIcon)
	.component('PaddleIcon', PaddleIcon)
	.component('BackgroundIcon', BackgroundIcon)
	.component('TextIcon', TextIcon)

app.use(ElementPlus)
app.use(router)
app.use(Particles)
app.use(VueAxios, axios)

app.mount('#app')
