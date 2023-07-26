

void counter(){
  ROOT::EnableImplicitMT(15); 
  ROOT::RDataFrame df_raw("events", "analysis/eff.root");

  auto df = df_raw.Filter("s1_e > 0 && s2_e > 0")
                  .Filter("ic_atomic_num == 4")
                  .Filter("ic_de > 0")
                  .Filter("scint_e > 0.001");

  std::cout<< *df.Count() / (double)*df_raw.Count();
}
