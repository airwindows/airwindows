/*
*	File:		AtmosphereChannel.cpp
*	
*	Version:	1.0
* 
*	Created:	3/10/18
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
	AtmosphereChannel.cpp
	
=============================================================================*/
#include "AtmosphereChannel.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, AtmosphereChannel)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AtmosphereChannel::AtmosphereChannel
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AtmosphereChannel::AtmosphereChannel(AudioUnit component)
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
//	AtmosphereChannel::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AtmosphereChannel::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AtmosphereChannel::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AtmosphereChannel::GetParameterInfo(AudioUnitScope		inScope,
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
//	AtmosphereChannel::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AtmosphereChannel::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AtmosphereChannel::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AtmosphereChannel::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	AtmosphereChannel::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult AtmosphereChannel::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____AtmosphereChannelEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AtmosphereChannel::AtmosphereChannelKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		AtmosphereChannel::AtmosphereChannelKernel::Reset()
{
	gainchase = -90.0;
	settingchase = -90.0;
	chasespeed = 350.0;

	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
	lastSampleA = 0.0;
	lastSampleB = 0.0;
	lastSampleC = 0.0;
	lastSampleD = 0.0;
	lastSampleE = 0.0;
	lastSampleF = 0.0;
	lastSampleG = 0.0;
	lastSampleH = 0.0;
	lastSampleI = 0.0;
	lastSampleJ = 0.0;
	lastSampleK = 0.0;
	lastSampleL = 0.0;
	lastSampleM = 0.0;
	thresholdA = 0.618033988749894;
	thresholdB = 0.679837387624884;
	thresholdC = 0.747821126387373;
	thresholdD = 0.82260323902611;
	thresholdE = 0.904863562928721;
	thresholdF = 0.995349919221593;
	thresholdG = 1.094884911143752;
	thresholdH = 1.204373402258128;
	thresholdI = 1.32481074248394;
	thresholdJ = 1.457291816732335;
	thresholdK = 1.603020998405568;
	thresholdL = 1.763323098246125;
	thresholdM = 1.939655408070737;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AtmosphereChannel::AtmosphereChannelKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		AtmosphereChannel::AtmosphereChannelKernel::Process(	const Float32 	*inSourceP,
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
	double inputSample;
	double drySample;
	double clamp;
	Float64 inputgain = GetParameter( kParam_One );
	if (settingchase != inputgain) {
		chasespeed *= 2.0;
		settingchase = inputgain;
	}
	if (chasespeed > 2500.0) chasespeed = 2500.0;
	if (gainchase < 0.0) gainchase = inputgain;	
	
	thresholdA = 0.618033988749894 / overallscale;
	thresholdB = 0.679837387624884 / overallscale;
	thresholdC = 0.747821126387373 / overallscale;
	thresholdD = 0.82260323902611 / overallscale;
	thresholdE = 0.904863562928721 / overallscale;
	thresholdF = 0.995349919221593 / overallscale;
	thresholdG = 1.094884911143752 / overallscale;
	thresholdH = 1.204373402258128 / overallscale;
	thresholdI = 1.32481074248394 / overallscale;
	thresholdJ = 1.457291816732335 / overallscale;
	thresholdK = 1.603020998405568 / overallscale;
	thresholdL = 1.763323098246125 / overallscale;
	thresholdM = 1.939655408070737 / overallscale;	
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;

		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		chasespeed *= 0.9999;
		chasespeed -= 0.01;
		if (chasespeed < 350.0) chasespeed = 350.0;
		//we have our chase speed compensated for recent fader activity
		
		gainchase = (((gainchase*chasespeed)+inputgain)/(chasespeed+1.0));
		//gainchase is chasing the target, as a simple multiply gain factor
		
		if (1.0 != gainchase) inputSample *= gainchase;
		//done with trim control
		
		drySample = inputSample;
		
		inputSample = sin(inputSample);
		//amplitude aspect
				
		clamp = inputSample - lastSampleA;
		if (clamp > thresholdA) inputSample = lastSampleA + thresholdA;
		if (-clamp > thresholdA) inputSample = lastSampleA - thresholdA;

		clamp = inputSample - lastSampleB;
		if (clamp > thresholdB) inputSample = lastSampleB + thresholdB;
		if (-clamp > thresholdB) inputSample = lastSampleB - thresholdB;

		clamp = inputSample - lastSampleC;
		if (clamp > thresholdC) inputSample = lastSampleC + thresholdC;
		if (-clamp > thresholdC) inputSample = lastSampleC - thresholdC;
		
		clamp = inputSample - lastSampleD;
		if (clamp > thresholdD) inputSample = lastSampleD + thresholdD;
		if (-clamp > thresholdD) inputSample = lastSampleD - thresholdD;
		
		clamp = inputSample - lastSampleE;
		if (clamp > thresholdE) inputSample = lastSampleE + thresholdE;
		if (-clamp > thresholdE) inputSample = lastSampleE - thresholdE;
		
		clamp = inputSample - lastSampleF;
		if (clamp > thresholdF) inputSample = lastSampleF + thresholdF;
		if (-clamp > thresholdF) inputSample = lastSampleF - thresholdF;
		
		clamp = inputSample - lastSampleG;
		if (clamp > thresholdG) inputSample = lastSampleG + thresholdG;
		if (-clamp > thresholdG) inputSample = lastSampleG - thresholdG;
		
		clamp = inputSample - lastSampleH;
		if (clamp > thresholdH) inputSample = lastSampleH + thresholdH;
		if (-clamp > thresholdH) inputSample = lastSampleH - thresholdH;
		
		clamp = inputSample - lastSampleI;
		if (clamp > thresholdI) inputSample = lastSampleI + thresholdI;
		if (-clamp > thresholdI) inputSample = lastSampleI - thresholdI;
		
		clamp = inputSample - lastSampleJ;
		if (clamp > thresholdJ) inputSample = lastSampleJ + thresholdJ;
		if (-clamp > thresholdJ) inputSample = lastSampleJ - thresholdJ;
		
		clamp = inputSample - lastSampleK;
		if (clamp > thresholdK) inputSample = lastSampleK + thresholdK;
		if (-clamp > thresholdK) inputSample = lastSampleK - thresholdK;
		
		clamp = inputSample - lastSampleL;
		if (clamp > thresholdL) inputSample = lastSampleL + thresholdL;
		if (-clamp > thresholdL) inputSample = lastSampleL - thresholdL;
		
		clamp = inputSample - lastSampleM;
		if (clamp > thresholdM) inputSample = lastSampleM + thresholdM;
		if (-clamp > thresholdM) inputSample = lastSampleM - thresholdM;
		
		
		lastSampleM = lastSampleL;
		lastSampleL = lastSampleK;
		lastSampleK = lastSampleJ;
		lastSampleJ = lastSampleI;
		lastSampleI = lastSampleH;
		lastSampleH = lastSampleG;
		lastSampleG = lastSampleF;
		lastSampleF = lastSampleE;
		lastSampleE = lastSampleD;
		lastSampleD = lastSampleC;
		lastSampleC = lastSampleB;
		lastSampleB = lastSampleA;
		lastSampleA = drySample;
		//store the raw input sample again for use next time
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

