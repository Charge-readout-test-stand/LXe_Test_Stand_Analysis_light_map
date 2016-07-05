#ifndef nEXOChargeCluster_hh
#define nEXOChargeCluster_hh

#include "nEXOUtilities/nEXOChargeElecChannel.hh"
#include "nEXOUtilities/nEXOTClonesArray.hh"

#ifndef ROOT_TRef
#include "TRef.h"
#endif

#ifndef nEXOTRefArray_hh
#include "nEXOUtilities/nEXOTRefArray.hh"
#endif

#ifndef nEXOScintillationCluster_hh
#include "nEXOUtilities/nEXOScintillationCluster.hh"
#endif

/*
 * nEXOChargeCluster is a collection of X-, Y- stripe channels
 * that coincident in time and space
*/

#ifndef ROOT_TObject
#include "TObject.h"
#endif
#include <map>
#include <cstddef> //for size_t
using namespace std;

class nEXOChargeCluster : public TObject
{
  public:

    // Constructors
    nEXOChargeCluster();
    nEXOChargeCluster(const nEXOChargeCluster& cc);

    // Modifiers
    void Clear(Option_t* opt = "");
    //nEXOChargeCluster& operator = (const nEXOChargeCluster& cc);

    const nEXOChargeElecChannel* FindChargeChannel(Int_t chanId) const;
    //const nEXOChargeElecChannel* FindOrAddChargeChannel(Int_t chanId) const;
    //nEXOChargeElecChannel* FindOrAddChargeChannel(Int_t chanId);

    // Get/Set associated scintillation clusters and wire signals
    void SetScintillationCluster( nEXOScintillationCluster* aValue );
    nEXOScintillationCluster*  GetScintillationCluster();
    const nEXOScintillationCluster*  GetScintillationCluster() const;
    bool HaveScintillationCluster() const;

    void InsertXChannel( nEXOChargeElecChannel* aValue );
    void InsertYChannel( nEXOChargeElecChannel* aValue );
    void InsertInductionChannel( nEXOChargeElecChannel* aValue );
    nEXOChargeElecChannel*  GetXChargeSignalAt( size_t index );
    nEXOChargeElecChannel*  GetYChargeSignalAt( size_t index );
    nEXOChargeElecChannel*  GetInductionSignalAt( size_t index );
    const nEXOChargeElecChannel*  GetXChargeSignalAt( size_t index ) const;
    const nEXOChargeElecChannel*  GetYChargeSignalAt( size_t index ) const;
    const nEXOChargeElecChannel*  GetInductionSignalAt( size_t index ) const;
    Int_t GetXChargeChannelAt( size_t index ) const; 
    Int_t GetYChargeChannelAt( size_t index ) const; // channel number of [index] Y- collection signal.
    Int_t GetInductionChannelAt( size_t index ) const; 

    size_t GetNumXChannels() const;
    size_t GetNumYChannels() const;
    size_t GetNumInductionChannels() const;

	 double fT_cluster;
	 double fTRMS_cluster;
	 double fX_cluster;
	 double fXRMS_cluster;
	 double fY_cluster;
	 double fYRMS_cluster;
	 double fCharge_cluster;
	 double fChannelsnum_cluster;
	 char fChanneltype_cluster;




    double fT;
    double fTRMS;
    double fX;
    double fXRMS;
    double fY;
    double fYRMS;
    double fZ;
    double fZRMS;
    double fEnergy;
    double fEnergyError;
	 double fCharge;
	 int fChannelsnum;
	 char fChanneltype;

  protected:
    std::map<Int_t, Int_t> fClusterMap;

    TRef              fRefScintCluster;
    nEXOTRefArray      fRefXChannels;
    nEXOTRefArray      fRefYChannels;
    nEXOTRefArray      fRefInductionChannels;

  ClassDef(nEXOChargeCluster,1) // Increment this number whenever the data members of nEXOChargeCluster are changed!

};

inline size_t nEXOChargeCluster::GetNumXChannels() const
{
  return fRefXChannels.GetLast() + 1;
}

inline size_t nEXOChargeCluster::GetNumYChannels() const
{
  return fRefYChannels.GetLast() + 1;
}

inline size_t nEXOChargeCluster::GetNumInductionChannels() const
{
  return fRefInductionChannels.GetLast() + 1;
}

inline bool nEXOChargeCluster::HaveScintillationCluster() const
{
  // do we have scintillation cluster assotiated to this charge cluster
  return (fRefScintCluster.GetObject());
}

inline void nEXOChargeCluster::InsertXChannel( nEXOChargeElecChannel* aValue )
{
  // Add a x charge signal that this charge cluster is made of
  if(fRefXChannels.IsEmpty()) fRefXChannels = nEXOTRefArray(TProcessID::GetProcessWithUID(aValue));
  fRefXChannels.Add(aValue);
}

inline void nEXOChargeCluster::InsertYChannel( nEXOChargeElecChannel* aValue )
{
  // Add a y charge signal that this charge cluster is made of
  if(fRefYChannels.IsEmpty()) fRefYChannels = nEXOTRefArray(TProcessID::GetProcessWithUID(aValue));
  fRefYChannels.Add(aValue);
}

inline void nEXOChargeCluster::InsertInductionChannel( nEXOChargeElecChannel* aValue )
{
  // Add a x charge signal that this charge cluster is made of
  if(fRefInductionChannels.IsEmpty()) fRefInductionChannels = nEXOTRefArray(TProcessID::GetProcessWithUID(aValue));
  fRefInductionChannels.Add(aValue);
}

inline nEXOChargeElecChannel* nEXOChargeCluster::GetXChargeSignalAt( size_t index )
{
  // Get an associated x charge signal
  return static_cast<nEXOChargeElecChannel*>(fRefXChannels.At(index));
}

inline nEXOChargeElecChannel* nEXOChargeCluster::GetYChargeSignalAt( size_t index )
{
  // Get an associated y charge signal
  return static_cast<nEXOChargeElecChannel*>(fRefYChannels.At(index));
}

inline nEXOChargeElecChannel* nEXOChargeCluster::GetInductionSignalAt( size_t index )
{
  // Get an associated y charge signal
  return static_cast<nEXOChargeElecChannel*>(fRefInductionChannels.At(index));
}

inline const nEXOChargeElecChannel* nEXOChargeCluster::GetXChargeSignalAt( size_t index ) const
{
  // Get an associated x charge signal
  return static_cast<nEXOChargeElecChannel*>(fRefXChannels.At(index));
}

inline const nEXOChargeElecChannel* nEXOChargeCluster::GetYChargeSignalAt( size_t index ) const
{
  // Get an associated y charge signal
  return static_cast<nEXOChargeElecChannel*>(fRefYChannels.At(index));
}

inline const nEXOChargeElecChannel* nEXOChargeCluster::GetInductionSignalAt( size_t index ) const
{
  // Get an associated y charge signal
  return static_cast<nEXOChargeElecChannel*>(fRefInductionChannels.At(index));
}

inline Int_t nEXOChargeCluster::GetXChargeChannelAt( size_t index ) const
{
  // Get the (physical) channel number of the associated x charge signal
  return static_cast<nEXOChargeElecChannel*>(fRefXChannels.At(index))->fChannelId;
}

inline Int_t nEXOChargeCluster::GetYChargeChannelAt( size_t index ) const
{
  // Get the (physical) channel number of the associated y charge signal
  return static_cast<nEXOChargeElecChannel*>(fRefYChannels.At(index))->fChannelId;
}

inline Int_t nEXOChargeCluster::GetInductionChannelAt( size_t index ) const
{
  // Get the (physical) channel number of the associated y charge signal
  return static_cast<nEXOChargeElecChannel*>(fRefInductionChannels.At(index))->fChannelId;
}
#endif /* nEXOChargeCluster_hh */
