
void eff_plot() {
  gStyle->SetPalette(53);
  
  ROOT::EnableImplicitMT(8); 
  ROOT::RDataFrame df_raw("events", "analysis/single_neut_eff.root");
  // ROOT::RDataFrame df_raw("events", "analysis/B10elastic.root");

  //auto df = df_raw
    //.Filter("ThetaCM < 40 && ThetaCM > 20")
    //.Filter("QValue < -1 && QValue > -2")
    //.Filter("si_atomic_num == 1")
    //.Filter("si_atomic_mass == 2")
    //.Filter("s1_e + s2_e > 10 && s1_e+s2_e < 25 && s1_theta < 22 && s2_theta < 22")
    //.Filter("ic_ex+ic_ey > 10")
    //.Filter("ic_de + ic_e > 0")
    //.Filter("s1_phi < -65 || s1_phi > -43"); 
    //.Filter("s1_e+s2_e > 7 && s1_e+s2_e < 10");
    //.Filter("FragmentEx == 0")
    //.Filter("ic_atomic_num == 4")
    //.Filter("ic_atomic_mass == 7")
    //.Filter("scint_e > 0.0");
  
  auto df = df_raw;

  std::cout<< *df.Count() / (double)*df_raw.Count() * 100.0 << "%" << std::endl;
  
  auto Qvalue = df.Histo1D(
    {"Qvalue","Qvalue", 2000, -10, 10}, "QValue");
  
  auto theta_cm_raw = df_raw.Histo1D(
    {"theta_cm_raw","theta_cm_raw", 56, 0, 180}, "ThetaCM");
  
  auto theta_cm = df.Histo1D(
    {"theta_cm","theta_cm", 56, 0, 180}, "ThetaCM");
  
  auto kin_ejectile = df.Histo2D(
    {"kin_ejectile","kin_ejectile;theta;eject ke",2000, 0, 200, 200, 0, 50}, "ThetaEjectile", "EjectileKE");

  auto kin_fragment = df.Histo2D(
    {"kin_fragment","kin_fragment;theta;frag ke",200, 0, 20, 500, 0, 50}, "ThetaFragment", "FragmentKE");

  auto kin_decay_light = df.Histo2D(
    {"kin_decay_light","kin_decay_light;theta;decay ke light",4500, 0, 45, 500, 0, 50}, "ThetaDecayLight", "DecayLightKE");

  auto kin_decay_heavy = df.Histo2D(
    {"kin_decay_heavy","kin_decay_heavy;theta;decay ke heavy",2000, 0, 20, 500, 0, 50}, "ThetaDecayHeavy", "DecayHeavyKE");

  auto si_ede = df.Define("si_ede","s1_e+s2_e")
    .Histo2D({"si_ede","si_ede;s1 + s2 e;s2 e",600, 0, 30, 500, 0, 15},"si_ede", "s2_e");

  auto ic_ede = df
    //.Define("ic_ede","ic_de")
    .Define("ic_ede","ic_e+ic_de")
    .Define("ic_exy","ic_ex+ic_ey")
    .Histo2D({"ic_ede","ic_ede;ic_e+ic_de;ic_ex+ic_ey",500, 0, 50, 200, 0, 20},"ic_ede", "ic_exy");
  //auto ic_ede = df.Histo2D({"ic_ede","ic_ede;ic_e;ic_de",500, 0, 50, 200, 0, 20},"ic_e", "ic_de");
  
  auto si_theta_corr = df.Histo2D(
    {"si_theta_corr","si_theta_corr;s1_theta;s2_theta",350, 0, 35, 350, 0, 35},"s1_theta", "s2_theta");

  auto si_kin = df.Define("si_ede","s1_e+s2_e")
    .Histo2D({"si_kin","si_kin;s1_theta;s1+s2 e",350, 0, 35, 350, 0, 35},"s1_theta", "si_ede");
  
  auto ic_kin = df.Define("ic_ede","ic_e+ic_de")
    .Histo2D({"ic_kin","ic_kin;ic_theta;ic e+de",150, 0, 15, 500, 0, 50},"ic_theta", "ic_ede");
  
  auto ic_phi_si_phi = df.Histo2D(
    {"ic_phi_si_phi","ic_phi_si_phi;s1_phi;ic_phi",200, -200, 200, 200, -200, 200},"s1_phi", "ic_phi");
  
  auto scint_e = df.Histo1D(
    {"scint_e","scint_e", 1000, 0, 2}, "scint_e");

  auto scint_t = df.Histo1D(
    {"scint_t","scint_t", 1000, -10, 110}, "scint_t");
  
  auto scint_theta = df.Histo1D({"scint_theta","scint_theta", 360, 0, 180}, "scint_theta");

  auto scint_x_y = df.Histo2D(
    {"scint_x_y","scint_x_y",250, -500, 500, 250, -500, 500},"scint_x", "scint_y");

  auto ic_x_y = df.Histo2D(
    {"ic_x_y","ic_x_y;ic_x;ic_y",100, -30, 30, 100, -30, 30},"ic_x", "ic_y");

  auto decay_heavy_light = df.Histo2D(
    {"decay_heavy_light","decay_heavy_light;heavy;light",1000, 0, 100, 1000, 0, 100}, "DecayHeavyKE", "DecayLightKE");
  
  auto si_e_ic_e = df 
    .Define("ic_ede","ic_e+ic_de+ic_ey+ic_ex")
    .Define("si_ede","s1_e+s2_e")
    .Histo2D({"si_e_ic_e","si_e_ic_e;si_e;ic_etot",1000, 0, 100, 1000, 0, 100},"si_ede", "ic_ede");

  ROOT::RDF::RunGraphs(
  {
    Qvalue,
    theta_cm,
    theta_cm_raw,
    kin_ejectile,
    kin_fragment,
    kin_decay_light,
    kin_decay_heavy,
    si_ede,
    ic_ede,
    ic_kin,
    si_theta_corr,
    si_kin,
    scint_e,
    scint_t,
    scint_theta,
    scint_x_y,
    ic_x_y,
    ic_phi_si_phi,
    si_e_ic_e,
    decay_heavy_light
  });

  new TCanvas("c1","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  //auto theta_cm_raw_ptr = theta_cm_raw->GetPtr()
  theta_cm->Divide(theta_cm_raw.GetPtr());
  theta_cm->DrawCopy("");
  
  new TCanvas("c2","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  theta_cm_raw->DrawCopy("");
  
  //new TCanvas("c2","",1000,800); 
  //gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  //kin_ejectile->DrawCopy("col");
  
  //new TCanvas("c3","",1000,800);
  //gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15); 
  //kin_fragment->DrawCopy("col");
  
  // new TCanvas(); kin_decay_light->DrawCopy("col");
  // new TCanvas(); kin_decay_heavy->DrawCopy("col");
  
  new TCanvas("c4","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  si_ede->DrawCopy("col");

  new TCanvas("c5","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  ic_ede->DrawCopy("col");

  new TCanvas("c6","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  si_theta_corr->DrawCopy("col");
  
  new TCanvas("c7","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  scint_e->DrawCopy();

  //new TCanvas("c8","",1000,800); 
  //gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  //scint_t->DrawCopy();

  new TCanvas("c9","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  scint_x_y->DrawCopy("col");
  
  //new TCanvas(); ic_x_y->DrawCopy("lego2");

  //new TCanvas("c10","",1000,800); 
  //gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  //ic_phi_si_phi->DrawCopy("col");

  new TCanvas("c11","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  si_kin->DrawCopy("col");

  new TCanvas("c12","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  ic_kin->DrawCopy("col");

  new TCanvas("c13","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  Qvalue->DrawCopy("");


  new TCanvas("c14","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  si_e_ic_e->DrawCopy("col");


  new TCanvas("c15","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  decay_heavy_light->DrawCopy("col");

  new TCanvas("c16","",1000,800); 
  gPad->SetLeftMargin(0.17); gPad->SetBottomMargin(0.15);
  scint_theta->DrawCopy("");
} 
