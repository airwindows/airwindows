/*
*	File:		Cabs.h
*	
*	Version:	1.0
* 
*	Created:	3/27/11
*	
*	Copyright:  Copyright © 2011 Airwindows, Airwindows uses the MIT license
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
#include "CabsVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Cabs_h__
#define __Cabs_h__


#pragma mark ____Cabs Parameters

// parameters
static CFStringRef kParameterOneName = CFSTR("Cab Type");
static const int kHighPowerStack = 1;
static const int kVintageStack = 2;
static const int kBoutiqueStack = 3;
static const int kLargeCombo = 4;
static const int kSmallCombo = 5;
static const int kBass = 6;
static const int kDefaultValue_ParamOne = kHighPowerStack;

static CFStringRef kMenuItem_HighPowerStack = CFSTR ("High Power Stack");
static CFStringRef kMenuItem_VintageStack = CFSTR ("Vintage Stack");
static CFStringRef kMenuItem_BoutiqueStack = CFSTR ("Boutique Stack");
static CFStringRef kMenuItem_LargeCombo = CFSTR ("Large Combo");
static CFStringRef kMenuItem_SmallCombo = CFSTR ("Small Combo");
static CFStringRef kMenuItem_Bass = CFSTR ("Bass Amp");

static CFStringRef kParameterTwoName = CFSTR("Intensity");
static const float kDefaultValue_ParamTwo = 0.66;

static CFStringRef kParameterThreeName = CFSTR("Rm Loud");
static const float kDefaultValue_ParamThree = 0.33;

static CFStringRef kParameterFourName = CFSTR("Cab Size");
static const float kDefaultValue_ParamFour = 0.66;


static CFStringRef kParameterFiveName = CFSTR("Off Axis");
static const float kDefaultValue_ParamFive = 0.33;

static CFStringRef kParameterSixName = CFSTR("Output");
static const float kDefaultValue_ParamSix = 0.5;




//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	kParam_Five =4,
	kParam_Six =5,
	//Add your parameters here...
	kNumberOfParameters=6
};

#pragma mark ____Cabs
class Cabs : public AUEffectBase
{
public:
	Cabs(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Cabs () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new CabsKernel(this); }
	
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
    virtual Float64				GetTailTime() {return 1.0/GetSampleRate()*91.0;} //in SECONDS!
    virtual Float64				GetLatency() {return 0.0;}	// in SECONDS!

	/*! @method Version */
	virtual ComponentResult	Version() { return kCabsVersion; }
	
    
	
protected:
		class CabsKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		CabsKernel(AUEffectBase *inAudioUnit )
		: AUKernelBase(inAudioUnit)
	{
	}
		
		// *Required* overides for the process method for this effect
		// processes one channel of interleaved samples
        virtual void 		Process(	const Float32 	*inSourceP,
										Float32		 	*inDestP,
										UInt32 			inFramesToProcess,
										UInt32			inNumChannels,
										bool			&ioSilence);
		
        virtual void		Reset();
		
		private: 
		Float64 ataLast3Sample;
		Float64 ataLast2Sample;
		Float64 ataLast1Sample;
		Float64 ataHalfwaySample;
		Float64 ataHalfDrySample;
		Float64 ataHalfDiffSample;
		Float64 ataA;
		Float64 ataB;
		Float64 ataC;
		Float64 ataDrySample;
		Float64 ataDiffSample;
		Float64 ataPrevDiffSample;
		Float64 b[90];
		Float64 lastSample;
		Float64 lastHalfSample;
		Float64 lastPostSample;
		Float64 lastPostHalfSample;
		Float64 postPostSample;
		Float64 d[21];
		Float64 control;
		Float64 iirHeadBumpA;
		Float64 iirHeadBumpB;
		Float64 iirHalfHeadBumpA;
		Float64 iirHalfHeadBumpB;
		double lastRef[7];
		
		bool flip;
		bool ataFlip;
		int gcount;
		int cycle;
		
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif