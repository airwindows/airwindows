/*
*	File:		Ditherbox.h
*	
*	Version:	1.0
* 
*	Created:	1/1/09
*	
*	Copyright:  Copyright © 2009 Airwindows, Airwindows uses the MIT license
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
#include "DitherboxVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Ditherbox_h__
#define __Ditherbox_h__


#pragma mark ____Ditherbox Parameters

// parameters
static CFStringRef kParameterOneName = CFSTR("Dither Type");
static const int kTruncate = 1;
static const int kFlat = 2;
static const int kTPDF = 3;
static const int kPaul = 4;
static const int kDoublePaul = 5;
static const int kTape = 6;
static const int kQuadratic = 7;
static const int kTenNines = 8;
static const int kContingent = 9;
static const int kNaturalize = 10;
static const int kNJAD = 11;
static const int kTruncateHR = 12;
static const int kFlatHR = 13;
static const int kTPDFHR = 14;
static const int kPaulHR = 15;
static const int kDoublePaulHR = 16;
static const int kTapeHR = 17;
static const int kQuadraticHR = 18;
static const int kTenNinesHR = 19;
static const int kContingentHR = 20;
static const int kNaturalizeHR = 21;
static const int kNJADHR = 22;
static const int kSlewOnly = 23;
static const int kSubsOnly = 24;
static const int kSilhouette = 25;
static const int kDefaultValue_ParamOne = 22;


static CFStringRef kMenuItem_Truncate = CFSTR ("16 Bit Truncation");
static CFStringRef kMenuItem_Flat = CFSTR ("16 bit Flat Dither");
static CFStringRef kMenuItem_TPDF = CFSTR ("16 bit TPDF Dither");
static CFStringRef kMenuItem_Paul = CFSTR ("16 bit Paul Dither");
static CFStringRef kMenuItem_DoublePaul = CFSTR ("16 bit DoublePaul Dither");
static CFStringRef kMenuItem_Tape = CFSTR ("16 bit Tape Dither");
static CFStringRef kMenuItem_Quadratic = CFSTR ("16 bit High Gloss Dither");
static CFStringRef kMenuItem_TenNines = CFSTR ("16 bit Vinyl Dither");
static CFStringRef kMenuItem_Contingent = CFSTR ("16 bit Spatialize Dither");
static CFStringRef kMenuItem_Naturalize = CFSTR ("16 bit Naturalize Dither");
static CFStringRef kMenuItem_NJAD = CFSTR ("16 bit Not Just Another Dither");
static CFStringRef kMenuItem_TruncateHR = CFSTR ("24 bit Truncation");
static CFStringRef kMenuItem_FlatHR = CFSTR ("24 bit Flat Dither");
static CFStringRef kMenuItem_TPDFHR = CFSTR ("24 bit TPDF Dither");
static CFStringRef kMenuItem_PaulHR = CFSTR ("24 bit Paul Dither");
static CFStringRef kMenuItem_DoublePaulHR = CFSTR ("24 bit DoublePaul Dither");
static CFStringRef kMenuItem_TapeHR = CFSTR ("24 bit Tape Dither");
static CFStringRef kMenuItem_QuadraticHR = CFSTR ("24 bit High Gloss Dither");
static CFStringRef kMenuItem_TenNinesHR = CFSTR ("24 bit Vinyl Dither");
static CFStringRef kMenuItem_ContingentHR = CFSTR ("24 bit Spatialize Dither");
static CFStringRef kMenuItem_NaturalizeHR = CFSTR ("24 bit Naturalize Dither");
static CFStringRef kMenuItem_NJADHR = CFSTR ("24 bit Not Just Another Dither");
static CFStringRef kMenuItem_SlewOnly = CFSTR ("Slew Only Monitoring");
static CFStringRef kMenuItem_SubsOnly = CFSTR ("Subs Only Monitoring");
static CFStringRef kMenuItem_Silhouette = CFSTR ("Noise Silhouette Monitoring");


enum {
	kParam_One = 0,
	//Add your parameters here...
	kNumberOfParameters=1
};

#pragma mark ____Ditherbox
class Ditherbox : public AUEffectBase
{
public:
	Ditherbox(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Ditherbox () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new DitherboxKernel(this); }
	
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
    virtual Float64				GetTailTime() {return 1.0/GetSampleRate()*1.0;}
    virtual Float64				GetLatency() {return 0.0;}

	/*! @method Version */
	virtual ComponentResult	Version() { return kDitherboxVersion; }
	
    
	
protected:
		class DitherboxKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		DitherboxKernel(AUEffectBase *inAudioUnit )
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
		double byn[13];
		double noiseShaping;
		double contingentErr;
		double currentDither;
		double NSOdd;
		double NSEven;
		double prev;
		double ns[16];
		int Position;
		bool flip;
		double lastSample;
		double outSample;
		double iirSampleA;
		double iirSampleB;
		double iirSampleC;
		double iirSampleD;
		double iirSampleE;
		double iirSampleF;
		double iirSampleG;
		double iirSampleH;
		double iirSampleI;
		double iirSampleJ;
		double iirSampleK;
		double iirSampleL;
		double iirSampleM;
		double iirSampleN;
		double iirSampleO;
		double iirSampleP;
		double iirSampleQ;
		double iirSampleR;
		double iirSampleS;
		double iirSampleT;
		double iirSampleU;
		double iirSampleV;
		double iirSampleW;
		double iirSampleX;
		double iirSampleY;
		double iirSampleZ;
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif