/* ========================================
 *  Echojam - Echojam.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Echojam_H
#define __Echojam_H

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
  kNumParameters = 4
}; //

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'echo';    //Change this to what the AU identity is!

const int d4A = 6654; const int d4B = 7450; const int d4C = 6998; const int d4D = 6650; 
const int d4E = 7136; const int d4F = 7198; const int d4G = 7253; const int d4H = 7221; 
const int d4I = 7355; const int d4J = 4838; const int d4K = 7069; const int d4L = 7175; 
const int d4M = 7307; const int d4N = 6819; const int d4O = 7104; const int d4P = 7443;

class Echojam : 
    public AudioEffectX 
{
public:
    Echojam(audioMasterCallback audioMaster);
    ~Echojam();
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

	double lA[d4A+5];
	double lB[d4B+5];
	double lC[d4C+5];
	double lD[d4D+5];
	double lE[d4E+5];
	double lF[d4F+5];
	double lG[d4G+5];
	double lH[d4H+5];
	double lI[d4I+5];
	double lJ[d4J+5];
	double lK[d4K+5];
	double lL[d4L+5];
	double lM[d4M+5];
	double lN[d4N+5];
	double lO[d4O+5];
	double lP[d4P+5];
	
	double rA[d4A+5];
	double rB[d4B+5];
	double rC[d4C+5];
	double rD[d4D+5];
	double rE[d4E+5];
	double rF[d4F+5];
	double rG[d4G+5];
	double rH[d4H+5];
	double rI[d4I+5];
	double rJ[d4J+5];
	double rK[d4K+5];
	double rL[d4L+5];
	double rM[d4M+5];
	double rN[d4N+5];
	double rO[d4O+5];
	double rP[d4P+5];
	
	int cLA,cLB,cLC,cLD,cLE,cLF,cLG,cLH;
	int cLI,cLJ,cLK,cLL,cLM,cLN,cLO,cLP;
	
	int cRA,cRB,cRC,cRD,cRE,cRF,cRG,cRH;
	int cRI,cRJ,cRK,cRL,cRM,cRN,cRO,cRP;
	
	double fAL,fBL,fCL,fDL;
	double hAL, hBL, hCL, hDL, hEL; //do not allocate in audio code
	
	double fDR,fHR,fLR,fPR;
	double hAR, hBR, hCR, hDR, hER; //do not allocate in audio code
	
	enum {
		bez_AL,
		bez_AR,
		bez_BL,
		bez_BR,
		bez_CL,
		bez_CR,	
		bez_SampL,
		bez_SampR,
		bez_cycle,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bez[bez_total];
	
	int shortA;
	int shortB;
	int shortC;
	int shortD;
	int shortE;
	int shortF;
	int shortG;
	int shortH;
	int shortI;
	int shortJ;
	int shortK;
	int shortL;
	int shortM;
	int shortN;
	int shortO;
	int shortP;
	
	int prevclearcoat;
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
