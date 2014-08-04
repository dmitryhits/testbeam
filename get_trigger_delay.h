//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep 11 16:57:21 2013 by ROOT version 5.32/01
// from TTree get_trigger_delay/get_trigger_delay
// found on file: PLTS116-m350V.root
//////////////////////////////////////////////////////////

#ifndef get_trigger_delay_h
#define get_trigger_delay_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TGraph.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class get_trigger_delay {
  char *filename;
 public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  Double_t        t[1024];
  Double_t        meanval;
  Double_t        integral_short_val_50;
  Double_t        integral_short_val_100;
  Double_t        integral_short_val;
  Double_t        integral_medium_val;
  Double_t        integral_long_val;
  Double_t        min_val;
  Int_t           n;
  Double_t        chn_sig[1024];
  Double_t        chn_trg[1024];
  Int_t           n_triggers_val;
  Int_t           Second_trigger_time_val;

  // List of branches
  TBranch        *b_t;   //!
  TBranch        *b_n;   //!
  TBranch        *b_chn_sig;   //!
  TBranch        *b_chn_trg;   //!
  
  get_trigger_delay(char*);
  virtual ~get_trigger_delay();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  Float_t GetAvrgMean(Int_t first, Int_t last,Float_t avrgMean);
  Float_t FindSignalMax(Int_t first, Int_t last, Int_t & time);
  Int_t Get1stTriggerTime(Float_t trigger_level);

  
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef get_trigger_delay_cxx
get_trigger_delay::get_trigger_delay(char *init_filename) : fChain(0) 
{
   TTree *tree;
   filename = init_filename;
   TFile *f = new TFile(filename);
   f->GetObject("rec", tree);

   Init(tree);
}

get_trigger_delay::~get_trigger_delay()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t get_trigger_delay::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t get_trigger_delay::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void get_trigger_delay::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("t", t, &b_t);
   fChain->SetBranchAddress("n", &n, &b_n);
   // CHANGE SIGNAL CHANNEL HERE
   fChain->SetBranchAddress("chn1", chn_sig, &b_chn_sig);
   // CHANGE TRIGGER CHANNEL HERE
   fChain->SetBranchAddress("chn2", chn_trg, &b_chn_trg);
   Notify();
}

Bool_t get_trigger_delay::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void get_trigger_delay::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t get_trigger_delay::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef get_trigger_delay_cxx
