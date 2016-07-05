#include "TROOT.h"
#include "TString.h"
#include "nEXODigiAnalysis.hh"
#include "nEXOChargeReadoutDigitize.hh"
#include "nEXOClustering.hh"

#ifndef nEXOEventData_hh
#include "nEXOUtilities/nEXOEventData.hh"
#endif

#include<iostream>
#include <cstdlib>

using namespace std;

namespace {
  void PrintUsage() {
  std::cout << " Usage: " << std::endl;
  std::cout << " OpNovice [-i InputFileName] [-t InputTreeName] [-tmap tileNampName]"
            << " [-pmap padsMapName] [-o OutputFileName] [-n nEvents] [-r seed]"
            << " [-el elifetime] [-dcoef diffusionCoef] [-ids isInductionSim]"
            << " [-field electricField]"
            << std::endl;
  std::cout << " note: -tmap, -pmap, -r, -n, -field options could have default values."
            << std::endl;
  }
}

int main(int argc,char** argv)
{
  TString InputFileName, InputTreeName;
  TString tileMapName, padsMapName;
  TString coType="Pads", wpFileName;
  if(getenv("NEXOANALYSIS")) {
    tileMapName = TString(getenv("NEXOANALYSIS")) + "/utilities/scripts/tilesMap.txt";
    padsMapName = TString(getenv("NEXOANALYSIS")) + "/utilities/scripts/localChannelsMap.txt";
    wpFileName = TString(getenv("NEXOANALYSIS")) + "/utilities/data/";
  }
  TString OutputFileName, OutputTreeName="evtTree";
  Int_t nEvents = 0;

  nEXOChargeReadoutDigitize* chargeDigi = new nEXOChargeReadoutDigitize();

  for ( Int_t i=1; i<argc; i=i+2 ) {
    if      ( TString(argv[i]) == "-i" ) InputFileName = argv[i+1];
    else if ( TString(argv[i]) == "-t" ) InputTreeName = argv[i+1];
    else if ( TString(argv[i]) == "-tmap" ) tileMapName = argv[i+1];
    else if ( TString(argv[i]) == "-pmap" ) padsMapName = argv[i+1];
    else if ( TString(argv[i]) == "-o" ) OutputFileName = argv[i+1];
    else if ( TString(argv[i]) == "-wpFile" ) wpFileName += argv[i+1];
    else if ( TString(argv[i]) == "-coType" ) coType = argv[i+1];
    else if ( TString(argv[i]) == "-n" ) nEvents = TString(argv[i+1]).Atoi();
    else if ( TString(argv[i]) == "-r" ) {
      chargeDigi->SetSeed( (UInt_t) (TString(argv[i+1]).Atoi()) );
    }
    else if ( TString(argv[i]) == "-el" ) {
      chargeDigi->SetElectronLifetime( TString(argv[i+1]).Atof()*CLHEP::microsecond );
    }
    else if ( TString(argv[i]) == "-dcoef" ) {
      chargeDigi->SetDiffusionCoef( TString(argv[i+1]).Atof()*(CLHEP::cm*CLHEP::cm)/CLHEP::second );
    }
	 else if	( TString(argv[i]) == "-noise" )	{
		 chargeDigi->SetNoiseCoef( TString(argv[i+1]).Atof() );
	 }
	 else if	( TString(argv[i]) == "-padsize" )	{
		 nEXOChannelMap::GetInstance()->SetPadSize( TString(argv[i+1]).Atof()*CLHEP::mm );
	 }
    else if ( TString(argv[i]) == "-ids" ) {
      if(TString(argv[i+1])=="true") chargeDigi->SetInductionSim(true);
      if(TString(argv[i+1])=="false") chargeDigi->SetInductionSim(false);
    }
    else if ( TString(argv[i]) == "-field" ) {
      // Efield unit: V/cm
      chargeDigi->CalcDriftVelocity( TString(argv[i+1]).Atof() );
    }
    else if (TString(argv[i]) == "-indtype"){
        chargeDigi->SetInductionType( TString(argv[i+1]));
    }
    else {
      PrintUsage();
      return 1;
    }
  }

  if(chargeDigi->LoadChannelMap(tileMapName, padsMapName)==false) exit(-1);
  if(chargeDigi->LoadWP(coType, wpFileName)==false) exit(-1);

  cout << "---------------Setting Branches----------------" << endl;
  nEXODigiAnalysis::GetInstance()->SetTreeBranches(InputFileName, InputTreeName);
  
  cout << "---------------Create Output----------------" << endl;
  nEXODigiAnalysis::GetInstance()->CreateOutputFile(OutputFileName, OutputTreeName);

  if(nEvents<0 || nEvents>nEXODigiAnalysis::GetInstance()->GetEntries()) 
    nEvents = nEXODigiAnalysis::GetInstance()->GetEntries();

  chargeDigi->PrintParameters();

  nEXOClustering* clusterTool = new nEXOClustering();
 
  cout << "---------------Beggining Loop over events----------------" << endl;
  cout << "there are nEvents = " << nEvents << endl;
  for(Long64_t i=0; i<nEvents; i++) {
    nEXOEventData* ED = new nEXOEventData();
    chargeDigi->GeneratePCDs(i, ED);
    chargeDigi->Digitize(ED);
    nEXODigiAnalysis::GetInstance()->Fill(chargeDigi);

    // clustering test
    //cout << "---------------Create Cluster----------------" << endl;
    clusterTool->CreateChargeClusters(ED);
    //cout << "---------------Fill Cluster----------------" << endl;
    nEXODigiAnalysis::GetInstance()->FillClusters(ED);
    //clusterTool->CreateScintillationClusters(ED);
    //clusterTool->Clear(ED);
    //cout << "---------------Fill tree----------------" << endl;
    nEXODigiAnalysis::GetInstance()->FillOutputTree();
    //cout << "---------------Delete NULL----------------" << endl;
    if(ED!=NULL) delete ED;
  }

  nEXODigiAnalysis::GetInstance()->Write();

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
