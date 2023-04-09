/* ========================================
 *  kPlateB - kPlateB.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kPlateB_H
#define __kPlateB_H

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
  kNumParameters = 5
}; //

const int earlyA = 97; const int earlyB = 1123; const int earlyC = 379; const int earlyD = 619; const int earlyE = 1187; const int earlyF = 757; const int earlyG = 1151; const int earlyH = 13; const int earlyI = 1093; const int predelay = 24010; //78 ms, 732 seat theater
const int delayA = 631; const int delayB = 251; const int delayC = 137; const int delayD = 673; const int delayE = 293; const int delayF = 163; const int delayG = 389; const int delayH = 37; const int delayI = 641; const int delayJ = 661; const int delayK = 17; const int delayL = 719; const int delayM = 787; const int delayN = 31; const int delayO = 83; const int delayP = 823; const int delayQ = 127; const int delayR = 653; const int delayS = 3; const int delayT = 613; const int delayU = 857; const int delayV = 431; const int delayW = 53; const int delayX = 607; const int delayY = 193; //86 ms, 883 seat hall. Scarcity, 1 in 34934
//this must be included to set up the delay lines

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'kplb';    //Change this to what the AU identity is!

class kPlateB : 
    public AudioEffectX 
{
public:
    kPlateB(audioMasterCallback audioMaster);
    ~kPlateB();
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
    
	double iirAL;
	double iirBL;
	
	double gainIn;
	double gainOutL;
	double gainOutR;
	
	double eAL[earlyA+5];
	double eBL[earlyB+5];
	double eCL[earlyC+5];
	double eDL[earlyD+5];
	double eEL[earlyE+5];
	double eFL[earlyF+5];
	double eGL[earlyG+5];
	double eHL[earlyH+5];
	double eIL[earlyI+5];
	
	double eAR[earlyA+5];
	double eBR[earlyB+5];
	double eCR[earlyC+5];
	double eDR[earlyD+5];
	double eER[earlyE+5];
	double eFR[earlyF+5];
	double eGR[earlyG+5];
	double eHR[earlyH+5];
	double eIR[earlyI+5];
	
	int earlyAL, earlyAR;
	int earlyBL, earlyBR;
	int earlyCL, earlyCR;
	int earlyDL, earlyDR;
	int earlyEL, earlyER;
	int earlyFL, earlyFR;
	int earlyGL, earlyGR;
	int earlyHL, earlyHR;
	int earlyIL, earlyIR;
	
	double aAL[delayA+5];
	double aBL[delayB+5];
	double aCL[delayC+5];
	double aDL[delayD+5];
	double aEL[delayE+5];
	double aFL[delayF+5];
	double aGL[delayG+5];
	double aHL[delayH+5];
	double aIL[delayI+5];
	double aJL[delayJ+5];
	double aKL[delayK+5];
	double aLL[delayL+5];
	double aML[delayM+5];
	double aNL[delayN+5];
	double aOL[delayO+5];
	double aPL[delayP+5];
	double aQL[delayQ+5];
	double aRL[delayR+5];
	double aSL[delayS+5];
	double aTL[delayT+5];
	double aUL[delayU+5];
	double aVL[delayV+5];
	double aWL[delayW+5];
	double aXL[delayX+5];
	double aYL[delayY+5];
	
	double aAR[delayA+5];
	double aBR[delayB+5];
	double aCR[delayC+5];
	double aDR[delayD+5];
	double aER[delayE+5];
	double aFR[delayF+5];
	double aGR[delayG+5];
	double aHR[delayH+5];
	double aIR[delayI+5];
	double aJR[delayJ+5];
	double aKR[delayK+5];
	double aLR[delayL+5];
	double aMR[delayM+5];
	double aNR[delayN+5];
	double aOR[delayO+5];
	double aPR[delayP+5];
	double aQR[delayQ+5];
	double aRR[delayR+5];
	double aSR[delayS+5];
	double aTR[delayT+5];
	double aUR[delayU+5];
	double aVR[delayV+5];
	double aWR[delayW+5];
	double aXR[delayX+5];
	double aYR[delayY+5];
	
	double aZL[predelay+5];
	double aZR[predelay+5];
	
	
	double feedbackAL;
	double feedbackBL;
	double feedbackCL;
	double feedbackDL;
	double feedbackEL;
	
	double feedbackER;
	double feedbackJR;
	double feedbackOR;
	double feedbackTR;
	double feedbackYR;
	
	double previousAL;
	double previousBL;
	double previousCL;
	double previousDL;
	double previousEL;
	
	double lastRefL[7];
	
	double iirAR;
	double iirBR;
	
	double previousAR;
	double previousBR;
	double previousCR;
	double previousDR;
	double previousER;
	
	double lastRefR[7];
	
	int countAL;
	int countBL;
	int countCL;
	int countDL;
	int countEL;
	int countFL;
	int countGL;
	int countHL;
	int countIL;
	int countJL;
	int countKL;
	int countLL;		
	int countML;		
	int countNL;		
	int countOL;		
	int countPL;		
	int countQL;		
	int countRL;		
	int countSL;		
	int countTL;		
	int countUL;		
	int countVL;		
	int countWL;		
	int countXL;		
	int countYL;		
	
	int countAR;
	int countBR;
	int countCR;
	int countDR;
	int countER;
	int countFR;
	int countGR;
	int countHR;
	int countIR;
	int countJR;
	int countKR;
	int countLR;		
	int countMR;		
	int countNR;		
	int countOR;		
	int countPR;		
	int countQR;		
	int countRR;		
	int countSR;		
	int countTR;		
	int countUR;		
	int countVR;		
	int countWR;		
	int countXR;		
	int countYR;
	
	int countZ;		
	
	int cycle;
	
	enum {
		fix_freq,
		fix_reso,
		fix_a0,
		fix_a1,
		fix_a2,
		fix_b1,
		fix_b2,
		fix_sL1,
		fix_sL2,
		fix_sR1,
		fix_sR2,
		fix_total
	}; //fixed frequency biquad filter for ultrasonics, stereo
	double fixA[fix_total];
	double fixB[fix_total];
	double fixC[fix_total];
	double fixD[fix_total];
	
	double prevMulchBL;
	double prevMulchBR;
	double prevMulchCL;
	double prevMulchCR;
	double prevMulchDL;
	double prevMulchDR;
	double prevMulchEL;
	double prevMulchER;
	
	double prevOutAL;
	double prevOutAR;
	double prevOutBL;
	double prevOutBR;
	double prevOutCL;
	double prevOutCR;
	
	double prevInCL;
	double prevInCR;
	double prevInDL;
	double prevInDR;
	double prevInEL;
	double prevInER;
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
	
    float A;
    float B;
    float C;
    float D;
    float E; //parameters. Always 0-1, and we scale/alter them elsewhere.
	
};

#endif
