#include "libft.h"
#include "ft_printf.h"
#include "animation.h"
#include "sprite.h"
#include "utils.h"
#include <fcntl.h>

#define ft_max(x, y) ((x) > (y) ? (x) : (y))

static t_sprite * parse_header(char **list, t_win * win) {
	t_sprite * s;

	if (!list)
		return NULL;
	if (ft_list_size(list) != 2) {
		ft_printf("Wrong Header expected:\n\tpath name\n");
		free_list(list);
		return NULL;
	}
	s = (t_sprite *)ft_calloc(sizeof(t_sprite), 1);
	if (!s)
		return NULL;
	ft_printf("New Sprite %s %s\n", list[0], list[1]);
	*s = new_sprite(list[1], list[0], win);
	if (!s->sprite_img.img_ptr) {
		free(s);
		return NULL;
	}
	free_list(list);
	return s;
}

static enum entity parse_entity(char *line) {
	enum entity e;

	e = OBJ;
	if (ft_strncmp("PLAYER", line, 6))
		e = PLAYER;
	else if (ft_strncmp("ENEMY", line, 5))
		e = ENEMY;
	else if (ft_strncmp("EVENT", line, 5))
		e = EVENT;
	else if (ft_strncmp("ANIMAL", line, 5))
		e = ANIMAL;
	return e;
}

static parsed_slice * new_parsed_slice(char **list) {
	parsed_slice *	p_slice;
	char **			slice_list;

	if (ft_list_size(list) != 4) {
		free (list);
		ft_printf("Error while parsing line, must be of type:\n\tname x,y,widht,height #frames #delay\n");
		return NULL;
	}
	slice_list = ft_split(list[1], ',');
	if (!slice_list || ft_list_size(slice_list) != 4) {
		ft_printf("Error while parsing slice %s\n", list[0]);
		free_list(list);
		free(slice_list);
		return NULL;
	}
	p_slice = (parsed_slice *)ft_calloc(sizeof(parsed_slice), 1);
	if (!p_slice) {
		free_list(list);
		free(slice_list);
		return NULL;
	}
	p_slice->slice = (sprite_slice){ft_atoi(slice_list[0]), ft_atoi(slice_list[1]), ft_atoi(slice_list[2]), ft_atoi(slice_list[3])};
	p_slice->name = ft_strdup(list[0]);
	p_slice->frames = ft_atoi(list[2]);
	p_slice->delay = ft_atoi(list[3]);
	free_list(list);
	free_list(slice_list);
	return p_slice;
}

static parsed_slice * get_parsed_slice_by_name(t_list * slices, char * name) {
	t_list	* n;
	parsed_slice * p_slice;

	n = slices;
	while (n != NULL) {
		p_slice = (parsed_slice *)n->content;
		if (p_slice) {
			if (!ft_strncmp(name, p_slice->name, ft_max(ft_strlen(name), ft_strlen(p_slice->name))))
				return (p_slice);
		}
		n = n->next;
	}
	return NULL;
}

static void parse_animation(t_sprite * s, t_list * slices, enum entity e, char ** list) {
	parsed_slice * p_slice;

	if (!s || !slices || !list || ft_list_size(list) < 2 || ft_list_size(list) > 3) {
		ft_printf("Error while parsing animation\n");
		free_list(list);
		return ;
	}
	//key_mapping = ft_atoi(list[1]);
	if (ft_list_size(list) == 3) {
		if (ft_strncmp("mirrored", list[1], ft_max(ft_strlen("mirrored"), ft_strlen(list[1])))) {
			ft_printf("Error while parsing animation %s, do you mean \"mirrored\" instead of %s\n", list[0], list[1]);
			free_list(list);
			return ;
		}
		//key_mapping = ft_atoi(list[2]);
	}
	p_slice = get_parsed_slice_by_name(slices, list[0]);
	if (!p_slice) {
		ft_printf("Error while parsing animation \"%s\", name not founded\n", list[0]);
		return ;
	}
	ft_lstadd_back(&s->animations, ft_lstnew(slice_sprite(*s, p_slice->slice, ft_list_size(list) == 3, p_slice->frames, p_slice->delay, e, list[0])));
	free_list(list);
}

static void free_parsed_slice(void * ptr) {
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
	ft_printf("%- 10s /%03d-%03d-%03d-%03d/, %d frames, %d delay\n", s->name, s->slice.x, s->slice.y, s->slice.width, s->slice.height, s->frames, s->delay);
}

static void print_animations(void * ptr) {
	t_animation * a;

	a = (t_animation *)ptr;
	if (!a)
		return ;
	ft_printf("[%c] %s (%03dx%03d)\n", a->mirrored ? 'm' : ' ', a->name, a->width, a->height);
}

t_sprite * load_file_sprite(t_win * win, char * path) {
	t_sprite *	s;
	t_list *	slices;
	enum entity	e;
	char *		line;
	int			fd;

	if (((fd = open(path, O_RDONLY)) == -1)) {
		ft_printf("Error opening the file %s\n", path);
		return NULL;
	}
	if (!(line = get_next_line_nl(fd))) {
		return NULL;
	}
	s = parse_header(ft_split(line, ' '), win);
	free(line);
	if (!s || !(line = get_next_line_nl(fd)))
		return NULL;
	e = parse_entity(line);
	free(line);
	slices = (t_list *)ft_calloc(sizeof(t_list), 1);
	if (!slices)
		return NULL;
	while((line = get_next_line_nl(fd))) {
		if (!ft_strlen(line))
			break ;
		ft_lstadd_back(&slices, ft_lstnew(new_parsed_slice(ft_split(line, ' '))));
		free(line);
	}
	while((line = get_next_line_nl(fd))) {
		if (!ft_strlen(line))
			continue ;
		parse_animation(s, slices, e, ft_split(line, ' '));
		free(line);
	}
	ft_printf("#%d slices:\n", ft_lstsize(slices));
	ft_lstiter(slices, print_parsed_slices);
	ft_printf("#%d animations:\t[m] = mirrored\n", ft_lstsize(s->animations));
	ft_lstiter(s->animations, print_animations);
	ft_lstclear(&slices, free_parsed_slice);
	free(slices);
	return (s);
}