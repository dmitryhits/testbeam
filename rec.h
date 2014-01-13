//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Sep 11 16:57:21 2013 by ROOT version 5.32/01
// from TTree rec/rec
// found on file: PLTS116-m350V.root
//////////////////////////////////////////////////////////

#ifndef rec_h
#define rec_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TGraph.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class rec {
  char *filename;
 public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  Double_t        t[1024];
  Double_t        meanval;
  Double_t        integralval;
  Double_t        integral2val;
  Double_t        minval;
  Int_t           n;
  Double_t        chn_sig[1024];
  
  // List of branches
  TBranch        *b_t;   //!
  TBranch        *b_n;   //!
  TBranch        *b_chn_sig;   //!
  
  rec(char*);
  virtual ~rec();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  Float_t GetAvrgMean(Int_t first, Int_t last,Float_t avrgMean);
  
  Float_t FindMinimum(Int_t first, Int_t last);
  void DrawGraph();
  
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef rec_cxx
rec::rec(char *init_filename) : fChain(0) 
{
   TTree *tree;
   filename = init_filename;
   TFile *f = new TFile(filename);
   f->GetObject("rec", tree);

   Init(tree);
}

rec::~rec()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t rec::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t rec::LoadTree(Long64_t entry)
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

void rec::Init(TTree *tree)
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
   fChain->SetBranchAddress("chn3", chn_sig, &b_chn_sig);
   Notify();
}

Bool_t rec::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void rec::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t rec::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef rec_cxx
