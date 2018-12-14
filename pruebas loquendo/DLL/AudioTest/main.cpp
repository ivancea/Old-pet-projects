#include <iostream>
#include <cstring>

#include "loqtts.h"     // Loquendo TTS include file

using namespace std;

#define OUTOFMEMORY    1
#define INVALIDPARAM   2
// TODO: Add here your error codes

typedef struct {
    unsigned int SampleRate;
    ttsAudioEncodingType coding;
    ttsAudioSampleType nChannels;
    const void *pUser;
    // TODO: Add here your member data
} ChannelType;

ttsResultType GetVersion(char * strversion){
    // Please do not modify this function
    ttsGetVersionType This_is_for_checking_only;
    This_is_for_checking_only = GetVersion;
    strcpy(strversion,"7.0.0");
    return tts_OK;
}

ttsResultType Open(void **channel,const char *DeviceName, unsigned int SampleRate, ttsAudioEncodingType coding, ttsAudioSampleType nChannels, ttsBoolType *bRealTime, const void *pUser)
{
    // void **channel            [OUT] audio destination channel
    // const char *DeviceName    [IN] audio destination device name (e.g. filename)
    // unsigned int SampleRate   [IN] sampling rate in Hz
    // ttsAudioEncodingType coding [IN] audio encoding (e.g. tts_LINEAR for linear - tts_ALAW for alaw, etc.)
    // ttsAudioSampleType nCh    [IN] sample type (tts_MONO or tts_STEREO)
    // ttsBoolType *bRealTime    [OUT] realtime capability (see below)
    // const void *pUser         [IN] this is for user specific data
    ChannelType *ch;
    ttsAudioOpenType This_is_for_checking_only;
    ttsResultType r = tts_OK;
    // The following assignment is for realtime audio destinations (e.g audio boards)
    // Use ttsFALSE for NON-realtime audio destinations (e.g. audio files)
    // TODO: assign the correct value to *bRealTime
    if(NULL != bRealTime) *bRealTime = ttsTRUE;
    This_is_for_checking_only = Open;
    *channel = ch = (ChannelType *)malloc(sizeof(ChannelType));
        memset(ch,0,sizeof(ChannelType));
    if(ch == NULL)
        return OUTOFMEMORY;
    ch->pUser = pUser;
    ch->coding = coding;
    ch->nChannels = nChannels;
    ch->SampleRate = SampleRate;
    // TODO: Add here specific code for opening your audio destination
    return r;
}

ttsResultType Close(void *channel)
{
    // void *channel        [IN]    audio destination channel
    ttsResultType r = tts_OK;
    ChannelType *ch = (ChannelType*)channel;
    ttsAudioCloseType This_is_for_checking_only;
    This_is_for_checking_only = Close;
    if(ch==NULL) return tts_OK;
    // TODO: Add here specific code for closing your audio destination
    free(ch);
    return r;
}

ttsResultType PutData(void *channel,void *pData,unsigned int nBytes,unsigned int *nSent)
{
    // void *channel        [IN]    audio destination channel
    // void *pData          [IN]    audio sample buffer
    // unsigned int nBytes  [IN]    pData buffer length in bytes
    // unsigned int *nSent  [UNUSED]  do not use: reserved
    ChannelType *ch = (ChannelType*)channel;
    ttsAudioPutDataType This_is_for_checking_only;
    This_is_for_checking_only = PutData;
    if(ch==NULL) return tts_OK;
    if(nSent) *nSent = 0;
    if(nBytes!=0)
    {
        // Loquendo TTS calls this function as far as an audio buffer is ready
        // TODO: Add here specific code for sending audio samples to your audio destination

        // If you return TTS_AUDIOFULL, Loquendo TTS will call this function again, with
        // the same audio buffer as parameter, until tts_OK is returned.
        // This is the best method to slow down Loquendo TTS (e.g. to avoid your internal
        // buffer overflow)
        for(int i=0; i<nBytes; i++)
            cout << (int)((char*)pData)[i] << endl;
        return tts_OK;
    }
    else
    {
        // Loquendo TTS calls always this function once more with nBytes=0
        // after all audio samples have been sent

        // TODO: Add here specific code for managing the end of an audio streaming

        return tts_OK;
    }
}

ttsResultType Stop(void *channel)
{
    // void *channel        [IN]    audio destination channel
    ttsResultType r = tts_OK;
    ChannelType *ch = (ChannelType*)channel;
    ttsAudioStopType This_is_for_checking_only;
    This_is_for_checking_only = Stop;
    if(ch==NULL) return tts_OK;
    // TODO: Add here specific code for stopping your audio destination
    return r;
}

ttsResultType Pause(void *channel)
{
    // void *channel        [IN]    audio destination channel
    ttsResultType r = tts_OK;
    ChannelType *ch = (ChannelType*)channel;
    ttsAudioPauseType This_is_for_checking_only;
    This_is_for_checking_only = Pause;
    if(ch==NULL) return tts_OK;
    // TODO: Add here specific code for pausing your audio destination
    return r;
}

ttsResultType Resume(void *channel)
{
    // void *channel        [IN]    audio destination channel
    ttsResultType r = tts_OK;
    ChannelType *ch = (ChannelType*)channel;
    ttsAudioResumeType This_is_for_checking_only;
    This_is_for_checking_only = Resume;
    if(ch==NULL) return tts_OK;
    // TODO: Add here specific code for resuming (after pause) your audio destination
    return r;
}

ttsResultType GetErrorMessage(ttsResultType ErrorCode,char *message,unsigned int size)
{
    // ttsResultType ErrorCode  [IN]    user-defined error code
    // char *message            [OUT]   user-defined error message
    // unsigned int size        [IN]    maximum number of characters to copy

    ttsResultType r = tts_OK;
    const char *pmes = NULL;
    ttsAudioGetErrorMessageType This_is_for_checking_only;
    This_is_for_checking_only = GetErrorMessage;
    if(message == NULL) return INVALIDPARAM;
    switch(ErrorCode)
    {
    case OUTOFMEMORY: pmes = "Out of memory"; break;
    case INVALIDPARAM: pmes = "Invalid parameter"; break;
    // TODO: Add here your error strings
    default: pmes = "Unknown error"; break;
    }
    (void)strncpy(message,pmes,size-1);
    message[size-1]='\0';
    return r;
}

// THE FOLLOWING FUNCTION IS THE ONLY ENTRY POINT OF THE AUDIO DESTINATION

// TODO: Replace "AudioStub" with the name of your audio
// destination (e.g. "MyAudioDest),
// that is the name of the .DLL or .SO you are creating (without extension)
// This name must be changed also in file AudioStub.def in order to export
// this function.

extern "C" ttspFuncType AudioTest(ttsProcIdType procname, const void *pUser)
{
// Please do not modify this function's body
    static const struct {
        ttsProcIdType name;
        ttspFuncType func;
    } FuncTable[] = {
        {tts_GET_VERSION, (ttspFuncType)GetVersion},
        {TTSAUDIO_OPEN, (ttspFuncType)Open},
        {TTSAUDIO_CLOSE, (ttspFuncType)Close},
        {TTSAUDIO_PUTDATA, (ttspFuncType)PutData},
        {TTSAUDIO_STOP, (ttspFuncType)Stop},
        {TTSAUDIO_PAUSE, (ttspFuncType)Pause},
        {TTSAUDIO_RESUME, (ttspFuncType)Resume},
        {TTSAUDIO_GETERRORMESSAGE, (ttspFuncType)GetErrorMessage},
    };
    unsigned int i,size=sizeof(FuncTable)/sizeof(FuncTable[0]);
    if(procname == NULL) return NULL;
    for(i=0;i<size;i++)
    {
        if(strcmp(procname,FuncTable[i].name)==0)
            return FuncTable[i].func;
    }
    return NULL;
}

