/*
*	File:		FatEQ.cpp
*	
*	Version:	1.0
* 
*	Created:	10/7/25
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
	FatEQ.cpp
	
=============================================================================*/
#include "FatEQ.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(FatEQ)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FatEQ::FatEQ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FatEQ::FatEQ(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
	SetParameter(kParam_D, kDefaultValue_ParamD );
	SetParameter(kParam_E, kDefaultValue_ParamE );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FatEQ::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FatEQ::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FatEQ::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FatEQ::GetParameterInfo(AudioUnitScope		inScope,
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
//	FatEQ::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FatEQ::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FatEQ::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FatEQ::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	FatEQ::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult FatEQ::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____FatEQEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FatEQ::FatEQKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		FatEQ::FatEQKernel::Reset()
{
	for (int x = 0; x < pear_total; x++) {
		pearA[x] = 0.0;
		pearB[x] = 0.0;
		pearC[x] = 0.0;
	}
	lastSample = 0.0;
	wasPosClip = false;
	wasNegClip = false;
	for (int x = 0; x < 16; x++) intermediate[x] = 0.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FatEQ::FatEQKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		FatEQ::FatEQKernel::Process(	const Float32 	*inSourceP,
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
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
	double topLevl = GetParameter( kParam_A )*2.0; if (topLevl < 1.0) topLevl = sqrt(topLevl);
	pearA[levl] = GetParameter( kParam_B )*2.0; if (pearA[levl] < 1.0) pearA[levl] = sqrt(pearA[levl]);
	pearB[levl] = GetParameter( kParam_C )*2.0; if (pearB[levl] < 1.0) pearB[levl] = sqrt(pearB[levl]);
	pearC[levl] = GetParameter( kParam_D )*2.0; if (pearC[levl] < 1.0) pearC[levl] = sqrt(pearC[levl]);
	double out = GetParameter( kParam_E );
	
	double freqFactor = sqrt(overallscale) + (overallscale*0.5);
	pearA[freq] = pow(0.564,freqFactor+0.85);
	pearB[freq] = pow(0.564,freqFactor+4.1);
	pearC[freq] = pow(0.564,freqFactor+7.1);
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpd * 1.18e-17;
		
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
			
			double altered = inputSampleL;
			if (topLevl > 1.0) {
				altered = fmax(fmin(inputSampleL*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (topLevl < 1.0) {
				altered = fmax(fmin(inputSampleL,1.0),-1.0);
				double polarity = altered;
				double X = inputSampleL * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL = (inputSampleL*(1.0-fabs(topLevl-1.0)))+(altered*fabs(topLevl-1.0));
			
			altered = pearA[figL];
			if (pearA[levl] > 1.0) {
				altered = fmax(fmin(pearA[figL]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearA[levl] < 1.0) {
				altered = fmax(fmin(pearA[figL],1.0),-1.0);
				double polarity = altered;
				double X = pearA[figL] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL += (pearA[figL]*(1.0-fabs(pearA[levl]-1.0)))+(altered*fabs(pearA[levl]-1.0));
			
			altered = pearB[figL];
			if (pearB[levl] > 1.0) {
				altered = fmax(fmin(pearB[figL]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearB[levl] < 1.0) {
				altered = fmax(fmin(pearB[figL],1.0),-1.0);
				double polarity = altered;
				double X = pearB[figL] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL += (pearB[figL]*(1.0-fabs(pearB[levl]-1.0)))+(altered*fabs(pearB[levl]-1.0));
			
			altered = pearC[figL];
			if (pearC[levl] > 1.0) {
				altered = fmax(fmin(pearC[figL]*M_PI_2,M_PI_2),-M_PI_2);
				double X = altered*altered;
				double temp = altered*X; altered -= (temp/6.0); temp *= X;
				altered += (temp / 120.0); temp *= X; altered -= (temp / 5040.0); temp *= X;
				altered += (temp / 362880.0); temp *= X; altered -= (temp / 39916800.0);
			}
			if (pearC[levl] < 1.0) {
				altered = fmax(fmin(pearC[figL],1.0),-1.0);
				double polarity = altered;
				double X = pearC[figL] * altered;
				double temp = X; altered = (temp / 2.0); temp *= X;
				altered -= (temp / 24.0); temp *= X; altered += (temp / 720.0); temp *= X;
				altered -= (temp / 40320.0); temp *= X; altered += (temp / 3628800.0);
				altered *= ((polarity<0.0)?-1.0:1.0);
			}
			inputSampleL += (pearC[figL]*(1.0-fabs(pearC[levl]-1.0)))+(altered*fabs(pearC[levl]-1.0));
		}
		
		inputSampleL *= out;
		
		//begin ClipOnly2 as a little, compressed chunk that can be dropped into code
		if (inputSampleL > 4.0) inputSampleL = 4.0; if (inputSampleL < -4.0) inputSampleL = -4.0;
		if (wasPosClip == true) { //current will be over
			if (inputSampleL<lastSample) lastSample=0.7058208+(inputSampleL*0.2609148);
			else lastSample = 0.2491717+(lastSample*0.7390851);
		} wasPosClip = false;
		if (inputSampleL>0.9549925859) {wasPosClip=true;inputSampleL=0.7058208+(lastSample*0.2609148);}
		if (wasNegClip == true) { //current will be -over
			if (inputSampleL > lastSample) lastSample=-0.7058208+(inputSampleL*0.2609148);
			else lastSample=-0.2491717+(lastSample*0.7390851);
		} wasNegClip = false;
		if (inputSampleL<-0.9549925859) {wasNegClip=true;inputSampleL=-0.7058208+(lastSample*0.2609148);}
		intermediate[spacing] = inputSampleL;
        inputSampleL = lastSample; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediate[x-1] = intermediate[x];
		lastSample = intermediate[0]; //run a little buffer to handle this
		//end ClipOnly2 as a little, compressed chunk that can be dropped into code		
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSampleL;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

