#include "PdParameters.hh"

PdParameters* PdParameters::fParameters = 0;

PdParameters* PdParameters::GetInstance() {
  if(fParameters == 0) fParameters = new PdParameters();
  return fParameters;
}

PdParameters::PdParameters()
{
  fNSiPMPads = 92*300;
  fNSiPMPadsAlongZ = 92;
  fNSiPMPadsAlongPhi = 300;

  fRiseTime = 2;
  fDecayTime = 10;

  //for Novosibirsk function
  fNovosibirskMean = 17.3;
  fNovosibirskSigma = 8.4;
  fNovosibirskTail = -0.43;
}

PdParameters::~PdParameters()
{
}

void PdParameters::SetSiPMQE(double qe)
{
  fQuantumEfficiency = qe;
}

void PdParameters::SetDarkRate(double rate)
{
  fDarkRate = rate;
}

void PdParameters::SetReadoutWindow(double win)
{
  fReadoutWindow = win;
}

void PdParameters::SetIntegralWindow(double win)
{
  fIntegralWindow = win;
}

void PdParameters::SetNChannels(int chan)
{
  fNChannels = chan;
}

void PdParameters::SetLambda(double l)
{
  fLambda = l;
}

void PdParameters::SetENCs(double e)
{
  fENCs = e;
}

void PdParameters::SetCrossTalkProb(double p)
{
  fCrossTalkProb = p;
}

void PdParameters::SetCrossTalkFlag(int f)
{
  fCrossTalkFlag = f;
}

void PdParameters::SetCorrAvalancheFlag(int f)
{
  fCorrAvalancheFlag = f;
}

void PdParameters::SetTrapTauFast(double t)
{
  fTrapTauFast = t;
}

void PdParameters::SetTrapTauSlow(double t)
{
  fTrapTauSlow = t;
}

void PdParameters::SetSiPMGainFluct(double g)
{
  fSiPMGainFluct = g;
}

void PdParameters::SetEnergy(double e)
{
  fEnergy = e;
}

void PdParameters::SetSeed(unsigned int s)
{
  fSeed = s;
}

void PdParameters::SetRiseTime(double t)
{
  fRiseTime = t;
}

void PdParameters::SetDecayTime(double t)
{
  fDecayTime = t;
}

void PdParameters::SetNovosibirskMean(double m)
{
  fNovosibirskMean = m;
}

void PdParameters::SetNovosibirskSigma(double s)
{
  fNovosibirskSigma = s;
}

void PdParameters::SetNovosibirskTail(double t)
{
  fNovosibirskTail = t;
}

void PdParameters::SetNSiPMPads(int n)
{
  fNSiPMPads = n;
}

void PdParameters::SetNSiPMPadsAlongZ(int n)
{
  fNSiPMPadsAlongZ = n;
}

void PdParameters::SetNSiPMPadsAlongPhi(int n)
{
  fNSiPMPadsAlongPhi = n;
}

double PdParameters::GetSiPMQE()
{
  return fQuantumEfficiency;
}

double PdParameters::GetDarkRate()
{
  return fDarkRate;
}

double PdParameters::GetReadoutWindow()
{
  return fReadoutWindow;
}

double PdParameters::GetIntegralWindow()
{
  return fIntegralWindow;
}

int PdParameters::GetNChannels()
{
  return fNChannels;
}

double PdParameters::GetLambda()
{
  return fLambda;
}

double PdParameters::GetENCs()
{
  return fENCs;
}

double PdParameters::GetCrossTalkProb()
{
  return fCrossTalkProb;
}

int PdParameters::GetCrossTalkFlag()
{
  return fCrossTalkFlag;
}

int PdParameters::GetCorrAvalancheFlag()
{
  return fCorrAvalancheFlag;
}

double PdParameters::GetTrapTauFast()
{
  return fTrapTauFast;
}

double PdParameters::GetTrapTauSlow()
{
  return fTrapTauSlow;
}

double PdParameters::GetSiPMGainFluct()
{
  return fSiPMGainFluct;
}

double PdParameters::GetEnergy()
{
  return fEnergy;
}

unsigned int PdParameters::GetSeed()
{
  return fSeed;
}

double PdParameters::GetRiseTime()
{
  return fRiseTime;
}

double PdParameters::GetDecayTime()
{
  return fDecayTime;
}

double PdParameters::GetNovosibirskMean()
{
  return fNovosibirskMean;
}

double PdParameters::GetNovosibirskSigma()
{
  return fNovosibirskSigma;
}

double PdParameters::GetNovosibirskTail()
{
  return fNovosibirskTail;
}

int PdParameters::GetNSiPMPads()
{
  return fNSiPMPads;
}

int PdParameters::GetNSiPMPadsAlongZ()
{
  return fNSiPMPadsAlongZ;
}

int PdParameters::GetNSiPMPadsAlongPhi()
{
  return fNSiPMPadsAlongPhi;
}
