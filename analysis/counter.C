

// ULong64_t counter(){
void counter(){
  gStyle->SetPalette(53);

  ROOT::EnableImplicitMT(15); 
  ROOT::RDataFrame df_raw("events", "analysis/eff.root");

  auto df = df_raw.Filter("s1_e > 0 && s2_e > 0")
                  .Filter("ic_atomic_num == 4");
  // auto df = df_raw;
  std::cout<< *df.Count() / (double)*df_raw.Count();
  // return *df.Count();
}
