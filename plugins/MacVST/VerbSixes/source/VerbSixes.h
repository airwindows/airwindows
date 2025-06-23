/* ========================================
 *  VerbSixes - VerbSixes.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VerbSixes_H
#define __VerbSixes_H

#ifndef __audioeffect__
#include "audioeffectx.h"
#endif

#include <set>
#include <string>
#include <math.h>

enum {
  kNumParameters = 0
}; //

const int d4A = 1060; const int d4B = 3620; const int d4C = 1750; const int d4D = 530; const int d4E = 2930; const int d4F = 4460; const int d4G = 6820; const int d4H = 1090; const int d4I = 6770; const int d4J = 6120; const int d4K = 6110; const int d4L = 5400; const int d4M = 6930; const int d4N = 3650; const int d4O = 2330; const int d4P = 6660; //15 to 60 ms, 171 seat club  
#define FOURBYFOUR true //171*10-GCFCEG4 on 2025-06-11
const int d6A = 729; const int d6B = 1170; const int d6C = 20; const int d6D = 1089; const int d6E = 17; const int d6F = 221; const int d6G = 180; const int d6H = 332; const int d6I = 119; const int d6J = 657; const int d6K = 1129; const int d6L = 1142; const int d6M = 742; const int d6N = 30; const int d6O = 786; const int d6P = 124; const int d6Q = 1107; const int d6R = 85; const int d6S = 1164; const int d6T = 1052; const int d6U = 416; const int d6V = 642; const int d6W = 427; const int d6X = 120; const int d6Y = 891; const int d6ZA = 22; const int d6ZB = 1171; const int d6ZC = 278; const int d6ZD = 476; const int d6ZE = 376; const int d6ZF = 573; const int d6ZG = 1089; const int d6ZH = 487; const int d6ZI = 123; const int d6ZJ = 329; const int d6ZK = 465; //9 to 155 ms, 808 seat hall  
#define SIXBYSIX true //808b-GCCCEG6 on 2025-06-13 - VerbSixes

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'vsix';    //Change this to what the AU identity is!

class VerbSixes : 
    public AudioEffectX 
{
public:
    VerbSixes(audioMasterCallback audioMaster);
    ~VerbSixes();
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
    
#ifdef THREEBYTHREE
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
	double f3AL,f3BL,f3CL,f3CR,f3FR,f3IR;	
	double avg3L,avg3R; 
#endif
#ifdef FOURBYFOUR
	double a4AL[d4A+5];
	double a4BL[d4B+5];
	double a4CL[d4C+5];
	double a4DL[d4D+5];
	double a4EL[d4E+5];
	double a4FL[d4F+5];
	double a4GL[d4G+5];
	double a4HL[d4H+5];
	double a4IL[d4I+5];
	double a4JL[d4J+5];
	double a4KL[d4K+5];
	double a4LL[d4L+5];
	double a4ML[d4M+5];
	double a4NL[d4N+5];
	double a4OL[d4O+5];
	double a4PL[d4P+5];
	double a4AR[d4A+5];
	double a4BR[d4B+5];
	double a4CR[d4C+5];
	double a4DR[d4D+5];
	double a4ER[d4E+5];
	double a4FR[d4F+5];
	double a4GR[d4G+5];
	double a4HR[d4H+5];
	double a4IR[d4I+5];
	double a4JR[d4J+5];
	double a4KR[d4K+5];
	double a4LR[d4L+5];
	double a4MR[d4M+5];
	double a4NR[d4N+5];
	double a4OR[d4O+5];
	double a4PR[d4P+5];
	int c4AL,c4BL,c4CL,c4DL,c4EL,c4FL,c4GL,c4HL;
	int c4IL,c4JL,c4KL,c4LL,c4ML,c4NL,c4OL,c4PL;		
	int c4AR,c4BR,c4CR,c4DR,c4ER,c4FR,c4GR,c4HR;
	int c4IR,c4JR,c4KR,c4LR,c4MR,c4NR,c4OR,c4PR;
	double f4AL,f4BL,f4CL,f4DL,f4DR,f4HR,f4LR,f4PR;
	double avg4L,avg4R; 
#endif
#ifdef FIVEBYFIVE
	double a5AL[d5A+5];
	double a5BL[d5B+5];
	double a5CL[d5C+5];
	double a5DL[d5D+5];
	double a5EL[d5E+5];
	double a5FL[d5F+5];
	double a5GL[d5G+5];
	double a5HL[d5H+5];
	double a5IL[d5I+5];
	double a5JL[d5J+5];
	double a5KL[d5K+5];
	double a5LL[d5L+5];
	double a5ML[d5M+5];
	double a5NL[d5N+5];
	double a5OL[d5O+5];
	double a5PL[d5P+5];
	double a5QL[d5Q+5];
	double a5RL[d5R+5];
	double a5SL[d5S+5];
	double a5TL[d5T+5];
	double a5UL[d5U+5];
	double a5VL[d5V+5];
	double a5WL[d5W+5];
	double a5XL[d5X+5];
	double a5YL[d5Y+5];
	double a5AR[d5A+5];
	double a5BR[d5B+5];
	double a5CR[d5C+5];
	double a5DR[d5D+5];
	double a5ER[d5E+5];
	double a5FR[d5F+5];
	double a5GR[d5G+5];
	double a5HR[d5H+5];
	double a5IR[d5I+5];
	double a5JR[d5J+5];
	double a5KR[d5K+5];
	double a5LR[d5L+5];
	double a5MR[d5M+5];
	double a5NR[d5N+5];
	double a5OR[d5O+5];
	double a5PR[d5P+5];
	double a5QR[d5Q+5];
	double a5RR[d5R+5];
	double a5SR[d5S+5];
	double a5TR[d5T+5];
	double a5UR[d5U+5];
	double a5VR[d5V+5];
	double a5WR[d5W+5];
	double a5XR[d5X+5];
	double a5YR[d5Y+5];
	int c5AL,c5BL,c5CL,c5DL,c5EL,c5FL,c5GL,c5HL;
	int c5IL,c5JL,c5KL,c5LL,c5ML,c5NL,c5OL,c5PL;	
	int c5QL,c5RL,c5SL,c5TL,c5UL,c5VL,c5WL,c5XL,c5YL;		
	int c5AR,c5BR,c5CR,c5DR,c5ER,c5FR,c5GR,c5HR;
	int c5IR,c5JR,c5KR,c5LR,c5MR,c5NR,c5OR,c5PR;	
	int c5QR,c5RR,c5SR,c5TR,c5UR,c5VR,c5WR,c5XR,c5YR;
	double f5AL,f5BL,f5CL,f5DL,f5EL;
	double f5ER,f5JR,f5OR,f5TR,f5YR;
	double avg5L,avg5R; 
#endif
#ifdef SIXBYSIX
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
#endif
	
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
