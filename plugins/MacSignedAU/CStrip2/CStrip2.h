/*
*	File:		CStrip2.h
*	
*	Version:	1.0
* 
*	Created:	1/20/23
*	
*	Copyright:  Copyright © 2023 Airwindows, Airwindows uses the MIT license
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
#include "CStrip2Version.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __CStrip2_h__
#define __CStrip2_h__


#pragma mark ____CStrip2 Parameters

// parameters
static const float kDefaultValue_ParamOne = 0.0;
static const float kDefaultValue_ParamTwo = 0.0;
static const float kDefaultValue_ParamThree = 0.0;
static const float kDefaultValue_ParamFour = 6.0;
static const float kDefaultValue_ParamFive = 100.0;
static const float kDefaultValue_ParamSix = 1.0;
static const float kDefaultValue_ParamSeven = 0.0;
static const float kDefaultValue_ParamEight = 0.0;
static const float kDefaultValue_ParamNine = 0.0;
static const float kDefaultValue_ParamTen = 1.0;

static CFStringRef kParameterOneName = CFSTR("Treble");
static CFStringRef kParameterTwoName = CFSTR("Mid");
static CFStringRef kParameterThreeName = CFSTR("Bass");
static CFStringRef kParameterFourName = CFSTR("Treble Freq");
static CFStringRef kParameterFourUnit = CFSTR("Khz");
static CFStringRef kParameterFiveName = CFSTR("Bass Freq");
static CFStringRef kParameterFiveUnit = CFSTR("hz");
static CFStringRef kParameterSixName = CFSTR("Low Cap");
static CFStringRef kParameterSevenName = CFSTR("High Cap");
static CFStringRef kParameterEightName = CFSTR("Compression");
static CFStringRef kParameterNineName = CFSTR("Comp Speed");
static CFStringRef kParameterTenName = CFSTR("Output");
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

#pragma mark ____CStrip2
class CStrip2 : public AUEffectBase
{
public:
	CStrip2(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~CStrip2 () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new CStrip2Kernel(this); }
	
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
	virtual ComponentResult		Version() { return kCStrip2Version; }
	
    
	
protected:
		class CStrip2Kernel : public AUKernelBase		// most of the real work happens here
	{
public:
		CStrip2Kernel(AUEffectBase *inAudioUnit )
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
		uint32_t fpd;
		
		Float64 lastSample;
		Float64 last2Sample;
		
		//begin Capacitor
		Float64 iirHighpassA;
		Float64 iirHighpassB;
		Float64 iirHighpassC;
		Float64 iirHighpassD;
		Float64 iirHighpassE;
		Float64 iirHighpassF;
		Float64 iirLowpassA;
		Float64 iirLowpassB;
		Float64 iirLowpassC;
		Float64 iirLowpassD;
		Float64 iirLowpassE;
		Float64 iirLowpassF;
		int count;
		//end Capacitor
		
		//begin EQ
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
				
		bool flip;
		int flipthree;
		//end EQ
				
		//begin ButterComp
		Float64 controlApos;
		Float64 controlAneg;
		Float64 controlBpos;
		Float64 controlBneg;
		Float64 targetpos;
		Float64 targetneg;
		Float64 avgA;
		Float64 avgB;
		Float64 nvgA;
		Float64 nvgB;
		//end ButterComp
		//flip is already covered in EQ		
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif