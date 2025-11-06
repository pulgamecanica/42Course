#include "steque.h"
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <printf.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <unistd.h>

// Note that the -n and -z parameters are NOT used for Part 1

#define USAGE                                                                  \
  "usage:\n"                                                                   \
  "  webproxy [options]\n"                                                     \
  "options:\n"                                                                 \
  "  -n [segment_count]  Number of segments to use (Default: 8)\n"             \
  "  -p [listen_port]    Listen port (Default: 25362)\n"                       \
  "  -s [server]         The server to connect to (Default: GitHub test "      \
  "data)\n"                                                                    \
  "  -t [thread_count]   Num worker threads (Default: 8 Range: 200)\n"         \
  "  -z [segment_size]   The segment size (in bytes, Default: 5712).\n"        \
  "  -h                  Show this help message\n"

shmpool_t *pool;

// Options
static struct option gLongOptions[] = {
    {"server", required_argument, NULL, 's'},
    {"segment-count", required_argument, NULL, 'n'},
    {"listen-port", required_argument, NULL, 'p'},
    {"thread-count", required_argument, NULL, 't'},
    {"segment-size", required_argument, NULL, 'z'},
    {"help", no_argument, NULL, 'h'},

    {"hidden", no_argument, NULL, 'i'}, // server side
    {NULL, 0, NULL, 0}};

// gfs
static gfserver_t gfs;

// handles cache
extern ssize_t handle_with_cache(gfcontext_t *ctx, char *path, void *arg);

int create_shmseg(shmseg_t *seg, char *name, size_t size) {
  seg->fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  if (seg->fd == -1)
    return -1;

  if (ftruncate(seg->fd, size) == -1)
    return -1;

  seg->addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, seg->fd, 0);
  if (seg->addr == MAP_FAILED)
    return -1;

  snprintf(seg->name, NAME_LEN, "%s", name);
  seg->size = size;

  return 0;
}

static void _sig_handler(int signo) {
  if (signo == SIGTERM || signo == SIGINT) {
    // cleanup could go here
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
    steque_destroy(&pool->shmqueue);

    for (int i = 0; i < pool->count; i++) {
      close(pool->pool[i].fd);
      munmap(pool->pool[i].addr, pool->pool[i].size);
      shm_unlink(pool->pool[i].name);
    }

    free(pool->pool);
    free(pool);

    gfserver_stop(&gfs);
    exit(signo);
  }
}

int main(int argc, char **argv) {
  int option_char = 0;
  char *server = "https://raw.githubusercontent.com/gt-cs6200/image_data";
  unsigned int nsegments = 8;
  unsigned short port = 25362;
  unsigned short nworkerthreads = 8;
  size_t segsize = 5712;

  setbuf(stdout, NULL);

  if (signal(SIGTERM, _sig_handler) == SIG_ERR) {
    fprintf(stderr, "Can't catch SIGTERM...exiting.\n");
    exit(SERVER_FAILURE);
  }

  if (signal(SIGINT, _sig_handler) == SIG_ERR) {
    fprintf(stderr, "Can't catch SIGINT...exiting.\n");
    exit(SERVER_FAILURE);
  }
  // Parse and set command line arguments */
  while ((option_char = getopt_long(argc, argv, "s:qht:xn:p:lz:", gLongOptions,
                                    NULL)) != -1) {
    switch (option_char) {
    default:
      fprintf(stderr, "%s", USAGE);
      exit(__LINE__);
    case 'h': // help
      fprintf(stdout, "%s", USAGE);
      exit(0);
      break;
    case 'p': // listen-port
      port = atoi(optarg);
      break;
    case 's': // file-path
      server = optarg;
      break;
    case 'n': // segment count
      nsegments = atoi(optarg);
      break;
    case 'z': // segment size
      segsize = atoi(optarg);
      break;
    case 't': // thread-count
      nworkerthreads = atoi(optarg);
      break;
    case 'i':
    // do not modify
    case 'O':
    case 'A':
    case 'N':
      // do not modify
    case 'k':
      break;
    }
  }

  if (server == NULL) {
    fprintf(stderr, "Invalid (null) server name\n");
    exit(__LINE__);
  }

  if (segsize < 824) {
    fprintf(stderr, "Invalid segment size\n");
    exit(__LINE__);
  }

  if (port > 65332) {
    fprintf(stderr, "Invalid port number\n");
    exit(__LINE__);
  }
  if ((nworkerthreads < 1) || (nworkerthreads > 200)) {
    fprintf(stderr, "Invalid number of worker threads\n");
    exit(__LINE__);
  }
  if (nsegments < 1) {
    fprintf(stderr, "Must have a positive number of segments\n");
    exit(__LINE__);
  }

  pool = malloc(sizeof(shmpool_t));
  if (!pool)
    return -1;

  pool->pool = malloc(nsegments * sizeof(shmseg_t));
  if (!pool->pool) {
    free(pool);
    return -1;
  }

  pool->count = nsegments;
  if (pthread_mutex_init(&pool->lock, NULL) != 0) {
    free(pool->pool);
    free(pool);
    return -1;
  }

  if (pthread_cond_init(&pool->cond, NULL) != 0) {
    pthread_mutex_destroy(&pool->lock);
    free(pool->pool);
    free(pool);
    return -1;
  }

  steque_init(&pool->shmqueue);

  for (int i = 0; i < nsegments; i++) {
    char name[NAME_LEN];
    snprintf(name, NAME_LEN, "/shmseg%d", i);

    if ((create_shmseg(&pool->pool[i], name, segsize)) == -1) {
      pthread_mutex_destroy(&pool->lock);
      pthread_cond_destroy(&pool->cond);
      steque_destroy(&pool->shmqueue);
      free(pool->pool);
      free(pool);
      return -1;
    }
    steque_enqueue(&pool->shmqueue, &pool->pool[i]);
  }

  gfserver_init(&gfs, nworkerthreads);

  gfserver_setopt(&gfs, GFS_PORT, port);
  gfserver_setopt(&gfs, GFS_WORKER_FUNC, handle_with_cache);
  gfserver_setopt(&gfs, GFS_MAXNPENDING, 187);

  for (int i = 0; i < nworkerthreads; i++) {
    gfserver_setopt(&gfs, GFS_WORKER_ARG, i, pool);
  }

  gfserver_serve(&gfs);

  // line never reached
  return -1;
}