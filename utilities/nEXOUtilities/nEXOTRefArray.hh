#ifndef nEXOTRefArray_hh
#define nEXOTRefArray_hh

#ifndef ROOT_TRefArray
#include "TRefArray.h"
#endif

class nEXOTRefArray : public TRefArray {

  // An initial wrapper of TClonesArray, in case 
  // extra functionality is required.
  public:

    nEXOTRefArray(TProcessID *pid = 0) : TRefArray(pid) {}
    nEXOTRefArray(Int_t s, TProcessID *pid) : TRefArray(s, pid) {}
    nEXOTRefArray(Int_t s, Int_t lowerBound = 0, TProcessID *pid = 0) : TRefArray(s, lowerBound, pid) {}
    nEXOTRefArray(const nEXOTRefArray &a) : TRefArray(a) {}
    nEXOTRefArray& operator=(const nEXOTRefArray &a);

    TObject *operator[](Int_t i) const;
    TObject *At(Int_t idx) const;
    TObject *AtWithoutTable(Int_t idx) const;
    void Compress();

  ClassDef( nEXOTRefArray, 1 )
};

#endif /* nEXOTRefArray_hh */
