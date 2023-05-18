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
		char * time_str = my_ctime(&(file->stat.st_mtime));
		time_str[ft_strlen(time_str) - 1] = 0;
		// Inode   | block size | permissions | #links | owner        | group | size (MB) | last modified  | name | -> link?
		ft_printf("%d %d %c%c%c%c%c%c%c%c%c%c %d %s %s% 4d %s %s %s %s\n",
			file->stat.st_ino,
			file->stat.st_blksize / 1000,
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
			file->stat.st_nlink,
			getgrgid(file->stat.st_gid)->gr_name,
			getpwuid(file->stat.st_uid)->pw_name,
			file->stat.st_size,
			time_str,
			file->name,
			file->link_name ? "->" : "",
			file->link_name ? file->link_name : ""
			);
	}
}

t_file	* setup_file(char * name, char * path) {
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
	//char  * link_name;
	if (((file->stat.st_mode) & S_IFMT) == S_IFLNK) {
		file->link_name = ft_calloc(sizeof(char), 1028);
		if (readlink(full_name, file->link_name,1028) == -1)
			perror("Couldn't open Link");
		ft_printf("LINK DETECTED %s -> %s\n", file->name, file->link_name);
		file->linkFileType = file->fileType;
		file->fileType = SymbolicLink;
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