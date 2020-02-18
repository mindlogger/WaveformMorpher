#include "WaveOsc.hpp"
#include "portaudio.h"
#include "GlobalDefinitions.hpp"
#include <sys/types.h>
#include <stdio.h>
#include <iostream>

typedef struct
{
    float left_phase;
    float right_phase;
}
paTestData;

static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    /* Cast data passed through stream to our structure. */
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    //unsigned int i;

    (void) inputBuffer; /* Prevent unused variable warning. */

    *out++ = data->left_phase;  /* left */
    *out++ = data->right_phase;  /* right */
    /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
    float x = getWavetableValue() * master_gain;
    data->left_phase = x;
    data->right_phase = x;
    return 0;
}

static paTestData data;
void initAudio()
{
    master_gain = 0;

    PaStream *stream;
    PaError err;

    printf("PortAudio initialized");
    /* Initialize our data for use by callback. */
    data.left_phase = data.right_phase = 0.0;
    /* Initialize library before making any other calls. */
    err = Pa_Initialize();
    if( err != paNoError ) /*goto error*/;

    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                0,          /* no input channels */
                                2,          /* stereo output */
                                paFloat32,  /* 32 bit floating point output */
                                SAMPLE_RATE,
                                1,        /* frames per buffer */
                                patestCallback,
                                &data );
    if( err != paNoError ) /*goto error*/;

    err = Pa_StartStream( stream );
    if( err != paNoError ) /*goto error*/;


    // = Pa_StopStream( stream );
    //if( err != paNoError ) /*goto error*/;
    //err = Pa_CloseStream( stream );
    //if( err != paNoError ) /*goto error*/;
    //Pa_Terminate();
    //printf("Test finished.\n");
    //return err;
    /*error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );*/



}
