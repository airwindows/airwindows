/* ========================================
 *  PlatinumSlew - PlatinumSlew.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PlatinumSlew_H
#define __PlatinumSlew_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParamA = 0,
  kNumParameters = 1
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'plsl';    //Change this to what the AU identity is!

class PlatinumSlew : 
    public AudioEffectX 
{
public:
    PlatinumSlew(audioMasterCallback audioMaster);
    ~PlatinumSlew();
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
    
	enum {
		prevSampL1,
		prevSampR1,
		invSampL1,
		invSampR1,
		threshold1,
		prevSampL2,
		prevSampR2,
		invSampL2,
		invSampR2,
		threshold2,
		prevSampL3,
		prevSampR3,
		invSampL3,
		invSampR3,
		threshold3,
		prevSampL4,
		prevSampR4,
		invSampL4,
		invSampR4,
		threshold4,
		prevSampL5,
		prevSampR5,
		invSampL5,
		invSampR5,
		threshold5,
		prevSampL6,
		prevSampR6,
		invSampL6,
		invSampR6,
		threshold6,
		prevSampL7,
		prevSampR7,
		invSampL7,
		invSampR7,
		threshold7,
		prevSampL8,
		prevSampR8,
		invSampL8,
		invSampR8,
		threshold8,
		prevSampL9,
		prevSampR9,
		invSampL9,
		invSampR9,
		threshold9,
		prevSampL10,
		prevSampR10,
		invSampL10,
		invSampR10,
		threshold10,
		gslew_total
	}; //fixed frequency pear filter for ultrasonics, stereo
	double gslew[gslew_total]; //probably worth just using a number here

	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
};

#endif
