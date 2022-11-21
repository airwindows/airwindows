/*
*	File:		Holt2.cpp
*	
*	Version:	1.0
* 
*	Created:	9/19/22
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
/*=============================================================================
	Holt2.cpp
	
=============================================================================*/
#include "Holt2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Holt2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Holt2::Holt2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Holt2::Holt2(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
	SetParameter(kParam_Five, kDefaultValue_ParamFive );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Holt2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Holt2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Holt2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Holt2::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 8.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
                break;
           case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFour;
                break;
           case kParam_Five:
                AUBase::FillInParameterName (outParameterInfo, kParameterFiveName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFive;
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
//	Holt2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Holt2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Holt2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Holt2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Holt2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Holt2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Holt2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Holt2::Holt2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Holt2::Holt2Kernel::Reset()
{
	previousSampleA = 0.0;
	previousTrendA = 0.0;
	previousSampleB = 0.0;
	previousTrendB = 0.0;
	previousSampleC = 0.0;
	previousTrendC = 0.0;
	previousSampleD = 0.0;
	previousTrendD = 0.0;
	previousSampleE = 0.0;
	previousTrendE = 0.0;
	previousSampleF = 0.0;
	previousTrendF = 0.0;
	previousSampleG = 0.0;
	previousTrendG = 0.0;
	previousSampleH = 0.0;
	previousTrendH = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Holt2::Holt2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Holt2::Holt2Kernel::Process(	const Float32 	*inSourceP,
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
	
	Float64 alpha = pow(GetParameter( kParam_One ),4)+0.00001;
	if (alpha > 1.0) alpha = 1.0;
	Float64 resControl = (GetParameter( kParam_Two )*0.15)+0.12;
	Float64 beta = (alpha * pow(resControl,2));
	//0.27 max resonance for full stages on white noise keeping below 0dB
	//0.12 min resonance for not losing all the level as we go down
	//as we remove the 'avoid zero' +0.00001 on beta, our subsonic stability improves
	
	alpha += ((1.0-beta)*pow(GetParameter( kParam_One ),3)); //correct for droop in frequency
	if (alpha > 1.0) alpha = 1.0;
	
	double trend;
	double forecast; //defining these here because we're copying the routine eight times
	
	Float64 aWet = 0.0;
	Float64 bWet = 0.0;
	Float64 cWet = 0.0;
	Float64 dWet = 0.0;
	Float64 eWet = 0.0;
	Float64 fWet = 0.0;
	Float64 gWet = 0.0;
	Float64 hWet = GetParameter( kParam_Three );
	//eight-stage wet/dry control using progressive stages that bypass when not engaged
	if (hWet < 1.0) {aWet = hWet; hWet = 0.0;}
	else if (hWet < 2.0) {bWet = hWet - 1.0; aWet = 1.0; hWet = 0.0;}
	else if (hWet < 3.0) {cWet = hWet - 2.0; bWet = aWet = 1.0; hWet = 0.0;}
	else if (hWet < 4.0) {dWet = hWet - 3.0; cWet = bWet = aWet = 1.0; hWet = 0.0;}
	else if (hWet < 5.0) {eWet = hWet - 4.0; dWet = cWet = bWet = aWet = 1.0; hWet = 0.0;}
	else if (hWet < 6.0) {fWet = hWet - 5.0; eWet = dWet = cWet = bWet = aWet = 1.0; hWet = 0.0;}
	else if (hWet < 7.0) {gWet = hWet - 6.0; fWet = eWet = dWet = cWet = bWet = aWet = 1.0; hWet = 0.0;}
	else {hWet -= 7.0; gWet = fWet = eWet = dWet = cWet = bWet = aWet = 1.0;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	
	Float64 gain = GetParameter( kParam_Four );	
	Float64 wet = GetParameter( kParam_Five );	
	
	while (nSampleFrames-- > 0) {
		long double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		if (aWet > 0.0) {
			trend = (beta * (inputSample - previousSampleA) + ((0.999-beta) * previousTrendA));
			forecast = previousSampleA + previousTrendA;
			inputSample = (alpha * inputSample) + ((0.999-alpha) * forecast);
			previousSampleA = inputSample; previousTrendA = trend;
			inputSample = (inputSample * aWet) + (drySample * (1.0-aWet));
		}
		
		if (bWet > 0.0) {
			trend = (beta * (inputSample - previousSampleB) + ((0.999-beta) * previousTrendB));
			forecast = previousSampleB + previousTrendB;
			inputSample = (alpha * inputSample) + ((0.999-alpha) * forecast);
			previousSampleB = inputSample; previousTrendB = trend;
			inputSample = (inputSample * bWet) + (previousSampleA * (1.0-bWet));
		}
		
		if (cWet > 0.0) {
			trend = (beta * (inputSample - previousSampleC) + ((0.999-beta) * previousTrendC));
			forecast = previousSampleC + previousTrendC;
			inputSample = (alpha * inputSample) + ((0.999-alpha) * forecast);
			previousSampleC = inputSample; previousTrendC = trend;
			inputSample = (inputSample * cWet) + (previousSampleB * (1.0-cWet));
		}
		
		if (dWet > 0.0) {
			trend = (beta * (inputSample - previousSampleD) + ((0.999-beta) * previousTrendD));
			forecast = previousSampleD + previousTrendD;
			inputSample = (alpha * inputSample) + ((0.999-alpha) * forecast);
			previousSampleD = inputSample; previousTrendD = trend;
			inputSample = (inputSample * dWet) + (previousSampleC * (1.0-dWet));
		}
		
		if (eWet > 0.0) {
			trend = (beta * (inputSample - previousSampleE) + ((0.999-beta) * previousTrendE));
			forecast = previousSampleE + previousTrendE;
			inputSample = (alpha * inputSample) + ((0.999-alpha) * forecast);
			previousSampleE = inputSample; previousTrendE = trend;
			inputSample = (inputSample * eWet) + (previousSampleD * (1.0-eWet));
		}
		
		if (fWet > 0.0) {
			trend = (beta * (inputSample - previousSampleF) + ((0.999-beta) * previousTrendF));
			forecast = previousSampleF + previousTrendF;
			inputSample = (alpha * inputSample) + ((0.999-alpha) * forecast);
			previousSampleF = inputSample; previousTrendF = trend;
			inputSample = (inputSample * fWet) + (previousSampleE * (1.0-fWet));
		}
		
		if (gWet > 0.0) {
			trend = (beta * (inputSample - previousSampleG) + ((0.999-beta) * previousTrendG));
			forecast = previousSampleG + previousTrendG;
			inputSample = (alpha * inputSample) + ((0.999-alpha) * forecast);
			previousSampleG = inputSample; previousTrendG = trend;
			inputSample = (inputSample * gWet) + (previousSampleF * (1.0-gWet));
		}
		
		if (hWet > 0.0) {
			trend = (beta * (inputSample - previousSampleH) + ((0.999-beta) * previousTrendH));
			forecast = previousSampleH + previousTrendH;
			inputSample = (alpha * inputSample) + ((0.999-alpha) * forecast);
			previousSampleH = inputSample; previousTrendH = trend;
			inputSample = (inputSample * hWet) + (previousSampleG * (1.0-hWet));
		}
		
		if (gain < 1.0) {
			inputSample *= gain;
		}
		
		if (wet < 1.0) {
			inputSample = (inputSample*wet)+(drySample*(1.0-wet));
		}
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

