#include "file.h"
#include "ft_printf.h"

void	free_file(void * ptr) {
	t_file * file;

	file = (t_file *)ptr;
	if (!file) {
		return ;
	}
	free(file->name);
	free(file->path);
	free(file->link_name);
	free(file);
}

static enum file_indicator_type indicator_type(mode_t mode) {
	switch (mode & S_IFMT) {
		case S_IFREG: return (regular_file);
		case S_IFDIR: return (directory);
		case S_IFIFO: return (named_pipe);
		case S_IFCHR: return (char_device_file);
		case S_IFBLK: return (block_device_fille);
		case S_IFSOCK: return (socket_file);
		case S_IFLNK: return (symbolic_link);
		default: return (missing_file);
	}
}

t_file	*	new_file(char * name, char * path) {
	t_file * file;
	char * full_path;

	file = (t_file *)ft_calloc(sizeof(t_file), 1);
	if (!file)
		return (NULL);

	file->name = ft_strdup(name);
	if (!path)
		file->path = NULL;
	else if (*(path + ft_strlen(path) - 1) == '/')
		file->path = ft_strdup(path);
	else
		file->path = ft_strjoin(ft_strdup(path), "/");
	file->link_name = NULL;

	full_path = ft_strjoin(ft_strdup(file->path ? file->path : ""), name);
	if ((lstat(full_path, &file->stat)) == -1) {
		ft_printf("ls: cannot access '%s': No such file or directory\n", full_path);
		free(full_path);
		free(file->name);
		free(file->path);
		free(file);
		return (NULL);
	}
	free(full_path);

	file->fit = indicator_type(file->stat.st_mode);

	return (file);
}