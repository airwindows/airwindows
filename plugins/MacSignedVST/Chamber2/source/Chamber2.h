/* ========================================
 *  Chamber2 - Chamber2.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Chamber2_H
#define __Chamber2_H

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
const unsigned long kUniqueId = 'chm2';    //Change this to what the AU identity is!

class Chamber2 : 
    public AudioEffectX 
{
public:
    Chamber2(audioMasterCallback audioMaster);
    ~Chamber2();
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
    
	double aEL[10000];
	double aFL[10000];
	double aGL[10000];
	double aHL[10000];
	double aAL[10000];
	double aBL[10000];
	double aCL[10000];
	double aDL[10000];
	double aIL[10000];
	double aJL[10000];
	double aKL[10000];
	double aLL[10000];
	double aML[10000];
	
	double feedbackAL;
	double feedbackBL;
	double feedbackCL;
	double feedbackDL;
	double previousAL;
	double previousBL;
	double previousCL;
	double previousDL;
	
	double lastRefL[10];
		
	double aER[10000];
	double aFR[10000];
	double aGR[10000];
	double aHR[10000];
	double aAR[10000];
	double aBR[10000];
	double aCR[10000];
	double aDR[10000];
	double aIR[10000];
	double aJR[10000];
	double aKR[10000];
	double aLR[10000];
	double aMR[10000];
	
	double feedbackAR;
	double feedbackBR;
	double feedbackCR;
	double feedbackDR;
	double previousAR;
	double previousBR;
	double previousCR;
	double previousDR;
	
	double lastRefR[10];
	
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
	int cycle; //all these ints are shared across channels, not duplicated	
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
	
    float A;
    float B;
    float C;
    float D;	
};

#endif
