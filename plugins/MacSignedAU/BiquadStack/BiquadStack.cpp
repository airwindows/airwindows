/*
*	File:		BiquadStack.cpp
*	
*	Version:	1.0
* 
*	Created:	4/15/24
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
	BiquadStack.cpp
	
=============================================================================*/
#include "BiquadStack.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, BiquadStack)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadStack::BiquadStack
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BiquadStack::BiquadStack(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadStack::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BiquadStack::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadStack::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BiquadStack::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_A:
                AUBase::FillInParameterName (outParameterInfo, kParameterAName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamA;
                break;
            case kParam_B:
                AUBase::FillInParameterName (outParameterInfo, kParameterBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamB;
                break;
            case kParam_C:
                AUBase::FillInParameterName (outParameterInfo, kParameterCName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
				outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamC;
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
//	BiquadStack::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BiquadStack::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadStack::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BiquadStack::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	BiquadStack::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult BiquadStack::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____BiquadStackEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadStack::BiquadStackKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BiquadStack::BiquadStackKernel::Reset()
{
	for (int x = 0; x < biqs_total; x++) {biqs[x] = 0.0;}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadStack::BiquadStackKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BiquadStack::BiquadStackKernel::Process(	const Float32 	*inSourceP,
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
	
	biqs[biqs_levelA] = biqs[biqs_levelB];
	biqs[biqs_aA0] = biqs[biqs_aB0];
	biqs[biqs_bA1] = biqs[biqs_bB1];
	biqs[biqs_bA2] = biqs[biqs_bB2];
	biqs[biqs_cA0] = biqs[biqs_cB0];
	biqs[biqs_dA1] = biqs[biqs_dB1];
	biqs[biqs_dA2] = biqs[biqs_dB2];
	biqs[biqs_eA0] = biqs[biqs_eB0];
	biqs[biqs_fA1] = biqs[biqs_fB1];
	biqs[biqs_fA2] = biqs[biqs_fB2];
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.
	
	biqs[biqs_freq] = (((pow(GetParameter( kParam_A ),4)*19980.0)+20.0)/GetSampleRate());	
	biqs[biqs_nonlin] = GetParameter( kParam_B );
	biqs[biqs_levelB] = (biqs[biqs_nonlin]*2.0)-1.0;
	if (biqs[biqs_levelB] > 0.0) biqs[biqs_levelB] *= 2.0;
	biqs[biqs_reso] = ((0.5+(biqs[biqs_nonlin]*0.5)+sqrt(biqs[biqs_freq]))-(1.0-pow(1.0-GetParameter( kParam_C ),2.0)))+0.5+(biqs[biqs_nonlin]*0.5);

	double K = tan(M_PI * biqs[biqs_freq]);
	double norm = 1.0 / (1.0 + K / (biqs[biqs_reso]*1.93185165) + K * K);
	biqs[biqs_aB0] = K / (biqs[biqs_reso]*1.93185165) * norm;
	biqs[biqs_bB1] = 2.0 * (K * K - 1.0) * norm;
	biqs[biqs_bB2] = (1.0 - K / (biqs[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (biqs[biqs_reso]*0.70710678) + K * K);
	biqs[biqs_cB0] = K / (biqs[biqs_reso]*0.70710678) * norm;
	biqs[biqs_dB1] = 2.0 * (K * K - 1.0) * norm;
	biqs[biqs_dB2] = (1.0 - K / (biqs[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (biqs[biqs_reso]*0.51763809) + K * K);
	biqs[biqs_eB0] = K / (biqs[biqs_reso]*0.51763809) * norm;
	biqs[biqs_fB1] = 2.0 * (K * K - 1.0) * norm;
	biqs[biqs_fB2] = (1.0 - K / (biqs[biqs_reso]*0.51763809) + K * K) * norm;

	if (biqs[biqs_aA0] == 0.0) { // if we have just started, start directly with raw info
		biqs[biqs_levelA] = biqs[biqs_levelB];
		biqs[biqs_aA0] = biqs[biqs_aB0];
		biqs[biqs_bA1] = biqs[biqs_bB1];
		biqs[biqs_bA2] = biqs[biqs_bB2];
		biqs[biqs_cA0] = biqs[biqs_cB0];
		biqs[biqs_dA1] = biqs[biqs_dB1];
		biqs[biqs_dA2] = biqs[biqs_dB2];
		biqs[biqs_eA0] = biqs[biqs_eB0];
		biqs[biqs_fA1] = biqs[biqs_fB1];
		biqs[biqs_fA2] = biqs[biqs_fB2];
	}
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		double buf = (double)nSampleFrames/inFramesToProcess;
		biqs[biqs_level] = (biqs[biqs_levelA]*buf)+(biqs[biqs_levelB]*(1.0-buf));
		biqs[biqs_a0] = (biqs[biqs_aA0]*buf)+(biqs[biqs_aB0]*(1.0-buf));
		biqs[biqs_b1] = (biqs[biqs_bA1]*buf)+(biqs[biqs_bB1]*(1.0-buf));
		biqs[biqs_b2] = (biqs[biqs_bA2]*buf)+(biqs[biqs_bB2]*(1.0-buf));
		biqs[biqs_c0] = (biqs[biqs_cA0]*buf)+(biqs[biqs_cB0]*(1.0-buf));
		biqs[biqs_d1] = (biqs[biqs_dA1]*buf)+(biqs[biqs_dB1]*(1.0-buf));
		biqs[biqs_d2] = (biqs[biqs_dA2]*buf)+(biqs[biqs_dB2]*(1.0-buf));
		biqs[biqs_e0] = (biqs[biqs_eA0]*buf)+(biqs[biqs_eB0]*(1.0-buf));
		biqs[biqs_f1] = (biqs[biqs_fA1]*buf)+(biqs[biqs_fB1]*(1.0-buf));
		biqs[biqs_f2] = (biqs[biqs_fA2]*buf)+(biqs[biqs_fB2]*(1.0-buf));
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		biqs[biqs_outL] = inputSample * fabs(biqs[biqs_level]);
		biqs[biqs_dis] = fabs(biqs[biqs_a0] * (1.0+(biqs[biqs_outL]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outL] * biqs[biqs_dis]) + biqs[biqs_aL1];
		biqs[biqs_aL1] = biqs[biqs_aL2] - (biqs[biqs_temp]*biqs[biqs_b1]);
		biqs[biqs_aL2] = (biqs[biqs_outL] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_b2]);
		biqs[biqs_outL] = biqs[biqs_temp];
		biqs[biqs_dis] = fabs(biqs[biqs_c0] * (1.0+(biqs[biqs_outL]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outL] * biqs[biqs_dis]) + biqs[biqs_cL1];
		biqs[biqs_cL1] = biqs[biqs_cL2] - (biqs[biqs_temp]*biqs[biqs_d1]);
		biqs[biqs_cL2] = (biqs[biqs_outL] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_d2]);
		biqs[biqs_outL] = biqs[biqs_temp];
		biqs[biqs_dis] = fabs(biqs[biqs_e0] * (1.0+(biqs[biqs_outL]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outL] * biqs[biqs_dis]) + biqs[biqs_eL1];
		biqs[biqs_eL1] = biqs[biqs_eL2] - (biqs[biqs_temp]*biqs[biqs_f1]);
		biqs[biqs_eL2] = (biqs[biqs_outL] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_f2]);
		biqs[biqs_outL] = biqs[biqs_temp];
		biqs[biqs_outL] *= biqs[biqs_level];
		if (biqs[biqs_level] > 1.0) biqs[biqs_outL] *= biqs[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		inputSample += biqs[biqs_outL]; //purely a parallel filter stage here
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

