#include "ft_ls.h"

void free_file(void * ptr) {
  t_file * file;

	file = (t_file *)ptr;
	if (file->children)
		ft_lstclear(&file->children, free_file);
	if (file->f_type == Symbolic_Link)
		free_file(file->f_link_file);
	free(file->f_name);
	free(file->f_path);
	free(ptr);
}

static void print_children_files(void * ptr1, void * ptr2) {
	t_file * file;
	ls_flags * flags;
	char	* file_time;

	file = (t_file *)ptr1;
	flags = (ls_flags *)ptr2;
	if (!file || !flags)
		ft_exit(1, "Bad Error invalid (void *) cast\n", 0);
	if (file->f_errors == NotFounded)
		ft_printf("ls: cannot access '%s': No such file or directory\n", file->f_name);
//	file_time = NULL;
	if (file->f_errors != NotFounded) {
		if (flags->flagl) {
			file_time = get_file_time(file, flags);
			ft_printf(""
				"%.0d"/*inode number [only print when -i is present]*/
				"%.0d"/*file sistem allocation space [only print when -s is present]*/
				"%c"/*file type*/
				"%c"/*file mode bit-1*/
				"%c"/*file mode bit-2*/
				"%c"/*file mode bit-3*/
				"%c"/*file mode bit-4*/
				"%c"/*file mode bit-5*/
				"%c"/*file mode bit-6*/
				"%c"/*file mode bit-7*/
				"%c"/*file mode bit-8*/
				"%c"/*file mode bit-9*/
				" %d"/*number of hard links*/
				" %s"/*owner name [-g should omit owner information] [-n print right justified id numbers instead of names]*/
				" %s"/*group name [-G should omit the group display	]*/
				"  %10d"/*size, power of 1024 but could be changed to power of 1000 with flag --si*/
				" %s"/*timestamp [default last time modification]*/
				" %s"/*file name*/
				"%s"/*" -> " when soft link*/
				"%s"/*soft link pointed file name*/
				"\n"
				, 0/*inode number*/
				, 0/*file sistem allocation space*/
				, file->f_type/*file type*/
				, file->f_stat.st_mode & S_IRUSR ? 'r' : '-'/*file mode bit-1*/
				, file->f_stat.st_mode & S_IWUSR ? 'w' : '-' /*file mode bit-2*/
				, file->f_stat.st_mode & S_IXUSR ? 'x' : '-' /*file mode bit-3*/
				, file->f_stat.st_mode & S_IRGRP ? 'r' : '-' /*file mode bit-4*/
				, file->f_stat.st_mode & S_IWGRP ? 'w' : '-' /*file mode bit-5*/
				, file->f_stat.st_mode & S_IXGRP ? 'x' : '-' /*file mode bit-6*/
				, file->f_stat.st_mode & S_IROTH ? 'r' : '-' /*file mode bit-7*/
				, file->f_stat.st_mode & S_IWOTH ? 'w' : '-' /*file mode bit-8*/
				, file->f_stat.st_mode & S_IXOTH ? 'x' : '-' /*file mode bit-9*/
				, file->f_stat.st_nlink/*number of hard links*/
				, getpwuid(file->f_stat.st_uid)->pw_name/*owner name*/
				, getgrgid(file->f_stat.st_gid)->gr_name/*group name*/
				, file->f_stat.st_size/*size*/
				, file_time/*timestamp*/
				, file->f_name/*file name*/
				, (file->f_type == Symbolic_Link) ? " -> " : ""/*" -> " when soft link*/
				, (file->f_type == Symbolic_Link) ? file->f_link_file->f_name : ""/*soft link pointed file name*/
				);
			free(file_time);
		} else {
			ft_printf ("%s  ", file->f_name);
		}
	}
}

void	ft_print_directory_files(void * ptr1, void * ptr2) {
	t_file * file;
	ls_flags * flags;

	file = (t_file *)ptr1;
	flags = (ls_flags *)ptr2;
	if (!file || !flags)
		ft_exit(1, "Bad Error invalid (void *) cast\n", 0);
	if (file->f_type != Directory)
		return ;
	if(file->f_errors == PermissionDenied) {
		ft_printf("ls: cannot open directory '%s%s': Permission denied\n", file->f_path == NULL ? "" : file->f_path, file->f_name);
	} else {
		printf("%s:\ntotal %d\n", file->f_name, 1000);
		ft_lstiter_param(file->children, print_children_files, flags);
	}
	if (file->children && flags->flagR)
		ft_lstiter_param(file->children, ft_print_directory_files, flags);
	printf("\n");
}

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
					//if (file->f_type == Symbolic_Link)
					//	ft_printf("%s -> %s  ", file->f_name, file->f_link_file->f_name);
					//else
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

