#include <signal.h>
#include <iostream>
#include <sys/time.h>

#include <stdio.h>
#include <string.h>

#include "GlobalDefinitions.hpp"

#include "FbGraphics.hpp"
#include "FourierTransformer.hpp"
#include "UI.hpp"

timer_t t1,t2,t3,t4;

static void timer3hz()//TODO NAME THESE CORREECTLY
{
    postTransformerSem();
}

static void timer18hz()//TODO NAME THESE CORREECTLY
{
    postScreenSem();
}

static void timer1hz()//TODO NAME THESE CORREECTLY
{
    postUISem();
}

static void debugTimer()
{
}

static void timerHandler( int sig, siginfo_t *si, void *uc )
{
    timer_t *tidp;
    tidp = (void**) si->si_value.sival_ptr;

    if ( *tidp == t1 )
        timer3hz();
    else if ( *tidp == t2 )
        timer18hz();
    else if ( *tidp == t3 )
        timer1hz();
    else if ( *tidp == t4 )
        debugTimer();
}

static int makeTimer(timer_t *timerID, int expireMS, int intervalMS )
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
        fprintf(stderr, "Failed to setup signal handling for a timer.\n");
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
    makeTimer(&t1, 500, 500);//Transformer Timer
    makeTimer(&t2, 200, 200);//Screen Timer
    makeTimer(&t3, 100, 100);//UI Timer
    //makeTimer("Fourth Timer", &t4, 100, 100);
}

void endTimer()//TODO TURN TIMERS OFF
{

}
