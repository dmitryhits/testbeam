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
   TH1F* h_Integral_short_50 = new TH1F("h_Integral_short_50","Integral short, 50 bins",2024,-600,+600);
   TH1F* h_Integral_short_mean_50 = new TH1F("h_Integral_short_mean_50","Integral short - before the trigger mean, 50 bins",2024,-600,+600);
   TH1F* h_Integral_short_100 = new TH1F("h_Integral_short_100","Integral short, 100 ns ",2024,-600,+600);
   TH1F* h_Integral_short_mean_100 = new TH1F("h_Integral_short_mean_100","Integral short - before trigger mean, 100 bins ",2024,-600,+600);
   TH1F* h_Integral_short = new TH1F("h_Integral_short","Integral short, 200 bins",2024,-600,+600);
   TH1F* h_Integral_medium = new TH1F("h_Integral_medium","Integral medium, 300 bins",2024,-600,+600);
   TH1F* h_Integral_long = new TH1F("Integral_long","Integral long, 400 bins",2024,-600,+600);
   TH1F* h_Mean = new TH1F("h_Mean","Mean 1024 bins",2024,-600,+600);
   TH1F* h_Mean_short_50 = new TH1F("h_Mean_short_50","Mean before the trigger, 50 bins",2024,-600,+600);
   TH1F* h_Mean_short_100 = new TH1F("h_Mean_short_100","Mean before the trigger, 100 bins",2024,-600,+600);
   TH1F* h_Min = new TH1F("h_Min","Min",2024,-600,+600);
   TH1F* h_n_triggers = new TH1F("h_n_triggers","Number of triggers",20,0,20);
   TH1F* h_n_triggers_1 = new TH1F("h_n_triggers_1","Number of triggers, when 1st trigger is at the correct position",20,0,20);
   TH1F* h_n_triggers_2 = new TH1F("h_n_triggers_2","Number of triggers, when 2nd trigger is not immedeatly following the first trigger",20,0,20);
   TH1F* h_1st_trigger_time = new TH1F("h_1st_trigger_time","The time of the 1st trigger",1024,0,1024);
   TH1F* h_2nd_trigger_time = new TH1F("h_2nd_trigger_time","The time of the 2nd trigger",1024,0,1024);

   //Append _histos to the file name
   char output_filename[1024];
   if((sizeof(filename)+12) < sizeof(output_filename)){
     strncpy(output_filename,filename, sizeof(output_filename));
     char* ext;
     ext = strstr(output_filename,".root");
     strncpy(ext,"_histos.root",12);
   }
   else {
     cout << "filename is too long" << endl;
   } 

   TFile *ft = new TFile(output_filename,"RECREATE");
   // Book tree branches
   TTree *newtree = fChain->CloneTree(0);
   newtree->SetAutoSave(10000000);  // autosave when 10 Mbyte written
   newtree->Branch("mean",        &meanval,           "mean/D");
   newtree->Branch("mean_short_50",        &mean_short_val_50,           "mean_short_50/D");
   newtree->Branch("mean_short_100",        &mean_short_val_100,           "mean_short_100/D");
   newtree->Branch("integral_short_50",    &integral_short_val_50,   "integral_short_50/D");
   newtree->Branch("integral_short_100",    &integral_short_val_100,   "integral_short_100/D");
   newtree->Branch("integral_short",    &integral_short_val,   "integral_short/D");
   newtree->Branch("integral_medium",   &integral_medium_val, "integral_medium/D");
   newtree->Branch("integral_long",   &integral_long_val, "integral_long/D");
   newtree->Branch("integral_short_mean_50",    &integral_short_mean_val_50,   "integral_short_mean_50/D");
   newtree->Branch("integral_short_mean_100",    &integral_short_mean_val_100,   "integral_short_mean_100/D");
   newtree->Branch("n_triggers",   &n_triggers_val, "n_triggers/I");
   newtree->Branch("First_trigger_time_val",   &First_trigger_time_val, "First_trigger_time_val/I");
   newtree->Branch("Second_trigger_time_val",   &Second_trigger_time_val, "Second_trigger_time_val/I");
   newtree->Branch("min",   &min_val, "min/D");

   Long64_t nentries = fChain->GetEntriesFast();

   TCanvas* c1 = new TCanvas();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
	Int_t First_trigger_time =  Get1stTriggerTime(-200);
	if (First_trigger_time<0) continue;
	//cout << "First trigger time "<< First_trigger_time << endl;
	Int_t Second_trigger_time =  Get2ndTriggerTime(-200);
	Float_t mean = GetAvrgMean(0,1024,0);
	Float_t mean_short_50 = -999;
	//cout << "1" << endl;
	if (First_trigger_time>50)
	  mean_short_50 = GetAvrgMean(First_trigger_time-50,First_trigger_time,0); //short (50 bin)average before the trigger
	else
	  cout << "First trigger is less than 50 bins after the trace start : " << First_trigger_time << endl;
	Float_t mean_short_100 = -999;
	if (First_trigger_time>100)
	  mean_short_100 = GetAvrgMean(First_trigger_time-100,First_trigger_time,0); //short (100 bin) average before the trigger
	else
	  cout << "First trigger is less than 100 bins after the trace start : " << First_trigger_time << endl;
	//Float_t trigger_delay = 13;
	//cout << "2" << endl;
	Float_t integral_long = GetAvrgMean(First_trigger_time+trigger_delay-200,First_trigger_time+trigger_delay+200,0); //400
	Float_t integral_medium = GetAvrgMean(First_trigger_time+trigger_delay-150,First_trigger_time+trigger_delay+150,0);//300
	Float_t integral_short = GetAvrgMean(First_trigger_time+trigger_delay-100,First_trigger_time+trigger_delay+100,0); //200
	Float_t integral_short_100 = GetAvrgMean(First_trigger_time+trigger_delay-50,First_trigger_time+trigger_delay+50,0); //100
	Float_t integral_short_mean_100 = -999;
	//cout << "3" << endl;
	if (First_trigger_time>100)
	  integral_short_mean_100 = GetAvrgMean(First_trigger_time+trigger_delay-50,First_trigger_time+trigger_delay-50,mean_short_100); //100 and subtract the 100 bin mean
	Float_t integral_short_50 = GetAvrgMean(First_trigger_time+trigger_delay-25,First_trigger_time+trigger_delay+25,0);
	Float_t integral_short_mean_50 = -999;
	if (First_trigger_time>50)
	  integral_short_mean_50 = GetAvrgMean(First_trigger_time+trigger_delay-25,First_trigger_time+trigger_delay+25,mean_short_50); //50 and subtract the 50 bin mean
	Float_t min = FindMinimum(First_trigger_time+trigger_delay-150,First_trigger_time+trigger_delay+150); // find minimum in 300 bin range
	Int_t n_triggers =  GetNumberOfTriggers(-200);
	//min -= mean;
	if (ientry%100==0)
	  cout<<TString::Format("\r%4d/%4d \t%5.1f",ientry,nentries,(Float_t)ientry/(Float_t)(nentries)*100)<<flush;

//	if(ientry>150000 && ientry<290000){ // limits for run 41
//	if(ientry<150000){ // limits for run 16
//	if(ientry<27000){ // limits for run 12
//	if(ientry<54000){ // limits for run 13
	h_Integral_short->Fill(integral_short); //200
	h_Integral_short_50->Fill(integral_short_50); //50
	h_Integral_short_100->Fill(integral_short_100); //100
	h_Integral_medium->Fill(integral_medium); //300
	h_Integral_long->Fill(integral_long); //400
	h_Mean->Fill(mean); //1024 mean
	h_Mean_short_50->Fill(mean_short_50); //50 bin mean before trigger
	h_Mean_short_100->Fill(mean_short_100); //100 bin mean before trigger
	h_Min->Fill(min);
	if (First_trigger_time>50)
	  h_Integral_short_mean_50->Fill(integral_short_mean_50);
	if (First_trigger_time>100)
	  h_Integral_short_mean_100->Fill(integral_short_mean_100);
	h_n_triggers->Fill(n_triggers);
	//if(ientry>150000 && ientry<150100) cout << "First trigger time is " << First_trigger_time << endl;
	if (First_trigger_time>83 && First_trigger_time<92) h_n_triggers_1->Fill(n_triggers);
	if (Second_trigger_time>98) h_n_triggers_2->Fill(n_triggers);

	if(Second_trigger_time>0) h_2nd_trigger_time->Fill(Second_trigger_time);
	if(First_trigger_time>0) h_1st_trigger_time->Fill(First_trigger_time);
	//	}
	// if (Cut(ientry) < 0) continue;

	meanval      =    mean;
	mean_short_val_50      =    mean_short_50;
	mean_short_val_100      =    mean_short_100;
	n_triggers_val  = n_triggers;
	First_trigger_time_val  = First_trigger_time;
	Second_trigger_time_val  = Second_trigger_time;
	integral_short_val_50  = integral_short_50;
	integral_short_val_100  = integral_short_100;
	integral_short_val  = integral_short;
	integral_medium_val  = integral_medium;
	integral_long_val  = integral_long;
	min_val  = min;
	integral_short_mean_val_50  = integral_short_mean_50;
	integral_short_mean_val_100  = integral_short_mean_100;
	
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
   h_n_triggers_1->Draw("same");
   h_n_triggers_2->Draw("same");
   c1->cd(8);
   h_2nd_trigger_time->Draw();   
   h_1st_trigger_time->Write();
   h_2nd_trigger_time->Write();
   h_Integral_short_50->Write();
   h_Integral_short_100->Write();
   h_Integral_short_mean_50->Write();
   h_Integral_short_mean_100->Write();
   h_Integral_short->Write();
   h_Integral_medium->Write();
   h_Integral_long->Write();
   h_Mean->Write();
   h_Mean_short_50->Write();
   h_Mean_short_100->Write();
   h_n_triggers->Write();
   h_n_triggers_1->Write();
   h_n_triggers_2->Write();
   h_Min->Write();
   ft->Write();
   delete ft;  
}
Int_t rec::GetNumberOfTriggers(Float_t trigger_level){
  Int_t  n_triggers = 0;
  Int_t trigger_on = 0;
  Int_t i_on=0;
  Int_t min_trigger_length = 7; // 7  = 10 ns at 0.7 GSamples/s
  for (int i=0;i<1024;i++){
    if (chn_trg[i]<trigger_level && trigger_on == 0) {
      trigger_on = 1;
      i_on=i;
    }
    if (chn_trg[i]>trigger_level and trigger_on == 1) {
      trigger_on = 0;
      if(i_on+min_trigger_length<i) n_triggers++;
    }
  }
  return n_triggers;
}
Int_t rec::Get1stTriggerTime(Float_t trigger_level){
  Int_t  n_triggers = 0;
  Int_t trigger_on = 0;
  Int_t i_on = -1;
  //vector<Int_t> trigger_time  = -1;
  Int_t min_trigger_length = 7; // 7  = 10 ns at 0.7 GSamples/s
  Int_t first_trigger_time  = -1;
  for (Int_t i=0;i<1024;i++){
    if (chn_trg[i]<trigger_level && trigger_on == 0) {
      trigger_on = 1;
      i_on=i;
    }
    if (chn_trg[i]>trigger_level and trigger_on == 1) {
      trigger_on = 0;
      if(i_on+min_trigger_length<i) {
	//trigger_time.push_back(i_on);
	n_triggers++;
      }
      if(n_triggers == 1) first_trigger_time = i_on;
    }
  }
    return first_trigger_time;
}
Int_t rec::Get2ndTriggerTime(Float_t trigger_level){
  Int_t  n_triggers = 0;
  Int_t trigger_on = 0;
  Int_t i_on = -1;
  //vector<Int_t> trigger_time  = -1;
  Int_t min_trigger_length = 7; // 7  = 10 ns at 0.7 GSamples/s
  Int_t Second_trigger_time  = -1;
  for (Int_t i=0;i<1024;i++){
    if (chn_trg[i]<trigger_level && trigger_on == 0) {
      trigger_on = 1;
      i_on=i;
    }
    if (chn_trg[i]>trigger_level and trigger_on == 1) {
      trigger_on = 0;
      if(i_on+min_trigger_length<i) {
	//trigger_time.push_back(i_on);
	n_triggers++;
      }
      if(n_triggers == 2) Second_trigger_time = i_on;
    }
  }
    return Second_trigger_time;
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
