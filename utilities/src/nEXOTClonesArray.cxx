//______________________________________________________________________________
// nEXOTClonesArray derives from TClonesArray, and contains no additional data members.
// It guarantees that the array will remain compressed; this guarantee is required by
// other classes to improve performance.
//
// Note:  As of root v. 5.28, using nEXOTClonesArray directly for I/O has unpredictable
// or suboptimal performance.
// See http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=12595 for details.
//
// As a result, classes using nEXOTClonesArray will generally store it with a
// TClonesArray pointer.  But TClonesArray::operator= is not virtual; so the
// classes should cast the lvalue as follows:
//
// TClonesArray* array1 = new nEXOTClonesArray(...);
// TClonesArray* array2 = new nEXOTClonesArray(...);
// *static_cast<nEXOTClonesArray*>(array1) = *array2;
//
// June 28, L.J Wen: v0, copied from misc/src/EXOTClonesArray.cc

#include "nEXOUtilities/nEXOTClonesArray.hh"
#include "TClass.h"

ClassImp(nEXOTClonesArray)
//______________________________________________________________________________
nEXOTClonesArray& nEXOTClonesArray::operator=(const TClonesArray& array)
{
  // Assignment operator;
  this->TClonesArray::operator=(array);
  this->Compress();
  return *this;
}
//______________________________________________________________________________
TObject *&nEXOTClonesArray::operator[](Int_t idx)
{
   // Overload of TClonesArray::operator[], zeros memory allocated so
   // that calls like:
   //
   // temp = myClonesArray[i];
   // temp->TestBit(TObject::kNotDeleted)
   // 
   // will behave correctly. 
   Bool_t mustZero = false;
   if (idx >= 0 && fClass && (idx >= fSize || !fKeep->UncheckedAt(idx))) {
     mustZero = true;
   }
   TObject *&temp = TClonesArray::operator[](idx);

   // If we must zero, this means that the constructor has not been called over
   // this region of memory.  The explicit cast to a void* is to avoid compiler
   // warnings that the TObject pointer is a pointer to a non-POD type and that
   // this will overwrite the vtable pointer.  This is not a concern because at
   // this point the vtable data has not been generated as the constructor has
   // not been called.
   if ( temp && mustZero ) memset(static_cast<void*>(temp), 0, fClass->Size());
   return temp;
}

//______________________________________________________________________________
TObject *nEXOTClonesArray::GetNewOrCleanedObject(Int_t idx) 
{
   // Get a new or cleaned object from the array.  Tests to see if the
   // destructor has been called on the object.  If so, or if the object
   // has never been constructed the class constructor is called using
   // New().  If not, return a pointer to the correct memory location.
   // This explicitly to deal with TObject classes that allocate memory
   // which will be reset (but not deallocated) in their Clear()
   // functions.  

   TObject* temp = (*this)[idx];
   if ( temp && temp->TestBit(TObject::kNotDeleted) ) {
     temp->Clear();
     return temp;
   }
   return (fClass) ? static_cast<TObject*>(fClass->New(temp)) : 0;
}
//______________________________________________________________________________
void nEXOTClonesArray::Clear(Option_t *option)
{
   //
   // Properly reset the UUID, reference bits and reset the UniqueID of each
   // object.
   //
   if (option && option[0] == 'C') {
      Int_t n = GetEntriesFast();
      for (Int_t i = 0; i < n; i++) {
         TObject *obj = UncheckedAt(i);
         if (obj) {
            obj->ResetBit( kHasUUID ); 
            obj->ResetBit( kIsReferenced ); 
            obj->SetUniqueID( 0 ); 
         }
      }
   }
   TClonesArray::Clear(option);
}
//______________________________________________________________________________
void nEXOTClonesArray::RemoveAndCompress(TObject *obj)
{
   // Remove an object, compress to remove spaces in the array if necessary.
   // The compression will only be called if obj is not the last element in the
   // array.  

   Int_t index = IndexOf(obj);
   if (index < LowerBound()) return;

   // check if it's the last guy;
   bool mustCompress =  ( !(index == GetAbsLast()) );
   // RemoveAt calls the destructor of the object which calls cleanups, removing
   // object from any referencing objects (TRefs, etc.) around. 
   RemoveAt(index);
   if (mustCompress) Compress();
}
