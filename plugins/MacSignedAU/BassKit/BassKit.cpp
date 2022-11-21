/*
*	File:		BassKit.cpp
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
	BassKit.cpp
	
=============================================================================*/
#include "BassKit.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, BassKit)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassKit::BassKit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BassKit::BassKit(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassKit::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassKit::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassKit::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassKit::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
                break;
			case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFour;
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
//	BassKit::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassKit::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 BassKit::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	BassKit::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassKit::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	BassKit::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult BassKit::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____BassKitEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassKit::BassKitKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		BassKit::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	WasNegative = false;
	SubOctave = false;
	flip = false;
	bflip = 0;
	iirDriveSampleA = 0.0;
	iirDriveSampleB = 0.0;
	iirDriveSampleC = 0.0;
	iirDriveSampleD = 0.0;
	iirDriveSampleE = 0.0;
	iirDriveSampleF = 0.0;
	
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
//	BassKit::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		BassKit::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	
	Float64 ataLowpass;
	Float64 randy;
	Float64 invrandy;
	Float64 HeadBump = 0.0;
	Float64 BassGain = GetParameter( kParam_One ) * 0.1;
	Float64 HeadBumpFreq = ((GetParameter( kParam_Two )*0.1)+0.02)/overallscale;
	Float64 iirAmount = HeadBumpFreq/44.1;
	Float64 BassOutGain = GetParameter( kParam_Three )*fabs(GetParameter( kParam_Three ));
	Float64 SubBump = 0.0;
	Float64 SubOutGain = GetParameter( kParam_Four )*fabs(GetParameter( kParam_Four ))*4.0;
	Float64 clamp = 0.0;
	Float64 fuzz = 0.111;
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		ataLowpass = (inputSampleL + inputSampleR) / 2.0;
		iirDriveSampleA = (iirDriveSampleA * (1.0 - HeadBumpFreq)) + (ataLowpass * HeadBumpFreq); ataLowpass = iirDriveSampleA;
		iirDriveSampleB = (iirDriveSampleB * (1.0 - HeadBumpFreq)) + (ataLowpass * HeadBumpFreq); ataLowpass = iirDriveSampleB;

				
		oscGate += fabs(ataLowpass * 10.0);
		oscGate -= 0.001;
		if (oscGate > 1.0) oscGate = 1.0;
		if (oscGate < 0) oscGate = 0;
		//got a value that only goes down low when there's silence or near silence on input
		clamp = 1.0-oscGate;
		clamp *= 0.00001;
		//set up the thing to choke off oscillations- belt and suspenders affair
		
		if (ataLowpass > 0)
		{if (WasNegative){SubOctave = !SubOctave;} WasNegative = false;}
		else {WasNegative = true;}
		//set up polarities for sub-bass version
		randy = (double(fpdL)/UINT32_MAX)*fuzz; //0 to 1 the noise, may not be needed
		invrandy = (1.0-randy);
		randy /= 2.0;
		//set up the noise
		
		iirSampleA = (iirSampleA * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleA;
		iirSampleB = (iirSampleB * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleB;
		iirSampleC = (iirSampleC * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleC;
		iirSampleD = (iirSampleD * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleD;
		iirSampleE = (iirSampleE * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleE;
		iirSampleF = (iirSampleF * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleF;
		iirSampleG = (iirSampleG * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleG;
		iirSampleH = (iirSampleH * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleH;
		iirSampleI = (iirSampleI * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleI;
		iirSampleJ = (iirSampleJ * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleJ;
		iirSampleK = (iirSampleK * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleK;
		iirSampleL = (iirSampleL * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleL;
		iirSampleM = (iirSampleM * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleM;
		iirSampleN = (iirSampleN * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleN;
		iirSampleO = (iirSampleO * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleO;
		iirSampleP = (iirSampleP * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleP;
		iirSampleQ = (iirSampleQ * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleQ;
		iirSampleR = (iirSampleR * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleR;
		iirSampleS = (iirSampleS * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleS;
		iirSampleT = (iirSampleT * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleT;
		iirSampleU = (iirSampleU * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleU;
		iirSampleV = (iirSampleV * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleV;
		
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
		
		iirSampleW = (iirSampleW * (1.0 - iirAmount)) + (HeadBump * iirAmount); HeadBump -= iirSampleW;
		iirSampleX = (iirSampleX * (1.0 - iirAmount)) + (HeadBump * iirAmount); HeadBump -= iirSampleX;
		
		SubBump = HeadBump;
		iirSampleY = (iirSampleY * (1.0 - iirAmount)) + (SubBump * iirAmount); SubBump -= iirSampleY;
		
		iirDriveSampleC = (iirDriveSampleC * (1.0 - HeadBumpFreq)) + (SubBump * HeadBumpFreq); SubBump = iirDriveSampleC;
		iirDriveSampleD = (iirDriveSampleD * (1.0 - HeadBumpFreq)) + (SubBump * HeadBumpFreq); SubBump = iirDriveSampleD;
		
		
		SubBump = fabs(SubBump);
		if (SubOctave == false) {SubBump = -SubBump;}
		
		switch (bflip)
		{
			case 1:				
				iirSubBumpA += SubBump;// * BassGain);
				iirSubBumpA -= (iirSubBumpA * iirSubBumpA * iirSubBumpA * HeadBumpFreq);
				iirSubBumpA = (invrandy * iirSubBumpA) + (randy * iirSubBumpB) + (randy * iirSubBumpC);
				if (iirSubBumpA > 0) iirSubBumpA -= clamp;
				if (iirSubBumpA < 0) iirSubBumpA += clamp;
				SubBump = iirSubBumpA;
				break;
			case 2:
				iirSubBumpB += SubBump;// * BassGain);
				iirSubBumpB -= (iirSubBumpB * iirSubBumpB * iirSubBumpB * HeadBumpFreq);
				iirSubBumpB = (randy * iirSubBumpA) + (invrandy * iirSubBumpB) + (randy * iirSubBumpC);
				if (iirSubBumpB > 0) iirSubBumpB -= clamp;
				if (iirSubBumpB < 0) iirSubBumpB += clamp;
				SubBump = iirSubBumpB;
				break;
			case 3:
				iirSubBumpC += SubBump;// * BassGain);
				iirSubBumpC -= (iirSubBumpC * iirSubBumpC * iirSubBumpC * HeadBumpFreq);
				iirSubBumpC = (randy * iirSubBumpA) + (randy * iirSubBumpB) + (invrandy * iirSubBumpC);
				if (iirSubBumpC > 0) iirSubBumpC -= clamp;
				if (iirSubBumpC < 0) iirSubBumpC += clamp;
				SubBump = iirSubBumpC;
				break;
		}
		
		iirSampleZ = (iirSampleZ * (1.0 - HeadBumpFreq)) + (SubBump * HeadBumpFreq); SubBump = iirSampleZ;
		iirDriveSampleE = (iirDriveSampleE * (1.0 - iirAmount)) + (SubBump * iirAmount); SubBump = iirDriveSampleE;
		iirDriveSampleF = (iirDriveSampleF * (1.0 - iirAmount)) + (SubBump * iirAmount); SubBump = iirDriveSampleF;

		
		inputSampleL += (HeadBump * BassOutGain);
		inputSampleL += (SubBump * SubOutGain);
		
		inputSampleR += (HeadBump * BassOutGain);
		inputSampleR += (SubBump * SubOutGain);
				

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

