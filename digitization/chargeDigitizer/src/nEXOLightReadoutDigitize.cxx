#include "nEXOLightReadoutDigitize.hh"
#include <iostream>
using namespace std;
nEXOLightReadoutDigitize::nEXOLightReadoutDigitize()
{
}

nEXOLightReadoutDigitize::~nEXOLightReadoutDigitize()
{
}

void nEXOLightReadoutDigitize::GeneratePCDs()
{
}

void nEXOLightReadoutDigitize::Digitize()
{
}

void nEXOLightReadoutDigitize::SetNoiseModel()
{
}
bool nEXOLightReadoutDigitize::LoadLightMap(TString lightMapName)
{
fFile = new TFile(lightMapName);
fHist = (TH2D*) fFile->Get("hist");  
if (fHist == 0) return false; 
cout << " Entries  " << fHist->GetEntries() << endl;
return true;
}

double nEXOLightReadoutDigitize::GetEfficiency(double x, double y, double z)
{
double rho = sqrt(x*x+y*y);
double z0 = z;
double iBin = fHist->FindBin(rho, z0);

return fHist->GetBinContent(iBin); 
}
