// #include "libft.h"
// #include "utils.h"
#include "animations.h"

void    update_animation(t_animation * a) {
    if (a) {
        if (a->_tmp_delay++ == a->delay) {
            a->_tmp_delay = 0;
            a->current_frame_num++;
            a->current_frame_num %= ft_lstsize(a->frames);
        }
    }
}

static void _helper_destroy_img(void * ptr1, void * ptr2) {
    mlx_delete_image(ptr2, ptr1);
}

void    destroy_animation(void * ptr, void * mlx) {
    t_animation * a;

    a = (t_animation *)ptr;
    if (!a)
        return ;
    ft_lstiter_param(a->frames, _helper_destroy_img, mlx);
    // ft_lstclear(&a->frames, free);
    free(a->name);
    free(a);
}