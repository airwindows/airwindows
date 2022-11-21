/* ========================================
 *  Acceleration2 - Acceleration2.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Acceleration2_H
#define __Acceleration2_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParamA = 0,
	kParamB = 1,
  kNumParameters = 2
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'acem';    //Change this to what the AU identity is!

class Acceleration2 : 
    public AudioEffectX 
{
public:
    Acceleration2(audioMasterCallback audioMaster);
    ~Acceleration2();
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
    
	
	double lastSampleL;
	double sL[34];
	double m1L;
	double m2L;
	
	double lastSampleR;
	double sR[34];
	double m1R;
	double m2R;
	
	double biquadA[11];
	double biquadB[11]; //note that this stereo form doesn't require L and R forms!
	//This is because so much of it is coefficients etc. that are the same on both channels.
	//So the stored samples are in 7-8 and 9-10, and freq/res/coefficients serve both.
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
    float B;
};

#endif
