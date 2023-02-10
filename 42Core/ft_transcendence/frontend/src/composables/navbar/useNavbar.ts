import { reactive, toRefs } from "vue";

interface NavbarState {
  isOpen: boolean;
  isSearchOpen: boolean;
  isQueueOpen: boolean;
}

const state = reactive<NavbarState>({
  isOpen: false,
  isSearchOpen: false,
  isQueueOpen: false,
});

export const useNavbar = () => {

  const toggle = () => {
    state.isOpen = !state.isOpen;
  };

  const open = () => {
    state.isOpen = true;
  };

  const close = () => {
    state.isOpen = false;
  };

  const toggleSearch = () => {
    state.isSearchOpen = !state.isSearchOpen;
  };

  const openSearch = () => {
    state.isSearchOpen = true;
  };

  const closeSearch = () => {
    state.isSearchOpen = false;
  };

  const toggleQueue = () => {
    state.isQueueOpen = !state.isQueueOpen;
  };

  const openQueue = () => {
    state.isQueueOpen = true;
  };

  const closeQueue = () => {
    state.isQueueOpen = false;
  };

  return {
    toggle,
    open,
    close,
    toggleQueue,
    openQueue,
    closeQueue,
    toggleSearch,
    openSearch,
    closeSearch,
    ...toRefs(state)
  };
};
