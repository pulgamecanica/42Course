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

### Behaviour analysis

> [default listing]  => short listing, each file will be separated by two spaces

> [default sorting]  => alphabetically

> [default location] => current root, where you execute the command

#### NO FLAGS / NO [OPTION]
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
