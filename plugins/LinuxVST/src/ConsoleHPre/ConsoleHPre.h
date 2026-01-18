/* ========================================
 *  ConsoleHPre - ConsoleHPre.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleHPre_H
#define __ConsoleHPre_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParam_TRM =0,
	kParam_MOR =1,
	kParam_HIG =2,
	kParam_MID =3,
	kParam_LOW =4,
	kParam_CRS =5,
	kParam_TRF =6,
	kParam_TRG =7,
	kParam_TRB =8,
	kParam_HMF =9,
	kParam_HMG =10,
	kParam_HMB =11,
	kParam_LMF =12,
	kParam_LMG =13,
	kParam_LMB =14,
	kParam_BSF =15,
	kParam_BSG =16,
	kParam_BSB =17,
	kParam_THR =18,
	kParam_ATK =19,
	kParam_RLS =20,
	kParam_GAT =21,
	kParam_LOP =22,
	kParam_HIP =23,
	kParam_FAD =24,
	//Add your parameters here...
	kNumParameters=25
};

const int dscBuf = 256;

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'chpp';    //Change this to what the AU identity is!

class ConsoleHPre : 
public AudioEffectX 
{
public:
    ConsoleHPre(audioMasterCallback audioMaster);
    ~ConsoleHPre();
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
    
	float TRM;
	float MOR;
	float HIG;
	float MID;
	float LOW;
	float CRS;
	float TRF;
	float TRG;
	float TRB;
	float HMF;
	float HMG;
	float HMB;
	float LMF;
	float LMG;
	float LMB;
	float BSF;
	float BSG;
	float BSB;
	float THR;
	float ATK;
	float RLS;
	float GAT;
	float LOP;
	float HIP;
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
		biqs_freq, biqs_reso, biqs_level,
		biqs_temp, biqs_bit,
		biqs_a0, biqs_a1, biqs_b1, biqs_b2,
		biqs_c0, biqs_c1, biqs_d1, biqs_d2,
		biqs_aL1, biqs_aL2, biqs_aR1, biqs_aR2,
		biqs_cL1, biqs_cL2, biqs_cR1, biqs_cR2,
		biqs_outL, biqs_outR, biqs_total
	};
	double high[biqs_total];
	double hmid[biqs_total];
	double lmid[biqs_total];	
	double bass[biqs_total];	
	//HipCrush with four bands
	
	enum {
		bez_AL,
		bez_BL,
		bez_CL,
		bez_CtrlL,
		bez_AR,
		bez_BR,
		bez_CR,
		bez_CtrlR,
		bez_cycle,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bezComp[bez_total];
	double bezMaxL;
	double bezMinL;
	double bezGateL;
	double bezMaxR;
	double bezMinR;
	double bezGateR;
	//Dynamics3
	
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
	double post32L[33];
	double post32R[33];
	double post16L[17];
	double post16R[17];
	double post8L[9];
	double post8R[9];
	double post4L[5];
	double post4R[5];
	double post2L[3];
	double post2R[3];
	double lastDarkL;
	double lastDarkR;
	int avgPos;
	//preTapeHack
	
	double inTrimA;
	double inTrimB;
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
