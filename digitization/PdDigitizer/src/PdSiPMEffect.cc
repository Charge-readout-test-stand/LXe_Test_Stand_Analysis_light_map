#include "PdSiPMEffect.hh"
#include "PdRootIO.hh"
#include "PdSiPMHit.hh"
#include "PdParameters.hh"

#include "TMath.h"
#include "TRandom.h"

PdSiPMEffect::PdSiPMEffect() 
{
  fProb_ct = 0;
  fFunction = 0;
  fObjArray = new TObjArray();
}

PdSiPMEffect::~PdSiPMEffect()
{
}

double PdSiPMEffect::Expo(double* x, double* par)
{
  double value = TMath::Exp(-x[0]/par[0]) + 0.01*TMath::Exp(-x[0]/par[1]);

  return value;
}

TObjArray* PdSiPMEffect::ApplySiPMEffect(TObjArray* obj)
{
  //for(Int_t i = 0; i < fObjArray->GetEntries(); i++) {
  //  (obj->At(i))->Delete();
  //}
  fObjArray->Clear();

  Int_t nentries = obj->GetEntries();

  for(Int_t i = 0; i < nentries; i++) {
    PdSiPMHit* hit = static_cast<PdSiPMHit*> (obj->At(i));

    if(gRandom->Uniform() < PdParameters::GetInstance()->GetSiPMQE()) {
      fObjArray->Add(hit);
    }
  }

  ApplyDarkNoise();
  ApplyCorrAvalanche();
  ApplyPhotonCrossTalk();

  return fObjArray;
}

void PdSiPMEffect::ApplyDarkNoise()
{
  double darkrate = PdParameters::GetInstance()->GetDarkRate();
  double readoutwindow = PdParameters::GetInstance()->GetReadoutWindow();
  int nSiPMPads = PdParameters::GetInstance()->GetNSiPMPads();

  int ndarknoise = gRandom->Poisson(darkrate*readoutwindow*1.0e-9*nSiPMPads);

  for(int i = 0; i < ndarknoise; i++) {
    int siPMPadID = (int) (gRandom->Uniform()*nSiPMPads);
    PdSiPMHit* hit = new PdSiPMHit(siPMPadID, -100, 0.0, gRandom->Uniform()*readoutwindow);
    fObjArray->Add(hit);
  }
}

void PdSiPMEffect::ApplyCorrAvalanche()
{
  if(fProb_ct == 0) {
    fProb_ct = new TH1F("prob_ct", "", 10, 1, 11);
    double lambda = PdParameters::GetInstance()->GetLambda();
    for(int ibin = 0; ibin < 10; ibin++) {
      double prob = TMath::Power(lambda*(ibin+1),ibin)*TMath::Exp(-lambda*(ibin+1))/TMath::Factorial(ibin+1);
      fProb_ct->SetBinContent(ibin+1,prob);
    }
  }

  if(fFunction == 0) {
    fFunction = new TF1("function", this, &PdSiPMEffect::Expo, 0, 1000, 2, "PdSiPMEffect", "Evaluate");
    fFunction->SetParameter(0, PdParameters::GetInstance()->GetTrapTauFast());
    fFunction->SetParameter(1, PdParameters::GetInstance()->GetTrapTauSlow());
  }

  int nentries = fObjArray->GetEntries();

  for(int i = 0; i < nentries; i++) {
    PdSiPMHit* hit = static_cast<PdSiPMHit*> (fObjArray->At(i));

    int siPMPadID = hit->GetSiPMPadID();
    double time = hit->GetHitTime();
    int nCorrA = (int) fProb_ct->GetRandom() -  1;

    for(int i = 0; i < nCorrA; i++) {
      PdSiPMHit* newhit = new PdSiPMHit(siPMPadID, -1000, 0.0, time + fFunction->GetRandom());
      fObjArray->AddLast(newhit);
    } 
  }
}

void PdSiPMEffect::ApplyPhotonCrossTalk()
{
  int nentries = fObjArray->GetEntries();

  for(int i = 0; i < nentries; i++) {
    PdSiPMHit* hit = static_cast<PdSiPMHit*> (fObjArray->At(i));

    int siPMPadID = hit->GetSiPMPadID();
    double time = hit->GetHitTime();

    if(gRandom->Uniform() < PdParameters::GetInstance()->GetCrossTalkProb()) {
      PdSiPMHit* newhit = new PdSiPMHit(siPMPadID, -10000, 0.0, time + gRandom->Uniform()*5);
      fObjArray->AddLast(newhit);
    }
  }
}
