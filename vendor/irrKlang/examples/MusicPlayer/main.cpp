// This program is an example application for the irrKlang audio library.
// It is copyright 2007-2014 by N.Gebhardt, Ambiera and licensed under the LGPL2 license.
// For more information, please see www.ambiera.com/irrklang.

#include "main.h"
#include "window.h"
#include <irrKlang.h>
#include <string.h>
#include <stdio.h>

using namespace irrklang;


// global variables
ISoundEngine* SoundEngine = 0;
ISound* CurrentPlayingSound = 0;
bool LoopSound = true;
char LastPlayedSoundFile[FL_PATH_MAX];
char lastDisplayedPlayStatus = -1;
bool PlayListVisible = true;


// our main() function, see main.h
IRRKLANG_MUSIC_PLAYER_APPENTRY 
{
	// create irrKlang sound device

	SoundEngine = createIrrKlangDevice();

	if (!SoundEngine)
	{
		fl_alert("Could not create audio device\n");
		return 0;
	}

	// create window

	Fl_Double_Window* wnd = createPlayerWindow();

	wnd->label("irrKlang Player");
	MusicTitleDisplay->value("nothing selected to play");

	LoopButton->value(LoopSound ? 1 : 0);

	changeDir((char*)"../../media"); // try to select irrKlang media directory by default

	FileBrowser->filter("{*.mp3|*.ogg|*.wav|*.flac|*.mod|*.s3m|*.it|*.xm}");
	FileBrowser->type(FL_SELECT_BROWSER); 

	PlayPositionSlider->type(FL_HOR_NICE_SLIDER);
	PlayPositionSlider->bounds(0, 1);
	PlayPositionSlider->deactivate();

	SpeedSlider->type(FL_HOR_NICE_SLIDER);
	VolumeSlider->type(FL_HOR_NICE_SLIDER);
	PanSlider->type(FL_HOR_NICE_SLIDER);
	
	FileBrowser->load(".");

	Fl_Tooltip::delay(0.2f);
	Fl_Tooltip::size(10);


	// initialize timer and other stuff

	Fl::add_timeout(updateTimerSpeed, UpdateTimerCallback);
	LastPlayedSoundFile[0] = 0x0;


	// show and run player

	wnd->show();
	int ret = Fl::run();


	// that's it, clean up and exit

	if (CurrentPlayingSound)
		CurrentPlayingSound->drop();
	CurrentPlayingSound = 0;

	SoundEngine->drop();

	delete MainWindow;
	
	return ret;
}


void UpdateTimerCallback(void*) 
{
	if (CurrentPlayingSound)
	{
		// update play position

		ik_u32 pos = CurrentPlayingSound->getPlayPosition();
		if (pos == -1)
			pos = 0;

		PlayPositionSlider->value(pos * timeDisplayFactor);
	}

	// update play button

	char valueToSet = (!CurrentPlayingSound || CurrentPlayingSound->isFinished()) ? 0 : 1;
	if (lastDisplayedPlayStatus != valueToSet)
	{
		lastDisplayedPlayStatus = valueToSet;
		PlayButton->value(valueToSet);
		PlayButton->redraw();
	}
	
	// repeat timer
	Fl::repeat_timeout(updateTimerSpeed, UpdateTimerCallback);
}


void PlayFile(const char* filename)
{
	MusicTitleDisplay->value(filename);
	lastDisplayedPlayStatus = -1;

	if (SoundEngine)
	{
		strcpy(LastPlayedSoundFile, filename);

		// display loading text
		MusicTitleDisplay->value("loading...");
		MusicTitleDisplay->redraw();

		// stop previous sound and remove it from memory
		if (CurrentPlayingSound)
			CurrentPlayingSound->drop();
        SoundEngine->stopAllSounds();

		if (strcmp(LastPlayedSoundFile, filename))
			SoundEngine->removeAllSoundSources();

		// play new sound
		CurrentPlayingSound = SoundEngine->play2D(filename, LoopSound, false, true);
		if (!CurrentPlayingSound)
		{
			MusicTitleDisplay->value("");
			MusicTitleDisplay->redraw();
			fl_alert("Could not play sound %s", filename);
			return;
		}

		// update UI with info about the playing file
		ik_u32 len = CurrentPlayingSound->getPlayLength();
		PlayPositionSlider->bounds(0, len * timeDisplayFactor);
		PlayPositionSlider->activate();

		PlayButton->value(1);
		PlayButton->redraw();

		SAudioStreamFormat format = CurrentPlayingSound->getSoundSource()->getAudioFormat();

		char display[2048];
		sprintf(display, "%s [%dkHz %s %s %sbit]", 
			fl_filename_name(filename),
			format.SampleRate / 1000,
			format.ChannelCount > 1 ? "stereo" : "mono",
			CurrentPlayingSound->getSoundSource()->getStreamMode() == ::ESM_STREAMING ? "stream" : "buffered",
			format.SampleFormat == ::ESF_U8 ? "8" : "16");

		MusicTitleDisplay->value(display);
		MusicTitleDisplay->redraw();

		// test sound for seeking capability and disable slider if not
		if (!CurrentPlayingSound->getSoundSource()->getIsSeekingSupported())
		{
			PlayPositionSlider->deactivate();
			PlayPositionSlider->redraw();
		}

		// reset speed and volume slider
		SpeedSlider->value(1.0f);
		SpeedSlider->redraw();
		VolumeSlider->value(1.0f);
		VolumeSlider->redraw();
		PanSlider->value(0);
		PanSlider->redraw();
	}
}


void OnPlayPressed(Fl_Light_Button*, void*)
{
	// user pressed play button, toggle pause mode of sound

	if (CurrentPlayingSound)
	{
		bool pause = PlayButton->value() == 0;
		CurrentPlayingSound->setIsPaused(pause);
	}
	else
		lastDisplayedPlayStatus = -1;
}


void OnBrowserChanged(Fl_File_Browser* browser, void*)
{
	// user selected entry in playlist

	if (!browser)
		return;

	int selected = browser->value();
	int cnt = browser->size();

	if (selected < 0 || selected > cnt)
		return;

	const char* tx = browser->text(selected);
	if (!tx)
		return;

	char name[FL_PATH_MAX];
	fl_filename_absolute(name, tx);

	if (fl_filename_isdir(name))
	{
		if (!changeDir((char*)tx))
		{
			fl_alert("Could not change directory to %s", tx);
			return;
		}

		PathDisplay->value(name);
		PathDisplay->redraw();

		FileBrowser->load(".");
		FileBrowser->redraw();
	}
	else
	{
		PlayFile(name);
	}
}


void OnAbout(Fl_Button*, void*)
{
	// user pressed '?' button, show about box

	fl_alert("This program is an example application for the irrKlang audio library.\n"\
		     "It is copyright 2007-2010 by N.Gebhardt, Ambiera and licensed under the LGPL2 license.\n"\
			 "For more information, please see www.ambiera.com/irrklang.\n\n"\
			 "This player currently support the following file formats:\n"\
			 "MP3, OGG, WAV, FLAC, MOD, IT, S3M, XM\n");
}


void OnDirectoryButton(Fl_Repeat_Button*, void*)
{
	// user pressed directory button, show disk names

	FileBrowser->load("");
	FileBrowser->redraw();
}


void OnLoopPressed(Fl_Light_Button* button, void*)
{
	// user pressed loop button, switch sound loop mode

	char v = LoopButton->value();
	LoopSound = v != 0;

	if (CurrentPlayingSound)
		CurrentPlayingSound->setIsLooped(LoopSound);
}


void OnTogglePlayList(Fl_Button*, void*)
{
	// user pressed playlist button, show or hide playlist

	PlayListVisible = !PlayListVisible;

	if (PlayListVisible)
		MainWindow->size(419, 281);
	else
		MainWindow->size(419, 115);
}


void OnSliderMoved(Fl_Value_Slider*, void*)
{
	// user moved slider, seek to another play position

	double pos = PlayPositionSlider->value();

	if (CurrentPlayingSound)
	{
		ik_u32 pos = (ik_u32)(PlayPositionSlider->value() / timeDisplayFactor);
		CurrentPlayingSound->setPlayPosition(pos);	
	}
}


void OnPlaybackSpeedChanged(Fl_Value_Slider*, void*)
{
	// user moved playbackspeed slider

	double pos = SpeedSlider->value();

	if (CurrentPlayingSound)
	{
		CurrentPlayingSound->setPlaybackSpeed((ik_f32)pos);	
	}

}


void OnVolumeSliderChanged(Fl_Value_Slider*, void*)
{
	double pos = VolumeSlider->value();

	if (CurrentPlayingSound)
	{
		CurrentPlayingSound->setVolume((ik_f32)pos);	
	}
}


void OnPanSliderChanged(Fl_Value_Slider*, void*)
{
	double pos = PanSlider->value();

	if (CurrentPlayingSound)
	{
		CurrentPlayingSound->setPan((ik_f32)pos);	
	}
}
