#include "ft_ls.h"
#include <stdint.h>
#include <errno.h>
#include <sys/sysmacros.h>

static void	set_file_type(t_file * file, ls_flags * flags) {
	int result;
	char * full_path;
	char * link_file_name;

	if (!file || !flags)
		return ;
	full_path = ft_strjoin(ft_strdup((file->f_path != NULL) ? file->f_path : ""), file->f_name);
	if (!full_path)
			ft_exit(1, "Failed to allocate memory\n", 0);
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
	switch (file->f_stat.st_mode & S_IFMT) {
	case S_IFBLK: file->f_type = Block_Device_File;		break;
	case S_IFCHR: file->f_type = Character_Device_File;	break;
	case S_IFDIR: file->f_type = Directory;				break;
	case S_IFIFO: file->f_type = Named_Pipe;			break;
	case S_IFLNK: file->f_type = Symbolic_Link;			break;
	case S_IFREG: file->f_type = Regular_File;			break;
	case S_IFSOCK: file->f_type = Local_Socket_File;	break;
	default: file->f_type = Unknown;				break;
	}
	if (file->f_type == Symbolic_Link) {
		//ft_printf("Symlink %s\n", full_path);
		link_file_name = (char *)ft_calloc(sizeof(char), 4096);
		if (readlink(full_path, link_file_name, 4096) == -1)
			ft_exit(1, "Link Buf biggger than allowed\n", 0);	
		file->f_link_file = init_file(link_file_name, file->f_path);
		if (!file->f_link_file)
			ft_exit(1, "Failed to allocate memory\n", 0);
		set_file_type(file->f_link_file, flags);
		free(link_file_name);
	}

	if (DEBUG)
		ft_printf("[%s]\n", full_path);
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
	if (file->f_type == Symbolic_Link) {
		printf("%s -> %s\n", file->f_name, file->f_link_file->f_name);
	}
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
		tmp->d_ino = ent->d_ino;
		set_file_type(tmp, flags); 	
		// I can sort the files here!!!
		ft_lstadd_front_cmp(&parent->children, ft_lstnew(tmp), cmp_ascii_order);
	}
	free(full_path);
}

static void setup_file_directory(t_file * file, ls_flags * flags);

static void setup_file_directory_recursive(void * ptr1, void * ptr2) {
	t_file * file;
	ls_flags * flags;

	file = (t_file *)ptr1;
	flags = (ls_flags *)ptr2;
	if (file && flags
		&& ft_strncmp(file->f_name, "..", ft_max(ft_strlen(file->f_name), ft_strlen("..")))
		&& ft_strncmp(file->f_name, ".", ft_max(ft_strlen(file->f_name), ft_strlen(".")))
		&& (file->f_stat.st_mode & S_IFMT) == S_IFDIR)
		setup_file_directory(file, flags);
}

static void setup_file_directory(t_file * file, ls_flags * flags) {
	char *	full_path;
	DIR * dir;

	if (!file || !flags)
		return ;
	full_path = ft_strjoin(ft_strdup((file->f_path != NULL) ? file->f_path : ""), file->f_name);
	if (!full_path)
		ft_exit(1, "Failed to allocate memory\n", 0);
	dir = opendir(full_path);
	free(full_path);
	if (!dir) {
		file->f_errors = PermissionDenied;
		return ;
	}
	setup_directory_children(file, dir, flags);
	closedir(dir);
	if (flags->flagR && file->children)
		ft_lstiter_param(file->children, setup_file_directory_recursive, flags);
}

void	setup_file(void * ptr1, void * ptr2) {
	t_file * file;
	ls_flags * flags;

	file = (t_file *)ptr1;
	flags = (ls_flags *)ptr2;
	if (!file || !flags)
		ft_exit(1, "Bad Error invalid (void *) cast\n", 0);
	set_file_type(file, flags);
	if (file->f_errors == NoError && (file->f_stat.st_mode & S_IFMT) == S_IFDIR && !flags->flagD) // AND FLAG D NOT PRESENT
		setup_file_directory(file, flags);
}
