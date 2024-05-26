/*
*	File:		Wolfbot.cpp
*	
*	Version:	1.0
* 
*	Created:	5/22/24
*	
*	Copyright:  Copyright © 2024 Airwindows, Airwindows uses the MIT license
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
	Wolfbot.cpp
	
=============================================================================*/
#include "Wolfbot.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Wolfbot)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Wolfbot::Wolfbot
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Wolfbot::Wolfbot(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Wolfbot::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Wolfbot::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Wolfbot::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Wolfbot::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
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
//	Wolfbot::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Wolfbot::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Wolfbot::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Wolfbot::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Wolfbot::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Wolfbot::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____WolfbotEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Wolfbot::WolfbotKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Wolfbot::WolfbotKernel::Reset()
{
	for (int x = 0; x < kal_total; x++) {kHP[x] = 0.0; kLP[x] = 0.0;}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Wolfbot::WolfbotKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Wolfbot::WolfbotKernel::Process(	const Float32 	*inSourceP,
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
	
	double kalHP = 1.0-(0.004225/overallscale);
	double kalLP = 1.0-(0.954529/overallscale);
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		//begin Kalman Filter
		double dryKal = inputSample = inputSample*(1.0-kalHP)*0.777;
		inputSample *= (1.0-kalHP);
		//set up gain levels to control the beast
		kHP[prevSlewL3] += kHP[prevSampL3] - kHP[prevSampL2]; kHP[prevSlewL3] *= 0.5;
		kHP[prevSlewL2] += kHP[prevSampL2] - kHP[prevSampL1]; kHP[prevSlewL2] *= 0.5;
		kHP[prevSlewL1] += kHP[prevSampL1] - inputSample; kHP[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kHP[accSlewL2] += kHP[prevSlewL3] - kHP[prevSlewL2]; kHP[accSlewL2] *= 0.5;
		kHP[accSlewL1] += kHP[prevSlewL2] - kHP[prevSlewL1]; kHP[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kHP[accSlewL3] += (kHP[accSlewL2] - kHP[accSlewL1]); kHP[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kHP[kalOutL] += kHP[prevSampL1] + kHP[prevSlewL2] + kHP[accSlewL3]; kHP[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kHP[kalGainL] += fabs(dryKal-kHP[kalOutL])*kalHP*8.0; kHP[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kHP[kalGainL] > kalHP*0.5) kHP[kalGainL] = kalHP*0.5;
		//attempts to avoid explosions
		kHP[kalOutL] += (dryKal*(1.0-(0.68+(kalHP*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kHP[prevSampL3] = kHP[prevSampL2];
		kHP[prevSampL2] = kHP[prevSampL1];
		kHP[prevSampL1] = (kHP[kalGainL] * kHP[kalOutL]) + ((1.0-kHP[kalGainL])*dryKal);
		//feed the chain of previous samples
		if (kHP[prevSampL1] > 1.0) kHP[prevSampL1] = 1.0;
		if (kHP[prevSampL1] < -1.0) kHP[prevSampL1] = -1.0;
		//end Kalman Filter, except for trim on output		
		inputSample = drySample+(kHP[kalOutL]*-0.777); //highpass
		
		//begin Kalman Filter
		dryKal = inputSample = inputSample*(1.0-kalLP)*0.777;
		inputSample *= (1.0-kalLP);
		//set up gain levels to control the beast
		kLP[prevSlewL3] += kLP[prevSampL3] - kLP[prevSampL2]; kLP[prevSlewL3] *= 0.5;
		kLP[prevSlewL2] += kLP[prevSampL2] - kLP[prevSampL1]; kLP[prevSlewL2] *= 0.5;
		kLP[prevSlewL1] += kLP[prevSampL1] - inputSample; kLP[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kLP[accSlewL2] += kLP[prevSlewL3] - kLP[prevSlewL2]; kLP[accSlewL2] *= 0.5;
		kLP[accSlewL1] += kLP[prevSlewL2] - kLP[prevSlewL1]; kLP[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kLP[accSlewL3] += (kLP[accSlewL2] - kLP[accSlewL1]); kLP[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kLP[kalOutL] += kLP[prevSampL1] + kLP[prevSlewL2] + kLP[accSlewL3]; kLP[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kLP[kalGainL] += fabs(dryKal-kLP[kalOutL])*kalLP*8.0; kLP[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kLP[kalGainL] > kalLP*0.5) kLP[kalGainL] = kalLP*0.5;
		//attempts to avoid explosions
		kLP[kalOutL] += (dryKal*(1.0-(0.68+(kalLP*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kLP[prevSampL3] = kLP[prevSampL2];
		kLP[prevSampL2] = kLP[prevSampL1];
		kLP[prevSampL1] = (kLP[kalGainL] * kLP[kalOutL]) + ((1.0-kLP[kalGainL])*dryKal);
		//feed the chain of previous samples
		if (kLP[prevSampL1] > 1.0) kLP[prevSampL1] = 1.0;
		if (kLP[prevSampL1] < -1.0) kLP[prevSampL1] = -1.0;
		//end Kalman Filter, except for trim on output		
		inputSample = sin(kLP[kalOutL]*0.7943)*1.2589; //lowpass
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

