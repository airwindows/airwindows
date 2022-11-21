/*
*	File:		BassAmp.cpp
*	
*	Version:	1.0
* 
*	Created:	5/30/20
*	
*	Copyright:  Copyright © 2020 Airwindows, Airwindows uses the MIT license
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
	BassAmp.cpp
	
=============================================================================*/
#include "BassAmp.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(BassAmp)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassAmp::BassAmp
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BassAmp::BassAmp(AudioUnit component)
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
//	BassAmp::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassAmp::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassAmp::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassAmp::GetParameterInfo(AudioUnitScope		inScope,
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
           case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
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
//	BassAmp::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassAmp::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassAmp::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassAmp::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	BassAmp::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult BassAmp::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____BassAmpEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassAmp::BassAmpKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BassAmp::BassAmpKernel::Reset()
{
	ataLast6Sample = ataLast5Sample = ataLast4Sample = ataLast3Sample = ataLast2Sample = ataLast1Sample = 0.0;
	ataHalfwaySample = ataHalfDrySample = ataHalfDiffSample = 0.0;
	ataLastDiffSample = ataDrySample = ataDiffSample = ataPrevDiffSample = 0.0;
	ataK1 = -0.646; //first FIR shaping of interpolated sample, brightens
	ataK2 = 0.311; //second FIR shaping of interpolated sample, thickens
	ataK6 = -0.093; //third FIR shaping of interpolated sample, brings air
	ataK7 = 0.057; //fourth FIR shaping of interpolated sample, thickens
	ataK8 = -0.023; //fifth FIR shaping of interpolated sample, brings air
	ataK3 = 0.114; //add raw to interpolated dry, toughens
	ataK4 = 0.886; //remainder of interpolated dry, adds up to 1.0
	ataK5 = 0.122; //subtract this much prev. diff sample, brightens
	WasNegative = false;
	SubOctave = false;
	bflip = 0;
	flip = false;
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
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassAmp::BassAmpKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BassAmp::BassAmpKernel::Process(	const Float32 	*inSourceP,
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
	
	Float64 contHigh = GetParameter( kParam_One );
	Float64 dry = GetParameter( kParam_Two );
	Float64 contDub = GetParameter( kParam_Three )*1.3;
	Float64 contSub = GetParameter( kParam_Four )/2.0;
	
	Float64 driveone = pow(contHigh*3.0,2);
	Float64 driveoutput = contHigh;
	Float64 iirAmount = 0.344/overallscale;
	Float64 altAmount = 1.0 - iirAmount;	
	Float64 BassGain = contDub * 0.1;
	Float64 HeadBumpFreq = (BassGain+0.0001)/overallscale;
	Float64 BassOutGain = contDub * 0.2;
	Float64 SubGain = contSub * 0.1;
	Float64 SubBumpFreq = (SubGain+0.0001)/overallscale;
	Float64 SubOutGain = contSub * 0.3;
	Float64 iirHPAmount = 0.0000014/overallscale;
	Float64 altHPAmount = 1.0 - iirHPAmount;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		ataDrySample = inputSample = *sourceP;
		ataHalfDrySample = ataHalfwaySample = (inputSample + ataLast1Sample + (ataLast2Sample*ataK1) + (ataLast3Sample*ataK2) + (ataLast4Sample*ataK6) + (ataLast5Sample*ataK7) + (ataLast6Sample*ataK8)) / 2.0;
		ataLast6Sample = ataLast5Sample; ataLast5Sample = ataLast4Sample; ataLast4Sample = ataLast3Sample; ataLast3Sample = ataLast2Sample; ataLast2Sample = ataLast1Sample; ataLast1Sample = inputSample;
		//setting up oversampled special antialiasing
		
		Float64 correction;
		Float64 subtractSample;
		Float64 tempSample; //used repeatedly in different places
		
		//begin first half- change inputSample -> ataHalfwaySample, ataDrySample -> ataHalfDrySample
		
		Float64 ataHalfwayLowpass;
		if (flip)
		{
			tempSample = subtractSample = ataHalfwaySample;
			iirDriveSampleA = (iirDriveSampleA * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleA; correction = iirDriveSampleA;
			iirDriveSampleC = (iirDriveSampleC * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleC; correction += iirDriveSampleC;
			iirDriveSampleE = (iirDriveSampleE * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleE; correction += iirDriveSampleE;
			ataHalfwaySample -= correction;
			ataHalfwayLowpass = subtractSample - ataHalfwaySample;
		}
		else
		{
			tempSample = subtractSample = ataHalfwaySample;
			iirDriveSampleB = (iirDriveSampleB * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleB; correction = iirDriveSampleB;
			iirDriveSampleD = (iirDriveSampleD * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleD; correction += iirDriveSampleD;
			iirDriveSampleF = (iirDriveSampleF * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleF; correction += iirDriveSampleF;
			ataHalfwaySample -= correction;			
			ataHalfwayLowpass = subtractSample - ataHalfwaySample;
		}
		//highpass section
		if (ataHalfwaySample > 1.0) {ataHalfwaySample = 1.0;}
		if (ataHalfwaySample < -1.0) {ataHalfwaySample = -1.0;}
		Float64 out = driveone;
		while (out > 0.60)
		{
			out -= 0.60;
			ataHalfwaySample -= (ataHalfwaySample * (fabs(ataHalfwaySample) * 0.60) * (fabs(ataHalfwaySample) * 0.60) );
			ataHalfwaySample *= (1.0+0.60);
		}
		//that's taken care of the really high gain stuff
		
		ataHalfwaySample -= (ataHalfwaySample * (fabs(ataHalfwaySample) * out) * (fabs(ataHalfwaySample) * out) );
		ataHalfwaySample *= (1.0+out);
		
		
		//end first half
		//begin second half- inputSample and ataDrySample handled separately here
		
		Float64 ataLowpass;
		if (flip)
		{
			tempSample = subtractSample = inputSample;
			iirDriveSampleA = (iirDriveSampleA * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleA; correction = iirDriveSampleA;
			iirDriveSampleC = (iirDriveSampleC * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleC; correction += iirDriveSampleC;
			iirDriveSampleE = (iirDriveSampleE * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleE; correction += iirDriveSampleE;
			inputSample -= correction;
			ataLowpass = subtractSample - inputSample;
		}
		else
		{
			tempSample = subtractSample = inputSample;
			iirDriveSampleB = (iirDriveSampleB * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleB; correction = iirDriveSampleB;
			iirDriveSampleD = (iirDriveSampleD * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleD; correction += iirDriveSampleD;
			iirDriveSampleF = (iirDriveSampleF * altAmount) + (tempSample * iirAmount); tempSample -= iirDriveSampleF; correction += iirDriveSampleF;
			inputSample -= correction;
			ataLowpass = subtractSample - inputSample;
		}
		//highpass section
		if (inputSample > 1.0) {inputSample = 1.0;}
		if (inputSample < -1.0) {inputSample = -1.0;}
		out = driveone;
		while (out > 0.60)
		{
			out -= 0.60;
			inputSample -= (inputSample * (fabs(inputSample) * 0.60) * (fabs(inputSample) * 0.60) );
			inputSample *= (1.0+0.60);
		}
		//that's taken care of the really high gain stuff
		
		inputSample -= (inputSample * (fabs(inputSample) * out) * (fabs(inputSample) * out) );
		inputSample *= (1.0+out);
		//end second half
		
		
		ataHalfDrySample = (ataDrySample*ataK3)+(ataHalfDrySample*ataK4);
		ataHalfDiffSample = (ataHalfwaySample - ataHalfDrySample)/2.0;
		ataLastDiffSample = ataDiffSample*ataK5;
		ataDiffSample = (inputSample - ataDrySample)/2.0;
		ataDiffSample += ataHalfDiffSample;
		ataDiffSample -= ataLastDiffSample;
		inputSample = ataDrySample;
		inputSample += ataDiffSample;
		//apply processing as difference to non-oversampled raw input
		
		//now we've got inputSample as the Drive top-end output, and we have ataLowpass and ataHalfwayLowpass
		ataLowpass += ataHalfwayLowpass; //and combined them. Now we make sub-octaves
		
		
		//set up polarities for sub-bass version
		Float64 randy = (double(fpd)/UINT32_MAX)*0.0555; //0 to 1 the noise, may not be needed		
		
		switch (bflip)
		{
			case 1:				
				iirHeadBumpA += (ataLowpass * BassGain);
				iirHeadBumpA -= (iirHeadBumpA * iirHeadBumpA * iirHeadBumpA * HeadBumpFreq);
				iirHeadBumpA = ((1.0-randy) * iirHeadBumpA) + (randy * 0.5 * iirHeadBumpB) + (randy * 0.5 * iirHeadBumpC);
				break;
			case 2:
				iirHeadBumpB += (ataLowpass * BassGain);
				iirHeadBumpB -= (iirHeadBumpB * iirHeadBumpB * iirHeadBumpB * HeadBumpFreq);
				iirHeadBumpB = (randy * 0.5 * iirHeadBumpA) + ((1.0-randy) * iirHeadBumpB) + (randy * 0.5 * iirHeadBumpC);
				break;
			case 3:
				iirHeadBumpC += (ataLowpass * BassGain);
				iirHeadBumpC -= (iirHeadBumpC * iirHeadBumpC * iirHeadBumpC * HeadBumpFreq);
				iirHeadBumpC = (randy * 0.5 * iirHeadBumpA) + (randy * 0.5 * iirHeadBumpB) + ((1.0-randy) * iirHeadBumpC);
				break;
		}
		Float64 HeadBump = iirHeadBumpA + iirHeadBumpB + iirHeadBumpC;
		
		Float64 SubBump = fabs(HeadBump);
		
		if (HeadBump > 0.0) {
			if (WasNegative) SubOctave = !SubOctave;
			WasNegative = false;
		} else WasNegative = true;
		if (SubOctave == false) SubBump = -SubBump;
		
		switch (bflip)
		{
			case 1:				
				iirSubBumpA += (SubBump * SubGain);
				iirSubBumpA -= (iirSubBumpA * iirSubBumpA * iirSubBumpA * SubBumpFreq);
				iirSubBumpA = ((1.0-randy) * iirSubBumpA) + (randy * 0.5 * iirSubBumpB) + (randy * 0.5 * iirSubBumpC);
				break;
			case 2:
				iirSubBumpB += (SubBump * SubGain);
				iirSubBumpB -= (iirSubBumpB * iirSubBumpB * iirSubBumpB * SubBumpFreq);
				iirSubBumpB = (randy * 0.5 * iirSubBumpA) + ((1.0-randy) * iirSubBumpB) + (randy * 0.5 * iirSubBumpC);
				break;
			case 3:
				iirSubBumpC += (SubBump * SubGain);
				iirSubBumpC -= (iirSubBumpC * iirSubBumpC * iirSubBumpC * SubBumpFreq);
				iirSubBumpC = (randy * 0.5 * iirSubBumpA) + (randy * 0.5 * iirSubBumpB) + ((1.0-randy) * iirSubBumpC);
				break;
		}
		SubBump = iirSubBumpA + iirSubBumpB + iirSubBumpC;
		
		
		flip = !flip;
		bflip++;
		if (bflip < 1 || bflip > 3) bflip = 1;
		
		inputSample *= driveoutput;
		correction = ataDrySample*dry;
		correction += (HeadBump * BassOutGain);
		correction += (SubBump * SubOutGain);
		inputSample += correction;
		
		tempSample = inputSample;
		
		iirSampleA = (iirSampleA * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleA; correction = iirSampleA;
		iirSampleB = (iirSampleB * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleB; correction += iirSampleB;
		iirSampleC = (iirSampleC * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleC; correction += iirSampleC;
		iirSampleD = (iirSampleD * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleD; correction += iirSampleD;
		iirSampleE = (iirSampleE * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleE; correction += iirSampleE;
		iirSampleF = (iirSampleF * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleF; correction += iirSampleF;
		iirSampleG = (iirSampleG * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleG; correction += iirSampleG;
		iirSampleH = (iirSampleH * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleH; correction += iirSampleH;
		iirSampleI = (iirSampleI * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleI; correction += iirSampleI;
		iirSampleJ = (iirSampleJ * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleJ; correction += iirSampleJ;
		iirSampleK = (iirSampleK * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleK; correction += iirSampleK;
		iirSampleL = (iirSampleL * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleL; correction += iirSampleL;
		iirSampleM = (iirSampleM * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleM; correction += iirSampleM;
		iirSampleN = (iirSampleN * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleN; correction += iirSampleN;
		iirSampleO = (iirSampleO * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleO; correction += iirSampleO;
		iirSampleP = (iirSampleP * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleP; correction += iirSampleP;
		iirSampleQ = (iirSampleQ * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleQ; correction += iirSampleQ;
		iirSampleR = (iirSampleR * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleR; correction += iirSampleR;
		iirSampleS = (iirSampleS * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleS; correction += iirSampleS;
		iirSampleT = (iirSampleT * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleT; correction += iirSampleT;
		iirSampleU = (iirSampleU * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleU; correction += iirSampleU;
		iirSampleV = (iirSampleV * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleV; correction += iirSampleV;
		iirSampleW = (iirSampleW * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleW; correction += iirSampleW;
		iirSampleX = (iirSampleX * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleX; correction += iirSampleX;
		iirSampleY = (iirSampleY * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleY; correction += iirSampleY;
		iirSampleZ = (iirSampleZ * altHPAmount) + (tempSample * iirHPAmount); tempSample -= iirSampleZ; correction += iirSampleZ;
		//do the IIR on a dummy sample, and store up the correction in a variable at the same scale as the very low level
		//numbers being used. Don't keep doing it against the possibly high level signal number.
		inputSample -= correction;
		//apply stored up tiny corrections
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

