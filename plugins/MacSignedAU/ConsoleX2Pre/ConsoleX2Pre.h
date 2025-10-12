/*
*	File:		ConsoleX2Pre.h
*	
*	Version:	1.0
* 
*	Created:	10/7/25
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
#include "ConsoleX2PreVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __ConsoleX2Pre_h__
#define __ConsoleX2Pre_h__


#pragma mark ____ConsoleX2Pre Parameters

// parameters
static const float kDefaultValue_ParamA = 0.5;
static const float kDefaultValue_ParamB = 0.5;
static const float kDefaultValue_ParamC = 0.5;
static const float kDefaultValue_ParamD = 0.5;
static const float kDefaultValue_ParamE = 0.5;
static const float kDefaultValue_ParamF = 0.5;
static const float kDefaultValue_ParamG = 0.5;
static const float kDefaultValue_ParamH = 0.5;
static const float kDefaultValue_ParamI = 1.0;
static const float kDefaultValue_ParamJ = 0.5;
static const float kDefaultValue_ParamK = 0.5;
static const float kDefaultValue_ParamL = 0.0;
static const float kDefaultValue_ParamM = 1.0;
static const float kDefaultValue_ParamN = 0.0;
static const float kDefaultValue_ParamO = 0.0;
static const float kDefaultValue_ParamP = 0.5;

static CFStringRef kParameterAUnit = CFSTR("eq");
static CFStringRef kParameterAName = CFSTR("High");
static CFStringRef kParameterBName = CFSTR("HMid");
static CFStringRef kParameterCName = CFSTR("LMid");
static CFStringRef kParameterDName = CFSTR("Bass");
static CFStringRef kParameterEUnit = CFSTR("freq");
static CFStringRef kParameterEName = CFSTR("HighF");
static CFStringRef kParameterFName = CFSTR("HMidF");
static CFStringRef kParameterGName = CFSTR("LMidF");
static CFStringRef kParameterHName = CFSTR("BassF");
static CFStringRef kParameterIUnit = CFSTR("dyn");
static CFStringRef kParameterIName = CFSTR("Thresh");
static CFStringRef kParameterJName = CFSTR("Attack");
static CFStringRef kParameterKName = CFSTR("Release");
static CFStringRef kParameterLName = CFSTR("Gate");
static CFStringRef kParameterMUnit = CFSTR("fltr");
static CFStringRef kParameterMName = CFSTR("Lowpass");
static CFStringRef kParameterNName = CFSTR("Hipass");
static CFStringRef kParameterOUnit = CFSTR("+");
static CFStringRef kParameterOName = CFSTR("More");
static CFStringRef kParameterPName = CFSTR("Fader");

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
	kParam_K =10,
	kParam_L =11,
	kParam_M =12,
	kParam_N =13,
	kParam_O =14,
	kParam_P =15,
	//Add your parameters here...
	kNumberOfParameters=16
};

const int dscBuf = 256;

#pragma mark ____ConsoleX2Pre
class ConsoleX2Pre : public AUEffectBase
{
public:
	ConsoleX2Pre(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~ConsoleX2Pre () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new ConsoleX2PreKernel(this); }
	
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
	virtual ComponentResult		Version() { return kConsoleX2PreVersion; }
	
    
	
protected:
		class ConsoleX2PreKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		ConsoleX2PreKernel(AUEffectBase *inAudioUnit )
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
			biq_freq,
			biq_reso,
			biq_a0,
			biq_a1,
			biq_a2,
			biq_b1,
			biq_b2,
			biq_sL1,
			biq_sL2,
			biq_total
		}; //coefficient interpolating bessel filter, stereo
		double highA[biq_total];
		double highB[biq_total];
		double highC[biq_total];
		double highIIR;
		
		double midA[biq_total];
		double midB[biq_total];
		double midC[biq_total];
		double midIIR;
		
		double lowA[biq_total];
		double lowB[biq_total];
		double lowC[biq_total];
		double lowIIR;
		//SmoothEQ2
		
		enum {
			bez_AL,
			bez_BL,
			bez_CL,
			bez_InL,
			bez_UnInL,
			bez_SampL,
			bez_cycle,
			bez_total
		}; //the new undersampling. bez signifies the bezier curve reconstruction
		double bezCompF[bez_total];
		double bezMaxF;
		double bezCompS[bez_total];
		double bezGate;
		//Dynamics2
		
		double iirHPosition[23];
		double iirHAngle[23];
		bool hBypass;
		double iirLPosition[15];
		double iirLAngle[15];
		bool lBypass;
		double lFreqA;
		double lFreqB; //the lowpass
		double hFreqA;
		double hFreqB; //the highpass
		//Cabs2
		
		double dBaL[dscBuf+5];
		double dBaPosL;
		int dBaXL;
		//Discontapeity
		
		double inTrimA;
		double inTrimB;
		
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif