/*
*	File:		Isolator3.cpp
*	
*	Version:	1.0
* 
*	Created:	8/15/24
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
	Isolator3.cpp
	
=============================================================================*/
#include "Isolator3.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Isolator3)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator3::Isolator3
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Isolator3::Isolator3(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator3::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Isolator3::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator3::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Isolator3::GetParameterInfo(AudioUnitScope		inScope,
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
//	Isolator3::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Isolator3::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator3::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Isolator3::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Isolator3::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Isolator3::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Isolator3EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator3::Isolator3Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Isolator3::Isolator3Kernel::Reset()
{
	for (int x = 0; x < biq_total; x++) {
		biquadA[x] = 0.0;
		biquadB[x] = 0.0;
		biquadC[x] = 0.0;
		biquadD[x] = 0.0;
		biquadE[x] = 0.0;
		biquadF[x] = 0.0;
		biquadG[x] = 0.0;
		hiquadA[x] = 0.0;
		hiquadB[x] = 0.0;
		hiquadC[x] = 0.0;
		hiquadD[x] = 0.0;
		hiquadE[x] = 0.0;
		hiquadF[x] = 0.0;
		hiquadG[x] = 0.0;
	}
	
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	for (int x = 0; x < 16; x++) {intermediateL[x] = 0.0;}
	//this is reset: values being initialized only once. Startup values, whatever they are.
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Isolator3::Isolator3Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Isolator3::Isolator3Kernel::Process(	const Float32 	*inSourceP,
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
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;

	double f = GetParameter( kParam_A );
	double q = GetParameter( kParam_B );	
	double isoFreq = ((f*2.0)*(1.0-q))+(f*q);
	double hisoFreq = (((f*2.0)-1.0)*(1.0-q))+(f*q);
	
	if (isoFreq > 1.0) isoFreq = 1.0;
	if (hisoFreq < 0.0) hisoFreq = 0.0;
		
	biquadA[biq_freq] = pow(isoFreq,(2.0*sqrt(overallscale)))*0.4999;
	if (biquadA[biq_freq] < 0.00025) biquadA[biq_freq] = 0.00025;
	biquadG[biq_freq] = biquadF[biq_freq] = biquadE[biq_freq] = biquadD[biq_freq] = biquadC[biq_freq] = biquadB[biq_freq] = biquadA[biq_freq];

	hiquadA[biq_freq] = pow(hisoFreq,(2.0*sqrt(overallscale)))*0.4999;
	if (hiquadA[biq_freq] < 0.00025) hiquadA[biq_freq] = 0.00025;
	hiquadG[biq_freq] = hiquadF[biq_freq] = hiquadE[biq_freq] = hiquadD[biq_freq] = hiquadC[biq_freq] = hiquadB[biq_freq] = hiquadA[biq_freq];
		
	hiquadA[biq_reso] = biquadA[biq_reso] = 4.46570214;
	hiquadB[biq_reso] = biquadB[biq_reso] = 1.51387132;
	hiquadC[biq_reso] = biquadC[biq_reso] = 0.93979296;
	hiquadD[biq_reso] = biquadD[biq_reso] = 0.70710678;
	hiquadE[biq_reso] = biquadE[biq_reso] = 0.59051105;
	hiquadF[biq_reso] = biquadF[biq_reso] = 0.52972649;
	hiquadG[biq_reso] = biquadG[biq_reso] = 0.50316379;
	
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
	
	hiquadA[biq_aA0] = hiquadA[biq_aB0];
	hiquadA[biq_aA1] = hiquadA[biq_aB1];
	hiquadA[biq_aA2] = hiquadA[biq_aB2];
	hiquadA[biq_bA1] = hiquadA[biq_bB1];
	hiquadA[biq_bA2] = hiquadA[biq_bB2];
	
	hiquadB[biq_aA0] = hiquadB[biq_aB0];
	hiquadB[biq_aA1] = hiquadB[biq_aB1];
	hiquadB[biq_aA2] = hiquadB[biq_aB2];
	hiquadB[biq_bA1] = hiquadB[biq_bB1];
	hiquadB[biq_bA2] = hiquadB[biq_bB2];
	
	hiquadC[biq_aA0] = hiquadC[biq_aB0];
	hiquadC[biq_aA1] = hiquadC[biq_aB1];
	hiquadC[biq_aA2] = hiquadC[biq_aB2];
	hiquadC[biq_bA1] = hiquadC[biq_bB1];
	hiquadC[biq_bA2] = hiquadC[biq_bB2];
	
	hiquadD[biq_aA0] = hiquadD[biq_aB0];
	hiquadD[biq_aA1] = hiquadD[biq_aB1];
	hiquadD[biq_aA2] = hiquadD[biq_aB2];
	hiquadD[biq_bA1] = hiquadD[biq_bB1];
	hiquadD[biq_bA2] = hiquadD[biq_bB2];
	
	hiquadE[biq_aA0] = hiquadE[biq_aB0];
	hiquadE[biq_aA1] = hiquadE[biq_aB1];
	hiquadE[biq_aA2] = hiquadE[biq_aB2];
	hiquadE[biq_bA1] = hiquadE[biq_bB1];
	hiquadE[biq_bA2] = hiquadE[biq_bB2];
	
	hiquadF[biq_aA0] = hiquadF[biq_aB0];
	hiquadF[biq_aA1] = hiquadF[biq_aB1];
	hiquadF[biq_aA2] = hiquadF[biq_aB2];
	hiquadF[biq_bA1] = hiquadF[biq_bB1];
	hiquadF[biq_bA2] = hiquadF[biq_bB2];
	
	hiquadG[biq_aA0] = hiquadG[biq_aB0];
	hiquadG[biq_aA1] = hiquadG[biq_aB1];
	hiquadG[biq_aA2] = hiquadG[biq_aB2];
	hiquadG[biq_bA1] = hiquadG[biq_bB1];
	hiquadG[biq_bA2] = hiquadG[biq_bB2];
	
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
	
	K = tan(M_PI * hiquadA[biq_freq]);
	norm = 1.0 / (1.0 + K / hiquadA[biq_reso] + K * K);
	hiquadA[biq_aB0] = K * K * norm;
	hiquadA[biq_aB1] = 2.0 * hiquadA[biq_aB0];
	hiquadA[biq_aB2] = hiquadA[biq_aB0];
	hiquadA[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	hiquadA[biq_bB2] = (1.0 - K / hiquadA[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * hiquadB[biq_freq]);
	norm = 1.0 / (1.0 + K / hiquadB[biq_reso] + K * K);
	hiquadB[biq_aB0] = K * K * norm;
	hiquadB[biq_aB1] = 2.0 * hiquadB[biq_aB0];
	hiquadB[biq_aB2] = hiquadB[biq_aB0];
	hiquadB[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	hiquadB[biq_bB2] = (1.0 - K / hiquadB[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * hiquadC[biq_freq]);
	norm = 1.0 / (1.0 + K / hiquadC[biq_reso] + K * K);
	hiquadC[biq_aB0] = K * K * norm;
	hiquadC[biq_aB1] = 2.0 * hiquadC[biq_aB0];
	hiquadC[biq_aB2] = hiquadC[biq_aB0];
	hiquadC[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	hiquadC[biq_bB2] = (1.0 - K / hiquadC[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * hiquadD[biq_freq]);
	norm = 1.0 / (1.0 + K / hiquadD[biq_reso] + K * K);
	hiquadD[biq_aB0] = K * K * norm;
	hiquadD[biq_aB1] = 2.0 * hiquadD[biq_aB0];
	hiquadD[biq_aB2] = hiquadD[biq_aB0];
	hiquadD[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	hiquadD[biq_bB2] = (1.0 - K / hiquadD[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * hiquadE[biq_freq]);
	norm = 1.0 / (1.0 + K / hiquadE[biq_reso] + K * K);
	hiquadE[biq_aB0] = K * K * norm;
	hiquadE[biq_aB1] = 2.0 * hiquadE[biq_aB0];
	hiquadE[biq_aB2] = hiquadE[biq_aB0];
	hiquadE[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	hiquadE[biq_bB2] = (1.0 - K / hiquadE[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * hiquadF[biq_freq]);
	norm = 1.0 / (1.0 + K / hiquadF[biq_reso] + K * K);
	hiquadF[biq_aB0] = K * K * norm;
	hiquadF[biq_aB1] = 2.0 * hiquadF[biq_aB0];
	hiquadF[biq_aB2] = hiquadF[biq_aB0];
	hiquadF[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	hiquadF[biq_bB2] = (1.0 - K / hiquadF[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * hiquadG[biq_freq]);
	norm = 1.0 / (1.0 + K / hiquadG[biq_reso] + K * K);
	hiquadG[biq_aB0] = K * K * norm;
	hiquadG[biq_aB1] = 2.0 * hiquadG[biq_aB0];
	hiquadG[biq_aB2] = hiquadG[biq_aB0];
	hiquadG[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	hiquadG[biq_bB2] = (1.0 - K / hiquadG[biq_reso] + K * K) * norm;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		double temp = (double)nSampleFrames/inFramesToProcess;
		
		hiquadA[biq_a0] = (hiquadA[biq_aA0]*temp)+(hiquadA[biq_aB0]*(1.0-temp));
		hiquadA[biq_a1] = (hiquadA[biq_aA1]*temp)+(hiquadA[biq_aB1]*(1.0-temp));
		hiquadA[biq_a2] = (hiquadA[biq_aA2]*temp)+(hiquadA[biq_aB2]*(1.0-temp));
		hiquadA[biq_b1] = (hiquadA[biq_bA1]*temp)+(hiquadA[biq_bB1]*(1.0-temp));
		hiquadA[biq_b2] = (hiquadA[biq_bA2]*temp)+(hiquadA[biq_bB2]*(1.0-temp));
		
		hiquadB[biq_a0] = (hiquadB[biq_aA0]*temp)+(hiquadB[biq_aB0]*(1.0-temp));
		hiquadB[biq_a1] = (hiquadB[biq_aA1]*temp)+(hiquadB[biq_aB1]*(1.0-temp));
		hiquadB[biq_a2] = (hiquadB[biq_aA2]*temp)+(hiquadB[biq_aB2]*(1.0-temp));
		hiquadB[biq_b1] = (hiquadB[biq_bA1]*temp)+(hiquadB[biq_bB1]*(1.0-temp));
		hiquadB[biq_b2] = (hiquadB[biq_bA2]*temp)+(hiquadB[biq_bB2]*(1.0-temp));
		
		hiquadC[biq_a0] = (hiquadC[biq_aA0]*temp)+(hiquadC[biq_aB0]*(1.0-temp));
		hiquadC[biq_a1] = (hiquadC[biq_aA1]*temp)+(hiquadC[biq_aB1]*(1.0-temp));
		hiquadC[biq_a2] = (hiquadC[biq_aA2]*temp)+(hiquadC[biq_aB2]*(1.0-temp));
		hiquadC[biq_b1] = (hiquadC[biq_bA1]*temp)+(hiquadC[biq_bB1]*(1.0-temp));
		hiquadC[biq_b2] = (hiquadC[biq_bA2]*temp)+(hiquadC[biq_bB2]*(1.0-temp));
		
		hiquadD[biq_a0] = (hiquadD[biq_aA0]*temp)+(hiquadD[biq_aB0]*(1.0-temp));
		hiquadD[biq_a1] = (hiquadD[biq_aA1]*temp)+(hiquadD[biq_aB1]*(1.0-temp));
		hiquadD[biq_a2] = (hiquadD[biq_aA2]*temp)+(hiquadD[biq_aB2]*(1.0-temp));
		hiquadD[biq_b1] = (hiquadD[biq_bA1]*temp)+(hiquadD[biq_bB1]*(1.0-temp));
		hiquadD[biq_b2] = (hiquadD[biq_bA2]*temp)+(hiquadD[biq_bB2]*(1.0-temp));
		
		hiquadE[biq_a0] = (hiquadE[biq_aA0]*temp)+(hiquadE[biq_aB0]*(1.0-temp));
		hiquadE[biq_a1] = (hiquadE[biq_aA1]*temp)+(hiquadE[biq_aB1]*(1.0-temp));
		hiquadE[biq_a2] = (hiquadE[biq_aA2]*temp)+(hiquadE[biq_aB2]*(1.0-temp));
		hiquadE[biq_b1] = (hiquadE[biq_bA1]*temp)+(hiquadE[biq_bB1]*(1.0-temp));
		hiquadE[biq_b2] = (hiquadE[biq_bA2]*temp)+(hiquadE[biq_bB2]*(1.0-temp));
		
		hiquadF[biq_a0] = (hiquadF[biq_aA0]*temp)+(hiquadF[biq_aB0]*(1.0-temp));
		hiquadF[biq_a1] = (hiquadF[biq_aA1]*temp)+(hiquadF[biq_aB1]*(1.0-temp));
		hiquadF[biq_a2] = (hiquadF[biq_aA2]*temp)+(hiquadF[biq_aB2]*(1.0-temp));
		hiquadF[biq_b1] = (hiquadF[biq_bA1]*temp)+(hiquadF[biq_bB1]*(1.0-temp));
		hiquadF[biq_b2] = (hiquadF[biq_bA2]*temp)+(hiquadF[biq_bB2]*(1.0-temp));
		
		hiquadG[biq_a0] = (hiquadG[biq_aA0]*temp)+(hiquadG[biq_aB0]*(1.0-temp));
		hiquadG[biq_a1] = (hiquadG[biq_aA1]*temp)+(hiquadG[biq_aB1]*(1.0-temp));
		hiquadG[biq_a2] = (hiquadG[biq_aA2]*temp)+(hiquadG[biq_aB2]*(1.0-temp));
		hiquadG[biq_b1] = (hiquadG[biq_bA1]*temp)+(hiquadG[biq_bB1]*(1.0-temp));
		hiquadG[biq_b2] = (hiquadG[biq_bA2]*temp)+(hiquadG[biq_bB2]*(1.0-temp));
		//this is the interpolation code for the hiquad
		
		double outSample = (inputSample * hiquadA[biq_a0]) + hiquadA[biq_sL1];
		hiquadA[biq_sL1] = (inputSample * hiquadA[biq_a1]) - (outSample * hiquadA[biq_b1]) + hiquadA[biq_sL2];
		hiquadA[biq_sL2] = (inputSample * hiquadA[biq_a2]) - (outSample * hiquadA[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * hiquadB[biq_a0]) + hiquadB[biq_sL1];
		hiquadB[biq_sL1] = (inputSample * hiquadB[biq_a1]) - (outSample * hiquadB[biq_b1]) + hiquadB[biq_sL2];
		hiquadB[biq_sL2] = (inputSample * hiquadB[biq_a2]) - (outSample * hiquadB[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * hiquadC[biq_a0]) + hiquadC[biq_sL1];
		hiquadC[biq_sL1] = (inputSample * hiquadC[biq_a1]) - (outSample * hiquadC[biq_b1]) + hiquadC[biq_sL2];
		hiquadC[biq_sL2] = (inputSample * hiquadC[biq_a2]) - (outSample * hiquadC[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * hiquadD[biq_a0]) + hiquadD[biq_sL1];
		hiquadD[biq_sL1] = (inputSample * hiquadD[biq_a1]) - (outSample * hiquadD[biq_b1]) + hiquadD[biq_sL2];
		hiquadD[biq_sL2] = (inputSample * hiquadD[biq_a2]) - (outSample * hiquadD[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * hiquadE[biq_a0]) + hiquadE[biq_sL1];
		hiquadE[biq_sL1] = (inputSample * hiquadE[biq_a1]) - (outSample * hiquadE[biq_b1]) + hiquadE[biq_sL2];
		hiquadE[biq_sL2] = (inputSample * hiquadE[biq_a2]) - (outSample * hiquadE[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * hiquadF[biq_a0]) + hiquadF[biq_sL1];
		hiquadF[biq_sL1] = (inputSample * hiquadF[biq_a1]) - (outSample * hiquadF[biq_b1]) + hiquadF[biq_sL2];
		hiquadF[biq_sL2] = (inputSample * hiquadF[biq_a2]) - (outSample * hiquadF[biq_b2]);
		inputSample = outSample;
		
		outSample = (inputSample * hiquadG[biq_a0]) + hiquadG[biq_sL1];
		hiquadG[biq_sL1] = (inputSample * hiquadG[biq_a1]) - (outSample * hiquadG[biq_b1]) + hiquadG[biq_sL2];
		hiquadG[biq_sL2] = (inputSample * hiquadG[biq_a2]) - (outSample * hiquadG[biq_b2]);
		inputSample = outSample;
		
		inputSample = (drySample - inputSample);
		
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
		
		outSample = (inputSample * biquadA[biq_a0]) + biquadA[biq_sL1];
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
		
		
		//begin ClipOnly2 as a little, compressed chunk that can be dropped into code
		if (inputSample > 4.0) inputSample = 4.0; if (inputSample < -4.0) inputSample = -4.0;
		if (wasPosClipL == true) { //current will be over
			if (inputSample<lastSampleL) lastSampleL=0.7058208+(inputSample*0.2609148);
			else lastSampleL = 0.2491717+(lastSampleL*0.7390851);
		} wasPosClipL = false;
		if (inputSample>0.9549925859) {wasPosClipL=true;inputSample=0.7058208+(lastSampleL*0.2609148);}
		if (wasNegClipL == true) { //current will be -over
			if (inputSample > lastSampleL) lastSampleL=-0.7058208+(inputSample*0.2609148);
			else lastSampleL=-0.2491717+(lastSampleL*0.7390851);
		} wasNegClipL = false;
		if (inputSample<-0.9549925859) {wasNegClipL=true;inputSample=-0.7058208+(lastSampleL*0.2609148);}
		intermediateL[spacing] = inputSample;
        inputSample = lastSampleL; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateL[x-1] = intermediateL[x];
		lastSampleL = intermediateL[0]; //run a little buffer to handle this
		//end ClipOnly2 as a little, compressed chunk that can be dropped into code		
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

