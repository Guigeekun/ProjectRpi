#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#define NUM_SECONDS   (5)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif


#define HARM (10) //nb d'harmonique à calculer

#define TABLE_SIZE   (200)
typedef struct
{
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    char message[20];
}
paTestData;

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned long i;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;
    
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->sine[data->left_phase];  /* left */
        *out++ = data->sine[data->right_phase];  /* right */
        data->left_phase += 1;
        if( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
        data->right_phase += 3; /* higher pitch so we can distinguish left and right. */
        if( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
    }
    
    return paContinue;
}

/*
 * This routine is called by portaudio when playback is done.
 */
static void StreamFinished( void* userData )
{
   paTestData *data = (paTestData *) userData;
   //printf( "Stream Completed: %s\n", data->message );
}


float GenerateTone(float,unsigned*,float);
/*
 * Permet de créer un sinus avec pour paramétre sa fréquence
 */
float GenerateTone(float frequency, unsigned* bufferLen,float AMP){
    float tone;
    unsigned duration=0.1; //on fixe la durée tres courte pour que le fait de rester appuyé fasse pérsister le son
    const float freq = frequency/SAMPLE_RATE; 

    const unsigned len = SAMPLE_RATE * duration; 
    for(int i = 0; i < len; i++ )
        tone =(AMP * sin(2*M_PI*freq  * ((float)i)/SAMPLE_RATE));
        return tone;
}   

/*******************************************************************/
int main(void);
int main(void)
{
    PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    paTestData data; //ce qu'on va jouer comme sons
    int i;
    unsigned len;

    printf("SynthPi v0 test\n");

    /* initialise sinusoidal wavetable */
    float sound;
    for( i=0; i<TABLE_SIZE; i++ )
    {
        for(int k;k<HARM;k++){
            data.sine[i] += GenerateTone(440,len,0.8/k);
        }
    }
    data.left_phase = data.right_phase = 0;
    
    err = Pa_Initialize(); //Init de Portaudio
    if( err != paNoError ) goto error; //si erreur goto 

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* périph par défaut */
    if (outputParameters.device == paNoDevice) { //si pas de périph par défaut (cette erreur veut souvent dire que PortAudio s'est mal installé, merci de suivre le Readme)
      fprintf(stderr,"Error: No default output device.\n");
      goto error;
    }
    outputParameters.channelCount = 2;       /* mode stereo (support pour du 5.0 ou 7.1 à l'avenir ?) */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream( //ouverture du stream
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              patestCallback,
              &sound );
    if( err != paNoError ) goto error;

    sprintf( data.message, "No Message" );
    err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;

    printf("Play for %d seconds.\n", NUM_SECONDS );
    Pa_Sleep( NUM_SECONDS * 1000 );

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;

    Pa_Terminate();
    printf("Test finished.\n");
    
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}
