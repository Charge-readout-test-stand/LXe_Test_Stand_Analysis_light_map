#ifndef nEXOChargeElecChannel_hh
#define nEXOChargeElecChannel_hh

#ifndef ROOT_TObject
#include "TObject.h"
#endif
#include <vector>

class nEXOChargeElecChannel : public TObject
{

  public:

    // Constructors
    nEXOChargeElecChannel(): TObject() { nEXOChargeElecChannel::Clear();}

    nEXOChargeElecChannel(Int_t chanId);
    nEXOChargeElecChannel(Int_t tileId, Int_t localId);
    nEXOChargeElecChannel(const nEXOChargeElecChannel& channel);

    // Modifiers
    void Clear(Option_t* opt = "");
    nEXOChargeElecChannel& operator = (const nEXOChargeElecChannel& rhs);

    void AddWaveformPoint(Double_t amplitude);
    void SetChannelTime(Double_t time);
    void SetChannelFirstTime(Double_t time);
    void SetChannelLatestTime(Double_t time);
    void SetChannelId(Int_t chanId);
    void SetChannelId(Int_t tileId, Int_t localId);
    void SetChannelCharge(Double_t charge);
    void SetChannelInductionAmplitude(Double_t amplitude);
    //void SetLocalId(Int_t localId);

    Double_t GetChannelTime();
    Double_t GetChannelFirstTime();
    Double_t GetChannelLatestTime();
    Int_t GetChannelId();
    Double_t GetChannelCharge();
    Double_t GetChannelInductionAmplitude();
    Double_t GetWaveformPoint(UInt_t i);
    //Int_t GetLocalId();
    //Int_t GetTileId();

    Int_t fChannelId;
    //Int_t fLocalId;
    Double_t fChannelCharge;
    Double_t fInductionAmplitude;
    Double_t fTime;
    Double_t fFirstTime;
    Double_t fLatestTime;

    std::vector<Double_t> fWaveform;
    
  ClassDef(nEXOChargeElecChannel,1) // Increment this number whenever the data members of nEXOChargeElecChannel are changed!

};

#endif
