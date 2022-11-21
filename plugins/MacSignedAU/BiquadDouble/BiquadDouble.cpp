/*
*	File:		BiquadDouble.cpp
*	
*	Version:	1.0
* 
*	Created:	10/2/20
*	
*	Copyright:  Copyright © 2020 Airwindows, Airwindows uses the MIT license
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
	BiquadDouble.cpp
	
=============================================================================*/
#include "BiquadDouble.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, BiquadDouble)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadDouble::BiquadDouble
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BiquadDouble::BiquadDouble(AudioUnit component)
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
//	BiquadDouble::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BiquadDouble::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadDouble::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BiquadDouble::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 1.0;
                outParameterInfo.maxValue = 4.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
				outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
                outParameterInfo.minValue = 0.0001;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
				outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
                outParameterInfo.minValue = 0.01;
                outParameterInfo.maxValue = 30.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
                break;
			case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
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
//	BiquadDouble::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BiquadDouble::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadDouble::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BiquadDouble::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	BiquadDouble::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult BiquadDouble::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____BiquadDoubleEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadDouble::BiquadDoubleKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BiquadDouble::BiquadDoubleKernel::Reset()
{
	for (int x = 0; x < 9; x++) {biquadA[x] = 0.0;biquadB[x] = 0.0;}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BiquadDouble::BiquadDoubleKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BiquadDouble::BiquadDoubleKernel::Process(	const Float32 	*inSourceP,
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
	
	int type = GetParameter( kParam_One);
	
	biquadA[0] = GetParameter( kParam_Two )*0.499;
	if (biquadA[0] < 0.0001) biquadA[0] = 0.0001;
	
    biquadA[1] = GetParameter( kParam_Three );
	if (biquadA[1] < 0.0001) biquadA[1] = 0.0001;
	
	Float64 wet = GetParameter( kParam_Four );
	
	//biquad contains these values:
	//[0] is frequency: 0.000001 to 0.499999 is near-zero to near-Nyquist
	//[1] is resonance, 0.7071 is Butterworth. Also can't be zero
	//[2] is a0 but you need distinct ones for additional biquad instances so it's here
	//[3] is a1 but you need distinct ones for additional biquad instances so it's here
	//[4] is a2 but you need distinct ones for additional biquad instances so it's here
	//[5] is b1 but you need distinct ones for additional biquad instances so it's here
	//[6] is b2 but you need distinct ones for additional biquad instances so it's here
	//[7] is a stored delayed sample (freq and res are stored so you can move them sample by sample)
	//[8] is a stored delayed sample (you have to include the coefficient making code if you do that)
	
	//to build a dedicated filter, rename 'biquad' to whatever the new filter is, then
	//put this code either within the sample buffer (for smoothly modulating freq or res)
	//or in this 'read the controls' area (for letting you change freq and res with controls)
	//or in 'reset' if the freq and res are absolutely fixed (use GetSampleRate to define freq)
	
	if (type == 1) { //lowpass
		double K = tan(M_PI * biquadA[0]);
		double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
		biquadA[2] = K * K * norm;
		biquadA[3] = 2.0 * biquadA[2];
		biquadA[4] = biquadA[2];
		biquadA[5] = 2.0 * (K * K - 1.0) * norm;
		biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	}
	
	if (type == 2) { //highpass
		double K = tan(M_PI * biquadA[0]);
		double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
		biquadA[2] = norm;
		biquadA[3] = -2.0 * biquadA[2];
		biquadA[4] = biquadA[2];
		biquadA[5] = 2.0 * (K * K - 1.0) * norm;
		biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	}
	
	if (type == 3) { //bandpass
		double K = tan(M_PI * biquadA[0]);
		double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
		biquadA[2] = K / biquadA[1] * norm;
		biquadA[3] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
		biquadA[4] = -biquadA[2];
		biquadA[5] = 2.0 * (K * K - 1.0) * norm;
		biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	}
	
	if (type == 4) { //notch
		double K = tan(M_PI * biquadA[0]);
		double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
		biquadA[2] = (1.0 + K * K) * norm;
		biquadA[3] = 2.0 * (K * K - 1) * norm;
		biquadA[4] = biquadA[2];
		biquadA[5] = biquadA[3];
		biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	}
	
	for (int x = 0; x < 7; x++) {biquadB[x] = biquadA[x];}

	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = *sourceP;
		
		
		inputSample = sin(inputSample);
		//encode Console5: good cleanness
		
		double tempSample = (inputSample * biquadA[2]) + biquadA[7];
		biquadA[7] = (inputSample * biquadA[3]) - (tempSample * biquadA[5]) + biquadA[8];
		biquadA[8] = (inputSample * biquadA[4]) - (tempSample * biquadA[6]);
		inputSample = tempSample;
		
		tempSample = (inputSample * biquadB[2]) + biquadB[7];
		biquadB[7] = (inputSample * biquadB[3]) - (tempSample * biquadB[5]) + biquadB[8];
		biquadB[8] = (inputSample * biquadB[4]) - (tempSample * biquadB[6]);
		inputSample = tempSample;
		
		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
		//without this, you can get a NaN condition where it spits out DC offset at full blast!
		inputSample = asin(inputSample);
		//amplitude aspect
		
		if (wet < 1.0) {
			inputSample = (inputSample*wet) + (drySample*(1.0-fabs(wet)));
			//inv/dry/wet lets us turn LP into HP and band into notch
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

/*
 if (type == 5) { //peak, but I prefer to assemble this from bandpass/notch
 double biquadA[0] = freq; // 0.000001 to 0.499999 is near-zero to near-Nyquist
 double biquadA[1] = reso; // 0.000001 to >10 is resonance, 0.7071 is Butterworth
 double peakGain = boost; //negative or positive gain, in dB
 if (peakGain >= 0) {    // boost
 double V = pow(10, fabs(peakGain) / 20.0);
 double K = tan(M_PI * biquadA[0]);
 double norm = 1 / (1 + 1/biquadA[1] * K + K * K);
 biquadA[2] = (1 + V/biquadA[1] * K + K * K) * norm;
 biquadA[3] = 2 * (K * K - 1) * norm;
 biquadA[4] = (1 - V/biquadA[1] * K + K * K) * norm;
 biquadA[5] = biquadA[3];
 biquadA[6] = (1 - 1/biquadA[1] * K + K * K) * norm;
 } else {// cut
 double V = pow(10, fabs(peakGain) / 20.0);
 double K = tan(M_PI * biquadA[0]);
 double norm = 1 / (1 + V/biquadA[1] * K + K * K);
 biquadA[2] = (1 + 1/biquadA[1] * K + K * K) * norm;
 biquadA[3] = 2 * (K * K - 1) * norm;
 biquadA[4] = (1 - 1/biquadA[1] * K + K * K) * norm;
 biquadA[5] = biquadA[3];
 biquadA[6] = (1 - V/biquadA[1] * K + K * K) * norm;
 }
 }
 
 if (type == 6) { //lowshelf, but I prefer to assemble this from raw lowpass/highpass
 double biquadA[0] = freq; // 0.000001 to 0.499999 is near-zero to near-Nyquist
 double peakGain = 0.0; //negative or positive gain, in dB
 if (peakGain >= 0) {    // boost
 double V = pow(10, fabs(peakGain) / 20.0);
 double K = tan(M_PI * biquadA[0]);
 double norm = 1 / (1 + sqrt(2) * K + K * K);
 biquadA[2] = (1 + sqrt(2*V) * K + V * K * K) * norm;
 biquadA[3] = 2 * (V * K * K - 1) * norm;
 biquadA[4] = (1 - sqrt(2*V) * K + V * K * K) * norm;
 biquadA[5] = 2 * (K * K - 1) * norm;
 biquadA[6] = (1 - sqrt(2) * K + K * K) * norm;
 } else {    // cut
 double V = pow(10, fabs(peakGain) / 20.0);
 double K = tan(M_PI * biquadA[0]);
 double norm = 1 / (1 + sqrt(2*V) * K + V * K * K);
 biquadA[2] = (1 + sqrt(2) * K + K * K) * norm;
 biquadA[3] = 2 * (K * K - 1) * norm;
 biquadA[4] = (1 - sqrt(2) * K + K * K) * norm;
 biquadA[5] = 2 * (V * K * K - 1) * norm;
 biquadA[6] = (1 - sqrt(2*V) * K + V * K * K) * norm;
 }
 }
 
 if (type == 7) { //highshelf, but I prefer to assemble this from raw lowpass/highpass
 double biquadA[0] = freq; // 0.000001 to 0.499999 is near-zero to near-Nyquist
 double peakGain = 0.0; //negative or positive gain, in dB
 if (peakGain >= 0) {    // boost
 double V = pow(10, fabs(peakGain) / 20.0);
 double K = tan(M_PI * biquadA[0]);
 double norm = 1 / (1 + sqrt(2) * K + K * K);
 biquadA[2] = (V + sqrt(2*V) * K + K * K) * norm;
 biquadA[3] = 2 * (K * K - V) * norm;
 biquadA[4] = (V - sqrt(2*V) * K + K * K) * norm;
 biquadA[5] = 2 * (K * K - 1) * norm;
 biquadA[6] = (1 - sqrt(2) * K + K * K) * norm;
 } else {    // cut
 double V = pow(10, fabs(peakGain) / 20.0);
 double K = tan(M_PI * biquadA[0]);
 double norm = 1 / (V + sqrt(2*V) * K + K * K);
 biquadA[2] = (1 + sqrt(2) * K + K * K) * norm;
 biquadA[3] = 2 * (K * K - 1) * norm;
 biquadA[4] = (1 - sqrt(2) * K + K * K) * norm;
 biquadA[5] = 2 * (K * K - V) * norm;
 biquadA[6] = (V - sqrt(2*V) * K + K * K) * norm;
 }
 }
 */

