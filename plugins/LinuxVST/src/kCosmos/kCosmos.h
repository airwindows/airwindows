/* ========================================
 *  kCosmos - kCosmos.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kCosmos_H
#define __kCosmos_H

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
  kNumParameters = 6
}; //

const int predelay = 15000;

const int delayA = 857; const int delayB = 1433; const int delayC = 1597; const int delayD = 1789; const int delayE = 1987; const int delayF = 373; const int delayG = 883; const int delayH = 1471; const int delayI = 1601; const int delayJ = 1973; const int delayK = 191; const int delayL = 397; const int delayM = 941; const int delayN = 1483; const int delayO = 1663; const int delayP = 149; const int delayQ = 227; const int delayR = 593; const int delayS = 1061; const int delayT = 1549; const int delayU = 137; const int delayV = 167; const int delayW = 313; const int delayX = 641; const int delayY = 1153; //38 to 188 ms, 1538 seat hall  
//1538-OGBP 2025-05-05 - kCosmos

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'kcsm';    //Change this to what the AU identity is!

class kCosmos : 
    public AudioEffectX 
{
public:
    kCosmos(audioMasterCallback audioMaster);
    ~kCosmos();
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

	double eAL[delayA+5];
	double eBL[delayB+5];
	double eCL[delayC+5];
	double eDL[delayD+5];
	double eEL[delayE+5];
	double eFL[delayF+5];
	double eGL[delayG+5];
	double eHL[delayH+5];
	double eIL[delayI+5];
	double eJL[delayJ+5];
	double eKL[delayK+5];
	double eLL[delayL+5];
	double eML[delayM+5];
	double eNL[delayN+5];
	double eOL[delayO+5];
	double ePL[delayP+5];
	double eQL[delayQ+5];
	double eRL[delayR+5];
	double eSL[delayS+5];
	double eTL[delayT+5];
	double eUL[delayU+5];
	double eVL[delayV+5];
	double eWL[delayW+5];
	double eXL[delayX+5];
	double eYL[delayY+5];
	
	double eAR[delayA+5];
	double eBR[delayB+5];
	double eCR[delayC+5];
	double eDR[delayD+5];
	double eER[delayE+5];
	double eFR[delayF+5];
	double eGR[delayG+5];
	double eHR[delayH+5];
	double eIR[delayI+5];
	double eJR[delayJ+5];
	double eKR[delayK+5];
	double eLR[delayL+5];
	double eMR[delayM+5];
	double eNR[delayN+5];
	double eOR[delayO+5];
	double ePR[delayP+5];
	double eQR[delayQ+5];
	double eRR[delayR+5];
	double eSR[delayS+5];
	double eTR[delayT+5];
	double eUR[delayU+5];
	double eVR[delayV+5];
	double eWR[delayW+5];
	double eXR[delayX+5];
	double eYR[delayY+5];
	
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
		bez_cycle,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bez[bez_total];
	
	double firBufferL[32768];
	double firBufferR[32768];
	int firPosition;
	
	double earlyReflectionL;
	double earlyReflectionR;
	
	double prevAL;
	double prevBL;
	double prevCL;
	double prevDL;
	double prevEL;
	double prevER;
	double prevJR;
	double prevOR;
	double prevTR;
	double prevYR;
	
	double derezA, derezB;
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
