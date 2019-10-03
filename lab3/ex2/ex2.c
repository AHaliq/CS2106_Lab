
/*************************************
 * Lab 3 Exercise 2
 * Name: Abdul Haliq
 * Student No: A0125431U
 * Lab Group: B-08
 *************************************/
#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock *lock)
{
  pthread_mutex_init(&(lock->rmutex), NULL);
  pthread_mutex_init(&(lock->wmutex), NULL);
  pthread_mutex_init(&(lock->empty), NULL);
  pthread_mutex_init(&(lock->holdreaders), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->holdreaders));
  pthread_mutex_lock(&(lock->empty));
  pthread_mutex_lock(&(lock->wmutex));
  lock->writer_count++;
  pthread_mutex_unlock(&(lock->wmutex));
}

void writer_release(rw_lock *lock)
{
  pthread_mutex_unlock(&(lock->empty));
  pthread_mutex_lock(&(lock->wmutex));
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->wmutex));
  pthread_mutex_unlock(&(lock->holdreaders));
}

void reader_acquire(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->holdreaders));
  pthread_mutex_lock(&(lock->rmutex));
  if (lock->reader_count == 0)
  {
    pthread_mutex_lock(&(lock->empty));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->rmutex));
  pthread_mutex_unlock(&(lock->holdreaders));
}

void reader_release(rw_lock *lock)
{
  pthread_mutex_lock(&(lock->rmutex));
  lock->reader_count--;
  if (lock->reader_count == 0)
  {
    pthread_mutex_unlock(&(lock->empty));
  }
  pthread_mutex_unlock(&(lock->rmutex));
}

void cleanup(rw_lock *lock)
{
  pthread_mutex_destroy(&(lock->rmutex));
  pthread_mutex_destroy(&(lock->wmutex));
  pthread_mutex_destroy(&(lock->empty));
  pthread_mutex_destroy(&(lock->holdreaders));
}
