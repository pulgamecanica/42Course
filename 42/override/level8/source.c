#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

/*
 * Level 8 - Path Traversal Vulnerability
 *
 * This program creates backups of files by copying them to ./backups/
 * directory. The vulnerability is that it uses relative paths and doesn't
 * validate the input path, allowing an attacker to read arbitrary files
 * by creating a matching directory structure.
 *
 * Key vulnerability: Predictable backup location with relative paths
 * Exploitation: Create directory structure in writable location (/tmp)
 */

void log_wrapper(FILE *log_file, const char *prefix, const char *filename)
{
    char message[264];

    // Build log message
    strcpy(message, prefix);
    snprintf(&message[strlen(message)], 254 - strlen(message), "%s", filename);

    // Remove newline
    message[strcspn(message, "\n")] = 0;

    // Write to log
    fprintf(log_file, "LOG: %s\n", message);
}

int main(int argc, const char **argv)
{
    FILE *log_file;
    FILE *source_file;
    int backup_fd;
    char ch;
    char backup_path[104];

    ch = -1;

    // Check arguments
    if (argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }

    // Open log file for writing
    log_file = fopen("./backups/.log", "w");
    if (!log_file)
    {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        exit(1);
    }

    // Log start
    log_wrapper(log_file, "Starting back up: ", argv[1]);

    // Open source file for reading
    source_file = fopen(argv[1], "r");
    if (!source_file)
    {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(1);
    }

    // Build backup file path
    strcpy(backup_path, "./backups/");
    strncat(backup_path, argv[1], 99 - strlen(backup_path));

    // Open/create backup file
    // Flags: O_WRONLY | O_CREAT | O_EXCL = 193
    // Mode: 0660 = 432
    backup_fd = open(backup_path, O_WRONLY | O_CREAT | O_EXCL, 0660);
    if (backup_fd < 0)
    {
        printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
        exit(1);
    }

    // Copy file contents byte by byte
    while (1)
    {
        ch = fgetc(source_file);
        if (ch == -1)  // EOF
            break;
        write(backup_fd, &ch, 1);
    }

    // Log finish
    log_wrapper(log_file, "Finished back up ", argv[1]);

    // Cleanup
    fclose(source_file);
    close(backup_fd);

    return 0;
}
