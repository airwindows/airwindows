/*
*	File:		ToTape7Mono.h
*	
*	Version:	1.0
* 
*	Created:	5/9/25
*	
*	Copyright:  Copyright � 2025 Airwindows, Airwindows uses the MIT license
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
#include "ToTape7MonoVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __ToTape7Mono_h__
#define __ToTape7Mono_h__


#pragma mark ____ToTape7Mono Parameters

// parameters
static const float kDefaultValue_ParamA = 0.5;
static const float kDefaultValue_ParamB = 0.5;
static const float kDefaultValue_ParamC = 0.5;
static const float kDefaultValue_ParamD = 0.5;
static const float kDefaultValue_ParamE = 0.5;
static const float kDefaultValue_ParamF = 0.5;
static const float kDefaultValue_ParamG = 0.5;
static const float kDefaultValue_ParamH = 50.0;
static const float kDefaultValue_ParamI = 0.5;
static const float kDefaultValue_ParamJ = 0.5;

static CFStringRef kParameterAName = CFSTR("EncAmt");
static CFStringRef kParameterBName = CFSTR("EncFreq");
static CFStringRef kParameterCName = CFSTR("TapeDrv");
static CFStringRef kParameterDName = CFSTR("Flutter");
static CFStringRef kParameterEName = CFSTR("FlutSpd");
static CFStringRef kParameterFName = CFSTR("Bias");
static CFStringRef kParameterGName = CFSTR("HeadBmp");
static CFStringRef kParameterHName = CFSTR("HeadFrq");
static CFStringRef kParameterHUnit = CFSTR("hz");
static CFStringRef kParameterIName = CFSTR("DecAmt");
static CFStringRef kParameterJName = CFSTR("DecFreq");

enum {
	kParam_A =0,
	kParam_B =1,
	kParam_C =2,
	kParam_D =3,
	kParam_E =4,
	kParam_F =5,
	kParam_G =6,
	kParam_H =7,
	kParam_I =8,
	kParam_J =9,
	//Add your parameters here...
	kNumberOfParameters=10
};

#pragma mark ____ToTape7Mono
class ToTape7Mono : public AUEffectBase
{
public:
	ToTape7Mono(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~ToTape7Mono () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new ToTape7MonoKernel(this); }
	
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
	virtual ComponentResult		Version() { return kToTape7MonoVersion; }
	
    
	
protected:
		class ToTape7MonoKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		ToTape7MonoKernel(AUEffectBase *inAudioUnit )
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
		
		double iirEncL;
		
		double dL[1002];
		double sweepL;
		double nextmaxL;
		int gcount;		
		
		enum {
			prevSampL1,
			prevSampR1,
			threshold1,
			prevSampL2,
			prevSampR2,
			threshold2,
			prevSampL3,
			prevSampR3,
			threshold3,
			prevSampL4,
			prevSampR4,
			threshold4,
			prevSampL5,
			prevSampR5,
			threshold5,
			prevSampL6,
			prevSampR6,
			threshold6,
			prevSampL7,
			prevSampR7,
			threshold7,
			prevSampL8,
			prevSampR8,
			threshold8,
			prevSampL9,
			prevSampR9,
			threshold9,
			gslew_total
		}; //fixed frequency pear filter for ultrasonics, stereo
		double gslew[gslew_total]; //end bias	
		
		double iirMidRollerL;
		double iirLowCutoffL;
		
		double headBumpL;
		enum {
			hdb_freq,
			hdb_reso,
			hdb_a0,
			hdb_a1,
			hdb_a2,
			hdb_b1,
			hdb_b2,
			hdb_sL1,
			hdb_sL2,
			hdb_total
		}; //fixed frequency biquad filter for ultrasonics, stereo
		double hdbA[hdb_total];
		double hdbB[hdb_total];
		
		double iirDecL;
		
		double lastSampleL;
		double intermediateL[16];
		bool wasPosClipL;
		bool wasNegClipL;
		
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif