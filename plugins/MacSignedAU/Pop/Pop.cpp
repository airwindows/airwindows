/*
*	File:		Pop.cpp
*	
*	Version:	1.0
* 
*	Created:	10/9/10
*	
*	Copyright:  Copyright © 2010 Airwindows, Airwindows uses the MIT license
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
	Pop.h
	
=============================================================================*/
#include "Pop.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Pop)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Pop::Pop
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pop::Pop(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
        
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Pop::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Pop::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Pop::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Pop::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
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
//	Pop::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Pop::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Pop::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Pop::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Pop::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Pop::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____PopEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Pop::PopKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Pop::PopKernel::Reset()
{
	for(int count = 0; count < 10000; count++) {d[count] = 0;}
	delay = 0;
	muSpeedA = 10000;
	muSpeedB = 10000;
	muCoefficientA = 1;
	muCoefficientB = 1;
	thicken = 1;
	muVary = 1;
	flip = false;
	previous = 0.0;
	previous2 = 0.0;
	previous3 = 0.0;
	previous4 = 0.0;
	previous5 = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Pop::PopKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Pop::PopKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels,
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	Float64 overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	
	Float64 highGainOffset = pow(GetParameter( kParam_One ),2)*0.023;
	Float64 threshold = 1.001 - (1.0-pow(1.0-GetParameter( kParam_One ),5));
	Float64 muMakeupGain = sqrt(1.0 / threshold);
	//gain settings around threshold
	Float64 release = (GetParameter( kParam_One )*100000.0) + 300000.0;
	int maxdelay = (int)(1450.0 * overallscale);
	if (maxdelay > 9999) maxdelay = 9999;
	release /= overallscale;
	Float64 fastest = sqrt(release);
	//speed settings around release
	Float64 output = GetParameter( kParam_Two );
	Float64 wet = GetParameter( kParam_Three );
	// µ µ µ µ µ µ µ µ µ µ µ µ is the kitten song o/~
	
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		d[delay] = inputSample;
		delay--;
		if (delay < 0 || delay > maxdelay) {delay = maxdelay;}
		//yes this is a second bounds check. it's cheap, check EVERY time
		inputSample = (inputSample * thicken) + (d[delay] * (1.0-thicken));
		
		double lowestSample = inputSample;
		if (fabs(inputSample) > fabs(previous)) lowestSample = previous;
		if (fabs(lowestSample) > fabs(previous2)) lowestSample = (lowestSample + previous2) / 1.99;
		if (fabs(lowestSample) > fabs(previous3)) lowestSample = (lowestSample + previous3) / 1.98;
		if (fabs(lowestSample) > fabs(previous4)) lowestSample = (lowestSample + previous4) / 1.97;
		if (fabs(lowestSample) > fabs(previous5)) lowestSample = (lowestSample + previous5) / 1.96;
		previous5 = previous4;
		previous4 = previous3;
		previous3 = previous2;
		previous2 = previous;
		previous = inputSample;
		inputSample *= muMakeupGain;
		Float64 punchiness = 0.95-fabs(inputSample*0.08);
		if (punchiness < 0.65) punchiness = 0.65;

		
		if (flip)
			{
				if (fabs(lowestSample) > threshold)
					{
						muVary = threshold / fabs(lowestSample);
						muAttack = sqrt(fabs(muSpeedA));
						muCoefficientA = muCoefficientA * (muAttack-1.0);
						if (muVary < threshold)
							{
								muCoefficientA = muCoefficientA + threshold;
							}
						else
							{
								muCoefficientA = muCoefficientA + muVary;
							}
						muCoefficientA = muCoefficientA / muAttack;
					}
				else
					{
						muCoefficientA = muCoefficientA * ((muSpeedA * muSpeedA)-1.0);
						muCoefficientA = muCoefficientA + 1.0;
						muCoefficientA = muCoefficientA / (muSpeedA * muSpeedA);
					}
				muNewSpeed = muSpeedA * (muSpeedA-1);
				muNewSpeed = muNewSpeed + fabs(lowestSample*release)+fastest;
				muSpeedA = muNewSpeed / muSpeedA;
			}
		else
			{
				if (fabs(lowestSample) > threshold)
					{
						muVary = threshold / fabs(lowestSample);
						muAttack = sqrt(fabs(muSpeedB));
						muCoefficientB = muCoefficientB * (muAttack-1);
						if (muVary < threshold)
							{
								muCoefficientB = muCoefficientB + threshold;
							}
						else
							{
								muCoefficientB = muCoefficientB + muVary;
							}
						muCoefficientB = muCoefficientB / muAttack;
					}
				else
					{
						muCoefficientB = muCoefficientB * ((muSpeedB * muSpeedB)-1.0);
						muCoefficientB = muCoefficientB + 1.0;
						muCoefficientB = muCoefficientB / (muSpeedB * muSpeedB);
					}
				muNewSpeed = muSpeedB * (muSpeedB-1);
				muNewSpeed = muNewSpeed + fabs(lowestSample*release)+fastest;
				muSpeedB = muNewSpeed / muSpeedB;
			}
		//got coefficients, adjusted speeds
		double coefficient = highGainOffset;
		if (flip) coefficient += pow(muCoefficientA,2);
		else coefficient += pow(muCoefficientB,2);
		inputSample *= coefficient;
		thicken = (coefficient/5)+punchiness;//0.80;
		thicken = (1.0-wet)+(wet*thicken);
		//applied compression with vari-vari-µ-µ-µ-µ-µ-µ-is-the-kitten-song o/~
		//applied gain correction to control output level- tends to constrain sound rather than inflate it
		
		double bridgerectifier = fabs(inputSample);
		if (bridgerectifier > 1.2533141373155) bridgerectifier = 1.2533141373155;
		bridgerectifier = sin(bridgerectifier * fabs(bridgerectifier)) / ((fabs(bridgerectifier) == 0.0) ?1:fabs(bridgerectifier));
		//using Spiral instead of Density algorithm
		if (inputSample > 0) inputSample = (inputSample*coefficient)+(bridgerectifier*(1-coefficient));
		else inputSample = (inputSample*coefficient)-(bridgerectifier*(1-coefficient));
		//second stage of overdrive to prevent overs and allow bloody loud extremeness
		flip = !flip;
		
		if (output < 1.0) inputSample *= output;
		if (wet < 1.0) inputSample = (drySample*(1.0-wet))+(inputSample*wet);
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		//end 32 bit floating point dither

		*destP = inputSample;
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

