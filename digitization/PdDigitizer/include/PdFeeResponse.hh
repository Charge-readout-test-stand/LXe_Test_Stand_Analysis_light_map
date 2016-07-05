//**********************************************************************
//  A class is used to model FEE response.
//  caogf@ihep.ac.cn
//  2014.07.25
//**********************************************************************

#ifndef PDFEERESPONSE_H
#define PDFEERESPONSE_H

#include "TObjArray.h"

class PdFeeResponse
{
public:

  PdFeeResponse();
  ~PdFeeResponse();

  double SiPMWaveform(double* x, double* par);
  TObjArray* ApplyFeeResponse(TObjArray* objarr);
  int PadToChannelID(int id);

private:

};

#endif
