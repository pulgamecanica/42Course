#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>

#include <sys/types.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int main() {

    char* const ENV[3] = {"PATH=HelloWorld", "QUERY_STRING=?id=123&name=andre#world", NULL};

    char* const arg[3] = {"/home/pulgamecanica/Desktop/my_webserv/utils/CGI_Scripts/C++_CGI/cgi_country", NULL};

// open file
    int fd = open("tempfile.txt", O_RDWR);
    printf("open %d\n", fd);

// write to that file
    size_t file_write = write(fd, "?country=Brasil", 15);
    lseek(fd, -file_write, SEEK_CUR);

// saving stdin
    int save_fd = dup(STDIN_FILENO);
    printf("std::in? %d\n", save_fd);

// overriding stdin;
    //printf("overwritten fd: %d\n", new_fd);

    if (fork() == 0) {
        dup2(fd, STDIN_FILENO);
        execve("/home/pulgamecanica/Desktop/my_webserv/utils/CGI_Scripts/C++_CGI/cgi_country", arg, ENV);
    }
    else {
        wait(NULL);
        printf("back to parent");
    }

}
