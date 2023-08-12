/*
*	File:		Inflamer.cpp
*	
*	Version:	1.0
* 
*	Created:	8/7/23
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
	Inflamer.cpp
	
=============================================================================*/
#include "Inflamer.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Inflamer)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Inflamer::Inflamer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Inflamer::Inflamer(AudioUnit component)
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
//	Inflamer::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Inflamer::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Inflamer::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Inflamer::GetParameterInfo(AudioUnitScope		inScope,
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
//	Inflamer::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Inflamer::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Inflamer::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Inflamer::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Inflamer::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Inflamer::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____InflamerEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Inflamer::InflamerKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Inflamer::InflamerKernel::Reset()
{
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Inflamer::InflamerKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Inflamer::InflamerKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	double gainControl = (GetParameter( kParam_One )*0.5)+0.05; //0.0 to 1.0
	int gainBits = 20; //start beyond maximum attenuation
	if (gainControl > 0.0) gainBits = floor(1.0 / gainControl)-2;
	if (gainBits < -2) gainBits = -2; if (gainBits > 17) gainBits = 17;
	double gain = 1.0;
	switch (gainBits)
	{
		case 17: gain = 0.0; break;
		case 16: gain = 0.0000152587890625; break;
		case 15: gain = 0.000030517578125; break;
		case 14: gain = 0.00006103515625; break;
		case 13: gain = 0.0001220703125; break;
		case 12: gain = 0.000244140625; break;
		case 11: gain = 0.00048828125; break;
		case 10: gain = 0.0009765625; break;
		case 9: gain = 0.001953125; break;
		case 8: gain = 0.00390625; break;
		case 7: gain = 0.0078125; break;
		case 6: gain = 0.015625; break;
		case 5: gain = 0.03125; break;
		case 4: gain = 0.0625; break;
		case 3: gain = 0.125; break;
		case 2: gain = 0.25; break;
		case 1: gain = 0.5; break;
		case 0: gain = 1.0; break;
		case -1: gain = 2.0; break;
		case -2: gain = 4.0; break;
	} //now we have our input trim
	
	int bitshiftL = 1;
	int bitshiftR = 1;
	double panControl = (GetParameter( kParam_Two )*2.0)-1.0; //-1.0 to 1.0
	double panAttenuation = (1.0-fabs(panControl));
	int panBits = 20; //start centered
	if (panAttenuation > 0.0) panBits = floor(1.0 / panAttenuation);
	if (panControl > 0.25) bitshiftL += panBits;
	if (panControl < -0.25) bitshiftR += panBits;
	if (bitshiftL < -2) bitshiftL = -2; if (bitshiftL > 17) bitshiftL = 17;
	if (bitshiftR < -2) bitshiftR = -2; if (bitshiftR > 17) bitshiftR = 17;
	double negacurve = 0.5;
	double curve = 0.5;
	switch (bitshiftL)
	{
		case 17: negacurve = 0.0; break;
		case 16: negacurve = 0.0000152587890625; break;
		case 15: negacurve = 0.000030517578125; break;
		case 14: negacurve = 0.00006103515625; break;
		case 13: negacurve = 0.0001220703125; break;
		case 12: negacurve = 0.000244140625; break;
		case 11: negacurve = 0.00048828125; break;
		case 10: negacurve = 0.0009765625; break;
		case 9: negacurve = 0.001953125; break;
		case 8: negacurve = 0.00390625; break;
		case 7: negacurve = 0.0078125; break;
		case 6: negacurve = 0.015625; break;
		case 5: negacurve = 0.03125; break;
		case 4: negacurve = 0.0625; break;
		case 3: negacurve = 0.125; break;
		case 2: negacurve = 0.25; break;
		case 1: negacurve = 0.5; break;
		case 0: negacurve = 1.0; break;
		case -1: negacurve = 2.0; break;
		case -2: negacurve = 4.0; break;
	}
	switch (bitshiftR)
	{
		case 17: curve = 0.0; break;
		case 16: curve = 0.0000152587890625; break;
		case 15: curve = 0.000030517578125; break;
		case 14: curve = 0.00006103515625; break;
		case 13: curve = 0.0001220703125; break;
		case 12: curve = 0.000244140625; break;
		case 11: curve = 0.00048828125; break;
		case 10: curve = 0.0009765625; break;
		case 9: curve = 0.001953125; break;
		case 8: curve = 0.00390625; break;
		case 7: curve = 0.0078125; break;
		case 6: curve = 0.015625; break;
		case 5: curve = 0.03125; break;
		case 4: curve = 0.0625; break;
		case 3: curve = 0.125; break;
		case 2: curve = 0.25; break;
		case 1: curve = 0.5; break;
		case 0: curve = 1.0; break;
		case -1: curve = 2.0; break;
		case -2: curve = 4.0; break;
	}
	double effectOut = GetParameter( kParam_Three );
	
	while (nSampleFrames-- > 0) {
		long double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		
		inputSample *= gain; //input trim
		double inflamerPlus = (inputSample*2.0)-pow(inputSample,2); //+50, very much just second harmonic
		double inflamerMinus = inputSample+(pow(inputSample,3)*0.25)-((pow(inputSample,2)+pow(inputSample,4))*0.0625); //-50
		inputSample = (inflamerPlus * curve) + (inflamerMinus * negacurve);
		inputSample = (inputSample * effectOut) + (drySample * (1.0-effectOut));
		
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

