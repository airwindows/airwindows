/*
*	File:		ConsoleHPre.h
*	
*	Version:	1.0
* 
*	Created:	11/4/25
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
#include "ConsoleHPreVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __ConsoleHPre_h__
#define __ConsoleHPre_h__


#pragma mark ____ConsoleHPre Parameters

// parameters
static const int kDefaultValue_ParamTRM = 1;
static const float kDefaultValue_ParamMOR = 0.0;

static const float kDefaultValue_ParamHIG = 0.5;
static const float kDefaultValue_ParamMID = 0.5;
static const float kDefaultValue_ParamLOW = 0.5;
static const float kDefaultValue_ParamCRS = 0.0;

static const float kDefaultValue_ParamTRF = 0.5;
static const float kDefaultValue_ParamTRG = 0.0;
static const float kDefaultValue_ParamTRB = 0.5;
static const float kDefaultValue_ParamHMF = 0.5;
static const float kDefaultValue_ParamHMG = 0.0;
static const float kDefaultValue_ParamHMB = 0.5;
static const float kDefaultValue_ParamLMF = 0.5;
static const float kDefaultValue_ParamLMG = 0.0;
static const float kDefaultValue_ParamLMB = 0.5;
static const float kDefaultValue_ParamBSF = 0.5;
static const float kDefaultValue_ParamBSG = 0.0;
static const float kDefaultValue_ParamBSB = 0.5;

static const float kDefaultValue_ParamTHR = 1.0;
static const float kDefaultValue_ParamATK = 0.5;
static const float kDefaultValue_ParamRLS = 0.5;
static const float kDefaultValue_ParamGAT = 0.0;

static const float kDefaultValue_ParamLOP = 1.0;
static const float kDefaultValue_ParamHIP = 0.0;
static const float kDefaultValue_ParamFAD = 0.5;

static CFStringRef kParameterTRMName = CFSTR("Trim");
static CFStringRef kParameterMORName = CFSTR("More");

static CFStringRef kParameterHIGUnit = CFSTR("eq");
static CFStringRef kParameterHIGName = CFSTR("High");
static CFStringRef kParameterMIDName = CFSTR("Mid");
static CFStringRef kParameterLOWName = CFSTR("Low");
static CFStringRef kParameterCRSName = CFSTR("CrossFd");

static CFStringRef kParameterTRFUnit = CFSTR("crush");
static CFStringRef kParameterTRFName = CFSTR("Tr Freq");
static CFStringRef kParameterTRGName = CFSTR("Treble");
static CFStringRef kParameterTRBName = CFSTR("TrCrush");
static CFStringRef kParameterHMFName = CFSTR("HM Freq");
static CFStringRef kParameterHMGName = CFSTR("HighMid");
static CFStringRef kParameterHMBName = CFSTR("HMCrush");
static CFStringRef kParameterLMFName = CFSTR("LM Freq");
static CFStringRef kParameterLMGName = CFSTR("LowMid");
static CFStringRef kParameterLMBName = CFSTR("LMCrush");
static CFStringRef kParameterBSFName = CFSTR("Bs Freq");
static CFStringRef kParameterBSGName = CFSTR("Bass");
static CFStringRef kParameterBSBName = CFSTR("BsCrush");

static CFStringRef kParameterTHRUnit = CFSTR("dyn");
static CFStringRef kParameterTHRName = CFSTR("Thresh");
static CFStringRef kParameterATKName = CFSTR("Attack");
static CFStringRef kParameterRLSName = CFSTR("Release");
static CFStringRef kParameterGATName = CFSTR("Gate");

static CFStringRef kParameterLOPUnit = CFSTR("fltr");
static CFStringRef kParameterLOPName = CFSTR("Lowpass");
static CFStringRef kParameterHIPName = CFSTR("Hipass");
static CFStringRef kParameterFADName = CFSTR("Fader");

enum {
	kParam_TRM =0,
	kParam_MOR =1,
	kParam_HIG =2,
	kParam_MID =3,
	kParam_LOW =4,
	kParam_CRS =5,
	kParam_TRF =6,
	kParam_TRG =7,
	kParam_TRB =8,
	kParam_HMF =9,
	kParam_HMG =10,
	kParam_HMB =11,
	kParam_LMF =12,
	kParam_LMG =13,
	kParam_LMB =14,
	kParam_BSF =15,
	kParam_BSG =16,
	kParam_BSB =17,
	kParam_THR =18,
	kParam_ATK =19,
	kParam_RLS =20,
	kParam_GAT =21,
	kParam_LOP =22,
	kParam_HIP =23,
	kParam_FAD =24,
	//Add your parameters here...
	kNumberOfParameters=25
};

const int dscBuf = 256;

#pragma mark ____ConsoleHPre
class ConsoleHPre : public AUEffectBase
{
public:
	ConsoleHPre(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~ConsoleHPre () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new ConsoleHPreKernel(this); }
	
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
	virtual ComponentResult		Version() { return kConsoleHPreVersion; }
	
    
	
protected:
		class ConsoleHPreKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		ConsoleHPreKernel(AUEffectBase *inAudioUnit )
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
		}; //coefficient interpolating filter, stereo
		double highFast[biq_total];
		double lowFast[biq_total];
		double highFastLIIR;
		double lowFastLIIR;
		//SmoothEQ3
		
		enum { 
			biqs_freq, biqs_reso, biqs_level,
			biqs_temp, biqs_bit,
			biqs_a0, biqs_a1, biqs_b1, biqs_b2,
			biqs_c0, biqs_c1, biqs_d1, biqs_d2,
			biqs_aL1, biqs_aL2,
			biqs_cL1, biqs_cL2,
			biqs_outL, biqs_total
		};
		double high[biqs_total];
		double hmid[biqs_total];
		double lmid[biqs_total];	
		double bass[biqs_total];	
		//HipCrush with four bands
		
		enum {
			bez_A,
			bez_B,
			bez_C,
			bez_Ctrl,
			bez_cycle,
			bez_total
		}; //the new undersampling. bez signifies the bezier curve reconstruction
		double bezComp[bez_total];
		double bezMax;
		double bezMin;
		double bezGate;
		//Dynamics3
		
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
		
		double avg32L[33];
		double avg16L[17];
		double avg8L[9];
		double avg4L[5];
		double avg2L[3];
		double post32L[33];
		double post16L[17];
		double post8L[9];
		double post4L[5];
		double post2L[3];
		int avgPos;
		double lastDarkL;
		//preTapeHack	
		
		double inTrimA;
		double inTrimB;
		
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif