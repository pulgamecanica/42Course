<template>
  <div>
    <button
      class="cn-button"
      id="cn-button"
      @click="toggleNavbar"
    >
      <Logo42 :isOpen="isOpen" />
    </button>
    <div
      class="cn-wrapper animate__animated animate__bounceInUp"
      id="cn-wrapper"
      v-if="me"
      v-show="isOpen">
      <ul>
        <li>
          <RouterLink
            class="nav-btn"
            :to="{ name: 'profile', params: { username42: me.username42 }}"
          >
            <svg
              fill="#000000"
              viewBox="0 0 32 32"
            >
              <g>
                <path
                  d="M16,16A7,7,0,1,0,9,9,7,7,0,0,0,16,16ZM16,4a5,5,0,1,1-5,5A5,5,0,0,1,16,4Z"
                />
                <path
                  d="M17,18H15A11,11,0,0,0,4,29a1,1,0,0,0,1,1H27a1,1,0,0,0,1-1A11,11,0,0,0,17,18ZM6.06,28A9,9,0,0,1,15,20h2a9,9,0,0,1,8.94,8Z"
                />
              </g>
            </svg>
          </RouterLink>
        </li>
        <li>
          <button
            class="nav-btn"
            @click="toggleQueue"
          >
            <Controller />
          </button>
        </li>
        <li>
          <RouterLink
            class="nav-btn"
            :to="{ name: 'home'}"
            href="/"
          >
            <svg
              fill="#000000"
              width="800px"
              height="800px"
              viewBox="0 0 32 32"
              enable-background="new 0 0 32 32"
              xml:space="preserve"
            >
              <path
                d="M30.854,16.548C30.523,17.43,29.703,18,28.764,18H28v11c0,0.552-0.448,1-1,1h-6v-7c0-2.757-2.243-5-5-5  s-5,2.243-5,5v7H5c-0.552,0-1-0.448-1-1V18H3.235c-0.939,0-1.759-0.569-2.09-1.451c-0.331-0.882-0.088-1.852,0.62-2.47L13.444,3.019  c1.434-1.357,3.679-1.357,5.112,0l11.707,11.086C30.941,14.696,31.185,15.666,30.854,16.548z"
              />
            </svg>
          </RouterLink>
        </li>
        <li>
          <button
            class="nav-btn"
            @click="toggleSearch"
          >
            <svg viewBox="0 0 24 24" fill="none">
              <path fill-rule="evenodd" clip-rule="evenodd" d="M10 1C5.02944 1 1 5.02944 1 10C1 14.9706 5.02944 19 10 19C12.125 19 14.078 18.2635 15.6177 17.0319L20.2929 21.7071C20.6834 22.0976 21.3166 22.0976 21.7071 21.7071C22.0976 21.3166 22.0976 20.6834 21.7071 20.2929L17.0319 15.6177C18.2635 14.078 19 12.125 19 10C19 5.02944 14.9706 1 10 1ZM3 10C3 6.13401 6.13401 3 10 3C13.866 3 17 6.13401 17 10C17 13.866 13.866 17 10 17C6.13401 17 3 13.866 3 10Z" fill="#000000"/>
            </svg>
          </button>
        </li>
        <li>
          <a
            class="nav-btn"
            @click="logout"
          >
            <svg
              viewBox="0 0 24 24"
              fill=""
            >
              <path
                fill="#000000"
                fill-rule="evenodd"
                d="M6 2a3 3 0 0 0-3 3v14a3 3 0 0 0 3 3h6a3 3 0 0 0 3-3V5a3 3 0 0 0-3-3H6zm10.293 5.293a1 1 0 0 1 1.414 0l4 4a1 1 0 0 1 0 1.414l-4 4a1 1 0 0 1-1.414-1.414L18.586 13H10a1 1 0 1 1 0-2h8.586l-2.293-2.293a1 1 0 0 1 0-1.414z"
                clip-rule="evenodd"
              />
            </svg>
          </a>
        </li>
      </ul>
    </div>
  </div>
</template>

<script setup lang="ts">
import Controller from '@/components/Icons/Controller.vue';
import Logo42 from '@/components/Icons/Logo42.vue';
import { useMe } from "@/composables/user/useMe";
import { useNavbar } from "@/composables/navbar/useNavbar";

const {
  toggle: toggleNavbar,
  isOpen,
  isQueueOpen,
  isSearchOpen,
  open,
  close,
  openQueue,
  closeQueue,
  openSearch,
  closeSearch,
} = useNavbar();
const { fetchMe, me, logout } = useMe();

fetchMe();

const toggleQueue = () => {

  if (isQueueOpen.value) {
    closeQueue();
    open()
  } else {
    openQueue();
    close()
  }
}
const toggleSearch = () => {
  if (isSearchOpen.value) {
    closeSearch();
    open()
  } else {
    openSearch();
    close()
  }
}
</script>

<style lang="scss" scoped>
@use '../assets/foundation/vars';

* {
  position: relative;
  box-sizing: border-box;
}

.cn-button {
  border: none;
  background: none;
  color: var(white);
  text-align: center;
  font-size: 1.5em;
  padding-bottom: 1em;
  height: 3.5em;
  width: 3.5em;
  background-color: var(--primary);
  position: fixed;
  left: 50%;
  margin-left: -1.75em;
  bottom: -1.85em;
  border-radius: 50%;
  cursor: pointer;
  z-index: 22
}

.cn-button:hover {
  bottom: -1.75em;
}

.cn-button svg {
  width: 50%;
}

.cn-button:hover,
.cn-button:active{
  background-color: var(--dark);
}

.cn-wrapper {
  width: 26em;
  height: 26em;
  position: fixed;
  z-index: 21;
  bottom: 0;
  left: 50%;
  bottom: -13em;
  margin-left: -210px;
  -webkit-transition: all .3s ease;
  transition: all .3s ease;
  border-radius:  50%;
  overflow: hidden;
}

.cn-wrapper, .cn-wrapper * {
  margin-top: 0;
}

.cn-wrapper li {
  position: absolute;
  font-size: 1.5em;
  width: 10em;
  height: 10em;
  overflow: hidden;
  -webkit-transform-origin: 100% 100%;
  transform-origin: 100% 100%;
  border: 4px solid transparent;
  -webkit-transition: all 1s ease;
  transition: all 1s ease;
  color: #aaa;
  left: 50%;
  top: 50%;
  margin-top: -1.3em !important;
  margin-left: -10em !important;
  overflow: hidden;
}

.cn-wrapper li:nth-child(1) {
  transform: rotate(-10deg) skew(50deg);
}
.cn-wrapper li:nth-child(2) {
  transform: rotate(30deg) skew(50deg);
}
.cn-wrapper li:nth-child(3) {
  transform: rotate(70deg) skew(50deg);
}
.cn-wrapper li:nth-child(4) {
  transform: rotate(110deg) skew(50deg);
}
.cn-wrapper li:nth-child(5) {
  transform: rotate(150deg) skew(50deg);
}

.cn-wrapper li .nav-btn {
  display: block;
  font-size: 1.2em;
  height: 13.5em;
  width: 13.5em;
  position: absolute;
  bottom: -6.75em;
  right: -6.75em;
  cursor: pointer;
  text-decoration: none;
  color: white;
  -webkit-transition: background-color .3s ease, -webkit-transform .8s ease;
  transition: background-color .3s ease, -webkit-transform .8s ease;
  transition: background-color .3s ease, transform .8s ease;
  transform: skew(-50deg) rotate(-70deg) scale(1.2);
  border-radius: 50%;
  text-align: center;
  padding-top: 2em;
}

.cn-wrapper li .nav-btn:hover :deep(*) {
  fill: white;
}

.cn-wrapper li:nth-child(1) .nav-btn {
  background-color: var(--yellow);
}
.cn-wrapper li:nth-child(2) .nav-btn {
  background-color: var(--orange);
}
.cn-wrapper li:nth-child(3) .nav-btn {
  background-color: var(--teal);
}
.cn-wrapper li:nth-child(4) .nav-btn {
  background-color: var(--primary-light);
}
.cn-wrapper li:nth-child(5) .nav-btn {
  background-color: #741A1A;
}

.cn-wrapper li .nav-btn svg {
  width: 1.5em;
  height: auto;
  position: absolute;
  top: 15%;
  right: 44.5%;
}
</style>