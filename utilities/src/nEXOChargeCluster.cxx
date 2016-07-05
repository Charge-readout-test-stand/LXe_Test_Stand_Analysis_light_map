#include "nEXOUtilities/nEXOChargeCluster.hh"
#include "nEXOUtilities/nEXOScintillationCluster.hh"
#include "nEXOUtilities/nEXOTreeArrayLengths.hh"

#include <iostream>
using namespace std;

ClassImp(nEXOChargeCluster)

nEXOChargeCluster::nEXOChargeCluster()
: TObject()
{
  nEXOChargeCluster::Clear();
}

nEXOChargeCluster::nEXOChargeCluster(const nEXOChargeCluster& cluster) : TObject(cluster)
{
  nEXOChargeCluster::Clear();
  *this = cluster;
}

void nEXOChargeCluster::Clear( Option_t* opt )
{     // Clear charge info, reset to initial state
  fT = 0.0;
  fX = 0.0;
  fY = 0.0; 
  fZ = 0.0;
  fEnergy = 0.0;
  fTRMS = 0.0;
  fXRMS = 0.0;
  fYRMS = 0.0;
  fZRMS = 0.0;
  fEnergyError = 0.0;
  fCharge = 0.0;
  fChannelsnum = 0;
  fChanneltype ='N';
  fRefScintCluster             = NULL;
  fRefXChannels.Clear();
  fRefYChannels.Clear();
  fRefInductionChannels.Clear();
}

const nEXOChargeElecChannel* nEXOChargeCluster::FindChargeChannel(Int_t chanId) const
{
  if (GetNumXChannels()!=0) { 
    return NULL;
  }
  else if(GetNumYChannels()!=0)  {
    return NULL;
  }
  else {
    // Returns NULL if no key is found.
    return NULL;
  }
}

nEXOScintillationCluster*  nEXOChargeCluster::GetScintillationCluster()
{
  // Get associated scintillation cluster
  return static_cast< nEXOScintillationCluster* >(fRefScintCluster.GetObject() );
}

const nEXOScintillationCluster*  nEXOChargeCluster::GetScintillationCluster() const
{
  // Get associated scintillation cluster
  return static_cast< const nEXOScintillationCluster* >(fRefScintCluster.GetObject() );
}

void nEXOChargeCluster::SetScintillationCluster( nEXOScintillationCluster* aValue )
{
  // Set associated charge cluster
  fRefScintCluster = aValue;
}
