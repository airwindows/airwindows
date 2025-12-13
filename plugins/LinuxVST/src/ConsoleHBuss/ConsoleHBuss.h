/* ========================================
 *  ConsoleHBuss - ConsoleHBuss.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleHBuss_H
#define __ConsoleHBuss_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParam_HIG =0,
	kParam_MID =1,
	kParam_LOW =2,
	
	kParam_THR =3,
	
	kParam_LOP =4,
	kParam_HIP =5,
	
	kParam_PAN =6,
	kParam_FAD =7,
	//Add your parameters here...
	kNumParameters=8
};

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'chpb';    //Change this to what the AU identity is!

class ConsoleHBuss : 
public AudioEffectX 
{
public:
    ConsoleHBuss(audioMasterCallback audioMaster);
    ~ConsoleHBuss();
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
    
	float HIG;
	float MID;
	float LOW;
	float THR;
	float LOP;
	float HIP;
	float PAN;
	float FAD;
	
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
	double highFast[biq_total];
	double lowFast[biq_total];
	double highFastLIIR;
	double highFastRIIR;
	double lowFastLIIR;
	double lowFastRIIR;
	//SmoothEQ3
		
	enum {
		bez_A,
		bez_B,
		bez_C,
		bez_Ctrl,
		bez_cycle,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bezCompF[bez_total];
	double bezCompS[bez_total];
	//Dynamics2 custom for buss
	
	enum {
		hilp_freq, hilp_temp,
		hilp_a0, hilp_aA0, hilp_aB0, hilp_a1, hilp_aA1, hilp_aB1, hilp_b1, hilp_bA1, hilp_bB1, hilp_b2, hilp_bA2, hilp_bB2,
		hilp_c0, hilp_cA0, hilp_cB0, hilp_c1, hilp_cA1, hilp_cB1, hilp_d1, hilp_dA1, hilp_dB1, hilp_d2, hilp_dA2, hilp_dB2,
		hilp_e0, hilp_eA0, hilp_eB0, hilp_e1, hilp_eA1, hilp_eB1, hilp_f1, hilp_fA1, hilp_fB1, hilp_f2, hilp_fA2, hilp_fB2,
		hilp_aL1, hilp_aL2, hilp_aR1, hilp_aR2,
		hilp_cL1, hilp_cL2, hilp_cR1, hilp_cR2,
		hilp_eL1, hilp_eL2, hilp_eR1, hilp_eR2,
		hilp_total
	};
	double highpass[hilp_total];
	double lowpass[hilp_total];	
		
	double panA;
	double panB;
	double inTrimA;
	double inTrimB;
	
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
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
