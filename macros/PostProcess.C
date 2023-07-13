
{
  std::unique_ptr<TFile> file2(TFile::Open("experiment/run5.root"));
  auto tree2 = (TTree*)file2->Get("DataTree");
  DataTree->Draw("0.0001326*qdc3[1] + 0.009128 >> TH1F(250,0.02,0.6)");

  std::unique_ptr<TFile> file(TFile::Open("Ntuple-Cs137-662.root"));
  // std::unique_ptr<TFile> file(TFile::Open("Ntuple-Am241-59.root"));
  // std::unique_ptr<TFile> file(TFile::Open("Ntuple-Ba133-356.root"));

  auto tree = (TTree*)file->Get("Hits");
  auto hist = new TH1F("hist", "hist", 500, 0.01, 0.8);
  tree->Draw("Edep >> TH1F(250,0.02,0.6)", "","SAME");

  Double_t Edep;
  tree->SetBranchAddress("Edep", &Edep);

  double a = 0.078; //0.078
  double b = 0.00; //0.017
  double c = 0.028; //0.028

  Long64_t nevents = tree->GetEntries();
  for (Long64_t i=0; i < nevents; i++) {
    tree->GetEntry(i);
    if (Edep > 0.02)
    {
      //  hist->Fill(Edep);
      //  hist->Fill( gRandom->Gaus( Edep, (Edep / 2.355) * TMath::Sqrt( (a * a) + (b * b) / Edep + (c * c) / (Edep * Edep) )));
      hist->Fill( gRandom->Gaus( Edep, 0.06 * Edep / 2.355 ));
    }
  }
  hist->Draw("HIST SAME");

  gPad->BuildLegend();

 // myFile->WriteObject(h_Am241, "h_Am241");
}
