/*
*	File:		ADClip7.cpp
*	
*	Version:	1.0
* 
*	Created:	8/2/17
*	
*	Copyright:  Copyright © 2017 Airwindows, Airwindows uses the MIT license
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
	ADClip7.cpp
	
=============================================================================*/
#include "ADClip7.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(ADClip7)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip7::ADClip7
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ADClip7::ADClip7(AudioUnit component)
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
//	ADClip7::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ADClip7::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_Four)) //ID must be actual name of parameter identifier, not number
	{
		if (outStrings == NULL) return noErr;
		CFStringRef strings [] =
		{
			kMenuItem_Normal,
			kMenuItem_Gain,
			kMenuItem_Clip,
		};
		*outStrings = CFArrayCreate (
									 NULL,
									 (const void **) strings,
									 (sizeof (strings) / sizeof (strings [0])),
									 NULL
									 );
		return noErr;
	}
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip7::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ADClip7::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 18.0;
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kNormal;
                outParameterInfo.maxValue = kClip;
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
//	ADClip7::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ADClip7::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip7::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ADClip7::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ADClip7::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ADClip7::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ADClip7EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip7::ADClip7Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ADClip7::ADClip7Kernel::Reset()
{
	lastSample = 0.0;
	for(int count = 0; count < 22199; count++) {b[count] = 0;}
	gcount = 0;
	lows = 0;
	refclip = 0.99;
	iirLowsA = 0.0;
	iirLowsB = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ADClip7::ADClip7Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ADClip7::ADClip7Kernel::Process(	const Float32 	*inSourceP,
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
	Float64 fpOld = 0.618033988749894848204586; //golden ratio!
	Float64 fpNew = 1.0 - fpOld;
	
	Float64 inputGain = pow(10.0,(GetParameter( kParam_One ))/20.0);
	Float64 softness = GetParameter( kParam_Two ) * fpNew;
	Float64 hardness = 1.0 - softness;
	Float64 highslift = 0.307 * GetParameter( kParam_Three );
	Float64 adjust = pow(highslift,3) * 0.416;
	Float64 subslift = 0.796 * GetParameter( kParam_Three );
	Float64 calibsubs = subslift/53;
	Float64 invcalibsubs = 1.0 - calibsubs;
	Float64 subs = 0.81 + (calibsubs*2);
	double bridgerectifier;
	int mode = (int) GetParameter( kParam_Four );
	Float64 overshoot;
	Float64 offsetH1 = 1.84;
	offsetH1 *= overallscale;
	Float64 offsetH2 = offsetH1 * 1.9;
	Float64 offsetH3 = offsetH1 * 2.7;
	Float64 offsetL1 = 612;
	offsetL1 *= overallscale;
	Float64 offsetL2 = offsetL1 * 2.0;
	int refH1 = (int)floor(offsetH1);
	int refH2 = (int)floor(offsetH2);
	int refH3 = (int)floor(offsetH3);
	int refL1 = (int)floor(offsetL1);
	int refL2 = (int)floor(offsetL2);
	int temp;
	Float64 fractionH1 = offsetH1 - floor(offsetH1);
	Float64 fractionH2 = offsetH2 - floor(offsetH2);
	Float64 fractionH3 = offsetH3 - floor(offsetH3);
	Float64 minusH1 = 1.0 - fractionH1;
	Float64 minusH2 = 1.0 - fractionH2;
	Float64 minusH3 = 1.0 - fractionH3;
	Float64 highs = 0.0;
	int count = 0;
	double inputSample;
	
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		if (inputGain != 1.0) {
			inputSample *= inputGain;
		}
		
		overshoot = fabs(inputSample) - refclip;
		if (overshoot < 0.0) overshoot = 0.0;
				
		if (gcount < 0 || gcount > 11020) {gcount = 11020;}
		count = gcount;
		b[count+11020] = b[count] = overshoot;
		gcount--;
		
		if (highslift > 0.0)
		{
			//we have a big pile of b[] which is overshoots
			temp = count+refH3;
			highs = -(b[temp] * minusH3); //less as value moves away from .0
			highs -= b[temp+1]; //we can assume always using this in one way or another?
			highs -= (b[temp+2] * fractionH3); //greater as value moves away from .0
			highs += (((b[temp]-b[temp+1])-(b[temp+1]-b[temp+2]))/50); //interpolation hacks 'r us
			highs *= adjust; //add in the kernel elements backwards saves multiplies
			//stage 3 is a negative add
			temp = count+refH2;
			highs += (b[temp] * minusH2); //less as value moves away from .0
			highs += b[temp+1]; //we can assume always using this in one way or another?
			highs += (b[temp+2] * fractionH2); //greater as value moves away from .0
			highs -= (((b[temp]-b[temp+1])-(b[temp+1]-b[temp+2]))/50); //interpolation hacks 'r us
			highs *= adjust; //add in the kernel elements backwards saves multiplies
			//stage 2 is a positive feedback of the overshoot
			temp = count+refH1;
			highs -= (b[temp] * minusH1); //less as value moves away from .0
			highs -= b[temp+1]; //we can assume always using this in one way or another?
			highs -= (b[temp+2] * fractionH1); //greater as value moves away from .0
			highs += (((b[temp]-b[temp+1])-(b[temp+1]-b[temp+2]))/50); //interpolation hacks 'r us
			highs *= adjust; //add in the kernel elements backwards saves multiplies
			//stage 1 is a negative feedback of the overshoot
			//done with interpolated mostly negative feedback of the overshoot
		}
		
		bridgerectifier = sin(fabs(highs) * hardness);
		//this will wrap around and is scaled back by softness
		//wrap around is the same principle as Fracture: no top limit to sin()
		if (highs > 0) highs = bridgerectifier;
		else highs = -bridgerectifier;
		
		if (subslift > 0.0) 
		{
			lows *= subs;
			//going in we'll reel back some of the swing
			temp = count+refL1;
			lows -= b[temp+127];
			lows -= b[temp+113];
			lows -= b[temp+109];
			lows -= b[temp+107];
			lows -= b[temp+103];
			lows -= b[temp+101];
			lows -= b[temp+97];
			lows -= b[temp+89];
			lows -= b[temp+83];
			lows -= b[temp+79];
			lows -= b[temp+73];
			lows -= b[temp+71];
			lows -= b[temp+67];
			lows -= b[temp+61];
			lows -= b[temp+59];
			lows -= b[temp+53];
			lows -= b[temp+47];
			lows -= b[temp+43];
			lows -= b[temp+41];
			lows -= b[temp+37];
			lows -= b[temp+31];
			lows -= b[temp+29];
			lows -= b[temp+23];
			lows -= b[temp+19];
			lows -= b[temp+17];
			lows -= b[temp+13];
			lows -= b[temp+11];
			lows -= b[temp+7];
			lows -= b[temp+5];
			lows -= b[temp+3];
			lows -= b[temp+2];
			lows -= b[temp+1];
			//initial negative lobe
			lows *= subs;
			lows *= subs;
			//twice, to minimize the suckout in low boost situations
			temp = count+refL2;
			lows += b[temp+127];
			lows += b[temp+113];
			lows += b[temp+109];
			lows += b[temp+107];
			lows += b[temp+103];
			lows += b[temp+101];
			lows += b[temp+97];
			lows += b[temp+89];
			lows += b[temp+83];
			lows += b[temp+79];
			lows += b[temp+73];
			lows += b[temp+71];
			lows += b[temp+67];
			lows += b[temp+61];
			lows += b[temp+59];
			lows += b[temp+53];
			lows += b[temp+47];
			lows += b[temp+43];
			lows += b[temp+41];
			lows += b[temp+37];
			lows += b[temp+31];
			lows += b[temp+29];
			lows += b[temp+23];
			lows += b[temp+19];
			lows += b[temp+17];
			lows += b[temp+13];
			lows += b[temp+11];
			lows += b[temp+7];
			lows += b[temp+5];
			lows += b[temp+3];
			lows += b[temp+2];
			lows += b[temp+1];
			lows *= subs;
			//followup positive lobe
			//now we have the lows content to use
		}
		
		bridgerectifier = sin(fabs(lows) * softness);
		//this will wrap around and is scaled back by hardness: hard = less bass push, more treble
		//wrap around is the same principle as Fracture: no top limit to sin()
		if (lows > 0) lows = bridgerectifier;
		else lows = -bridgerectifier;
		
		iirLowsA = (iirLowsA * invcalibsubs) + (lows * calibsubs);
		lows = iirLowsA;
		bridgerectifier = sin(fabs(lows));
		if (lows > 0) lows = bridgerectifier;
		else lows = -bridgerectifier;
		
		iirLowsB = (iirLowsB * invcalibsubs) + (lows * calibsubs);
		lows = iirLowsB;
		bridgerectifier = sin(fabs(lows)) * 2.0;
		if (lows > 0) lows = bridgerectifier;
		else lows = -bridgerectifier;
		
		if (highslift > 0.0) inputSample += (highs * (1.0-fabs(inputSample*hardness)));
		if (subslift > 0.0) inputSample += (lows * (1.0-fabs(inputSample*softness)));
		
		if (inputSample > refclip && refclip > 0.9) refclip -= 0.01;
		if (inputSample < -refclip && refclip > 0.9) refclip -= 0.01;
		if (refclip < 0.99) refclip += 0.00001;
		//adjust clip level on the fly
		
		if (lastSample >= refclip)
		{
			if (inputSample < refclip) lastSample = ((refclip*hardness) + (inputSample * softness));
			else lastSample = refclip;
		}
		
		if (lastSample <= -refclip)
		{
			if (inputSample > -refclip) lastSample = ((-refclip*hardness) + (inputSample * softness));
			else lastSample = -refclip;
		}
		
		if (inputSample > refclip)
		{
			if (lastSample < refclip) inputSample = ((refclip*hardness) + (lastSample * softness));
			else inputSample = refclip;
		}
		
		if (inputSample < -refclip)
		{
			if (lastSample > -refclip) inputSample = ((-refclip*hardness) + (lastSample * softness));
			else inputSample = -refclip;
		}
		lastSample = inputSample;
		
		switch (mode)
		{
			case 1: break; //Normal
			case 2: inputSample /= inputGain; break; //Gain Match
			case 3: inputSample = overshoot + highs + lows; break; //Clip Only
		}
		//this is our output mode switch, showing the effects
		
		if (inputSample > refclip) inputSample = refclip;
		if (inputSample < -refclip) inputSample = -refclip;
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

