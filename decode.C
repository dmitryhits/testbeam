#include <string.h>
#include <stdio.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <iostream>

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
  char           chn4_header[4];
  unsigned short chn4[1024];
};

void decode(char *filename) {
  Header_t header;
  Waveform_t waveform;
  Int_t year, month, day;
  Double_t  time_stamp;
  Double_t t[1024], chn1[1024], chn2[1024], chn3[1024], chn4[1024];
  Int_t n_channels = 4;
  Int_t n;
  Int_t k =0;
  //  cout << "0" << endl;
  
  // open the binary waveform file
  FILE *f;
  f = fopen(Form("%s.dat", filename), "r");
  //  cout << "1" << endl;
  
  //open the root file
  TFile *outfile = new TFile(Form("%s.root", filename), "RECREATE");
  
  // define the output tree
  TTree *rec = new TTree("rec","rec");
  rec->Branch("year", &year ,"year/I");
  rec->Branch("month", &month ,"month/I");
  rec->Branch("day", &day ,"day/I");
  rec->Branch("time_stamp", &time_stamp ,"time_stamp/D");
  rec->Branch("t", &t   ,"t[1024]/D");  
  rec->Branch("n", &k   ,"n/I");  
  // Create branches for the channels
  rec->Branch("chn1", &chn1 ,"chn1[1024]/D");
  rec->Branch("chn2", &chn2 ,"chn2[1024]/D");
  rec->Branch("chn3", &chn3 ,"chn3[1024]/D");
  rec->Branch("chn4", &chn4 ,"chn4[1024]/D");
  //  cout << "21" << endl;

  
  // loop over all events in data file
  for (n=0 ; fread(&header, sizeof(header), 1, f) > 0; n++) {
    //  cout << "30" << endl;
    if (n%1000 == 0) cout << n << endl;
    // decode time     
    if( n == 0) cout << "event header  " << header.event_header << endl;
    year = header.year;
    month = header.month;
    day = header.day;
    if (n%10000 == 0)cout << "year "<< year << ": month " << month << ": day " << day << " : hour " << header.hour << " : minute " << header.minute << endl; 
    time_stamp = header.hour*3600+header.minute*60+header.second+header.millisecond/1000.; 
    if (n == 0) cout << "millisecond " << header.millisecond << endl;
    for (Int_t i=0; i<1024; i++)
      t[i] = (Double_t) header.time[i];

    //  cout << "3" << endl;
    if(n == 0) cout << "waveform size : " <<    sizeof(waveform)/4*n_channels << endl;

    fread(&waveform, sizeof(waveform), 1, f);
    
    // decode amplitudes in mV
    for (Int_t i=0; i<1024; i++) {
      chn1[i] = (Double_t) ((waveform.chn1[i]) / 65535. - 0.5) * 1000;   	
      chn2[i] = (Double_t) ((waveform.chn2[i]) / 65535. - 0.5) * 1000;   
      chn3[i] = (Double_t) ((waveform.chn3[i]) / 65535. - 0.5) * 1000;   
      chn4[i] = (Double_t) ((waveform.chn4[i]) / 65535. - 0.5) * 1000;   
    }
    //cout << "5" << endl;
    rec->Fill();
    k++;
  }
      
  // print number of events
  //  cout<<TString::Format("\r%4d/%4d \t%5.1f",ientry,nentries,(Float_t)ientry/(Float_t)(nentries)*100)<<flush;
  cout<<n<<" events processed"<<endl;
  cout<<"\""<<Form("%s.root", filename)<<"\" written"<<endl;
  
  // save and close root file
  rec->Write();
  outfile->Close();
}
