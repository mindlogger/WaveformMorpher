#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "GlobalDefinitions.hpp"

#include "UI.hpp"
#include "FbGraphics.hpp"
#include "Touchscreen.hpp"

timer_t t1,t2,t3,t4;

static void timer3hz()
{
    if(!touch_is_happening_flag)
    {
        //clearTouchscreenList(); //TODO TEST IF THIS IS ACTUALLY NECCEARY SINCE HAPPENS AFTER PROCESSING
    }
}

static void timer18hz()
{
    postScreenSem();
}

static void timerHandler( int sig, siginfo_t *si, void *uc )
{
    timer_t *tidp;
    tidp = (void**) si->si_value.sival_ptr;

    if ( *tidp == t1 )
        timer3hz();
    else if ( *tidp == t2 )
        timer18hz();
    //else if ( *tidp == t3 )
        //handle_input();
    //else if ( *tidp == t4 )
        //printf("FOUR\n");
    //flush(0);
}

static int makeTimer( char *name, timer_t *timerID, int expireMS, int intervalMS )
{
    struct sigevent         te;
    struct itimerspec       its;
    struct sigaction        sa;
    int                     sigNo = SIGRTMIN;

    /* Set up signal handler. */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timerHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(sigNo, &sa, NULL) == -1)
    {
        fprintf(stderr, "Failed to setup signal handling for %s.\n",name);
        return(-1);
    }

    /* Set and enable alarm */
    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = sigNo;
    te.sigev_value.sival_ptr = timerID;
    timer_create(CLOCK_REALTIME, &te, timerID);

    its.it_interval.tv_sec = intervalMS / 1000;
    its.it_interval.tv_nsec = (intervalMS % 1000)*1000000;
    its.it_value.tv_sec = expireMS / 1000;
    its.it_value.tv_nsec = (expireMS % 1000)*1000000;
    timer_settime(*timerID, 0, &its, NULL);

    return(0);
}

void setupTimer()
{
    makeTimer("First Timer", &t1, 300, 300);
    makeTimer("Second Timer", &t2, 200, 200);
    //makeTimer("Third Timer", &t3, 1000, 1000);
    //makeTimer("Fourth Timer", &t4, 300, 300);
}

void endTimer()
{

}
