#include "libft.h"
#include "ft_ls.h"
#include <sys/stat.h>

static enum fileType getFileType(struct stat * st) {
	if (!st)
		return Unkown;
	switch (st->st_mode & S_IFMT) {
		case S_IFLNK:  return (SymbolicLink);
		case S_IFREG:  return (RegularFile);
		case S_IFDIR:  return (Directory);
		case S_IFIFO:  return (NamedPipe);
		case S_IFCHR:  return (CharDeviceFile);
		case S_IFBLK:  return (BlockDeviceFile);
		case S_IFSOCK: return (LocalSocketFile);
		default:       return (Unkown);
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

	lstat(full_name, &file->stat);
  file->fileType = getFileType(&file->stat);

	free(full_name);



  /* When the file is sym link, the name of the file which is pointed else NULL */
	//char  * link_name;
  file->link_name = NULL;
  
  /* The type of the file */
  
  /* The lt_mode of the file linked */
  //mode_t  link_mode;
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