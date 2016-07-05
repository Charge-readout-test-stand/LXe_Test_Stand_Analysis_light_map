//**********************************************************************
//  A class is used to model effects from SiPM.
//  caogf@ihep.ac.cn
//  2014.07.25
//**********************************************************************

#ifndef PDSIPMEFFECT_H
#define PDSIPMEFFECT_H

#include "TF1.h"
#include "TH1F.h"
#include "TObjArray.h"

class PdSiPMEffect
{
public:

  PdSiPMEffect();
  ~PdSiPMEffect();

  double Expo(double* x, double* par);

  TObjArray* ApplySiPMEffect(TObjArray* obj);

  void ApplyDarkNoise();
  void ApplyCorrAvalanche();
  void ApplyPhotonCrossTalk();

private:

  TF1* fFunction;
  TH1F* fProb_ct;
  TObjArray* fObjArray;
};

#endif
