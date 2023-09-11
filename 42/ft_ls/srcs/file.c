#include "file.h"
#include "ft_printf.h"
#include <sys/xattr.h>

void	free_file(void * ptr) {
	t_file * file;

	file = (t_file *)ptr;
	if (!file) {
		return ;
	}
	free(file->name);
	free(file->path);
	free(file->link_name);
	free(file->scontext);
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
	file->scontext = NULL;
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

// man chattr(1)
// man setfacl(1)
// man getfattr(1)
// man chcon(1)
// man getxattr(2)
// man listxattr(2)
void setup_scontext(void * ptr) {
	t_file * f;
	ssize_t buflen, keylen, vallen;
	char *buf, *key, *val;

	key = NULL;
	if (!(f = (t_file *)ptr))
		return ;
   // Determine the length of the buffer needed.
	buflen = listxattr(f->name, NULL, 0);
	if (buflen == 0 || buflen == -1)
		return ;
	buf = (char *)malloc(buflen);
	if (!buf)
		return ;
   // Copy the list of attribute keys to the buffer.
   buflen = listxattr(f->name, buf, buflen);
   if (buflen == -1)
		return ;
	/*
	* Loop over the list of zero terminated strings with the
	* attribute keys. Use the remaining buffer length to determine
	* the end of the list.
	*/
	key = buf;
	while (buflen > 0) {
		/* Determine length of the value. */
		vallen = getxattr(f->name, key, NULL, 0);
		if (vallen == -1 || vallen <= 0)
		   perror("getxattr");
		if (vallen > 0) {
			if (!(val = (char *)malloc(vallen + 1)))
				return ;
			vallen = getxattr(f->name, key, val, vallen);
			if (vallen != -1) {
				val[vallen] = 0;
				f->scontext = ft_strjoin(f->scontext, val);
			}
			free(val);
		}
		// Forward to next attribute key.
		keylen = ft_strlen(key) + 1;
		buflen -= keylen;
		key += keylen;
	}
	free(buf);
}
