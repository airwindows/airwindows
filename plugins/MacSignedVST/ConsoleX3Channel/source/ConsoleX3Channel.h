/* ========================================
 *  ConsoleX3Channel - ConsoleX3Channel.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleX3Channel_H
#define __ConsoleX3Channel_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParamSMO =0,
	kParamTRM =1,
	
	kParamHIG =2,
	kParamHMG =3,
	kParamLMG =4,
	kParamBSG =5,
	kParamHIF =6,
	kParamHMF =7,
	kParamLMF =8,
	kParamBSF =9,
	
	kParamTHR =10,
	kParamATK =11,
	kParamRLS =12,
	kParamRAT =13,
	kParamMOR =14,
	
	kParamLOP =15,
	kParamLPQ =16,
	kParamHIP =17,
	kParamHPQ =18,
	
	kParamPAN =19,
	kParamFAD =20,
	kNumParameters=21
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'cx3c';    //Change this to what the AU identity is!

class ConsoleX3Channel : 
    public AudioEffectX 
{
public:
    ConsoleX3Channel(audioMasterCallback audioMaster);
    ~ConsoleX3Channel();
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
	float TRM;
	float HIG;
	float HMG;
	float LMG;
	float BSG;
	float HIF;
	float HMF;
	float LMF;
	float BSF;
	float THR;
	float ATK;
	float RLS;
	float RAT;
	float MOR;
	float LOP;
	float LPQ;
	float HIP;
	float HPQ;
	float PAN;
	float FAD;
	
	enum {
		bezierHigh_A,bezierHigh_B,bezierHigh_C,bezierHigh,
		bezierHmid_A,bezierHmid_B,bezierHmid_C,bezierHmid,
		bezierLmid_A,bezierLmid_B,bezierLmid_C,bezierLmid,
		bezierBass_A,bezierBass_B,bezierBass_C,bezierBass,
		bezierHighQ_A,bezierHighQ_B,bezierHighQ_C,bezierHighQ,
		bezierMidQ_A,bezierMidQ_B,bezierMidQ_C,bezierMidQ,
		bezierLowQ_A,bezierLowQ_B,bezierLowQ_C,bezierLowQ,
		bezierThresh_A,bezierThresh_B,bezierThresh_C,bezierThresh,
		bezierRatio_A,bezierRatio_B,bezierRatio_C,bezierRatio,
		bezierMore_A,bezierMore_B,bezierMore_C,bezierMore,
		bezierLFreq_A,bezierLFreq_B,bezierLFreq_C,bezierLFreq,
		bezierHFreq_A,bezierHFreq_B,bezierHFreq_C,bezierHFreq,
		bezierGainL_A,bezierGainL_B,bezierGainL_C,bezierGainL,
		bezierGainR_A,bezierGainR_B,bezierGainR_C,bezierGainR,
		bezier_cycle,
		bezier_total
	}; //control smoothing as basic use of bezier curves
	double bezier[bezier_total];
	
	enum {
		bez_HMAL, bez_HMBL, bez_HMCL, bez_HMDL,
		bez_MAL, bez_MBL, bez_MCL, bez_MDL,
		bez_LMAL, bez_LMBL, bez_LMCL, bez_LMDL,
		bez_HMAR, bez_HMBR, bez_HMCR, bez_HMDR, bez_HMcycle,
		bez_MAR, bez_MBR, bez_MCR, bez_MDR, bez_Mcycle,
		bez_LMAR, bez_LMBR, bez_LMCR, bez_LMDR, bez_LMcycle,
		bez_EQtotal
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bezEQ[bez_EQtotal][3];
	
	enum {
		bez_A,
		bez_B,
		bez_C,
		bez_Ctrl,
		bez_cycle,
		bez_min,
		bez_comp,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bezComp[bez_total];
	//Dynamics3
	
	double iirHPositionL[29];
	double iirHAngleL[29];
	double iirHPositionR[29];
	double iirHAngleR[29];
	bool hBypass;
	double iirLPositionL[29];
	double iirLAngleL[29];
	double iirLPositionR[29];
	double iirLAngleR[29];
	bool lBypass;
	//Cabs2
	
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
	double lastDarkL;
	double lastDarkR;
	int avgPos;
	//preTapeHack
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
