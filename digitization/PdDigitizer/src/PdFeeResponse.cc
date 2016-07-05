#include "PdFeeResponse.hh"
#include "PdFeeChannel.hh"
#include "PdSiPMHit.hh"
#include "PdParameters.hh"
#include <vector>

#include "TMath.h"
#include "TF1.h"
#include "TApplication.h"
#include "TRandom.h"

using namespace std;

PdFeeResponse::PdFeeResponse() 
{
}

PdFeeResponse::~PdFeeResponse() 
{
}

double PdFeeResponse::SiPMWaveform(double* x, double* par)
{

  /*
  double val = 1/par[0]*TMath::Exp(-(x[0]-par[2])/par[0]) - 1/par[1]*TMath::Exp(-(x[0]-par[2])/par[1]);
  if(val > 0) val = 0.0;
  return val;
  */

  //Change to use Novosibirsk function to describe the waveform given by Lorenzo
  double peak = par[0];
  double width = par[1];
  double tail = par[2];

  //If tail=eta=0 the Belle distribution becomes gaussian
  if (TMath::Abs(tail) < 1.e-7) {
    return TMath::Exp( -0.5 * TMath::Power( ( (x[0] - peak) / width ), 2 ));
  }

  Double_t arg = 1.0 - ( x[0] - peak ) * tail / width;

  if (arg < 1.e-7) {
  //Argument of logaritem negative. Real continuation -> function equals zero
  return 0.0;
  }

  Double_t log = TMath::Log(arg);
  static const Double_t xi = 2.3548200450309494; // 2 Sqrt( Ln(4) )

  Double_t width_zero = ( 2.0 / xi ) * TMath::ASinH( tail * xi * 0.5 );
  Double_t width_zero2 = width_zero * width_zero;
  Double_t exponent = ( -0.5 / (width_zero2) * log * log ) - ( width_zero2 * 0.5 );

  return TMath::Exp(exponent);
}

TObjArray* PdFeeResponse::ApplyFeeResponse(TObjArray* objarr)
{
  /*
  TF1* function = new TF1("waveform", this, &PdFeeResponse::SiPMWaveform, 0.0, PdParameters::GetInstance()->GetReadoutWindow(), 3, "PdFeeResponse", "Evaluate");
  function->SetParameter(0, PdParameters::GetInstance()->GetRiseTime());
  function->SetParameter(1, PdParameters::GetInstance()->GetDecayTime());
  function->SetParameter(2, 20);
  */
  //Change to use Novosibirsk function to describe the waveform given by Lorenzo
  TF1* function = new TF1("waveform", this, &PdFeeResponse::SiPMWaveform, 0.0, PdParameters::GetInstance()->GetReadoutWindow(), 3, "PdFeeResponse", "Evaluate");
  function->SetParameter(0, PdParameters::GetInstance()->GetNovosibirskMean());
  function->SetParameter(1, PdParameters::GetInstance()->GetNovosibirskSigma());
  function->SetParameter(2, PdParameters::GetInstance()->GetNovosibirskTail());

  TObjArray* newObjArr = new TObjArray();

  std::vector<int> chanIDVec;
  chanIDVec.clear();

  double QofSinglePE = fabs(function->Integral(0.0, PdParameters::GetInstance()->GetIntegralWindow()));
  double encs = PdParameters::GetInstance()->GetENCs();
  double QofNoise = encs*QofSinglePE;
  double noisePerBin = QofNoise/sqrt(PdParameters::GetInstance()->GetIntegralWindow());
  std::cout << "QofSinglePE: " << QofSinglePE << std::endl;
  std::cout << "encs: " << encs << std::endl;
  std::cout << "Q of noise: " << QofNoise << std::endl;
  std::cout << "noise per bin: " << noisePerBin << std::endl;
  int nentries = objarr->GetEntries();
  for(int i = 0; i < nentries; i++) {
    PdSiPMHit* hit = static_cast<PdSiPMHit*> (objarr->At(i));

    int chanID = PadToChannelID(hit->GetSiPMPadID());
    double time = hit->GetHitTime();
    int type = hit->GetHitType();

    if(find(chanIDVec.begin(), chanIDVec.end(), chanID) == chanIDVec.end()) {
      chanIDVec.push_back(chanID);

      PdFeeChannel* newchan = new PdFeeChannel();
      newchan->SetChannelID(chanID);
      newchan->SetChannelTime(time);
      newchan->SetSignalType(type);

      function->SetParameter(0, time + PdParameters::GetInstance()->GetNovosibirskMean());
      double gainfluct = gRandom->Gaus(1, PdParameters::GetInstance()->GetSiPMGainFluct());
      double charge = 0;

      for(int ibin = 0; ibin < (int) PdParameters::GetInstance()->GetReadoutWindow(); ibin++) {
        double amplitude = gainfluct*function->Eval(ibin+0.5);
        double enoise = gRandom->Gaus(0, noisePerBin);

        newchan->SetPulseShape(ibin, amplitude + enoise);

        //calculate total charge per channel
        charge += amplitude + enoise;
      }

      newchan->SetChannelCharge(charge);

      newObjArr->AddLast(newchan);
    }
    else {
      int idx = 0;
      PdFeeChannel* chan = 0;
      for(unsigned int j = 0; j < newObjArr->GetEntries(); j++) {
        chan = static_cast<PdFeeChannel*> (newObjArr->At(j));
        if(chan->GetChannelID() == chanID) {
          break;
        }
      }

      double oldtime = chan->GetChannelTime();
      if(oldtime > time) chan->SetChannelTime(time);
      int oldtype = chan->GetSignalType();
      chan->SetSignalType(oldtype + type);

      function->SetParameter(0, time + PdParameters::GetInstance()->GetNovosibirskMean());
      double gainfluct = gRandom->Gaus(1, PdParameters::GetInstance()->GetSiPMGainFluct());
      double charge = 0;
      for(int ibin = 0; ibin < (int) PdParameters::GetInstance()->GetReadoutWindow(); ibin++) {
        double amplitude = gainfluct*function->Eval(ibin+0.5);
        double oldAmplitude = chan->GetPulseShape(ibin);
        chan->SetPulseShape(ibin, amplitude + oldAmplitude);
        charge += amplitude + oldAmplitude;
      }

      chan->SetChannelCharge(charge);
    }
  }
 
  delete function;
  objarr->Clear();

  return newObjArr;
}

int PdFeeResponse::PadToChannelID(int id)
{
  int nSiPMPads = PdParameters::GetInstance()->GetNSiPMPads();
  int nSiPMPadsAlongPhi = PdParameters::GetInstance()->GetNSiPMPadsAlongPhi();
  int nChannels = PdParameters::GetInstance()->GetNChannels();

  int nSiPMPadsPerChannel = (int) nSiPMPads/nChannels;
  if((float) nSiPMPads/nChannels - nSiPMPadsPerChannel > 0.5) nSiPMPadsPerChannel += 1;

  int idz = (int) id/10000.0;
  int idphi = id - idz*10000;

  int iSiPMPad = idz*nSiPMPadsAlongPhi + idphi;

  int channelID = (int) iSiPMPad/nSiPMPadsPerChannel;

  return channelID;
}
