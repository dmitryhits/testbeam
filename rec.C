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
   Long64_t nentries = fChain->GetEntriesFast();

   TCanvas* c1 = new TCanvas();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
	Float_t mean = GetAvrgMean(0,100,0);
	Float_t integral = GetAvrgMean(0,1024,mean);
	Float_t integral2 = GetAvrgMean(0,1024,0);
	Float_t min = FindMinimum(0,1024);
	min -= mean;
	if (ientry%100==0)
		cout<<TString::Format("\r%4d/%4d \t%5.1f",ientry,nentries,(Float_t)ientry/(Float_t)(nentries)*100)<<flush;
	//cout<<ientry<<" ";
	//cout<<"\tmean:    "<<mean;
	//cout<<"\tintegral: "<<integral<<endl;
//	DrawGraph();

	histo->Fill(-1*integral);
	histo1->Fill(-1*integral2);
	histo2->Fill(mean);
	histo3->Fill(-1.*min);
      // if (Cut(ientry) < 0) continue;
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
}

Float_t rec::FindMinimum(Int_t first, Int_t last){
	if(!(first>=0&&first<last&&last<=1024))
		return -9999999;
	Float_t min = chn3[first];
	for(int i = first; i<last;i++){
		if(chn3[i]<min)min=chn3[i];
	}
	return min;
	
}

Float_t rec::GetAvrgMean(Int_t first, Int_t last,Float_t avrgMean){
	Float_t mean = 0;
	Int_t nAvrg = last-first;
	for(UInt_t i = first;i< last;i++){
		mean+=chn3[i];
		//cout<<i<<" "<<mean<<endl;
	}
	mean = mean/(Float_t)nAvrg;
	mean = mean - avrgMean;//*(Float_t)nAvrg;
	return mean;
}


void rec::DrawGraph(){
	TGraph *gShape = new TGraph(1024,t,chn3);
	gShape->Draw("APL");
}
