#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE 4096
#define ft_max(x,y) ((x) > (y) ? (x) : (y))

#include <signal.h>
int stop_serv = 0;
void sig_handler(int) {
	stop_serv = 1;
}

typedef struct client {
	int id;
	int fd;
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


void ft_exit(int exit_status, char *msg, server * s) {
	write(2, msg, strlen(msg));
	if (s)
		free(s->clients);
	exit(exit_status);
}

void server_emit(server * s, client c, char * msg) {
	int i;
	char str[BUFF_SIZE + 100];

        if (!s || !msg)
                return;
        for (i = 0; i < s->num_clients; i++) {
		if (c.fd != s->clients[i].fd) {
			sprintf(str, "client %d: %s", c.id, msg);
			write(s->clients[i].fd, str, strlen(str));
		}
        }
}

/**
 * add_client(), remove_client()
 * @params server * s, int client_fd
 *  - s is an allocated server pointer.
 *  - client_fd is a valid file descriptior.
 * @description
 *  - Attempt to add/remove a client to the server, the function
 *    uses realloc to allocate enough space for the new client o
 *    shrink in case of delition.
 *    The new client will be assigned the next available id.
 *    If s is NULL the function will have no effect.
 *    In case the client_fd a negative number or higher than
 *    FD_SETSIZE the function will have no effect.
 * @return
 *   - returns 0 on succes and 1 on failure
 **/
int add_client(server * s, int client_fd) {
	if (!s || client_fd < 0 || client_fd > FD_SETSIZE)
		return 1;
	s->clients = (client *)realloc(s->clients, sizeof(client) * (s->num_clients + 1));
	if (!s->clients)
		ft_exit(1, "Fatal Error\n", s);
	s->clients[s->num_clients] = (client){s->id_counter++, client_fd};
	server_emit(s, s->clients[s->num_clients], "just arrived\n");
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
		server_emit(s, s->clients[i], "just left\n");
		s->clients[i] = s->clients[s->num_clients - 1];
		s->clients = (client *)realloc(s->clients, sizeof(client) * s->num_clients - 1);
		if (!s->clients)
			ft_exit(1, "Fatal Error\n", s);
		s->num_clients--;
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
void init_fdset(server * s, fd_set * set) {
	FD_ZERO(set);
	for (int i = 0; i < s->num_clients; i++) {
		FD_SET(s->clients[i].fd, set);
		s->max_fd = ft_max(s->max_fd, s->clients[i].fd);
	}
	s->max_fd = ft_max(s->max_fd, s->fd);
	FD_SET(s->fd, set); // SET THE SERVER FD, READ WHEN CLIENT WANTS TO CONNECT
}
void select_loop(server * s) {
	fd_set rfds;
	int retval, i, r_size;
	char * buff, * msg;

	while (!stop_serv) {
		init_fdset(s, &rfds);
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
				buff = (char *)calloc(sizeof(char),  BUFF_SIZE + 1);
				r_size = read(s->clients[i].fd, buff, BUFF_SIZE);
				if (r_size == 0) {
					remove_client(s, s->clients[i].fd);
				} else if (r_size > 0) {
					while (extract_message(&buff, &msg)) {
						server_emit(s, s->clients[i], msg);
						free(msg);
					}
				}
				free(buff);
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
