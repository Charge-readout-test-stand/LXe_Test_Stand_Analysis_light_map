#ifndef nEXOEventData_hh
#define nEXOEventData_hh

#ifndef nEXOTClonesArray_hh
#include "nEXOUtilities/nEXOTClonesArray.hh"
#endif

#ifndef nEXOChargeReadout_hh
#include "nEXOUtilities/nEXOChargeReadout.hh"
#endif

#ifndef nEXOChargeCluster_hh
#include "nEXOUtilities/nEXOChargeCluster.hh"
#endif

class nEXOEventData : public TObject
{

  public:
//    nEXOMonteCarloData    fMonteCarloData;   // MC data.
    Int_t     fEventNumber; 

  public:

    // Default constructor
    nEXOEventData(); 
    nEXOEventData( const nEXOEventData& );
    nEXOEventData& operator=( const nEXOEventData& ); 
    virtual ~nEXOEventData(); 

    //*********************************************
    const nEXOTClonesArray* GetChargeReadoutArray() const;
    nEXOChargeReadout* GetChargeReadout( size_t i ); 
    nEXOChargeReadout* GetNewChargeReadout(); 
    size_t GetNumChargeReadouts() const; 
    //*********************************************
    
    //*********************************************
    const nEXOTClonesArray* GetChargeClusterArray() const;
    nEXOChargeCluster* GetChargeCluster( size_t i ); 
    nEXOChargeCluster* GetNewChargeCluster(); 
    size_t GetNumChargeClusters() const; 
    //*********************************************
    //
//    void ResetForDigitization(); 
    virtual void Clear( Option_t* option = "");
    
  protected:
    void InitializeArrays();
    void InitializeChargeReadouts();
    void InitializeChargeClusters();
    nEXOTClonesArray* GetChargeReadoutArray();
    nEXOTClonesArray* GetChargeClusterArray();

    // Digitized charge readout info
    TClonesArray*     fChargeReadouts;

    // reconstructed charge cluster info
    TClonesArray*     fChargeClusters;

  ClassDef(nEXOEventData,1);

};

inline nEXOTClonesArray* nEXOEventData::GetChargeReadoutArray()
{
  return static_cast< nEXOTClonesArray* >( fChargeReadouts );
}

inline const nEXOTClonesArray* nEXOEventData::GetChargeReadoutArray() const
{ 
  return static_cast< const nEXOTClonesArray* >( fChargeReadouts ); 
}

inline size_t nEXOEventData::GetNumChargeReadouts() const
{
  return ( fChargeReadouts) ? fChargeReadouts->GetEntriesFast() : 0;
}

inline nEXOChargeReadout* nEXOEventData::GetChargeReadout( size_t i )
{
  return static_cast< nEXOChargeReadout* >( GetChargeReadoutArray()->At(i) );
}

inline nEXOTClonesArray* nEXOEventData::GetChargeClusterArray()
{
  return static_cast< nEXOTClonesArray* >( fChargeClusters );
}

inline const nEXOTClonesArray* nEXOEventData::GetChargeClusterArray() const
{ 
  return static_cast< const nEXOTClonesArray* >( fChargeClusters ); 
}

inline size_t nEXOEventData::GetNumChargeClusters() const
{
  return ( fChargeClusters) ? fChargeClusters->GetEntriesFast() : 0;
}

inline nEXOChargeCluster* nEXOEventData::GetChargeCluster( size_t i )
{
  return static_cast< nEXOChargeCluster* >( GetChargeClusterArray()->At(i) );
}

inline nEXOChargeCluster* nEXOEventData::GetNewChargeCluster()
{
  // Get a new reconstructed charge cluster.  This is called when a new charge
  // cluster is to be loaded into EXOEventData.  For example, a user would do:
  return static_cast<nEXOChargeCluster*>(
        GetChargeClusterArray()->GetNewOrCleanedObject( GetNumChargeClusters() ) );
}

inline nEXOChargeReadout* nEXOEventData::GetNewChargeReadout()
{
  // Get a new reconstructed charge cluster.  This is called when a new charge
  // cluster is to be loaded into EXOEventData.  For example, a user would do:
  return static_cast<nEXOChargeReadout*>(
        GetChargeReadoutArray()->GetNewOrCleanedObject( GetNumChargeReadouts() ) );
}

//inline void EXOEventData::ResetForDigitization()
//{
//  GetChargeReadoutArray()->Delete();
//}
#endif

