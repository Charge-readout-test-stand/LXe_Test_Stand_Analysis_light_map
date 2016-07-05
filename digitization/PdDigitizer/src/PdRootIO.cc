#include "PdRootIO.hh"
#include "PdParameters.hh"

#include <iostream>

#ifdef __MAKECINT__
#pragma link C++ class vector<vector<double> >+;
#endif

PdRootIO* PdRootIO::fRootIO = 0;

PdRootIO* PdRootIO::GetInstance()
{
  if(fRootIO == 0) fRootIO = new PdRootIO();
  return fRootIO;
}

PdRootIO::PdRootIO() 
{
  fOutputFile = NULL;
  fOutputTree = NULL;

  fEvtID = 0;
  fGenx = 0;
  fGeny = 0;
  fGenz = 0;
  fNumOP = 0;
  for(int i = 0; i < 1000000; i++) {
    fSiPMID[i] = 0;
    fOPEnergy[i] = 0;
    fOPTime[i] = 0;
  }

  fInputObjArray = new TObjArray();
}

PdRootIO::~PdRootIO() 
{
}

void PdRootIO::OpenInputFiles(TString file)
{
  if(file != NULL) fInputFileName = file;

  fChain = new TChain("tree");
  fChain->Add(fInputFileName);

  fChain->SetBranchAddress("EventNumber", &fEvtID);
  fChain->SetBranchAddress("GenX", &fGenx);
  fChain->SetBranchAddress("GenY", &fGeny);
  fChain->SetBranchAddress("GenZ", &fGenz);
  fChain->SetBranchAddress("NumOP", &fNumOP);
  fChain->SetBranchAddress("SiPMID", fSiPMID);
  //fChain->SetBranchAddress("OPEnergy", fOPEnergy);
  fChain->SetBranchAddress("OPTime", fOPTime);
}

Int_t PdRootIO::GetEntries()
{
  Int_t nentries = fChain->GetEntries();

  return nentries;
}

TObjArray* PdRootIO::GetEvent(int i)
{
  fInputObjArray->Delete();
  
  fChain->GetEntry(i);

  for(Int_t iOP = 0; iOP < fNumOP; iOP++) {
    PdSiPMHit* siPMHit = new PdSiPMHit(fSiPMID[iOP], 1, fOPEnergy[iOP], fOPTime[iOP]);
    fInputObjArray->Add(siPMHit);
  }

  return fInputObjArray;
}

Double_t PdRootIO::GetGenX()
{
  return fGenx;
}

Double_t PdRootIO::GetGenY()
{
  return fGeny;
}

Double_t PdRootIO::GetGenZ()
{
  return fGenz;
}

void PdRootIO::CreateOutputFileAndTree(TString file, TString tree)
{
  if(file != NULL) fOutputFileName = file;

  fOutputFile = new TFile(fOutputFileName, "RECREATE");

  if(tree != NULL) fOutputTree = new TTree(tree, "A tree to store digitizer output information");
  else fOutputTree = new TTree("evt", "A tree to store digitizer output information");

  fOutputTree->Branch("EvtID", &fEvtID, "EvtID/I");
  fOutputTree->Branch("GenX", &fGenx, "GenX/D");
  fOutputTree->Branch("GenY", &fGeny, "GenY/D");
  fOutputTree->Branch("GenZ", &fGenz, "GenZ/D");
  fOutputTree->Branch("NFiredChannel", &fNFiredChannel, "NFiredChannel/I");
  fOutputTree->Branch("SignalType", fSignalType, "SignalType[NFiredChannel]/I");
  fOutputTree->Branch("ChannelID", fChannelID, "ChannelID[NFiredChannel]/I");
  fOutputTree->Branch("Charge", fCharge, "Charge[NFiredChannel]/D");
  fOutputTree->Branch("ReadoutWindow", &fReadoutWindow, "ReadoutWindow/I");
  fOutputTree->Branch("Pulse", &fPulse);

}

void PdRootIO::WriteFile()
{
  fOutputTree->Write();
  fOutputFile->Write();
}

void PdRootIO::WriteEvent(TObjArray* objArr)
{
  Int_t nentries = objArr->GetEntries();
  fReadoutWindow = (int) PdParameters::GetInstance()->GetReadoutWindow();

  fPulse.clear();

  for(Int_t i = 0; i < nentries; i++) {
    PdFeeChannel* chan = static_cast<PdFeeChannel*> (objArr->At(i));
    fChannelID[i] = chan->GetChannelID();
    fCharge[i] = chan->GetChannelCharge();
    fSignalType[i] = chan->GetSignalType();

    std::vector<double> pulseInChannel;
    pulseInChannel.clear();
    for(Int_t j = 0; j < (int) PdParameters::GetInstance()->GetReadoutWindow(); j++) {
      pulseInChannel.push_back(chan->GetPulseShape(j));
    }
    fPulse.push_back(pulseInChannel);
  }

  fNFiredChannel = nentries;

  fOutputTree->Fill();
  objArr->Delete();
}

void PdRootIO::SetInputFileName(TString file)
{
  fInputFileName = file;
}

TString PdRootIO::GetInputFileName()
{
  return fInputFileName;
}

void PdRootIO::SetOutputFileName(TString file)
{
  fOutputFileName = file;
}

TString PdRootIO::GetOutputFileName()
{
  return fOutputFileName;
}
