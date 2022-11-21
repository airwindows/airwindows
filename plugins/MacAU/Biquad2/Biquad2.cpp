/*
*	File:		Biquad2.cpp
*	
*	Version:	1.0
* 
*	Created:	8/29/19
*	
*	Copyright:  Copyright © 2019 Airwindows, Airwindows uses the MIT license
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
	Biquad2.cpp
	
=============================================================================*/
#include "Biquad2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Biquad2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Biquad2::Biquad2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Biquad2::Biquad2(AudioUnit component)
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
//	Biquad2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Biquad2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Biquad2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Biquad2::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = 1;
                outParameterInfo.maxValue = 4;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
				outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
				outParameterInfo.minValue = 0.003;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
				outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
				outParameterInfo.minValue = 1.0;
                outParameterInfo.maxValue = 50.0;
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
                outParameterInfo.minValue = -1.0;
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
//	Biquad2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Biquad2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Biquad2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Biquad2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Biquad2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Biquad2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Biquad2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Biquad2::Biquad2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Biquad2::Biquad2Kernel::Reset()
{
	for (int x = 0; x < 11; x++) {biquad[x] = 0.0; b[x] = 0.0; f[x] = 0.0;}
	frequencychase = 0.0015;
	resonancechase = 0.001;
	outputchase = 1.0;
	wetchase = 1.0;
	
	frequencysetting = -1.0;
	resonancesetting = -1.0;
	outputsetting = -1.0;
	wetsetting = -2.0; //-1.0 is a possible setting here and this forces an update on chasespeed
	
	chasespeed = 500.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Biquad2::Biquad2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Biquad2::Biquad2Kernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	Float64 overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	
	int type = GetParameter( kParam_One);
	Float64 average = GetParameter( kParam_Two );
	Float64 frequencytarget = average*0.39; //biquad[0], goes to 1.0
	frequencytarget /= overallscale;
	if (frequencytarget < 0.0015/overallscale) frequencytarget = 0.0015/overallscale;
    Float64 resonancetarget = GetParameter( kParam_Three ); //biquad[1], goes to 50.0
	if (resonancetarget < 1.0) resonancetarget = 1.0;
	Float64 outputtarget = GetParameter( kParam_Four ); //scaled to res
	if (type < 3) outputtarget /= sqrt(resonancetarget);
	Float64 wettarget = GetParameter( kParam_Five ); //wet, goes -1.0 to 1.0
	
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
	//[9] is a stored delayed sample (you have to include the coefficient making code if you do that)
	//[10] is a stored delayed sample (you have to include the coefficient making code if you do that)
	Float64 K = tan(M_PI * biquad[0]);
	Float64 norm = 1.0 / (1.0 + K / biquad[1] + K * K);
	//finished setting up biquad
	
	average = (1.0-average)*10.0; //max taps is 10, and low settings use more
	
	if (type == 1 || type == 3) average = 1.0;
	
	Float64 gain = average;
	if (gain > 1.0) {f[0] = 1.0; gain -= 1.0;} else {f[0] = gain; gain = 0.0;}
	if (gain > 1.0) {f[1] = 1.0; gain -= 1.0;} else {f[1] = gain; gain = 0.0;}
	if (gain > 1.0) {f[2] = 1.0; gain -= 1.0;} else {f[2] = gain; gain = 0.0;}
	if (gain > 1.0) {f[3] = 1.0; gain -= 1.0;} else {f[3] = gain; gain = 0.0;}
	if (gain > 1.0) {f[4] = 1.0; gain -= 1.0;} else {f[4] = gain; gain = 0.0;}
	if (gain > 1.0) {f[5] = 1.0; gain -= 1.0;} else {f[5] = gain; gain = 0.0;}
	if (gain > 1.0) {f[6] = 1.0; gain -= 1.0;} else {f[6] = gain; gain = 0.0;}
	if (gain > 1.0) {f[7] = 1.0; gain -= 1.0;} else {f[7] = gain; gain = 0.0;}
	if (gain > 1.0) {f[8] = 1.0; gain -= 1.0;} else {f[8] = gain; gain = 0.0;}
	if (gain > 1.0) {f[9] = 1.0; gain -= 1.0;} else {f[9] = gain; gain = 0.0;}
	//there, now we have a neat little moving average with remainders
	
	if (average < 1.0) average = 1.0;
	f[0] /= average;
	f[1] /= average;
	f[2] /= average;
	f[3] /= average;
	f[4] /= average;
	f[5] /= average;
	f[6] /= average;
	f[7] /= average;
	f[8] /= average;
	f[9] /= average;
	//and now it's neatly scaled, too
	//finished setting up average
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = *sourceP;
		
		Float64 chasespeed = 50000;
		if (frequencychase < frequencytarget) chasespeed = 500000;
		chasespeed /= resonancechase;
		chasespeed *= overallscale;
		
		frequencychase = (((frequencychase*chasespeed)+frequencytarget)/(chasespeed+1.0));
		
		Float64 fasterchase = 1000 * overallscale;		
		resonancechase = (((resonancechase*fasterchase)+resonancetarget)/(fasterchase+1.0));
		outputchase = (((outputchase*fasterchase)+outputtarget)/(fasterchase+1.0));
		wetchase = (((wetchase*fasterchase)+wettarget)/(fasterchase+1.0));
		if (biquad[0] != frequencychase) {biquad[0] = frequencychase; K = tan(M_PI * biquad[0]);}
		if (biquad[1] != resonancechase) {biquad[1] = resonancechase; norm = 1.0 / (1.0 + K / biquad[1] + K * K);}
		
		if (type == 1) { //lowpass
			biquad[2] = K * K * norm;
			biquad[3] = 2.0 * biquad[2];
			biquad[4] = biquad[2];
			biquad[5] = 2.0 * (K * K - 1.0) * norm;
		}
		
		if (type == 2) { //highpass
			biquad[2] = norm;
			biquad[3] = -2.0 * biquad[2];
			biquad[4] = biquad[2];
			biquad[5] = 2.0 * (K * K - 1.0) * norm;
		}
		
		if (type == 3) { //bandpass
			biquad[2] = K / biquad[1] * norm;
			biquad[3] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
			biquad[4] = -biquad[2];
			biquad[5] = 2.0 * (K * K - 1.0) * norm;
		}
		
		if (type == 4) { //notch
			biquad[2] = (1.0 + K * K) * norm;
			biquad[3] = 2.0 * (K * K - 1) * norm;
			biquad[4] = biquad[2];
			biquad[5] = biquad[3];
		}
		
		biquad[6] = (1.0 - K / biquad[1] + K * K) * norm;
				
		inputSample = sin(inputSample);
		//encode Console5: good cleanness
				
		double outSample = biquad[2]*inputSample+biquad[3]*biquad[7]+biquad[4]*biquad[8]-biquad[5]*biquad[9]-biquad[6]*biquad[10];
		biquad[8] = biquad[7]; biquad[7] = inputSample; inputSample = outSample; biquad[10] = biquad[9]; biquad[9] = inputSample; //DF1
				
		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
			
		b[9] = b[8]; b[8] = b[7]; b[7] = b[6]; b[6] = b[5];
		b[5] = b[4]; b[4] = b[3]; b[3] = b[2]; b[2] = b[1];
		b[1] = b[0]; b[0] = inputSample;
		
		inputSample *= f[0];
		inputSample += (b[1] * f[1]);
		inputSample += (b[2] * f[2]);
		inputSample += (b[3] * f[3]);
		inputSample += (b[4] * f[4]);
		inputSample += (b[5] * f[5]);
		inputSample += (b[6] * f[6]);
		inputSample += (b[7] * f[7]);
		inputSample += (b[8] * f[8]);
		inputSample += (b[9] * f[9]); //intense averaging on deeper cutoffs
		
		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
		//without this, you can get a NaN condition where it spits out DC offset at full blast!
		inputSample = asin(inputSample);
		//amplitude aspect
		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
		//and then Console5 will spit out overs if you let it
		
		if (outputchase < 1.0) {
			inputSample *= outputchase;
		}
		
		if (wetchase < 1.0) {
			inputSample = (inputSample*wetchase) + (drySample*(1.0-fabs(wetchase)));
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

