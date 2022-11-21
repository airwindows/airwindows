/*
*	File:		EQ.h
*	
*	Version:	1.0
* 
*	Created:	10/26/12
*	
*	Copyright:  Copyright © 2012 Airwindows, Airwindows uses the MIT license
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
#include "EQVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __EQ_h__
#define __EQ_h__


#pragma mark ____EQ Parameters

// parameters
static const float kDefaultValue_ParamOne = 0.0;
static const float kDefaultValue_ParamTwo = 0.0;
static const float kDefaultValue_ParamThree = 0.0;
static const float kDefaultValue_ParamFour = 16.0;
static const float kDefaultValue_ParamFive = 6.0;
static const float kDefaultValue_ParamSix = 100.0;
static const float kDefaultValue_ParamSeven = 30.0;
static const float kDefaultValue_ParamEight = 0.0;

static CFStringRef kParameterOneName = CFSTR("Treble");
static CFStringRef kParameterTwoName = CFSTR("Mid");
static CFStringRef kParameterThreeName = CFSTR("Bass");
static CFStringRef kParameterFourName = CFSTR("Lowpass");
static CFStringRef kParameterFourUnit = CFSTR("Khz");
static CFStringRef kParameterFiveName = CFSTR("Treble Freq");
static CFStringRef kParameterFiveUnit = CFSTR("Khz");
static CFStringRef kParameterSixName = CFSTR("Bass Freq");
static CFStringRef kParameterSixUnit = CFSTR("hz");
static CFStringRef kParameterSevenName = CFSTR("Highpass");
static CFStringRef kParameterSevenUnit = CFSTR("hz");
static CFStringRef kParameterEightName = CFSTR("Output Gain");
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
	//Add your parameters here...
	kNumberOfParameters=8
};

#pragma mark ____EQ
class EQ : public AUEffectBase
{
public:
	EQ(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~EQ () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new EQKernel(this); }
	
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
	virtual ComponentResult		Version() { return kEQVersion; }
	
    
	
protected:
		class EQKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		EQKernel(AUEffectBase *inAudioUnit )
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
		Float64 iirHighSampleA;
		Float64 iirHighSampleB;
		Float64 iirHighSampleC;
		Float64 iirHighSampleD;
		Float64 iirHighSampleE;
		Float64 iirLowSampleA;
		Float64 iirLowSampleB;
		Float64 iirLowSampleC;
		Float64 iirLowSampleD;
		Float64 iirLowSampleE;
		Float64 iirHighSample;
		Float64 iirLowSample;
		
		Float64 tripletA;
		Float64 tripletB;
		Float64 tripletC;
		Float64 tripletFactor;
		
		Float64 lowpassSampleAA;
		Float64 lowpassSampleAB;
		Float64 lowpassSampleBA;
		Float64 lowpassSampleBB;
		Float64 lowpassSampleCA;
		Float64 lowpassSampleCB;
		Float64 lowpassSampleDA;
		Float64 lowpassSampleDB;
		Float64 lowpassSampleE;
		Float64 lowpassSampleF;
		Float64 lowpassSampleG;
		
		Float64 highpassSampleAA;
		Float64 highpassSampleAB;
		Float64 highpassSampleBA;
		Float64 highpassSampleBB;
		Float64 highpassSampleCA;
		Float64 highpassSampleCB;
		Float64 highpassSampleDA;
		Float64 highpassSampleDB;
		Float64 highpassSampleE;
		Float64 highpassSampleF;
		
		bool flip;
		int flipthree;
		
		Float64 lastSample;
		Float64 last2Sample;

		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif