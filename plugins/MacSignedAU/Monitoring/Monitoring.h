/*
*	File:		Monitoring.h
*	
*	Version:	1.0
* 
*	Created:	9/2/19
*	
*	Copyright:  Copyright © 2019 Airwindows, Airwindows uses the MIT license
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
#include "MonitoringVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Monitoring_h__
#define __Monitoring_h__


#pragma mark ____Monitoring Parameters

// parameters
static CFStringRef kParameterOneName = CFSTR("Monitor");
static const int kNJAD = 0;
static const int kNJCD = 1;
static const int kPEAK = 2;
static const int kSLEW = 3;
static const int kSUBS = 4;
static const int kMONO = 5;
static const int kSIDE = 6;
static const int kVINYL = 7;
static const int kAURAT = 8;
static const int kMONORAT = 9;
static const int kMONOLAT = 10;
static const int kPHONE = 11;
static const int kCANSA = 12;
static const int kCANSB = 13;
static const int kCANSC = 14;
static const int kCANSD = 15;
static const int kTRICK = 16;
static const int kDefaultValue_ParamOne = kNJAD;

static CFStringRef kMenuItem_NJAD = CFSTR ("Out24");
static CFStringRef kMenuItem_NJCD = CFSTR ("Out16");
static CFStringRef kMenuItem_PEAK = CFSTR ("Peaks");
static CFStringRef kMenuItem_SLEW = CFSTR ("Slew");
static CFStringRef kMenuItem_SUBS = CFSTR ("Subs");
static CFStringRef kMenuItem_MONO = CFSTR ("Mono");
static CFStringRef kMenuItem_SIDE = CFSTR ("Side");
static CFStringRef kMenuItem_VINYL = CFSTR ("Vinyl");
static CFStringRef kMenuItem_AURAT = CFSTR ("Aurat");
static CFStringRef kMenuItem_MONORAT = CFSTR ("MonoRat");
static CFStringRef kMenuItem_MONOLAT = CFSTR ("MonoLat");
static CFStringRef kMenuItem_PHONE = CFSTR ("Phone");
static CFStringRef kMenuItem_CANSA = CFSTR ("Cans A");
static CFStringRef kMenuItem_CANSB = CFSTR ("Cans B");
static CFStringRef kMenuItem_CANSC = CFSTR ("Cans C");
static CFStringRef kMenuItem_CANSD = CFSTR ("Cans D");
static CFStringRef kMenuItem_TRICK = CFSTR ("Voice Trick");

enum {
	kParam_One =0,
	//Add your parameters here...
	kNumberOfParameters=1
};

#pragma mark ____Monitoring
class Monitoring : public AUEffectBase
{
public:
	Monitoring(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Monitoring () { delete mDebugDispatcher; }
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
	virtual ComponentResult		Version() { return kMonitoringVersion; }
	
	private: 
	double bynL[13], bynR[13];
	double noiseShapingL, noiseShapingR;
	//NJAD
	Float64 aL[1503], bL[1503], cL[1503], dL[1503];
	Float64 aR[1503], bR[1503], cR[1503], dR[1503];
	int ax, bx, cx, dx;
	//PeaksOnly
	Float64 lastSampleL, lastSampleR;
	//SlewOnly
	Float64 iirSampleAL, iirSampleBL, iirSampleCL, iirSampleDL, iirSampleEL, iirSampleFL, iirSampleGL;
	Float64 iirSampleHL, iirSampleIL, iirSampleJL, iirSampleKL, iirSampleLL, iirSampleML, iirSampleNL, iirSampleOL, iirSamplePL;
	Float64 iirSampleQL, iirSampleRL, iirSampleSL;
	Float64 iirSampleTL, iirSampleUL, iirSampleVL;
	Float64 iirSampleWL, iirSampleXL, iirSampleYL, iirSampleZL;
	
	Float64 iirSampleAR, iirSampleBR, iirSampleCR, iirSampleDR, iirSampleER, iirSampleFR, iirSampleGR;
	Float64 iirSampleHR, iirSampleIR, iirSampleJR, iirSampleKR, iirSampleLR, iirSampleMR, iirSampleNR, iirSampleOR, iirSamplePR;
	Float64 iirSampleQR, iirSampleRR, iirSampleSR;
	Float64 iirSampleTR, iirSampleUR, iirSampleVR;
	Float64 iirSampleWR, iirSampleXR, iirSampleYR, iirSampleZR; // o/`
	//SubsOnly
	double biquad[11];
	//Bandpasses

	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif