#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>

using namespace std;

void plotMcProjection(bool isRot = true)
{
  string mRot = "woRot";
  if(isRot) mRot = "Rot";
  string inputname = Form("../../output/simulation/FstMcProjection_%s.root",mRot.c_str());
  TFile *File_InPut = TFile::Open(inputname.c_str());
  TH1F *h_mIstProjResX_2Layer    = (TH1F*)File_InPut->Get("h_mIstProjResX_2Layer");
  TH1F *h_mIstProjResY_2Layer    = (TH1F*)File_InPut->Get("h_mIstProjResY_2Layer");
  TH2F *h_mIstProjResXY_2Layer   = (TH2F*)File_InPut->Get("h_mIstProjResXY_2Layer");
  TH1F *h_mFstProjResX_2Layer    = (TH1F*)File_InPut->Get("h_mFstProjResX_2Layer");
  TH1F *h_mFstProjResY_2Layer    = (TH1F*)File_InPut->Get("h_mFstProjResY_2Layer");
  TH2F *h_mFstProjResXY_2Layer   = (TH2F*)File_InPut->Get("h_mFstProjResXY_2Layer");
  TH1F *h_mFstProjResR_2Layer    = (TH1F*)File_InPut->Get("h_mFstProjResR_2Layer");
  TH1F *h_mFstProjResPhi_2Layer  = (TH1F*)File_InPut->Get("h_mFstProjResPhi_2Layer");
  TH2F *h_mFstProjResRPhi_2Layer = (TH2F*)File_InPut->Get("h_mFstProjResRPhi_2Layer");

  TH1F *h_mFstSimResR_2Layer     = (TH1F*)File_InPut->Get("h_mFstSimResR_2Layer");
  TH1F *h_mFstSimResPhi_2Layer   = (TH1F*)File_InPut->Get("h_mFstSimResPhi_2Layer");
  TH2F *h_mFstSimResRPhi_2Layer  = (TH2F*)File_InPut->Get("h_mFstSimResRPhi_2Layer");

  string mode = "Scan";
  // string mode = "Simple";

  TFile *File_InPutData = TFile::Open("../../output/configuration/ScanClusterRadiusPed/Mixed/FstTracking_HV200V_Th4.0Tb2Ped2.5Ped3.5_withPed_withCMNCorr.root");
  TH1F *h_mClustersTrackFstResR_2Layer[4];
  TH1F *h_mClustersTrackFstResPhi_2Layer[4];
  for(int i_rstrp = 0; i_rstrp < 4; ++i_rstrp)
  {
    string HistName;
    HistName = Form("h_m%sClustersTrackFstResR_2Layer_Rstrip%d",mode.c_str(),i_rstrp);
    h_mClustersTrackFstResR_2Layer[i_rstrp] = (TH1F*)File_InPutData->Get(HistName.c_str());
    HistName = Form("h_m%sClustersTrackFstResPhi_2Layer_Rstrip%d",mode.c_str(),i_rstrp);
    h_mClustersTrackFstResPhi_2Layer[i_rstrp] = (TH1F*)File_InPutData->Get(HistName.c_str());
  }

  TCanvas *c_Residual = new TCanvas("c_Residual","c_Residual",10,10,900,900);
  c_Residual->Divide(3,3);
  for(int i_pad = 0; i_pad < 9; ++ i_pad)
  {
    c_Residual->cd(i_pad+1);
    c_Residual->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Residual->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Residual->cd(i_pad+1)->SetTicks(1,1);
    c_Residual->cd(i_pad+1)->SetGrid(0,0);
  }

  // IST
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

  //FST X-Y
  c_Residual->cd(4);
  h_mFstProjResXY_2Layer->GetXaxis()->SetTitle("x^{FST,proj}_{center} - x^{FST,readout}_{gen} (mm)");
  h_mFstProjResXY_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mFstProjResXY_2Layer->GetXaxis()->CenterTitle();
  h_mFstProjResXY_2Layer->GetYaxis()->SetTitle("y^{FST,proj}_{center} - y^{FST,readout}_{gen} (rad)");
  h_mFstProjResXY_2Layer->GetYaxis()->SetTitleSize(0.06);
  h_mFstProjResXY_2Layer->GetYaxis()->CenterTitle();
  h_mFstProjResXY_2Layer->Draw("colz");

  c_Residual->cd(5);
  h_mFstProjResX_2Layer->GetXaxis()->SetTitle("x^{FST,proj}_{center} - x^{FST,readout}_{gen} (mm)");
  h_mFstProjResX_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mFstProjResX_2Layer->GetXaxis()->CenterTitle();
  h_mFstProjResX_2Layer->SetMarkerStyle(24);
  h_mFstProjResX_2Layer->SetMarkerSize(0.8);
  h_mFstProjResX_2Layer->SetMarkerColor(1);
  h_mFstProjResX_2Layer->Draw("pE");

  c_Residual->cd(6);
  h_mFstProjResY_2Layer->GetXaxis()->SetTitle("y^{FST,proj}_{center} - y^{FST,readout}_{gen} (mm)");
  h_mFstProjResY_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mFstProjResY_2Layer->GetXaxis()->CenterTitle();
  h_mFstProjResY_2Layer->SetMarkerStyle(24);
  h_mFstProjResY_2Layer->SetMarkerSize(0.8);
  h_mFstProjResY_2Layer->SetMarkerColor(1);
  h_mFstProjResY_2Layer->Draw("pE");

  //FST R-Phi
  c_Residual->cd(7);
  h_mFstProjResRPhi_2Layer->GetXaxis()->SetTitle("r^{FST,proj}_{center} - r^{FST,readout}_{gen} (mm)");
  h_mFstProjResRPhi_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mFstProjResRPhi_2Layer->GetXaxis()->CenterTitle();
  h_mFstProjResRPhi_2Layer->GetYaxis()->SetTitle("#phi^{FST,proj}_{center} - #phi^{FST,readout}_{gen} (rad)");
  h_mFstProjResRPhi_2Layer->GetYaxis()->SetTitleSize(0.06);
  h_mFstProjResRPhi_2Layer->GetYaxis()->CenterTitle();
  h_mFstProjResRPhi_2Layer->Draw("colz");

  c_Residual->cd(8);
  double InteR_MC = h_mFstProjResR_2Layer->GetMaximum();
  double InteR_MC_IST = h_mFstSimResR_2Layer->GetMaximum();
  double InteR_Data[4];
  for(int i_rstrp = 0; i_rstrp < 4; ++i_rstrp)
  {
    InteR_Data[i_rstrp] = h_mClustersTrackFstResR_2Layer[i_rstrp]->GetMaximum();
  }
  TLegend *leg = new TLegend(0.60,0.3,0.85,0.7);
  h_mFstProjResR_2Layer->GetXaxis()->SetTitle("r^{FST,proj}_{center} - r^{FST,readout}_{gen} (mm)");
  h_mFstProjResR_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mFstProjResR_2Layer->GetXaxis()->CenterTitle();
  h_mFstProjResR_2Layer->GetXaxis()->SetRangeUser(-100.0,100.0);
  h_mFstProjResR_2Layer->SetMarkerStyle(24);
  h_mFstProjResR_2Layer->SetMarkerSize(0.8);
  h_mFstProjResR_2Layer->SetMarkerColor(1);
  h_mFstProjResR_2Layer->Scale(InteR_Data[0]/InteR_MC);
  h_mFstProjResR_2Layer->Draw("pE");
  leg->AddEntry(h_mFstProjResR_2Layer,"MC","p");
  // h_mFstSimResR_2Layer->SetLineStyle(2);
  // h_mFstSimResR_2Layer->SetLineWidth(2);
  // h_mFstSimResR_2Layer->SetLineColor(1);
  // h_mFstSimResR_2Layer->Scale(InteR_Data[0]/InteR_MC_IST);
  // h_mFstSimResR_2Layer->Draw("l same");
  // leg->AddEntry(h_mFstSimResR_2Layer,"MC with perfect IST","l");
  for(int i_rstrp = 0; i_rstrp < 4; ++i_rstrp)
  {
    h_mClustersTrackFstResR_2Layer[i_rstrp]->SetLineColor(i_rstrp+1);
    h_mClustersTrackFstResR_2Layer[i_rstrp]->Scale(InteR_Data[0]/InteR_Data[i_rstrp]);
    if(!isRot) 
    {
      h_mClustersTrackFstResR_2Layer[i_rstrp]->Draw("hE same");
      string legend = Form("%s Rstrip%d",mode.c_str(),i_rstrp);
      leg->AddEntry(h_mClustersTrackFstResR_2Layer[i_rstrp],legend.c_str(),"l");
    }
  }
  if(!isRot) leg->Draw("same");

  c_Residual->cd(9);
  double IntePhi_MC = h_mFstProjResPhi_2Layer->GetMaximum();
  double IntePhi_Data[4];
  for(int i_rstrp = 0; i_rstrp < 4; ++i_rstrp)
  {
    IntePhi_Data[i_rstrp] = h_mClustersTrackFstResPhi_2Layer[i_rstrp]->GetMaximum();
  }
  h_mFstProjResPhi_2Layer->GetXaxis()->SetTitle("#phi^{FST,proj}_{center} - #phi^{FST,readout}_{gen} (rad)");
  h_mFstProjResPhi_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mFstProjResPhi_2Layer->GetXaxis()->CenterTitle();
  h_mFstProjResPhi_2Layer->GetXaxis()->SetRangeUser(-0.1,0.1);
  if(!isRot) h_mFstProjResPhi_2Layer->GetXaxis()->SetRangeUser(-0.025,0.03);
  h_mFstProjResPhi_2Layer->SetMarkerStyle(24);
  h_mFstProjResPhi_2Layer->SetMarkerSize(1.2);
  h_mFstProjResPhi_2Layer->SetMarkerColor(1);
  h_mFstProjResPhi_2Layer->Scale(IntePhi_Data[0]/IntePhi_MC);
  h_mFstProjResPhi_2Layer->Draw("pE");
  for(int i_rstrp = 0; i_rstrp < 4; ++i_rstrp)
  {
    h_mClustersTrackFstResPhi_2Layer[i_rstrp]->SetLineColor(i_rstrp+1);
    h_mClustersTrackFstResPhi_2Layer[i_rstrp]->Scale(IntePhi_Data[0]/IntePhi_Data[i_rstrp]);
    if(!isRot) h_mClustersTrackFstResPhi_2Layer[i_rstrp]->Draw("hE same");
  }
  if(!isRot) leg->Draw("same");

  string FigName = Form("./figures/c_Residual_%s_%s.eps",mRot.c_str(),mode.c_str());
  c_Residual->SaveAs(FigName.c_str());
}
