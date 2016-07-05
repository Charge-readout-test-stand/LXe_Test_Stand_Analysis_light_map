//**********************************************************************
//  A class is used to access data for photo-detector digitizer.
//  caogf@ihep.ac.cn
//  2014.07.25
//**********************************************************************

#ifndef PDROOTIO_H
#define PDROOTIO_H

#include "TObjArray.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "PdSiPMHit.hh"
#include "PdFeeChannel.hh"

#include <vector>

class PdRootIO 
{
public:
  static PdRootIO* GetInstance();

  PdRootIO();
  ~PdRootIO();

  void SetInputFileName(TString file); 
  TString GetInputFileName(); 

  void SetOutputFileName(TString file);
  TString GetOutputFileName();

  TObjArray* GetEvent(int i);

  void WriteEvent(TObjArray* arr);

  void OpenInputFiles(TString file = NULL);

  Int_t GetEntries();

  Double_t GetGenX();
  Double_t GetGenY();
  Double_t GetGenZ();

  void CreateOutputFileAndTree(TString file = NULL, TString tree = NULL);

  void WriteFile();

private:

  static PdRootIO* fRootIO;

  TObjArray* fInputObjArray;

  TString fInputFileName;
  TString fOutputFileName;

  TChain* fChain;
  TTree* fOutputTree;
  TFile* fOutputFile;

  Int_t fEvtID;
  Double_t fGenx;
  Double_t fGeny;
  Double_t fGenz;
  Int_t fNumOP;
  Int_t fSiPMID[1000000];
  Double_t fOPEnergy[1000000];
  Double_t fOPTime[1000000];

  Int_t fNFiredChannel;
  Int_t fReadoutWindow;
  Int_t fChannelID[50000];
  Int_t fSignalType[50000];
  Double_t fCharge[50000];
  //Double_t fPulse[50000][1000];
  std::vector<std::vector<double> > fPulse;
  
};

#endif
