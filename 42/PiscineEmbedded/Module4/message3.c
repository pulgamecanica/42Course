#include "./gfserver.h"
#include "./shm_channel.h"
#include "./simplecache.h"
#include "./steque.h"
#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <curl/curl.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <printf.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

// CACHE_FAILURE
#if !defined(CACHE_FAILURE)
#define CACHE_FAILURE (-1)
#endif

#define MAX_CACHE_REQUEST_LEN 6112
#define MAX_SIMPLE_CACHE_QUEUE_SIZE 783

#define BACKLOG 256
#define BUFSIZE 512

unsigned long int cache_delay;
int nthreads;

steque_t queue;
pthread_t *pool;
pthread_mutex_t lock;
pthread_cond_t cond;

static void _sig_handler(int signo) {
  if (signo == SIGTERM || signo == SIGINT) {
    // This is where your IPC clean up should occur
    unlink(SOCK_PATH);

    for (int i = 0; i < nthreads; i++) {
      pthread_join(pool[i], NULL);
    }
    free(pool);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    steque_destroy(&queue);

    exit(signo);
  }
}

#define USAGE                                                                  \
  "usage:\n"                                                                   \
  "  simplecached [options]\n"                                                 \
  "options:\n"                                                                 \
  "  -c [cachedir]       Path to static files (Default: ./)\n"                 \
  "  -t [thread_count]   Thread count for work queue (Default is 8, Range is " \
  "1-100)\n"                                                                   \
  "  -d [delay]          Delay in simplecache_get (Default is 0, Range is "    \
  "0-2500000 (microseconds)\n "                                                \
  "  -h                  Show this help message\n"

// OPTIONS
static struct option gLongOptions[] = {
    {"cachedir", required_argument, NULL, 'c'},
    {"nthreads", required_argument, NULL, 't'},
    {"help", no_argument, NULL, 'h'},
    {"hidden", no_argument, NULL, 'i'},      /* server side */
    {"delay", required_argument, NULL, 'd'}, // delay.
    {NULL, 0, NULL, 0}};

void Usage() { fprintf(stdout, "%s", USAGE); }

void *workerfunc(void *arg) {
  while (1) {
    pthread_mutex_lock(&lock);

    while (steque_isempty(&queue)) {
      pthread_cond_wait(&cond, &lock);
    }
    int cfd = (int)(intptr_t)steque_pop(&queue);

    pthread_mutex_unlock(&lock);

    creq_t creq;
    ssize_t res = fullrecv(cfd, &creq, sizeof(creq_t));

    if (res != sizeof(creq_t)) {
      fprintf(stderr, "cache: bad request size\n");
      close(cfd);
      continue;
    }

    cresp_t resp;
    memset(&resp, 0, sizeof resp);

    int cachedfd = simplecache_get(creq.path);
    if (cachedfd < 0) {
      resp.status = -1;
      fullsend(cfd, &resp, sizeof(resp));
      close(cfd);
      continue;
    }

    int reqfd = dup(cachedfd);
    if (reqfd < 0) {
      resp.status = -1;
      fullsend(cfd, &resp, sizeof(resp));
      close(cfd);
      continue;
    }

    struct stat st;
    if (fstat(reqfd, &st) < 0) {
      resp.status = -1;
      fullsend(cfd, &resp, sizeof(resp));
      close(reqfd);
      close(cfd);
      continue;
    }

    ssize_t filesize = st.st_size;

    int shmfd = shm_open(creq.segname, O_RDWR, 0666);
    if (shmfd == -1) {
      resp.status = -1;
      fullsend(cfd, &resp, sizeof(resp));
      close(reqfd);
      close(cfd);
      continue;
    }

    void *addr =
        mmap(NULL, creq.segsize, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (addr == MAP_FAILED) {
      resp.status = -1;
      fullsend(cfd, &resp, sizeof(resp));
      close(shmfd);
      close(reqfd);
      close(cfd);
      continue;
    }

    ssize_t total = 0;
    ssize_t bytesleft = filesize - creq.offset;
    ssize_t toread;
    if (bytesleft < creq.segsize) {
      toread = bytesleft;
    } else {
      toread = creq.segsize;
    }

    while (total < toread) {
      ssize_t bytesread = pread(reqfd, (char *)addr + total, toread - total,
                                creq.offset + total);
      if (bytesread <= 0)
        break;
      total += bytesread;
    }

    resp.segbytes = total;
    resp.filesize = filesize;

    fullsend(cfd, &resp, sizeof resp);

    munmap(addr, creq.segsize);
    close(shmfd);
    close(reqfd);
    close(cfd);
  }

  return NULL;
}

int main(int argc, char **argv) {
  nthreads = 6;
  char *cachedir = "locals.txt";
  char option_char;

  /* disable buffering to stdout */
  setbuf(stdout, NULL);

  while ((option_char = getopt_long(argc, argv, "d:ic:hlt:x", gLongOptions,
                                    NULL)) != -1) {
    switch (option_char) {
    default:
      Usage();
      exit(1);
    case 't': // thread-count
      nthreads = atoi(optarg);
      break;
    case 'h': // help
      Usage();
      exit(0);
      break;
    case 'c': // cache directory
      cachedir = optarg;
      break;
    case 'd':
      cache_delay = (unsigned long int)atoi(optarg);
      break;
    case 'i': // server side usage
    case 'o': // do not modify
    case 'a': // experimental
      break;
    }
  }

  if (cache_delay > 2500000) {
    fprintf(stderr, "Cache delay must be less than 2500000 (us)\n");
    exit(__LINE__);
  }

  if ((nthreads > 100) || (nthreads < 1)) {
    fprintf(stderr, "Invalid number of threads must be in between 1-100\n");
    exit(__LINE__);
  }
  if (SIG_ERR == signal(SIGINT, _sig_handler)) {
    fprintf(stderr, "Unable to catch SIGINT...exiting.\n");
    exit(CACHE_FAILURE);
  }
  if (SIG_ERR == signal(SIGTERM, _sig_handler)) {
    fprintf(stderr, "Unable to catch SIGTERM...exiting.\n");
    exit(CACHE_FAILURE);
  }
  /*Initialize cache*/
  simplecache_init(cachedir);
  steque_init(&queue);

  // Cache should go here
  int sockfd, newfd;
  struct sockaddr_un unaddr;

  memset(&unaddr, 0, sizeof unaddr);
  unaddr.sun_family = AF_UNIX;
  strncpy(unaddr.sun_path, SOCK_PATH, sizeof(unaddr.sun_path) - 1);
  unlink(unaddr.sun_path);

  sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

  if (bind(sockfd, (struct sockaddr *)&unaddr, sizeof(unaddr)) != 0)
    return -1;

  if (listen(sockfd, BACKLOG) != 0)
    return -1;

  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond, NULL);

  pool = malloc(nthreads * sizeof(pthread_t));
  if (!pool)
    return -1;

  for (int i = 0; i < nthreads; i++) {
    pthread_create(&pool[i], NULL, workerfunc, NULL);
  }

  while (1) {
    newfd = accept(sockfd, NULL, NULL);
    if (newfd == -1) {
      continue;
    }

    pthread_mutex_lock(&lock);
    steque_enqueue(&queue, (void *)(intptr_t)newfd);
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond);
  }

  // Line never reached
  return -1;
}
