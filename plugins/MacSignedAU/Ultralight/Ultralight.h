/*
*	File:		Ultralight.h
*	
*	Version:	1.0
* 
*	Created:	8/26/26
*	
*	Copyright:  Copyright © 2026 Airwindows, Airwindows uses the MIT license
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
#include "UltralightVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Ultralight_h__
#define __Ultralight_h__


#pragma mark ____Ultralight Parameters

// parameters
static const int kDefaultValue_ParamA = 6;
static const int kDefaultValue_ParamB = 3;
static const int kDefaultValue_ParamC = 3;
static const int kDefaultValue_ParamD = 3;
static const int kDefaultValue_ParamE = 1194;
static const float kDefaultValue_ParamF = 0.5f;
static const float kDefaultValue_ParamG = 0.0f;

static CFStringRef kParameterAName = CFSTR("RT60");
static CFStringRef kParameterBName = CFSTR("Furnish");
static CFStringRef kParameterCName = CFSTR("SolidWl");
static CFStringRef kParameterDName = CFSTR("ReflcWl");
static CFStringRef kParameterEName = CFSTR("Predlay");
static CFStringRef kParameterFName = CFSTR("Dry/Wet");
static CFStringRef kParameterGName = CFSTR("Distnce");

enum {
	kParam_A =0,
	kParam_B =1,
	kParam_C =2,
	kParam_D =3,
	kParam_E =4,
	kParam_F =5,
	kParam_G =6,
	//Add your parameters here...
	kNumberOfParameters=7
};

const int d4A = 17; const int d4B = 1832; const int d4C = 1189; const int d4D = 1207; const int d4E = 172; const int d4F = 1740; const int d4G = 165; const int d4H = 43; const int d4I = 47; const int d4J = 53; const int d4K = 644; const int d4L = 61; const int d4M = 67; const int d4N = 71; const int d4O = 73; const int d4P = 79; //3 to 97 ms, 305 seat club Ultralight
// 305seat3DFE on 2026-8-27

//const int d4Q = 5; const int d4R = 4; const int d4S = 4; const int d4T = 4; const int d4U = 2800;
const int predelay = 4097;

#pragma mark ____Ultralight
class Ultralight : public AUEffectBase
{
public:
	Ultralight(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~Ultralight () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new UltralightKernel(this); }
	
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
	virtual ComponentResult		Version() { return kUltralightVersion; }
	
    
	
protected:
		class UltralightKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		UltralightKernel(AUEffectBase *inAudioUnit )
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
		
		float aZ[predelay+5];
		int cZ;
		
		float aA[d4A+5];
		float aB[d4B+5];
		float aC[d4C+5];
		float aD[d4D+5];
		float aE[d4E+5];
		float aF[d4F+5];
		float aG[d4G+5];
		float aH[d4H+5];
		float aI[d4I+5];
		float aJ[d4J+5];
		float aK[d4K+5];
		float aL[d4L+5];
		float aM[d4M+5];
		float aN[d4N+5];
		float aO[d4O+5];
		float aP[d4P+5];
		int cA,cB,cC,cD,cE,cF,cG,cH;
		int cI,cJ,cK,cL,cM,cN,cO,cP;		
		float fA,fB,fC,fD;
		float fhA,fhB,fhC,fhD;
		float flA,flB,flC,flD;
		float hA, hB, hC, hD, hE; //do not allocate in audio code
		
		float firstDry,firstAvg;
		float psA,psB,psC,psD,psE,psF,psG,psH,psI,psJ,psK,psL,psM;
		float psN,psO,psP,psQ,psR,psS,psT,psU,psV,psW,psX,psY,psZ;
		float psa,psb,psc,psd,pse,psf,psg,psh,psi,psj,psk,psl,psm;
		float psn,pso,psp,psq,psr,pss,pst,psu,psv,psw,psx,psy,psz;
		int prevDistance;
		
		float pointCycle, pointAL, pointBL;
		//this is a simple linear interpolation sample rate routine for the plugin version
		
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif