#include "PdSiPMHit.hh"

//ClassImp(PdSiPMHit);

PdSiPMHit::PdSiPMHit() 
{
}

PdSiPMHit::PdSiPMHit(Int_t id, Int_t type, Double_t energy, Double_t time) 
{
  fSiPMPadID = id;
  fHitType = type;
  fPhotonEnergy = energy;
  fHitTime = time;
}

PdSiPMHit::PdSiPMHit(const PdSiPMHit& right)
{
  fSiPMPadID     = right.fSiPMPadID;
  fHitType       = right.fHitType;
  fPhotonEnergy  = right.fPhotonEnergy;
  fHitTime       = right.fHitTime;
}

const PdSiPMHit& PdSiPMHit::operator=(const PdSiPMHit& right)
{
  fSiPMPadID     = right.fSiPMPadID;
  fHitType       = right.fHitType;
  fPhotonEnergy  = right.fPhotonEnergy;
  fHitTime       = right.fHitTime;
  
  return *this;
}

int PdSiPMHit::operator==(const PdSiPMHit& right) const
{
  return (this==&right) ? 1 : 0;
}

PdSiPMHit::~PdSiPMHit() 
{
}

Int_t PdSiPMHit::GetSiPMPadID()
{
  return fSiPMPadID;
}

Double_t PdSiPMHit::GetPhotonEnergy()
{
  return fPhotonEnergy;
}

Double_t PdSiPMHit::GetHitTime()
{
  return fHitTime;
}

Int_t PdSiPMHit::GetHitType()
{
  return fHitType;
}

void PdSiPMHit::SetSiPMPadID(Int_t id)
{
  fSiPMPadID = id;
}

void PdSiPMHit::SetPhotonEnergy(Double_t energy)
{
  fPhotonEnergy = energy;
}

void PdSiPMHit::SetHitTime(Double_t time)
{
  fHitTime = time;
}

void PdSiPMHit::SetHitType(Int_t t)
{
  fHitType = t;
}
