#include "./cache-student.h"
#include "./gfserver.h"
#include "./shm_channel.h"
#include "steque.h"
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUFSIZE 1024

ssize_t handle_with_cache(gfcontext_t *ctx, char *path, void *arg) {
  shmpool_t *pool = (shmpool_t *)arg;
  size_t bytestransferred = 0;
  struct sockaddr_un unaddr;

  memset(&unaddr, 0, sizeof unaddr);
  unaddr.sun_family = AF_UNIX;
  strncpy(unaddr.sun_path, SOCK_PATH, sizeof(unaddr.sun_path));

  creq_t creq;
  memset(&creq, 0, sizeof creq);
  strncpy(creq.path, path, sizeof creq.path);

  pthread_mutex_lock(&pool->lock);

  shmseg_t *seg;
  while (steque_isempty(&pool->shmqueue)) {
    pthread_cond_wait(&pool->cond, &pool->lock);
  }
  seg = (shmseg_t *)steque_pop(&pool->shmqueue);

  pthread_mutex_unlock(&pool->lock);

  memset(seg->addr, 0, seg->size);

  strncpy(creq.segname, seg->name, sizeof creq.segname);
  creq.segsize = seg->size;
  creq.offset = 0;

  int sockfd = -1;

  while (1) {
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
      usleep(10000);
      continue;
    }

    if (connect(sockfd, (struct sockaddr *)&unaddr, sizeof unaddr) == 0) {
      break;
    }

    close(sockfd);
    sockfd = -1;
    usleep(10000);
  }

  if (fullsend(sockfd, &creq, sizeof creq) != sizeof creq) {
    close(sockfd);
    gfs_sendheader(ctx, GF_FILE_NOT_FOUND, 0);
    pthread_mutex_lock(&pool->lock);
    steque_enqueue(&pool->shmqueue, seg);
    pthread_mutex_unlock(&pool->lock);
    pthread_cond_broadcast(&pool->cond);
    return 0;
  }

  cresp_t cresp;
  ssize_t recvbytes = fullrecv(sockfd, &cresp, sizeof cresp);

  close(sockfd);

  if (recvbytes != sizeof(cresp)) {
    gfs_sendheader(ctx, GF_FILE_NOT_FOUND, 0);
    pthread_mutex_lock(&pool->lock);
    steque_enqueue(&pool->shmqueue, seg);
    pthread_mutex_unlock(&pool->lock);
    pthread_cond_broadcast(&pool->cond);
    return 0;
  }

  if (cresp.status == -1) {
    gfs_sendheader(ctx, GF_FILE_NOT_FOUND, 0);
    pthread_mutex_lock(&pool->lock);
    steque_enqueue(&pool->shmqueue, seg);
    pthread_mutex_unlock(&pool->lock);
    pthread_cond_broadcast(&pool->cond);
    return 0;
  }

  gfs_sendheader(ctx, GF_OK, cresp.filesize);

  ssize_t fileoffset = 0;

  while (1) {
    ssize_t segoffset = 0;

    while (segoffset < cresp.segbytes) {
      ssize_t tosend = cresp.segbytes - segoffset;
      if (tosend > BUFSIZE)
        tosend = BUFSIZE;

      ssize_t byteswritten =
          gfs_send(ctx, (char *)seg->addr + segoffset, tosend);

      if (byteswritten < 1) {
        pthread_mutex_lock(&pool->lock);
        steque_enqueue(&pool->shmqueue, seg);
        pthread_mutex_unlock(&pool->lock);
        pthread_cond_broadcast(&pool->cond);
        return bytestransferred;
      }

      segoffset += byteswritten;
      bytestransferred += byteswritten;
    }

    fileoffset += cresp.segbytes;

    if (fileoffset >= cresp.filesize) {
      break;
    }

    creq.offset = fileoffset;

    sockfd = -1;

    while (1) {
      sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
      if (sockfd == -1) {
        continue;
      }

      if (connect(sockfd, (struct sockaddr *)&unaddr, sizeof unaddr) == 0) {
        break;
      }

      close(sockfd);
      sockfd = -1;
    }

    if (fullsend(sockfd, &creq, sizeof creq) != sizeof creq) {
      pthread_mutex_lock(&pool->lock);
      steque_enqueue(&pool->shmqueue, seg);
      pthread_mutex_unlock(&pool->lock);
      pthread_cond_broadcast(&pool->cond);
      close(sockfd);
      return bytestransferred;
    }

    recvbytes = fullrecv(sockfd, &cresp, sizeof cresp);

    close(sockfd);

    if (recvbytes != sizeof(cresp) || cresp.status == -1) {
      pthread_mutex_lock(&pool->lock);
      steque_enqueue(&pool->shmqueue, seg);
      pthread_mutex_unlock(&pool->lock);
      pthread_cond_broadcast(&pool->cond);
      return bytestransferred;
    }
  }

  pthread_mutex_lock(&pool->lock);
  steque_enqueue(&pool->shmqueue, seg);
  pthread_mutex_unlock(&pool->lock);
  pthread_cond_broadcast(&pool->cond);

  return bytestransferred;
}
