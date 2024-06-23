/*
*	File:		DeRez3.cpp
*	
*	Version:	1.0
* 
*	Created:	6/13/24
*	
*	Copyright:  Copyright © 2024 Airwindows, Airwindows uses the MIT license
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
	DeRez3.cpp
	
=============================================================================*/
#include "DeRez3.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, DeRez3)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeRez3::DeRez3
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DeRez3::DeRez3(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeRez3::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeRez3::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeRez3::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeRez3::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamA;
                break;
            case kParam_B:
                AUBase::FillInParameterName (outParameterInfo, kParameterBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamB;
                break;
            case kParam_C:
                AUBase::FillInParameterName (outParameterInfo, kParameterCName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamC;
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
//	DeRez3::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeRez3::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeRez3::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeRez3::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	DeRez3::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult DeRez3::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____DeRez3EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeRez3::DeRez3Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		DeRez3::DeRez3Kernel::Reset()
{
	rezA = 1.0; rezB = 1.0;
	bitA = 1.0; bitB = 1.0;
	wetA = 1.0; wetB = 1.0;
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeRez3::DeRez3Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		DeRez3::DeRez3Kernel::Process(	const Float32 	*inSourceP,
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

	rezA = rezB;
	rezB = pow(GetParameter( kParam_A ),3.0)/overallscale;
	bitA = bitB;
	bitB = (GetParameter( kParam_B)*15.0)+1.0;
	wetA = wetB;
	wetB = GetParameter( kParam_C )*2.0;
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpd * 1.18e-17;
		double drySampleL = inputSampleL;
		
		double temp = (double)nSampleFrames/inFramesToProcess;
		double rez = (rezA*temp)+(rezB*(1.0-temp));
		double bit = (bitA*temp)+(bitB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
		if (rez < 0.0005) rez = 0.0005;
		double bitFactor = pow(2.0,bit);
		double dry = 2.0 - wet;
		if (wet > 1.0) wet = 1.0;
		if (wet < 0.0) wet = 0.0;
		if (dry > 1.0) dry = 1.0;
		if (dry < 0.0) dry = 0.0;
		//this bitcrush makes 50% full dry AND full wet, not crossfaded.
		//that's so it can be on tracks without cutting back dry channel when adjusted
		
		inputSampleL *= bitFactor;		
		inputSampleL = floor(inputSampleL+(0.5/bitFactor));
		inputSampleL /= bitFactor;
		
		bez[bez_cycle] += rez;
		bez[bez_SampL] += (inputSampleL * rez);
		if (bez[bez_cycle] > 1.0) {
			bez[bez_cycle] -= 1.0;
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
		}
		double CBL = (bez[bez_CL]*(1.0-bez[bez_cycle]))+(bez[bez_BL]*bez[bez_cycle]);
		double BAL = (bez[bez_BL]*(1.0-bez[bez_cycle]))+(bez[bez_AL]*bez[bez_cycle]);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-bez[bez_cycle]))+(BAL*bez[bez_cycle]))*0.5;
		
		inputSampleL = (wet*CBAL)+(dry*drySampleL);
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSampleL;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

