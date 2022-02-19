# Fork

<img src="https://banner2.cleanpng.com/20180429/gzw/kisspng-computer-icons-fork-branching-source-code-5ae55a968fa6e5.6869739515249803745884.jpg" width="100" height="100" style="float: right;">

### Create a new proccess

The fork function will create a new child process which is an exact copy of the parent proccess exept:

- Each process has their own id
- Each proccess will execute one line after another at the same time
- I don't understand the order of the execution....
- Static variables are independant in each process...

### Notes

- #include <unistd.h>
- Returns the id of the child procces
- It's wize to use it as follow:
```sh
    int id = 0;
    id = fork(); // From this point the child procces will have an id == 0.
    if (id == 0) // The parent procces will assign to the id var the return value of fork.
        //THIS WILL ONLY BE EXECUTED ON THE CHILD...
```
---

# Wait & Waitpid

<img src="https://www.clipartmax.com/png/full/52-523942_hourglass-clipart-transparent-sand-clock-gif-png.png" width="100" height="100" style="float: right;">

### Wait for a process to finish
    pid_t   wait(int *stat_loc);
    pid_t   waitpid(pid_t pid, int *stat_loc, int options);

The wait function will wait untill the stat_loc var is available.

- The wait recieves an argument which is an int which is defined on the man 2 sleep.
- waitpid is the implementation of wait4 with rusage dafault 0.
- wait function stops the execution of the calling process until the stat_loc is meet... while waitpid wil stop execution of a given process id (not the calling process necessarily).

### Notes

- #include <sys/wait.h>
- Wait with NULL parameters will wait for the children processes to finish (only ONE children to finish).
- It would be wize to use wait like this:
```sh
    int status;
    status = WIFEXITED(id); // Where id is the id of the process.
    if (id != 0) // If this is not the child process 
        wait(&status); // Wait until the child is finished. Then continue.
```

---

# Pipe

<img src="https://t3.ftcdn.net/jpg/01/34/59/08/360_F_134590857_W1F1DVkYl8KnvXKqK4SCTKDcdxdLNqOj.jpg" width="100" height="100" style="float: right;">

#### Create a descriptor pair to comunicate between processes
    int pipe(int fds[2]);

The Pipe function will create a pair of file descriptors that behave like this:

- fds[0] will be assigned to a file descriptor where you can READ from.
- fds[1] will be assigned to a file descriptor from where you can WRITE.
- Obvioulsy the READ fd is going to read the data written in fds[1]

#### Notes

- #include <unistd.h>
- When you create a child process after a pipe the file descriptors will be copied over the memory. Therefore they will behave independently from the ones in the parent process.
- You have to close both fds in every process that is created afterwards. Closing the fds in a process won't close the child's fds.
- I believe that the read function blocks the programm until something is read when we use File Descriptors... But still good to wait for write before read...
- Code Hint:
```sh
    int fds[2];
    int stat = pipe(fds);
    if (stat == -1) # Protect the pipe when it fails 
    {
        perror ("PIPE Error: ");
        return ;
    }
    #Somewhere in your code
    write(fds[1], &smthng, sizeof(smthng));
    close(fds[1]);
    #Somewhere in the code after write
    read(fds[0], &buff, sizeof(smthng));
    close(fds[0]);
```

---

# Execve

#### Execute a program
    int execve(const char *pathname, char *const argv[], char *const envp[]);

The execve function will execute a program refered by the path.

- The function will REPLACE the process which is running, so it won't execute whatever is after the function execution unless it returns an error (-1).
- This is the reason why we need to use child processes to do this exercise
- to see more about third argument man 3 environ

---

# Dup & Dup2 
	int dup(int oldfd);
	int dup2(int oldfd, int newfd);

- man stdin

