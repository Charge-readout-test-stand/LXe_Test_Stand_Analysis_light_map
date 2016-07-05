#ifndef nEXOTClonesArray_hh
#define nEXOTClonesArray_hh

#ifndef ROOT_TClonesArray
#include "TClonesArray.h"
#endif

// June 28, L.J Wen: v0, copied from misc/EXOUtilities/EXOTClonesArray.hh

class nEXOTClonesArray : public TClonesArray {

  // An initial wrapper of TClonesArray, in case 
  // extra functionality is required.
  public:
    nEXOTClonesArray() : TClonesArray() {}

    nEXOTClonesArray( const TClonesArray& array ) : TClonesArray( array ) {}

    nEXOTClonesArray( const char* classname, Int_t size = 1000, Bool_t call_dtor = false ) : 
      TClonesArray( classname, size, call_dtor ) {}

    nEXOTClonesArray( const TClass* aClass, Int_t size = 1000, Bool_t call_dtor = false ) : 
      TClonesArray( aClass, size, call_dtor ) {}

    virtual nEXOTClonesArray& operator=(const TClonesArray& array);
    virtual TObject *&operator[](Int_t idx);
    virtual TObject *operator[](Int_t idx) const { return TClonesArray::operator[](idx); }
    virtual TObject *GetNewOrCleanedObject(Int_t idx);
    virtual void Clear(Option_t *opt);
    virtual void RemoveAndCompress(TObject *obj);

  ClassDef( nEXOTClonesArray, 1 )
};

#endif /* nEXOTClonesArray_hh */
