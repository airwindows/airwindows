/* ========================================
 *  kCathedral3 - kCathedral3.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kCathedral3_H
#define __kCathedral3_H

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
  kNumParameters = 5
}; //

const int dscBuf = 90;

const int predelay = 15000; const int vlfpredelay = 11000;

const int delayA = 824; const int delayB = 982; const int delayC = 753; const int delayD = 402; const int delayE = 1206; const int delayF = 1235; const int delayG = 1396; const int delayH = 1330; const int delayI = 248; const int delayJ = 62; const int delayK = 1407; const int delayL = 40; const int delayM = 245; const int delayN = 1088; const int delayO = 775; const int delayP = 13; const int delayQ = 1299; const int delayR = 127; const int delayS = 1497; const int delayT = 386; const int delayU = 11; const int delayV = 971; const int delayW = 1403; const int delayX = 81; const int delayY = 1480; //7 to 162 ms, 860 seat hall  
//860-SLLU rated incompressible if filesize larger than 25,270,485 bytes

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'kctj';    //Change this to what the AU identity is!

class kCathedral3 : 
    public AudioEffectX 
{
public:
    kCathedral3(audioMasterCallback audioMaster);
    ~kCathedral3();
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

	double gainOutL;
 	double gainOutR;
	
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
	
	double aVLFL[vlfpredelay+5];
	double aVLFR[vlfpredelay+5];
	
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
	int countVLF;		
	
	enum {
		prevSampL1,
		prevSlewL1,
		prevSampR1,
		prevSlewR1,
		prevSampL2,
		prevSlewL2,
		prevSampR2,
		prevSlewR2,
		prevSampL3,
		prevSlewL3,
		prevSampR3,
		prevSlewR3,
		prevSampL4,
		prevSlewL4,
		prevSampR4,
		prevSlewR4,
		prevSampL5,
		prevSlewL5,
		prevSampR5,
		prevSlewR5,
		prevSampL6,
		prevSlewL6,
		prevSampR6,
		prevSlewR6,
		prevSampL7,
		prevSlewL7,
		prevSampR7,
		prevSlewR7,
		prevSampL8,
		prevSlewL8,
		prevSampR8,
		prevSlewR8,
		prevSampL9,
		prevSlewL9,
		prevSampR9,
		prevSlewR9,
		prevSampL10,
		prevSlewL10,
		prevSampR10,
		prevSlewR10,
		pear_total
	}; //fixed frequency pear filter for ultrasonics, stereo
	
	double pearA[pear_total]; //probably worth just using a number here
	double pearB[pear_total]; //probably worth just using a number here
	double pearC[pear_total]; //probably worth just using a number here
	double pearD[pear_total]; //probably worth just using a number here
	double pearE[pear_total]; //probably worth just using a number here
	double pearF[pear_total]; //probably worth just using a number here
	
	double vibratoL;
	double vibratoR;
	double vibAL;
	double vibAR;
	double vibBL;
	double vibBR;
	
	double subAL;
	double subAR;
	double subBL;
	double subBR;
	double subCL;
	double subCR;
	
	double sbAL;
	double sbAR;
	double sbBL;
	double sbBR;
	double sbCL;
	double sbCR;
	
	double dBaL[dscBuf+5];
	double dBaPosL;
	int dBaXL;
	
	double dBaR[dscBuf+5];
	double dBaPosR;
	int dBaXR;
	
	double dBbL[dscBuf+5];
	double dBbPosL;
	int dBbXL;
	
	double dBbR[dscBuf+5];
	double dBbPosR;
	int dBbXR;
	
	double dBcL[dscBuf+5];
	double dBcPosL;
	int dBcXL;	
	
	double dBcR[dscBuf+5];
	double dBcPosR;
	int dBcXR;	
	
	double dBdL[dscBuf+5];
	double dBdPosL;
	int dBdXL;	
	
	double dBdR[dscBuf+5];
	double dBdPosR;
	int dBdXR;	
	
	double dBeL[dscBuf+5];
	double dBePosL;
	int dBeXL;	
	
	double dBeR[dscBuf+5];
	double dBePosR;
	int dBeXR;	
	
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
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
