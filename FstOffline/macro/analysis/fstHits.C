#define fstHits_cxx
#include "fstHits.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void fstHits::Loop()
{
//   In a ROOT session, you can do:
//      root> .L fstHits.C
//      root> fstHits t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
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

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   Int_t cEventId=0;
   InitHistogram();
   Int_t mult1=0,mult2=0,mult3=0;
    
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
    
      if(cEventId!=hits_EventId) {
          hMult12->Fill(mult1,mult2);
          hMult13->Fill(mult1,mult3);
          hMult23->Fill(mult2,mult3);
          mult1=0;mult2=0;mult3=0;
          FillHistogram();
          cEventId=hits_EventId;
      } else {
          float r=sqrt(hits_x*hits_x+hits_y*hits_y);
          float phi=TMath::ATan2(hits_y,hits_x);
          if(hits_wedge<13) {
              if(hits_sensor>0) mult1++;
              hTemp1_xy->Fill(hits_x,hits_y);
              hTemp1_r->Fill(r);
              hTemp1_rphi->Fill(r,phi);
              hTemp1_phi->Fill(phi);
          } else if(hits_wedge<25) {
              if(hits_sensor>0) mult2++;
              hTemp2_xy->Fill(hits_x,hits_y);
              hTemp2_rphi->Fill(r,phi);
              hTemp2_r->Fill(r);
              hTemp2_phi->Fill(phi);
          } else if(hits_wedge<37) {
              if(hits_sensor>0) mult3++;
              hTemp3_xy->Fill(hits_x,hits_y);
              hTemp3_rphi->Fill(r,phi);
              hTemp3_r->Fill(r);
              hTemp3_phi->Fill(phi);
          }
      }
   }
    
   DrawHistogram();
}

void fstHits::InitHistogram()
{
    hMult12 =new TH2D("hMult12" ,"",NBIN,0,70,NBIN,0,70);
    hMult13 =new TH2D("hMult13" ,"",NBIN,0,70,NBIN,0,70);
    hMult23 =new TH2D("hMult23" ,"",NBIN,0,70,NBIN,0,70);

    hTemp1_xy =new TH2D("hTemp1_xy" ,"",NBIN,-35,35,NBIN,-35,35);
    hTemp2_xy =new TH2D("hTemp2_xy" ,"",NBIN,-35,35,NBIN,-35,35);
    hTemp3_xy =new TH2D("hTemp3_xy" ,"",NBIN,-35,35,NBIN,-35,35);
    hTrack12_xy=new TH2D("hTrack12_xy","",NBIN,-35,35,NBIN,-35,35);
    hTrack13_xy=new TH2D("hTrack13_xy","",NBIN,-35,35,NBIN,-35,35);
    hTrack23_xy=new TH2D("hTrack23_xy","",NBIN,-35,35,NBIN,-35,35);
    hTrack123_xy=new TH2D("hTrack123_xy","",NBIN,-35,35,NBIN,-35,35);
    hEff1_xy =new TH2D("hEff1_xy" ,"",NBIN,-35,35,NBIN,-35,35);
    hEff2_xy =new TH2D("hEff2_xy" ,"",NBIN,-35,35,NBIN,-35,35);
    hEff3_xy =new TH2D("hEff3_xy" ,"",NBIN,-35,35,NBIN,-35,35);
    
    hTemp1_rphi =new TH2D("hTemp1_rphi" ,"",NRBIN,RMIN,RMAX,NPHIBIN,-3.1415927,3.1415927);
    hTemp2_rphi =new TH2D("hTemp2_rphi" ,"",NRBIN,RMIN,RMAX,NPHIBIN,-3.1415927,3.1415927);
    hTemp3_rphi =new TH2D("hTemp3_rphi" ,"",NRBIN,RMIN,RMAX,NPHIBIN,-3.1415927,3.1415927);
    hTrack12_rphi=new TH2D("hTrack12_rphi","",NRBIN,RMIN,RMAX,NPHIBIN,-3.1415927,3.1415927);
    hTrack13_rphi=new TH2D("hTrack13_rphi","",NRBIN,RMIN,RMAX,NPHIBIN,-3.1415927,3.1415927);
    hTrack23_rphi=new TH2D("hTrack23_rphi","",NRBIN,RMIN,RMAX,NPHIBIN,-3.1415927,3.1415927);
    hTrack123_rphi=new TH2D("hTrack123_rphi","",NRBIN,RMIN,RMAX,NPHIBIN,-3.1415927,3.1415927);
    hEff1_rphi =new TH2D("hEff1_rphi" ,"",NRBIN,RMIN,RMAX,NPHIBIN,-3.1415927,3.1415927);
    hEff2_rphi =new TH2D("hEff2_rphi" ,"",NRBIN,RMIN,RMAX,NPHIBIN,-3.1415927,3.1415927);
    hEff3_rphi =new TH2D("hEff3_rphi" ,"",NRBIN,RMIN,RMAX,NPHIBIN,-3.1415927,3.1415927);
    
    hTemp1_r =new TH1D("hTemp1_r" ,"",NRBIN,RMIN,RMAX);
    hTemp2_r =new TH1D("hTemp2_r" ,"",NRBIN,RMIN,RMAX);
    hTemp3_r =new TH1D("hTemp3_r" ,"",NRBIN,RMIN,RMAX);
    hTrack12_r=new TH1D("hTrack12_r","",NRBIN,RMIN,RMAX);
    hTrack13_r=new TH1D("hTrack13_r","",NRBIN,RMIN,RMAX);
    hTrack23_r=new TH1D("hTrack23_r","",NRBIN,RMIN,RMAX);
    hTrack123_r=new TH1D("hTrack123_r","",NRBIN,RMIN,RMAX);
    hEff1_r =new TH1D("hEff1_r" ,"",NRBIN,RMIN,RMAX);
    hEff2_r =new TH1D("hEff2_r" ,"",NRBIN,RMIN,RMAX);
    hEff3_r =new TH1D("hEff3_r" ,"",NRBIN,RMIN,RMAX);
    
    hTemp1_phi =new TH1D("hTemp1_phi" ,"",NPHIBIN,-3.1415927,3.1415927);
    hTemp2_phi =new TH1D("hTemp2_phi" ,"",NPHIBIN,-3.1415927,3.1415927);
    hTemp3_phi =new TH1D("hTemp3_phi" ,"",NPHIBIN,-3.1415927,3.1415927);
    hTrack12_phi=new TH1D("hTrack12_phi","",NPHIBIN,-3.1415927,3.1415927);
    hTrack13_phi=new TH1D("hTrack13_phi","",NPHIBIN,-3.1415927,3.1415927);
    hTrack23_phi=new TH1D("hTrack23_phi","",NPHIBIN,-3.1415927,3.1415927);
    hTrack123_phi=new TH1D("hTrack123_phi","",NPHIBIN,-3.1415927,3.1415927);
    hEff1_phi =new TH1D("hEff1_phi" ,"",NPHIBIN,-3.1415927,3.1415927);
    hEff2_phi =new TH1D("hEff2_phi" ,"",NPHIBIN,-3.1415927,3.1415927);
    hEff3_phi =new TH1D("hEff3_phi" ,"",NPHIBIN,-3.1415927,3.1415927);
}

void fstHits::FillHistogram()
{
    for(Int_t i=1;i<=NBIN;i++) {
        for(Int_t j=1;j<=NBIN;j++) {
            if(hTemp1_xy->GetBinContent(i,j)>0&&hTemp2_xy->GetBinContent(i,j)>0) hTrack12_xy->SetBinContent(i,j,hTrack12_xy->GetBinContent(i,j)+1);
            if(hTemp1_xy->GetBinContent(i,j)>0&&hTemp3_xy->GetBinContent(i,j)>0) hTrack13_xy->SetBinContent(i,j,hTrack13_xy->GetBinContent(i,j)+1);
            if(hTemp2_xy->GetBinContent(i,j)>0&&hTemp3_xy->GetBinContent(i,j)>0) hTrack23_xy->SetBinContent(i,j,hTrack23_xy->GetBinContent(i,j)+1);
            if(hTemp1_xy->GetBinContent(i,j)>0&&hTemp2_xy->GetBinContent(i,j)>0&&hTemp3_xy->GetBinContent(i,j)>0) hTrack123_xy->SetBinContent(i,j,hTrack123_xy->GetBinContent(i,j)+1);
        }
    }

    for(Int_t j=1;j<=NPHIBIN;j++) {
        for(Int_t i=5;i<=4;i++) {
            if(hTemp1_rphi->GetBinContent(i,j)>0&&hTemp2_rphi->GetBinContent(i,j)>0&&hTemp3_rphi->GetBinContent(i,j)>0) {  hTrack12_rphi->SetBinContent(i,j,hTrack12_rphi->GetBinContent(i,j)+1);
                hTrack13_rphi->SetBinContent(i,j,hTrack13_rphi->GetBinContent(i,j)+1);
                hTrack23_rphi->SetBinContent(i,j,hTrack23_rphi->GetBinContent(i,j)+1);
                hTrack123_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j)+1);
                
                hTrack12_r->SetBinContent(i,hTrack12_r->GetBinContent(i)+1);
                hTrack13_r->SetBinContent(i,hTrack13_r->GetBinContent(i)+1);
                hTrack23_r->SetBinContent(i,hTrack23_r->GetBinContent(i)+1);
                hTrack123_r->SetBinContent(i,hTrack123_r->GetBinContent(i)+1);
                
                hTrack12_phi->SetBinContent(j,hTrack12_phi->GetBinContent(j)+1);
                hTrack13_phi->SetBinContent(j,hTrack13_phi->GetBinContent(j)+1);
                hTrack23_phi->SetBinContent(j,hTrack23_phi->GetBinContent(j)+1);
                hTrack123_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j)+1);
            }
            if(hTemp1_rphi->GetBinContent(i,j)>0&&hTemp2_rphi->GetBinContent(i,j)>0&&hTemp3_rphi->GetBinContent(i,j)==0) {
                hTrack12_rphi->SetBinContent(i,j,hTrack12_rphi->GetBinContent(i,j)+1);
                hTrack12_r->SetBinContent(i,hTrack12_r->GetBinContent(i)+1);
                hTrack12_phi->SetBinContent(j,hTrack12_phi->GetBinContent(j)+1);
                for(Int_t k=100;k<=4;k++) {
                    if(hTemp3_rphi->GetBinContent(k,j)>0) {
                        hTrack123_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j)+1);
                        hTrack123_r->SetBinContent(i,hTrack123_r->GetBinContent(i)+1);
                        hTrack123_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j)+1);
                        k=100;
                    }
                }
            }
            if(hTemp1_rphi->GetBinContent(i,j)>0&&hTemp3_rphi->GetBinContent(i,j)>0&&hTemp2_rphi->GetBinContent(i,j)==0) {
                hTrack13_rphi->SetBinContent(i,j,hTrack13_rphi->GetBinContent(i,j)+1);
                hTrack13_r->SetBinContent(i,hTrack13_r->GetBinContent(i)+1);
                hTrack13_phi->SetBinContent(j,hTrack13_phi->GetBinContent(j)+1);
                for(Int_t k=100;k<=4;k++) {
                    if(hTemp2_rphi->GetBinContent(k,j)>0) {
                        hTrack123_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j)+1);
                        hTrack123_r->SetBinContent(i,hTrack123_r->GetBinContent(i)+1);
                        hTrack123_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j)+1);
                        k=100;
                    }
                }
            }
            if(hTemp2_rphi->GetBinContent(i,j)>0&&hTemp3_rphi->GetBinContent(i,j)>0&&hTemp1_rphi->GetBinContent(i,j)==0) {
                hTrack23_rphi->SetBinContent(i,j,hTrack23_rphi->GetBinContent(i,j)+1);
                hTrack23_r->SetBinContent(i,hTrack23_r->GetBinContent(i)+1);
                hTrack23_phi->SetBinContent(j,hTrack23_phi->GetBinContent(j)+1);
                for(Int_t k=100;k<=4;k++) {
                    if(hTemp1_rphi->GetBinContent(k,j)>0) {
                        hTrack123_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j)+1);
                        hTrack123_r->SetBinContent(i,hTrack123_r->GetBinContent(i)+1);
                        hTrack123_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j)+1);
                        k=100;
                    }
                }
            }
        }

        for(Int_t i=5;i<=8;i++) {
            if(hTemp1_rphi->GetBinContent(i,j)>0&&hTemp2_rphi->GetBinContent(i,j)>0&&hTemp3_rphi->GetBinContent(i,j)>0) {  hTrack12_rphi->SetBinContent(i,j,hTrack12_rphi->GetBinContent(i,j)+1);
                hTrack13_rphi->SetBinContent(i,j,hTrack13_rphi->GetBinContent(i,j)+1);
                hTrack23_rphi->SetBinContent(i,j,hTrack23_rphi->GetBinContent(i,j)+1);
                hTrack123_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j)+1);
                
                hTrack12_r->SetBinContent(i,hTrack12_r->GetBinContent(i)+1);
                hTrack13_r->SetBinContent(i,hTrack13_r->GetBinContent(i)+1);
                hTrack23_r->SetBinContent(i,hTrack23_r->GetBinContent(i)+1);
                hTrack123_r->SetBinContent(i,hTrack123_r->GetBinContent(i)+1);
                
                hTrack12_phi->SetBinContent(j,hTrack12_phi->GetBinContent(j)+1);
                hTrack13_phi->SetBinContent(j,hTrack13_phi->GetBinContent(j)+1);
                hTrack23_phi->SetBinContent(j,hTrack23_phi->GetBinContent(j)+1);
                hTrack123_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j)+1);
            }
            if(hTemp1_rphi->GetBinContent(i,j)>0&&hTemp2_rphi->GetBinContent(i,j)>0&&hTemp3_rphi->GetBinContent(i,j)==0) {
                hTrack12_rphi->SetBinContent(i,j,hTrack12_rphi->GetBinContent(i,j)+1);
                hTrack12_r->SetBinContent(i,hTrack12_r->GetBinContent(i)+1);
                hTrack12_phi->SetBinContent(j,hTrack12_phi->GetBinContent(j)+1);
                for(Int_t k=500;k<=8;k++) {
                    if(hTemp3_rphi->GetBinContent(k,j)>0) {
                        hTrack123_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j)+1);
                        hTrack123_r->SetBinContent(i,hTrack123_r->GetBinContent(i)+1);
                        hTrack123_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j)+1);
                        k=100;
                    }
                }
            }
            if(hTemp1_rphi->GetBinContent(i,j)>0&&hTemp3_rphi->GetBinContent(i,j)>0&&hTemp2_rphi->GetBinContent(i,j)==0) {
                hTrack13_rphi->SetBinContent(i,j,hTrack13_rphi->GetBinContent(i,j)+1);
                hTrack13_r->SetBinContent(i,hTrack13_r->GetBinContent(i)+1);
                hTrack13_phi->SetBinContent(j,hTrack13_phi->GetBinContent(j)+1);
                for(Int_t k=500;k<=8;k++) {
                    if(hTemp2_rphi->GetBinContent(k,j)>0) {
                        hTrack123_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j)+1);
                        hTrack123_r->SetBinContent(i,hTrack123_r->GetBinContent(i)+1);
                        hTrack123_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j)+1);
                        k=100;
                    }
                }
            }
            if(hTemp2_rphi->GetBinContent(i,j)>0&&hTemp3_rphi->GetBinContent(i,j)>0&&hTemp1_rphi->GetBinContent(i,j)==0) {
                hTrack23_rphi->SetBinContent(i,j,hTrack23_rphi->GetBinContent(i,j)+1);
                hTrack23_r->SetBinContent(i,hTrack23_r->GetBinContent(i)+1);
                hTrack23_phi->SetBinContent(j,hTrack23_phi->GetBinContent(j)+1);
                for(Int_t k=500;k<=8;k++) {
                    if(hTemp1_rphi->GetBinContent(k,j)>0) {
                        hTrack123_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j)+1);
                        hTrack123_r->SetBinContent(i,hTrack123_r->GetBinContent(i)+1);
                        hTrack123_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j)+1);
                        k=100;
                    }
                }
            }
        }
    }
    
    for(Int_t i=1;i<=NBIN;i++) {
        for(Int_t j=1;j<=NBIN;j++) {
            hTemp1_xy->SetBinContent(i,j,0);
            hTemp2_xy->SetBinContent(i,j,0);
            hTemp3_xy->SetBinContent(i,j,0);
        }
    }
    for(Int_t i=1;i<=NRBIN;i++) {
        for(Int_t j=1;j<=NPHIBIN;j++) {
            hTemp1_rphi->SetBinContent(i,j,0);
            hTemp2_rphi->SetBinContent(i,j,0);
            hTemp3_rphi->SetBinContent(i,j,0);
        }
    }
    for(Int_t i=1;i<=NRBIN;i++) {
        hTemp1_r->SetBinContent(i,0);
        hTemp2_r->SetBinContent(i,0);
        hTemp3_r->SetBinContent(i,0);
    }
    for(Int_t j=1;j<=NPHIBIN;j++) {
        hTemp1_phi->SetBinContent(j,0);
        hTemp2_phi->SetBinContent(j,0);
        hTemp3_phi->SetBinContent(j,0);
    }    
}

void fstHits::DrawHistogram() {
    for(Int_t i=1;i<=NBIN;i++) {
        for(Int_t j=1;j<=NBIN;j++) {
            hEff1_xy->SetBinContent(i,j,hTrack123_xy->GetBinContent(i,j));
            hEff2_xy->SetBinContent(i,j,hTrack123_xy->GetBinContent(i,j));
            hEff3_xy->SetBinContent(i,j,hTrack123_xy->GetBinContent(i,j));
        }
    }
    for(Int_t i=1;i<=8;i++) {
        for(Int_t j=1;j<=NPHIBIN;j++) {
            hEff1_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j));
            hEff2_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j));
            hEff3_rphi->SetBinContent(i,j,hTrack123_rphi->GetBinContent(i,j));
        }
    }
    for(Int_t i=1;i<=NBIN;i++) {
        hEff1_r->SetBinContent(i,hTrack123_r->GetBinContent(i));
        hEff2_r->SetBinContent(i,hTrack123_r->GetBinContent(i));
        hEff3_r->SetBinContent(i,hTrack123_r->GetBinContent(i));
    }
    for(Int_t j=1;j<=NPHIBIN;j++) {
        hEff1_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j));
        hEff2_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j));
        hEff3_phi->SetBinContent(j,hTrack123_phi->GetBinContent(j));
    }
    
    TCanvas *c_mult=new TCanvas("c_mult","",1200,400);
    c_mult->Divide(3,1);
    c_mult->cd(1);
    hMult12->Draw("COLZ");
    c_mult->cd(2);
    hMult13->Draw("COLZ");
    c_mult->cd(3);
    hMult23->Draw("COLZ");
    c_mult->SaveAs(inFilename+"_mult.pdf");

    TCanvas *c_xy=new TCanvas("c_xy","",1200,800);
    c_xy->Divide(4,2);
    c_xy->cd(1);
    hTrack12_xy->Draw("COLZ");
    c_xy->cd(2);
    hTrack13_xy->Draw("COLZ");
    c_xy->cd(3);
    hTrack23_xy->Draw("COLZ");
    c_xy->cd(4);
    hTrack123_xy->Draw("COLZ");
    c_xy->cd(5);
    hEff1_xy->Divide(hTrack23_xy);
    hEff1_xy->Draw("COLZ");
    c_xy->cd(6);
    hEff2_xy->Divide(hTrack13_xy);
    hEff2_xy->Draw("COLZ");
    c_xy->cd(7);
    hEff3_xy->Divide(hTrack12_xy);
    hEff3_xy->Draw("COLZ");
    c_xy->SaveAs(inFilename+"_xy.pdf");
    
    TCanvas *c_rphi=new TCanvas("c_rphi","",1200,800);
    c_rphi->Divide(4,2);
    c_rphi->cd(1);
    hTrack12_rphi->Draw("COLZ");
    c_rphi->cd(2);
    hTrack13_rphi->Draw("COLZ");
    c_rphi->cd(3);
    hTrack23_rphi->Draw("COLZ");
    c_rphi->cd(4);
    hTrack123_rphi->Draw("COLZ");
    c_rphi->cd(5);
    hEff1_rphi->Divide(hTrack23_rphi);
    hEff1_rphi->SetMaximum(1.1);
    hEff1_rphi->SetMinimum(0);
    hEff1_rphi->Draw("COLZ");
    c_rphi->cd(6);
    hEff2_rphi->Divide(hTrack13_rphi);
    hEff2_rphi->SetMaximum(1.1);
    hEff2_rphi->SetMinimum(0);
    hEff2_rphi->Draw("COLZ");
    c_rphi->cd(7);
    hEff3_rphi->Divide(hTrack12_rphi);
    hEff3_rphi->SetMaximum(1.1);
    hEff3_rphi->SetMinimum(0);
    hEff3_rphi->Draw("COLZ");
    c_rphi->SaveAs(inFilename+"_rphi.pdf");
 
    TCanvas *c_r=new TCanvas("c_r","",1200,800);
    c_r->Divide(4,2);
    c_r->cd(1);
    hTrack12_r->Draw();
    c_r->cd(2);
    hTrack13_r->Draw();
    c_r->cd(3);
    hTrack23_r->Draw();
    c_r->cd(4);
    hTrack123_r->Draw();
    c_r->cd(5);
    hEff1_r->Divide(hTrack23_r);
    hEff1_r->SetMaximum(1.1);
    hEff1_r->SetMinimum(0);
    hEff1_r->Draw();
    c_r->cd(6);
    hEff2_r->Divide(hTrack13_r);
    hEff2_r->SetMaximum(1.1);
    hEff2_r->SetMinimum(0);
    hEff2_r->Draw();
    c_r->cd(7);
    hEff3_r->Divide(hTrack12_r);
    hEff3_r->SetMaximum(1.1);
    hEff3_r->SetMinimum(0);
    hEff3_r->Draw();
    c_r->SaveAs(inFilename+"_r.pdf");
    
    TCanvas *c_phi=new TCanvas("c_phi","",1200,800);
    c_phi->Divide(4,2);
    c_phi->cd(1);
    hTrack12_phi->Draw();
    c_phi->cd(2);
    hTrack13_phi->Draw();
    c_phi->cd(3);
    hTrack23_phi->Draw();
    c_phi->cd(4);
    hTrack123_phi->Draw();
    c_phi->cd(5);
    hEff1_phi->Divide(hTrack23_phi);
    hEff1_phi->SetMaximum(1.1);
    hEff1_phi->SetMinimum(0);
    hEff1_phi->Draw();
    c_phi->cd(6);
    hEff2_phi->Divide(hTrack13_phi);
    hEff2_phi->SetMaximum(1.1);
    hEff2_phi->SetMinimum(0);
    hEff2_phi->Draw();
    c_phi->cd(7);
    hEff3_phi->Divide(hTrack12_phi);
    hEff3_phi->SetMaximum(1.1);
    hEff3_phi->SetMinimum(0);
    hEff3_phi->Draw();
    c_phi->SaveAs(inFilename+"_phi.pdf");
    
    TFile *output=new TFile(inFilename+".eff.root","recreate");
    output->cd();
    hEff1_rphi->Write();
    hEff1_r->Write();
    hEff1_phi->Write();
    hEff2_rphi->Write();
    hEff2_r->Write();
    hEff2_phi->Write();
    hEff3_rphi->Write();
    hEff3_r->Write();
    hEff3_phi->Write();
    output->Close();
}

