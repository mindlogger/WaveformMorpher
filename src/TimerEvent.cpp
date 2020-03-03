#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <sys/time.h>

static void timer1khz(int signum)
{
    //EVERYTHING KINDA PERIODIC LIKE: AMP ADSR, WAVETABLE ADSR, FILTER COEF
}

void setupTimer()
{
    itimerval tv;
    tv.it_interval.tv_sec = 0;  //defines interval
    tv.it_interval.tv_usec = 1000;  // when timer expires, reset to 100ms
    tv.it_value.tv_sec = 0;     //defines starting pos (sec till next interrupt)
    tv.it_value.tv_usec = 1000;   // 100 ms == 100000 us
    setitimer(ITIMER_REAL, &tv, NULL);
    signal(SIGALRM,&timer1khz); //attach interrupt
}

void endTimer()
{
    itimerval tv;
    tv.it_interval.tv_sec = 0;  //defines interval
    tv.it_interval.tv_usec = 0;
    tv.it_value.tv_sec = 0;     //defines starting pos (sec till next interrupt)
    tv.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &tv, NULL); //All set to 0 to stop timer
    signal(SIGALRM,&timer1khz); //attach interrupt

    //signal(SIGALRM,SIG_IGN); //THIS DISATTACHES THE HANDLER FROM THE SIGNAL TODO MAYBE BETTER TO TURN TIMER OFF
}
