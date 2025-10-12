/* ========================================
 *  DeNoise - DeNoise.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeNoise_H
#define __DeNoise_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParamA =0,
	kParamB =1,
	kParamC =2,
	kParamD =3,
	kParamE =4,
	kParamF =5,
	kParamG =6,
	kParamH =7,
  kNumParameters = 8
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'dnoi';    //Change this to what the AU identity is!

class DeNoise : 
    public AudioEffectX 
{
public:
    DeNoise(audioMasterCallback audioMaster);
    ~DeNoise();
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
    
    float A;
    float B;
    float C;
    float D;
    float E;
    float F;
    float G;
    float H;
	
	enum {
		prevSampL1,
		prevSlewL1,
		prevSampL2,
		prevSlewL2,
		prevSampL3,
		prevSlewL3,
		prevSampL4,
		prevSlewL4,
		prevSampL5,
		prevSlewL5,
		prevSampL6,
		prevSlewL6,
		prevSampL7,
		prevSlewL7,
		prevSampL8,
		prevSlewL8,
		prevSampL9,
		prevSlewL9,
		prevSampR1,
		prevSlewR1,
		prevSampR2,
		prevSlewR2,
		prevSampR3,
		prevSlewR3,
		prevSampR4,
		prevSlewR4,
		prevSampR5,
		prevSlewR5,
		prevSampR6,
		prevSlewR6,
		prevSampR7,
		prevSlewR7,
		prevSampR8,
		prevSlewR8,
		prevSampR9,
		prevSlewR9,
		pear_max,
		figL,
		gndL,
		figR,
		gndR,
		slew,
		freq,
		levlL,
		levlR,
		pear_total
	}; //new pear filter
	double topLevlL;
	double topLevlR;
	double pearA[pear_total];
	double pearB[pear_total];
	double pearC[pear_total];
	double pearD[pear_total];
	double pearE[pear_total];

	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
