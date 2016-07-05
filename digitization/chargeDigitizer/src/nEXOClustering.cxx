#ifndef nEXOClustering_hh
#include "nEXOClustering.hh"
#endif

#ifndef nEXOChargeCluster_hh
#include "nEXOUtilities/nEXOChargeCluster.hh"
#endif

#ifndef nEXOEventData_hh
#include "nEXOUtilities/nEXOEventData.hh"
#endif

#ifndef SystemOfUnits_hh
#include "nEXOUtilities/SystemOfUnits.hh"
#endif

#ifndef nEXODimensions_hh
#include "nEXOUtilities/nEXODimensions.hh"
#endif

#ifndef nEXOChannelMap_hh
#include "nEXOUtilities/nEXOChannelMap.hh"
#endif

#include<iostream>
#include<map>
#include "TMath.h"
#include "nEXOChargeReadoutDigitize.hh"
using namespace std;
using namespace CLHEP;

class nEXOChargeReadout;

nEXOClustering::nEXOClustering()
: fXMatchTime(SAMPLE_TIME),
  fYMatchTime(SAMPLE_TIME),
 // fXMatchTime(cuttime),
 // fYMatchTime(cuttime),
  fSiPMMatchTime(3.5*CLHEP::micrometer),
  fInductionMatchTime(3.5*SAMPLE_TIME)
{
}

nEXOClustering::~nEXOClustering()
{
}

bool nEXOClustering::CreateScintillationClusters(nEXOEventData *ED)
{
  return kTRUE;
}


bool nEXOClustering::CreateChargeClusters(nEXOEventData *ED)
{
  //std::cout << "NumChargeReadouts: " << ED->GetNumChargeReadouts() << std::endl;
 
  if(ED->GetNumChargeReadouts()==0) {
    // this event has no charge readout
    return kTRUE;
  }
  fNoiseSigma = nEXOChargeReadoutDigitize::GetInstance()->GetNoise();
  fChannelNumOnTile = nEXOChannelMap::GetInstance()->GetChannelNumOnTile()/2;
  zcut = 1000;
  xycut = nEXOChannelMap::GetInstance()->GetPadSize();
  noisecut = 5*fNoiseSigma;
  nEXOChargeCluster* cl = 0;
  // loop the tile in the event
  std::map<Double_t, nEXOChargeElecChannel*> timeSeq;
  std::map<Double_t, nEXOChargeElecChannel*> inductionTimeSeq;
  numxchannel = 0;
  numychannel = 0;
  for(size_t i=0; i<ED->GetNumChargeReadouts(); i++){
	  //cycle different tiles in one event
    nEXOChargeReadout* cro = ED->GetChargeReadout(i);
   // std::cout << "NumChargeChannels: " << cro->GetNumChargeChannels() << std::endl;
    for(size_t k=0; k<cro->GetNumChargeChannels(); k++) {
		 //cycle different channels in one tile
      nEXOChargeElecChannel* cec = cro->GetChargeChannel(k);

      // channel position, charge
      Double_t time = cec->GetChannelTime();
      Double_t charge = cec->GetChannelCharge();
      Double_t inductionAmp = cec->GetChannelInductionAmplitude();
      //std::cout << "tileId, localId, time, charge, inductionAmplitude: " 
      //          << nEXOChannelMap::GetInstance()->GetTileId(cec->fChannelId) << ", "
      //          << nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId) << ", "
      //          << time << ", " << charge << ", " << inductionAmp << std::endl;
      if( charge!=0. && inductionAmp==0. )
        timeSeq.insert(std::pair<Double_t, nEXOChargeElecChannel*>(time, cec));
     
      if( inductionAmp!=0.) 
        inductionTimeSeq.insert(std::pair<Double_t, nEXOChargeElecChannel*>(time, cec));
		
		//distinct to X/Y channel
		Double_t tileId = nEXOChannelMap::GetInstance()->GetTileId(cec->fChannelId);
		Double_t localId = nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId);
		if(charge>noisecut){
			if(nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId)<fChannelNumOnTile){
				xposition[numxchannel] = nEXOChannelMap::GetInstance()->GetChannelPosition(tileId, localId).x();
				yptile[numxchannel] = nEXOChannelMap::GetInstance()->GetChannelPosition(tileId, localId).y();
				xchanneltime[numxchannel] = time;
				xchannelcharge[numxchannel] = charge;
				numxchannel++;
			}
			if(nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId)>=fChannelNumOnTile){
				yposition[numychannel] = nEXOChannelMap::GetInstance()->GetChannelPosition(tileId, localId).y();
				xptile[numychannel] = nEXOChannelMap::GetInstance()->GetChannelPosition(tileId, localId).x();
				ychanneltime[numychannel] = time;
				ychannelcharge[numychannel] = charge;
				numychannel++;
			}
		}
    }
  }
  //cout<<"X channels:	"<<numxchannel<<endl
	//  <<"Y channels:	"<<numychannel<<endl;
  if(numxchannel+numychannel<1){
	  return kTRUE;
  }
  //X/Y channel ranked by x channel position/y channel position
  Double_t exchange=0;
  for(Int_t ix=0;ix<numxchannel-1;ix++){
	  for(Int_t jx=ix+1;jx<numxchannel;jx++){
		  if(xposition[ix]<=xposition[jx])   continue;
		  exchange = xposition[ix];
		  xposition[ix] = xposition[jx];
		  xposition[jx] = exchange;

		  exchange = yptile[ix];
		  yptile[ix] = yptile[jx];
		  yptile[jx] = exchange;

		  exchange = xchanneltime[ix];
		  xchanneltime[ix] = xchanneltime[jx];
		  xchanneltime[jx] = exchange;

		  exchange = xchannelcharge[ix];
		  xchannelcharge[ix] = xchannelcharge[jx];
		  xchannelcharge[jx] = exchange;
	  }
  }
  for(Int_t iy=0;iy<numychannel-1;iy++){
	  for(Int_t jy=iy+1;jy<numychannel;jy++){
		  if(yposition[iy]<=yposition[jy])	continue;
		  exchange = yposition[iy];
		  yposition[iy] = yposition[jy];
		  yposition[jy] = exchange;

		  exchange = xptile[iy];
		  xptile[iy] = xptile[jy];
		  xptile[jy] = exchange;

		  exchange = ychanneltime[iy];
		  ychanneltime[iy] = ychanneltime[jy];
		  ychanneltime[jy] = exchange;

		  exchange = ychannelcharge[iy];
		  ychannelcharge[iy] = ychannelcharge[jy];
		  ychannelcharge[jy] = exchange;
	  }
  }
  for(Int_t ii=0;ii<100;ii++){
	  xps[ii]=0;
	  yptiles[ii]=0;
	  yps[ii]=0;
	  xptiles[ii]=0;
	  totalchargex[ii]=0;
	  totalchargey[ii]=0;
	  xchannelnum[ii]=0;
	  ychannelnum[ii]=0;
	  RMSx[ii]=0;
	  RMSxptiles[ii]=0;
	  RMSy[ii]=0;
	  RMSyptiles[ii]=0;
	  xcluster[ii]=0;
	  ycluster[ii]=0;
	  xtime[ii]=0;
	  ytime[ii]=0;
	  xtimeRMS[ii]=0;
	  ytimeRMS[ii]=0;
  }

  //clustering X/Y channel
  Int_t XCluster=0, YCluster=0, kx=0, ky=0;
  for(Int_t ix=0;ix<numxchannel;ix++){
	  if(kx==0){
		  XCluster++;
		  xcluster[XCluster]=ix;
		  xps[XCluster] = xposition[ix];
		  yptiles[XCluster] = yptile[ix];
		  xtime[XCluster] = xchanneltime[ix];
		  totalchargex[XCluster] = xchannelcharge[ix];
		  xchannelnum[XCluster] = 1;
		  kx=1;
	  }
	  if(ix+1>=numxchannel)	break;
	  if((fabs(xposition[ix+1] - xposition[ix])<=xycut)&&fabs(xchanneltime[ix] - xchanneltime[ix+1])<=zcut){
		  xchannelnum[XCluster]++;
		  xps[XCluster] = (xps[XCluster]*totalchargex[XCluster] + xposition[ix+1]*xchannelcharge[ix+1])/(totalchargex[XCluster] + xchannelcharge[ix+1]);
		  yptiles[XCluster] = (yptiles[XCluster]*totalchargex[XCluster] + yptile[ix+1]*xchannelcharge[ix+1])/(totalchargex[XCluster] + xchannelcharge[ix+1]);
		  xtime[XCluster] = (xtime[XCluster]*totalchargex[XCluster] + xchanneltime[ix+1]*xchannelcharge[ix+1])/(totalchargex[XCluster] + xchannelcharge[ix+1]);
		  totalchargex[XCluster] = totalchargex[XCluster] + xchannelcharge[ix+1];
	  }
	  else{
		  kx=0;
	  }
  }
  xcluster[XCluster+1] = numxchannel;

  for(Int_t iy=0;iy<numychannel;iy++){
	  if(ky==0){
		  YCluster++;
		  ycluster[YCluster]=iy;
		  yps[YCluster] = yposition[iy];
		  xptiles[YCluster] = xptile[iy];
		  ytime[YCluster] = ychanneltime[iy];
		  totalchargey[YCluster] = ychannelcharge[iy];
		  ychannelnum[YCluster] = 1;
		  ky=1;
	  }
	  if(iy+1>=numychannel)	break;
	  if((fabs(yposition[iy+1] - yposition[iy])<=xycut)&&fabs(ychanneltime[iy] - ychanneltime[iy+1])<=zcut){
		  ychannelnum[YCluster]++;
		  yps[YCluster] = (yps[YCluster]*totalchargey[YCluster] + yposition[iy+1]*ychannelcharge[iy+1])/(totalchargey[YCluster] + ychannelcharge[iy+1]);
		  xptiles[YCluster] = (xptiles[YCluster]*totalchargey[YCluster] + xptile[iy+1]*ychannelcharge[iy+1])/(totalchargey[YCluster] + ychannelcharge[iy+1]);
		  ytime[YCluster] = (ytime[YCluster]*totalchargey[YCluster] + ychanneltime[iy+1]*ychannelcharge[iy+1])/(totalchargey[YCluster] + ychannelcharge[iy+1]);
		  totalchargey[YCluster] = totalchargey[YCluster] + ychannelcharge[iy+1];
	  }
	  else{
		  ky=0;
	  }
  }
  ycluster[YCluster+1] = numychannel;

  //X/Y cluster RMS
  for(Int_t ix=1;ix<=XCluster;ix++){
	  for(Int_t jx=xcluster[ix];jx<xcluster[ix+1];jx++){
		  RMSx[ix] += (xps[ix]-xposition[jx])*(xps[ix]-xposition[jx]);
		  RMSyptiles[ix] += (yptiles[ix]-yptile[jx])*(yptiles[ix]-yptile[jx]);
		  xtimeRMS[ix] += (xtime[ix]-xchanneltime[jx])*(xtime[ix]-xchanneltime[jx]);
	  }
	  RMSx[ix] = sqrt(RMSx[ix]/(xcluster[ix+1]-xcluster[ix]));
	  RMSyptiles[ix] = sqrt(RMSyptiles[ix]/(xcluster[ix+1]-xcluster[ix]));
	  xtimeRMS[ix] = sqrt(xtimeRMS[ix]/(xcluster[ix+1]-xcluster[ix]));
  }
  for(Int_t iy=1;iy<=YCluster;iy++){
	  for(Int_t jy=ycluster[iy];jy<ycluster[iy+1];jy++){
		  RMSy[iy]+= (yps[iy]-yposition[jy])*(yps[iy]-yposition[jy]);
		  RMSxptiles[iy] += (xptiles[iy]-xptile[jy])*(xptiles[iy]-xptile[jy]);
		  ytimeRMS[iy]+= (ytime[iy]-ychanneltime[jy])*(ytime[iy]-ychanneltime[jy]);
	  }
	  RMSy[iy] = sqrt(RMSy[iy]/(ycluster[iy+1]-ycluster[iy]));
	  RMSxptiles[iy] = sqrt(RMSxptiles[iy]/(ycluster[iy+1]-ycluster[iy]));
	  ytimeRMS[iy] = sqrt(ytimeRMS[iy]/(ycluster[iy+1]-ycluster[iy]));
  }

  if(XCluster>0||YCluster>0){
	  //SS event choice
	  if((XCluster>1||YCluster>1)||(XCluster==1&&YCluster==1&&fabs(xtime[1]-ytime[1])>1000))
		  ss_evt=0;
	  else
		  ss_evt=1;

	  //save X/Y clusters
	  for(Int_t ix=1;ix<=XCluster;ix++){
		  cl = ED->GetNewChargeCluster();
		  cl->fT_cluster = xtime[ix];
		  cl->fTRMS_cluster = xtimeRMS[ix];
		  cl->fX_cluster = xps[ix];
		  cl->fXRMS_cluster = RMSx[ix];
		  cl->fY_cluster = yptiles[ix];
		  cl->fYRMS_cluster = RMSyptiles[ix];
		  cl->fCharge_cluster = totalchargex[ix];
		  cl->fChannelsnum_cluster = xchannelnum[ix];
		  cl->fChanneltype_cluster = 'X';
	  }
	  for(Int_t iy=1;iy<=YCluster;iy++){
		  cl = ED->GetNewChargeCluster();
		  cl->fT_cluster = ytime[iy];
		  cl->fTRMS_cluster = ytimeRMS[iy];
		  cl->fY_cluster = yps[iy];
		  cl->fYRMS_cluster = RMSy[iy];
		  cl->fX_cluster = xptiles[iy];
		  cl->fXRMS_cluster = RMSxptiles[iy];
		  cl->fCharge_cluster = totalchargey[iy];
		  cl->fChannelsnum_cluster = ychannelnum[iy];
		  cl->fChanneltype_cluster = 'Y';
	  }
  }  



///////  if(timeSeq.size()==0) {
///////    // this event has no visible charge collection, e.g, below threshold
///////    return kTRUE;
///////  }
///////
///////
///////
///////  // clustering the channels that have real charge collection
///////  nEXOChargeCluster* cc = 0;
///////  nEXOChargeElecChannel* cec = 0;
///////  std::map<Double_t, nEXOChargeElecChannel*>::iterator it=timeSeq.begin();
///////  if(it!=timeSeq.end()) {
///////    // create the first charge cluster
///////    cc = ED->GetNewChargeCluster(); 
///////    cec = it->second;
///////    if( nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId)<fChannelNumOnTile) 
///////      cc->InsertXChannel(cec);
///////    else cc->InsertYChannel(cec);
///////  }
///////  it++;
///////
///////  for(; it!=timeSeq.end(); it++) {
///////    Double_t dT = it->first -  cec->GetChannelTime();
///////    // update the cec pointer to be the current charge signal
///////    cec = it->second;
///////    if(dT<fXMatchTime && nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId)<fChannelNumOnTile) {
///////      // if find X channel within match time, insert it into charge cluster
///////      cc->InsertXChannel(it->second);
///////    }
///////    else if(dT<fYMatchTime && nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId)>=fChannelNumOnTile) {
///////      // if find Y channel within match time, insert it into charge cluster
///////      cc->InsertYChannel(it->second);
///////    }
///////    else {
///////      // if not find any channel, create new charge cluster
///////      cc = ED->GetNewChargeCluster();
///////      if(nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId)<fChannelNumOnTile) cc->InsertXChannel(cec);
///////      else cc->InsertYChannel(cec);
///////    }
///////  }
///////
///////  // associate the induction channels to the charge collection channels
///////  for(size_t i=0; i<ED->GetNumChargeClusters(); i++) {
///////    cc = ED->GetChargeCluster(i);
///////    
///////    // get earliest and latest time of charge collection channels
///////    double earliestT = 1e6, latestT = 0.;
///////    for(size_t k=0; k<cc->GetNumXChannels(); k++) {
///////      cec = cc->GetXChargeSignalAt(k);
///////      if(cec->GetChannelTime() < earliestT) earliestT = cec->GetChannelTime();
///////      if(cec->GetChannelTime() > latestT) latestT = cec->GetChannelTime();
///////    }
///////    for(size_t k=0; k<cc->GetNumYChannels(); k++) {
///////      cec = cc->GetYChargeSignalAt(k);
///////      if(cec->GetChannelTime() < earliestT) earliestT = cec->GetChannelTime();
///////      if(cec->GetChannelTime() > latestT) latestT = cec->GetChannelTime();
///////    }
///////
///////    // associate the induction channels
///////    for(it=inductionTimeSeq.begin(); it!=inductionTimeSeq.end(); it++) {
///////      cec = it->second;
///////      double inductionT = cec->GetChannelTime();
///////      if( TMath::Abs( inductionT - earliestT)<fInductionMatchTime ||
///////            TMath::Abs( inductionT - latestT)<fInductionMatchTime )
///////        cc->InsertInductionChannel(cec);
///////    }
///////  }
///////
///////  timeSeq.clear();
///////  inductionTimeSeq.clear();
///////
///////  // check 
	PrintChargeClusters(ED);
///////
///////  // Calculate the position and energy of each charge cluster
///////  StatsChargeClusters(ED);
///////
  return kTRUE;
}
void nEXOClustering::PrintChargeClusters(nEXOEventData* ED) {
	//std::cout << "NumChargeClusters: " << ED->GetNumChargeClusters() << std::endl;
	//for(size_t i=0; i<ED->GetNumChargeClusters(); i++) {
	//	nEXOChargeCluster* cl = ED->GetChargeCluster(i);
	//	std::cout<< "-----" << i << "th charge cluster has -----"<<endl
	//	<<cl->fChanneltype_cluster<<" channels:	"<<cl->fChannelsnum_cluster<<endl
	//	<<"X: "<<cl->fX_cluster<<"	X RMS: "<<cl->fXRMS_cluster<<endl
	//	<<"Y: "<<cl->fY_cluster<<"	Y RMS: "<<cl->fYRMS_cluster<<endl
	//	<<"T: "<<cl->fT_cluster<<"	T RMS: "<<cl->fTRMS_cluster<<endl
	//	<<"Charge: "<<cl->fCharge_cluster<<std::endl;
	//}
}
//////void nEXOClustering::PrintChargeClusters(nEXOEventData* ED) {
//////  std::cout << "NumChargeClusters: " << ED->GetNumChargeClusters() << std::endl;
//////  for(size_t i=0; i<ED->GetNumChargeClusters(); i++) {
//////    nEXOChargeCluster* cc = ED->GetChargeCluster(i);
//////////	 std::cout<<"X:"<<cl->GetX()<<"	X RMS:"<<cl->GetXRMS()<<" Y:"<<cl->GetY()<<"	Y RMS:"<<cl->GetYRMS()<<"	T:"<<cl->GetT()<<"	T RMS:"<<cl->GetTRMS()<<"	Charge:"<<cl->GetCharge()<<"	Channel number:"<<cl->GetChannelsnum()<<"	Channel Type:"<<cl->GetChanneltype()<<std::endl;
//////    std::cout << "-----" << i << "th charge cluster has " 
//////              << cc->GetNumXChannels() << " XChannels, "
//////              << cc->GetNumYChannels() << " YChannels, " << std::endl;
//////    // check X- channels
//////    for(size_t k=0; k<cc->GetNumXChannels(); k++) {
//////      nEXOChargeElecChannel* cec = cc->GetXChargeSignalAt(k);
//////      std::cout << k << "th X-channel (tileId, localId, time, energy): " 
//////                << nEXOChannelMap::GetInstance()->GetTileId(cec->fChannelId) << ", "
//////                << nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId) << ", "
//////                << cec->GetChannelTime() << ", "
//////                << cec->GetChannelCharge() << std::endl;
//////      cc->fEnergy += cec->GetChannelCharge(); 
//////    }
//////    // check Y- channels
//////    for(size_t k=0; k<cc->GetNumYChannels(); k++) {
//////      nEXOChargeElecChannel* cec = cc->GetYChargeSignalAt(k);
//////      std::cout << k << "th Y-channel (tileId, localId, time, energy): " 
//////                << nEXOChannelMap::GetInstance()->GetTileId(cec->fChannelId) << ", "
//////                << nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId) << ", "
//////                << cec->fTime << ", "
//////                << cec->fChannelCharge << std::endl;
//////      cc->fEnergy += cec->GetChannelCharge(); 
//////    }
//////    // check induction channels
//////    for(size_t k=0; k<cc->GetNumInductionChannels(); k++) {
//////      nEXOChargeElecChannel* cec = cc->GetInductionSignalAt(k);
//////      std::cout << k << "th induction channel (tileId, localId, time, energy, inductionAmp): " 
//////                << nEXOChannelMap::GetInstance()->GetTileId(cec->fChannelId) << ", "
//////                << nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId) << ", "
//////                << cec->fTime << ", "
//////                << cec->fChannelCharge << ", " 
//////                << cec->fInductionAmplitude << std::endl;
//////    }
//////    std::cout << "charge cluster info (t, x, y, z, E): "
//////              << cc->fT << ", "
//////              << cc->fX << ", "
//////              << cc->fY << ", "
//////              << cc->fZ << ", "
//////              << cc->fEnergy << std::endl;
//////  }
//////}

//void nEXOClustering::StatsChargeClusters(nEXOEventData *ED) {
//  for(size_t i=0; i<ED->GetNumChargeClusters(); i++) {
//    nEXOChargeCluster* cc = ED->GetChargeCluster(i);
//    cc->fEnergy = 0.;
//    // X- channels
//    Double_t posX[33]={0.}, meanX = 0., rmsX = 0.;
//    for(size_t k=0; k<cc->GetNumXChannels(); k++) {
//      nEXOChargeElecChannel* cec = cc->GetXChargeSignalAt(k);
//      cc->fEnergy += cec->GetChannelCharge(); 
//      posX[k] = nEXOChannelMap::GetInstance()->GetChannelPosition(cec->fChannelId).x();
//      meanX += posX[k];
//    }
//    if(cc->GetNumXChannels()>1) { // if multiple X- channels
//      meanX = meanX/cc->GetNumXChannels();
//      for(size_t k=0; k<cc->GetNumXChannels(); k++) {
//        rmsX += (posX[k] - meanX)*(posX[k] - meanX);
//      }
//      rmsX = TMath::Sqrt(rmsX/(cc->GetNumXChannels()-1));
//      cc->fXRMS = rmsX;
//    }
//
//    // Y- channels
//    Double_t posY[33]={0.}, meanY = 0., rmsY = 0.;
//    for(size_t k=0; k<cc->GetNumYChannels(); k++) {
//      nEXOChargeElecChannel* cec = cc->GetYChargeSignalAt(k);
//      cc->fEnergy += cec->GetChannelCharge(); 
//      posY[k] = nEXOChannelMap::GetInstance()->GetChannelPosition(cec->fChannelId).y();
//      meanY += posY[k];
//    }
//    if(cc->GetNumYChannels()>1) { // if multiple Y- channels
//      meanY = meanY/cc->GetNumYChannels();
//      for(size_t k=0; k<cc->GetNumYChannels(); k++) {
//        rmsY += (posY[k] - meanY)*(posY[k] - meanY);
//      }
//      rmsY = TMath::Sqrt(rmsY/(cc->GetNumYChannels()-1));
//      cc->fYRMS = rmsY;
//    }
//  }
//}

void nEXOClustering::StatsChargeClusters(nEXOEventData* ED)
{
	for(size_t i=0; i<ED->GetNumChargeClusters(); i++) {
		nEXOChargeCluster* cl = ED->GetChargeCluster(i);

	}
}
//////void nEXOClustering::StatsChargeClusters(nEXOEventData* ED)
//////{
//////  for(size_t i=0; i<ED->GetNumChargeClusters(); i++) {
//////    nEXOChargeCluster* cc = ED->GetChargeCluster(i);
//////    cc->fEnergy = 0.;
//////    // X- channels
//////    Double_t posX[33]={0.}, meanX = 0., rmsX = 0., indX[33]={0.};
//////    for(size_t k=0; k<cc->GetNumXChannels(); k++) {
//////      nEXOChargeElecChannel* cec = cc->GetXChargeSignalAt(k);
//////      cc->fEnergy += cec->GetChannelCharge(); 
//////    }
//////
//////    // Y- channels
//////    Double_t posY[33]={0.}, meanY = 0., rmsY = 0., indY[33]={0.};
//////    for(size_t k=0; k<cc->GetNumYChannels(); k++) {
//////      nEXOChargeElecChannel* cec = cc->GetYChargeSignalAt(k);
//////      cc->fEnergy += cec->GetChannelCharge(); 
//////    }
//////    
//////    // Induction channels
//////    size_t NumICX = 0, NumICY = 0;
//////    for(size_t k=0; k<cc->GetNumInductionChannels(); k++) {
//////      nEXOChargeElecChannel* cec = cc->GetInductionSignalAt(k);
//////
//////      Int_t localId = nEXOChannelMap::GetInstance()->GetLocalId(cec->fChannelId);
//////      TString chanType = nEXOChannelMap::GetInstance()->GetChannelType(localId);
//////
//////      if(chanType=="X") {
//////        posX[NumICX] = nEXOChannelMap::GetInstance()->GetChannelPosition(cec->fChannelId).x();
//////        meanX += posX[NumICX];
//////        indX[NumICX] = cec->fInductionAmplitude;
//////        NumICX += 1;
//////      }
//////      if(chanType=="Y") {
//////        posY[NumICY] = nEXOChannelMap::GetInstance()->GetChannelPosition(cec->fChannelId).y();
//////        meanY += posY[NumICY];
//////        indY[NumICY] = cec->fInductionAmplitude;
//////        NumICY += 1;
//////      }
//////    }
//////    if(NumICX>1) { // if multiple X- channels
//////      double sumIndX = 0.;
//////      meanX = meanX/NumICX;
//////      for(size_t k=0; k<NumICX; k++) {
//////        //rmsX += (posX[k] - meanX)*(posX[k] - meanX);
//////        rmsX += (posX[k] - meanX)*(posX[k] - meanX)*indX[k];
//////        sumIndX += indX[k];
//////      }
//////      //rmsX = TMath::Sqrt(rmsX/(NumICX-1));
//////      rmsX = TMath::Sqrt(rmsX/sumIndX/(NumICX-1));
//////      cc->fXRMS = rmsX;
//////    }
//////    cc->fX= meanX;
//////    if(NumICY>1) { // if multiple Y- channels
//////      double sumIndY = 0.;
//////      meanY = meanY/NumICY;
//////      for(size_t k=0; k<NumICY; k++) {
//////        //rmsY += (posY[k] - meanY)*(posY[k] - meanY);
//////        rmsY += (posY[k] - meanY)*(posY[k] - meanY)*indY[k];
//////        sumIndY += indY[k];
//////      }
//////      //rmsY = TMath::Sqrt(rmsY/(NumICY-1));
//////      rmsY = TMath::Sqrt(rmsY/sumIndY/(NumICY-1));
//////      cc->fYRMS = rmsY;
//////    }
//////    cc->fY= meanY;
//////  }
//////}
