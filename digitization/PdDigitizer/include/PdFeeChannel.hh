//**********************************************************************
//  A class is designed for data model of each FEE channel.
//  caogf@ihep.ac.cn
//  2014.07.25
//*********************************************************************

#ifndef PDFEECHANNEL_H
#define PDFEECHANNEL_H

#include "TObject.h"

class PdFeeChannel : public TObject
{
public:

  PdFeeChannel();
  PdFeeChannel(Int_t id, Double_t energy, Double_t time, Double_t* pusle);
  PdFeeChannel(const PdFeeChannel&);
  ~PdFeeChannel();

  const PdFeeChannel& operator=(const PdFeeChannel&);
  int operator==(const PdFeeChannel&) const;

  void Reset();

  Int_t GetChannelID();
  Int_t GetSignalType();
  Double_t GetChannelCharge();
  Double_t GetChannelTime();
  Double_t GetPulseShape(Int_t i);

  void SetChannelID(Int_t id);
  void SetSignalType(Int_t t);
  void SetChannelCharge(Double_t charge);
  void SetChannelTime(Double_t time);
  void SetPulseShape(Int_t i, Double_t amplitude);

private:

  Int_t fChannelID; //ID of FEE channel

  Int_t fSignalType; //hit type, signal or dark noise/correlated noise/cross talk
  Double_t fChannelCharge; //total charge of each channel
  Double_t fChannelTime; //time of each channel
  Double_t fPulseShape[1000]; //pulse shape, 1GHz sample rate, 1us readout window

//public:

  //ClassDef(PdFeeChannel, 1);
};

#endif
