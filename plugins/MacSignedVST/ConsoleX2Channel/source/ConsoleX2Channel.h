/* ========================================
 *  ConsoleX2Channel - ConsoleX2Channel.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleX2Channel_H
#define __ConsoleX2Channel_H

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
	kParamK =10,
	kParamL =11,
	kParamM =12,
	kParamN =13,
	kParamO =14,
	kParamP =15,
	kParamQ =16,
	kParamR =17,
  kNumParameters = 18
}; //
const int dscBuf = 256;

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'cx2c';    //Change this to what the AU identity is!

class ConsoleX2Channel : 
    public AudioEffectX 
{
public:
    ConsoleX2Channel(audioMasterCallback audioMaster);
    ~ConsoleX2Channel();
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
    float K;
    float L;
    float M;
    float N;
    float O;
    float P;
    float Q;
    float R;

	enum {
		biq_freq,
		biq_reso,
		biq_a0,
		biq_a1,
		biq_a2,
		biq_b1,
		biq_b2,
		biq_sL1,
		biq_sL2,
		biq_sR1,
		biq_sR2,
		biq_total
	}; //coefficient interpolating filter, stereo
	double highA[biq_total];
	double highB[biq_total];
	double highC[biq_total];
	double highLIIR;
	double highRIIR;
	
	double midA[biq_total];
	double midB[biq_total];
	double midC[biq_total];
	double midLIIR;
	double midRIIR;
	
	double lowA[biq_total];
	double lowB[biq_total];
	double lowC[biq_total];
	double lowLIIR;
	double lowRIIR;
	//SmoothEQ2
	
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
	double bezCompF[bez_total];
	double bezMaxF;
	double bezCompS[bez_total];
	double bezGate;
	//Dynamics2
	
	double iirHPositionL[23];
	double iirHAngleL[23];
	double iirHPositionR[23];
	double iirHAngleR[23];
	bool hBypass;
	double iirLPositionL[15];
	double iirLAngleL[15];
	double iirLPositionR[15];
	double iirLAngleR[15];
	bool lBypass;
	double lFreqA;
	double lFreqB; //the lowpass
	double hFreqA;
	double hFreqB; //the highpass
	//Cabs2
	
	double dBaL[dscBuf+5];
	double dBaPosL;
	int dBaXL;
	double dBaR[dscBuf+5];
	double dBaPosR;
	int dBaXR;
	//Discontapeity
	
	double avg32L[33];
	double avg32R[33];
	double avg16L[17];
	double avg16R[17];
	double avg8L[9];
	double avg8R[9];
	double avg4L[5];
	double avg4R[5];
	double avg2L[3];
	double avg2R[3];
	int avgPos;
	double lastSlewL;
	double lastSlewR;
	double lastSlewpleL;
	double lastSlewpleR;
	//preTapeHack		
	
	double panA;
	double panB;
	double inTrimA;
	double inTrimB;
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
