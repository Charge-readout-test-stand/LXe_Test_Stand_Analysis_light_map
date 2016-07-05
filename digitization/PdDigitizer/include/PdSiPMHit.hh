//**********************************************************************
//  A class is designed for data model of SiPM input.
//  caogf@ihep.ac.cn
//  2014.07.25
//*********************************************************************

#ifndef PDSIPMHIT_H
#define PDSIPMHIT_H

#include "TObject.h"

class PdSiPMHit : public TObject
{
public:

  PdSiPMHit();
  ~PdSiPMHit();
  PdSiPMHit(Int_t id, Int_t type, Double_t energy, Double_t time);
  PdSiPMHit(const PdSiPMHit&);

  const PdSiPMHit& operator=(const PdSiPMHit&);
  int operator==(const PdSiPMHit&) const;

  Int_t GetSiPMPadID();
  Double_t GetPhotonEnergy();
  Double_t GetHitTime();
  Int_t GetHitType();

  void SetSiPMPadID(Int_t id);
  void SetPhotonEnergy(Double_t e);
  void SetHitTime(Double_t t);
  void SetHitType(Int_t t);

private:

  Int_t fSiPMPadID; //ID of SiPM

  Int_t fHitType; //hit type
  Double_t fPhotonEnergy; //the energy of photon that hits on the SiPM
  Double_t fHitTime; //the time to hit on the SiPM

//public:

  //ClassDef(PdSiPMHit, 1);

};

#endif
