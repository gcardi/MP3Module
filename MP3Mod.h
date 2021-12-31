//---------------------------------------------------------------------------

#ifndef MP3ModH
#define MP3ModH

#include <System.hpp>
#include <strmif.h>
#include <control.h>

class Mp3
{
public:
    Mp3();
    ~Mp3();

    void LoadFromFile( String FileName );
    void Cleanup();

    void Play();
    void Pause();
    void Stop();

    bool WaitForCompletion( long TimeoutMS, long* pEventCode = nullptr );

    void SetVolume( float dB );

    float GetVolume(); // dB

    __int64 GetDuration() const { // 1/10 millionth of a second
        return duration_;
    }

    __int64 GetCurrentPosition(); // 1/10 millionth of a second

    // Current and Stop in 1/10 millionth of a second
    void SetPositions( __int64 Current, __int64 Stop, bool AbsolutePos );

private:
    using _di_IGraphBuilder = DelphiInterface<IGraphBuilder>;
    using _di_IMediaControl = DelphiInterface<IMediaControl>;
    using _di_IMediaEventEx = DelphiInterface<IMediaEventEx>;
    using _di_IBasicAudio = DelphiInterface<IBasicAudio>;
    using _di_IMediaSeeking = DelphiInterface<IMediaSeeking>;

    _di_IGraphBuilder gb_;
    _di_IMediaControl mc_;
    _di_IMediaEventEx mex_;
    _di_IBasicAudio ba_;
    _di_IMediaSeeking ms_;
    __int64 duration_ {};

    void Reset();

};

//---------------------------------------------------------------------------
#endif
