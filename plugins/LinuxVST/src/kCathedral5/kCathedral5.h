/* ========================================
 *  kCathedral5 - kCathedral5.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kCathedral5_H
#define __kCathedral5_H

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

const int d3A = 1683; const int d3B = 2395; const int d3C = 2432;
const int d3D = 1552; const int d3E = 1735; const int d3F = 2392;
const int d3G = 1364; const int d3H = 1468; const int d3I = 1961;
#define THREEBYTHREE true
const int d6A = 2392; const int d6B = 710; const int d6C = 35; const int d6D = 396; const int d6E = 2395; const int d6F = 81; const int d6G = 20; const int d6H = 2432; const int d6I = 1031; const int d6J = 529; const int d6K = 921; const int d6L = 116; const int d6M = 1961; const int d6N = 747; const int d6O = 160; const int d6P = 231; const int d6Q = 820; const int d6R = 493; const int d6S = 188; const int d6T = 412; const int d6U = 1364; const int d6V = 43; const int d6W = 206; const int d6X = 855; const int d6Y = 216; const int d6ZA = 1735; const int d6ZB = 53; const int d6ZC = 1468; const int d6ZD = 1272; const int d6ZE = 1683; const int d6ZF = 1287; const int d6ZG = 8; const int d6ZH = 14; const int d6ZI = 1552; const int d6ZJ = 88; const int d6ZK = 914; //6 to 259 ms, 2094 seat arena  
#define SIXBYSIX true //2094-GIBCDG-VACX6 kCathedral5
const int early[] = {8, 14, 20, 35, 43, 53, 81, 88, 116, 160, 188, 206, 216, 231, 396, 412, 493, 529, 710, 747, 820, 855, 914, 921, 1031, 1272, 1287, 1364, 1468, 1552, 1683, 1735, 1961, 2392, 2395, 2432};

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'kct5';    //Change this to what the AU identity is!

class kCathedral5 : 
    public AudioEffectX 
{
public:
    kCathedral5(audioMasterCallback audioMaster);
    ~kCathedral5();
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
	
	double a3AL[d3A+5];
	double a3BL[d3B+5];
	double a3CL[d3C+5];
	double a3DL[d3D+5];
	double a3EL[d3E+5];
	double a3FL[d3F+5];
	double a3GL[d3G+5];
	double a3HL[d3H+5];
	double a3IL[d3I+5];
	double a3AR[d3A+5];
	double a3BR[d3B+5];
	double a3CR[d3C+5];
	double a3DR[d3D+5];
	double a3ER[d3E+5];
	double a3FR[d3F+5];
	double a3GR[d3G+5];
	double a3HR[d3H+5];
	double a3IR[d3I+5];
	int c3AL,c3AR,c3BL,c3BR,c3CL,c3CR,c3DL,c3DR,c3EL,c3ER;
	int c3FL,c3FR,c3GL,c3GR,c3HL,c3HR,c3IL,c3IR;
	
	double a6AL[d6A+5];
	double a6BL[d6B+5];
	double a6CL[d6C+5];
	double a6DL[d6D+5];
	double a6EL[d6E+5];
	double a6FL[d6F+5];
	double a6GL[d6G+5];
	double a6HL[d6H+5];
	double a6IL[d6I+5];
	double a6JL[d6J+5];
	double a6KL[d6K+5];
	double a6LL[d6L+5];
	double a6ML[d6M+5];
	double a6NL[d6N+5];
	double a6OL[d6O+5];
	double a6PL[d6P+5];
	double a6QL[d6Q+5];
	double a6RL[d6R+5];
	double a6SL[d6S+5];
	double a6TL[d6T+5];
	double a6UL[d6U+5];
	double a6VL[d6V+5];
	double a6WL[d6W+5];
	double a6XL[d6X+5];
	double a6YL[d6Y+5];
	double a6ZAL[d6ZA+5];
	double a6ZBL[d6ZB+5];
	double a6ZCL[d6ZC+5];
	double a6ZDL[d6ZD+5];
	double a6ZEL[d6ZE+5];
	double a6ZFL[d6ZF+5];
	double a6ZGL[d6ZG+5];
	double a6ZHL[d6ZH+5];
	double a6ZIL[d6ZI+5];
	double a6ZJL[d6ZJ+5];
	double a6ZKL[d6ZK+5];
	double a6AR[d6A+5];
	double a6BR[d6B+5];
	double a6CR[d6C+5];
	double a6DR[d6D+5];
	double a6ER[d6E+5];
	double a6FR[d6F+5];
	double a6GR[d6G+5];
	double a6HR[d6H+5];
	double a6IR[d6I+5];
	double a6JR[d6J+5];
	double a6KR[d6K+5];
	double a6LR[d6L+5];
	double a6MR[d6M+5];
	double a6NR[d6N+5];
	double a6OR[d6O+5];
	double a6PR[d6P+5];
	double a6QR[d6Q+5];
	double a6RR[d6R+5];
	double a6SR[d6S+5];
	double a6TR[d6T+5];
	double a6UR[d6U+5];
	double a6VR[d6V+5];
	double a6WR[d6W+5];
	double a6XR[d6X+5];
	double a6YR[d6Y+5];
	double a6ZAR[d6ZA+5];
	double a6ZBR[d6ZB+5];
	double a6ZCR[d6ZC+5];
	double a6ZDR[d6ZD+5];
	double a6ZER[d6ZE+5];
	double a6ZFR[d6ZF+5];
	double a6ZGR[d6ZG+5];
	double a6ZHR[d6ZH+5];
	double a6ZIR[d6ZI+5];
	double a6ZJR[d6ZJ+5];
	double a6ZKR[d6ZK+5];
	int c6AL,c6BL,c6CL,c6DL,c6EL,c6FL,c6GL,c6HL,c6IL;
	int c6JL,c6KL,c6LL,c6ML,c6NL,c6OL,c6PL,c6QL,c6RL;
	int c6SL,c6TL,c6UL,c6VL,c6WL,c6XL,c6YL,c6ZAL,c6ZBL;	
	int c6ZCL,c6ZDL,c6ZEL,c6ZFL,c6ZGL,c6ZHL,c6ZIL,c6ZJL,c6ZKL;		
	int c6AR,c6BR,c6CR,c6DR,c6ER,c6FR,c6GR,c6HR,c6IR;
	int c6JR,c6KR,c6LR,c6MR,c6NR,c6OR,c6PR,c6QR,c6RR;
	int c6SR,c6TR,c6UR,c6VR,c6WR,c6XR,c6YR,c6ZAR,c6ZBR;	
	int c6ZCR,c6ZDR,c6ZER,c6ZFR,c6ZGR,c6ZHR,c6ZIR,c6ZJR,c6ZKR;
	double f6AL,f6BL,f6CL,f6DL,f6EL,f6FL;
	double f6FR,f6LR,f6RR,f6XR,f6ZER,f6ZKR;
	double avg6L,avg6R; 
	
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
		bez_AvgInSampL,
		bez_AvgInSampR,
		bez_AvgOutSampL,
		bez_AvgOutSampR,
		bez_cycle,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bez[bez_total];
	
	double bezF[bez_total];
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
