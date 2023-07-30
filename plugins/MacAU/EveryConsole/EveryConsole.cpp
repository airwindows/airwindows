/*
*	File:		EveryConsole.cpp
*	
*	Version:	1.0
* 
*	Created:	7/24/23
*	
*	Copyright:  Copyright © 2023 Airwindows, Airwindows uses the MIT license
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
	EveryConsole.cpp
	
=============================================================================*/
#include "EveryConsole.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(EveryConsole)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EveryConsole::EveryConsole
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EveryConsole::EveryConsole(AudioUnit component)
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
//	EveryConsole::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			EveryConsole::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_One)) //ID must be actual name of parameter identifier, not number
	{
		if (outStrings == NULL) return noErr;
		CFStringRef strings [] =
		{
			kMenuItem_RC,
			kMenuItem_RB,
			kMenuItem_SC,
			kMenuItem_SB,
			kMenuItem_6C,
			kMenuItem_6B,
			kMenuItem_7C,
			kMenuItem_7B,
			kMenuItem_BC,
			kMenuItem_BB,
			kMenuItem_ZC,
			kMenuItem_ZB,
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
//	EveryConsole::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			EveryConsole::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = kRC;
                outParameterInfo.maxValue = kZB;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 2.0;
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
//	EveryConsole::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			EveryConsole::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EveryConsole::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			EveryConsole::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	EveryConsole::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult EveryConsole::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____EveryConsoleEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EveryConsole::EveryConsoleKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		EveryConsole::EveryConsoleKernel::Reset()
{
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	EveryConsole::EveryConsoleKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		EveryConsole::EveryConsoleKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	double half = 0.0;
	double falf = 0.0;
	int console = (int) GetParameter( kParam_One );
	Float64 inTrim = GetParameter( kParam_Two ); //0-2
	Float64 outTrim = GetParameter( kParam_Three );
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		if (inTrim != 1.0) {
			inputSample *= inTrim;
		}

		switch (console)
		{
			case kRC:
				half = inputSample * 0.83;
				falf = fabs(half);
				half *= falf;
				half *= falf;
				inputSample -= half;
				break; //original ConsoleChannel, before sin/asin
				
			case kRB:
				half = inputSample * 0.885;
				falf = fabs(half);
				half *= falf;
				half *= falf;
				inputSample += half;
				break; //original ConsoleBuss, before sin/asin
				
			case kSC:
				if (inputSample > M_PI_2) inputSample = M_PI_2;
				if (inputSample < -M_PI_2) inputSample = -M_PI_2;
				//clip to max sine without any wavefolding
				inputSample = sin(inputSample);
				break; //sin() function ConsoleChannel
				
			case kSB:
				if (inputSample > 1.0) inputSample = 1.0;
				if (inputSample < -1.0) inputSample = -1.0;
				//without this, you can get a NaN condition where it spits out DC offset at full blast!
				inputSample = asin(inputSample);
				break; //sin() function ConsoleBuss
				
			case k6C:
				//encode/decode courtesy of torridgristle under the MIT license
				if (inputSample > 1.0) inputSample= 1.0;
				else if (inputSample > 0.0) inputSample = 1.0 - pow(1.0-inputSample,2.0);
				if (inputSample < -1.0) inputSample = -1.0;
				else if (inputSample < 0.0) inputSample = -1.0 + pow(1.0+inputSample,2.0);
				//Inverse Square 1-(1-x)^2 and 1-(1-x)^0.5 for Console6Channel
				break; //crude sine. Note that because modern processors love math more than extra variables, this is optimized
				
			case k6B:
				//encode/decode courtesy of torridgristle under the MIT license
				if (inputSample > 1.0) inputSample= 1.0;
				else if (inputSample > 0.0) inputSample = 1.0 - pow(1.0-inputSample,0.5);
				if (inputSample < -1.0) inputSample = -1.0;
				else if (inputSample < 0.0) inputSample = -1.0 + pow(1.0+inputSample,0.5);
				//Inverse Square 1-(1-x)^2 and 1-(1-x)^0.5 for Console6Buss
				break; //crude arcsine. Note that because modern processors love math more than extra variables, this is optimized
				
			case k7C:
				if (inputSample > 1.097) inputSample = 1.097;
				if (inputSample < -1.097) inputSample = -1.097;
				inputSample = ((sin(inputSample*fabs(inputSample))/((fabs(inputSample) == 0.0) ?1:fabs(inputSample)))*0.8)+(sin(inputSample)*0.2);
				//this is a version of Spiral blended 80/20 with regular Density.
				//It's blending between two different harmonics in the overtones of the algorithm
				break; //Console7Channel
				
			case k7B:
				if (inputSample > 1.0) inputSample = 1.0;
				if (inputSample < -1.0) inputSample = -1.0;
				inputSample = ((asin(inputSample*fabs(inputSample))/((fabs(inputSample) == 0.0) ?1:fabs(inputSample)))*0.618033988749894848204586)+(asin(inputSample)*0.381966011250105);
				//this is an asin version of Spiral blended with regular asin ConsoleBuss.
				//It's blending between two different harmonics in the overtones of the algorithm.
				break; //Console7Buss
				
			case kBC:
				inputSample += ((pow(inputSample,5)/128.0) + (pow(inputSample,9)/262144.0)) - ((pow(inputSample,3)/8.0) + (pow(inputSample,7)/4096.0));
				break; //crude sine. Note that because modern processors love math more than extra variables, this is optimized
				
			case kBB:
				inputSample += (pow(inputSample,3)/4.0)+(pow(inputSample,5)/8.0)+(pow(inputSample,7)/16.0)+(pow(inputSample,9)/32.0);
				break; //crude arcsine. Note that because modern processors love math more than extra variables, this is optimized
				
			case kZC:
				if (inputSample > 1.4137166941154) inputSample = 1.4137166941154;
				if (inputSample < -1.4137166941154) inputSample = -1.4137166941154;
				if (inputSample > 0.0) inputSample = (inputSample/2.0)*(2.8274333882308-inputSample);
				else inputSample = -(inputSample/-2.0)*(2.8274333882308+inputSample);
				break; //ConsoleZeroChannel
				
			case kZB:
				if (inputSample > 2.8) inputSample = 2.8;
				if (inputSample < -2.8) inputSample = -2.8;
				if (inputSample > 0.0) inputSample = (inputSample*2.0)/(3.0-inputSample);
				else inputSample = -(inputSample*-2.0)/(3.0+inputSample);
				break; //ConsoleZeroBuss
		}
		
		if (outTrim != 1.0) {
			inputSample *= outTrim;
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

