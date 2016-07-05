#ifndef nEXOChannelMap_hh
#define nEXOChannelMap_hh 1

#ifndef nEXOCoordinates_hh 
#include "nEXOUtilities/nEXOCoordinates.hh"
#endif

#ifndef nEXODimensions_hh
#include "nEXOUtilities/nEXODimensions.hh"
#endif

#include <map>
#include <vector>
#include "TString.h"
#include "TVector3.h"

class nEXOChannelMap
{

  public:

    static nEXOChannelMap* GetInstance() {
      if(nEXOChannelMap::fInstance == NULL) nEXOChannelMap::fInstance = new nEXOChannelMap();
      return nEXOChannelMap::fInstance;

    }

    // Functions
    void Clear(Option_t* opt = "");
    Bool_t ReadInTileMap(TString filename);
    Bool_t ReadInPadsMap(TString filename);

    Bool_t FindHitChargeChannel(const nEXOCoordinates& coord, Int_t& tileId, Int_t& localId);
    TVector3 GetChannelLocalPosition(Int_t i);
    TVector3 GetTilePosition(Int_t i);

    TVector3 GetChannelPosition(Int_t tileId, Int_t localId);
    TVector3 GetChannelPosition(Int_t chanId);
    TString  GetChannelType(Int_t chanId);
	 double PAD_PITCH;
	 int ChannelNumOnTile;
	 void SetPadSize(double padsize) {PAD_PITCH = padsize;};
	 double GetPadSize(){return PAD_PITCH;};
	 int GetChannelNumOnTile(){return (int(TILE_SIZE/PAD_PITCH) + 1)*2;};
	 double GetMCCHARGE_PIXEL_SIZE(){return PAD_PITCH/20/sqrt(2.)*5.0;};
	 double GetMCINDUCTION_PIXEL_SIZE(){return PAD_PITCH/10;};


    // extract the tile Id and local Id from the global channel Id
    Int_t GetTileId(Int_t chanId);
    Int_t GetLocalId(Int_t chanId);

    // convert (tileId, localId) pair into a global channel Id
    Int_t GetChannelId(Int_t tileId, Int_t localId);

    std::vector<TVector3> GetPadsCoord(Int_t chanId);

  protected:

    std::map<Int_t, TVector3> fTiles;
    std::map<Int_t, TVector3> fLocalChannels;
    std::map<Int_t, TString> fLocalChannelsType;

  private:
    nEXOChannelMap();
    ~nEXOChannelMap();
    static nEXOChannelMap* fInstance;
};
#endif
