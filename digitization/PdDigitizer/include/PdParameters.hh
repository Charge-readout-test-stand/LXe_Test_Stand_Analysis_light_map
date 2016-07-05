//**********************************************************************
//  A class is used to control parameters used in PdDigitizer.
//  caogf@ihep.ac.cn
//  2014.07.25
//**********************************************************************

#ifndef PDPARAMETERS_H
#define PDPARAMETERS_H

#include <cstdlib>

class PdParameters
{
public:
  static PdParameters* GetInstance();

  PdParameters();
  ~PdParameters();

  void SetSiPMQE(double qe);
  void SetDarkRate(double rate);
  void SetReadoutWindow(double win);
  void SetIntegralWindow(double win);
  void SetNChannels(int chan);
  void SetLambda(double lambda);
  void SetENCs(double enc);
  void SetCrossTalkProb(double prob);
  void SetCrossTalkFlag(int flag);
  void SetCorrAvalancheFlag(int flag);
  void SetTrapTauFast(double tauf);
  void SetTrapTauSlow(double taus);
  void SetSiPMGainFluct(double g);
  void SetEnergy(double e);
  void SetSeed(unsigned int s);
  void SetRiseTime(double rt);
  void SetDecayTime(double rt);
  void SetNovosibirskMean(double m);
  void SetNovosibirskSigma(double s);
  void SetNovosibirskTail(double t);
  void SetNSiPMPads(int n);
  void SetNSiPMPadsAlongZ(int n);
  void SetNSiPMPadsAlongPhi(int n);

  double GetSiPMQE();
  double GetDarkRate();
  double GetReadoutWindow();
  double GetIntegralWindow();
  int GetNChannels();
  double GetLambda();
  double GetENCs();
  double GetCrossTalkProb();
  int GetCrossTalkFlag();
  int GetCorrAvalancheFlag();
  double GetTrapTauFast();
  double GetTrapTauSlow();
  double GetSiPMGainFluct();
  double GetEnergy();
  unsigned int GetSeed();
  double GetRiseTime();
  double GetDecayTime();
  double GetNovosibirskMean();
  double GetNovosibirskSigma();
  double GetNovosibirskTail();
  int GetNSiPMPads();
  int GetNSiPMPadsAlongZ();
  int GetNSiPMPadsAlongPhi();

private:

  static PdParameters* fParameters;

  double fQuantumEfficiency;
  double fDarkRate;
  double fReadoutWindow;
  double fIntegralWindow;
  int fNSiPMPads;
  int fNSiPMPadsAlongZ;
  int fNSiPMPadsAlongPhi;
  int fNChannels;
  double fLambda;
  double fENCs;
  double fCrossTalkProb;
  double fTrapTauFast;
  double fTrapTauSlow;
  int fCrossTalkFlag;
  int fCorrAvalancheFlag;
  double fSiPMGainFluct;
  double fEnergy;
  unsigned int fSeed;
  double fRiseTime;
  double fDecayTime;
  double fNovosibirskMean;
  double fNovosibirskSigma;
  double fNovosibirskTail;
};

#endif
