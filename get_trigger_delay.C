#define get_trigger_delay_cxx
#include "get_trigger_delay.h"
#include <TH2.h>
#include <iostream>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TF1.h>

using namespace std;

void get_trigger_delay::Loop() {
  //   In a ROOT session, you can do:
  //      Root > .L get_trigger_delay.C
  //      Root > get_trigger_delay t
  //      Root > t.GetEntry(12); // Fill t data members with entry number 12
  //      Root > t.Show();       // Show values of entry 12
  //      Root > t.Show(16);     // Read and show values of entry 16
  //      Root > t.Loop();       // Loop on all entries
  //

  //     This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  //  Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  //
  //       To read only selected branches, Insert statements like:
  // METHOD1:
  //    fChain->SetBranchStatus("*",0);  // disable all branches
  //    fChain->SetBranchStatus("branchname",1);  // activate branchname
  // METHOD2: replace line
  //    fChain->GetEntry(jentry);       //read all branches
  //by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0) return;
  
  //Append _time to the file name
  char output_filename[1024];
  if((sizeof(filename)+12) < sizeof(output_filename)){
    strncpy(output_filename,filename, sizeof(output_filename));
    char* ext;
    ext = strstr(output_filename,".root");
    strncpy(ext,"_time.root",12);
  }
  else {
    cout << "filename is too long" << endl;
  }
 
  // oepn file for writing histograms
  TFile *ft = new TFile(output_filename,"RECREATE");
  //Book histograms
  TH1F* h_max_signal = new TH1F("h_max_signal","Max signal",1200,-600,+600);
  TH1F* h_1st_trigger_time = new TH1F("h_1st_trigger_time","The time of the 1st trigger",1024,0,1024);
  TH1F* h_max_signal_time = new TH1F("h_max_signal_time","The time of the max signal",2048,-1024,1024);
  TH1F* h_max_signal_time1 = new TH1F("h_max_signal_time1","The time of the max signal 1",1024,0,1024);
  TH2F* h_max_signal_time_vs_ampitude = new TH2F("h_max_signal_time_vs_ampitude","Max signal time vs ampitude",2048,-1024,1024,1200,-600,600);
  
  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    Int_t Max_signal_time =  -1;
    Float_t Max_signal = FindSignalMax(0,1024,Max_signal_time);
    Int_t First_trigger_time =  Get1stTriggerTime(-200);
    //if (ientry%100==0)
      //cout<<TString::Format("\r%4d/%4d \t%5.1f",ientry,nentries,(Float_t)ientry/(Float_t)(nentries)*100)<<flush;
    h_max_signal->Fill(Max_signal);
    h_1st_trigger_time->Fill(First_trigger_time);
    h_max_signal_time->Fill(Max_signal_time-First_trigger_time);
    h_max_signal_time1->Fill(Max_signal_time);
    h_max_signal_time_vs_ampitude->Fill(Max_signal_time-First_trigger_time, Max_signal);
      
  }
  Float_t RMS = h_max_signal_time->GetRMS();
  Float_t MAX = h_max_signal_time->GetBinCenter(h_max_signal_time->GetMaximumBin());
  h_max_signal_time->Fit("gaus","Q0","",MAX-RMS,MAX+RMS);
  TF1 *fit = h_max_signal_time->GetFunction("gaus");
  Float_t delay = fit->GetParameter(1);
  Float_t delay_bins = delay*0.7;
  cout << "Run " << filename << " has delay " << delay << " ns or " << delay_bins << " bins" << endl; 
  ft->Write();
}

Int_t get_trigger_delay::Get1stTriggerTime(Float_t trigger_level) {
  Int_t  n_triggers = 0;
  Int_t trigger_on = 0;
  Int_t i_on = -1;
  //vector<Int_t> trigger_time  = -1;
  Int_t min_trigger_length = 7; // 7  = 10 ns at 0.7 GSamples/s
  Int_t first_trigger_time  = -1;
  for (Int_t i=0;i<1024;i++){
    //Turn the trigger status ON if signal goes below trigger level (Usually -200 mV)
    if (chn_trg[i]<trigger_level && trigger_on == 0) {
      trigger_on = 1;
      i_on=i;
    }
    //Turn trigger status to 0 (OFF) if the singal goes back above the trigger level
    if (chn_trg[i]>trigger_level and trigger_on == 1) {
      trigger_on = 0;
      // If the trigger was longer than the minimum trigger length count the trigger
      if(i_on+min_trigger_length<i) {
	//trigger_time.push_back(i_on);
	n_triggers++;
      }
      if(n_triggers == 1) first_trigger_time = i_on;
    }
  }
    return first_trigger_time;
}

Float_t get_trigger_delay::FindSignalMax(Int_t first, Int_t last, Int_t & max_signal_time) {
	if(!(first>=0 && first<last && last<=1024))
		return -9999999;
	Float_t max = chn_sig[first];
	for(Int_t i = first; i<last;i++){
	  if(fabs(chn_sig[i])>fabs(max)) {
	    max=chn_sig[i];
	    max_signal_time = i;
	  }
	}
	return max;
	
}

Float_t get_trigger_delay::GetAvrgMean(Int_t first, Int_t last,Float_t avrgMean){
	Float_t mean = 0;
	Int_t nAvrg = last-first;
	for(Int_t i = first;i< last;i++){
		mean+=chn_sig[i];
		//cout<<i<<" "<<mean<<endl;
	}
	mean = mean/(Float_t)nAvrg;
	mean = mean - avrgMean;//*(Float_t)nAvrg;
	return mean;
}


