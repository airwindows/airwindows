/*
*	File:		ADClip9.cpp
*	
*	Version:	1.0
* 
*	Created:	4/14/26
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
/*=============================================================================
	ADClip9.cpp
	
=============================================================================*/
#include "ADClip9.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(ADClip9)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip9::ADClip9
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADClip9::ADClip9(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
	SetParameter(kParam_D, kDefaultValue_ParamD );
	SetParameter(kParam_E, kDefaultValue_ParamE );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip9::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ADClip9::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_E)) //ID must be actual name of parameter identifier, not number
	{
		if (outStrings == NULL) return noErr;
		CFStringRef strings [] =
		{
			kMenuItem_Boost,
			kMenuItem_Match,
			kMenuItem_ClipOnly,
		};
		*outStrings = CFArrayCreate (
									 NULL,
									 (const void **) strings,
									 (sizeof (strings) / sizeof (strings [0])),
									 NULL
									 );
		return noErr;
	}
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip9::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ADClip9::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_A:
                AUBase::FillInParameterName (outParameterInfo, kParameterAName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 18.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamA;
                break;
            case kParam_B:
                AUBase::FillInParameterName (outParameterInfo, kParameterBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 18.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamB;
                break;
            case kParam_C:
                AUBase::FillInParameterName (outParameterInfo, kParameterCName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamC;
                break;
            case kParam_D:
                AUBase::FillInParameterName (outParameterInfo, kParameterDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamD;
                break;
            case kParam_E:
                AUBase::FillInParameterName (outParameterInfo, kParameterEName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kBoost;
                outParameterInfo.maxValue = kClipOnly;
                outParameterInfo.defaultValue = kDefaultValue_ParamE;
                break;
           default:
                result = kAudioUnitErr_InvalidParameter;
                break;
            }
	} else {
        result = kAudioUnitErr_InvalidParameter;
    }
    


	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip9::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ADClip9::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip9::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ADClip9::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ADClip9::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ADClip9::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ADClip9EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip9::ADClip9Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ADClip9::ADClip9Kernel::Reset()
{
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	for (int x = 0; x < 17; x++) intermediateL[x] = 0.0;
	for (int x = 0; x < 33; x++) slewL[x] = 0.0;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip9::ADClip9Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ADClip9::ADClip9Kernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;

	double inputGain = pow(10.0,(GetParameter( kParam_A ))/20.0);
	double match = pow(10.0,(GetParameter( kParam_B ))/20.0);
	double noiseLevel = 1.0-pow(1.0-GetParameter( kParam_C ),2.0);
	double ceiling = 1.0-pow(1.0-GetParameter( kParam_D ),2.0);
	int mode = (int) GetParameter( kParam_E );
	
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		
		switch (mode)
		{
			case 1: inputSampleL *= inputGain; break; //Boost
			case 2: inputSampleL *= match; break; //Match
			case 3: inputSampleL *= inputGain; break; //ClipOnly
		}
		//this is our output mode switch, showing the effects
				
		double overshootL = lastDryL;
		lastDryL = inputSampleL;
		
		//begin ClipOnly3 as a little, compressed chunk that can be dropped into code
		double noise = 1.0-((double(fpdL)/UINT32_MAX)*(1.0-noiseLevel));//0.076
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=(noiseLevel*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = ceiling;
		} wasPosClipL = false;
		if (inputSampleL>noiseLevel) {wasPosClipL=true;inputSampleL=(noiseLevel*noise)+(lastSampleL*(1.0-noise));}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=(-noiseLevel*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = -ceiling;
		} wasNegClipL = false;
		if (inputSampleL<-noiseLevel) {wasNegClipL=true;inputSampleL=(-noiseLevel*noise)+(lastSampleL*(1.0-noise));}
		slewL[spacing*2] = fabs(lastSampleL-inputSampleL);
		for (int x = spacing*2; x > 0; x--) slewL[x-1] = slewL[x];
		intermediateL[spacing] = inputSampleL; inputSampleL = lastSampleL;
		//Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateL[x-1] = intermediateL[x];} lastSampleL = intermediateL[0];
		if (wasPosClipL || wasNegClipL) {
			for (int x = spacing; x > 0; x--) lastSampleL += intermediateL[x];
			lastSampleL /= spacing;
		} double finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewL[x]) finalSlew = slewL[x];
		double postclip = ceiling / (1.0+(finalSlew*1.3986013));
		if (inputSampleL > postclip) inputSampleL = postclip; if (inputSampleL < -postclip) inputSampleL = -postclip;
		//end ClipOnly3 as a little, compressed chunk that can be dropped into code
		
		switch (mode)
		{
			case 1: break; //Boost
			case 2: inputSampleL /= match; break; //Match
			case 3: inputSampleL = (inputSampleL-overshootL)/inputGain; break; //Clip Only
		}
		//this is our output mode switch, showing the effects
	
		//begin 32 bit floating point dither
		//int expon; frexpf((float)inputSample, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSampleL;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

