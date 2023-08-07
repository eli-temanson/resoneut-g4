

void counter(){
  ROOT::EnableImplicitMT(15); 
  //ROOT::RDataFrame df_raw("events", "analysis/eff.root");
  ROOT::RDataFrame df_raw("events", "analysis/elastic-2H.root");

  auto df = df_raw
    .Filter("s1_e > 0 || s2_e > 0")
    .Filter("FragmentEx == 0.0")
    //.Filter("ic_atomic_num == 4")
    .Filter("ic_de > 0");
    //.Filter("scint_e > 0.001");

  std::cout<< *df.Count() / (double)*df_raw.Count() * 100.0 << "%" << std::endl;
}
