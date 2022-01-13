//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Dec 18 17:17:49 2021 by ROOT version 6.22/02
// from TTree fstHits/fstHits_QA
// found on file: 2.root
//////////////////////////////////////////////////////////

#ifndef fstHits_h
#define fstHits_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#define NBIN 70
#define RMIN 5
#define RMAX 28
#define NRBIN 8
#define NPHIBIN 128

// Header file for the classes stored in the TTree if any.

class fstHits {
public :
    TString inFilename;
    TH2D* hMult12;
    TH2D* hMult13;
    TH2D* hMult23;

    TH2D* hTemp1_xy;
    TH2D* hTemp2_xy;
    TH2D* hTemp3_xy;
    TH2D* hTrack12_xy;
    TH2D* hTrack13_xy;
    TH2D* hTrack23_xy;
    TH2D* hTrack123_xy;
    TH2D* hEff1_xy;
    TH2D* hEff2_xy;
    TH2D* hEff3_xy;
    
    TH2D* hTemp1_rphi;
    TH2D* hTemp2_rphi;
    TH2D* hTemp3_rphi;
    TH2D* hTrack12_rphi;
    TH2D* hTrack13_rphi;
    TH2D* hTrack23_rphi;
    TH2D* hTrack123_rphi;
    TH2D* hEff1_rphi;
    TH2D* hEff2_rphi;
    TH2D* hEff3_rphi;

    TH1D* hTemp1_r;
    TH1D* hTemp2_r;
    TH1D* hTemp3_r;
    TH1D* hTrack12_r;
    TH1D* hTrack13_r;
    TH1D* hTrack23_r;
    TH1D* hTrack123_r;
    TH1D* hEff1_r;
    TH1D* hEff2_r;
    TH1D* hEff3_r;

    TH1D* hTemp1_phi;
    TH1D* hTemp2_phi;
    TH1D* hTemp3_phi;
    TH1D* hTrack12_phi;
    TH1D* hTrack13_phi;
    TH1D* hTrack23_phi;
    TH1D* hTrack123_phi;
    TH1D* hEff1_phi;
    TH1D* hEff2_phi;
    TH1D* hEff3_phi;

    void InitHistogram();
    void FillHistogram();
    void DrawHistogram();

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           hits_hitId;
   Int_t           hits_wedge;
   Int_t           hits_sensor;
   Int_t           hits_apv;
   Int_t           hits_idTruth;
   Int_t           hits_EventId;
   Int_t           hits_maxTimeBin;
   Int_t           hits_clusteringType;
   Int_t           hits_nRawHits;
   Int_t           hits_nRawHitsR;
   Int_t           hits_nRawHitsPhi;
   Float_t         hits_meanPhiStrip;
   Float_t         hits_meanRStrip;
   Float_t         hits_localR;
   Float_t         hits_localPhi;
   Float_t         hits_localZ;
   Float_t         hits_x;
   Float_t         hits_y;
   Float_t         hits_z;
   Float_t         hits_charge;
   Float_t         hits_chargeErr;

   // List of branches
   TBranch        *b_hits;   //!

   fstHits(TTree *tree=0,TString infilename="st_physics_22349008_raw_3000003.fstQa.root");
   virtual ~fstHits();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef fstHits_cxx
fstHits::fstHits(TTree *tree,TString infilename) : fChain(0)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   inFilename=infilename;
    
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(infilename);
      if (!f || !f->IsOpen()) {
         f = new TFile(infilename);
      }
      f->GetObject("fstHits",tree);

   }
   Init(tree);
}

fstHits::~fstHits()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t fstHits::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t fstHits::LoadTree(Long64_t entry)
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

void fstHits::Init(TTree *tree)
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

   fChain->SetBranchAddress("hits", &hits_hitId, &b_hits);
   Notify();
}

Bool_t fstHits::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void fstHits::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t fstHits::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef fstHits_cxx
