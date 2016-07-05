#ifndef nEXOCoordinates_hh
#define nEXOCoordinates_hh
#ifndef ROOT_TObject
#include "TObject.h"
#endif

#ifndef nEXOCoordinateKey_hh
#include "nEXOUtilities/nEXOCoordinateKey.hh"
#endif

class nEXOCoordinateKey;

class nEXOCoordinates : public TObject
{

    enum ECoordinateStatusBits{
      kIsInitialized = BIT(14) // Stored in TObject; set if coordinates have been initialized.
    };

  public:


    // Constructors
    nEXOCoordinates();
    nEXOCoordinates(const nEXOCoordinates& Coord);
    nEXOCoordinates(Double_t UorX, Double_t VorY, Double_t Z, Double_t T);

    // Accessors
    Double_t GetX() const;
    Double_t GetY() const;
    Double_t GetZ() const;
    Double_t GetT() const;
    nEXOCoordinateKey GetCoordinateKey(Int_t keyType=0) const;
    Bool_t IsInitialized() const {return TestBit(kIsInitialized);}

    // Modifiers
    void Clear(Option_t* opt = "");
    void SetCoordinates(Double_t UorX, Double_t VorY, Double_t Z, Double_t T);
    void ChangeX(Double_t X);
    void ChangeY(Double_t Y);
    void ChangeZ(Double_t Z);
    void ChangeT(Double_t T);
    void RotateZDeg(Double_t degrees);
    void RotateZRad(Double_t radians);

    // Convenience functions
    friend Double_t Distance(const nEXOCoordinates& Coord1, const nEXOCoordinates& Coord2);
    Double_t PolarRadius() const;
    Double_t PolarAngleDeg() const;
    Double_t PolarAngleRad() const;

  protected:

    Double_t fX;
    Double_t fY;
    Double_t fZ;
    Double_t fT;

  ClassDef(nEXOCoordinates,1) // Increment this number whenever the data members of nEXOCoordinates are changed!

};

#endif
