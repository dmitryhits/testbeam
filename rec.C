#define rec_cxx
#include "rec.h"
#include <TH2.h>
#include <iostream>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>

using namespace std;

void rec::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L rec.C
//      Root > rec t
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

   //Book histograms
   TH1F* h_Integral_short_50 = new   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
	Float_t mean = GetAvrgMean(0,200,0);
	Float_t integral_long = GetAvrgMean(224,644,0);
	Float_t integral_medium = GetAvrgMean(294,574,0);
	Float_t integral_short = GetAvrgMean(364,504,0);
	Float_t integral_short_100 = GetAvrgMean(399,469,0);
	Float_t integral_short_50 = GetAvrgMean(417,452,0);
	Float_t min = FindMinimum(364,504);
	Int_t n_triggers =  GetNumberOfTriggers(-200);
	//min -= mean;
	if (ientry%100==0)
	  cout<<TString::Format("\r%4d/%4d \t%5.1f",ientry,nentries,(Float_t)ientry/(Float_t)(nentries)*100)<<flush;
	//cout<<ientry<<" ";
	//cout<<"\tmean:    "<<mean;
	//cout<<"\tintegral: "<<integral<<endl;
//	DrawGraph();
//
	if(ientry>150000 && ientry<290000){ // limits for run 41
//	if(ientry<150000){ // limits for run 16
//	if(ientry<27000){ // limits for run 12
//	if(ientry<54000){ // limits for run 13
	  h_Integral_short->Fill(-1*integral_short);
	  h_Integral_short_50->Fill(-1*integral_short_50);
	  h_Integral_short_100->Fill(-1*integral_short_100);
	  h_Integral_medium->Fill(-1*integral_medium);
	  h_Integral_long->Fill(-1*integral_long);
	  h_Mean->Fill(mean);
	  h_Min->Fill(min);
	  h_n_triggers->Fill(n_triggers);
	}
      // if (Cut(ientry) < 0) continue;

	meanval      =    mean;
	n_triggers_val  = n_triggers;
	integral_short_val_50  = -1*integral_short_50;
	integral_short_val_100  = -1*integral_short_100;
	integral_short_val  = -1*integral_short;
	integral_medium_val  = -1*integral_medium;
	integral_long_val  = -1*integral_long;
	min_val  = -1*min;
	
	newtree->Fill();
	
   }
   cout<<endl;
   c1->Divide(2,4);
   c1->cd(1);
   h_Integral_short_50->Draw();
   c1->cd(2);
   h_Integral_short_100->Draw();
   c1->cd(3);
   h_Integral_short->Draw();
   c1->cd(4);
   h_Integral_medium->Draw();
   c1->cd(5);
   h_Integral_long->Draw();
   c1->cd(6);
   h_Mean->Draw();
   c1->cd(7);
   h_n_triggers->Draw();
   c1->cd(8);
   h_Min->Draw();   
   h_Integral_short_50->Write();
   h_Integral_short_100->Write();
   h_Integral_short->Write();
   h_Integral_medium->Write();
   h_Integral_long->Write();
   h_Mean->Write();
   h_n_triggers->Write();
   h_Min->Write();
   ft->Write();
   delete ft;  
}
Int_t rec::GetNumberOfTriggers(Float_t trigger_level){
  Int_t  n_triggers = 0;
  Int_t trigger_on = 0;
  Int_t i_on=0;
  Int_t min_trigger_length = 7 // 7 = 10 ns at 0.7 GSAMPLES/s
  for (int i=0;i<1024;i++){
    if (chn_trg[i]<trigger_level && trigger_on == 0) {
      trigger_on = 1;
      i_on=i;
  }
    if (chn_trg[i]>trigger_level && trigger_on == 1 ) 
      trigger_on = 0;
    if (i_on+min_ttrigger_length<i) n_triggers++;
  }
  return n_triggers;
}
Float_t rec::Get2ndTriggerTime(Float_t trigger_level){
  Int_t  n_triggers = 0;
  Int_t trigger_on = 0;
  Float_t trigger_time  = -1;
  for (int i=0;i<1024;i++){
    if (chn_trg[i]<trigger_level and trigger_on == 0) {
      trigger_on = 1;
      n_triggers++;
  }
    if (chn_trg[i]>trigger_level and trigger_on == 1) 
      trigger_on = 0;
  }
  return n_triggers;
}

Float_t rec::FindMinimum(Int_t first, Int_t last){
	if(!(first>=0&&first<last&&last<=1024))
		return -9999999;
	Float_t min = chn_sig[first];
	for(int i = first; i<last;i++){
		if(chn_sig[i]<min)min=chn_sig[i];
	}
	return min;
	
}

Float_t rec::GetAvrgMean(Int_t first, Int_t last,Float_t avrgMean){
	Float_t mean = 0;
	Int_t nAvrg = last-first;
	for(UInt_t i = first;i< last;i++){
		mean+=chn_sig[i];
		//cout<<i<<" "<<mean<<endl;
	}
	mean = mean/(Float_t)nAvrg;
	mean = mean - avrgMean;//*(Float_t)nAvrg;
	return mean;
}


void rec::DrawGraph(){
	TGraph *gShape = new TGraph(1024,t,chn_sig);
	gShape->Draw("APL");
}
