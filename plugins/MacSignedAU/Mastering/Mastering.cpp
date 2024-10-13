/*
*	File:		Mastering.cpp
*	
*	Version:	1.0
* 
*	Created:	10/8/24
*	
*	Copyright:  Copyright © 2024 Airwindows, Airwindows uses the MIT license
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
	Mastering.cpp
	
=============================================================================*/
#include "Mastering.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Mastering)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Mastering::Mastering
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Mastering::Mastering(AudioUnit component)
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
	SetParameter(kParam_I, kDefaultValue_ParamI );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Mastering::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Mastering::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Mastering::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Mastering::GetParameterInfo(AudioUnitScope		inScope,
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
            case kParam_I:
                AUBase::FillInParameterName (outParameterInfo, kParameterIName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamI;
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
//	Mastering::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Mastering::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Mastering::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Mastering::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Mastering::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Mastering::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____MasteringEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Mastering::MasteringKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Mastering::MasteringKernel::Reset()
{
	for (int x = 0; x < air_total; x++) air[x] = 0.0;
	for (int x = 0; x < kal_total; x++) {kalM[x] = 0.0;kalS[x] = 0.0;}
	lastSinewL = 0.0;
	lastSample = 0.0;
	wasPosClip = false;
	wasNegClip = false;
	for (int x = 0; x < 16; x++) {intermediate[x] = 0.0;}

	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Mastering::MasteringKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Mastering::MasteringKernel::Process(	const Float32 	*inSourceP,
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
	
	long double trebleGain = GetParameter( kParam_A )+0.5;
	if (trebleGain > 1.0) trebleGain = pow(trebleGain,3.0+sqrt(overallscale));
	//this boost is necessary to adapt to higher sample rates
	long double midGain = GetParameter( kParam_B )+0.5;
	long double bassGain = (1.0-GetParameter( kParam_C ))+0.5; //control inverted
	long double subGain = GetParameter( kParam_D )+0.5;
	//simple four band to adjust
	double kalMid = pow(1.0-GetParameter( kParam_E ),3);
	//crossover frequency between mid/bass
	double kalSub = (1.0-(pow(GetParameter( kParam_F ),3)));
	//crossover frequency between bass/sub
	double zoom = (GetParameter( kParam_G )*2.0)-1.0;
	double zoomStages = (fabs(zoom)*4.0)+1.0;
	for (int count = 0; count < sqrt(zoomStages); count++) zoom *= fabs(zoom);
	double threshSinew = pow(GetParameter( kParam_H ),2)/overallscale;
	double depthSinew = GetParameter( kParam_I );
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
	while (nSampleFrames-- > 0) {
		long double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpd * 1.18e-17;
		double drySampleL = inputSampleL;
		
		//begin Air3L
		air[pvSL4] = air[pvAL4] - air[pvAL3]; air[pvSL3] = air[pvAL3] - air[pvAL2];
		air[pvSL2] = air[pvAL2] - air[pvAL1]; air[pvSL1] = air[pvAL1] - inputSampleL;
		air[accSL3] = air[pvSL4] - air[pvSL3]; air[accSL2] = air[pvSL3] - air[pvSL2];
		air[accSL1] = air[pvSL2] - air[pvSL1];
		air[acc2SL2] = air[accSL3] - air[accSL2]; air[acc2SL1] = air[accSL2] - air[accSL1];		
		air[outAL] = -(air[pvAL1] + air[pvSL3] + air[acc2SL2] - ((air[acc2SL2] + air[acc2SL1])*0.5));
		air[gainAL] *= 0.5; air[gainAL] += fabs(drySampleL-air[outAL])*0.5;
		if (air[gainAL] > 0.3*sqrt(overallscale)) air[gainAL] = 0.3*sqrt(overallscale);
		air[pvAL4] = air[pvAL3]; air[pvAL3] = air[pvAL2];
		air[pvAL2] = air[pvAL1]; air[pvAL1] = (air[gainAL] * air[outAL]) + drySampleL;
		long double midL = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
		long double temp = (midL + air[gndavgL])*0.5; air[gndavgL] = midL; midL = temp;
		//we have a single averaging stage to smooth stuff out
		long double trebleL = drySampleL-midL;
		//end Air3L
		
		//begin KalmanML
		temp = midL;
		kalM[prevSlewL3] += kalM[prevSampL3] - kalM[prevSampL2]; kalM[prevSlewL3] *= 0.5;
		kalM[prevSlewL2] += kalM[prevSampL2] - kalM[prevSampL1]; kalM[prevSlewL2] *= 0.5;
		kalM[prevSlewL1] += kalM[prevSampL1] - midL; kalM[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kalM[accSlewL2] += kalM[prevSlewL3] - kalM[prevSlewL2]; kalM[accSlewL2] *= 0.5;
		kalM[accSlewL1] += kalM[prevSlewL2] - kalM[prevSlewL1]; kalM[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalM[accSlewL3] += (kalM[accSlewL2] - kalM[accSlewL1]); kalM[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kalM[kalOutL] += kalM[prevSampL1] + kalM[prevSlewL2] + kalM[accSlewL3]; kalM[kalOutL] *= 0.5;

		//resynthesizing predicted result (all iir smoothed)
		kalM[kalGainL] += fabs(temp-kalM[kalOutL])*kalMid*8.0; kalM[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalM[kalGainL] > kalMid*0.5) kalM[kalGainL] = kalMid*0.5;
		//attempts to avoid explosions
		kalM[kalOutL] += (temp*(1.0-(0.68+(kalMid*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalM[prevSampL3] = kalM[prevSampL2]; kalM[prevSampL2] = kalM[prevSampL1];
		kalM[prevSampL1] = (kalM[kalGainL] * kalM[kalOutL]) + ((1.0-kalM[kalGainL])*temp);
		//feed the chain of previous samples
		long double bassL = (kalM[kalOutL]+kalM[kalAvgL])*0.5;
		kalM[kalAvgL] = kalM[kalOutL];
		midL -= bassL;
		//end KalmanML
		
		//begin KalmanSL
		temp = bassL;
		kalS[prevSlewL3] += kalS[prevSampL3] - kalS[prevSampL2]; kalS[prevSlewL3] *= 0.5;
		kalS[prevSlewL2] += kalS[prevSampL2] - kalS[prevSampL1]; kalS[prevSlewL2] *= 0.5;
		kalS[prevSlewL1] += kalS[prevSampL1] - bassL; kalS[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kalS[accSlewL2] += kalS[prevSlewL3] - kalS[prevSlewL2]; kalS[accSlewL2] *= 0.5;
		kalS[accSlewL1] += kalS[prevSlewL2] - kalS[prevSlewL1]; kalS[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalS[accSlewL3] += (kalS[accSlewL2] - kalS[accSlewL1]); kalS[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kalS[kalOutL] += kalS[prevSampL1] + kalS[prevSlewL2] + kalS[accSlewL3]; kalS[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kalS[kalGainL] += fabs(temp-kalS[kalOutL])*kalSub*8.0; kalS[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalS[kalGainL] > kalSub*0.5) kalS[kalGainL] = kalSub*0.5;
		//attempts to avoid explosions
		kalS[kalOutL] += (temp*(1.0-(0.68+(kalSub*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalS[prevSampL3] = kalS[prevSampL2]; kalS[prevSampL2] = kalS[prevSampL1];
		kalS[prevSampL1] = (kalS[kalGainL] * kalS[kalOutL]) + ((1.0-kalS[kalGainL])*temp);
		//feed the chain of previous samples
		long double subL = (kalS[kalOutL]+kalS[kalAvgL])*0.5;
		kalS[kalAvgL] = kalS[kalOutL];
		bassL -= subL;
		//end KalmanSL
		
		inputSampleL = (subL*subGain);
		inputSampleL += (bassL*bassGain);
		inputSampleL += (midL*midGain);
		inputSampleL += (trebleL*trebleGain);
		
		for (int count = 0; count < zoomStages; count++) {
			if (zoom > 0.0) {
				long double closer = inputSampleL * 1.57079633;
				if (closer > 1.57079633) closer = 1.57079633;
				if (closer < -1.57079633) closer = -1.57079633;
				inputSampleL = (inputSampleL*(1.0-zoom))+(sin(closer)*zoom);
			} //zooming in will make the body of the sound louder: it's just Density
			if (zoom < 0.0) {
				long double farther = fabs(inputSampleL) * 1.57079633;
				if (farther > 1.57079633) farther = 1.0;
				else farther = 1.0-cos(farther);
				if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(1.0+zoom))-(farther*zoom*1.57079633);
				if (inputSampleL < 0.0) inputSampleL = (inputSampleL*(1.0+zoom))+(farther*zoom*1.57079633);			
			} //zooming out boosts the hottest peaks but cuts back softer stuff
		}
		
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
		
		temp = inputSampleL;
		long double sinew = threshSinew * cos(lastSinewL*lastSinewL);
		if (inputSampleL - lastSinewL > sinew) temp = lastSinewL + sinew;
		if (-(inputSampleL - lastSinewL) > sinew) temp = lastSinewL - sinew;
		lastSinewL = temp;
		inputSampleL = (inputSampleL * (1.0-depthSinew))+(lastSinewL*depthSinew);
		//run Sinew to stop excess slews, but run a dry/wet to allow a range of brights
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSampleL;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

