#ifndef  TRIGGERDELAY_H
#define  TRIGGERDELAY_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "TROOT.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include <math.h>

using namespace std;

class TriggerDelay{
  public:  
    TriggerDelay(Int_t n_avrg);
    ~TriggerDelay();
    void AddWaveform(Double_t wf[],UInt_t size);
    Double_t GetMean();
    Double_t GetRMS();
    void SetName(TString name){this->name = name;};
    void SetTriggerTime(Int_t* time){First_trigger_time = time;};
    
  private:
    void AnalyzeWaveform();
    Double_t FindSignalMax(Int_t first, Int_t last, Double_t sig[], Int_t nAvrg, Int_t & max_signal_time);
    UInt_t n_avrg;
    vector<Double_t> avrgSig;
    vector< vector <Double_t> > savedSigs;
    UInt_t nSigs;
    Double_t mean;
    Double_t sigma;
    Double_t n;
    Int_t *First_trigger_time;
    TString name;
    TH2D* h_max_signal_time_vs_ampitude;
};
    
  
#endif/*  TRIGGERDELAY_H*/

