/*
*	File:		FromTape.cpp
*	
*	Version:	1.0
* 
*	Created:	7/3/17
*	
*	Copyright:  Copyright © 2017 Airwindows, All Rights Reserved
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
	FromTape.cpp
	
=============================================================================*/
#include "FromTape.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, FromTape)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FromTape::FromTape
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FromTape::FromTape(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
	SetParameter(kParam_Five, kDefaultValue_ParamFive );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FromTape::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FromTape::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FromTape::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FromTape::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.maxValue = 2.0;
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
                outParameterInfo.maxValue = 2.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFour;
                break;
			case kParam_Five:
                AUBase::FillInParameterName (outParameterInfo, kParameterFiveName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFive;
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
//	FromTape::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FromTape::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FromTape::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FromTape::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	FromTape::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult FromTape::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____FromTapeEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FromTape::FromTapeKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		FromTape::FromTapeKernel::Reset()
{
	iirMidRollerA = 0.0;
	iirMidRollerB = 0.0;
	iirMidRollerC = 0.0;
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
	flip = 0;
	//noisesource = 0;
	fpNShape = 0.0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FromTape::FromTapeKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		FromTape::FromTapeKernel::Process(	const Float32 	*inSourceP,
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
	Float64 inputgain = GetParameter( kParam_One );
	Float64 SoftenControl = GetParameter( kParam_Two );
	Float64 RollAmount = (1.0-SoftenControl)/overallscale;
	Float64 iirAmount = (0.004*(1.0-GetParameter( kParam_Three )))/overallscale;
	Float64 altAmount = 1.0 - iirAmount;
	Float64 outputgain = GetParameter( kParam_Four );
	Float64 wet = GetParameter( kParam_Five );
	Float64 dry = 1.0 - wet;
	Float64 HighsSample = 0.0;
	Float64 Subtract;
	Float64 bridgerectifier;
	Float64 randy;
	Float64 invrandy;
	Float64 tempSample;
	Float64 drySample;
	
	
	long double inputSample;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (inputSample<1.2e-38 && -inputSample<1.2e-38) {
			static int noisesource = 0;
			//this declares a variable before anything else is compiled. It won't keep assigning
			//it to 0 for every sample, it's as if the declaration doesn't exist in this context,
			//but it lets me add this denormalization fix in a single place rather than updating
			//it in three different locations. The variable isn't thread-safe but this is only
			//a random seed and we can share it with whatever.
			noisesource = noisesource % 1700021; noisesource++;
			int residue = noisesource * noisesource;
			residue = residue % 170003; residue *= residue;
			residue = residue % 17011; residue *= residue;
			residue = residue % 1709; residue *= residue;
			residue = residue % 173; residue *= residue;
			residue = residue % 17;
			double applyresidue = residue;
			applyresidue *= 0.00000001;
			applyresidue *= 0.00000001;
			inputSample = applyresidue;
			//this denormalization routine produces a white noise at -300 dB which the noise
			//shaping will interact with to produce a bipolar output, but the noise is actually
			//all positive. That should stop any variables from going denormal, and the routine
			//only kicks in if digital black is input. As a final touch, if you save to 24-bit
			//the silence will return to being digital black again.
		}
		drySample = inputSample;
		
		
		if (inputgain != 1.0) {
			inputSample *= inputgain;
		}		
		
		randy = (rand()/(double)RAND_MAX) * SoftenControl; //for soften
		invrandy = (1.0-randy);
		randy /= 2.0;
		//we've set up so that we dial in the amount of the alt sections (in pairs) with invrandy being the source section
		
		Subtract = 0.0;
		tempSample = inputSample;		
		iirSampleA = (iirSampleA * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleA; Subtract += iirSampleA;
		iirSampleB = (iirSampleB * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleB; Subtract += iirSampleB;
		iirSampleC = (iirSampleC * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleC; Subtract += iirSampleC;
		iirSampleD = (iirSampleD * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleD; Subtract += iirSampleD;
		iirSampleE = (iirSampleE * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleE; Subtract += iirSampleE;
		iirSampleF = (iirSampleF * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleF; Subtract += iirSampleF;
		iirSampleG = (iirSampleG * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleG; Subtract += iirSampleG;
		iirSampleH = (iirSampleH * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleH; Subtract += iirSampleH;
		iirSampleI = (iirSampleI * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleI; Subtract += iirSampleI;
		iirSampleJ = (iirSampleJ * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleJ; Subtract += iirSampleJ;
		iirSampleK = (iirSampleK * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleK; Subtract += iirSampleK;
		iirSampleL = (iirSampleL * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleL; Subtract += iirSampleL;
		iirSampleM = (iirSampleM * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleM; Subtract += iirSampleM;
		iirSampleN = (iirSampleN * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleN; Subtract += iirSampleN;
		iirSampleO = (iirSampleO * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleO; Subtract += iirSampleO;
		iirSampleP = (iirSampleP * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleP; Subtract += iirSampleP;
		iirSampleQ = (iirSampleQ * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleQ; Subtract += iirSampleQ;
		iirSampleR = (iirSampleR * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleR; Subtract += iirSampleR;
		iirSampleS = (iirSampleS * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleS; Subtract += iirSampleS;
		iirSampleT = (iirSampleT * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleT; Subtract += iirSampleT;
		iirSampleU = (iirSampleU * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleU; Subtract += iirSampleU;
		iirSampleV = (iirSampleV * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleV; Subtract += iirSampleV;
		iirSampleW = (iirSampleW * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleW; Subtract += iirSampleW;
		iirSampleX = (iirSampleX * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleX; Subtract += iirSampleX;
		iirSampleY = (iirSampleY * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleY; Subtract += iirSampleY;
		iirSampleZ = (iirSampleZ * altAmount) + (tempSample * iirAmount); tempSample -= iirSampleZ; Subtract += iirSampleZ;
		//do the IIR on a dummy sample, and store up the correction in a variable at the same scale as the very low level
		//numbers being used. Don't keep doing it against the possibly high level signal number.
		//This has been known to add a resonant quality to the cutoff, which we're using on purpose.
		inputSample -= Subtract;
		//apply stored up tiny corrections.
		
				
		if (flip < 1 || flip > 3) flip = 1;
		switch (flip)
		{
			case 1:				
				iirMidRollerA = (iirMidRollerA * (1.0 - RollAmount)) + (inputSample * RollAmount);
				iirMidRollerA = (invrandy * iirMidRollerA) + (randy * iirMidRollerB) + (randy * iirMidRollerC);
				HighsSample = inputSample - iirMidRollerA;
				break;
			case 2:
				iirMidRollerB = (iirMidRollerB * (1.0 - RollAmount)) + (inputSample * RollAmount);
				iirMidRollerB = (randy * iirMidRollerA) + (invrandy * iirMidRollerB) + (randy * iirMidRollerC);
				HighsSample = inputSample - iirMidRollerB;
				break;
			case 3:
				iirMidRollerC = (iirMidRollerC * (1.0 - RollAmount)) + (inputSample * RollAmount);
				iirMidRollerC = (randy * iirMidRollerA) + (randy * iirMidRollerB) + (invrandy * iirMidRollerC);
				HighsSample = inputSample - iirMidRollerC;
				break;
		}
		flip++; //increment the triplet counter
		
		Subtract = HighsSample;		
		bridgerectifier = fabs(Subtract)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = 1-cos(bridgerectifier);
		if (Subtract > 0) Subtract = bridgerectifier;
		if (Subtract < 0) Subtract = -bridgerectifier;
		inputSample -= Subtract;

		//Soften works using the MidRoller stuff, defining a bright parallel channel that we apply negative Density
		//to, and then subtract from the main audio. That makes the 'highs channel subtract' hit only the loudest
		//transients, plus we are subtracting any artifacts we got from the negative Density.
		
		
		if (outputgain != 1.0) {
			inputSample *= outputgain;
		}
		
		if (wet !=1.0) {
			inputSample = (inputSample * wet) + (drySample * dry);
		}
		
		//32 bit dither, made small and tidy.
		int expon; frexpf((Float32)inputSample, &expon);
		long double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
		inputSample += (dither-fpNShape); fpNShape = dither;
		//end 32 bit dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

