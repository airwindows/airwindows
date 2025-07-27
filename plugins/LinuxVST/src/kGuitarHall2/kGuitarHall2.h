/* ========================================
 *  kGuitarHall2 - kGuitarHall2.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __kGuitarHall2_H
#define __kGuitarHall2_H

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

const int d3A = 1015; const int d3B = 1288; const int d3C = 1396;
const int d3D = 784; const int d3E = 1188; const int d3F = 1258;
const int d3G = 575; const int d3H = 771; const int d3I = 1257;
#define THREEBYTHREE true
const int d6A = 121; const int d6B = 1396; const int d6C = 784; const int d6D = 184; const int d6E = 132; const int d6F = 434; const int d6G = 91; const int d6H = 144; const int d6I = 155; const int d6J = 771; const int d6K = 122; const int d6L = 1288; const int d6M = 191; const int d6N = 224; const int d6O = 108; const int d6P = 35; const int d6Q = 1257; const int d6R = 575; const int d6S = 504; const int d6T = 264; const int d6U = 198; const int d6V = 14; const int d6W = 246; const int d6X = 81; const int d6Y = 17; const int d6ZA = 71; const int d6ZB = 1015; const int d6ZC = 1258; const int d6ZD = 24; const int d6ZE = 7; const int d6ZF = 255; const int d6ZG = 72; const int d6ZH = 68; const int d6ZI = 117; const int d6ZJ = 1188; const int d6ZK = 339; //6 to 156 ms, 784 seat theater  
#define SIXBYSIX true //784-EFCCFE-JLBY6 kGuitarHall2

const int early[] = {7, 14, 17, 24, 35, 68, 71, 72, 81, 91, 108, 117, 121, 122, 132, 144, 155, 184, 191, 198, 224, 246, 255, 264, 339, 434, 504, 575, 771, 784, 1015, 1188, 1257, 1258, 1288, 1396};

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'kgti';    //Change this to what the AU identity is!

class kGuitarHall2 : 
    public AudioEffectX 
{
public:
    kGuitarHall2(audioMasterCallback audioMaster);
    ~kGuitarHall2();
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
		bez_cycle,
		bez_total
	}; //the new undersampling. bez signifies the bezier curve reconstruction
	double bez[bez_total];
	
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
		pear_total
	}; //fixed frequency pear filter for ultrasonics, stereo
	double pear[pear_total]; //probably worth just using a number here
		
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
