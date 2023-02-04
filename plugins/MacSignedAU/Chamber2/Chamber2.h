/*
*	File:		Chamber2.h
*	
*	Version:	1.0
* 
*	Created:	2/1/23
*	
*	Copyright:  Copyright © 2023 Airwindows, Airwindows uses the MIT license
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
#include "Chamber2Version.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Chamber2_h__
#define __Chamber2_h__


#pragma mark ____Chamber2 Parameters

// parameters
static const float kDefaultValue_ParamOne = 0.34;
static const float kDefaultValue_ParamTwo = 0.31;
static const float kDefaultValue_ParamThree = 0.28;
static const float kDefaultValue_ParamFour = 0.25;

static CFStringRef kParameterOneName = CFSTR("Delay");
static CFStringRef kParameterTwoName = CFSTR("Regen");
static CFStringRef kParameterThreeName = CFSTR("Thick");
static CFStringRef kParameterFourName = CFSTR("Wet");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	//Add your parameters here...
	kNumberOfParameters=4
};

#pragma mark ____Chamber2
class Chamber2 : public AUEffectBase
{
public:
	Chamber2(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Chamber2 () { delete mDebugDispatcher; }
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
	virtual ComponentResult		Version() { return kChamber2Version; }
	
		private: 
    
	double aEL[10000];
	double aFL[10000];
	double aGL[10000];
	double aHL[10000];
	double aAL[10000];
	double aBL[10000];
	double aCL[10000];
	double aDL[10000];
	double aIL[10000];
	double aJL[10000];
	double aKL[10000];
	double aLL[10000];
	double aML[10000];
	
	double feedbackAL;
	double feedbackBL;
	double feedbackCL;
	double feedbackDL;
	double previousAL;
	double previousBL;
	double previousCL;
	double previousDL;
	
	double lastRefL[10];
	
	double aER[10000];
	double aFR[10000];
	double aGR[10000];
	double aHR[10000];
	double aAR[10000];
	double aBR[10000];
	double aCR[10000];
	double aDR[10000];
	double aIR[10000];
	double aJR[10000];
	double aKR[10000];
	double aLR[10000];
	double aMR[10000];
	
	double feedbackAR;
	double feedbackBR;
	double feedbackCR;
	double feedbackDR;
	double previousAR;
	double previousBR;
	double previousCR;
	double previousDR;
	
	double lastRefR[10];
	
	int countA, delayA;
	int countB, delayB;
	int countC, delayC;
	int countD, delayD;
	int countE, delayE;
	int countF, delayF;
	int countG, delayG;
	int countH, delayH;
	int countI, delayI;
	int countJ, delayJ;
	int countK, delayK;
	int countL, delayL;
	int countM, delayM;
	int cycle; //all these ints are shared across channels, not duplicated	
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif