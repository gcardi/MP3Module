#include <vcl.h>
#include <windows.h>

#pragma hdrstop
#pragma argsused

#include <tchar.h>

#include <iostream>
#include <regex>

#include <conio.h>

#include <System.DateUtils.hpp>

#include "MP3Mod.h"
#include "OleUtils.h"

using std::wcout;
using std::wcerr;
using std::endl;
using std::flush;
using std::wregex;
using std::wcmatch;
using std::regex_match;

extern String StripFileExt( String Val );

// https://www.codeguru.com/multimedia/simple-c-mp3-player-class/

int _tmain(int argc, _TCHAR* argv[])
{
    if ( argc >= 2 ) {
        try {
            TOleSession OleSession;

            Mp3 mp3;
            /*
            wcout << L"Volume: " << mp3.GetVolume() << L" dB" << endl;
            mp3.SetVolume( -10 ); // <-- Set to -10 dB
            wcout << L"New volume level: " << mp3.GetVolume() << L" dB" << endl;
            */
            for ( int i = 1 ; i < argc ; ++i ) {
                wcout << L"Loading: \'" << argv[i] << L'\'' << endl;
                mp3.LoadFromFile( argv[i] );
                TDateTime const Duration { IncMilliSecond( {}, mp3.GetDuration() / 10000 ) };
                wcout << L"Duration: " << Duration.TimeString() << endl;
                wcout << L"Duration in 1/10000 sec: " << mp3.GetDuration() << endl;
                wcout << L"Now playing (press any key to stop)" << endl;
                mp3.Play();
                long EvCode {};
                int n {};
                for ( ; !mp3.WaitForCompletion( 100, &EvCode ) ; ++n ) {
                    if ( kbhit() ) {
                        getch();
                        break;
                    }
                    if ( !( n % 5 ) ) {
                        wcout << L"." << flush;
                    }
                }
                if ( n ) {
                    wcout << endl;
                }
                mp3.Stop();
                wcout << L"End playing \'" << argv[i] << L'\'' << endl;
            }
        }
        catch ( Exception const & E ) {
            wcerr << E.Message.c_str() << endl;
        }
        catch ( std::exception const & e ) {
            wcerr << e.what() << endl;
        }
        catch ( ... ) {
            wcerr << L"Unknown exception" << endl;
        }
    }
    else {
        wcerr << L"Use:\n  "
              << LowerCase(
                    StripFileExt( ExtractFileName( String( argv[0] ) ) )
                 )
              << L" [mp3 file name]" << endl;
    }

    //wcout << L"Press a key" << endl;
    //getch();

	return 0;
}
//---------------------------------------------------------------------------

String StripFileExt( String Val )
{
    wregex re( L"^(.*\?).\\w+$" );
    wcmatch m;
    if ( regex_match( Val.c_str(), m, re ) ) {
        return m[1].str().c_str();
    }
    return Val;
}
//---------------------------------------------------------------------------

