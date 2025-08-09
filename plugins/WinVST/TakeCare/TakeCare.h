/* ========================================
 *  TakeCare - TakeCare.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TakeCare_H
#define __TakeCare_H

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
const unsigned long kUniqueId = 'tcar';    //Change this to what the AU identity is!

class TakeCare : 
    public AudioEffectX 
{
public:
    TakeCare(audioMasterCallback audioMaster);
    ~TakeCare();
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

	double a3AL[32767+5];
	double a3BL[32767+5];
	double a3CL[32767+5];
	double a3DL[32767+5];
	double a3EL[32767+5];
	double a3FL[32767+5];
	double a3GL[32767+5];
	double a3HL[32767+5];
	double a3IL[32767+5];
	double a3AR[32767+5];
	double a3BR[32767+5];
	double a3CR[32767+5];
	double a3DR[32767+5];
	double a3ER[32767+5];
	double a3FR[32767+5];
	double a3GR[32767+5];
	double a3HR[32767+5];
	double a3IR[32767+5];
	int c3AL,c3AR,c3BL,c3BR,c3CL,c3CR,c3DL,c3DR,c3EL,c3ER;
	int c3FL,c3FR,c3GL,c3GR,c3HL,c3HR,c3IL,c3IR;
	double f3AL,f3BL,f3CL,f3CR,f3FR,f3IR;	
	double avg3L,avg3R;
	
	enum {
		bez_AL,
		bez_AR,
		bez_BL,
		bez_BR,
		bez_CL,
		bez_CR,	
		bez_InL,
		bez_InR,
		bez_UnInL,
		bez_UnInR,
		bez_SampL,
		bez_SampR,
		bez_AvgInSampL,
		bez_AvgInSampR,
		bez_AvgOutSampL,
		bez_AvgOutSampR,
		bez_cycle,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bez[bez_total];
	
	double rotate;
	double oldfpd;
	
	int buf;
	double vibDepth;
	double derezA;
	double derezB;
	double outA;
	double outB;
	double wetA;
	double wetB;
	
	double lastSampleL;
	bool wasPosClipL;
	bool wasNegClipL;
	double lastSampleR;
	bool wasPosClipR;
	bool wasNegClipR; //Stereo ClipOnly
		
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
