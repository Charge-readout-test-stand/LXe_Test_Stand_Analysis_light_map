#ifndef nEXOCoordinateKey_hh
#define nEXOCoordinateKey_hh

#ifndef ROOT_TObject
#include "TObject.h"
#endif
#ifndef nEXOCoordinates_hh
#include "nEXOUtilities/nEXOCoordinates.hh"
#endif
#include "nEXOUtilities/nEXODimensions.hh"

class nEXOCoordinates;

class nEXOCoordinateKey : public TObject
{

    enum ECoordinateStatusBits{
      kIsInitialized = BIT(14) // Stored in TObject; set if coordinates have been initialized.
    };

  public:

    // Constructors
    nEXOCoordinateKey(Int_t keyType=0);
    nEXOCoordinateKey(const nEXOCoordinateKey& Coord);
    nEXOCoordinateKey(Double_t X, Double_t Y, Double_t Z, Double_t T, Int_t keyType=0);
    void SetKeyType(Int_t keyType);

    // Modifiers
    void Clear(Option_t* opt = "");
    void SetCoordinates(Double_t UorX, Double_t VorY, Double_t Z, Double_t T);
    nEXOCoordinateKey& operator = (const nEXOCoordinateKey& rhs);

    // Pixel size information.
    Double_t GetPixellationSize() {return fMCChargePixelSize;}
    Double_t GetPixellationSizeZScaling() {return fMCChargePixelZScaling;}
    Double_t GetPixellationTime() {return fMCChargePixelTime;}
    Bool_t CheckVersion() const;

    // comparison
    Bool_t operator < (const nEXOCoordinateKey& rhs) const;
    Bool_t operator == (const nEXOCoordinateKey& rhs) const;
    Bool_t operator != (const nEXOCoordinateKey& rhs) const {return not (*this == rhs);}
    Bool_t operator > (const nEXOCoordinateKey& rhs) const {return (rhs < *this);}
    Bool_t operator <= (const nEXOCoordinateKey& rhs) const {return not (*this > rhs);}
    Bool_t operator >= (const nEXOCoordinateKey& rhs) const {return (rhs <= *this);}

    // Accessors
    nEXOCoordinates GetCenter() const;
    Bool_t IsInitialized() const;

  protected:

    Int_t fUorX;
    Int_t fVorY;
    Int_t fZ;
    Int_t fT;
    Int_t fKeyType; // 0: for charge collection simulation
                    // 1: for induction simulation
                    // the PCD size could be different for different keyTypes
    Double_t fMCChargePixelSize;
    Double_t fMCChargePixelTime;
    Double_t fMCChargePixelZScaling; // z (drift) coordinate needs finer binning for Stanford setup
    const Double_t kTolerance;

  ClassDef(nEXOCoordinateKey,1) // Increment this number whenever the data members of nEXOCoordinateKey are changed!

};

#endif
