#ifndef nEXOChargeReadoutDigitize_hh
#define nEXOChargeReadoutDigitize_hh
#include <set>
#ifndef ROOT_TObject
#include "TObject.h"
#endif
#include "TRandom3.h"
#include "TStopwatch.h"

#ifndef nEXOMCPixelatedChargeDeposit_hh 
#include "nEXOUtilities/nEXOMCPixelatedChargeDeposit.hh"
#endif

#ifndef nEXOCoordinateKey_hh 
#include "nEXOUtilities/nEXOCoordinateKey.hh"
#endif

#ifndef nEXOChargeElecChannel_hh
#include "nEXOUtilities/nEXOChargeElecChannel.hh"
#endif

#ifndef nEXOChargeReadout_hh
#include "nEXOUtilities/nEXOChargeReadout.hh"
#endif

#ifndef nEXOChannelMap_hh
#include "nEXOUtilities/nEXOChannelMap.hh"
#endif

#ifndef nEXOEventData_hh
#include "nEXOUtilities/nEXOEventData.hh"
#endif

#include <queue>
#include <vector>
#include <map>

const int WFLENGTH = 800;
const int NUM_CHANNELS = 60;

class nEXOChargeReadoutDigitize
{
public:
  
  static nEXOChargeReadoutDigitize* GetInstance() {
	  if(nEXOChargeReadoutDigitize::fInstance == NULL) nEXOChargeReadoutDigitize::fInstance = new nEXOChargeReadoutDigitize();
	  return nEXOChargeReadoutDigitize::fInstance;
  }

  nEXOChargeReadoutDigitize();
  ~nEXOChargeReadoutDigitize();

  void GeneratePCDs(Long64_t evtEntry, nEXOEventData* ED);
  void GroupPCDsForInductionCalc();
  void CalcInductionSignal(nEXOChargeElecChannel* chargeCh);
  void Digitize(nEXOEventData* ED);
  void SetNoiseModel();
  nEXOMCPixelatedChargeDeposit* FindOrCreatePCD(const nEXOCoordinates& coord);
  nEXOChargeReadout* FindOrCreateChargeReadout(Int_t tileId, nEXOEventData* ED);
  bool LoadChannelMap(TString tileFile, TString padsFile);
  bool LoadWP(TString coType, TString filename);
  Int_t FindHitChannel(const nEXOCoordinates& coord);
  void ClearPCDs();
  void ClearHitMap();
  void FillPCDMap(nEXOMCPixelatedChargeDeposit* pcd) const;
  void FillHitMap(nEXOChargeReadout* readout) const;
  void Reset(); // Reset the arrays of drift time and diffusion spread
  void PrintPCDMap();
  void PrintHitMap();

  std::map<nEXOCoordinateKey, nEXOMCPixelatedChargeDeposit*>* GetPCDMap();
  std::map<nEXOCoordinateKey, nEXOMCPixelatedChargeDeposit*>* GetInductionPCDMap();
  std::map<Int_t, nEXOChargeReadout*>* GetHitMap();

  void SetSeed(UInt_t seed) { fSeed = seed; fRand = TRandom3(fSeed); };
  void SetDiffusionCoef(Double_t coef) { fDiffusionCoef = coef; };
  void SetNoiseCoef(Double_t noisecoef) { fNoiseSigma = noisecoef; };
  void SetDriftVelocity(Double_t velocity) { fDriftVelocity = velocity; };
  void SetElectronLifetime(Double_t lifetime) { fElectronLifetime = lifetime; };
  void SetInductionSim(Bool_t isSim) { fInductionSim = isSim; };
  void SetInductionType(TString indType) { fInductionType = indType;};
  void CalcDriftVelocity(Double_t field);
  void PrintParameters();
  double GetNoise(){return fNoiseSigma;};

  Double_t fAnodePosition;
  Double_t fElectronLifetime;
  Double_t fDriftVelocity;
  Double_t fDiffusionCoef;
  Double_t fSamplingInterval;
  Double_t fMinimumZ;
  Double_t fMaximumZ;
  Double_t fZoffset;
  Double_t fNoiseSigma;
  Double_t fInductionZoffset;
  Double_t fInductionToffset;

  Bool_t fFastDiffusionSim;
  Bool_t fInductionSim;
  Bool_t fDebug;
  Bool_t fSaveWaveform;
  Bool_t fApplyThreshold;

  Int_t fSampleWindow;
  Int_t fTriggerOffset;
  Int_t fPrintEvent;
  UInt_t fSeed;

  TRandom3 fRand;

  TString fInductionType;

  //std::vector< std::vector<double>> fWaveforms;
  Double_t fWaveforms[NUM_CHANNELS][WFLENGTH];


protected:
  void AddTEPoint(double xpos, double ypos, double zpos, double time);

  typedef std::map<nEXOCoordinateKey, nEXOMCPixelatedChargeDeposit*> PCDMap;
  mutable PCDMap fPCDMap;
  mutable PCDMap fInductionPCDMap;

  // The key of HitMap is the readout tile Id
  typedef std::map<Int_t, nEXOChargeReadout*> HitMap;
  mutable HitMap fHitMap;

private:
  std::queue<std::vector<nEXOMCPixelatedChargeDeposit> > fChargeDeps;
  std::queue<std::vector<nEXOMCPixelatedChargeDeposit> > fInitChargeDeps;
  static nEXOChargeReadoutDigitize* fInstance;
  TStopwatch eventTimer; 

};

#endif
