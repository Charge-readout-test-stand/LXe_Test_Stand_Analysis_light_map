#ifndef nEXOFieldWP_hh
#define nEXOFieldWP_hh 1

#ifndef nEXOCoordinates_hh 
#include "nEXOUtilities/nEXOCoordinates.hh"
#endif

#ifndef nEXODimensions_hh
#include "nEXOUtilities/nEXODimensions.hh"
#endif

#include <map>
#include <vector>
#include <utility>
#include "TString.h"
#include "TVector3.h"
#include "TH3F.h"
#include "TFile.h"

class nEXOFieldWP
{
  public:

    static nEXOFieldWP* GetInstance() {
      if(nEXOFieldWP::fInstance == NULL) nEXOFieldWP::fInstance = new nEXOFieldWP();
      return nEXOFieldWP::fInstance;
    }

    // Functions
    void Clear(Option_t* opt = "");
    bool LoadWP(TString coType, TString filename);
    Double_t  CalcInducedChargeOnPads_COMSOL(Int_t chanId, Double_t xq, Double_t yq, Double_t zq);
    Double_t  CalcInducedChargeOnPads_Analytic(Int_t chanId, Double_t xq, Double_t yq, Double_t zq);

    TString fChargeReadoutType;
    TFile*  fWPFile;

  protected:
    TH3D* fPadWP;

  private:
    nEXOFieldWP();
    ~nEXOFieldWP();
    static nEXOFieldWP* fInstance;
    Double_t EvaluateInduction(Double_t xpcd, Double_t ypcd, Double_t zpcd, Double_t chx, Double_t chy);
    Double_t InductionFunction(Double_t x, Double_t y, Double_t z);
};

#endif
