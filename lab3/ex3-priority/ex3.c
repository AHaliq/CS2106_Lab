/*************************************
 * Lab 3 Exercise 3
 * Name: Abdul Haliq
 * Student No: A0125431U
 * Lab Group: B-08
 *************************************/

#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from ex3_runner.c in your code.
extern int num_of_cars;
extern int num_of_segments;
segment_struct *S;

sem_t roundabout_counting;

void initialise()
{
    sem_init(&roundabout_counting, 0, num_of_segments - 1);
    S = malloc(sizeof(segment_struct) * num_of_segments);
    for (int i = 0; i < num_of_segments; i++)
    {
        sem_init(&S[i].cars_in_seg_mutex, 0, 1);
        sem_init(&S[i].entry_mutex, 0, 1);
    }
}

void cleanup()
{
    sem_destroy(&roundabout_counting);
    for (int i = 0; i < num_of_segments; i++)
    {
        sem_destroy(&(S[i].cars_in_seg_mutex));
        sem_destroy(&(S[i].entry_mutex));
    }
    free(S);
}

void *car(void *car)
{
    car_struct *c = (struct car_struct *)car;
    //This function modeles a thread
    //A car:
    //   -should call enter_roundabout (...)
    //   -followed by some calls to move_to_next_segment (...)
    //   -finally call exit_roundabout (...)

    // duplicate segment
    sem_wait(&roundabout_counting);
    sem_wait(&S[c->entry_seg].entry_mutex); // barrier before entering roundabout
    sem_wait(&S[c->entry_seg].cars_in_seg_mutex);
    enter_roundabout(c);
    while (c->current_seg != c->exit_seg)
    {
        int old = c->current_seg;
        sem_wait(&S[NEXT(old, num_of_segments)].cars_in_seg_mutex);
        move_to_next_segment(c);

        int v;
        sem_getvalue(&S[old].cars_in_seg_mutex, &v);
        if (v >= 0)
        {
            sem_post(&S[old].entry_mutex);
        } // barrier only opens when no one in queue to enter segment (cars already in roundabout)

        sem_post(&S[old].cars_in_seg_mutex);
    }
    if (c->current_seg == c->exit_seg)
    {
        int old = c->current_seg;
        exit_roundabout(c);
        sem_post(&S[old].cars_in_seg_mutex);
        sem_post(&roundabout_counting);
    }
    pthread_exit(NULL);
}
