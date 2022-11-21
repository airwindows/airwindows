/*
 *	File:		Logical4.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	11/9/15
 *	
 *	Copyright:  Copyright © 2015 Airwindows, Airwindows uses the MIT license
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
 Logical4.cpp
 
 =============================================================================*/
#include "Logical4.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Logical4)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Logical4::Logical4
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Logical4::Logical4(AudioUnit component)
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
//	Logical4::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Logical4::GetParameterValueStrings(AudioUnitScope		inScope,
														AudioUnitParameterID	inParameterID,
														CFArrayRef *		outStrings)
{
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Logical4::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Logical4::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -20.0;
                outParameterInfo.maxValue = 20.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
				outParameterInfo.unitName = kParameterTwoUnit;
                outParameterInfo.minValue = 1.0;
                outParameterInfo.maxValue = 16.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
 				outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
 				outParameterInfo.unitName = kParameterThreeUnit;
				outParameterInfo.minValue = 1.0;
                outParameterInfo.maxValue = 100.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
                break;
			case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -20.0;
                outParameterInfo.maxValue = 20.0;
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
//	Logical4::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Logical4::GetPropertyInfo (AudioUnitPropertyID	inID,
												AudioUnitScope		inScope,
												AudioUnitElement	inElement,
												UInt32 &		outDataSize,
												Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 Logical4::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	Logical4::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Logical4::GetProperty(	AudioUnitPropertyID inID,
										   AudioUnitScope 		inScope,
										   AudioUnitElement 	inElement,
										   void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Logical4::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Logical4::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Logical4EffectKernel

//-----------------------------------------------------------------------------------------
// this is called the reset the DSP state (clear buffers, reset counters, etc.)
ComponentResult		Logical4::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	//begin ButterComps
	controlAposL = 1.0;
	controlAnegL = 1.0;
	controlBposL = 1.0;
	controlBnegL = 1.0;
	targetposL = 1.0;
	targetnegL = 1.0;
	
	controlAposBL = 1.0;
	controlAnegBL = 1.0;
	controlBposBL = 1.0;
	controlBnegBL = 1.0;
	targetposBL = 1.0;
	targetnegBL = 1.0;
	
	controlAposCL = 1.0;
	controlAnegCL = 1.0;
	controlBposCL = 1.0;
	controlBnegCL = 1.0;
	targetposCL = 1.0;
	targetnegCL = 1.0;
	
	avgAL = avgBL = avgCL = avgDL = avgEL = avgFL = 0.0;
	nvgAL = nvgBL = nvgCL = nvgDL = nvgEL = nvgFL = 0.0;
	//end ButterComps
	
	//begin ButterComps
	controlAposR = 1.0;
	controlAnegR = 1.0;
	controlBposR = 1.0;
	controlBnegR = 1.0;
	targetposR = 1.0;
	targetnegR = 1.0;
	
	controlAposBR = 1.0;
	controlAnegBR = 1.0;
	controlBposBR = 1.0;
	controlBnegBR = 1.0;
	targetposBR = 1.0;
	targetnegBR = 1.0;
	
	controlAposCR = 1.0;
	controlAnegCR = 1.0;
	controlBposCR = 1.0;
	controlBnegCR = 1.0;
	targetposCR = 1.0;
	targetnegCR = 1.0;
	
	avgAR = avgBR = avgCR = avgDR = avgER = avgFR = 0.0;
	nvgAR = nvgBR = nvgCR = nvgDR = nvgER = nvgFR = 0.0;
	//end ButterComps
	
	//begin Power Sags
	for(int count = 0; count < 999; count++) {dL[count] = 0; bL[count] = 0; cL[count] = 0; dR[count] = 0; bR[count] = 0; cR[count] = 0;}
	controlL = 0; controlBL = 0; controlCL = 0;
	controlR = 0; controlBR = 0; controlCR = 0;
	
	gcount = 0;
	//end Power Sags
	
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	fpFlip = true;
	return noErr;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FarLogical43::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		Logical4::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
										  const AudioBufferList & inBuffer,
										  AudioBufferList & outBuffer,
										  UInt32 			inFramesToProcess)
{
	
	Float32 * inputL = (Float32*)(inBuffer.mBuffers[0].mData);
	Float32 * inputR = (Float32*)(inBuffer.mBuffers[1].mData);
	Float32 * outputL = (Float32*)(outBuffer.mBuffers[0].mData);
	Float32 * outputR = (Float32*)(outBuffer.mBuffers[1].mData);
	
	Float64 overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();	
	UInt32 nSampleFrames = inFramesToProcess;
	
	Float32 drySampleL;
	Float32 drySampleR;
	double inputSampleL;
	double inputSampleR;

	double fpOld = 0.618033988749894848204586; //golden ratio!
	double fpNew = 1.0 - fpOld;

	//begin ButterComp
	Float64 inputpos;
	Float64 inputneg;
	Float64 calcpos;
	Float64 calcneg;
	Float64 outputpos;
	Float64 outputneg;
	Float64 totalmultiplier;
	Float64 inputgain = pow(10.0,(-GetParameter( kParam_One ))/20.0);
	//fussing with the controls to make it hit the right threshold values
	Float64 compoutgain = inputgain; //let's try compensating for this
	
	Float64 attackspeed = GetParameter( kParam_Three );
	//is in ms
	attackspeed = 10.0 / sqrt(attackspeed);
	//convert to a remainder for use in comp
	Float64 divisor = 0.000782*attackspeed;
	//First Speed control
	divisor /= overallscale;
	Float64 remainder = divisor;
	divisor = 1.0 - divisor;
	
	Float64 divisorB = 0.000819*attackspeed;
	//Second Speed control
	divisorB /= overallscale;
	Float64 remainderB = divisorB;
	divisorB = 1.0 - divisorB;
	
	Float64 divisorC = 0.000857;
	//Third Speed control
	divisorC /= overallscale;
	Float64 remainderC = divisorC*attackspeed;
	divisorC = 1.0 - divisorC;
	//end ButterComp
	
	Float64 dynamicDivisor;
	Float64 dynamicRemainder;
	//used for variable release
	
	//begin Desk Power Sag
	Float64 intensity = 0.0445556;
	Float64 depthA = 2.42;
	int offsetA = (int)(depthA * overallscale);
	if (offsetA < 1) offsetA = 1;
	if (offsetA > 498) offsetA = 498;
	
	Float64 depthB = 2.42; //was 3.38
	int offsetB = (int)(depthB * overallscale);
	if (offsetB < 1) offsetB = 1;
	if (offsetB > 498) offsetB = 498;
	
	Float64 depthC = 2.42; //was 4.35
	int offsetC = (int)(depthC * overallscale);
	if (offsetC < 1) offsetC = 1;
	if (offsetC > 498) offsetC = 498;
	
	Float64 clamp;
	Float64 thickness;
	Float64 out;
	Float64 bridgerectifier;
	Float64 powerSag = 0.003300223685324102874217; //was .00365 
	//the Power Sag constant is how much the sag is cut back in high compressions. Increasing it
	//increases the guts and gnarl of the music, decreasing it or making it negative causes the texture to go 
	//'ethereal' and unsolid under compression. Very subtle!
	//end Desk Power Sag	
	
	Float64 ratio = sqrt(GetParameter( kParam_Two ))-1.0;
	if (ratio > 2.99999) ratio = 2.99999;
	if (ratio < 0.0) ratio = 0.0;
	//anything we do must adapt to our dry/a/b/c output stages
	int ratioselector = floor( ratio );
	//zero to three, it'll become, always with 3 as the max
	ratio -= ratioselector;
	Float64 invRatio = 1.0 - ratio;
	//for all processing we've settled on the 'stage' and are just interpolating between top and bottom
	//ratio is the more extreme case, invratio becomes our 'floor' case including drySample
	
	Float64 outSampleAL = 0.0;
	Float64 outSampleBL = 0.0;
	Float64 outSampleCL = 0.0;
	Float64 outSampleAR = 0.0;
	Float64 outSampleBR = 0.0;
	Float64 outSampleCR = 0.0;
	//what we interpolate between using ratio
	
	Float64 outputgain = pow(10.0,GetParameter( kParam_Four )/20.0);
	Float64 wet = GetParameter( kParam_Five );
	//removed unnecessary dry variable
	
	while (nSampleFrames-- > 0) {
		inputSampleL = *inputL;
		inputSampleR = *inputR;
		//assign working variables like the dry
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		gcount--;
		if (gcount < 0 || gcount > 499) {gcount = 499;}

		//begin first Power SagL
		dL[gcount+499] = dL[gcount] = fabs(inputSampleL)*(intensity-((controlAposL+controlBposL+controlAnegL+controlBnegL)*powerSag));
		controlL += (dL[gcount] / offsetA);
		controlL -= (dL[gcount+offsetA] / offsetA);
		controlL -= 0.000001;
		clamp = 1;
		if (controlL < 0) {controlL = 0;}
		if (controlL > 1) {clamp -= (controlL - 1); controlL = 1;}
		if (clamp < 0.5) {clamp = 0.5;}
		//control = 0 to 1
		thickness = ((1.0 - controlL) * 2.0) - 1.0;
		out = fabs(thickness);		
		bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (thickness > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-out))+(bridgerectifier*out);
		else inputSampleL = (inputSampleL*(1-out))-(bridgerectifier*out);
		//blend according to density control
		if (clamp != 1.0) inputSampleL *= clamp;
		//end first Power SagL
		
		//begin first Power SagR
		dR[gcount+499] = dR[gcount] = fabs(inputSampleR)*(intensity-((controlAposR+controlBposR+controlAnegR+controlBnegR)*powerSag));
		controlR += (dR[gcount] / offsetA);
		controlR -= (dR[gcount+offsetA] / offsetA);
		controlR -= 0.000001;
		clamp = 1;
		if (controlR < 0) {controlR = 0;}
		if (controlR > 1) {clamp -= (controlR - 1); controlR = 1;}
		if (clamp < 0.5) {clamp = 0.5;}
		//control = 0 to 1
		thickness = ((1.0 - controlR) * 2.0) - 1.0;
		out = fabs(thickness);		
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (thickness > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1-out))-(bridgerectifier*out);
		//blend according to density control
		if (clamp != 1.0) inputSampleR *= clamp;
		//end first Power SagR
		
		//begin first compressorL
		if (inputgain != 1.0) inputSampleL *= inputgain;
		inputpos = (inputSampleL * fpOld) + (avgAL * fpNew) + 1.0;
		avgAL = inputSampleL;
		//hovers around 1 when there's no signal
		
		if (inputpos < 0.001) inputpos = 0.001;
		outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainder * (inputpos + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetposL *= dynamicDivisor;
		targetposL += (inputpos * dynamicRemainder);
		calcpos = pow((1.0/targetposL),2);
		//extra tiny, quick, if the inputpos of this polarity is high
		
		inputneg = (-inputSampleL * fpOld) + (nvgAL * fpNew) + 1.0;
		nvgAL = -inputSampleL;
		
		if (inputneg < 0.001) inputneg = 0.001;
		outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainder * (inputneg + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetnegL *= dynamicDivisor;
		targetnegL += (inputneg * dynamicRemainder);
		calcneg = pow((1.0/targetnegL),2);
		//now we have mirrored targets for comp
		//each calc is a blowed up chased target from tiny (at high levels of that polarity) to 1 at no input
		//calc is the one we want to react differently: go tiny quick, 
		//outputpos and outputneg go from 0 to 1
		
		if (inputSampleL > 0)
		{ //working on pos
			if (true == fpFlip)
			{
				controlAposL *= divisor;
				controlAposL += (calcpos*remainder);
				if (controlAposR > controlAposL) controlAposR = (controlAposR + controlAposL) * 0.5;
				//this part makes the compressor linked: both channels will converge towards whichever
				//is the most compressed at the time.
			}
			else
			{
				controlBposL *= divisor;
				controlBposL += (calcpos*remainder);
				if (controlBposR > controlBposL) controlBposR = (controlBposR + controlBposL) * 0.5;
			}	
		}
		else
		{ //working on neg
			if (true == fpFlip)
			{
				controlAnegL *= divisor;
				controlAnegL += (calcneg*remainder);
				if (controlAnegR > controlAnegL) controlAnegR = (controlAnegR + controlAnegL) * 0.5;
			}
			else
			{
				controlBnegL *= divisor;
				controlBnegL += (calcneg*remainder);
				if (controlBnegR > controlBnegL) controlBnegR = (controlBnegR + controlBnegL) * 0.5;
			}
		}
		//this causes each of the four to update only when active and in the correct 'fpFlip'
		
		if (true == fpFlip)
		{totalmultiplier = (controlAposL * outputpos) + (controlAnegL * outputneg);}
		else
		{totalmultiplier = (controlBposL * outputpos) + (controlBnegL * outputneg);}
		//this combines the sides according to fpFlip, blending relative to the input value
		
		if (totalmultiplier != 1.0) inputSampleL *= totalmultiplier;
		//if (compoutgain != 1.0) inputSample /= compoutgain;
		if (inputSampleL > 36.0) inputSampleL = 36.0;
		if (inputSampleL < -36.0) inputSampleL = -36.0;
		//build in +18db hard clip on insano inputs
		outSampleAL = inputSampleL / compoutgain;
		//end first compressorL
		
		//begin first compressorR
		if (inputgain != 1.0) inputSampleR *= inputgain;
		inputpos = (inputSampleR * fpOld) + (avgAR * fpNew) + 1.0;
		avgAR = inputSampleR;
		//hovers around 1 when there's no signal
		
		if (inputpos < 0.001) inputpos = 0.001;
		outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainder * (inputpos + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetposR *= dynamicDivisor;
		targetposR += (inputpos * dynamicRemainder);
		calcpos = pow((1.0/targetposR),2);
		//extra tiny, quick, if the inputpos of this polarity is high
		
		inputneg = (-inputSampleR * fpOld) + (nvgAR * fpNew) + 1.0;
		nvgAR = -inputSampleR;
		
		if (inputneg < 0.001) inputneg = 0.001;
		outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainder * (inputneg + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetnegR *= dynamicDivisor;
		targetnegR += (inputneg * dynamicRemainder);
		calcneg = pow((1.0/targetnegR),2);
		//now we have mirrored targets for comp
		//each calc is a blowed up chased target from tiny (at high levels of that polarity) to 1 at no input
		//calc is the one we want to react differently: go tiny quick, 
		//outputpos and outputneg go from 0 to 1
		
		if (inputSampleR > 0)
		{ //working on pos
			if (true == fpFlip)
			{
				controlAposR *= divisor;
				controlAposR += (calcpos*remainder);
				if (controlAposL > controlAposR) controlAposL = (controlAposR + controlAposL) * 0.5;
				//this part makes the compressor linked: both channels will converge towards whichever
				//is the most compressed at the time.				
			}
			else
			{
				controlBposR *= divisor;
				controlBposR += (calcpos*remainder);
				if (controlBposL > controlBposR) controlBposL = (controlBposR + controlBposL) * 0.5;
			}	
		}
		else
		{ //working on neg
			if (true == fpFlip)
			{
				controlAnegR *= divisor;
				controlAnegR += (calcneg*remainder);
				if (controlAnegL > controlAnegR) controlAnegL = (controlAnegR + controlAnegL) * 0.5;
			}
			else
			{
				controlBnegR *= divisor;
				controlBnegR += (calcneg*remainder);
				if (controlBnegL > controlBnegR) controlBnegL = (controlBnegR + controlBnegL) * 0.5;
			}
		}
		//this causes each of the four to update only when active and in the correct 'fpFlip'
		
		if (true == fpFlip)
		{totalmultiplier = (controlAposR * outputpos) + (controlAnegR * outputneg);}
		else
		{totalmultiplier = (controlBposR * outputpos) + (controlBnegR * outputneg);}
		//this combines the sides according to fpFlip, blending relative to the input value
		
		if (totalmultiplier != 1.0) inputSampleR *= totalmultiplier;
		//if (compoutgain != 1.0) inputSample /= compoutgain;
		if (inputSampleR > 36.0) inputSampleR = 36.0;
		if (inputSampleR < -36.0) inputSampleR = -36.0;
		//build in +18db hard clip on insano inputs
		outSampleAR = inputSampleR / compoutgain;
		//end first compressorR
		
		if (ratioselector > 0) {
			
		//begin second Power SagL
		bL[gcount+499] = bL[gcount] = fabs(inputSampleL)*(intensity-((controlAposBL+controlBposBL+controlAnegBL+controlBnegBL)*powerSag));
		controlBL += (bL[gcount] / offsetB);
		controlBL -= (bL[gcount+offsetB] / offsetB);
		controlBL -= 0.000001;
		clamp = 1;
		if (controlBL < 0) {controlBL = 0;}
		if (controlBL > 1) {clamp -= (controlBL - 1); controlBL = 1;}
		if (clamp < 0.5) {clamp = 0.5;}
		//control = 0 to 1
		thickness = ((1.0 - controlBL) * 2.0) - 1.0;
		out = fabs(thickness);		
		bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (thickness > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-out))+(bridgerectifier*out);
		else inputSampleL = (inputSampleL*(1-out))-(bridgerectifier*out);
		//blend according to density control
		if (clamp != 1.0) inputSampleL *= clamp;
		//end second Power SagL
		
		//begin second Power SagR
		bR[gcount+499] = bR[gcount] = fabs(inputSampleR)*(intensity-((controlAposBR+controlBposBR+controlAnegBR+controlBnegBR)*powerSag));
		controlBR += (bR[gcount] / offsetB);
		controlBR -= (bR[gcount+offsetB] / offsetB);
		controlBR -= 0.000001;
		clamp = 1;
		if (controlBR < 0) {controlBR = 0;}
		if (controlBR > 1) {clamp -= (controlBR - 1); controlBR = 1;}
		if (clamp < 0.5) {clamp = 0.5;}
		//control = 0 to 1
		thickness = ((1.0 - controlBR) * 2.0) - 1.0;
		out = fabs(thickness);		
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (thickness > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1-out))-(bridgerectifier*out);
		//blend according to density control
		if (clamp != 1.0) inputSampleR *= clamp;
		//end second Power SagR
		
		
		//begin second compressorL
		inputpos = (inputSampleL * fpOld) + (avgBL * fpNew) + 1.0;
		avgBL = inputSampleL;
		
		if (inputpos < 0.001) inputpos = 0.001;
		outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainderB * (inputpos + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetposBL *= dynamicDivisor;
		targetposBL += (inputpos * dynamicRemainder);
		calcpos = pow((1.0/targetposBL),2);
		
		inputneg = (-inputSampleL * fpOld) + (nvgBL * fpNew) + 1.0;
		nvgBL = -inputSampleL;
		
		if (inputneg < 0.001) inputneg = 0.001;
		outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainderB * (inputneg + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetnegBL *= dynamicDivisor;
		targetnegBL += (inputneg * dynamicRemainder);
		calcneg = pow((1.0/targetnegBL),2);
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		if (inputSampleL > 0)
		{ //working on pos
			if (true == fpFlip)
			{
				controlAposBL *= divisorB;
				controlAposBL += (calcpos*remainderB);
				if (controlAposBR > controlAposBL) controlAposBR = (controlAposBR + controlAposBL) * 0.5;
				//this part makes the compressor linked: both channels will converge towards whichever
				//is the most compressed at the time.				
			}
			else
			{
				controlBposBL *= divisorB;
				controlBposBL += (calcpos*remainderB);
				if (controlBposBR > controlBposBL) controlBposBR = (controlBposBR + controlBposBL) * 0.5;
			}	
		}
		else
		{ //working on neg
			if (true == fpFlip)
			{
				controlAnegBL *= divisorB;
				controlAnegBL += (calcneg*remainderB);
				if (controlAnegBR > controlAnegBL) controlAnegBR = (controlAnegBR + controlAnegBL) * 0.5;
			}
			else
			{
				controlBnegBL *= divisorB;
				controlBnegBL += (calcneg*remainderB);
				if (controlBnegBR > controlBnegBL) controlBnegBR = (controlBnegBR + controlBnegBL) * 0.5;
			}
		}
		//this causes each of the four to update only when active and in the correct 'fpFlip'
		
		if (true == fpFlip)
		{totalmultiplier = (controlAposBL * outputpos) + (controlAnegBL * outputneg);}
		else
		{totalmultiplier = (controlBposBL * outputpos) + (controlBnegBL * outputneg);}
		//this combines the sides according to fpFlip, blending relative to the input value
		
		if (totalmultiplier != 1.0) inputSampleL *= totalmultiplier;
		//if (compoutgain != 1.0) inputSample /= compoutgain;
		if (inputSampleL > 36.0) inputSampleL = 36.0;
		if (inputSampleL < -36.0) inputSampleL = -36.0;
		//build in +18db hard clip on insano inputs
		outSampleBL = inputSampleL / compoutgain;
		//end second compressorL
		
		//begin second compressorR
		inputpos = (inputSampleR * fpOld) + (avgBR * fpNew) + 1.0;
		avgBR = inputSampleR;
		
		if (inputpos < 0.001) inputpos = 0.001;
		outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainderB * (inputpos + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetposBR *= dynamicDivisor;
		targetposBR += (inputpos * dynamicRemainder);
		calcpos = pow((1.0/targetposBR),2);
		
		inputneg = (-inputSampleR * fpOld) + (nvgBR * fpNew) + 1.0;
		nvgBR = -inputSampleR;
		
		if (inputneg < 0.001) inputneg = 0.001;
		outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainderB * (inputneg + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetnegBR *= dynamicDivisor;
		targetnegBR += (inputneg * dynamicRemainder);
		calcneg = pow((1.0/targetnegBR),2);
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		if (inputSampleR > 0)
		{ //working on pos
			if (true == fpFlip)
			{
				controlAposBR *= divisorB;
				controlAposBR += (calcpos*remainderB);
				if (controlAposBL > controlAposBR) controlAposBL = (controlAposBR + controlAposBL) * 0.5;
				//this part makes the compressor linked: both channels will converge towards whichever
				//is the most compressed at the time.				
			}
			else
			{
				controlBposBR *= divisorB;
				controlBposBR += (calcpos*remainderB);
				if (controlBposBL > controlBposBR) controlBposBL = (controlBposBR + controlBposBL) * 0.5;
			}	
		}
		else
		{ //working on neg
			if (true == fpFlip)
			{
				controlAnegBR *= divisorB;
				controlAnegBR += (calcneg*remainderB);
				if (controlAnegBL > controlAnegBR) controlAnegBL = (controlAnegBR + controlAnegBL) * 0.5;
			}
			else
			{
				controlBnegBR *= divisorB;
				controlBnegBR += (calcneg*remainderB);
				if (controlBnegBL > controlBnegBR) controlBnegBL = (controlBnegBR + controlBnegBL) * 0.5;
			}
		}
		//this causes each of the four to update only when active and in the correct 'fpFlip'
		
		if (true == fpFlip)
		{totalmultiplier = (controlAposBR * outputpos) + (controlAnegBR * outputneg);}
		else
		{totalmultiplier = (controlBposBR * outputpos) + (controlBnegBR * outputneg);}
		//this combines the sides according to fpFlip, blending relative to the input value
		
		if (totalmultiplier != 1.0) inputSampleR *= totalmultiplier;
		//if (compoutgain != 1.0) inputSample /= compoutgain;
		if (inputSampleR > 36.0) inputSampleR = 36.0;
		if (inputSampleR < -36.0) inputSampleR = -36.0;
		//build in +18db hard clip on insano inputs
		outSampleBR = inputSampleR / compoutgain;
		//end second compressorR
			
			if (ratioselector > 1) {
		
		//begin third Power SagL
		cL[gcount+499] = cL[gcount] = fabs(inputSampleL)*(intensity-((controlAposCL+controlBposCL+controlAnegCL+controlBnegCL)*powerSag));
		controlCL += (cL[gcount] / offsetC);
		controlCL -= (cL[gcount+offsetB] / offsetC);
		controlCL -= 0.000001;
		clamp = 1;
		if (controlCL < 0) {controlCL = 0;}
		if (controlCL > 1) {clamp -= (controlCL - 1); controlCL = 1;}
		if (clamp < 0.5) {clamp = 0.5;}
		//control = 0 to 1
		thickness = ((1.0 - controlCL) * 2.0) - 1.0;
		out = fabs(thickness);		
		bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (thickness > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-out))+(bridgerectifier*out);
		else inputSampleL = (inputSampleL*(1-out))-(bridgerectifier*out);
		//blend according to density control
		if (clamp != 1.0) inputSampleL *= clamp;
		//end third Power SagL
		
		//begin third Power SagR
		cR[gcount+499] = cR[gcount] = fabs(inputSampleR)*(intensity-((controlAposCR+controlBposCR+controlAnegCR+controlBnegCR)*powerSag));
		controlCR += (cR[gcount] / offsetC);
		controlCR -= (cR[gcount+offsetB] / offsetC);
		controlCR -= 0.000001;
		clamp = 1;
		if (controlCR < 0) {controlCR = 0;}
		if (controlCR > 1) {clamp -= (controlCR - 1); controlCR = 1;}
		if (clamp < 0.5) {clamp = 0.5;}
		//control = 0 to 1
		thickness = ((1.0 - controlCR) * 2.0) - 1.0;
		out = fabs(thickness);		
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (thickness > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1-out))-(bridgerectifier*out);
		//blend according to density control
		if (clamp != 1.0) inputSampleR *= clamp;
		//end third Power SagR
		
		//begin third compressorL
		inputpos = (inputSampleL * fpOld) + (avgCL * fpNew) + 1.0;
		avgCL = inputSampleL;
		
		if (inputpos < 0.001) inputpos = 0.001;
		outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainderC * (inputpos + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetposCL *= dynamicDivisor;
		targetposCL += (inputpos * dynamicRemainder);
		calcpos = pow((1.0/targetposCL),2);
		
		inputneg = (-inputSampleL * fpOld) + (nvgCL * fpNew) + 1.0;
		nvgCL = -inputSampleL;
		
		if (inputneg < 0.001) inputneg = 0.001;
		outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainderC * (inputneg + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetnegCL *= dynamicDivisor;
		targetnegCL += (inputneg * dynamicRemainder);
		calcneg = pow((1.0/targetnegCL),2);
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		if (inputSampleL > 0)
		{ //working on pos
			if (true == fpFlip)
			{
				controlAposCL *= divisorC;
				controlAposCL += (calcpos*remainderC);
				if (controlAposCR > controlAposCL) controlAposCR = (controlAposCR + controlAposCL) * 0.5;
				//this part makes the compressor linked: both channels will converge towards whichever
				//is the most compressed at the time.				
			}
			else
			{
				controlBposCL *= divisorC;
				controlBposCL += (calcpos*remainderC);
				if (controlBposCR > controlBposCL) controlBposCR = (controlBposCR + controlBposCL) * 0.5;
			}	
		}
		else
		{ //working on neg
			if (true == fpFlip)
			{
				controlAnegCL *= divisorC;
				controlAnegCL += (calcneg*remainderC);
				if (controlAnegCR > controlAnegCL) controlAnegCR = (controlAnegCR + controlAnegCL) * 0.5;
			}
			else
			{
				controlBnegCL *= divisorC;
				controlBnegCL += (calcneg*remainderC);
				if (controlBnegCR > controlBnegCL) controlBnegCR = (controlBnegCR + controlBnegCL) * 0.5;
			}
		}
		//this causes each of the four to update only when active and in the correct 'fpFlip'
		
		if (true == fpFlip)
		{totalmultiplier = (controlAposCL * outputpos) + (controlAnegCL * outputneg);}
		else
		{totalmultiplier = (controlBposCL * outputpos) + (controlBnegCL * outputneg);}
		//this combines the sides according to fpFlip, blending relative to the input value
		
		if (totalmultiplier != 1.0) inputSampleL *= totalmultiplier;
		if (inputSampleL > 36.0) inputSampleL = 36.0;
		if (inputSampleL < -36.0) inputSampleL = -36.0;
		//build in +18db hard clip on insano inputs
		outSampleCL = inputSampleL / compoutgain;
		//if (compoutgain != 1.0) inputSample /= compoutgain;
		//end third compressorL
		
		//begin third compressorR
		inputpos = (inputSampleR * fpOld) + (avgCR * fpNew) + 1.0;
		avgCR = inputSampleR;
		
		if (inputpos < 0.001) inputpos = 0.001;
		outputpos = inputpos / 2.0;
		if (outputpos > 1.0) outputpos = 1.0;		
		inputpos *= inputpos;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainderC * (inputpos + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetposCL *= dynamicDivisor;
		targetposCL += (inputpos * dynamicRemainder);
		calcpos = pow((1.0/targetposCR),2);
		
		inputneg = (-inputSampleR * fpOld) + (nvgCR * fpNew) + 1.0;
		nvgCR = -inputSampleR;
		
		if (inputneg < 0.001) inputneg = 0.001;
		outputneg = inputneg / 2.0;
		if (outputneg > 1.0) outputneg = 1.0;		
		inputneg *= inputneg;
		//will be very high for hot input, can be 0.00001-1 for other-polarity
		
		dynamicRemainder = remainderC * (inputneg + 1.0);
		if (dynamicRemainder > 1.0) dynamicRemainder = 1.0;
		dynamicDivisor = 1.0 - dynamicRemainder;
		//calc chases much faster if input swing is high
		
		targetnegCR *= dynamicDivisor;
		targetnegCR += (inputneg * dynamicRemainder);
		calcneg = pow((1.0/targetnegCR),2);
		//now we have mirrored targets for comp
		//outputpos and outputneg go from 0 to 1
		if (inputSampleR > 0)
		{ //working on pos
			if (true == fpFlip)
			{
				controlAposCR *= divisorC;
				controlAposCR += (calcpos*remainderC);
				if (controlAposCL > controlAposCR) controlAposCL = (controlAposCR + controlAposCL) * 0.5;
				//this part makes the compressor linked: both channels will converge towards whichever
				//is the most compressed at the time.				
			}
			else
			{
				controlBposCR *= divisorC;
				controlBposCR += (calcpos*remainderC);
				if (controlBposCL > controlBposCR) controlBposCL = (controlBposCR + controlBposCL) * 0.5;
			}	
		}
		else
		{ //working on neg
			if (true == fpFlip)
			{
				controlAnegCR *= divisorC;
				controlAnegCR += (calcneg*remainderC);
				if (controlAnegCL > controlAnegCR) controlAnegCL = (controlAnegCR + controlAnegCL) * 0.5;
			}
			else
			{
				controlBnegCR *= divisorC;
				controlBnegCR += (calcneg*remainderC);
				if (controlBnegCL > controlBnegCR) controlBnegCL = (controlBnegCR + controlBnegCL) * 0.5;
			}
		}
		//this causes each of the four to update only when active and in the correct 'fpFlip'
		
		if (true == fpFlip)
		{totalmultiplier = (controlAposCR * outputpos) + (controlAnegCR * outputneg);}
		else
		{totalmultiplier = (controlBposCR * outputpos) + (controlBnegCR * outputneg);}
		//this combines the sides according to fpFlip, blending relative to the input value
		
		if (totalmultiplier != 1.0) inputSampleR *= totalmultiplier;
		if (inputSampleR > 36.0) inputSampleR = 36.0;
		if (inputSampleR < -36.0) inputSampleR = -36.0;
		//build in +18db hard clip on insano inputs
		outSampleCR = inputSampleR / compoutgain;
		//if (compoutgain != 1.0) inputSample /= compoutgain;
		//end third compressorR
		}
		} //these nested if blocks are not indented because the old xCode doesn't support it
		
		//here we will interpolate between dry, and the three post-stages of processing
		switch (ratioselector) {
			case 0:
				inputSampleL = (drySampleL * invRatio) + (outSampleAL * ratio);
				inputSampleR = (drySampleR * invRatio) + (outSampleAR * ratio);
				break;
			case 1:
				inputSampleL = (outSampleAL * invRatio) + (outSampleBL * ratio);
				inputSampleR = (outSampleAR * invRatio) + (outSampleBR * ratio);
				break;
			default:
				inputSampleL = (outSampleBL * invRatio) + (outSampleCL * ratio);
				inputSampleR = (outSampleBR * invRatio) + (outSampleCR * ratio);
				break;
		}
		//only then do we reconstruct the output, but our ratio is built here
		
		if (outputgain != 1.0) {
			inputSampleL *= outputgain;
			inputSampleR *= outputgain;
		}

		if (wet != 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}		
		fpFlip = !fpFlip;
		
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
		
		inputL += 1; inputR += 1; outputL += 1; outputR += 1;
	}
	return noErr;
}

