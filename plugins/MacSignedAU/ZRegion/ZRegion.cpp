/*
*	File:		ZRegion.cpp
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
	ZRegion.cpp
	
=============================================================================*/
#include "ZRegion.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, ZRegion)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZRegion::ZRegion
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ZRegion::ZRegion(AudioUnit component)
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
//	ZRegion::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZRegion::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZRegion::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZRegion::GetParameterInfo(AudioUnitScope		inScope,
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
//	ZRegion::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZRegion::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZRegion::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ZRegion::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ZRegion::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ZRegion::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ZRegionEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZRegion::ZRegionKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ZRegion::ZRegionKernel::Reset()
{
	for (int x = 0; x < 11; x++) {biquad[x] = 0.0; biquadA[x] = 0.0; biquadB[x] = 0.0; biquadC[x] = 0.0; biquadD[x] = 0.0; biquadE[x] = 0.0; biquadF[x] = 0.0;}
	iirSampleA = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ZRegion::ZRegionKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ZRegion::ZRegionKernel::Process(	const Float32 	*inSourceP,
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
	
	biquad[0] = (pow(high,3)*20000.0)/GetSampleRate();
	if (biquad[0] < 0.00009) biquad[0] = 0.00009;
	Float64 compensation = sqrt(biquad[0])*6.4*spread;
	Float64 clipFactor = 0.75+(biquad[0]*GetParameter( kParam_Four )*37.0);
	
	biquadA[0] = (pow((high+mid)*0.5,3)*20000.0)/GetSampleRate();
	if (biquadA[0] < 0.00009) biquadA[0] = 0.00009;
	Float64 compensationA = sqrt(biquadA[0])*6.4*spread;
	Float64 clipFactorA = 0.75+(biquadA[0]*GetParameter( kParam_Four )*37.0);
	
	biquadB[0] = (pow(mid,3)*20000.0)/GetSampleRate();
	if (biquadB[0] < 0.00009) biquadB[0] = 0.00009;
	Float64 compensationB = sqrt(biquadB[0])*6.4*spread;
	Float64 clipFactorB = 0.75+(biquadB[0]*GetParameter( kParam_Four )*37.0);
	
	biquadC[0] = (pow((mid+low)*0.5,3)*20000.0)/GetSampleRate();
	if (biquadC[0] < 0.00009) biquadC[0] = 0.00009;
	Float64 compensationC = sqrt(biquadC[0])*6.4*spread;
	Float64 clipFactorC = 0.75+(biquadC[0]*GetParameter( kParam_Four )*37.0);
	
	biquadD[0] = (pow(low,3)*20000.0)/GetSampleRate();
	if (biquadD[0] < 0.00009) biquadD[0] = 0.00009;
	Float64 compensationD = sqrt(biquadD[0])*6.4*spread;
	Float64 clipFactorD = 0.75+(biquadD[0]*GetParameter( kParam_Four )*37.0);
	
	double K = tan(M_PI * biquad[0]);
	double norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquad[2] = K / 0.7071 * norm;
	biquad[4] = -biquad[2];
	biquad[5] = 2.0 * (K * K - 1.0) * norm;
	biquad[6] = (1.0 - K / 0.7071 + K * K) * norm;
	
	K = tan(M_PI * biquadA[0]);
	norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquadA[2] = K / 0.7071 * norm;
	biquadA[4] = -biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / 0.7071 + K * K) * norm;
	
	K = tan(M_PI * biquadB[0]);
	norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquadB[2] = K / 0.7071 * norm;
	biquadB[4] = -biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / 0.7071 + K * K) * norm;
	
	K = tan(M_PI * biquadC[0]);
	norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquadC[2] = K / 0.7071 * norm;
	biquadC[4] = -biquadC[2];
	biquadC[5] = 2.0 * (K * K - 1.0) * norm;
	biquadC[6] = (1.0 - K / 0.7071 + K * K) * norm;
	
	K = tan(M_PI * biquadD[0]);
	norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquadD[2] = K / 0.7071 * norm;
	biquadD[4] = -biquadD[2];
	biquadD[5] = 2.0 * (K * K - 1.0) * norm;
	biquadD[6] = (1.0 - K / 0.7071 + K * K) * norm;	
	
	//opamp stuff
	double inTrim = GetParameter( kParam_One )*10.0;
	inTrim *= inTrim; inTrim *= inTrim;
	double iirAmountA = 0.00069/overallscale;
	biquadF[0] = biquadE[0] = 15500.0 / GetSampleRate();
    biquadF[1] = biquadE[1] = 0.935;
	K = tan(M_PI * biquadE[0]); //lowpass
	norm = 1.0 / (1.0 + K / biquadE[1] + K * K);
	biquadE[2] = K * K * norm;
	biquadE[3] = 2.0 * biquadE[2];
	biquadE[4] = biquadE[2];
	biquadE[5] = 2.0 * (K * K - 1.0) * norm;
	biquadE[6] = (1.0 - K / biquadE[1] + K * K) * norm;
	for (int x = 0; x < 7; x++) biquadF[x] = biquadE[x];
	//end opamp stuff	
	
	double aWet = 1.0;
	double bWet = 1.0;
	double cWet = 1.0;
	double dWet = GetParameter( kParam_Four )*4.0;
	//four-stage wet/dry control using progressive stages that bypass when not engaged
	if (dWet < 1.0) {aWet = dWet; bWet = 0.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 2.0) {bWet = dWet - 1.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 3.0) {cWet = dWet - 2.0; dWet = 0.0;}
	else {dWet -= 3.0;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	double wet = GetParameter( kParam_Five );
	double outSample = 0.0;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		double overallDrySample = drySample;
		
		if (inTrim != 1.0) inputSample *= inTrim;
		
		double nukeLevel = inputSample;

		//begin XRegion
		inputSample *= clipFactor;
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		outSample = biquad[2]*inputSample+biquad[4]*biquad[8]-biquad[5]*biquad[9]-biquad[6]*biquad[10];
		biquad[8] = biquad[7]; biquad[7] = inputSample; biquad[10] = biquad[9];
		biquad[9] = outSample; //DF1
		inputSample = outSample / compensation; nukeLevel = inputSample;
		
		if (aWet > 0.0) {
			inputSample *= clipFactorA;
			if (inputSample > 1.57079633) inputSample = 1.57079633;
			if (inputSample < -1.57079633) inputSample = -1.57079633;
			inputSample = sin(inputSample);
			outSample = biquadA[2]*inputSample+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
			biquadA[8] = biquadA[7]; biquadA[7] = inputSample; biquadA[10] = biquadA[9];
			biquadA[9] = outSample; //DF1
			inputSample = outSample / compensationA; inputSample = (inputSample * aWet) + (nukeLevel * (1.0-aWet));
			nukeLevel = inputSample;
		}
		if (bWet > 0.0) {
			inputSample *= clipFactorB;
			if (inputSample > 1.57079633) inputSample = 1.57079633;
			if (inputSample < -1.57079633) inputSample = -1.57079633;
			inputSample = sin(inputSample);
			outSample = biquadB[2]*inputSample+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSample; biquadB[10] = biquadB[9]; 
			biquadB[9] = outSample; //DF1
			inputSample = outSample / compensationB; inputSample = (inputSample * bWet) + (nukeLevel * (1.0-bWet));
			nukeLevel = inputSample;
		}
		if (cWet > 0.0) {
			inputSample *= clipFactorC;
			if (inputSample > 1.57079633) inputSample = 1.57079633;
			if (inputSample < -1.57079633) inputSample = -1.57079633;
			inputSample = sin(inputSample);
			outSample = biquadC[2]*inputSample+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
			biquadC[8] = biquadC[7]; biquadC[7] = inputSample; biquadC[10] = biquadC[9]; 
			biquadC[9] = outSample; //DF1
			inputSample = outSample / compensationC; inputSample = (inputSample * cWet) + (nukeLevel * (1.0-cWet));
			nukeLevel = inputSample;
		}
		if (dWet > 0.0) {
			inputSample *= clipFactorD;
			if (inputSample > 1.57079633) inputSample = 1.57079633;
			if (inputSample < -1.57079633) inputSample = -1.57079633;
			inputSample = sin(inputSample);
			outSample = biquadD[2]*inputSample+biquadD[4]*biquadD[8]-biquadD[5]*biquadD[9]-biquadD[6]*biquadD[10];
			biquadD[8] = biquadD[7]; biquadD[7] = inputSample; biquadD[10] = biquadD[9]; 
			biquadD[9] = outSample; //DF1
			inputSample = outSample / compensationD; inputSample = (inputSample * dWet) + (nukeLevel * (1.0-dWet));
		}
		//end XRegion
		
		//opamp stage
		if (fabs(iirSampleA)<1.18e-37) iirSampleA = 0.0;
		iirSampleA = (iirSampleA * (1.0 - iirAmountA)) + (inputSample * iirAmountA);
		inputSample -= iirSampleA;
		
		outSample = biquadE[2]*inputSample+biquadE[3]*biquadE[7]+biquadE[4]*biquadE[8]-biquadE[5]*biquadE[9]-biquadE[6]*biquadE[10];
		biquadE[8] = biquadE[7]; biquadE[7] = inputSample; inputSample = outSample; biquadE[10] = biquadE[9]; biquadE[9] = inputSample; //DF1		
		
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		inputSample -= (inputSample*inputSample*inputSample*inputSample*inputSample*0.1768);
		
		outSample = biquadF[2]*inputSample+biquadF[3]*biquadF[7]+biquadF[4]*biquadF[8]-biquadF[5]*biquadF[9]-biquadF[6]*biquadF[10];
		biquadF[8] = biquadF[7]; biquadF[7] = inputSample; inputSample = outSample; biquadF[10] = biquadF[9]; biquadF[9] = inputSample; //DF1		
		//end opamp stage
		
		if (wet !=1.0) {
			inputSample = (inputSample * wet) + (overallDrySample * (1.0-wet));
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

