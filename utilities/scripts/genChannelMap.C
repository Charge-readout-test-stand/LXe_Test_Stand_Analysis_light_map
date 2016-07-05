#include "TString.h"
#include <bitset>
#include <string>

void genChannelMap() {
  // units in mm
  const double PAD_PITCH = 2.0;
  const double MCCHARGE_PIXEL_SIZE = PAD_PITCH/20.0;
  const double TILE_SIZE = 96.0;
  const double TPC_RADIUS = 650.0;
  const double ANODE_POS = 650.0;

  // Generate the Tiles Map, this is a temporary one
  // there might be overlap with the real geometry, will be improved.
  ofstream outdat;
  outdat.open("tilesMap.txt");
  Int_t tileId = 0;
  for(Int_t i=0; i<20; i++) {
    Double_t X = TILE_SIZE/2. + TILE_SIZE*(i-10);
    for(Int_t j=0; j<20; j++) {
      Double_t Y = TILE_SIZE/2. + TILE_SIZE*(j-10);

      Double_t Xl = X - TILE_SIZE/2.;
      Double_t Xr = X + TILE_SIZE/2.;

      Double_t Yl = Y - TILE_SIZE/2.;
      Double_t Yr = Y + TILE_SIZE/2.;

      Double_t radius1 = TMath::Sqrt( Xl*Xl + Yl*Yl );
      Double_t radius2 = TMath::Sqrt( Xl*Xl + Yr*Yr );
      Double_t radius3 = TMath::Sqrt( Xr*Xr + Yl*Yl );
      Double_t radius4 = TMath::Sqrt( Xr*Xr + Yr*Yr );

      if( radius1>TPC_RADIUS && radius2>TPC_RADIUS && radius3>TPC_RADIUS && radius4>TPC_RADIUS) continue;

      Int_t xId = X<0? i-10: i-9;
      Int_t yId = Y<0? j-10: j-9;

      cout << "tile (X, Y) position: (" << X << ", " << Y
           << "), index ("<< xId << ", " << yId
           << ")" << endl;
      outdat << Form("%5d %10.4f %10.4f %10.4f\n", tileId, X, Y, ANODE_POS);

      tileId++;
    }
  }
  outdat.close();

  // Generate the map for local channels in a tile
  outdat.open("localChannelsMap_2mm.txt");
  Int_t chanId = 0;
  // Local channels in vertical direction
  Int_t Nch = (Int_t) TILE_SIZE/PAD_PITCH + 1;
  for(Int_t i=0; i<Nch; i++) {
    Double_t X = PAD_PITCH*(i-(Nch-1)/2);
    Double_t Y = 0.;
    outdat << Form("%5d  %10.4f %10.4f %10.4f   X\n", chanId, X, Y, 0.);
    chanId++;
  }
  // Local channels in vertical direction
  for(Int_t j=0; j<Nch; j++) {
    Double_t X = 0.;
    Double_t Y = PAD_PITCH*(j-(Nch-1)/2);
    outdat << Form("%5d  %10.4f %10.4f %10.4f   Y\n", chanId, X, Y, 0.);
    chanId++;
  }
  outdat.close();
}
