#ifndef nEXOChannelMap_hh
#include "nEXOUtilities/nEXOChannelMap.hh"
#endif

#ifdef nEXODimensions_hh
#include "nEXOUtilities/nEXODimensions.hh"
#endif

#include "TString.h"
#include "TRandom.h"
#include <fstream>
#include <iostream>
using namespace std;
nEXOChannelMap* nEXOChannelMap::fInstance = NULL;


nEXOChannelMap::nEXOChannelMap() 
{
  Clear();
}

nEXOChannelMap::~nEXOChannelMap() 
{
}

// Functions
void nEXOChannelMap::Clear(Option_t* opt) {
  fLocalChannels.clear();
  fLocalChannelsType.clear();
  fTiles.clear();
}

bool nEXOChannelMap::ReadInTileMap(TString filename)
{
  // for the time being, read in the channel map from external .txt file
  std::ifstream indat;
  indat.open(filename);

  if(indat.fail()) return false;

  Int_t tileNum=0, tileId=0;
  Double_t X=0., Y=0., Z=0.;

  indat >> tileId >> X >> Y >> Z;
  while(indat.eof()!=true) {
    std::cout << Form("Tile: %4d %9.4f %9.4f %9.4f", tileId, X, Y, Z) << std::endl;
    TVector3 pos = TVector3(X, Y, Z);
    fTiles[tileId] = pos;
    tileNum++;
    indat >> tileId >> X >> Y >> Z;
  }
  std::cout << Form("Total %4d tiles", tileNum) << std::endl;
  indat.close();

  return true;
}

bool nEXOChannelMap::ReadInPadsMap(TString filename)
{
  // for the time being, read in the channel map from external .txt file
  std::ifstream indat;
  indat.open(filename);

  if(indat.fail()) return false;

  Int_t chanNum=0, chanId=0;
  Double_t X=0., Y=0., Z=0.;
  TString chanType;

  indat >> chanId >> X >> Y >> Z >> chanType;
  while(indat.eof()!=true) {
    std::cout << Form("Local channel: %4d %9.4f %9.4f %9.4f  %s", chanId, X, Y, Z, chanType.Data()) << std::endl;
    TVector3 pos = TVector3(X, Y, Z);
    fLocalChannels[chanId] = pos;
    fLocalChannelsType[chanId] = chanType;
    chanNum++;
    indat >> chanId >> X >> Y >> Z >> chanType;
  }
  std::cout << Form("Total %4d local channels on each tile", chanNum) << std::endl;
  indat.close();

  return true;
}

Bool_t nEXOChannelMap::FindHitChargeChannel(const nEXOCoordinates& coord, Int_t& tileId, Int_t& localId)
{
  // find the channel that be hit by this PCD with position of coord
  // If NO hit channel could be find, returen value is false.
  // Loop the tiles, and find out which tile this PCD will hit
  std::map<Int_t, TVector3>::iterator iter;
  std::vector<Int_t> atTileEdge;
  atTileEdge.clear();
  Bool_t isFindHitInsideTile = false;
  TVector3 tilePos;
  Double_t epsilon = 1.e-9;
  // find the tile that be hit
  for(iter = fTiles.begin(); iter!=fTiles.end(); iter++){
    tileId = iter->first;
    tilePos = iter->second;

    Int_t deltaX = TMath::Abs(tilePos.x() - coord.GetX());
    Int_t deltaY = TMath::Abs(tilePos.y() - coord.GetY());

    if( deltaX < TILE_SIZE/2.-epsilon && deltaY < TILE_SIZE/2.-epsilon ) 
    {
      isFindHitInsideTile = true;
      break;
    }
    else if( (TMath::Abs(deltaX - TILE_SIZE/2.) < epsilon && deltaY < TILE_SIZE/2.-epsilon) 
          || (TMath::Abs(deltaY - TILE_SIZE/2.) < epsilon && deltaX < TILE_SIZE/2.-epsilon) 
          || (TMath::Abs(deltaY - TILE_SIZE/2.) < epsilon && TMath::Abs(deltaX - TILE_SIZE/2.) < epsilon) ) {
      atTileEdge.push_back(tileId);
    }
  }

  if(isFindHitInsideTile==false) {
    if(atTileEdge.size()==0) return false;
    else {
      Int_t idx = (Int_t)(gRandom->Uniform()*atTileEdge.size());
      //std::cout << "This PCD is shared by " << atTileEdge.size() << "tiles." << std::endl;
      tileId = atTileEdge[idx];
      tilePos = fTiles[tileId];
    }
  }

  // find the channel that be hit
  Double_t pX = coord.GetX()-tilePos.x()+TILE_SIZE/2.;
  Double_t pY = coord.GetY()-tilePos.y()+TILE_SIZE/2.;
  Int_t xLow = (Int_t) (pX/nEXOChannelMap::GetInstance()->GetPadSize());
  Int_t yLow = (Int_t) (pY/nEXOChannelMap::GetInstance()->GetPadSize());
  Int_t xUp = xLow+1;
  Int_t yUp = yLow+1;
  if(yUp==nEXOChannelMap::GetInstance()->GetChannelNumOnTile()/2) yUp = nEXOChannelMap::GetInstance()->GetChannelNumOnTile()/2-1;
  if(xUp==nEXOChannelMap::GetInstance()->GetChannelNumOnTile()/2) xUp = nEXOChannelMap::GetInstance()->GetChannelNumOnTile()/2-1;

  Double_t dXl = TMath::Abs(pX - xLow*nEXOChannelMap::GetInstance()->GetPadSize());
  Double_t dXr = TMath::Abs(pX - xUp*nEXOChannelMap::GetInstance()->GetPadSize());
  Double_t dYl = TMath::Abs(pY - yLow*nEXOChannelMap::GetInstance()->GetPadSize());
  Double_t dYr = TMath::Abs(pY - yUp*nEXOChannelMap::GetInstance()->GetPadSize());

  if( TMath::Abs(dXl-dYl)<epsilon || TMath::Abs(dXl-dYr)<epsilon ||
      TMath::Abs(dXr-dYl)<epsilon || TMath::Abs(dXr-dYr)<epsilon) {
    std::cout << "at Channnel Edge!" << std::endl;
  }

  Double_t dist=1e6;
  if(dist>dXl) { dist=dXl; localId = xLow; }
  if(dist>dXr) { dist=dXr; localId = xUp; }
  if(dist>dYl) { dist=dYl; localId = yLow + nEXOChannelMap::GetInstance()->GetChannelNumOnTile()/2; }
  if(dist>dYr) { dist=dYr; localId = yUp + nEXOChannelMap::GetInstance()->GetChannelNumOnTile()/2; }

  return true;
}

TVector3 nEXOChannelMap::GetChannelPosition(Int_t tileId, Int_t localId) {
  TVector3 tilePos = GetTilePosition(tileId);
  TVector3 localChannelPos = GetChannelLocalPosition(localId);
  TVector3 chanPos = tilePos + localChannelPos;
  return chanPos;
}

TVector3 nEXOChannelMap::GetChannelPosition(Int_t chanId) {
  return GetChannelPosition(GetTileId(chanId), GetLocalId(chanId));
}

TVector3 nEXOChannelMap::GetChannelLocalPosition(Int_t i) {
  return fLocalChannels[i];
}

TString nEXOChannelMap::GetChannelType(Int_t chanId) {
  return fLocalChannelsType[chanId];
}

TVector3 nEXOChannelMap::GetTilePosition(Int_t i) {
  return fTiles[i];
}

Int_t nEXOChannelMap::GetTileId(Int_t chanId) {
  return ((chanId & 0x0000ff00) >> 8);
}

Int_t nEXOChannelMap::GetLocalId(Int_t chanId) {
  return (chanId & 0x000000ff);
}

Int_t nEXOChannelMap::GetChannelId(Int_t tileId, Int_t localId) {
  // the format of the channel Id, a 32-bit number
  // 00000000 | 00000000 | tileId | localId 
  Int_t chanId = 0;
  chanId = ( (0x000000ff & tileId) << 8);
  chanId = ( (0x000000ff & localId) | chanId );
  return chanId;
}

std::vector<TVector3> nEXOChannelMap::GetPadsCoord(Int_t chanId) {
  TVector3 chanPos = nEXOChannelMap::GetInstance()->GetChannelPosition(chanId);
  Int_t localId = nEXOChannelMap::GetInstance()->GetLocalId(chanId);
  std::vector<TVector3> padsCoord;
  padsCoord.clear();

  Int_t halfNum = (nEXOChannelMap::GetInstance()->GetChannelNumOnTile()/2 - 1)/2;
  
  //For the Given X/Y channel cycle over each of the squares on the tile by changing the Y/X by one pitch
  //Cycles from -1/2num to +1/2num
  //So extend up by 15 squares
  //Change this to go up all the squares so get rid of the 2nd factor of 2 in halfNum 
  if(localId<nEXOChannelMap::GetInstance()->GetChannelNumOnTile()/2) {
    // it's X- channel
    for(Int_t k=-halfNum; k<halfNum; k++) {
      padsCoord.push_back(TVector3(chanPos.x(), chanPos.y()-k*nEXOChannelMap::GetInstance()->GetPadSize(), chanPos.z()));
    }
  }
  else {
    // it's Y- channel
    for(Int_t k=-halfNum; k<halfNum; k++) {
      padsCoord.push_back(TVector3(chanPos.x()-k*nEXOChannelMap::GetInstance()->GetPadSize(), chanPos.y(), chanPos.z()));
    }
  }
  
  // check the padsCoord vector
  //std::cout << padsCoord.size() << std::endl;
  //for(size_t k=0; k<padsCoord.size(); k++) {
  //  std::cout << padsCoord[k].x() << ", " << padsCoord[k].y() << ", " << padsCoord[k].z() << std::endl;
  //}
  return padsCoord;
}
