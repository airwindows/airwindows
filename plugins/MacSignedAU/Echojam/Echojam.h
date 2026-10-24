/*
*	File:		Echojam.h
*	
*	Version:	1.0
* 
*	Created:	9/22/26
*	
*	Copyright:  Copyright © 2026 Airwindows, Airwindows uses the MIT license
* 
*	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in 
*				consideration of your agreement to the following terms, and your use, installation, modification 
*				or redistribution of this Apple software constitutes acceptance of these terms.  If you do 
*				not agree with these terms, please do not use, install, modify or redistribute this Apple 
*				software.
*
*				In consideration of your agreement to abide by the following terms, and subject to these terms, 
*				Apple grants you a personal, non-exclusive license, under Apple's copyrights in this 
*				original Apple software (the "Apple Software"), to use, reproduce, modify and redistribute the 
*				Apple Software, with or without modifications, in source and/or binary forms; provided that if you 
*				redistribute the Apple Software in its entirety and without modifications, you must retain this 
*				notice and the following text and disclaimers in all such redistributions of the Apple Software. 
*				Neither the name, trademarks, service marks or logos of Apple Computer, Inc. may be used to 
*				endorse or promote products derived from the Apple Software without specific prior written 
*				permission from Apple.  Except as expressly stated in this notice, no other rights or 
*				licenses, express or implied, are granted by Apple herein, including but not limited to any 
*				patent rights that may be infringed by your derivative works or by other works in which the 
*				Apple Software may be incorporated.
*
*				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO WARRANTIES, EXPRESS OR 
*				IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
*				AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE 
*				OR IN COMBINATION WITH YOUR PRODUCTS.
*
*				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
*				DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*				OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
*				REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER 
*				UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN 
*				IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#include "AUEffectBase.h"
#include "EchojamVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Echojam_h__
#define __Echojam_h__


#pragma mark ____Echojam Parameters

// parameters
static const float kDefaultValue_ParamA = 5;
static const float kDefaultValue_ParamB = 0.5;
static const float kDefaultValue_ParamC = 0.5;
static const float kDefaultValue_ParamD = 0.5;

static CFStringRef kParameterAName = CFSTR("Select");
static CFStringRef kParameterBName = CFSTR("Regen");
static CFStringRef kParameterCName = CFSTR("Speed");
static CFStringRef kParameterDName = CFSTR("Dry/Wet");

enum {
	kParam_A =0,
	kParam_B =1,
	kParam_C =2,
	kParam_D =3,
	//Add your parameters here...
	kNumberOfParameters=4
};

const int d4A = 6654; const int d4B = 7450; const int d4C = 6998; const int d4D = 6650; 
const int d4E = 7136; const int d4F = 7198; const int d4G = 7253; const int d4H = 7221; 
const int d4I = 7355; const int d4J = 4838; const int d4K = 7069; const int d4L = 7175; 
const int d4M = 7307; const int d4N = 6819; const int d4O = 7104; const int d4P = 7443;

#pragma mark ____Echojam
class Echojam : public AUEffectBase
{
public:
	Echojam(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Echojam () { delete mDebugDispatcher; }
#endif
	
	virtual ComponentResult Reset(AudioUnitScope inScope, AudioUnitElement inElement);

	virtual OSStatus ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags, 
						const AudioBufferList & inBuffer, AudioBufferList & outBuffer, 
						UInt32 inFramesToProcess);
	virtual UInt32 SupportedNumChannels(const AUChannelInfo ** outInfo);

	virtual	ComponentResult		GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	ComponentResult		GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual ComponentResult		GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual ComponentResult		GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);

	virtual ComponentResult    Initialize();
	virtual bool				SupportsTail () { return true; }
    virtual Float64				GetTailTime() {return (1.0/GetSampleRate())*0.0;} //in SECONDS! gsr * a number = in samples
    virtual Float64				GetLatency() {return (1.0/GetSampleRate())*0.0;}	// in SECONDS! gsr * a number = in samples
	
	/*! @method Version */
	virtual ComponentResult		Version() { return kEchojamVersion; }
	
private: 
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
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif