#pragma once
#include "../include/audio_device.h"
#include <vector>
#include <dmo.h>
#include <Mmsystem.h>
#include <objbase.h>
#include <mediaobj.h>
#include <uuids.h>
#include <propidl.h>
#include <wmcodecdsp.h>

#include <atlbase.h>
#include <ATLComCli.h>
#include <audioclient.h>
#include <MMDeviceApi.h>
#include <AudioEngineEndPoint.h>
#include <DeviceTopology.h>
#include <propkey.h>
#include <strsafe.h>
#include <conio.h>
#include <MMDeviceApi.h>
#include <AudioEngineEndPoint.h>
#include <DeviceTopology.h>
#include <EndpointVolume.h>

#include "media_buffer.h"
#include <avrt.h>            // Avrt
#include <mutex>
// Use Multimedia Class Scheduler Service (MMCSS) to boost the thread priority
#pragma comment( lib, "avrt.lib" )
// AVRT function pointers
typedef BOOL( WINAPI *PAvRevertMmThreadCharacteristics )( HANDLE );
typedef HANDLE( WINAPI *PAvSetMmThreadCharacteristicsA )( LPCSTR, LPDWORD );
typedef BOOL( WINAPI *PAvSetMmThreadPriority )( HANDLE, AVRT_PRIORITY );


typedef struct
{
    KSPROPERTY KsProperty;
    BOOLEAN bEndpointFlag;
    ULONG ulEntityId;
    union {
        ULONG ulEndpoint;
        ULONG ulInterface;
    };
    ULONG ulOffset;
} USBAUDIO_MEMORY_PROPERTY, *PUSBAUDIO_MEMORY_PROPERTY;

static const GUID USB_AUDIO_PROP_SET_GUID =
{ 0xC3FA16D7, 0x274E, 0x4f2b,
{ 0xA6, 0x3B, 0xD5, 0xE1, 0x09, 0x55, 0xFA, 0x27 } };
const DWORD USBAUDIO_PROPERTY_GETSET_MEM = 0;

#define MAX_STR_LEN 512
typedef struct
{
    wchar_t szDeviceName[MAX_STR_LEN];
    wchar_t szDeviceID[MAX_STR_LEN];
    bool bIsMicArrayDevice;
    bool bDefaultDevice;
} AUDIO_DEVICE_INFO, *PAUDIO_DEVICE_INFO;
typedef std::vector<AUDIO_DEVICE_INFO>  AUDIO_DEVICE_INFO_LIST;

// Utility class which initializes COM in the constructor (STA or MTA),
// and uninitializes COM in the destructor.
class ScopedCOMInitializer {
public:
    // Enum value provided to initialize the thread as an MTA instead of STA.
    enum SelectMTA { kMTA };

    // Constructor for STA initialization.
    ScopedCOMInitializer()
    {
        Initialize( COINIT_APARTMENTTHREADED );
    }

    // Constructor for MTA initialization.
    explicit ScopedCOMInitializer( SelectMTA /*mta*/ )
    {
        Initialize( COINIT_MULTITHREADED );
    }

    ScopedCOMInitializer::~ScopedCOMInitializer()
    {
        if ( SUCCEEDED( hr_ ) )
            CoUninitialize();
    }

    bool succeeded() const { return SUCCEEDED( hr_ ); }

private:
    void Initialize( COINIT init )
    {
        hr_ = CoInitializeEx( NULL, init );
    }

    HRESULT hr_;

    ScopedCOMInitializer( const ScopedCOMInitializer& );
    void operator=( const ScopedCOMInitializer& );
};



class WindowsCoreAudio:public AudioDevice
{
    typedef std::lock_guard<std::mutex> lock_guard;
public:


    WindowsCoreAudio();
    virtual ~WindowsCoreAudio();
    virtual bool Initialize();
    virtual void Terminate();
    virtual size_t GetRecordingDeviceNum()const;
    virtual size_t GetPlayoutDeviceNum()const;

    virtual bool GetPlayoutDeviceName(
        int16_t index,
        wchar_t name[kAdmMaxDeviceNameSize],
        wchar_t guid[kAdmMaxGuidSize] );
    virtual bool RecordingDeviceName(
        int16_t index,
        wchar_t name[kAdmMaxDeviceNameSize],
        wchar_t guid[kAdmMaxGuidSize] );

    virtual bool SetPlayoutDevice( int16_t index );
    virtual bool SetRecordingDevice( int16_t index );

    virtual bool IsRecordingFormatSupported( uint32_t nSampleRate, uint16_t nChannels );
    virtual bool IsPlayoutFormatSupported( uint32_t nSampleRate, uint16_t nChannels );

    virtual bool SetRecordingFormat( uint32_t nSampleRate, uint16_t nChannels );
    virtual bool SetPlayoutFormat( uint32_t nSampleRate, uint16_t nChannels );
    virtual bool GetRecordingFormat( uint32_t& nSampleRate, uint16_t& nChannels );
    virtual bool GetPlayoutFormat( uint32_t& nSampleRate, uint16_t& nChannels );

    virtual bool InitPlayout();
    virtual bool InitRecording();

    virtual bool StartPlayout();
    virtual bool StopPlayout();
    virtual bool Playing() const;
    virtual bool StartRecording();
    virtual bool StopRecording();
    virtual bool Recording() const;

    virtual void SetAudioBufferCallback( AudioBufferProc* pCallback );
private:
    HRESULT DeviceBindTo( EDataFlow eDataFlow, /* eCapture/eRender */
                          int16_t iDevIdx, /* Device Index. -1 - default device. */
                          IAudioClient **ppAudioClient, /* pointer pointer to IAudioClient interface */
                          IAudioEndpointVolume **ppEndpointVolume,
                          WCHAR** ppszEndpointDeviceId ); // Device ID. Need to be freed in caller with CoTaskMemoryFree if it is not NULL;
    HRESULT GetDeviceNum( EDataFlow eDataFlow, UINT &uDevCount )const;
    HRESULT EnumDevice( EDataFlow eDataFlow, AUDIO_DEVICE_INFO_LIST& devices )const;
    HRESULT DeviceIsMicArray( wchar_t szDeviceId[], bool &bIsMicArray )const;
    HRESULT EndpointIsMicArray( IMMDevice* pEndpoint, bool & isMicrophoneArray )const;
    HRESULT GetJackSubtypeForEndpoint( IMMDevice* pEndpoint, GUID* pgSubtype )const;
    HRESULT GetInputJack( IMMDevice* pDevice, CComPtr<IPart>& spPart );
    HRESULT GetMicArrayGeometry( wchar_t szDeviceId[], KSAUDIO_MIC_ARRAY_GEOMETRY** ppGeometry, ULONG& cbSize );
    HRESULT SetDMOProperties();
    HRESULT SetBoolProperty( IPropertyStore* ptrPS, REFPROPERTYKEY key, VARIANT_BOOL value );
    HRESULT SetVtI4Property( IPropertyStore* ptrPS, REFPROPERTYKEY key, LONG value );
    HRESULT InitRecordingMedia();
    bool    InitRecordingDMO();
    HANDLE  SetThreadPriority( const char* thread_name );
    void    RevertThreadPriority( HANDLE hMmTask );
    bool    IsFormatSupported( CComPtr<IAudioClient> audioClient, DWORD nSampleRate, WORD nChannels );

    static DWORD WINAPI WSAPIRenderThread( LPVOID context );
    static DWORD WINAPI WSAPICaptureThreadPollDMO( LPVOID context );
    static DWORD WINAPI WSAPICaptureThread( LPVOID context );
    DWORD DoRenderThread();
    DWORD DoCaptureThreadPollDMO();
    DWORD DoCaptureThread();
private:
    ScopedCOMInitializer                    m_comInit;
    std::mutex                              m_audiolock;
    CComPtr<IMediaObject>                   m_dmo;// DirectX Media Object (DMO) for the built-in AEC.
    CComPtr<IMediaBuffer>                   m_mediaBuffer;
    CComPtr<IAudioClient>                   m_spClientIn;
    CComPtr<IAudioClient>                   m_spClientOut;
    CComPtr<IAudioRenderClient>             m_spRenderClient;
    CComPtr<IAudioCaptureClient>            m_spCaptureClient;
    CComPtr<IMediaBuffer>                   m_pMediaBuffer;

    AudioBufferProc*                        m_pBufferProc;
    size_t                                  m_recSampleRate;
    size_t                                  m_plySampleRate;
    uint16_t                                m_recChannels;
    uint16_t                                m_plyChannels;
    int16_t                                 m_inDevIndex;
    int16_t                                 m_outDevIndex;
    int16_t                                 m_inDefaultDevIndex;
    int16_t                                 m_outDetaultDevIndex;

    bool                                    m_bUseDMO;
    bool                                    m_DMOIsAvailble;
   
    bool                                    m_bInitialize;
    bool                                    m_recIsInitialized;
    bool                                    m_playIsInitialized;
    bool                                    m_recording;
    bool                                    m_playing;


    HANDLE                                  m_hRenderSamplesReadyEvent;
    HANDLE                                  m_hPlayThread;
    HANDLE                                  m_hRenderStartedEvent;
    HANDLE                                  m_hShutdownRenderEvent;

    HANDLE                                  m_hCaptureSamplesReadyEvent;
    HANDLE                                  m_hRecThread;
    HANDLE                                  m_hCaptureStartedEvent;
    HANDLE                                  m_hShutdownCaptureEvent;


  


    AUDIO_DEVICE_INFO_LIST                  m_CaptureDeviceList;
    AUDIO_DEVICE_INFO_LIST                  m_RenderDeviceList;



};