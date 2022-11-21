/*
 *	File:		Logical4.h
 *	
 *	Version:	1.0 
 * 
 *	Created:	11/9/15
 *	
 *	Copyright:  Copyright © 2015 Airwindows, Airwindows uses the MIT license
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
#include "Logical4Version.h"

#if AU_DEBUG_DISPATCHER
#include "AUDebugDispatcher.h"
#endif


#ifndef __Logical4_h__
#define __Logical4_h__


#pragma mark ____Logical4 Parameters

// parameters
static const float kDefaultValue_ParamOne = 0.0;
static const float kDefaultValue_ParamTwo = 4.0;
static const float kDefaultValue_ParamThree = 20.0;
static const float kDefaultValue_ParamFour = 0.0;
static const float kDefaultValue_ParamFive = 1.0;

static CFStringRef kParameterOneName = CFSTR("Threshold");
static CFStringRef kParameterTwoName = CFSTR("Ratio");
static CFStringRef kParameterTwoUnit = CFSTR("/ 1");
static CFStringRef kParameterThreeName = CFSTR("Reaction Speed");
static CFStringRef kParameterThreeUnit = CFSTR("ms");
static CFStringRef kParameterFourName = CFSTR("Makeup Gain");
static CFStringRef kParameterFiveName = CFSTR("Dry/Wet");

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	kParam_Five =4,
	//Add your parameters here...
	kNumberOfParameters=5
};

#pragma mark ____Logical4
class Logical4 : public AUEffectBase
{
public:
	Logical4(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Logical4 () { delete mDebugDispatcher; }
#endif
	
	virtual ComponentResult Reset(AudioUnitScope inScope, AudioUnitElement inElement);
	
	virtual OSStatus ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags, 
										const AudioBufferList & inBuffer, AudioBufferList & outBuffer, 
										UInt32 inFramesToProcess);
	
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
	
	virtual UInt32 SupportedNumChannels(const AUChannelInfo ** outInfo);
	
	virtual ComponentResult    Initialize();
	virtual bool				SupportsTail () { return true; }
    virtual Float64				GetTailTime() {return 1.0/GetSampleRate()*22050;} //roughly half seconds tail
    virtual Float64				GetLatency() {return 0.0;}	// edit these because tail time isn't 1000 samples and latency isn't 1
	
	/*! @method Version */
	virtual ComponentResult		Version() { return kLogical4Version; }
	
private:
	
	//begin ButterComp
	Float64 controlAposL;
	Float64 controlAnegL;
	Float64 controlBposL;
	Float64 controlBnegL;
	Float64 targetposL;
	Float64 targetnegL;
	Float64 controlAposBL;
	Float64 controlAnegBL;
	Float64 controlBposBL;
	Float64 controlBnegBL;
	Float64 targetposBL;
	Float64 targetnegBL;
	Float64 controlAposCL;
	Float64 controlAnegCL;
	Float64 controlBposCL;
	Float64 controlBnegCL;
	Float64 targetposCL;
	Float64 targetnegCL;
	Float64 avgAL;
	Float64 avgBL;
	Float64 avgCL;
	Float64 avgDL;
	Float64 avgEL;
	Float64 avgFL;
	Float64 nvgAL;
	Float64 nvgBL;
	Float64 nvgCL;
	Float64 nvgDL;
	Float64 nvgEL;
	Float64 nvgFL;
	//end ButterComp
	
	//begin Power Sag
	Float64 dL[1000];
	Float64 controlL;
	Float64 bL[1000];
	Float64 controlBL;
	Float64 cL[1000];
	Float64 controlCL;
	//end Power Sag
	
	//begin ButterComp
	Float64 controlAposR;
	Float64 controlAnegR;
	Float64 controlBposR;
	Float64 controlBnegR;
	Float64 targetposR;
	Float64 targetnegR;
	Float64 controlAposBR;
	Float64 controlAnegBR;
	Float64 controlBposBR;
	Float64 controlBnegBR;
	Float64 targetposBR;
	Float64 targetnegBR;
	Float64 controlAposCR;
	Float64 controlAnegCR;
	Float64 controlBposCR;
	Float64 controlBnegCR;
	Float64 targetposCR;
	Float64 targetnegCR;
	Float64 avgAR;
	Float64 avgBR;
	Float64 avgCR;
	Float64 avgDR;
	Float64 avgER;
	Float64 avgFR;
	Float64 nvgAR;
	Float64 nvgBR;
	Float64 nvgCR;
	Float64 nvgDR;
	Float64 nvgER;
	Float64 nvgFR;
	//end ButterComp
	
	//begin Power Sag
	Float64 dR[1000];
	Float64 controlR;
	Float64 bR[1000];
	Float64 controlBR;
	Float64 cR[1000];
	Float64 controlCR;
	//end Power Sag
	
	int gcount;
	
	bool fpFlip;
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif