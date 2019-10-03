/*************************************
 * Lab 3 Exercise 2
 * Name: Abdul Haliq
 * Student No: A0125431U
 * Lab Group: B-08
 *************************************/

#include <pthread.h>

typedef struct
{
  pthread_mutex_t rmutex;
  pthread_mutex_t wmutex;
  pthread_mutex_t empty;
  pthread_mutex_t holdreaders;
  int reader_count;
  int writer_count;
} rw_lock;
