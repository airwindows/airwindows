/*
*	File:		Thunder.cpp
*	
*	Version:	1.0
* 
*	Created:	9/19/16
*	
*	Copyright:  Copyright © 2016 Airwindows, Airwindows uses the MIT license
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
	Thunder.cpp
	
=============================================================================*/
#include "Thunder.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Thunder)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Thunder::Thunder
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Thunder::Thunder(AudioUnit component)
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
//	Thunder::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Thunder::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Thunder::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Thunder::GetParameterInfo(AudioUnitScope		inScope,
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
//	Thunder::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Thunder::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 Thunder::SupportedNumChannels(const AUChannelInfo ** outInfo)
{
	if (outInfo != NULL)
	{
		static AUChannelInfo info;
		info.inChannels = 2;
		info.outChannels = 2;
		*outInfo = &info;
	}
	
	return 1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Thunder::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Thunder::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Thunder::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Thunder::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ThunderEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Thunder::ThunderKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Thunder::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	muSpeedA = 10000;
	muSpeedB = 10000;
	muCoefficientA = 1;
	muCoefficientB = 1;
	muVary = 1;
	gateL = 0.0;
	gateR = 0.0;
	iirSampleAL = 0.0;
	iirSampleBL = 0.0;
	iirSampleAR = 0.0;
	iirSampleBR = 0.0;
	iirSampleAM = 0.0;
	iirSampleBM = 0.0;
	iirSampleCM = 0.0;
	flip = false;	
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Thunder::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		Thunder::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
											  const AudioBufferList & inBuffer,
											  AudioBufferList & outBuffer,
											  UInt32 			inFramesToProcess)
{
	Float32 * inputL = (Float32*)(inBuffer.mBuffers[0].mData);
	Float32 * inputR = (Float32*)(inBuffer.mBuffers[1].mData);
	Float32 * outputL = (Float32*)(outBuffer.mBuffers[0].mData);
	Float32 * outputR = (Float32*)(outBuffer.mBuffers[1].mData);
	UInt32 nSampleFrames = inFramesToProcess;
	Float64 overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	
	Float64 thunder = GetParameter( kParam_One ) * 0.4;
	Float64 threshold = 1.0 - (thunder * 2.0);
	if (threshold < 0.01) threshold = 0.01;
	Float64 muMakeupGain = 1.0 / threshold;
	Float64 release = pow((1.28-thunder),5)*32768.0;
	release /= overallscale;
	Float64 fastest = sqrt(release);
	Float64 EQ = ((0.0275 / GetSampleRate())*32000.0);
	Float64 dcblock = EQ / 300.0;
	Float64 basstrim = (0.01/EQ)+1.0;
	//FF parameters also ride off Speed
	Float64 outputGain = GetParameter( kParam_Two );
	
	Float64 coefficient;
	Float64 inputSense;
	
	Float64 resultL;
	Float64 resultR;
	Float64 resultM;
	Float64 resultML;
	Float64 resultMR;
	
	double inputSampleL;
	double inputSampleR;
	
	while (nSampleFrames-- > 0) {
		inputSampleL = *inputL;
		inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL = inputSampleL * muMakeupGain;
		inputSampleR = inputSampleR * muMakeupGain;
		
		if (gateL < fabs(inputSampleL)) gateL = inputSampleL;
		else gateL -= dcblock;
		if (gateR < fabs(inputSampleR)) gateR = inputSampleR;
		else gateR -= dcblock;
		//setting up gated DC blocking to control the tendency for rumble and offset
		
		//begin three FathomFive stages
		iirSampleAL += (inputSampleL * EQ * thunder);
		iirSampleAL -= (iirSampleAL * iirSampleAL * iirSampleAL * EQ);
		if (iirSampleAL > gateL) iirSampleAL -= dcblock;
		if (iirSampleAL < -gateL) iirSampleAL += dcblock;
		resultL = iirSampleAL*basstrim;
		iirSampleBL = (iirSampleBL * (1 - EQ)) + (resultL * EQ);
		resultL = iirSampleBL;
		
		iirSampleAR += (inputSampleR * EQ * thunder);
		iirSampleAR -= (iirSampleAR * iirSampleAR * iirSampleAR * EQ);
		if (iirSampleAR > gateR) iirSampleAR -= dcblock;
		if (iirSampleAR < -gateR) iirSampleAR += dcblock;
		resultR = iirSampleAR*basstrim;
		iirSampleBR = (iirSampleBR * (1 - EQ)) + (resultR * EQ);
		resultR = iirSampleBR;
		
		iirSampleAM += ((inputSampleL + inputSampleR) * EQ * thunder);
		iirSampleAM -= (iirSampleAM * iirSampleAM * iirSampleAM * EQ);
		resultM = iirSampleAM*basstrim;
		iirSampleBM = (iirSampleBM * (1 - EQ)) + (resultM * EQ);
		resultM = iirSampleBM;
		iirSampleCM = (iirSampleCM * (1 - EQ)) + (resultM * EQ);
		
		resultM = fabs(iirSampleCM);
		resultML = fabs(resultL);
		resultMR = fabs(resultR);
		
		if (resultM > resultML) resultML = resultM;
		if (resultM > resultMR) resultMR = resultM;
		//trying to restrict the buzziness
		
		if (resultML > 1.0) resultML = 1.0;
		if (resultMR > 1.0) resultMR = 1.0;
		//now we have result L, R and M the trigger modulator which must be 0-1
		
		//begin compressor section
		inputSampleL -= (iirSampleBL * thunder);
		inputSampleR -= (iirSampleBR * thunder);
		//highpass the comp section by sneaking out what will be the reinforcement
		
		inputSense = fabs(inputSampleL);
		if (fabs(inputSampleR) > inputSense)
			inputSense = fabs(inputSampleR);
		//we will take the greater of either channel and just use that, then apply the result
		//to both stereo channels.

		if (flip)
		{
			if (inputSense > threshold)
			{
				muVary = threshold / inputSense;
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
			muNewSpeed = muNewSpeed + fabs(inputSense*release)+fastest;
			muSpeedA = muNewSpeed / muSpeedA;
		}
		else
		{
			if (inputSense > threshold)
			{
				muVary = threshold / inputSense;
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
			muNewSpeed = muNewSpeed + fabs(inputSense*release)+fastest;
			muSpeedB = muNewSpeed / muSpeedB;
		}
		//got coefficients, adjusted speeds
		
		if (flip)
		{
			coefficient = pow(muCoefficientA,2);
			inputSampleL *= coefficient;
			inputSampleR *= coefficient;
		}
		else
		{
			coefficient = pow(muCoefficientB,2);
			inputSampleL *= coefficient;
			inputSampleR *= coefficient;
		}
		//applied compression with vari-vari-µ-µ-µ-µ-µ-µ-is-the-kitten-song o/~
		//applied gain correction to control output level- tends to constrain sound rather than inflate it
		
		inputSampleL += (resultL * resultM);
		inputSampleR += (resultR * resultM);
		//combine the two by adding the summed channnel of lows
		
		if (outputGain != 1.0) {
			inputSampleL *= outputGain;
			inputSampleR *= outputGain;
		}
		flip = !flip;
				
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		*outputL = inputSampleL;
		*outputR = inputSampleR;
		//don't know why we're getting a volume boost, cursed thing
		
		inputL += 1;
		inputR += 1;
		outputL += 1;
		outputR += 1;
	}
	return noErr;
}