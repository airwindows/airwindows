/*
*	File:		VoiceOfTheStarship.cpp
*	
*	Version:	1.0
* 
*	Created:	10/12/10
*	
*	Copyright:  Copyright © 2010 Airwindows, Airwindows uses the MIT license
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
	VoiceOfTheStarship.h
	
=============================================================================*/
#include "VoiceOfTheStarship.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, VoiceOfTheStarship)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VoiceOfTheStarship::VoiceOfTheStarship
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
VoiceOfTheStarship::VoiceOfTheStarship(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
        
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VoiceOfTheStarship::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		VoiceOfTheStarship::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VoiceOfTheStarship::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		VoiceOfTheStarship::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 16;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
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
//	VoiceOfTheStarship::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		VoiceOfTheStarship::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VoiceOfTheStarship::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		VoiceOfTheStarship::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VoiceOfTheStarship::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult VoiceOfTheStarship::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____VoiceOfTheStarshipEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VoiceOfTheStarship::VoiceOfTheStarshipKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		VoiceOfTheStarship::VoiceOfTheStarshipKernel::Reset()
{
	position = 99999999;
	quadratic = 0;
	noiseA = 0.0;
	noiseB = 0.0;
	noiseC = 0.0;
	flip = false;
	filterflip = false;
	for(int count = 0; count < 11; count++) {b[count] = 0.0; f[count] = 0.0;}
	lastAlgorithm = 0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VoiceOfTheStarship::VoiceOfTheStarshipKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		VoiceOfTheStarship::VoiceOfTheStarshipKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels,
                                                    bool			&ioSilence )
{
	//This code will pass-thru the audio data.
	//This is where you want to process data to produce an effect.

	
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	Float64 cutoff = pow((GetParameter( kParam_One )*0.89)+0.1,3);
	if (cutoff > 1.0) cutoff = 1.0;
	Float64 invcutoff = 1.0 - cutoff;
	//this is the lowpass
	
	Float64 overallscale = ((1.0-GetParameter( kParam_One ))*9.0)+1.0;
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
	//this is the moving average with remainders
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

	int lowcut = GetParameter( kParam_Two );
	if (lastAlgorithm != lowcut) {
		noiseA = 0.0; noiseB = 0.0; noiseC = 0.0;
		for(int count = 0; count < 11; count++) {b[count] = 0.0;}
		lastAlgorithm = lowcut;
	}
	//cuts the noise back to 0 if we are changing algorithms,
	//because that also changes gains and can make loud pops.
	//We still get pops, but they'd be even worse
	int dcut;
	if (lowcut > 15) {lowcut = 1151; dcut= 11517;}
	if (lowcut == 15) {lowcut = 113; dcut= 1151;}
	if (lowcut == 14) {lowcut = 71; dcut= 719;}
	if (lowcut == 13) {lowcut = 53; dcut= 541;}
	if (lowcut == 12) {lowcut = 31; dcut= 311;}
	if (lowcut == 11) {lowcut = 23; dcut= 233;}
	if (lowcut == 10) {lowcut = 19; dcut= 191;}
	if (lowcut == 9) {lowcut = 17; dcut= 173;}
	if (lowcut == 8) {lowcut = 13; dcut= 131;}
	if (lowcut == 7) {lowcut = 11; dcut= 113;}
	if (lowcut == 6) {lowcut = 7; dcut= 79;}
	if (lowcut == 5) {lowcut = 6; dcut= 67;}
	if (lowcut == 4) {lowcut = 5; dcut= 59;}
	if (lowcut == 3) {lowcut = 4; dcut= 43;}
	if (lowcut == 2) {lowcut = 3; dcut= 37;}
	if (lowcut == 1) {lowcut = 2; dcut= 23;}
	if (lowcut < 1) {lowcut = 1; dcut= 11;}
	//this is the mechanism for cutting back subs without filtering
	
	Float64 rumbletrim = sqrt(lowcut);
	//this among other things is just to give volume compensation
	Float64 inputSample;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		//we then ignore this!
		
		quadratic -= 1;
		if (quadratic < 0)
		{
			position += 1;		
			quadratic = position * position;
			quadratic = quadratic % 170003; //% is C++ mod operator
			quadratic *= quadratic;
			quadratic = quadratic % 17011; //% is C++ mod operator
			quadratic *= quadratic;
			quadratic = quadratic % 1709; //% is C++ mod operator
			quadratic *= quadratic;
			quadratic = quadratic % dcut; //% is C++ mod operator
			quadratic *= quadratic;
			quadratic = quadratic % lowcut;
			//sets density of the centering force
			if (noiseA < 0) {flip = true;}
			else {flip = false;}
			//every time we come here, we force the random walk to be
			//toward the center of the waveform. Without this,
			//it's a pure random walk that will generate DC.
		}
		
		if (flip) noiseA += (double(fpd)/UINT32_MAX);
		else noiseA -= (double(fpd)/UINT32_MAX);
		//here's the guts of the random walk
	
		if (filterflip)
			{noiseB *= invcutoff; noiseB += (noiseA*cutoff); inputSample = noiseB;}
		else
			{noiseC *= invcutoff; noiseC += (noiseA*cutoff); inputSample = noiseC;}
		//now we have the output of the filter as inputSample.
		//this filter is shallower than a straight IIR: it's interleaved
	
		
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
		inputSample += (b[9] * f[9]);
		
		inputSample *= 0.1;
		inputSample *= invcutoff;
		inputSample /= rumbletrim;
		
		flip = !flip;
		filterflip = !filterflip;
		
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

