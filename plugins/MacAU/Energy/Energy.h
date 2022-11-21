/*
*	File:		Energy.h
*	
*	Version:	1.0
* 
*	Created:	10/2/10
*	
*	Copyright:  Copyright © 2010 Airwindows, Airwindows uses the MIT license
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
#include "EnergyVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Energy_h__
#define __Energy_h__


#pragma mark ____Energy Parameters

// parameters
static const float kDefaultValue_ParamOne = 0.0;
static const float kDefaultValue_ParamTwo = 0.0;
static const float kDefaultValue_ParamThree = 0.0;
static const float kDefaultValue_ParamFour = 0.0;
static const float kDefaultValue_ParamFive = 0.0;
static const float kDefaultValue_ParamSix = 0.0;
static const float kDefaultValue_ParamSeven = 0.0;
static const float kDefaultValue_ParamEight = 0.0;
static const float kDefaultValue_ParamNine = 1.0;
//let's assume we always have a default of 0.0, for no effect
static CFStringRef kParameterOneName = CFSTR("Hiss");
static CFStringRef kParameterTwoName = CFSTR("Glitter");
static CFStringRef kParameterThreeName = CFSTR("Rat");
static CFStringRef kParameterFourName = CFSTR("Fizz");
static CFStringRef kParameterFiveName = CFSTR("Scrape");
static CFStringRef kParameterSixName = CFSTR("Chug");
static CFStringRef kParameterSevenName = CFSTR("Yowr");
static CFStringRef kParameterEightName = CFSTR("Snarl");
static CFStringRef kParameterNineName = CFSTR("InvDryWet");

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
	//Add your parameters here...
	kNumberOfParameters=9
};

#pragma mark ____Energy
class Energy : public AUEffectBase
{
public:
	Energy(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Energy () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new EnergyKernel(this); }
	
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
    virtual Float64				GetTailTime() {return 1.0/GetSampleRate()*64.0;} //in SECONDS!
    virtual Float64				GetLatency() {return 0.0;}	// in SECONDS!

	/*! @method Version */
	virtual ComponentResult	Version() { return kEnergyVersion; }
	
    
	
protected:
		class EnergyKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		EnergyKernel(AUEffectBase *inAudioUnit )
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
		Float64 duoEven;
		Float64 duoOdd;
		Float64 duoFactor;
		bool flip;
				
		Float64 tripletA;
		Float64 tripletB;
		Float64 tripletC;
		Float64 tripletFactor;
		int countA;
		
		Float64 quadA;
		Float64 quadB;
		Float64 quadC;
		Float64 quadD;
		Float64 quadFactor;
		int countB;
				
		Float64 quintA;
		Float64 quintB;
		Float64 quintC;
		Float64 quintD;
		Float64 quintE;
		Float64 quintFactor;
		int countC;
		
		Float64 sextA;
		Float64 sextB;
		Float64 sextC;
		Float64 sextD;
		Float64 sextE;
		Float64 sextF;
		Float64 sextFactor;
		int countD;
		
		Float64 septA;
		Float64 septB;
		Float64 septC;
		Float64 septD;
		Float64 septE;
		Float64 septF;
		Float64 septG;
		Float64 septFactor;
		int countE;
		
		Float64 octA;
		Float64 octB;
		Float64 octC;
		Float64 octD;
		Float64 octE;
		Float64 octF;
		Float64 octG;
		Float64 octH;
		Float64 octFactor;
		int countF;
		
		Float64 nintA;
		Float64 nintB;
		Float64 nintC;
		Float64 nintD;
		Float64 nintE;
		Float64 nintF;
		Float64 nintG;
		Float64 nintH;
		Float64 nintI;
		Float64 nintFactor;
		int countG;
		
		Float64 PrevH;
		Float64 PrevG;
		Float64 PrevF;
		Float64 PrevE;
		Float64 PrevD;
		Float64 PrevC;
		Float64 PrevB;
		Float64 PrevA;

		uint32_t fpd;

	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif