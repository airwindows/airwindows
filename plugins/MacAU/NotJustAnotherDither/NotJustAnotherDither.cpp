/*
*	File:		NotJustAnotherDither.cpp
*	
*	Version:	1.0
* 
*	Created:	1/31/17
*	
*	Copyright:  Copyright © 2017 Airwindows, Airwindows uses the MIT license
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
	NotJustAnotherDither.cpp
	
=============================================================================*/
#include "NotJustAnotherDither.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(NotJustAnotherDither)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NotJustAnotherDither::NotJustAnotherDither
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NotJustAnotherDither::NotJustAnotherDither(AudioUnit component)
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
//	NotJustAnotherDither::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			NotJustAnotherDither::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
	if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_One)) //ID must be actual name of parameter identifier, not number
	{
		if (outStrings == NULL) return noErr;
		CFStringRef strings [] =
		{
			kMenuItem_CD,
			kMenuItem_HD,
		};
		*outStrings = CFArrayCreate (
									 NULL,
									 (const void **) strings,
									 (sizeof (strings) / sizeof (strings [0])),
									 NULL
									 );
		return noErr;
	}
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NotJustAnotherDither::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			NotJustAnotherDither::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = kCD;
                outParameterInfo.maxValue = kHD;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
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
//	NotJustAnotherDither::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			NotJustAnotherDither::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NotJustAnotherDither::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			NotJustAnotherDither::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	NotJustAnotherDither::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult NotJustAnotherDither::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____NotJustAnotherDitherEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NotJustAnotherDither::NotJustAnotherDitherKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		NotJustAnotherDither::NotJustAnotherDitherKernel::Reset()
{
	byn[0] = 1000.0;
	byn[1] = 301.0;
	byn[2] = 176.0;
	byn[3] = 125.0;
	byn[4] = 97.0;
	byn[5] = 79.0;
	byn[6] = 67.0;
	byn[7] = 58.0;
	byn[8] = 51.0;
	byn[9] = 46.0;
	byn[10] = 1000.0;
	noiseShaping = 0.0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NotJustAnotherDither::NotJustAnotherDitherKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		NotJustAnotherDither::NotJustAnotherDitherKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;	
	
	bool highres = false;
	if (GetParameter( kParam_One ) == 1) highres = true;
	Float32 scaleFactor;
	if (highres) scaleFactor = 8388608.0;
	else scaleFactor = 32768.0;
	Float32 derez = GetParameter( kParam_Two );
	if (derez > 0.0) scaleFactor *= pow(1.0-derez,6);
	if (scaleFactor < 0.0001) scaleFactor = 0.0001;
	Float32 outScale = scaleFactor;
	if (outScale < 8.0) outScale = 8.0;
	
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		
		inputSample *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		bool cutbins; cutbins = false;
		double drySample = inputSample;
		inputSample -= noiseShaping;
		
		double benfordize; benfordize = floor(inputSample);
		while (benfordize >= 1.0) benfordize /= 10;
		while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;
		int hotbinA; hotbinA = floor(benfordize);
		//hotbin becomes the Benford bin value for this number floored
		double totalA; totalA = 0;
		if ((hotbinA > 0) && (hotbinA < 10))
		{
			byn[hotbinA] += 1; if (byn[hotbinA] > 982) cutbins = true;
			totalA += (301-byn[1]); totalA += (176-byn[2]); totalA += (125-byn[3]);
			totalA += (97-byn[4]); totalA += (79-byn[5]); totalA += (67-byn[6]);
			totalA += (58-byn[7]); totalA += (51-byn[8]); totalA += (46-byn[9]);
			byn[hotbinA] -= 1;
		} else hotbinA = 10;
		//produce total number- smaller is closer to Benford real
		
		benfordize = ceil(inputSample);
		while (benfordize >= 1.0) benfordize /= 10;
		while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;
		int hotbinB; hotbinB = floor(benfordize);
		//hotbin becomes the Benford bin value for this number ceiled
		double totalB; totalB = 0;
		if ((hotbinB > 0) && (hotbinB < 10))
		{
			byn[hotbinB] += 1; if (byn[hotbinB] > 982) cutbins = true;
			totalB += (301-byn[1]); totalB += (176-byn[2]); totalB += (125-byn[3]);
			totalB += (97-byn[4]); totalB += (79-byn[5]); totalB += (67-byn[6]);
			totalB += (58-byn[7]); totalB += (51-byn[8]); totalB += (46-byn[9]);
			byn[hotbinB] -= 1;
		} else hotbinB = 10;
		//produce total number- smaller is closer to Benford real
		
		double outputSample;
		if (totalA < totalB) {byn[hotbinA] += 1; outputSample = floor(inputSample);}
		else {byn[hotbinB] += 1; outputSample = floor(inputSample+1);}
		//assign the relevant one to the delay line
		//and floor/ceil signal accordingly
		if (cutbins) {
			byn[1] *= 0.99; byn[2] *= 0.99; byn[3] *= 0.99; byn[4] *= 0.99; byn[5] *= 0.99; 
			byn[6] *= 0.99; byn[7] *= 0.99; byn[8] *= 0.99; byn[9] *= 0.99; byn[10] *= 0.99; 
		}
		noiseShaping += outputSample - drySample;
		if (noiseShaping > fabs(inputSample)) noiseShaping = fabs(inputSample);
		if (noiseShaping < -fabs(inputSample)) noiseShaping = -fabs(inputSample);		
		
		inputSample = outputSample / outScale;
		
		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
				
		*destP = inputSample;
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

