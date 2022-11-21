/*
*	File:		Deckwrecka.cpp
*	
*	Version:	1.0
* 
*	Created:	10/23/13
*	
*	Copyright:  Copyright © 2013 Airwindows, Airwindows uses the MIT license
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
	Deckwrecka.cpp
	
=============================================================================*/
#include "Deckwrecka.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Deckwrecka)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Deckwrecka::Deckwrecka
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Deckwrecka::Deckwrecka(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Deckwrecka::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Deckwrecka::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Deckwrecka::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Deckwrecka::GetParameterInfo(AudioUnitScope		inScope,
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
//	Deckwrecka::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Deckwrecka::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Deckwrecka::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Deckwrecka::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Deckwrecka::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Deckwrecka::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____DeckwreckaEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Deckwrecka::DeckwreckaKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Deckwrecka::DeckwreckaKernel::Reset()
{
	bflip = 1;
	iirHeadBumpA = 0.0;
	iirHeadBumpB = 0.0;
	iirHeadBumpC = 0.0;
		
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
//	Deckwrecka::DeckwreckaKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Deckwrecka::DeckwreckaKernel::Process(	const Float32 	*inSourceP,
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
	
	Float64 wreck = pow(GetParameter( kParam_One ),2);
	Float64 randy;
	Float64 HeadBump = 0.0;
	Float64 HeadBumpFreq = 0.01/overallscale;
	Float64 iirHPAmount = 0.0000014/overallscale;
	Float64 altHPAmount = 1.0 - iirHPAmount;
	Float64 inputSample;
	Float64 tempSample;
	Float64 bridgerectifier;
	Float64 correction;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;

		randy = (double(fpd)/UINT32_MAX);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		randy += (double(fpd)/UINT32_MAX);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		randy += (double(fpd)/UINT32_MAX);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		randy += (double(fpd)/UINT32_MAX);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		randy += (double(fpd)/UINT32_MAX);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		randy += (double(fpd)/UINT32_MAX);
		randy /= 6.0;
		randy *= wreck; //0 to 1 the noise, may not be needed
		//set up the noise
		
		switch (bflip)
		{
			case 1:				
				iirHeadBumpA += (inputSample * randy);
				iirHeadBumpA -= (iirHeadBumpA * iirHeadBumpA * iirHeadBumpA * HeadBumpFreq);
				break;
			case 2:
				iirHeadBumpB += (inputSample * randy);
				iirHeadBumpB -= (iirHeadBumpB * iirHeadBumpB * iirHeadBumpB * HeadBumpFreq);
				break;
			case 3:
				iirHeadBumpC += (inputSample * randy);
				iirHeadBumpC -= (iirHeadBumpC * iirHeadBumpC * iirHeadBumpC * HeadBumpFreq);
				break;
		}

		HeadBump = iirHeadBumpA + iirHeadBumpB + iirHeadBumpC;
		
		bflip++;
		if (bflip < 1 || bflip > 3) bflip = 1;
		
		tempSample = HeadBump;
		
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
		HeadBump -= correction;
		//apply stored up tiny corrections
		
		HeadBump *= (1.0 - (wreck/2));
		
		bridgerectifier = fabs(HeadBump);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (HeadBump > 0.0) HeadBump = bridgerectifier;
		else HeadBump = -bridgerectifier;
		//clamp HeadBump to -1 to 1 with distortion		

		inputSample *= (1.0 - wreck);
		inputSample += (HeadBump * wreck);
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		//end 32 bit floating point dither
		
		*destP = inputSample;
		//built in output trim and dry/wet by default
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
