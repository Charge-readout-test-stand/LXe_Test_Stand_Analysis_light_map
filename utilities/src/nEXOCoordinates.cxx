#include "nEXOUtilities/nEXOCoordinates.hh"

#include <cassert>
#include <cmath>
#include "TMath.h"

ClassImp(nEXOCoordinates)

nEXOCoordinates::nEXOCoordinates() : TObject()
{
  Clear();
}

nEXOCoordinates::nEXOCoordinates(const nEXOCoordinates& Coord)
: TObject(Coord)
{
  Clear();
  *this = Coord;
}
nEXOCoordinates::nEXOCoordinates(Double_t UorX, Double_t VorY, Double_t Z, Double_t T)
: TObject()
{
  Clear();
  SetCoordinates(UorX, VorY, Z, T);
}

void nEXOCoordinates::Clear(Option_t*)
{
  TObject::Clear();
  ResetBit(kIsInitialized);
}

void nEXOCoordinates::ChangeX(Double_t X)
{
  // Only call on an initialized object; we do not permit partially-filled coordinates.
  assert(IsInitialized());
  fX = X;
}

void nEXOCoordinates::SetCoordinates(Double_t UorX, Double_t VorY, Double_t Z, Double_t T)
{
  // First argument should be either EXOMiscUtil::kUVCoordinates or EXOMiscUtil::kXYCoordinates.
  SetBit(kIsInitialized);
  fZ = Z;
  fT = T;
  fX = UorX;
  fY = VorY;
}

void nEXOCoordinates::ChangeY(Double_t Y)
{
  // Only call on an initialized object; we do not permit partially-filled coordinates.
  assert(IsInitialized());
  fY = Y;
}

void nEXOCoordinates::ChangeZ(Double_t Z)
{
  // Note this function can change U and V if we're changing halves of the TPC.
  // Only call on an initialized object; we do not permit partially-filled coordinates.
  assert(IsInitialized());
  fZ = Z;
}

void nEXOCoordinates::ChangeT(Double_t T)
{
  // Only call on an initialized object; we do not permit partially-filled coordinates.
  assert(IsInitialized());
  fT = T;
}

void nEXOCoordinates::RotateZDeg(Double_t degrees)
{
  //Rotates the coordinates around the Z-axis.
  // Only call on an initialized object; we do not permit partially-filled coordinates.
  RotateZRad(degrees / 180. * TMath::Pi());
}

void nEXOCoordinates::RotateZRad(Double_t radians)
{
  //Rotates the coordinates around the Z-axis.
  // Only call on an initialized object; we do not permit partially-filled coordinates.
  assert(IsInitialized());
  double cosA = cos(radians);
  double sinA = sin(radians);
  double newX = cosA*fX - sinA*fY;
  fY = sinA*fX + cosA*fY;
  fX = newX;
}

Double_t nEXOCoordinates::GetX() const
{
  // Only call on an initialized object.
  assert(IsInitialized());
  return fX;
}

Double_t nEXOCoordinates::GetY() const
{
  // Only call on an initialized object.
  assert(IsInitialized());
  return fY;
}

Double_t nEXOCoordinates::GetZ() const
{
  // Only call on an initialized object.
  assert(IsInitialized());
  return fZ;
}

Double_t nEXOCoordinates::GetT() const
{
  // Only call on an initialized object.
  assert(IsInitialized());
  return fT;
}

nEXOCoordinateKey nEXOCoordinates::GetCoordinateKey(Int_t keyType) const
{
  // Return an nEXOCoordinateKey (pixel) containing this position/time.
  // Pixel will be fixed in the coordinate system given by UVorXY.
  // Only call on an initialized object.
  assert(IsInitialized());

  return nEXOCoordinateKey(fX, fY, fZ, fT, keyType);
}

Double_t Distance(const nEXOCoordinates& Coord1, const nEXOCoordinates& Coord2)
{
  // Returns the distance between the spatial coordinates of the two objects.
  // The two coordinates must both be initialized.
  // The function is a friend of nEXOCoordinates.
  assert(Coord1.IsInitialized() and Coord2.IsInitialized());

  Double_t SqX = (Coord1.fX - Coord2.fX)*(Coord1.fX - Coord2.fX);
  Double_t SqY = (Coord1.fY - Coord2.fY)*(Coord1.fY - Coord2.fY);
  Double_t SqZ = (Coord1.fZ - Coord2.fZ)*(Coord1.fZ - Coord2.fZ);

  return sqrt(SqX + SqY + SqZ);
}

Double_t nEXOCoordinates::PolarRadius() const
{
  // Gives the polar radius in the UV/XY plane; ie. returns sqrt(x^2+y^2).
  // The object should be initialized.
  assert(IsInitialized());

  return sqrt(fX*fX + fY*fY);
}

Double_t nEXOCoordinates::PolarAngleDeg() const
{
  //Return the polar angle in the XY plane in degrees. Zero degree is along X-axis.
  //The object should be initialized.

  return 180. / TMath::Pi() * PolarAngleRad();
}

Double_t nEXOCoordinates::PolarAngleRad() const
{
  //Return the polar angle in the XY plane in radians. Zero rad is along X-axis.
  //The object should be initialized.

  double angle = atan2(fY,fX);
  if(fY < 0.0){
    angle += TMath::TwoPi();
  }
  return angle;
}
