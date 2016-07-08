#ifndef nEXOLightReadoutDigitize_hh
#define nEXOLightReadoutDigitize_hh
#include <set>
#ifndef ROOT_TObject
#include "TObject.h"
#endif
#include "TFile.h"
#include "TH2D.h"

class nEXOLightReadoutDigitize
{
public:
  nEXOLightReadoutDigitize();
  ~nEXOLightReadoutDigitize();

  void GeneratePCDs();
  void Digitize();
  void SetNoiseModel();
  void CalcNPE(Long64_t evtEntry);
  double GetEfficiency(double x, double y, double z);
  bool LoadLightMap(TString lightMapName);

  Double_t GetNPE() { return fNPE; }
  Double_t GetNPEactive() { return fNPEactive; }
  Int_t GetNOPactive() { return fNOPactive; }

private:
TH2D* fHist;
TFile* fFile;
Double_t fNPE;
Double_t fNPEactive;
Int_t fNOPactive;
};

#endif
