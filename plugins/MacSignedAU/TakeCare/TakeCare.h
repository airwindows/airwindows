/*
*	File:		TakeCare.h
*	
*	Version:	1.0
* 
*	Created:	7/29/25
*	
*	Copyright:  Copyright © 2025 Airwindows, Airwindows uses the MIT license
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
#include "TakeCareVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __TakeCare_h__
#define __TakeCare_h__


#pragma mark ____TakeCare Parameters

// parameters
static const float kDefaultValue_ParamA = 0.15;
static const float kDefaultValue_ParamB = 0.5;
static const float kDefaultValue_ParamC = 0.5;
static const float kDefaultValue_ParamD = 0.5;
static const float kDefaultValue_ParamE = 0.5;
static const float kDefaultValue_ParamF = 0.5;
static const float kDefaultValue_ParamG = 1.0;
static const float kDefaultValue_ParamH = 1.0;

static CFStringRef kParameterAName = CFSTR("Speed");
static CFStringRef kParameterBName = CFSTR("Rando");
static CFStringRef kParameterCName = CFSTR("Depth");
static CFStringRef kParameterDName = CFSTR("Regen");
static CFStringRef kParameterEName = CFSTR("Derez");
static CFStringRef kParameterFName = CFSTR("Buffer");
static CFStringRef kParameterGName = CFSTR("Output");
static CFStringRef kParameterHName = CFSTR("Dry/Wet");

enum {
	kParam_A =0,
	kParam_B =1,
	kParam_C =2,
	kParam_D =3,
	kParam_E =4,
	kParam_F =5,
	kParam_G =6,
	kParam_H =7,
	//Add your parameters here...
	kNumberOfParameters=8
};

#pragma mark ____TakeCare
class TakeCare : public AUEffectBase
{
public:
	TakeCare(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~TakeCare () { delete mDebugDispatcher; }
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
	virtual ComponentResult		Version() { return kTakeCareVersion; }
	
	private: 
	double a3AL[32767+5];
	double a3BL[32767+5];
	double a3CL[32767+5];
	double a3DL[32767+5];
	double a3EL[32767+5];
	double a3FL[32767+5];
	double a3GL[32767+5];
	double a3HL[32767+5];
	double a3IL[32767+5];
	double a3AR[32767+5];
	double a3BR[32767+5];
	double a3CR[32767+5];
	double a3DR[32767+5];
	double a3ER[32767+5];
	double a3FR[32767+5];
	double a3GR[32767+5];
	double a3HR[32767+5];
	double a3IR[32767+5];
	int c3AL,c3AR,c3BL,c3BR,c3CL,c3CR,c3DL,c3DR,c3EL,c3ER;
	int c3FL,c3FR,c3GL,c3GR,c3HL,c3HR,c3IL,c3IR;
	double f3AL,f3BL,f3CL,f3CR,f3FR,f3IR;	
	double avg3L,avg3R;
	
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
	
	double rotate;
	double oldfpd;
	
	int buf;
	double vibDepth;
	double derezA;
	double derezB;
	double outA;
	double outB;
	double wetA;
	double wetB;
	
	double lastSampleL;
	bool wasPosClipL;
	bool wasNegClipL;
	double lastSampleR;
	bool wasPosClipR;
	bool wasNegClipR; //Stereo ClipOnly
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif