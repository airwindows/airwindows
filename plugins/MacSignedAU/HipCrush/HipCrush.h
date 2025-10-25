/*
*	File:		HipCrush.h
*	
*	Version:	1.0
* 
*	Created:	10/23/25
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
#include "HipCrushVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __HipCrush_h__
#define __HipCrush_h__


#pragma mark ____HipCrush Parameters

// parameters
static const float kDefaultValue_ParamTRF = 0.5;
static const float kDefaultValue_ParamTRG = 0.0;
static const float kDefaultValue_ParamTRB = 0.5;
static const float kDefaultValue_ParamHMF = 0.5;
static const float kDefaultValue_ParamHMG = 0.0;
static const float kDefaultValue_ParamHMB = 0.5;
static const float kDefaultValue_ParamLMF = 0.5;
static const float kDefaultValue_ParamLMG = 0.0;
static const float kDefaultValue_ParamLMB = 0.5;
static const float kDefaultValue_ParamDW = 1.0;

static CFStringRef kParameterTRFName = CFSTR("Hi Freq");
static CFStringRef kParameterTRGName = CFSTR("High");
static CFStringRef kParameterTRBName = CFSTR("HiCrush");
static CFStringRef kParameterHMFName = CFSTR("MidFreq");
static CFStringRef kParameterHMGName = CFSTR("Mid");
static CFStringRef kParameterHMBName = CFSTR("MdCrush");
static CFStringRef kParameterLMFName = CFSTR("Lo Freq");
static CFStringRef kParameterLMGName = CFSTR("Low");
static CFStringRef kParameterLMBName = CFSTR("LoCrush");
static CFStringRef kParameterDWName = CFSTR("Dry/Wet");

enum {
	kParam_TRF =0,
	kParam_TRG =1,
	kParam_TRB =2,
	kParam_HMF =3,
	kParam_HMG =4,
	kParam_HMB =5,
	kParam_LMF =6,
	kParam_LMG =7,
	kParam_LMB =8,
	kParam_DW = 9,
	//Add your parameters here...
	kNumberOfParameters=10
};

#pragma mark ____HipCrush
class HipCrush : public AUEffectBase
{
public:
	HipCrush(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~HipCrush () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new HipCrushKernel(this); }
	
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
	virtual ComponentResult		Version() { return kHipCrushVersion; }
	
    
	
protected:
		class HipCrushKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		HipCrushKernel(AUEffectBase *inAudioUnit )
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
		
		enum { 
			biqs_freq, biqs_reso, biqs_level,
			biqs_temp, biqs_bit,
			biqs_a0, biqs_a1, biqs_b1, biqs_b2,
			biqs_c0, biqs_c1, biqs_d1, biqs_d2,
			biqs_aL1, biqs_aL2, biqs_aR1, biqs_aR2,
			biqs_cL1, biqs_cL2, biqs_cR1, biqs_cR2,
			biqs_outL, biqs_outR, biqs_total
		};
		double high[biqs_total];
		double hmid[biqs_total];
		double lmid[biqs_total];
		
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif