#include "libft.h"
#include "ft_printf.h"
#include "animation.h"
#include "sprite.h"
#include "utils.h"
#include "colors.h"
#include <fcntl.h>

/**
 * Util functions to handle files
 * defining slices for a sprite sheet
 * Exit Codes
 * 2 -> Memory allocation Error
 * 3 -> mlx return error, discrepancy
 **/
static  void  parse_header_optional_padding(char ** list, int * padding_x, int * padding_y) {
  char ** padding_list;

  if (!list)
    return ;
  if (ft_list_size(list) != 3) {
    free_list(list);
    return ;
  }
  padding_list = ft_split(list[2], ',');
  if (ft_list_size(padding_list) == 1) {
    *padding_x = ft_atoi(padding_list[0]);
    *padding_y = *padding_x;
  } else if (ft_list_size(padding_list) == 2) {
    *padding_x = ft_atoi(padding_list[0]);
    *padding_y = ft_atoi(padding_list[1]); 
  } else {
    ft_printf(""
      "%sload_file_sprite(win, path)\n\tparse_header_optional_padding(list, padding_x, padding_y)%s"
      "\n\t\t[Line: 1] %sError parsing padding, must follow the correct formats:\n\t\t"
      "%sOption 1%s path name padding\n\t\t%sOption 2%s path name padding_x,padding_y%s\n",
      BLUE, GREEN, RED, GREEN, RED, GREEN, RED, ENDC);
  }
  free_list(padding_list);
  free_list(list);
}

static  t_sprite  * parse_header(char ** list, t_win * win) {
  t_sprite  * s;

  if (!list)
    return NULL;
  if (ft_list_size(list) != 2 && ft_list_size(list) != 3) {
    free_list(list);
    return NULL;
  }
  s = (t_sprite *)ft_calloc(sizeof(t_sprite), 1);
  if (!s)
    exit(2);
  ft_printf("%sNew Sprite%s %s %s\n", GREEN, ENDC, list[0], list[1]);
  *s = new_sprite(list[1], list[0], win);
  if (!s->sprite_img->img_ptr) {
    free(s);
    return NULL;
  }
  free_list(list);
  return s;
}

static  parsed_slice  * new_parsed_slice(char ** list, int padding_x, int padding_y, int line_num) {
  parsed_slice  *   p_slice;
  char          **  slice_list;

  if (!list)
    exit(2);
  if (ft_list_size(list) != 4) {
    ft_printf(""
      "%sload_file_sprite(win, path)\n\tnew_parsed_slice(list, line_num)%s"
      "\n\t\t[Line: %d] %sError parsing line, must follow the correct format:\n\t\t          "
      "name x,y,widht,height #frames #delay%s\n", BLUE, GREEN, line_num, RED, ENDC);
    free_list(list);
    return NULL;
  }
  slice_list = ft_split(list[1], ',');
  if (!slice_list)
    exit(2);
  if (ft_list_size(slice_list) != 4) {
    ft_printf(""
      "%sload_file_sprite(win, path)\n\tnew_parsed_slice(list, line_num)%s"
      "\n\t\t[Line: %d] %sError parsing slice, must follow the correct format:\n\t\t          "
      "%sname %sx,y,widht,height%s #frames #delay%s\n", BLUE, GREEN, line_num, RED, GREEN, RED, GREEN, ENDC);
    free_list(list);
    free_list(slice_list);
    return NULL;
  }
  p_slice = (parsed_slice *)ft_calloc(sizeof(parsed_slice), 1);
  if (!p_slice) {
    free_list(list);
    free_list(slice_list);
    exit(2);
  }
  p_slice->slice = (sprite_slice){ft_atoi(slice_list[0]), ft_atoi(slice_list[1]), ft_atoi(slice_list[2]), ft_atoi(slice_list[3]), padding_x, padding_y};
  p_slice->name = ft_strdup(list[0]);
  p_slice->frames = ft_atoi(list[2]);
  p_slice->delay = ft_atoi(list[3]);
  free_list(list);
  free_list(slice_list);
  return p_slice;
}

static parsed_slice * get_parsed_slice_by_name(t_list * slices, char * name) {
  t_list        * n;
  parsed_slice  * p_slice;

  n = slices;
  while (n != NULL) {
    p_slice = (parsed_slice *)n->content;
    if (p_slice)
      if (!ft_strcmp(name, p_slice->name))
        return (p_slice);
    n = n->next;
  }
  return NULL;
}

/**
 * While a slice cuts each frame we can still create many animations with out slices
 * In the end we will have a t_sprite object with all our animations
 **/
static void parse_animation(t_sprite * s, t_list * slices, char ** list, int line_num) {
  parsed_slice * p_slice;

  if (!list)
    exit(2);
  if (ft_list_size(list) != 1 && ft_list_size(list) != 2) {
    ft_printf(""
      "%sload_file_sprite(win, path)\n\tparse_animation(sprite, slices, params, line_num)%s"
      "\n\t\t[Line: %d] %sError parsing animation, must follow the correct format:\n\t\t"
      "animation_name mirrored(optional)\n",
      BLUE, GREEN, line_num, RED, ENDC);
    free_list(list);
    return ;
  }
  if (ft_list_size(list) == 2) {
    if (ft_strcmp("mirrored", list[1])) {
      ft_printf(""
        "%sload_file_sprite(win, path)\n\tparse_animation(sprite, slices, params, line_num)%s"
        "\n\t\t[Line: %d] %sError optional animation argument: do you mean %s\"mirrored\"%s instead of %s%s\n",
        BLUE, GREEN, line_num, RED, GREEN, RED, list[1], ENDC);
      free_list(list);
      return ;
    }
  }
  p_slice = get_parsed_slice_by_name(slices, list[0]);
  if (!p_slice) {
    ft_printf(""
        "%sload_file_sprite(win, path)\n\tparse_animation(sprite, slices, params, line_num)%s"
        "\n\t\t[Line: %d] %sError animation not found: \"%s\" didn't match any previous declared animation%s\n",
        BLUE, GREEN, line_num, RED, list[0], ENDC);
    free_list(list);
    return ;
  }
  ft_lstadd_back(&s->animations, ft_lstnew(slice_sprite(*s, p_slice->slice, ft_list_size(list) == 2, p_slice->frames, p_slice->delay, list[0])));
  free_list(list);
}

static void destroy_parsed_slice(void * ptr) {
  parsed_slice * s;

  s = (parsed_slice *)ptr;
  if (!s)
    return ;
  if (s)
    free(s->name);
  free(s);
}

static void print_parsed_slices(void * ptr) {
  parsed_slice * s;

  s = (parsed_slice *)ptr;
  if (!s)
    return ;
  ft_printf("\t%s%- 10s%s /%03d-%03d-%03d-%03d/, %- 2d frames, %d delay\n", GREEN, s->name, ENDC, s->slice.x, s->slice.y, s->slice.width, s->slice.height, s->frames, s->delay);
}

static void print_animations(void * ptr) {
  t_animation * a;

  a = (t_animation *)ptr;
  if (!a)
    return ;
  ft_printf("\t[%s%c%s] %s%- 12s%s %dx%d(px)\n", GREEN, a->mirrored ? 'm' : ' ', ENDC, GREEN, a->name, ENDC, a->width, a->height);
}

t_sprite * load_file_sprite(t_win * win, char * path) {
  t_sprite  * s;
  t_list    * slices;
  char      * line;
  int         fd;
  int         line_num;
  int         padding_x;
  int         padding_y;

  padding_x = 0;
  padding_y = 0;
  if (((fd = open(path, O_RDONLY)) == -1)) {
    ft_printf("%sload_file_sprite(win, path)\n\topen(path, flags) %s\n\t\tError opening the file '%s'%s\n", BLUE, RED, path, ENDC);
    return NULL;
  }
  if (!(line = get_next_line_nl(fd))) {
    close(fd);
    return NULL;
  }
  s = parse_header(ft_split(line, ' '), win);
  parse_header_optional_padding(ft_split(line, ' '), &padding_x, &padding_y);
  free(line);
  if (!s) {
    ft_printf("%sload_file_sprite(win, path)\n\tparse_header(list, win) %s\n\t\t[Line: 1] Wrong Header expected 'path name'%s\n", BLUE, RED, ENDC);
    close(fd);
    return NULL;
  }
  slices = (t_list *)ft_calloc(sizeof(t_list), 1);
  if (!slices)
    exit(2);
  line_num = 1;
  while(line_num++ && (line = get_next_line_nl(fd))) {
    if (!ft_strlen(line)) {
      break ;
    }
    ft_lstadd_back(&slices, ft_lstnew(new_parsed_slice(ft_split(line, ' '), padding_x, padding_y, line_num)));
    free(line);
  }
  free(line);
  while(line_num++ && (line = get_next_line_nl(fd))) {
    if (!ft_strlen(line)) {
      free(line);
      continue ;
    }
    parse_animation(s, slices, ft_split(line, ' '), line_num);
    free(line);
  }
  close(fd);
  ft_printf("Loaded %s[%d]%s slices:\n", GREEN, ft_lstsize(slices), ENDC);
  ft_printf("\t%- 10s /%- 3s %- 3s %- 3s %- 3s/  #x  frames  t(x) delay\n", "Name", "x", "y", "w", "h");
  ft_printf("\t---------- -----------------  ----------  ----------\n");
  ft_lstiter(slices, print_parsed_slices);
  ft_printf("Loaded %s[%d]%s animations\t([m] = mirrored):\n", GREEN, ft_lstsize(s->animations), ENDC);
  ft_lstiter(s->animations, print_animations);
  ft_lstclear(&slices, destroy_parsed_slice);
  free(slices);
  return (s);
}