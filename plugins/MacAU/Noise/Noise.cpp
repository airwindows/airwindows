/*
*	File:		Noise.cpp
*	
*	Version:	1.0
* 
*	Created:	5/7/14
*	
*	Copyright:  Copyright © 2014 Airwindows, Airwindows uses the MIT license
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
	Noise.cpp
	
=============================================================================*/
#include "Noise.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Noise)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Noise::Noise
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Noise::Noise(AudioUnit component)
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
//	Noise::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Noise::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Noise::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Noise::GetParameterInfo(AudioUnitScope		inScope,
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
//	Noise::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Noise::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Noise::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Noise::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Noise::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Noise::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____NoiseEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Noise::NoiseKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Noise::NoiseKernel::Reset()
{
	position = 99999999;
	quadratic = 0;
	noiseA = 0.0;
	noiseB = 0.0;
	noiseC = 0.0;
	rumbleA = 0.0;
	rumbleB = 0.0;
	surge = 0.0;
	flip = false;
	filterflip = false;	
	for(int count = 0; count < 11; count++) {b[count] = 0.0; f[count] = 0.0;}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Noise::NoiseKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Noise::NoiseKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	Float64 cutoff;
	Float64 cutofftarget = (GetParameter( kParam_One )*3.5);
	Float64 rumblecutoff = cutofftarget * 0.005;
	Float64 invcutoff;
	double inputSample;
	Float64 drySample;
	Float64 highpass = GetParameter( kParam_Three )*38.0;
	int lowcut = floor(highpass);
	int dcut;
	if (lowcut > 37) {dcut= 1151;}
	if (lowcut == 37) {dcut= 1091;}
	if (lowcut == 36) {dcut= 1087;}
	if (lowcut == 35) {dcut= 1031;}
	if (lowcut == 34) {dcut= 1013;}
	if (lowcut == 33) {dcut= 971;}
	if (lowcut == 32) {dcut= 907;}
	if (lowcut == 31) {dcut= 839;}
	if (lowcut == 30) {dcut= 797;}
	if (lowcut == 29) {dcut= 733;}
	if (lowcut == 28) {dcut= 719;}
	if (lowcut == 27) {dcut= 673;}
	if (lowcut == 26) {dcut= 613;}
	if (lowcut == 25) {dcut= 593;}
	if (lowcut == 24) {dcut= 541;}
	if (lowcut == 23) {dcut= 479;}
	if (lowcut == 22) {dcut= 431;}
	if (lowcut == 21) {dcut= 419;}
	if (lowcut == 20) {dcut= 373;}
	if (lowcut == 19) {dcut= 311;}
	if (lowcut == 18) {dcut= 293;}
	if (lowcut == 17) {dcut= 233;}
	if (lowcut == 16) {dcut= 191;}
	if (lowcut == 15) {dcut= 173;}
	if (lowcut == 14) {dcut= 131;}
	if (lowcut == 13) {dcut= 113;}
	if (lowcut == 12) {dcut= 71;}
	if (lowcut == 11) {dcut= 53;}
	if (lowcut == 10) {dcut= 31;}
	if (lowcut == 9) {dcut= 27;}
	if (lowcut == 8) {dcut= 23;}
	if (lowcut == 7) {dcut= 19;}
	if (lowcut == 6) {dcut= 17;}
	if (lowcut == 5) {dcut= 13;}
	if (lowcut == 4) {dcut= 11;}
	if (lowcut == 3) {dcut= 7;}
	if (lowcut == 2) {dcut= 5;}
	if (lowcut < 2) {dcut= 3;}
	highpass = GetParameter( kParam_Two ) * 22.0;
	lowcut = floor(highpass)+1;
		
	Float64 decay = 0.001 - ((1.0-pow(1.0-GetParameter( kParam_Four ),3))*0.001);
	if (decay == 0.001) decay = 0.1;
	Float64 wet = GetParameter( kParam_Six );
	//removed unnecessary dry variable
	wet *= 0.01; //correct large gain issue
	Float64 correctionSample;
	Float64 accumulatorSample;
	Float64 overallscale = (GetParameter( kParam_Five )*9.0)+1.0;
	Float64 gain = overallscale;
	
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
	
	if (overallscale < 1.0) overallscale = 1.0;
	f[0] /= overallscale;
	f[1] /= overallscale;
	f[2] /= overallscale;
	f[3] /= overallscale;
	f[4] /= overallscale;
	f[5] /= overallscale;
	f[6] /= overallscale;
	f[7] /= overallscale;
	f[8] /= overallscale;
	f[9] /= overallscale;
	//and now it's neatly scaled, too
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		drySample = inputSample;
		
		if (surge<fabs(inputSample))
		{
			surge += (double(fpd)/UINT32_MAX)*(fabs(inputSample)-surge);
			if (surge > 1.0) surge = 1.0;
		}
		else
		{
			surge -= ((double(fpd)/UINT32_MAX)*(surge-fabs(inputSample))*decay);
			if (surge < 0.0) surge = 0.0;
		}
		
		cutoff = pow((cutofftarget*surge),5);
		if (cutoff > 1.0) cutoff = 1.0;
		invcutoff = 1.0 - cutoff;
		//set up modified cutoff
		
		flip = !flip;
		filterflip = !filterflip;
		quadratic -= 1;
		if (quadratic < 0)
		{
			position += 1;		
			quadratic = position * position;
			quadratic = quadratic % 170003; //% is C++ mod operator
			quadratic *= quadratic;
			quadratic = quadratic % 17011; //% is C++ mod operator
			quadratic *= quadratic;
			//quadratic = quadratic % 1709; //% is C++ mod operator
			//quadratic *= quadratic;
			quadratic = quadratic % dcut; //% is C++ mod operator
			quadratic *= quadratic;
			quadratic = quadratic % lowcut;
			//sets density of the centering force
			if (noiseA < 0) {flip = true;}
			else {flip = false;}
		}
		
		
		if (flip) noiseA += (double(fpd)/UINT32_MAX);
		else noiseA -= (double(fpd)/UINT32_MAX);
		
		if (filterflip)
		{
			noiseB *= invcutoff; noiseB += (noiseA*cutoff);
			inputSample = noiseB+noiseC;
			rumbleA *= (1.0-rumblecutoff);
			rumbleA += (inputSample*rumblecutoff);
		}
		else 
		{
			noiseC *= invcutoff; noiseC += (noiseA*cutoff);
			inputSample = noiseB+noiseC;
			rumbleB *= (1.0-rumblecutoff);
			rumbleB += (inputSample*rumblecutoff);
		}
		
		inputSample -= (rumbleA+rumbleB);
		inputSample *= (1.0-rumblecutoff);
		
		inputSample *= wet;
		inputSample += (drySample * (1.0-wet));
		//apply the dry to the noise
		
		b[9] = b[8]; b[8] = b[7]; b[7] = b[6]; b[6] = b[5];
		b[5] = b[4]; b[4] = b[3]; b[3] = b[2]; b[2] = b[1];
		b[1] = b[0]; b[0] = accumulatorSample = inputSample;
		
		accumulatorSample *= f[0];
		accumulatorSample += (b[1] * f[1]);
		accumulatorSample += (b[2] * f[2]);
		accumulatorSample += (b[3] * f[3]);
		accumulatorSample += (b[4] * f[4]);
		accumulatorSample += (b[5] * f[5]);
		accumulatorSample += (b[6] * f[6]);
		accumulatorSample += (b[7] * f[7]);
		accumulatorSample += (b[8] * f[8]);
		accumulatorSample += (b[9] * f[9]);
		//we are doing our repetitive calculations on a separate value
		
		correctionSample = inputSample - accumulatorSample;
		//we're gonna apply the total effect of all these calculations as a single subtract
		
		inputSample -= correctionSample;
		//applying the distance calculation to both the dry AND the noise output to blend them
				
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels;
		destP += inNumChannels;
	}
}

