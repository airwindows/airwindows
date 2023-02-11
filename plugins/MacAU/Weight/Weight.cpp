/*
 *	File:		Weight.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	1/31/23
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
/*=============================================================================
 Weight.cpp
 
 =============================================================================*/
#include "Weight.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Weight)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Weight::Weight
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Weight::Weight(AudioUnit component)
: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Weight::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Weight::GetParameterValueStrings(AudioUnitScope		inScope,
														 AudioUnitParameterID	inParameterID,
														 CFArrayRef *		outStrings)
{
	
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Weight::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Weight::GetParameterInfo(AudioUnitScope		inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;
	
	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
	|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
			case kParam_One:
                AUBase::FillInParameterName (outParameterInfo, kParameterOneName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
 				outParameterInfo.unitName = kParameterOneUnit;
                outParameterInfo.minValue = 20.0;
                outParameterInfo.maxValue = 120.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
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
//	Weight::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Weight::GetPropertyInfo (AudioUnitPropertyID	inID,
												 AudioUnitScope		inScope,
												 AudioUnitElement	inElement,
												 UInt32 &		outDataSize,
												 Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Weight::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Weight::GetProperty(	AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 	inElement,
											void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Weight::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Weight::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____WeightEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Weight::WeightKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Weight::WeightKernel::Reset()
{
	
	for (int i = 0; i < 8; i++) {
		previousSample[i] = 0.0;
		previousTrend[i] = 0.0;
	}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Weight::WeightKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Weight::WeightKernel::Process(	const Float32 	*inSourceP,
										  Float32		 	*inDestP,
										  UInt32 			inFramesToProcess,
										  UInt32			inNumChannels, 
										  bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	long double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	
	Float64 targetFreq = (GetParameter( kParam_One )-20.0)/100.0;
	//gives us a 0-1 value like the VST will be. For the VST, start with 0-1 and
	//have the plugin display the number as 20-120.
	
	targetFreq = ((targetFreq+0.53)*0.2)/sqrt(overallscale);
	//must use square root of what the real scale would be, to get correct output
	Float64 alpha = pow(targetFreq,4);
	Float64 out = GetParameter( kParam_Two );
	Float64 resControl = (out*0.05)+0.2;
	Float64 beta = (alpha * pow(resControl,2));	
	alpha += ((1.0-beta)*pow(targetFreq,3)); //correct for droop in frequency
		
	while (nSampleFrames-- > 0) {
		long double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		//begin Weight
		double trend;
		double forecast;
		for (int i = 0; i < 8; i++) {
			trend = (beta * (inputSample - previousSample[i]) + ((0.999-beta) * previousTrend[i]));
			forecast = previousSample[i] + previousTrend[i];
			inputSample = (alpha * inputSample) + ((0.999-alpha) * forecast);
			previousSample[i] = inputSample; previousTrend[i] = trend;
		}
		//inputSample is now the bass boost to be added
		
		inputSample *= out;
		inputSample += drySample;
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

