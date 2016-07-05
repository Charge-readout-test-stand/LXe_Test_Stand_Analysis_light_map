//______________________________________________________________________________
//                                                                        
// nEXOScintillationCluster 
//

#include "nEXOUtilities/nEXOScintillationCluster.hh"
#include "nEXOUtilities/nEXOChargeCluster.hh"
#include "TROOT.h"
#include "TMath.h"
#include <string>
#include <iostream>
#include <cmath>

ClassImp( nEXOScintillationCluster )

//______________________________________________________________________________
void nEXOScintillationCluster::Clear( Option_t* ) 
{ 
  // Clear event, reset to initial state
  fX = 0.0; 
  fY = 0.0; 
  fZ = 0.0; 
  fTime = 0.0; 
  fRefChargeClusters.Clear();
  fRawEnergy = 0.0; 
  fEnergy = 0.0;
  fEnergyError = 0.0;
}

//______________________________________________________________________________
nEXOScintillationCluster::nEXOScintillationCluster() : 
  TObject()
{ 
  nEXOScintillationCluster::Clear(); 
  gROOT->GetListOfCleanups()->Add(this);
}

//______________________________________________________________________________
nEXOScintillationCluster::~nEXOScintillationCluster() 
{
  gROOT->GetListOfCleanups()->Remove(this);
}
//______________________________________________________________________________
void nEXOScintillationCluster::InsertChargeCluster( nEXOChargeCluster* obj )
{
  // Insert an associated charge cluster into this object.
  if(fRefChargeClusters.IsEmpty()) fRefChargeClusters = nEXOTRefArray(TProcessID::GetProcessWithUID(obj)); 
  fRefChargeClusters.Add(obj);
}
//______________________________________________________________________________
nEXOChargeCluster* nEXOScintillationCluster::GetChargeClusterAt(size_t index)
{
  // Get an associated charge cluster from this object. 
  return static_cast<nEXOChargeCluster*>(fRefChargeClusters.At(index));
}
//______________________________________________________________________________
const nEXOChargeCluster* nEXOScintillationCluster::GetChargeClusterAt(size_t index) const
{
  // Get an associated charge cluster from this object. 
  return static_cast<const nEXOChargeCluster*>(fRefChargeClusters.At(index));
}
//______________________________________________________________________________
double nEXOScintillationCluster::GetEnergy() const { return fEnergy; }
double nEXOScintillationCluster::GetEnergyError() const { return 0; }

nEXOScintillationCluster::nEXOScintillationCluster(const nEXOScintillationCluster& other) : TObject(other)
{
  fX = other.fX; 
  fY = other.fY; 
  fZ = other.fZ; 
  fTime = other.fTime; 
  fRefChargeClusters.Clear();
  fRawEnergy = other.fRawEnergy; 
  fEnergy = other.fEnergy; 
}
//______________________________________________________________________________
nEXOScintillationCluster& nEXOScintillationCluster::operator=(const nEXOScintillationCluster& other)
{
  if (this == &other) return *this;
  fX = other.fX; 
  fY = other.fY; 
  fZ = other.fZ; 
  fTime = other.fTime; 
  fRefChargeClusters.Clear();
  fRawEnergy = other.fRawEnergy; 
  fEnergy = other.fEnergy; 
  return *this;
}
//______________________________________________________________________________
//bool nEXOScintillationCluster::operator==( const nEXOScintillationCluster& other ) const
//{
//  // Deeply compare two scintillation clusters.
//  bool retVal = fX == other.fX and 
//          fY == other.fY and 
//          fZ == other.fZ and 
//          fTime == other.fTime and 
//          fRawEnergy == other.fRawEnergy and 
//          fEnergy == other.fEnergy and 
//          GetNumChargeClusters() == other.GetNumChargeClusters();
//  if (not retVal) return retVal;
//
//  // Else check the rest, this is very deep, we try to compare all the referenced
//  // clusters. 
//  for (size_t i=0;i<GetNumChargeClusters();i++) {
//      if (not (*GetChargeClusterAt(i) == *other.GetChargeClusterAt(i))) return false; 
//  }
//  return true;
//}
