
// void plotter() 
{
  gStyle->SetPalette(53);

  ROOT::EnableImplicitMT(15); 
  ROOT::RDataFrame df_raw("events", "analysis/eff.root");
  // ROOT::RDataFrame df_raw("events", "analysis/C12dn.root");
  // ROOT::RDataFrame df_raw("events", "analysis/B10dn.root");

  auto df = df_raw
    .Filter("s1_e > 0 && s2_e > 0")
    .Filter("ic_atomic_num == 4")
    .Filter("scint_e > 0.001");
  // auto df = df_raw;

  auto theta_cm = df.Histo1D(
    {"theta_cm","theta_cm", 360, 0, 180}, "ThetaCM");
  
  auto kin_ejectile = df.Histo2D(
    {"kin_ejectile","kin_ejectile",2000, 0, 200, 200, 0, 20}, "ThetaEjectile", "EjectileKE");

  auto kin_fragment = df.Histo2D(
    {"kin_fragment","kin_fragment",200, 0, 20, 500, 0, 50}, "ThetaFragment", "FragmentKE");

  auto kin_decay_light = df.Histo2D(
    {"kin_decay_light","kin_decay_light",4500, 0, 45, 500, 0, 50}, "ThetaDecayLight", "DecayLightKE");

  auto kin_decay_heavy = df.Histo2D(
    {"kin_decay_heavy","kin_decay_heavy",2000, 0, 20, 500, 0, 50}, "ThetaDecayHeavy", "DecayHeavyKE");

  auto si_ede = df.Histo2D(
    {"si_ede","si_ede",200, 0, 20, 200, 0, 20},"s1_e", "s2_e");

  //auto ic_ede = df.Define("ic_ede","ic_e+ic_de").Histo2D(
  //  {"ic_ede","ic_ede",500, 0, 50, 200, 0, 20},"ic_ede", "ic_de");
  auto ic_ede = df.Histo2D(
    {"ic_ede","ic_ede",500, 0, 50, 200, 0, 20},"ic_e", "ic_de");
  
  auto si_theta_corr = df.Histo2D(
    {"si_theta_corr","si_theta_corr",350, 0, 35, 350, 0, 35},"s1_theta", "s2_theta");

  auto scint_e = df.Histo1D(
    {"scint_e","scint_e", 1000, 0, 2}, "scint_e");

  auto scint_t = df.Histo1D(
    {"scint_t","scint_t", 1000, -10, 110}, "scint_t");

  auto scint_x_y = df.Histo2D(
    {"scint_x_y","scint_x_y",250, -500, 500, 250, -500, 500},"scint_x", "scint_y");


  ROOT::RDF::RunGraphs(
  {
    theta_cm,
    // kin_ejectile,
    // kin_fragment,
    kin_decay_light,
    kin_decay_heavy,
    si_ede,
    ic_ede,
    si_theta_corr,
    scint_e,
    scint_t,
    scint_x_y
  });

  new TCanvas(); theta_cm->DrawCopy("");
  // new TCanvas(); kin_ejectile->DrawCopy("col");
  // new TCanvas(); kin_fragment->DrawCopy("col");
  new TCanvas(); kin_decay_light->DrawCopy("col");
  new TCanvas(); kin_decay_heavy->DrawCopy("col");
  new TCanvas(); si_ede->DrawCopy("col");
  new TCanvas(); ic_ede->DrawCopy("col");
  new TCanvas(); si_theta_corr->DrawCopy("col");
  new TCanvas(); scint_e->DrawCopy();
  new TCanvas(); scint_t->DrawCopy();
  new TCanvas(); scint_x_y->DrawCopy("col");

}
