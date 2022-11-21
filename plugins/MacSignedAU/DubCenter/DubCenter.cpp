/*
*	File:		DubCenter.cpp
*	
*	Version:	1.0
* 
*	Created:	10/14/18
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
	DubCenter.cpp
	
=============================================================================*/
#include "DubCenter.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, DubCenter)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DubCenter::DubCenter
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DubCenter::DubCenter(AudioUnit component)
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
	SetParameter(kParam_Seven, kDefaultValue_ParamSeven );
	SetParameter(kParam_Eight, kDefaultValue_ParamEight );
	SetParameter(kParam_Nine, kDefaultValue_ParamNine );
	SetParameter(kParam_Ten, kDefaultValue_ParamTen );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DubCenter::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DubCenter::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DubCenter::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DubCenter::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = -1.0;
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
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSix;
                break;
			case kParam_Seven:
                AUBase::FillInParameterName (outParameterInfo, kParameterSevenName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSeven;
                break;
			case kParam_Eight:
                AUBase::FillInParameterName (outParameterInfo, kParameterEightName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamEight;
                break;
			case kParam_Nine:
                AUBase::FillInParameterName (outParameterInfo, kParameterNineName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamNine;
                break;
			case kParam_Ten:
                AUBase::FillInParameterName (outParameterInfo, kParameterTenName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTen;
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
//	DubCenter::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DubCenter::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 DubCenter::SupportedNumChannels(const AUChannelInfo ** outInfo)
{
	if (outInfo != NULL)
	{
		static AUChannelInfo info;
		info.inChannels = 2;
		info.outChannels = 2;
		*outInfo = &info;
	}

	return 1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DubCenter::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DubCenter::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	DubCenter::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult DubCenter::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____DubCenterEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DubCenter::DubCenterKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		DubCenter::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	WasNegative = false;
	SubOctave = false;
	flip = false;
	bflip = 0;
	iirDriveSampleAL = 0.0;
	iirDriveSampleBL = 0.0;
	iirDriveSampleCL = 0.0;
	iirDriveSampleDL = 0.0;
	iirDriveSampleEL = 0.0;
	iirDriveSampleFL = 0.0;
	iirDriveSampleAR = 0.0;
	iirDriveSampleBR = 0.0;
	iirDriveSampleCR = 0.0;
	iirDriveSampleDR = 0.0;
	iirDriveSampleER = 0.0;
	iirDriveSampleFR = 0.0;
	
	iirHeadBumpA = 0.0;
	iirHeadBumpB = 0.0;
	iirHeadBumpC = 0.0;
	
	iirSubBumpA = 0.0;
	iirSubBumpB = 0.0;
	iirSubBumpC = 0.0;
	
	lastHeadBump = 0.0;
	lastSubBump = 0.0;
	
	iirSampleA = 0.0;
	iirSampleB = 0.0;
	iirSampleC = 0.0;
	iirSampleD = 0.0;
	iirSampleE = 0.0;
	iirSampleF = 0.0;
	iirSampleG = 0.0;
	iirSampleH = 0.0;
	iirSampleI = 0.0;
	iirSampleJ = 0.0;
	iirSampleK = 0.0;
	iirSampleL = 0.0;
	iirSampleM = 0.0;
	iirSampleN = 0.0;
	iirSampleO = 0.0;
	iirSampleP = 0.0;
	iirSampleQ = 0.0;
	iirSampleR = 0.0;
	iirSampleS = 0.0;
	iirSampleT = 0.0;
	iirSampleU = 0.0;
	iirSampleV = 0.0;
	iirSampleW = 0.0;
	iirSampleX = 0.0;
	iirSampleY = 0.0;
	iirSampleZ = 0.0;	
	
	oscGate = 1.0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DubCenter::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		DubCenter::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
													const AudioBufferList & inBuffer,
                                                    AudioBufferList & outBuffer,
                                                    UInt32 			inFramesToProcess)
{
	Float32 * inputL = (Float32*)(inBuffer.mBuffers[0].mData);
	Float32 * inputR = (Float32*)(inBuffer.mBuffers[1].mData);
	Float32 * outputL = (Float32*)(outBuffer.mBuffers[0].mData);
	Float32 * outputR = (Float32*)(outBuffer.mBuffers[1].mData);
	UInt32 nSampleFrames = inFramesToProcess;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	Float64 driveone = pow(GetParameter( kParam_One )*3.0,2);
	Float64 driveoutput = GetParameter( kParam_Two );
	Float64 iirAmount = ((GetParameter( kParam_Three )*0.33)+0.1)/overallscale;
	Float64 ataLowpassL;
	Float64 ataLowpassR;
	Float64 ataLowpass;
	Float64 randy;
	Float64 invrandy;
	Float64 HeadBump = 0.0;
	Float64 BassGain = GetParameter( kParam_Four ) * 0.1;
	Float64 HeadBumpFreq = ((GetParameter( kParam_Five )*0.1)+0.0001)/overallscale;
	Float64 iirBmount = HeadBumpFreq/44.1;
	Float64 altBmount = 1.0 - iirBmount;
	Float64 BassOutGain = GetParameter( kParam_Six );
	Float64 SubBump = 0.0;
	Float64 SubGain = GetParameter( kParam_Seven ) * 0.1;
	Float64 SubBumpFreq = ((GetParameter( kParam_Eight )*0.1)+0.0001)/overallscale;
	Float64 iirCmount = SubBumpFreq/44.1;
	Float64 altCmount = 1.0 - iirCmount;
	Float64 SubOutGain = GetParameter( kParam_Nine );
	Float64 clamp = 0.0;
	Float64 out;
	Float64 fuzz = 0.111;
	Float64 wet = GetParameter( kParam_Ten );
	Float64 glitch = 0.60;
	Float64 tempSample;
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;

		// here's the plan.
		// Grind Boost
		// Grind Output Level
		// Bass Split Freq
		// Bass Drive
		// Bass Voicing
		// Bass Output Level
		// Sub Oct Drive
		// Sub Voicing
		// Sub Output Level
		// Dry/Wet
		
		oscGate += fabs((inputSampleL + inputSampleR) * 5.0);
		oscGate -= 0.001;
		if (oscGate > 1.0) oscGate = 1.0;
		if (oscGate < 0) oscGate = 0;
		//got a value that only goes down low when there's silence or near silence on input
		clamp = 1.0-oscGate;
		clamp *= 0.00001;
		//set up the thing to choke off oscillations- belt and suspenders affair
		
		
		if (flip)
		{
			tempSample = inputSampleL;
			iirDriveSampleAL = (iirDriveSampleAL * (1 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL -= iirDriveSampleAL;
			iirDriveSampleCL = (iirDriveSampleCL * (1 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL -= iirDriveSampleCL;
			iirDriveSampleEL = (iirDriveSampleEL * (1 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL -= iirDriveSampleEL;
			ataLowpassL = tempSample - inputSampleL;

			tempSample = inputSampleR;
			iirDriveSampleAR = (iirDriveSampleAR * (1 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR -= iirDriveSampleAR;
			iirDriveSampleCR = (iirDriveSampleCR * (1 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR -= iirDriveSampleCR;
			iirDriveSampleER = (iirDriveSampleER * (1 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR -= iirDriveSampleER;
			ataLowpassR = tempSample - inputSampleR;
		}
		else
		{
			tempSample = inputSampleL;
			iirDriveSampleBL = (iirDriveSampleBL * (1 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL -= iirDriveSampleBL;
			iirDriveSampleDL = (iirDriveSampleDL * (1 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL -= iirDriveSampleDL;
			iirDriveSampleFL = (iirDriveSampleFL * (1 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL -= iirDriveSampleFL;
			ataLowpassL = tempSample - inputSampleL;

			tempSample = inputSampleR;
			iirDriveSampleBR = (iirDriveSampleBR * (1 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR -= iirDriveSampleBR;
			iirDriveSampleDR = (iirDriveSampleDR * (1 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR -= iirDriveSampleDR;
			iirDriveSampleFR = (iirDriveSampleFR * (1 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR -= iirDriveSampleFR;
			ataLowpassR = tempSample - inputSampleR;
		}
		//highpass section
		
		if (inputSampleL > 1.0) {inputSampleL = 1.0;}
		if (inputSampleL < -1.0) {inputSampleL = -1.0;}
		if (inputSampleR > 1.0) {inputSampleR = 1.0;}
		if (inputSampleR < -1.0) {inputSampleR = -1.0;}
		
		out = driveone;
		while (out > glitch)
		{
			out -= glitch;
			inputSampleL -= (inputSampleL * (fabs(inputSampleL) * glitch) * (fabs(inputSampleL) * glitch) );
			inputSampleL *= (1.0+glitch);
		}
		//that's taken care of the really high gain stuff
		inputSampleL -= (inputSampleL * (fabs(inputSampleL) * out) * (fabs(inputSampleL) * out) );
		inputSampleL *= (1.0+out);
		
		
		out = driveone;
		while (out > glitch)
		{
			out -= glitch;
			inputSampleR -= (inputSampleR * (fabs(inputSampleR) * glitch) * (fabs(inputSampleR) * glitch) );
			inputSampleR *= (1.0+glitch);
		}
		//that's taken care of the really high gain stuff
		inputSampleR -= (inputSampleR * (fabs(inputSampleR) * out) * (fabs(inputSampleR) * out) );
		inputSampleR *= (1.0+out);
		
		ataLowpass = (ataLowpassL + ataLowpassR) / 2.0;
		
		if (ataLowpass > 0)
		{if (WasNegative){SubOctave = !SubOctave;} WasNegative = false;}
		else {WasNegative = true;}
		//set up polarities for sub-bass version
		randy = (double(fpdL)/UINT32_MAX)*fuzz; //0 to 1 the noise, may not be needed
		invrandy = (1.0-randy);
		randy /= 2.0;
		//set up the noise
		
		iirSampleA = (iirSampleA * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleA;
		iirSampleB = (iirSampleB * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleB;
		iirSampleC = (iirSampleC * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleC;
		iirSampleD = (iirSampleD * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleD;
		iirSampleE = (iirSampleE * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleE;
		iirSampleF = (iirSampleF * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleF;
		iirSampleG = (iirSampleG * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleG;
		iirSampleH = (iirSampleH * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleH;
		iirSampleI = (iirSampleI * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleI;
		iirSampleJ = (iirSampleJ * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleJ;
		iirSampleK = (iirSampleK * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleK;
		iirSampleL = (iirSampleL * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleL;
		iirSampleM = (iirSampleM * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleM;
		iirSampleN = (iirSampleN * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleN;
		iirSampleO = (iirSampleO * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleO;
		iirSampleP = (iirSampleP * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleP;
		iirSampleQ = (iirSampleQ * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleQ;
		iirSampleR = (iirSampleR * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleR;
		iirSampleS = (iirSampleS * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleS;
		iirSampleT = (iirSampleT * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleT;
		iirSampleU = (iirSampleU * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleU;
		iirSampleV = (iirSampleV * altBmount) + (ataLowpass * iirBmount); ataLowpass -= iirSampleV;
		
		switch (bflip)
		{
			case 1:				
				iirHeadBumpA += (ataLowpass * BassGain);
				iirHeadBumpA -= (iirHeadBumpA * iirHeadBumpA * iirHeadBumpA * HeadBumpFreq);
				iirHeadBumpA = (invrandy * iirHeadBumpA) + (randy * iirHeadBumpB) + (randy * iirHeadBumpC);
				if (iirHeadBumpA > 0) iirHeadBumpA -= clamp;
				if (iirHeadBumpA < 0) iirHeadBumpA += clamp;
				HeadBump = iirHeadBumpA;
				break;
			case 2:
				iirHeadBumpB += (ataLowpass * BassGain);
				iirHeadBumpB -= (iirHeadBumpB * iirHeadBumpB * iirHeadBumpB * HeadBumpFreq);
				iirHeadBumpB = (randy * iirHeadBumpA) + (invrandy * iirHeadBumpB) + (randy * iirHeadBumpC);
				if (iirHeadBumpB > 0) iirHeadBumpB -= clamp;
				if (iirHeadBumpB < 0) iirHeadBumpB += clamp;
				HeadBump = iirHeadBumpB;
				break;
			case 3:
				iirHeadBumpC += (ataLowpass * BassGain);
				iirHeadBumpC -= (iirHeadBumpC * iirHeadBumpC * iirHeadBumpC * HeadBumpFreq);
				iirHeadBumpC = (randy * iirHeadBumpA) + (randy * iirHeadBumpB) + (invrandy * iirHeadBumpC);
				if (iirHeadBumpC > 0) iirHeadBumpC -= clamp;
				if (iirHeadBumpC < 0) iirHeadBumpC += clamp;
				HeadBump = iirHeadBumpC;
				break;
		}
		
		iirSampleW = (iirSampleW * altBmount) + (HeadBump * iirBmount); HeadBump -= iirSampleW;
		iirSampleX = (iirSampleX * altBmount) + (HeadBump * iirBmount); HeadBump -= iirSampleX;
		
		SubBump = HeadBump;
		iirSampleY = (iirSampleY * altCmount) + (SubBump * iirCmount); SubBump -= iirSampleY;
		
		SubBump = fabs(SubBump);
		if (SubOctave == false) {SubBump = -SubBump;}
		
		switch (bflip)
		{
			case 1:				
				iirSubBumpA += (SubBump * SubGain);
				iirSubBumpA -= (iirSubBumpA * iirSubBumpA * iirSubBumpA * SubBumpFreq);
				iirSubBumpA = (invrandy * iirSubBumpA) + (randy * iirSubBumpB) + (randy * iirSubBumpC);
				if (iirSubBumpA > 0) iirSubBumpA -= clamp;
				if (iirSubBumpA < 0) iirSubBumpA += clamp;
				SubBump = iirSubBumpA;
				break;
			case 2:
				iirSubBumpB += (SubBump * SubGain);
				iirSubBumpB -= (iirSubBumpB * iirSubBumpB * iirSubBumpB * SubBumpFreq);
				iirSubBumpB = (randy * iirSubBumpA) + (invrandy * iirSubBumpB) + (randy * iirSubBumpC);
				if (iirSubBumpB > 0) iirSubBumpB -= clamp;
				if (iirSubBumpB < 0) iirSubBumpB += clamp;
				SubBump = iirSubBumpB;
				break;
			case 3:
				iirSubBumpC += (SubBump * SubGain);
				iirSubBumpC -= (iirSubBumpC * iirSubBumpC * iirSubBumpC * SubBumpFreq);
				iirSubBumpC = (randy * iirSubBumpA) + (randy * iirSubBumpB) + (invrandy * iirSubBumpC);
				if (iirSubBumpC > 0) iirSubBumpC -= clamp;
				if (iirSubBumpC < 0) iirSubBumpC += clamp;
				SubBump = iirSubBumpC;
				break;
		}
		
		iirSampleZ = (iirSampleZ * altCmount) + (SubBump * iirCmount); SubBump -= iirSampleZ;
		
		inputSampleL *= driveoutput; //start with the drive section then add lows and subs
		inputSampleR *= driveoutput; //start with the drive section then add lows and subs
		
		inputSampleL += ((HeadBump + lastHeadBump) * BassOutGain);
		inputSampleL += ((SubBump + lastSubBump) * SubOutGain);
		
		inputSampleR += ((HeadBump + lastHeadBump) * BassOutGain);
		inputSampleR += ((SubBump + lastSubBump) * SubOutGain);
		
		lastHeadBump = HeadBump;
		lastSubBump = SubBump;
		
		if (wet != 1.0) {
		 inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
		 inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
		flip = !flip;
		bflip++;
		if (bflip < 1 || bflip > 3) bflip = 1;
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		*outputL = inputSampleL;
		*outputR = inputSampleR;
		//direct stereo out
		
		inputL += 1;
		inputR += 1;
		outputL += 1;
		outputR += 1;
	}
	return noErr;
}

