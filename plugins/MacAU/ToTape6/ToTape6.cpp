/*
*	File:		ToTape6.cpp
*	
*	Version:	1.0
* 
*	Created:	11/30/19
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
	ToTape6.cpp
	
=============================================================================*/
#include "ToTape6.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(ToTape6)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape6::ToTape6
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ToTape6::ToTape6(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
	SetParameter(kParam_Five, kDefaultValue_ParamFive );
	SetParameter(kParam_Six, kDefaultValue_ParamSix );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape6::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ToTape6::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape6::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ToTape6::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -12.0;
                outParameterInfo.maxValue = 12.0;
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -12.0;
                outParameterInfo.maxValue = 12.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFive;
                break;
           case kParam_Six:
                AUBase::FillInParameterName (outParameterInfo, kParameterSixName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSix;
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
//	ToTape6::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ToTape6::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape6::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ToTape6::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ToTape6::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ToTape6::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ToTape6EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape6::ToTape6Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ToTape6::ToTape6Kernel::Reset()
{
	iirMidRollerA = 0.0;
	iirMidRollerB = 0.0;
	iirHeadBumpA = 0.0;
	iirHeadBumpB = 0.0;	
	for (int x = 0; x < 9; x++) {biquadA[x] = 0.0;biquadB[x] = 0.0;biquadC[x] = 0.0;biquadD[x] = 0.0;}
	flip = false;
	for (int temp = 0; temp < 501; temp++) {d[temp] = 0.0;}
	gcount = 0;	
	sweep = pi;
	rateof = 0.5;
	nextmax = 0.5;
	lastSample = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape6::ToTape6Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ToTape6::ToTape6Kernel::Process(	const Float32 	*inSourceP,
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
	
	Float64 inputgain = pow(10.0,GetParameter( kParam_One )/20.0);
	Float64 SoftenControl = pow(GetParameter( kParam_Two ),2);
	Float64 RollAmount = (1.0-(SoftenControl * 0.45))/overallscale;
	Float64 HeadBumpControl = GetParameter( kParam_Three ) * 0.25 * inputgain;
	Float64 HeadBumpFreq = 0.12/overallscale;
	//[0] is frequency: 0.000001 to 0.499999 is near-zero to near-Nyquist
	//[1] is resonance, 0.7071 is Butterworth. Also can't be zero
	biquadA[0] = biquadB[0] = 0.007/overallscale;
	biquadA[1] = biquadB[1] = 0.0009;
	double K = tan(M_PI * biquadB[0]);
	double norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadA[2] = biquadB[2] = K / biquadB[1] * norm;
	biquadA[4] = biquadB[4] = -biquadB[2];
	biquadA[5] = biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;
	
	biquadC[0] = biquadD[0] = 0.032/overallscale;
	biquadC[1] = biquadD[1] = 0.0007;
	K = tan(M_PI * biquadD[0]);
	norm = 1.0 / (1.0 + K / biquadD[1] + K * K);
	biquadC[2] = biquadD[2] = K / biquadD[1] * norm;
	biquadC[4] = biquadD[4] = -biquadD[2];
	biquadC[5] = biquadD[5] = 2.0 * (K * K - 1.0) * norm;
	biquadC[6] = biquadD[6] = (1.0 - K / biquadD[1] + K * K) * norm;
	
	Float64 depth = pow(GetParameter( kParam_Four ),2)*overallscale*70;
	Float64 fluttertrim = (0.0024*pow(GetParameter( kParam_Four ),2))/overallscale;
	Float64 outputgain = pow(10.0,GetParameter( kParam_Five )/20.0);
	
	Float64 refclip = 0.99;
	Float64 softness = 0.618033988749894848204586;
	
	Float64 wet = GetParameter( kParam_Six );
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		if (inputgain < 1.0) {
			inputSample *= inputgain;
		} //gain cut before plugin
		

		Float64 flutterrandy = fpd / (double)UINT32_MAX;
		//now we've got a random flutter, so we're messing with the pitch before tape effects go on
		if (gcount < 0 || gcount > 499) {gcount = 499;}
		d[gcount] = inputSample;
		int count = gcount;
		if (depth != 0.0) {
			
			double offset = depth + (depth * pow(rateof,2) * sin(sweep));
			
			count += (int)floor(offset);
			inputSample = (d[count-((count > 499)?500:0)] * (1-(offset-floor(offset))) );
			inputSample += (d[count+1-((count+1 > 499)?500:0)] * (offset-floor(offset)) );
			
			rateof = (rateof * (1.0-fluttertrim)) + (nextmax * fluttertrim);
			sweep += rateof * fluttertrim;
			
			if (sweep >= (pi*2.0)) {
				sweep -= pi;
				nextmax = 0.24 + (flutterrandy * 0.74);
			}
			//apply to input signal only when flutter is present, interpolate samples
		}
		gcount--;

		double vibDrySample = inputSample;
		double HighsSample = 0.0;
		double NonHighsSample = 0.0;
		double tempSample;
		
		if (flip)
		{
			iirMidRollerA = (iirMidRollerA * (1.0 - RollAmount)) + (inputSample * RollAmount);
			HighsSample = inputSample - iirMidRollerA;
			NonHighsSample = iirMidRollerA;
			
			iirHeadBumpA += (inputSample * 0.05);
			iirHeadBumpA -= (iirHeadBumpA * iirHeadBumpA * iirHeadBumpA * HeadBumpFreq);
			iirHeadBumpA = sin(iirHeadBumpA);
			tempSample = (iirHeadBumpA * biquadA[2]) + biquadA[7];
			biquadA[7] = (iirHeadBumpA * biquadA[3]) - (tempSample * biquadA[5]) + biquadA[8];
			biquadA[8] = (iirHeadBumpA * biquadA[4]) - (tempSample * biquadA[6]);
			iirHeadBumpA = tempSample; //interleaved biquad
			if (iirHeadBumpA > 1.0) iirHeadBumpA = 1.0;
			if (iirHeadBumpA < -1.0) iirHeadBumpA = -1.0;
			iirHeadBumpA = asin(iirHeadBumpA);
		
			inputSample = sin(inputSample);
			tempSample = (inputSample * biquadC[2]) + biquadC[7];
			biquadC[7] = (inputSample * biquadC[3]) - (tempSample * biquadC[5]) + biquadC[8];
			biquadC[8] = (inputSample * biquadC[4]) - (tempSample * biquadC[6]);
			inputSample = tempSample; //interleaved biquad
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			inputSample = asin(inputSample);
		} else {
			iirMidRollerB = (iirMidRollerB * (1.0 - RollAmount)) + (inputSample * RollAmount);
			HighsSample = inputSample - iirMidRollerB;
			NonHighsSample = iirMidRollerB;
			
			iirHeadBumpB += (inputSample * 0.05);
			iirHeadBumpB -= (iirHeadBumpB * iirHeadBumpB * iirHeadBumpB * HeadBumpFreq);
			iirHeadBumpB = sin(iirHeadBumpB);
			tempSample = (iirHeadBumpB * biquadB[2]) + biquadB[7];
			biquadB[7] = (iirHeadBumpB * biquadB[3]) - (tempSample * biquadB[5]) + biquadB[8];
			biquadB[8] = (iirHeadBumpB * biquadB[4]) - (tempSample * biquadB[6]);
			iirHeadBumpB = tempSample; //interleaved biquad
			if (iirHeadBumpB > 1.0) iirHeadBumpB = 1.0;
			if (iirHeadBumpB < -1.0) iirHeadBumpB = -1.0;
			iirHeadBumpB = asin(iirHeadBumpB);
		
			inputSample = sin(inputSample);
			tempSample = (inputSample * biquadD[2]) + biquadD[7];
			biquadD[7] = (inputSample * biquadD[3]) - (tempSample * biquadD[5]) + biquadD[8];
			biquadD[8] = (inputSample * biquadD[4]) - (tempSample * biquadD[6]);
			inputSample = tempSample; //interleaved biquad
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			inputSample = asin(inputSample);
		}
		flip = !flip;

		double groundSample = vibDrySample - inputSample; //set up UnBox on fluttered audio
		
		if (inputgain > 1.0) {
			inputSample *= inputgain;
		} //gain boost inside UnBox/Mojo
		
		double applySoften = fabs(HighsSample)*1.57079633;
		if (applySoften > 1.57079633) applySoften = 1.57079633;
		applySoften = 1-cos(applySoften);
		if (HighsSample > 0) inputSample -= applySoften;
		if (HighsSample < 0) inputSample += applySoften;
		//apply Soften depending on polarity
		
		Float64 suppress = (1.0-fabs(inputSample)) * 0.00013;
		if (iirHeadBumpA > suppress) iirHeadBumpA -= suppress;
		if (iirHeadBumpA < -suppress) iirHeadBumpA += suppress;
		if (iirHeadBumpB > suppress) iirHeadBumpB -= suppress;
		if (iirHeadBumpB < -suppress) iirHeadBumpB += suppress;
		//restrain resonant quality of head bump algorithm
		
		inputSample += ((iirHeadBumpA + iirHeadBumpB) * HeadBumpControl);
		//apply Fatten.
		
		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
		double mojo; mojo = pow(fabs(inputSample),0.25);
		if (mojo > 0.0) inputSample = (sin(inputSample * mojo * M_PI * 0.5) / mojo);
		//mojo is the one that flattens WAAAAY out very softly before wavefolding		
		
		inputSample += groundSample; //apply UnBox processing

		if (outputgain != 1.0) {
			inputSample *= outputgain;
		}
		
		
		if (lastSample >= refclip)
		{
			if (inputSample < refclip) lastSample = ((refclip*softness) + (inputSample * (1.0-softness)));
			else lastSample = refclip;
		}
		
		if (lastSample <= -refclip)
		{
			if (inputSample > -refclip) lastSample = ((-refclip*softness) + (inputSample * (1.0-softness)));
			else lastSample = -refclip;
		}
		
		if (inputSample > refclip)
		{
			if (lastSample < refclip) inputSample = ((refclip*softness) + (lastSample * (1.0-softness)));
			else inputSample = refclip;
		}
		
		if (inputSample < -refclip)
		{
			if (lastSample > -refclip) inputSample = ((-refclip*softness) + (lastSample * (1.0-softness)));
			else inputSample = -refclip;
		}
		lastSample = inputSample;
		
		if (inputSample > refclip) inputSample = refclip;
		if (inputSample < -refclip) inputSample = -refclip;
		//final iron bar
		
		if (wet !=1.0) {
			inputSample = (inputSample * wet) + (drySample * (1.0 - wet));
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

