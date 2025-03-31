/* ========================================
 *  AngleEQ - AngleEQ.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AngleEQ_H
#define __AngleEQ_H

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
const unsigned long kUniqueId = 'ange';    //Change this to what the AU identity is!

class AngleEQ : 
    public AudioEffectX 
{
public:
    AngleEQ(audioMasterCallback audioMaster);
    ~AngleEQ();
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

	double iirHPositionL[37];
	double iirHAngleL[37];
	double iirMHPositionL[37];
	double iirMHAngleL[37];
	double iirMLPositionL[37];
	double iirMLAngleL[37];
	double iirLPositionL[37];
	double iirLAngleL[37];
	
	double iirHPositionR[37];
	double iirHAngleR[37];
	double iirMHPositionR[37];
	double iirMHAngleR[37];
	double iirMLPositionR[37];
	double iirMLAngleR[37];
	double iirLPositionR[37];
	double iirLAngleR[37];
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
