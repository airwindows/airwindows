/* ========================================
 *  Ultralight - Ultralight.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Ultralight_H
#define __Ultralight_H

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
	kParamG =6,
  kNumParameters = 7
};


const int d4A = 92; const int d4B = 350; const int d4C = 926; const int d4D = 93; const int d4E = 78; const int d4F = 932; const int d4G = 936; const int d4H = 841; const int d4I = 933; const int d4J = 48; const int d4K = 867; const int d4L = 741; const int d4M = 119; const int d4N = 128; const int d4O = 217; const int d4P = 70; //6 to 68 ms, 166 seat club  
// 166seat8D7082 on 2026-09-11

//const int d4Q = 5; const int d4R = 4; const int d4S = 4; const int d4T = 4; const int d4U = 2800;
const int predelay = 4097;

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'ultl';    //Change this to what the AU identity is!

class Ultralight : 
    public AudioEffectX 
{
public:
    Ultralight(audioMasterCallback audioMaster);
    ~Ultralight();
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
    float G;

	
	float lZ[predelay+5];
	float rZ[predelay+5];
	int cZ;
	
	float lA[d4A+5];
	float lB[d4B+5];
	float lC[d4C+5];
	float lD[d4D+5];
	float lE[d4E+5];
	float lF[d4F+5];
	float lG[d4G+5];
	float lH[d4H+5];
	float lI[d4I+5];
	float lJ[d4J+5];
	float lK[d4K+5];
	float lL[d4L+5];
	float lM[d4M+5];
	float lN[d4N+5];
	float lO[d4O+5];
	float lP[d4P+5];
	
	float rA[d4A+5];
	float rB[d4B+5];
	float rC[d4C+5];
	float rD[d4D+5];
	float rE[d4E+5];
	float rF[d4F+5];
	float rG[d4G+5];
	float rH[d4H+5];
	float rI[d4I+5];
	float rJ[d4J+5];
	float rK[d4K+5];
	float rL[d4L+5];
	float rM[d4M+5];
	float rN[d4N+5];
	float rO[d4O+5];
	float rP[d4P+5];
	
	int cA,cB,cC,cD,cE,cF,cG,cH;
	int cI,cJ,cK,cL,cM,cN,cO,cP;
	
	float fAL,fBL,fCL,fDL;
	float fhAL,fhBL,fhCL,fhDL;
	float flAL,flBL,flCL,flDL;
	float hAL, hBL, hCL, hDL, hEL; //do not allocate in audio code
	
	float fAR,fBR,fCR,fDR;
	float fhAR,fhBR,fhCR,fhDR;
	float flAR,flBR,flCR,flDR;
	float hAR, hBR, hCR, hDR, hER; //do not allocate in audio code
	
	float firstDryL,firstAvgL;
	float lsA,lsB,lsC,lsD,lsE,lsF,lsG,lsH,lsI,lsJ,lsK,lsL,lsM;
	float lsN,lsO,lsP,lsQ,lsR,lsS,lsT,lsU,lsV,lsW,lsX,lsY,lsZ;
	float lsa,lsb,lsc,lsd,lse,lsf,lsg,lsh,lsi,lsj,lsk,lsl,lsm;
	float lsn,lso,lsp,lsq,lsr,lss,lst,lsu,lsv,lsw,lsx,lsy,lsz;
	float firstDryR,firstAvgR;
	float rsA,rsB,rsC,rsD,rsE,rsF,rsG,rsH,rsI,rsJ,rsK,rsL,rsM;
	float rsN,rsO,rsP,rsQ,rsR,rsS,rsT,rsU,rsV,rsW,rsX,rsY,rsZ;
	float rsa,rsb,rsc,rsd,rse,rsf,rsg,rsh,rsi,rsj,rsk,rsl,rsm;
	float rsn,rso,rsp,rsq,rsr,rss,rst,rsu,rsv,rsw,rsx,rsy,rsz;
	int prevDistance;
	float pointCycle, pointAL, pointBL, pointAR, pointBR;
	//this is a simple linear interpolation sample rate routine for the plugin version
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
