

#include "TriggerDelay.hh"

TriggerDelay::TriggerDelay(Int_t n_avrg){
  cout<<"Init TriggerDelay::TriggerDelay "<<n_avrg<<endl;
  this->n_avrg = n_avrg;
  nSigs = 0;
  savedSigs.resize(n_avrg);
  mean = 0;
  sigma = 0;
  n = 0;
  name = "";
  h_max_signal_time_vs_ampitude = new TH2D("h_max_signal_time_vs_ampitude","Max signal time vs ampitude",2048,-1024,1024,1200,-600,600);
  First_trigger_time = 0;
}


TriggerDelay::~TriggerDelay(){};


void TriggerDelay::AddWaveform(Double_t wf[],UInt_t size){
    bool remove = nSigs>=n_avrg;
    Int_t pos = nSigs%n_avrg;
    if (savedSigs[pos].size() < size)
      savedSigs[nSigs%n_avrg].resize(size,0);
    if (avrgSig.size() < size)
      avrgSig.resize(size,0);
    for (UInt_t i = 0; i<size;i++){
      avrgSig[i] += wf[i];
      if (remove) 
        avrgSig[i] -= savedSigs[pos][i];
      savedSigs[pos][i] = wf[i];
    }
    if (savedSigs[pos].size() > size)
      savedSigs[nSigs%n_avrg].resize(size,0);
    nSigs++;
    //cout<<"AddedWaveform "<<nSigs<<endl;
    if (nSigs>=n_avrg && nSigs%n_avrg==0)
      AnalyzeWaveform();
};

void TriggerDelay::AnalyzeWaveform(){
  cout<<name<<"::AnalyzeWaveform()"<<endl;

  Int_t max_signal_time;
 
  Double_t Max_signal = FindSignalMax(0,1024,&avrgSig[0],n_avrg,max_signal_time);
  if (First_trigger_time)
    h_max_signal_time_vs_ampitude->Fill(max_signal_time-*First_trigger_time, Max_signal);
  else
    h_max_signal_time_vs_ampitude->Fill(max_signal_time, Max_signal);

}
//int feld[]
Double_t TriggerDelay::FindSignalMax(Int_t first, Int_t last, Double_t sig[], Int_t nAvrg, Int_t & max_signal_time) {
  if(!(first>=0 && first<last && last<=1024))
    return -9999999;
  Float_t max = sig[first];
  for(Int_t i = first; i<last;i++){
    if(fabs(sig[i])>fabs(max)) {
      max=sig[i];
      max_signal_time = i;
    }
  }
  max/=nAvrg;
  cout<<name<<"::get max signal "<<first<<" "<<last<<" "<<max<<" "<<max_signal_time<<endl;
  return max;
}
Double_t TriggerDelay::GetMean() {return n==0?-1:mean/n;};

Double_t TriggerDelay::GetRMS() {Double_t retVal = n==0?-1:sigma-mean/n*mean/n; return retVal>=0?sqrt(retVal):retVal;}
    
