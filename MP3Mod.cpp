//---------------------------------------------------------------------------

#pragma hdrstop

#include <windows.h>
#include <uuids.h>
#include <mmsystem.h>
#include <evcode.h>

#include <System.Win.ComObj.hpp>

#include "MP3Mod.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

Mp3::Mp3()
{
}
//---------------------------------------------------------------------------

Mp3::~Mp3()
{
    try {
        if ( mc_ ) {
            mc_->Stop();
        }
    }
    catch ( ... ) {
    }
}
//---------------------------------------------------------------------------

void Mp3::Reset()
{
    ms_ = nullptr;
    mc_ = nullptr;
    mex_ = nullptr;
    ba_ = nullptr;
    gb_ = nullptr;

    OleCheck(
        ::CoCreateInstance(
            CLSID_FilterGraph,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_IGraphBuilder,
            reinterpret_cast<void **>( &gb_ )
        )
    );
    gb_->QueryInterface(IID_IMediaControl, reinterpret_cast<void **>( &mc_ ) );
    if ( !mc_ ) {
        throw Exception( _D( "Can't acquire IMediaControl interface" ) );
    }

    gb_->QueryInterface(IID_IMediaEventEx, reinterpret_cast<void **>( &mex_ ) );
    if ( !mex_ ) {
        throw Exception( _D( "Can't acquire IMediaEventEx interface" ) );
    }

    gb_->QueryInterface(IID_IBasicAudio, reinterpret_cast<void **>( &ba_ ) );
    if ( !ba_ ) {
        throw Exception( _D( "Can't acquire IBasicAudio interface" ) );
    }

    gb_->QueryInterface(IID_IMediaSeeking, reinterpret_cast<void **>( &ms_ ) );
    if( !ms_ ) {
        throw Exception( _D( "Can't acquire IMediaSeeking interface" ) );
    }
}
//---------------------------------------------------------------------------

void Mp3::LoadFromFile( String FileName )
{
    Reset();

    OleCheck( gb_->RenderFile( FileName.c_str(), nullptr ) );
    ms_->SetTimeFormat( &TIME_FORMAT_MEDIA_TIME );
    ms_->GetDuration( &duration_ );
}
//---------------------------------------------------------------------------

void Mp3::Play()
{
    OleCheck( mc_->Run() );
}
//---------------------------------------------------------------------------

void Mp3::Pause()
{
    OleCheck( mc_->Pause() );
}
//---------------------------------------------------------------------------

void Mp3::Stop()
{
    OleCheck( mc_->Stop() );
}
//---------------------------------------------------------------------------

float Mp3::GetVolume()
{
    long Volume {};
    OleCheck( ba_->get_Volume( &Volume ) );
    return static_cast<float>( Volume ) / 100.0F;
}
//---------------------------------------------------------------------------

void Mp3::SetVolume( float dB )
{
    long Volume = dB * 100.0F;
    OleCheck( ba_->put_Volume( Volume ) );
}
//---------------------------------------------------------------------------

bool Mp3::WaitForCompletion( long TimeoutMS, long* pEventCode )
{
    long EventCode {};
    if ( !pEventCode ) {
        pEventCode = &EventCode;
    }
    mex_->WaitForCompletion( TimeoutMS, pEventCode );
    return *pEventCode == EC_COMPLETE;
}
//---------------------------------------------------------------------------

__int64 Mp3::GetCurrentPosition()
{
    __int64 CurPos {};
    OleCheck( ms_->GetCurrentPosition( &CurPos ) );
    return CurPos;
}
//---------------------------------------------------------------------------

void Mp3::SetPositions( __int64 Current, __int64 Stop, bool AbsolutePos )
{
    DWORD const Flags = (
        AbsolutePos ?
          AM_SEEKING_AbsolutePositioning
        :
          AM_SEEKING_RelativePositioning
        ) |
        AM_SEEKING_SeekToKeyFrame;
    OleCheck( ms_->SetPositions( &Current, Flags, &Stop, Flags ) );
}
//---------------------------------------------------------------------------

