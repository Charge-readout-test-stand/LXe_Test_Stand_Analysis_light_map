#include "nEXODigiAnalysis.hh"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include <iostream>

#ifndef nEXODimensions_hh
#include "nEXOUtilities/nEXODimensions.hh"
#endif

nEXODigiAnalysis* nEXODigiAnalysis::fInstance = NULL;

nEXODigiAnalysis::nEXODigiAnalysis():
fChannelWaveform(60, std::vector<double>(1))
{
  fRootFile = NULL;
  fRootTree = NULL;
  
  ResetTreeVariables();
}

nEXODigiAnalysis::~nEXODigiAnalysis(){;}

void nEXODigiAnalysis::GetTE(int i, Double_t& E, Double_t& X, Double_t& Y, Double_t& Z, Double_t& T)
{
  E = fTEEnergy[i];
  X = fTEX[i];
  Y = fTEY[i];
  Z = fTEZ[i];
  T = fTET[i];
}

Int_t nEXODigiAnalysis::GetNumTE() {return fNTE; }

void nEXODigiAnalysis::GetOP(int i, Double_t& E, Double_t& X, Double_t& Y, Double_t& Z, Double_t& T)
{
  E = fOPEnergy[i];
  X = fOPX[i];
  Y = fOPY[i];
  Z = fOPZ[i];
 // T = fOPT[i];
}

Int_t nEXODigiAnalysis::GetNumOP() {return fNOP; }

/*void nEXODigiAnalysis::GetPE(int i, Double_t& E, Double_t& X, Double_t& Y, Double_t& Z, Double_t& T)
{
  E = fPEEnergy[i];
  X = fPEX[i];
  Y = fPEY[i];
  Z = fPEZ[i];
 // T = fPET[i];
}

Int_t nEXODigiAnalysis::GetNumPE() {return fNPE; }
*/
void nEXODigiAnalysis::ResetTreeVariables(void)
{
  fEventNumber = 0;
  fSubEventNumber = 0; 
  fEnergy = 0;
  fNestEventEnergy = 0; 
  fEventTime = 0; 
  fNumDeposits = 0;
  fInitNOP = 0;
  fNOP = 0;
  fNTE = 0;
  fGenX = 0;
  fGenY = 0;
  fGenZ = 0;

  for (int i = 0; i < MAXOPNUM; i++) {
    fSiPMID[i] = 0; 
    fOPTime[i] = 0; 
    fOPEnergy[i] = 0; 
    fOPStopVolume[i] = 0; 
    fOPX[i] = 0; 
    fOPY[i] = 0; 
    fOPZ[i] = 0; 
  }
  for (int i = 0; i < MAXTENUM; i++)
  {
    fTEX[i] = 0; 
    fTEY[i] = 0;
    fTEZ[i] = 0;
    fTET[i] = 0;
    fTEEnergy[i] = 0; 
  }
  for (int i = 0; i< MAXDEPNUM; i++)
  {
    //fLengthDeposit[i] = 0; 
    fEnergyDeposit[i] = 0; 
    //fPreEnergyDeposit[i] = 0; 
    //fPostEnergyDeposit[i] = 0; 
    fTrackNumber[i] = 0; 
    fXpos[i] = 0; 
    fYpos[i] = 0; 
    fZpos[i] = 0;
    //fTglob[i] = 0;
    //fTloc[i] = 0;
  }
}

void nEXODigiAnalysis::SetTreeBranches(TString filename, TString treename)
{
  if(!fRootFile) fRootFile = new TFile(filename);
  fRootTree = (TTree*) fRootFile->Get(treename);
  fRootTree->SetBranchAddress("EventNumber",&fEventNumber,&b_EventNumber);
  fRootTree->SetBranchAddress("SubEventNumber",&fSubEventNumber);
  fRootTree->SetBranchAddress("GenX", &fGenX, &b_GenX);
  fRootTree->SetBranchAddress("GenY", &fGenY, &b_GenY);
  fRootTree->SetBranchAddress("GenZ", &fGenZ, &b_GenZ);
  // Primary info:
  fRootTree->SetBranchAddress("NPrimaries", &fNPrimaries); 
  fRootTree->SetBranchAddress("PdgCode", fPdgCode); 
  fRootTree->SetBranchAddress("KineticEnergy", fKineticEnergy); 
  fRootTree->SetBranchAddress("TotalEventEnergy", &fEnergy);
  fRootTree->SetBranchAddress("TotalEventLXeEnergy", &fTotalEventLXeEnergy);
  fRootTree->SetBranchAddress("NestEventEnergy",&fNestEventEnergy);

  fRootTree->SetBranchAddress("NumDeposits",&fNumDeposits, &b_NumDeposits);
  fRootTree->SetBranchAddress("EnergyDeposit",&fEnergyDeposit, &b_EnergyDeposit);
//  fRootTree->SetBranchAddress("TrackNumber",&fTrackNumber, &b_TrackNumber);
  //fRootTree->SetBranchAddress("Xpos", &fXpos, &b_Xpos);
  //fRootTree->SetBranchAddress("Ypos", &fYpos, &b_Ypos);
  //fRootTree->SetBranchAddress("Zpos", &fZpos, &b_Zpos); 
  fRootTree->SetBranchAddress("InitNumOP", &fInitNOP, &b_InitNumOP);
  //fRootTree->SetBranchAddress("OPStopVolume", fOPStopVolume, &b_OPStopVolume);
  fRootTree->SetBranchAddress("NumOP", &fNOP, &b_NumOP);
//  fRootTree->SetBranchAddress("SiPMID", &fSiPMID, &b_SiPMID);
 // fRootTree->SetBranchAddress("OPEnergy", &fOPEnergy, &b_OPEnergy);
//  fRootTree->SetBranchAddress("OPTime", &fOPTime, &b_OPTime);
  fRootTree->SetBranchAddress("OPX", &fOPX, &b_OPX);
  fRootTree->SetBranchAddress("OPY", &fOPY, &b_OPY);
  fRootTree->SetBranchAddress("OPZ", &fOPZ, &b_OPZ);
  fRootTree->SetBranchAddress("NumTE", &fNTE, &b_NTE);
//  fRootTree->SetBranchAddress("TEEnergy", &fTEEnergy, &b_TEEnergy);
  fRootTree->SetBranchAddress("TEX", &fTEX, &b_TEX);
  fRootTree->SetBranchAddress("TEY", &fTEY, &b_TEY);
  fRootTree->SetBranchAddress("TEZ", &fTEZ, &b_TEZ);
  fRootTree->SetBranchAddress("TET", &fTET);

  // Geometry info
  fAnodeZ = 18.16*CLHEP::mm; // 7th LXe
  
  //I think this would work if I had something called Anode
  //but until I fix in the nEXO_MC I will hardcode this
  //just need something called anode at z = 17mm in the sim
  /*
  TGeoManager* gm = (TGeoManager*) fRootFile->Get("nEXOGeometry");
  TGeoNode* gnd = gm->FindNode(0, 0, 0);
  gm->SetCurrentDirection(0, 0, 1);
  TString name = gnd->GetName();
  while(name!="/nEXO/TPCInternals/Anode_0") {
    gm->FindNextBoundaryAndStep();
    gnd = gm->GetCurrentNode();
    name = gnd->GetName();
  }
  Double_t* trans = gm->GetCurrentMatrix()->GetTranslation();
  cout << name << ", current Z: " << trans[2] << endl;
  fAnodeZ = trans[2]*CLHEP::cm/CLHEP::mm; // cm --> mm
  */

}

void nEXODigiAnalysis::GetEntry(Int_t i)
{
  //cout << "Reset" << endl;
  ResetTreeVariables();
  //cout << "Vars " << i << endl;
  fRootTree->GetEntry(i);
  //cout << "Got Vars" << endl;
}

Long64_t nEXODigiAnalysis::GetEntries()
{
  return fRootTree->GetEntries();
}

void nEXODigiAnalysis::CreateOutputFile(TString OutputFileName, TString OutputTreeName)
{
  fOutFile = new TFile(OutputFileName, "recreate");
  fOutTree = new TTree(OutputTreeName, "");

  fOutTree->Branch("Energy", &fEnergy, "Energy/D");
  fOutTree->Branch("TotalEventLXeEnergy",&fTotalEventLXeEnergy, "TotalEventLXeEnergy/D"); 
  fOutTree->Branch("NestEventEnergy",&fNestEventEnergy, "NestEventEnergy/D"); 
  fOutTree->Branch("EventTime",&fEventTime, "EventTime/D"); 

  // Geant4 event number
  fOutTree->Branch("EventNumber",&fEventNumber,"EventNumber/I");
  fOutTree->Branch("SubEventNumber",&fSubEventNumber,"SubEventNumber/I");

  // Geant4 vertex:
  fOutTree->Branch("GenX", &fGenX, "GenX/D");
  fOutTree->Branch("GenY", &fGenY, "GenY/D");
  fOutTree->Branch("GenZ", &fGenZ, "GenZ/D");
  fOutTree->Branch("NPE", &fNPE, "NPE/D");
  fOutTree->Branch("NPEactive", &fNPEactive, "NPEactive/D");
  fOutTree->Branch("NOP", &fNOP, "NOP/I");
  fOutTree->Branch("NOPactive", &fNOPactive, "NOPactive/I"); 
 
  // Primary info:
  fOutTree->Branch("NPrimaries", &fNPrimaries,"NPrimaries/I"); 
  fOutTree->Branch("PdgCode", fPdgCode,"PdgCode[NPrimaries]/I"); 
  fOutTree->Branch("KineticEnergy", fKineticEnergy,"KineticEnergy[NPrimaries]/D"); 

  fOutTree->Branch("NumTE", &fNTE, "NumTE/I");
  // PCD info
  fOutTree->Branch("NumPCDs", &fNumPCDs, "NumPCDs/I");
  fOutTree->Branch("PCDx", &fPCDx, "PCDx[NumPCDs]/D");
  fOutTree->Branch("PCDy", &fPCDy, "PCDy[NumPCDs]/D");
  fOutTree->Branch("PCDz", &fPCDz, "PCDz[NumPCDs]/D");
  fOutTree->Branch("PCDe", &fPCDe, "PCDe[NumPCDs]/D");
  fOutTree->Branch("PCDq", &fPCDq, "PCDq[NumPCDs]/D");

/*
  fOutTree->Branch("NumTiles", &fNumTiles, "NumTiles/I");
  fOutTree->Branch("NumChannelsOnTile", &fNumChannelsOnTile, "NumChannelsOnTile[NumTiles]/I");
  fOutTree->Branch("TileCharge", &fTileCharge, "TileCharge[NumTiles]/D");
*/
  fOutTree->Branch("NumChannels", &fNumChannels, "NumChannels/I");

//  fOutTree->Branch("TileId", &fTileId, "TileId[NumChannels]/I");
  fOutTree->Branch("xTile", &fxTile, "xTile[NumChannels]/D");
  fOutTree->Branch("yTile", &fyTile, "yTile[NumChannels]/D");
  fOutTree->Branch("XPosition", &fXPosition, "XPosition[NumChannels]/D");
  fOutTree->Branch("YPosition", &fYPosition, "YPosition[NumChannels]/D");
  fOutTree->Branch("ChannelLocalId", &fChannelLocalId, "ChannelLocalId[NumChannels]/I");
  //fOutTree->Branch("ChannelWaveform", &fChannelWaveform, "ChannelWaveform[NumChannels]/D");
  fOutTree->Branch("ChannelWaveform", &fChannelWaveform);
  fOutTree->Branch("ChannelCharge", &fChannelCharge, "ChannelCharge[NumChannels]/D");
//  fOutTree->Branch("ChannelFirstTime", &fChannelFirstTime, "ChannelFirstTime[NumChannels]/D");
//  fOutTree->Branch("ChannelLatestTime", &fChannelLatestTime, "ChannelLatestTime[NumChannels]/D");
  fOutTree->Branch("ChannelTime", &fChannelTime, "ChannelTime[NumChannels]/D");
//  fOutTree->Branch("InductionAmplitude", &fInductionAmplitude, "InductionAmplitude[NumChannels]/D");

  //std::vector<vector<Double_t> > fChannelWaveform;
  //std::vector<Double_t > fChannelWaveform[10000];

  fOutTree->Branch("NumCC", &fNumCC, "NumCC/I");
  fOutTree->Branch("ccEnergy", &fccEnergy, "ccEnergy[NumCC]/D");
  fOutTree->Branch("ccX", &fccX, "ccX[NumCC]/D");
  fOutTree->Branch("ccY", &fccY, "ccY[NumCC]/D");
  fOutTree->Branch("ccT", &fccT, "ccT[NumCC]/D");
  fOutTree->Branch("ccXRMS", &fccXRMS, "ccXRMS[NumCC]/D");
  fOutTree->Branch("ccYRMS", &fccYRMS, "ccYRMS[NumCC]/D");
  fOutTree->Branch("ccTRMS", &fccTRMS, "ccTRMS[NumCC]/D");
  fOutTree->Branch("ccType", &fccType, "ccType[NumCC]/C");

  fOutTree->Branch("ssEnergy", &fssEnergy, "ssEnergy/D");
  fOutTree->Branch("msEnergy", &fmsEnergy, "msEnergy/D");
}

void nEXODigiAnalysis::FillClusters(nEXOEventData* ED)
{
  fNumCC = ED->GetNumChargeClusters();
  int xcluster=0, ycluster=0;
  for(int i=0; i<fNumCC; i++) {
    nEXOChargeCluster* cc = ED->GetChargeCluster(i);
    fccEnergy[i] = cc->fCharge_cluster;
    fccX[i] = cc->fX_cluster;
    fccY[i] = cc->fY_cluster;
    fccT[i] = cc->fT_cluster;
    fccXRMS[i] = cc->fXRMS_cluster;
    fccYRMS[i] = cc->fYRMS_cluster;
    fccTRMS[i] = cc->fTRMS_cluster;
	 fccType[i] = cc->fChanneltype_cluster;
	 if(cc->fChanneltype_cluster=='X'){
		 xcluster++;
	 }
	 else if(cc->fChanneltype_cluster=='Y'){
		 ycluster++;
	 }
  }

  if(xcluster+ycluster<=1) {
    fssEnergy = fccEnergy[0];
  }
  else if(xcluster==1&&ycluster==1&&fabs(fccT[0]-fccT[1])<1000){
	  fssEnergy = fccEnergy[0]+fccEnergy[1];
  }
  else {
    fmsEnergy = 0.;
    for(int i=0; i<fNumCC; i++) fmsEnergy += fccEnergy[i];
  }
}
void nEXODigiAnalysis::Fill(nEXOLightReadoutDigitize* lightDigi)
{
fNPE = lightDigi->GetNPE();
fNPEactive = lightDigi->GetNPEactive();
fNOPactive = lightDigi->GetNOPactive();
}
void nEXODigiAnalysis::Fill(nEXOChargeReadoutDigitize* chargeDigi)
{
  //-----------------------PCD Map-----------------------
  std::map<nEXOCoordinateKey, nEXOMCPixelatedChargeDeposit*>::iterator pcdIter;
  std::map<nEXOCoordinateKey, nEXOMCPixelatedChargeDeposit*> *pcdMap = chargeDigi->GetPCDMap();
  fNumPCDs = pcdMap->size();
  
  Int_t iPCD = 0;
  for(pcdIter=pcdMap->begin(); pcdIter!=pcdMap->end(); pcdIter++) {
    nEXOMCPixelatedChargeDeposit *pcd = pcdIter->second;
    nEXOCoordinates coord = pcd->GetPixelCenter();
    fPCDx[iPCD] = coord.GetX();
    fPCDy[iPCD] = coord.GetY();
    fPCDz[iPCD] = coord.GetZ();
    fPCDe[iPCD] = pcd->fTotalEnergy;
    fPCDq[iPCD] = pcd->fTotalTE;

    iPCD++;
  }

  //-----------------------Hit Map-----------------------
  //chargeDigi->PrintHitMap();

  std::map<Int_t, nEXOChargeReadout*>::iterator iter;
  std::map<Int_t, nEXOChargeReadout*> *hitMap = chargeDigi->GetHitMap();
  fNumTiles = hitMap->size();

  //Fill in the waveforms for each channel first
  //This is total WF per channel for each event
  for(Int_t k=0; k<chargeDigi->fSampleWindow; k++)
  {
      for (Int_t j=0; j<NUM_CHANNEL_ON_TILE; j++){
          //std::cout << " Adding Waveform " << chargeDigi->fWaveforms[j][k]  << std::endl;
          fChannelWaveform[j].push_back(chargeDigi->fWaveforms[j][k]);
      }
  }


  Int_t iTile = 0, iCh = 0;
  for(iter=hitMap->begin(); iter!=hitMap->end(); iter++) {
    Int_t tileId = iter->first;
    nEXOChargeReadout* readout = iter->second;

    
    fNumChannelsOnTile[iTile] = readout->GetNumChargeChannels(); //Gets the number of PCDs deposited
    fNumChannels += fNumChannelsOnTile[iTile];

    for (int k=0; k<fNumChannelsOnTile[iTile]; k++) {
      nEXOChargeElecChannel* elecChannel = readout->GetChargeChannel(k);
      Int_t localId = nEXOChannelMap::GetInstance()->GetLocalId(elecChannel->GetChannelId());
      fTileId[iCh] = tileId;
      fxTile[iCh] = nEXOChannelMap::GetInstance()->GetTilePosition(tileId).x();
      fyTile[iCh] = nEXOChannelMap::GetInstance()->GetTilePosition(tileId).y();;
      fXPosition[iCh] = nEXOChannelMap::GetInstance()->GetChannelPosition(tileId, localId).x();
      fYPosition[iCh] = nEXOChannelMap::GetInstance()->GetChannelPosition(tileId, localId).y();
      fChannelCharge[iCh] = elecChannel->GetChannelCharge();
      fChannelFirstTime[iCh] = elecChannel->GetChannelFirstTime();
      fChannelLatestTime[iCh] = elecChannel->GetChannelLatestTime();
      fChannelTime[iCh] = elecChannel->GetChannelTime();
      fChannelLocalId[iCh] = localId;
      fChannelInductionAmplitude[iCh] = elecChannel->GetChannelInductionAmplitude();
      fTileCharge[iTile] += fChannelCharge[iCh];
      iCh++;
    }

    iTile++;
  }
  fEventTime = fTET[0]; // use time of first TE as event time for now 

}

void nEXODigiAnalysis::FillOutputTree()
{
  //cout << "Here Fill Tree " << endl;
  fOutTree->Fill();
  //cout << "Do Reset  " << endl;
  ResetOutputTree();
  //cout << "Did Reset " << endl;
}

void nEXODigiAnalysis::ResetOutputTree()
{
  fNumPCDs = 0;
  for( Int_t i=0; i<MAXPCDNUM; i++) {
    fPCDx[i] = 0.;
    fPCDy[i] = 0.;
    fPCDz[i] = 0.;
    fPCDe[i] = 0.;
    fPCDq[i] = 0.;
  }

  fNumTiles = 0;
  for( Int_t i=0; i<200; i++) {
    fNumChannelsOnTile[i] = 0;
    fTileCharge[i] = 0.;
  }
  fNumChannels = 0;
  for( Int_t i=0; i<MAXPCDNUM; i++) {
    fTileId[i] = 0;
    fxTile[i] = 0;
    fyTile[i] = 0;
    fXPosition[i] = 0;
    fYPosition[i] = 0;
    fChannelCharge[i] = 0.;
    fChannelFirstTime[i] = 0.;
    fChannelLatestTime[i] = 0.;
    fChannelTime[i] = 0.;
    fChannelLocalId[i] = 0; 
    //fChannelWaveform[i] = 0;
    fInductionAmplitude[i] = 0.;
  }

  for( Int_t i=0; i<60; i++) {
    fChannelWaveform[i].clear();
  }

  fNumCC = 0;
  fssEnergy = 0.;
  fmsEnergy = 0.;
  for( Int_t i=0; i<100; i++) {
    fccEnergy[i] = 0.;
    fccX[i] = 0.;
    fccY[i] = 0.;
    fccT[i] = 0.;
    fccXRMS[i] = 0.;
    fccYRMS[i] = 0.;
    fccTRMS[i] = 0.;
	 fccType[i] = 'N';
  }

}

void nEXODigiAnalysis::Write()
{
  fOutFile->Write();
  fOutFile->Close();
}
