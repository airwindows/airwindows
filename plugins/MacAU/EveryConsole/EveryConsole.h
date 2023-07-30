/*
*	File:		EveryConsole.h
*	
*	Version:	1.0
* 
*	Created:	7/24/23
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
#include "EveryConsoleVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __EveryConsole_h__
#define __EveryConsole_h__


#pragma mark ____EveryConsole Parameters

// parameters
static CFStringRef kParameterOneName = CFSTR("Console Type");
static const int kRC = 1;
static const int kRB = 2;
static const int kSC = 3;
static const int kSB = 4;
static const int k6C = 5;
static const int k6B = 6;
static const int k7C = 7;
static const int k7B = 8;
static const int kBC = 9;
static const int kBB = 10;
static const int kZC = 11;
static const int kZB = 12;
static const int kDefaultValue_ParamOne = kSC;

static CFStringRef kParameterTwoName = CFSTR("In Trim");
static const float kDefaultValue_ParamTwo = 1.0;

static CFStringRef kParameterThreeName = CFSTR("Out Trim");
static const float kDefaultValue_ParamThree = 1.0;


static CFStringRef kMenuItem_RC = CFSTR ("Retro Channel");
static CFStringRef kMenuItem_RB = CFSTR ("Retro Buss");
static CFStringRef kMenuItem_SC = CFSTR ("Sin() Channel");
static CFStringRef kMenuItem_SB = CFSTR ("aSin() Buss");
static CFStringRef kMenuItem_6C = CFSTR ("C6 Channel");
static CFStringRef kMenuItem_6B = CFSTR ("C6 Buss");
static CFStringRef kMenuItem_7C = CFSTR ("C7 Channel");
static CFStringRef kMenuItem_7B = CFSTR ("C7 Buss");
static CFStringRef kMenuItem_BC = CFSTR ("BShifty Channel");
static CFStringRef kMenuItem_BB = CFSTR ("BShifty Buss");
static CFStringRef kMenuItem_ZC = CFSTR ("CZero Channel");
static CFStringRef kMenuItem_ZB = CFSTR ("CZero Buss");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	//Add your parameters here...
	kNumberOfParameters=3
};

#pragma mark ____EveryConsole
class EveryConsole : public AUEffectBase
{
public:
	EveryConsole(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~EveryConsole () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new EveryConsoleKernel(this); }
	
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
	virtual ComponentResult		Version() { return kEveryConsoleVersion; }
	
    
	
protected:
		class EveryConsoleKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		EveryConsoleKernel(AUEffectBase *inAudioUnit )
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
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif