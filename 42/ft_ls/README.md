## FT_LS
This project is a replica of the command.

The madatory part includes the implmentation of the following flags [OPTION]:
 - -l								=> use a long listing format
 - -R, --recursive	=> list subdirectories recursively
 - -a, --all				=> do not ignore entries starting with .
 - -r --reverse			=> reverse order while sorting
 - -t								=> sort by time, newest first; see --time

### Definition
> ```ls [OPTION]... [FILE]...```

Although it's may appear that the options must be before the [FILE]... in actuallity, the flags can be anywhere in between.

### Behaviour analisis

NO FLAGS / NO [OPTION]
```bash
	ls
```

	ls will generate a file listing using the current location as root (default),
	separating each element with 2 spaces, it will only display files which are children of the current root

```bash
	ls [FILE]...
```
	When a file is provided there will be no default location and instead the root of the listing will be the file provided.
	If the user provides more than one file, each file will be used to output the files listing.
	Such are the behaviours:

	- When the user provides a file, only the file will be listed.
	- When the user provides a directory all the children of the directory will be listed, excluding the directory
  - If more than one [FILE]... are provided, each location will be listed, in the order of appearance, separated by an empty line.
  - When more than one [FILE]... are listed, if any is a directory, it should print the name of the directory one line avobe the listing.
		EX: ls dir1 dir2
		dir1/:
		file1.txt

		dir2/:
		file9.txt
 
	
***

```bash
	ls -l
```
