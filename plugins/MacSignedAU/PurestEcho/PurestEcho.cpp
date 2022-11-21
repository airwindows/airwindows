/*
*	File:		PurestEcho.cpp
*	
*	Version:	1.0
* 
*	Created:	6/2/17
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
	PurestEcho.cpp
	
=============================================================================*/
#include "PurestEcho.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, PurestEcho)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestEcho::PurestEcho
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PurestEcho::PurestEcho(AudioUnit component)
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
//	PurestEcho::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PurestEcho::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestEcho::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PurestEcho::GetParameterInfo(AudioUnitScope		inScope,
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
//	PurestEcho::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PurestEcho::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestEcho::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PurestEcho::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	PurestEcho::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult PurestEcho::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____PurestEchoEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestEcho::PurestEchoKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		PurestEcho::PurestEchoKernel::Reset()
{
	//totalsamples comes from the .h file: it's a const static number that defines
	//the whole delay buffer. We still have a hardcoded delay buffer, but some might like
	//to use this to define the buffer in terms of seconds: samples as a factor of GetSampleRate()
	//The danger there, of course, is having a user start up the plugin at 384K and smashing their memory
	
	for(int count = 0; count < totalsamples-1; count++) {d[count] = 0;}
	gcount = 0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PurestEcho::PurestEchoKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		PurestEcho::PurestEchoKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	int loopLimit = (int)(totalsamples * 0.499);
	//this is a double buffer so we will be splitting it in two
	
	Float64 time = pow(GetParameter( kParam_One ),2) * 0.999;
	Float64 tap1 = GetParameter( kParam_Two );
	Float64 tap2 = GetParameter( kParam_Three );
	Float64 tap3 = GetParameter( kParam_Four );
	Float64 tap4 = GetParameter( kParam_Five );
	
	Float64 gainTrim = 1.0 / (1.0 + tap1 + tap2 + tap3 + tap4);
	//this becomes our equal-loudness mechanic. 0.2 to 1.0 gain on all things.
	Float64 tapsTrim = gainTrim * 0.5;
	//the taps interpolate and require half that gain: 0.1 to 0.5 on all taps.
	
	int position1 = (int)(loopLimit * time * 0.25);
	int position2 = (int)(loopLimit * time * 0.5);
	int position3 = (int)(loopLimit * time * 0.75);
	int position4 = (int)(loopLimit * time);
	//basic echo information: we're taking four equally spaced echoes and setting their levels as desired.
	//position4 is what you'd have for 'just set a delay time'

	Float64 volAfter1 = (loopLimit * time * 0.25) - position1;
	Float64 volAfter2 = (loopLimit * time * 0.5) - position2;
	Float64 volAfter3 = (loopLimit * time * 0.75) - position3;
	Float64 volAfter4 = (loopLimit * time) - position4;
	//these are 0-1: casting to an (int) truncates fractional numbers towards zero (and is faster than floor() )
	//so, when we take the integer number (all above zero) and subtract it from the real value, we get 0-1
	Float64 volBefore1 = (1.0 - volAfter1) * tap1;
	Float64 volBefore2 = (1.0 - volAfter2) * tap2;
	Float64 volBefore3 = (1.0 - volAfter3) * tap3;
	Float64 volBefore4 = (1.0 - volAfter4) * tap4;
	//and if we are including a bit of the previous/next sample to interpolate, then if the sample position is 1.0001
	//we'll be leaning most heavily on the 'before' sample which is nearer to us, and the 'after' sample is almost not used.
	//if the sample position is 1.9999, the 'after' sample is strong and 'before' is almost not used.
	
	volAfter1 *= tap1;
	volAfter2 *= tap2;
	volAfter3 *= tap3;
	volAfter4 *= tap4;
	//and like with volBefore, we also want to scale this 'interpolate' to the loudness of this tap.
	//We do it here because we can do it only once per audio buffer, not on every sample. This assumes we're
	//not moving the tap every sample: if so we'd have to do this every sample as well.	
	
	int oneBefore1 = position1 - 1;
	int oneBefore2 = position2 - 1;
	int oneBefore3 = position3 - 1;
	int oneBefore4 = position4 - 1;
	if (oneBefore1 < 0) oneBefore1 = 0;
	if (oneBefore2 < 0) oneBefore2 = 0;
	if (oneBefore3 < 0) oneBefore3 = 0;
	if (oneBefore4 < 0) oneBefore4 = 0;
	int oneAfter1 = position1 + 1;
	int oneAfter2 = position2 + 1;
	int oneAfter3 = position3 + 1;
	int oneAfter4 = position4 + 1;
	//this is setting up the way we interpolate samples: we're doing an echo-darkening thing
	//to make it sound better. Pretty much no acoustic delay in human-breathable air will give
	//you zero attenuation at 22 kilohertz: forget this at your peril ;)
	
	Float64 delaysBuffer;
		
	double inputSample;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		if (gcount < 0 || gcount > loopLimit) gcount = loopLimit;
		d[gcount+loopLimit] = d[gcount] = inputSample * tapsTrim; //this is how the double buffer works:
		//we can look for delay taps without ever having to 'wrap around' within our calculation.
		//As long as the delay tap is less than our loop limit we can always just add it to where we're
		//at, and get a valid sample back right away, no matter where we are in the buffer.
		//The 0.5 is taking into account the interpolation, by padding down the whole buffer.
		
		delaysBuffer = (d[gcount+oneBefore4]*volBefore4);
		delaysBuffer += (d[gcount+oneAfter4]*volAfter4);
		delaysBuffer += (d[gcount+oneBefore3]*volBefore3);
		delaysBuffer += (d[gcount+oneAfter3]*volAfter3);
		delaysBuffer += (d[gcount+oneBefore2]*volBefore2);
		delaysBuffer += (d[gcount+oneAfter2]*volAfter2);
		delaysBuffer += (d[gcount+oneBefore1]*volBefore1);
		delaysBuffer += (d[gcount+oneAfter1]*volAfter1);
		//These are the interpolated samples. We're adding them first, because we know they're smaller
		//and while the value of delaysBuffer is small we'll add similarly small values to it. Note the order.
		
		delaysBuffer += (d[gcount+position4]*tap4);
		delaysBuffer += (d[gcount+position3]*tap3);
		delaysBuffer += (d[gcount+position2]*tap2);
		delaysBuffer += (d[gcount+position1]*tap1);
		//These are the primary samples for the echo, and we're adding them last. As before we're starting with the
		//most delayed echoes, and ending with what we think might be the loudest echo. We're building this delaybuffer
		//from the faintest noises to the loudest, to avoid adding a bunch of teeny values at the end.
		//You can of course put the last echo as loudest, but with diminishing echo volumes this is optimal.
		//This technique is also present in other plugins such as Iron Oxide.
		
		inputSample = (inputSample * gainTrim) + delaysBuffer;
		//this could be just inputSample += d[gcount+position1];
		//for literally a single, full volume echo combined with dry.
		//What I'm doing is making the echoes more interesting.
		
		gcount--;
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

