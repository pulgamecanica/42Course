## FT_LS
This project is a replica of the command ls.

Mandatory flags [OPTION]:
> - -l : use a long listing format
> - -R, --recursive : list subdirectories recursively
> - -a, --all : do not ignore entries starting with .
> - -r --reverse : reverse order while sorting
> - -t : sort by time, newest first; see --time

### Definition
> ```ls [OPTION]... [FILE]...```

	Although it may appear that the options must be before the [FILE]... in actuallity, the flags can be anywhere in between.
	The ls program lists information about files (of any type, including directories).
	Options and file arguments can be intermixed arbitrarily, as usual. Later options override earlier options that are incompatible.

	For non-option command-line arguments that are directories, by default ls lists the contents of directories, not recursively, and omitting files with names beginning with ‘.’.
	For other non-option arguments, by default ls lists just the file name.
	If no non-option argument is specified, ls operates on the current directory, acting as if it had been invoked with a single argument of ‘.’. 

The documentation describes the behaviour by listing how the [OPTION]... might affect teh output.
I.e, how flags can affect the output, and explain the different types of flags.
> + Which files are listed
> + What information is listed
> + Sorting the output
> + General output formatting
> + Formatting file timestamps	  	
> + Formatting the file names

***

#### Which files are listed

	These options determine which files ls lists information for.
	By default, ls lists files and the contents of any directories on the command line, except that in directories it ignores files whose names start with ‘.’. 

| Flag(s)  | Description | Priority |
| ------------- | ------------- | ------------- |
| -a --all  | In directories, do not ignore file names that start with ‘.’ | always |
| -A --almost-all | In directories, do not ignore all file names that start with ‘.’; ignore only . and .. | if ! -a |
| -B --ignore-backups | In directories, ignore files that end with ‘~’. This option is equivalent to ```--ignore='*~' --ignore='.*~'``` | always |
| -d --derectory | List just the names of directories, as with other types of files, rather than listing their contents | always |
| -R --recursive | List the contents of all directories recursively.  | if ! -d |
| -L --dereference | When showing file information for a symbolic link, show information for the file the link references rather than the link itself. However, even with this option, ls still prints the name of the link itself, not the name of the file that the link points to | always |
| --group-directories-first | Group all the directories before the files and then sort the directories and the files separately using the selected sort key (see –sort option). That is, this option specifies a primary sort key, and the –sort option specifies a secondary key. | if ! --sort=none (-U) |

***

#### What information is listed

	By default, only file names are shown.

| Flag(s)  | Description | Priority |
| ------------- | ------------- | ------------- |
| -D --dired | Print an additional line after the main output: ```//DIRED// beg1 end1 beg2 end2 …``` The begn and endn are unsigned integers that record the byte position of the beginning and end of each file name in the output. This makes it easy for Emacs to find the names, even when they contain unusual characters such as space or newline, without fancy searching. If directories are being listed recursively (or more than one file is given), output a similar line with offsets for each subdirectory name: ```//SUBDIRED// beg1 end1 …``` Finally, output a line of the form: ```//DIRED-OPTIONS// --quoting-style=word``` where word is the quoting style | always |
| --full-time | Produce long format, and list times in full. It is equivalent to using --format=long (-l) with --time-style=full-iso | always, literally simulate the -l and --time-style=full-iso |
| -g | Produce long format, but omit owner information. | always |
| -G --no-group | Inhibit(prevent) display of group information in long format. (This is the default in some non-GNU versions of ls, so we provide this option for compatibility.)  | always |
| -h --human-readable | Append a size letter to each size, such as ‘M’ for mebibytes. Powers of 1024 are used, not 1000; ‘M’ stands for 1,048,576 bytes. This option is equivalent to --block-size=human-readable. Use the --si option if you prefer powers of 1000.  | always and --si can overwrite powers |
| -i --inode | Print the inode number (also called the file serial number and index number) of each file to the left of the file name. (This number uniquely identifies each file within a particular file system.) | always |
| -l --format=long --format=verbose | Produce long format (See description bellow) | always |
| -n --numeric-uid-gid | Produce long format, but display right-justified numeric user and group IDs instead of left-justified owner and group names | always |
| -o | Produce long format, but omit group information. It is equivalent to using --format=long (-l) with --no-group (-G) | always |
| -s --size | Print the file system allocation of each file to the left of the file name. This is the amount of file system space used by the file | always |
| --si | Append an SI-style abbreviation to each size, such as ‘M’ for megabytes. Powers of 1000 are used, not 1024; ‘M’ stands for 1,000,000 bytes. This option is equivalent to --block-size=si. Use the -h or --human-readable option if you prefer powers of 1024 | always |

	Long Format Description:

	In addition to the name of each file, print the file type, file mode bits, number of hard links, owner name, group name, size, and timestamp, normally the modification timestamp.
	If the owner or group name cannot be determined, print the owner or group ID instead, right-justified as a cue that it is a number rather than a textual name. Print question marks for other information that cannot be determined. 
	Normally the size is printed as a byte count without punctuation, but this can be overridden.
	For example, --human-readable (-h) prints an abbreviated, human-readable count, and ```--block-size="'1"``` prints a byte count with the thousands separator of the current locale.
	For each directory that is listed, preface the files with a line ‘total blocks’, where blocks is the file system allocation for all files in that directory. The block size currently defaults to 1024 bytes, but this can be overridden. The blocks computed counts each hard link separately; this is arguably a deficiency. 
	
***
| | | |


#### Sorting the output
	By default, the output is sorted alphabetically, according to the locale settings in effect.4
	If standard output is a terminal, the output is in columns (sorted vertically).
	Control characters are output as question marks; otherwise, the output is listed one per line and control characters are output as-is.
	
	(4)
	If you use a non-POSIX locale (e.g., by setting LC_ALL to ‘en_US’), then ls may produce output that is sorted differently than you’re accustomed to.
	In that case, set the LC_ALL environment variable to ‘C’.
***
#### Exit status

> - 0 success
> - 1 minor problems  (e.g., failure to access a file or directory not
> specified as a command line argument.  This happens when listing a
> directory in which entries are actively being removed or renamed.)
> - 2 serious trouble (e.g., memory exhausted, invalid option, failure
> to access a file or directory specified as a command line argument
> or a directory loop)
***

#### General output formatting

***

#### Formatting file timestamps

***

#### Formatting the file names

***

### Behaviour analysis

```bash
	ls
```

	ls will generate a file listing using the current location as root (default),
	it will only display files which are children of the current root.

```bash
	ls [FILE]...
```
	When a file is provided there will be no default location and instead each listing will be the file provided.
	When the user provides a directory all the children of the directory will be listed, excluding the directory.
	If more one or more directories are provided, the listing will have this header: directory:\n	
  	With the exeption of the directory was the only argument provided.
	EX: ls dir1 dir2
		dir1/:
		file1.txt

		dir2/:
		file9.txt
 
	
***

#### [OPTION] -l

```bash
	ls -l
```


### Sources
```man ls```

[oficial documentation](https://www.gnu.org/software/coreutils/manual/html_node/ls-invocation.html#ls-invocation)
