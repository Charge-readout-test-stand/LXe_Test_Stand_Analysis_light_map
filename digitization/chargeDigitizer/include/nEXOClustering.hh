#include "nEXOUtilities/nEXOChargeCluster.hh"
//#include "nEXOUtilities/nEXOCluster.hh"
#include "nEXOUtilities/nEXOEventData.hh"
#include "nEXOUtilities/SystemOfUnits.hh"
#include <deque>
#include <iostream>
#include "TMath.h"
#include "TTree.h"
#include "TFile.h"
#include "nEXOChargeReadoutDigitize.hh"
class nEXOEventData;

class nEXOClustering
{
  public:
    nEXOClustering();
    virtual ~nEXOClustering();

    bool CreateScintillationClusters(nEXOEventData* ED);
    bool CreateChargeClusters(nEXOEventData* ED);
    void PrintChargeClusters(nEXOEventData* ED);
    void StatsChargeClusters(nEXOEventData* ED);

  public:
    double fXMatchTime;          //Time used for X- channels clustering in CLHEP standard units
    double fYMatchTime;          //Time used for Y- channels clustering in CLHEP standard units
    double fSiPMMatchTime;       //Time used for SiPM channels clustering in CLHEP standard units
    double fInductionMatchTime;  //Time used for induction channels clustering in CLHEP standard units
	 int fChannelNumOnTile;
	 double fNoiseSigma;
	 double zcut;
	 double xycut;
	 double noisecut;
	 double xposition[265];
	 double xchanneltime[265];
	 double xchannelcharge[265];
	 double xptile[265];
	 int numxchannel;
	 double xps[100];
	 double yptiles[100];
	 double RMSyptiles[100];
	 double xtime[100];
	 double xtimeRMS[100];
	 double RMSx[100];
	 double totalchargex[100];
	 int xchannelnum[100];
	 int xcluster[100];
	 double yposition[265];
	 double ychanneltime[265];
	 double ychannelcharge[265];
	 double yptile[265];
	 int numychannel;
	 double yps[100];
	 double xptiles[100];
	 double RMSxptiles[100];
	 double ytime[100];
	 double ytimeRMS[100];
	 double RMSy[100];
	 double totalchargey[100];
	 int ychannelnum[100];
	 int ycluster[100];
	 int ss_evt;



	 void SetCutTime(Double_t CutTime){ 
		 fXMatchTime = CutTime;
		 fYMatchTime = CutTime;
	 }
};
