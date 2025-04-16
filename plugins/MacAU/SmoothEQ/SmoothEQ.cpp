/*
*	File:		SmoothEQ.cpp
*	
*	Version:	1.0
* 
*	Created:	3/24/25
*	
*	Copyright:  Copyright © 2025 Airwindows, Airwindows uses the MIT license
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
	SmoothEQ.cpp
	
=============================================================================*/
#include "SmoothEQ.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(SmoothEQ)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SmoothEQ::SmoothEQ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SmoothEQ::SmoothEQ(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
	SetParameter(kParam_D, kDefaultValue_ParamD );
	SetParameter(kParam_E, kDefaultValue_ParamE );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SmoothEQ::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SmoothEQ::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SmoothEQ::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SmoothEQ::GetParameterInfo(AudioUnitScope		inScope,
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
            case kParam_D:
                AUBase::FillInParameterName (outParameterInfo, kParameterDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamD;
                break;
            case kParam_E:
                AUBase::FillInParameterName (outParameterInfo, kParameterEName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamE;
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
//	SmoothEQ::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SmoothEQ::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SmoothEQ::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SmoothEQ::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	SmoothEQ::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult SmoothEQ::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____SmoothEQEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SmoothEQ::SmoothEQKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		SmoothEQ::SmoothEQKernel::Reset()
{
	for (int x = 0; x < biq_total; x++) {
		besselA[x] = 0.0;
		besselB[x] = 0.0;
		besselC[x] = 0.0;
		besselD[x] = 0.0;
		besselE[x] = 0.0;
		besselF[x] = 0.0;
	}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SmoothEQ::SmoothEQKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		SmoothEQ::SmoothEQKernel::Process(	const Float32 	*inSourceP,
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
	
	double trebleGain = GetParameter( kParam_A )*2.0;
	double midGain = GetParameter( kParam_B )*2.0;
	double bassGain = GetParameter( kParam_C )*2.0; //amount ends up being pow(gain,3)
	//simple three band to adjust
	
	besselA[biq_freq] = pow(GetParameter( kParam_D ),2) * (0.25/overallscale) * 1.9047076123;
	if (besselA[biq_freq] < 0.00025) besselA[biq_freq] = 0.00025;
	if (besselA[biq_freq] > 0.4999) besselA[biq_freq] = 0.4999;
	besselA[biq_reso] = 1.02331395383;
	besselB[biq_freq] = pow(GetParameter( kParam_D ),2) * (0.25/overallscale) * 1.68916826762;
	if (besselB[biq_freq] < 0.00025) besselB[biq_freq] = 0.00025;
	if (besselB[biq_freq] > 0.4999) besselB[biq_freq] = 0.4999;
	besselB[biq_reso] = 0.611194546878;
	besselC[biq_freq] = pow(GetParameter( kParam_D ),2) * (0.25/overallscale) * 1.60391912877;
	if (besselC[biq_freq] < 0.00025) besselC[biq_freq] = 0.00025;
	if (besselC[biq_freq] > 0.4999) besselC[biq_freq] = 0.4999;
	besselC[biq_reso] = 0.510317824749;
	
	besselD[biq_freq] = pow(GetParameter( kParam_E ),4) * (0.25/overallscale) * 1.9047076123;
	if (besselD[biq_freq] < 0.00025) besselD[biq_freq] = 0.00025;
	if (besselD[biq_freq] > 0.4999) besselD[biq_freq] = 0.4999;
	besselD[biq_reso] = 1.02331395383;
	besselE[biq_freq] = pow(GetParameter( kParam_E ),4) * (0.25/overallscale) * 1.68916826762;
	if (besselE[biq_freq] < 0.00025) besselE[biq_freq] = 0.00025;
	if (besselE[biq_freq] > 0.4999) besselE[biq_freq] = 0.4999;
	besselE[biq_reso] = 0.611194546878;
	besselF[biq_freq] = pow(GetParameter( kParam_E ),4) * (0.25/overallscale) * 1.60391912877;
	if (besselF[biq_freq] < 0.00025) besselF[biq_freq] = 0.00025;
	if (besselF[biq_freq] > 0.4999) besselF[biq_freq] = 0.4999;
	besselF[biq_reso] = 0.510317824749;
		
	double K = tan(M_PI * besselA[biq_freq]);
	double norm = 1.0 / (1.0 + K / besselA[biq_reso] + K * K);
	besselA[biq_a0] = K * K * norm;
	besselA[biq_a1] = 2.0 * besselA[biq_a0];
	besselA[biq_a2] = besselA[biq_a0];
	besselA[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselA[biq_b2] = (1.0 - K / besselA[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselB[biq_freq]);
	norm = 1.0 / (1.0 + K / besselB[biq_reso] + K * K);
	besselB[biq_a0] = K * K * norm;
	besselB[biq_a1] = 2.0 * besselB[biq_a0];
	besselB[biq_a2] = besselB[biq_a0];
	besselB[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselB[biq_b2] = (1.0 - K / besselB[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselC[biq_freq]);
	norm = 1.0 / (1.0 + K / besselC[biq_reso] + K * K);
	besselC[biq_a0] = K * K * norm;
	besselC[biq_a1] = 2.0 * besselC[biq_a0];
	besselC[biq_a2] = besselC[biq_a0];
	besselC[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselC[biq_b2] = (1.0 - K / besselC[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselD[biq_freq]);
	norm = 1.0 / (1.0 + K / besselD[biq_reso] + K * K);
	besselD[biq_a0] = K * K * norm;
	besselD[biq_a1] = 2.0 * besselD[biq_a0];
	besselD[biq_a2] = besselD[biq_a0];
	besselD[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselD[biq_b2] = (1.0 - K / besselD[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselE[biq_freq]);
	norm = 1.0 / (1.0 + K / besselE[biq_reso] + K * K);
	besselE[biq_a0] = K * K * norm;
	besselE[biq_a1] = 2.0 * besselE[biq_a0];
	besselE[biq_a2] = besselE[biq_a0];
	besselE[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselE[biq_b2] = (1.0 - K / besselE[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselF[biq_freq]);
	norm = 1.0 / (1.0 + K / besselF[biq_reso] + K * K);
	besselF[biq_a0] = K * K * norm;
	besselF[biq_a1] = 2.0 * besselF[biq_a0];
	besselF[biq_a2] = besselF[biq_a0];
	besselF[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselF[biq_b2] = (1.0 - K / besselF[biq_reso] + K * K) * norm;
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpd * 1.18e-17;
		
		double trebleL = inputSampleL;		
		double outSample = (trebleL * besselA[biq_a0]) + besselA[biq_sL1];
		besselA[biq_sL1] = (trebleL * besselA[biq_a1]) - (outSample * besselA[biq_b1]) + besselA[biq_sL2];
		besselA[biq_sL2] = (trebleL * besselA[biq_a2]) - (outSample * besselA[biq_b2]);
		double midL = outSample; trebleL -= midL;
		outSample = (midL * besselD[biq_a0]) + besselD[biq_sL1];
		besselD[biq_sL1] = (midL * besselD[biq_a1]) - (outSample * besselD[biq_b1]) + besselD[biq_sL2];
		besselD[biq_sL2] = (midL * besselD[biq_a2]) - (outSample * besselD[biq_b2]);
		double bassL = outSample; midL -= bassL;
		trebleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		outSample = (trebleL * besselB[biq_a0]) + besselB[biq_sL1];
		besselB[biq_sL1] = (trebleL * besselB[biq_a1]) - (outSample * besselB[biq_b1]) + besselB[biq_sL2];
		besselB[biq_sL2] = (trebleL * besselB[biq_a2]) - (outSample * besselB[biq_b2]);
		midL = outSample; trebleL -= midL;
		outSample = (midL * besselE[biq_a0]) + besselE[biq_sL1];
		besselE[biq_sL1] = (midL * besselE[biq_a1]) - (outSample * besselE[biq_b1]) + besselE[biq_sL2];
		besselE[biq_sL2] = (midL * besselE[biq_a2]) - (outSample * besselE[biq_b2]);
		bassL = outSample; midL -= bassL;
		trebleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		outSample = (trebleL * besselC[biq_a0]) + besselC[biq_sL1];
		besselC[biq_sL1] = (trebleL * besselC[biq_a1]) - (outSample * besselC[biq_b1]) + besselC[biq_sL2];
		besselC[biq_sL2] = (trebleL * besselC[biq_a2]) - (outSample * besselC[biq_b2]);
		midL = outSample; trebleL -= midL;
		outSample = (midL * besselF[biq_a0]) + besselF[biq_sL1];
		besselF[biq_sL1] = (midL * besselF[biq_a1]) - (outSample * besselF[biq_b1]) + besselF[biq_sL2];
		besselF[biq_sL2] = (midL * besselF[biq_a2]) - (outSample * besselF[biq_b2]);
		bassL = outSample; midL -= bassL;
		inputSampleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSampleL;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

