/*************************************
 * Lab 3 Exercise 2
 * Name: Abdul Haliq
 * Student No: A0125431U
 * Lab Group: B-08
 *************************************/

#include <pthread.h>

typedef struct
{
  pthread_mutex_t mutexW;
  pthread_mutex_t mutexR;
  pthread_mutex_t empty;
  pthread_mutex_t noreader;
  pthread_mutex_t holdwriters;
  pthread_mutex_t holdreaders;
  int reader_count;
  int writer_count;
  int reader_tot;
  int writer_tot;
} rw_lock;
