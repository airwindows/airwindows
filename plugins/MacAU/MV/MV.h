/*
*	File:		MV.h
*	
*	Version:	1.0
* 
*	Created:	2/14/19
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
#include "MVVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __MV_h__
#define __MV_h__


#pragma mark ____MV Parameters

// parameters
static const float kDefaultValue_ParamOne = 0.5;
static const float kDefaultValue_ParamTwo = 0.5;
static const float kDefaultValue_ParamThree = 0.5;
static const float kDefaultValue_ParamFour = 1.0;
static const float kDefaultValue_ParamFive = 1.0;

static CFStringRef kParameterOneName = CFSTR("Depth");
static CFStringRef kParameterTwoName = CFSTR("Bright");
static CFStringRef kParameterThreeName = CFSTR("Regen");
static CFStringRef kParameterFourName = CFSTR("Output");
static CFStringRef kParameterFiveName = CFSTR("Dry/Wet");
//Alter the name if desired, but using the plugin name is a start

enum {
	kParam_One =0,
	kParam_Two =1,
	kParam_Three =2,
	kParam_Four =3,
	kParam_Five =4,
	//Add your parameters here...
	kNumberOfParameters=5
};

#pragma mark ____MV
class MV : public AUEffectBase
{
public:
	MV(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~MV () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new MVKernel(this); }
	
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
	virtual ComponentResult		Version() { return kMVVersion; }
	
    
	
protected:
		class MVKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		MVKernel(AUEffectBase *inAudioUnit )
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
				
		int alpA, delayA;
		int alpB, delayB;
		int alpC, delayC;
		int alpD, delayD;
		int alpE, delayE;
		int alpF, delayF;
		int alpG, delayG;
		int alpH, delayH;
		int alpI, delayI;
		int alpJ, delayJ;
		int alpK, delayK;
		int alpL, delayL;
		int alpM, delayM;
		int alpN, delayN;
		int alpO, delayO;
		int alpP, delayP;
		int alpQ, delayQ;
		int alpR, delayR;
		int alpS, delayS;
		int alpT, delayT;
		int alpU, delayU;
		int alpV, delayV;
		int alpW, delayW;
		int alpX, delayX;
		int alpY, delayY;
		int alpZ, delayZ;
		
		Float64 avgA;
		Float64 avgB;
		Float64 avgC;
		Float64 avgD;
		Float64 avgE;
		Float64 avgF;
		Float64 avgG;
		Float64 avgH;
		Float64 avgI;
		Float64 avgJ;
		Float64 avgK;
		Float64 avgL;
		Float64 avgM;
		Float64 avgN;
		Float64 avgO;
		Float64 avgP;
		Float64 avgQ;
		Float64 avgR;
		Float64 avgS;
		Float64 avgT;
		Float64 avgU;
		Float64 avgV;
		Float64 avgW;
		Float64 avgX;
		Float64 avgY;
		Float64 avgZ;
		
		Float64 feedback;
		
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif