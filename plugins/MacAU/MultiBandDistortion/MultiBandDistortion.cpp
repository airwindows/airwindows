/*
*	File:		MultiBandDistortion.cpp
*	
*	Version:	1.0
* 
*	Created:	4/24/11
*	
*	Copyright:  Copyright © 2011 Airwindows, Airwindows uses the MIT license
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
	MultiBandDistortion.h
	
=============================================================================*/
#include "MultiBandDistortion.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(MultiBandDistortion)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MultiBandDistortion::MultiBandDistortion
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MultiBandDistortion::MultiBandDistortion(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
	SetParameter(kParam_Five, kDefaultValue_ParamFive );
	SetParameter(kParam_Six, kDefaultValue_ParamSix );
	SetParameter(kParam_Seven, kDefaultValue_ParamSeven );
	SetParameter(kParam_Eight, kDefaultValue_ParamEight );
	SetParameter(kParam_Nine, kDefaultValue_ParamNine );
        
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MultiBandDistortion::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		MultiBandDistortion::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MultiBandDistortion::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		MultiBandDistortion::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 48.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 48.0;
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
           case kParam_Six:
                AUBase::FillInParameterName (outParameterInfo, kParameterSixName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSix;
                break;
           case kParam_Seven:
                AUBase::FillInParameterName (outParameterInfo, kParameterSevenName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSeven;
                break;
           case kParam_Eight:
                AUBase::FillInParameterName (outParameterInfo, kParameterEightName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamEight;
                break;
           case kParam_Nine:
                AUBase::FillInParameterName (outParameterInfo, kParameterNineName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -48.0;
                outParameterInfo.maxValue = 0.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamNine;
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
//	MultiBandDistortion::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		MultiBandDistortion::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MultiBandDistortion::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		MultiBandDistortion::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MultiBandDistortion::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult MultiBandDistortion::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____MultiBandDistortionEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MultiBandDistortion::MultiBandDistortionKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		MultiBandDistortion::MultiBandDistortionKernel::Reset()
{
	ataLast3Sample = ataLast2Sample = ataLast1Sample = 0.0;
	ataHalfwaySample = ataHalfDrySample = ataHalfDiffSample = 0.0;
	ataA = ataB = ataC = ataDrySample = ataDiffSample = ataPrevDiffSample = 0.0;
	ataUpsampleHighTweak = 0.0414213562373095048801688; //more adds treble to upsampling
	ataDecay = 0.915965594177219015; //Catalan's constant, more adds focus and clarity
	ataFlip = false; //end reset of antialias parameters
	iirSampleA = 0; iirSampleB = 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MultiBandDistortion::MultiBandDistortionKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		MultiBandDistortion::MultiBandDistortionKernel::Process(	const Float32 	*inSourceP,
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
	
	Float64 iirAmount = pow(GetParameter( kParam_One ),3)/overallscale;
	Float64 gainH = pow(10.0,GetParameter( kParam_Two )/20);

	Float64 thresholdH = GetParameter( kParam_Four );
	Float64 hardnessH;
	if (thresholdH < 1.0) hardnessH = 1.0 / (1.0-thresholdH);
	else hardnessH = 999999999999999999999.0;

	Float64 gainL = pow(10.0,GetParameter( kParam_Three )/20);
	Float64 thresholdL = GetParameter( kParam_Five );
	Float64 hardnessL;
	if (thresholdL < 1.0) hardnessL = 1.0 / (1.0-thresholdL);
	else hardnessL = 999999999999999999999.0;
	
	Float64 breakup = 1.5707963267949;
	Float64 bridgerectifier;

	Float64 outputH = GetParameter( kParam_Six );
	Float64 outputL = GetParameter( kParam_Seven );
	Float64 outputD = GetParameter( kParam_Eight )*0.597;
	Float64 outtrim = outputH + outputL + outputD;
	outputH *= outtrim;
	outputL *= outtrim;
	outputD *= outtrim;
	Float64 outputGlobal = pow(10.0,GetParameter( kParam_Nine )/20);
	

	Float64 inputSample;
	Float64 tempH;
	Float64 tempL;
	
	while (nSampleFrames-- > 0) {
		ataDrySample = inputSample = *sourceP;
		ataHalfDrySample = ataHalfwaySample = (inputSample + ataLast1Sample + ((-ataLast2Sample + ataLast3Sample) * ataUpsampleHighTweak)) / 2.0;
		ataLast3Sample = ataLast2Sample; ataLast2Sample = ataLast1Sample; ataLast1Sample = inputSample;
		//setting up oversampled special antialiasing
		
	//pre-center code on inputSample and halfwaySample in parallel
//begin interpolated sample- change inputSample -> ataHalfwaySample, ataDrySample -> ataHalfDrySample
		tempL = iirSampleA = (iirSampleA * (1 - iirAmount)) + (ataHalfwaySample * iirAmount);
		tempH = ataHalfwaySample - iirSampleA;
		//highpass section


		tempH *= gainH;
		if (fabs(tempH) > thresholdH)
			{
				bridgerectifier = (fabs(tempH)-thresholdH)*hardnessH;
				//skip flat area if any, scale to distortion limit
				if (bridgerectifier > breakup) bridgerectifier = breakup;
				//max value for sine function, 'breakup' modeling for trashed console tone
				//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
				bridgerectifier = sin(bridgerectifier)/hardnessH;
				//do the sine factor, scale back to proper amount
				if (tempH > 0) tempH = bridgerectifier+thresholdH;
				else tempH = -(bridgerectifier+thresholdH);
			}
			//ADClip
		tempL *= gainL;
		if (fabs(tempL) > thresholdL)
			{
				bridgerectifier = (fabs(tempL)-thresholdL)*hardnessL;
				//skip flat area if any, scale to distortion limit
				if (bridgerectifier > breakup) bridgerectifier = breakup;
				//max value for sine function, 'breakup' modeling for trashed console tone
				//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
				bridgerectifier = sin(bridgerectifier)/hardnessL;
				//do the sine factor, scale back to proper amount
				if (tempL > 0) tempL = bridgerectifier+thresholdL;
				else tempL = -(bridgerectifier+thresholdL);
			}
			//ADClip
			ataHalfwaySample = (tempL * outputL) + (tempH * outputH);
	//end interpolated sample


//begin raw sample- inputSample and ataDrySample handled separately here
		tempL = iirSampleB = (iirSampleB * (1 - iirAmount)) + (inputSample * iirAmount);
		tempH = inputSample - iirSampleB;
		//highpass section

		tempH *= gainH;
		if (fabs(tempH) > thresholdH)
			{
				bridgerectifier = (fabs(tempH)-thresholdH)*hardnessH;
				//skip flat area if any, scale to distortion limit
				if (bridgerectifier > breakup) bridgerectifier = breakup;
				//max value for sine function, 'breakup' modeling for trashed console tone
				//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
				bridgerectifier = sin(bridgerectifier)/hardnessH;
				//do the sine factor, scale back to proper amount
				if (tempH > 0) tempH = bridgerectifier+thresholdH;
				else tempH = -(bridgerectifier+thresholdH);
			}
			//ADClip
		tempL *= gainL;
		if (fabs(tempL) > thresholdL)
			{
				bridgerectifier = (fabs(tempL)-thresholdL)*hardnessL;
				//skip flat area if any, scale to distortion limit
				if (bridgerectifier > breakup) bridgerectifier = breakup;
				//max value for sine function, 'breakup' modeling for trashed console tone
				//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
				bridgerectifier = sin(bridgerectifier)/hardnessL;
				//do the sine factor, scale back to proper amount
				if (tempL > 0) tempL = bridgerectifier+thresholdL;
				else tempL = -(bridgerectifier+thresholdL);
			}
			//ADClip
			inputSample = (tempL * outputL) + (tempH * outputH);
	//end raw sample
	
		
//begin center code handling conv stuff tied to 44.1K, or stuff in time domain like delays
		//ataHalfwaySample -= inputSample;
		//retain only difference with raw signal
		
		//inputSample += convolutionstuff[count];

		//ataHalfwaySample += inputSample;
		//restore interpolated signal including time domain stuff now
	//end center code for handling timedomain/conv stuff
		
	//post-center code on inputSample and halfwaySample in parallel
//begin raw sample- inputSample and ataDrySample handled separately here
		//inputSample *= gain;
	//end raw sample
	
//begin interpolated sample- change inputSample -> ataHalfwaySample, ataDrySample -> ataHalfDrySample
		//ataHalfwaySample *= gain;
	//end interpolated sample
	
		//begin antialiasing section for halfway sample
		ataC = ataHalfwaySample - ataHalfDrySample;
		if (ataFlip) {ataA *= ataDecay; ataB *= ataDecay; ataA += ataC; ataB -= ataC; ataC = ataA;}
		else {ataB *= ataDecay; ataA *= ataDecay; ataB += ataC; ataA -= ataC; ataC = ataB;}
		ataHalfDiffSample = (ataC * ataDecay);
		//end antialiasing section for halfway sample
		
		//begin antialiasing section for raw sample
		ataC = inputSample - ataDrySample;
		if (ataFlip) {ataA *= ataDecay; ataB *= ataDecay; ataA += ataC; ataB -= ataC; ataC = ataA;}
		else {ataB *= ataDecay; ataA *= ataDecay; ataB += ataC; ataA -= ataC; ataC = ataB;}
		ataDiffSample = (ataC * ataDecay);
		//end antialiasing section for input sample
		
		ataFlip = !ataFlip;
		
		inputSample = ataDrySample*outputD; inputSample += (ataDiffSample + ataHalfDiffSample);
		//apply processing as difference to non-oversampled raw input
		
		
		
		//inputSample *= output; *destP = (ataDrySample*dry)+(inputSample*wet);
		//built in output trim and dry/wet if desired
		*destP = inputSample*outputGlobal;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

