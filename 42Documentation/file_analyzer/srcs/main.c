#include "libft.h"
#include "file_analyser.h"
/**
 * What to check for a file?
 * File Exists?
 * |  | (NO)
 * |  L (SHOW ERROR)
 * | 
 * | (YES)
 * |
 * L IS SYMLINK?
 *   |  | (YES)
 *   |  L 
 *   |
 *   | (NO)
 *   L Size
 *     Name
 *     Path
 *     HardLinks
 *     Total
 *     Owner
 *     Group
 *     Data
 *     Permissions
 **/

void file_analyser(char *path_to_file) {
	struct stat sb;

	if (lstat(path_to_file, &sb) == -1) {
	   perror("lstat");
	   return ;
	}

	/**
	 * Print the id of the containing device
	 * Devices are interfaces which help you interact with hardware more less
	 * There are also virtual devices such as /dev/urandom 
	 **/
	printf("ID of containing device:  [%jx,%jx]\n",
	       (uintmax_t) major(sb.st_dev),
	       (uintmax_t) minor(sb.st_dev));

	/**
	 * In Linux there is a philosophu that everything is a file
	 * that is why there exists some certain files that seam strange
	 * like block device or character device, this files are not
	 * your typical kind of files, this files represent a device from
	 * your computer, like a driver, whcich helps you interact with the
	 * computer thorugh an interface.
	 **/
	printf("File type:                ");

	switch (sb.st_mode & S_IFMT) {
	case S_IFBLK:  printf("block device\n");            break;
	case S_IFCHR:  printf("character device\n");        break;
	case S_IFDIR:  printf("directory\n");               break;
	case S_IFIFO:  printf("FIFO/pipe\n");               break;
	case S_IFLNK:  printf("symlink\n");                 break;
	case S_IFREG:  printf("regular file\n");            break;
	case S_IFSOCK: printf("socket\n");                  break;
	default:       printf("unknown?\n");                break;
	}

	/**
	 * This is the unique identifier of a file in the computer.
	 * It helps locate files in the phisical memory, so it's
	 * fater to write, read or execute. 
	 **/
	printf("I-node number:            %ju\n", (uintmax_t) sb.st_ino);

	/**
	 * File Permissions
	 **/
	printf("Mode:                     %jo (octal)\n",
	       (uintmax_t) sb.st_mode);

	printf("Owner rights:             %c%c%c (rwx)\n", sb.st_mode & S_IRUSR ? 'r' : '-', sb.st_mode & S_IWUSR ? 'w' : '-', sb.st_mode & S_IXUSR ? 'x' : '-');
	printf("Group rights:             %c%c%c (rwx)\n", sb.st_mode & S_IRGRP ? 'r' : '-', sb.st_mode & S_IWGRP ? 'w' : '-', sb.st_mode & S_IXGRP ? 'x' : '-');
	printf("Others rights:            %c%c%c (rwx)\n", sb.st_mode & S_IROTH ? 'r' : '-', sb.st_mode & S_IWOTH ? 'w' : '-', sb.st_mode & S_IXOTH ? 'x' : '-');

	/**
	 * How many files reference this file thorugh hard or soft links.
	 **/
	printf("Link count:               %ju\n", (uintmax_t) sb.st_nlink);

	printf("Ownership:                UID=%ju   GID=%ju\n",
	       (uintmax_t) sb.st_uid, (uintmax_t) sb.st_gid);

	/**
	* struct passwd {
	*	char   *pw_name;       // username
	*	char   *pw_passwd;     // user password
	*	uid_t   pw_uid;        // user ID
	*	gid_t   pw_gid;        // group ID
	*	char   *pw_gecos;      // user information
	*	char   *pw_dir;        // home directory
	*	char   *pw_shell;      // shell program
	* };
	**/
	struct passwd * file_psw;
	/**
	* struct group {
	*	char   *gr_name;        // group name
	*	char   *gr_passwd;      // group password
	*	gid_t   gr_gid;         // group ID
	*	char  **gr_mem;         // NULL-terminated array of pointersto names of group members
	* };
	**/
	struct group * file_grp;

	file_psw = getpwuid(sb.st_uid);
	file_grp = getgrgid(sb.st_gid);

	if (file_psw) {
		printf("\nOwner:\n");
		printf("%-10.10s | %-10.10s | %-10.10s | %-10.10s | %-10.10s | %-10.10s | %-10.10s\n", "username", "password", "user id", "group id", "user info", "home dir", "shell prog");
		printf("---------- | ---------- | ---------- | ---------- | ---------- | ---------- | ----------\n");		printf("%-10.10s | %-10.10s | %-10d | %-10d | %-10.10s | %-10.10s | %.10s\n",
			file_psw->pw_name, file_psw->pw_passwd,
			file_psw->pw_uid, file_psw->pw_gid, file_psw->pw_gecos,
			file_psw->pw_dir, file_psw->pw_shell);
	} else {
		perror("getpwuid");
	}
	if (file_grp) {
		printf("\nGroup:\n");
		printf("%-10.10s | %-10.10s | %-10.10s | %-10.10s\n", "group name", "password", "group id", "# members");
		printf("---------- | ---------- | ---------- | ----------\n");
		printf("%-10.10s | %-10.10s | %-10d | %-10d\n",
			file_grp->gr_name, file_grp->gr_passwd, file_grp->gr_gid, ft_arrlen(file_grp->gr_mem));
	} else {
		perror("getgrgid");
	}

	printf("Preferred I/O block size: %jd bytes\n",
	       (intmax_t) sb.st_blksize);
	printf("File size:                %jd bytes\n",
	       (intmax_t) sb.st_size);
	printf("Blocks allocated:         %jd (512B each block) & %jd (1024B each block)\n",
	       (intmax_t) sb.st_blocks, sb.st_blocks / 2);

	printf("Last status change:       %s", ctime(&sb.st_ctime));
	printf("Last file access:         %s", ctime(&sb.st_atime));
	printf("Last file modification:   %s", ctime(&sb.st_mtime));

}

int main(int ac, char *av[]) {
	if (ac <= 1) {
		ft_putendl_fd("[usage]: ./exec path_to_file ...", 2);
		return(0);
	}
	for (int i = 1; i < ac; i++) {
		if (i == 1)
			ft_putendl_fd("************************ / ************************", 1);
		if (ac > 1)
			ft_putendl_fd(av[i], 2);
		file_analyser(av[i]);
		ft_putendl_fd("************************ / ************************", 1);
	}
	return (0);
}
