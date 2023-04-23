/* ========================================
 *  Galactic2 - Galactic2.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Galactic2_H
#define __Galactic2_H

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
  kNumParameters = 4
}; //
const int shortA = 683; const int shortB = 2339; const int shortC = 2381; const int shortD = 887; const int shortE = 743; const int shortF = 1823; const int shortG = 1151; const int shortH = 2833; const int shortI = 521; const int shortJ = 3331; const int shortK = 2851; const int shortL = 1747; const int shortM = 3389; const int shortN = 83; const int shortO = 443; const int shortP = 3221; //290 ms, 10004 seat stadium. Scarcity, 1 in 55796
//Galactic2

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'galb';    //Change this to what the AU identity is!

class Galactic2 : 
    public AudioEffectX 
{
public:
    Galactic2(audioMasterCallback audioMaster);
    ~Galactic2();
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
	
	double feedbackA;
	double feedbackB;
	double feedbackC;
	double feedbackD;
	
	double iirA;
	double iirB;
	double iirC;
	double iirD;
	double iirE;
	double iirF;
	double iirG;
	double iirH;
	
	double aAL[shortA+5];
	double aBL[shortB+5];
	double aCL[shortC+5];
	double aDL[shortD+5];
	double aEL[shortE+5];
	double aFL[shortF+5];
	double aGL[shortG+5];
	double aHL[shortH+5];
	double aIL[shortI+5];
	double aJL[shortJ+5];
	double aKL[shortK+5];
	double aLL[shortL+5];
	double aML[shortM+5];
	double aNL[shortN+5];
	double aOL[shortO+5];
	double aPL[shortP+5];
	
	double aAR[shortA+5];
	double aBR[shortB+5];
	double aCR[shortC+5];
	double aDR[shortD+5];
	double aER[shortE+5];
	double aFR[shortF+5];
	double aGR[shortG+5];
	double aHR[shortH+5];
	double aIR[shortI+5];
	double aJR[shortJ+5];
	double aKR[shortK+5];
	double aLR[shortL+5];
	double aMR[shortM+5];
	double aNR[shortN+5];
	double aOR[shortO+5];
	double aPR[shortP+5];
	
	double feedbackAL;
	double feedbackBL;
	double feedbackCL;
	double feedbackDL;
	
	double feedbackDR;
	double feedbackHR;
	double feedbackLR;
	double feedbackPR;
	
	double previousAL;
	double previousBL;
	double previousCL;
	double previousDL;
	double previousEL;
	
	double lastRefL[7];
	
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
	
	int countZ;		
	
	int cycle;
	
	double prevMulchAL;
	double prevMulchAR;
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
	double prevOutDL;
	double prevOutDR;
	double prevOutEL;
	double prevOutER;
	
	double finalOutAL;
	double finalOutAR;
	double finalOutBL;
	double finalOutBR;
	double finalOutCL;
	double finalOutCR;
	double finalOutDL;
	double finalOutDR;
	double finalOutEL;
	double finalOutER;
	
	double prevInAL;
	double prevInAR;
	double prevInBL;
	double prevInBR;
	double prevInCL;
	double prevInCR;
	double prevInDL;
	double prevInDR;
	double prevInEL;
	double prevInER;
	
	double inTrimA;
	double inTrimB;
	double wetA;
	double wetB;	
		
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
    float B;
    float C;
    float D;
};

#endif
