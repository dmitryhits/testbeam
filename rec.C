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


   TH1F* histo = new TH1F("histo","Integral",2024,-600,+600);
   TH1F* histo1 = new TH1F("histo1","Integral-2",2024,-600,+600);
   TH1F* histo2 = new TH1F("histo2","Mean_{0-200}",2024,-600,+600);
   TH1F* histo3 = new TH1F("histo3","Peak",2024,-600,+600);

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
   TTree *newtree = fChain->CloneTree(0);
   newtree->SetAutoSave(10000000);  // autosave when 10 Mbyte written
   newtree->Branch("mean",        &meanval,           "mean/D");
   newtree->Branch("integral",    &integralval,   "integral/D");
   newtree->Branch("integral2",   &integral2val, "integral2/D");
   newtree->Branch("min",         &minval,             "min/D");

   Long64_t nentries = fChain->GetEntriesFast();

   TCanvas* c1 = new TCanvas();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
	Float_t mean = GetAvrgMean(0,200,0);
	Float_t integral = GetAvrgMean(224,644,mean);
	Float_t integral2 = GetAvrgMean(224,644,0);
	Float_t min = FindMinimum(224,644);
	min -= mean;
	if (ientry%100==0)
		cout<<TString::Format("\r%4d/%4d \t%5.1f",ientry,nentries,(Float_t)ientry/(Float_t)(nentries)*100)<<flush;
	//cout<<ientry<<" ";
	//cout<<"\tmean:    "<<mean;
	//cout<<"\tintegral: "<<integral<<endl;
//	DrawGraph();
	if(ientry>150000 && ientry<290000){
	  histo->Fill(-1*integral);
	  histo1->Fill(-1*integral2);
	  histo2->Fill(mean);
	  histo3->Fill(-1.*min);
	}
      // if (Cut(ientry) < 0) continue;

	meanval      =    mean;
	integralval  = -1*integral;
	integral2val = -1*integral2;
	minval       = -1*min;

	newtree->Fill();

   }
cout<<endl;
c1->Divide(2,2);
c1->cd(1);
histo->Draw();
c1->cd(2);
histo1->Draw();
c1->cd(3);
histo2->Draw();
c1->cd(4);
histo3->Draw();

histo->Write();
histo1->Write();
histo2->Write();
histo3->Write();
ft->Write();
delete ft;


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
