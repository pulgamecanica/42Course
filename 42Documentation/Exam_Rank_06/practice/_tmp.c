#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFF_SIZE 4096
#define max(x,y) ((x) > (y) ? (x) : (y))

typedef struct client {
	int fd;
	int id;
}	client;
typedef struct server {
	int fd;
	int port;
	int ip;
	int id_count;
	int num_clients;
	int max_fd;
	client * clients;
}	server;

void    server_emit(server * s, client c, char * msg, int announcement);

void	ft_exit(int status, char * msg, server * s) {
	if (s)	{
		if (s->clients) {
			for (int i = 0; i < s->num_clients; i++)
				close(s->clients[i].fd);
			free(s->clients);
		}
		close(s->fd);
	}
	write(2, msg, strlen(msg));
	exit(status);
}

void	add_client(server * s, int client_fd) {
	if (!s || client_fd < 0 || client_fd > FD_SETSIZE)
		return ;
	s->clients = (client *)realloc(s->clients, sizeof(client) * (s->num_clients + 1));
	if (!s->clients)
		ft_exit(1, "Fatal Error\n", s);
	s->clients[s->num_clients] = (client){client_fd, s->id_count++};
	server_emit(s, s->clients[s->num_clients], "just arrived\n", 1);
	s->num_clients++;
}

void	remove_client(server * s, int fd) {
	int i;

	if (!s || fd < 0 || fd > FD_SETSIZE)
		return ;
	for(i = 0; i < s->num_clients; i++)
		if (s->clients[i].fd == fd)
			break;
	if (i < s->num_clients) {
		server_emit(s, s->clients[i], "just left\n", 1);
		close(s->clients[i].fd);
		s->num_clients--;
		s->clients[i] = s->clients[s->num_clients];
		s->clients = (client *)realloc(s->clients, sizeof(client) * s->num_clients);
		if (!s->clients)
			s->clients = NULL;
	}
}

void	server_emit(server * s, client c, char * msg, int announcement) {
	char buff[BUFF_SIZE * 42];

	memset(buff, 0, BUFF_SIZE * 42);
	if (!s || !msg)
		return ;
	if (announcement)
		sprintf(buff, "server: client %d %s", c.id, msg);
	else
		sprintf(buff, "client %d: %s", c.id, msg);
	for (int i = 0; i < s->num_clients; i++)
		if (s->clients[i].fd != c.fd)
			write(s->clients[i].fd, buff, strlen(buff));
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

server init_server(int port, int ip) {
	struct sockaddr_in addr;
	int sockfd;

	if (port <= 0 || ip < 0)
		ft_exit(1, "Fatal Error\n", NULL);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		ft_exit(1, "Fatal Error\n", NULL);
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		close(sockfd);
		ft_exit(1, "Fatal Error\n", NULL);
	}
	if(listen(sockfd, 10) != 0) {
		close(sockfd);
		ft_exit(1, "Fatal Error\n", NULL);
	}
	return (server){sockfd, port, ip, 0, 0, 0, NULL};
}

void init_fdset(server * s, fd_set * list) {
	FD_ZERO(list);
	s->max_fd = 0;
	for (int i = 0; i < s->num_clients; i++) {
		FD_SET(s->clients[i].fd, list);
		s->max_fd = max(s->max_fd, s->clients[i].fd);
	}
	s->max_fd = max(s->max_fd, s->fd);
	FD_SET(s->fd, list);
}

void accept_connection(server * s) {
	socklen_t addrlen;
	struct sockaddr_in client_addr;
	int fd;

	addrlen = sizeof(client_addr);
	memset(&client_addr, 0, addrlen);
	fd = accept(s->fd, (struct sockaddr *) &client_addr, &addrlen);
	if (fd == -1)
		ft_exit(1, "Fatal Error\n", s);
	add_client(s, fd);
}

void server_loop(server *s) {
	char * buff, * msg;
	fd_set list;
	int ready, rval;

	while (42) {
		init_fdset(s, &list);
		ready = select(s->max_fd + 1, &list, NULL, NULL, NULL);
		if (ready <= -1)
			ft_exit(1, "Fatal Error\n", s);
		if (!ready)
			continue;
		if (FD_ISSET(s->fd, &list))
			accept_connection(s);
		for (int i = 0; i < s->num_clients; i++) {
			if (FD_ISSET(s->clients[i].fd, &list)) {
				buff = (char *)calloc(sizeof(char), (BUFF_SIZE * 42) + 1);
				rval = read(s->clients[i].fd, buff, BUFF_SIZE * 42);
				if (rval == -1) {
					free(buff);
					ft_exit(1, "Fatal Error\n", s);
				}
				else if (rval == 0)
					remove_client(s, s->clients[i].fd);
				else {
					while (extract_message(&buff, &msg)) {
						server_emit(s, s->clients[i], msg, 0);
						free(msg);
					}
				}
				free(buff);
			}
		}
	}
}

int main(int ac, char *av[]) {
	server s;
	int port;

	if (ac != 2)
		ft_exit(1, "Wrong number of arguments\n", NULL);
	port = atoi(av[1]);
	s = init_server(port, 2130706433);
	server_loop(&s);
	return (0);
}
