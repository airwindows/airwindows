/*
*	File:		Floor.cpp
*	
*	Version:	1.0
* 
*	Created:	12/8/10
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
	Floor.h
	
=============================================================================*/
#include "Floor.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Floor)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Floor::Floor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Floor::Floor(AudioUnit component)
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
//	Floor::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Floor::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Floor::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Floor::GetParameterInfo(AudioUnitScope		inScope,
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
//	Floor::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Floor::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Floor::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Floor::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Floor::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Floor::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____FloorEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Floor::FloorKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Floor::FloorKernel::Reset()
{
	flip = false;
	iirSample1A = 0.0;
	iirSample1B = 0.0;
	iirSample1C = 0.0;
	iirSample1D = 0.0;
	iirSample1E = 0.0;
	iirSample2A = 0.0;
	iirSample2B = 0.0;
	iirSample2C = 0.0;
	iirSample2D = 0.0;
	iirSample2E = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Floor::FloorKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Floor::FloorKernel::Process(	const Float32 	*inSourceP,
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

	Float64 setting = pow(GetParameter( kParam_One ),2);
	Float64 iirAmount = (setting/4.0)/overallscale;
	Float64 tight = -1.0;
	Float64 gaintrim = 1.0 + (setting/4.0);
	Float64 offset;
	Float64 lows;
	Float64 density = GetParameter( kParam_Two );
	Float64 bridgerectifier;
	Float64 temp;
	iirAmount += (iirAmount * tight * tight);
	tight /=  3.0;
	if (iirAmount <= 0.0) iirAmount = 0.0;
	if (iirAmount > 1.0) iirAmount = 1.0;
	Float64 wet = GetParameter( kParam_Three );
	Float64 dry = 1.0-wet;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSample)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSample))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSample1A = (iirSample1A * (1 - (offset * iirAmount))) + (inputSample * (offset * iirAmount));
		lows = iirSample1A;
		
		inputSample -= lows;
		
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		
		inputSample += lows;
		inputSample *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSample)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSample))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		
		iirSample1B = (iirSample1B * (1 - (offset * iirAmount))) + (inputSample * (offset * iirAmount));
		lows = iirSample1B;
		
		inputSample -= lows;
		
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		
		inputSample += lows;
		inputSample *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSample)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSample))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		
		iirSample1C = (iirSample1C * (1 - (offset * iirAmount))) + (inputSample * (offset * iirAmount));
		lows = iirSample1C;
		
		inputSample -= lows;
		
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		
		inputSample += lows;
		inputSample *= gaintrim;
		
		if (tight > 0) offset = (1 - tight) + (fabs(inputSample)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSample))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		
		iirSample1D = (iirSample1D * (1 - (offset * iirAmount))) + (inputSample * (offset * iirAmount));
		lows = iirSample1D;
		
		inputSample -= lows;
		
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		
		inputSample += lows;
		inputSample *= gaintrim;

		if (tight > 0) offset = (1 - tight) + (fabs(inputSample)*tight);
		else offset = (1 + tight) + ((1-fabs(inputSample))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		
		iirSample1E = (iirSample1E * (1 - (offset * iirAmount))) + (inputSample * (offset * iirAmount));
		lows = iirSample1E;
		
		inputSample -= lows;
		
		temp = lows;
		if (lows < 0) {lows = -sin(-lows*1.5707963267949);}
		if (lows > 0) {lows = sin(lows*1.5707963267949);}
		lows -= temp;
		
		inputSample += lows;
		inputSample *= gaintrim;

		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
		bridgerectifier = fabs(inputSample)*1.57079633;
		bridgerectifier = sin(bridgerectifier)*1.57079633;
		bridgerectifier = (fabs(inputSample)*(1-density))+(bridgerectifier*density);
		bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0) inputSample = (inputSample*(1-density))+(bridgerectifier*density);
		else inputSample = (inputSample*(1-density))-(bridgerectifier*density);
		//drive section
		
		if (wet < 1.0) inputSample = (drySample * dry)+(inputSample*wet);

		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

