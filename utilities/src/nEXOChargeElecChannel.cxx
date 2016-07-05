#ifndef nEXOChargeElecChannel_hh
#include "nEXOUtilities/nEXOChargeElecChannel.hh"
#endif

#ifndef nEXOChannelMap_hh
#include "nEXOUtilities/nEXOChannelMap.hh"
#endif

#ifndef HEP_SYSTEM_OF_UNITS_H
#include "nEXOUtilities/SystemOfUnits.hh"
#endif

#include <cassert>

ClassImp(nEXOChargeElecChannel)

nEXOChargeElecChannel::nEXOChargeElecChannel(Int_t tileId, Int_t localId)
: TObject()
{
  Clear();
  // if each channel has an unique channel Id,
  // the tile id and local id can be extracted by the channel map service
  fChannelId = nEXOChannelMap::GetInstance()->GetChannelId(tileId, localId);
  fTime = 0.0;
  fFirstTime = 1.*CLHEP::second;
  fLatestTime = 0.0;
  fChannelCharge = 0.0;
  fInductionAmplitude = 0.0;
  fWaveform.clear();
}

nEXOChargeElecChannel::nEXOChargeElecChannel(Int_t chanId)
: TObject()
{
  Clear();
  fChannelId = chanId;
  fTime = 0.0;
  fFirstTime = 1.*CLHEP::second;
  fLatestTime = 0.0;
  fChannelCharge = 0.0;
  fInductionAmplitude = 0.0;
  fWaveform.clear();
}

nEXOChargeElecChannel::nEXOChargeElecChannel(const nEXOChargeElecChannel& channel) : TObject(channel)
{
  Clear();
  *this = channel;
}

nEXOChargeElecChannel& nEXOChargeElecChannel::operator = (const nEXOChargeElecChannel& rhs)
{
  TObject::operator=(rhs);
  fChannelId = rhs.fChannelId;
  //fLocalId = rhs.fLocalId;
  fTime = rhs.fTime;
  fFirstTime = rhs.fFirstTime;
  fLatestTime = rhs.fLatestTime;
  fChannelCharge = rhs.fChannelCharge;
  fInductionAmplitude = rhs.fInductionAmplitude;
  fWaveform = rhs.fWaveform;

  return *this;
}

void nEXOChargeElecChannel::Clear(Option_t*)
{
  TObject::Clear();
}

void nEXOChargeElecChannel::AddWaveformPoint(Double_t amplitude)
{
  fWaveform.push_back(amplitude);
}

void nEXOChargeElecChannel::SetChannelTime(Double_t time)
{
  fTime = time;
}

void nEXOChargeElecChannel::SetChannelFirstTime(Double_t time)
{
  fFirstTime = time;
}

void nEXOChargeElecChannel::SetChannelLatestTime(Double_t time)
{
  fLatestTime = time;
}

void nEXOChargeElecChannel::SetChannelId(Int_t chanId)
{
  fChannelId = chanId;
}

void nEXOChargeElecChannel::SetChannelId(Int_t tileId, Int_t localId)
{
  fChannelId = nEXOChannelMap::GetInstance()->GetChannelId(tileId, localId);
}

//void nEXOChargeElecChannel::SetLocalId(Int_t localId)
//{
//  fLocalId = localId;
//}

void nEXOChargeElecChannel::SetChannelCharge(Double_t charge)
{
  fChannelCharge = charge;
}

void nEXOChargeElecChannel::SetChannelInductionAmplitude(Double_t amplitude)
{
  fInductionAmplitude = amplitude;
}

Double_t nEXOChargeElecChannel::GetChannelTime()
{
  return fTime;
}

Double_t nEXOChargeElecChannel::GetChannelFirstTime()
{
  return fFirstTime;
}

Double_t nEXOChargeElecChannel::GetChannelLatestTime()
{
  return fLatestTime;
}

Int_t nEXOChargeElecChannel::GetChannelId()
{
  return fChannelId;
}

//Int_t nEXOChargeElecChannel::GetLocalId()
//{
//  return fLocalId;
//}

Double_t nEXOChargeElecChannel::GetChannelCharge()
{
  return fChannelCharge;
}

Double_t nEXOChargeElecChannel::GetChannelInductionAmplitude()
{
  return fInductionAmplitude;
}

Double_t nEXOChargeElecChannel::GetWaveformPoint(UInt_t i)
{
  if(i>fWaveform.size()) 
    return -9999.;
  else 
    return fWaveform[i];
}
