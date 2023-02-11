/*
*	File:		Console8LiteChannel.cpp
*	
*	Version:	1.0
* 
*	Created:	11/21/22
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
	Console8LiteChannel.cpp
	
=============================================================================*/
#include "Console8LiteChannel.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Console8LiteChannel)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Console8LiteChannel::Console8LiteChannel
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Console8LiteChannel::Console8LiteChannel(AudioUnit component)
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
//	Console8LiteChannel::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Console8LiteChannel::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Console8LiteChannel::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Console8LiteChannel::GetParameterInfo(AudioUnitScope		inScope,
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
//	Console8LiteChannel::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Console8LiteChannel::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Console8LiteChannel::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Console8LiteChannel::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Console8LiteChannel::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Console8LiteChannel::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Console8LiteChannelEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Console8LiteChannel::Console8LiteChannelKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Console8LiteChannel::Console8LiteChannelKernel::Reset()
{
	iirA = 0.0; iirB = 0.0;
	for (int x = 0; x < fix_total; x++) fix[x] = 0.0;
	for (int x = 0; x < 10; x++) soft[x] = 0.0;
	
	inTrimA = 0.5; inTrimB = 0.5;
	for (int x = 0; x < fix_total; x++) fixB[x] = 0.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Console8LiteChannel::Console8LiteChannelKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Console8LiteChannel::Console8LiteChannelKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	double iirAmountA = 12.66/GetSampleRate();
	//this is our distributed unusual highpass, which is
	//adding subtle harmonics to the really deep stuff to define it
	if (fabs(iirA)<1.18e-37) iirA = 0.0;
	if (fabs(iirB)<1.18e-37) iirB = 0.0;
	//catch denormals early and only check once per buffer
	if (GetSampleRate() > 49000.0) hsr = true; else hsr = false;
	fix[fix_freq] = 24000.0 / GetSampleRate();
	fix[fix_reso] = 2.24697960;
	double K = tan(M_PI * fix[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fix[fix_reso] + K * K);
	fix[fix_a0] = K * K * norm;
	fix[fix_a1] = 2.0 * fix[fix_a0];
	fix[fix_a2] = fix[fix_a0];
	fix[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fix[fix_b2] = (1.0 - K / fix[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter
	
	inTrimA = inTrimB; inTrimB = GetParameter( kParam_One )*2.0;
	//0.5 is unity gain, and we can attenuate to silence or boost slightly over 12dB
	//into softclipping overdrive.
	fixB[fix_freq] = 24000.0 / GetSampleRate();
	fixB[fix_reso] = 0.80193774;
	K = tan(M_PI * fixB[fix_freq]); //lowpass
	norm = 1.0 / (1.0 + K / fixB[fix_reso] + K * K);
	fixB[fix_a0] = K * K * norm;
	fixB[fix_a1] = 2.0 * fixB[fix_a0];
	fixB[fix_a2] = fixB[fix_a0];
	fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixB[fix_b2] = (1.0 - K / fixB[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd == 3) cycleEnd = 4;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 4 for 176 or 192k
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		double position = (double)nSampleFrames/inFramesToProcess;
		double inTrim = (inTrimA*position)+(inTrimB*(1.0-position));
		//input trim smoothed to cut out zipper noise
		
		iirA = (iirA * (1.0 - iirAmountA)) + (inputSample * iirAmountA);
		double iirAmountB = fabs(iirA)+0.00001;
		iirB = (iirB * (1.0 - iirAmountB)) + (iirA * iirAmountB);
		inputSample -= iirB;
		//Console8 highpass
		if (cycleEnd == 4) {
			soft[8] = soft[7]; soft[7] = soft[6]; soft[6] = soft[5]; soft[5] = soft[4];
			soft[4] = soft[3]; soft[3] = soft[2]; soft[2] = soft[1]; soft[1] = soft[0];
			soft[0] = inputSample;
		}
		if (cycleEnd == 2) {
			soft[8] = soft[6]; soft[6] = soft[4];
			soft[4] = soft[2]; soft[2] = soft[0];
			soft[0] = inputSample;
		}
		if (cycleEnd == 1) {
			soft[8] = soft[4];
			soft[4] = soft[0];
			soft[0] = inputSample;
		}
		soft[9] = ((soft[0]-soft[4])-(soft[4]-soft[8]));
		if (soft[9] > 0.91416342) inputSample = soft[4]+(fabs(soft[4])*sin(soft[9]-0.91416342)*0.08583658);
		if (-soft[9] > 0.91416342) inputSample = soft[4]-(fabs(soft[4])*sin(-soft[9]-0.91416342)*0.08583658);
		if (inputSample > 1.57079633) inputSample = 1.57079633; if (inputSample < -1.57079633) inputSample = -1.57079633;
		//Console8 slew soften: must be clipped or it can generate NAN out of the full system
		if (hsr){
			double outSample = (inputSample * fix[fix_a0]) + fix[fix_sL1];
			fix[fix_sL1] = (inputSample * fix[fix_a1]) - (outSample * fix[fix_b1]) + fix[fix_sL2];
			fix[fix_sL2] = (inputSample * fix[fix_a2]) - (outSample * fix[fix_b2]);
			inputSample = outSample;
		} //fixed biquad filtering ultrasonics
		//we can go directly into the first distortion stage of ChannelOut
		//with a filtered signal, so its biquad is between stages
		//on the input channel we have direct signal, not Console8 decode
		
		inputSample *= inTrim;
		if (inputSample > 1.57079633) inputSample = 1.57079633; if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		if (hsr){
			double outSample = (inputSample * fixB[fix_a0]) + fixB[fix_sL1];
			fixB[fix_sL1] = (inputSample * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
			fixB[fix_sL2] = (inputSample * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
			inputSample = outSample;
		} //fixed biquad filtering ultrasonics
		inputSample *= inTrim;
		if (inputSample > 1.57079633) inputSample = 1.57079633; if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}