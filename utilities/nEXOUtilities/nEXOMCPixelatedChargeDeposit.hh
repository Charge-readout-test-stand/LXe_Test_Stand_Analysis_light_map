#ifndef nEXOMCPixelatedChargeDeposit_hh
#define nEXOMCPixelatedChargeDeposit_hh
#include <set>
#ifndef ROOT_TObject
#include "TObject.h"
#endif
#ifndef nEXOCoordinateKey_hh
#include "nEXOUtilities/nEXOCoordinateKey.hh"
#endif

class nEXOCoordinates;

class nEXOMCPixelatedChargeDeposit : public TObject 
{
  // Class to encapsulate all EXO Monte Carlo Pixelated Charge Deposit Info  
  public:
    Double_t       fTotalEnergy;           //etq : Charge pixel total energy in CLHEP units (MeV = 1).
    Double_t       fTotalIonizationEnergy; //eiq : Charge pixel ionization energy in CLHEP units (MeV = 1), accounting for quenching in alpha events and smearing due to LXe resolution.
    Double_t       fWireHitTime;  // Time at which the charge was deposited on the U wire
    Double_t       fWeight;       // Weight of a hit.  In principal, all of these must be equal for an event. 

    Int_t fDepositChannel;  // Software U-channel on which the ionized charge deposited. (If it deposited on a V-channel, then we set this value to +999 since there is no direct correspondence between drifting and real geometry.  If no wire is hit, the value is -999.)
    std::set<Int_t> fWireChannelsAffected;   // Set containing all (software) channel numbers of the wires (U and V) whose waveforms were influenced by this charge deposit.
    Double_t       fTotalTE;

  public:
    nEXOMCPixelatedChargeDeposit() : TObject() { nEXOMCPixelatedChargeDeposit::Clear(); }
    nEXOMCPixelatedChargeDeposit(const nEXOCoordinates& Coord, Int_t keyType=0);
    nEXOMCPixelatedChargeDeposit(const nEXOCoordinateKey& CoordKey);
    virtual void Clear( Option_t* = "" );

    void SetCoordinates(const nEXOCoordinates& Coord, Int_t keyType=0);
    nEXOCoordinates GetPixelCenter() const;
    const nEXOCoordinateKey& GetPixelCoordinateKey() const;

  protected:
    nEXOCoordinateKey fCoordinateKey;

  ClassDef( nEXOMCPixelatedChargeDeposit, 1 )
};

#endif /* nEXOMCPixelatedChargeDeposit_hh */
