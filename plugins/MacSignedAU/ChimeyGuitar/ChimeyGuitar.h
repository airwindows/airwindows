/*
*	File:		ChimeyGuitar.h
*	
*	Version:	1.0
* 
*	Created:	5/8/25
*	
*	Copyright:  Copyright © 2025 Airwindows, Airwindows uses the MIT license
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
#include "ChimeyGuitarVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __ChimeyGuitar_h__
#define __ChimeyGuitar_h__


#pragma mark ____ChimeyGuitar Parameters

// parameters
static const float kDefaultValue_ParamA = 0.5;
static const float kDefaultValue_ParamB = 0.8;
static const float kDefaultValue_ParamC = 0.7;
static const float kDefaultValue_ParamD = 0.5;
static const float kDefaultValue_ParamE = 0.7;
static const float kDefaultValue_ParamF = 0.4;
static const float kDefaultValue_ParamG = 0.8;
static const float kDefaultValue_ParamH = 0.4;
static const float kDefaultValue_ParamI = 0.3;
static const float kDefaultValue_ParamJ = 1.0;

static CFStringRef kParameterAName = CFSTR("Compres");
static CFStringRef kParameterBName = CFSTR("Presnce");
static CFStringRef kParameterCName = CFSTR("High");
static CFStringRef kParameterDName = CFSTR("Mid");
static CFStringRef kParameterEName = CFSTR("Low");
static CFStringRef kParameterFName = CFSTR("Sub");
static CFStringRef kParameterGName = CFSTR("HSpeakr");
static CFStringRef kParameterHName = CFSTR("LSpeakr");
static CFStringRef kParameterIName = CFSTR("Speed");
static CFStringRef kParameterJName = CFSTR("Output");

enum {
	kParam_A =0,
	kParam_B =1,
	kParam_C =2,
	kParam_D =3,
	kParam_E =4,
	kParam_F =5,
	kParam_G =6,
	kParam_H =7,
	kParam_I =8,
	kParam_J =9,
	//Add your parameters here...
	kNumberOfParameters=10
};

#pragma mark ____ChimeyGuitar
class ChimeyGuitar : public AUEffectBase
{
public:
	ChimeyGuitar(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~ChimeyGuitar () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new ChimeyGuitarKernel(this); }
	
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
	virtual ComponentResult		Version() { return kChimeyGuitarVersion; }
	
    
	
protected:
		class ChimeyGuitarKernel : public AUKernelBase		// most of the real work happens here
	{
public:
		ChimeyGuitarKernel(AUEffectBase *inAudioUnit )
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
		double angS[22][12];
		double angA[22][12];
		double angG[12];
		double iirHPosition[37];
		double iirHAngle[37];
		double iirBPosition[37];
		double iirBAngle[37];
		
		enum {
			bez_AL,
			bez_BL,
			bez_CL,
			bez_InL,
			bez_UnInL,
			bez_SampL,
			bez_cycle,
			bez_total
		}; //the new undersampling. bez signifies the bezier curve reconstruction
		double bezComp[bez_total][12];
		double bezRezA, bezRezB;
		uint32_t fpd;
	};
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif