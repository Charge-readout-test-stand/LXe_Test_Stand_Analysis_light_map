#ifndef nEXOLightReadoutDigitize_hh
#define nEXOLightReadoutDigitize_hh
#include <set>
#ifndef ROOT_TObject
#include "TObject.h"
#endif

class nEXOLightReadoutDigitize
{
public:
  nEXOLightReadoutDigitize();
  ~nEXOLightReadoutDigitize();

  void GeneratePCDs();
  void Digitize();
  void SetNoiseModel();
};

#endif
