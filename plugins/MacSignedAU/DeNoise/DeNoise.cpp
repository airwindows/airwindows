/*
*	File:		DeNoise.cpp
*	
*	Version:	1.0
* 
*	Created:	2/20/25
*	
*	Copyright:  Copyright © 2025 Airwindows, Airwindows uses the MIT license
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
	DeNoise.cpp
	
=============================================================================*/
#include "DeNoise.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(DeNoise)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeNoise::DeNoise
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DeNoise::DeNoise(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
	SetParameter(kParam_D, kDefaultValue_ParamD );
	SetParameter(kParam_E, kDefaultValue_ParamE );
	SetParameter(kParam_F, kDefaultValue_ParamF );
	SetParameter(kParam_G, kDefaultValue_ParamG );
	SetParameter(kParam_H, kDefaultValue_ParamH );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeNoise::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeNoise::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeNoise::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeNoise::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_A:
                AUBase::FillInParameterName (outParameterInfo, kParameterAName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamA;
                break;
            case kParam_B:
                AUBase::FillInParameterName (outParameterInfo, kParameterBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamB;
                break;
            case kParam_C:
                AUBase::FillInParameterName (outParameterInfo, kParameterCName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamC;
                break;
            case kParam_D:
                AUBase::FillInParameterName (outParameterInfo, kParameterDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamD;
                break;
            case kParam_E:
                AUBase::FillInParameterName (outParameterInfo, kParameterEName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamE;
                break;
            case kParam_F:
                AUBase::FillInParameterName (outParameterInfo, kParameterFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamF;
                break;
            case kParam_G:
                AUBase::FillInParameterName (outParameterInfo, kParameterGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamG;
                break;
            case kParam_H:
                AUBase::FillInParameterName (outParameterInfo, kParameterHName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamH;
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
//	DeNoise::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeNoise::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeNoise::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeNoise::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	DeNoise::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult DeNoise::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____DeNoiseEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeNoise::DeNoiseKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		DeNoise::DeNoiseKernel::Reset()
{
	for (int x = 0; x < pear_total; x++) {
		pearA[x] = 0.0;
		pearB[x] = 0.0;
		pearC[x] = 0.0;
		pearD[x] = 0.0;
		pearE[x] = 0.0;
	}
	topLevl = 1.0;
	pearA[levl] = 1.0;
	pearB[levl] = 1.0;
	pearC[levl] = 1.0;
	pearD[levl] = 1.0;
	pearE[levl] = 1.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeNoise::DeNoiseKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		DeNoise::DeNoiseKernel::Process(	const Float32 	*inSourceP,
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
	double topThresh =   pow(GetParameter( kParam_A )*0.25,4.0);
	double pearAThresh = pow(GetParameter( kParam_B )*0.25,4.0);
	double pearBThresh = pow(GetParameter( kParam_C )*0.25,4.0);
	double pearCThresh = pow(GetParameter( kParam_D )*0.25,4.0);
	double pearDThresh = pow(GetParameter( kParam_E )*0.25,4.0);
	double pearEThresh = pow(GetParameter( kParam_F )*0.25,4.0);
	double spd = pow(GetParameter( kParam_G ),3.0) / overallscale;
	double wet = GetParameter( kParam_H );
	
	double freqFactor = sqrt(overallscale)+(overallscale*0.5);
	pearA[freq] = pow(0.564,freqFactor+0.85);
	pearB[freq] = pow(0.564,freqFactor+1.9);
	pearC[freq] = pow(0.564,freqFactor+4.1);
	pearD[freq] = pow(0.564,freqFactor+6.2);
	pearE[freq] = pow(0.564,freqFactor+7.7);
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpd * 1.18e-17;
		double drySampleL = inputSampleL;
		
		for (int x = 0; x < pear_max; x += 2) {
			//begin Pear filter stages
			pearA[figL] = inputSampleL;
			pearA[slew] = ((pearA[figL]-pearA[x])+pearA[x+1])*pearA[freq]*0.5;
			pearA[x] = pearA[figL] = (pearA[freq]*pearA[figL])+((1.0-pearA[freq])*(pearA[x]+pearA[x+1]));
			pearA[x+1] = pearA[slew];
			inputSampleL -= pearA[figL];
			
			pearB[figL] = pearA[figL];
			pearB[slew] = ((pearB[figL]-pearB[x])+pearB[x+1])*pearB[freq]*0.5;
			pearB[x] = pearB[figL] = (pearB[freq]*pearA[figL])+((1.0-pearB[freq])*(pearB[x]+pearB[x+1]));
			pearB[x+1] = pearB[slew];
			pearA[figL] -= pearB[figL];
			
			pearC[figL] = pearB[figL];
			pearC[slew] = ((pearC[figL]-pearC[x])+pearC[x+1])*pearC[freq]*0.5;
			pearC[x] = pearC[figL] = (pearC[freq]*pearB[figL])+((1.0-pearC[freq])*(pearC[x]+pearC[x+1]));
			pearC[x+1] = pearC[slew];
			pearB[figL] -= pearC[figL];
			
			pearD[figL] = pearC[figL];
			pearD[slew] = ((pearD[figL]-pearD[x])+pearD[x+1])*pearD[freq]*0.5;
			pearD[x] = pearD[figL] = (pearD[freq]*pearC[figL])+((1.0-pearD[freq])*(pearD[x]+pearD[x+1]));
			pearD[x+1] = pearD[slew];
			pearC[figL] -= pearD[figL];
			
			pearE[figL] = pearD[figL];
			pearE[slew] = ((pearE[figL]-pearE[x])+pearE[x+1])*pearE[freq]*0.5;
			pearE[x] = pearE[figL] = (pearE[freq]*pearD[figL])+((1.0-pearE[freq])*(pearE[x]+pearE[x+1]));
			pearE[x+1] = pearE[slew];
			pearD[figL] -= pearE[figL];
			
			topLevl     *= (1.0-spd);
			topLevl     += ((fabs(inputSampleL)<topThresh)?0.0:spd*2.0);
			inputSampleL = (inputSampleL * fmin(topLevl,1.0));
			
			pearA[levl] *= (1.0-(spd*pearA[freq]));
			pearA[levl] += ((fabs(pearA[figL])<pearAThresh)?0.0:(spd*2.0*pearA[freq]));
			inputSampleL += (pearA[figL] * fmin(pearA[levl],1.0));
			
			pearB[levl] *= (1.0-(spd*pearB[freq]));
			pearB[levl] += ((fabs(pearB[figL])<pearBThresh)?0.0:(spd*2.0*pearB[freq]));
			inputSampleL += (pearB[figL] * fmin(pearB[levl],1.0));
			
			pearC[levl] *= (1.0-(spd*pearC[freq]));
			pearC[levl] += ((fabs(pearC[figL])<pearCThresh)?0.0:(spd*2.0*pearC[freq]));
			inputSampleL += (pearC[figL] * fmin(pearC[levl],1.0));
			
			pearD[levl] *= (1.0-(spd*pearD[freq]));
			pearD[levl] += ((fabs(pearD[figL])<pearDThresh)?0.0:(spd*2.0*pearD[freq]));
			inputSampleL += (pearD[figL] * fmin(pearD[levl],1.0));
			
			pearE[levl] *= (1.0-(spd*pearE[freq]));						
			pearE[levl] += ((fabs(pearE[figL])<pearEThresh)?0.0:(spd*2.0*pearE[freq]));
			inputSampleL += (pearE[figL] * fmin(pearE[levl],1.0));
		}
		
		if (wet == 0.0) {
			drySampleL = inputSampleL - drySampleL;
		} //clicks only at full dry
		
		inputSampleL = (inputSampleL*wet) + (drySampleL*(1.0-wet));
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSampleL;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

