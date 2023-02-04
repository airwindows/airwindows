/* ========================================
 *  Verbity2 - Verbity2.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Verbity2_H
#define __Verbity2_H

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

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'vbt2';    //Change this to what the AU identity is!

class Verbity2 : 
    public AudioEffectX 
{
public:
    Verbity2(audioMasterCallback audioMaster);
    ~Verbity2();
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
	
	//double aZL[5191];
	
	double aAL[5005]; //AFKPU
	double aFL[4953];
	double aKL[4921];
	double aPL[4801];
	double aUL[4753];
	
	double aBL[4351]; //BGLQV
	double aGL[4159];
	double aLL[3931];
	double aQL[3531];
	double aVL[3331];
	
	double aCL[3325]; //CHMRW
	double aHL[2793];
	double aML[2769];
	double aRL[2391];
	double aWL[2349];
	
	double aDL[2143]; //DINSX
	double aIL[1813];
	double aNL[1735];
	double aSL[1173];
	double aXL[789];
	
	double aEL[679]; //EJOTY
	double aJL[645];
	double aOL[441];
	double aTL[351];
	double aYL[283];
	
	double feedbackAL;
	double feedbackBL;
	double feedbackCL;
	double feedbackDL;
	double feedbackEL;
	
	double previousAL;
	double previousBL;
	double previousCL;
	double previousDL;
	double previousEL;
	
	double lastRefL[7];
	
	double iirAR;
	double iirBR;
	
	//double aZR[5191];
	
	double aAR[5005]; //AFKPU
	double aFR[4953];
	double aKR[4921];
	double aPR[4801];
	double aUR[4753];
	
	double aBR[4351]; //BGLQV
	double aGR[4159];
	double aLR[3931];
	double aQR[3531];
	double aVR[3331];
	
	double aCR[3325]; //CHMRW
	double aHR[2793];
	double aMR[2769];
	double aRR[2391];
	double aWR[2349];
	
	double aDR[2143]; //DINSX
	double aIR[1813];
	double aNR[1735];
	double aSR[1173];
	double aXR[789];
	
	double aER[679]; //EJOTY
	double aJR[645];
	double aOR[441];
	double aTR[351];
	double aYR[283];
	
	double feedbackAR;
	double feedbackBR;
	double feedbackCR;
	double feedbackDR;
	double feedbackER;
	
	double previousAR;
	double previousBR;
	double previousCR;
	double previousDR;
	double previousER;
	
	double lastRefR[7];
	
	int countA, delayA;
	int countB, delayB;
	int countC, delayC;
	int countD, delayD;
	int countE, delayE;
	int countF, delayF;
	int countG, delayG;
	int countH, delayH;
	int countI, delayI;
	int countJ, delayJ;
	int countK, delayK;
	int countL, delayL;		
	int countM, delayM;		
	int countN, delayN;		
	int countO, delayO;		
	int countP, delayP;		
	int countQ, delayQ;		
	int countR, delayR;		
	int countS, delayS;		
	int countT, delayT;		
	int countU, delayU;		
	int countV, delayV;		
	int countW, delayW;		
	int countX, delayX;		
	int countY, delayY;		
	int countZ, delayZ;		
	int cycle;
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
    float B;
    float C;
    float D;
};

#endif
