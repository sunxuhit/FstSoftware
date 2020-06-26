#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>

void plotMcProjection()
{
  TFile *File_InPut = TFile::Open("../../output/simulation/FstMcProjection.root");
  TH1F *h_mIstProjResX_2Layer    = (TH1F*)File_InPut->Get("h_mIstProjResX_2Layer");
  TH1F *h_mIstProjResY_2Layer    = (TH1F*)File_InPut->Get("h_mIstProjResY_2Layer");
  TH2F *h_mIstProjResXY_2Layer   = (TH2F*)File_InPut->Get("h_mIstProjResXY_2Layer");
  TH1F *h_mFstProjResR_2Layer    = (TH1F*)File_InPut->Get("h_mFstProjResR_2Layer");
  TH1F *h_mFstProjResPhi_2Layer  = (TH1F*)File_InPut->Get("h_mFstProjResPhi_2Layer");
  TH2F *h_mFstProjResRPhi_2Layer = (TH2F*)File_InPut->Get("h_mFstProjResRPhi_2Layer");

  TFile *File_InPutData = TFile::Open("../../output/configuration/ScanClusterRadiusPed/Mixed/FstTracking_HV200V_Th4.0Tb2Ped2.5Ped3.5_withPed_withCMNCorr.root");
  TH1F *h_mSimpleClustersTrackFstResR_2Layer_Rstrip0 = (TH1F*)File_InPutData->Get("h_mSimpleClustersTrackFstResR_2Layer_Rstrip0");
  TH1F *h_mSimpleClustersTrackFstResPhi_2Layer_Rstrip0 = (TH1F*)File_InPutData->Get("h_mSimpleClustersTrackFstResPhi_2Layer_Rstrip0");

  TCanvas *c_Residual = new TCanvas("c_Residual","c_Residual",10,10,1200,800);
  c_Residual->Divide(3,2);
  for(int i_pad = 0; i_pad < 6; ++ i_pad)
  {
    c_Residual->cd(i_pad+1);
    c_Residual->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Residual->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Residual->cd(i_pad+1)->SetTicks(1,1);
    c_Residual->cd(i_pad+1)->SetGrid(0,0);
  }

  c_Residual->cd(1);
  h_mIstProjResXY_2Layer->GetXaxis()->SetTitle("x^{FST,proj}_{center} - x^{FST,proj}_{gen} (mm)");
  h_mIstProjResXY_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mIstProjResXY_2Layer->GetXaxis()->CenterTitle();
  h_mIstProjResXY_2Layer->GetYaxis()->SetTitle("y^{FST,proj}_{center} - y^{FST,proj}_{gen} (mm)");
  h_mIstProjResXY_2Layer->GetYaxis()->SetTitleSize(0.06);
  h_mIstProjResXY_2Layer->GetYaxis()->CenterTitle();
  h_mIstProjResXY_2Layer->Draw("colz");

  c_Residual->cd(2);
  h_mIstProjResX_2Layer->GetXaxis()->SetTitle("x^{FST,proj}_{center} - x^{FST,proj}_{gen} (mm)");
  h_mIstProjResX_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mIstProjResX_2Layer->GetXaxis()->CenterTitle();
  h_mIstProjResX_2Layer->Draw("hE");

  c_Residual->cd(3);
  h_mIstProjResY_2Layer->GetXaxis()->SetTitle("y^{FST,proj}_{center} - y^{FST,proj}_{gen} (mm)");
  h_mIstProjResY_2Layer->GetXaxis()->CenterTitle();
  h_mIstProjResY_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mIstProjResY_2Layer->Draw("hE");

  c_Residual->cd(4);
  h_mFstProjResRPhi_2Layer->GetXaxis()->SetTitle("r^{FST,proj}_{center} - r^{FST,readout}_{gen} (mm)");
  h_mFstProjResRPhi_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mFstProjResRPhi_2Layer->GetXaxis()->CenterTitle();
  h_mFstProjResRPhi_2Layer->GetYaxis()->SetTitle("#phi^{FST,proj}_{center} - y^{FST,readout}_{gen} (mm)");
  h_mFstProjResRPhi_2Layer->GetYaxis()->SetTitleSize(0.06);
  h_mFstProjResRPhi_2Layer->GetYaxis()->CenterTitle();
  h_mFstProjResRPhi_2Layer->Draw("colz");

  c_Residual->cd(5);
  h_mFstProjResR_2Layer->GetXaxis()->SetTitle("r^{FST,proj}_{center} - r^{FST,readout}_{gen} (mm)");
  h_mFstProjResR_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mFstProjResR_2Layer->GetXaxis()->CenterTitle();
  double InteR_MC = h_mFstProjResR_2Layer->GetMaximum();
  double InteR_Data = h_mSimpleClustersTrackFstResR_2Layer_Rstrip0->GetMaximum();
  h_mFstProjResR_2Layer->Scale(InteR_Data/InteR_MC);
  h_mFstProjResR_2Layer->Draw("hE");
  h_mSimpleClustersTrackFstResR_2Layer_Rstrip0->SetLineColor(2);
  h_mSimpleClustersTrackFstResR_2Layer_Rstrip0->Draw("hE same");

  TLegend *leg = new TLegend(0.20,0.6,0.4,0.7);
  leg->AddEntry(h_mFstProjResR_2Layer,"MC","l");
  leg->AddEntry(h_mSimpleClustersTrackFstResR_2Layer_Rstrip0,"Data","l");
  leg->Draw("same");

  c_Residual->cd(6);
  h_mFstProjResPhi_2Layer->GetXaxis()->SetTitle("#phi^{FST,proj}_{center} - #phi^{FST,readout}_{gen} (mm)");
  h_mFstProjResPhi_2Layer->GetXaxis()->CenterTitle();
  h_mFstProjResPhi_2Layer->GetXaxis()->SetTitleSize(0.06);
  double IntePhi_MC = h_mFstProjResPhi_2Layer->GetMaximum();
  double IntePhi_Data = h_mSimpleClustersTrackFstResPhi_2Layer_Rstrip0->GetMaximum();
  h_mFstProjResPhi_2Layer->Scale(IntePhi_Data/IntePhi_MC);
  h_mFstProjResPhi_2Layer->Draw("hE");
  h_mSimpleClustersTrackFstResPhi_2Layer_Rstrip0->SetLineColor(2);
  h_mSimpleClustersTrackFstResPhi_2Layer_Rstrip0->Draw("hE same");
  leg->Draw("same");

  c_Residual->SaveAs("./figures/c_Residual.eps");
}
