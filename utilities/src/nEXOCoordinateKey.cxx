#include "nEXOUtilities/nEXOCoordinateKey.hh"
#include "nEXOUtilities/nEXOChannelMap.hh"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

ClassImp(nEXOCoordinateKey)

nEXOCoordinateKey::nEXOCoordinateKey(Int_t keyType)
: TObject(),
  fUorX(0),
  fVorY(0),
  fZ(0),
  fT(0),
  fKeyType(0), 
  fMCChargePixelSize(0.),
  fMCChargePixelTime(0.),
  fMCChargePixelZScaling(1.0),
  kTolerance(0.0001*CLHEP::mm)
{
  Clear();
  SetKeyType(keyType);
}

void nEXOCoordinateKey::SetKeyType(Int_t keyType)
{
  fKeyType = keyType;
  if(fKeyType==0) {
    fMCChargePixelSize = nEXOChannelMap::GetInstance()->GetMCCHARGE_PIXEL_SIZE(); // default value
    fMCChargePixelZScaling = (MCCHARGE_PIXEL_TIME*DRIFT_VELOCITY)/fMCChargePixelSize; 
  } else if(fKeyType==1)
    fMCChargePixelSize = nEXOChannelMap::GetInstance()->GetMCINDUCTION_PIXEL_SIZE(); // change pixel size for induction calculation
  else
    std::cout << "ERROR! Non-identified keyType!" << std::endl;

  fMCChargePixelTime = MCCHARGE_PIXEL_TIME; // default value
}

nEXOCoordinateKey::nEXOCoordinateKey(const nEXOCoordinateKey& Coord)
: TObject(Coord),
  fUorX(0),
  fVorY(0),
  fZ(0),
  fT(0),
  fKeyType(0), 
  fMCChargePixelSize(0.),
  fMCChargePixelTime(0.),
  kTolerance(0.0001*CLHEP::mm)
{
  Clear();
  *this = Coord;
}

nEXOCoordinateKey::nEXOCoordinateKey(Double_t UorX, Double_t VorY, Double_t Z, Double_t T, Int_t keyType)
: TObject(),
  kTolerance(0.0001*CLHEP::mm)
{
  // First argument should be either EXOMiscUtil::kUVCoordinates or EXOMiscUtil::kXYCoordinates.
  Clear();
  SetKeyType(keyType);
  SetCoordinates(UorX, VorY, Z, T);
}

nEXOCoordinateKey& nEXOCoordinateKey::operator = (const nEXOCoordinateKey& rhs)
{
  assert(rhs.CheckVersion());
  TObject::operator=(rhs);
  fUorX = rhs.fUorX;
  fVorY = rhs.fVorY;
  fZ = rhs.fZ;
  fT = rhs.fT;
  fMCChargePixelSize = rhs.fMCChargePixelSize;
  fMCChargePixelTime = rhs.fMCChargePixelTime;
  fKeyType = rhs.fKeyType;

  return *this;
}

void nEXOCoordinateKey::Clear(Option_t*)
{
  TObject::Clear();
  ResetBit(kIsInitialized);
}

void nEXOCoordinateKey::SetCoordinates(Double_t UorX, Double_t VorY, Double_t Z, Double_t T)
{
  // Converts coordinates into internally-stored pixel indices.
  // First argument should be either EXOMiscUtil::kUVCoordinates or EXOMiscUtil::kXYCoordinates.
  // Store the current pixel dimensions, to detect changes in future builds.
  //fMCChargePixelSize = nEXOChannelMap::GetInstance()->GetMCCHARGE_PIXEL_SIZE();
  //fMCChargePixelTime = MCCHARGE_PIXEL_TIME;

  SetBit(kIsInitialized);

  fUorX = (Int_t)( (UorX + VorY)*sqrt(2)/2/fMCChargePixelSize);
  if(UorX + VorY < 0) fUorX--;
  fVorY = (Int_t)( (VorY - UorX)*sqrt(2)/2/fMCChargePixelSize);
  if(VorY - UorX < 0) fVorY--;

  fZ = (Int_t)(Z/(fMCChargePixelSize*fMCChargePixelZScaling));
  if(Z < 0) fZ--;
  fT = (Int_t)(T/fMCChargePixelTime);
  if(T < 0) fT--;
}

Bool_t nEXOCoordinateKey::CheckVersion() const
{
  // Check that the pixel sizes defined in EXODimensions are the same ones that were used when this object was created.
  // This is useful for nEXOCoordinateKeys that are being read from a file.
  // The object is invalid if since initialization, the pixel size has been changed.
  // True means the object is valid.  We also returns true if it is uninitialized.


 
  if(not IsInitialized()) return true;
  if(fKeyType==0) {
    return ( fabs(fMCChargePixelSize - nEXOChannelMap::GetInstance()->GetMCCHARGE_PIXEL_SIZE()) < kTolerance and 
             fabs(fMCChargePixelTime - MCCHARGE_PIXEL_TIME) < kTolerance );
  }
  if(fKeyType==1) {
    return ( fabs(fMCChargePixelSize - nEXOChannelMap::GetInstance()->GetMCINDUCTION_PIXEL_SIZE()) < kTolerance and 
             fabs(fMCChargePixelTime - MCCHARGE_PIXEL_TIME) < kTolerance );
  }
  return false; 
}

Bool_t nEXOCoordinateKey::operator < (const nEXOCoordinateKey& rhs) const
{
  // Compare t; then z; then x or u; then v or y.
  // Two objects must be initialized and have the same coordinate system.
  assert(IsInitialized() and rhs.IsInitialized());
  assert(CheckVersion() and rhs.CheckVersion());

  if(fT != rhs.fT) return fT < rhs.fT;
  else if(fZ != rhs.fZ) return fZ < rhs.fZ;
  else if(fUorX != rhs.fUorX) return fUorX < rhs.fUorX;
  else return fVorY < rhs.fVorY;
}

Bool_t nEXOCoordinateKey::operator == (const nEXOCoordinateKey& rhs) const
{
  // Two objects must be initialized and have the same coordinate system.
  assert(IsInitialized() and rhs.IsInitialized());
  assert(CheckVersion() and rhs.CheckVersion());

  return ( (fUorX == rhs.fUorX) and (fVorY == rhs.fVorY) and (fZ == rhs.fZ) and (fT == rhs.fT) );
}

nEXOCoordinates nEXOCoordinateKey::GetCenter() const
{
  // Return the coordinates corresponding to the center of this pixel.
  // Only call on an initialized object.
  assert(IsInitialized());
  assert(CheckVersion());

  Double_t UorX = fMCChargePixelSize * (0.5 + (Double_t)fUorX);
  Double_t VorY = fMCChargePixelSize * (0.5 + (Double_t)fVorY);
  Double_t Z = fMCChargePixelSize * fMCChargePixelZScaling * (0.5 + (Double_t)fZ);
  Double_t T = fMCChargePixelTime * (0.5 + (Double_t)fT);

  nEXOCoordinates coord(UorX, VorY, Z, T);
  coord.RotateZDeg(45.);
  return coord;
}

Bool_t nEXOCoordinateKey::IsInitialized() const
{
  return TestBit(kIsInitialized);
}
