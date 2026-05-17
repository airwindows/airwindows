/* ========================================
 *  PurestConsole4Channel - PurestConsole4Channel.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestConsole4Channel_H
#define __PurestConsole4Channel_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParamSMO =0,
	kParamPAN =1,
	kParamFAD =2,
  kNumParameters = 3
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'pc4c';    //Change this to what the AU identity is!

class PurestConsole4Channel : 
    public AudioEffectX 
{
public:
    PurestConsole4Channel(audioMasterCallback audioMaster);
    ~PurestConsole4Channel();
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
    
    float SMO;
    float PAN;
    float FAD;

	enum {
		bezierGainL_A,bezierGainL_B,bezierGainL_C,bezierGainL_Out,
		bezierGainR_A,bezierGainR_B,bezierGainR_C,bezierGainR_Out,
		bezier_cycle,
		bezier_total
	}; //control smoothing as basic use of bezier curves
	double bezier[bezier_total];
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
