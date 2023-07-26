// Eli Temanson, April 2023
// void b();

{

  gStyle->SetOptTitle(0); // remove title
  // gStyle->SetOptStat("nemri"); // set stat box (name, entries, mean & SD and integral)
  gStyle->SetOptStat("i"); // set stat box (i=integral in window)
  gStyle->SetOptFit(1); // add fit results to stat box
  gStyle->SetCanvasDefH(800); // change canvas to a square
  gStyle->SetCanvasDefW(800);

  // This isn't really used right now
  Int_t ci;      // for color index setting 
  TColor *color; // for color definition with alpha
  ci = TColor::GetColor("#782f40");
  gStyle->SetFillColor(ci);

  gStyle->SetNdivisions(507, "X");
  gStyle->SetNdivisions(507, "Y");

  gStyle->SetPadGridX(kTRUE); // Add grid lines
  gStyle->SetPadGridY(kTRUE);
  gStyle->SetFrameLineWidth(2); // Thicker Frame
  gStyle->SetHistLineWidth(2); // Thicker Hist Lines
  gStyle->SetStatBorderSize(4); // Stat Boarder
  gStyle->SetLegendBorderSize(4); // If you make have a legend

  gStyle->SetStatFontSize(0.07); // Larger Text for the Legend
  gStyle->SetLabelSize(0.07,"XY"); // Larger Text for the label
  gStyle->SetTitleSize(0.07,"XY"); // Larger Text for the Axis titles
  gStyle->SetStatFontSize(0.05); // Larger Text for the Stat box
  // gStyle->SetStatColor(kGray); 
  gStyle->SetLabelFont(132,"XY"); // Change fonts of axis label to a times new roman style
  gStyle->SetTitleFont(132,"XYZ"); // Change fonts of axis title
  gStyle->SetTitleFont(132,"T"); // Change font of canvas title
  gStyle->SetLegendFont(132); // Change font of legend
  gStyle->SetStatFont(132); // Change font of stat box
  gStyle->SetMarkerStyle(33); // Change default marker style, better for low stat. data

  gStyle->SetTitleOffset(0.95,"XY"); // increase axis title offsets.

  // c1->SetRightMargin(0.09);
  // gStyle->SetLeftMargin(0.17);
  // gStyle->SetBottomMargin(0.15);

  gROOT->ForceStyle(); // Make it law.


  // new TBrowser("b","b",1000,800);
  // gPad->SetCrosshair();
  // gPad->SetLeftMargin(0.17);
  // gPad->SetBottomMargin(0.15);


}