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

static void	setup_link(char * path, t_file * file) {
	int bufsiz = 1024;
	char * buf;

	if (!(buf = ft_calloc(sizeof(char), bufsiz)))
		return ;
	if (readlink(path, buf, bufsiz) == -1) {
		perror("Couldn't open link\n");
	} else {
		file->link_name = ft_strdup(buf);
		stat(file->link_name, &file->lstat);
		file->lit = indicator_type(file->lstat.st_mode);
	}
	free(buf);
}

t_file	*	new_file(char * name, char * path) {
	t_file * file;
	char * full_path;

	file = (t_file *)ft_calloc(sizeof(t_file), 1);
	if (!file)
		return (NULL);
	file->name = ft_strdup(name);
	file->link_name = NULL;
	if (!path)
		file->path = NULL;
	else if (*(path + ft_strlen(path) - 1) == '/')
		file->path = ft_strdup(path);
	else
		file->path = ft_strjoin(ft_strdup(path), "/");
	full_path = ft_strjoin(ft_strdup(file->path ? file->path : ""), name);
	if ((lstat(full_path, &file->stat)) == -1) {
		ft_printf("ls: cannot access '%s': No such file or directory\n", full_path);
		free(full_path);
		free_file(file);
		return (NULL);
	}
	file->fit = indicator_type(file->stat.st_mode);
	if (file->fit == SYMBOLIC_LINK)
		setup_link(full_path, file);
	free(full_path);
	return (file);
}
