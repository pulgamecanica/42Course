#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

#define BUF_SIZE 4096
#define max(x,y) ((x) > (y) ? (x) : (y))

typedef struct client {
	int id;
	int fd;
} client;

typedef struct server {
	int fd;
	int port;
	int ip;
	int next_id;
	int num_clients;
	int max_fd;
	client * clients;
} server;


void ft_exit(int status, char * msg, server * s) {
	if (s) {
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

void server_emit(server * s, client c, char * msg, int announcement) {
	char buf[BUF_SIZE * 42];

	bzero(buf, BUF_SIZE * 42);
	if (announcement)
		sprintf(buf, "server: client %d %s", c.id, msg);
	else
		sprintf(buf, "client %d: %s", c.id, msg);
	for (int i = 0; i < s->num_clients; i++)
		if (s->clients[i].fd != c.fd)
			write(s->clients[i].fd, buf, strlen(buf));
}

void add_client(server * s, int client_fd) {
	if (!s || client_fd < 0 || client_fd > FD_SETSIZE)
		return ;
	s->clients = (client *)realloc(s->clients, sizeof(client) * (s->num_clients + 1));
	if (!s->clients)
		ft_exit(1, "Fatal error\n", s);
	s->clients[s->num_clients] = (client){s->next_id++, client_fd};
	server_emit(s, s->clients[s->num_clients], "has arrived\n", 1);
	s->num_clients++;
}

void remove_client(server *s, int client_fd) {
	int i;

	if (!s || client_fd < 0 || client_fd > FD_SETSIZE)
		return ;
	for(i = 0; i < s->num_clients; i++)
		if (s->clients[i].fd == client_fd)
			break;
	if (i == s->num_clients)
		return ;
	server_emit(s, s->clients[i], "has left\n", 1);
	close(s->clients[i].fd);
	s->num_clients--;
	s->clients[i] = s->clients[s->num_clients];
	s->clients = (client *)realloc(s->clients, sizeof(client) * (s->num_clients));
	if (s->num_clients <= 0)
		s->clients = NULL;
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
	int fd;

	if (port < 0 || ip < 0)
		ft_exit(1, "Fatal error\n", NULL);
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
		ft_exit(1, "Fatal error\n", NULL);
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = htonl(ip);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	if (bind(fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
		close(fd);
		ft_exit(1, "Fatal error\n", NULL);
	}
	if (listen(fd, 10) != 0) {
		close(fd);
		ft_exit(1, "Fatal error\n", NULL);
	}
	return (server){fd, ip, port, 0, 0, 0, NULL};
}

void init_fdset(server * s, fd_set * rset) {
	FD_ZERO(rset);
	s->max_fd = 0;
	for (int i = 0; i < s->num_clients; i++) {
		FD_SET(s->clients[i].fd, rset);
		s->max_fd = max(s->max_fd, s->clients[i].fd);
	}
	s->max_fd = max(s->max_fd, s->fd);
	FD_SET(s->fd, rset);
}


void accept_connection(server * s) {
	socklen_t addrlen;
	struct sockaddr_in client_addr;
	int fd;

	if (!s)
		ft_exit(1, "Fatal error\n", s);
	addrlen = sizeof(client_addr);
	memset(&client_addr, 0, addrlen);
	fd = accept(s->fd, (struct sockaddr *) &client_addr, &addrlen);
	if (fd == -1)
		ft_exit(1, "Fatal error\n", s);
	add_client(s, fd);
}

void select_loop(server * s) {
	fd_set rset;
	int ready, rval;
	char * buf, * msg;

	while (42) {
		init_fdset(s, &rset);
		ready = select(s->max_fd + 1, &rset, NULL, NULL, NULL);
		if (ready < 0)
			ft_exit(1, "Fatal error\n", s);
		if (!ready)
			continue ;
		if (FD_ISSET(s->fd, &rset))
			accept_connection(s);
		for (int i = 0; i < s->num_clients; i++) {
			if (FD_ISSET(s->clients[i].fd, &rset)) {
				buf = (char *)calloc(sizeof(char), BUF_SIZE * 42 + 1);
				rval = read(s->clients[i].fd, buf, BUF_SIZE * 42);
				if (rval < 0) {
					free(buf);
					ft_exit(1, "Fatal error\n", s);
				} else if (rval == 0) {
					remove_client(s, s->clients[i].fd);
				} else {
					while (extract_message(&buf, &msg)) {
						server_emit(s, s->clients[i], msg, 0);
						free(msg);
					}
				}
				free(buf);
			}
		}
	}
}

int main(int ac, char *av[]) {
	int  port;
	server s;

	if (ac != 2)
		ft_exit(1, "Wrong number of arguments\n", NULL);
	port = atoi(av[1]);
	s = init_server(port, 2130706433);
	select_loop(&s);
	return (0);
}
