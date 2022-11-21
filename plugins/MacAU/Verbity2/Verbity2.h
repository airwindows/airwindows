/*
*	File:		Verbity2.h
*	
*	Version:	1.0
* 
*	Created:	9/3/22
*	
*	Copyright:  Copyright © 2022 Airwindows, Airwindows uses the MIT license
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
	
	virtual AUKernelBase *		NewKernel() { return new Verbity2Kernel(this); }
	
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
	
    
	
protected:
		class Verbity2Kernel : public AUKernelBase		// most of the real work happens here
	{
public:
		Verbity2Kernel(AUEffectBase *inAudioUnit )
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
		Float64 iirA;
		Float64 iirB;
		
		
		Float64 aZ[5191];
		
		Float64 aA[5005]; //AFKPU
		Float64 aF[4953];
		Float64 aK[4921];
		Float64 aP[4801];
		Float64 aU[4753];
		
		Float64 aB[4351]; //BGLQV
		Float64 aG[4159];
		Float64 aL[3931];
		Float64 aQ[3531];
		Float64 aV[3331];
		
		Float64 aC[3325]; //CHMRW
		Float64 aH[2793];
		Float64 aM[2769];
		Float64 aR[2391];
		Float64 aW[2349];
		
		Float64 aD[2143]; //DINSX
		Float64 aI[1813];
		Float64 aN[1735];
		Float64 aS[1173];
		Float64 aX[789];
		
		Float64 aE[679]; //EJOTY
		Float64 aJ[645];
		Float64 aO[441];
		Float64 aT[351];
		Float64 aY[283];
		
		Float64 feedbackA;
		Float64 feedbackB;
		Float64 feedbackC;
		Float64 feedbackD;
		Float64 feedbackE;
		
		Float64 previousA;
		Float64 previousB;
		Float64 previousC;
		Float64 previousD;
		Float64 previousE;
		
		Float64 lastRef[7];
		
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
		
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif