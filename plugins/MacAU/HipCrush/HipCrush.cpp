/*
*	File:		HipCrush.cpp
*	
*	Version:	1.0
* 
*	Created:	10/23/25
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
	HipCrush.cpp
	
=============================================================================*/
#include "HipCrush.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(HipCrush)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	HipCrush::HipCrush
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HipCrush::HipCrush(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_TRF, kDefaultValue_ParamTRF );
	SetParameter(kParam_TRG, kDefaultValue_ParamTRG );
	SetParameter(kParam_TRB, kDefaultValue_ParamTRB );
	SetParameter(kParam_HMF, kDefaultValue_ParamHMF );
	SetParameter(kParam_HMG, kDefaultValue_ParamHMG );
	SetParameter(kParam_HMB, kDefaultValue_ParamHMB );
	SetParameter(kParam_LMF, kDefaultValue_ParamLMF );
	SetParameter(kParam_LMG, kDefaultValue_ParamLMG );
	SetParameter(kParam_LMB, kDefaultValue_ParamLMB );
	SetParameter(kParam_DW, kDefaultValue_ParamDW );
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	HipCrush::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			HipCrush::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	HipCrush::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			HipCrush::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_TRF:
                AUBase::FillInParameterName (outParameterInfo, kParameterTRFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = CFSTR("High");
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTRF;
                break;
            case kParam_TRG:
                AUBase::FillInParameterName (outParameterInfo, kParameterTRGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTRG;
                break;
            case kParam_TRB:
                AUBase::FillInParameterName (outParameterInfo, kParameterTRBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTRB;
                break;
            case kParam_HMF:
                AUBase::FillInParameterName (outParameterInfo, kParameterHMFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
                outParameterInfo.unitName = CFSTR("Mid");
				outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHMF;
                break;
            case kParam_HMG:
                AUBase::FillInParameterName (outParameterInfo, kParameterHMGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHMG;
                break;
            case kParam_HMB:
                AUBase::FillInParameterName (outParameterInfo, kParameterHMBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHMB;
                break;
            case kParam_LMF:
                AUBase::FillInParameterName (outParameterInfo, kParameterLMFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
                outParameterInfo.unitName = CFSTR("Low");
				outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLMF;
                break;
            case kParam_LMG:
                AUBase::FillInParameterName (outParameterInfo, kParameterLMGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLMG;
                break;
            case kParam_LMB:
                AUBase::FillInParameterName (outParameterInfo, kParameterLMBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLMB;
                break;
            case kParam_DW:
                AUBase::FillInParameterName (outParameterInfo, kParameterDWName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
                outParameterInfo.unitName = CFSTR("Wet");
				outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamDW;
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
//	HipCrush::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			HipCrush::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	HipCrush::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			HipCrush::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	HipCrush::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult HipCrush::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____HipCrushEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	HipCrush::HipCrushKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		HipCrush::HipCrushKernel::Reset()
{
	for (int x = 0; x < biqs_total; x++) {
		high[x] = 0.0;
		hmid[x] = 0.0;
		lmid[x] = 0.0;
	}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	HipCrush::HipCrushKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		HipCrush::HipCrushKernel::Process(	const Float32 	*inSourceP,
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
	
	high[biqs_freq] = (((pow(GetParameter( kParam_TRF ),3)*16000.0)+1000.0)/GetSampleRate());
	if (high[biqs_freq] < 0.0001) high[biqs_freq] = 0.0001;
	high[biqs_bit] = (GetParameter( kParam_TRB )*2.0)-1.0;
	high[biqs_level] = (1.0-pow(1.0-GetParameter( kParam_TRG ),2.0))*1.618033988749894848204586;
	high[biqs_reso] = pow(GetParameter( kParam_TRG )+0.618033988749894848204586,2.0);
	double K = tan(M_PI * high[biqs_freq]);
	double norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.618033988749894848204586) + K * K);
	high[biqs_a0] = K / (high[biqs_reso]*0.618033988749894848204586) * norm;
	high[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_b2] = (1.0 - K / (high[biqs_reso]*0.618033988749894848204586) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (high[biqs_reso]*1.618033988749894848204586) + K * K);
	high[biqs_c0] = K / (high[biqs_reso]*1.618033988749894848204586) * norm;
	high[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_d2] = (1.0 - K / (high[biqs_reso]*1.618033988749894848204586) + K * K) * norm;
	//high
	
	hmid[biqs_freq] = (((pow(GetParameter( kParam_HMF ),3)*7000.0)+300.0)/GetSampleRate());
	if (hmid[biqs_freq] < 0.0001) hmid[biqs_freq] = 0.0001;
	hmid[biqs_bit] = (GetParameter( kParam_HMB )*2.0)-1.0;
	hmid[biqs_level] = (1.0-pow(1.0-GetParameter( kParam_HMG ),2.0))*1.618033988749894848204586;
	hmid[biqs_reso] = pow(GetParameter( kParam_HMG )+0.618033988749894848204586,2.0);
	K = tan(M_PI * hmid[biqs_freq]);
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.618033988749894848204586) + K * K);
	hmid[biqs_a0] = K / (hmid[biqs_reso]*0.618033988749894848204586) * norm;
	hmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_b2] = (1.0 - K / (hmid[biqs_reso]*0.618033988749894848204586) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*1.618033988749894848204586) + K * K);
	hmid[biqs_c0] = K / (hmid[biqs_reso]*1.618033988749894848204586) * norm;
	hmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_d2] = (1.0 - K / (hmid[biqs_reso]*1.618033988749894848204586) + K * K) * norm;
	//hmid
	
	lmid[biqs_freq] = (((pow(GetParameter( kParam_LMF ),3)*3000.0)+20.0)/GetSampleRate());
	if (lmid[biqs_freq] < 0.00001) lmid[biqs_freq] = 0.00001;
	lmid[biqs_bit] = (GetParameter( kParam_LMB )*2.0)-1.0;
	lmid[biqs_level] = (1.0-pow(1.0-GetParameter( kParam_LMG ),2.0))*1.618033988749894848204586;
	lmid[biqs_reso] = pow(GetParameter( kParam_LMG )+0.618033988749894848204586,2.0);
	K = tan(M_PI * lmid[biqs_freq]);
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.618033988749894848204586) + K * K);
	lmid[biqs_a0] = K / (lmid[biqs_reso]*0.618033988749894848204586) * norm;
	lmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_b2] = (1.0 - K / (lmid[biqs_reso]*0.618033988749894848204586) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*1.618033988749894848204586) + K * K);
	lmid[biqs_c0] = K / (lmid[biqs_reso]*1.618033988749894848204586) * norm;
	lmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_d2] = (1.0 - K / (lmid[biqs_reso]*1.618033988749894848204586) + K * K) * norm;
	//lmid
	
	double wet = GetParameter( kParam_DW );
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;

		//begin Stacked Biquad With Reversed Neutron Flow L
		high[biqs_outL] = inputSample * fabs(high[biqs_level]);
		high[biqs_temp] = (high[biqs_outL] * high[biqs_a0]) + high[biqs_aL1];
		high[biqs_aL1] = high[biqs_aL2] - (high[biqs_temp]*high[biqs_b1]);
		high[biqs_aL2] = (high[biqs_outL] * -high[biqs_a0]) - (high[biqs_temp]*high[biqs_b2]);
		high[biqs_outL] = high[biqs_temp];
		if (high[biqs_bit] != 0.0) {
			double bitFactor = high[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			high[biqs_outL] *= bitFactor;		
			high[biqs_outL] = floor(high[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
			high[biqs_outL] /= bitFactor;
		}
		high[biqs_temp] = (high[biqs_outL] * high[biqs_c0]) + high[biqs_cL1];
		high[biqs_cL1] = high[biqs_cL2] - (high[biqs_temp]*high[biqs_d1]);
		high[biqs_cL2] = (high[biqs_outL] * -high[biqs_c0]) - (high[biqs_temp]*high[biqs_d2]);
		high[biqs_outL] = high[biqs_temp];
		high[biqs_outL] *= high[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		hmid[biqs_outL] = inputSample * fabs(hmid[biqs_level]);
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_a0]) + hmid[biqs_aL1];
		hmid[biqs_aL1] = hmid[biqs_aL2] - (hmid[biqs_temp]*hmid[biqs_b1]);
		hmid[biqs_aL2] = (hmid[biqs_outL] * -hmid[biqs_a0]) - (hmid[biqs_temp]*hmid[biqs_b2]);
		hmid[biqs_outL] = hmid[biqs_temp];
		if (hmid[biqs_bit] != 0.0) {
			double bitFactor = hmid[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			hmid[biqs_outL] *= bitFactor;		
			hmid[biqs_outL] = floor(hmid[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
			hmid[biqs_outL] /= bitFactor;
		}
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_c0]) + hmid[biqs_cL1];
		hmid[biqs_cL1] = hmid[biqs_cL2] - (hmid[biqs_temp]*hmid[biqs_d1]);
		hmid[biqs_cL2] = (hmid[biqs_outL] * -hmid[biqs_c0]) - (hmid[biqs_temp]*hmid[biqs_d2]);
		hmid[biqs_outL] = hmid[biqs_temp];
		hmid[biqs_outL] *= hmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		lmid[biqs_outL] = inputSample * fabs(lmid[biqs_level]);
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_a0]) + lmid[biqs_aL1];
		lmid[biqs_aL1] = lmid[biqs_aL2] - (lmid[biqs_temp]*lmid[biqs_b1]);
		lmid[biqs_aL2] = (lmid[biqs_outL] * -lmid[biqs_a0]) - (lmid[biqs_temp]*lmid[biqs_b2]);
		lmid[biqs_outL] = lmid[biqs_temp];
		if (lmid[biqs_bit] != 0.0) {
			double bitFactor = lmid[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			lmid[biqs_outL] *= bitFactor;		
			lmid[biqs_outL] = floor(lmid[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
			lmid[biqs_outL] /= bitFactor;
		}
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_c0]) + lmid[biqs_cL1];
		lmid[biqs_cL1] = lmid[biqs_cL2] - (lmid[biqs_temp]*lmid[biqs_d1]);
		lmid[biqs_cL2] = (lmid[biqs_outL] * -lmid[biqs_c0]) - (lmid[biqs_temp]*lmid[biqs_d2]);
		lmid[biqs_outL] = lmid[biqs_temp];
		lmid[biqs_outL] *= lmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		double parametric = high[biqs_outL] + hmid[biqs_outL] + lmid[biqs_outL];
		inputSample = (drySample * (1.0-wet)) + (parametric * wet);
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

