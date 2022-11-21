/*
*	File:		Dynamics.cpp
*	
*	Version:	1.0
* 
*	Created:	8/19/22
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
	Dynamics.cpp
	
=============================================================================*/
#include "Dynamics.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Dynamics)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Dynamics::Dynamics
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Dynamics::Dynamics(AudioUnit component)
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
//	Dynamics::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Dynamics::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Dynamics::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Dynamics::GetParameterInfo(AudioUnitScope		inScope,
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
//	Dynamics::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Dynamics::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 Dynamics::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	Dynamics::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Dynamics::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Dynamics::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Dynamics::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____DynamicsEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Dynamics::DynamicsKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Dynamics::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	//begin Gate
	WasNegativeL = false;
	ZeroCrossL = 0;
	gaterollerL = 0.0;
	gateL = 0.0;
	
	WasNegativeR = false;
	ZeroCrossR = 0;
	gaterollerR = 0.0;
	gateR = 0.0;
	//end Gate
	
	//begin ButterComp
	controlAposL = 1.0;
	controlAnegL = 1.0;
	controlBposL = 1.0;
	controlBnegL = 1.0;
	targetposL = 1.0;
	targetnegL = 1.0;	
	avgLA = avgLB = 0.0;
	nvgLA = nvgLB = 0.0;
	
	controlAposR = 1.0;
	controlAnegR = 1.0;
	controlBposR = 1.0;
	controlBnegR = 1.0;
	targetposR = 1.0;
	targetnegR = 1.0;	
	avgRA = avgRB = 0.0;
	nvgRA = nvgRB = 0.0;
	//end ButterComp
	flip = false;

	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Dynamics::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		Dynamics::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
													const AudioBufferList & inBuffer,
                                                    AudioBufferList & outBuffer,
                                                    UInt32 			inFramesToProcess)
{
	Float32 * inputL = (Float32*)(inBuffer.mBuffers[0].mData);
	Float32 * inputR = (Float32*)(inBuffer.mBuffers[1].mData);
	Float32 * outputL = (Float32*)(outBuffer.mBuffers[0].mData);
	Float32 * outputR = (Float32*)(outBuffer.mBuffers[1].mData);
	UInt32 nSampleFrames = inFramesToProcess;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
		
	//begin ButterComp
	double inputgain = (pow(GetParameter( kParam_One ),5)*35)+1.0;
	double divisor = (pow(GetParameter( kParam_Two ),4) * 0.01)+0.0005;
	divisor /= overallscale;
	double remainder = divisor;
	divisor = 1.0 - divisor;
	//end ButterComp
	
	//begin Gate
	double onthreshold = (pow(GetParameter( kParam_Three ),3)/3)+0.00018;
	double offthreshold = onthreshold * 1.1;	
	double release = 0.028331119964586;
	double absmax = 220.9;
	//speed to be compensated w.r.t sample rate
	//end Gate
	
	double wet = GetParameter(kParam_Four );
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;		
		
		//begin compressor
		//begin L
		inputSampleL *= inputgain;
		double inputpos = inputSampleL + 1.0;		
		if (inputpos < 0.0) inputpos = 0.0;
		double outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		targetposL *= divisor;
		targetposL += (inputpos * remainder);
		double calcpos = 1.0/targetposL;
		
		double inputneg = -inputSampleL + 1.0;		
		if (inputneg < 0.0) inputneg = 0.0;
		double outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		targetnegL *= divisor;
		targetnegL += (inputneg * remainder);
		double calcneg = 1.0/targetnegL;
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		
		if (inputSampleL > 0)
		{ //working on pos
			if (true == flip)
			{
				controlAposL *= divisor;
				controlAposL += (calcpos*remainder);
			} else {
				controlBposL *= divisor;
				controlBposL += (calcpos*remainder);
			}	
		} else { //working on neg
			if (true == flip)
			{
				controlAnegL *= divisor;
				controlAnegL += (calcneg*remainder);
			} else {
				controlBnegL *= divisor;
				controlBnegL += (calcneg*remainder);
			}
		}
		//this causes each of the four to update only when active and in the correct 'flip'
		
		double totalmultiplier;
		if (true == flip) totalmultiplier = (controlAposL * outputpos) + (controlAnegL * outputneg);
		else totalmultiplier = (controlBposL * outputpos) + (controlBnegL * outputneg);
		//this combines the sides according to flip, blending relative to the input value
		inputSampleL *= totalmultiplier;
		inputSampleL /= inputgain;
		//end L
		
		//begin R
		inputSampleR *= inputgain;
		inputpos = inputSampleR + 1.0;		
		if (inputpos < 0.0) inputpos = 0.0;
		outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		targetposR *= divisor;
		targetposR += (inputpos * remainder);
		calcpos = 1.0/targetposR;
		
		inputneg = -inputSampleR + 1.0;		
		if (inputneg < 0.0) inputneg = 0.0;
		outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		targetnegR *= divisor;
		targetnegR += (inputneg * remainder);
		calcneg = 1.0/targetnegR;
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		
		if (inputSampleR > 0)
		{ //working on pos
			if (true == flip)
			{
				controlAposR *= divisor;
				controlAposR += (calcpos*remainder);
			} else {
				controlBposR *= divisor;
				controlBposR += (calcpos*remainder);
			}	
		} else { //working on neg
			if (true == flip)
			{
				controlAnegR *= divisor;
				controlAnegR += (calcneg*remainder);
			} else {
				controlBnegR *= divisor;
				controlBnegR += (calcneg*remainder);
			}
		}
		//this causes each of the four to update only when active and in the correct 'flip'
		
		if (true == flip) totalmultiplier = (controlAposR * outputpos) + (controlAnegR * outputneg);
		else totalmultiplier = (controlBposR * outputpos) + (controlBnegR * outputneg);
		//this combines the sides according to flip, blending relative to the input value
		
		inputSampleR *= totalmultiplier;
		inputSampleR /= inputgain;
		//end R
		flip = !flip;
		//end compressor
		
		
		
		//begin Gate
		if (drySampleL > 0.0)
		{
			if (WasNegativeL == true) ZeroCrossL = absmax * 0.3;
			WasNegativeL = false;
		} else {
			ZeroCrossL += 1; WasNegativeL = true;
		}
		
		if (drySampleR > 0.0)
		{
			if (WasNegativeR == true) ZeroCrossR = absmax * 0.3;
			WasNegativeR = false;
		} else {
			ZeroCrossR += 1; WasNegativeR = true;
		}
		
		if (ZeroCrossL > absmax) ZeroCrossL = absmax;
		if (ZeroCrossR > absmax) ZeroCrossR = absmax;
		
		if (gateL == 0.0)
		{
			//if gate is totally silent
			if (fabs(drySampleL) > onthreshold)
			{
				if (gaterollerL == 0.0) gaterollerL = ZeroCrossL;
				else gaterollerL -= release;
				// trigger from total silence only- if we're active then signal must clear offthreshold
			}
			else gaterollerL -= release;
		} else {
			//gate is not silent but closing
			if (fabs(drySampleL) > offthreshold)
			{
				if (gaterollerL < ZeroCrossL) gaterollerL = ZeroCrossL;
				else gaterollerL -= release;
				//always trigger if gate is over offthreshold, otherwise close anyway
			}
			else gaterollerL -= release;
		}
		
		if (gateR == 0.0)
		{
			//if gate is totally silent
			if (fabs(drySampleR) > onthreshold)
			{
				if (gaterollerR == 0.0) gaterollerR = ZeroCrossR;
				else gaterollerR -= release;
				// trigger from total silence only- if we're active then signal must clear offthreshold
			}
			else gaterollerR -= release;
		} else {
			//gate is not silent but closing
			if (fabs(drySampleR) > offthreshold)
			{
				if (gaterollerR < ZeroCrossR) gaterollerR = ZeroCrossR;
				else gaterollerR -= release;
				//always trigger if gate is over offthreshold, otherwise close anyway
			}
			else gaterollerR -= release;
		}
		
		if (gaterollerL < 0.0) gaterollerL = 0.0;
		if (gaterollerR < 0.0) gaterollerR = 0.0;
		
		if (gaterollerL < 1.0)
		{
			gateL = gaterollerL;
			double bridgerectifier = 1-cos(fabs(inputSampleL));			
			if (inputSampleL > 0) inputSampleL = (inputSampleL*gateL)+(bridgerectifier*(1.0-gateL));
			else inputSampleL = (inputSampleL*gateL)-(bridgerectifier*(1.0-gateL));
			if (gateL == 0.0) inputSampleL = 0.0;			
		} else gateL = 1.0;
		
		if (gaterollerR < 1.0)
		{
			gateR = gaterollerR;
			double bridgerectifier = 1-cos(fabs(inputSampleR));			
			if (inputSampleR > 0) inputSampleR = (inputSampleR*gateR)+(bridgerectifier*(1.0-gateR));
			else inputSampleR = (inputSampleR*gateR)-(bridgerectifier*(1.0-gateR));
			if (gateR == 0.0) inputSampleR = 0.0;			
		} else gateR = 1.0;
		//end Gate
		
		if (wet != 1.0) {
		 inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
		 inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider

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
		//direct stereo out
		
		inputL += 1;
		inputR += 1;
		outputL += 1;
		outputR += 1;
	}
	return noErr;
}

