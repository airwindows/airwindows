/*
*	File:		ConsoleHBuss.cpp
*	
*	Version:	1.0
* 
*	Created:	11/4/25
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
	ConsoleHBuss.cpp
	
=============================================================================*/
#include "ConsoleHBuss.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(ConsoleHBuss)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHBuss::ConsoleHBuss
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ConsoleHBuss::ConsoleHBuss(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_HIG, kDefaultValue_ParamHIG );
	SetParameter(kParam_MID, kDefaultValue_ParamMID );
	SetParameter(kParam_LOW, kDefaultValue_ParamLOW );
	
	SetParameter(kParam_THR, kDefaultValue_ParamTHR );

	SetParameter(kParam_LOP, kDefaultValue_ParamLOP );
	SetParameter(kParam_HIP, kDefaultValue_ParamHIP );
	
	SetParameter(kParam_PAN, kDefaultValue_ParamPAN );
	SetParameter(kParam_FAD, kDefaultValue_ParamFAD );
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHBuss::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleHBuss::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHBuss::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleHBuss::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_HIG:
                AUBase::FillInParameterName (outParameterInfo, kParameterHIGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterHIGUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHIG;
                break;
            case kParam_MID:
                AUBase::FillInParameterName (outParameterInfo, kParameterMIDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamMID;
                break;
            case kParam_LOW:
                AUBase::FillInParameterName (outParameterInfo, kParameterLOWName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLOW;
                break;
								
            case kParam_THR:
                AUBase::FillInParameterName (outParameterInfo, kParameterTHRName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterTHRUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTHR;
                break;
				
            case kParam_LOP:
                AUBase::FillInParameterName (outParameterInfo, kParameterLOPName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterLOPUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLOP;
                break;
            case kParam_HIP:
                AUBase::FillInParameterName (outParameterInfo, kParameterHIPName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHIP;
                break;
				
            case kParam_PAN:
                AUBase::FillInParameterName (outParameterInfo, kParameterPANName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamPAN;
                break;
            case kParam_FAD:
                AUBase::FillInParameterName (outParameterInfo, kParameterFADName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFAD;
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
//	ConsoleHBuss::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleHBuss::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 ConsoleHBuss::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	ConsoleHBuss::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleHBuss::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ConsoleHBuss::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ConsoleHBuss::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ConsoleHBussEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHBuss::ConsoleHBussKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		ConsoleHBuss::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for (int x = 0; x < biq_total; x++) {
		highFast[x] = 0.0;
		lowFast[x] = 0.0;
	}
	highFastLIIR = 0.0;
	highFastRIIR = 0.0;
	lowFastLIIR = 0.0;
	lowFastRIIR = 0.0;
	//SmoothEQ3
	
	for (int x = 0; x < bez_total; x++) {bezCompF[x] = 0.0;bezCompS[x] = 0.0;}
	bezCompF[bez_cycle] = 1.0; bezMaxF = 0.0;
	bezCompS[bez_cycle] = 1.0;
	//Dynamics2
	
	for (int x = 0; x < hilp_total; x++) {
		highpass[x] = 0.0;
		lowpass[x] = 0.0;
	}	
	
	for (int x = 0; x < 33; x++) {avg32L[x] = 0.0; avg32R[x] = 0.0;}
	for (int x = 0; x < 17; x++) {avg16L[x] = 0.0; avg16R[x] = 0.0;}
	for (int x = 0; x < 9; x++) {avg8L[x] = 0.0; avg8R[x] = 0.0;}
	for (int x = 0; x < 5; x++) {avg4L[x] = 0.0; avg4R[x] = 0.0;}
	for (int x = 0; x < 3; x++) {avg2L[x] = 0.0; avg2R[x] = 0.0;}
	avgPos = 0;
	lastSlewL = 0.0; lastSlewR = 0.0;
	lastSlewpleL = 0.0; lastSlewpleR = 0.0;
	//preTapeHack
	
	panA = 0.5; panB = 0.5;
	inTrimA = 0.5; inTrimB = 0.5;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHBuss::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		ConsoleHBuss::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
													const AudioBufferList & inBuffer,
                                                    AudioBufferList & outBuffer,
                                                    UInt32 			inFramesToProcess)
{
	Float32 * inputL = (Float32*)(inBuffer.mBuffers[0].mData);
	Float32 * inputR = (Float32*)(inBuffer.mBuffers[1].mData);
	Float32 * outputL = (Float32*)(outBuffer.mBuffers[0].mData);
	Float32 * outputR = (Float32*)(outBuffer.mBuffers[1].mData);
	UInt32 nSampleFrames = inFramesToProcess;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	int spacing = floor(overallscale*2.0);
	if (spacing < 2) spacing = 2; if (spacing > 32) spacing = 32;
	
	double trebleGain = (GetParameter( kParam_HIG )-0.5)*2.0;
	trebleGain = 1.0+(trebleGain*fabs(trebleGain)*fabs(trebleGain));
	double midGain = (GetParameter( kParam_MID )-0.5)*2.0;
	midGain = 1.0+(midGain*fabs(midGain)*fabs(midGain));
	double bassGain = (GetParameter( kParam_LOW )-0.5)*2.0;
	bassGain = 1.0+(bassGain*fabs(bassGain)*fabs(bassGain));
	//separate from filtering stage, this is amplitude, centered on 1.0 unity gain
	
	//SmoothEQ3 is how to get 3rd order steepness at very low CPU.
	//because sample rate varies, you could also vary the crossovers
	//you can't vary Q because math is simplified to take advantage of
	//how the accurate Q value for this filter is always exactly 1.0.
	highFast[biq_freq] = (4000.0/GetSampleRate());
	double omega = 2.0*M_PI*(4000.0/GetSampleRate()); //mid-high crossover freq
	double biqK = 2.0 - cos(omega);
	double highCoef = -sqrt(biqK*biqK - 1.0) + biqK;
	lowFast[biq_freq] = (200.0/GetSampleRate());
	omega = 2.0*M_PI*(200.0/GetSampleRate()); //low-mid crossover freq
	biqK = 2.0 - cos(omega);
	double lowCoef = -sqrt(biqK*biqK - 1.0) + biqK;
	//exponential IIR filter as part of an accurate 3rd order Butterworth filter 
	biqK = tan(M_PI * highFast[biq_freq]);
	double norm = 1.0 / (1.0 + biqK + biqK*biqK);
	highFast[biq_a0] = biqK * biqK * norm;
	highFast[biq_a1] = 2.0 * highFast[biq_a0];
	highFast[biq_a2] = highFast[biq_a0];
	highFast[biq_b1] = 2.0 * (biqK*biqK - 1.0) * norm;
	highFast[biq_b2] = (1.0 - biqK + biqK*biqK) * norm;
	biqK = tan(M_PI * lowFast[biq_freq]);
	norm = 1.0 / (1.0 + biqK + biqK*biqK);
	lowFast[biq_a0] = biqK * biqK * norm;
	lowFast[biq_a1] = 2.0 * lowFast[biq_a0];
	lowFast[biq_a2] = lowFast[biq_a0];
	lowFast[biq_b1] = 2.0 * (biqK*biqK - 1.0) * norm;
	lowFast[biq_b2] = (1.0 - biqK + biqK*biqK) * norm;
	//custom biquad setup with Q = 1.0 gets to omit some divides
	//SmoothEQ3
	
	double bezCThresh = pow(1.0-GetParameter( kParam_THR ), 6.0) * 8.0;
	double bezRez = pow(1.0-GetParameter( kParam_THR ), 12.360679774997898) / overallscale;
	double sloRez = pow(1.0-GetParameter( kParam_THR ),10.0) / overallscale;
	sloRez = fmin(fmax(sloRez,0.00001),1.0);
	bezRez = fmin(fmax(bezRez,0.00001),1.0);
	//Dynamics2
	
	highpass[hilp_freq] = ((pow(GetParameter( kParam_HIP ),3)*24000.0)+10.0)/GetSampleRate();
	if (highpass[hilp_freq] > 0.495) highpass[hilp_freq] = 0.495;
	bool highpassEngage = true; if (GetParameter( kParam_HIP ) == 0.0) highpassEngage = false;
	
	lowpass[hilp_freq] = ((pow(GetParameter( kParam_LOP ),3)*24000.0)+10.0)/GetSampleRate();
	if (lowpass[hilp_freq] > 0.495) lowpass[hilp_freq] = 0.495;
	bool lowpassEngage = true; if (GetParameter( kParam_LOP ) == 0.0) lowpassEngage = false;
	
	highpass[hilp_aA0] = highpass[hilp_aB0];
	highpass[hilp_aA1] = highpass[hilp_aB1];
	highpass[hilp_bA1] = highpass[hilp_bB1];
	highpass[hilp_bA2] = highpass[hilp_bB2];
	highpass[hilp_cA0] = highpass[hilp_cB0];
	highpass[hilp_cA1] = highpass[hilp_cB1];
	highpass[hilp_dA1] = highpass[hilp_dB1];
	highpass[hilp_dA2] = highpass[hilp_dB2];
	highpass[hilp_eA0] = highpass[hilp_eB0];
	highpass[hilp_eA1] = highpass[hilp_eB1];
	highpass[hilp_fA1] = highpass[hilp_fB1];
	highpass[hilp_fA2] = highpass[hilp_fB2];
	lowpass[hilp_aA0] = lowpass[hilp_aB0];
	lowpass[hilp_aA1] = lowpass[hilp_aB1];
	lowpass[hilp_bA1] = lowpass[hilp_bB1];
	lowpass[hilp_bA2] = lowpass[hilp_bB2];
	lowpass[hilp_cA0] = lowpass[hilp_cB0];
	lowpass[hilp_cA1] = lowpass[hilp_cB1];
	lowpass[hilp_dA1] = lowpass[hilp_dB1];
	lowpass[hilp_dA2] = lowpass[hilp_dB2];
	lowpass[hilp_eA0] = lowpass[hilp_eB0];
	lowpass[hilp_eA1] = lowpass[hilp_eB1];
	lowpass[hilp_fA1] = lowpass[hilp_fB1];
	lowpass[hilp_fA2] = lowpass[hilp_fB2];
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.
	//On the buss, highpass and lowpass are isolators meant to be moved,
	//so they are interpolated where the channels are not
	
	biqK = tan(M_PI * highpass[hilp_freq]); //highpass
	norm = 1.0 / (1.0 + biqK / 1.93185165 + biqK * biqK);
	highpass[hilp_aB0] = norm;
	highpass[hilp_aB1] = -2.0 * highpass[hilp_aB0];
	highpass[hilp_bB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highpass[hilp_bB2] = (1.0 - biqK / 1.93185165 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.70710678 + biqK * biqK);
	highpass[hilp_cB0] = norm;
	highpass[hilp_cB1] = -2.0 * highpass[hilp_cB0];
	highpass[hilp_dB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highpass[hilp_dB2] = (1.0 - biqK / 0.70710678 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.51763809 + biqK * biqK);
	highpass[hilp_eB0] = norm;
	highpass[hilp_eB1] = -2.0 * highpass[hilp_eB0];
	highpass[hilp_fB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highpass[hilp_fB2] = (1.0 - biqK / 0.51763809 + biqK * biqK) * norm;
	
	biqK = tan(M_PI * lowpass[hilp_freq]); //lowpass
	norm = 1.0 / (1.0 + biqK / 1.93185165 + biqK * biqK);
	lowpass[hilp_aB0] = biqK * biqK * norm;
	lowpass[hilp_aB1] = 2.0 * lowpass[hilp_aB0];
	lowpass[hilp_bB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowpass[hilp_bB2] = (1.0 - biqK / 1.93185165 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.70710678 + biqK * biqK);
	lowpass[hilp_cB0] = biqK * biqK * norm;
	lowpass[hilp_cB1] = 2.0 * lowpass[hilp_cB0];
	lowpass[hilp_dB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowpass[hilp_dB2] = (1.0 - biqK / 0.70710678 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.51763809 + biqK * biqK);
	lowpass[hilp_eB0] = biqK * biqK * norm;
	lowpass[hilp_eB1] = 2.0 * lowpass[hilp_eB0];
	lowpass[hilp_fB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowpass[hilp_fB2] = (1.0 - biqK / 0.51763809 + biqK * biqK) * norm;
	
	if (highpass[hilp_aA0] == 0.0) { // if we have just started, start directly with raw info
		highpass[hilp_aA0] = highpass[hilp_aB0];
		highpass[hilp_aA1] = highpass[hilp_aB1];
		highpass[hilp_bA1] = highpass[hilp_bB1];
		highpass[hilp_bA2] = highpass[hilp_bB2];
		highpass[hilp_cA0] = highpass[hilp_cB0];
		highpass[hilp_cA1] = highpass[hilp_cB1];
		highpass[hilp_dA1] = highpass[hilp_dB1];
		highpass[hilp_dA2] = highpass[hilp_dB2];
		highpass[hilp_eA0] = highpass[hilp_eB0];
		highpass[hilp_eA1] = highpass[hilp_eB1];
		highpass[hilp_fA1] = highpass[hilp_fB1];
		highpass[hilp_fA2] = highpass[hilp_fB2];
		lowpass[hilp_aA0] = lowpass[hilp_aB0];
		lowpass[hilp_aA1] = lowpass[hilp_aB1];
		lowpass[hilp_bA1] = lowpass[hilp_bB1];
		lowpass[hilp_bA2] = lowpass[hilp_bB2];
		lowpass[hilp_cA0] = lowpass[hilp_cB0];
		lowpass[hilp_cA1] = lowpass[hilp_cB1];
		lowpass[hilp_dA1] = lowpass[hilp_dB1];
		lowpass[hilp_dA2] = lowpass[hilp_dB2];
		lowpass[hilp_eA0] = lowpass[hilp_eB0];
		lowpass[hilp_eA1] = lowpass[hilp_eB1];
		lowpass[hilp_fA1] = lowpass[hilp_fB1];
		lowpass[hilp_fA2] = lowpass[hilp_fB2];
	}
	
	panA = panB; panB = GetParameter( kParam_PAN )*1.57079633;
	inTrimA = inTrimB; inTrimB = GetParameter( kParam_FAD )*2.0;
	//Console
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		const double temp = (double)nSampleFrames/inFramesToProcess;
		highpass[hilp_a0] = (highpass[hilp_aA0]*temp)+(highpass[hilp_aB0]*(1.0-temp));
		highpass[hilp_a1] = (highpass[hilp_aA1]*temp)+(highpass[hilp_aB1]*(1.0-temp));
		highpass[hilp_b1] = (highpass[hilp_bA1]*temp)+(highpass[hilp_bB1]*(1.0-temp));
		highpass[hilp_b2] = (highpass[hilp_bA2]*temp)+(highpass[hilp_bB2]*(1.0-temp));
		highpass[hilp_c0] = (highpass[hilp_cA0]*temp)+(highpass[hilp_cB0]*(1.0-temp));
		highpass[hilp_c1] = (highpass[hilp_cA1]*temp)+(highpass[hilp_cB1]*(1.0-temp));
		highpass[hilp_d1] = (highpass[hilp_dA1]*temp)+(highpass[hilp_dB1]*(1.0-temp));
		highpass[hilp_d2] = (highpass[hilp_dA2]*temp)+(highpass[hilp_dB2]*(1.0-temp));
		highpass[hilp_e0] = (highpass[hilp_eA0]*temp)+(highpass[hilp_eB0]*(1.0-temp));
		highpass[hilp_e1] = (highpass[hilp_eA1]*temp)+(highpass[hilp_eB1]*(1.0-temp));
		highpass[hilp_f1] = (highpass[hilp_fA1]*temp)+(highpass[hilp_fB1]*(1.0-temp));
		highpass[hilp_f2] = (highpass[hilp_fA2]*temp)+(highpass[hilp_fB2]*(1.0-temp));
		lowpass[hilp_a0] = (lowpass[hilp_aA0]*temp)+(lowpass[hilp_aB0]*(1.0-temp));
		lowpass[hilp_a1] = (lowpass[hilp_aA1]*temp)+(lowpass[hilp_aB1]*(1.0-temp));
		lowpass[hilp_b1] = (lowpass[hilp_bA1]*temp)+(lowpass[hilp_bB1]*(1.0-temp));
		lowpass[hilp_b2] = (lowpass[hilp_bA2]*temp)+(lowpass[hilp_bB2]*(1.0-temp));
		lowpass[hilp_c0] = (lowpass[hilp_cA0]*temp)+(lowpass[hilp_cB0]*(1.0-temp));
		lowpass[hilp_c1] = (lowpass[hilp_cA1]*temp)+(lowpass[hilp_cB1]*(1.0-temp));
		lowpass[hilp_d1] = (lowpass[hilp_dA1]*temp)+(lowpass[hilp_dB1]*(1.0-temp));
		lowpass[hilp_d2] = (lowpass[hilp_dA2]*temp)+(lowpass[hilp_dB2]*(1.0-temp));
		lowpass[hilp_e0] = (lowpass[hilp_eA0]*temp)+(lowpass[hilp_eB0]*(1.0-temp));
		lowpass[hilp_e1] = (lowpass[hilp_eA1]*temp)+(lowpass[hilp_eB1]*(1.0-temp));
		lowpass[hilp_f1] = (lowpass[hilp_fA1]*temp)+(lowpass[hilp_fB1]*(1.0-temp));
		lowpass[hilp_f2] = (lowpass[hilp_fA2]*temp)+(lowpass[hilp_fB2]*(1.0-temp));
		
		
		if (highpassEngage) { //distributed Highpass
			highpass[hilp_temp] = (inputSampleL*highpass[hilp_a0])+highpass[hilp_aL1];
			highpass[hilp_aL1] = (inputSampleL*highpass[hilp_a1])-(highpass[hilp_temp]*highpass[hilp_b1])+highpass[hilp_aL2];
			highpass[hilp_aL2] = (inputSampleL*highpass[hilp_a0])-(highpass[hilp_temp]*highpass[hilp_b2]); inputSampleL = highpass[hilp_temp];
			highpass[hilp_temp] = (inputSampleR*highpass[hilp_a0])+highpass[hilp_aR1];
			highpass[hilp_aR1] = (inputSampleR*highpass[hilp_a1])-(highpass[hilp_temp]*highpass[hilp_b1])+highpass[hilp_aR2];
			highpass[hilp_aR2] = (inputSampleR*highpass[hilp_a0])-(highpass[hilp_temp]*highpass[hilp_b2]); inputSampleR = highpass[hilp_temp];
		} else highpass[hilp_aR1] = highpass[hilp_aR2] = highpass[hilp_aL1] = highpass[hilp_aL2] = 0.0;
		if (lowpassEngage) { //distributed Lowpass
			lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_a0])+lowpass[hilp_aL1];
			lowpass[hilp_aL1] = (inputSampleL*lowpass[hilp_a1])-(lowpass[hilp_temp]*lowpass[hilp_b1])+lowpass[hilp_aL2];
			lowpass[hilp_aL2] = (inputSampleL*lowpass[hilp_a0])-(lowpass[hilp_temp]*lowpass[hilp_b2]); inputSampleL = lowpass[hilp_temp];
			lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_a0])+lowpass[hilp_aR1];
			lowpass[hilp_aR1] = (inputSampleR*lowpass[hilp_a1])-(lowpass[hilp_temp]*lowpass[hilp_b1])+lowpass[hilp_aR2];
			lowpass[hilp_aR2] = (inputSampleR*lowpass[hilp_a0])-(lowpass[hilp_temp]*lowpass[hilp_b2]); inputSampleR = lowpass[hilp_temp];
		} else lowpass[hilp_aR1] = lowpass[hilp_aR2] = lowpass[hilp_aL1] = lowpass[hilp_aL2] = 0.0;
		//first Highpass/Lowpass blocks aliasing before the nonlinearity of ConsoleXBuss and Parametric		
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 0.6180339887498949));
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 0.6180339887498949));
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 0.6180339887498949));
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 0.6180339887498949));
		
		if (highpassEngage) { //distributed Highpass
			highpass[hilp_temp] = (inputSampleL*highpass[hilp_c0])+highpass[hilp_cL1];
			highpass[hilp_cL1] = (inputSampleL*highpass[hilp_c1])-(highpass[hilp_temp]*highpass[hilp_d1])+highpass[hilp_cL2];
			highpass[hilp_cL2] = (inputSampleL*highpass[hilp_c0])-(highpass[hilp_temp]*highpass[hilp_d2]); inputSampleL = highpass[hilp_temp];
			highpass[hilp_temp] = (inputSampleR*highpass[hilp_c0])+highpass[hilp_cR1];
			highpass[hilp_cR1] = (inputSampleR*highpass[hilp_c1])-(highpass[hilp_temp]*highpass[hilp_d1])+highpass[hilp_cR2];
			highpass[hilp_cR2] = (inputSampleR*highpass[hilp_c0])-(highpass[hilp_temp]*highpass[hilp_d2]); inputSampleR = highpass[hilp_temp];
		} else highpass[hilp_cR1] = highpass[hilp_cR2] = highpass[hilp_cL1] = highpass[hilp_cL2] = 0.0;
		if (lowpassEngage) { //distributed Lowpass
			lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_c0])+lowpass[hilp_cL1];
			lowpass[hilp_cL1] = (inputSampleL*lowpass[hilp_c1])-(lowpass[hilp_temp]*lowpass[hilp_d1])+lowpass[hilp_cL2];
			lowpass[hilp_cL2] = (inputSampleL*lowpass[hilp_c0])-(lowpass[hilp_temp]*lowpass[hilp_d2]); inputSampleL = lowpass[hilp_temp];
			lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_c0])+lowpass[hilp_cR1];
			lowpass[hilp_cR1] = (inputSampleR*lowpass[hilp_c1])-(lowpass[hilp_temp]*lowpass[hilp_d1])+lowpass[hilp_cR2];
			lowpass[hilp_cR2] = (inputSampleR*lowpass[hilp_c0])-(lowpass[hilp_temp]*lowpass[hilp_d2]); inputSampleR = lowpass[hilp_temp];
		} else lowpass[hilp_cR1] = lowpass[hilp_cR2] = lowpass[hilp_cL1] = lowpass[hilp_cL2] = 0.0;
		//another stage of Highpass/Lowpass before bringing in the parametric bands		
		
		double trebleFastL = inputSampleL;		
		double outSample = (trebleFastL * highFast[biq_a0]) + highFast[biq_sL1];
		highFast[biq_sL1] = (trebleFastL * highFast[biq_a1]) - (outSample * highFast[biq_b1]) + highFast[biq_sL2];
		highFast[biq_sL2] = (trebleFastL * highFast[biq_a2]) - (outSample * highFast[biq_b2]);
		double midFastL = outSample; trebleFastL -= midFastL;
		outSample = (midFastL * lowFast[biq_a0]) + lowFast[biq_sL1];
		lowFast[biq_sL1] = (midFastL * lowFast[biq_a1]) - (outSample * lowFast[biq_b1]) + lowFast[biq_sL2];
		lowFast[biq_sL2] = (midFastL * lowFast[biq_a2]) - (outSample * lowFast[biq_b2]);
		double bassFastL = outSample; midFastL -= bassFastL;
		trebleFastL = (bassFastL*bassGain) + (midFastL*midGain) + (trebleFastL*trebleGain);
		//first stage of two crossovers is biquad of exactly 1.0 Q
		highFastLIIR = (highFastLIIR*highCoef) + (trebleFastL*(1.0-highCoef));
		midFastL = highFastLIIR; trebleFastL -= midFastL;
		lowFastLIIR = (lowFastLIIR*lowCoef) + (midFastL*(1.0-lowCoef));
		bassFastL = lowFastLIIR; midFastL -= bassFastL;
		inputSampleL = (bassFastL*bassGain) + (midFastL*midGain) + (trebleFastL*trebleGain);		
		//second stage of two crossovers is the exponential filters
		//this produces a slightly steeper Butterworth filter very cheaply
		
		double trebleFastR = inputSampleR;		
		outSample = (trebleFastR * highFast[biq_a0]) + highFast[biq_sR1];
		highFast[biq_sR1] = (trebleFastR * highFast[biq_a1]) - (outSample * highFast[biq_b1]) + highFast[biq_sR2];
		highFast[biq_sR2] = (trebleFastR * highFast[biq_a2]) - (outSample * highFast[biq_b2]);
		double midFastR = outSample; trebleFastR -= midFastR;
		outSample = (midFastR * lowFast[biq_a0]) + lowFast[biq_sR1];
		lowFast[biq_sR1] = (midFastR * lowFast[biq_a1]) - (outSample * lowFast[biq_b1]) + lowFast[biq_sR2];
		lowFast[biq_sR2] = (midFastR * lowFast[biq_a2]) - (outSample * lowFast[biq_b2]);
		double bassFastR = outSample; midFastR -= bassFastR;
		trebleFastR = (bassFastR*bassGain) + (midFastR*midGain) + (trebleFastR*trebleGain);
		//first stage of two crossovers is biquad of exactly 1.0 Q
		highFastRIIR = (highFastRIIR*highCoef) + (trebleFastR*(1.0-highCoef));
		midFastR = highFastRIIR; trebleFastR -= midFastR;
		lowFastRIIR = (lowFastRIIR*lowCoef) + (midFastR*(1.0-lowCoef));
		bassFastR = lowFastRIIR; midFastR -= bassFastR;
		inputSampleR = (bassFastR*bassGain) + (midFastR*midGain) + (trebleFastR*trebleGain);		
		//second stage of two crossovers is the exponential filters
		//this produces a slightly steeper Butterworth filter very cheaply
		//SmoothEQ3
		
		if (bezCThresh > 0.0) {
			inputSampleL *= ((bezCThresh*0.5)+1.0);
			inputSampleR *= ((bezCThresh*0.5)+1.0);
		}
		
		bezCompF[bez_cycle] += bezRez;
		bezCompF[bez_SampL] += (fabs(inputSampleL) * bezRez);
		bezCompF[bez_SampR] += (fabs(inputSampleR) * bezRez);
		bezMaxF = fmax(bezMaxF,fmax(fabs(inputSampleL),fabs(inputSampleR)));
		
		if (bezCompF[bez_cycle] > 1.0) {
			bezCompF[bez_cycle] -= 1.0;
			bezCompF[bez_CL] = bezCompF[bez_BL];
			bezCompF[bez_BL] = bezCompF[bez_AL];
			bezCompF[bez_AL] = bezCompF[bez_SampL];
			bezCompF[bez_SampL] = 0.0;
			bezCompF[bez_CR] = bezCompF[bez_BR];
			bezCompF[bez_BR] = bezCompF[bez_AR];
			bezCompF[bez_AR] = bezCompF[bez_SampR];
			bezCompF[bez_SampR] = 0.0;
			bezMaxF = 0.0;
		}
		bezCompS[bez_cycle] += sloRez;
		bezCompS[bez_SampL] += (fabs(inputSampleL) * sloRez); //note: SampL is a control voltage
		bezCompS[bez_SampR] += (fabs(inputSampleR) * sloRez); //note: SampR is a control voltage
		if (bezCompS[bez_cycle] > 1.0) {
			bezCompS[bez_cycle] -= 1.0;
			bezCompS[bez_CL] = bezCompS[bez_BL];
			bezCompS[bez_BL] = bezCompS[bez_AL];
			bezCompS[bez_AL] = bezCompS[bez_SampL];
			bezCompS[bez_SampL] = 0.0;
			bezCompS[bez_CR] = bezCompS[bez_BR];
			bezCompS[bez_BR] = bezCompS[bez_AR];
			bezCompS[bez_AR] = bezCompS[bez_SampR];
			bezCompS[bez_SampR] = 0.0;
		}
		double CBFL = (bezCompF[bez_CL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BL]*bezCompF[bez_cycle]);
		double BAFL = (bezCompF[bez_BL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AL]*bezCompF[bez_cycle]);
		double CBAFL = (bezCompF[bez_BL]+(CBFL*(1.0-bezCompF[bez_cycle]))+(BAFL*bezCompF[bez_cycle]))*0.5;
		double CBSL = (bezCompS[bez_CL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BL]*bezCompS[bez_cycle]);
		double BASL = (bezCompS[bez_BL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AL]*bezCompS[bez_cycle]);
		double CBASL = (bezCompS[bez_BL]+(CBSL*(1.0-bezCompS[bez_cycle]))+(BASL*bezCompS[bez_cycle]))*0.5;
		double CBAMax = fmax(CBASL,CBAFL); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		double CBAFade = ((CBASL*-CBAMax)+(CBAFL*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleL *= 1.0-(fmin(((CBASL*(1.0-CBAFade))+(CBAFL*CBAFade))*bezCThresh,1.0));
		
		double CBFR = (bezCompF[bez_CR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BR]*bezCompF[bez_cycle]);
		double BAFR = (bezCompF[bez_BR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AR]*bezCompF[bez_cycle]);
		double CBAFR = (bezCompF[bez_BR]+(CBFR*(1.0-bezCompF[bez_cycle]))+(BAFR*bezCompF[bez_cycle]))*0.5;
		double CBSR = (bezCompS[bez_CR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BR]*bezCompS[bez_cycle]);
		double BASR = (bezCompS[bez_BR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AR]*bezCompS[bez_cycle]);
		double CBASR = (bezCompS[bez_BR]+(CBSR*(1.0-bezCompS[bez_cycle]))+(BASR*bezCompS[bez_cycle]))*0.5;
		CBAMax = fmax(CBASR,CBAFR); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		CBAFade = ((CBASR*-CBAMax)+(CBAFR*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleR *= 1.0-(fmin(((CBASR*(1.0-CBAFade))+(CBAFR*CBAFade))*bezCThresh,1.0));
		//Dynamics2
		
		if (highpassEngage) { //distributed Highpass
			highpass[hilp_temp] = (inputSampleL*highpass[hilp_e0])+highpass[hilp_eL1];
			highpass[hilp_eL1] = (inputSampleL*highpass[hilp_e1])-(highpass[hilp_temp]*highpass[hilp_f1])+highpass[hilp_eL2];
			highpass[hilp_eL2] = (inputSampleL*highpass[hilp_e0])-(highpass[hilp_temp]*highpass[hilp_f2]); inputSampleL = highpass[hilp_temp];
			highpass[hilp_temp] = (inputSampleR*highpass[hilp_e0])+highpass[hilp_eR1];
			highpass[hilp_eR1] = (inputSampleR*highpass[hilp_e1])-(highpass[hilp_temp]*highpass[hilp_f1])+highpass[hilp_eR2];
			highpass[hilp_eR2] = (inputSampleR*highpass[hilp_e0])-(highpass[hilp_temp]*highpass[hilp_f2]); inputSampleR = highpass[hilp_temp];
		} else highpass[hilp_eR1] = highpass[hilp_eR2] = highpass[hilp_eL1] = highpass[hilp_eL2] = 0.0;
		if (lowpassEngage) { //distributed Lowpass
			lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_e0])+lowpass[hilp_eL1];
			lowpass[hilp_eL1] = (inputSampleL*lowpass[hilp_e1])-(lowpass[hilp_temp]*lowpass[hilp_f1])+lowpass[hilp_eL2];
			lowpass[hilp_eL2] = (inputSampleL*lowpass[hilp_e0])-(lowpass[hilp_temp]*lowpass[hilp_f2]); inputSampleL = lowpass[hilp_temp];
			lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_e0])+lowpass[hilp_eR1];
			lowpass[hilp_eR1] = (inputSampleR*lowpass[hilp_e1])-(lowpass[hilp_temp]*lowpass[hilp_f1])+lowpass[hilp_eR2];
			lowpass[hilp_eR2] = (inputSampleR*lowpass[hilp_e0])-(lowpass[hilp_temp]*lowpass[hilp_f2]); inputSampleR = lowpass[hilp_temp];
		} else lowpass[hilp_eR1] = lowpass[hilp_eR2] = lowpass[hilp_eL1] = lowpass[hilp_eL2] = 0.0;		
		//final Highpass/Lowpass continues to address aliasing
		//final stacked biquad section is the softest Q for smoothness
		
		double gainR = (panA*temp)+(panB*(1.0-temp));
		double gainL = 1.57079633-gainR;
		gainR = sin(gainR); gainL = sin(gainL);
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		if (gain > 1.0) gain *= gain;
		if (gain < 1.0) gain = 1.0-pow(1.0-gain,2);
		gain *= 2.0;
		
		inputSampleL = inputSampleL * gainL * gain;
		inputSampleR = inputSampleR * gainR * gain;
		//applies pan section, and smoothed fader gain
		
		double darkSampleL = inputSampleL;
		double darkSampleR = inputSampleR;
		if (avgPos > 31) avgPos = 0;
		if (spacing > 31) {
			avg32L[avgPos] = darkSampleL; avg32R[avgPos] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 32; x++) {darkSampleL += avg32L[x]; darkSampleR += avg32R[x];}
			darkSampleL /= 32.0; darkSampleR /= 32.0;
		} if (spacing > 15) {
			avg16L[avgPos%16] = darkSampleL; avg16R[avgPos%16] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 16; x++) {darkSampleL += avg16L[x]; darkSampleR += avg16R[x];}
			darkSampleL /= 16.0; darkSampleR /= 16.0;
		} if (spacing > 7) {
			avg8L[avgPos%8] = darkSampleL; avg8R[avgPos%8] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 8; x++) {darkSampleL += avg8L[x]; darkSampleR += avg8R[x];}
			darkSampleL /= 8.0; darkSampleR /= 8.0;
		} if (spacing > 3) {
			avg4L[avgPos%4] = darkSampleL; avg4R[avgPos%4] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 4; x++) {darkSampleL += avg4L[x]; darkSampleR += avg4R[x];}
			darkSampleL /= 4.0; darkSampleR /= 4.0;
		} if (spacing > 1) {
			avg2L[avgPos%2] = darkSampleL; avg2R[avgPos%2] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 2; x++) {darkSampleL += avg2L[x]; darkSampleR += avg2R[x];}
			darkSampleL /= 2.0; darkSampleR /= 2.0; 
		} avgPos++;
		lastSlewL += fabs(lastSlewpleL-inputSampleL); lastSlewpleL = inputSampleL;
		double avgSlewL = fmin(lastSlewL,1.0);
		lastSlewL = fmax(lastSlewL*0.78,2.39996322972865332223);
		lastSlewR += fabs(lastSlewpleR-inputSampleR); lastSlewpleR = inputSampleR;
		double avgSlewR = fmin(lastSlewR,1.0);
		lastSlewR = fmax(lastSlewR*0.78,2.39996322972865332223); //look up Golden Angle, it's cool
		inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
		inputSampleR = (inputSampleR*(1.0-avgSlewR)) + (darkSampleR*avgSlewR);
		
		//begin TapeHack section
		inputSampleL = fmax(fmin(inputSampleL,2.305929007734908),-2.305929007734908);
		double addtwo = inputSampleL * inputSampleL;
		double empower = inputSampleL * addtwo; // inputSampleL to the third power
		inputSampleL -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleL += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleL -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleL += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleL -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		inputSampleL *= 0.92;
		//end TapeHack section
		
		//begin TapeHack section
		inputSampleR = fmax(fmin(inputSampleR,2.305929007734908),-2.305929007734908);
		addtwo = inputSampleR * inputSampleR;
		empower = inputSampleR * addtwo; // inputSampleR to the third power
		inputSampleR -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleR += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleR -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleR += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleR -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		inputSampleR *= 0.92;
		//end TapeHack section
		
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
		//direct stereo out
		
		inputL += 1;
		inputR += 1;
		outputL += 1;
		outputR += 1;
	}
	return noErr;
}

