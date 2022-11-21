/*
*	File:		PodcastDeluxe.cpp
*	
*	Version:	1.0
* 
*	Created:	5/24/19
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
	PodcastDeluxe.cpp
	
=============================================================================*/
#include "PodcastDeluxe.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(PodcastDeluxe)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PodcastDeluxe::PodcastDeluxe
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PodcastDeluxe::PodcastDeluxe(AudioUnit component)
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
//	PodcastDeluxe::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PodcastDeluxe::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PodcastDeluxe::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PodcastDeluxe::GetParameterInfo(AudioUnitScope		inScope,
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
//	PodcastDeluxe::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PodcastDeluxe::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PodcastDeluxe::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			PodcastDeluxe::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	PodcastDeluxe::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult PodcastDeluxe::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____PodcastDeluxeEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PodcastDeluxe::PodcastDeluxeKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		PodcastDeluxe::PodcastDeluxeKernel::Reset()
{
	for(int count = 0; count < 502; count++) {d1[count] = 0.0; d2[count] = 0.0; d3[count] = 0.0; d4[count] = 0.0; d5[count] = 0.0;}
	tap1 = 1; tap2 = 1; tap3 = 1; tap4 = 1; tap5 = 1;
	maxdelay1 = 9001; maxdelay2 = 9001; maxdelay3 = 9001; maxdelay4 = 9001; maxdelay5 = 9001;
	c1 = 2.0; c2 = 2.0; c3 = 2.0; c4 = 2.0; c5 = 2.0; //startup comp gains
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	PodcastDeluxe::PodcastDeluxeKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		PodcastDeluxe::PodcastDeluxeKernel::Process(	const Float32 	*inSourceP,
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
	
	int allpasstemp;
	Float64 outallpass = 0.618033988749894848204586;
	
	Float64 compress = 1.0+pow(GetParameter( kParam_One )*0.8,2);
	
	Float64 speed1 = 128.0 / pow(compress,2);
	speed1 *= overallscale;
	Float64 speed2 = speed1 * 1.4;
	Float64 speed3 = speed2 * 1.5;
	Float64 speed4 = speed3 * 1.6;
	Float64 speed5 = speed4 * 1.7;
	
	maxdelay1 = (int)(23.0*overallscale);
	maxdelay2 = (int)(19.0*overallscale);
	maxdelay3 = (int)(17.0*overallscale);
	maxdelay4 = (int)(13.0*overallscale);
	maxdelay5 = (int)(11.0*overallscale);
	//set up the prime delays
	
	Float64 refclip = 0.999;
	Float64 softness = 0.435;
	Float64 invsoft = 0.56;
	Float64 outsoft = 0.545;
	Float64 trigger;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
								
		allpasstemp = tap1 - 1;
		if (allpasstemp < 0 || allpasstemp > maxdelay1) {allpasstemp = maxdelay1;}
		//set up the delay position
		//using 'tap' and 'allpasstemp' to position the tap		
		inputSample -= d1[allpasstemp]*outallpass;
		d1[tap1] = inputSample;
		inputSample *= outallpass;
		inputSample += (d1[allpasstemp]);
		//allpass stage
		tap1--; if (tap1 < 0 || tap1 > maxdelay1) {tap1 = maxdelay1;}
		//decrement the position for reals
		
		inputSample *= c1;
		trigger = fabs(inputSample)*4.7;
		if (trigger > 4.7) trigger = 4.7;
		trigger = sin(trigger);
		if (trigger < 0) trigger *= 8.0;
		if (trigger < -4.2) trigger = -4.2;
		c1 += trigger/speed5;
		if (c1 > compress) c1 = compress;
		//compress stage
				
		allpasstemp = tap2 - 1;
		if (allpasstemp < 0 || allpasstemp > maxdelay2) {allpasstemp = maxdelay2;}
		//set up the delay position
		//using 'tap' and 'allpasstemp' to position the tap
		inputSample -= d2[allpasstemp]*outallpass;
		d2[tap2] = inputSample;
		inputSample *= outallpass;
		inputSample += (d2[allpasstemp]);
		//allpass stage
		tap2--; if (tap2 < 0 || tap2 > maxdelay2) {tap2 = maxdelay2;}
		//decrement the position for reals
		
		inputSample *= c2;
		trigger = fabs(inputSample)*4.7;
		if (trigger > 4.7) trigger = 4.7;
		trigger = sin(trigger);
		if (trigger < 0) trigger *= 8.0;
		if (trigger < -4.2) trigger = -4.2;
		c2 += trigger/speed4;
		if (c2 > compress) c2 = compress;
		//compress stage
				
		allpasstemp = tap3 - 1;
		if (allpasstemp < 0 || allpasstemp > maxdelay3) {allpasstemp = maxdelay3;}
		//set up the delay position
		//using 'tap' and 'allpasstemp' to position the tap
		inputSample -= d3[allpasstemp]*outallpass;
		d3[tap3] = inputSample;
		inputSample *= outallpass;
		inputSample += (d3[allpasstemp]);
		//allpass stage
		tap3--; if (tap3 < 0 || tap3 > maxdelay3) {tap3 = maxdelay3;}
		//decrement the position for reals
		
		inputSample *= c3;
		trigger = fabs(inputSample)*4.7;
		if (trigger > 4.7) trigger = 4.7;
		trigger = sin(trigger);
		if (trigger < 0) trigger *= 8.0;
		if (trigger < -4.2) trigger = -4.2;
		c3 += trigger/speed3;
		if (c3 > compress) c3 = compress;
		//compress stage
				
		allpasstemp = tap4 - 1;
		if (allpasstemp < 0 || allpasstemp > maxdelay4) {allpasstemp = maxdelay4;}
		//set up the delay position
		//using 'tap' and 'allpasstemp' to position the tap
		inputSample -= d4[allpasstemp]*outallpass;
		d4[tap4] = inputSample;
		inputSample *= outallpass;
		inputSample += (d4[allpasstemp]);
		//allpass stage
		tap4--; if (tap4 < 0 || tap4 > maxdelay4) {tap4 = maxdelay4;}
		//decrement the position for reals
		
		inputSample *= c4;
		trigger = fabs(inputSample)*4.7;
		if (trigger > 4.7) trigger = 4.7;
		trigger = sin(trigger);
		if (trigger < 0) trigger *= 8.0;
		if (trigger < -4.2) trigger = -4.2;
		c4 += trigger/speed2;
		if (c4 > compress) c4 = compress;
		//compress stage
				
		allpasstemp = tap5 - 1;
		if (allpasstemp < 0 || allpasstemp > maxdelay5) {allpasstemp = maxdelay5;}
		//set up the delay position
		//using 'tap' and 'allpasstemp' to position the tap
		inputSample -= d5[allpasstemp]*outallpass;
		d5[tap5] = inputSample;
		inputSample *= outallpass;
		inputSample += (d5[allpasstemp]);
		//allpass stage
		tap5--; if (tap5 < 0 || tap5 > maxdelay5) {tap5 = maxdelay5;}
		//decrement the position for reals
		
		inputSample *= c5;
		trigger = fabs(inputSample)*4.7;
		if (trigger > 4.7) trigger = 4.7;
		trigger = sin(trigger);
		if (trigger < 0) trigger *= 8.0;
		if (trigger < -4.2) trigger = -4.2;
		c5 += trigger/speed1;
		if (c5 > compress) c5 = compress;
		//compress stage
		
		if (compress > 1.0) inputSample /= compress;
		
		if (lastSample >= refclip)
		{
			if (inputSample < refclip)
			{
				lastSample = (outsoft + (inputSample * softness));
			}
			else lastSample = refclip;
		}
		
		if (lastSample <= -refclip)
		{
			if (inputSample > -refclip)
			{
				lastSample = (-outsoft + (inputSample * softness));
			}
			else lastSample = -refclip;
		}
		
		if (inputSample > refclip)
		{
			if (lastSample < refclip)
			{
				inputSample = (invsoft + (lastSample * softness));
			}
			else inputSample = refclip;
		}
		
		if (inputSample < -refclip)
		{
			if (lastSample > -refclip)
			{
				inputSample = (-invsoft + (lastSample * softness));
			}
			else inputSample = -refclip;
		}

		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

