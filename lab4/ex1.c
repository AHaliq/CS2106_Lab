/*************************************
 * Lab 4 Exercise 1
 * Name: Abdul Haliq
 * Student No: A0125431U
 * Lab Group: B-08
 *************************************/

// You can modify anything in this file.
// Unless otherwise stated, a line of code being present in this template
//  does not imply that it is correct/necessary!
// You can also add any global or local variables you need (e.g. to implement your page replacement algorithm).

#include <signal.h>
#include <stdio.h>
#include "api.h"

void os_run(int initial_num_pages, page_table *pg_table)
{
    // The main loop of your memory manager
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGUSR1);

    // create the pages in the disk first, because every page must be backed by the disk for this lab
    for (int i = 0; i != initial_num_pages; ++i)
    {
        disk_create(i);
    }

    /** number of frames available */
    int numofframes = 1 << FRAME_BITS;
    /** mapping of frame to page initialized to -1 (no map) */
    int frame_to_page[numofframes];
    for (int i = 0; i < numofframes; i++)
    {
        frame_to_page[i] = -1;
    }
    /** prospective chosen frame*/
    int victim_frame = 0;
    /** designated frame where requested page is to be written into */
    int chosen_frame = 0;
    /** flag to determine if frame slot has been found */
    int found_frame = 0;

    while (1)
    {
        siginfo_t info;
        sigwaitinfo(&signals, &info);
        found_frame = 0;

        // retrieve the index of the page that the user program wants, or -1 if the user program has terminated
        int const requested_page = info.si_value.sival_int;

        if (requested_page == -1)
            break;

        //custom code
        while (found_frame == 0)
        {
            int victim_page = frame_to_page[victim_frame];
            if (victim_page == -1)
            {
                //printf("FOUND EMPTY FRAME %d\n", victim_frame);
                chosen_frame = victim_frame;
                found_frame = 1;
            }
            else if (pg_table->entries[victim_page].referenced == 0)
            {
                //printf("CLEARING FRAME %d PAGE %d\n", victim_frame, victim_page);
                pg_table->entries[victim_page].valid = 0;
                pg_table->entries[victim_page].frame_index = -1;
                chosen_frame = victim_frame;
                found_frame = 1;
            }
            else
            {
                //printf("UNCHECK FRAME %d PAGE %d\n", victim_frame, victim_page);
                pg_table->entries[victim_page].referenced = 0;
            }
            //victim_frame = (victim_frame + 1) & (~(~0 << FRAME_BITS));
            victim_frame++;
            if (victim_frame == 1 << FRAME_BITS)
                victim_frame = 0;
        }
        // find frame
        //printf("ACCEPTED FRAME %d PAGE %d\n", chosen_frame, requested_page);

        disk_read(chosen_frame, requested_page);
        pg_table->entries[requested_page].frame_index = chosen_frame;
        pg_table->entries[requested_page].valid = 1;
        pg_table->entries[requested_page].referenced = 1;
        frame_to_page[chosen_frame] = requested_page;

        // tell the MMU that we are done updating the page table
        union sigval reply_value;
        reply_value.sival_int = 0; // set to 0 if the page is successfully loaded, set to 1 if the page is not mapped to the user process (i.e. segfault)
        sigqueue(info.si_pid, SIGCONT, reply_value);
    }
}
