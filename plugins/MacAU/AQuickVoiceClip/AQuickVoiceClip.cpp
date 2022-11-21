/*
*	File:		AQuickVoiceClip.cpp
*	
*	Version:	1.0
* 
*	Created:	10/3/13
*	
*	Copyright:  Copyright © 2013 Airwindows, Airwindows uses the MIT license
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
	AQuickVoiceClip.cpp
	
=============================================================================*/
#include "AQuickVoiceClip.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(AQuickVoiceClip)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AQuickVoiceClip::AQuickVoiceClip
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AQuickVoiceClip::AQuickVoiceClip(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AQuickVoiceClip::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AQuickVoiceClip::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AQuickVoiceClip::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AQuickVoiceClip::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = 30.0;
                outParameterInfo.maxValue = 3000.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
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
//	AQuickVoiceClip::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AQuickVoiceClip::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AQuickVoiceClip::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AQuickVoiceClip::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	AQuickVoiceClip::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult AQuickVoiceClip::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____AQuickVoiceClipEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AQuickVoiceClip::AQuickVoiceClipKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		AQuickVoiceClip::AQuickVoiceClipKernel::Reset()
{
	ataLast6Sample = ataLast5Sample = ataLast4Sample = ataLast3Sample = ataLast2Sample = ataLast1Sample = 0.0;
	ataHalfwaySample = ataHalfDrySample = ataHalfDiffSample = 0.0;
	ataLastDiffSample = ataDrySample = ataDiffSample = ataPrevDiffSample = 0.0;
	ataK1 = -0.646; //first FIR shaping of interpolated sample, brightens
	ataK2 = 0.311; //second FIR shaping of interpolated sample, thickens
	ataK6 = -0.093; //third FIR shaping of interpolated sample, brings air
	ataK7 = 0.057; //fourth FIR shaping of interpolated sample, thickens
	ataK8 = -0.023; //fifth FIR shaping of interpolated sample, brings air
	ataK3 = 0.114; //add raw to interpolated dry, toughens
	ataK4 = 0.886; //remainder of interpolated dry, adds up to 1.0
	ataK5 = 0.431; //subtract this much prev. diff sample, brightens.  0.431 becomes flat
	lastSample = 0.0;
	lastOutSample = 0.0;
	lastOut2Sample = 0.0;
	lastOut3Sample = 0.0;
	lpDepth = 0.0;
	overshoot = 0.0;
	overall = 0;
	iirSampleA = 0.0;
	iirSampleB = 0.0;
	iirSampleC = 0.0;
	iirSampleD = 0.0;
	flip = false;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AQuickVoiceClip::AQuickVoiceClipKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		AQuickVoiceClip::AQuickVoiceClipKernel::Process(	const Float32 	*inSourceP,
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
	
	Float64 softness = 0.484416;
	Float64 hardness = 1.0 - softness;
	
	Float64 iirAmount = GetParameter( kParam_One )/8000.0;
	
	iirAmount /= overallscale;
	Float64 altAmount = (1.0 - iirAmount);
	
	Float64 cancelnew = 0.0682276;
	Float64 cancelold = 1.0 - cancelnew;
	
	Float64 maxRecent;
	
	Float64 lpSpeed = 0.0009;

	Float64 cliplevel = 0.98;
	
	Float64 refclip = 0.5; //preset to cut out gain quite a lot. 91%? no touchy unless clip
	
	Float64 inputSample;
	Float64 passThrough;
	Float64 outputSample;
	bool clipOnset;
	Float64 drySample;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		passThrough = ataDrySample = inputSample;
		
		
		ataHalfDrySample = ataHalfwaySample = (inputSample + ataLast1Sample + (ataLast2Sample*ataK1) + (ataLast3Sample*ataK2) + (ataLast4Sample*ataK6) + (ataLast5Sample*ataK7) + (ataLast6Sample*ataK8)) / 2.0;
		ataLast6Sample = ataLast5Sample; ataLast5Sample = ataLast4Sample; ataLast4Sample = ataLast3Sample; ataLast3Sample = ataLast2Sample; ataLast2Sample = ataLast1Sample; ataLast1Sample = inputSample;
		//setting up oversampled special antialiasing
		clipOnset = false;
		
		
		maxRecent = fabs( ataLast6Sample );
		if (fabs( ataLast5Sample ) > maxRecent ) maxRecent = fabs( ataLast5Sample );
		if (fabs( ataLast4Sample ) > maxRecent ) maxRecent = fabs( ataLast4Sample );
		if (fabs( ataLast3Sample ) > maxRecent ) maxRecent = fabs( ataLast3Sample );
		if (fabs( ataLast2Sample ) > maxRecent ) maxRecent = fabs( ataLast2Sample );
		if (fabs( ataLast1Sample ) > maxRecent ) maxRecent = fabs( ataLast1Sample );
		if (fabs( inputSample ) > maxRecent ) maxRecent = fabs( inputSample );
		//this gives us something that won't cut out in zero crossings, to interpolate with
		
		maxRecent *= 2.0;
		//by refclip this is 1.0 and fully into the antialiasing
		if (maxRecent > 1.0) maxRecent = 1.0;
		//and it tops out at 1. Higher means more antialiasing, lower blends into passThrough without antialiasing
		
		ataHalfwaySample -= overall;
		//subtract dist-cancel from input after getting raw input, before doing anything
		
		drySample = ataHalfwaySample;
		

		if (lastSample >= refclip)
		{
			lpDepth += 0.1;
			if (ataHalfwaySample < refclip)
			{
				lastSample = ((refclip*hardness) + (ataHalfwaySample * softness));
			}
			else lastSample = refclip;
		}
		
		if (lastSample <= -refclip)
		{
			lpDepth += 0.1;
			if (ataHalfwaySample > -refclip)
			{
				lastSample = ((-refclip*hardness) + (ataHalfwaySample * softness));
			}
			else lastSample = -refclip;
		}
		
		if (ataHalfwaySample > refclip)
		{
			lpDepth += 0.1;
			if (lastSample < refclip)
			{
				ataHalfwaySample = ((refclip*hardness) + (lastSample * softness));
			}
			else ataHalfwaySample = refclip;
		}
		
		if (ataHalfwaySample < -refclip)
		{
			lpDepth += 0.1;
			if (lastSample > -refclip)
			{
				ataHalfwaySample = ((-refclip*hardness) + (lastSample * softness));
			}
			else ataHalfwaySample = -refclip;
		}
				
        outputSample = lastSample;
		lastSample = ataHalfwaySample;
		ataHalfwaySample = outputSample;
		//swap around in a circle for one final ADClip,
		//this time not tracking overshoot anymore
		
		//end interpolated sample
		
		//begin raw sample- inputSample and ataDrySample handled separately here
		
		inputSample -= overall;
		//subtract dist-cancel from input after getting raw input, before doing anything
		
		drySample = inputSample;
		
		if (lastSample >= refclip)
		{
			lpDepth += 0.1;
			if (inputSample < refclip)
			{
				lastSample = ((refclip*hardness) + (inputSample * softness));
			}
			else lastSample = refclip;
		}
		
		if (lastSample <= -refclip)
		{
			lpDepth += 0.1;
			if (inputSample > -refclip)
			{
				lastSample = ((-refclip*hardness) + (inputSample * softness));
			}
			else lastSample = -refclip;
		}
		
		if (inputSample > refclip)
		{
			lpDepth += 0.1;
			if (lastSample < refclip)
			{
				inputSample = ((refclip*hardness) + (lastSample * softness));
			}
			else inputSample = refclip;
		}
		
		if (inputSample < -refclip)
		{
			lpDepth += 0.1;
			if (lastSample > -refclip)
			{
				inputSample = ((-refclip*hardness) + (lastSample * softness));
			}
			else inputSample = -refclip;
		}
		
        outputSample = lastSample;
		lastSample = inputSample;
		inputSample = outputSample;
		
		//end raw sample
		
		ataHalfDrySample = (ataDrySample*ataK3)+(ataHalfDrySample*ataK4);
		ataHalfDiffSample = (ataHalfwaySample - ataHalfDrySample)/2.0;
		ataLastDiffSample = ataDiffSample*ataK5;
		ataDiffSample = (inputSample - ataDrySample)/2.0;
		ataDiffSample += ataHalfDiffSample;
		ataDiffSample -= ataLastDiffSample;
		inputSample = ataDrySample;
		inputSample += ataDiffSample;
		
		
		overall = (overall * cancelold) + (ataDiffSample * cancelnew);
		//apply all the diffs to a lowpassed IIR
		
		if (flip)
		{
			iirSampleA = (iirSampleA * altAmount) + (inputSample * iirAmount);
			inputSample -= iirSampleA;
			iirSampleC = (iirSampleC * altAmount) + (passThrough * iirAmount);
			passThrough -= iirSampleC;
		}
		else
		{
			iirSampleB = (iirSampleB * altAmount) + (inputSample * iirAmount);
			inputSample -= iirSampleB;
			iirSampleD = (iirSampleD * altAmount) + (passThrough * iirAmount);
			passThrough -= iirSampleD;
		}
		flip = !flip;
		//highpass section 
		
		
		lastOut3Sample = lastOut2Sample;
		lastOut2Sample = lastOutSample;
		lastOutSample = inputSample;
		
		lpDepth -= lpSpeed;
		if (lpDepth > 0.0)
		{
			if (lpDepth > 1.0) lpDepth = 1.0;
			inputSample *= (1.0-lpDepth);
			inputSample += (((lastOutSample + lastOut2Sample + lastOut3Sample) / 3.6)*lpDepth);
		}
		if (lpDepth < 0.0) lpDepth = 0.0;
				
		inputSample *= (1.0-maxRecent);
		inputSample += (passThrough * maxRecent);
		//there's our raw signal, without antialiasing. Brings up low level stuff and softens more when hot
		
		if (inputSample > cliplevel) inputSample = cliplevel;
		if (inputSample < -cliplevel) inputSample = -cliplevel;
		//final iron bar
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

