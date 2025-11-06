#ifndef __GETFILE_SERVER_H__
#define __GETFILE_SERVER_H__

#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <getopt.h>
#include <printf.h>
#include <stddef.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/signal.h>
#include "steque.h"


#define MAX_REQUEST_LEN 128

typedef int gfstatus_t;

//Status definitions
#define  GF_OK 200
#define  GF_FILE_NOT_FOUND 400
#define  GF_ERROR 500

/*
Appropriate server failure definition
*/
#if !defined(SERVER_FAILURE)
#define SERVER_FAILURE (-1) 
#endif 

typedef struct _gfserver_t gfserver_t;
typedef struct _gfcontext_t gfcontext_t;

/*
Main struct definition
*/
struct _gfserver_t{
	steque_t req_queue;
	unsigned short port;
	int max_npending;
	int nthreads;
	int socket_fd;
	ssize_t (*worker_func)(gfcontext_t *, const char *, void*);
	gfcontext_t *contexts;
	pthread_mutex_t queue_lock;
	pthread_cond_t req_inserted;
};

struct _gfcontext_t{
	pthread_t thread;
	void *arg;
	gfserver_t *gfs;

	int socket;
	size_t file_len;
	size_t bytes_transferred;

	char *protocol;
	char *method;
	char *path;
	char request[MAX_REQUEST_LEN];	
};

typedef enum{
  GFS_PORT,
  GFS_MAXNPENDING,
  GFS_WORKER_FUNC,
  GFS_WORKER_ARG
} gfserver_option_t;


/* 
 * Initializes the input gfserver_t object to use nthreads.
 */
void gfserver_init(gfserver_t *gfh, int nthreads);

/* 
 * Sets options for the gfserver_t object. The table below
 * lists the values for option in the left column and the
 * additional arguments in the right.
 *
 * GFS_PORT 	  		unsigned short indicating the port on which 
 * 						the server should receive connections.
 *
 * GFS_MAXNPENDING 	 	int indicating the maximum number of pending
 * 						connections the receiving socket should permit.
 *
 * GFS_WORKER_FUNC 		a function pointer with the signature
 * 						ssize_t (*)(gfcontext_t *, char *, void*);
 *
 *						The first argument contains the needed context
 *						information for the server and should be passed
 *						into calls to gfs_sendheader and gfs_send.
 *						
 *						The second argument is the path of the requested
 *						resource.
 * 
 * 						The third argument is the argument registered for
 *						this particular thread with the GFS_WORKER_ARG
 *						option.
 *
 *						Returning a negative number will cause the 
 *						gfserver library to send an error message to the
 *						client.  Otherwise, gfserver will assume that
 *						this function has performed all the necessary 
 *						communication.
 *
 *
 * GFS_WORKER_ARG		This option is followed by two arguments, an int
 *						indicating the thread index [0,...,nthreads-1] and
 * 						a pointer which will be passed into the callback
 * 						registered via the GFS_WORKER_FUNC option on this 
 *						thread.
 *						
 */
void gfserver_setopt(gfserver_t *gfh, gfserver_option_t option, ...);

/*
 * Connects the server to the socket so that it can begin handling requests.  
 */
void gfserver_serve(gfserver_t *gfh);

/*
 * Shuts down the server and severs conenctions associated with the input gfserver_t object.  
 */
void gfserver_stop(gfserver_t *gfh);

/*
 * Sends to the client the Getfile header containing the appropriate 
 * status and file length for the given inputs.  This function should
 * only be called from within a callback registered with the 
 * GFS_WORKER_FUNC option.
 */
ssize_t gfs_sendheader(gfcontext_t *ctx, gfstatus_t status, size_t file_len);

/*
 * Sends size bytes starting at the pointer data to the client 
 * This function should only be called from within a callback registered 
 * with the GFS_WORKER_FUNC option.  It returns once the data has been
 * written to the socket.
 */
ssize_t gfs_send(gfcontext_t *ctx, void *data, size_t size);

#endif