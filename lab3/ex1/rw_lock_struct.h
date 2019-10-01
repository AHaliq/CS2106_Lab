/*************************************
 * Lab 3 Exercise 1
 * Name: Abdul Haliq
 * Student No: A0125431U
 * Lab Group: B-08
 *************************************/

#include <pthread.h>

typedef struct
{
  pthread_mutex_t mutexWA;
  pthread_mutex_t mutexWR;
  pthread_mutex_t mutexRA;
  pthread_mutex_t mutexRR;
  pthread_mutex_t empty;
  pthread_mutex_t noreader;
  int reader_count;
  int writer_count;
} rw_lock;
