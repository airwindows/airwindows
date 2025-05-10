/* ========================================
 *  ChimeyGuitar - ChimeyGuitar.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ChimeyGuitar_H
#define __ChimeyGuitar_H

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
	kParamI =8,
	kParamJ =9,
	kNumParameters = 10
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'cgtr';    //Change this to what the AU identity is!

class ChimeyGuitar : 
    public AudioEffectX 
{
public:
    ChimeyGuitar(audioMasterCallback audioMaster);
    ~ChimeyGuitar();
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
    float I;
    float J;
	
	double angSL[22][12];
	double angAL[22][12];
	double iirHPositionL[37];
	double iirHAngleL[37];
	double iirBPositionL[37];
	double iirBAngleL[37];
	
	double angSR[22][12];
	double angAR[22][12];
	double iirHPositionR[37];
	double iirHAngleR[37];
	double iirBPositionR[37];
	double iirBAngleR[37];
	
	double angG[12];
	
	enum {
		bez_AL,
		bez_BL,
		bez_CL,
		bez_InL,
		bez_UnInL,
		bez_SampL,
		bez_cycleL,
		bez_AR,
		bez_BR,
		bez_CR,
		bez_InR,
		bez_UnInR,
		bez_SampR,
		bez_cycleR,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bezComp[bez_total][12];
	double bezRezA, bezRezB;	

	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
