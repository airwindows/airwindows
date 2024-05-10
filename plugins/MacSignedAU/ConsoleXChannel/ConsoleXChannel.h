/*
*	File:		ConsoleXChannel.h
*	
*	Version:	1.0
* 
*	Created:	2/23/24
*	
*	Copyright:  Copyright © 2024 Airwindows, Airwindows uses the MIT license
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
#include "ConsoleXChannelVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __ConsoleXChannel_h__
#define __ConsoleXChannel_h__


#pragma mark ____ConsoleXChannel Parameters

// parameters
static const float kDefaultValue_ParamHIP = 0.0;
static const float kDefaultValue_ParamLOP = 0.0;
static CFStringRef kParameterHIPName = CFSTR("Highpas");
static CFStringRef kParameterLOPName = CFSTR("Lowpass");
//distributed highpass and lowpass
static const float kDefaultValue_ParamAIR = 0.5;
static const float kDefaultValue_ParamFIR = 0.5;
static const float kDefaultValue_ParamSTO = 0.5;
static const float kDefaultValue_ParamRNG = 0.5;
static const float kDefaultValue_ParamFCT = 1.0;
static const float kDefaultValue_ParamSCT = 1.0;
static const float kDefaultValue_ParamFCR = 1.0;
static const float kDefaultValue_ParamSCR = 1.0;
static const float kDefaultValue_ParamFCA = 0.5;
static const float kDefaultValue_ParamSCA = 0.5;
static const float kDefaultValue_ParamFCL = 0.5;
static const float kDefaultValue_ParamSCL = 0.5;
static const float kDefaultValue_ParamFGT = 0.0;
static const float kDefaultValue_ParamSGT = 0.0;
static const float kDefaultValue_ParamFGR = 1.0;
static const float kDefaultValue_ParamSGR = 1.0;
static const float kDefaultValue_ParamFGS = 0.5;
static const float kDefaultValue_ParamSGS = 0.5;
static const float kDefaultValue_ParamFGL = 0.5;
static const float kDefaultValue_ParamSGL = 0.5;
static CFStringRef kParameterAIRName = CFSTR("Air");
static CFStringRef kParameterFIRName = CFSTR("Fire");
static CFStringRef kParameterSTOName = CFSTR("Stone");
static CFStringRef kParameterRNGName = CFSTR("Range");
static CFStringRef kParameterFCTName = CFSTR("FC Thrs");
static CFStringRef kParameterSCTName = CFSTR("SC Thrs");
static CFStringRef kParameterFCRName = CFSTR("FC Rati");
static CFStringRef kParameterSCRName = CFSTR("SC Rati");
static CFStringRef kParameterFCAName = CFSTR("FC Atk");
static CFStringRef kParameterSCAName = CFSTR("SC Atk");
static CFStringRef kParameterFCLName = CFSTR("FC Rls");
static CFStringRef kParameterSCLName = CFSTR("SC Rls");
static CFStringRef kParameterFGTName = CFSTR("FG Thrs");
static CFStringRef kParameterSGTName = CFSTR("SG Thrs");
static CFStringRef kParameterFGRName = CFSTR("FG Rati");
static CFStringRef kParameterSGRName = CFSTR("SG Rati");
static CFStringRef kParameterFGSName = CFSTR("FG Sust");
static CFStringRef kParameterSGSName = CFSTR("SG Sust");
static CFStringRef kParameterFGLName = CFSTR("FG Rls");
static CFStringRef kParameterSGLName = CFSTR("SG Rls");
//Stonefire Compression and Gate
static const float kDefaultValue_ParamTRF = 0.5;
static const float kDefaultValue_ParamTRG = 0.5;
static const float kDefaultValue_ParamTRR = 0.5;
static const float kDefaultValue_ParamHMF = 0.5;
static const float kDefaultValue_ParamHMG = 0.5;
static const float kDefaultValue_ParamHMR = 0.5;
static const float kDefaultValue_ParamLMF = 0.5;
static const float kDefaultValue_ParamLMG = 0.5;
static const float kDefaultValue_ParamLMR = 0.5;
static const float kDefaultValue_ParamBSF = 0.5;
static const float kDefaultValue_ParamBSG = 0.5;
static const float kDefaultValue_ParamBSR = 0.5;
static CFStringRef kParameterTRFName = CFSTR("Tr Freq");
static CFStringRef kParameterTRGName = CFSTR("Treble");
static CFStringRef kParameterTRRName = CFSTR("Tr Reso");
static CFStringRef kParameterHMFName = CFSTR("HM Freq");
static CFStringRef kParameterHMGName = CFSTR("HighMid");
static CFStringRef kParameterHMRName = CFSTR("HM Reso");
static CFStringRef kParameterLMFName = CFSTR("LM Freq");
static CFStringRef kParameterLMGName = CFSTR("LowMid");
static CFStringRef kParameterLMRName = CFSTR("LM Reso");
static CFStringRef kParameterBSFName = CFSTR("Bs Freq");
static CFStringRef kParameterBSGName = CFSTR("Bass");
static CFStringRef kParameterBSRName = CFSTR("Bs Reso");
//Parametric
static const float kDefaultValue_ParamDSC = 100.0;
static const float kDefaultValue_ParamPAN = 0.5;
static const float kDefaultValue_ParamFAD = 0.5;
static CFStringRef kParameterDSCName = CFSTR("Top dB");
static CFStringRef kParameterPANName = CFSTR("Pan");
static CFStringRef kParameterFADName = CFSTR("Fader");
//Discontinuity, Pan, Fader

enum {
	kParam_HIP = 0,
	kParam_LOP = 1,
	kParam_AIR = 2,
	kParam_FIR = 3,
	kParam_STO = 4,
	kParam_RNG = 5,
	kParam_FCT = 6,
	kParam_SCT = 7,
	kParam_FCR = 8,
	kParam_SCR = 9,
	kParam_FCA = 10,
	kParam_SCA = 11,
	kParam_FCL = 12,
	kParam_SCL = 13,
	kParam_FGT = 14,
	kParam_SGT = 15,
	kParam_FGR = 16,
	kParam_SGR = 17,
	kParam_FGS = 18,
	kParam_SGS = 19,
	kParam_FGL = 20,
	kParam_SGL = 21,	
	kParam_TRF = 22,
	kParam_TRG = 23,
	kParam_TRR = 24,
	kParam_HMF = 25,
	kParam_HMG = 26,
	kParam_HMR = 27,
	kParam_LMF = 28,
	kParam_LMG = 29,
	kParam_LMR = 30,
	kParam_BSF = 31,
	kParam_BSG = 32,
	kParam_BSR = 33,
	kParam_DSC = 34,
	kParam_PAN = 35,
	kParam_FAD = 36,
	//Add your parameters here...
	kNumberOfParameters=37
};
const int dscBuf = 90;


#pragma mark ____ConsoleXChannel
class ConsoleXChannel : public AUEffectBase
{
public:
	ConsoleXChannel(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~ConsoleXChannel () { delete mDebugDispatcher; }
#endif
	
	virtual ComponentResult Reset(AudioUnitScope inScope, AudioUnitElement inElement);

	virtual OSStatus ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags, 
						const AudioBufferList & inBuffer, AudioBufferList & outBuffer, 
						UInt32 inFramesToProcess);
	virtual UInt32 SupportedNumChannels(const AUChannelInfo ** outInfo);

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
	virtual ComponentResult		Version() { return kConsoleXChannelVersion; }
	
	private:
	
	enum {
		hilp_freq, hilp_temp,
		hilp_a0, hilp_a1, hilp_b1, hilp_b2,
		hilp_c0, hilp_c1, hilp_d1, hilp_d2,
		hilp_e0, hilp_e1, hilp_f1, hilp_f2,
		hilp_aL1, hilp_aL2, hilp_aR1, hilp_aR2,
		hilp_cL1, hilp_cL2, hilp_cR1, hilp_cR2,
		hilp_eL1, hilp_eL2, hilp_eR1, hilp_eR2,
		hilp_total
	};
	double highpass[hilp_total];
	double lowpass[hilp_total];
	
	enum {
		pvAL1, pvSL1, accSL1, acc2SL1,
		pvAL2, pvSL2, accSL2, acc2SL2,
		pvAL3, pvSL3, accSL3,
		pvAL4, pvSL4,
		gndavgL, outAL, gainAL,
		pvAR1, pvSR1, accSR1, acc2SR1,
		pvAR2, pvSR2, accSR2, acc2SR2,
		pvAR3, pvSR3, accSR3,
		pvAR4, pvSR4,
		gndavgR, outAR, gainAR,
		air_total
	};
	double air[air_total];
	
	enum {
		prevSampL1, prevSlewL1, accSlewL1,
		prevSampL2, prevSlewL2, accSlewL2,
		prevSampL3, prevSlewL3, accSlewL3,
		kalGainL, kalOutL,
		prevSampR1, prevSlewR1, accSlewR1,
		prevSampR2, prevSlewR2, accSlewR2,
		prevSampR3, prevSlewR3, accSlewR3,
		kalGainR, kalOutR,
		kal_total
	};
	double kal[kal_total];
	double fireCompL;
	double fireCompR;
	double fireGate;
	double stoneCompL;
	double stoneCompR;
	double stoneGate;
	double airGainA;
	double airGainB;
	double fireGainA;
	double fireGainB;
	double stoneGainA;
	double stoneGainB;
	
	enum { 
		biqs_freq, biqs_reso, biqs_level,
		biqs_nonlin, biqs_temp, biqs_dis,
		biqs_a0, biqs_a1, biqs_b1, biqs_b2,
		biqs_c0, biqs_c1, biqs_d1, biqs_d2,
		biqs_e0, biqs_e1, biqs_f1, biqs_f2,
		biqs_aL1, biqs_aL2, biqs_aR1, biqs_aR2,
		biqs_cL1, biqs_cL2, biqs_cR1, biqs_cR2,
		biqs_eL1, biqs_eL2, biqs_eR1, biqs_eR2,
		biqs_outL, biqs_outR, biqs_total
	};
	double high[biqs_total];
	double hmid[biqs_total];
	double lmid[biqs_total];
	double bass[biqs_total];
	
	double dBaL[dscBuf+5];
	double dBaR[dscBuf+5];
	double dBaPosL;
	double dBaPosR;
	int dBaXL;
	int dBaXR;
	
	double panA;
	double panB;
	double inTrimA;
	double inTrimB;
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif