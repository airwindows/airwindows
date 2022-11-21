/*
 *	File:		Isolator2.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	2/21/22
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
 Isolator2.cpp
 
 =============================================================================*/
#include "Isolator2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Isolator2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator2::Isolator2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Isolator2::Isolator2(AudioUnit component)
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
//	Isolator2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Isolator2::GetParameterValueStrings(AudioUnitScope		inScope,
															AudioUnitParameterID	inParameterID,
															CFArrayRef *		outStrings)
{
	
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Isolator2::GetParameterInfo(AudioUnitScope		inScope,
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
//	Isolator2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Isolator2::GetPropertyInfo (AudioUnitPropertyID	inID,
													AudioUnitScope		inScope,
													AudioUnitElement	inElement,
													UInt32 &		outDataSize,
													Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Isolator2::GetProperty(	AudioUnitPropertyID inID,
											   AudioUnitScope 		inScope,
											   AudioUnitElement 	inElement,
											   void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Isolator2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Isolator2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Isolator2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator2::Isolator2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Isolator2::Isolator2Kernel::Reset()
{
	for (int x = 0; x < biq_total; x++) {
		biquadA[x] = 0.0;
		biquadB[x] = 0.0;
		biquadC[x] = 0.0;
		biquadD[x] = 0.0;
		biquadE[x] = 0.0;
		biquadF[x] = 0.0;
		biquadG[x] = 0.0;
	}
	highA = 0.0; highB = 0.0;
	lowA = 1.0; lowB = 1.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator2::Isolator2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Isolator2::Isolator2Kernel::Process(	const Float32 	*inSourceP,
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
	
	biquadA[biq_freq] = pow(GetParameter( kParam_One ),(2.0*sqrt(overallscale)))*0.4999;
	if (biquadA[biq_freq] < 0.0005) biquadA[biq_freq] = 0.0005;
	biquadG[biq_freq] = biquadF[biq_freq] = biquadE[biq_freq] = biquadD[biq_freq] = biquadC[biq_freq] = biquadB[biq_freq] = biquadA[biq_freq];
	
	double reso = pow(GetParameter( kParam_Two ),2);
	double resoBoost = reso+1.0;
	reso = 1.0-reso;
	
	biquadA[biq_reso] = 4.46570214 * resoBoost;
	biquadB[biq_reso] = 1.51387132 * resoBoost;
	biquadC[biq_reso] = 0.93979296 * resoBoost;
	biquadD[biq_reso] = 0.70710678 * resoBoost;
	biquadE[biq_reso] = 0.59051105 * resoBoost;
	biquadF[biq_reso] = 0.52972649 * resoBoost;
	biquadG[biq_reso] = 0.50316379 * resoBoost;
	
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
	
	biquadE[biq_aA0] = biquadE[biq_aB0];
	biquadE[biq_aA1] = biquadE[biq_aB1];
	biquadE[biq_aA2] = biquadE[biq_aB2];
	biquadE[biq_bA1] = biquadE[biq_bB1];
	biquadE[biq_bA2] = biquadE[biq_bB2];
	
	biquadF[biq_aA0] = biquadF[biq_aB0];
	biquadF[biq_aA1] = biquadF[biq_aB1];
	biquadF[biq_aA2] = biquadF[biq_aB2];
	biquadF[biq_bA1] = biquadF[biq_bB1];
	biquadF[biq_bA2] = biquadF[biq_bB2];
	
	biquadG[biq_aA0] = biquadG[biq_aB0];
	biquadG[biq_aA1] = biquadG[biq_aB1];
	biquadG[biq_aA2] = biquadG[biq_aB2];
	biquadG[biq_bA1] = biquadG[biq_bB1];
	biquadG[biq_bA2] = biquadG[biq_bB2];
	
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.	
	double K = tan(M_PI * biquadA[biq_freq]);
	double norm = 1.0 / (1.0 + K / biquadA[biq_reso] + K * K);
	biquadA[biq_aB0] = K * K * norm;
	biquadA[biq_aB1] = 2.0 * biquadA[biq_aB0];
	biquadA[biq_aB2] = biquadA[biq_aB0];
	biquadA[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadA[biq_bB2] = (1.0 - K / biquadA[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadB[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadB[biq_reso] + K * K);
	biquadB[biq_aB0] = K * K * norm;
	biquadB[biq_aB1] = 2.0 * biquadB[biq_aB0];
	biquadB[biq_aB2] = biquadB[biq_aB0];
	biquadB[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadB[biq_bB2] = (1.0 - K / biquadB[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadC[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadC[biq_reso] + K * K);
	biquadC[biq_aB0] = K * K * norm;
	biquadC[biq_aB1] = 2.0 * biquadC[biq_aB0];
	biquadC[biq_aB2] = biquadC[biq_aB0];
	biquadC[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadC[biq_bB2] = (1.0 - K / biquadC[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadD[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadD[biq_reso] + K * K);
	biquadD[biq_aB0] = K * K * norm;
	biquadD[biq_aB1] = 2.0 * biquadD[biq_aB0];
	biquadD[biq_aB2] = biquadD[biq_aB0];
	biquadD[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadD[biq_bB2] = (1.0 - K / biquadD[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadE[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadE[biq_reso] + K * K);
	biquadE[biq_aB0] = K * K * norm;
	biquadE[biq_aB1] = 2.0 * biquadE[biq_aB0];
	biquadE[biq_aB2] = biquadE[biq_aB0];
	biquadE[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadE[biq_bB2] = (1.0 - K / biquadE[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadF[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadF[biq_reso] + K * K);
	biquadF[biq_aB0] = K * K * norm;
	biquadF[biq_aB1] = 2.0 * biquadF[biq_aB0];
	biquadF[biq_aB2] = biquadF[biq_aB0];
	biquadF[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadF[biq_bB2] = (1.0 - K / biquadF[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadG[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadG[biq_reso] + K * K);
	biquadG[biq_aB0] = K * K * norm;
	biquadG[biq_aB1] = 2.0 * biquadG[biq_aB0];
	biquadG[biq_aB2] = biquadG[biq_aB0];
	biquadG[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadG[biq_bB2] = (1.0 - K / biquadG[biq_reso] + K * K) * norm;
	
	bool bypass = (GetParameter( kParam_One ) == 1.0);
	highA = highB;
	highB = GetParameter( kParam_Three )*reso;
	if (highB > 0.0) bypass = false;
	lowA = lowB;
	lowB = GetParameter( kParam_Four )*reso;
	if (lowB < 1.0) bypass = false;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		double temp = (double)nSampleFrames/inFramesToProcess;
		
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
		
		biquadE[biq_a0] = (biquadE[biq_aA0]*temp)+(biquadE[biq_aB0]*(1.0-temp));
		biquadE[biq_a1] = (biquadE[biq_aA1]*temp)+(biquadE[biq_aB1]*(1.0-temp));
		biquadE[biq_a2] = (biquadE[biq_aA2]*temp)+(biquadE[biq_aB2]*(1.0-temp));
		biquadE[biq_b1] = (biquadE[biq_bA1]*temp)+(biquadE[biq_bB1]*(1.0-temp));
		biquadE[biq_b2] = (biquadE[biq_bA2]*temp)+(biquadE[biq_bB2]*(1.0-temp));
		
		biquadF[biq_a0] = (biquadF[biq_aA0]*temp)+(biquadF[biq_aB0]*(1.0-temp));
		biquadF[biq_a1] = (biquadF[biq_aA1]*temp)+(biquadF[biq_aB1]*(1.0-temp));
		biquadF[biq_a2] = (biquadF[biq_aA2]*temp)+(biquadF[biq_aB2]*(1.0-temp));
		biquadF[biq_b1] = (biquadF[biq_bA1]*temp)+(biquadF[biq_bB1]*(1.0-temp));
		biquadF[biq_b2] = (biquadF[biq_bA2]*temp)+(biquadF[biq_bB2]*(1.0-temp));
		
		biquadG[biq_a0] = (biquadG[biq_aA0]*temp)+(biquadG[biq_aB0]*(1.0-temp));
		biquadG[biq_a1] = (biquadG[biq_aA1]*temp)+(biquadG[biq_aB1]*(1.0-temp));
		biquadG[biq_a2] = (biquadG[biq_aA2]*temp)+(biquadG[biq_aB2]*(1.0-temp));
		biquadG[biq_b1] = (biquadG[biq_bA1]*temp)+(biquadG[biq_bB1]*(1.0-temp));
		biquadG[biq_b2] = (biquadG[biq_bA2]*temp)+(biquadG[biq_bB2]*(1.0-temp));
		
		//this is the interpolation code for the biquad
		double high = (highA*temp)+(highB*(1.0-temp));
		double low = (lowA*temp)+(lowB*(1.0-temp));
		
		double outSample = (inputSample * biquadA[biq_a0]) + biquadA[biq_sL1];
		biquadA[biq_sL1] = (inputSample * biquadA[biq_a1]) - (outSample * biquadA[biq_b1]) + biquadA[biq_sL2];
		biquadA[biq_sL2] = (inputSample * biquadA[biq_a2]) - (outSample * biquadA[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * biquadB[biq_a0]) + biquadB[biq_sL1];
		biquadB[biq_sL1] = (inputSample * biquadB[biq_a1]) - (outSample * biquadB[biq_b1]) + biquadB[biq_sL2];
		biquadB[biq_sL2] = (inputSample * biquadB[biq_a2]) - (outSample * biquadB[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * biquadC[biq_a0]) + biquadC[biq_sL1];
		biquadC[biq_sL1] = (inputSample * biquadC[biq_a1]) - (outSample * biquadC[biq_b1]) + biquadC[biq_sL2];
		biquadC[biq_sL2] = (inputSample * biquadC[biq_a2]) - (outSample * biquadC[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * biquadD[biq_a0]) + biquadD[biq_sL1];
		biquadD[biq_sL1] = (inputSample * biquadD[biq_a1]) - (outSample * biquadD[biq_b1]) + biquadD[biq_sL2];
		biquadD[biq_sL2] = (inputSample * biquadD[biq_a2]) - (outSample * biquadD[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * biquadE[biq_a0]) + biquadE[biq_sL1];
		biquadE[biq_sL1] = (inputSample * biquadE[biq_a1]) - (outSample * biquadE[biq_b1]) + biquadE[biq_sL2];
		biquadE[biq_sL2] = (inputSample * biquadE[biq_a2]) - (outSample * biquadE[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * biquadF[biq_a0]) + biquadF[biq_sL1];
		biquadF[biq_sL1] = (inputSample * biquadF[biq_a1]) - (outSample * biquadF[biq_b1]) + biquadF[biq_sL2];
		biquadF[biq_sL2] = (inputSample * biquadF[biq_a2]) - (outSample * biquadF[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * biquadG[biq_a0]) + biquadG[biq_sL1];
		biquadG[biq_sL1] = (inputSample * biquadG[biq_a1]) - (outSample * biquadG[biq_b1]) + biquadG[biq_sL2];
		biquadG[biq_sL2] = (inputSample * biquadG[biq_a2]) - (outSample * biquadG[biq_b2]);
		inputSample = outSample;
		
		if (bypass) inputSample = drySample;
		else inputSample = (inputSample * low) + ((drySample - inputSample)*high);
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

