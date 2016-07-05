#include "nEXOUtilities/nEXOChargeReadout.hh"
#include "nEXOUtilities/nEXOTreeArrayLengths.hh"

#include <iostream>
using namespace std;

ClassImp(nEXOChargeReadout)

nEXOChargeReadout::nEXOChargeReadout()
: TObject(),
  fChargeChannels(0)
{
  Clear();
  fChargeChannels = new nEXOTClonesArray( nEXOChargeElecChannel::Class(), MAXCHANNEL );
}

nEXOChargeReadout::nEXOChargeReadout(Int_t tileId)
: TObject(),
  fChargeChannels(0)
{
  Clear();
  fTileId = tileId;
  fChargeChannels = new nEXOTClonesArray( nEXOChargeElecChannel::Class(), MAXCHANNEL );
}

nEXOChargeReadout::nEXOChargeReadout(const nEXOChargeReadout& readout) : TObject(readout)
{
  Clear();
  *this = readout;
}

nEXOChargeReadout& nEXOChargeReadout::operator = (const nEXOChargeReadout& rhs)
{
  TObject::operator=(rhs);
  fTileId = rhs.fTileId;

  return *this;
}

void nEXOChargeReadout::Clear(Option_t*)
{
  TObject::Clear();
}

void nEXOChargeReadout::ClearCRTs()
{
  fCRTMap.clear();
}

Int_t nEXOChargeReadout::GetTileId() 
{
  return fTileId;
}

void nEXOChargeReadout::SetTileId(Int_t tileId)
{
  fTileId = tileId;
}

const nEXOChargeElecChannel* nEXOChargeReadout::FindChargeChannel(Int_t chanId) const
{
  // Get Pixelated charge object at coordinate pixel key.
  // Returns NULL if no key is found.
  if (GetNumChargeChannels() == 0) return NULL;
  //CheckCRTMap();
  CRTMap::iterator iter = fCRTMap.find( chanId );
  if ( iter == fCRTMap.end() ) return NULL;
  return static_cast<const nEXOChargeElecChannel*>(iter->second);
}

void nEXOChargeReadout::FillCRTMap(nEXOChargeElecChannel* chargeChannel) const
{
  fCRTMap[chargeChannel->fChannelId] = chargeChannel;
}
