
/*************************************
 * Lab 3 Exercise 1
 * Name: Abdul Haliq
 * Student No: A0125431U
 * Lab Group: B-08
 *************************************/
#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock *lock)
{
  pthread_mutex_init(&(lock->mutexW), NULL);
  pthread_mutex_init(&(lock->mutexR), NULL);
  pthread_mutex_init(&(lock->empty), NULL);
  pthread_mutex_init(&(lock->noreader), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->empty));
  pthread_mutex_lock(&(lock->mutexW));
  lock->writer_count++;
  pthread_mutex_unlock(&(lock->mutexW));
}

void writer_release(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->mutexW));
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->mutexW));
  pthread_mutex_unlock(&(lock->empty));
}

void reader_acquire(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->noreader));
  if (lock->reader_count == 0)
  {
    pthread_mutex_lock(&(lock->empty));
  }
  pthread_mutex_lock(&(lock->mutexR));
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->mutexR));
  pthread_mutex_unlock(&(lock->noreader));
}

void reader_release(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->mutexR));
  lock->reader_count--;
  if (lock->reader_count == 0)
  {
    pthread_mutex_unlock(&(lock->empty));
  }
  pthread_mutex_unlock(&(lock->mutexR));
}

void cleanup(rw_lock *lock)
{
  pthread_mutex_destroy(&(lock->mutexW));
  pthread_mutex_destroy(&(lock->mutexR));
  pthread_mutex_destroy(&(lock->empty));
  pthread_mutex_destroy(&(lock->noreader));
}
