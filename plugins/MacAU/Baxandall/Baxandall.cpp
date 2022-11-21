/*
*	File:		Baxandall.cpp
*	
*	Version:	1.0
* 
*	Created:	12/22/19
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
	Baxandall.cpp
	
=============================================================================*/
#include "Baxandall.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Baxandall)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Baxandall::Baxandall
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Baxandall::Baxandall(AudioUnit component)
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
//	Baxandall::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Baxandall::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Baxandall::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Baxandall::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = -15.0;
                outParameterInfo.maxValue = 15.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -15.0;
                outParameterInfo.maxValue = 15.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -15.0;
                outParameterInfo.maxValue = 15.0;
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
//	Baxandall::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Baxandall::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Baxandall::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Baxandall::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Baxandall::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Baxandall::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____BaxandallEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Baxandall::BaxandallKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Baxandall::BaxandallKernel::Reset()
{
	for (int x = 0; x < 9; x++) {
		trebleA[x] = 0.0;
		trebleB[x] = 0.0;
		bassA[x] = 0.0;
		bassB[x] = 0.0;
	}
	flip = false;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Baxandall::BaxandallKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Baxandall::BaxandallKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	Float64 trebleGain = pow(10.0,GetParameter( kParam_One )/20.0);
	Float64 trebleFreq = (4410.0*trebleGain)/GetSampleRate();
	if (trebleFreq > 0.45) trebleFreq = 0.45;
	trebleA[0] = trebleB[0] = trebleFreq;
	Float64 bassGain = pow(10.0,GetParameter( kParam_Two )/20.0);
	Float64 bassFreq = pow(10.0,-GetParameter( kParam_Two )/20.0);
	bassFreq = (8820.0*bassFreq)/GetSampleRate();
	if (bassFreq > 0.45) bassFreq = 0.45;
	bassA[0] = bassB[0] = bassFreq;
    trebleA[1] = trebleB[1] = 0.4;
    bassA[1] = bassB[1] = 0.2;
	Float64 output = pow(10.0,GetParameter( kParam_Three )/20.0);
	
	double K = tan(M_PI * trebleA[0]);
	double norm = 1.0 / (1.0 + K / trebleA[1] + K * K);
	trebleB[2] = trebleA[2] = K * K * norm;
	trebleB[3] = trebleA[3] = 2.0 * trebleA[2];
	trebleB[4] = trebleA[4] = trebleA[2];
	trebleB[5] = trebleA[5] = 2.0 * (K * K - 1.0) * norm;
	trebleB[6] = trebleA[6] = (1.0 - K / trebleA[1] + K * K) * norm;
	
	K = tan(M_PI * bassA[0]);
	norm = 1.0 / (1.0 + K / bassA[1] + K * K);
	bassB[2] = bassA[2] = K * K * norm;
	bassB[3] = bassA[3] = 2.0 * bassA[2];
	bassB[4] = bassA[4] = bassA[2];
	bassB[5] = bassA[5] = 2.0 * (K * K - 1.0) * norm;
	bassB[6] = bassA[6] = (1.0 - K / bassA[1] + K * K) * norm;
		
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		if (output != 1.0) {
			inputSample *= output;
		}//gain trim in front of plugin, in case Console stage clips
		
		inputSample = sin(inputSample);
		//encode Console5: good cleanness
		
		double trebleSample;
		double bassSample;

		if (flip)
		{
			trebleSample = (inputSample * trebleA[2]) + trebleA[7];
			trebleA[7] = (inputSample * trebleA[3]) - (trebleSample * trebleA[5]) + trebleA[8];
			trebleA[8] = (inputSample * trebleA[4]) - (trebleSample * trebleA[6]);
			trebleSample = inputSample - trebleSample;
			
			bassSample = (inputSample * bassA[2]) + bassA[7];
			bassA[7] = (inputSample * bassA[3]) - (bassSample * bassA[5]) + bassA[8];
			bassA[8] = (inputSample * bassA[4]) - (bassSample * bassA[6]);
		}
		else
		{
			trebleSample = (inputSample * trebleB[2]) + trebleB[7];
			trebleB[7] = (inputSample * trebleB[3]) - (trebleSample * trebleB[5]) + trebleB[8];
			trebleB[8] = (inputSample * trebleB[4]) - (trebleSample * trebleB[6]);
			trebleSample = inputSample - trebleSample;
			
			bassSample = (inputSample * bassB[2]) + bassB[7];
			bassB[7] = (inputSample * bassB[3]) - (bassSample * bassB[5]) + bassB[8];
			bassB[8] = (inputSample * bassB[4]) - (bassSample * bassB[6]);
		}
		flip = !flip;
		
		trebleSample *= trebleGain;
		bassSample *= bassGain;
		inputSample = bassSample + trebleSample; //interleaved biquad

		
		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
		//without this, you can get a NaN condition where it spits out DC offset at full blast!
		inputSample = asin(inputSample);
		//amplitude aspect
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

