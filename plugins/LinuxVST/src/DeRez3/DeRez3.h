/* ========================================
 *  DeRez3 - DeRez3.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeRez3_H
#define __DeRez3_H

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
  kNumParameters = 3
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'drzz';    //Change this to what the AU identity is!

class DeRez3 : 
    public AudioEffectX 
{
public:
    DeRez3(audioMasterCallback audioMaster);
    ~DeRez3();
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
	
	
	double rezA;
	double rezB;
	double bitA;
	double bitB;
	double wetA;
	double wetB;
	
	enum {
		bez_AL,
		bez_BL,
		bez_CL,	
		bez_InL,
		bez_UnInL,
		bez_SampL,
		bez_AR,
		bez_BR,
		bez_CR,	
		bez_InR,
		bez_UnInR,
		bez_SampR,
		bez_cycle,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bez[bez_total];
    
    float A;
    float B;
    float C;

	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
