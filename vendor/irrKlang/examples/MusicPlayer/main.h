// This program is an example application for the irrKlang audio library.
// It is copyright 2007 by N.Gebhardt, Ambiera and licensed under the LGPL2 license.
// For more information, please see www.ambiera.com/irrklang.

// include FLTK

#pragma warning(disable: 4312)
#pragma warning(disable: 4311)
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Tooltip.H>
#pragma warning(default: 4312)
#pragma warning(default: 4311)

// define entry function
// use WinMain in Windows, otherwise main() on all other platforms
#ifndef WIN32
#define IRRKLANG_MUSIC_PLAYER_APPENTRY int main(int argc, char **argv) 
#else
#define IRRKLANG_MUSIC_PLAYER_APPENTRY int __stdcall WinMain(void* hInstance, void* hPrevInstance, void* lpCmdLine, int nCmdShow)
#endif

// change path function
#ifdef _MSC_VER
#include <direct.h>
#else
#if defined(LINUX) || defined(MACOSX)
#include <unistd.h>
#endif
#endif
bool changeDir(char* path)
{
#ifdef _MSC_VER
	return (_chdir(path) == 0);
#else
#if defined(LINUX) || defined(MACOSX)
	return (chdir(path) == 0);
#endif
#endif
}

// forward declarations and constants
void UpdateTimerCallback(void*);

const double updateTimerSpeed = 1.0 / 20; // x times a second
const double timeDisplayFactor = 1 / 1000.0;

