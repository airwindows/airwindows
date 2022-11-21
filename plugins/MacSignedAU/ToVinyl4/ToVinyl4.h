/*
*	File:		ToVinyl4.h
*	
*	Version:	1.0
* 
*	Created:	6/10/14
*	
*	Copyright:  Copyright © 2014 Airwindows, Airwindows uses the MIT license
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
#include "ToVinyl4Version.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __ToVinyl4_h__
#define __ToVinyl4_h__


#pragma mark ____ToVinyl4 Parameters

// parameters
static const float kDefaultValue_ParamOne = 22.0;
static const float kDefaultValue_ParamTwo = 44.0;
static const float kDefaultValue_ParamThree = 0.32;
static const float kDefaultValue_ParamFour = 0.064;

static CFStringRef kParameterOneName = CFSTR("Mid Highpass");
static CFStringRef kParameterOneUnit = CFSTR("hz");
static CFStringRef kParameterTwoName = CFSTR("Side Highpass");
static CFStringRef kParameterTwoUnit = CFSTR("hz");
static CFStringRef kParameterThreeName = CFSTR("HF Limiter");
static CFStringRef kParameterFourName = CFSTR("Groove Wear");


enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	//Add your parameters here...
	kNumberOfParameters=4
};

#pragma mark ____ToVinyl4
class ToVinyl4 : public AUEffectBase
{
public:
	ToVinyl4(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~ToVinyl4 () { delete mDebugDispatcher; }
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
    virtual Float64				GetTailTime() {return 0.0;} //in SECONDS!
    virtual Float64				GetLatency() {return 0.0;}	// in SECONDS!
		
	/*! @method Version */
	virtual ComponentResult		Version() { return kToVinyl4Version; }
	
    
private: 
	Float64 ataLastOutL;
	Float64 ataLastOutR;
	
	Float64 s1L;
	Float64 s2L;
	Float64 s3L;
	Float64 o1L;
	Float64 o2L;
	Float64 o3L;
	Float64 m1L;
	Float64 m2L;
	Float64 s1R;
	Float64 s2R;
	Float64 s3R;
	Float64 o1R;
	Float64 o2R;
	Float64 o3R;
	Float64 m1R;
	Float64 m2R;
	Float64 desL;
	Float64 desR;
	
	
	Float64 midSampleA;
	Float64 midSampleB;
	Float64 midSampleC;
	Float64 midSampleD;
	Float64 midSampleE;
	Float64 midSampleF;
	Float64 midSampleG;
	Float64 midSampleH;
	Float64 midSampleI;
	Float64 midSampleJ;
	Float64 midSampleK;
	Float64 midSampleL;
	Float64 midSampleM;
	Float64 midSampleN;
	Float64 midSampleO;
	Float64 midSampleP;
	Float64 midSampleQ;
	Float64 midSampleR;
	Float64 midSampleS;
	Float64 midSampleT;
	Float64 midSampleU;
	Float64 midSampleV;
	Float64 midSampleW;
	Float64 midSampleX;
	Float64 midSampleY;
	Float64 midSampleZ;
	
	Float64 sideSampleA;
	Float64 sideSampleB;
	Float64 sideSampleC;
	Float64 sideSampleD;
	Float64 sideSampleE;
	Float64 sideSampleF;
	Float64 sideSampleG;
	Float64 sideSampleH;
	Float64 sideSampleI;
	Float64 sideSampleJ;
	Float64 sideSampleK;
	Float64 sideSampleL;
	Float64 sideSampleM;
	Float64 sideSampleN;
	Float64 sideSampleO;
	Float64 sideSampleP;
	Float64 sideSampleQ;
	Float64 sideSampleR;
	Float64 sideSampleS;
	Float64 sideSampleT;
	Float64 sideSampleU;
	Float64 sideSampleV;
	Float64 sideSampleW;
	Float64 sideSampleX;
	Float64 sideSampleY;
	Float64 sideSampleZ;
	
	Float64 aMid[11];
	Float64 bMid[11];
	Float64 fMid[11];		
	Float64 aSide[11];
	Float64 bSide[11];
	Float64 fSide[11];		
	Float64 aMidPrev;
	Float64 aSidePrev;
	Float64 bMidPrev;
	Float64 bSidePrev;
	uint32_t fpdL;
	uint32_t fpdR;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif