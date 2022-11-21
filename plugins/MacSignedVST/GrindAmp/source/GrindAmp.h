/* ========================================
 *  GrindAmp - GrindAmp.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#define __Gain_H

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
  kNumParameters = 4
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'grda';    //Change this to what the AU identity is!

class GrindAmp : 
    public AudioEffectX 
{
public:
    GrindAmp(audioMasterCallback audioMaster);
    ~GrindAmp();
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
    
	double smoothAL;
	double smoothBL;
	double smoothCL;
	double smoothDL;
	double smoothEL;
	double smoothFL;
	double smoothGL;
	double smoothHL;
	double smoothIL;
	double smoothJL;
	double smoothKL;
	double secondAL;
	double secondBL;
	double secondCL;
	double secondDL;
	double secondEL;
	double secondFL;
	double secondGL;
	double secondHL;
	double secondIL;
	double secondJL;
	double secondKL;
	double thirdAL;
	double thirdBL;
	double thirdCL;
	double thirdDL;
	double thirdEL;
	double thirdFL;
	double thirdGL;
	double thirdHL;
	double thirdIL;
	double thirdJL;
	double thirdKL;
	double iirSampleAL;
	double iirSampleBL;
	double iirSampleCL;
	double iirSampleDL;
	double iirSampleEL;
	double iirSampleFL;
	double iirSampleGL;
	double iirSampleHL;
	double iirSampleIL;
	double iirLowpassL;
	double iirSubL;
	double storeSampleL; //amp
	
	double smoothAR;
	double smoothBR;
	double smoothCR;
	double smoothDR;
	double smoothER;
	double smoothFR;
	double smoothGR;
	double smoothHR;
	double smoothIR;
	double smoothJR;
	double smoothKR;
	double secondAR;
	double secondBR;
	double secondCR;
	double secondDR;
	double secondER;
	double secondFR;
	double secondGR;
	double secondHR;
	double secondIR;
	double secondJR;
	double secondKR;
	double thirdAR;
	double thirdBR;
	double thirdCR;
	double thirdDR;
	double thirdER;
	double thirdFR;
	double thirdGR;
	double thirdHR;
	double thirdIR;
	double thirdJR;
	double thirdKR;
	double iirSampleAR;
	double iirSampleBR;
	double iirSampleCR;
	double iirSampleDR;
	double iirSampleER;
	double iirSampleFR;
	double iirSampleGR;
	double iirSampleHR;
	double iirSampleIR;
	double iirLowpassR;
	double iirSubR;
	double storeSampleR; //amp
	
	double bL[90];
	double lastCabSampleL;
	double smoothCabAL;
	double smoothCabBL; //cab
	
	double bR[90];
	double lastCabSampleR;
	double smoothCabAR;
	double smoothCabBR; //cab
	
	double lastRefL[10];
	double lastRefR[10];
	int cycle;	//undersampling
	
	enum {
		fix_freq,
		fix_reso,
		fix_a0,
		fix_a1,
		fix_a2,
		fix_b1,
		fix_b2,
		fix_sL1,
		fix_sL2,
		fix_sR1,
		fix_sR2,
		fix_total
	}; //fixed frequency biquad filter for ultrasonics, stereo
	double fixA[fix_total];
	double fixB[fix_total];
	double fixC[fix_total];
	double fixD[fix_total];
	double fixE[fix_total];
	double fixF[fix_total]; //filtering
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
    float B;
    float C;
    float D;
};

#endif
