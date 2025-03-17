/*
*	File:		Mastering2.h
*	
*	Version:	1.0
* 
*	Created:	3/11/25
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
#include "Mastering2Version.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Mastering2_h__
#define __Mastering2_h__


#pragma mark ____Mastering2 Parameters

// parameters
static const float kDefaultValue_ParamA = 0.0;
static const float kDefaultValue_ParamB = 0.0;
static const float kDefaultValue_ParamC = 0.5;
static const float kDefaultValue_ParamD = 0.5;
static const float kDefaultValue_ParamE = 0.5;
static const float kDefaultValue_ParamF = 0.5;
static const int kDark = 1;
static const int kTenNines = 2;
static const int kTPDFWide = 3;
static const int kPaulWide = 4;
static const int kNJAD = 5;
static const int kBypass = 6;
static const int kDefaultValue_ParamG = kBypass;

static CFStringRef kMenuItem_Dark = CFSTR ("Dark");
static CFStringRef kMenuItem_TenNines = CFSTR ("Ten Nines");
static CFStringRef kMenuItem_TPDFWide = CFSTR ("TPDFWide");
static CFStringRef kMenuItem_PaulWide = CFSTR ("PaulWide");
static CFStringRef kMenuItem_NJAD = CFSTR ("NJAD");
static CFStringRef kMenuItem_Bypass = CFSTR ("Bypass");

static CFStringRef kParameterAName = CFSTR("Sidepass");
static CFStringRef kParameterBName = CFSTR("Glue");
static CFStringRef kParameterCName = CFSTR("Scope");
static CFStringRef kParameterDName = CFSTR("Skronk");
static CFStringRef kParameterEName = CFSTR("Girth");
static CFStringRef kParameterFName = CFSTR("Drive");
static CFStringRef kParameterGName = CFSTR("Dither");

enum {
	kParam_A =0,
	kParam_B =1,
	kParam_C =2,
	kParam_D =3,
	kParam_E =4,
	kParam_F =5,
	kParam_G =6,
	//Add your parameters here...
	kNumberOfParameters=7
};

#pragma mark ____Mastering2
class Mastering2 : public AUEffectBase
{
public:
	Mastering2(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Mastering2 () { delete mDebugDispatcher; }
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
	virtual ComponentResult		Version() { return kMastering2Version; }
	
	private:
	
	double iirA;
	double iirB; //first stage is the flipping one, for lowest slope. It is always engaged, and is the highest one
	double iirC; //we introduce the second pole at the same frequency, to become a pseudo-Capacitor behavior
	bool fpFlip;
	
	enum {
		pvAL1,
		pvSL1,
		accSL1,
		acc2SL1,
		pvAL2,
		pvSL2,
		accSL2,
		acc2SL2,
		pvAL3,
		pvSL3,
		accSL3,
		pvAL4,
		pvSL4,
		gndavgL,
		outAL,
		gainAL,
		pvAR1,
		pvSR1,
		accSR1,
		acc2SR1,
		pvAR2,
		pvSR2,
		accSR2,
		acc2SR2,
		pvAR3,
		pvSR3,
		accSR3,
		pvAR4,
		pvSR4,
		gndavgR,
		outAR,
		gainAR,
		air_total
	};
	double air[air_total];
	
	enum {
		prevSampL1,
		prevSlewL1,
		accSlewL1,
		prevSampL2,
		prevSlewL2,
		accSlewL2,
		prevSampL3,
		prevSlewL3,
		accSlewL3,
		kalGainL,
		kalOutL,
		kalAvgL,
		prevSampR1,
		prevSlewR1,
		accSlewR1,
		prevSampR2,
		prevSlewR2,
		accSlewR2,
		prevSampR3,
		prevSlewR3,
		accSlewR3,
		kalGainR,
		kalOutR,
		kalAvgR,
		kal_total
	};
	double kalM[kal_total];
	double kalS[kal_total];
	
	long double lastSinewL;
	long double lastSinewR;
	//this is overkill, used to run both Zoom and Sinew stages as they are after
	//the summing in StoneFire, which sums three doubles to a long double.
	
	double lastSampleL;
	double intermediateL[16];
	bool wasPosClipL;
	bool wasNegClipL;
	double lastSampleR;
	double intermediateR[16];
	bool wasPosClipR;
	bool wasNegClipR; //Stereo ClipOnly2
	
	int quantA;
	int quantB;
	float expectedSlew;
	float testA;
	float testB;
	double correction;
	double shapedSampleL;
	double shapedSampleR;
	double currentDither;
	double ditherL;
	double ditherR;
	bool cutbinsL;
	bool cutbinsR;
	int hotbinA;
	int hotbinB;
	double benfordize;
	double totalA;
	double totalB;
	double outputSample;
	int expon; //internal dither variables
	
	double NSOddL; //dither section!
	double NSEvenL;
	double prevShapeL;
	double NSOddR;
	double NSEvenR;
	double prevShapeR;
	bool flip; //VinylDither
	double darkSampleL[100];
	double darkSampleR[100]; //Dark
	double previousDitherL;
	double previousDitherR; //PaulWide
	double bynL[13], bynR[13];
	double noiseShapingL, noiseShapingR; //NJAD
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif