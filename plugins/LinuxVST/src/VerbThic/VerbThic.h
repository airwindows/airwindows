/* ========================================
 *  VerbThic - VerbThic.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) Airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VerbThic_H
#define __VerbThic_H

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

const int kNumPrograms = 0;
const int kNumInputs = 2;
const int kNumOutputs = 2;
const unsigned long kUniqueId = 'vthc';    //Change this to what the AU identity is!

const int d4A = 1439; const int d4B = 3; const int d4C = 259; const int d4D = 578;
const int d4E = 1562; const int d4F = 1152; const int d4G = 189; const int d4H = 3;
const int d4I = 9; const int d4J = 101; const int d4K = 34; const int d4L = 638;
const int d4M = 719; const int d4N = 1154; const int d4O = 38; const int d4P = 530; //1 to 108 ms, 358 seat club  
#define FOURBYFOUR true // 358seat14393259x4 on 2025-11-09 VerbThic

class VerbThic : 
    public AudioEffectX 
{
public:
    VerbThic(audioMasterCallback audioMaster);
    ~VerbThic();
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
	//base stereo reverb
	double b4AL[d4A+5];
	double b4BL[d4B+5];
	double b4CL[d4C+5];
	double b4DL[d4D+5];
	double b4EL[d4E+5];
	double b4FL[d4F+5];
	double b4GL[d4G+5];
	double b4HL[d4H+5];
	double b4IL[d4I+5];
	double b4JL[d4J+5];
	double b4KL[d4K+5];
	double b4LL[d4L+5];
	double b4ML[d4M+5];
	double b4NL[d4N+5];
	double b4OL[d4O+5];
	double b4PL[d4P+5];
	double b4AR[d4A+5];
	double b4BR[d4B+5];
	double b4CR[d4C+5];
	double b4DR[d4D+5];
	double b4ER[d4E+5];
	double b4FR[d4F+5];
	double b4GR[d4G+5];
	double b4HR[d4H+5];
	double b4IR[d4I+5];
	double b4JR[d4J+5];
	double b4KR[d4K+5];
	double b4LR[d4L+5];
	double b4MR[d4M+5];
	double b4NR[d4N+5];
	double b4OR[d4O+5];
	double b4PR[d4P+5];
	double g4AL,g4BL,g4CL,g4DL,g4DR,g4HR,g4LR,g4PR;
	//changed letter is the dual mono, with rearranged grid
	
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
	
	double bezF[bez_total];
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff
};

#endif
