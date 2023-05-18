# data file for the Fltk User Interface Designer (fluid)
version 1.0107 
header_name {.h} 
code_name {.cxx}
declblock {\#if 1} {open public after {\#endif}
} {
  decl {\#pragma warning(disable: 4311)} {}
  decl {\#pragma warning(disable: 4312)} {}
} 

Function {createPlayerWindow()} {open
} {
  Fl_Window MainWindow {open
    xywh {762 248 419 294} type Double box PLASTIC_UP_BOX color 48 selection_color 35 labelsize 8 labelcolor 198 visible
  } {
    Fl_Output MusicTitleDisplay {
      xywh {10 5 400 30} color 33 labelfont 4 labelsize 11 labelcolor 88 textfont 1 textcolor 48
    }
    Fl_Value_Slider PlayPositionSlider {
      callback OnSliderMoved
      xywh {10 33 400 20} type Horizontal labelsize 11 textsize 11
    }
    Fl_Light_Button PlayButton {
      label {@>}
      callback OnPlayPressed
      tooltip {Play/Pause} xywh {10 62 45 26} labelsize 11 labelcolor 24
    }
    Fl_Light_Button LoopButton {
      label {@reload}
      callback OnLoopPressed
      tooltip Loop xywh {60 62 45 26} labelsize 11 labelcolor 24
    }
    Fl_File_Browser FileBrowser {
      callback OnBrowserChanged
      xywh {10 115 400 150} color 55 selection_color 40 labelsize 11 when 3 textsize 11 textcolor 32
    }
    Fl_Button {} {
      label {?}
      callback OnAbout
      tooltip About xywh {388 62 20 26} labelsize 11
    }
    Fl_Repeat_Button DirectoryButton {
      label {[drive]}
      callback OnDirectoryButton
      tooltip {Select drive to browse} xywh {365 266 45 20} labelsize 11
    }
    Fl_Output PathDisplay {
      xywh {10 267 350 20} color 48 labelsize 11 textsize 11 deactivate
    }
    Fl_Button {} {
      label {@+3<->}
      callback OnTogglePlayList
      tooltip {Show/Hide Playlist} xywh {338 62 45 26} labelsize 8 labelcolor 24
    }
    Fl_Value_Slider SpeedSlider {
      label {playback speed:}
      callback OnPlaybackSpeedChanged
      xywh {205 60 115 15} type Horizontal labelsize 11 align 4 maximum 5 value 1 textsize 11
    }
    Fl_Value_Slider VolumeSlider {
      label {volume:}
      callback OnVolumeSliderChanged
      xywh {205 74 115 15} type Horizontal labelsize 11 align 4 value 1 textsize 11
    }
    Fl_Value_Slider PanSlider {
      label {pan:}
      callback OnPanSliderChanged selected
      xywh {205 87 115 15} type Horizontal labelsize 11 align 4 minimum -1 textsize 11
    }
  }
} 
