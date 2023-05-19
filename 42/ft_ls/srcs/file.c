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

static char * getFileTypeColor(enum fileType fileType) {
	switch (fileType) {
		case Directory:  return (BLUE);
		case RegularFile:  return (WHITE);
		case NamedPipe:  return (YELLOW);
		case CharDeviceFile:  return (YELLOW);
		case LocalSocketFile:  return (PURPLE);
		case BlockDeviceFile: return (YELLOW);
		case SymbolicLink:  return (CYAN);
		default:       return (WHITE);
	}
}

void print_files(void * ptr1, void * ptr2) {
	t_file * file;
	t_conf * conf;

	file = (t_file *)ptr1;
	conf = (t_conf *)ptr2;
	if (!file || file->fileType == Directory) {
		return ;
	}
	if (file->fileType == Unkown) {
		ft_printf("File not found\n");
		return;
	}
	if (conf->format == LongFormat) {
		// Inode   | block size | permissions | #links | owner        | group | size (MB) | last modified  | name | -> link?
		
		// INODE
		if (conf->print_inode)
			ft_printf("%d ", file->stat.st_ino);
		// BLOCK SIZE
		if (conf->print_block_size)
			ft_printf("%d ", file->stat.st_blksize / 1000);
		// PERMISSIONS & #links
		ft_printf("%c%c%c%c%c%c%c%c%c%c %d ",
			file->fileType,
			file->stat.st_mode & S_IRUSR ? 'r' : '-',
			file->stat.st_mode & S_IWUSR ? 'w' : '-',
			file->stat.st_mode & S_IXUSR ? 'x' : '-',
			file->stat.st_mode & S_IRGRP ? 'r' : '-',
			file->stat.st_mode & S_IWGRP ? 'w' : '-',
			file->stat.st_mode & S_IXGRP ? 'x' : '-',
			file->stat.st_mode & S_IROTH ? 'r' : '-',
			file->stat.st_mode & S_IWOTH ? 'w' : '-',
			file->stat.st_mode & S_IXOTH ? 'x' : '-',
			file->stat.st_nlink);
		// OWNER
		if (conf->print_owner)
			ft_putstr_fd(getpwuid(file->stat.st_uid)->pw_name, 1);
		// GROUP
		if (conf->print_group) {
			ft_putstr_fd(" ", 1);
			ft_putstr_fd(getgrgid(file->stat.st_gid)->gr_name, 1);
		}
		// AUTOR
		if (conf->print_author) {
			ft_putstr_fd(" ", 1);
			ft_putstr_fd(getpwuid(file->stat.st_uid)->pw_name, 1);
		}
		// FILE SIZE % TIME
		ft_printf("%4 d %s ",
			file->stat.st_size,
			my_ctime(&(file->stat.st_mtime)));
		// FILE NAME & FILE LINK NAME?
		ft_printf("%s%s%s %s%s %s%s%s\n",
			getFileTypeColor(file->fileType),
			BOLD,
			file->name,
			WHITE,
			file->link_name ? "->" : "",
			getFileTypeColor(file->linkFileType),
			file->link_name ? file->link_name : "",
			ENDC);
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

  /* File information which contains the group, password, permissions time; etc */
	full_name = ft_strjoin(ft_strdup(path), name);

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
	if (((file->stat.st_mode) & S_IFMT) == S_IFLNK) {
		file->fileType = SymbolicLink;
		file->link_name = ft_calloc(sizeof(char), 1028);
		if (readlink(full_name, file->link_name,1028) == -1)
			perror("Couldn't open Link");
		stat(file->link_name, &tmp_stat);
  	file->linkFileType = getFileType(&tmp_stat);
	} else {
	  file->link_name = NULL;
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