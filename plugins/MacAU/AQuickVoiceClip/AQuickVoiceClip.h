/*
*	File:		AQuickVoiceClip.h
*	
*	Version:	1.0
* 
*	Created:	10/3/13
*	
*	Copyright:  Copyright © 2013 Airwindows, Airwindows uses the MIT license
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
#include "AQuickVoiceClipVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __AQuickVoiceClip_h__
#define __AQuickVoiceClip_h__


#pragma mark ____AQuickVoiceClip Parameters

// parameters
static const float kDefaultValue_ParamOne = 186.0;

static CFStringRef kParameterOneName = CFSTR("Highpass");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	//Add your parameters here...
	kNumberOfParameters=1
};

#pragma mark ____AQuickVoiceClip
class AQuickVoiceClip : public AUEffectBase
{
public:
	AQuickVoiceClip(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~AQuickVoiceClip () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new AQuickVoiceClipKernel(this); }
	
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
	virtual ComponentResult		Version() { return kAQuickVoiceClipVersion; }
	
    
	
protected:
		class AQuickVoiceClipKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		AQuickVoiceClipKernel(AUEffectBase *inAudioUnit )
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
		Float64 ataLast6Sample;
		Float64 ataLast5Sample;
		Float64 ataLast4Sample;
		Float64 ataLast3Sample;
		Float64 ataLast2Sample;
		Float64 ataLast1Sample;
		Float64 ataHalfwaySample;
		Float64 ataHalfDrySample;
		Float64 ataHalfDiffSample;
		Float64 ataLastDiffSample;
		Float64 ataDrySample;
		Float64 ataDiffSample;
		Float64 ataPrevDiffSample;
		Float64 ataK1;
		Float64 ataK2;
		Float64 ataK3;
		Float64 ataK4;
		Float64 ataK5;
		Float64 ataK6;
		Float64 ataK7;
		Float64 ataK8; //end antialiasing variables
		Float64 lastSample;
		Float64 lastOutSample;
		Float64 lastOut2Sample;
		Float64 lastOut3Sample;
		Float64 lpDepth;
		Float64 overshoot;
		Float64 overall;
		Float64 iirSampleA;
		Float64 iirSampleB;
		Float64 iirSampleC;
		Float64 iirSampleD;
		bool flip;
		Float64 fpNShape;
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif