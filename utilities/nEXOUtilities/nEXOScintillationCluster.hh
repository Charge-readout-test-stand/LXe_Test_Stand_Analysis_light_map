#ifndef nEXOScintillationCluster_hh
#define nEXOScintillationCluster_hh

#ifndef ROOT_TObject
#include "TObject.h"
#endif
#ifndef nEXOTClonesArray_hh
#include "nEXOUtilities/nEXOTClonesArray.hh"
#endif
#ifndef nEXOTRefArray_hh
#include "nEXOUtilities/nEXOTRefArray.hh"
#endif
class nEXOChargeCluster;
#include <cstddef> //for size_t

class nEXOScintillationCluster : public TObject {
  public:

    Double_t       fX;
    Double_t       fY;
    Double_t       fZ;
    Double_t       fTime;
    Double_t       fEnergy;
    Double_t       fEnergyError;
    Double_t       fRawEnergy;

    double GetEnergy() const;
    double GetEnergyError() const;

    void InsertChargeCluster( nEXOChargeCluster* obj );
    nEXOChargeCluster* GetChargeClusterAt(size_t index);
    const nEXOChargeCluster* GetChargeClusterAt(size_t index) const;
    size_t GetNumChargeClusters() const;

  public:
    nEXOScintillationCluster();
    nEXOScintillationCluster(const nEXOScintillationCluster&);
    nEXOScintillationCluster& operator=(const nEXOScintillationCluster&);
    virtual ~nEXOScintillationCluster();
    virtual void Clear( Option_t* opt = "" ); 
    //virtual void Print( Option_t* opt = "" ) const;

    //bool operator==(const nEXOScintillationCluster& sc) const;
    
  protected:

    nEXOTRefArray  fRefChargeClusters;    //scint_ichargecluster
//
    ClassDef( nEXOScintillationCluster, 1 )
}; 

//---- inlines -----------------------------------------------------------------

inline size_t nEXOScintillationCluster::GetNumChargeClusters() const 
{ 
  // Get number of associated charge clusters.
  return fRefChargeClusters.GetLast() + 1; 
}

#endif /* nEXOScintillationCluster_hh */
