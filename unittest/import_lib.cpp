#ifdef _DEBUG
#pragma comment(lib,"../build/winx/Debug/audio_device.lib")
#pragma comment(lib,"../build/winx/Debug/audio_effect.lib")
#pragma comment(lib,"../build/winx/Debug/audio_io.lib")
#pragma comment(lib,"../build/winx/Debug/audio_base.lib")
#pragma comment(lib,"../build/winx/Debug/audio_processing.lib")
#pragma comment(lib,"../build/winx/Debug/libmpg123.lib")
#pragma comment(lib,"../build/winx/Debug/libmp3lame.lib")
#pragma comment(lib,"../build/winx/Debug/aac.lib")
#pragma comment(lib,"../build/winx/Debug/g7221.lib")
#pragma comment(lib,"../build/winx/Debug/opus.lib")
//#pragma comment(lib,"../effect/speechtotext/winx/lib/msc.lib")
#else
#pragma comment(lib,"../build/winx/Release/audio_device.lib")
#pragma comment(lib,"../build/winx/Release/audio_effect.lib")
#pragma comment(lib,"../build/winx/Release/audio_io.lib")
#pragma comment(lib,"../build/winx/Release/audio_base.lib")
#pragma comment(lib,"../build/winx/Release/audio_processing.lib")
#pragma comment(lib,"../build/winx/Release/libmpg123.lib")
#pragma comment(lib,"../build/winx/Release/libmp3lame.lib")
#pragma comment(lib,"../build/winx/Release/aac.lib")
#pragma comment(lib,"../build/winx/Release/g7221.lib")
#pragma comment(lib,"../build/winx/Release/opus.lib")
#pragma comment(linker,"/nodefaultlib:LIBCMT.lib")
//#pragma comment(lib,"../effect/speechtotext/ifly/winx/lib/msc.lib")
#endif
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "strmiids")
#pragma comment(lib, "msdmo")
#pragma comment(lib, "dmoguids")
#pragma comment(lib, "wmcodecdspuuid")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")
