#include "libft.h"
#include "ft_ls.h"
#include <sys/stat.h>

static enum fileType getFileType(struct stat * st) {
	if (!st)
		return Unkown;
	switch (st->st_mode & S_IFMT) {
		case S_IFREG:  return (RegularFile);
		case S_IFDIR:  return (Directory);
		case S_IFIFO:  return (NamedPipe);
		case S_IFCHR:  return (CharDeviceFile);
		case S_IFBLK:  return (BlockDeviceFile);
		case S_IFSOCK: return (LocalSocketFile);
		case S_IFLNK:  return (SymbolicLink);
		default:       return (Unkown);
	}
}

static char * getFileTypeColor(enum fileType fileType, t_conf * conf) {
	if (!conf->print_with_color)
		return ("");
	switch (fileType) {
		case Directory:				return (BLUEBOLD);
		case RegularFile:			return ("");
		case NamedPipe:				return (YELLOW);
		case CharDeviceFile:	return (YELLOWBOLD);
		case LocalSocketFile:	return (PURPLEBOLD);
		case BlockDeviceFile:	return (YELLOWBOLD);
		case SymbolicLink:		return (CYANBOLD);
		default:							return (WHITE);
	}
}

void print_files(void * ptr1, void * ptr2) {
	t_file * file;
	t_conf * conf;

	file = (t_file *)ptr1;
	conf = (t_conf *)ptr2;
	// HERE IS MISSING, If it's a param file, dont print dirs, else print dirs.
	if (!file || ((file->fileType == Directory) && !conf->no_explore && conf->params_on)) {
		return ;
	}
	if (file->fileType == Unkown) {
		ft_printf("File not found\n");
		return;
	}
	if (conf->format == LongFormat) {
		// Inode   | block size | permissions | #links | owner        | group | size (MB) | last modified  | name | -> link?
		char * format_bl_size;
		char * format_inode;
		char * format_owner;
		char * format_group;
		char * format_author;
		char * format_size;
		char * format_file_links;

		format_owner = format_padding('s', conf->padding.owner_width, false, true);
		format_group = format_padding('s', conf->padding.group_width, false, true);
		format_author = format_padding('s', conf->padding.author_width, false, true);
		format_size = format_padding('d', conf->padding.file_size_width, true, false);
		format_inode = format_padding('d', conf->padding.inode_width, false, false);
		format_file_links = format_padding('d', conf->padding.file_links_width, false, false);
		// INODE
		if (conf->print_inode) {
			ft_printf(format_inode, file->stat.st_ino);
			ft_putstr_fd(" ", 1);
		}
		// BLOCK SIZE
		//st_blksisze comes in 512 byte units
		// by default it should be printed by 1024 blocks
		if (conf->print_block_size) {
			format_bl_size = format_padding('d', conf->padding.block_size_width, false, false);
			ft_printf(format_bl_size, file->stat.st_blocks / 2);
			ft_putstr_fd(" ", 1);
		}
		// PERMISSIONS & #links
		ft_printf("%c%c%c%c%c%c%c%c%c%c",
			file->fileType,
			file->stat.st_mode & S_IRUSR ? 'r' : '-',
			file->stat.st_mode & S_IWUSR ? 'w' : '-',
			file->stat.st_mode & S_IXUSR ? 'x' : '-',
			file->stat.st_mode & S_IRGRP ? 'r' : '-',
			file->stat.st_mode & S_IWGRP ? 'w' : '-',
			file->stat.st_mode & S_IXGRP ? 'x' : '-',
			file->stat.st_mode & S_IROTH ? 'r' : '-',
			file->stat.st_mode & S_IWOTH ? 'w' : '-',
			file->stat.st_mode & S_IXOTH ? 'x' : '-');
		ft_putstr_fd(" ", 1); // SEPARATOR
		ft_printf(format_file_links, file->stat.st_nlink);
		ft_putstr_fd(" ", 1); // SEPARATOR
		// OWNER
		if (conf->print_owner)
			ft_printf(format_owner,
				getpwuid(file->stat.st_uid)->pw_name);
		// GROUP
		if (conf->print_group) {
			ft_putstr_fd(" ", 1);
			ft_printf(format_group, getgrgid(file->stat.st_gid)->gr_name);
		}
		// AUTOR
		if (conf->print_author) {
			ft_putstr_fd(" ", 1);
			ft_printf(format_author, getpwuid(file->stat.st_uid)->pw_name);
		}
		// FILE SIZE
		ft_printf(format_size, file->stat.st_size);
		ft_putstr_fd(" ", 1);
		// FILE TIME LAST MODIFICATION
		ft_printf("%s ", my_ctime(&(file->stat.st_mtime)));
		// FILE NAME & FILE LINK NAME?
		ft_printf("%s%s%s%s%s%s\n",
			getFileTypeColor(file->fileType, conf),
			file->name,
			file->link_name ? " -> " : "",
			getFileTypeColor(file->linkFileType, conf),
			file->link_name ? file->link_name : "",
			conf->print_with_color ? ENDC : "");
		free(format_owner);
		free(format_group);
		free(format_author);
		free(format_size);
		free(format_inode);
	}
}

t_file	* setup_file(char * name, char * path) {
	struct stat tmp_stat;
	t_file	* file;
	char	* full_name;
	
	file = (t_file *)malloc(sizeof(t_file));
	if (!file)
		return (NULL);
	file->name = ft_strdup(name);
	file->path = ft_strdup(path);
	if (ft_strlen(file->path) && path[ft_strlen(file->path) - 1] != '/') {
		file->path = ft_strjoin(file->path, "/");
	}
  /* File information which contains the group, password, permissions time; etc */
	full_name = ft_strjoin(ft_strdup(file->path), name);
	ft_bzero(&file->stat, sizeof(struct stat));

	if (!full_name) {
		free(file->name);
		free(file->path);
		return NULL;
	}

	if (lstat(full_name, &file->stat) == -1)
		file->fileType = Unkown;
	else
  	file->fileType = getFileType(&file->stat);

  /* When the file is sym link, the name of the file which is pointed else NULL */
	file->link_name = NULL;
	file->linkFileType = Unkown;
	if (((file->stat.st_mode) & S_IFMT) == S_IFLNK) {
		file->fileType = SymbolicLink;
		file->link_name = ft_calloc(sizeof(char), 1028);
		if (readlink(full_name, file->link_name,1028) == -1)
			perror("Couldn't open Link");
		stat(file->link_name, &tmp_stat);
  	file->linkFileType = getFileType(&tmp_stat);
	}
	free(full_name);
  return (file);
}

void	free_file(void * ptr) {
	t_file * file;

	file = (t_file *)ptr;
	if (!file)
		return ;
	free(file->name);
	free(file->path);
	free(file->link_name);
	free(file);
}