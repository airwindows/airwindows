/* ========================================
 *  Reverb - Reverb.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Reverb_H
#define __Reverb_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
	kParamA = 0,
	kParamB = 1,
  kNumParameters = 2
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'revb';    //Change this to what the AU identity is!

class Reverb : 
    public AudioEffectX 
{
public:
    Reverb(audioMasterCallback audioMaster);
    ~Reverb();
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
    
	double biquadA[11];
	double biquadB[11];
	double biquadC[11];
	
	double aAL[8111];
	double aBL[7511];
	double aCL[7311];
	double aDL[6911];		
	double aEL[6311];
	double aFL[6111];
	double aGL[5511];
	double aHL[4911];
	double aIL[4511];
	double aJL[4311];
	double aKL[3911];
	double aLL[3311];
	double aML[3111];
	
	double aAR[8111];
	double aBR[7511];
	double aCR[7311];
	double aDR[6911];		
	double aER[6311];
	double aFR[6111];
	double aGR[5511];
	double aHR[4911];
	double aIR[4511];
	double aJR[4311];
	double aKR[3911];
	double aLR[3311];
	double aMR[3111];
	
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
	
	double feedbackAL, vibAL, depthA;
	double feedbackBL, vibBL, depthB;
	double feedbackCL, vibCL, depthC;
	double feedbackDL, vibDL, depthD;
	double feedbackEL, vibEL, depthE;
	double feedbackFL, vibFL, depthF;
	double feedbackGL, vibGL, depthG;
	double feedbackHL, vibHL, depthH;
	
	double feedbackAR, vibAR;
	double feedbackBR, vibBR;
	double feedbackCR, vibCR;
	double feedbackDR, vibDR;
	double feedbackER, vibER;
	double feedbackFR, vibFR;
	double feedbackGR, vibGR;
	double feedbackHR, vibHR;
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
	
    float A;
    float B;
};

#endif
