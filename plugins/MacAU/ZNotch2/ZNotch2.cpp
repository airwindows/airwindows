/*
*	File:		ZNotch2.cpp
*	
*	Version:	1.0
* 
*	Created:	12/20/21
*	
*	Copyright:  Copyright © 2021 Airwindows, Airwindows uses the MIT license
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
	ZNotch2.cpp
	
=============================================================================*/
#include "ZNotch2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(ZNotch2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZNotch2::ZNotch2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ZNotch2::ZNotch2(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZNotch2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZNotch2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZNotch2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZNotch2::GetParameterInfo(AudioUnitScope		inScope,
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
//	ZNotch2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZNotch2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZNotch2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZNotch2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ZNotch2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ZNotch2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ZNotch2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZNotch2::ZNotch2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ZNotch2::ZNotch2Kernel::Reset()
{
	iirSampleA = 0.0;
	for (int x = 0; x < biq_total; x++) {biquadA[x] = 0.0; biquadB[x] = 0.0; biquadC[x] = 0.0; biquadD[x] = 0.0;}
	inTrimA = 0.1; inTrimB = 0.1;
	outTrimA = 1.0; outTrimB = 1.0;
	wetA = 0.5; wetB = 0.5;
	for (int x = 0; x < fix_total; x++) {fixA[x] = 0.0; fixB[x] = 0.0;}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZNotch2::ZNotch2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ZNotch2::ZNotch2Kernel::Process(	const Float32 	*inSourceP,
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
	
	biquadA[biq_freq] = ((pow(GetParameter( kParam_Two ),2)*4700.0)/GetSampleRate())+0.0009963;
	double clipFactor = 0.91-((1.0-GetParameter( kParam_Two ))*0.15);
	biquadA[biq_reso] = 0.618033988749894848204586;
	biquadA[biq_aA0] = biquadA[biq_aB0];
	biquadA[biq_aA1] = biquadA[biq_aB1];
	biquadA[biq_aA2] = biquadA[biq_aB2];
	biquadA[biq_bA1] = biquadA[biq_bB1];
	biquadA[biq_bA2] = biquadA[biq_bB2];
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.	
	double K = tan(M_PI * biquadA[biq_freq]);
	double norm = 1.0 / (1.0 + K / biquadA[biq_reso] + K * K);
	biquadA[biq_aB0] = (1.0 + K * K) * norm;
	biquadA[biq_aB1] = 2.0 * (K * K - 1.0) * norm;
	biquadA[biq_aB2] = biquadA[biq_aB0];
	biquadA[biq_bB1] = biquadA[biq_aB1];
	biquadA[biq_bB2] = (1.0 - K / biquadA[biq_reso] + K * K) * norm;
	
	//opamp stuff
	inTrimA = inTrimB;
	inTrimB = GetParameter( kParam_One )*10.0;
	inTrimB *= inTrimB; inTrimB *= inTrimB;
	outTrimA = outTrimB;
	outTrimB = GetParameter( kParam_Three )*10.0;
	wetA = wetB;
	wetB = pow(GetParameter( kParam_Four ),2);
	
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
	
 	double trim = 0.1+(3.712*biquadA[biq_freq]);
	double outSample = 0.0;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = *sourceP;
		double overallDrySample = *sourceP;
		
		double temp = (double)nSampleFrames/inFramesToProcess;
		biquadA[biq_a0] = (biquadA[biq_aA0]*temp)+(biquadA[biq_aB0]*(1.0-temp));
		biquadA[biq_a1] = (biquadA[biq_aA1]*temp)+(biquadA[biq_aB1]*(1.0-temp));
		biquadA[biq_a2] = (biquadA[biq_aA2]*temp)+(biquadA[biq_aB2]*(1.0-temp));
		biquadA[biq_b1] = (biquadA[biq_bA1]*temp)+(biquadA[biq_bB1]*(1.0-temp));
		biquadA[biq_b2] = (biquadA[biq_bA2]*temp)+(biquadA[biq_bB2]*(1.0-temp));
		for (int x = 0; x < 7; x++) {biquadD[x] = biquadC[x] = biquadB[x] = biquadA[x];}
		//this is the interpolation code for the biquad
		double inTrim = (inTrimA*temp)+(inTrimB*(1.0-temp));
		double outTrim = (outTrimA*temp)+(outTrimB*(1.0-temp));
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
		
		if (inTrim != 1.0) inputSample *= inTrim;
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		inputSample *= trim;
		inputSample /= clipFactor;
		outSample = (inputSample * biquadA[biq_a0]) + biquadA[biq_sL1];
		if (outSample > 1.0) outSample = 1.0; if (outSample < -1.0) outSample = -1.0;
		biquadA[biq_sL1] = (inputSample * biquadA[biq_a1]) - (outSample * biquadA[biq_b1]) + biquadA[biq_sL2];
		biquadA[biq_sL2] = (inputSample * biquadA[biq_a2]) - (outSample * biquadA[biq_b2]);
		drySample = inputSample = outSample;
		
		if (bWet > 0.0) {
			inputSample /= clipFactor;
			outSample = (inputSample * biquadB[biq_a0]) + biquadB[biq_sL1];
			if (outSample > 1.0) outSample = 1.0; if (outSample < -1.0) outSample = -1.0;
			biquadB[biq_sL1] = (inputSample * biquadB[biq_a1]) - (outSample * biquadB[biq_b1]) + biquadB[biq_sL2];
			biquadB[biq_sL2] = (inputSample * biquadB[biq_a2]) - (outSample * biquadB[biq_b2]);
			drySample = inputSample = (outSample * bWet) + (drySample * (1.0-bWet));
		}
		if (cWet > 0.0) {
			inputSample /= clipFactor;
			outSample = (inputSample * biquadC[biq_a0]) + biquadC[biq_sL1];
			if (outSample > 1.0) outSample = 1.0; if (outSample < -1.0) outSample = -1.0;
			biquadC[biq_sL1] = (inputSample * biquadC[biq_a1]) - (outSample * biquadC[biq_b1]) + biquadC[biq_sL2];
			biquadC[biq_sL2] = (inputSample * biquadC[biq_a2]) - (outSample * biquadC[biq_b2]);
			drySample = inputSample = (outSample * cWet) + (drySample * (1.0-cWet));
		}
		if (dWet > 0.0) {
			inputSample /= clipFactor;
			outSample = (inputSample * biquadD[biq_a0]) + biquadD[biq_sL1];
			if (outSample > 1.0) outSample = 1.0; if (outSample < -1.0) outSample = -1.0;
			biquadD[biq_sL1] = (inputSample * biquadD[biq_a1]) - (outSample * biquadD[biq_b1]) + biquadD[biq_sL2];
			biquadD[biq_sL2] = (inputSample * biquadD[biq_a2]) - (outSample * biquadD[biq_b2]);
			drySample = inputSample = (outSample * dWet) + (drySample * (1.0-dWet));
		}
		
		inputSample /= clipFactor;
		
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
		
		if (outTrim != 1.0) inputSample *= outTrim;		
		//end opamp stage
		
		if (aWet !=1.0) {
			inputSample = (inputSample * aWet) + (overallDrySample * (1.0-aWet));
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

