#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>
#include "../../src/FstUtil/FstCons.h"

using namespace std;

void plotMcProjection(int sensorId = 0, string mod = "Mod03", string hv = "HV140V")
{
  string mode = "Scan";
  // string mode = "Simple";

  //-----------------------------------
  // simulation 
  string inputSim = Form("../../output/simulation/%s/FstMcProjection_Sensor%d.root",mod.c_str(),sensorId);
  TFile *File_InPutSim = TFile::Open(inputSim.c_str());
  TH1F *h_mFstProjResR_2Layer    = (TH1F*)File_InPutSim->Get("h_mFstProjResR_2Layer");
  TH1F *h_mFstProjResPhi_2Layer  = (TH1F*)File_InPutSim->Get("h_mFstProjResPhi_2Layer");

  TH1F *h_mFstProjResR_2Layer_Rstrips[FST::mFstNumRstripPerSensor];
  TH1F *h_mFstProjResPhi_2Layer_Rstrips[FST::mFstNumRstripPerSensor];
  TH2F *h_mFstProjResRPhi_2Layer_Rstrips[FST::mFstNumRstripPerSensor]; // 2D distribution
  for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
  {
    string HistName;
    HistName = Form("h_mFstProjResR_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip);
    if(sensorId > 0) HistName = Form("h_mFstProjResR_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip+4);
    h_mFstProjResR_2Layer_Rstrips[i_rstrip]    = (TH1F*)File_InPutSim->Get(HistName.c_str());
    HistName = Form("h_mFstProjResPhi_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip);
    if(sensorId > 0) HistName = Form("h_mFstProjResPhi_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip+4);
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrip]  = (TH1F*)File_InPutSim->Get(HistName.c_str());
    HistName = Form("h_mFstProjResRPhi_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip);
    if(sensorId > 0) HistName = Form("h_mFstProjResRPhi_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip+4);
    h_mFstProjResRPhi_2Layer_Rstrips[i_rstrip] = (TH2F*)File_InPutSim->Get(HistName.c_str());
  }
  TH1F *h_mFstSimResR_2Layer     = (TH1F*)File_InPutSim->Get("h_mFstSimResR_2Layer");
  TH1F *h_mFstSimResPhi_2Layer   = (TH1F*)File_InPutSim->Get("h_mFstSimResPhi_2Layer");
  TH2F *h_mFstSimResRPhi_2Layer  = (TH2F*)File_InPutSim->Get("h_mFstSimResRPhi_2Layer");
  //-----------------------------------

  //-----------------------------------
  // simulation with x-talk
  string inputSimXtalk = Form("../../output/simulation/%s/xtalk/FstMcProjection_Sensor%d.root",mod.c_str(),sensorId);
  TFile *File_InPutSimXtalk = TFile::Open(inputSimXtalk.c_str());
  TH1F *h_mFstProjXTalkResR_2Layer    = (TH1F*)File_InPutSimXtalk->Get("h_mFstProjResR_2Layer");
  TH1F *h_mFstProjXTalkResPhi_2Layer  = (TH1F*)File_InPutSimXtalk->Get("h_mFstProjResPhi_2Layer");

  TH1F *h_mFstProjXTalkResR_2Layer_Rstrips[FST::mFstNumRstripPerSensor];
  TH1F *h_mFstProjXTalkResPhi_2Layer_Rstrips[FST::mFstNumRstripPerSensor];
  TH2F *h_mFstProjXTalkResRPhi_2Layer_Rstrips[FST::mFstNumRstripPerSensor]; // 2D distribution
  for(int i_rstrip = 0; i_rstrip < FST::mFstNumRstripPerSensor; ++i_rstrip)
  {
    string HistName;
    HistName = Form("h_mFstProjResR_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip);
    if(sensorId > 0) HistName = Form("h_mFstProjResR_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip+4);
    h_mFstProjXTalkResR_2Layer_Rstrips[i_rstrip]    = (TH1F*)File_InPutSimXtalk->Get(HistName.c_str());
    HistName = Form("h_mFstProjResPhi_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip);
    if(sensorId > 0) HistName = Form("h_mFstProjResPhi_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip+4);
    h_mFstProjXTalkResPhi_2Layer_Rstrips[i_rstrip]  = (TH1F*)File_InPutSimXtalk->Get(HistName.c_str());
    HistName = Form("h_mFstProjResRPhi_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip);
    if(sensorId > 0) HistName = Form("h_mFstProjResRPhi_2Layer_Sensor%d_Rstrip%d",sensorId,i_rstrip+4);
    h_mFstProjXTalkResRPhi_2Layer_Rstrips[i_rstrip] = (TH2F*)File_InPutSimXtalk->Get(HistName.c_str());
  }
  TH1F *h_mFstSimXTalkResR_2Layer     = (TH1F*)File_InPutSimXtalk->Get("h_mFstSimResR_2Layer");
  TH1F *h_mFstSimXTalkResPhi_2Layer   = (TH1F*)File_InPutSimXtalk->Get("h_mFstSimResPhi_2Layer");
  TH2F *h_mFstSimXTalkResRPhi_2Layer  = (TH2F*)File_InPutSimXtalk->Get("h_mFstSimResRPhi_2Layer");
  //-----------------------------------

  //-----------------------------------
  // Data
  string inputData = Form("../../output/signal/%s/FstTracking_%s_%s_Th4.0Tb2Ped2.5Ped3.5_withPed_withCMNCorr.root",mod.c_str(),mod.c_str(),hv.c_str());
  TFile *File_InPutData = TFile::Open(inputData.c_str());
  TH1F *h_mClustersTrackFstResR_2Layer;
  TH1F *h_mClustersTrackFstResPhi_2Layer;
  {
    string HistName;
    HistName = Form("h_m%sClustersTrackFstResR_2Layer_Sensor%d",mode.c_str(),sensorId);
    h_mClustersTrackFstResR_2Layer = (TH1F*)File_InPutData->Get(HistName.c_str());
    HistName = Form("h_m%sClustersTrackFstResPhi_2Layer_Sensor%d",mode.c_str(),sensorId);
    h_mClustersTrackFstResPhi_2Layer = (TH1F*)File_InPutData->Get(HistName.c_str());
  }
  TH1F *h_mClustersTrackFstResR_2Layer_Rstrips[FST::mFstNumRstripPerSensor];
  TH1F *h_mClustersTrackFstResPhi_2Layer_Rstrips[FST::mFstNumRstripPerSensor];
  for(int i_rstrp = 0; i_rstrp < FST::mFstNumRstripPerSensor; ++i_rstrp)
  {
    string HistName;
    HistName = Form("h_m%sClustersTrackFstResR_2Layer_Sensor%d_Rstrip%d",mode.c_str(),sensorId,i_rstrp); // R Residual
    if(sensorId > 0) HistName = Form("h_m%sClustersTrackFstResR_2Layer_Sensor%d_Rstrip%d",mode.c_str(),sensorId,i_rstrp+4);
    h_mClustersTrackFstResR_2Layer_Rstrips[i_rstrp] = (TH1F*)File_InPutData->Get(HistName.c_str());
    HistName = Form("h_m%sClustersTrackFstResPhi_2Layer_Sensor%d_Rstrip%d",mode.c_str(),sensorId,i_rstrp); // R Residual
    if(sensorId > 0) HistName = Form("h_m%sClustersTrackFstResPhi_2Layer_Sensor%d_Rstrip%d",mode.c_str(),sensorId,i_rstrp+4);
    h_mClustersTrackFstResPhi_2Layer_Rstrips[i_rstrp] = (TH1F*)File_InPutData->Get(HistName.c_str());
  }
  //-----------------------------------

  // calculate scaling factor
  double NormR_InteData = h_mClustersTrackFstResR_2Layer->GetMaximum();
  double NormR_InteMC = h_mFstProjResR_2Layer->GetMaximum();
  double NormR_InteMCXTalk = h_mFstProjXTalkResR_2Layer->GetMaximum();
  double NormR_InteIST = h_mFstSimResR_2Layer->GetMaximum();
  double NormR_Data[4];
  double NormR_MC[4];
  double NormR_MCXTalk[4];
  for(int i_rstrp = 0; i_rstrp < 4; ++i_rstrp)
  {
    NormR_Data[i_rstrp] = h_mClustersTrackFstResR_2Layer_Rstrips[i_rstrp]->GetMaximum();
    NormR_MC[i_rstrp] = h_mFstProjResR_2Layer_Rstrips[i_rstrp]->GetMaximum();
    NormR_MCXTalk[i_rstrp] = h_mFstProjXTalkResR_2Layer_Rstrips[i_rstrp]->GetMaximum();
  }
  double NormPhi_InteData = h_mClustersTrackFstResPhi_2Layer->GetMaximum();
  double NormPhi_InteMC = h_mFstProjResPhi_2Layer->GetMaximum();
  double NormPhi_InteMCXTalk = h_mFstProjXTalkResPhi_2Layer->GetMaximum();
  double NormPhi_InteIST = h_mFstSimResPhi_2Layer->GetMaximum();
  double NormPhi_Data[4];
  double NormPhi_MC[4];
  double NormPhi_MCXTalk[4];
  for(int i_rstrp = 0; i_rstrp < 4; ++i_rstrp)
  {
    NormPhi_Data[i_rstrp] = h_mClustersTrackFstResPhi_2Layer_Rstrips[i_rstrp]->GetMaximum();
    NormPhi_MC[i_rstrp] = h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->GetMaximum();
    NormPhi_MCXTalk[i_rstrp] = h_mFstProjXTalkResPhi_2Layer_Rstrips[i_rstrp]->GetMaximum();
  }

  TCanvas *c_Residual = new TCanvas("c_Residual","c_Residual",10,10,800,400);
  c_Residual->Divide(2,1);
  for(int i_pad = 0; i_pad < 2; ++ i_pad)
  {
    c_Residual->cd(i_pad+1);
    c_Residual->cd(i_pad+1)->SetLeftMargin(0.15);
    c_Residual->cd(i_pad+1)->SetBottomMargin(0.15);
    c_Residual->cd(i_pad+1)->SetTicks(1,1);
    c_Residual->cd(i_pad+1)->SetGrid(0,0);
  }

  //FST R-Phi
  c_Residual->cd(1);
  h_mFstProjResR_2Layer->SetTitle("FST");
  h_mFstProjResR_2Layer->GetXaxis()->SetTitle("r_{ro} - r_{proj} (mm)");
  h_mFstProjResR_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mFstProjResR_2Layer->GetXaxis()->CenterTitle();
  h_mFstProjResR_2Layer->GetXaxis()->SetRangeUser(-100.0,100.0);
  h_mFstProjResR_2Layer->SetLineStyle(2);
  h_mFstProjResR_2Layer->SetLineWidth(2);
  h_mFstProjResR_2Layer->SetLineColor(4);
  h_mFstProjResR_2Layer->Scale(NormR_InteData/NormR_InteMC);
  h_mFstProjResR_2Layer->Draw("h");
  h_mFstProjXTalkResR_2Layer->SetLineStyle(1);
  h_mFstProjXTalkResR_2Layer->SetLineWidth(2);
  h_mFstProjXTalkResR_2Layer->SetLineColor(1);
  h_mFstProjXTalkResR_2Layer->Scale(NormR_InteData/NormR_InteMCXTalk);
  h_mFstProjXTalkResR_2Layer->Draw("h same");
  h_mFstSimResR_2Layer->SetLineStyle(2);
  h_mFstSimResR_2Layer->SetLineWidth(2);
  h_mFstSimResR_2Layer->SetLineColor(2);
  h_mFstSimResR_2Layer->Scale(NormR_InteData/NormR_InteIST);
  h_mFstSimResR_2Layer->Draw("h same");
  h_mClustersTrackFstResR_2Layer->SetMarkerStyle(24);
  h_mClustersTrackFstResR_2Layer->SetMarkerSize(0.8);
  h_mClustersTrackFstResR_2Layer->SetMarkerColor(1);
  h_mClustersTrackFstResR_2Layer->SetLineColor(1);
  h_mClustersTrackFstResR_2Layer->Draw("pE same");
  TLegend *leg = new TLegend(0.60,0.5,0.85,0.7);
  leg->AddEntry(h_mClustersTrackFstResR_2Layer,"Data","p");
  leg->AddEntry(h_mFstProjXTalkResR_2Layer,"MC with X-Talk","l");
  leg->AddEntry(h_mFstProjResR_2Layer,"MC w/o X-Talk","l");
  leg->AddEntry(h_mFstSimResR_2Layer,"IST Proj Err.","l");
  leg->Draw("same");

  c_Residual->cd(2);
  h_mFstProjResPhi_2Layer->SetTitle("FST");
  h_mFstProjResPhi_2Layer->GetXaxis()->SetTitle("#phi_{ro} - #phi_{proj} (rad)");
  h_mFstProjResPhi_2Layer->GetXaxis()->SetTitleSize(0.06);
  h_mFstProjResPhi_2Layer->GetXaxis()->CenterTitle();
  h_mFstProjResPhi_2Layer->GetXaxis()->SetRangeUser(-0.05,0.05);
  h_mFstProjResPhi_2Layer->SetLineStyle(1);
  h_mFstProjResPhi_2Layer->SetLineWidth(2);
  h_mFstProjResPhi_2Layer->SetLineColor(4);
  h_mFstProjResPhi_2Layer->Scale(NormPhi_InteData/NormPhi_InteMC);
  h_mFstProjResPhi_2Layer->Draw("h");
  h_mFstProjXTalkResPhi_2Layer->SetLineStyle(1);
  h_mFstProjXTalkResPhi_2Layer->SetLineWidth(2);
  h_mFstProjXTalkResPhi_2Layer->SetLineColor(1);
  h_mFstProjXTalkResPhi_2Layer->Scale(NormPhi_InteData/NormPhi_InteMCXTalk);
  h_mFstProjXTalkResPhi_2Layer->Draw("h same");
  h_mFstSimResPhi_2Layer->SetLineStyle(2);
  h_mFstSimResPhi_2Layer->SetLineWidth(2);
  h_mFstSimResPhi_2Layer->SetLineColor(2);
  h_mFstSimResPhi_2Layer->Scale(NormPhi_InteData/NormPhi_InteIST);
  h_mFstSimResPhi_2Layer->Draw("h same");
  h_mClustersTrackFstResPhi_2Layer->SetMarkerStyle(24);
  h_mClustersTrackFstResPhi_2Layer->SetMarkerSize(0.8);
  h_mClustersTrackFstResPhi_2Layer->SetMarkerColor(1);
  h_mClustersTrackFstResPhi_2Layer->SetLineColor(1);
  h_mClustersTrackFstResPhi_2Layer->Draw("pE same");
  leg->Draw("same");

  string FigName = Form("./figures/%s/c_Residual_%s_%s_Sensor%d.eps",mod.c_str(),mode.c_str(),hv.c_str(),sensorId);
  c_Residual->SaveAs(FigName.c_str());

  TCanvas *c_ResDiff = new TCanvas("c_ResDiff","c_ResDiff",10,10,900,1200);
  c_ResDiff->Divide(3,4);
  for(int i_pad = 0; i_pad < 12; ++ i_pad)
  {
    c_ResDiff->cd(i_pad+1);
    c_ResDiff->cd(i_pad+1)->SetLeftMargin(0.15);
    c_ResDiff->cd(i_pad+1)->SetBottomMargin(0.15);
    c_ResDiff->cd(i_pad+1)->SetTicks(1,1);
    c_ResDiff->cd(i_pad+1)->SetGrid(0,0);
  }

  for(int i_rstrp = 0; i_rstrp < 4; ++i_rstrp)
  {
    c_ResDiff->cd(i_rstrp*3+1);
    h_mFstProjResRPhi_2Layer_Rstrips[i_rstrp]->GetXaxis()->SetTitle("r_{ro} - r_{proj} (mm)");
    h_mFstProjResRPhi_2Layer_Rstrips[i_rstrp]->GetXaxis()->SetTitleSize(0.06);
    h_mFstProjResRPhi_2Layer_Rstrips[i_rstrp]->GetXaxis()->CenterTitle();
    h_mFstProjResRPhi_2Layer_Rstrips[i_rstrp]->GetYaxis()->SetTitle("#phi_{ro} - #phi_{proj} (rad)");
    h_mFstProjResRPhi_2Layer_Rstrips[i_rstrp]->GetYaxis()->SetTitleSize(0.06);
    h_mFstProjResRPhi_2Layer_Rstrips[i_rstrp]->GetYaxis()->CenterTitle();
    h_mFstProjResRPhi_2Layer_Rstrips[i_rstrp]->Draw("colz");

    c_ResDiff->cd(i_rstrp*3+2);
    h_mFstProjResR_2Layer_Rstrips[i_rstrp]->GetXaxis()->SetTitle("r_{ro} - r_{proj} (mm)");
    h_mFstProjResR_2Layer_Rstrips[i_rstrp]->GetXaxis()->SetTitleSize(0.06);
    h_mFstProjResR_2Layer_Rstrips[i_rstrp]->GetXaxis()->CenterTitle();
    h_mFstProjResR_2Layer_Rstrips[i_rstrp]->GetXaxis()->SetRangeUser(-100.0,100.0);
    if(NormR_Data[i_rstrp] > 0 && NormR_MC[i_rstrp] > 0) h_mFstProjResR_2Layer_Rstrips[i_rstrp]->Scale(NormR_Data[i_rstrp]/NormR_MC[i_rstrp]);
    h_mFstProjResR_2Layer_Rstrips[i_rstrp]->GetYaxis()->SetRangeUser(0.0,h_mFstProjResR_2Layer_Rstrips[i_rstrp]->GetMaximum()*1.1);
    h_mFstProjResR_2Layer_Rstrips[i_rstrp]->SetLineStyle(2);
    h_mFstProjResR_2Layer_Rstrips[i_rstrp]->SetLineWidth(2);
    h_mFstProjResR_2Layer_Rstrips[i_rstrp]->SetLineColor(i_rstrp+1);
    h_mFstProjResR_2Layer_Rstrips[i_rstrp]->Draw("h");
    if(NormR_Data[i_rstrp] > 0 && NormR_MCXTalk[i_rstrp] > 0) h_mFstProjXTalkResR_2Layer_Rstrips[i_rstrp]->Scale(NormR_Data[i_rstrp]/NormR_MCXTalk[i_rstrp]);
    h_mFstProjXTalkResR_2Layer_Rstrips[i_rstrp]->SetLineStyle(1);
    h_mFstProjXTalkResR_2Layer_Rstrips[i_rstrp]->SetLineWidth(2);
    h_mFstProjXTalkResR_2Layer_Rstrips[i_rstrp]->SetLineColor(i_rstrp+1);
    h_mFstProjXTalkResR_2Layer_Rstrips[i_rstrp]->Draw("h same");
    h_mClustersTrackFstResR_2Layer_Rstrips[i_rstrp]->SetMarkerStyle(24);
    h_mClustersTrackFstResR_2Layer_Rstrips[i_rstrp]->SetMarkerSize(0.8);
    h_mClustersTrackFstResR_2Layer_Rstrips[i_rstrp]->SetMarkerColor(i_rstrp+1);
    h_mClustersTrackFstResR_2Layer_Rstrips[i_rstrp]->SetLineColor(i_rstrp+1);
    h_mClustersTrackFstResR_2Layer_Rstrips[i_rstrp]->Draw("pE same");
    TLegend *leg = new TLegend(0.60,0.4,0.85,0.7);
    string legend = Form("Rstrip %d",i_rstrp); 
    leg->AddEntry((TObject*)0,legend.c_str(),"");
    legend = Form("%s Cluster",mode.c_str());
    leg->AddEntry(h_mClustersTrackFstResR_2Layer_Rstrips[i_rstrp],legend.c_str(),"p");
    leg->AddEntry(h_mFstProjXTalkResR_2Layer_Rstrips[i_rstrp],"MC with X-Talk","l");
    leg->AddEntry(h_mFstProjResR_2Layer_Rstrips[i_rstrp],"MC w/o X-Talk","l");
    leg->Draw("same");

    c_ResDiff->cd(i_rstrp*3+3);
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->GetXaxis()->SetTitle("#phi_{ro} - #phi_{proj} (rad)");
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->GetXaxis()->SetTitleSize(0.06);
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->GetXaxis()->CenterTitle();
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->GetXaxis()->SetRangeUser(-0.05,0.05);
    if(NormPhi_Data[i_rstrp] > 0 && NormPhi_MC[i_rstrp] > 0) h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->Scale(NormPhi_Data[i_rstrp]/NormPhi_MC[i_rstrp]);
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->GetYaxis()->SetRangeUser(0.0,h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->GetMaximum()*1.1);
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->SetLineStyle(2);
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->SetLineWidth(2);
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->SetLineColor(i_rstrp+1);
    h_mFstProjResPhi_2Layer_Rstrips[i_rstrp]->Draw("h");
    if(NormPhi_Data[i_rstrp] > 0 && NormPhi_MCXTalk[i_rstrp] > 0) h_mFstProjXTalkResPhi_2Layer_Rstrips[i_rstrp]->Scale(NormPhi_Data[i_rstrp]/NormPhi_MCXTalk[i_rstrp]);
    h_mFstProjXTalkResPhi_2Layer_Rstrips[i_rstrp]->SetLineStyle(1);
    h_mFstProjXTalkResPhi_2Layer_Rstrips[i_rstrp]->SetLineWidth(2);
    h_mFstProjXTalkResPhi_2Layer_Rstrips[i_rstrp]->SetLineColor(i_rstrp+1);
    h_mFstProjXTalkResPhi_2Layer_Rstrips[i_rstrp]->Draw("h");
    h_mClustersTrackFstResPhi_2Layer_Rstrips[i_rstrp]->SetMarkerStyle(24);
    h_mClustersTrackFstResPhi_2Layer_Rstrips[i_rstrp]->SetMarkerSize(0.8);
    h_mClustersTrackFstResPhi_2Layer_Rstrips[i_rstrp]->SetMarkerColor(i_rstrp+1);
    h_mClustersTrackFstResPhi_2Layer_Rstrips[i_rstrp]->SetLineColor(i_rstrp+1);
    h_mClustersTrackFstResPhi_2Layer_Rstrips[i_rstrp]->Draw("pE same");
    leg->Draw("same");
  }

  FigName = Form("./figures/%s/c_ResDiff_%s_%s_Sensor%d.eps",mod.c_str(),mode.c_str(),hv.c_str(),sensorId);
  c_ResDiff->SaveAs(FigName.c_str());
}
