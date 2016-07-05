#ifndef nEXODigiAnalysis_hh
#define nEXODigiAnalysis_hh 1

#ifndef nEXOChargeReadoutDigitize_hh 
#include "nEXOChargeReadoutDigitize.hh"
#endif

#include <vector>

#ifndef nEXOEventData_hh
#include "nEXOEventData.hh"
#endif

#include "TROOT.h"
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"

const int MAXTENUM = 200000;
const int MAXOPNUM = 200000;
const int MAXDEPNUM = 100000;
const int MAXPCDNUM = 50000;
const int MAXCCNUM = 100;

class nEXODigiAnalysis
{
public:
  static nEXODigiAnalysis* GetInstance(){
    if(nEXODigiAnalysis::fInstance == NULL) nEXODigiAnalysis::fInstance = new nEXODigiAnalysis();
    return nEXODigiAnalysis::fInstance;
  }
  
  virtual ~nEXODigiAnalysis();
  
  void GetTE(Int_t i, Double_t& E, Double_t& X, Double_t& Y, Double_t& Z, Double_t& T);
  void GetEntry(Int_t i);
  Int_t GetNumTE();
  Long64_t GetEntries();
  void SetTreeBranches(TString filename, TString treename);
  Double_t GetAnodeZ() { return fAnodeZ; }

  void CreateOutputFile(TString filename, TString treename);
  void Fill(nEXOChargeReadoutDigitize* chargeDigi);
  void FillClusters(nEXOEventData* ED);
  void FillOutputTree();
  void ResetOutputTree();
  void Write();

private:
  nEXODigiAnalysis();
  static nEXODigiAnalysis* fInstance;

  void ResetTreeVariables();

  TFile* fRootFile; 
  TTree* fRootTree;

  TFile* fOutFile;
  TTree* fOutTree;

private:
  
  // Variable in input tree
  Int_t fEventNumber;
  Int_t fSubEventNumber;
  Double_t fGenX;
  Double_t fGenY;
  Double_t fGenZ;
  Int_t fNumDeposits;
  Double_t fEnergyDeposit[MAXDEPNUM];
  Int_t fTrackNumber[MAXDEPNUM];
  Double_t fXpos[MAXDEPNUM];
  Double_t fYpos[MAXDEPNUM];
  Double_t fZpos[MAXDEPNUM];
  
  // info about particles created by radioactive decay
  Int_t fNPrimaries;
  Int_t fPdgCode[MAXDEPNUM]; //http://pdg.lbl.gov/mc_particle_id_contents.html
  Double_t fKineticEnergy[MAXDEPNUM];
  
  Int_t fNOP;
  Int_t fInitNOP;
  Double_t fOPEnergy[MAXOPNUM];
  Int_t fSiPMID[MAXOPNUM];
  Double_t fOPTime[MAXOPNUM];
  Int_t fOPStopVolume[MAXOPNUM];
  Double_t fOPX[MAXOPNUM];
  Double_t fOPY[MAXOPNUM];
  Double_t fOPZ[MAXOPNUM];
  Int_t fNTE;
  Double_t fTEEnergy[MAXTENUM];
  Double_t fTEX[MAXTENUM];
  Double_t fTEY[MAXTENUM];
  Double_t fTEZ[MAXTENUM];
  Double_t fTET[MAXTENUM];
  Double_t fAnodeZ;

  TBranch* b_EventNumber;
  TBranch* b_GenX;
  TBranch* b_GenY;
  TBranch* b_GenZ;
  TBranch* b_TotalEventEnergy;
  TBranch* b_NumDeposits;
  TBranch* b_EnergyDeposit;
  TBranch* b_TrackNumber;
  TBranch* b_Xpos;
  TBranch* b_Ypos;
  TBranch* b_Zpos;
  
  TBranch* b_NOP;
  TBranch* b_InitNOP;
  TBranch* b_OPEnergy;
  TBranch* b_SiPMID;
  TBranch* b_OPTime;
  TBranch* b_OPStopVolume;
  TBranch* b_OPX;
  TBranch* b_OPY;
  TBranch* b_OPZ;
  TBranch* b_NTE;
  TBranch* b_TEEnergy;
  TBranch* b_TEX;
  TBranch* b_TEY;
  TBranch* b_TEZ;

  // Variable in output tree
  Double_t fEnergy; // energy in active LXe 
  Double_t fTotalEventLXeEnergy; // energy in all LXe
  Double_t fNestEventEnergy; // energy processed by NEST
  Double_t fEventTime; 

  Int_t fNumPCDs;
  Double_t fPCDx[MAXPCDNUM];
  Double_t fPCDy[MAXPCDNUM];
  Double_t fPCDz[MAXPCDNUM];
  Double_t fPCDe[MAXPCDNUM];
  Double_t fPCDq[MAXPCDNUM];

  Int_t fNumTiles; // Number of tiles be hit
  Int_t fNumChannelsOnTile[200]; // Number of channels be hit on tile
  Double_t fTileCharge[200]; // total charge on each tile

  // temporarily use array to save the hit info of each channel
  // the position of hit channel can be obtained by looking up the channel map
  Int_t fNumChannels; // Number of channels be hit on tile
  Int_t fTileId[MAXPCDNUM]; // the tile Id of the hit channel
  Double_t fxTile[MAXPCDNUM]; // the tile X position of the hit channel
  Double_t fyTile[MAXPCDNUM]; // the tile Y position of the hit channel
  Double_t fXPosition[MAXPCDNUM]; // the tile X position of the hit channel
  Double_t fYPosition[MAXPCDNUM]; // the tile Y position of the hit channel
  Int_t fChannelLocalId[MAXPCDNUM]; // local Id
  Double_t fChannelCharge[MAXPCDNUM]; // charge collected on the hit channel
  Double_t fChannelInductionAmplitude[MAXPCDNUM]; // charge induction on the hit channel
  Double_t fChannelFirstTime[MAXPCDNUM]; // earliest hit time on the channel
  Double_t fChannelLatestTime[MAXPCDNUM]; // earliest hit time on the channel
  Double_t fChannelTime[MAXPCDNUM]; // hit time on the channel


  Double_t fInductionAmplitude[MAXPCDNUM]; // amplitude of induction signal
  // waveform 
  //std::vector<Double_t> fChannelWaveform[200];
  
  std::vector< std::vector<double> > fChannelWaveform;

  Int_t fNumCC;
  Double_t fccEnergy[MAXCCNUM];
  Double_t fccX[MAXCCNUM];
  Double_t fccY[MAXCCNUM];
  Double_t fccT[MAXCCNUM];
  Double_t fccXRMS[MAXCCNUM];
  Double_t fccYRMS[MAXCCNUM];
  Double_t fccTRMS[MAXCCNUM];
  char 	  fccType[MAXCCNUM];

  Double_t fssEnergy;
  Double_t fmsEnergy;
};

#endif
