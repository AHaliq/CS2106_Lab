
/*************************************
 * Lab 3 Exercise 2
 * Name: Abdul Haliq
 * Student No: A0125431U
 * Lab Group: B-08
 *************************************/
#include <pthread.h>
#include "rw_lock.h"

#define RATIO 0.75

void release_held(rw_lock *lock)
{
  if (lock->reader_tot * RATIO < lock->writer_tot)
  {
    pthread_mutex_unlock(&(lock->holdreaders));
  }
  if (lock->reader_tot * RATIO >= lock->writer_tot)
  {
    pthread_mutex_unlock(&(lock->holdwriters));
  }
}

void release_empty(rw_lock *lock)
{
  if (lock->reader_count == 0 && lock->writer_count == 0)
  {
    pthread_mutex_unlock(&(lock->holdwriters));
    pthread_mutex_unlock(&(lock->holdreaders));
  }
}

void initialise(rw_lock *lock)
{
  pthread_mutex_init(&(lock->mutexWA), NULL);
  pthread_mutex_init(&(lock->mutexWR), NULL);
  pthread_mutex_init(&(lock->mutexRA), NULL);
  pthread_mutex_init(&(lock->mutexRR), NULL);
  pthread_mutex_init(&(lock->empty), NULL);
  pthread_mutex_init(&(lock->noreader), NULL);
  pthread_mutex_init(&(lock->holdwriters), NULL);
  pthread_mutex_init(&(lock->holdreaders), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
  lock->reader_tot = 0;
  lock->writer_tot = 0;
}

void writer_acquire(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->holdwriters));
  pthread_mutex_lock(&(lock->empty));
  pthread_mutex_lock(&(lock->mutexWA));
  lock->writer_count++;
  lock->writer_tot++;
  release_held(lock);
  pthread_mutex_unlock(&(lock->mutexWA));
}

void writer_release(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->mutexWR));
  lock->writer_count--;
  release_empty(lock);
  pthread_mutex_unlock(&(lock->mutexWR));
  pthread_mutex_unlock(&(lock->empty));
}

void reader_acquire(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->holdreaders));
  pthread_mutex_lock(&(lock->noreader));
  if (lock->reader_count == 0)
  {
    pthread_mutex_lock(&(lock->empty));
  }
  pthread_mutex_lock(&(lock->mutexRA));
  lock->reader_count++;
  lock->reader_tot++;
  release_held(lock);
  pthread_mutex_unlock(&(lock->mutexRA));
  pthread_mutex_unlock(&(lock->noreader));
}

void reader_release(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->mutexRR));
  lock->reader_count--;
  if (lock->reader_count == 0)
  {
    pthread_mutex_unlock(&(lock->empty));
  }
  release_empty(lock);
  pthread_mutex_unlock(&(lock->mutexRR));
}

void cleanup(rw_lock *lock)
{
  pthread_mutex_destroy(&(lock->mutexWA));
  pthread_mutex_destroy(&(lock->mutexWR));
  pthread_mutex_destroy(&(lock->mutexRA));
  pthread_mutex_destroy(&(lock->mutexRR));
  pthread_mutex_destroy(&(lock->empty));
  pthread_mutex_destroy(&(lock->noreader));
  pthread_mutex_destroy(&(lock->holdwriters));
  pthread_mutex_destroy(&(lock->holdreaders));
}
