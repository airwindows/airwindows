/*
*	File:		Verbity2.h
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
#include "Verbity2Version.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Verbity2_h__
#define __Verbity2_h__


#pragma mark ____Verbity2 Parameters

// parameters
static const float kDefaultValue_ParamOne = 0.5;
static const float kDefaultValue_ParamTwo = 0.5;
static const float kDefaultValue_ParamThree = 0.5;
static const float kDefaultValue_ParamFour = 1.0;

static CFStringRef kParameterOneName = CFSTR("RmSize");
static CFStringRef kParameterTwoName = CFSTR("Sustain");
static CFStringRef kParameterThreeName = CFSTR("Mulch");
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

#pragma mark ____Verbity2
class Verbity2 : public AUEffectBase
{
public:
	Verbity2(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Verbity2 () { delete mDebugDispatcher; }
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
	virtual ComponentResult		Version() { return kVerbity2Version; }
	
		private: 
	double iirAL;
	double iirBL;
	
	//double aZL[5191];
	
	double aAL[5005]; //AFKPU
	double aFL[4953];
	double aKL[4921];
	double aPL[4801];
	double aUL[4753];
	
	double aBL[4351]; //BGLQV
	double aGL[4159];
	double aLL[3931];
	double aQL[3531];
	double aVL[3331];
	
	double aCL[3325]; //CHMRW
	double aHL[2793];
	double aML[2769];
	double aRL[2391];
	double aWL[2349];
	
	double aDL[2143]; //DINSX
	double aIL[1813];
	double aNL[1735];
	double aSL[1173];
	double aXL[789];
	
	double aEL[679]; //EJOTY
	double aJL[645];
	double aOL[441];
	double aTL[351];
	double aYL[283];
	
	double feedbackAL;
	double feedbackBL;
	double feedbackCL;
	double feedbackDL;
	double feedbackEL;
	
	double previousAL;
	double previousBL;
	double previousCL;
	double previousDL;
	double previousEL;
	
	double lastRefL[7];
	
	double iirAR;
	double iirBR;
	
	//double aZR[5191];
	
	double aAR[5005]; //AFKPU
	double aFR[4953];
	double aKR[4921];
	double aPR[4801];
	double aUR[4753];
	
	double aBR[4351]; //BGLQV
	double aGR[4159];
	double aLR[3931];
	double aQR[3531];
	double aVR[3331];
	
	double aCR[3325]; //CHMRW
	double aHR[2793];
	double aMR[2769];
	double aRR[2391];
	double aWR[2349];
	
	double aDR[2143]; //DINSX
	double aIR[1813];
	double aNR[1735];
	double aSR[1173];
	double aXR[789];
	
	double aER[679]; //EJOTY
	double aJR[645];
	double aOR[441];
	double aTR[351];
	double aYR[283];
	
	double feedbackAR;
	double feedbackBR;
	double feedbackCR;
	double feedbackDR;
	double feedbackER;
	
	double previousAR;
	double previousBR;
	double previousCR;
	double previousDR;
	double previousER;
	
	double lastRefR[7];
	
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
	int countN, delayN;		
	int countO, delayO;		
	int countP, delayP;		
	int countQ, delayQ;		
	int countR, delayR;		
	int countS, delayS;		
	int countT, delayT;		
	int countU, delayU;		
	int countV, delayV;		
	int countW, delayW;		
	int countX, delayX;		
	int countY, delayY;		
	int countZ, delayZ;		
	int cycle;
	
	uint32_t fpdL;
	uint32_t fpdR;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif