/*
*	File:		Galactic2.h
*	
*	Version:	1.0
* 
*	Created:	4/10/23
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
#include "Galactic2Version.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Galactic2_h__
#define __Galactic2_h__


#pragma mark ____Galactic2 Parameters

// parameters
static const float kDefaultValue_ParamOne = 0.5;
static const float kDefaultValue_ParamTwo = 0.5;
static const float kDefaultValue_ParamThree = 0.5;
static const float kDefaultValue_ParamFour = 1.0;

static CFStringRef kParameterOneName = CFSTR("Drive");
static CFStringRef kParameterTwoName = CFSTR("Sustain");
static CFStringRef kParameterThreeName = CFSTR("Darken");
static CFStringRef kParameterFourName = CFSTR("Wetness");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	//Add your parameters here...
	kNumberOfParameters=4
};
const int shortA = 683; const int shortB = 2339; const int shortC = 2381; const int shortD = 887; const int shortE = 743; const int shortF = 1823; const int shortG = 1151; const int shortH = 2833; const int shortI = 521; const int shortJ = 3331; const int shortK = 2851; const int shortL = 1747; const int shortM = 3389; const int shortN = 83; const int shortO = 443; const int shortP = 3221; //290 ms, 10004 seat stadium. Scarcity, 1 in 55796
//Galactic2

#pragma mark ____Galactic2
class Galactic2 : public AUEffectBase
{
public:
	Galactic2(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Galactic2 () { delete mDebugDispatcher; }
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
	virtual ComponentResult		Version() { return kGalactic2Version; }
	
	private: 
	
	double feedbackA;
	double feedbackB;
	double feedbackC;
	double feedbackD;
	
	double iirA;
	double iirB;
	double iirC;
	double iirD;
	double iirE;
	double iirF;
	double iirG;
	double iirH;
	
	double aAL[shortA+5];
	double aBL[shortB+5];
	double aCL[shortC+5];
	double aDL[shortD+5];
	double aEL[shortE+5];
	double aFL[shortF+5];
	double aGL[shortG+5];
	double aHL[shortH+5];
	double aIL[shortI+5];
	double aJL[shortJ+5];
	double aKL[shortK+5];
	double aLL[shortL+5];
	double aML[shortM+5];
	double aNL[shortN+5];
	double aOL[shortO+5];
	double aPL[shortP+5];
	
	double aAR[shortA+5];
	double aBR[shortB+5];
	double aCR[shortC+5];
	double aDR[shortD+5];
	double aER[shortE+5];
	double aFR[shortF+5];
	double aGR[shortG+5];
	double aHR[shortH+5];
	double aIR[shortI+5];
	double aJR[shortJ+5];
	double aKR[shortK+5];
	double aLR[shortL+5];
	double aMR[shortM+5];
	double aNR[shortN+5];
	double aOR[shortO+5];
	double aPR[shortP+5];
	
	double feedbackAL;
	double feedbackBL;
	double feedbackCL;
	double feedbackDL;
	
	double feedbackDR;
	double feedbackHR;
	double feedbackLR;
	double feedbackPR;
	
	double previousAL;
	double previousBL;
	double previousCL;
	double previousDL;
	double previousEL;
	
	double lastRefL[7];
	
	double previousAR;
	double previousBR;
	double previousCR;
	double previousDR;
	double previousER;
	
	double lastRefR[7];
	
	int countAL;
	int countBL;
	int countCL;
	int countDL;
	int countEL;
	int countFL;
	int countGL;
	int countHL;
	int countIL;
	int countJL;
	int countKL;
	int countLL;		
	int countML;		
	int countNL;		
	int countOL;		
	int countPL;		
	
	int countAR;
	int countBR;
	int countCR;
	int countDR;
	int countER;
	int countFR;
	int countGR;
	int countHR;
	int countIR;
	int countJR;
	int countKR;
	int countLR;		
	int countMR;		
	int countNR;		
	int countOR;		
	int countPR;		
	
	int countZ;		
	
	int cycle;
	
	double prevMulchAL;
	double prevMulchAR;
	double prevMulchBL;
	double prevMulchBR;
	double prevMulchCL;
	double prevMulchCR;
	double prevMulchDL;
	double prevMulchDR;
	double prevMulchEL;
	double prevMulchER;
	
	double prevOutAL;
	double prevOutAR;
	double prevOutBL;
	double prevOutBR;
	double prevOutCL;
	double prevOutCR;
	double prevOutDL;
	double prevOutDR;
	double prevOutEL;
	double prevOutER;
	
	double finalOutAL;
	double finalOutAR;
	double finalOutBL;
	double finalOutBR;
	double finalOutCL;
	double finalOutCR;
	double finalOutDL;
	double finalOutDR;
	double finalOutEL;
	double finalOutER;
	
	double prevInAL;
	double prevInAR;
	double prevInBL;
	double prevInBR;
	double prevInCL;
	double prevInCR;
	double prevInDL;
	double prevInDR;
	double prevInEL;
	double prevInER;
	
	double inTrimA;
	double inTrimB;
	double wetA;
	double wetB;	
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif