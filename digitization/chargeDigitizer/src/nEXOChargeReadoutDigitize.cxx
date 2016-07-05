#ifndef nEXODigiAnalysis_hh
#include "nEXODigiAnalysis.hh"
#endif

#ifndef nEXOChannelMap_hh
#include "nEXOUtilities/nEXOChannelMap.hh"
#endif

#ifndef nEXOFieldWP_hh
#include "nEXOUtilities/nEXOFieldWP.hh"
#endif

#ifndef nEXODimensions_hh
#include "nEXOUtilities/nEXODimensions.hh"
#endif

#ifndef nEXOEventData_hh
#include "nEXOUtilities/nEXOEventData.hh"
#endif

#include "nEXOChargeReadoutDigitize.hh"
#include<iostream>
#include "TMath.h"
#include "TGraph.h"

using namespace std;

nEXOChargeReadoutDigitize* nEXOChargeReadoutDigitize::fInstance = NULL;

nEXOChargeReadoutDigitize::nEXOChargeReadoutDigitize()
{
  fFastDiffusionSim = false;
  fInductionSim = false;
  fElectronLifetime = 10.0e3*CLHEP::microsecond; // us
  fDiffusionCoef = 1e2*(CLHEP::cm*CLHEP::cm)/CLHEP::second; // cm^2/s
  fDriftVelocity = DRIFT_VELOCITY;
  fSeed = 1;
  fRand = TRandom3(fSeed);
  fDebug = false;
  fPrintEvent = 1;
  //fSamplingInterval = 1.0*CLHEP::microsecond; // us
  fSamplingInterval = 0.04*CLHEP::microsecond;
  fSaveWaveform = true;
  fApplyThreshold = true;
  fNoiseSigma = 0; // noise on sigle channel: 0 electrons
  fInductionZoffset = 0.;
  fInductionToffset = 0.;
  fMinimumZ = 1e6; // maximum Z of PCDs in this MC event
  fMaximumZ = -1e6; // maximum Z of PCDs in this MC event
  fZoffset = 20*fSamplingInterval*DRIFT_VELOCITY; 
  //fLXeOffset = 208.988 * CLHEP::mm; // the center of LXe is not at zero in MC
  //fLXeOffset = 0 * CLHEP::mm;
  fSampleWindow = WFLENGTH;
  fTriggerOffset = 200;
  fInductionType = "COM";
  //fWaveforms(60, std::vector<double>(800));
  eventTimer.Start();
}

bool nEXOChargeReadoutDigitize::LoadChannelMap(TString tileFile, TString padsFile)
{
  if(nEXOChannelMap::GetInstance()->ReadInTileMap(tileFile)) std::cout << "Successfully";
  else { std::cout << "Error Tile File " <<  tileFile; return false; }
  std::cout << " loading readout tiles map: " << tileFile << std::endl;

  if(nEXOChannelMap::GetInstance()->ReadInPadsMap(padsFile)) std::cout << "Successfully";
  else { std::cout << "Error Pad File " << padsFile; return false; }
  std::cout << " loading local pads map in each readout tile: " << padsFile << std::endl;

  return true;
}

bool nEXOChargeReadoutDigitize::LoadWP(TString coType, TString filename) {
  if( nEXOFieldWP::GetInstance()->LoadWP(coType, filename)) std::cout << "Successfully";
  else { std::cout << "Error WP File"; return false; }
  std::cout << " loading WP field map: " << coType << ", " << filename << std::endl;

  return true;
}

void nEXOChargeReadoutDigitize::Reset() 
{
}

void nEXOChargeReadoutDigitize::GeneratePCDs(Long64_t evtEntry, nEXOEventData* ED)
{
  // GeneratePCDs produces a collection of PCDs arrived at the anode plane,
  // it simulates the diffusion and transportation of thermal electrons
  // that generated in the ionziation processes
  ClearPCDs();

  //std::cout << "------Try  Processing event " << evtEntry << " ------" << std::endl;
  nEXODigiAnalysis::GetInstance()->GetEntry(evtEntry);
  //std::cout << "------ Got Tree " << evtEntry << " ------" << std::endl;
  
  Int_t NTE = nEXODigiAnalysis::GetInstance()->GetNumTE();
  //std::cout << "NTE: " << NTE << std::endl;

  if(evtEntry%1==0) {
    std::cout << "------ Processing event " << evtEntry 
      << " (" << NTE << " TEs)"
      << " at " << eventTimer.RealTime() << " s "
      << " ------" << std::endl; 
      eventTimer.Continue();
  }

  Double_t teX=0., teY=0., teZ=0., teE=0., teT=0.;
  Double_t DriftTime = 0.;

  for (Int_t i=0; i<NTE; i++) {
      // Get each thermal electron's position
      nEXODigiAnalysis::GetInstance()->GetTE(i, teE, teX, teY, teZ, teT);
      // calculate drift time, NOTE: teZ should be in mm unit
      DriftTime = (nEXODigiAnalysis::GetInstance()->GetAnodeZ()-teZ*CLHEP::mm)/fDriftVelocity;

      //Is it outside of the anode(18.16mm) and cathode(0.0mm)
      //This is a bug from the nEXO_MC that produces TE in the non Active Region
      //Haven't fixed it yet so doing this which isn't very good
      if(teZ < 0.0 or teZ > nEXODigiAnalysis::GetInstance()->GetAnodeZ()){continue;}

      // calculate attenuation 
      Double_t attn = TMath::Exp(-DriftTime/fElectronLifetime);
      if(fRand.Uniform()>attn) {
        if(fDebug && evtEntry<fPrintEvent) std::cout << "electron be absorbed." << std::endl;
        continue; // electron disappeared
      }

       // calculate diffusion, sigma = sqrt(2*D*t)
      Double_t sigmaDiffusion = TMath::Sqrt(2. * fDiffusionCoef * DriftTime);
      // Double_t DiffusionSpread = TMath::Abs(fRand.Gaus(0, sigmaDiffusion));
      // Double_t phi = fRand.Uniform(0, 2*TMath::Pi());
      // Double_t dx = TMath::Sin(phi)*DiffusionSpread;
      // Double_t dy = TMath::Cos(phi)*DiffusionSpread;
      Double_t dx=fRand.Gaus(0, sigmaDiffusion);
      Double_t dy=fRand.Gaus(0, sigmaDiffusion);

      if(fDebug && evtEntry<fPrintEvent) {
        std::cout << "(X,Y,Z): (" << teX << ", " << teY << ", " << teZ << ")" 
                  << " drift time (us): " << DriftTime/CLHEP::microsecond
                  << " sigmaDiffusion (mm): " << sigmaDiffusion/CLHEP::mm
                  << std::endl;
      }

      AddTEPoint( teX + dx, teY + dy, teZ, DriftTime);
   }
  GroupPCDsForInductionCalc();

  //PrintPCDMap();
  std::cout << "n PCDs: " << fPCDMap.size() << std::endl;
}

void nEXOChargeReadoutDigitize::Digitize(nEXOEventData* ED)
{
  ClearHitMap();

  //Clear the Waveforms for each channel before filling in
  for(Int_t k=0; k<fSampleWindow; k++)
  {
      for (Int_t j=0; j<NUM_CHANNEL_ON_TILE; j++){
          fWaveforms[j][k] = 0.0;
      }
  }

  // Simulate the charge collection signal
  // Determine which channels the PCDs will hit
  PCDMap::iterator iter;
  Int_t tileId=0, localId=0;
  fMinimumZ = 1e6;
  fMaximumZ = -1e6;
  Double_t tmax=-1e9, tmin=1e9;
  //std::cout << "fPCDMap.size(): " << fPCDMap.size() << std::endl;

  for(iter= fPCDMap.begin(); iter!=fPCDMap.end(); iter++) {
    nEXOMCPixelatedChargeDeposit* pcd = iter->second;
    nEXOCoordinates pcdCenter = pcd->GetPixelCenter();
    if( pcdCenter.GetZ()<fMinimumZ ) fMinimumZ = pcdCenter.GetZ();
    if( pcdCenter.GetZ()>fMaximumZ ) fMaximumZ = pcdCenter.GetZ();

    Bool_t isFound=nEXOChannelMap::GetInstance()->FindHitChargeChannel(pcdCenter, tileId, localId);
    if(!isFound) {
      std::cout << "Warning! no hit channel is found!" << std::endl;
      std::cout << "tileId: " << tileId << " localId: " << localId << std::endl;
      std::cout << "pcdCenter: (" <<pcdCenter.GetX()<<","<<pcdCenter.GetY()<<","<<pcdCenter.GetZ()<<") "<< std::endl;
      continue;
    }

    if(fDebug) {
      std::cout << "Found tileId: " << tileId << " localId: " << localId;
      std::cout << " pcdCenter: (" <<pcdCenter.GetX()<<","<<pcdCenter.GetY()<<","<<pcdCenter.GetZ()<<")"<< std::endl;
    }

    nEXOChargeReadout* readout = FindOrCreateChargeReadout(tileId, ED);
    Int_t chanId = nEXOChannelMap::GetInstance()->GetChannelId(tileId, localId);
    nEXOChargeElecChannel* chargeCh = readout->FindOrCreateChargeChannel(chanId);

    Double_t preCharge = chargeCh->GetChannelCharge();
    //Here is where we set the charge for a collection channel
    //Add something here to simulate the waveform for a collection channel
    chargeCh->SetChannelCharge( preCharge + pcd->fTotalTE);

    // Hit Time
    Double_t time =  pcdCenter.GetT(); // CLHEP::microsecond
	 if(tmax<time)	tmax = time;
	 if(tmin>time)	tmin = time;
  //  time += fRand.Uniform(-0.1*CLHEP::microsecond, 0.1*CLHEP::microsecond); // smear the hit time
    if( chargeCh->GetChannelFirstTime()>time) chargeCh->SetChannelFirstTime( time );
    if( chargeCh->GetChannelLatestTime()<time) chargeCh->SetChannelLatestTime( time );

    chargeCh->SetChannelTime( time );
  }
  for(Int_t tileId=0;tileId<172;tileId++){
	  for(Int_t localId=0;localId<=nEXOChannelMap::GetInstance()->GetChannelNumOnTile();localId++){
		  Double_t noisetime = fRand.Uniform(tmin,tmax);
		  Int_t chanId = nEXOChannelMap::GetInstance()->GetChannelId(tileId, localId);
		  Double_t noise = fRand.Gaus(0,fNoiseSigma);
		  if(noise>fNoiseSigma*5){
	      nEXOChargeReadout* readout = FindOrCreateChargeReadout(tileId, ED);
		    if(readout->FindChargeChannel(chanId)!=NULL)	continue;
		    nEXOChargeElecChannel* chargeCh = readout->FindOrCreateChargeChannel(chanId);
			  chargeCh->SetChannelCharge(noise);
			  chargeCh->SetChannelTime(noisetime);
			  chargeCh->SetChannelFirstTime(noisetime);
			  chargeCh->SetChannelLatestTime(noisetime);
		  }
	  }
    //if(readout->GetNumChargeChannels()==0) { 
    //  fHitMap.erase(tileId); 
    //  std::cout << "before remove: " << ED->GetNumChargeReadouts() << std::endl;
    //  ED->RemoveChargeReadout(readout);
    //  std::cout << "after remove: " << ED->GetNumChargeReadouts() << std::endl;
    //  delete readout; 
    //}
  }

  //std::cout << "fMaximumZ: " << fMaximumZ << std::endl;
  //std::cout << "fMinimumZ: " << fMinimumZ << std::endl;

  if(fDebug) PrintHitMap();

  // Simulate the charge induction signal
  if(fInductionSim==true) {
    // only calcualte the tiles which have real charge collection
    HitMap::iterator ccIt;
    for(ccIt=fHitMap.begin(); ccIt!=fHitMap.end(); ccIt++) {
      nEXOChargeReadout* cro = ccIt->second;

      for(Int_t k=0; k<NUM_CHANNEL_ON_TILE; k++) {
        //bool isCalcInduction = false;
        bool isCalcInduction = true;
        Int_t chanId = nEXOChannelMap::GetInstance()->GetChannelId(cro->GetTileId(), k);
        if( cro->FindChargeChannel(chanId)!=NULL ) {
          // if this channel has real charge collection
          if(fApplyThreshold==true) {
            const nEXOChargeElecChannel* cCh = cro->FindChargeChannel(chanId);
            if(cCh->fChannelCharge<3*fNoiseSigma) isCalcInduction = true;
          }
        }
        else {
          // if this channel has NO charge collection, 
          // create ElecChannel and calculate the induction signal
          TVector3 chanPos = nEXOChannelMap::GetInstance()->GetChannelPosition(chanId);
          for(iter= fPCDMap.begin(); iter!=fPCDMap.end(); iter++) {
            nEXOMCPixelatedChargeDeposit* pcd = iter->second;
            nEXOCoordinates pcdCenter = pcd->GetPixelCenter();
            if(nEXOChannelMap::GetInstance()->GetChannelType(k)=="X") {
              // if local channel type is X- channel
              double distTmpX = TMath::Abs(chanPos.x() - pcdCenter.GetX());
              double distTmpY = TMath::Abs(chanPos.y() - pcdCenter.GetY());
              if(distTmpX<3*nEXOChannelMap::GetInstance()->GetPadSize() && distTmpY<TILE_SIZE/2) { isCalcInduction = true; }
            }
            if(nEXOChannelMap::GetInstance()->GetChannelType(k)=="Y") {
              // if local channel type is Y- channel
              double distTmpX = TMath::Abs(chanPos.x() - pcdCenter.GetX());
              double distTmpY = TMath::Abs(chanPos.y() - pcdCenter.GetY());
              if(distTmpY<3*nEXOChannelMap::GetInstance()->GetPadSize() && distTmpX<TILE_SIZE/2) { isCalcInduction = true; }
            }
          }
        }

        //if(isCalcInduction==true) {
        if(true){
           //cout << "IDIS = " << chanId << endl;
           nEXOChargeElecChannel* chargeCh = cro->FindOrCreateChargeChannel(chanId);
          //Calculate Induction Signal (Also need to account for fact that this has time offset)
          //chargeChannel->GetChannelTime()
          CalcInductionSignal(chargeCh);
        }
      }
    }
  }
  
  // Combine the collection and induction signals
  
  // After Digitization, Output to root file, then clear the HitMap
  
  // assign the charge readout to the nEXOEventData object
  //HitMap::iterator ccIt;
  //for(ccIt=fHitMap.begin(); ccIt!=fHitMap.end(); ccIt++) {
  //  (ccIt->second);
  //}

}

void nEXOChargeReadoutDigitize::CalcInductionSignal(nEXOChargeElecChannel* chargeCh) {
  // Calculate the induction signal for one charge channel
  // for all pcds in the current event

  
  fInductionToffset = (nEXODigiAnalysis::GetInstance()->GetAnodeZ() - fMaximumZ - fZoffset)/DRIFT_VELOCITY; 
  //cout << "Max" << fMaximumZ << " " << fMinimumZ << " " << fZoffset << " " << nEXODigiAnalysis::GetInstance()->GetAnodeZ() << endl;
  
  int const maxNT = fSampleWindow;
  int  iCh = chargeCh->fChannelId;
  TVector3 chanPos = nEXOChannelMap::GetInstance()->GetChannelPosition(iCh);

  //std::cout << "Checking Channel " << iCh << std::endl;

  //Create and 0 out the waveform for this channel
  double tmpQ[maxNT];
  for(int k=0; k<maxNT; k++) 
  {
      tmpQ[k] = 0.0;
  }
  
  PCDMap::iterator iter;  //Iterate over all of the PCDs in the event
  int pcdIdx = 0;

  //for(iter= fInductionPCDMap.begin(); iter!=fInductionPCDMap.end(); iter++) {
  for(iter=fPCDMap.begin(); iter!=fPCDMap.end(); iter++){
   // loop the PCDs and calculate the induction on this channel for each
    nEXOMCPixelatedChargeDeposit* pcd = iter->second;
    nEXOCoordinates pcdCenter = pcd->GetPixelCenter();

    //I don't get this??
    //double Z = fMaximumZ - pcdCenter.GetZ() + fZoffset;
    

    //Determine where the waveform should start being generated
    //Where the waveform triggers 
    //Triggers on the Scint cluster this occurs at some set point (500 samples)
    //The time index needs to be offset by this much since it peaks at collection which is after scint
    double TimeIndex = (int)fTriggerOffset;
    
    int khit = 0;
    double Q = 0;
    double Z = pcdCenter.GetZ();
    double Zion = pcdCenter.GetZ(); //Position of Positive Ion
    //Get the Positive Ion Induced Charge with supression from Cathode
    //cout << " Check Anode Pos " << nEXODigiAnalysis::GetInstance()->GetAnodeZ()  << endl;
    Double_t Qion = nEXOFieldWP::GetInstance()->CalcInducedChargeOnPads_Analytic(iCh, pcdCenter.GetX(), pcdCenter.GetY(), nEXODigiAnalysis::GetInstance()->GetAnodeZ()-Zion);
    //Supress the cathode.  See Q = 0 if at cathode(z = 0) and Q = 1 if at anode (z=18.16)
    Qion = Qion*(Zion)/nEXODigiAnalysis::GetInstance()->GetAnodeZ();
    int ktest = 0;

    //cout << "Start Z = " << Z << endl;
    //cout << "Checking Channel at x = " << chanPos.x() << " y = " << chanPos.y() << endl;
    for(int k=TimeIndex; k<maxNT; k++) {
      double dZ = fSamplingInterval*DRIFT_VELOCITY;
      //double Z = fMaximumZ - pcdCenter.GetZ() + fZoffset - fSamplingInterval*k*DRIFT_VELOCITY; //CLHEP::mm
      if(Z+dZ <= nEXODigiAnalysis::GetInstance()->GetAnodeZ()) {
        
        if(fInductionType == "COM"){
            double dQ = nEXOFieldWP::GetInstance()->CalcInducedChargeOnPads_COMSOL(iCh, pcdCenter.GetX(), pcdCenter.GetY(), Z+dZ)  -  
                        nEXOFieldWP::GetInstance()->CalcInducedChargeOnPads_COMSOL(iCh, pcdCenter.GetX(), pcdCenter.GetY(), Z);

            Q+=dQ;
        }
        else{
            Double_t Znew = Z+dZ;
            Q = nEXOFieldWP::GetInstance()->CalcInducedChargeOnPads_Analytic(iCh, pcdCenter.GetX(), pcdCenter.GetY(), nEXODigiAnalysis::GetInstance()->GetAnodeZ() - Znew);
            Q = Q*(Znew)/nEXODigiAnalysis::GetInstance()->GetAnodeZ();
            Q = Q - Qion; //Subtract the charge induced by positive ion
            /*if(iCh == 15)
            {
                cout << endl;
                cout << " Qion = " << Qion << " Ch " << iCh << endl;
                cout << "(" << pcdCenter.GetX() << " , " << pcdCenter.GetY() << " , " << Znew << ") Q = "  << Q << " " <<
                    nEXOFieldWP::GetInstance()->CalcInducedChargeOnPads_Analytic(iCh, pcdCenter.GetX(), pcdCenter.GetY(), nEXODigiAnalysis::GetInstance()->GetAnodeZ() - Znew)
                    << endl;
            }
            */
        }
        tmpQ[k] += Q * pcd->fTotalTE;
        khit = k;
        Z+=dZ;
      }
      else {
          //Once we hit just fill in the rest with the last value    
          if(fInductionType != "COM"){
              //Hit anode so set exactly at anode
              Double_t Znew = nEXODigiAnalysis::GetInstance()->GetAnodeZ();
              Q = nEXOFieldWP::GetInstance()->CalcInducedChargeOnPads_Analytic(iCh, pcdCenter.GetX(), pcdCenter.GetY(), nEXODigiAnalysis::GetInstance()->GetAnodeZ() - Znew);
              Q = Q*(Znew)/nEXODigiAnalysis::GetInstance()->GetAnodeZ();
              Q = Q - Qion;
          }
          tmpQ[k] += Q*pcd->fTotalTE; 
      }
      /*
      if(iCh==16 or iCh==46)
      {
        cout << "PCD (x,y,q) " << pcdCenter.GetX() << " " << pcdCenter.GetY() << " " << pcd->fTotalTE << " Ch = " << iCh << " k = " << k  << " Q = " << tmpQ[k] << " " << Z << " " << Q << endl;
      }
      */
    }
    pcdIdx++;
  }

  double maxQ = 0., maxQT = 0.;
  for(int k=0; k<maxNT; k++) {
    if( tmpQ[k]>maxQ ) { maxQ = tmpQ[k]; maxQT = k*fSamplingInterval; }
    if(fSaveWaveform) chargeCh->fWaveform.push_back( tmpQ[k] );
    fWaveforms[iCh][k] += tmpQ[k];
  }



  chargeCh->fInductionAmplitude = maxQ;
  chargeCh->fTime = maxQT + fInductionToffset;
//                  + fRand.Uniform(-0.1*CLHEP::microsecond, 0.1*CLHEP::microsecond);
//  chargeCh->fTime = 3.5*CLHEP::microsecond;
}

nEXOChargeReadout* nEXOChargeReadoutDigitize::FindOrCreateChargeReadout(Int_t tileId, nEXOEventData* ED)
{
  // Use channel Map to determine which channel get hit
  // Find if it's in the exsiting HitMap
  HitMap::iterator iter = fHitMap.find(tileId);
  if ( iter != fHitMap.end() ) { 
    return static_cast<nEXOChargeReadout*>(iter->second);
  }
  else {
    //nEXOChargeReadout* readout = new nEXOChargeReadout(tileId);
    nEXOChargeReadout* readout = ED->GetNewChargeReadout();
    readout->SetTileId(tileId);
    FillHitMap(readout);
    return readout;
  }
}

void nEXOChargeReadoutDigitize::SetNoiseModel()
{
  // Sampling the noise power spectra
}

void nEXOChargeReadoutDigitize::AddTEPoint(double xpos, double ypos, double zpos, double time)
{
    // Add a charge point at a given x, y, z position (in mm), 
    // at a certain time (in us)
    const nEXOCoordinates Coord(xpos, ypos, zpos, time);
    nEXOMCPixelatedChargeDeposit* pc = FindOrCreatePCD(Coord);
    pc->fTotalTE += 1;
}

nEXOMCPixelatedChargeDeposit* nEXOChargeReadoutDigitize::FindOrCreatePCD(const nEXOCoordinates& coord)
{
  const nEXOCoordinateKey& MyKey = coord.GetCoordinateKey();
  PCDMap::iterator iter = fPCDMap.find( MyKey );
  if ( iter != fPCDMap.end() ) { 
    return static_cast<nEXOMCPixelatedChargeDeposit*>(iter->second);
  }
  else {
    //nEXOMCPixelatedChargeDeposit* ChargeDeposit = new nEXOMCPixelatedChargeDeposit(coord);
    nEXOMCPixelatedChargeDeposit* ChargeDeposit = new nEXOMCPixelatedChargeDeposit(MyKey);
    FillPCDMap(ChargeDeposit);
    return ChargeDeposit;
  }
}

void nEXOChargeReadoutDigitize::FillPCDMap(nEXOMCPixelatedChargeDeposit* pcd) const
{
  fPCDMap[pcd->GetPixelCoordinateKey()] = pcd;
}

void nEXOChargeReadoutDigitize::FillHitMap(nEXOChargeReadout* readout) const
{
  fHitMap[readout->GetTileId()] = readout;
}

void nEXOChargeReadoutDigitize::ClearPCDs() 
{
  fPCDMap.clear();
  fInductionPCDMap.clear();
}

void nEXOChargeReadoutDigitize::GroupPCDsForInductionCalc() {
  PCDMap::iterator iter;
  for(iter= fPCDMap.begin(); iter!=fPCDMap.end(); iter++) {
    nEXOMCPixelatedChargeDeposit* pcd = iter->second;
    nEXOCoordinates pcdCenter = pcd->GetPixelCenter();

    nEXOCoordinateKey MyKey = nEXOCoordinateKey(pcdCenter.GetX(), pcdCenter.GetY(), pcdCenter.GetZ(),
                                                pcdCenter.GetT(), 1);

    nEXOMCPixelatedChargeDeposit* ChargeDeposit;
    PCDMap::iterator iterInd = fInductionPCDMap.find( MyKey );
    if ( iterInd == fInductionPCDMap.end() ) { 
      // Not find this nEXOCoordinateKey
      ChargeDeposit = new nEXOMCPixelatedChargeDeposit(MyKey);
      fInductionPCDMap[MyKey] = ChargeDeposit;
    }
    else {
      ChargeDeposit = iterInd->second;
    }
    ChargeDeposit->fTotalTE += pcd->fTotalTE;
  }
}

void nEXOChargeReadoutDigitize::ClearHitMap() 
{
  fHitMap.clear();
}

void nEXOChargeReadoutDigitize::PrintPCDMap()
{
  PCDMap::iterator iter;
  Int_t totTE = 0;
  for(iter=fPCDMap.begin(); iter!=fPCDMap.end(); iter++) {
    nEXOCoordinateKey key = iter->first;
    nEXOMCPixelatedChargeDeposit* pcd = iter->second;
    std::cout << "(" << key.GetCenter().GetX()
               << ", " << key.GetCenter().GetY()
               << ", " << key.GetCenter().GetZ()
               << "): " << pcd->fTotalTE << std::endl;
    totTE += pcd->fTotalTE;
  }
  std::cout << "fPCDMap size, total TE: "
            << fPCDMap.size() << ", "
            << totTE << std::endl;

  totTE = 0;
  for(iter=fInductionPCDMap.begin(); iter!=fInductionPCDMap.end(); iter++) {
    nEXOCoordinateKey key = iter->first;
    nEXOMCPixelatedChargeDeposit* pcd = iter->second;
    //std::cout << "(" << key.GetCenter().GetX()
    //           << ", " << key.GetCenter().GetY()
    //           << ", " << key.GetCenter().GetZ()
    //           << "): " << pcd->fTotalTE << std::endl;
    totTE += pcd->fTotalTE;
  }
  std::cout << "fInductionPCDMap size, total TE: "
            << fInductionPCDMap.size() << ", "
            << totTE << std::endl;
}

void nEXOChargeReadoutDigitize::PrintHitMap()
{
  HitMap::iterator iter;
  for(iter=fHitMap.begin(); iter!=fHitMap.end(); iter++) {
    Int_t tileId = iter->first;
    nEXOChargeReadout* readout = iter->second;
    std::cout << "-------- tile: " << tileId << "---------" << std::endl;

    for(size_t i=0; i<readout->GetNumChargeChannels(); i++) {
      nEXOChargeElecChannel* chargeChannel = readout->GetChargeChannel(i);
      std::cout << "localId: " 
        << nEXOChannelMap::GetInstance()->GetLocalId(chargeChannel->GetChannelId())
        << "charge: " << chargeChannel->GetChannelCharge()
        << "time: " << chargeChannel->GetChannelTime()
        << std::endl;
    }
  }
}

std::map<nEXOCoordinateKey, nEXOMCPixelatedChargeDeposit*>* nEXOChargeReadoutDigitize::GetPCDMap()
{
  return &fPCDMap;
}

std::map<nEXOCoordinateKey, nEXOMCPixelatedChargeDeposit*>* nEXOChargeReadoutDigitize::GetInductionPCDMap()
{
  return &fInductionPCDMap;
}

std::map<Int_t, nEXOChargeReadout*>* nEXOChargeReadoutDigitize::GetHitMap() {
  return &fHitMap;
}

//nEXOChannelMap* nEXOChargeReadoutDigitize::GetChannelMap() {
//  return fChannelMap;
//}

void nEXOChargeReadoutDigitize::CalcDriftVelocity(Double_t field) {
  // digitized table from the Liquid Xenon page:
  // http://www.pd.infn.it/~conti/images/LXe/LXe_drift_velocity.gif. 
  // NOTE that, in this plot, the velocity is ~1.89mm/us @ 380V/cm, 
  // different from the EXO-200 measured data. 
  // Such table need be updated in future.
  const Int_t Npoints = 56;
  Double_t field_velocity[Npoints*2] = {
           10, 0.1,
      10.8809, 0.234476,
      11.5433, 0.246678,
      12.9914, 0.280096,
      13.5516, 0.292206,
      15.6429, 0.331729,
      17.7547, 0.370274,
      19.9821, 0.413325,
      21.5595, 0.442259,
      22.872 , 0.461323,
      24.6775, 0.489426,
      25.7414, 0.510587,
      28.0089, 0.546295,
      29.4642, 0.569879,
      36.0817, 0.674847,
      38.6027, 0.716002,
      44.9378, 0.805892,
      50.1505, 0.862092,
      60.8978, 0.978328, 
      69.7049, 1.06436,
      79.1149, 1.13844,
      97.7052, 1.26993,
      113.74 , 1.35806,
      129.094, 1.42802,
      146.522, 1.48883,
      164.904, 1.53915,
      191.967, 1.60439,
      225.365, 1.67229,
      260.145, 1.72848,
      307.994, 1.80153,
      377.168, 1.89326,
      465.794, 1.97265,
      595.002, 2.05486,
      792.821, 2.13982,
      1021.33, 2.24794,
      1282.8 , 2.32202,
      1557.7 , 2.37877,
      1940.03, 2.45731,
      2355.79, 2.51737,
      3112.62, 2.59936,
      3811.7 , 2.68552,
      4474.86, 2.70531,
      5342.85, 2.74823,
      6013.15, 2.76934,
      7059.32, 2.78975,
      7944.96, 2.78731,
      9171.07, 2.80819,
      10766.7, 2.82888, 
      13523  , 2.84829,
      15478.7, 2.84544,
      18795.8, 2.84135,
      21880.4, 2.86247,
      30669.7, 2.83107,
      38847.9, 2.85032,
      49206.9, 2.79721,
      64468.7, 2.74441
  };
  TGraph* graph = new TGraph();
  for(Int_t i=0; i<Npoints; i++) {
    graph->SetPoint(i, field_velocity[i*2], field_velocity[i*2+1]);
  }
  fDriftVelocity = graph->Eval(field)*CLHEP::mm/CLHEP::microsecond;
}

void nEXOChargeReadoutDigitize::PrintParameters()
{
  std::cout << "fFastDiffusionSim: " << fFastDiffusionSim << std::endl;
  std::cout << "fInductionSim: " << fInductionSim << std::endl;
  std::cout << "fElectronLifetime (us): " << fElectronLifetime/CLHEP::microsecond << std::endl; // us
  std::cout << "fDiffusionCoef (cm^2/s): " << fDiffusionCoef/(CLHEP::cm*CLHEP::cm/CLHEP::second) << std::endl; // cm^2/s
  std::cout << "Diffusion Spread with 500us drift time (mm): "
            << TMath::Sqrt(fDiffusionCoef * 500*CLHEP::microsecond)/CLHEP::mm << endl;
  std::cout << "Diffusion Spread with " << nEXODigiAnalysis::GetInstance()->GetAnodeZ()/fDriftVelocity/CLHEP::microsecond << "us drift time (mm): "
            << TMath::Sqrt(fDiffusionCoef * nEXODigiAnalysis::GetInstance()->GetAnodeZ()/fDriftVelocity)/CLHEP::mm << endl;
  std::cout << "fNoiseSigma: " << fNoiseSigma << std::endl;
  std::cout << "Padsize: " << nEXOChannelMap::GetInstance()->GetPadSize()  << std::endl;
  std::cout << "fDriftVelocity (mm/us): " << fDriftVelocity/(CLHEP::mm/CLHEP::microsecond) << std::endl;
  std::cout << "fSeed: " << fSeed << std::endl;
  std::cout << "fDebug: " << fDebug << std::endl;
  std::cout << "fSamplingInterval for signal calc (us): " << fSamplingInterval/CLHEP::microsecond << std::endl; // us
  std::cout << "fSaveWaveform: " << fSaveWaveform << std::endl;
  std::cout << "PCD T size: " << MCCHARGE_PIXEL_TIME/fDriftVelocity/CLHEP::mm << " mm = " << MCCHARGE_PIXEL_TIME/CLHEP::microsecond << " us" << std::endl;
  std::cout << "PCD x, y size (mm): " << nEXOChannelMap::GetInstance()->GetMCCHARGE_PIXEL_SIZE()/CLHEP::mm << std::endl;
  std::cout << "pad size (mm): " << nEXOChannelMap::GetInstance()->GetPadSize()/CLHEP::mm << std::endl;
  nEXOCoordinateKey test(0,0,0,0);
  std::cout << "PCD z scaling:" << test.GetPixellationSizeZScaling() << endl;
  std::cout << "PCD z size (mm): " << nEXOChannelMap::GetInstance()->GetMCCHARGE_PIXEL_SIZE()/CLHEP::mm*test.GetPixellationSizeZScaling()  << std::endl;
}
