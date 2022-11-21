/* ========================================
 *  StereoEnsemble - StereoEnsemble.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __StereoEnsemble_H
#define __StereoEnsemble_H

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
const unsigned long kUniqueId = 'sten';    //Change this to what the AU identity is!

class StereoEnsemble : 
    public AudioEffectX 
{
public:
    StereoEnsemble(audioMasterCallback audioMaster);
    ~StereoEnsemble();
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
	
	double dA[7491];
	double dB[7533];
	double dC[5789];
	double dD[5747];		
	double dE[4841];
	double dF[4871];
	double dG[3119];
	double dH[3089];
	double dI[2213];
	double dJ[2223];
	double dK[1337];
	double dL[1331];
	
	double levelA;
	double levelB;
	double levelC;
	double levelD;
	double levelE;
	double levelF;
	
	double levelG;
	double levelH;
	double levelI;
	double levelJ;
	double levelK;
	double levelL;
	
	double dryL;
	double dryR;
	
	int oneA, maxdelayA;
	int oneB, maxdelayB;
	int oneC, maxdelayC;
	int oneD, maxdelayD;
	int oneE, maxdelayE;
	int oneF, maxdelayF;
	
	int oneG, maxdelayG;
	int oneH, maxdelayH;
	int oneI, maxdelayI;
	int oneJ, maxdelayJ;
	int oneK, maxdelayK;
	int oneL, maxdelayL;
	
	double lastRefL[7];
	double lastRefR[7];
	int cycle;
	    
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
    float B;
};

#endif
