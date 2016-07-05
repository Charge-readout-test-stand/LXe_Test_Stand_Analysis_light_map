#include "PdParameters.hh"
#include "PdSiPMHit.hh"
#include "PdSiPMEffect.hh"
#include "PdFeeChannel.hh"
#include "PdFeeResponse.hh"
#include "PdRootIO.hh"

#include "TObjArray.h"
#include "TObject.h"

#include <iostream>

using namespace std;

void ProcessArgs(int argc, char** argv, char** inputfilename, char** outfilename, int* nevents,
                 double* siPMQE, double* darkrate, double* readoutwindow, double* integralWin, int* nchannels, double* lambda,
                 double* ENCs, double* crosstalkprob, int* crosstalkflag, int* corravalancheflag, 
                 double* fasttau, double* slowtau, double* gainfluct, double* energy, unsigned int* seed);

void Usage();

int main(int argc, char** argv)
{
  //===============================================
  //*** set default values for input parameters ***
  //===============================================
  char* inputfilename = NULL;
  char* outfilename = NULL;

  //number of events need to be processed, "-1" indicates looping over all events.
  int nevents = 1000000;
  //the quantum efficiency of SiPM, need to add wavelength dependence in future.
  double siPMQE = 0.1;
  //the dark rate of each SiPM pad, gotten from Hz/mm^2 and pad size.
  double darkrate = 1000;
  //readout window length of each event.
  double readoutwindow = 500;
  //the integral window used to integrate waveform.
  double integralwindow = 500;
  //number of electronics channels, each channel includes a set of pads.
  int nchannels = 1000;
  //lambda, which is used to control the probability of correlated avalanches.
  double lambda = 0.2;
  //equivalent electrnoics noise in each channel.
  double ENCs = 0.1;
  //the probability of photon cross talk.
  double crosstalkprob = 0.01;
  //if apply cross talk effect in photo-detector digitizer.
  int crosstalkflag = 1;
  //if apply correlated avalanche effect in photo-detector digitizer.
  int corravalancheflag = 1;
  //the time structure of correlated avalanches
  double fasttau = 50;
  double slowtau = 100;
  //the gain fluctuation of each SiPM.
  double gainfluct = 0.1;
  //true energy of simulated events.
  double energy = 2500;
  //random seed.
  unsigned int seed = 1000;

  //===============================================
  //************** Process args *******************
  //===============================================
  ProcessArgs(argc, argv, &inputfilename, &outfilename, &nevents, &siPMQE,
              &darkrate, &readoutwindow, &integralwindow, &nchannels, &lambda, &ENCs, &crosstalkprob,
              &crosstalkflag, &corravalancheflag, &fasttau, &slowtau, &gainfluct, &energy, &seed);

  PdParameters::GetInstance()->SetSiPMQE(siPMQE);
  PdParameters::GetInstance()->SetDarkRate(darkrate);
  PdParameters::GetInstance()->SetReadoutWindow(readoutwindow);
  PdParameters::GetInstance()->SetIntegralWindow(integralwindow);
  PdParameters::GetInstance()->SetNChannels(nchannels);
  PdParameters::GetInstance()->SetLambda(lambda);
  PdParameters::GetInstance()->SetENCs(ENCs);
  PdParameters::GetInstance()->SetCrossTalkProb(crosstalkprob);
  PdParameters::GetInstance()->SetCrossTalkFlag(crosstalkflag);
  PdParameters::GetInstance()->SetCorrAvalancheFlag(corravalancheflag);
  PdParameters::GetInstance()->SetTrapTauFast(fasttau);
  PdParameters::GetInstance()->SetTrapTauSlow(slowtau);
  PdParameters::GetInstance()->SetSiPMGainFluct(gainfluct);
  PdParameters::GetInstance()->SetEnergy(energy);
  PdParameters::GetInstance()->SetSeed(seed);

  //===============================================
  //******* open input files and read TTree *******
  //===============================================
  PdRootIO::GetInstance()->OpenInputFiles(inputfilename);
  PdRootIO::GetInstance()->CreateOutputFileAndTree(outfilename, "evt");

  //===============================================
  //************** process events *****************
  //===============================================

  int nentries = PdRootIO::GetInstance()->GetEntries();

  int nprocess = 0;

  if(nevents < 0 || nevents > nentries) nprocess = nentries;
  else if(nevents < nentries) nprocess = nevents;
  else nprocess = nentries;

  PdSiPMEffect* siPMEffect = new PdSiPMEffect();
  PdFeeResponse* feeRes = new PdFeeResponse();

  for(int i = 0; i < nprocess; i++) {
    TObjArray* objArr = PdRootIO::GetInstance()->GetEvent(i);
    TObjArray* newobjArr = siPMEffect->ApplySiPMEffect(objArr);
    TObjArray* pulseArr = feeRes->ApplyFeeResponse(newobjArr);
    PdRootIO::GetInstance()->WriteEvent(pulseArr);
  }

  //===============================================
  //************ write out event tree *************
  //===============================================
  PdRootIO::GetInstance()->WriteFile();

  return 0;
}

void ProcessArgs(int argc, char** argv, char** inputfilename, char** outfilename, int* nevents,
                 double* siPMQE, double* darkrate, double* readoutwindow, double* integralwindow, int* nchannels, double* lambda,
                 double* ENCs, double* crosstalkprob, int* crosstalkflag, int* corravalancheflag,
                 double* fasttau, double* slowtau, double* gainfluct, double* energy, unsigned int* seed)
{
  int i;
  for( i=1 ; i< argc ; i++ ) {
    if(strcmp(argv[i], "-i")==0) {
      i++;
      *inputfilename = new char[strlen(argv[i]) +1];
      strcpy(*inputfilename, argv[i]);
    } else if(strcmp(argv[i], "-o")==0) {
      i++;
      *outfilename = new char[strlen(argv[i]) +1];
      strcpy(*outfilename, argv[i]);
    } else if(strcmp(argv[i], "-n")==0) {
      i++;
      sscanf(argv[i], "%d", nevents);
    } else if(strcmp(argv[i], "-qe")==0) {
      i++;
      sscanf(argv[i], "%lg", siPMQE);
    } else if(strcmp(argv[i], "-darkrate")==0) {
      i++;
      sscanf(argv[i], "%lg", darkrate);
    } else if(strcmp(argv[i], "-rw")==0){
      i++;
      sscanf(argv[i], "%lg", readoutwindow);
    } else if(strcmp(argv[i], "-iw")==0){
      i++;
      sscanf(argv[i], "%lg", integralwindow);
    } else if(strcmp(argv[i], "-nc")==0){
      i++;
      sscanf(argv[i], "%d", nchannels);
    } else if(strcmp(argv[i], "-lambda")==0){
      i++;
      sscanf(argv[i], "%lg", lambda);
    } else if(strcmp(argv[i], "-enc")==0) {
      i++;
      sscanf(argv[i], "%lg", ENCs);
    } else if(strcmp(argv[i], "-ctprob")==0) {
      i++;
      sscanf(argv[i], "%lg", crosstalkprob);
    } else if(strcmp(argv[i], "-ctflag")==0) {
      i++;
      sscanf(argv[i], "%d", crosstalkflag);
    } else if(strcmp(argv[i], "-caflag")==0) {
      i++;
      sscanf(argv[i], "%d", corravalancheflag);
    } else if(strcmp(argv[i], "-ftau")==0) {
      i++;
      sscanf(argv[i], "%lg", fasttau);
    } else if(strcmp(argv[i], "-stau")==0) {
      i++;
      sscanf(argv[i], "%lg", slowtau);
    } else if(strcmp(argv[i], "-gf")==0) {
      i++;
      sscanf(argv[i], "%lg", gainfluct);
    } else if(strcmp(argv[i], "-energy")==0) {
      i++;
      sscanf(argv[i], "%lg", energy);
    } else if(strcmp(argv[i], "-seed")==0) {
      i++;
      sscanf(argv[i], "%d", seed);
    } else if (strcmp(argv[i],"-h")==0) {
      i++;
      Usage();
      exit(0);
    } else {
      Usage();
      exit(0);
    }
  }
}

void Usage() {
  printf("digitizer.exe [-i inputfile] [-o outfile] [-n nevents] [-qe siPMQE] [-darkrate darkrate] [-rw readoutwindow] [-iw integralwindow] [-nc nchannels] [-lambda lambda] [-enc ENCs] [-rt risetime] [-dt decaytime] [-ctprob crosstalkprob] [-ctflag crosstalkflag] [-caflag corravalancheflag] [-ftau fasttau] [-stau slowtau] [-gf gainfluct] [-energy energy] [-seed seed] [-h]\n");
}
