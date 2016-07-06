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
  double GetEfficiency(double x, double y, double z);
  bool LoadLightMap(TString lightMapName);
private:
TH2D* fHist;
TFile* fFile;
};

#endif
