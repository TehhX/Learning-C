// This file requires the portaudio library to be linked and the header file to be included to be ran. Multiple parts are taken from the portaudio docs and examples.
// WARNING: This program may produce an incredibly loud sound when run. Turn your volume down to <= 15% beforehand.

#include "portaudio.h"

#include "stdio.h"
#include "string.h"

typedef struct
{
    float left_phase;
    float right_phase;
}
__attribute__((packed)) paTestData;

static paTestData g_data;

static int patestCallback
(
    __attribute__((unused)) const void *inputBuffer,
    void *outputBuffer,
    unsigned long framesPerBuffer,
    __attribute__((unused)) const PaStreamCallbackTimeInfo* timeInfo,
    __attribute__((unused)) PaStreamCallbackFlags statusFlags,
    void *userData
)
{
    paTestData *data = userData;
    float *out = outputBuffer;

    for(unsigned int i = 0; i < framesPerBuffer; i++ )
    {
        *(out++) = data->left_phase;
        *(out++) = data->right_phase;

        // Increase left, if gr/eq max, reset. Will create a sawtooth shape if viewed on an osciloscope or similar.
        if((data->left_phase += 0.01f) >= 1.0f)
        {
            data->left_phase = -1.0f;
        }

        // Increase right, if gr/eq max, reset. Will create a sawtooth shape if viewed on an osciloscope or similar.
        if((data->right_phase += 0.03f) >= 1.0f)
        {
            data->right_phase = -1.0f;
        }
    }

    return 0;
}

int main()
{
    if (Pa_Initialize() != paNoError)
    {
        puts("Error while initializing PortAudio.");
        return 1;
    }

    PaStreamParameters output_parameters =
    {
        .channelCount = 2, // Mono == 1, Stereo == 2
        .hostApiSpecificStreamInfo = NULL, // No idea
        .sampleFormat = paFloat32, // Format of data, in this case two 32bit floats, one for each channel
        .device = -1 // Device -1 for finding device later
    };

    for (int i = Pa_GetDeviceCount() - 1; i >= 0; --i)
    {
        // Look for pulseaudio device
        if (!strcmp(Pa_GetDeviceInfo(i)->name, "pulse"))
        {
            output_parameters.device = i;
            break;
        }
    }

    if (output_parameters.device == -1)
    {
        puts("Couldn't find pulseaudio output device. Trying default output.");

        output_parameters.device = Pa_GetDefaultOutputDevice();
    }

    output_parameters.suggestedLatency = Pa_GetDeviceInfo(output_parameters.device)->defaultLowOutputLatency;

    PaStream *stream;
    if (paNoError != Pa_OpenStream
    (
        &stream, // Stream pointer
        NULL, // No input, as this is not a mic
        &output_parameters, // Stereo output, so 2 channels
        44100, // Sample rate (Hz)
        paFramesPerBufferUnspecified, // Frames per buffer, leave unspecified to let Pa choose.
        paNoFlag, // Can choose different flags in this argument
        patestCallback, // Callback function defined & declared above
        &g_data // Pointer to global data variable
    ))
    {
        puts("Error opening stream.");
        return 1;
    }

    if (paNoError != Pa_StartStream(stream))
    {
        puts("Couldn't start the stream.");
        return 1;
    }

    // Sleep for 5 seconds (5000ms) to let audio play before closing
    Pa_Sleep(5000);

    if (paNoError != Pa_StopStream(stream))
    {
        puts("Failed to stop stream.");
        return 1;
    }

    if (paNoError != Pa_CloseStream(stream))
    {
        puts("Failed to close stream.");
        return 1;
    }

    if (paNoError != Pa_Terminate())
    {
        puts("Failed to terminate portaudio.");
        return 1;
    }
}
