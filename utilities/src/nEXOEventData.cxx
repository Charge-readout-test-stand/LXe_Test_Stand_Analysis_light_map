#include "nEXOUtilities/nEXOEventData.hh"
#include "nEXOUtilities/nEXOTreeArrayLengths.hh"

ClassImp( nEXOEventData )

nEXOEventData::nEXOEventData() : 
  TObject(),
  fChargeReadouts(0),
  fChargeClusters(0)
{
  // Default constructor, clear and setup arrays
  nEXOEventData::Clear();
  InitializeArrays();
}

void nEXOEventData::InitializeArrays()
{
  InitializeChargeReadouts();
  InitializeChargeClusters();
}

void nEXOEventData::InitializeChargeReadouts()
{
  fChargeReadouts = 
    new nEXOTClonesArray( nEXOChargeReadout::Class(), MAXCOLL_TOT ); 
}

void nEXOEventData::InitializeChargeClusters()
{
  fChargeClusters = 
    new nEXOTClonesArray( nEXOChargeCluster::Class(), MAXCOLL_TOT ); 
}

nEXOEventData::~nEXOEventData()
{
  delete fChargeReadouts;
}

void nEXOEventData::Clear( Option_t* /*option*/ )
{
  fEventNumber                = 0;

//  fMonteCarloData.Clear( "C" );

  if ( fChargeReadouts) fChargeReadouts->Clear( "C" );
}

nEXOEventData::nEXOEventData(const nEXOEventData& other) : TObject(other) 
{
  // Copy constructor
  fEventNumber = other.fEventNumber;

//  fMonteCarloData = other.fMonteCarloData;
  fChargeReadouts = new nEXOTClonesArray(*other.fChargeReadouts);
}

nEXOEventData& nEXOEventData::operator=(const nEXOEventData& other)
{
  // Copy constructor
  if (this == &other) return *this;

  fEventNumber = other.fEventNumber;

//  fMonteCarloData = other.fMonteCarloData;
  *static_cast<nEXOTClonesArray*>(fChargeReadouts) = *other.fChargeReadouts;

  return *this;
}
