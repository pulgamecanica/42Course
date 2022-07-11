#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int main() {
    char* const env[4] = {"PATH=HelloWorld", "QUERY_STRING=?id=123&name=andre#world", "CONTENT_LENGTH=10", NULL};
    char* const arg[2] = {"/home/jaman/42/webserver/www/cgi-bin/countries.py", NULL};
    int restore_input = dup(STDIN_FILENO);
    FILE * tmp_file_in = tmpfile();
    FILE * tmp_file_out = tmpfile();

    if (!tmp_file_in || !tmp_file_out || restore_input < 0)
      exit(1);

    int tmp_fd_in = fileno(tmp_file_in);
    int tmp_fd_out = fileno(tmp_file_out);
    if (tmp_fd_in < 0)
      exit(1);

    write(tmp_fd_in, "This is 10\n\n", 12);

    rewind(tmp_file_in);

    int pid = fork();

    if (pid == 0)
    {
       dup2(tmp_fd_in, STDIN_FILENO);
       dup2(tmp_fd_out, STDOUT_FILENO);
       execve("/home/jaman/42/webserver/www/cgi-bin/countries.py", arg, env);
       perror ("Didn't work");
       exit(2);
    }
    wait(NULL);
    close(tmp_fd_in);
    dup2(STDIN_FILENO, restore_input);
    close(restore_input);

    rewind(tmp_file_out);
    char buff[1024];
    int valread = -1;
    while(valread != 0) {
    	bzero(buff, 1024);
	valread = read(tmp_fd_out, buff, 1023);
	if (valread < 0)
	  exit (3);
	printf("%s", buff);
    }
    close(tmp_fd_out);
    printf("Finished!\n");
    exit(0);
}
