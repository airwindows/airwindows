/* ========================================
 *  ConsoleXBuss - ConsoleXBuss.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleXBuss_H
#define __ConsoleXBuss_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParamA = 0,
	kParamB = 1,
	kParamC = 2,
	kParamD = 3,
	kParamE = 4,
	kParamF = 5,
	kParamG = 6,
	kParamH = 7,
	kNumParameters = 8
}; //
const int dscBuf = 90;

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'cnxb';    //Change this to what the AU identity is!

class ConsoleXBuss : 
    public AudioEffectX 
{
public:
    ConsoleXBuss(audioMasterCallback audioMaster);
    ~ConsoleXBuss();
    virtual bool getEffectName(char* name);                       // The plug-in name
    virtual VstPlugCategory getPlugCategory();                    // The general category for the plug-in
    virtual bool getProductString(char* text);                    // This is a unique plug-in string provided by Steinberg
    virtual bool getVendorString(char* text);                     // Vendor info
    virtual VstInt32 getVendorVersion();                          // Version number
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
    virtual void getProgramName(char *name);                      // read the name from the host
    virtual void setProgramName(char *name);                      // changes the name of the preset displayed in the host
	virtual VstInt32 getChunk (void** data, bool isPreset);
	virtual VstInt32 setChunk (void* data, VstInt32 byteSize, bool isPreset);
    virtual float getParameter(VstInt32 index);                   // get the parameter value at the specified index
    virtual void setParameter(VstInt32 index, float value);       // set the parameter at index to value
    virtual void getParameterLabel(VstInt32 index, char *text);  // label for the parameter (eg dB)
    virtual void getParameterName(VstInt32 index, char *text);    // name of the parameter
    virtual void getParameterDisplay(VstInt32 index, char *text); // text description of the current value    
    virtual VstInt32 canDo(char *text);
private:
    char _programName[kVstMaxProgNameLen + 1];
    std::set< std::string > _canDo;
    
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
	
	enum {
		biq_freq,
		biq_reso,
		biq_a0,
		biq_a1,
		biq_a2,
		biq_b1,
		biq_b2,
		biq_sL1,
		biq_sL2,
		biq_sR1,
		biq_sR2,
		biq_total
	};
	double biquad[biq_total];
	
	enum {
		pvAL1,
		pvSL1,
		accSL1,
		acc2SL1,
		pvAL2,
		pvSL2,
		accSL2,
		acc2SL2,
		pvAL3,
		pvSL3,
		accSL3,
		pvAL4,
		pvSL4,
		gndavgL,
		outAL,
		gainAL,
		pvAR1,
		pvSR1,
		accSR1,
		acc2SR1,
		pvAR2,
		pvSR2,
		accSR2,
		acc2SR2,
		pvAR3,
		pvSR3,
		accSR3,
		pvAR4,
		pvSR4,
		gndavgR,
		outAR,
		gainAR,
		air_total
	};
	double air[air_total];
	
	enum {
		prevSampL1,
		prevSlewL1,
		accSlewL1,
		prevSampL2,
		prevSlewL2,
		accSlewL2,
		prevSampL3,
		prevSlewL3,
		accSlewL3,
		kalGainL,
		kalOutL,
		prevSampR1,
		prevSlewR1,
		accSlewR1,
		prevSampR2,
		prevSlewR2,
		accSlewR2,
		prevSampR3,
		prevSlewR3,
		accSlewR3,
		kalGainR,
		kalOutR,
		kal_total
	};
	double kal[kal_total];
	
	double mpkL[2005];
	double mpkR[2005];
	double f[66];
	double prevfreqMPeak;
	double prevamountMPeak;
	int mpc;
	
	double dBaL[dscBuf+5];
	double dBaR[dscBuf+5];
	double dBaPosL;
	double dBaPosR;
	int dBaXL;
	int dBaXR;
	
	double trebleGainA;
	double trebleGainB;
	double midGainA;
	double midGainB;
	double mPeakA;
	double mPeakB;
	double bassGainA;
	double bassGainB;
	double panA;
	double panB;
	double inTrimA;
	double inTrimB;
	
    float A;
    float B;
    float C;
    float D;
    float E;
    float F;
    float G;
    float H;
};

#endif
