void plot() {
    TFile *t1=new TFile("st_physics_22351023_raw_1500003.fstQa.root.eff.root");
    TFile *t2=new TFile("st_physics_22351023_raw_1500004.fstQa.root.eff.root");
    TFile *t3=new TFile("st_physics_22351023_raw_1500005.fstQa.root.eff.root");
    TFile *t4=new TFile("st_physics_22351023_raw_1500006.fstQa.root.eff.root");
    TFile *t5=new TFile("st_physics_22351023_raw_1500007.fstQa.root.eff.root");
    /*TFile *t1=new TFile("st_physics_22351015_raw_1500001.fstQa.root.eff.root");
    TFile *t2=new TFile("st_physics_22351021_raw_1500001.fstQa.root.eff.root");
    TFile *t3=new TFile("st_physics_22351023_raw_1500001.fstQa.root.eff.root");
    TFile *t4=new TFile("st_physics_22351026_raw_1500001.fstQa.root.eff.root");
    TFile *t5=new TFile("st_physics_22351028_raw_1500001.fstQa.root.eff.root");*/

    TH1D* h1=(TH1D*)t1->Get("hEff2_phi");
    h1->SetLineColor(1);
    h1->SetMarkerColor(1);
    h1->SetMarkerStyle(20);
    TH1D* h2=(TH1D*)t2->Get("hEff2_phi");
    h2->SetLineColor(2);
    h2->SetMarkerColor(2);
    h2->SetMarkerStyle(20);
    TH1D* h3=(TH1D*)t3->Get("hEff2_phi");
    h3->SetLineColor(3);
    h3->SetMarkerColor(3);
    h3->SetMarkerStyle(20);
    TH1D* h4=(TH1D*)t4->Get("hEff2_phi");
    h4->SetLineColor(4);
    h4->SetMarkerColor(4);
    h4->SetMarkerStyle(20);
    TH1D* h5=(TH1D*)t5->Get("hEff2_phi");
    h5->SetLineColor(5);
    h5->SetMarkerColor(5);
    h5->SetMarkerStyle(20);

    TH1D* eff1=new TH1D("eff1","",20,0.4,1.0);
    TH1D* eff2=new TH1D("eff2","",20,0.4,1.0);
    TH1D* eff3=new TH1D("eff3","",20,0.4,1.0);
    TH1D* eff4=new TH1D("eff4","",20,0.4,1.0);
    TH1D* eff5=new TH1D("eff5","",20,0.4,1.0);

    for(Int_t i=1;i<=h1->GetNbinsX();i++) {
        eff1->Fill(h1->GetBinContent(i));
        eff2->Fill(h2->GetBinContent(i));
        eff3->Fill(h3->GetBinContent(i));
        eff4->Fill(h4->GetBinContent(i));
        eff5->Fill(h5->GetBinContent(i));
    }

    eff1->SetLineColor(1);
    eff2->SetLineColor(2);
    eff3->SetLineColor(3);
    eff4->SetLineColor(4);
    eff5->SetLineColor(5);

    eff1->Draw();
    eff2->Draw("SAME");
    eff3->Draw("SAME");
    eff4->Draw("SAME");
    eff5->Draw("SAME");

    cout<<eff1->GetMean()<<" "<<eff2->GetMean()<<" "<<eff3->GetMean()<<" "<<eff4->GetMean()<<" "<<eff5->GetMean()<<" "<<endl;
}

