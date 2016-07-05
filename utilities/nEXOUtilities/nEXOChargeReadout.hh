#ifndef nEXOChargeReadout_hh
#define nEXOChargeReadout_hh

#include "nEXOUtilities/nEXOChargeElecChannel.hh"
#include "nEXOUtilities/nEXOTClonesArray.hh"

/*
 * nEXOChargeReadout is a collection of nEXOChargeElecChannel 
 * in each charge readout tile
*/

#ifndef ROOT_TObject
#include "TObject.h"
#endif
#include <map>
using namespace std;

class nEXOChargeReadout : public TObject
{
  public:

    // Constructors
    nEXOChargeReadout();
    nEXOChargeReadout(Int_t tileId);
    nEXOChargeReadout(const nEXOChargeReadout& readout);

    // Modifiers
    void Clear(Option_t* opt = "");
    nEXOChargeReadout& operator = (const nEXOChargeReadout& rhs);

    const nEXOChargeElecChannel* FindChargeChannel(Int_t chanId) const;
    nEXOChargeElecChannel* FindOrCreateChargeChannel(Int_t chanId);
    void SetTileId(Int_t tileId);
    void ClearCRTs();

    const nEXOTClonesArray* GetChargeChannelsArray() const;
    nEXOChargeElecChannel* GetChargeChannel(size_t i);
    Int_t GetTileId();

    size_t GetNumChargeChannels() const;
    //size_t GetCRTMapSize() {return fCRTMap.size();};

    void FillCRTMap(nEXOChargeElecChannel* chargeChannel) const;

  protected:
    Int_t fTileId;
    nEXOTClonesArray* fChargeChannels;
    // CRT: Charge Readout Tile, the key here is the local channel Id
    typedef std::map<Int_t, nEXOChargeElecChannel*> CRTMap;
    mutable CRTMap fCRTMap;

    nEXOTClonesArray* GetChargeChannelsArray();

  ClassDef(nEXOChargeReadout,1) // Increment this number whenever the data members of nEXOChargeReadout are changed!

};

inline size_t nEXOChargeReadout::GetNumChargeChannels() const
{
  // Get Number of pixelated charge deposit objects in event
  return (fChargeChannels) ? fChargeChannels->GetEntriesFast() : 0;
}

inline nEXOChargeElecChannel* nEXOChargeReadout::GetChargeChannel(size_t i)
{
  return static_cast< nEXOChargeElecChannel* >( GetChargeChannelsArray()->At(i) );
}

inline const nEXOTClonesArray* nEXOChargeReadout::GetChargeChannelsArray() const
{
  return static_cast< const nEXOTClonesArray* >( fChargeChannels );
}

inline nEXOTClonesArray* nEXOChargeReadout::GetChargeChannelsArray()
{
  return static_cast< nEXOTClonesArray* >( fChargeChannels );
}

inline nEXOChargeElecChannel* nEXOChargeReadout::FindOrCreateChargeChannel(Int_t chanId)
{
  // Try to find the pixel, in case it already exists.
  nEXOChargeElecChannel* chargeChannel =
  const_cast<nEXOChargeElecChannel*>(FindChargeChannel(chanId));
  if(chargeChannel) return chargeChannel;
  
  // If it doesn't exist.  Create it, and initialize it to the appropriate pixel.
  chargeChannel = static_cast<nEXOChargeElecChannel*>(
  GetChargeChannelsArray()->GetNewOrCleanedObject(GetNumChargeChannels()) );
  chargeChannel->SetChannelId(chanId);
  chargeChannel->SetChannelFirstTime(1e9);
  chargeChannel->SetChannelLatestTime(0.);
  FillCRTMap(chargeChannel);
  return chargeChannel;
}

#endif
