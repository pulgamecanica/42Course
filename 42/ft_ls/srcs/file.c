#include "ft_ls.h"

/*
int     ft_lstsize(t_list *lst);
void    ft_lstadd_front(t_list **lst, t_list *new);
void    ft_lstadd_back(t_list **lst, t_list *new);
void    ft_lstdelone(t_list *lst, void (*del)(void *));
void    ft_lstdelelem(t_list **lst, t_list *elem, void (*del)(void *));
void    ft_lstclear(t_list **lst, void (*del)(void *));
void    ft_lstiter(t_list *lst, void (*f)(void *));
t_list    *ft_lstnew(void *content);
t_list    *ft_lstlast(t_list *lst);
t_list    *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

typedef struct s_file {
  enum filetype f_errors;
  size_t    f_size;
  size_t    f_create_date;
  size_t    f_last_modify;
  char *    f_name;
  short int f_mode;
  int       total;
  t_list *  children;
} t_file;


typedef struct ls_config {
	ls_flags	flags;
	t_list *	files;
}	ls_file;
*/

void free_file(void * ptr) {
  t_file * file;

  file = (t_file *)ptr;
  if (file->children)
    ft_lstclear(&file->children, free_file);
	free(file->f_name);
	free(file->f_path);
	free(ptr);
}

/*
static void ft_print_file(t_file * file) {
	ft_printf(""
	"type: %d, "
	"path: %-12.12s, "
	"name: %-12.12s, "
	"total: %d, "
	"# children: %d",
	file->f_errors,
	file->f_path,
	file->f_name,
	file->total,
	ft_lstsize(file->children));
}
*/

#include <stdint.h>
#include <errno.h>
#include <sys/sysmacros.h>

static void	set_file_type(t_file * file, ls_flags * flags) {
	int result;
	char * full_path;

	if (!file || !flags)
		return ;
	full_path = ft_strjoin(ft_strdup((file->f_path != NULL) ? file->f_path : ""), file->f_name);
	if (!full_path)
			ft_exit(1, "Failed to allocate memory\n", 0);
	//if (flags->flagL)
	//	lstat();
	//else
	result = lstat(full_path, &file->f_stat);
	if (result == -1) {
		if (EACCES) {
			file->f_errors = NotFounded;
		} else {
			file->f_errors = PermissionDenied;
		}
		if (DEBUG)
			perror("ft_ls");
	}
	if (DEBUG)
		printf("[%s]\n", full_path);
	free(full_path);
	if (!DEBUG)
		return ;
	printf("ID of containing device:  [%jx,%jx]\n",
	       (uintmax_t) major(file->f_stat.st_dev),
	       (uintmax_t) minor(file->f_stat.st_dev));

	printf("File type:                ");

	switch (file->f_stat.st_mode & S_IFMT) {
	case S_IFBLK:  printf("block device\n");            break;
	case S_IFCHR:  printf("character device\n");        break;
	case S_IFDIR:  printf("directory\n");               break;
	case S_IFIFO:  printf("FIFO/pipe\n");               break;
	case S_IFLNK:  printf("symlink\n");                 break;
	case S_IFREG:  printf("regular file\n");            break;
	case S_IFSOCK: printf("socket\n");                  break;
	default:       printf("unknown?\n");                break;
	}

	printf("I-node number:            %ju\n", (uintmax_t) file->f_stat.st_ino);

	printf("Mode:                     %jo (octal)\n",
	       (uintmax_t) file->f_stat.st_mode);

	printf("Link count:               %ju\n", (uintmax_t) file->f_stat.st_nlink);
	printf("Ownership:                UID=%ju   GID=%ju\n",
	       (uintmax_t) file->f_stat.st_uid, (uintmax_t) file->f_stat.st_gid);

	printf("Preferred I/O block size: %jd bytes\n",
	       (intmax_t) file->f_stat.st_blksize);
	printf("File size:                %jd bytes\n",
	       (intmax_t) file->f_stat.st_size);
	printf("Blocks allocated:         %jd\n",
	       (intmax_t) file->f_stat.st_blocks);

	printf("Last status change:       %s", ctime(&file->f_stat.st_ctime));
	printf("Last file access:         %s", ctime(&file->f_stat.st_atime));
	printf("Last file modification:   %s", ctime(&file->f_stat.st_mtime));
	printf("*********************************************************\n");
}

static void	setup_directory_children(t_file * parent, DIR * dir, ls_flags * flags) {
	t_file * tmp;
	struct dirent * ent;
	char * full_path;

	if (!parent || !dir || !flags)
		return ;
	full_path = ft_strjoin(ft_strdup((parent->f_path != NULL) ? parent->f_path : ""), parent->f_name);
	while ((ent = readdir(dir))) {
		if (*ent->d_name == '.' && !flags->flaga)
			continue ;
		if (DEBUG)
			printf("[%s] %s\n\td_ino [%lu]\n\td_off [%lu]\n\td_reclen [%u]\n\td_type [%d]\n", full_path, ent->d_name, ent->d_ino, ent->d_off, ent->d_reclen, ent->d_type);
		tmp = init_file(ent->d_name, full_path);
		if (!tmp) {
			free(full_path);
			return ;
		}
		//tmp->f_size = ent->d_reclen;
		tmp->d_ino = ent->d_ino;
		set_file_type(tmp, flags);
		ft_lstadd_front(&parent->children, ft_lstnew(tmp));
	}
	free(full_path);
}

static void setup_file_directory_recursive(void * ptr1, void * ptr2) {
	t_file * file;
	ls_flags * flags;
	DIR * dir;
	char * full_path;

	file = (t_file *)ptr1;
	flags = (ls_flags *)ptr2;
	if (!file || !flags || !ft_strncmp(file->f_name, "..", ft_max(ft_strlen(file->f_name), ft_strlen("..")))
		|| !ft_strncmp(file->f_name, ".", ft_max(ft_strlen(file->f_name), ft_strlen(".")))
		|| (file->f_stat.st_mode & S_IFMT) != S_IFDIR)
		return ;
	full_path = ft_strjoin(ft_strdup((file->f_path != NULL) ? file->f_path : ""), file->f_name);
	if (!full_path)
		return ;
	dir = opendir(full_path);
	free(full_path);
	if (!dir) {
		file->f_errors = NotFounded;
		if (DEBUG)
			ft_printf("File %s%s not founded :( Or some error\n", (file->f_path == NULL) ? "" : file->f_path, file->f_name);
		return ;
	}
	setup_directory_children(file, dir, flags);
	closedir(dir);
	if (flags->flagR && file->children)
		ft_lstiter_param(file->children, setup_file_directory_recursive, ptr2);
}

void	setup_file(void * ptr1, void * ptr2) {
	t_file * file;
	ls_flags * flags;
	DIR * dir;
	char * full_path;

	file = (t_file *)ptr1;
	flags = (ls_flags *)ptr2;
	if (!file || !flags)
		ft_exit(1, "Bad Error invalid (void *) cast\n", 0);
	set_file_type(file, flags);
	full_path = ft_strjoin(ft_strdup((file->f_path != NULL) ? file->f_path : ""), file->f_name);
	if (file->f_errors == NoError && (file->f_stat.st_mode & S_IFMT) == S_IFDIR) { // AND FLAG D NOT PRESENT
		if (!full_path)
			ft_exit(1, "Failed to allocate memory\n", 0);
		dir = opendir(full_path);
		free(full_path);
		if (!dir) {
			file->f_errors = PermissionDenied;
			if (DEBUG)
				ft_printf("ls: cannot open directory '%s%s': Permission denied\n", (file->f_path != NULL) ? file->f_path : "", file->f_name);
			return ;
		}
		setup_directory_children(file, dir, flags);
		closedir(dir);
		if (flags->flagR && file->children) {
			ft_lstiter_param(file->children, setup_file_directory_recursive, ptr2);
		}
	}
}

//void	setup_files(ls_config * config) {
//	if (!config)
//		return;
//	ft_lstiter(config->files, setup_file);
	// if flag R and childre
	// ft_lstiter(congig., void (*f)(void *));
//}

void	ft_print_files(t_list * head, int depth, bool various) {
	t_file	* file;

	if (head) {
		file = (t_file *)head->content;
		if (!file)
			ft_exit(1, "Bad Error invalid (void *) cast\n", 0);
		// Probably good to check flagD here...
		if (file->f_errors == PermissionDenied) {
			ft_printf("ls: cannot open directory '%s%s': Permission denied\n", (file->f_path != NULL) ? file->f_path : "", file->f_name);
		} else if (file->f_errors == NotFounded) {
			ft_printf("ls: cannot access '%s%s': No such file or directory\n", (file->f_path == NULL) ? "" : file->f_path, file->f_name);
		}
		//ft_print_file(head->content);
		if (file->f_errors == NoError) {
			if ((!depth && (file->f_stat.st_mode & S_IFMT) != S_IFDIR) || depth) {
				if (head->next) {
					ft_printf("%s  ", file->f_name);
				} else {
					ft_printf("%s\n", file->f_name);
				}
			}
		}
		ft_print_files(head->next, depth, various);
		if (file->children && (file->f_stat.st_mode & S_IFMT) == S_IFDIR) {
			if (head->next) {
				ft_printf("\n");
			}
			if ((various || depth) && (!various || !depth))
				ft_printf("%s%s:\n", (file->f_path == NULL) ? "" : file->f_path, file->f_name);
			ft_print_files(file->children, ++depth, various);
		}
	}
}

t_file * init_file(char * str, char * path) {
  t_file *  file;

  file = (t_file *)ft_calloc(sizeof(t_file), 1);
  if (!file)
    return NULL;
	file->f_errors = 0;
  file->f_errors = NoError;
  file->f_name = ft_strdup(str);
  if (!file->f_name) {
    free(file);
    return NULL;
  }
	if (!path)
		file->f_path = NULL;
	else {
		if (path[ft_strlen(path) - 1] == '/')
			file->f_path = ft_strdup(path);
		else
			file->f_path = ft_strjoin(ft_strdup(path), "/");
	}
  file->total = 0;
  file->children = NULL;
  return (file);
}

