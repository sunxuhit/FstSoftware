// includes {{{
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std ;


#include <rtsLog.h>	// for my LOG() call
#include <rtsSystems.h>

#include <curses.h>

// this needs to be always included
#include <DAQ_READER/daqReader.h>
#include <DAQ_READER/daq_dta.h>

// only the detectors we will use need to be included
// for their structure definitions...
#include <DAQ_FGT/daq_fgt.h>

#include "TFile.h"
#include "TVector3.h"
#include "TChain.h"
#include "TTree.h"
#include "TApplication.h"
#include "TGraphErrors.h"
#include "TGraph2DErrors.h"
#include "TStyle.h"
#include "TH2.h"
#include "TGraph2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TBrowser.h"
#include <TROOT.h>

// }}}

//-------------------------------------------------------------------------------
// PARAMETERS THAT CAN BE CHANGED WITH SOME CAUTION
int STAVEID = 4;
int DATE 	= 130402;
int TIME	= 1400;
bool SENSOR	= 0;
int BIAS	= 60;
int VPSP	= 43; 

// MAXIMUM NUMBER OF EVENTS THAT ARE PROCESSED
// NOTE THAT THIS IS _NOT_ THE NUMBER EVENTS FOR PEDESTAL CALCULATION
const int numEvents = 5000;

// Select the APV range we want to look at
int SEL_LO_APV1 =  0;
int SEL_HI_APV1 =  11;

// NUMBER OF EVENTS THAT SHOULD BE USED TO CALCULATE THE PEDESTALS
// Note, we should make up our mind whether to generate the pedestals
// using numEvents events or to use a subset, this has consequences
// for the broken event determination!
const int numPeds     = 1000;
const int numPedsDown = 0;
const int numPedsUp   = 1000;
// THIS IS THE PEDESTAL CUT!
// Raw ADC values outside ( pedestal +- PEDCUT*sigma) are excluded from the second pedestal pass.
int PEDCUT = 3;

// THIS IS THE HIT CUT!
// Pedestal substracted ADC values above ( HITCUT*sigma ) are accepted as a hit
int HITCUT = 5;

// DEAD CHANNEL CRITERIUM = any channel with less than 10 ADC noise is declared dead.
const float MinimumNoise = 10.0;

// MAXIMUM NUMBER OF HITS ALLOWED PER EVENT
// if we find more then this is a broken event and we should bail
int MAX_NO_HITS_PER_EVENT = 10;
const int MAX_NO_HITS = 10;

// CLUSTERING ON OR OFF
//bool CLUSTER = true;
bool CLUSTER = false;

// POLARITY OF THE SIGNAL
const int POLARITY = +1;

// Alignment rotations of layer 2 and 3 wrt layer 1
// const float ROTATE2 = -0.30;
// const float ROTATE3 = +0.10;
const float ROTATE2 = 0.00; // modified by Xu Sun @ 02/13/2020 => no rotation in UIC setup
const float ROTATE3 = 0.00;
// Alignment shifts of layer 2 and 3 wrt layer 1
// const float SHIFT2  = -600.0;
// const float SHIFT3  = -600.0;
const float SHIFT2  = 0.0; // modified by Xu Sun @ 02/13/2020
const float SHIFT3  = 0.0;
// const float SHIFT3  = -3.96;


// DON'T CHANGE ANYTHING BELOW THIS LINE
//-------------------------------------------------------------------------------

const int UseTimeBin = 3;

// This is the timebin which is going to be used to calculate the pedestals
const int PedTimeBin = 0;

const int numARMs     = 6;
const int numPorts    = 2;  // note that the readout just gives 24 APVs on a single ARM, there is
const int numAPVs     = 12; // no actual Port information, we just introduce Port information ourselves
const int numChannels = 128;
const int numTBins    = 7;

// Maximum track angle, should be set by geometry
//const float maxAngle = 26.0;
const float maxAngle = 100.0;

char * mountpoint = NULL;

double pedestals[numARMs][numPorts][numAPVs][numChannels];
double pedestalsStdDev[numARMs][numPorts][numAPVs][numChannels];
double pedestalsRMS[numARMs][numPorts][numAPVs][numChannels];
double REFpedestals[numARMs][numPorts][numAPVs][numChannels];
double REFpedestalsStdDev[numARMs][numPorts][numAPVs][numChannels];
double REFpedestalsRMS[numARMs][numPorts][numAPVs][numChannels];

// For common mode noise calculations and finding out of whack channels
double PedMeanAPV[numAPVs];
double PedSigmaAPV[numAPVs];

// DEAD CHANNEL MAP, NOT IMPLEMENTED YET!!!
//bool PedOutofRange[numAPV][numChannels];
//bool PedSigmaOutofRange[numAPV][numChannels];
//bool HotChannel[numAPV][numChannels];
//bool ColdChannel[numAPV][numChannels];
//bool DeadChannel[numAPV][numChannels]; // AND of all suspicious channels

// STORAGE OF FOUND HITS
struct hit_store {
       int layer;
       int sensor;
       int row;
       int column;
       int ADC;
} Hit[MAX_NO_HITS];
int Found_No_Hits;

TCanvas *c1_MainDisplay;
TPad *p1_PedDisplay;
TPad *p1_PedSigma;
TPad *p1_hitColmap;
TPad *p1_hitmap;
TPad *p1_hitRowmap;
TPad *p1_PEDspectrum;
TPad *p1_EventDisplay;
TPad *p1_PulseDisplay;
TPad *p1_ADCspectrum;
TPad *p1_signal2noise;
TPad *p1_maxTB;

TCanvas *c2_MainDisplay;
TPad *p2_PedDisplay;
TPad *p2_PedSigma;
TPad *p2_hitColmap;
TPad *p2_hitmap;
TPad *p2_hitRowmap;
TPad *p2_PEDspectrum;
TPad *p2_EventDisplay;
TPad *p2_PulseDisplay;
TPad *p2_ADCspectrum;
TPad *p2_signal2noise;
TPad *p2_maxTB;

TCanvas *c3_MainDisplay;
TPad *p3_PedDisplay;
TPad *p3_PedSigma;
TPad *p3_hitColmap;
TPad *p3_hitmap;
TPad *p3_hitRowmap;
TPad *p3_PEDspectrum;
TPad *p3_EventDisplay;
TPad *p3_PulseDisplay;
TPad *p3_ADCspectrum;
TPad *p3_signal2noise;
TPad *p3_maxTB;

TCanvas *c0_MainDisplay;
TPad *p0_PedDisplay;
TPad *p0_PedSigma;
TPad *p0_hitColmap;
TPad *p0_hitmap;
TPad *p0_hitRowmap;
TPad *p0_PEDspectrum;
TPad *p0_EventDisplay;
TPad *p0_PulseDisplay;
TPad *p0_ADCspectrum;
TPad *p0_signal2noise;
TPad *p0_maxTB;

TCanvas *c1_TrackDisplay;
TPad *p1_track_ADCspectrum;
TPad *p2_track_ADCspectrum;
TPad *p1_track_angcorr_ADCspectrum;
TPad *p2_track_angcorr_ADCspectrum;
// TPad *p9_ADC1ADC2;
TPad *p9_YResidual;
TPad *p9_XResidual;

TCanvas *c1_QAplotsDisplay;
TPad *p1_NoiseHistogram;
TPad *p2_NoiseHistogram;
TPad *p3_NoiseHistogram;
TPad *p1_StrangeNoiseHistogram;
TPad *p2_StrangeNoiseHistogram;
TPad *p3_StrangeNoiseHistogram;

/*
TCanvas *c1_SecondDisplay;
TPad *p1_APVaveragePED[12];

TCanvas *c2_SecondDisplay;
TPad *p2_APVaveragePED[12];
*/


// Pedestals, Sigma of Pedestals and their References
// Layer 1
TGraph *g1_ped1Sigma    = new TGraphErrors();
TGraph *g1_REFped1Sigma = new TGraphErrors();
TGraph *g1_sigmaPed     = new TGraph();
TGraph *g1_REFsigmaPed  = new TGraph();
// Layer 2
TGraph *g2_ped1Sigma    = new TGraphErrors();
TGraph *g2_REFped1Sigma = new TGraphErrors();
TGraph *g2_sigmaPed     = new TGraph();
TGraph *g2_REFsigmaPed  = new TGraph();
// Layer 3
TGraph *g3_ped1Sigma    = new TGraphErrors();
TGraph *g3_REFped1Sigma = new TGraphErrors();
TGraph *g3_sigmaPed     = new TGraph();
TGraph *g3_REFsigmaPed  = new TGraph();
// Layer 0
TGraph *g0_ped1Sigma    = new TGraphErrors();
TGraph *g0_REFped1Sigma = new TGraphErrors();
TGraph *g0_sigmaPed     = new TGraph();
TGraph *g0_REFsigmaPed  = new TGraph();


// Event display showing pedestal corrected ADC value for all channels,
// each timebin has one, we'll show only one timebin
// Layer 1
TGraph *g1_eventADCvsChannel[numTBins];
// Layer 2
TGraph *g2_eventADCvsChannel[numTBins];
// Layer 3
TGraph *g3_eventADCvsChannel[numTBins];
// Layer 0
TGraph *g0_eventADCvsChannel[numTBins];

// Event display showing the pulses found for this event
// Layer 1
TGraph *g1_eventADCvsTimebin;
// Layer 2
TGraph *g2_eventADCvsTimebin;
// Layer 3
TGraph *g3_eventADCvsTimebin;
// Layer 0
TGraph *g0_eventADCvsTimebin;

// Poor mans geometrical picture of sensors
// Note that we have 2 x 12 rows for 2 sensors per section
const int noRows    = 64;
const int noColumns = 24;
const Float_t pitchRow    = 0.596  ; //mm
const Float_t pitchColumn = 6.275  ; //mm
const Float_t pitchLayer  = 37.0   ; //mm, distances between the three layers are the same
const Float_t pitchLayer12  = 34.925   ; //mm, distances between the 1st & 2nd Layer => Added by Xu Sun @ 02/13/2020
const Float_t pitchLayer23  = 30.1625   ; //mm, distances between the 2nd & 3rd Layer
const float rad2deg = 180.0/3.141593;

// Hits on the sensor(s), in row/col and projected col and projected on row
// Layer 1
TH2F *h1_hits_on_sensor = new TH2F("h1_hits_on_sensor","h1_hits_on_sensor", noColumns, -0.5, noColumns-0.5, noRows,-0.5,noRows-0.5 );
TH1F *h1_hits_on_Col    = new TH1F("h1_hits_on_Col",   "h1_hits_on_Col",    noColumns, -0.5, noColumns-0.5 );
TH1F *h1_hits_on_Row    = new TH1F("h1_hits_on_Row",   "h1_hits_on_Row",    noRows   , -0.5, noRows-0.5    );
// Layer 2
TH2F *h2_hits_on_sensor = new TH2F("h2_hits_on_sensor","h2_hits_on_sensor", noColumns, -0.5, noColumns-0.5, noRows,-0.5,noRows-0.5 );
TH1F *h2_hits_on_Col    = new TH1F("h2_hits_on_Col",   "h2_hits_on_Col",    noColumns, -0.5, noColumns-0.5 );
TH1F *h2_hits_on_Row    = new TH1F("h2_hits_on_Row",   "h2_hits_on_Row",    noRows   , -0.5, noRows-0.5    );
// Layer 3
TH2F *h3_hits_on_sensor = new TH2F("h3_hits_on_sensor","h3_hits_on_sensor", noColumns, -0.5, noColumns-0.5, noRows,-0.5,noRows-0.5 );
TH1F *h3_hits_on_Col    = new TH1F("h3_hits_on_Col",   "h3_hits_on_Col",    noColumns, -0.5, noColumns-0.5 );
TH1F *h3_hits_on_Row    = new TH1F("h3_hits_on_Row",   "h3_hits_on_Row",    noRows   , -0.5, noRows-0.5    );
// Layer 0 => FST
TH2F *h0_hits_on_sensor = new TH2F("h0_hits_on_sensor","h0_hits_on_sensor", noColumns, -0.5, noColumns-0.5, noRows,-0.5,noRows-0.5 );
TH1F *h0_hits_on_Col    = new TH1F("h0_hits_on_Col",   "h0_hits_on_Col",    noColumns, -0.5, noColumns-0.5 );
TH1F *h0_hits_on_Row    = new TH1F("h0_hits_on_Row",   "h0_hits_on_Row",    noRows   , -0.5, noRows-0.5    );

// ADC and Pedestal spectrum
// Layer 1
TH1F *h1_PEDspectrum  = new TH1F("h1_PEDspectrum", "h1_PEDspectrum",   50, 0,   50);
TH1F *h1_ADCspectrum  = new TH1F("h1_ADCspectrum", "h1_ADCspectrum",  100, 0, 4000);
TH1F *h1_signal2noise = new TH1F("h1_signal2noise","h1_signal2noise", 100, 0,  100);
// Layer 2
TH1F *h2_PEDspectrum  = new TH1F("h2_PEDspectrum", "h2_PEDspectrum",   50, 0,   50);
TH1F *h2_ADCspectrum  = new TH1F("h2_ADCspectrum", "h2_ADCspectrum",  100, 0, 4000);
TH1F *h2_signal2noise = new TH1F("h2_signal2noise","h2_signal2noise", 100, 0,  100);
// Layer 3
TH1F *h3_PEDspectrum  = new TH1F("h3_PEDspectrum", "h3_PEDspectrum",   50, 0,   50);
TH1F *h3_ADCspectrum  = new TH1F("h3_ADCspectrum", "h3_ADCspectrum",  100, 0, 4000);
TH1F *h3_signal2noise = new TH1F("h3_signal2noise","h3_signal2noise", 100, 0,  100);
// Layer 0
TH1F *h0_PEDspectrum  = new TH1F("h0_PEDspectrum", "h0_PEDspectrum",   50, 0,   50);
TH1F *h0_ADCspectrum  = new TH1F("h0_ADCspectrum", "h0_ADCspectrum",  100, 0, 4000);
TH1F *h0_signal2noise = new TH1F("h0_signal2noise","h0_signal2noise", 100, 0,  100);

// ADC and Pedestal spectrum from found hits on tracks
// Layer 1
TH1F *h1_track_PEDspectrum  = new TH1F("h1_track_PEDspectrum", "h1_track_PEDspectrum",   50, 0,   50);
TH1F *h1_track_ADCspectrum  = new TH1F("h1_track_ADCspectrum", "h1_track_ADCspectrum",  100, 0, 4000);
TH1F *h1_track_signal2noise = new TH1F("h1_track_signal2noise","h1_track_signal2noise", 100, 0,  100);
// Layer 2
TH1F *h2_track_PEDspectrum  = new TH1F("h2_track_PEDspectrum", "h2_track_PEDspectrum",   50, 0,   50);
TH1F *h2_track_ADCspectrum  = new TH1F("h2_track_ADCspectrum", "h2_track_ADCspectrum",  100, 0, 4000);
TH1F *h2_track_signal2noise = new TH1F("h2_track_signal2noise","h2_track_signal2noise", 100, 0,  100);
// Layer 3
TH1F *h3_track_PEDspectrum  = new TH1F("h3_track_PEDspectrum", "h3_track_PEDspectrum",   50, 0,   50);
TH1F *h3_track_ADCspectrum  = new TH1F("h3_track_ADCspectrum", "h3_track_ADCspectrum",  100, 0, 4000);
TH1F *h3_track_signal2noise = new TH1F("h3_track_signal2noise","h3_track_signal2noise", 100, 0,  100);

// Angle corrected ADC spectrum for the 3 layers
TH1F *h1_track_angcorr_ADCspectrum  = new TH1F("h1_track_angcorr_ADCspectrum", "h1_track_ang_corr_ADCspectrum",  100, 0, 4000);
TH1F *h2_track_angcorr_ADCspectrum  = new TH1F("h2_track_angcorr_ADCspectrum", "h2_track_ang_corr_ADCspectrum",  100, 0, 4000);
TH1F *h3_track_angcorr_ADCspectrum  = new TH1F("h3_track_angcorr_ADCspectrum", "h3_track_ang_corr_ADCspectrum",  100, 0, 4000);

// This is pretty much the sandbox to play around in
// Correlations between layers
// TH1F *h9_YResidual        = new TH1F("h9_YResidual",        "h9_YResidual",      50,-3,3);
TH1F *h9_YResidual        = new TH1F("h9_YResidual",        "h9_YResidual",      150,-9,9); // modified by Xu Sun @ 02/13/2020
TH1F *h9_XResidual        = new TH1F("h9_XResidual",        "h9_XResidual",      150,-9,9); // modified by Xu Sun @ 02/13/2020
TH2F *h9_Residual        = new TH2F("h9_Residual",        "h9_Residual",         69,-12,21,  50,-3,3);
TH2F *h9_ADC1ADC2        = new TH2F("h9_ADC1ADC2",        "h9_ADC1ADC2",        100,0,1000, 100,0,1000);
TH2F *h9_ADC1_TrackAngle = new TH2F("h9_ADC1_TrackAngle", "h9_ADC1_TrackAngle", 100,0,60, 100,0,1000);
TH1F *h9_TrackAngle      = new TH1F("h9_TrackAngle",      "h9_TrackAngle",      100, 0, 90);
TH2F *h9_ADC1_ADCnext    = new TH2F("h9_ADC1_ADCnext",    "h9_ADC1_ADCnext",    100,0,4000,100,0,4000);

// Noise spectra and QA stuff
TH1F *h1_NoiseSpectrum  = new TH1F("h1_NoiseSpectrum", "h1_NoiseSpectrum",  100, 0, 100);
TH1F *h2_NoiseSpectrum  = new TH1F("h2_NoiseSpectrum", "h2_NoiseSpectrum",  100, 0, 100);
TH1F *h3_NoiseSpectrum  = new TH1F("h3_NoiseSpectrum", "h3_NoiseSpectrum",  100, 0, 100);
TH1F *h0_NoiseSpectrum  = new TH1F("h0_NoiseSpectrum", "h0_NoiseSpectrum",  100, 0, 100);
TH1F *h1_StrangeNoiseSpectrum  = new TH1F("h1_StrangeNoiseSpectrum", "h1_StrangeNoiseSpectrum",  100, 0, 100);
TH1F *h2_StrangeNoiseSpectrum  = new TH1F("h2_StrangeNoiseSpectrum", "h2_StrangeNoiseSpectrum",  100, 0, 100);
TH1F *h3_StrangeNoiseSpectrum  = new TH1F("h3_StrangeNoiseSpectrum", "h3_StrangeNoiseSpectrum",  100, 0, 100);
TH1F *h0_StrangeNoiseSpectrum  = new TH1F("h0_StrangeNoiseSpectrum", "h0_StrangeNoiseSpectrum",  100, 0, 100);

// Let's take the average of the pedestals of all channels in an APV
// Assuming that common mode noise moves all the channels up and down in the same way
// this should give a handle on how to treat common mode noise.
// The 3 layers
TH1F *h1_APVaveragePED[numARMs][numPorts][numAPVs];
TH1F *h2_APVaveragePED[numARMs][numPorts][numAPVs];
TH1F *h3_APVaveragePED[numARMs][numPorts][numAPVs];
// Any event where any average APV pedestal goes outside of 3 sigma should be marked broken
bool BrokenEvent[numEvents];

int StripeCount = 0;
int HighCount   = 0;
int TrackCount  = 0;


//  To keep track of where we are and what we are doing.
daqReader *rdr = NULL;
// Event handle
char *argv1;
long long currentEvent = -1;

// Gives the time bin position of the found maximum.
// Used for latency setting.
// For the 3 layers
TH1F *h1_maxTB = new TH1F("h1_maxTB","h1_maxTB", 10, 0, 10);
TH1F *h2_maxTB = new TH1F("h2_maxTB","h2_maxTB", 10, 0, 10);
TH1F *h3_maxTB = new TH1F("h3_maxTB","h3_maxTB", 10, 0, 10);
TH1F *h0_maxTB = new TH1F("h0_maxTB","h0_maxTB", 10, 0, 10);

// TTree to save hit information
// std::vector<double> *evt_rdo;
// std::vector<double> *evt_arm;
// std::vector<double> *evt_port;
// std::vector<double> *evt_apv;
// std::vector<double> hit_ch[128][9];
// std::vector<double> hit_tb[128][9];
// std::vector<double> hit_adc[128][9];

const int numFstARMs     = 2;
const int numFstPorts    = 2;  // note that the readout just gives 24 APVs on a single ARM, there is
const int numFstAPVs     = 12; // no actual Port information, we just introduce Port information ourselves
const int numFstROChannels = 1152; // numFstChannels*numFstTBins
const int numFstChannels = 128;
const int numFstTBins    = 9;
int evt_rdo[numFstARMs][numFstPorts][numFstAPVs];
int evt_arm[numFstARMs][numFstPorts][numFstAPVs];
int evt_port[numFstARMs][numFstPorts][numFstAPVs];
int evt_apv[numFstARMs][numFstPorts][numFstAPVs];
int hit_ch[numFstARMs][numFstPorts][numFstAPVs][numFstROChannels];
int hit_tb[numFstARMs][numFstPorts][numFstAPVs][numFstROChannels];
int hit_adc[numFstARMs][numFstPorts][numFstAPVs][numFstROChannels];
TTree *tree_Hits;


bool FileExists( std::string filename ) 
{{{
    struct stat stFileInfo;
    bool binReturn;
    int intStat;

    intStat = stat(filename.c_str(), &stFileInfo);
    if ( intStat == 0 )
	return true;
    else
	return false;
}}}

std::string constructPedestalFileName( char * daqFileName ) 
{{{

    std::string parser( daqFileName );

    std::string filePart = parser.substr( parser.find_last_of( "/" )+1 );

    std::stringstream outFileString;
    outFileString << filePart << ".pedestals.txt";


    return outFileString.str();
}}}

void getRefPedestalsFromFile( std::string pedestalFileName ) 
{{{
    int apvNum=0;
    int apvChannel=0;
    double pedestal=0.0;
    double pedestalStdDev=0.0;
    double pedestalRMS=0.0;

// We're skipping the reference pedestals for the time being, they just confuse things
    //std::ifstream pedFile( pedestalFileName.c_str() );
    //while(
//	pedFile 
//	    >> apvNum
//	    >> apvChannel
//	    >> pedestal
//	    >> pedestalStdDev
//	    >> pedestalRMS
  //  )
//    {
//	REFpedestals[apvNum][apvChannel]       = pedestal;
//	REFpedestalsStdDev[apvNum][apvChannel] = pedestalStdDev;
//	REFpedestalsRMS[apvNum][apvChannel]    = pedestalRMS;
        // cout << "REFpedestals[" << apvNum << "][" << apvChannel << "] = " << REFpedestals[apvNum][apvChannel] << endl;
//    }

}}}

void CalculatePedestals(char* daqFileName, char* mountpoint )
{{{
	cout << "===> Calculate Pedestals....." << endl;

        std::string pedestalFileName = constructPedestalFileName( daqFileName );

	//  Calculate a rolling average and standard deviation
	int counters[numARMs][numPorts][numAPVs][numChannels];
	double sumValues[numARMs][numPorts][numAPVs][numChannels];
	double sumValuesSquared[numARMs][numPorts][numAPVs][numChannels];

        // rough mean pedestal per APV so we can get this in the first pass and subtract in the second pass
        // this is just a number to get the histogram to center around zero instead of some arbitrary average pedestal value
        // be wary that this number is skewed because of hits which are not taken out in the first pass!
        double APVPedMean[numARMs][numPorts][numAPVs];
        int    countAPVPedMean[numARMs][numPorts][numAPVs];
        // this is the final average that should now be centered around zero because of subtracting APVPedMean and should not include hits anymore
        double APVPedAverage[numARMs][numPorts][numAPVs];
        int    countAPVPedAverage[numARMs][numPorts][numAPVs];

	int count;

        // Sometimes events are corrupted, don't know how to detect this
        // I think these events can result in something that looks like common mode noise
        // Anyway, any event that has any APV exhibiting more than (RejectCMN * sigmaCMN) will be flagged
        //   as broken and taken out of the pedestal generation and out of the subsequent analysis
        // We start with all events being accepted
        for ( int i = 0; i < numEvents; i++ ) {
            BrokenEvent[i] = false;
        }


        // {{{ First pass over the data to find pedestals including hits
        cout << "=====>First Pedestal Pass...." << endl;

	//  Just initializing the variables.
        for ( int g = 0; g < numARMs;  g++ ) {
            for ( int h = 0; h < numPorts; h++ ) {
	        for ( int i = 0; i < numAPVs;  i++ ) {
                    APVPedMean[g][h][i]      = 0.0;
                    countAPVPedMean[g][h][i] = 0;
	            for( int j = 0; j < numChannels; j++ ) {
		       counters[g][h][i][j]         = 0;
		       sumValues[g][h][i][j]        = 0.0;
		       sumValuesSquared[g][h][i][j] = 0.0;
	            }
	        }
            }
        }

	//  Setting up the daq reader.
	daqReader *rdr1 = new daqReader(daqFileName);
	if(mountpoint)
	    rdr1->setEvpDisk(mountpoint);
	//  Reading EVENTS.
	rdr1->get(0,EVP_TYPE_ANY);

        // run over numPeds events (if possible)
        count = 0;
	while( rdr1->status != EVP_STAT_EOR && count < numPeds )
	{
	    ++count;

	    daq_dta *dd ;
	    dd = rdr1->det("fgt")->get("adc");

	    double adc;

	 if( count >= numPedsDown && count < numPedsUp )	{
	    //	Processing RDO/ARM/APVs.
	    while( dd && dd->iterate() ) 
	    {
		fgt_adc_t *f = (fgt_adc_t *) dd->Void;

                int rdo  = dd->rdo;
                int arm  = dd->sec;
                int port = -999;
		int apv  = dd->pad;

                // there is no information about which ARM port this data is coming from
                // have to deduce from APV id
                if ( (apv >= 0) && (apv <= 11) ) {
                   port = 0;
                }
                else if ( (apv >= 12) && (apv <= 23) ) {
                   port = 1;
                   apv  = apv - 12; // just so we get a useful apv number in this port
                }

                //cout << "=======>RDO ARM PORT APV " << rdo << " " << arm << " " << port << " " << apv << endl;

                // NOTE THAT RDO/ARM/PORT ARE HARDWIRED HERE!!!
		bool pass = ( ( rdo == 1 ) && ( (arm == 0) || (arm == 1 ) ) && ( (port == 0) || (port == 1) ) &&  ( apv >= 0 ) && ( apv < numAPVs ) ) ;
		//  Processing each channel/timebin with a given adc.
		if( pass )
		{
                    //cout << "==========> Passed Cut: RDO ARM PORT APV " << rdo << " " << arm << " " << port << " " << apv << endl;
                    //cout << "+++++++>>>>> Event size = " << dd->ncontent << endl;
		    for( u_int ii = 0; ii < dd->ncontent; ii++ ) 
		    {
                        // ONLY make pedestals for timebin 0
                        //cout << "++++++>>>> TimeBin for " << ii << " = " << f[ii].tb << endl;
                        if ( f[ii].tb == PedTimeBin) {
			   adc = f[ii].adc;
			   sumValues[arm][port][apv][f[ii].ch] += adc;
			   sumValuesSquared[arm][port][apv][f[ii].ch] += adc * adc;
			   counters[arm][port][apv][f[ii].ch]++;
                           // average pedestal for channel
                           APVPedMean[arm][port][apv] += adc ;
                           countAPVPedMean[arm][port][apv] += 1;
                        }
		    }
		}
	    }
	  }//event number cut
	    rdr1->get(0,EVP_TYPE_ANY);
	} // end of event loop

        cout << "============>Ready with reading events " << endl;

        for ( int g = 0; g < numARMs; g++ ) {
            for ( int h = 0; h < numPorts; h++ ) {
	        for ( int i = 0; i < numAPVs; i++ ) {
                    // we can now calculate the average pedestal per APV for this event
                    APVPedMean[g][h][i] = APVPedMean[g][h][i] / countAPVPedMean[g][h][i];
	            for ( int j = 0; j < numChannels; j++ ) {
                        // and calculate the average pedestal, sigma and RMS for each channel
		        pedestals[g][h][i][j] = sumValues[g][h][i][j] / counters[g][h][i][j];
		        pedestalsStdDev[g][h][i][j] = sqrt ( ( sumValuesSquared[g][h][i][j] - (double) counters[g][h][i][j] * pedestals[g][h][i][j] * pedestals[g][h][i][j]) 
                                                             / (double) ( counters[g][h][i][j] - 1 ));
		        pedestalsRMS[g][h][i][j]    = sqrt ( pedestals[g][h][i][j] * pedestals[g][h][i][j] - pedestalsStdDev[g][h][i][j] * pedestalsStdDev[g][h][i][j]);
	            }
	        }
            }
        }

	delete rdr1;
        // }}} end of first pedestal pass


        // {{{ Second pass over the data to find pedestals excluding hits
        cout << "=====>Second Pedestal Pass...." << endl;

	//  Just initializing the variables.
        for ( int g = 0; g < numARMs;  g++ ) {
            for ( int h = 0; h < numPorts; h++ ) {
                for ( int i = 0; i < numAPVs;  i++ ) {

                    // stupid C++ string handling....
                    stringstream tname;
                    tname << "h1_APVaveragePED_" << g << "_" << h << "_" << i ;
                    TString name1 = tname.str();
                    h1_APVaveragePED[g][h][i] = new TH1F(name1, name1, 100, -500, 500); // temporary values for range! needs to be dynamic
                    tname << "h2_APVaveragePED_" << g << "_" << h << "_" << i ;
                    TString name2 = tname.str();
                    h2_APVaveragePED[g][h][i] = new TH1F(name2, name2, 100, -500, 500); // temporary values for range! needs to be dynamic
                    tname << "h3_APVaveragePED_" << g << "_" << h << "_" << i ;
                    TString name3 = tname.str();
                    h3_APVaveragePED[g][h][i] = new TH1F(name3, name3, 100, -500, 500); // temporary values for range! needs to be dynamic

                    APVPedAverage[g][h][i]      = 0.0;
                    countAPVPedAverage[g][h][i] = 0;
	            for ( int j = 0; j < numChannels; j++ ) {
		        counters[g][h][i][j]         = 0;
		        sumValues[g][h][i][j]        = 0.0;
		        sumValuesSquared[g][h][i][j] = 0.0;
                    }
                }
	    }
	}

	//  Setting up the daq reader.
	daqReader *rdr2 = new daqReader(daqFileName);
	if(mountpoint)
	    rdr2->setEvpDisk(mountpoint);
	//  Reading EVENTS.
	rdr2->get(0,EVP_TYPE_ANY);

        // Run over numPeds events (if possible)       
        count = 0;
	while( rdr2->status != EVP_STAT_EOR && count < numPeds )
	{
	    ++count;

	    daq_dta *dd ;
	    dd = rdr2->det("fgt")->get("adc");

	    double adc;

	 if( count >= numPedsDown && count < numPedsUp )        {
	    //	Processing RDO/ARM/APVs.
	    while( dd && dd->iterate() ) 
	    {
		fgt_adc_t *f = (fgt_adc_t *) dd->Void;

                int rdo  = dd->rdo;
                int arm  = dd->sec;
                int port = -999;
                int apv  = dd->pad;

                // there is no information about which ARM port this data is coming from
                // have to deduce from APV id
                if ( (apv >= 0) && (apv <= 11) ) {
                   port = 0;
                }
                else if ( (apv >= 12) && (apv <= 23) ) {
                   port = 1;
                   apv  = apv - 12; // just so we get a useful apv number in this port
                }

                //cout << "===PASS 2====>RDO ARM PORT APV " << rdo << " " << arm << " " << port << " " << apv << endl;

                // NOTE THAT RDO/ARM/PORT ARE HARDWIRED HERE!!!
                bool pass = ( ( rdo == 1 ) && ( (arm == 0) || (arm == 1) ) && ( (port == 0) || (port == 1) ) &&  ( apv >= 0 ) && ( apv < numAPVs ) ) ;
		//  Processing each channel/timebin with a given adc.
		if ( pass )
		{
		   // std::cout << "zzz Pass2" << " = " << dd->sec << " | " << dd->pad << " -> " << apv << std::endl;
		

		    for( u_int ii=0; ii < dd->ncontent; ii++ ) 
		    {
                        // ONLY make pedestals for timebin 0
                        if ( f[ii].tb == PedTimeBin ) {
			   adc = f[ii].adc;
                           // Exclude entries which look like hits, make PEDCUT*sigma cut
                           // Let's also make sure that the ADC values are in a range that is sane, i.e. 0 to 4095
                           if ( ( adc <  ( pedestals[arm][port][apv][f[ii].ch] + ( PEDCUT * pedestalsStdDev[arm][port][apv][f[ii].ch] ) ) ) &&
                                ( adc >= 0 && adc < 4096)                                                                 ) {
			      sumValues[arm][port][apv][f[ii].ch]        += adc;
			      sumValuesSquared[arm][port][apv][f[ii].ch] += adc * adc;
			      counters[arm][port][apv][f[ii].ch]++;
                              // average pedestal for channel
                              APVPedAverage[arm][port][apv]      += adc;
                              countAPVPedAverage[arm][port][apv] += 1;
                           }
                           else {
                              //cout << "PedPass2 Event: " << count << "  Excl APV/ch: " << apv << " / " << f[ii].ch << endl;
                           }
                        }
		    }
		}
	    }
          } //event number cut

            for ( int g = 0; g < numARMs; g++ ) {
                for ( int h = 0; h < numPorts; h++ ) {
                    for ( int i = 0; i < numAPVs; i++ ) {
                        // we can now calculate the average pedestal per APV for this event
                        APVPedAverage[g][h][i] = APVPedAverage[g][h][i] / countAPVPedAverage[g][h][i];
                        // Corrupted events keep on slipping through, this is a VERY dirty trick to keep them out of the histograms
                        if ( (APVPedAverage[g][h][i] >= 0) && (APVPedAverage[g][h][i]< 4096) ) {
                           h1_APVaveragePED[g][h][i]->Fill(APVPedAverage[g][h][i]-APVPedMean[g][h][i], 1.0);
                        }
                        APVPedAverage[g][h][i]      = 0.0;
                        countAPVPedAverage[g][h][i] = 0;
                    }
                }
            }

	    rdr2->get(0,EVP_TYPE_ANY);
	} // end of event loop

        for ( int g = 0; g < numARMs; g++ ) {
            for ( int h = 0; h < numPorts; h++ ) {
                for ( int i = 0; i < numAPVs; i++ ) {
	            for ( int j=0; j < numChannels; j++ ) {
		        pedestals[g][h][i][j]       = sumValues[g][h][i][j] / counters[g][h][i][j];
		        pedestalsStdDev[g][h][i][j] = sqrt ( ( sumValuesSquared[g][h][i][j] - (double) counters[g][h][i][j] * pedestals[g][h][i][j] * pedestals[g][h][i][j]) 
                                                       / (double) ( counters[g][h][i][j] - 1 ));
		        pedestalsRMS[g][h][i][j]    = sqrt ( pedestals[g][h][i][j] * pedestals[g][h][i][j] - pedestalsStdDev[g][h][i][j] * pedestalsStdDev[g][h][i][j]);
                        // Can't remember what this is supposed to do
                        //if ( (g==3) && (h==0) && (i==3) ) {
                        //   cout << "ARM Port APV ADC : " << g << " " << h << " " << i << " " << pedestals[g][h][i][j] << endl;
                        //}
                    }
                }
	    }
	}

	delete rdr2;
        // }}} end of second pedestal pass

/*
        // NOTE THAT NOTHING HAS BEEN DONE TO THIS FOR A WHILE, NEEDS TO BE OVERHAULED COMPLETELY GvN 06/25/2012
        // {{{ Third pass over the data to find average APV pedestals that are out of 3 sigma = broken events

	//  Just initializing the variables.
	for( int i = 0; i < numAPVs; i++ ) {
            APVPedAverage[i] = 0.0;
            countAPVPedAverage[i] = 0;
	    for( int j = 0; j < numChannels; j++ ) {
		counters[i][j] = 0;
		sumValues[i][j] = 0.0;
		sumValuesSquared[i][j] = 0.0;
	    }
	}

	//  Setting up the daq reader.
	daqReader *rdr3 = new daqReader(daqFileName);
	if(mountpoint)
	    rdr3->setEvpDisk(mountpoint);
	//  Reading EVENTS.
	rdr3->get(0,EVP_TYPE_ANY);

        // Run over numPeds events (if possible)       
        count = 0;
	while( rdr3->status != EVP_STAT_EOR && count < numPeds )
	{
	    ++count;

	    daq_dta *dd ;
	    dd = rdr3->det("fgt")->get("adc");

	    double adc;

	    //	Processing RDO/ARM/APVs.
	    while( dd && dd->iterate() ) 
	    {
		fgt_adc_t *f = (fgt_adc_t *) dd->Void;

		int apv = dd->pad;

                bool pass = ( ( dd->rdo == 1 ) && ( dd->sec == 4 ) &&  ( apv >= 0 ) && ( apv < numAPVs ) ) ;
		//  Processing each channel/timebin with a given adc.
		if ( pass )
		{

		    for( u_int ii=0; ii < dd->ncontent; ii++ ) 
		    {
                        // ONLY make pedestals for timebin 0
                        if ( f[ii].tb == PedTimeBin ) {
			   adc = f[ii].adc;
                           // Exclude entries which look like hits, make PEDCUT*sigma cut
                           // Note that hits are negative still, let's exclude bigger and smaller
                           // Let's also make sure that the ADC values are in a range that is sane, i.e. 0 to 4095
                           if ( ( adc >  ( pedestals[apv][f[ii].ch] - ( PEDCUT * pedestalsStdDev[apv][f[ii].ch] ) ) ) &&
                                ( adc <  ( pedestals[apv][f[ii].ch] + ( PEDCUT * pedestalsStdDev[apv][f[ii].ch] ) ) ) &&
                                ( adc >= 0 && adc < 4096)                                                                 ) {
			      sumValues[apv][f[ii].ch] += adc;
			      sumValuesSquared[apv][f[ii].ch] += adc * adc;
			      counters[apv][f[ii].ch]++;
                              // average pedestal for channel
                              // MAYBE THIS SHOULD BE INSIDE THE CUT SELECTION? --> YES, if only to make sure that no corrupted events slip in
                              // Interesting, corrupt events still slip through, too much screwed up too reliably using a hit cut
                              // imposed also a sanity cut on ADC values --> ugh, even that didn't help
                              APVPedAverage[apv] += adc;
                              countAPVPedAverage[apv] += 1;
                           }
                           else {
                              //cout << "PedPass2 Event: " << count << "  Excl APV/ch: " << apv << " / " << f[ii].ch << endl;
                           }
                        }
		    }
		}
	    }
            // we can now calculate the average pedestal per APV for this event
            for ( int i=0; i < numAPVs; i++) {
                APVPedAverage[i] = APVPedAverage[i] / countAPVPedAverage[i];
                }
                APVPedAverage[i] = 0.0;
                countAPVPedAverage[i] = 0;
            }

	    rdr3->get(0,EVP_TYPE_ANY);
	} // end of event loop

	for( int i=0; i < numAPVs; i++)
	{
	    for( int j=0; j < numChannels; j++)
	    {
		pedestals[i][j] = sumValues[i][j] / counters[i][j];
		pedestalsStdDev[i][j] =
		    sqrt
		    (
			(
			    sumValuesSquared[i][j]
			    -
			    (double) counters[i][j] * pedestals[i][j] * pedestals[i][j]
			)
			/ (double) ( counters[i][j] - 1 )
		    );
		pedestalsRMS[i][j] =
		    sqrt
		    (
			pedestals[i][j] * pedestals[i][j]
			-
			pedestalsStdDev[i][j] * pedestalsStdDev[i][j]
		    );
	    }
	}

	delete rdr3;
        // }}} end of third pedestal pass
*/

        // NO PEDESTAL FILE WRITING ANYMORE, WHENEVER REACTIVATED, FIX THE LOOP INDEXES TO ADD ARM AND PORT GvN 06/25/2012
	//std::ofstream pedFile(pedestalFileName.c_str());
	//if ( !pedFile ) {
	//    std::cerr
	//	<< "Could not open pedestal file for writing" 
	//	<< std::endl;
	//}
        //
	//for ( int i_apv = 0; i_apv < numAPVs; i_apv++ ) {
	//    for ( int i_ch = 0; i_ch < numChannels; i_ch++ ) {
	//	pedFile 
	//	    << i_apv                        << " " 
	//	    << i_ch                         << " " 
	//	    << pedestals[i_apv][i_ch]       << " "
	//	    << pedestalsStdDev[i_apv][i_ch] << " "
	//	    << pedestalsRMS[i_apv][i_ch]    << " "
	//	    << std::endl;
	//    }
	//}


}}}

void showPedestals() 
{{{
    cout << "=====> Showing Pedestals..." << endl;

    for ( int i = 0; i < numAPVs; ++i ) {
	for ( int j = 0; j < numChannels; ++j ) {
            // Layer 1 (ARM 0, PORT 0)
	    g1_ped1Sigma->SetPoint(    i*numChannels+j, i*numChannels+j,    pedestals[0][0][i][j]               );
	    g1_REFped1Sigma->SetPoint( i*numChannels+j, i*numChannels+j, REFpedestals[0][0][i][j]       + 10000 );
	    g1_sigmaPed->SetPoint(     i*numChannels+j, i*numChannels+j,    pedestalsStdDev[0][0][i][j]         );
	    g1_REFsigmaPed->SetPoint(  i*numChannels+j, i*numChannels+j, REFpedestalsStdDev[0][0][i][j] +   500 );
            //if ( i <= 5 ) {
               h1_NoiseSpectrum->Fill(pedestalsStdDev[0][0][i][j],1.0);
            //}
            // Layer 2 (ARM 0, PORT 1)
            g2_ped1Sigma->SetPoint(    i*numChannels+j, i*numChannels+j,    pedestals[0][1][i][j]               );
            g2_REFped1Sigma->SetPoint( i*numChannels+j, i*numChannels+j, REFpedestals[0][1][i][j]       + 10000 );
            g2_sigmaPed->SetPoint(     i*numChannels+j, i*numChannels+j,    pedestalsStdDev[0][1][i][j]         );
            g2_REFsigmaPed->SetPoint(  i*numChannels+j, i*numChannels+j, REFpedestalsStdDev[0][1][i][j] +   500 );
            h2_NoiseSpectrum->Fill(pedestalsStdDev[0][1][i][j],1.0);
            // Layer 3 (ARM 1, PORT 0)
            g3_ped1Sigma->SetPoint(    i*numChannels+j, i*numChannels+j,    pedestals[1][0][i][j]               );
            g3_REFped1Sigma->SetPoint( i*numChannels+j, i*numChannels+j, REFpedestals[1][0][i][j]       + 10000 );
            g3_sigmaPed->SetPoint(     i*numChannels+j, i*numChannels+j,    pedestalsStdDev[1][0][i][j]         );
            g3_REFsigmaPed->SetPoint(  i*numChannels+j, i*numChannels+j, REFpedestalsStdDev[1][0][i][j] +   500 );
            //if ( i <= 5 ) {
               h3_NoiseSpectrum->Fill(pedestalsStdDev[1][0][i][j],1.0);
            //}
            // Layer 0 (ARM 1, PORT 1)
            g0_ped1Sigma->SetPoint(    i*numChannels+j, i*numChannels+j,    pedestals[1][1][i][j]               );
            g0_REFped1Sigma->SetPoint( i*numChannels+j, i*numChannels+j, REFpedestals[1][1][i][j]       + 10000 );
            g0_sigmaPed->SetPoint(     i*numChannels+j, i*numChannels+j,    pedestalsStdDev[1][1][i][j]         );
            g0_REFsigmaPed->SetPoint(  i*numChannels+j, i*numChannels+j, REFpedestalsStdDev[1][1][i][j] +   500 );
            //if ( i <= 5 ) {
               h0_NoiseSpectrum->Fill(pedestalsStdDev[1][1][i][j],1.0);
            //}
	}
    }

    
    float Mean1 = h1_NoiseSpectrum->GetMean();
    float Mean2 = h2_NoiseSpectrum->GetMean();
    float Mean3 = h3_NoiseSpectrum->GetMean();
    float Mean0 = h0_NoiseSpectrum->GetMean();
    float WinDown1  = 3.0 * h1_NoiseSpectrum->GetRMS();
    float WinDown2  = 3.0 * h2_NoiseSpectrum->GetRMS();
    float WinDown3  = 3.0 * h3_NoiseSpectrum->GetRMS();
    float WinDown0  = 3.0 * h0_NoiseSpectrum->GetRMS();
    float WinUp1    = 8.0 * h1_NoiseSpectrum->GetRMS();
    float WinUp2    = 8.0 * h2_NoiseSpectrum->GetRMS();
    float WinUp3    = 8.0 * h3_NoiseSpectrum->GetRMS();
    float WinUp0    = 8.0 * h0_NoiseSpectrum->GetRMS();
    cout << "##########List of broken channels##########" << endl;
    for ( int i = 0; i < numAPVs; ++i ) {
	for ( int j = 0; j < numChannels; ++j ) {
            float Noise1 = pedestalsStdDev[0][0][i][j];
            float Noise2 = pedestalsStdDev[0][1][i][j];
            float Noise3 = pedestalsStdDev[1][0][i][j];
            float Noise0 = pedestalsStdDev[1][1][i][j];
            if (               ( ( Noise1 < Mean1-WinDown1 ) || ( Noise1 > Mean1+WinUp1 ) ) ) {
               cout << "###Section A - APV " << i << " - " << "Chan " << j << " ###" << endl;
               h1_StrangeNoiseSpectrum->Fill(Noise1,1.0);
            }
            if (               ( ( Noise2 < Mean2-WinDown2 ) || ( Noise2 > Mean2+WinUp2 ) ) ) {
               cout << "###Section B - APV " << i << " - " << "Chan " << j << " ###" << endl;
               h2_StrangeNoiseSpectrum->Fill(Noise2,1.0);
            }
            if (               ( ( Noise3 < Mean3-WinDown3 ) || ( Noise3 > Mean3+WinUp3 ) ) ) {
               cout << "###Section C - APV " << i << " - " << "Chan " << j << " ###" << endl;
               h3_StrangeNoiseSpectrum->Fill(Noise3,1.0);
            }
            if (               ( ( Noise0 < Mean0-WinDown0 ) || ( Noise0 > Mean0+WinUp0 ) ) ) {
               cout << "###FST - APV " << i << " - " << "Chan " << j << " ###" << endl;
               h0_StrangeNoiseSpectrum->Fill(Noise0,1.0);
            }
	}
    }
    cout << "###########################################" << endl;

    p1_PedDisplay->cd();
    p1_PedDisplay->SetGridx(1);
    p1_PedDisplay->SetGridy(1);
    g1_REFped1Sigma->GetHistogram()->SetTitle("Pedestals;Channel;ADC");
    //g1_REFped1Sigma->GetHistogram()->SetTitleSize(3.0);
    g1_REFped1Sigma->SetFillColor(kWhite); // White background
    g1_REFped1Sigma->SetFillStyle(1001);   // Solid background
    g1_REFped1Sigma->SetMarkerStyle(21);
    g1_REFped1Sigma->SetMarkerSize(0.3);
    g1_REFped1Sigma->SetMarkerColor(kGreen);
    g1_REFped1Sigma->SetLineColor(kGreen);
    g1_REFped1Sigma->SetLineColor(kGreen);
    //g1_REFped1Sigma->SetMinimum(0);
    g1_REFped1Sigma->SetMinimum(0000);
    g1_REFped1Sigma->SetMaximum(4095);
    //g1_REFped1Sigma->SetMaximum(2000);
    g1_REFped1Sigma->GetXaxis()->SetRangeUser(( 0  )*numChannels - (11+1-0)*numChannels/10.0, 
                                              (11+1)*numChannels + (11+1-0)*numChannels/10.0 );
    //g1_REFped1Sigma->GetXaxis()->SetNdivisions(10);
    g1_REFped1Sigma->GetXaxis()->SetLabelSize(0.1);
    g1_REFped1Sigma->GetYaxis()->SetLabelSize(0.1);
    g1_REFped1Sigma->Draw("AP");
    p1_PedDisplay->Update();
    g1_ped1Sigma->SetMarkerStyle(21);
    g1_ped1Sigma->SetMarkerSize(0.4);
    g1_ped1Sigma->SetMarkerColor(kBlue);
    g1_ped1Sigma->SetLineColor(kBlue);
    g1_ped1Sigma->Draw("P");
    p1_PedDisplay->Update();

    p1_PedSigma->cd();
    p1_PedSigma->SetGridx(1);
    p1_PedSigma->SetGridy(1);
    g1_REFsigmaPed->SetTitle("Sigma of Pedestals;Channel;ADC");
    g1_REFsigmaPed->SetMarkerStyle(21);
    g1_REFsigmaPed->SetMarkerSize(0.3);
    g1_REFsigmaPed->SetMarkerColor(kGreen);
    g1_REFsigmaPed->SetLineColor(kGreen);
    g1_REFsigmaPed->SetLineColor(kGreen);
    g1_REFsigmaPed->SetMinimum(0);
    g1_REFsigmaPed->SetMaximum(60);
    //g1_REFsigmaPed->SetMaximum(127);
    g1_REFsigmaPed->GetXaxis()->SetRangeUser((  0  )*numChannels - (11+1-0 )*numChannels/10.0, 
                                             (  11+1)*numChannels + (11+1-0 )*numChannels/10.0 );
    g1_REFsigmaPed->GetXaxis()->SetLabelSize(0.1);
    g1_REFsigmaPed->GetYaxis()->SetLabelSize(0.1);
    g1_REFsigmaPed->Draw("AP");
    g1_sigmaPed->SetMarkerStyle(21);
    g1_sigmaPed->SetMarkerSize(0.4);
    g1_sigmaPed->SetMarkerColor(kBlue);
    g1_sigmaPed->SetLineColor(kBlue);
    g1_sigmaPed->Draw("P");
    p1_PedSigma->Update();

    p2_PedDisplay->cd();
    p2_PedDisplay->SetGridx(1);
    p2_PedDisplay->SetGridy(1);
    g2_REFped1Sigma->GetHistogram()->SetTitle("Pedestals;Channel;ADC");
    //g2_REFped1Sigma->GetHistogram()->SetTitleSize(3.0);
    g2_REFped1Sigma->SetMarkerStyle(21);
    g2_REFped1Sigma->SetMarkerSize(0.3);
    g2_REFped1Sigma->SetMarkerColor(kGreen);
    g2_REFped1Sigma->SetLineColor(kGreen);
    g2_REFped1Sigma->SetLineColor(kGreen);
    //g2_REFped1Sigma->SetMinimum(0);
    g2_REFped1Sigma->SetMinimum(0000);
    g2_REFped1Sigma->SetMaximum(4095);
    //g2_REFped1Sigma->SetMaximum(2000);
    g2_REFped1Sigma->GetXaxis()->SetRangeUser((  0  )*numChannels - (11+1-0 )*numChannels/10.0, 
                                              ( 11+1)*numChannels + (11+1-0 )*numChannels/10.0 );

    //g2_REFped1Sigma->GetXaxis()->SetNdivisions(10);
    g2_REFped1Sigma->GetXaxis()->SetLabelSize(0.1);
    g2_REFped1Sigma->GetYaxis()->SetLabelSize(0.1);
    g2_REFped1Sigma->Draw("AP");
    p2_PedDisplay->Update();
    g2_ped1Sigma->SetMarkerStyle(21);
    g2_ped1Sigma->SetMarkerSize(0.4);
    g2_ped1Sigma->SetMarkerColor(kBlue);
    g2_ped1Sigma->SetLineColor(kBlue);
    g2_ped1Sigma->Draw("P");
    p2_PedDisplay->Update();

    p2_PedSigma->cd();
    p2_PedSigma->SetGridx(1);
    p2_PedSigma->SetGridy(1);
    g2_REFsigmaPed->SetTitle("Sigma of Pedestals;Channel;ADC");
    g2_REFsigmaPed->SetMarkerStyle(21);
    g2_REFsigmaPed->SetMarkerSize(0.3);
    g2_REFsigmaPed->SetMarkerColor(kGreen);
    g2_REFsigmaPed->SetLineColor(kGreen);
    g2_REFsigmaPed->SetLineColor(kGreen);
    g2_REFsigmaPed->SetMinimum(0);
    g2_REFsigmaPed->SetMaximum(60);
    //g2_REFsigmaPed->SetMaximum(127);
    g2_REFsigmaPed->GetXaxis()->SetRangeUser((  0  )*numChannels - (11+1-0 )*numChannels/10.0, 
                                             ( 11+1)*numChannels + (11+1-0 )*numChannels/10.0 );
    g2_REFsigmaPed->GetXaxis()->SetLabelSize(0.1);
    g2_REFsigmaPed->GetYaxis()->SetLabelSize(0.1);
    g2_REFsigmaPed->Draw("AP");
    g2_sigmaPed->SetMarkerStyle(21);
    g2_sigmaPed->SetMarkerSize(0.4);
    g2_sigmaPed->SetMarkerColor(kBlue);
    g2_sigmaPed->SetLineColor(kBlue);
    g2_sigmaPed->Draw("P");
    p2_PedSigma->Update();


    p3_PedDisplay->cd();
    p3_PedDisplay->SetGridx(1);
    p3_PedDisplay->SetGridy(1);
    g3_REFped1Sigma->GetHistogram()->SetTitle("Pedestals;Channel;ADC");
    //g3_REFped1Sigma->GetHistogram()->SetTitleSize(3.0);
    g3_REFped1Sigma->SetMarkerStyle(21);
    g3_REFped1Sigma->SetMarkerSize(0.3);
    g3_REFped1Sigma->SetMarkerColor(kGreen);
    g3_REFped1Sigma->SetLineColor(kGreen);
    g3_REFped1Sigma->SetLineColor(kGreen);
    //g3_REFped1Sigma->SetMinimum(0);
    g3_REFped1Sigma->SetMinimum(0000);
    g3_REFped1Sigma->SetMaximum(4095);
    //g3_REFped1Sigma->SetMaximum(2000);
    g3_REFped1Sigma->GetXaxis()->SetRangeUser((  0  )*numChannels - (11+1-0 )*numChannels/10.0, 
                                              ( 11+1)*numChannels + (11+1-0 )*numChannels/10.0 );
    //g3_REFped1Sigma->GetXaxis()->SetNdivisions(10);
    g3_REFped1Sigma->GetXaxis()->SetLabelSize(0.1);
    g3_REFped1Sigma->GetYaxis()->SetLabelSize(0.1);
    g3_REFped1Sigma->Draw("AP");
    p3_PedDisplay->Update();
    g3_ped1Sigma->SetMarkerStyle(21);
    g3_ped1Sigma->SetMarkerSize(0.4);
    g3_ped1Sigma->SetMarkerColor(kBlue);
    g3_ped1Sigma->SetLineColor(kBlue);
    g3_ped1Sigma->Draw("P");
    p3_PedDisplay->Update();


    p3_PedSigma->cd();
    p3_PedSigma->SetGridx(1);
    p3_PedSigma->SetGridy(1);
    g3_REFsigmaPed->SetTitle("Sigma of Pedestals;Channel;ADC");
    g3_REFsigmaPed->SetMarkerStyle(21);
    g3_REFsigmaPed->SetMarkerSize(0.3);
    g3_REFsigmaPed->SetMarkerColor(kGreen);
    g3_REFsigmaPed->SetLineColor(kGreen);
    g3_REFsigmaPed->SetLineColor(kGreen);
    g3_REFsigmaPed->SetMinimum(0);
    g3_REFsigmaPed->SetMaximum(60);
    //g3_REFsigmaPed->SetMaximum(127);
    g3_REFsigmaPed->GetXaxis()->SetRangeUser((  0  )*numChannels - (11+1-0 )*numChannels/10.0, 
                                             ( 11+1)*numChannels + (11+1-0 )*numChannels/10.0 );
    g3_REFsigmaPed->GetXaxis()->SetLabelSize(0.1);
    g3_REFsigmaPed->GetYaxis()->SetLabelSize(0.1);
    g3_REFsigmaPed->Draw("AP");
    g3_sigmaPed->SetMarkerStyle(21);
    g3_sigmaPed->SetMarkerSize(0.4);
    g3_sigmaPed->SetMarkerColor(kBlue);
    g3_sigmaPed->SetLineColor(kBlue);
    g3_sigmaPed->Draw("P");
    p3_PedSigma->Update();

    p0_PedDisplay->cd();
    p0_PedDisplay->SetGridx(1);
    p0_PedDisplay->SetGridy(1);
    g0_REFped1Sigma->GetHistogram()->SetTitle("Pedestals;Channel;ADC");
    //g0_REFped1Sigma->GetHistogram()->SetTitleSize(3.0);
    g0_REFped1Sigma->SetMarkerStyle(21);
    g0_REFped1Sigma->SetMarkerSize(0.3);
    g0_REFped1Sigma->SetMarkerColor(kGreen);
    g0_REFped1Sigma->SetLineColor(kGreen);
    g0_REFped1Sigma->SetLineColor(kGreen);
    //g0_REFped1Sigma->SetMinimum(0);
    g0_REFped1Sigma->SetMinimum(0000);
    g0_REFped1Sigma->SetMaximum(4095);
    //g0_REFped1Sigma->SetMaximum(2000);
    g0_REFped1Sigma->GetXaxis()->SetRangeUser((  0  )*numChannels - (11+1-0 )*numChannels/10.0, 
                                              ( 11+1)*numChannels + (11+1-0 )*numChannels/10.0 );
    //g0_REFped1Sigma->GetXaxis()->SetNdivisions(10);
    g0_REFped1Sigma->GetXaxis()->SetLabelSize(0.1);
    g0_REFped1Sigma->GetYaxis()->SetLabelSize(0.1);
    g0_REFped1Sigma->Draw("AP");
    p0_PedDisplay->Update();
    g0_ped1Sigma->SetMarkerStyle(21);
    g0_ped1Sigma->SetMarkerSize(0.4);
    g0_ped1Sigma->SetMarkerColor(kBlue);
    g0_ped1Sigma->SetLineColor(kBlue);
    g0_ped1Sigma->Draw("P");
    p0_PedDisplay->Update();


    p0_PedSigma->cd();
    p0_PedSigma->SetGridx(1);
    p0_PedSigma->SetGridy(1);
    g0_REFsigmaPed->SetTitle("Sigma of Pedestals;Channel;ADC");
    g0_REFsigmaPed->SetMarkerStyle(21);
    g0_REFsigmaPed->SetMarkerSize(0.3);
    g0_REFsigmaPed->SetMarkerColor(kGreen);
    g0_REFsigmaPed->SetLineColor(kGreen);
    g0_REFsigmaPed->SetLineColor(kGreen);
    g0_REFsigmaPed->SetMinimum(0);
    g0_REFsigmaPed->SetMaximum(60);
    //g0_REFsigmaPed->SetMaximum(127);
    g0_REFsigmaPed->GetXaxis()->SetRangeUser((  0  )*numChannels - (11+1-0 )*numChannels/10.0, 
                                             ( 11+1)*numChannels + (11+1-0 )*numChannels/10.0 );
    g0_REFsigmaPed->GetXaxis()->SetLabelSize(0.1);
    g0_REFsigmaPed->GetYaxis()->SetLabelSize(0.1);
    g0_REFsigmaPed->Draw("AP");
    g0_sigmaPed->SetMarkerStyle(21);
    g0_sigmaPed->SetMarkerSize(0.4);
    g0_sigmaPed->SetMarkerColor(kBlue);
    g0_sigmaPed->SetLineColor(kBlue);
    g0_sigmaPed->Draw("P");
    p0_PedSigma->Update();


/*
    // NOTE THAT NOTHING HAPPENED WITH THIS FOR A WHILE, NEEDS OVERHAUL, GvN 06/25/2012
    cout << "=======> First CMN..." << endl;
    //for ( int i = 0; i < numAPV; i++ ) {
    for ( int i = 0; i < 12; i++ ) {
        p1_APVaveragePED[i]->cd();
        gStyle->SetOptStat(111111);
        gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
        gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
        h1_APVaveragePED[i]->SetTitle("APV average pedestal;ADC;Counts");
        h1_APVaveragePED[i]->SetFillColor(kGreen);
        h1_APVaveragePED[i]->GetXaxis()->SetLabelSize(0.1);
        h1_APVaveragePED[i]->Draw("BAR");
        p1_APVaveragePED[i]->Update();
    }
    cout << "=======> Second CMN..." << endl;
    for ( int i = 0; i < 12; i++ ) {
        p2_APVaveragePED[i]->cd();
        gStyle->SetOptStat(111111);
        gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
        gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
        h2_APVaveragePED[i]->SetTitle("APV average pedestal;ADC;Counts");
        h2_APVaveragePED[i]->SetFillColor(kGreen);
        h2_APVaveragePED[i]->GetXaxis()->SetLabelSize(0.1);
        h2_APVaveragePED[i]->Draw("BAR");
        p2_APVaveragePED[i]->Update();
    }
*/

}}}

void processEvent( daq_dta *dd, bool display_event = false ) 
{{{
    if ( currentEvent%100 == 0 ) {
       cout << "===Process event " << currentEvent << "....." << endl;
    }

    // SANE EVENT HACK, record APV's that are encountered in event
    // Only proceed with analysis if ALL APV's are present
    bool APVpresent[numARMs][numPorts][numAPVs];
    for ( int i_arm = 0; i_arm < numARMs; i_arm++ ) {
        for ( int i_port = 0; i_port < numPorts; i_port++ ) { 
            for ( int i_apv = 0; i_apv < numAPVs; i_apv++ ) {
                APVpresent[numARMs][numPorts][i_apv] = false;
            }
        }
    } 

    // Another sanity check; if we find more than MAX_NO_HITS_PER_EVENT hits then we assume that this event is broken
    int noHits = 0;

    long long eventADCvsChannelNum1[ numTBins ] = {0,0,0,0,0,0,0};
    long long eventADCvsChannelNum2[ numTBins ] = {0,0,0,0,0,0,0};
    long long eventADCvsChannelNum3[ numTBins ] = {0,0,0,0,0,0,0};
    long long eventADCvsChannelNum0[ numTBins ] = {0,0,0,0,0,0,0};

    // Store all 7 time bins for all APV's and all channels, ugh
    double SignalPedCorrected[numARMs][numPorts][numAPVs][numChannels][numTBins];
    double SignalUnCorrected[numARMs][numPorts][numAPVs][numChannels][numTBins];
    // Clear everything
    for ( int i_arm = 0; i_arm < numARMs; i_arm++ ) {
        for ( int i_port = 0; i_port < numPorts; i_port++ ) {
            for ( int i_apv = 0; i_apv < numAPVs; i_apv++ ) {
                for ( int i_chan = 0; i_chan < numChannels; i_chan++ ) {
                    for ( int i_tbin = 0; i_tbin < numTBins; i_tbin++ ) {
                        SignalPedCorrected[i_arm][i_port][i_apv][i_chan][i_tbin] = 99999.00;
                        SignalUnCorrected[i_arm][i_port][i_apv][i_chan][i_tbin]  = 99999.00;
                    }
                }
            }
        }
    }

    // clear RO info for each event
    for ( int i_arm = 0; i_arm < numFstARMs; i_arm++ ) 
    {
      for ( int i_port = 0; i_port < numFstPorts; i_port++ ) 
      {
	for ( int i_apv = 0; i_apv < numFstAPVs; i_apv++ ) 
	{
	  evt_rdo[i_arm][i_port][i_apv] = -1;
	  evt_arm[i_arm][i_port][i_apv] = -1;
	  evt_port[i_arm][i_port][i_apv] = -1;
	  evt_apv[i_arm][i_port][i_apv] = -1;
	  for ( int i_chan = 0; i_chan < numFstROChannels; i_chan++ ) 
	  {
	    hit_ch[i_arm][i_port][i_apv][i_chan] = -1;
	    hit_tb[i_arm][i_port][i_apv][i_chan] = -1;
	    hit_adc[i_arm][i_port][i_apv][i_chan] = -1;
	  }
	}
      }
    }

    //	Process the data in event dd. 
    //  Effectively this loops over all APVs, in all ARM's, in all RDO's.
    while ( dd && dd->iterate() )
    {

        int rdo  = dd->rdo;
        int arm  = dd->sec;
        int port = -999;
        int apv  = dd->pad;

        // there is no information about which ARM port this data is coming from
        // have to deduce from APV id
        if ( (apv >= 0) && (apv <= 11) ) {
           port = 0;
        }
        else if ( (apv >= 12) && (apv <= 23) ) {
           port = 1;
           apv  = apv - 12; // just so we get a useful apv number in this port
        }

        //Display RDO, ARM and APV, still looking how to find the real RAW
        // data that includes Header/Trailer
        //cout << "=======>RDO ARM PORT APV " << rdo << " " << arm << " " << port << " " << apv << endl;


        // Bail out if we get an unexpected RDO and/or ARM and/or APV
        //if (  !( ( rdo == 1 ) && ( (arm == 0) || (arm == 1) ) && ( (port == 0) || (port == 1) ) &&  ( apv >= 0 ) && ( apv < numAPVs ) )   ) {
        if (  !( ( rdo == 1 ) && ( (arm == 0) || (arm == 1) ) && ( (port == 0) || (port == 1) ) &&  ( apv >= 0 ) && ( apv < numAPVs ) )   ) {
           cout << "======> Ugh, ugh, unexpected RDO or ARM or APV: " << rdo << " " << arm << " " << apv <<endl;
           return;
        }

	evt_rdo[arm][port][apv] = rdo;
	evt_arm[arm][port][apv] = arm;
	evt_port[arm][port][apv] = port;
	evt_apv[arm][port][apv] = apv;

        // APV number
        int thisAPV = apv;

        // Record that we have found this APV
        APVpresent[arm][port][thisAPV] = true;

        // This is what we get back from the data
        // struct fgt_adc_t {
	// unsigned short ch ;	// 0..127 for adc ch, 0..1137 for phys strip
	// unsigned char tb ;	// 0..9 (?)
	// unsigned short adc ;
        // } ;

        // Pull out 1 APV at a time
	fgt_adc_t *f = (fgt_adc_t *) dd->Void ;

	// Loop over the data in this APV to get (channel,timebin,adc)
	for ( u_int ii = 0; ii < dd->ncontent; ii++ ) {
            // CHannel, TimeBin and ADC
            int thisCH  = f[ii].ch;
            int thisTB  = f[ii].tb;
            int thisADC = f[ii].adc;

	    hit_ch[arm][port][apv][ii] = thisCH;
	    hit_tb[arm][port][apv][ii] = thisTB;
	    hit_adc[arm][port][apv][ii] = thisADC;

            // Store all information for ALL APV's and ALL channels in this event so we
            // can retrieve pulse shapes, cross talk, and so on
            // NOTE: we want to have all information available, not just for the APV's we are looking at!
            SignalPedCorrected[arm][port][thisAPV][thisCH][thisTB] = (POLARITY * thisADC) - pedestals[arm][port][thisAPV][thisCH];
            SignalUnCorrected[arm][port][thisAPV][thisCH][thisTB]  = (POLARITY * thisADC);

	    //  Here we are just plotting adc versus the channel number for this event for all timebins.
            // We're only interested to display this for the APV's we are looking at so.....
            // if      ( ( arm == 0 ) && ( port == 0 ) && ( thisAPV >= 0 ) && ( thisAPV <= 5 ) ) {
	    // modified by Xu Sun @ 02/10/2020
            if      ( ( arm == 0 ) && ( port == 0 ) && ( thisAPV >= 0 ) && ( thisAPV <= 11 ) ) {
               g1_eventADCvsChannel[thisTB]->SetPoint( eventADCvsChannelNum1[thisTB], thisAPV*numChannels+thisCH, SignalPedCorrected[arm][port][thisAPV][thisCH][thisTB]);
               ++eventADCvsChannelNum1[thisTB];
            }
            else if ( ( arm == 0 ) && ( port == 1 ) && ( thisAPV >= 0 ) && ( thisAPV <= 11) ) {
               g2_eventADCvsChannel[thisTB]->SetPoint( eventADCvsChannelNum2[thisTB], thisAPV*numChannels+thisCH, SignalPedCorrected[arm][port][thisAPV][thisCH][thisTB]);
               ++eventADCvsChannelNum2[thisTB];
            }
            // else if ( ( arm == 1 ) && ( port == 0 ) && ( thisAPV >= 0 ) && ( thisAPV <= 5 ) ) {
	    // modified by Xu Sun @ 02/10/2020
            else if ( ( arm == 1 ) && ( port == 0 ) && ( thisAPV >= 0 ) && ( thisAPV <= 11 ) ) {
               g3_eventADCvsChannel[thisTB]->SetPoint( eventADCvsChannelNum3[thisTB], thisAPV*numChannels+thisCH, SignalPedCorrected[arm][port][thisAPV][thisCH][thisTB]);
               ++eventADCvsChannelNum3[thisTB];
            }
            else if ( ( arm == 1 ) && ( port == 1 ) && ( thisAPV >= 0 ) && ( thisAPV <= 11 ) ) {
               g0_eventADCvsChannel[thisTB]->SetPoint( eventADCvsChannelNum0[thisTB], thisAPV*numChannels+thisCH, SignalPedCorrected[arm][port][thisAPV][thisCH][thisTB]);
               ++eventADCvsChannelNum0[thisTB];
            }
	}
    } // Finished looping over the APV's
	tree_Hits->Fill();


    // If not ALL APV's are present something went really wrong and we bail from this event
    for ( int i_arm = 0; i_arm < numARMs; i_arm++ ) {
        for ( int i_port = 0; i_port < numPorts; i_port++ ) {
            for ( int i_apv = SEL_LO_APV1; i_apv < SEL_HI_APV1; i_apv++ ) {
                // Bit dirty since we're not using all the Arms and Ports, also APV 23 in an ARM (mapped in this code to port 1, apv 11) is not read out, ugh!
                if ( (( i_arm == 0 ) && ( i_port == 0 )) || (( i_arm == 0 ) && ( i_port == 1 )) || (( i_arm == 1 ) && ( i_port == 0 )) ) {
                   if ( !APVpresent[i_arm][i_port][i_apv] ) {
                      cout << "=========> BAIL BAIL BAIL: APV missing from event!!! ARM,PORT,APV " << i_arm << " " << i_port << " " << i_apv << endl;
                      //return;
                   }
                }
            }
        }
    }

    // Storage for  the hits in the 2 layers
    int Row1, Col1;
    int Row2, Col2;  

    int maxADC = 0;
    int maxTB;

    // The whole (APV,channel,timebin) matrix has been filled
    // now we can find the pulseshapes
    for ( int i_arm  = 0; i_arm  < numARMs;  i_arm++ )  {
    for ( int i_port = 0; i_port < numPorts; i_port++ ) {
    for ( int i_apv  = 0; i_apv  < numAPVs;  i_apv++ )  {
        // Some simple clustering of neighbouring pads
        // Mark the channel in which the previous hit is sitting
        int    previousHitChannel = -999;
        double previousHitValue   = 0;
        for ( int i_chan = 0; i_chan < numChannels; i_chan++ ) {
            bool ImHit = kFALSE;
            // Note that we loop over the (minimum timebin)+1 to (maximum timebin)-1 only to avoid the edges
            for ( int i_tbin = 1; i_tbin < numTBins-1; i_tbin++ ) {
                // Only if 3 consequetive timebins exceed the threshold cut we call this a hit
                // OK, plus a bit of a hack. For some reason the first (=0th) event in a run seems to be
                // screwed up sometimes. If we concatenate the runs then we don't know which are the 0th
                // events anymore. I did notice though that timebin 0 always seems to be totally out of whack
                // Checking if timebin 0 is larger than timebin 3 should help a bit???
                if ( ( SignalPedCorrected[i_arm][i_port][i_apv][i_chan][i_tbin-1] > ( HITCUT * pedestalsStdDev[i_arm][i_port][i_apv][i_chan] ) ) &&
                     ( SignalPedCorrected[i_arm][i_port][i_apv][i_chan][i_tbin  ] > ( HITCUT * pedestalsStdDev[i_arm][i_port][i_apv][i_chan] ) ) &&
                     ( SignalPedCorrected[i_arm][i_port][i_apv][i_chan][i_tbin+1] > ( HITCUT * pedestalsStdDev[i_arm][i_port][i_apv][i_chan] ) ) &&
                     ( SignalPedCorrected[i_arm][i_port][i_apv][i_chan][0]        < ( SignalPedCorrected[i_arm][i_port][i_apv][i_chan][3])     ) &&
                     ( (( i_apv >= SEL_LO_APV1 ) && ( i_apv <= SEL_HI_APV1)) ) &&
                     ( pedestalsStdDev[i_arm][i_port][i_apv][i_chan] > MinimumNoise                                             )  ) {     // not sure if I want to leave this in!!!
                   // Found something that looks like a hit in this channel, 
                   // so we don't have to go through the rest of the timebins, cut timebin itteration short
                   i_tbin = 999;
                   // dump all timebins for this particular channel
                   // and fill the spectrum histogram
                   // and signal this as a hit channel
                   ImHit = kTRUE;
                   // {{{ Find maximum for this set of timebins, returns maxADC
                   double prevADC = -999.0;
                   maxADC  = 0.0;
                   double integratedADC = 0.0;
                   //cout << i_apv << " " << i_chan << "   " ;
                   for ( int ii_tbin = 0; ii_tbin < numTBins; ii_tbin++ ) {
                       //cout << SignalUnCorrected[i_apv][i_chan][ii_tbin] << "     "; 
                       // Store pulse for display
                       if      ( ( i_arm == 0 ) && ( i_port == 0) ) {
                               g1_eventADCvsTimebin->SetPoint(ii_tbin, ii_tbin, SignalPedCorrected[i_arm][i_port][i_apv][i_chan][ii_tbin]);
                       }
                       else if ( ( i_arm == 0 ) && ( i_port == 1) ) {
                               g2_eventADCvsTimebin->SetPoint(ii_tbin, ii_tbin, SignalPedCorrected[i_arm][i_port][i_apv][i_chan][ii_tbin]);
                       }
                       else if ( ( i_arm == 1 ) && ( i_port == 0) ) {
                               g3_eventADCvsTimebin->SetPoint(ii_tbin, ii_tbin, SignalPedCorrected[i_arm][i_port][i_apv][i_chan][ii_tbin]);
                       }
                       else if ( ( i_arm == 1 ) && ( i_port == 1) ) {
                               g0_eventADCvsTimebin->SetPoint(ii_tbin, ii_tbin, SignalPedCorrected[i_arm][i_port][i_apv][i_chan][ii_tbin]);
                       }
                       // Find maximum
                       if ( SignalPedCorrected[i_arm][i_port][i_apv][i_chan][ii_tbin] > prevADC ) {
                          maxADC  = SignalPedCorrected[i_arm][i_port][i_apv][i_chan][ii_tbin];
                          maxTB   = ii_tbin;
                          prevADC = maxADC;
                       }
                   }
                   //cout << endl;
                   // }}}
                   // If the previous channel was also hit we assume that this hit should be clustered with the previous hit
                   if ( (previousHitChannel == (i_chan - 1)) && CLUSTER ) {
                         if (      ( i_arm == 0 ) && ( i_port == 0 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                            h1_ADCspectrum->Fill(previousHitValue,-1.0);
                            h1_ADCspectrum->Fill((maxADC+previousHitValue),1.0);
                            h1_signal2noise->Fill(previousHitValue/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],-1.0);
                            h1_signal2noise->Fill((maxADC+previousHitValue)/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                            h1_maxTB->Fill(maxTB,1);
                         }
                         else if ( ( i_arm == 0 ) && ( i_port == 1 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                            h2_ADCspectrum->Fill(previousHitValue,-1.0);
                            h2_ADCspectrum->Fill((maxADC+previousHitValue),1.0);
                            h2_signal2noise->Fill(previousHitValue/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],-1.0);
                            h2_signal2noise->Fill((maxADC+previousHitValue)/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                            h2_maxTB->Fill(maxTB,1);
                         }
                         else if ( ( i_arm == 1 ) && ( i_port == 0 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                            h3_ADCspectrum->Fill(previousHitValue,-1.0);
                            h3_ADCspectrum->Fill((maxADC+previousHitValue),1.0);
                            h3_signal2noise->Fill(previousHitValue/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],-1.0);
                            h3_signal2noise->Fill((maxADC+previousHitValue)/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                            h3_maxTB->Fill(maxTB,1);
                         }
                         else if ( ( i_arm == 1 ) && ( i_port == 1 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                            h0_ADCspectrum->Fill(previousHitValue,-1.0);
                            h0_ADCspectrum->Fill((maxADC+previousHitValue),1.0);
                            h0_signal2noise->Fill(previousHitValue/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],-1.0);
                            h0_signal2noise->Fill((maxADC+previousHitValue)/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                            h0_maxTB->Fill(maxTB,1);
                         }
                   }
                   else {
                         if (      ( i_arm == 0 ) && ( i_port == 0 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                            h1_ADCspectrum->Fill(maxADC,1.0);
                            h1_signal2noise->Fill(maxADC/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                            h1_maxTB->Fill(maxTB,1);
                         }
                         else if ( ( i_arm == 0 ) && ( i_port == 1 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                            h2_ADCspectrum->Fill(maxADC,1.0);
                            h2_signal2noise->Fill(maxADC/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                            h2_maxTB->Fill(maxTB,1);
                         }
                         else if ( ( i_arm == 1 ) && ( i_port == 0 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                            h3_ADCspectrum->Fill(maxADC,1.0);
                            h3_signal2noise->Fill(maxADC/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                            h3_maxTB->Fill(maxTB,1);
                         }
                         else if ( ( i_arm == 1 ) && ( i_port == 1 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                            h0_ADCspectrum->Fill(maxADC,1.0);
                            h0_signal2noise->Fill(maxADC/pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                            h0_maxTB->Fill(maxTB,1);
                         }
                   }
                   previousHitChannel = i_chan;
                   previousHitValue = maxADC;
                   // Don't know how to handle 'clustering' of pedestals
                   // For the time being just dump the current pedestal sigma in a histogram
                   if (      ( i_arm == 0 ) && ( i_port == 0 ) && ( i_apv >=0 ) && ( i_apv <= 5 ) ) {
                      h1_PEDspectrum->Fill(pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                   }
                   else if ( ( i_arm == 0 ) && ( i_port == 1 ) && ( i_apv >=0 ) && ( i_apv <= 5 ) ) {
                      h2_PEDspectrum->Fill(pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                   }
                   else if ( ( i_arm == 1 ) && ( i_port == 0 ) && ( i_apv >=0 ) && ( i_apv <= 5 ) ) {
                      h3_PEDspectrum->Fill(pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                   }
                   else if ( ( i_arm == 1 ) && ( i_port == 1 ) && ( i_apv >=0 ) && ( i_apv <= 5 ) ) {
                      h0_PEDspectrum->Fill(pedestalsStdDev[i_arm][i_port][i_apv][i_chan],1.0);
                   }
                }
            } // EndLoop timebins

            // Don't accept events with too many hits
            if ( noHits > MAX_NO_HITS_PER_EVENT-1 ) {
               cout << "==============> TOO MANY HITS ---> Bail " << endl;
               return;
            }

            // Fill the 'sensor hit' histogram
            // NOTE THAT THERE IS NO CLUSTERING HERE, I.E. THE HITS RETURNED ARE NOT CLUSTERED!!!
            // Layer 1 is the top stave, Layer 2 is the middle stave, Layer 3 is the bottom stave
            if ( ImHit ) {
               noHits += 1;
               Found_No_Hits += 1;
               // Layer 1, sensor 0
               if ( ( i_arm == 0 ) && ( i_port == 0 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                  if ( ( i_chan >= 0 ) && ( i_chan <= 63 ) ) {
                     Col1 = (i_apv * 2);
                     Row1 = i_chan;
                  }
                  else {
                     Col1 = (i_apv * 2) + 1;
                     Row1 = (127 - i_chan);
                  }
                  h1_hits_on_sensor->Fill(Col1,Row1,1.0);
                  h1_hits_on_Col->Fill(Col1,1.0);
                  h1_hits_on_Row->Fill(Row1,1.0);
                  // Hit on layer 1
                  Hit[noHits-1].layer = 1;
                  Hit[noHits-1].sensor = 0;
                  Hit[noHits-1].row    = Row1;
                  Hit[noHits-1].column = Col1;
                  Hit[noHits-1].ADC    = SignalPedCorrected[i_arm][i_port][i_apv][i_chan][maxTB];
               }
               // Layer 1, sensor 1 => Added by Xu Sun @ 02/10/2020
               else if ( ( i_arm == 0 ) && ( i_port == 0 ) && ( i_apv >= 6 ) && ( i_apv <= 11 ) ) {
                  if ( ( i_chan >= 0 ) && ( i_chan <= 63 ) ) {
                     Col2 = ((i_apv) * 2);
                     Row2 = i_chan;
                  }
                  else {
                     Col2 = ((i_apv) * 2) + 1;
                     Row2 = (127 - i_chan);
                  }
                  h1_hits_on_sensor->Fill(Col2,Row2,1.0);
                  h1_hits_on_Col->Fill(Col2,1.0);
                  h1_hits_on_Row->Fill(Row2,1.0);
                  // Hit on layer 2
                  Hit[noHits-1].layer = 1;
                  Hit[noHits-1].sensor = 1;
                  Hit[noHits-1].row    = Row2; 
                  Hit[noHits-1].column = Col2;
                  Hit[noHits-1].ADC    = SignalPedCorrected[i_arm][i_port][i_apv][i_chan][maxTB];
               }
               // Layer 1, sensor 1 => Added by Xu Sun @ 02/10/2020
               // Layer 2, sensor 0
               if ( ( i_arm == 0 ) && ( i_port == 1 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                  if ( ( i_chan >= 0 ) && ( i_chan <= 63 ) ) {
                     Col1 = (i_apv * 2);
                     Row1 = i_chan;
                  }
                  else {
                     Col1 = (i_apv * 2) + 1;
                     Row1 = (127 - i_chan);
                  }
                  h2_hits_on_sensor->Fill(Col1,Row1,1.0);
                  h2_hits_on_Col->Fill(Col1,1.0);
                  h2_hits_on_Row->Fill(Row1,1.0);
                  // Hit on layer 1
                  Hit[noHits-1].layer = 2;
                  Hit[noHits-1].sensor = 0;
                  Hit[noHits-1].row    = Row1;
                  Hit[noHits-1].column = Col1;
                  Hit[noHits-1].ADC    = SignalPedCorrected[i_arm][i_port][i_apv][i_chan][maxTB];
               }
               // Layer 2, sensor 1
               else if ( ( i_arm == 0 ) && ( i_port == 1 ) && ( i_apv >= 6 ) && ( i_apv <= 11 ) ) {
                  if ( ( i_chan >= 0 ) && ( i_chan <= 63 ) ) {
                     Col2 = ((i_apv) * 2);
                     Row2 = i_chan;
                  }
                  else {
                     Col2 = ((i_apv) * 2) + 1;
                     Row2 = (127 - i_chan);
                  }
                  h2_hits_on_sensor->Fill(Col2,Row2,1.0);
                  h2_hits_on_Col->Fill(Col2,1.0);
                  h2_hits_on_Row->Fill(Row2,1.0);
                  // Hit on layer 2
                  Hit[noHits-1].layer = 2;
                  Hit[noHits-1].sensor = 1;
                  Hit[noHits-1].row    = Row2; 
                  Hit[noHits-1].column = Col2;
                  Hit[noHits-1].ADC    = SignalPedCorrected[i_arm][i_port][i_apv][i_chan][maxTB];
               }
               // Layer 3, sensor 0
               if ( ( i_arm == 1 ) && ( i_port == 0 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                  if ( ( i_chan >= 0 ) && ( i_chan <= 63 ) ) {
                     Col1 = (i_apv * 2);
                     Row1 = i_chan;
                  }
                  else {
                     Col1 = (i_apv * 2) + 1;
                     Row1 = (127 - i_chan);
                  }
                  h3_hits_on_sensor->Fill(Col1,Row1,1.0);
                  h3_hits_on_Col->Fill(Col1,1.0);
                  h3_hits_on_Row->Fill(Row1,1.0);
                  // Hit on layer 3
                  Hit[noHits-1].layer = 3;
                  Hit[noHits-1].sensor = 0;
                  Hit[noHits-1].row    = Row1;
                  Hit[noHits-1].column = Col1;
                  Hit[noHits-1].ADC    = SignalPedCorrected[i_arm][i_port][i_apv][i_chan][maxTB];
               }
               // Layer 3, sensor 1 => Added by Xu Sun @ 02/10/2020
               else if ( ( i_arm == 1 ) && ( i_port == 0 ) && ( i_apv >= 6 ) && ( i_apv <= 11 ) ) {
                  if ( ( i_chan >= 0 ) && ( i_chan <= 63 ) ) {
                     Col2 = ((i_apv) * 2);
                     Row2 = i_chan;
                  }
                  else {
                     Col2 = ((i_apv) * 2) + 1;
                     Row2 = (127 - i_chan);
                  }
                  h3_hits_on_sensor->Fill(Col2,Row2,1.0);
                  h3_hits_on_Col->Fill(Col2,1.0);
                  h3_hits_on_Row->Fill(Row2,1.0);
                  // Hit on layer 2
                  Hit[noHits-1].layer = 3;
                  Hit[noHits-1].sensor = 1;
                  Hit[noHits-1].row    = Row2; 
                  Hit[noHits-1].column = Col2;
                  Hit[noHits-1].ADC    = SignalPedCorrected[i_arm][i_port][i_apv][i_chan][maxTB];
               }
               // Layer 3, sensor 1 => Added by Xu Sun @ 02/10/2020
               // Layer 0, sensor 0
               if ( ( i_arm == 1 ) && ( i_port == 1 ) && ( i_apv >= 0 ) && ( i_apv <= 5 ) ) {
                  if ( ( i_chan >= 0 ) && ( i_chan <= 63 ) ) {
                     Col1 = (i_apv * 2);
                     Row1 = i_chan;
                  }
                  else {
                     Col1 = (i_apv * 2) + 1;
                     Row1 = (127 - i_chan);
                  }
                  h0_hits_on_sensor->Fill(Col1,Row1,1.0);
                  h0_hits_on_Col->Fill(Col1,1.0);
                  h0_hits_on_Row->Fill(Row1,1.0);
                  // Hit on layer 3
                  Hit[noHits-1].layer = 0;
                  Hit[noHits-1].sensor = 0;
                  Hit[noHits-1].row    = Row1;
                  Hit[noHits-1].column = Col1;
                  Hit[noHits-1].ADC    = SignalPedCorrected[i_arm][i_port][i_apv][i_chan][maxTB];
               }
               else if ( ( i_arm == 1 ) && ( i_port == 1 ) && ( i_apv >= 6 ) && ( i_apv <= 11 ) ) {
                  if ( ( i_chan >= 0 ) && ( i_chan <= 63 ) ) {
                     Col2 = ((i_apv) * 2);
                     Row2 = i_chan;
                  }
                  else {
                     Col2 = ((i_apv) * 2) + 1;
                     Row2 = (127 - i_chan);
                  }
                  h0_hits_on_sensor->Fill(Col2,Row2,1.0);
                  h0_hits_on_Col->Fill(Col2,1.0);
                  h0_hits_on_Row->Fill(Row2,1.0);
                  // Hit on layer 2
                  Hit[noHits-1].layer = 0;
                  Hit[noHits-1].sensor = 1;
                  Hit[noHits-1].row    = Row2; 
                  Hit[noHits-1].column = Col2;
                  Hit[noHits-1].ADC    = SignalPedCorrected[i_arm][i_port][i_apv][i_chan][maxTB];
               }

            }

        } // EndLoop channnels
    } // EndLoop apvs
    } // EndLoop Ports
    } // EndLoop ARMs


    if ( display_event ) {
       // {{{ Top Layer: Display ADC vs Channel and PulseShape
       p1_EventDisplay->cd();
       g1_eventADCvsChannel[UseTimeBin]->SetTitle( "ADC versus Channel;Channel;ADC" );
       g1_eventADCvsChannel[UseTimeBin]->SetMarkerStyle(21);
       g1_eventADCvsChannel[UseTimeBin]->SetMarkerSize(0.4);
       g1_eventADCvsChannel[UseTimeBin]->SetMarkerColor(kBlue);
       g1_eventADCvsChannel[UseTimeBin]->SetMinimum(-200);
       g1_eventADCvsChannel[UseTimeBin]->SetMaximum(+2000);
       g1_eventADCvsChannel[UseTimeBin]->GetXaxis()->SetRangeUser((SEL_LO_APV1)*numChannels   - (SEL_HI_APV1+1-SEL_LO_APV1)*numChannels/10.0,
                                                                  (SEL_HI_APV1+1)*numChannels + (SEL_HI_APV1+1-SEL_LO_APV1)*numChannels/10.0 );
       g1_eventADCvsChannel[UseTimeBin]->GetXaxis()->SetLabelSize(0.07);
       g1_eventADCvsChannel[UseTimeBin]->GetYaxis()->SetLabelSize(0.07);
       g1_eventADCvsChannel[UseTimeBin]->Draw("AP");
       p1_EventDisplay->Update();
       p1_PulseDisplay->cd();
       g1_eventADCvsTimebin->SetTitle( "ADC versus Timebin;Timebin;ADC" );
       g1_eventADCvsTimebin->SetMarkerStyle(21);
       g1_eventADCvsTimebin->SetMarkerSize(0.4);
       g1_eventADCvsTimebin->SetMarkerColor(kRed);
       g1_eventADCvsTimebin->SetLineWidth(0.2);
       g1_eventADCvsTimebin->SetLineColor(kBlue);
       g1_eventADCvsTimebin->SetMinimum(-200);
       g1_eventADCvsTimebin->SetMaximum(+2000);
       g1_eventADCvsTimebin->GetXaxis()->SetRangeUser(-1, numTBins);
       g1_eventADCvsTimebin->GetXaxis()->SetLabelSize(0.07);
       g1_eventADCvsTimebin->GetYaxis()->SetLabelSize(0.07);
       g1_eventADCvsTimebin->Draw("ALP");
       p1_PulseDisplay->Update();
       // }}}
       // {{{ Middle Layer: Display ADC vs Channel and PulseShape
       p2_EventDisplay->cd();
       g2_eventADCvsChannel[UseTimeBin]->SetTitle( "ADC versus Channel;Channel;ADC" );
       g2_eventADCvsChannel[UseTimeBin]->SetMarkerStyle(21);
       g2_eventADCvsChannel[UseTimeBin]->SetMarkerSize(0.4);
       g2_eventADCvsChannel[UseTimeBin]->SetMarkerColor(kBlue);
       g2_eventADCvsChannel[UseTimeBin]->SetMinimum(-200);
       g2_eventADCvsChannel[UseTimeBin]->SetMaximum(+2000);
       g2_eventADCvsChannel[UseTimeBin]->GetXaxis()->SetRangeUser((SEL_LO_APV1)*numChannels   - (SEL_HI_APV1+1-SEL_LO_APV1)*numChannels/10.0,
                                                                  (SEL_HI_APV1+1)*numChannels + (SEL_HI_APV1+1-SEL_LO_APV1)*numChannels/10.0 );
       g2_eventADCvsChannel[UseTimeBin]->GetXaxis()->SetLabelSize(0.07);
       g2_eventADCvsChannel[UseTimeBin]->GetYaxis()->SetLabelSize(0.07);
       g2_eventADCvsChannel[UseTimeBin]->Draw("AP");
       p2_EventDisplay->Update();
       p2_PulseDisplay->cd();
       g2_eventADCvsTimebin->SetTitle( "ADC versus Timebin;Timebin;ADC" );
       g2_eventADCvsTimebin->SetMarkerStyle(21);
       g2_eventADCvsTimebin->SetMarkerSize(0.4);
       g2_eventADCvsTimebin->SetMarkerColor(kRed);
       g2_eventADCvsTimebin->SetLineWidth(0.2);
       g2_eventADCvsTimebin->SetLineColor(kBlue);
       g2_eventADCvsTimebin->SetMinimum(-200);
       g2_eventADCvsTimebin->SetMaximum(+2000);
       g2_eventADCvsTimebin->GetXaxis()->SetRangeUser(-1, numTBins);
       g2_eventADCvsTimebin->GetXaxis()->SetLabelSize(0.07);
       g2_eventADCvsTimebin->GetYaxis()->SetLabelSize(0.07);
       g2_eventADCvsTimebin->Draw("ALP");
       p2_PulseDisplay->Update();
       // }}}
       // {{{ Bottom Layer: Display ADC vs Channel and PulseShape
       p3_EventDisplay->cd();
       g3_eventADCvsChannel[UseTimeBin]->SetTitle( "ADC versus Channel;Channel;ADC" );
       g3_eventADCvsChannel[UseTimeBin]->SetMarkerStyle(21);
       g3_eventADCvsChannel[UseTimeBin]->SetMarkerSize(0.4);
       g3_eventADCvsChannel[UseTimeBin]->SetMarkerColor(kBlue);
       g3_eventADCvsChannel[UseTimeBin]->SetMinimum(-200);
       g3_eventADCvsChannel[UseTimeBin]->SetMaximum(+2000);
       g3_eventADCvsChannel[UseTimeBin]->GetXaxis()->SetRangeUser((SEL_LO_APV1)*numChannels   - (SEL_HI_APV1+1-SEL_LO_APV1)*numChannels/10.0,
                                                                  (SEL_HI_APV1+1)*numChannels + (SEL_HI_APV1+1-SEL_LO_APV1)*numChannels/10.0 );
       g3_eventADCvsChannel[UseTimeBin]->GetXaxis()->SetLabelSize(0.07);
       g3_eventADCvsChannel[UseTimeBin]->GetYaxis()->SetLabelSize(0.07);
       g3_eventADCvsChannel[UseTimeBin]->Draw("AP");
       p3_EventDisplay->Update();
       p3_PulseDisplay->cd();
       g3_eventADCvsTimebin->SetTitle( "ADC versus Timebin;Timebin;ADC" );
       g3_eventADCvsTimebin->SetMarkerStyle(21);
       g3_eventADCvsTimebin->SetMarkerSize(0.4);
       g3_eventADCvsTimebin->SetMarkerColor(kRed);
       g3_eventADCvsTimebin->SetLineWidth(0.2);
       g3_eventADCvsTimebin->SetLineColor(kBlue);
       g3_eventADCvsTimebin->SetMinimum(-200);
       g3_eventADCvsTimebin->SetMaximum(+2000);
       g3_eventADCvsTimebin->GetXaxis()->SetRangeUser(-1, numTBins);
       g3_eventADCvsTimebin->GetXaxis()->SetLabelSize(0.07);
       g3_eventADCvsTimebin->GetYaxis()->SetLabelSize(0.07);
       g3_eventADCvsTimebin->Draw("ALP");
       p3_PulseDisplay->Update();
       // }}}
       // {{{ Bottom Layer: Display ADC vs Channel and PulseShape
       p0_EventDisplay->cd();
       g0_eventADCvsChannel[UseTimeBin]->SetTitle( "ADC versus Channel;Channel;ADC" );
       g0_eventADCvsChannel[UseTimeBin]->SetMarkerStyle(21);
       g0_eventADCvsChannel[UseTimeBin]->SetMarkerSize(0.4);
       g0_eventADCvsChannel[UseTimeBin]->SetMarkerColor(kBlue);
       g0_eventADCvsChannel[UseTimeBin]->SetMinimum(-200);
       g0_eventADCvsChannel[UseTimeBin]->SetMaximum(+2000);
       g0_eventADCvsChannel[UseTimeBin]->GetXaxis()->SetRangeUser((SEL_LO_APV1)*numChannels   - (SEL_HI_APV1+1-SEL_LO_APV1)*numChannels/10.0,
                                                                  (SEL_HI_APV1+1)*numChannels + (SEL_HI_APV1+1-SEL_LO_APV1)*numChannels/10.0 );
       g0_eventADCvsChannel[UseTimeBin]->GetXaxis()->SetLabelSize(0.07);
       g0_eventADCvsChannel[UseTimeBin]->GetYaxis()->SetLabelSize(0.07);
       g0_eventADCvsChannel[UseTimeBin]->Draw("AP");
       p0_EventDisplay->Update();
       p0_PulseDisplay->cd();
       g0_eventADCvsTimebin->SetTitle( "ADC versus Timebin;Timebin;ADC" );
       g0_eventADCvsTimebin->SetMarkerStyle(21);
       g0_eventADCvsTimebin->SetMarkerSize(0.4);
       g0_eventADCvsTimebin->SetMarkerColor(kRed);
       g0_eventADCvsTimebin->SetLineWidth(0.2);
       g0_eventADCvsTimebin->SetLineColor(kBlue);
       g0_eventADCvsTimebin->SetMinimum(-200);
       g0_eventADCvsTimebin->SetMaximum(+2000);
       g0_eventADCvsTimebin->GetXaxis()->SetRangeUser(-1, numTBins);
       g0_eventADCvsTimebin->GetXaxis()->SetLabelSize(0.07);
       g0_eventADCvsTimebin->GetYaxis()->SetLabelSize(0.07);
       g0_eventADCvsTimebin->Draw("ALP");
       p0_PulseDisplay->Update();
       // }}}
    }

}}}

void nextEvent() 
{{{
    currentEvent++;

    rdr->get(0,EVP_TYPE_ANY);

    if( rdr->status == EVP_STAT_EOR )
    {
        currentEvent--;
    }
    else
    {
	processEvent( rdr->det("fgt")->get("adc"), true );
    }
}}}

void previousEvent() 
{{{
    currentEvent--;

    if ( currentEvent < 0 )
    {
	currentEvent++;
	return;
    }

    delete rdr;
    rdr = new daqReader(argv1);

    for ( int ii = 0; ii <= currentEvent; ++ii )
    {
	rdr->get(0,EVP_TYPE_ANY);
    }

    processEvent( rdr->det("fgt")->get("adc"), true );
}}}

void jumpToEvent() 
{{{

    long long event;
    //std::cin >> event;

    event = 1;

//    if ( event < currentEvent )
//    {
	delete rdr;
        rdr = new daqReader(argv1);
	currentEvent = -1;
//    }

    currentEvent = 0;

    int ii;
    for ( ii = currentEvent; ii < event; ++ii )
    {
	rdr->get(0,EVP_TYPE_ANY);

	if( rdr->status == EVP_STAT_EOR )
	{
	    currentEvent = -1;
	    return;
	}
    }

    currentEvent = event;
    processEvent( rdr->det("fgt")->get("adc"), true );
}}}

void DumpEvents() 
{{{
    cout << "===> Dump Events....." << endl;


    currentEvent = -1;

    int event_count = 0;
    rdr->get(0,EVP_TYPE_ANY);
    while ( ( rdr->status != EVP_STAT_EOR ) && ( event_count < numEvents ) ) {
           currentEvent = event_count;
           Found_No_Hits = 0;
           processEvent( rdr->det("fgt")->get("adc"), false );
           //cout << "===== Event " << currentEvent << "====Found " << Found_No_Hits << " hits==============" << endl;
           // No assumptions about hit ordering, so have to make all combinations
           // for clustering
           for ( int i = 0; i < (Found_No_Hits-1); i++) {
               //cout << "+++++ Hit[i=" << i << "]: layer sensor row column ADC = " << Hit[i].layer << " " << Hit[i].sensor << 
               //        " " << Hit[i].row << " " << Hit[i].column << " " << Hit[i].ADC << " " << endl;
               for ( int j = i+1; j < Found_No_Hits; j++) {
                   //cout << "      Hit[j=" << j << "]: layer sensor row column ADC = " << Hit[j].layer << " " << Hit[j].sensor << 
                   //        " " << Hit[j].row << " " << Hit[j].column << " " << Hit[j].ADC << " " << endl;
                   // Clustering
                   if ( CLUSTER ) {
                      if ( ( Hit[i].layer  == Hit[j].layer  ) &&
                           ( Hit[i].sensor == Hit[j].sensor ) 
                         ) {
                         if ( (  ( Hit[i].column == Hit[j].column ) && ( ( Hit[i].row    == Hit[j].row    - 1 ) || ( Hit[i].row    == Hit[j].row    + 1 ) ) ) ||
                              (  ( Hit[i].row    == Hit[j].row    ) && ( ( Hit[i].column == Hit[j].column - 1 ) || ( Hit[i].column == Hit[j].column + 1 ) ) )
                            ) {
                            if ( Hit[i].ADC > Hit[j].ADC ) {
                               //cout << "       +++ Hit[i=" << i << "]: layer sensor row column ADC = " << Hit[i].layer << " " << Hit[i].sensor << 
                               //        " " << Hit[i].row << " " << Hit[i].column << " " << Hit[i].ADC << " " << endl;
                               //cout << "       +++ absorbed" << endl;
                               //cout << "       +++ Hit[j=" << j << "]: layer sensor row column ADC = " << Hit[j].layer << " " << Hit[j].sensor << 
                               //        " " << Hit[j].row << " " << Hit[j].column << " " << Hit[j].ADC << " " << endl;
                               Hit[i].ADC += Hit[j].ADC;
                               Hit[j].ADC  = -1 * Hit[j].ADC;
                            }
                            else {
                               //cout << "       +++ Hit[j=" << j << "]: layer sensor row column ADC = " << Hit[j].layer << " " << Hit[j].sensor << 
                               //        " " << Hit[j].row << " " << Hit[j].column << " " << Hit[j].ADC << " " << endl;
                               //cout << "       +++ absorbed" << endl;
                               //cout << "       +++ Hit[i=" << i << "]: layer sensor row column ADC = " << Hit[i].layer << " " << Hit[i].sensor << 
                               //        " " << Hit[i].row << " " << Hit[i].column << " " << Hit[i].ADC << " " << endl;
                               Hit[j].ADC += Hit[i].ADC;
                               Hit[i].ADC  = -1 * Hit[i].ADC;
                            }
                         }
                      }
                   }
               } //end of j loop
           } //end of i loop
           //cout << "---------------------------------------------------------------------" << endl;

           // No assumptions about hit ordering, so have to make all combinations
           // for tracking
           for ( int i = 0; i < (Found_No_Hits-1); i++) {
               for ( int j = i+1; j < Found_No_Hits; j++) {
                   TVector3 Normal, Track;
                   // Coordinates of hits with respect to row=0, column=0 in lower layer
                   Float_t  x1,y1,z1;
                   Float_t  x2,y2,z2;
                   Float_t  x3,y3,z3;
                   Float_t  xstore,ystore,zstore;
                   Float_t  Trot2, Trot3;
                   Float_t  x2proj,y2proj,z2proj;
                   Float_t  Residual;
                   Float_t  xResidual;
                   Float_t  yResidual;
                   // Find a hit on layer 1 and a hit on layer 3, we only have a sensor 0 in both layers
                   if ( (   ( Hit[i].ADC    >  -1 ) && ( Hit[j].ADC    >  -1 ) ) &&
                        // (   ( Hit[i].sensor ==  0 ) && ( Hit[j].sensor ==  0 ) ) && // commented out by Xu Sun @ 02/13/2020 => use both sensor from now
                        (   ( Hit[i].row    >=  0 ) && ( Hit[j].row    >=  0 ) ) &&
                        (   ( Hit[i].row    <=  63) && ( Hit[j].row    <=  63) ) &&
                        ( ( ( Hit[i].layer  ==  1 ) && ( Hit[j].layer  ==  3 ) ) || ( ( Hit[i].layer  ==  3 ) && ( Hit[j].layer  ==  1 ) )  )
                      ) {
                      // hits have discrete row and column coordinates and, although understandable,
                      // give non-smooth (not PR ready!) histograms.
                      // randomizing within the pads 'solves' this problem. {{{
                      x1 = Hit[i].column * pitchColumn + (pitchColumn*rand()/double(RAND_MAX));
                      y1 = Hit[i].row    * pitchRow    + (pitchRow   *rand()/double(RAND_MAX));
                      // x1 = Hit[i].column * pitchColumn;
                      // y1 = Hit[i].row    * pitchRow;
                      z1 = 0.0 ;
                      x3 = Hit[j].column   * pitchColumn + (pitchColumn*rand()/double(RAND_MAX));
                      y3 = Hit[j].row      * pitchRow    + (pitchRow   *rand()/double(RAND_MAX));
                      // x3 = Hit[j].column   * pitchColumn;
                      // y3 = Hit[j].row      * pitchRow;
                      // z3 = 2.0 * pitchLayer ; // Layer 1 and 3 are separated by 2 pitches
                      z3 = pitchLayer23 + pitchLayer12;  // Added by Xu Sun @ 02/13/2020
                      //}}}
                      Normal.SetXYZ(0, 0, z3-z1)         ;
                      Track.SetXYZ(x3-x1, y3-y1, z3-z1)  ;
                      //cout << "----->>> Hit1 = " << "( " << x1 << " , " << y1 << " , " << z1 << " )" << endl;
                      //cout << "----->>> Hit3 = " << "( " << x3 << " , " << y3 << " , " << z3 << " )" << endl;
                      //cout << "----->>> Angle = " << (rad2deg * Track.Angle(Normal)) << endl;
                      // Do all the hit and track calculations and fill some histograms {{{
                      h9_TrackAngle->Fill(rad2deg * Track.Angle(Normal), 1.0);
                      // Cut on maximum track angle to weed out invalid 2-hit combinations a bit
                      if ( (rad2deg * Track.Angle(Normal)) < maxAngle ) {
                         TrackCount += 1;
                         // not angle corrected hits on a track
                         h1_track_ADCspectrum->Fill(Hit[i].ADC, 1.0);
                         h2_track_ADCspectrum->Fill(Hit[j].ADC, 1.0);
                         // angle corrected hits on a track
                         h1_track_angcorr_ADCspectrum->Fill(( (Hit[i].ADC ) * cos(Track.Angle(Normal)) ), 1.0);
                         h2_track_angcorr_ADCspectrum->Fill(( (Hit[j].ADC ) * cos(Track.Angle(Normal)) ), 1.0);
                         // whole bunch of fun stuff
                         h9_ADC1_TrackAngle->Fill( rad2deg * Track.Angle(Normal) ,  Hit[i].ADC , 1.0);
                         h9_ADC1ADC2->Fill(Hit[i].ADC*cos(Track.Angle(Normal)) , Hit[j].ADC*cos(Track.Angle(Normal)) , 1.0);
                         // Do the residuals of the found track with the hit from Layer 2
                         for ( int k = 0; k < Found_No_Hits; k++) {
                             if ( ( Hit[k].ADC    >  -1 ) &&
                                  // ( Hit[k].sensor ==  0 ) && // commented out by Xu Sun @ 02/13/2020 => use both sensor from now
                                  ( Hit[k].layer  ==  2 ) &&
                                  ( Hit[k].row    >=  0 ) &&
                                  ( Hit[k].row    <=  63)
                                ) {
                                // Calculate randomized position of hit
                                x2 = Hit[k].column * pitchColumn + (pitchColumn*rand()/double(RAND_MAX));
                                y2 = Hit[k].row    * pitchRow    + (pitchRow   *rand()/double(RAND_MAX));
                                // x2 = Hit[k].column * pitchColumn;
                                // y2 = Hit[k].row    * pitchRow;
                                // z2 = pitchLayer ;
                                z2 = pitchLayer12 ; // Added by Xu Sun @ 02/13/2020
                                // Introduce rotations Trot2 and Trot3, all wrt the first layer
                                Trot2 = ROTATE2 / rad2deg; // degree converted to rad
                                Trot3 = ROTATE3 / rad2deg;
                                xstore = x3; ystore = y3;
                                x3 = xstore * cos(Trot3)  -  ystore * sin(Trot3);
                                y3 = xstore * sin(Trot3)  +  ystore * cos(Trot3);
                                xstore = x2; ystore = y2;
                                x2 = xstore * cos(Trot2)  -  ystore * sin(Trot2);
                                y2 = xstore * sin(Trot2)  +  ystore * cos(Trot2);
                                // Move third hit around a bit
                                y3 = y3 + SHIFT3;
                                // Move second hit around a bit
                                y2 = y2 + SHIFT2;
                                // Projected hit from track on layer 2
                                // x2proj = x1 + 0.5*(x3-x1);
                                // y2proj = y1 + 0.5*(y3-y1);
                                // z2proj = pitchLayer ;
                                x2proj = x1 + (x3-x1)*pitchLayer12/(pitchLayer12+pitchLayer23); // Added by Xu Sun @ 02/13/2020
                                y2proj = y1 + (y3-y1)*pitchLayer12/(pitchLayer12+pitchLayer23);
                                z2proj = pitchLayer12 ;
                                // Calculate residual
                                //Residual = sqrt( (x2-x2proj)*(x2-x2proj) + (y2-y2proj)*(y2-y2proj) );
                                xResidual = (x2-x2proj);
                                yResidual = (y2-y2proj);
                                //cout << "------------>Residual = " << Residual << " mm" << endl;
                                // h9_Residual->Fill(xResidual,yResidual, 1.0);
                                h9_XResidual->Fill(xResidual, 1.0);
                                h9_YResidual->Fill(yResidual, 1.0);
                             }
                         }
                      }
                      //}}}
                      // select tracks that go through the same column
                      // and which deposit a LOT of charge
                      /* if ( ( Hit[i].column == Hit[j].column ) &&
                           ( Hit[i].ADC    >  2000          )
                         ) {
                         HighCount += 1;
                         cout << "===================" << endl;
                         cout << "== " << HighCount << " Tracks with High ADC value out of a total of " << TrackCount << " tracks" << endl;
                         cout << "======>Hit[i=" << i << "] : " << Hit[i].layer << " " << Hit[i].sensor << " " << Hit[i].column << " " << Hit[i].row << " " << Hit[i].ADC << endl;
                         int StripeHit = 0;
                         // find hits in the same column {{{
                         for ( int k = 0; k < Found_No_Hits; k++ ) {
                             if ( ( Hit[k].layer  == Hit[i].layer  ) &&
                                  ( Hit[k].sensor == Hit[i].sensor ) &&
                                  ( Hit[k].column == Hit[i].column ) &&
                                  (     k         !=     i         ) 
                                ) {
                                StripeHit += 1;
                                cout << "======>Hit[k=" << k << "] : " << Hit[k].layer << " " << Hit[k].sensor << " " << Hit[k].column << " " << Hit[k].row << " " << Hit[k].ADC << endl;
                                //h9_ADC1_ADCnext->Fill( max( (max( abs(Hit[i].ADC) , abs(Hit[k].ADC) )) , abs(Hit[l].ADC) ), 
                                //                       min( (min( abs(Hit[i].ADC) , abs(Hit[k].ADC) )) , abs(Hit[l].ADC) ),
                                //                       1.0 );
                             }
                         }
                      //}}}
                         if ( StripeHit > 1 ) {
                            StripeCount += 1;
                         }
                         cout << "===================" << endl;
                         cout << "== StripeCount = " << StripeCount << endl;
                         cout << "===================" << endl;
                      } */
                   }
               }
           }
           //  Next event
           event_count++;
           rdr->get(0,EVP_TYPE_ANY);
    }

    //{{{ Draw the info for the top layer
           // DRAW ==========> Draw the sensor hit map
           gStyle->SetOptStat(111111);
           p1_hitmap->cd();
           h1_hits_on_sensor->SetTitle("Accumulated Hits on Sensor");
           h1_hits_on_sensor->SetTitleSize(0.5);
           h1_hits_on_sensor->GetXaxis()->SetTitle("Pad Column");
           h1_hits_on_sensor->GetYaxis()->SetTitle("Pad Row");
           //h1_hits_on_sensor->GetXaxis()->SetRange(0,21);
           //h1_hits_on_sensor->GetYaxis()->SetRange(0,83);
           h1_hits_on_sensor->GetXaxis()->SetLabelSize(0.07);
           h1_hits_on_sensor->GetYaxis()->SetLabelSize(0.07);
           h1_hits_on_sensor->SetStats(kFALSE);
           h1_hits_on_sensor->SetMinimum(1);
           h1_hits_on_sensor->Draw("colz");
	   cout << "hits on Layer 1: " << h1_hits_on_sensor->GetEntries() << endl;;
           p1_hitmap->Update();
           // DRAW ==========> Draw the Col hit map
           p1_hitColmap->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h1_hits_on_Col->SetTitle("Accumulated Hits on Col's");
           h1_hits_on_Col->SetTitleSize(0.5);
           h1_hits_on_Col->GetXaxis()->SetTitle("Col number");
           h1_hits_on_Col->GetYaxis()->SetTitle("No. Hits");
           h1_hits_on_Col->SetLineColor(kBlack);
           h1_hits_on_Col->SetFillColor(kGreen);
           // h1_hits_on_Col->GetXaxis()->SetRange(0,15);
           h1_hits_on_Col->GetXaxis()->SetLabelSize(0.07);
           h1_hits_on_Col->GetYaxis()->SetLabelSize(0.07);
           h1_hits_on_Col->SetStats(kFALSE);
           h1_hits_on_Col->Draw("BAR");
           p1_hitColmap->Update();
           //h1_hits_on_Col->Fit("gaus");
           p1_hitColmap->Update();
           // DRAW ==========> Draw the Row hit map
           p1_hitRowmap->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h1_hits_on_Row->SetTitle("Accumulated Hits on Row's");
           h1_hits_on_Row->SetTitleSize(0.5);
           h1_hits_on_Row->GetXaxis()->SetTitle("Row number");
           h1_hits_on_Row->GetYaxis()->SetTitle("No. Hits");
           h1_hits_on_Row->SetLineColor(kBlack);
           h1_hits_on_Row->SetFillColor(kGreen);
           // h1_hits_on_Row->GetXaxis()->SetRange(0,72);
           h1_hits_on_Row->GetXaxis()->SetLabelSize(0.07);
           h1_hits_on_Row->GetYaxis()->SetLabelSize(0.07);
           h1_hits_on_Row->SetStats(kFALSE);
	   h1_hits_on_Row->Draw("HBAR");
           p1_hitRowmap->Update();
//	   h1_hits_on_Row->Fit("gaus");
	   h1_hits_on_Row->Draw("HBAR");
           p1_hitRowmap->Update();
           // DRAW ==========> Draw the Sigma of the Pedestal spectrum
           p1_PEDspectrum->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h1_PEDspectrum->SetTitle("Pedestal Sigma");
           h1_PEDspectrum->SetTitleSize(0.5);
           h1_PEDspectrum->GetXaxis()->SetTitle("ADC value");
           h1_PEDspectrum->GetYaxis()->SetTitle("No. Hits");
           h1_PEDspectrum->SetMinimum(0);
           h1_PEDspectrum->SetLineColor(kBlack);
           h1_PEDspectrum->SetFillColor(kGreen);
           h1_PEDspectrum->GetXaxis()->SetLabelSize(0.05);
           h1_PEDspectrum->Draw("BAR");
           p1_PEDspectrum->Update();
           h1_PEDspectrum->Fit("gaus");
           p1_PEDspectrum->Update();
           // DRAW ==========> Draw the ADC spectrum
           p1_ADCspectrum->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h1_ADCspectrum->SetTitle("ADC spectrum");
           h1_ADCspectrum->SetTitleSize(0.5);
           h1_ADCspectrum->GetXaxis()->SetTitle("ADC value");
           h1_ADCspectrum->GetYaxis()->SetTitle("No. Hits");
           h1_ADCspectrum->SetMinimum(0);
           h1_ADCspectrum->SetLineColor(kBlack);
           h1_ADCspectrum->SetFillColor(kGreen);
           h1_ADCspectrum->GetXaxis()->SetLabelSize(0.05);
           h1_ADCspectrum->Draw("BAR");
           p1_ADCspectrum->Update();
           h1_ADCspectrum->Fit("landau");
           p1_ADCspectrum->Update();
           // DRAW ==========> Draw the signal-to-noise spectrum
           p1_signal2noise->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h1_signal2noise->SetTitle("Signal-to-Noise");
           h1_signal2noise->SetTitleSize(0.5);
           h1_signal2noise->GetXaxis()->SetTitle("ADC value");
           h1_signal2noise->GetYaxis()->SetTitle("No. Hits");
           h1_signal2noise->SetMinimum(0);
           h1_signal2noise->SetLineColor(kBlack);
           h1_signal2noise->SetFillColor(kGreen);
           h1_signal2noise->GetXaxis()->SetLabelSize(0.05);
           h1_signal2noise->Draw("BAR");
           p1_signal2noise->Update();
           h1_signal2noise->Fit("landau");
           p1_signal2noise->Update();
           // DRAW ==========> Draw the maximum TimeBin histogram
           p1_maxTB->cd();
           gStyle->SetOptStat(0);
           h1_maxTB->SetTitle("Maximum TimeBin");
           h1_maxTB->SetTitleSize(0.5);
           h1_maxTB->GetXaxis()->SetTitle("TimeBin of Maximum");
           h1_maxTB->GetYaxis()->SetTitle("No. Hits");
           h1_maxTB->SetMinimum(0);
           h1_maxTB->SetLineColor(kBlack);
           h1_maxTB->SetFillColor(kGreen);
           h1_maxTB->GetXaxis()->SetLabelSize(0.05);
           h1_maxTB->Draw("BAR");
           p1_maxTB->Update();
    //}}}
    //{{{ Draw the info for the middle layer
           // DRAW ==========> Draw the sensor hit map
           gStyle->SetOptStat(111111);
           p2_hitmap->cd();
           h2_hits_on_sensor->SetTitle("Accumulated Hits on Sensor");
           h2_hits_on_sensor->SetTitleSize(0.5);
           h2_hits_on_sensor->GetXaxis()->SetTitle("Pad Column");
           h2_hits_on_sensor->GetYaxis()->SetTitle("Pad Row");
           //h2_hits_on_sensor->GetXaxis()->SetRange(0,21);
           //h2_hits_on_sensor->GetYaxis()->SetRange(0,83);
           h2_hits_on_sensor->GetXaxis()->SetLabelSize(0.07);
           h2_hits_on_sensor->GetYaxis()->SetLabelSize(0.07);
           h2_hits_on_sensor->SetStats(kFALSE);
           h2_hits_on_sensor->SetMinimum(1);
           h2_hits_on_sensor->Draw("colz");
	   cout << "hits on Layer 2: " << h2_hits_on_sensor->GetEntries() << endl;;
           p2_hitmap->Update();
           // DRAW ==========> Draw the Col hit map
           p2_hitColmap->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h2_hits_on_Col->SetTitle("Accumulated Hits on Col's");
           h2_hits_on_Col->SetTitleSize(0.5);
           h2_hits_on_Col->GetXaxis()->SetTitle("Col number");
           h2_hits_on_Col->GetYaxis()->SetTitle("No. Hits");
           h2_hits_on_Col->SetLineColor(kBlack);
           h2_hits_on_Col->SetFillColor(kGreen);
           // h2_hits_on_Col->GetXaxis()->SetRange(0,15);
           h2_hits_on_Col->GetXaxis()->SetLabelSize(0.07);
           h2_hits_on_Col->GetYaxis()->SetLabelSize(0.07);
           h2_hits_on_Col->SetStats(kFALSE);
           h2_hits_on_Col->Draw("BAR");
           p2_hitColmap->Update();
//           h2_hits_on_Col->Fit("gaus");
           p2_hitColmap->Update();
           // DRAW ==========> Draw the Row hit map
           p2_hitRowmap->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h2_hits_on_Row->SetTitle("Accumulated Hits on Row's");
           h2_hits_on_Row->SetTitleSize(0.5);
           h2_hits_on_Row->GetXaxis()->SetTitle("Row number");
           h2_hits_on_Row->GetYaxis()->SetTitle("No. Hits");
           h2_hits_on_Row->SetLineColor(kBlack);
           h2_hits_on_Row->SetFillColor(kGreen);
           // h2_hits_on_Row->GetXaxis()->SetRange(0,72);
           h2_hits_on_Row->GetXaxis()->SetLabelSize(0.07);
           h2_hits_on_Row->GetYaxis()->SetLabelSize(0.07);
           h2_hits_on_Row->SetStats(kFALSE);
	   h2_hits_on_Row->Draw("HBAR");
           p2_hitRowmap->Update();
//	   h2_hits_on_Row->Fit("gaus");
	   h2_hits_on_Row->Draw("HBAR");
           p2_hitRowmap->Update();
           // DRAW ==========> Draw the Sigma of the Pedestal spectrum
           p2_PEDspectrum->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h2_PEDspectrum->SetTitle("Pedestal Sigma");
           h2_PEDspectrum->SetTitleSize(0.5);
           h2_PEDspectrum->GetXaxis()->SetTitle("ADC value");
           h2_PEDspectrum->GetYaxis()->SetTitle("No. Hits");
           h2_PEDspectrum->SetMinimum(0);
           h2_PEDspectrum->SetLineColor(kBlack);
           h2_PEDspectrum->SetFillColor(kGreen);
           h2_PEDspectrum->GetXaxis()->SetLabelSize(0.05);
           h2_PEDspectrum->Draw("BAR");
           p2_PEDspectrum->Update();
           h2_PEDspectrum->Fit("gaus");
           p2_PEDspectrum->Update();
           // DRAW ==========> Draw the ADC spectrum
           p2_ADCspectrum->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h2_ADCspectrum->SetTitle("ADC spectrum");
           h2_ADCspectrum->SetTitleSize(0.5);
           h2_ADCspectrum->GetXaxis()->SetTitle("ADC value");
           h2_ADCspectrum->GetYaxis()->SetTitle("No. Hits");
           h2_ADCspectrum->SetMinimum(0);
           h2_ADCspectrum->SetLineColor(kBlack);
           h2_ADCspectrum->SetFillColor(kGreen);
           h2_ADCspectrum->GetXaxis()->SetLabelSize(0.05);
           h2_ADCspectrum->Draw("BAR");
           p2_ADCspectrum->Update();
           h2_ADCspectrum->Fit("landau");
           p2_ADCspectrum->Update();
           // DRAW ==========> Draw the signal-to-noise spectrum
           p2_signal2noise->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h2_signal2noise->SetTitle("Signal-to-Noise");
           h2_signal2noise->SetTitleSize(0.5);
           h2_signal2noise->GetXaxis()->SetTitle("ADC value");
           h2_signal2noise->GetYaxis()->SetTitle("No. Hits");
           h2_signal2noise->SetMinimum(0);
           h2_signal2noise->SetLineColor(kBlack);
           h2_signal2noise->SetFillColor(kGreen);
           h2_signal2noise->GetXaxis()->SetLabelSize(0.05);
           h2_signal2noise->Draw("BAR");
           p2_signal2noise->Update();
           h2_signal2noise->Fit("landau");
           p2_signal2noise->Update();
           // DRAW ==========> Draw the maximum TimeBin histogram
           p2_maxTB->cd();
           gStyle->SetOptStat("neMR");
           h2_maxTB->SetTitle("Maximum TimeBin");
           h2_maxTB->SetTitleSize(0.5);
           h2_maxTB->GetXaxis()->SetTitle("TimeBin of Maximum");
           h2_maxTB->GetYaxis()->SetTitle("No. Hits");
           h2_maxTB->SetMinimum(0);
           h2_maxTB->SetLineColor(kBlack);
           h2_maxTB->SetFillColor(kGreen);
           h2_maxTB->GetXaxis()->SetLabelSize(0.05);
           h2_maxTB->Draw("BAR");
           p2_maxTB->Update();
    //}}}
    //{{{ Draw the info for the bottom layer
           // DRAW ==========> Draw the sensor hit map
           gStyle->SetOptStat(111111);
           p3_hitmap->cd();
           h3_hits_on_sensor->SetTitle("Accumulated Hits on Sensor");
           h3_hits_on_sensor->SetTitleSize(0.5);
           h3_hits_on_sensor->GetXaxis()->SetTitle("Pad Column");
           h3_hits_on_sensor->GetYaxis()->SetTitle("Pad Row");
           //h3_hits_on_sensor->GetXaxis()->SetRange(0,21);
           //h3_hits_on_sensor->GetYaxis()->SetRange(0,83);
           h3_hits_on_sensor->GetXaxis()->SetLabelSize(0.07);
           h3_hits_on_sensor->GetYaxis()->SetLabelSize(0.07);
           h3_hits_on_sensor->SetStats(kFALSE);
           h3_hits_on_sensor->SetMinimum(1);
           h3_hits_on_sensor->Draw("colz");
	   cout << "hits on Layer 3: " << h3_hits_on_sensor->GetEntries() << endl;;
           p3_hitmap->Update();
           // DRAW ==========> Draw the Col hit map
           p3_hitColmap->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h3_hits_on_Col->SetTitle("Accumulated Hits on Col's");
           h3_hits_on_Col->SetTitleSize(0.5);
           h3_hits_on_Col->GetXaxis()->SetTitle("Col number");
           h3_hits_on_Col->GetYaxis()->SetTitle("No. Hits");
           h3_hits_on_Col->SetLineColor(kBlack);
           h3_hits_on_Col->SetFillColor(kGreen);
           // h3_hits_on_Col->GetXaxis()->SetRange(0,15);
           h3_hits_on_Col->GetXaxis()->SetLabelSize(0.07);
           h3_hits_on_Col->GetYaxis()->SetLabelSize(0.07);
           h3_hits_on_Col->SetStats(kFALSE);
           h3_hits_on_Col->Draw("BAR");
           p3_hitColmap->Update();
//           h3_hits_on_Col->Fit("gaus");
           p3_hitColmap->Update();
           // DRAW ==========> Draw the Row hit map
           p3_hitRowmap->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h3_hits_on_Row->SetTitle("Accumulated Hits on Row's");
           h3_hits_on_Row->SetTitleSize(0.5);
           h3_hits_on_Row->GetXaxis()->SetTitle("Row number");
           h3_hits_on_Row->GetYaxis()->SetTitle("No. Hits");
           h3_hits_on_Row->SetLineColor(kBlack);
           h3_hits_on_Row->SetFillColor(kGreen);
           // h3_hits_on_Row->GetXaxis()->SetRange(0,72);
           h3_hits_on_Row->GetXaxis()->SetLabelSize(0.07);
           h3_hits_on_Row->GetYaxis()->SetLabelSize(0.07);
           h3_hits_on_Row->SetStats(kFALSE);
	   h3_hits_on_Row->Draw("HBAR");
           p3_hitRowmap->Update();
//	   h3_hits_on_Row->Fit("gaus");
	   h3_hits_on_Row->Draw("HBAR");
           p3_hitRowmap->Update();
           // DRAW ==========> Draw the Sigma of the Pedestal spectrum
           p3_PEDspectrum->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h3_PEDspectrum->SetTitle("Pedestal Sigma");
           h3_PEDspectrum->SetTitleSize(0.5);
           h3_PEDspectrum->GetXaxis()->SetTitle("ADC value");
           h3_PEDspectrum->GetYaxis()->SetTitle("No. Hits");
           h3_PEDspectrum->SetMinimum(0);
           h3_PEDspectrum->SetLineColor(kBlack);
           h3_PEDspectrum->SetFillColor(kGreen);
           h3_PEDspectrum->GetXaxis()->SetLabelSize(0.05);
           h3_PEDspectrum->Draw("BAR");
           p3_PEDspectrum->Update();
           h3_PEDspectrum->Fit("gaus");
           p3_PEDspectrum->Update();
           // DRAW ==========> Draw the ADC spectrum
           p3_ADCspectrum->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h3_ADCspectrum->SetTitle("ADC spectrum");
           h3_ADCspectrum->SetTitleSize(0.5);
           h3_ADCspectrum->GetXaxis()->SetTitle("ADC value");
           h3_ADCspectrum->GetYaxis()->SetTitle("No. Hits");
           h3_ADCspectrum->SetMinimum(0);
           h3_ADCspectrum->SetLineColor(kBlack);
           h3_ADCspectrum->SetFillColor(kGreen);
           h3_ADCspectrum->GetXaxis()->SetLabelSize(0.05);
           h3_ADCspectrum->Draw("BAR");
           p3_ADCspectrum->Update();
           h3_ADCspectrum->Fit("landau");
           p3_ADCspectrum->Update();
           // DRAW ==========> Draw the signal-to-noise spectrum
           p3_signal2noise->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h3_signal2noise->SetTitle("Signal-to-Noise");
           h3_signal2noise->SetTitleSize(0.5);
           h3_signal2noise->GetXaxis()->SetTitle("ADC value");
           h3_signal2noise->GetYaxis()->SetTitle("No. Hits");
           h3_signal2noise->SetMinimum(0);
           h3_signal2noise->SetLineColor(kBlack);
           h3_signal2noise->SetFillColor(kGreen);
           h3_signal2noise->GetXaxis()->SetLabelSize(0.05);
           h3_signal2noise->Draw("BAR");
           p3_signal2noise->Update();
           h3_signal2noise->Fit("landau");
           p3_signal2noise->Update();
           // DRAW ==========> Draw the maximum TimeBin histogram
           p3_maxTB->cd();
           gStyle->SetOptStat(0);
           h3_maxTB->SetTitle("Maximum TimeBin");
           h3_maxTB->SetTitleSize(0.5);
           h3_maxTB->GetXaxis()->SetTitle("TimeBin of Maximum");
           h3_maxTB->GetYaxis()->SetTitle("No. Hits");
           h3_maxTB->SetMinimum(0);
           h3_maxTB->SetLineColor(kBlack);
           h3_maxTB->SetFillColor(kGreen);
           h3_maxTB->GetXaxis()->SetLabelSize(0.05);
           h3_maxTB->Draw("BAR");
           p3_maxTB->Update();
    //}}}
    //{{{ Draw the info for the zero layer
           // DRAW ==========> Draw the sensor hit map
           gStyle->SetOptStat(111111);
           p0_hitmap->cd();
           h0_hits_on_sensor->SetTitle("Accumulated Hits on Sensor");
           h0_hits_on_sensor->SetTitleSize(0.5);
           h0_hits_on_sensor->GetXaxis()->SetTitle("Pad Column");
           h0_hits_on_sensor->GetYaxis()->SetTitle("Pad Row");
           //h0_hits_on_sensor->GetXaxis()->SetRange(0,21);
           //h0_hits_on_sensor->GetYaxis()->SetRange(0,83);
           h0_hits_on_sensor->GetXaxis()->SetLabelSize(0.07);
           h0_hits_on_sensor->GetYaxis()->SetLabelSize(0.07);
           h0_hits_on_sensor->SetStats(kFALSE);
           h0_hits_on_sensor->SetMinimum(1);
           h0_hits_on_sensor->Draw("colz");
	   cout << "hits on Layer 3: " << h0_hits_on_sensor->GetEntries() << endl;;
           p0_hitmap->Update();
           // DRAW ==========> Draw the Col hit map
           p0_hitColmap->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h0_hits_on_Col->SetTitle("Accumulated Hits on Col's");
           h0_hits_on_Col->SetTitleSize(0.5);
           h0_hits_on_Col->GetXaxis()->SetTitle("Col number");
           h0_hits_on_Col->GetYaxis()->SetTitle("No. Hits");
           h0_hits_on_Col->SetLineColor(kBlack);
           h0_hits_on_Col->SetFillColor(kGreen);
           // h0_hits_on_Col->GetXaxis()->SetRange(0,15);
           h0_hits_on_Col->GetXaxis()->SetLabelSize(0.07);
           h0_hits_on_Col->GetYaxis()->SetLabelSize(0.07);
           h0_hits_on_Col->SetStats(kFALSE);
           h0_hits_on_Col->Draw("BAR");
           p0_hitColmap->Update();
//           h0_hits_on_Col->Fit("gaus");
           p0_hitColmap->Update();
           // DRAW ==========> Draw the Row hit map
           p0_hitRowmap->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h0_hits_on_Row->SetTitle("Accumulated Hits on Row's");
           h0_hits_on_Row->SetTitleSize(0.5);
           h0_hits_on_Row->GetXaxis()->SetTitle("Row number");
           h0_hits_on_Row->GetYaxis()->SetTitle("No. Hits");
           h0_hits_on_Row->SetLineColor(kBlack);
           h0_hits_on_Row->SetFillColor(kGreen);
           // h0_hits_on_Row->GetXaxis()->SetRange(0,72);
           h0_hits_on_Row->GetXaxis()->SetLabelSize(0.07);
           h0_hits_on_Row->GetYaxis()->SetLabelSize(0.07);
           h0_hits_on_Row->SetStats(kFALSE);
	   h0_hits_on_Row->Draw("HBAR");
           p0_hitRowmap->Update();
//	   h0_hits_on_Row->Fit("gaus");
	   h0_hits_on_Row->Draw("HBAR");
           p0_hitRowmap->Update();
           // DRAW ==========> Draw the Sigma of the Pedestal spectrum
           p0_PEDspectrum->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h0_PEDspectrum->SetTitle("Pedestal Sigma");
           h0_PEDspectrum->SetTitleSize(0.5);
           h0_PEDspectrum->GetXaxis()->SetTitle("ADC value");
           h0_PEDspectrum->GetYaxis()->SetTitle("No. Hits");
           h0_PEDspectrum->SetMinimum(0);
           h0_PEDspectrum->SetLineColor(kBlack);
           h0_PEDspectrum->SetFillColor(kGreen);
           h0_PEDspectrum->GetXaxis()->SetLabelSize(0.05);
           h0_PEDspectrum->Draw("BAR");
           p0_PEDspectrum->Update();
           h0_PEDspectrum->Fit("gaus");
           p0_PEDspectrum->Update();
           // DRAW ==========> Draw the ADC spectrum
           p0_ADCspectrum->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h0_ADCspectrum->SetTitle("ADC spectrum");
           h0_ADCspectrum->SetTitleSize(0.5);
           h0_ADCspectrum->GetXaxis()->SetTitle("ADC value");
           h0_ADCspectrum->GetYaxis()->SetTitle("No. Hits");
           h0_ADCspectrum->SetMinimum(0);
           h0_ADCspectrum->SetLineColor(kBlack);
           h0_ADCspectrum->SetFillColor(kGreen);
           h0_ADCspectrum->GetXaxis()->SetLabelSize(0.05);
           h0_ADCspectrum->Draw("BAR");
           p0_ADCspectrum->Update();
           h0_ADCspectrum->Fit("landau");
           p0_ADCspectrum->Update();
           // DRAW ==========> Draw the signal-to-noise spectrum
           p0_signal2noise->cd();
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h0_signal2noise->SetTitle("Signal-to-Noise");
           h0_signal2noise->SetTitleSize(0.5);
           h0_signal2noise->GetXaxis()->SetTitle("ADC value");
           h0_signal2noise->GetYaxis()->SetTitle("No. Hits");
           h0_signal2noise->SetMinimum(0);
           h0_signal2noise->SetLineColor(kBlack);
           h0_signal2noise->SetFillColor(kGreen);
           h0_signal2noise->GetXaxis()->SetLabelSize(0.05);
           h0_signal2noise->Draw("BAR");
           p0_signal2noise->Update();
           h0_signal2noise->Fit("landau");
           p0_signal2noise->Update();
           // DRAW ==========> Draw the maximum TimeBin histogram
           p0_maxTB->cd();
           gStyle->SetOptStat(0);
           h0_maxTB->SetTitle("Maximum TimeBin");
           h0_maxTB->SetTitleSize(0.5);
           h0_maxTB->GetXaxis()->SetTitle("TimeBin of Maximum");
           h0_maxTB->GetYaxis()->SetTitle("No. Hits");
           h0_maxTB->SetMinimum(0);
           h0_maxTB->SetLineColor(kBlack);
           h0_maxTB->SetFillColor(kGreen);
           h0_maxTB->GetXaxis()->SetLabelSize(0.05);
           h0_maxTB->Draw("BAR");
           p0_maxTB->Update();
    //}}}
    //{{{ Draw the info for the tracked hits
           // TRACK INFO DRAW ==========> Draw the bottom ADC spectrum
           c1_TrackDisplay->cd();
           p1_track_ADCspectrum->cd();
           gStyle->SetOptStat(111111);
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h1_track_ADCspectrum->SetTitle("Layer 1: ADC spectrum");
           h1_track_ADCspectrum->SetTitleSize(0.5);
           h1_track_ADCspectrum->GetXaxis()->SetTitle("ADC value");
           h1_track_ADCspectrum->GetYaxis()->SetTitle("No. Hits");
           h1_track_ADCspectrum->SetMinimum(0);
           h1_track_ADCspectrum->SetLineColor(kBlack);
           h1_track_ADCspectrum->SetFillColor(kGreen);
           h1_track_ADCspectrum->GetXaxis()->SetLabelSize(0.05);
           h1_track_ADCspectrum->Draw("BAR");
           p1_track_ADCspectrum->Update();
           h1_track_ADCspectrum->Fit("landau");
           p1_track_ADCspectrum->Update();
           c1_TrackDisplay->Update();
           // TRACK INFO DRAW ==========> Draw the top ADC spectrum
           c1_TrackDisplay->cd();
           p2_track_ADCspectrum->cd();
           gStyle->SetOptStat(111111);
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h2_track_ADCspectrum->SetTitle("Layer 2: ADC spectrum");
           h2_track_ADCspectrum->SetTitleSize(0.5);
           h2_track_ADCspectrum->GetXaxis()->SetTitle("ADC value");
           h2_track_ADCspectrum->GetYaxis()->SetTitle("No. Hits");
           h2_track_ADCspectrum->SetMinimum(0);
           h2_track_ADCspectrum->SetLineColor(kBlack);
           h2_track_ADCspectrum->SetFillColor(kGreen);
           h2_track_ADCspectrum->GetXaxis()->SetLabelSize(0.05);
           h2_track_ADCspectrum->Draw("BAR");
           p2_track_ADCspectrum->Update();
           h2_track_ADCspectrum->Fit("landau");
           p2_track_ADCspectrum->Update();
           c1_TrackDisplay->Update();
           // TRACK INFO DRAW ==========> Draw the XY-residual plot
           // c1_TrackDisplay->cd(); // comment out by Xu Sun @ 02/13/2020
           // p9_ADC1ADC2->cd();
           // gStyle->SetOptStat("eMR");
           // gStyle->SetOptFit(0000);
           // gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           // gStyle->SetStatW(0.3); gStyle->SetStatH(0.3);
           // h9_Residual->SetTitle("Residual spectrum");
           // h9_Residual->SetTitleSize(0.5);
           // h9_Residual->GetXaxis()->SetTitle("X residual [mm]");
           // h9_Residual->GetYaxis()->SetTitle("Y residual [mm]");
           // h9_Residual->SetMinimum(0);
           // h9_Residual->SetLineColor(kBlack);
           // h9_Residual->SetFillColor(kGreen);
           // h9_Residual->GetXaxis()->SetLabelSize(0.05);
           // h9_Residual->GetYaxis()->SetLabelSize(0.05);
           // h9_Residual->Draw("colz");
           // p9_ADC1ADC2->Update();
           // c1_TrackDisplay->Update();
           // TRACK INFO DRAW ==========> Draw the X-residual plot
           c1_TrackDisplay->cd();
           p9_XResidual->cd();
           gStyle->SetOptStat("eMR");
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.3);
           h9_XResidual->SetTitle("X Residual spectrum");
           h9_XResidual->SetTitleSize(0.5);
           h9_XResidual->GetXaxis()->SetTitle("X residual [mm]");
           h9_XResidual->SetMinimum(0);
           h9_XResidual->SetLineColor(kBlack);
           h9_XResidual->SetFillColor(kGreen);
           h9_XResidual->GetXaxis()->SetLabelSize(0.05);
           h9_XResidual->GetYaxis()->SetLabelSize(0.05);
           h9_XResidual->Draw("");
           p9_XResidual->Update();
           h9_XResidual->Fit("gaus");
           p9_XResidual->Update();
           c1_TrackDisplay->Update();
           // TRACK INFO DRAW ==========> Draw the Y-residual plot
           c1_TrackDisplay->cd();
           p9_YResidual->cd();
           gStyle->SetOptStat("eMR");
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.3);
           h9_YResidual->SetTitle("Y Residual spectrum");
           h9_YResidual->SetTitleSize(0.5);
           h9_YResidual->GetXaxis()->SetTitle("Y residual [mm]");
           h9_YResidual->SetMinimum(0);
           h9_YResidual->SetLineColor(kBlack);
           h9_YResidual->SetFillColor(kGreen);
           h9_YResidual->GetXaxis()->SetLabelSize(0.05);
           h9_YResidual->GetYaxis()->SetLabelSize(0.05);
           h9_YResidual->Draw("");
           p9_YResidual->Update();
           h9_YResidual->Fit("gaus");
           p9_YResidual->Update();
           c1_TrackDisplay->Update();
/*           // TRACK INFO DRAW ==========> Draw the top ADC spectrum
           c1_TrackDisplay->cd();
           p9_ADC1ADC2->cd();
           gStyle->SetOptStat(000000);
           gStyle->SetOptFit(0000);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h9_ADC1_ADCnext->SetTitle("Cross talk");
           h9_ADC1_ADCnext->SetTitleSize(0.5);
           h9_ADC1_ADCnext->GetXaxis()->SetTitle("ADC value");
           h9_ADC1_ADCnext->GetYaxis()->SetTitle("ADC value next row");
           h9_ADC1_ADCnext->SetMinimum(0);
           h9_ADC1_ADCnext->SetLineColor(kBlack);
           h9_ADC1_ADCnext->SetFillColor(kGreen);
           h9_ADC1_ADCnext->GetXaxis()->SetLabelSize(0.05);
           h9_ADC1_ADCnext->Draw("colz");
           p9_ADC1ADC2->Update();
           c1_TrackDisplay->Update();
           // TRACK INFO DRAW ==========> Draw the top ADC-ADC correlation plot
           c1_TrackDisplay->cd();
           p9_ADC1ADC2->cd();
           gStyle->SetOptStat(000000);
           gStyle->SetOptFit(0000);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h9_ADC1_TrackAngle->SetTitle("Energy deposition vs track angle");
           h9_ADC1_TrackAngle->SetTitleSize(0.5);
           h9_ADC1_TrackAngle->GetXaxis()->SetTitle("Angle [degrees]");
           h9_ADC1_TrackAngle->GetYaxis()->SetTitle("ADC1 value");
           h9_ADC1_TrackAngle->SetMinimum(0);
           h9_ADC1_TrackAngle->SetLineColor(kBlack);
           h9_ADC1_TrackAngle->SetFillColor(kGreen);
           h9_ADC1_TrackAngle->GetXaxis()->SetLabelSize(0.05);
           h9_ADC1_TrackAngle->Draw("colz");
           p9_ADC1ADC2->Update();
           c1_TrackDisplay->Update();
           // TRACK INFO DRAW ==========> Draw the top ADC-ADC correlation plot
           c1_TrackDisplay->cd();
           p9_ADC1ADC2->cd();
           gStyle->SetOptStat(000000);
           gStyle->SetOptFit(0000);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h9_ADC1ADC2->SetTitle("Energy deposition Layer 1 vs Layer 2");
           h9_ADC1ADC2->SetTitleSize(0.5);
           h9_ADC1ADC2->GetXaxis()->SetTitle("ADC1 angle corrected");
           h9_ADC1ADC2->GetYaxis()->SetTitle("ADC2 angle corrected");
           h9_ADC1ADC2->SetMinimum(0);
           h9_ADC1ADC2->SetLineColor(kBlack);
           h9_ADC1ADC2->SetFillColor(kGreen);
           h9_ADC1ADC2->GetXaxis()->SetLabelSize(0.05);
           h9_ADC1ADC2->Draw("colz");
           p9_ADC1ADC2->Update();
           c1_TrackDisplay->Update();
           // TRACK INFO DRAW ==========> Draw the top ADC-ADC correlation plot
           c1_TrackDisplay->cd();
           p9_ADC1ADC2->cd();
           gStyle->SetOptFit(0000);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h9_TrackAngle->SetTitle("Track Angle spectrum");
           h9_TrackAngle->SetTitleSize(0.5);
           h9_TrackAngle->GetXaxis()->SetTitle("Track Angle [degrees]");
           h9_TrackAngle->GetYaxis()->SetTitle("counts");
           h9_TrackAngle->SetMinimum(0);
           h9_TrackAngle->SetLineColor(kBlack);
           h9_TrackAngle->SetFillColor(kGreen);
           h9_TrackAngle->GetXaxis()->SetLabelSize(0.05);
           h9_TrackAngle->Draw("BAR");
           p9_ADC1ADC2->Update();
           c1_TrackDisplay->Update();
*/           // TRACK INFO DRAW ==========> Draw the bottom ADC spectrum
           c1_TrackDisplay->cd();
           p1_track_angcorr_ADCspectrum->cd();
           gStyle->SetOptStat(111111);
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h1_track_angcorr_ADCspectrum->SetTitle("Layer 1: angle corrected ADC spectrum");
           h1_track_angcorr_ADCspectrum->SetTitleSize(0.5);
           h1_track_angcorr_ADCspectrum->GetXaxis()->SetTitle("ADC value");
           h1_track_angcorr_ADCspectrum->GetYaxis()->SetTitle("No. Hits");
           h1_track_angcorr_ADCspectrum->SetMinimum(0);
           h1_track_angcorr_ADCspectrum->SetLineColor(kBlack);
           h1_track_angcorr_ADCspectrum->SetFillColor(kGreen);
           h1_track_angcorr_ADCspectrum->GetXaxis()->SetLabelSize(0.05);
           h1_track_angcorr_ADCspectrum->Draw("BAR");
           p1_track_angcorr_ADCspectrum->Update();
           h1_track_angcorr_ADCspectrum->Fit("landau");
           p1_track_angcorr_ADCspectrum->Update();
           c1_TrackDisplay->Update();
           // TRACK INFO DRAW ==========> Draw the top ADC spectrum
           c1_TrackDisplay->cd();
           p2_track_angcorr_ADCspectrum->cd();
           gStyle->SetOptStat(111111);
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h2_track_angcorr_ADCspectrum->SetTitle("Layer 2: angle corrected ADC spectrum");
           h2_track_angcorr_ADCspectrum->SetTitleSize(0.5);
           h2_track_angcorr_ADCspectrum->GetXaxis()->SetTitle("ADC value");
           h2_track_angcorr_ADCspectrum->GetYaxis()->SetTitle("No. Hits");
           h2_track_angcorr_ADCspectrum->SetMinimum(0);
           h2_track_angcorr_ADCspectrum->SetLineColor(kBlack);
           h2_track_angcorr_ADCspectrum->SetFillColor(kGreen);
           h2_track_angcorr_ADCspectrum->GetXaxis()->SetLabelSize(0.05);
           h2_track_angcorr_ADCspectrum->Draw("BAR");
           p2_track_angcorr_ADCspectrum->Update();
           h2_track_angcorr_ADCspectrum->Fit("landau");
           p2_track_angcorr_ADCspectrum->Update();
           c1_TrackDisplay->Update();
    //}}}
    //{{{ Draw the info for QA
           // QA DRAW ==========> Draw the noise for Section A
           c1_QAplotsDisplay->cd();
           p1_NoiseHistogram->cd();
           gStyle->SetOptStat(111111);
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h1_NoiseSpectrum->SetTitle("Section A: Noise spectrum");
           h1_NoiseSpectrum->SetTitleSize(0.5);
           h1_NoiseSpectrum->GetXaxis()->SetTitle("ADC value");
           h1_NoiseSpectrum->GetYaxis()->SetTitle("No. Hits");
           h1_NoiseSpectrum->SetMinimum(0);
           h1_NoiseSpectrum->SetLineColor(kBlack);
           h1_NoiseSpectrum->SetFillColor(kGreen);
           h1_NoiseSpectrum->GetXaxis()->SetLabelSize(0.05);
           h1_NoiseSpectrum->Draw("BAR");
           p1_NoiseHistogram->Update();
           //h1_NoiseSpectrum->Fit("landau");
           //h1_NoiseSpectrum->Fit("gaus","","",0,50);
           p1_NoiseHistogram->Update();
           c1_QAplotsDisplay->Update();
           // QA DRAW ==========> Draw the noise for Section B
           c1_QAplotsDisplay->cd();
           p2_NoiseHistogram->cd();
           gStyle->SetOptStat(111111);
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h2_NoiseSpectrum->SetTitle("Section B: Noise spectrum");
           h2_NoiseSpectrum->SetTitleSize(0.5);
           h2_NoiseSpectrum->GetXaxis()->SetTitle("ADC value");
           h2_NoiseSpectrum->GetYaxis()->SetTitle("No. Hits");
           h2_NoiseSpectrum->SetMinimum(0);
           h2_NoiseSpectrum->SetLineColor(kBlack);
           h2_NoiseSpectrum->SetFillColor(kGreen);
           h2_NoiseSpectrum->GetXaxis()->SetLabelSize(0.05);
           h2_NoiseSpectrum->Draw("BAR");
           p2_NoiseHistogram->Update();
           //h2_NoiseSpectrum->Fit("landau");
           //h2_NoiseSpectrum->Fit("gaus","","",0,50);
           p2_NoiseHistogram->Update();
           c1_QAplotsDisplay->Update();
           // QA DRAW ==========> Draw the noise for Section C
           c1_QAplotsDisplay->cd();
           p3_NoiseHistogram->cd();
           gStyle->SetOptStat(111111);
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h3_NoiseSpectrum->SetTitle("Section C: Noise spectrum");
           h3_NoiseSpectrum->SetTitleSize(0.5);
           h3_NoiseSpectrum->GetXaxis()->SetTitle("ADC value");
           h3_NoiseSpectrum->GetYaxis()->SetTitle("No. Hits");
           h3_NoiseSpectrum->SetMinimum(0);
           h3_NoiseSpectrum->SetLineColor(kBlack);
           h3_NoiseSpectrum->SetFillColor(kGreen);
           h3_NoiseSpectrum->GetXaxis()->SetLabelSize(0.05);
           h3_NoiseSpectrum->Draw("BAR");
           p3_NoiseHistogram->Update();
           //h3_NoiseSpectrum->Fit("landau");
           //h3_NoiseSpectrum->Fit("gaus","","",0,50);
           p3_NoiseHistogram->Update();
           c1_QAplotsDisplay->Update();
           // Now remove everything within X sigma of the peak and see what is left
           // QA DRAW ==========> Draw the Strange noise for Section A
           c1_QAplotsDisplay->cd();
           p1_StrangeNoiseHistogram->cd();
           gStyle->SetOptStat(111111);
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h1_StrangeNoiseSpectrum->SetTitle("Section A: Strange Noise spectrum");
           h1_StrangeNoiseSpectrum->SetTitleSize(0.5);
           h1_StrangeNoiseSpectrum->GetXaxis()->SetTitle("ADC value");
           h1_StrangeNoiseSpectrum->GetYaxis()->SetTitle("No. Hits");
           h1_StrangeNoiseSpectrum->SetMinimum(0);
           h1_StrangeNoiseSpectrum->SetLineColor(kBlack);
           h1_StrangeNoiseSpectrum->SetFillColor(kGreen);
           h1_StrangeNoiseSpectrum->GetXaxis()->SetLabelSize(0.05);
           h1_StrangeNoiseSpectrum->Draw("BAR");
           p1_StrangeNoiseHistogram->Update();
           c1_QAplotsDisplay->Update();
           // QA DRAW ==========> Draw the Strange noise for Section B
           c1_QAplotsDisplay->cd();
           p2_StrangeNoiseHistogram->cd();
           gStyle->SetOptStat(111111);
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h2_StrangeNoiseSpectrum->SetTitle("Section B: Strange Noise spectrum");
           h2_StrangeNoiseSpectrum->SetTitleSize(0.5);
           h2_StrangeNoiseSpectrum->GetXaxis()->SetTitle("ADC value");
           h2_StrangeNoiseSpectrum->GetYaxis()->SetTitle("No. Hits");
           h2_StrangeNoiseSpectrum->SetMinimum(0);
           h2_StrangeNoiseSpectrum->SetLineColor(kBlack);
           h2_StrangeNoiseSpectrum->SetFillColor(kGreen);
           h2_StrangeNoiseSpectrum->GetXaxis()->SetLabelSize(0.05);
           h2_StrangeNoiseSpectrum->Draw("BAR");
           p2_StrangeNoiseHistogram->Update();
           c1_QAplotsDisplay->Update();
           // QA DRAW ==========> Draw the Strange noise for Section C
           c1_QAplotsDisplay->cd();
           p3_StrangeNoiseHistogram->cd();
           gStyle->SetOptStat(111111);
           gStyle->SetOptFit(1111);
           gStyle->SetStatX(0.9); gStyle->SetStatY(0.9);
           gStyle->SetStatW(0.3); gStyle->SetStatH(0.4);
           h3_StrangeNoiseSpectrum->SetTitle("Section C: Strange Noise spectrum");
           h3_StrangeNoiseSpectrum->SetTitleSize(0.5);
           h3_StrangeNoiseSpectrum->GetXaxis()->SetTitle("ADC value");
           h3_StrangeNoiseSpectrum->GetYaxis()->SetTitle("No. Hits");
           h3_StrangeNoiseSpectrum->SetMinimum(0);
           h3_StrangeNoiseSpectrum->SetLineColor(kBlack);
           h3_StrangeNoiseSpectrum->SetFillColor(kGreen);
           h3_StrangeNoiseSpectrum->GetXaxis()->SetLabelSize(0.05);
           h3_StrangeNoiseSpectrum->Draw("BAR");
           p3_StrangeNoiseHistogram->Update();
           c1_QAplotsDisplay->Update();
    //}}}

    currentEvent = -1;

}}}


int main(int argc, char *argv[]) 
{{{

    //==Peel the command line arguments of the program==========================================
    if (argc < 3) { 
        cout << "Minimal usage is -f <infile>" << endl;
        exit(0);
    } else {
        for (int i = 1; i < argc-1; i++) { /* We will iterate over argv[] to get the parameters stored inside.
                                          * Note that we're starting on 1 because we don't need to know the 
                                          * path of the program, which is stored in argv[0] */
            if        (string(argv[i]) == "-f") {
               argv1      = argv[i + 1];
               i++;
            } else if (string(argv[i]) == "--apvlo") {
               SEL_LO_APV1 = atoi(argv[i + 1]);
               i++;
            } else if (string(argv[i]) == "--apvhi") {
               SEL_HI_APV1 = atoi(argv[i + 1]);
               i++;
            } else if (string(argv[i]) == "--pedhitcut") {
               PEDCUT     = atoi(argv[i + 1]);
               i++;
            } else if (string(argv[i]) == "--hitcut") {
               HITCUT     = atoi(argv[i + 1]);
               i++;
            } else if (string(argv[i]) == "--maxhits") {
               MAX_NO_HITS_PER_EVENT = atoi(argv[i + 1]);
               i++;
            } else if (string(argv[i]) == "-stave") {
               STAVEID  = atoi(argv[i + 1]);
               i++;
            } else if (string(argv[i]) == "-bias") {
               BIAS     = atoi(argv[i + 1]);
               i++;
            } else if (string(argv[i]) == "-vpsp") {
               VPSP     = atoi(argv[i + 1]);
               i++;
            } else if (string(argv[i]) == "-date") {
               DATE     = atoi(argv[i + 1]);
               i++;
            } else if (string(argv[i]) == "-time") {
               TIME     = atoi(argv[i + 1]);
               i++;
            } else if (string(argv[i]) == "-sensor") {
               if (      string(argv[i+1]) == "on" ) {
                  SENSOR = true;
               } else if ( string(argv[i+1]) == "off" ) {
                  SENSOR = false;
               } else {
                  cout << "Wrong argument for sensor mount status, please try again..." << endl;
                  exit(0);
               }
               i++;
            } else if (string(argv[i]) == "--HELP") {
               cout << "Program Usage " << endl;
            } else {
               cout << "Not enough or invalid arguments, please try again.\n";
               exit(0);
            }
        }
    }
    //=============================================================================

    //  For graphics to work, I think. . .
    TApplication myapp( "Hello World", &argc, argv );
    // argv1 = myapp.Argv(1);

    //gROOT->SetStyle("classic");
    gStyle->SetPalette(1,0);
  

    // Event display graphs
    for ( int ii = 0; ii < numTBins; ++ii ) {
	g1_eventADCvsChannel[ ii ] = 0;
        g1_eventADCvsChannel[ ii ] = new TGraph();
	g2_eventADCvsChannel[ ii ] = 0;
        g2_eventADCvsChannel[ ii ] = new TGraph();
	g3_eventADCvsChannel[ ii ] = 0;
        g3_eventADCvsChannel[ ii ] = new TGraph();
	g0_eventADCvsChannel[ ii ] = 0;
        g0_eventADCvsChannel[ ii ] = new TGraph();
    }

    // Initialize the dead channel maps
    //for ( int i_apv = 0; i_apv < numAPVs; i_apv++ ) {
    //    for ( int i_ch = 0; i_ch < numChannels; i_ch++ ) {
    //        PedOutofRange[i_apv][i_ch]       = false;
    //        PedSigmaOutofRange[i_apv][i_ch]  = false;
    //        HotChannel[i_apv][i_ch]          = false;
    //        ColdChannel[i_apv][i_ch]         = false;
    //        DeadChannel[i_apv][i_ch]         = false;
    //    }
    //}


    // Pulse displays
    g1_eventADCvsTimebin = new TGraph();
    g2_eventADCvsTimebin = new TGraph();
    g3_eventADCvsTimebin = new TGraph();
    g0_eventADCvsTimebin = new TGraph();

    //cout << "Window W, H = " << gVirtualX->GetWidth() << " " <<  gVirtualX->GetHeight() << endl;

    // Screen size and location parameters for displays 
    Double_t w = 900;      // width
    Double_t h = 900;      // height
    Double_t shift = 0;    // start location x on screen
    Double_t step  = 50;   // amount of x offset between displays
    // {{{ Set up first section display
    c1_MainDisplay  = new TCanvas( "c1_MainDisplay", "c1_MainDisplay",   shift,   0,  w , h );
    c1_MainDisplay->SetWindowSize(w + (w - c1_MainDisplay->GetWw()), h + (h - c1_MainDisplay->GetWh()));
    c1_MainDisplay->SetFillColor(41);
    c1_MainDisplay->Draw();
    c1_MainDisplay->Update();
    p1_PedDisplay    = new TPad("p1_PedDisplay","p1_PedDisplay",       0.005, 0.855,   0.995, 0.995 );
    p1_PedSigma      = new TPad("p1_PedSigma","p1_PedSigma",           0.005, 0.710,   0.995, 0.850 );
    p1_hitColmap     = new TPad("p1_hitColmap","p1_hitColmap",         0.005, 0.535,   0.325, 0.705 );
    p1_hitmap        = new TPad("p1_hitmap","p1_hitmap",               0.335, 0.535,   0.655, 0.705 );
    p1_hitRowmap     = new TPad("p1_hitRowmap","p1_hitRowmap",         0.665, 0.535,   0.995, 0.705 );
    p1_PEDspectrum   = new TPad("p1_PEDspectrum","p1_PEDspectrum",     0.005, 0.360,   0.495, 0.530 );
    p1_EventDisplay  = new TPad("p1_EventDisplay","p1_EventDisplay",   0.505, 0.360,   0.995, 0.530 );
    p1_PulseDisplay  = new TPad("p1_PulseDisplay","p1_PulseDisplay",   0.505, 0.185,   0.995, 0.355 );
    p1_ADCspectrum   = new TPad("p1_ADCspectrum","p1_ADCspectrum",     0.005, 0.185,   0.495, 0.355 );
    p1_signal2noise  = new TPad("p1_signal2noise","p1_signal2noise",   0.005, 0.005,   0.495, 0.180 );
    p1_maxTB         = new TPad("p1_maxTB","p1_maxTB",                 0.505, 0.005,   0.995, 0.180 );
    p1_PedDisplay->SetFillColor(38);   p1_PedDisplay->SetBorderMode(1);    p1_PedDisplay->SetFrameFillColor(19);
    p1_PedSigma->SetFillColor(38);     p1_PedSigma->SetBorderMode(1);      p1_PedSigma->SetFrameFillColor(19);
    p1_hitColmap->SetFillColor(38);    p1_hitColmap->SetBorderMode(1);     p1_hitColmap->SetFrameFillColor(19);
    p1_hitmap->SetFillColor(38);       p1_hitmap->SetBorderMode(1);        p1_hitmap->SetFrameFillColor(19);
    p1_hitRowmap->SetFillColor(38);    p1_hitRowmap->SetBorderMode(1);     p1_hitRowmap->SetFrameFillColor(19);
    p1_PEDspectrum->SetFillColor(38);  p1_PEDspectrum->SetBorderMode(1);   p1_PEDspectrum->SetFrameFillColor(19);
    p1_EventDisplay->SetFillColor(38); p1_EventDisplay->SetBorderMode(1);  p1_EventDisplay->SetFrameFillColor(19);
    p1_PulseDisplay->SetFillColor(38); p1_PulseDisplay->SetBorderMode(1);  p1_PulseDisplay->SetFrameFillColor(19);
    p1_ADCspectrum->SetFillColor(38);  p1_ADCspectrum->SetBorderMode(1);   p1_ADCspectrum->SetFrameFillColor(19);
    p1_signal2noise->SetFillColor(38); p1_signal2noise->SetBorderMode(1);  p1_signal2noise->SetFrameFillColor(19);
    p1_maxTB->SetFillColor(38);        p1_maxTB->SetBorderMode(1);         p1_maxTB->SetFrameFillColor(19);
    p1_PedDisplay->Draw();
    p1_PedSigma->Draw();
    p1_hitColmap->Draw();
    p1_hitmap->Draw();
    p1_hitRowmap->Draw();
    p1_PEDspectrum->Draw();
    p1_EventDisplay->Draw();
    p1_PulseDisplay->Draw();
    p1_ADCspectrum->Draw();
    p1_signal2noise->Draw();
    p1_maxTB->Draw();
    c1_MainDisplay->Update();
    // }}}
    // {{{ Set up second section display
    shift += step;
    c2_MainDisplay  = new TCanvas( "c2_MainDisplay", "c2_MainDisplay",   shift,   0,  w , h );
    c2_MainDisplay->SetWindowSize(w + (w - c2_MainDisplay->GetWw()), h + (h - c2_MainDisplay->GetWh()));
    c2_MainDisplay->SetFillColor(41);
    c2_MainDisplay->Draw();
    c2_MainDisplay->Update();
    p2_PedDisplay    = new TPad("p2_PedDisplay","p2_PedDisplay",       0.005, 0.855,   0.995, 0.995 );
    p2_PedSigma      = new TPad("p2_PedSigma","p2_PedSigma",           0.005, 0.710,   0.995, 0.850 );
    p2_hitColmap     = new TPad("p2_hitColmap","p2_hitColmap",         0.005, 0.535,   0.325, 0.705 );
    p2_hitmap        = new TPad("p2_hitmap","p2_hitmap",               0.335, 0.535,   0.655, 0.705 );
    p2_hitRowmap     = new TPad("p2_hitRowmap","p2_hitRowmap",         0.665, 0.535,   0.995, 0.705 );
    p2_PEDspectrum   = new TPad("p2_PEDspectrum","p2_PEDspectrum",     0.005, 0.360,   0.495, 0.530 );
    p2_EventDisplay  = new TPad("p2_EventDisplay","p2_EventDisplay",   0.505, 0.360,   0.995, 0.530 );
    p2_PulseDisplay  = new TPad("p2_PulseDisplay","p2_PulseDisplay",   0.505, 0.185,   0.995, 0.355 );
    p2_ADCspectrum   = new TPad("p2_ADCspectrum","p2_ADCspectrum",     0.005, 0.185,   0.495, 0.355 );
    p2_signal2noise  = new TPad("p2_signal2noise","p2_signal2noise",   0.005, 0.005,   0.495, 0.180 );
    p2_maxTB         = new TPad("p2_maxTB","p2_maxTB",                 0.505, 0.005,   0.995, 0.180 );
    p2_PedDisplay->SetFillColor(38);   p2_PedDisplay->SetBorderMode(1);    p2_PedDisplay->SetFrameFillColor(19);
    p2_PedSigma->SetFillColor(38);     p2_PedSigma->SetBorderMode(1);      p2_PedSigma->SetFrameFillColor(19);
    p2_hitColmap->SetFillColor(38);    p2_hitColmap->SetBorderMode(1);     p2_hitColmap->SetFrameFillColor(19);
    p2_hitmap->SetFillColor(38);       p2_hitmap->SetBorderMode(1);        p2_hitmap->SetFrameFillColor(19);
    p2_hitRowmap->SetFillColor(38);    p2_hitRowmap->SetBorderMode(1);     p2_hitRowmap->SetFrameFillColor(19);
    p2_PEDspectrum->SetFillColor(38);  p2_PEDspectrum->SetBorderMode(1);   p2_PEDspectrum->SetFrameFillColor(19);
    p2_EventDisplay->SetFillColor(38); p2_EventDisplay->SetBorderMode(1);  p2_EventDisplay->SetFrameFillColor(19);
    p2_PulseDisplay->SetFillColor(38); p2_PulseDisplay->SetBorderMode(1);  p2_PulseDisplay->SetFrameFillColor(19);
    p2_ADCspectrum->SetFillColor(38);  p2_ADCspectrum->SetBorderMode(1);   p2_ADCspectrum->SetFrameFillColor(19);
    p2_signal2noise->SetFillColor(38); p2_signal2noise->SetBorderMode(1);  p2_signal2noise->SetFrameFillColor(19);
    p2_maxTB->SetFillColor(38);        p2_maxTB->SetBorderMode(1);         p2_maxTB->SetFrameFillColor(19);
    p2_PedDisplay->Draw();
    p2_PedSigma->Draw();
    p2_hitColmap->Draw();
    p2_hitmap->Draw();
    p2_hitRowmap->Draw();
    p2_PEDspectrum->Draw();
    p2_EventDisplay->Draw();
    p2_PulseDisplay->Draw();
    p2_ADCspectrum->Draw();
    p2_signal2noise->Draw();
    p2_maxTB->Draw();
    c2_MainDisplay->Update();
    // }}}
    // {{{ Set up third section display
    shift += step;
    c3_MainDisplay  = new TCanvas( "c3_MainDisplay", "c3_MainDisplay",   shift,   0,  w , h );
    c3_MainDisplay->SetWindowSize(w + (w - c3_MainDisplay->GetWw()), h + (h - c3_MainDisplay->GetWh()));
    c3_MainDisplay->SetFillColor(41);
    c3_MainDisplay->Draw();
    c3_MainDisplay->Update();
    p3_PedDisplay    = new TPad("p3_PedDisplay","p3_PedDisplay",       0.005, 0.855,   0.995, 0.995 );
    p3_PedSigma      = new TPad("p3_PedSigma","p3_PedSigma",           0.005, 0.710,   0.995, 0.850 );
    p3_hitColmap     = new TPad("p3_hitColmap","p3_hitColmap",         0.005, 0.535,   0.325, 0.705 );
    p3_hitmap        = new TPad("p3_hitmap","p3_hitmap",               0.335, 0.535,   0.655, 0.705 );
    p3_hitRowmap     = new TPad("p3_hitRowmap","p3_hitRowmap",         0.665, 0.535,   0.995, 0.705 );
    p3_PEDspectrum   = new TPad("p3_PEDspectrum","p3_PEDspectrum",     0.005, 0.360,   0.495, 0.530 );
    p3_EventDisplay  = new TPad("p3_EventDisplay","p3_EventDisplay",   0.505, 0.360,   0.995, 0.530 );
    p3_PulseDisplay  = new TPad("p3_PulseDisplay","p3_PulseDisplay",   0.505, 0.185,   0.995, 0.355 );
    p3_ADCspectrum   = new TPad("p3_ADCspectrum","p3_ADCspectrum",     0.005, 0.185,   0.495, 0.355 );
    p3_signal2noise  = new TPad("p3_signal2noise","p3_signal2noise",   0.005, 0.005,   0.495, 0.180 );
    p3_maxTB         = new TPad("p3_maxTB","p3_maxTB",                 0.505, 0.005,   0.995, 0.180 );
    p3_PedDisplay->SetFillColor(38);   p3_PedDisplay->SetBorderMode(1);    p3_PedDisplay->SetFrameFillColor(19);
    p3_PedSigma->SetFillColor(38);     p3_PedSigma->SetBorderMode(1);      p3_PedSigma->SetFrameFillColor(19);
    p3_hitColmap->SetFillColor(38);    p3_hitColmap->SetBorderMode(1);     p3_hitColmap->SetFrameFillColor(19);
    p3_hitmap->SetFillColor(38);       p3_hitmap->SetBorderMode(1);        p3_hitmap->SetFrameFillColor(19);
    p3_hitRowmap->SetFillColor(38);    p3_hitRowmap->SetBorderMode(1);     p3_hitRowmap->SetFrameFillColor(19);
    p3_PEDspectrum->SetFillColor(38);  p3_PEDspectrum->SetBorderMode(1);   p3_PEDspectrum->SetFrameFillColor(19);
    p3_EventDisplay->SetFillColor(38); p3_EventDisplay->SetBorderMode(1);  p3_EventDisplay->SetFrameFillColor(19);
    p3_PulseDisplay->SetFillColor(38); p3_PulseDisplay->SetBorderMode(1);  p3_PulseDisplay->SetFrameFillColor(19);
    p3_ADCspectrum->SetFillColor(38);  p3_ADCspectrum->SetBorderMode(1);   p3_ADCspectrum->SetFrameFillColor(19);
    p3_signal2noise->SetFillColor(38); p3_signal2noise->SetBorderMode(1);  p3_signal2noise->SetFrameFillColor(19);
    p3_maxTB->SetFillColor(38);        p3_maxTB->SetBorderMode(1);         p3_maxTB->SetFrameFillColor(19);
    p3_PedDisplay->Draw();
    p3_PedSigma->Draw();
    p3_hitColmap->Draw();
    p3_hitmap->Draw();
    p3_hitRowmap->Draw();
    p3_PEDspectrum->Draw();
    p3_EventDisplay->Draw();
    p3_PulseDisplay->Draw();
    p3_ADCspectrum->Draw();
    p3_signal2noise->Draw();
    p3_maxTB->Draw();
    c3_MainDisplay->Update();
    // }}}
    // {{{ Set up zero section display
    shift += step;
    c0_MainDisplay  = new TCanvas( "c0_MainDisplay", "c0_MainDisplay",   shift,   0,  w , h );
    c0_MainDisplay->SetWindowSize(w + (w - c0_MainDisplay->GetWw()), h + (h - c0_MainDisplay->GetWh()));
    c0_MainDisplay->SetFillColor(41);
    c0_MainDisplay->Draw();
    c0_MainDisplay->Update();
    p0_PedDisplay    = new TPad("p0_PedDisplay","p0_PedDisplay",       0.005, 0.855,   0.995, 0.995 );
    p0_PedSigma      = new TPad("p0_PedSigma","p0_PedSigma",           0.005, 0.710,   0.995, 0.850 );
    p0_hitColmap     = new TPad("p0_hitColmap","p0_hitColmap",         0.005, 0.535,   0.325, 0.705 );
    p0_hitmap        = new TPad("p0_hitmap","p0_hitmap",               0.335, 0.535,   0.655, 0.705 );
    p0_hitRowmap     = new TPad("p0_hitRowmap","p0_hitRowmap",         0.665, 0.535,   0.995, 0.705 );
    p0_PEDspectrum   = new TPad("p0_PEDspectrum","p0_PEDspectrum",     0.005, 0.360,   0.495, 0.530 );
    p0_EventDisplay  = new TPad("p0_EventDisplay","p0_EventDisplay",   0.505, 0.360,   0.995, 0.530 );
    p0_PulseDisplay  = new TPad("p0_PulseDisplay","p0_PulseDisplay",   0.505, 0.185,   0.995, 0.355 );
    p0_ADCspectrum   = new TPad("p0_ADCspectrum","p0_ADCspectrum",     0.005, 0.185,   0.495, 0.355 );
    p0_signal2noise  = new TPad("p0_signal2noise","p0_signal2noise",   0.005, 0.005,   0.495, 0.180 );
    p0_maxTB         = new TPad("p0_maxTB","p0_maxTB",                 0.505, 0.005,   0.995, 0.180 );
    p0_PedDisplay->SetFillColor(38);   p0_PedDisplay->SetBorderMode(1);    p0_PedDisplay->SetFrameFillColor(19);
    p0_PedSigma->SetFillColor(38);     p0_PedSigma->SetBorderMode(1);      p0_PedSigma->SetFrameFillColor(19);
    p0_hitColmap->SetFillColor(38);    p0_hitColmap->SetBorderMode(1);     p0_hitColmap->SetFrameFillColor(19);
    p0_hitmap->SetFillColor(38);       p0_hitmap->SetBorderMode(1);        p0_hitmap->SetFrameFillColor(19);
    p0_hitRowmap->SetFillColor(38);    p0_hitRowmap->SetBorderMode(1);     p0_hitRowmap->SetFrameFillColor(19);
    p0_PEDspectrum->SetFillColor(38);  p0_PEDspectrum->SetBorderMode(1);   p0_PEDspectrum->SetFrameFillColor(19);
    p0_EventDisplay->SetFillColor(38); p0_EventDisplay->SetBorderMode(1);  p0_EventDisplay->SetFrameFillColor(19);
    p0_PulseDisplay->SetFillColor(38); p0_PulseDisplay->SetBorderMode(1);  p0_PulseDisplay->SetFrameFillColor(19);
    p0_ADCspectrum->SetFillColor(38);  p0_ADCspectrum->SetBorderMode(1);   p0_ADCspectrum->SetFrameFillColor(19);
    p0_signal2noise->SetFillColor(38); p0_signal2noise->SetBorderMode(1);  p0_signal2noise->SetFrameFillColor(19);
    p0_maxTB->SetFillColor(38);        p0_maxTB->SetBorderMode(1);         p0_maxTB->SetFrameFillColor(19);
    p0_PedDisplay->Draw();
    p0_PedSigma->Draw();
    p0_hitColmap->Draw();
    p0_hitmap->Draw();
    p0_hitRowmap->Draw();
    p0_PEDspectrum->Draw();
    p0_EventDisplay->Draw();
    p0_PulseDisplay->Draw();
    p0_ADCspectrum->Draw();
    p0_signal2noise->Draw();
    p0_maxTB->Draw();
    c0_MainDisplay->Update();
    // }}}
    // {{{ Set up tracking display
    shift += step;
    c1_TrackDisplay  = new TCanvas( "c1_TrackDisplay", "c1_TrackDisplay",   shift,   0,  w , h );
    c1_TrackDisplay->SetWindowSize(w + (w - c1_TrackDisplay->GetWw()), h + (h - c1_TrackDisplay->GetWh()));
    c1_TrackDisplay->SetFillColor(41);
    c1_TrackDisplay->Draw();
    c1_TrackDisplay->Update();
    p1_track_ADCspectrum   = new TPad("p1_track_ADCspectrum","p1_track_ADCspectrum",                             0.005, 0.005,   0.995, 0.145 );
    p1_track_ADCspectrum->SetFillColor(38);  p1_track_ADCspectrum->SetBorderMode(1); p1_track_ADCspectrum->SetFrameFillColor(19);
    p1_track_ADCspectrum->Draw();
    p2_track_ADCspectrum   = new TPad("p2_track_ADCspectrum","p2_track_ADCspectrum",                             0.005, 0.150,   0.995, 0.295 );
    p2_track_ADCspectrum->SetFillColor(38);  p2_track_ADCspectrum->SetBorderMode(1); p2_track_ADCspectrum->SetFrameFillColor(19);
    p2_track_ADCspectrum->Draw();
    // p9_ADC1ADC2   = new TPad("p9_ADC1ADC2","p9_ADC1ADC2",                                                        0.005, 0.300,   0.995, 0.695 );
    // p9_ADC1ADC2->SetFillColor(38);  p9_ADC1ADC2->SetBorderMode(1); p9_ADC1ADC2->SetFrameFillColor(19);
    // p9_ADC1ADC2->Draw();
    //--------------------
    // added by Xu Sun @ 02/13/2020 => display xResidual & yResidual separately
    p9_XResidual = new TPad("p9_XResidual","p9_XResidual",                                                       0.005, 0.300,   0.495, 0.695 );
    p9_XResidual->SetFillColor(38);  p9_XResidual->SetBorderMode(1); p9_XResidual->SetFrameFillColor(19);
    p9_XResidual->Draw();
    p9_YResidual = new TPad("p9_YResidual","p9_YResidual",                                                       0.505, 0.300,   0.995, 0.695 );
    p9_YResidual->SetFillColor(38);  p9_YResidual->SetBorderMode(1); p9_YResidual->SetFrameFillColor(19);
    p9_YResidual->Draw();
    //--------------------
    p1_track_angcorr_ADCspectrum   = new TPad("p1_track_angcorr_ADCspectrum","p1_track_angcorr_ADCspectrum",     0.005, 0.700,   0.995, 0.845 );
    p1_track_angcorr_ADCspectrum->SetFillColor(38);  p1_track_angcorr_ADCspectrum->SetBorderMode(1); p1_track_angcorr_ADCspectrum->SetFrameFillColor(19);
    p1_track_angcorr_ADCspectrum->Draw();
    p2_track_angcorr_ADCspectrum   = new TPad("p2_track_angcorr_ADCspectrum","p2_track_angcorr_ADCspectrum",     0.005, 0.850,   0.995, 0.995 );
    p2_track_angcorr_ADCspectrum->SetFillColor(38);  p2_track_angcorr_ADCspectrum->SetBorderMode(1); p2_track_angcorr_ADCspectrum->SetFrameFillColor(19);
    p2_track_angcorr_ADCspectrum->Draw();
    c1_TrackDisplay->Update();
    // }}}
    // {{{ Set up QA display
    shift += step;
    c1_QAplotsDisplay  = new TCanvas( "c1_QAplotsDisplay", "c1_QAplotsDisplay",   shift,   0,  w , h );
    c1_QAplotsDisplay->SetWindowSize(w + (w - c1_QAplotsDisplay->GetWw()), h + (h - c1_QAplotsDisplay->GetWh()));
    c1_QAplotsDisplay->SetFillColor(41);
    c1_QAplotsDisplay->Draw();
    c1_QAplotsDisplay->Update();
    p1_NoiseHistogram   = new TPad("p1_NoiseHistogram","p1_NoiseHistogram",                             0.005, 0.550,   0.995, 0.695 );
    p1_NoiseHistogram->SetFillColor(38);  p1_NoiseHistogram->SetBorderMode(1); p1_NoiseHistogram->SetFrameFillColor(19);
    p1_NoiseHistogram->Draw();
    p2_NoiseHistogram   = new TPad("p2_NoiseHistogram","p2_NoiseHistogram",                             0.005, 0.700,   0.995, 0.845 );
    p2_NoiseHistogram->SetFillColor(38);  p2_NoiseHistogram->SetBorderMode(1); p2_NoiseHistogram->SetFrameFillColor(19);
    p2_NoiseHistogram->Draw();
    p3_NoiseHistogram   = new TPad("p3_NoiseHistogram","p3_NoiseHistogram",                             0.005, 0.850,   0.995, 0.995 );
    p3_NoiseHistogram->SetFillColor(38);  p3_NoiseHistogram->SetBorderMode(1); p3_NoiseHistogram->SetFrameFillColor(19);
    p3_NoiseHistogram->Draw();
    p1_StrangeNoiseHistogram   = new TPad("p1_StrangeNoiseHistogram","p1_StrangeNoiseHistogram",                             0.005, 0.005,   0.995, 0.145 );
    p1_StrangeNoiseHistogram->SetFillColor(38);  p1_StrangeNoiseHistogram->SetBorderMode(1); p1_StrangeNoiseHistogram->SetFrameFillColor(19);
    p1_StrangeNoiseHistogram->Draw();
    p2_StrangeNoiseHistogram   = new TPad("p2_StrangeNoiseHistogram","p2_StrangeNoiseHistogram",                             0.005, 0.150,   0.995, 0.295 );
    p2_StrangeNoiseHistogram->SetFillColor(38);  p2_StrangeNoiseHistogram->SetBorderMode(1); p2_StrangeNoiseHistogram->SetFrameFillColor(19);
    p2_StrangeNoiseHistogram->Draw();
    p3_StrangeNoiseHistogram   = new TPad("p3_StrangeNoiseHistogram","p3_StrangeNoiseHistogram",                             0.005, 0.300,   0.995, 0.445 );
    p3_StrangeNoiseHistogram->SetFillColor(38);  p3_StrangeNoiseHistogram->SetBorderMode(1); p3_StrangeNoiseHistogram->SetFrameFillColor(19);
    p3_StrangeNoiseHistogram->Draw();
    c1_QAplotsDisplay->Update();
    // }}}

    rtsLogOutput(RTS_LOG_FILE);
    rtsLogLevel((char *)WARN);

    //	Next, let's set up the pedestals.
    CalculatePedestals( argv1, mountpoint );
    getRefPedestalsFromFile("ReferencePedestals.SymLink");
    showPedestals();

    //	So that we have something.
    rdr = new daqReader(argv1);

    string outputfile(argv1);
    outputfile.replace(outputfile.end()-3,outputfile.end(),"root");
    cout << "outputfile set to: " << outputfile.c_str() << endl;

    // TFile *File_OutPut = new TFile("test.root","RECREATE");
    TFile *File_OutPut = new TFile(outputfile.c_str(),"RECREATE");
    File_OutPut->cd();

    tree_Hits = new TTree("tree_Hits","IST Hits info");
    tree_Hits->Branch("evt_rdo", evt_rdo, "evt_rdo[2][2][12]/I");
    tree_Hits->Branch("evt_arm", evt_arm, "evt_arm[2][2][12]/I");
    tree_Hits->Branch("evt_port", evt_port, "evt_port[2][2][12]/I");
    tree_Hits->Branch("evt_apv", evt_apv, "evt_apv[2][2][12]/I");
    tree_Hits->Branch("hit_ch", hit_ch, "hit_ch[2][2][12][1152]/I");
    tree_Hits->Branch("hit_tb", hit_tb, "hit_tb[2][2][12][1152]/I");
    tree_Hits->Branch("hit_adc", hit_adc, "hit_adc[2][2][12][1152]/I");
    tree_Hits->SetAutoSave(50000000);

    // Run over all events, accumulate statistics
    DumpEvents();

    tree_Hits->Write();
    File_OutPut->Close();

    // Fill Event displays
    // jumpToEvent(); // cause errors after tree deleted

    //define plot name
    char buffer_PedDisplay_A[100], buffer_PedDisplay_B[100], buffer_PedDisplay_C[100];
    char buffer_PedSigma_A[100], buffer_PedSigma_B[100], buffer_PedSigma_C[100];    
    char buffer_Noise_A[100], buffer_Noise_B[100], buffer_Noise_C[100];
    char buffer_StrangeNoise_A[100], buffer_StrangeNoise_B[100], buffer_StrangeNoise_C[100];
    if(!SENSOR) {
	sprintf(buffer_PedDisplay_A, "IST-%d_SectionA_Pedestals_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_PedDisplay_B, "IST-%d_SectionB_Pedestals_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_PedDisplay_C, "IST-%d_SectionC_Pedestals_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);

        sprintf(buffer_PedSigma_A, "IST-%d_SectionA_PedSigma_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_PedSigma_B, "IST-%d_SectionB_PedSigma_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_PedSigma_C, "IST-%d_SectionC_PedSigma_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);

	sprintf(buffer_Noise_A, "IST-%d_QA_SectionA_Noise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_Noise_B, "IST-%d_QA_SectionB_Noise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_Noise_C, "IST-%d_QA_SectionC_Noise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);

	sprintf(buffer_StrangeNoise_A, "IST-%d_QA_SectionA_StrangeNoise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_StrangeNoise_B, "IST-%d_QA_SectionB_StrangeNoise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_StrangeNoise_C, "IST-%d_QA_SectionC_StrangeNoise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
    }
    else {
        sprintf(buffer_PedDisplay_A, "IST-%d_WithSensors_SectionA_Pedestals_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_PedDisplay_B, "IST-%d_WithSensors_SectionB_Pedestals_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_PedDisplay_C, "IST-%d_WithSensors_SectionC_Pedestals_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);

        sprintf(buffer_PedSigma_A, "IST-%d_WithSensors_SectionA_PedSigma_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_PedSigma_B, "IST-%d_WithSensors_SectionB_PedSigma_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_PedSigma_C, "IST-%d_WithSensors_SectionC_PedSigma_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);

        sprintf(buffer_Noise_A, "IST-%d_WithSensors_QA_SectionA_Noise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_Noise_B, "IST-%d_WithSensors_QA_SectionB_Noise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_Noise_C, "IST-%d_WithSensors_QA_SectionC_Noise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);

        sprintf(buffer_StrangeNoise_A, "IST-%d_WithSensors_QA_SectionA_StrangeNoise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_StrangeNoise_B, "IST-%d_WithSensors_QA_SectionB_StrangeNoise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
        sprintf(buffer_StrangeNoise_C, "IST-%d_WithSensors_QA_SectionC_StrangeNoise_SensorBias%d_VPSP%d_%d-%d.png", STAVEID, BIAS, VPSP, DATE, TIME);
    }

    for (;;)
    {
	//  Finally, enter the main event loop. There are a few curses things
	//  that we want to set up first.
	WINDOW *theWin = initscr(); //   Initialize the window for curses.
	cbreak(); //    Turn off required carriage returns.
	noecho(); //    Turn off echoing.

	//  Let's print out a prompt.
	clear();
	int line = 0;
	//mvaddstr(line++,0,"Press \"e\" to move to a specific, numbered event.");
	mvaddstr(line++,0,"Press \"k\" and \"j\") to move between events.");
        mvaddstr(line++,0,"Press \"p\" to print the display.");
	mvaddstr(line++,0,"Press \"r\" to refresh the display.");
	mvaddstr(line++,0,"Press \"q\" to quit.");

	std::stringstream currentEventString;
	if ( currentEvent < 0 )
	{
	    currentEventString << "No current event";
	}
	else
	{
	    currentEventString << "Current event #: " << currentEvent;
	}

	mvaddstr(line++,0,currentEventString.str().c_str());
	mvaddstr(line++,0,"> ");

	int currentKey = getch();

	switch (currentKey)
	{
	    case 'j':
		endwin();
                nextEvent();
		break;
	    case 'k': 
		endwin();
                previousEvent();
		break;
	    //case 'e':
	    //	endwin();
            //    jumpToEvent();
	    //	break;
            case 'r':
                endwin();
                c1_MainDisplay->Update();
                c2_MainDisplay->Update();
                c3_MainDisplay->Update();
                c1_TrackDisplay->Update();
                c1_QAplotsDisplay->Update();
                //c1_SecondDisplay->Update();
                //c2_SecondDisplay->Update();
                break;
            case 'p':
                endwin();
		p1_PedDisplay->Print(buffer_PedDisplay_A);

                p2_PedDisplay->Print(buffer_PedDisplay_B);
                p3_PedDisplay->Print(buffer_PedDisplay_C);
		p1_PedSigma->Print(buffer_PedSigma_A);
                p2_PedSigma->Print(buffer_PedSigma_B);
                p3_PedSigma->Print(buffer_PedSigma_C);
                p1_NoiseHistogram->Print(buffer_Noise_A);
                p2_NoiseHistogram->Print(buffer_Noise_B);
                p3_NoiseHistogram->Print(buffer_Noise_C);
                p1_StrangeNoiseHistogram->Print(buffer_StrangeNoise_A);
                p2_StrangeNoiseHistogram->Print(buffer_StrangeNoise_B);
                p3_StrangeNoiseHistogram->Print(buffer_StrangeNoise_C);
/*
		p1_PedDisplay->Print("Pedestal_SectionA.png");
		p1_PedSigma->Print("PedSigma_SectionA.png");
                p2_PedDisplay->Print("Pedestal_SectionB.png");
                p2_PedSigma->Print("PedSigma_SectionB.png");
                p3_PedDisplay->Print("Pedestal_SectionC.png");
                p3_PedSigma->Print("PedSigma_SectionC.png");
		p1_NoiseHistogram->Print("Noise_SectionA.png");
                p2_NoiseHistogram->Print("Noise_SectionB.png");
                p3_NoiseHistogram->Print("Noise_SectionC.png");
                p1_StrangeNoiseHistogram->Print("StrangeNoise_SectionA.png");
                p2_StrangeNoiseHistogram->Print("StrangeNoise_SectionB.png");
                p3_StrangeNoiseHistogram->Print("StrangeNoise_SectionC.png");
                //c1_MainDisplay->Print("Pedestal_SectionA.png");
                //c2_MainDisplay->Print("Pedestal_SectionB.png");
                //c3_MainDisplay->Print("Pedestal_SectionC.png");
                //c1_TrackDisplay->Print("Tracking_Display.png");
                //c1_QAplotsDisplay->Print("Noise_SectionABC.png");
*/
                break;
	    case 'q':
		endwin();
		return 0;
	    default:
		break;
	}
    }

    //	Return to normal terminal mode.

    return 0;
}}}
