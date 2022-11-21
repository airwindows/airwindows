/*
*	File:		ToTape5.cpp
*	
*	Version:	1.0
* 
*	Created:	6/24/17
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
	ToTape5.cpp
	
=============================================================================*/
#include "ToTape5.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, ToTape5)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape5::ToTape5
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ToTape5::ToTape5(AudioUnit component)
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
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape5::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ToTape5::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape5::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ToTape5::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSix;
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
//	ToTape5::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ToTape5::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape5::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ToTape5::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ToTape5::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ToTape5::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ToTape5EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape5::ToTape5Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ToTape5::ToTape5Kernel::Reset()
{
	iirMidRollerA = 0.0;
	iirMidRollerB = 0.0;
	iirMidRollerC = 0.0;
	iirHeadBumpA = 0.0;
	iirHeadBumpB = 0.0;
	iirHeadBumpC = 0.0;
	iirMinHeadBump = 0.0;
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
	for (int temp = 0; temp < 999; temp++) {d[temp] = 0.0; e[temp] = 0.0;}
	gcount = 0;	
	hcount = 0;	
	sweep = 0.0;
	rateof = 0.5;
	nextmax = 0.5;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ToTape5::ToTape5Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ToTape5::ToTape5Kernel::Process(	const Float32 	*inSourceP,
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
	double fpOld = 0.618033988749894848204586; //golden ratio!

	Float64 inputgain = pow(GetParameter( kParam_One )+1.0,3);
	Float64 outputgain = GetParameter( kParam_Five );
	Float64 wet = GetParameter( kParam_Six );
	//removed unnecessary dry variable
	Float64 trim = 0.211324865405187117745425;
	Float64 SoftenControl = pow(GetParameter( kParam_Two ),2);
	Float64 tempRandy = 0.06 + (SoftenControl/10.0);
	//Float64 RollAmount = (1.0-((SoftenControl * 0.45)/overallscale));
	Float64 RollAmount = (1.0-(SoftenControl * 0.45))/overallscale;
	Float64 HeadBumpControl = pow(GetParameter( kParam_Three ),2);
	int allpasstemp;
	int maxdelay = (int)(floor(((HeadBumpControl+0.3)*2.2)*overallscale));
	HeadBumpControl *= fabs(HeadBumpControl);
	Float64 HeadBumpFreq = 0.044/overallscale;
	Float64 iirAmount = 0.000001/overallscale;
	Float64 altAmount = 1.0 - iirAmount;
	Float64 iirHBoostAmount = 0.0001/overallscale;
	Float64 altHBoostAmount = 1.0 - iirAmount;
	Float64 depth = pow(GetParameter( kParam_Four ),2)*overallscale;
	Float64 fluttertrim = 0.005/overallscale;
	Float64 sweeptrim = (0.0006*depth)/overallscale;
	Float64 offset;	
	Float64 tupi = 3.141592653589793238 * 2.0;
	Float64 newrate = 0.005/overallscale;
	Float64 oldrate = 1.0-newrate;
	Float64 HighsSample = 0.0;
	Float64 NonHighsSample = 0.0;
	Float64 HeadBump = 0.0;
	Float64 Subtract;
	Float64 bridgerectifier;
	Float64 flutterrandy;
	Float64 randy;
	Float64 invrandy;
	SInt32 count;
	Float64 tempSample;
	Float64 drySample;
	double inputSample;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		drySample = inputSample;

		flutterrandy = (double(fpd)/UINT32_MAX);
		randy = flutterrandy * tempRandy; //for soften
		invrandy = (1.0-randy);
		randy /= 2.0;
		//we've set up so that we dial in the amount of the alt sections (in pairs) with invrandy being the source section
		
			//now we've got a random flutter, so we're messing with the pitch before tape effects go on
		if (gcount < 0 || gcount > 300) {gcount = 300;}
		count = gcount;
		d[count+301] = d[count] = inputSample;
		gcount--;
		//we will also keep the buffer going, even when not in use
		
		if (depth != 0.0) {
			offset = (1.0 + sin(sweep)) * depth;
			count += (int)floor(offset);
			bridgerectifier = (d[count] * (1-(offset-floor(offset))));
			bridgerectifier += (d[count+1] * (offset-floor(offset)));
			bridgerectifier -= ((d[count+2] * (offset-floor(offset)))*trim);
			rateof = (nextmax * newrate) + (rateof * oldrate);
			sweep += rateof * fluttertrim;
			sweep += sweep * sweeptrim;
			if (sweep >= tupi){sweep = 0.0; nextmax = 0.02 + (flutterrandy*0.98);}
			inputSample = bridgerectifier;
			//apply to input signal only when flutter is present, interpolate samples
		}
		
		if (inputgain != 1.0) {
			inputSample *= inputgain;
		}
		
		if (flip < 1 || flip > 3) flip = 1;
		switch (flip)
		{
			case 1:				
				iirMidRollerA = (iirMidRollerA * (1.0 - RollAmount)) + (inputSample * RollAmount);
				iirMidRollerA = (invrandy * iirMidRollerA) + (randy * iirMidRollerB) + (randy * iirMidRollerC);
				HighsSample = inputSample - iirMidRollerA;
				NonHighsSample = iirMidRollerA;
				
				iirHeadBumpA += (inputSample * 0.05);
				iirHeadBumpA -= (iirHeadBumpA * iirHeadBumpA * iirHeadBumpA * HeadBumpFreq);
				iirHeadBumpA = (invrandy * iirHeadBumpA) + (randy * iirHeadBumpB) + (randy * iirHeadBumpC);
				break;
			case 2:
				iirMidRollerB = (iirMidRollerB * (1.0 - RollAmount)) + (inputSample * RollAmount);
				iirMidRollerB = (randy * iirMidRollerA) + (invrandy * iirMidRollerB) + (randy * iirMidRollerC);
				HighsSample = inputSample - iirMidRollerB;
				NonHighsSample = iirMidRollerB;
				
				iirHeadBumpB += (inputSample * 0.05);
				iirHeadBumpB -= (iirHeadBumpB * iirHeadBumpB * iirHeadBumpB * HeadBumpFreq);
				iirHeadBumpB = (randy * iirHeadBumpA) + (invrandy * iirHeadBumpB) + (randy * iirHeadBumpC);
				break;
			case 3:
				iirMidRollerC = (iirMidRollerC * (1.0 - RollAmount)) + (inputSample * RollAmount);
				iirMidRollerC = (randy * iirMidRollerA) + (randy * iirMidRollerB) + (invrandy * iirMidRollerC);
				HighsSample = inputSample - iirMidRollerC;
				NonHighsSample = iirMidRollerC;
				
				iirHeadBumpC += (inputSample * 0.05);
				iirHeadBumpC -= (iirHeadBumpC * iirHeadBumpC * iirHeadBumpC * HeadBumpFreq);
				iirHeadBumpC = (randy * iirHeadBumpA) + (randy * iirHeadBumpB) + (invrandy * iirHeadBumpC);
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
		
		bridgerectifier = fabs(inputSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0) inputSample = bridgerectifier;
		if (inputSample < 0) inputSample = -bridgerectifier;
		//drive section: the tape sound includes a very gentle saturation curve, which is always an attenuation.
		//we cut back on highs before hitting this, and then we're going to subtract highs a second time after.
		
		HeadBump = iirHeadBumpA + iirHeadBumpB + iirHeadBumpC;
		//begin PhaseNudge
		allpasstemp = hcount - 1;
		if (allpasstemp < 0 || allpasstemp > maxdelay) {allpasstemp = maxdelay;}
		HeadBump -= e[allpasstemp] * fpOld;
		e[hcount] = HeadBump;
		inputSample *= fpOld;
		hcount--; if (hcount < 0 || hcount > maxdelay) {hcount = maxdelay;}
		HeadBump += (e[hcount]);
		//end PhaseNudge on head bump in lieu of delay. 
		Subtract -= (HeadBump * (HeadBumpControl+iirMinHeadBump));
		//makes a second soften and a single head bump after saturation.
		//we are going to retain this, and then feed it into the highpass filter. That way, we can skip a subtract.
		//Head Bump retains a trace which is roughly as large as what the highpass will do.
		
		tempSample = inputSample;
		iirMinHeadBump = (iirMinHeadBump * altHBoostAmount) + (fabs(inputSample) * iirHBoostAmount);
		if (iirMinHeadBump > 0.01) iirMinHeadBump = 0.01;
		//we want this one rectified so that it's a relatively steady positive value. Boosts can cause it to be
		//greater than 1 so we clamp it in that case.
		
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
		
		if (outputgain != 1.0) {
			inputSample *= outputgain;
		}
		
		if (wet !=1.0) {
			inputSample = (inputSample * wet) + (drySample * (1.0-wet));
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

