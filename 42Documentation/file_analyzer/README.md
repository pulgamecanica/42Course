## Something about files...

***

#### > File Properties

	ID of containing device: This field describes the device(1) on which this file resides.
	File type: Describes the type of the file, directory, sym link, regular file; etc.
	I-node number: The inode(2) number referes to an identifier the Linux Kernel uses to locate the phisical file in the filesystem.
	Mode: Referes to the mode of the file and also the type
	Link count: How many references through hard, or soft links exists for this file
	Ownership: Who owns the file
	Preferred I/O block size:
	File size: The total bytes this file has
	Blocks allocated: The space this file ocupies in memory to be stored (not the content of the file)
	Last status change: When last permissions were changed on the file
	Last file access: When last the file was opened read or executed
	Last file modification: When last the file changed modification.

***

### Group, Author and Ownership




***

(1) devise: In the Linux Kernel a device is: A Special File which helps interact with an actual driver (Hardware piece on your Computer) usually through an interface.
Devise id's consist in two parts, major and minor, identifying the class of the device and the specific instance of the device respectively.
[man 3 minor, man 3 major, man 7 attributes, man 2 mknod]
Classification of the devises helps programmers identify the typeo of the devise file. The three classes are:

	- Character device
	- Block device
	- Network device

A character device can be accesses as a stream of bytes (like a file), usually to open, close, read and write system calls. The text console
Block devices are accessesd by filesystem nodes in the /dev directory. It can handle I/O operations that trasnfer one or more whole blockks, which are usually 512 bytes.
Network devices helps the computer exchange data with other hosts, throught an interface, usually the interface is a hardware device the network is in charge of receiving and sending packets usung protocoles usually stream oriented.

(2) inode: Each file has an inode containing metadata about the file. An application can retrieve this metadata using stat(2), this might help you find out:

	- Device where inode resides
	- Inode number
	- File type
	- File mode
	- Link count
	- User ID
	- Group ID
	- Devise represented by this inode
	- File size (stat.st_size; statx.stx_size)
	- Prefered Block (stat.st_blksize; statx.stx_blksize)
	- Number of blocks allocated to the file (stat.st_blocks; statx.stx_size)
	- Last access timestamp (stat.st_atime; statx.stx_atime)
	- File creation timestamp (statx.stx_btime) [! Not supported anymore by most Linux filesystems]
	- Last modification timestamp (stat.st_mtime; statx.stx_mtime)
	- Last status change timestamp (stat.st_ctime; statx.stx_ctime)

To find out the inode mode you can use the following masks:
```sh
S_IFSOCK   0140000   socket
S_IFLNK    0120000   symbolic link
S_IFREG    0100000   regular file
S_IFBLK    0060000   block device
S_IFDIR    0040000   directory
S_IFCHR    0020000   character device
S_IFIFO    0010000   FIFO
```

There are also more concise MACROS: 
```sh
S_ISREG(m)  is it a regular file?
S_ISDIR(m)  directory?
S_ISCHR(m)  character device?
S_ISBLK(m)  block device?
S_ISFIFO(m) FIFO (named pipe)?
S_ISLNK(m)  symbolic link?  (Not in POSIX.1-1996.)
S_ISSOCK(m) socket?  (Not in POSIX.1-1996.)
```

To find out the mode you should use the following masks:
```sh
S_ISUID     04000   set-user-ID bit (see execve(2))
S_ISGID     02000   set-group-ID bit (see below)
S_ISVTX     01000   sticky bit (see below)

S_IRWXU     00700   owner has read, write, and execute permission
S_IRUSR     00400   owner has read permission
S_IWUSR     00200   owner has write permission
S_IXUSR     00100   owner has execute permission

S_IRWXG     00070   group has read, write, and execute permission
S_IRGRP     00040   group has read permission
S_IWGRP     00020   group has write permission
S_IXGRP     00010   group has execute permission

S_IRWXO     00007   others (not in group) have read, write, and execute permission
S_IROTH     00004   others have read permission
S_IWOTH     00002   others have write permission
S_IXOTH     00001   others have execute permission
```
[man 7 inode]
***


