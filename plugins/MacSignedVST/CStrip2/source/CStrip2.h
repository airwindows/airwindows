/* ========================================
 *  CStrip2 - CStrip2.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __CStrip2_H
#define __CStrip2_H

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
	kParamI = 8,
	kParamJ = 9,
	kNumParameters = 10
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'cst2';    //Change this to what the AU identity is!

class CStrip2 : 
    public AudioEffectX 
{
public:
    CStrip2(audioMasterCallback audioMaster);
    ~CStrip2();
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
	
	double lastSampleL;
	double last2SampleL;
	double lastSampleR;
	double last2SampleR;
	
	//begin Capacitor
	double iirHighpassAL;
	double iirHighpassBL;
	double iirHighpassCL;
	double iirHighpassDL;
	double iirHighpassEL;
	double iirHighpassFL;
	double iirLowpassAL;
	double iirLowpassBL;
	double iirLowpassCL;
	double iirLowpassDL;
	double iirLowpassEL;
	double iirLowpassFL;
	
	double iirHighpassAR;
	double iirHighpassBR;
	double iirHighpassCR;
	double iirHighpassDR;
	double iirHighpassER;
	double iirHighpassFR;
	double iirLowpassAR;
	double iirLowpassBR;
	double iirLowpassCR;
	double iirLowpassDR;
	double iirLowpassER;
	double iirLowpassFR;	
	int count;
	//end Capacitor
	
	//begin EQ
	double iirHighSampleLA;
	double iirHighSampleLB;
	double iirHighSampleLC;
	double iirHighSampleLD;
	double iirHighSampleLE;
	double iirLowSampleLA;
	double iirLowSampleLB;
	double iirLowSampleLC;
	double iirLowSampleLD;
	double iirLowSampleLE;
	double iirHighSampleL;
	double iirLowSampleL;
	
	double iirHighSampleRA;
	double iirHighSampleRB;
	double iirHighSampleRC;
	double iirHighSampleRD;
	double iirHighSampleRE;
	double iirLowSampleRA;
	double iirLowSampleRB;
	double iirLowSampleRC;
	double iirLowSampleRD;
	double iirLowSampleRE;
	double iirHighSampleR;
	double iirLowSampleR;
	
	double tripletLA;
	double tripletLB;
	double tripletLC;
	double tripletFactorL;
	
	double tripletRA;
	double tripletRB;
	double tripletRC;
	double tripletFactorR;
		
	bool flip;
	int flipthree;
	//end EQ
		
	//begin ButterComp
	double controlAposL;
	double controlAnegL;
	double controlBposL;
	double controlBnegL;
	double targetposL;
	double targetnegL;
	double avgLA;
	double avgLB;
	double nvgLA;
	double nvgLB;
	
	double controlAposR;
	double controlAnegR;
	double controlBposR;
	double controlBnegR;
	double targetposR;
	double targetnegR;
	double avgRA;
	double avgRB;
	double nvgRA;
	double nvgRB;
	//end ButterComp
	//flip is already covered in EQ
	
    float A;
    float B;
    float C;
    float D;
    float E;
    float F;
    float G;
    float H;
    float I;
    float J;
	
};

#endif
