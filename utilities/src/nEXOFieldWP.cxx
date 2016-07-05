#ifndef nEXOFieldWP_hh
#include "nEXOUtilities/nEXOFieldWP.hh"
#endif

#ifndef nEXOChannelMap_hh
#include "nEXOUtilities/nEXOChannelMap.hh"
#endif

#ifdef nEXODimensions_hh
#include "nEXOUtilities/nEXODimensions.hh"
#endif

#include "TString.h"
#include "TMath.h"
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;
nEXOFieldWP* nEXOFieldWP::fInstance = NULL;

const double PI = 3.141592653589793;


nEXOFieldWP::nEXOFieldWP() 
{
  fWPFile = 0;
  fPadWP = 0;
}

nEXOFieldWP::~nEXOFieldWP() 
{
}

// Functions
void nEXOFieldWP::Clear(Option_t* opt)
{
  if(fWPFile!=NULL) fWPFile->Close();
}

bool nEXOFieldWP::LoadWP(TString coType, TString filename) 
{
  if(coType=="Pads") {
    // Pads charge readout design
    std::cout << filename << std::endl;
    fWPFile = new TFile(filename, "read");
    //fPadWP = (TH3D*) fWPFile->Get("q3d");
    fPadWP = (TH3D*) fWPFile->Get("WP_ROOT");

    if(fWPFile==NULL or fPadWP==NULL) return kFALSE;
    return kTRUE;
  }
  else {
    // other options
    return kFALSE;
  }
}

Double_t nEXOFieldWP::CalcInducedChargeOnPads_COMSOL(Int_t chanId, Double_t xq, Double_t yq, Double_t zq)
{

    /* Calculate the induced charge at position xq, yq, zq using WP file
     * generated in COMSOL from Dave Moore Nov 2015.  WP file is TH3D with
     * an x Strip at XPos = 0.  The range of file is 90mm in x and ycorresponding to
     * 30X and 30Y channels from -45.0mm to +45.0mm in both X and Y. The Z position
     * ranges from 17mm to 0mm where 17mm is the anode.
     */


  Int_t localId = nEXOChannelMap::GetInstance()->GetLocalId(chanId);
  TVector3 chanPos = nEXOChannelMap::GetInstance()->GetChannelPosition(chanId);
  TString  chanType = nEXOChannelMap::GetInstance()->GetChannelType(chanId);



  //The WP File is done for a YChannel in the center so channel positions 
  //are (1.5+3*m, 0) where m is an integer and 3 is the wire pitch
  //So call RelPos is the poistion relative to this channel (this is Y in file)
  //Call the Height the poisiton in the direciton of the Channel (this is X in file)
  Double_t xch = chanPos.x();
  Double_t ych = chanPos.y();
  Double_t RelPos, Height; 
  //File is for a given YChannel so make sure to switch x->y and y->x for XChannels
  //For looking up purposes
  //There is a 1.5 shift in the ypos in the COMSOL file from Dave
  if(chanType=="X")
  {
      //XChannel
      RelPos = (xq - xch);
      Height = (yq + 1.5);
  }
  else if(chanType=="Y")
  {
      //YChannel
      RelPos = (yq - ych);
      Height = (xq + 1.5);
  }
  else
  {
      //Do better than this but for now
      std::cout << chanType << " BAD CHannel Type" << std::endl;
  }   
  
  if (RelPos < fPadWP->GetXaxis()->GetXmin() or RelPos > fPadWP->GetXaxis()->GetXmax())
  {
      //This channel is very far and doesn't appear in this file so it must be small
      //just call it 0 since it probably doesn't induce anyways
      //std::cout << "Outside range Got 0.0" << std::endl;
      return 0.0;
  }
  if(Height < fPadWP->GetYaxis()->GetXmin() or  Height > fPadWP->GetXaxis()->GetXmax())
  {
    //This PCD is not under the tile so no induced signal
    return 0.0;

  }

  //The file from Dave goes from 0 to 17mm where the anode is at 17mm???
  //So I need to fix but for now anode in nEXO_MC is at 0mm
  Int_t xbin = fPadWP->GetXaxis()->FindBin(RelPos);
  Int_t ybin = fPadWP->GetYaxis()->FindBin(Height);      
  //Our Cathode is at 17mm so just need to flip around the z
  Int_t zbin = fPadWP->GetZaxis()->FindBin(zq);
  
  return fPadWP->GetBinContent(fPadWP->GetBin( xbin, ybin, zbin));

}

Double_t nEXOFieldWP::InductionFunction(Double_t x, Double_t y, Double_t z)
{
    //If x or y is 0 and z is 0 tan_arg will be nan so just in case skip doing that
    //If z = 0 tan_arg = inf and atan(inf) = pi/2
    Double_t tan_arg = (x*y)/(z*sqrt(x*x + y*y + z*z));
    if (isnan(tan_arg) and z < 1e-10){return PI/2;}
    return atan(tan_arg);
}

Double_t nEXOFieldWP::EvaluateInduction(Double_t xpcd, Double_t ypcd, Double_t zpcd, Double_t chx, Double_t chy)
{
    //Use Ralph's Method to get the Induced Charge at (x,y,z) for 
    //channel at chx chy.  All this requires is Evaluating the function
    //at the four corners of the pad.  This is done in the frame where the
    //pad is a square not a diamond for simplicity.

    //Length of the diagnoal is 3mm (PAD_PITCH_DEFAULT)

    //Rotate into the Rectangles Frame for this calculation
    //45degrees rotation
    Double_t xpcdrot = (sqrt(2)/2)*(xpcd - ypcd);
    Double_t ypcdrot = (sqrt(2)/2)*(xpcd + ypcd);

    Double_t chxrot=  (sqrt(2)/2)*(chx - chy);
    Double_t chyrot = (sqrt(2)/2)*(chx + chy);

    //The location of the four corners of the square
    Double_t x1 = chxrot - PAD_PITCH_DEFAULT/(2*sqrt(2));
    Double_t y1 = chyrot - PAD_PITCH_DEFAULT/(2*sqrt(2));
    Double_t x2 = chxrot + PAD_PITCH_DEFAULT/(2*sqrt(2));
    Double_t y2 = chyrot + PAD_PITCH_DEFAULT/(2*sqrt(2));

    //Translate coordinates so the charge is at (0,0)
    x2 = x2 - xpcdrot;
    x1 = x1 - xpcdrot;
    y2 = y2 - ypcdrot;
    y1 = y1 - ypcdrot;

    //Evaluate the four corners
    Double_t f22 = InductionFunction(x2,y2,zpcd);
    Double_t f21 = InductionFunction(x2,y1,zpcd);
    Double_t f12 = InductionFunction(x1,y2,zpcd);
    Double_t f11 = InductionFunction(x1,y1,zpcd);
    
    //Return the total Inegral
    return (f22 - f21 - f12 + f11)*(1/(2*PI));
}

Double_t nEXOFieldWP::CalcInducedChargeOnPads_Analytic(Int_t chanId, Double_t xq, Double_t yq, Double_t zq)
{
    Int_t localId = nEXOChannelMap::GetInstance()->GetLocalId(chanId);
    TVector3 chanPos = nEXOChannelMap::GetInstance()->GetChannelPosition(chanId);
    TString  chanType = nEXOChannelMap::GetInstance()->GetChannelType(chanId);

    Double_t xch = chanPos.x();
    Double_t ych = chanPos.y();

    Double_t Q =0;

    Double_t OffChStart =  -PAD_PITCH_DEFAULT*(NUM_CHANNEL_ON_TILE/4 - 1);

    //Channel Position for the height starts offset by 1.5
    if(chanType=="X")
    {
        ych = OffChStart;

        for(Int_t j=0; j<NUM_CHANNEL_ON_TILE/2; j++){
            Q += EvaluateInduction(xq, yq, zq, xch, ych);
            ych+=PAD_PITCH_DEFAULT;
        }
    }
    else if(chanType=="Y")
    {
        xch = OffChStart; 
        for(Int_t j=0; j<NUM_CHANNEL_ON_TILE/2; j++){
            Q += EvaluateInduction(xq, yq, zq, xch, ych);
            xch+=PAD_PITCH_DEFAULT;
        }
        
    }
    return Q;

}










