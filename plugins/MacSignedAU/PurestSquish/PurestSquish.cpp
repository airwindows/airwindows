/*
*	File:		PurestSquish.cpp
*	
*	Version:	1.0
* 
*	Created:	11/29/18
*	
*	Copyright:  Copyright © 2018 Airwindows, Airwindows uses the MIT license
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
	PurestSquish.cpp
	
=============================================================================*/
#include "PurestSquish.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, PurestSquish)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestSquish::PurestSquish
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PurestSquish::PurestSquish(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestSquish::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PurestSquish::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestSquish::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PurestSquish::GetParameterInfo(AudioUnitScope		inScope,
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
            case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFour;
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
//	PurestSquish::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PurestSquish::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestSquish::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PurestSquish::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	PurestSquish::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult PurestSquish::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____PurestSquishEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestSquish::PurestSquishKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		PurestSquish::PurestSquishKernel::Reset()
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
	iirSampleA = 0.0;
	iirSampleB = 0.0;
	iirSampleC = 0.0;
	iirSampleD = 0.0;
	iirSampleE = 0.0;
	lastCoefficientA = 1;
	lastCoefficientB = 1;
	lastCoefficientC = 1;
	lastCoefficientD = 1;
	mergedCoefficients = 1;
	muVary = 1;
	count = 1;
	fpFlip = true;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestSquish::PurestSquishKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		PurestSquish::PurestSquishKernel::Process(	const Float32 	*inSourceP,
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
	
	double highpassSample;
	double highpassSampleB;
	Float64 threshold = 1.01 - (1.0-pow(1.0-(GetParameter( kParam_One )*0.5),4));
	Float64 iirAmount = pow(GetParameter( kParam_Two ),4)/overallscale;
	Float64 output = GetParameter( kParam_Three );
	Float64 wet = GetParameter( kParam_Four );
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;

		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		Float64 muMakeupGain = 1.0 / threshold;
		Float64 outMakeupGain = sqrt(muMakeupGain);
		muMakeupGain += outMakeupGain;
		muMakeupGain *= 0.5;
		outMakeupGain *= 0.5;
		//gain settings around threshold
		Float64 release = mergedCoefficients * 32768.0;
		release /= overallscale;
		Float64 fastest = sqrt(release);
		//speed settings around release
		Float64 lastCorrection = mergedCoefficients;
		// µ µ µ µ µ µ µ µ µ µ µ µ is the kitten song o/~
		
		if (muMakeupGain != 1.0) inputSample = inputSample * muMakeupGain;
		
		highpassSampleB = highpassSample = inputSample;
		
		if (count < 1 || count > 3) count = 1;
		switch (count)
		{
			case 1:
				iirSampleA = (iirSampleA * (1 - iirAmount)) + (highpassSample * iirAmount);
				highpassSample -= iirSampleA;
				if (fabs(highpassSample) > threshold)
				{
					muVary = threshold / fabs(highpassSample);
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
				muNewSpeed = muNewSpeed + fabs(highpassSample*release)+fastest;
				muSpeedA = muNewSpeed / muSpeedA;
				lastCoefficientA = pow(muCoefficientA,2);
				mergedCoefficients = lastCoefficientB;
				mergedCoefficients += lastCoefficientA;
				lastCoefficientA *= (1.0 - lastCorrection);
				lastCoefficientA += (muCoefficientA * lastCorrection);
				lastCoefficientB = lastCoefficientA;
				break;
			case 2:
				iirSampleB = (iirSampleB * (1 - iirAmount)) + (highpassSample * iirAmount);
				highpassSample -= iirSampleB;
				if (fabs(highpassSample) > threshold)
				{
					muVary = threshold / fabs(highpassSample);
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
				muNewSpeed = muNewSpeed + fabs(highpassSample*release)+fastest;
				muSpeedB = muNewSpeed / muSpeedB;
				lastCoefficientA = pow(muCoefficientB,2);
				mergedCoefficients = lastCoefficientB;
				mergedCoefficients += lastCoefficientA;
				lastCoefficientA *= (1.0 - lastCorrection);
				lastCoefficientA += (muCoefficientB * lastCorrection);
				lastCoefficientB = lastCoefficientA;
				break;
			case 3:
				iirSampleC = (iirSampleC * (1 - iirAmount)) + (highpassSample * iirAmount);
				highpassSample -= iirSampleC;
				if (fabs(highpassSample) > threshold)
				{
					muVary = threshold / fabs(highpassSample);
					muAttack = sqrt(fabs(muSpeedC));
					muCoefficientC = muCoefficientC * (muAttack-1);
					if (muVary < threshold)
					{
						muCoefficientC = muCoefficientC + threshold;
					}
					else
					{
						muCoefficientC = muCoefficientC + muVary;
					}
					muCoefficientC = muCoefficientC / muAttack;
				}
				else
				{
					muCoefficientC = muCoefficientC * ((muSpeedC * muSpeedC)-1.0);
					muCoefficientC = muCoefficientC + 1.0;
					muCoefficientC = muCoefficientC / (muSpeedC * muSpeedC);
				}
				muNewSpeed = muSpeedC * (muSpeedC-1);
				muNewSpeed = muNewSpeed + fabs(highpassSample*release)+fastest;
				muSpeedC = muNewSpeed / muSpeedC;
				lastCoefficientA = pow(muCoefficientC,2);
				mergedCoefficients = lastCoefficientB;
				mergedCoefficients += lastCoefficientA;
				lastCoefficientA *= (1.0 - lastCorrection);
				lastCoefficientA += (muCoefficientC * lastCorrection);
				lastCoefficientB = lastCoefficientA;
				break;
		}		
		count++;
		
		//applied compression with vari-vari-µ-µ-µ-µ-µ-µ-is-the-kitten-song o/~
		//applied gain correction to control output level- tends to constrain sound rather than inflate it
		
		if (fpFlip) {
			iirSampleD = (iirSampleD * (1 - iirAmount)) + (highpassSampleB * iirAmount);
			highpassSampleB -= iirSampleD;
			if (fabs(highpassSampleB) > threshold)
			{
				muVary = threshold / fabs(highpassSampleB);
				muAttack = sqrt(fabs(muSpeedD));
				muCoefficientD = muCoefficientD * (muAttack-1.0);
				if (muVary < threshold)
				{
					muCoefficientD = muCoefficientD + threshold;
				}
				else
				{
					muCoefficientD = muCoefficientD + muVary;
				}
				muCoefficientD = muCoefficientD / muAttack;
			}
			else
			{
				muCoefficientD = muCoefficientD * ((muSpeedD * muSpeedD)-1.0);
				muCoefficientD = muCoefficientD + 1.0;
				muCoefficientD = muCoefficientD / (muSpeedD * muSpeedD);
			}
			muNewSpeed = muSpeedD * (muSpeedD-1);
			muNewSpeed = muNewSpeed + fabs(highpassSampleB*release)+fastest;
			muSpeedD = muNewSpeed / muSpeedD;
			lastCoefficientC = pow(muCoefficientE,2);
			mergedCoefficients += lastCoefficientD;
			mergedCoefficients += lastCoefficientC;
			lastCoefficientC *= (1.0 - lastCorrection);
			lastCoefficientC += (muCoefficientD * lastCorrection);
			lastCoefficientD = lastCoefficientC;
		} else {
			iirSampleE = (iirSampleE * (1 - iirAmount)) + (highpassSampleB * iirAmount);
			highpassSampleB -= iirSampleE;
			if (fabs(highpassSampleB) > threshold)
			{
				muVary = threshold / fabs(highpassSampleB);
				muAttack = sqrt(fabs(muSpeedE));
				muCoefficientE = muCoefficientE * (muAttack-1.0);
				if (muVary < threshold)
				{
					muCoefficientE = muCoefficientE + threshold;
				}
				else
				{
					muCoefficientE = muCoefficientE + muVary;
				}
				muCoefficientE = muCoefficientE / muAttack;
			}
			else
			{
				muCoefficientE = muCoefficientE * ((muSpeedE * muSpeedE)-1.0);
				muCoefficientE = muCoefficientE + 1.0;
				muCoefficientE = muCoefficientE / (muSpeedE * muSpeedE);
			}
			muNewSpeed = muSpeedE * (muSpeedE-1);
			muNewSpeed = muNewSpeed + fabs(highpassSampleB*release)+fastest;
			muSpeedE = muNewSpeed / muSpeedE;
			lastCoefficientC = pow(muCoefficientE,2);
			mergedCoefficients += lastCoefficientD;
			mergedCoefficients += lastCoefficientC;
			lastCoefficientC *= (1.0 - lastCorrection);
			lastCoefficientC += (muCoefficientE * lastCorrection);
			lastCoefficientD = lastCoefficientC;
		}
		mergedCoefficients *= 0.25;
		inputSample *= mergedCoefficients;
		
		if (outMakeupGain != 1.0) inputSample = inputSample * outMakeupGain;
		
		fpFlip = !fpFlip;
		
		if (output < 1.0) {
			inputSample *= output;
		}
		
		if (wet < 1.0) {
			inputSample = (inputSample * wet) + (drySample * (1.0-wet));
		}
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

