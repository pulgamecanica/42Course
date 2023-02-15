#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * BUF_SIZE is 4096 because in my coumputer I've
 * seen that read never reads more that 4096 characters...
 **/
#define BUFF_SIZE 4096
/**
 * Function that returns the max number, used to get the
 * max file descriptor of the fd_set.
 **/
#define ft_max(x,y) ((x) > (y) ? (x) : (y))

/**
 * This function is for testing only so I can stop the
 * program with CTL + C and not crash. Although you can
 * stop with CTL + Z and achieve the same thing and you
 * don't even need to use signals.
 **/
#include <signal.h>
int stop_serv = 0;
void sig_handler(int) {
	stop_serv = 42;
}

typedef struct client {
	int id;
	int fd;
	char * msg;
} client;
typedef struct server {
	int fd;
	int port;
	int ip;
	int id_counter;
	int num_clients;
	int max_fd;
	client *clients;
} server;

/**
 * In this version we set the fd_sets global
 * to only emit when wset has the client fd.
 * In my opinion this iw wrong, and shouldn't be implemented.
 * This also affects the selct behaviour terribly...
 **/
fd_set wset, rset;

/**
 * Exit with status and a message.
 * @params int exit_status, char * msg, server * s
 *  - exit_status indicates the status return of the program.
 *  - msg is the message to be printed on the stderr.
 *  - s is an allocated server pointer.
 * @description
 *  - Close all client fds and free their msgs.
 *  - The program might call this function before the server
 *    was initialzed in which case s will be null.
 **/
void ft_exit(int exit_status, char * msg, server * s) {
	write(2, msg, strlen(msg));
	if (s) {
		for (int i = 0; i < s->num_clients; i++) {
			close(s->clients[i].fd);
			free(s->clients[i].msg);
		}
		close(s->fd);
		free(s->clients);
	}
	exit(exit_status);
}
/**
 * Emit to every other client.
 * @params server * s, client c, char * msg, int announcement
 *  - s is an allocated server pointer.
 *  - c is a client instance, this client is the emit owner.
 *  - msg is the message to be emit, allways \n\0 terminated.
 *  - announcement is a number indicating if it's a client
 *    message or a server announcement.
 * @description
 *  - Messages can be server announcements or client messages.
 *    Ex server announcements: server: client %d %s
 *    Ex client message: client %d: %s
 **/
void server_emit(server * s, client c, char * msg, int announcement) {
	int i;
	char * str;

        if (!s || !msg)
                return;
	str = (char *)calloc(sizeof(char), strlen(msg) + 200);
	if (!str)
		ft_exit(1, "Fatal Error\n", s);
	if (announcement)
		sprintf(str, "server: client %d %s", c.id, msg);
	else
		sprintf(str, "client %d: %s", c.id, msg);
        for (i = 0; i < s->num_clients; i++)
		if (c.fd != s->clients[i].fd && FD_ISSET(s->clients[i].fd, &wset))
			write(s->clients[i].fd, str, strlen(str));
	free(str);
}

/**
 * Add client & Remove client
 * @params server * s, int client_fd
 *  - s is an allocated server pointer.
 *  - client_fd is a valid file descriptior.
 * @description
 *  - Attempt to add/remove a client to the server, the function
 *    uses realloc to allocate enough space for the new client o
 *    shrink in case of delition.
 *  - The new client will be assigned the next available id.
 *    If s is NULL the function will have no effect.
 *    In case the client_fd a negative number or higher than
 *    FD_SETSIZE the function will have no effect.
 *  # add_client, make sure to initialize msg with NULL.
 *  # remove_client, make sure to close the client fd and free
 *    the allocated msg, it's ok to free even if you didn't allocate,
 *    because free(NULL) has no effect, and we initialized it NULL;
 * @return
 *   - returns 0 on succes and 1 on failure
 **/
int add_client(server * s, int client_fd) {
	if (!s || client_fd < 0 || client_fd > FD_SETSIZE)
		return 1;
	s->clients = (client *)realloc(s->clients, sizeof(client) * (s->num_clients + 1));
	if (!s->clients)
		ft_exit(1, "Fatal Error\n", s);
	s->clients[s->num_clients] = (client){s->id_counter++, client_fd, NULL};
	server_emit(s, s->clients[s->num_clients], "just arrived\n", 1);
	s->num_clients++;
	return (0);
}
int remove_client(server * s, int client_fd) {
	int i;

	if (!s || client_fd < 0 || client_fd > FD_SETSIZE)
		return 1;
	for (i = 0; i < s->num_clients; i++) {
		if (s->clients[i].fd == client_fd) {
			break;
		}
	}
	if (i < s->num_clients) {
		server_emit(s, s->clients[i], "just left\n", 1);
		close(s->clients[i].fd);
		free(s->clients[i].msg);
		s->num_clients--;
		s->clients[i] = s->clients[s->num_clients];
		s->clients = (client *)realloc(s->clients, sizeof(client) * s->num_clients);
		if (s->num_clients == 0)
			s->clients = NULL;
		if (!s->clients && s->num_clients)
			ft_exit(1, "Fatal Error\n", s);
	}
	return (0);
}
int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}
char *str_join(char *buf, char *add) {
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = calloc(sizeof(*newbuf), (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}
/**
 * Initialize the server
 * @params int ip, int port
 *  - ip is the ip of the server.
 *  - port is the port where the server will listen.
 * @description
 *  - Creates a socket with the ip and port setup.
 * @return
 *  - Returns a server instance with initialized starting values.
 **/
server init_server(int ip, int port) {
	int sockfd;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		ft_exit(1, "Fatal Error\n", NULL);
	bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(ip);
        servaddr.sin_port = htons(port);
        if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		ft_exit(1, "Fatal Error\n", NULL);
	if (listen(sockfd, 10) != 0)
		ft_exit(1, "Fatal Error\n", NULL);
	return (server){sockfd, port, ip, 0, 0, 0, NULL};
}
/**
 * Accept Connection from socket
 * @params server * s
 *  - s is an allocated server pointer.
 * @description
 *  - Will attempt to accept a new connection from the client.
 *  - The accept will return the fd of the client, which is
 *    a clone of the original socket fd, so it will be a SOCK_STREAM,
 *    this means it will be a two-way connection and the write is going
 *    to be always open according to my theory.
 *  - That's why you don't need to check the write fd_set.
 *  - On success the client fd will be added to the clients with an id.
 **/
void accept_connection(server * s) {
	int connfd;
	struct sockaddr_in cli;
        socklen_t addrlen;

	addrlen = sizeof(cli);
        connfd = accept(s->fd, (struct sockaddr *)&cli, &addrlen);
        if (connfd < 0)
		ft_exit(1, "Fatal Error\n", s);
        else
		add_client(s, connfd);
}
/**
 * Initialize fd set
 * @params server * s, fd_set * set
 *  - s is an allocated server pointer.
 *  - set is a pointer to a fd_set
 * @description
 *  - It will initialize the list of fd_sets,
 *    copying every client and the socket fd.
 *  - This function will also set up the server max_fd.
 **/
void init_fdset(server * s, fd_set * set) {
	FD_ZERO(set);
	s->max_fd = 0;
	for (int i = 0; i < s->num_clients; i++) {
		FD_SET(s->clients[i].fd, set);
		s->max_fd = ft_max(s->max_fd, s->clients[i].fd);
	}
	FD_SET(s->fd, set);
	s->max_fd = ft_max(s->max_fd, s->fd);
}
/**
 * Infinite Loop to manage client connections
 * @params server * s
 *  - s is an allocated server pointer.
 * @description
 *  - The function starts by setting a few attributes.
 *    rfds is going to be the read fd set, while retval will be
 *    the number of event that select retruns, i and r_size are counters.
 *    buff is going to be the buffer of the read function, this means that
 *    read will only read BUFF_SIZE character at a time.
 *    msg is a variable which will be temporary for the extracted message.
 * - We must call init_fdset everytime before select, to initialize the
 *   set with new clients or removed clients.
 * - Select will monitor the rfds and we need to indicate what is the
 *   maximum file descriptor.
 * - I decided to protect !retval in case a signal triggers the select.
 * - When the s-> (socket fd) is in action we must accept new connections.
 * - Then we must check each client to detect who could be sending
 *   a message. We must clean the buf because the last byte should be zero,
 *   also to clean any traces of the last use.
 *   When the read returns 0 it means that the client sent a disconnection
 *   signal, in that case we remove the client.
 *   When we recieve a message we should append the message to the current
 *   client msg (char *). This is because clients might not send the full
 *   message all at once, that's also the reason why we must have one pointer
 *   for every client and keep the track of the sent bytes.
 *   You should only emit messages which end with \n (so messages extracted).
 **/
void select_loop(server * s) {
	fd_set rfds;
	int retval, i, r_size;
	char * msg, buff[BUFF_SIZE + 1];

	while (!stop_serv) {
		init_fdset(s, &rfds);
		rset = wset = rfds;
		retval = select(s->max_fd + 1, &rfds, NULL, NULL, 0);
		if (retval == -1)
			ft_exit(1, "Fatal Error\n", s);
		else if (!retval)
			continue;
		if (FD_ISSET(s->fd, &rfds)) {
			accept_connection(s);
		}
		for (i = 0; i < s->num_clients; i++) {
			if (FD_ISSET(s->clients[i].fd, &rfds)) {
				bzero(buff, BUFF_SIZE + 1);
				r_size = read(s->clients[i].fd, buff, BUFF_SIZE);
				if (r_size == 0) {
					remove_client(s, s->clients[i].fd);
				} else if (r_size == -1){
					ft_exit(1, "Fatal Error\n", s);
				} else if (r_size > 0) {
					s->clients[i].msg = str_join(s->clients[i].msg, buff);
					while (extract_message(&s->clients[i].msg, &msg)) {
						server_emit(s, s->clients[i], msg, 0);
						free(msg);
					}
				}
			}
		}
	}
}
int main(int ac, char *av[]) {
	int port;
	server s;

	signal(SIGINT, sig_handler);
	if (ac != 2)
		ft_exit(1, "Wrong number of arguments\n", NULL);
	port = atoi(av[1]);
	s = init_server(2130706433, port);
	select_loop(&s);
	free(s.clients);
}
