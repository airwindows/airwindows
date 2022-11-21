/*
*	File:		ZRegion2.cpp
*	
*	Version:	1.0
* 
*	Created:	9/5/22
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
	ZRegion2.cpp
	
=============================================================================*/
#include "ZRegion2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(ZRegion2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZRegion2::ZRegion2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ZRegion2::ZRegion2(AudioUnit component)
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
//	ZRegion2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZRegion2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZRegion2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZRegion2::GetParameterInfo(AudioUnitScope		inScope,
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
//	ZRegion2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZRegion2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZRegion2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZRegion2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ZRegion2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ZRegion2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ZRegion2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZRegion2::ZRegion2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ZRegion2::ZRegion2Kernel::Reset()
{
	iirSampleA = 0.0;
	for (int x = 0; x < biq_total; x++) {biquad[x] = 0.0; biquadA[x] = 0.0; biquadB[x] = 0.0; biquadC[x] = 0.0; biquadD[x] = 0.0;}
	inTrimA = 0.1; inTrimB = 0.1;
	//outTrimA = 1.0; outTrimB = 1.0;
	wetA = 0.5; wetB = 0.5;
	overallWetA = 1.0; overallWetB = 1.0;
	for (int x = 0; x < fix_total; x++) {fixA[x] = 0.0; fixB[x] = 0.0;}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZRegion2::ZRegion2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ZRegion2::ZRegion2Kernel::Process(	const Float32 	*inSourceP,
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
	
	//begin from XRegion
	Float64 high = GetParameter( kParam_Two );
	Float64 low = GetParameter( kParam_Three );
	Float64 mid = (high+low)*0.5;
	Float64 spread = 1.001-fabs(high-low);
	
	biquad[biq_freq] = (pow(high,3)*20000.0)/GetSampleRate();
	if (biquad[biq_freq] < 0.00009) biquad[biq_freq] = 0.00009;
	Float64 compensation = sqrt(biquad[biq_freq])*6.4*spread;
	Float64 clipFactor = 0.75+(biquad[biq_freq]*GetParameter( kParam_Four )*37.0);
	
	biquadA[biq_freq] = (pow((high+mid)*0.5,3)*20000.0)/GetSampleRate();
	if (biquadA[biq_freq] < 0.00009) biquadA[biq_freq] = 0.00009;
	Float64 compensationA = sqrt(biquadA[biq_freq])*6.4*spread;
	Float64 clipFactorA = 0.75+(biquadA[biq_freq]*GetParameter( kParam_Four )*37.0);
	
	biquadB[biq_freq] = (pow(mid,3)*20000.0)/GetSampleRate();
	if (biquadB[biq_freq] < 0.00009) biquadB[biq_freq] = 0.00009;
	Float64 compensationB = sqrt(biquadB[biq_freq])*6.4*spread;
	Float64 clipFactorB = 0.75+(biquadB[biq_freq]*GetParameter( kParam_Four )*37.0);
	
	biquadC[biq_freq] = (pow((mid+low)*0.5,3)*20000.0)/GetSampleRate();
	if (biquadC[biq_freq] < 0.00009) biquadC[biq_freq] = 0.00009;
	Float64 compensationC = sqrt(biquadC[biq_freq])*6.4*spread;
	Float64 clipFactorC = 0.75+(biquadC[biq_freq]*GetParameter( kParam_Four )*37.0);
	
	biquadD[biq_freq] = (pow(low,3)*20000.0)/GetSampleRate();
	if (biquadD[biq_freq] < 0.00009) biquadD[biq_freq] = 0.00009;
	Float64 compensationD = sqrt(biquadD[biq_freq])*6.4*spread;
	Float64 clipFactorD = 0.75+(biquadD[biq_freq]*GetParameter( kParam_Four )*37.0);
	
	//set up all the interpolations
	biquad[biq_aA0] = biquad[biq_aB0];
	biquad[biq_aA1] = biquad[biq_aB1];
	biquad[biq_aA2] = biquad[biq_aB2];
	biquad[biq_bA1] = biquad[biq_bB1];
	biquad[biq_bA2] = biquad[biq_bB2];

	biquadA[biq_aA0] = biquadA[biq_aB0];
	biquadA[biq_aA1] = biquadA[biq_aB1];
	biquadA[biq_aA2] = biquadA[biq_aB2];
	biquadA[biq_bA1] = biquadA[biq_bB1];
	biquadA[biq_bA2] = biquadA[biq_bB2];

	biquadB[biq_aA0] = biquadB[biq_aB0];
	biquadB[biq_aA1] = biquadB[biq_aB1];
	biquadB[biq_aA2] = biquadB[biq_aB2];
	biquadB[biq_bA1] = biquadB[biq_bB1];
	biquadB[biq_bA2] = biquadB[biq_bB2];

	biquadC[biq_aA0] = biquadC[biq_aB0];
	biquadC[biq_aA1] = biquadC[biq_aB1];
	biquadC[biq_aA2] = biquadC[biq_aB2];
	biquadC[biq_bA1] = biquadC[biq_bB1];
	biquadC[biq_bA2] = biquadC[biq_bB2];

	biquadD[biq_aA0] = biquadD[biq_aB0];
	biquadD[biq_aA1] = biquadD[biq_aB1];
	biquadD[biq_aA2] = biquadD[biq_aB2];
	biquadD[biq_bA1] = biquadD[biq_bB1];
	biquadD[biq_bA2] = biquadD[biq_bB2];
	//since this is Region, they are all different
	
	double K = tan(M_PI * biquad[biq_freq]);
	double norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquad[biq_aB0] = K / 0.7071 * norm;
	biquad[biq_aB2] = -biquad[biq_aB0];
	biquad[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquad[biq_bB2] = (1.0 - K / 0.7071 + K * K) * norm;
	
	K = tan(M_PI * biquadA[biq_freq]);
	norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquadA[biq_aB0] = K / 0.7071 * norm;
	biquadA[biq_aB2] = -biquadA[biq_aB0];
	biquadA[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadA[biq_bB2] = (1.0 - K / 0.7071 + K * K) * norm;
	
	K = tan(M_PI * biquadB[biq_freq]);
	norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquadB[biq_aB0] = K / 0.7071 * norm;
	biquadB[biq_aB2] = -biquadB[biq_aB0];
	biquadB[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadB[biq_bB2] = (1.0 - K / 0.7071 + K * K) * norm;
	
	K = tan(M_PI * biquadC[biq_freq]);
	norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquadC[biq_aB0] = K / 0.7071 * norm;
	biquadC[biq_aB2] = -biquadC[biq_aB0];
	biquadC[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadC[biq_bB2] = (1.0 - K / 0.7071 + K * K) * norm;
	
	K = tan(M_PI * biquadD[biq_freq]);
	norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquadD[biq_aB0] = K / 0.7071 * norm;
	biquadD[biq_aB2] = -biquadD[biq_aB0];
	biquadD[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadD[biq_bB2] = (1.0 - K / 0.7071 + K * K) * norm;	
	//end from XRegion	
		
	//opamp stuff
	inTrimA = inTrimB;
	inTrimB = GetParameter( kParam_One )*10.0;
	inTrimB *= inTrimB; inTrimB *= inTrimB;
	wetA = wetB;
	wetB = GetParameter( kParam_Four );
	overallWetA = overallWetB;
	overallWetB = GetParameter( kParam_Five );

	double iirAmountA = 0.00069/overallscale;
	fixA[fix_freq] = fixB[fix_freq] = 15500.0 / GetSampleRate();
    fixA[fix_reso] = fixB[fix_reso] = 0.935;
	K = tan(M_PI * fixB[fix_freq]); //lowpass
	norm = 1.0 / (1.0 + K / fixB[fix_reso] + K * K);
	fixA[fix_a0] = fixB[fix_a0] = K * K * norm;
	fixA[fix_a1] = fixB[fix_a1] = 2.0 * fixB[fix_a0];
	fixA[fix_a2] = fixB[fix_a2] = fixB[fix_a0];
	fixA[fix_b1] = fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixA[fix_b2] = fixB[fix_b2] = (1.0 - K / fixB[fix_reso] + K * K) * norm;
	//end opamp stuff	
	
	double outSample = 0.0;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double overallDrySample = *sourceP;
		double nukeLevel = inputSample;

		double temp = (double)nSampleFrames/inFramesToProcess;
		biquad[biq_a0] = (biquad[biq_aA0]*temp)+(biquad[biq_aB0]*(1.0-temp));
		biquad[biq_a1] = (biquad[biq_aA1]*temp)+(biquad[biq_aB1]*(1.0-temp));
		biquad[biq_a2] = (biquad[biq_aA2]*temp)+(biquad[biq_aB2]*(1.0-temp));
		biquad[biq_b1] = (biquad[biq_bA1]*temp)+(biquad[biq_bB1]*(1.0-temp));
		biquad[biq_b2] = (biquad[biq_bA2]*temp)+(biquad[biq_bB2]*(1.0-temp));
		
		biquadA[biq_a0] = (biquadA[biq_aA0]*temp)+(biquadA[biq_aB0]*(1.0-temp));
		biquadA[biq_a1] = (biquadA[biq_aA1]*temp)+(biquadA[biq_aB1]*(1.0-temp));
		biquadA[biq_a2] = (biquadA[biq_aA2]*temp)+(biquadA[biq_aB2]*(1.0-temp));
		biquadA[biq_b1] = (biquadA[biq_bA1]*temp)+(biquadA[biq_bB1]*(1.0-temp));
		biquadA[biq_b2] = (biquadA[biq_bA2]*temp)+(biquadA[biq_bB2]*(1.0-temp));
		
		biquadB[biq_a0] = (biquadB[biq_aA0]*temp)+(biquadB[biq_aB0]*(1.0-temp));
		biquadB[biq_a1] = (biquadB[biq_aA1]*temp)+(biquadB[biq_aB1]*(1.0-temp));
		biquadB[biq_a2] = (biquadB[biq_aA2]*temp)+(biquadB[biq_aB2]*(1.0-temp));
		biquadB[biq_b1] = (biquadB[biq_bA1]*temp)+(biquadB[biq_bB1]*(1.0-temp));
		biquadB[biq_b2] = (biquadB[biq_bA2]*temp)+(biquadB[biq_bB2]*(1.0-temp));
		
		biquadC[biq_a0] = (biquadC[biq_aA0]*temp)+(biquadC[biq_aB0]*(1.0-temp));
		biquadC[biq_a1] = (biquadC[biq_aA1]*temp)+(biquadC[biq_aB1]*(1.0-temp));
		biquadC[biq_a2] = (biquadC[biq_aA2]*temp)+(biquadC[biq_aB2]*(1.0-temp));
		biquadC[biq_b1] = (biquadC[biq_bA1]*temp)+(biquadC[biq_bB1]*(1.0-temp));
		biquadC[biq_b2] = (biquadC[biq_bA2]*temp)+(biquadC[biq_bB2]*(1.0-temp));
		
		biquadD[biq_a0] = (biquadD[biq_aA0]*temp)+(biquadD[biq_aB0]*(1.0-temp));
		biquadD[biq_a1] = (biquadD[biq_aA1]*temp)+(biquadD[biq_aB1]*(1.0-temp));
		biquadD[biq_a2] = (biquadD[biq_aA2]*temp)+(biquadD[biq_aB2]*(1.0-temp));
		biquadD[biq_b1] = (biquadD[biq_bA1]*temp)+(biquadD[biq_bB1]*(1.0-temp));
		biquadD[biq_b2] = (biquadD[biq_bA2]*temp)+(biquadD[biq_bB2]*(1.0-temp));
		//this is the interpolation code for all the biquads
		
		double inTrim = (inTrimA*temp)+(inTrimB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
		double aWet = 1.0;
		double bWet = 1.0;
		double cWet = 1.0;
		double dWet = wet*4.0;
		//four-stage wet/dry control using progressive stages that bypass when not engaged
		if (dWet < 1.0) {aWet = dWet; bWet = 0.0; cWet = 0.0; dWet = 0.0;}
		else if (dWet < 2.0) {bWet = dWet - 1.0; cWet = 0.0; dWet = 0.0;}
		else if (dWet < 3.0) {cWet = dWet - 2.0; dWet = 0.0;}
		else {dWet -= 3.0;}
		//this is one way to make a little set of dry/wet stages that are successively added to the
		//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
		//beyond that point: this is a way to progressively add a 'black box' sound processing
		//which lets you fall through to simpler processing at lower settings.
		double overallWet = (overallWetA*temp)+(overallWetB*(1.0-temp));
		
		if (inTrim != 1.0) inputSample *= inTrim;
		
		inputSample *= clipFactor;
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);		
		outSample = (inputSample * biquad[biq_a0]) + biquad[biq_sL1];
		biquad[biq_sL1] = (inputSample * biquad[biq_a1]) - (outSample * biquad[biq_b1]) + biquad[biq_sL2];
		biquad[biq_sL2] = (inputSample * biquad[biq_a2]) - (outSample * biquad[biq_b2]);
		inputSample = outSample / compensation; nukeLevel = inputSample;
		
		if (aWet > 0.0) {
			inputSample *= clipFactorA;
			if (inputSample > 1.57079633) inputSample = 1.57079633;
			if (inputSample < -1.57079633) inputSample = -1.57079633;
			inputSample = sin(inputSample);
			outSample = (inputSample * biquadA[biq_a0]) + biquadA[biq_sL1];
			biquadA[biq_sL1] = (inputSample * biquadA[biq_a1]) - (outSample * biquadA[biq_b1]) + biquadA[biq_sL2];
			biquadA[biq_sL2] = (inputSample * biquadA[biq_a2]) - (outSample * biquadA[biq_b2]);
			inputSample = outSample / compensationA; inputSample = (inputSample * aWet) + (nukeLevel * (1.0-aWet));
			nukeLevel = inputSample;
		}
		if (bWet > 0.0) {
			inputSample *= clipFactorB;
			if (inputSample > 1.57079633) inputSample = 1.57079633;
			if (inputSample < -1.57079633) inputSample = -1.57079633;
			inputSample = sin(inputSample);
			outSample = (inputSample * biquadB[biq_a0]) + biquadB[biq_sL1];
			biquadB[biq_sL1] = (inputSample * biquadB[biq_a1]) - (outSample * biquadB[biq_b1]) + biquadB[biq_sL2];
			biquadB[biq_sL2] = (inputSample * biquadB[biq_a2]) - (outSample * biquadB[biq_b2]);			
			inputSample = outSample / compensationB; inputSample = (inputSample * bWet) + (nukeLevel * (1.0-bWet));
			nukeLevel = inputSample;
		}
		if (cWet > 0.0) {
			inputSample *= clipFactorC;
			if (inputSample > 1.57079633) inputSample = 1.57079633;
			if (inputSample < -1.57079633) inputSample = -1.57079633;
			inputSample = sin(inputSample);
			outSample = (inputSample * biquadC[biq_a0]) + biquadC[biq_sL1];
			biquadC[biq_sL1] = (inputSample * biquadC[biq_a1]) - (outSample * biquadC[biq_b1]) + biquadC[biq_sL2];
			biquadC[biq_sL2] = (inputSample * biquadC[biq_a2]) - (outSample * biquadC[biq_b2]);
			inputSample = outSample / compensationC; inputSample = (inputSample * cWet) + (nukeLevel * (1.0-cWet));
			nukeLevel = inputSample;
		}
		if (dWet > 0.0) {
			inputSample *= clipFactorD;
			if (inputSample > 1.57079633) inputSample = 1.57079633;
			if (inputSample < -1.57079633) inputSample = -1.57079633;
			inputSample = sin(inputSample);
			outSample = (inputSample * biquadD[biq_a0]) + biquadD[biq_sL1];
			biquadD[biq_sL1] = (inputSample * biquadD[biq_a1]) - (outSample * biquadD[biq_b1]) + biquadD[biq_sL2];
			biquadD[biq_sL2] = (inputSample * biquadD[biq_a2]) - (outSample * biquadD[biq_b2]);
			inputSample = outSample / compensationD; inputSample = (inputSample * dWet) + (nukeLevel * (1.0-dWet));
		}
				
		//opamp stage
		if (fabs(iirSampleA)<1.18e-37) iirSampleA = 0.0;
		iirSampleA = (iirSampleA * (1.0 - iirAmountA)) + (inputSample * iirAmountA);
		inputSample -= iirSampleA;
		
		outSample = (inputSample * fixA[fix_a0]) + fixA[fix_sL1];
		fixA[fix_sL1] = (inputSample * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sL2];
		fixA[fix_sL2] = (inputSample * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		inputSample -= (inputSample*inputSample*inputSample*inputSample*inputSample*0.1768);
		
		outSample = (inputSample * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSample * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSample * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		//end opamp stage
		
		if (overallWet !=1.0) {
			inputSample = (inputSample * overallWet) + (overallDrySample * (1.0-overallWet));
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

