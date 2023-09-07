

void counter(){
  ROOT::EnableImplicitMT(8); 
  //ROOT::RDataFrame df_raw("events", "analysis/eff.root");
  ROOT::RDataFrame df_raw("events", "analysis/single_neut_eff.root");

  // auto df = df_raw
  //   .Filter("s1_e > 0 || s2_e > 0")
  //   .Filter("FragmentEx == 0.0")
  //   //.Filter("ic_atomic_num == 4")
  //   .Filter("ic_de > 0");
  //   //.Filter("scint_e > 0.001");

  auto df = df_raw
    //.Filter("s1_e > 0 || s2_e > 0")
    //.Filter("FragmentEx == 0.0")
    //.Filter("ic_atomic_num == 4")
    //.Filter("ic_de > 0");
    .Filter("scint_e > 0");

  // std::cout<< *df.Count() / (double)*df_raw.Count() * 100.0 << "%" << std::endl;
  std::cout<< (double)*df.Count() / (double)*df_raw.Count() << std::endl;
}
