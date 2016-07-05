#ifndef nEXODimensions_h
#define nEXODimensions_h 1

#ifndef HEP_SYSTEM_OF_UNITS_H
#include "nEXOUtilities/SystemOfUnits.hh"
#endif
#include <cstddef>
#include <cmath>

// This sample time is used to calculate the signals before shaping.
const double SAMPLE_TIME_HIGH_BANDWIDTH = 0.04*CLHEP::microsecond;

// This is the ADC sample time, and should be an integer multiple of the
// high bandwidth sample time
const unsigned int BANDWIDTH_FACTOR = 2;
const double SAMPLE_TIME = SAMPLE_TIME_HIGH_BANDWIDTH*BANDWIDTH_FACTOR; 

// ADC full scale counts
const int ADC_BITS = 16384;

//const double FINE_DRIFT_STEP_TIME = SAMPLE_TIME_HIGH_BANDWIDTH;
//const double COARSE_DRIFT_STEP_TIME = SAMPLE_TIME;

// This is the z location where the signal calculation switches from
// coarse stepping to fine stepping.
const double DRIFT_VELOCITY = 0.2*CLHEP::cm/CLHEP::microsecond;
const double DEFAULT_CHARGE_RMS_ELECTRONS_NOISE = 0.0;

// This defines the dimensions of charge pixels, 
// as used by monte carlo to group geant energy deposits.
//const double MCCHARGE_PIXEL_SIZE = WIRE_PITCH/10.0;
//const double ANODE_Z = 650*CLHEP::mm;
const double TPC_RADIUS = 650.0*CLHEP::mm;
const double TILE_SIZE= 90.0*CLHEP::mm;
const int TILE_NUM= 172;
const double PAD_PITCH_DEFAULT = 3*CLHEP::mm;
const double MCCHARGE_PIXEL_SIZE_DEFAULT = PAD_PITCH_DEFAULT/20/sqrt(2.);
const double MCCHARGE_PIXEL_TIME = SAMPLE_TIME/2.0;
const double MCINDUCTION_PIXEL_SIZE = PAD_PITCH_DEFAULT/10;
//const int NUM_CHANNEL_ON_TILE = (int(TILE_SIZE/PAD_PITCH_DEFAULT))*2;
const int NUM_CHANNEL_ON_TILE = 60;
const int SAMPLEWINDOW = 800;



// This defines t0
//const double ADC_FULL_SCALE_ELECTRONS_WIRE = 300.0*ADC_BITS;
//const double ADC_FULL_SCALE_ENERGY_WIRE = ADC_FULL_SCALE_ELECTRONS_WIRE*W_VALUE_LXE_EV_PER_ELECTRON;

// Total number of digitized signals
//const size_t NUMBER_READOUT_CHANNELS = NCHANNEL_PER_WIREPLANE*NWIREPLANE+NUMBER_APD_CHANNELS_PER_PLANE*NAPDPLANE;

// for Xe-136 double beta decay
const double DOUBLE_BETA_DECAY_Q_VALUE = 2457.8*CLHEP::keV;
// for Xe-134 double beta decay
const double XE_134_Q_VALUE = 830.0*CLHEP::keV;
// for Xe-124 double beta+ decay
const double XE_124_Q_VALUE = 822.0*CLHEP::keV;

const double ELECTRON_MASS = 0.51099892*CLHEP::MeV;

#endif
