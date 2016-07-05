#include "PdFeeChannel.hh"

#include <iostream>

//ClassImp(PdFeeChannel);

PdFeeChannel::PdFeeChannel() 
{
  Reset();
}

PdFeeChannel::PdFeeChannel(Int_t id, Double_t charge, Double_t time, Double_t* pulse) 
{
  fChannelID     = id;
  fChannelCharge = charge;
  fChannelTime   = time;
  for(int i = 0; i < 1000; i++) fPulseShape[i]   = pulse[i];
}

PdFeeChannel::PdFeeChannel(const PdFeeChannel& right)
{
  fChannelID     = right.fChannelID;
  fChannelCharge = right.fChannelCharge;
  fChannelTime   = right.fChannelTime;
  for(int i = 0; i < 1000; i++) fPulseShape[i]   = right.fPulseShape[i];
}

const PdFeeChannel& PdFeeChannel::operator=(const PdFeeChannel& right)
{
  fChannelID     = right.fChannelID;
  fChannelCharge = right.fChannelCharge;
  fChannelTime   = right.fChannelTime;
  for(int i = 0; i < 1000; i++) fPulseShape[i]   = right.fPulseShape[i];
  
  return *this;
}

int PdFeeChannel::operator==(const PdFeeChannel& right) const
{
  return (this==&right) ? 1 : 0;
}

PdFeeChannel::~PdFeeChannel() 
{
}

Int_t PdFeeChannel::GetChannelID()
{
  return fChannelID;
}

Int_t PdFeeChannel::GetSignalType()
{
  return fSignalType;
}

Double_t PdFeeChannel::GetChannelCharge()
{
  return fChannelCharge;
}

Double_t PdFeeChannel::GetChannelTime()
{
  return fChannelTime;
}

Double_t PdFeeChannel::GetPulseShape(int i)
{
  return fPulseShape[i];
}

void PdFeeChannel::SetChannelID(Int_t id)
{
  fChannelID = id;
}

void PdFeeChannel::SetSignalType(Int_t t)
{
  fSignalType = t;
}

void PdFeeChannel::SetChannelCharge(Double_t charge)
{
  fChannelCharge = charge;
}

void PdFeeChannel::SetChannelTime(Double_t time)
{
  fChannelTime = time;
}

void PdFeeChannel::SetPulseShape(Int_t i, Double_t pulse)
{
  fPulseShape[i] = pulse;
}

void PdFeeChannel::Reset() 
{
  fChannelID = 0;

  fChannelCharge = 0.0;
  fChannelTime = 0.0;
  fSignalType = 0;
  
  for(Int_t i = 0; i < 1000; i++) {
    fPulseShape[i] = 0.0;
  }
}
