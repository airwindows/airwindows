/*
*	File:		MasteringMono.h
*	
*	Version:	1.0
* 
*	Created:	9/25/25
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
#include "MasteringMonoVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __MasteringMono_h__
#define __MasteringMono_h__


#pragma mark ____MasteringMono Parameters

// parameters
static const float kDefaultValue_ParamA = 0.0;
static const float kDefaultValue_ParamB = 0.5;
static const float kDefaultValue_ParamC = 0.5;
static const float kDefaultValue_ParamD = 0.5;
static const float kDefaultValue_ParamE = 0.5;
static const int kDark = 1;
static const int kTenNines = 2;
static const int kTPDFWide = 3;
static const int kPaulWide = 4;
static const int kNJAD = 5;
static const int kBypass = 6;
static const int kDefaultValue_ParamF = kBypass;

static CFStringRef kMenuItem_Dark = CFSTR ("Dark");
static CFStringRef kMenuItem_TenNines = CFSTR ("Ten Nines");
static CFStringRef kMenuItem_TPDFWide = CFSTR ("TPDF");
static CFStringRef kMenuItem_PaulWide = CFSTR ("PaulDither");
static CFStringRef kMenuItem_NJAD = CFSTR ("NJAD");
static CFStringRef kMenuItem_Bypass = CFSTR ("Bypass");

static CFStringRef kParameterAName = CFSTR("Glue");
static CFStringRef kParameterBName = CFSTR("Scope");
static CFStringRef kParameterCName = CFSTR("Skronk");
static CFStringRef kParameterDName = CFSTR("Girth");
static CFStringRef kParameterEName = CFSTR("Drive");
static CFStringRef kParameterFName = CFSTR("Dither");

enum {
	kParam_A =0,
	kParam_B =1,
	kParam_C =2,
	kParam_D =3,
	kParam_E =4,
	kParam_F =5,
	//Add your parameters here...
	kNumberOfParameters=6
};

#pragma mark ____MasteringMono
class MasteringMono : public AUEffectBase
{
public:
	MasteringMono(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~MasteringMono () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new MasteringMonoKernel(this); }
	
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
	virtual ComponentResult		Version() { return kMasteringMonoVersion; }
	
    
	
protected:
		class MasteringMonoKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		MasteringMonoKernel(AUEffectBase *inAudioUnit )
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
			kal_total
		};
		double kalM[kal_total];
		double kalS[kal_total];
		
		long double lastSinewL;
		//this is overkill, used to run both Zoom and Sinew stages as they are after
		//the summing in StoneFire, which sums three doubles to a long double.
		
		double lastSampleL;
		double intermediateL[16];
		bool wasPosClipL;
		bool wasNegClipL;
		
		int quantA;
		int quantB;
		float expectedSlew;
		float testA;
		float testB;
		double correction;
		double shapedSampleL;
		double currentDither;
		double ditherL;
		bool cutbinsL;
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
		bool flip; //VinylDither
		double darkSampleL[100];
		double previousDitherL;
		double bynL[13];
		double noiseShapingL; //NJAD
		
		uint32_t fpdL;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif