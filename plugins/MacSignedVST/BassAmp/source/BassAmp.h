/* ========================================
 *  BassAmp - BassAmp.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BassAmp_H
#define __BassAmp_H

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
const unsigned long kUniqueId = 'basa';    //Change this to what the AU identity is!

class BassAmp : 
    public AudioEffectX 
{
public:
    BassAmp(audioMasterCallback audioMaster);
    ~BassAmp();
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
    
	double LataLast6Sample;
	double LataLast5Sample;
	double LataLast4Sample;
	double LataLast3Sample;
	double LataLast2Sample;
	double LataLast1Sample;
	double LataHalfwaySample;
	double LataHalfDrySample;
	double LataHalfDiffSample;
	double LataLastDiffSample;
	double LataDrySample;
	double LataDiffSample;
	double LataPrevDiffSample;
	
	double LiirDriveSampleA;
	double LiirDriveSampleB;
	double LiirDriveSampleC;
	double LiirDriveSampleD;
	double LiirDriveSampleE;
	double LiirDriveSampleF;
	
	bool LWasNegative;
	bool LSubOctave;
	double LiirHeadBumpA;
	double LiirHeadBumpB;
	double LiirHeadBumpC;
	
	double LiirSubBumpA;
	double LiirSubBumpB;
	double LiirSubBumpC;
	
	double LiirSampleA;
	double LiirSampleB;
	double LiirSampleC;
	double LiirSampleD;
	double LiirSampleE;
	double LiirSampleF;
	double LiirSampleG;
	double LiirSampleH;
	double LiirSampleI;
	double LiirSampleJ;
	double LiirSampleK;
	double LiirSampleL;
	double LiirSampleM;
	double LiirSampleN;
	double LiirSampleO;
	double LiirSampleP;
	double LiirSampleQ;
	double LiirSampleR;
	double LiirSampleS;
	double LiirSampleT;
	double LiirSampleU;
	double LiirSampleV;
	double LiirSampleW;
	double LiirSampleX;
	double LiirSampleY;
	double LiirSampleZ;		
	
	double RataLast6Sample;
	double RataLast5Sample;
	double RataLast4Sample;
	double RataLast3Sample;
	double RataLast2Sample;
	double RataLast1Sample;
	double RataHalfwaySample;
	double RataHalfDrySample;
	double RataHalfDiffSample;
	double RataLastDiffSample;
	double RataDrySample;
	double RataDiffSample;
	double RataPrevDiffSample;
	
	double RiirDriveSampleA;
	double RiirDriveSampleB;
	double RiirDriveSampleC;
	double RiirDriveSampleD;
	double RiirDriveSampleE;
	double RiirDriveSampleF;
	
	bool RWasNegative;
	bool RSubOctave;
	double RiirHeadBumpA;
	double RiirHeadBumpB;
	double RiirHeadBumpC;
	
	double RiirSubBumpA;
	double RiirSubBumpB;
	double RiirSubBumpC;
	
	double RiirSampleA;
	double RiirSampleB;
	double RiirSampleC;
	double RiirSampleD;
	double RiirSampleE;
	double RiirSampleF;
	double RiirSampleG;
	double RiirSampleH;
	double RiirSampleI;
	double RiirSampleJ;
	double RiirSampleK;
	double RiirSampleL;
	double RiirSampleM;
	double RiirSampleN;
	double RiirSampleO;
	double RiirSampleP;
	double RiirSampleQ;
	double RiirSampleR;
	double RiirSampleS;
	double RiirSampleT;
	double RiirSampleU;
	double RiirSampleV;
	double RiirSampleW;
	double RiirSampleX;
	double RiirSampleY;
	double RiirSampleZ;		
	
	double ataK1;
	double ataK2;
	double ataK3;
	double ataK4;
	double ataK5;
	double ataK6;
	double ataK7;
	double ataK8; //end antialiasing variables
	
	bool flip; //drive things	
	int bflip;
	
	uint32_t fpdL;
	uint32_t fpdR;
	//default stuff

    float A;
    float B;
    float C;
    float D;
};

#endif
