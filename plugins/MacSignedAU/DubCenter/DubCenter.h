/*
*	File:		DubCenter.h
*	
*	Version:	1.0
* 
*	Created:	10/14/18
*	
*	Copyright:  Copyright © 2018 Airwindows, Airwindows uses the MIT license
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
#include "DubCenterVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __DubCenter_h__
#define __DubCenter_h__


#pragma mark ____DubCenter Parameters

// parameters
static const float kDefaultValue_ParamOne = 0.9;
static const float kDefaultValue_ParamTwo = 0.33;
static const float kDefaultValue_ParamThree = 0.74;
static const float kDefaultValue_ParamFour = 1.0;
static const float kDefaultValue_ParamFive = 0.95;
static const float kDefaultValue_ParamSix = 0.49;
static const float kDefaultValue_ParamSeven = 0.2;
static const float kDefaultValue_ParamEight = 0.2;
static const float kDefaultValue_ParamNine = 0.0;
static const float kDefaultValue_ParamTen = 1.0;

static CFStringRef kParameterOneName = CFSTR("Treble Grind");
static CFStringRef kParameterTwoName = CFSTR("Grind Inv/Out");
static CFStringRef kParameterThreeName = CFSTR("Crossover");
static CFStringRef kParameterFourName = CFSTR("Bass Drive");
static CFStringRef kParameterFiveName = CFSTR("Bass Voicing");
static CFStringRef kParameterSixName = CFSTR("Bass Inv/Out");
static CFStringRef kParameterSevenName = CFSTR("Sub Drive");
static CFStringRef kParameterEightName = CFSTR("Sub Voicing");
static CFStringRef kParameterNineName = CFSTR("Sub Inv/Out");
static CFStringRef kParameterTenName = CFSTR("Dry/Wet");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	kParam_Five =4,
	kParam_Six =5,
	kParam_Seven =6,
	kParam_Eight =7,
	kParam_Nine =8,
	kParam_Ten =9,
	//Add your parameters here...
	kNumberOfParameters=10
};

#pragma mark ____DubCenter
class DubCenter : public AUEffectBase
{
public:
	DubCenter(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~DubCenter () { delete mDebugDispatcher; }
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
	virtual ComponentResult		Version() { return kDubCenterVersion; }
	
		private: 
	Float64 iirDriveSampleAL;
	Float64 iirDriveSampleBL;
	Float64 iirDriveSampleCL;
	Float64 iirDriveSampleDL;
	Float64 iirDriveSampleEL;
	Float64 iirDriveSampleFL;
	Float64 iirDriveSampleAR;
	Float64 iirDriveSampleBR;
	Float64 iirDriveSampleCR;
	Float64 iirDriveSampleDR;
	Float64 iirDriveSampleER;
	Float64 iirDriveSampleFR;
	bool flip; //drive things
	
	int bflip;
	bool WasNegative;
	bool SubOctave;
	Float64 iirHeadBumpA;
	Float64 iirHeadBumpB;
	Float64 iirHeadBumpC;
	
	Float64 iirSubBumpA;
	Float64 iirSubBumpB;
	Float64 iirSubBumpC;
	
	Float64 lastHeadBump;
	Float64 lastSubBump;
	
	Float64 iirSampleA;
	Float64 iirSampleB;
	Float64 iirSampleC;
	Float64 iirSampleD;
	Float64 iirSampleE;
	Float64 iirSampleF;
	Float64 iirSampleG;
	Float64 iirSampleH;
	Float64 iirSampleI;
	Float64 iirSampleJ;
	Float64 iirSampleK;
	Float64 iirSampleL;
	Float64 iirSampleM;
	Float64 iirSampleN;
	Float64 iirSampleO;
	Float64 iirSampleP;
	Float64 iirSampleQ;
	Float64 iirSampleR;
	Float64 iirSampleS;
	Float64 iirSampleT;
	Float64 iirSampleU;
	Float64 iirSampleV;
	Float64 iirSampleW;
	Float64 iirSampleX;
	Float64 iirSampleY;
	Float64 iirSampleZ;		
	Float64 oscGate;
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif