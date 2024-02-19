# Zappy

## Zappy is a game about a mythical world

***

## Tech Stack

-   C++20 (Server)
-   +   epoll API (file descriptor manager interface)
-   +   Functional over OO programming (because I like it more, so what?)
-   Ruby w/ YJIT (Because I love ruby)

***

## Epoll

Epoll is an API which allows you to control a list of file descriptors.

We must controll the following file descriptors:

    - Server Socket (allow to accept connections)
    - Client Player file descriptor (the fd of our accept client)
    - Client Graphic Game file descriptor (this is just as the fd if the client)

Server Socket: this is the socket of our server, we can receive request connections from clients.

Client Player: This is the IA player which will be able to controll their game player using different commands.

Client Graphic Game: This is a client which receives the game information at a frame rate defined by the game server (unlike udp this probably needs to behave as a server as well using tcp, and be ready to handle all the input when it's sent via the Game Server). 


#### Server - Epoll

The server will initialize a socket with TCP connection ipV4, two-way connection ~> AF_INET, SOCK_STREAM (man socket)

IP address and port should be provided by the user initializing the Zappy serevr, otherwize the default will be used: localhost:4242

Epoll will register the socket which will be used to handle clients

#### Server Commands


Client:

| CMD | OPTIONS | DESCRIPTION |
| --- | ------- | ----------- |
| help | (X) | Display info about the client the commands and server options |
| join | team name | Clients can use it to join a game |
| spectate | (X) | Clients can use it to spectate a game |
| status | (X) | Clients can request at any time the status of the server |
| exit | (X) | Leave the server |

Player: 

TODO

