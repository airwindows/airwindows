/*
*	File:		PocketVerbs.h
*	
*	Version:	1.0
* 
*	Created:	8/19/12
*	
*	Copyright:  Copyright © 2012 Airwindows, Airwindows uses the MIT license
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
#include "PocketVerbsVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __PocketVerbs_h__
#define __PocketVerbs_h__


#pragma mark ____PocketVerbs Parameters

// parameters
static CFStringRef kParameterOneName = CFSTR("Type");
static const int kChamber = 1;
static const int kSpring = 2;
static const int kTiled = 3;
static const int kRoom = 4;
static const int kStretch = 5;
static const int kZarathustra = 6;
static const int kDefaultValue_ParamOne = kChamber;

static CFStringRef kMenuItem_Chamber = CFSTR ("Chamber");
static CFStringRef kMenuItem_Spring = CFSTR ("Spring");
static CFStringRef kMenuItem_Tiled = CFSTR ("Tiled");
static CFStringRef kMenuItem_Room = CFSTR ("Room");
static CFStringRef kMenuItem_Stretch = CFSTR ("Stretch");
static CFStringRef kMenuItem_Zarathustra = CFSTR ("Zarathustra");


static const float kDefaultValue_ParamTwo = 0.5;
static const float kDefaultValue_ParamThree = 0.0;
static const float kDefaultValue_ParamFour = 0.5;

static CFStringRef kParameterTwoName = CFSTR("Size");
static CFStringRef kParameterThreeName = CFSTR("Gating");
static CFStringRef kParameterFourName = CFSTR("Dry/Wet");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	//Add your parameters here...
	kNumberOfParameters=4
};

#pragma mark ____PocketVerbs
class PocketVerbs : public AUEffectBase
{
public:
	PocketVerbs(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~PocketVerbs () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new PocketVerbsKernel(this); }
	
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
    virtual Float64				GetTailTime() {return 1.0/GetSampleRate()*220000.0;} //in SECONDS! gsr * a number = in samples
    virtual Float64				GetLatency() {return (1.0/GetSampleRate())*0.0;}	// in SECONDS! gsr * a number = in samples
	
	/*! @method Version */
	virtual ComponentResult		Version() { return kPocketVerbsVersion; }
	
    
	
protected:
		class PocketVerbsKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		PocketVerbsKernel(AUEffectBase *inAudioUnit )
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
		Float64 dA[7];
		Float64 dB[7];
		Float64 dC[7];
		Float64 dD[7];		
		Float64 dE[7];
		Float64 dF[7];
		Float64 dG[7];
		Float64 dH[7];
		Float64 dI[7];
		Float64 dJ[7];
		Float64 dK[7];
		Float64 dL[7];
		Float64 dM[7];
		Float64 dN[7];
		Float64 dO[7];
		Float64 dP[7];
		Float64 dQ[7];
		Float64 dR[7];
		Float64 dS[7];
		Float64 dT[7];
		Float64 dU[7];
		Float64 dV[7];
		Float64 dW[7];
		Float64 dX[7];
		Float64 dY[7];
		Float64 dZ[7];
		
		Float64 aA[15150];
		Float64 aB[14618];
		Float64 aC[14358];
		Float64 aD[13818];		
		Float64 aE[13562];
		Float64 aF[13046];
		Float64 aG[11966];
		Float64 aH[11130];
		Float64 aI[10598];
		Float64 aJ[9810];
		Float64 aK[9522];
		Float64 aL[8982];
		Float64 aM[8786];
		Float64 aN[8462];
		Float64 aO[8310];
		Float64 aP[7982];
		Float64 aQ[7322];
		Float64 aR[6818];
		Float64 aS[6506];
		Float64 aT[6002];
		Float64 aU[5838];
		Float64 aV[5502];
		Float64 aW[5010];
		Float64 aX[4850];
		Float64 aY[4296];
		Float64 aZ[4180];
		
		Float64 oA[15150];
		Float64 oB[14618];
		Float64 oC[14358];
		Float64 oD[13818];		
		Float64 oE[13562];
		Float64 oF[13046];
		Float64 oG[11966];
		Float64 oH[11130];
		Float64 oI[10598];
		Float64 oJ[9810];
		Float64 oK[9522];
		Float64 oL[8982];
		Float64 oM[8786];
		Float64 oN[8462];
		Float64 oO[8310];
		Float64 oP[7982];
		Float64 oQ[7322];
		Float64 oR[6818];
		Float64 oS[6506];
		Float64 oT[6002];
		Float64 oU[5838];
		Float64 oV[5502];
		Float64 oW[5010];
		Float64 oX[4850];
		Float64 oY[4296];
		Float64 oZ[4180];
		
		int outA, alpA, maxdelayA, delayA;
		int outB, alpB, maxdelayB, delayB;
		int outC, alpC, maxdelayC, delayC;
		int outD, alpD, maxdelayD, delayD;
		int outE, alpE, maxdelayE, delayE;
		int outF, alpF, maxdelayF, delayF;
		int outG, alpG, maxdelayG, delayG;
		int outH, alpH, maxdelayH, delayH;
		int outI, alpI, maxdelayI, delayI;
		int outJ, alpJ, maxdelayJ, delayJ;
		int outK, alpK, maxdelayK, delayK;
		int outL, alpL, maxdelayL, delayL;
		int outM, alpM, maxdelayM, delayM;
		int outN, alpN, maxdelayN, delayN;
		int outO, alpO, maxdelayO, delayO;
		int outP, alpP, maxdelayP, delayP;
		int outQ, alpQ, maxdelayQ, delayQ;
		int outR, alpR, maxdelayR, delayR;
		int outS, alpS, maxdelayS, delayS;
		int outT, alpT, maxdelayT, delayT;
		int outU, alpU, maxdelayU, delayU;
		int outV, alpV, maxdelayV, delayV;
		int outW, alpW, maxdelayW, delayW;
		int outX, alpX, maxdelayX, delayX;
		int outY, alpY, maxdelayY, delayY;
		int outZ, alpZ, maxdelayZ, delayZ;
		
		Float64 savedRoomsize;
		int countdown;
		Float64 peak;
		uint32_t fpd;

	};
													   };
													   
													   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
													   
													   
#endif