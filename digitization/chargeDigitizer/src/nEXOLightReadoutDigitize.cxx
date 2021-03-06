#include "nEXOLightReadoutDigitize.hh"
#include <iostream>
#include "nEXODigiAnalysis.hh"
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
if (fHist == 0) {
cout << "hist not found" << endl; 
return false;
}
cout << " Entries  " << fHist->GetEntries() << endl;
return true;
}

double nEXOLightReadoutDigitize::GetEfficiency(double x, double y, double z)
{
double anodeZ = nEXODigiAnalysis::GetInstance()->GetAnodeZ(); 
double rho = sqrt(x*x+y*y)/10.0; //x and y in mm, rho in cm
double z0 = (anodeZ - z)/10.0; //z in mm, z=0 is cathode, z0 in cm, z0=0 is anode
double iBin = fHist->FindBin(rho, z0);

return fHist->GetBinContent(iBin); 
}

void nEXOLightReadoutDigitize::CalcNPE(Long64_t evtEntry)
{
  double anodeZ = nEXODigiAnalysis::GetInstance()->GetAnodeZ(); 
  fNPE = 0;
  fNPEactive = 0;
  fNOPactive = 0;
  //cout << "PE " << fNPE << endl;
  nEXODigiAnalysis::GetInstance()->GetEntry(evtEntry);
  Int_t NOP = nEXODigiAnalysis::GetInstance()->GetNumOP();
  //cout << "NOP " << NOP << endl;
  for (Int_t i=0; i<NOP; i++) {
    Double_t opX=0., opY=0., opZ=0., opE=0., opT=0.;
    nEXODigiAnalysis::GetInstance()->GetOP(i, opE, opX, opY, opZ, opT);
    //cout << "i argument " << i << endl;
    //cout << "opX position " << opX << endl;
    //cout << "opY position " << opY << endl;
    //cout << "opZ position " << opZ << endl;
    
    Double_t efficiency = nEXOLightReadoutDigitize::GetEfficiency(opX, opY, opZ);  
    fNPE += efficiency;
    //cout << "efficiency " << efficiency << endl;
    //cout << "PE " << fNPE << endl;
 
    Double_t rho= sqrt(opX*opX + opY*opY);
    //cout << "rho position " << rho << endl; 
    if ((-105 <= rho && rho <= 105) && (0 <= opZ && opZ <= anodeZ)) {
    fNPEactive += nEXOLightReadoutDigitize::GetEfficiency(opX, opY, opZ);
    //cout << "PEactive " << fNPEactive << endl;  
    fNOPactive += 1;
    //cout << "OPactive " << fNOPactive << endl;
     }	
    
     	
 }   
}
