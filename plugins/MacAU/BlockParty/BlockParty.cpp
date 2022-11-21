/*
*	File:		BlockParty.cpp
*	
*	Version:	1.0
* 
*	Created:	2/16/19
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
/*=============================================================================
	BlockParty.cpp
	
=============================================================================*/
#include "BlockParty.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(BlockParty)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BlockParty::BlockParty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BlockParty::BlockParty(AudioUnit component)
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
//	BlockParty::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BlockParty::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BlockParty::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BlockParty::GetParameterInfo(AudioUnitScope		inScope,
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
//	BlockParty::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BlockParty::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BlockParty::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BlockParty::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	BlockParty::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult BlockParty::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____BlockPartyEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BlockParty::BlockPartyKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BlockParty::BlockPartyKernel::Reset()
{
	muSpeedA = 10000;
	muSpeedB = 10000;
	muSpeedC = 10000;
	muSpeedD = 10000;
	muSpeedE = 10000;
	muCoefficientA = 1;
	muCoefficientB = 1;
	muCoefficientC = 1;
	muCoefficientD = 1;
	muCoefficientE = 1;
	lastCoefficientA = 1;
	lastCoefficientB = 1;
	lastCoefficientC = 1;
	lastCoefficientD = 1;
	mergedCoefficients = 1;
	threshold = 1.0;
	thresholdB = 1.0;
	muVary = 1;
	count = 1;
	fpFlip = true;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BlockParty::BlockPartyKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BlockParty::BlockPartyKernel::Process(	const Float32 	*inSourceP,
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
	
	Float64 targetthreshold = 1.01 - (1.0-pow(1.0-(GetParameter( kParam_One )*0.5),4));
	Float64 wet = GetParameter( kParam_Two );
	Float64 voicing = 0.618033988749894848204586;
	if (overallscale > 0.0) voicing /= overallscale;
	//translate to desired sample rate, 44.1K is the base
	if (voicing < 0.0) voicing = 0.0;
	if (voicing > 1.0) voicing = 1.0;
	//some insanity checking
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		Float64 muMakeupGain = 1.0 / threshold;
		Float64 outMakeupGain = sqrt(muMakeupGain);
		muMakeupGain += outMakeupGain;
		muMakeupGain *= 0.5;
		//gain settings around threshold
		Float64 release = mergedCoefficients * 32768.0;
		release /= overallscale;
		Float64 fastest = sqrt(release);
		//speed settings around release
		Float64 lastCorrection = mergedCoefficients;
		// µ µ µ µ µ µ µ µ µ µ µ µ is the kitten song o/~
		
		if (muMakeupGain != 1.0) inputSample = inputSample * muMakeupGain;
				
		if (count < 1 || count > 3) count = 1;
		switch (count)
		{
			case 1:
				if (fabs(inputSample) > threshold)
				{
					if (inputSample > 0.0) {
						inputSample = (inputSample * voicing) + (targetthreshold * (1.0-voicing));
						threshold = fabs(inputSample);
					} else {
						inputSample = (inputSample * voicing) - (targetthreshold * (1.0-voicing));
						threshold = fabs(inputSample);
					}
					muVary = targetthreshold / fabs(inputSample);
					muAttack = sqrt(fabs(muSpeedA));
					muCoefficientA = muCoefficientA * (muAttack-1.0);
					if (muVary < threshold)
					{
						muCoefficientA = muCoefficientA + targetthreshold;
					}
					else
					{
						muCoefficientA = muCoefficientA + muVary;
					}
					muCoefficientA = muCoefficientA / muAttack;
				}
				else
				{
					threshold = targetthreshold;
					muCoefficientA = muCoefficientA * ((muSpeedA * muSpeedA)-1.0);
					muCoefficientA = muCoefficientA + 1.0;
					muCoefficientA = muCoefficientA / (muSpeedA * muSpeedA);
				}
				muNewSpeed = muSpeedA * (muSpeedA-1);
				muNewSpeed = muNewSpeed + fabs(inputSample*release)+fastest;
				muSpeedA = muNewSpeed / muSpeedA;
				lastCoefficientA = pow(muCoefficientA,2);
				mergedCoefficients = lastCoefficientB;
				mergedCoefficients += lastCoefficientA;
				lastCoefficientA *= (1.0-lastCorrection);
				lastCoefficientA += (muCoefficientA * lastCorrection);
				lastCoefficientB = lastCoefficientA;
				break;
			case 2:
				if (fabs(inputSample) > threshold)
				{
					if (inputSample > 0.0) {
						inputSample = (inputSample * voicing) + (targetthreshold * (1.0-voicing));
						threshold = fabs(inputSample);
					} else {
						inputSample = (inputSample * voicing) - (targetthreshold * (1.0-voicing));
						threshold = fabs(inputSample);
					}
					muVary = targetthreshold / fabs(inputSample);
					muAttack = sqrt(fabs(muSpeedB));
					muCoefficientB = muCoefficientB * (muAttack-1);
					if (muVary < threshold)
					{
						muCoefficientB = muCoefficientB + targetthreshold;
					}
					else
					{
						muCoefficientB = muCoefficientB + muVary;
					}
					muCoefficientB = muCoefficientB / muAttack;
				}
				else
				{
					threshold = targetthreshold;
					muCoefficientB = muCoefficientB * ((muSpeedB * muSpeedB)-1.0);
					muCoefficientB = muCoefficientB + 1.0;
					muCoefficientB = muCoefficientB / (muSpeedB * muSpeedB);
				}
				muNewSpeed = muSpeedB * (muSpeedB-1);
				muNewSpeed = muNewSpeed + fabs(inputSample*release)+fastest;
				muSpeedB = muNewSpeed / muSpeedB;
				lastCoefficientA = pow(muCoefficientB,2);
				mergedCoefficients = lastCoefficientB;
				mergedCoefficients += lastCoefficientA;
				lastCoefficientA *= (1.0-lastCorrection);
				lastCoefficientA += (muCoefficientB * lastCorrection);
				lastCoefficientB = lastCoefficientA;
				break;
			case 3:
				if (fabs(inputSample) > threshold)
				{
					if (inputSample > 0.0) {
						inputSample = (inputSample * voicing) + (targetthreshold * (1.0-voicing));
						threshold = fabs(inputSample);
					} else {
						inputSample = (inputSample * voicing) - (targetthreshold * (1.0-voicing));
						threshold = fabs(inputSample);
					}
					muVary = targetthreshold / fabs(inputSample);
					muAttack = sqrt(fabs(muSpeedC));
					muCoefficientC = muCoefficientC * (muAttack-1);
					if (muVary < threshold)
					{
						muCoefficientC = muCoefficientC + targetthreshold;
					}
					else
					{
						muCoefficientC = muCoefficientC + muVary;
					}
					muCoefficientC = muCoefficientC / muAttack;
				}
				else
				{
					threshold = targetthreshold;
					muCoefficientC = muCoefficientC * ((muSpeedC * muSpeedC)-1.0);
					muCoefficientC = muCoefficientC + 1.0;
					muCoefficientC = muCoefficientC / (muSpeedC * muSpeedC);
				}
				muNewSpeed = muSpeedC * (muSpeedC-1);
				muNewSpeed = muNewSpeed + fabs(inputSample*release)+fastest;
				muSpeedC = muNewSpeed / muSpeedC;
				lastCoefficientA = pow(muCoefficientC,2);
				mergedCoefficients = lastCoefficientB;
				mergedCoefficients += lastCoefficientA;
				lastCoefficientA *= (1.0-lastCorrection);
				lastCoefficientA += (muCoefficientC * lastCorrection);
				lastCoefficientB = lastCoefficientA;
				break;
		}		
		count++;
		
		//applied compression with vari-vari-µ-µ-µ-µ-µ-µ-is-the-kitten-song o/~
		//applied gain correction to control output level- tends to constrain sound rather than inflate it
		
		if (fpFlip) {
			if (fabs(inputSample) > thresholdB)
			{
				if (inputSample > 0.0) {
					inputSample = (inputSample * voicing) + (targetthreshold * (1.0-voicing));
					thresholdB = fabs(inputSample);
				} else {
					inputSample = (inputSample * voicing) - (targetthreshold * (1.0-voicing));
					thresholdB = fabs(inputSample);
				}
				muVary = targetthreshold / fabs(inputSample);
				muAttack = sqrt(fabs(muSpeedD));
				muCoefficientD = muCoefficientD * (muAttack-1.0);
				if (muVary < thresholdB)
				{
					muCoefficientD = muCoefficientD + targetthreshold;
				}
				else
				{
					muCoefficientD = muCoefficientD + muVary;
				}
				muCoefficientD = muCoefficientD / muAttack;
			}
			else
			{
				thresholdB = targetthreshold;
				muCoefficientD = muCoefficientD * ((muSpeedD * muSpeedD)-1.0);
				muCoefficientD = muCoefficientD + 1.0;
				muCoefficientD = muCoefficientD / (muSpeedD * muSpeedD);
			}
			muNewSpeed = muSpeedD * (muSpeedD-1);
			muNewSpeed = muNewSpeed + fabs(inputSample*release)+fastest;
			muSpeedD = muNewSpeed / muSpeedD;
			lastCoefficientC = pow(muCoefficientE,2);
			mergedCoefficients += lastCoefficientD;
			mergedCoefficients += lastCoefficientC;
			lastCoefficientC *= (1.0-lastCorrection);
			lastCoefficientC += (muCoefficientD * lastCorrection);
			lastCoefficientD = lastCoefficientC;
		} else {
			if (fabs(inputSample) > thresholdB)
			{
				if (inputSample > 0.0) {
					inputSample = (inputSample * voicing) + (targetthreshold * (1.0-voicing));
					thresholdB = fabs(inputSample);
				} else {
					inputSample = (inputSample * voicing) - (targetthreshold * (1.0-voicing));
					thresholdB = fabs(inputSample);
				}
				muVary = targetthreshold / fabs(inputSample);
				muAttack = sqrt(fabs(muSpeedE));
				muCoefficientE = muCoefficientE * (muAttack-1.0);
				if (muVary < thresholdB)
				{
					muCoefficientE = muCoefficientE + targetthreshold;
				}
				else
				{
					muCoefficientE = muCoefficientE + muVary;
				}
				muCoefficientE = muCoefficientE / muAttack;
			}
			else
			{
				thresholdB = targetthreshold;
				muCoefficientE = muCoefficientE * ((muSpeedE * muSpeedE)-1.0);
				muCoefficientE = muCoefficientE + 1.0;
				muCoefficientE = muCoefficientE / (muSpeedE * muSpeedE);
			}
			muNewSpeed = muSpeedE * (muSpeedE-1);
			muNewSpeed = muNewSpeed + fabs(inputSample*release)+fastest;
			muSpeedE = muNewSpeed / muSpeedE;
			lastCoefficientC = pow(muCoefficientE,2);
			mergedCoefficients += lastCoefficientD;
			mergedCoefficients += lastCoefficientC;
			lastCoefficientC *= (1.0-lastCorrection);
			lastCoefficientC += (muCoefficientE * lastCorrection);
			lastCoefficientD = lastCoefficientC;
		}
		mergedCoefficients *= 0.25;
		inputSample *= mergedCoefficients;
		
		if (outMakeupGain != 1.0) inputSample = inputSample * outMakeupGain;
		
		fpFlip = !fpFlip;
		
		if (wet < 1.0) {
			inputSample = (inputSample * wet) + (drySample * (1.0-wet));
		}
		
		if (inputSample > 0.999) inputSample = 0.999;
		if (inputSample < -0.999) inputSample = -0.999;
		//iron bar clip comes after the dry/wet: alternate way to clean things up
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

