//______________________________________________________________________________
//                                                                        
// nEXOMCPixelatedChargeDeposit 
//
#include "nEXOUtilities/nEXOMCPixelatedChargeDeposit.hh"
#include <cassert>

ClassImp( nEXOMCPixelatedChargeDeposit )

nEXOMCPixelatedChargeDeposit::nEXOMCPixelatedChargeDeposit(const nEXOCoordinates& Coord, Int_t keyType)
: TObject()
{
  nEXOMCPixelatedChargeDeposit::Clear();
  fCoordinateKey = Coord.GetCoordinateKey(keyType);
}

nEXOMCPixelatedChargeDeposit::nEXOMCPixelatedChargeDeposit(const nEXOCoordinateKey& CoordKey)
: TObject()
{
  nEXOMCPixelatedChargeDeposit::Clear();
  fCoordinateKey = CoordKey;
}

void nEXOMCPixelatedChargeDeposit::Clear( Option_t* )
{
  // Initialize the contents of the charge deposit to show no hits have yet been registered to this object.
  fTotalEnergy = 0;
  fTotalIonizationEnergy = 0;
  fWireHitTime = 0;
  fDepositChannel = -999;
  fWeight = 1.0;
  fWireChannelsAffected.clear();
  fCoordinateKey.Clear();
  fTotalTE= 0;
}

void nEXOMCPixelatedChargeDeposit::SetCoordinates(const nEXOCoordinates& Coord, Int_t keyType)
{
  // Set the coordinates of a pixel created using the default constructor.
  // WARNING:  This function must not be called if the coordinates have already been set.
  // If any of the variables in this class were set before a coordinate was defined, that was bad;
  // they will be cleared.
  assert(!fCoordinateKey.IsInitialized());

  Clear();
  fCoordinateKey = Coord.GetCoordinateKey(keyType);
} 

nEXOCoordinates nEXOMCPixelatedChargeDeposit::GetPixelCenter() const
{
  // Return the center of the pixel.
  // Use GetPixelCoordinateKey to access the raw pixel information.
  assert(fCoordinateKey.IsInitialized() and fCoordinateKey.CheckVersion());
  return fCoordinateKey.GetCenter();
}

const nEXOCoordinateKey& nEXOMCPixelatedChargeDeposit::GetPixelCoordinateKey() const
{
  // Return the nEXOCoordinateKey of the pixel.
  // Generally it will be more convenient to use GetPixelCenter,
  // which gives the coordinates corresponding to the center of the pixel.
  // Note there is no guarantee that the nEXOCoordinateKey has been initialized.
  assert(fCoordinateKey.CheckVersion());
  return fCoordinateKey;
}
