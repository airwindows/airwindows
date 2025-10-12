/*
*	File:		DeNoise.h
*	
*	Version:	1.0
* 
*	Created:	2/20/25
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
#include "DeNoiseVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __DeNoise_h__
#define __DeNoise_h__


#pragma mark ____DeNoise Parameters

// parameters
static const float kDefaultValue_ParamA = 0.0;
static const float kDefaultValue_ParamB = 0.0;
static const float kDefaultValue_ParamC = 0.0;
static const float kDefaultValue_ParamD = 0.0;
static const float kDefaultValue_ParamE = 0.0;
static const float kDefaultValue_ParamF = 0.0;
static const float kDefaultValue_ParamG = 0.25;
static const float kDefaultValue_ParamH = 1.0;

static CFStringRef kParameterAName = CFSTR("High");
static CFStringRef kParameterBName = CFSTR("HMid");
static CFStringRef kParameterCName = CFSTR("Mid");
static CFStringRef kParameterDName = CFSTR("LMid");
static CFStringRef kParameterEName = CFSTR("Bass");
static CFStringRef kParameterFName = CFSTR("Sub");
static CFStringRef kParameterGName = CFSTR("Speed");
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

#pragma mark ____DeNoise
class DeNoise : public AUEffectBase
{
public:
	DeNoise(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~DeNoise () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new DeNoiseKernel(this); }
	
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
	virtual ComponentResult		Version() { return kDeNoiseVersion; }
	
    
	
protected:
		class DeNoiseKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		DeNoiseKernel(AUEffectBase *inAudioUnit )
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
			prevSampL1,
			prevSlewL1,
			prevSampL2,
			prevSlewL2,
			prevSampL3,
			prevSlewL3,
			prevSampL4,
			prevSlewL4,
			prevSampL5,
			prevSlewL5,
			prevSampL6,
			prevSlewL6,
			prevSampL7,
			prevSlewL7,
			prevSampL8,
			prevSlewL8,
			prevSampL9,
			prevSlewL9,
			pear_max,
			figL,
			gndL,
			slew,
			freq,
			levl,
			pear_total
		}; //new pear filter
		double topLevl;
		double pearA[pear_total];
		double pearB[pear_total];
		double pearC[pear_total];
		double pearD[pear_total];
		double pearE[pear_total];
		
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif