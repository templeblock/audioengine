#include "include/audioencoder.h"
#include "g7721encoder.h"
#include "aacencoder.h"
AudioEncoder*AudioEncoder::Create( AudioFileType type,int samplerate,int16_t channel,int bitrate )
{
    AudioEncoder* pEncoder = nullptr;
    switch ( type )
    {
    case AFT_DEFAULT:
        break;
    case AFT_PCM:
        break;
    case AFT_WAV:
        break;
    case AFT_MP3:
        break;
    case AFT_AAC:
        pEncoder = new AACEncoder( samplerate, channel, bitrate );
        break;
    case AFT_G7221:
        pEncoder = new G7221Encoder(samplerate,channel, bitrate);
        break;
    default:
        break;
    }

    return pEncoder;
}
