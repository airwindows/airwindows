/* ========================================
 *  ConsoleXChannel - ConsoleXChannel.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleXChannel_H
#define __ConsoleXChannel_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParamHIP = 0,
	kParamLOP = 1,
	kParamAIR = 2,
	kParamFIR = 3,
	kParamSTO = 4,
	kParamRNG = 5,
	kParamFCT = 6,
	kParamSCT = 7,
	kParamFCR = 8,
	kParamSCR = 9,
	kParamFCA = 10,
	kParamSCA = 11,
	kParamFCL = 12,
	kParamSCL = 13,
	kParamFGT = 14,
	kParamSGT = 15,
	kParamFGR = 16,
	kParamSGR = 17,
	kParamFGS = 18,
	kParamSGS = 19,
	kParamFGL = 20,
	kParamSGL = 21,	
	kParamTRF = 22,
	kParamTRG = 23,
	kParamTRR = 24,
	kParamHMF = 25,
	kParamHMG = 26,
	kParamHMR = 27,
	kParamLMF = 28,
	kParamLMG = 29,
	kParamLMR = 30,
	kParamBSF = 31,
	kParamBSG = 32,
	kParamBSR = 33,
	kParamDSC = 34,
	kParamPAN = 35,
	kParamFAD = 36,
	kNumParameters = 37
}; //
const int dscBuf = 90;

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'cnxc';    //Change this to what the AU identity is!

class ConsoleXChannel : 
    public AudioEffectX 
{
public:
    ConsoleXChannel(audioMasterCallback audioMaster);
    ~ConsoleXChannel();
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
    
	float HIP;
	float LOP;
	float AIR;
	float FIR;
	float STO;
	float RNG;
	float FCT;
	float SCT;
	float FCR;
	float SCR;
	float FCA;
	float SCA;
	float FCL;
	float SCL;
	float FGT;
	float SGT;
	float FGR;
	float SGR;
	float FGS;
	float SGS;
	float FGL;
	float SGL;	
	float TRF;
	float TRG;
	float TRR;
	float HMF;
	float HMG;
	float HMR;
	float LMF;
	float LMG;
	float LMR;
	float BSF;
	float BSG;
	float BSR;
	float DSC;
	float PAN;
	float FAD;
	
	enum {
		hilp_freq, hilp_temp,
		hilp_a0, hilp_a1, hilp_b1, hilp_b2,
		hilp_c0, hilp_c1, hilp_d1, hilp_d2,
		hilp_e0, hilp_e1, hilp_f1, hilp_f2,
		hilp_aL1, hilp_aL2, hilp_aR1, hilp_aR2,
		hilp_cL1, hilp_cL2, hilp_cR1, hilp_cR2,
		hilp_eL1, hilp_eL2, hilp_eR1, hilp_eR2,
		hilp_total
	};
	double highpass[hilp_total];
	double lowpass[hilp_total];
	
	enum {
		pvAL1, pvSL1, accSL1, acc2SL1,
		pvAL2, pvSL2, accSL2, acc2SL2,
		pvAL3, pvSL3, accSL3,
		pvAL4, pvSL4,
		gndavgL, outAL, gainAL,
		pvAR1, pvSR1, accSR1, acc2SR1,
		pvAR2, pvSR2, accSR2, acc2SR2,
		pvAR3, pvSR3, accSR3,
		pvAR4, pvSR4,
		gndavgR, outAR, gainAR,
		air_total
	};
	double air[air_total];
	
	enum {
		prevSampL1, prevSlewL1, accSlewL1,
		prevSampL2, prevSlewL2, accSlewL2,
		prevSampL3, prevSlewL3, accSlewL3,
		kalGainL, kalOutL,
		prevSampR1, prevSlewR1, accSlewR1,
		prevSampR2, prevSlewR2, accSlewR2,
		prevSampR3, prevSlewR3, accSlewR3,
		kalGainR, kalOutR,
		kal_total
	};
	double kal[kal_total];
	double fireCompL;
	double fireCompR;
	double fireGate;
	double stoneCompL;
	double stoneCompR;
	double stoneGate;	
	double airGainA;
	double airGainB;
	double fireGainA;
	double fireGainB;
	double stoneGainA;
	double stoneGainB;
	
	enum { 
		biqs_freq, biqs_reso, biqs_level,
		biqs_nonlin, biqs_temp, biqs_dis,
		biqs_a0, biqs_a1, biqs_b1, biqs_b2,
		biqs_c0, biqs_c1, biqs_d1, biqs_d2,
		biqs_e0, biqs_e1, biqs_f1, biqs_f2,
		biqs_aL1, biqs_aL2, biqs_aR1, biqs_aR2,
		biqs_cL1, biqs_cL2, biqs_cR1, biqs_cR2,
		biqs_eL1, biqs_eL2, biqs_eR1, biqs_eR2,
		biqs_outL, biqs_outR, biqs_total
	};
	double high[biqs_total];
	double hmid[biqs_total];
	double lmid[biqs_total];
	double bass[biqs_total];
	
	double dBaL[dscBuf+5];
	double dBaR[dscBuf+5];
	double dBaPosL;
	double dBaPosR;
	int dBaXL;
	int dBaXR;
	
	double panA;
	double panB;
	double inTrimA;
	double inTrimB;
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
