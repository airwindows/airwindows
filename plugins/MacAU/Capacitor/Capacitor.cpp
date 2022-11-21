/*
*	File:		Capacitor.cpp
*	
*	Version:	1.0
* 
*	Created:	2/1/17
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
	Capacitor.cpp
	
=============================================================================*/
#include "Capacitor.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Capacitor)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Capacitor::Capacitor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Capacitor::Capacitor(AudioUnit component)
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
//	Capacitor::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Capacitor::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Capacitor::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Capacitor::GetParameterInfo(AudioUnitScope		inScope,
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
//	Capacitor::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Capacitor::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Capacitor::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Capacitor::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Capacitor::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Capacitor::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____CapacitorEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Capacitor::CapacitorKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Capacitor::CapacitorKernel::Reset()
{
	iirHighpassA = 0.0;
	iirHighpassB = 0.0;
	iirHighpassC = 0.0;
	iirHighpassD = 0.0;
	iirHighpassE = 0.0;
	iirHighpassF = 0.0;
	iirLowpassA = 0.0;
	iirLowpassB = 0.0;
	iirLowpassC = 0.0;
	iirLowpassD = 0.0;
	iirLowpassE = 0.0;
	iirLowpassF = 0.0;
	count = 0;
	lowpassChase = 0.0;
	highpassChase = 0.0;
	wetChase = 0.0;
	lowpassAmount = 1.0;
	highpassAmount = 0.0;
	wet = 1.0;
	lastLowpass = 1000.0;
	lastHighpass = 1000.0;
	lastWet = 1000.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Capacitor::CapacitorKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Capacitor::CapacitorKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	lowpassChase = pow(GetParameter( kParam_One ),2);
	highpassChase = pow(GetParameter( kParam_Two ),2);
	wetChase = GetParameter( kParam_Three );
	//should not scale with sample rate, because values reaching 1 are important
	//to its ability to bypass when set to max
	Float64 lowpassSpeed = 300 / (fabs( lastLowpass - lowpassChase)+1.0);
	Float64 highpassSpeed = 300 / (fabs( lastHighpass - highpassChase)+1.0);
	Float64 wetSpeed = 300 / (fabs( lastWet - wetChase)+1.0);
	lastLowpass = lowpassChase;
	lastHighpass = highpassChase;
	lastWet = wetChase;
	
	Float64 invLowpass;
	Float64 invHighpass;
	Float64 dry;

	double inputSample;
	Float32 drySample;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		drySample = inputSample;

		lowpassAmount = (((lowpassAmount*lowpassSpeed)+lowpassChase)/(lowpassSpeed + 1.0)); invLowpass = 1.0 - lowpassAmount;
		highpassAmount = (((highpassAmount*highpassSpeed)+highpassChase)/(highpassSpeed + 1.0)); invHighpass = 1.0 - highpassAmount;
		wet = (((wet*wetSpeed)+wetChase)/(wetSpeed+1.0)); dry = 1.0 - wet;
		
		count++; if (count > 5) count = 0; switch (count)
		{
			case 0:
				iirHighpassA = (iirHighpassA * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassA;
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirHighpassB = (iirHighpassB * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassB;
				iirLowpassB = (iirLowpassB * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassB;
				iirHighpassD = (iirHighpassD * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassD;
				iirLowpassD = (iirLowpassD * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassD;
				break;
			case 1:
				iirHighpassA = (iirHighpassA * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassA;
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirHighpassC = (iirHighpassC * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassC;
				iirLowpassC = (iirLowpassC * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassC;
				iirHighpassE = (iirHighpassE * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassE;
				iirLowpassE = (iirLowpassE * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassE;
				break;
			case 2:
				iirHighpassA = (iirHighpassA * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassA;
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirHighpassB = (iirHighpassB * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassB;
				iirLowpassB = (iirLowpassB * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassB;
				iirHighpassF = (iirHighpassF * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassF;
				iirLowpassF = (iirLowpassF * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassF;
				break;
			case 3:
				iirHighpassA = (iirHighpassA * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassA;
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirHighpassC = (iirHighpassC * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassC;
				iirLowpassC = (iirLowpassC * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassC;
				iirHighpassD = (iirHighpassD * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassD;
				iirLowpassD = (iirLowpassD * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassD;
				break;
			case 4:
				iirHighpassA = (iirHighpassA * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassA;
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirHighpassB = (iirHighpassB * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassB;
				iirLowpassB = (iirLowpassB * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassB;
				iirHighpassE = (iirHighpassE * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassE;
				iirLowpassE = (iirLowpassE * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassE;
				break;
			case 5:
				iirHighpassA = (iirHighpassA * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassA;
				iirLowpassA = (iirLowpassA * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassA;
				iirHighpassC = (iirHighpassC * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassC;
				iirLowpassC = (iirLowpassC * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassC;
				iirHighpassF = (iirHighpassF * invHighpass) + (inputSample * highpassAmount); inputSample -= iirHighpassF;
				iirLowpassF = (iirLowpassF * invLowpass) + (inputSample * lowpassAmount); inputSample = iirLowpassF;
				break;
		}
		//Highpass Filter chunk. This is three poles of IIR highpass, with a 'gearbox' that progressively
		//steepens the filter after minimizing artifacts.
		
		inputSample = (drySample * dry) + (inputSample * wet);

		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

