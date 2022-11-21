/*
*	File:		UnBox.cpp
*	
*	Version:	1.0
* 
*	Created:	8/31/18
*	
*	Copyright:  Copyright © 2018 Airwindows, Airwindows uses the MIT license
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
	UnBox.cpp
	
=============================================================================*/
#include "UnBox.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, UnBox)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UnBox::UnBox
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
UnBox::UnBox(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UnBox::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			UnBox::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UnBox::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			UnBox::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.maxValue = 2.0;
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
                outParameterInfo.maxValue = 2.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
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
//	UnBox::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			UnBox::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UnBox::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			UnBox::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	UnBox::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult UnBox::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____UnBoxEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UnBox::UnBoxKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		UnBox::UnBoxKernel::Reset()
{
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
	for(int count = 0; count < 5; count++) {a[count] = 0.0; b[count] = 0.0; e[count] = 0.0;}
	for(int count = 0; count < 11; count++) {c[count] = 0.0; f[count] = 0.0;}
	iirSampleA = 0.0;
	iirSampleB = 0.0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	UnBox::UnBoxKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		UnBox::UnBoxKernel::Process(	const Float32 	*inSourceP,
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

	Float64 input = GetParameter( kParam_One );
	Float64 unbox = GetParameter( kParam_Two )+1.0;
	unbox *= unbox; //let's get some more gain into this
	Float64 iirAmount = (unbox*0.00052)/overallscale;
	Float64 output = GetParameter( kParam_Three );
	
	Float64 treble = unbox; //averaging taps 1-4
	Float64 gain = treble;
	if (gain > 1.0) {e[0] = 1.0; gain -= 1.0;} else {e[0] = gain; gain = 0.0;}
	if (gain > 1.0) {e[1] = 1.0; gain -= 1.0;} else {e[1] = gain; gain = 0.0;}
	if (gain > 1.0) {e[2] = 1.0; gain -= 1.0;} else {e[2] = gain; gain = 0.0;}
	if (gain > 1.0) {e[3] = 1.0; gain -= 1.0;} else {e[3] = gain; gain = 0.0;}
	if (gain > 1.0) {e[4] = 1.0; gain -= 1.0;} else {e[4] = gain; gain = 0.0;}
	//there, now we have a neat little moving average with remainders
	if (treble < 1.0) treble = 1.0;
	e[0] /= treble;
	e[1] /= treble;
	e[2] /= treble;
	e[3] /= treble;
	e[4] /= treble;
	//and now it's neatly scaled, too
	
	treble = unbox*2.0; //averaging taps 1-8
	gain = treble;
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
	if (treble < 1.0) treble = 1.0;
	f[0] /= treble;
	f[1] /= treble;
	f[2] /= treble;
	f[3] /= treble;
	f[4] /= treble;
	f[5] /= treble;
	f[6] /= treble;
	f[7] /= treble;
	f[8] /= treble;
	f[9] /= treble;
	//and now it's neatly scaled, too
	
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		
		if (input != 1.0) inputSample *= input;

		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;

		a[4] = a[3]; a[3] = a[2]; a[2] = a[1];
		a[1] = a[0]; a[0] = inputSample;
		inputSample *= e[0];
		inputSample += (a[1] * e[1]);
		inputSample += (a[2] * e[2]);
		inputSample += (a[3] * e[3]);
		inputSample += (a[4] * e[4]);
		//this is now an average of inputSample
		
		b[4] = b[3]; b[3] = b[2]; b[2] = b[1];
		b[1] = b[0]; b[0] = inputSample;
		inputSample *= e[0];
		inputSample += (b[1] * e[1]);
		inputSample += (b[2] * e[2]);
		inputSample += (b[3] * e[3]);
		inputSample += (b[4] * e[4]);
		//this is now an average of an average of inputSample. Two poles
		
		inputSample *= unbox;
		//clip to 1.2533141373155 to reach maximum output
		if (inputSample > 1.2533141373155) inputSample = 1.2533141373155;
		if (inputSample < -1.2533141373155) inputSample = -1.2533141373155;
		inputSample = sin(inputSample * fabs(inputSample)) / ((fabs(inputSample) == 0.0) ?1:fabs(inputSample));
		inputSample /= unbox;	
		//now we have a distorted inputSample at the correct volume relative to drySample
		
		double accumulatorSample = (drySample - inputSample);
		c[9] = c[8]; c[8] = c[7]; c[7] = c[6]; c[6] = c[5];
		c[5] = c[4]; c[4] = c[3]; c[3] = c[2]; c[2] = c[1];
		c[1] = c[0]; c[0] = accumulatorSample;
		accumulatorSample *= f[0];
		accumulatorSample += (c[1] * f[1]);
		accumulatorSample += (c[2] * f[2]);
		accumulatorSample += (c[3] * f[3]);
		accumulatorSample += (c[4] * f[4]);
		accumulatorSample += (c[5] * f[5]);
		accumulatorSample += (c[6] * f[6]);
		accumulatorSample += (c[7] * f[7]);
		accumulatorSample += (c[8] * f[8]);
		accumulatorSample += (c[9] * f[9]);
		//this is now an average of all the recent variances from dry
		
		iirSampleA = (iirSampleA * (1 - iirAmount)) + (accumulatorSample * iirAmount);
		accumulatorSample -= iirSampleA;
		//two poles of IIR
		iirSampleB = (iirSampleB * (1 - iirAmount)) + (accumulatorSample * iirAmount);
		accumulatorSample -= iirSampleB;
		//highpass section
		//this is now a highpassed average of all the recent variances from dry

		inputSample = drySample - accumulatorSample;
		//we apply it as one operation, to get the result.
		
		if (output != 1.0) inputSample *= output;
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		//end 32 bit floating point dither

		*destP = inputSample;
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

