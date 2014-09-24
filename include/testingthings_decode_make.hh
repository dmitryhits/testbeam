#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "TStopwatch.h"


using namespace std;


//void usage();
//void decode(TString);

struct Header_t {
  char           event_header[4];
  unsigned int   serial_number;
  unsigned short year;
  unsigned short month;
  unsigned short day;
  unsigned short hour;
  unsigned short minute;
  unsigned short second;
  unsigned short millisecond;
  unsigned short reserved1;
  float time[1024];
};

struct Waveform_t {
  char           chn1_header[4];
  unsigned short chn1[1024];
  char           chn2_header[4];
  unsigned short chn2[1024];
  char           chn3_header[4];
  unsigned short chn3[1024];
//  char           chn4_header[4];
//  unsigned short chn4[1024];
};


class event{
  public:  
    int evt;
    int isCalib;
    Waveform_t waveform;
    double time_stamp;
};

