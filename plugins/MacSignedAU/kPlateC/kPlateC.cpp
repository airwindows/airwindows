/*
*	File:		kPlateC.cpp
*	
*	Version:	1.0
* 
*	Created:	3/31/23
*	
*	Copyright:  Copyright © 2023 Airwindows, Airwindows uses the MIT license
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
	kPlateC.cpp
	
=============================================================================*/
#include "kPlateC.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, kPlateC)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kPlateC::kPlateC
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
kPlateC::kPlateC(AudioUnit component)
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
//	kPlateC::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kPlateC::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kPlateC::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kPlateC::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.maxValue = 10.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 3.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
                break;
			case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 0.5;
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
//	kPlateC::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kPlateC::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 kPlateC::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	kPlateC::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kPlateC::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	kPlateC::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult kPlateC::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____kPlateCEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kPlateC::kPlateCKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		kPlateC::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	iirAL = 0.0;
	iirBL = 0.0;
	
	iirAR = 0.0;
	iirBR = 0.0;
	
	gainIn = gainOutL = gainOutR = 1.0;
		
	for(int count = 0; count < delayA+2; count++) {aAL[count] = 0.0; aAR[count] = 0.0;}
	for(int count = 0; count < delayB+2; count++) {aBL[count] = 0.0; aBR[count] = 0.0;}
	for(int count = 0; count < delayC+2; count++) {aCL[count] = 0.0; aCR[count] = 0.0;}
	for(int count = 0; count < delayD+2; count++) {aDL[count] = 0.0; aDR[count] = 0.0;}
	for(int count = 0; count < delayE+2; count++) {aEL[count] = 0.0; aER[count] = 0.0;}
	for(int count = 0; count < delayF+2; count++) {aFL[count] = 0.0; aFR[count] = 0.0;}
	for(int count = 0; count < delayG+2; count++) {aGL[count] = 0.0; aGR[count] = 0.0;}
	for(int count = 0; count < delayH+2; count++) {aHL[count] = 0.0; aHR[count] = 0.0;}
	for(int count = 0; count < delayI+2; count++) {aIL[count] = 0.0; aIR[count] = 0.0;}
	for(int count = 0; count < delayJ+2; count++) {aJL[count] = 0.0; aJR[count] = 0.0;}
	for(int count = 0; count < delayK+2; count++) {aKL[count] = 0.0; aKR[count] = 0.0;}
	for(int count = 0; count < delayL+2; count++) {aLL[count] = 0.0; aLR[count] = 0.0;}
	for(int count = 0; count < delayM+2; count++) {aML[count] = 0.0; aMR[count] = 0.0;}
	for(int count = 0; count < delayN+2; count++) {aNL[count] = 0.0; aNR[count] = 0.0;}
	for(int count = 0; count < delayO+2; count++) {aOL[count] = 0.0; aOR[count] = 0.0;}
	for(int count = 0; count < delayP+2; count++) {aPL[count] = 0.0; aPR[count] = 0.0;}
	for(int count = 0; count < delayQ+2; count++) {aQL[count] = 0.0; aQR[count] = 0.0;}
	for(int count = 0; count < delayR+2; count++) {aRL[count] = 0.0; aRR[count] = 0.0;}
	for(int count = 0; count < delayS+2; count++) {aSL[count] = 0.0; aSR[count] = 0.0;}
	for(int count = 0; count < delayT+2; count++) {aTL[count] = 0.0; aTR[count] = 0.0;}
	for(int count = 0; count < delayU+2; count++) {aUL[count] = 0.0; aUR[count] = 0.0;}
	for(int count = 0; count < delayV+2; count++) {aVL[count] = 0.0; aVR[count] = 0.0;}
	for(int count = 0; count < delayW+2; count++) {aWL[count] = 0.0; aWR[count] = 0.0;}
	for(int count = 0; count < delayX+2; count++) {aXL[count] = 0.0; aXR[count] = 0.0;}
	for(int count = 0; count < delayY+2; count++) {aYL[count] = 0.0; aYR[count] = 0.0;}
	
	for(int count = 0; count < earlyA+2; count++) {eAL[count] = 0.0; eAR[count] = 0.0;}
	for(int count = 0; count < earlyB+2; count++) {eBL[count] = 0.0; eBR[count] = 0.0;}
	for(int count = 0; count < earlyC+2; count++) {eCL[count] = 0.0; eCR[count] = 0.0;}
	for(int count = 0; count < earlyD+2; count++) {eDL[count] = 0.0; eDR[count] = 0.0;}
	for(int count = 0; count < earlyE+2; count++) {eEL[count] = 0.0; eER[count] = 0.0;}
	for(int count = 0; count < earlyF+2; count++) {eFL[count] = 0.0; eFR[count] = 0.0;}
	for(int count = 0; count < earlyG+2; count++) {eGL[count] = 0.0; eGR[count] = 0.0;}
	for(int count = 0; count < earlyH+2; count++) {eHL[count] = 0.0; eHR[count] = 0.0;}
	for(int count = 0; count < earlyI+2; count++) {eIL[count] = 0.0; eIR[count] = 0.0;}
	
	for(int count = 0; count < predelay+2; count++) {aZL[count] = 0.0; aZR[count] = 0.0;}
	
	feedbackAL = 0.0;
	feedbackBL = 0.0;
	feedbackCL = 0.0;
	feedbackDL = 0.0;
	feedbackEL = 0.0;
	
	previousAL = 0.0;
	previousBL = 0.0;
	previousCL = 0.0;
	previousDL = 0.0;
	previousEL = 0.0;
	
	feedbackER = 0.0;
	feedbackJR = 0.0;
	feedbackOR = 0.0;
	feedbackTR = 0.0;
	feedbackYR = 0.0;
	
	previousAR = 0.0;
	previousBR = 0.0;
	previousCR = 0.0;
	previousDR = 0.0;
	previousER = 0.0;
	
	prevMulchDL = 0.0;
	prevMulchDR = 0.0;
	prevMulchEL = 0.0;
	prevMulchER = 0.0;
	
	prevOutDL = 0.0;
	prevOutDR = 0.0;
	prevOutEL = 0.0;
	prevOutER = 0.0;
	
	prevInDL = 0.0;
	prevInDR = 0.0;
	prevInEL = 0.0;
	prevInER = 0.0;
	
	for(int count = 0; count < 6; count++) {lastRefL[count] = 0.0; lastRefR[count] = 0.0;}
	
	earlyAL = 1;
	earlyBL = 1;
	earlyCL = 1;
	earlyDL = 1;
	earlyEL = 1;
	earlyFL = 1;
	earlyGL = 1;
	earlyHL = 1;
	earlyIL = 1;
	
	earlyAR = 1;
	earlyBR = 1;
	earlyCR = 1;
	earlyDR = 1;
	earlyER = 1;
	earlyFR = 1;
	earlyGR = 1;
	earlyHR = 1;
	earlyIR = 1;
	
	countAL = 1;
	countBL = 1;
	countCL = 1;
	countDL = 1;	
	countEL = 1;
	countFL = 1;
	countGL = 1;
	countHL = 1;
	countIL = 1;
	countJL = 1;
	countKL = 1;
	countLL = 1;
	countML = 1;
	countNL = 1;
	countOL = 1;
	countPL = 1;
	countQL = 1;
	countRL = 1;
	countSL = 1;
	countTL = 1;
	countUL = 1;
	countVL = 1;
	countWL = 1;
	countXL = 1;
	countYL = 1;
	
	countAR = 1;
	countBR = 1;
	countCR = 1;
	countDR = 1;	
	countER = 1;
	countFR = 1;
	countGR = 1;
	countHR = 1;
	countIR = 1;
	countJR = 1;
	countKR = 1;
	countLR = 1;
	countMR = 1;
	countNR = 1;
	countOR = 1;
	countPR = 1;
	countQR = 1;
	countRR = 1;
	countSR = 1;
	countTR = 1;
	countUR = 1;
	countVR = 1;
	countWR = 1;
	countXR = 1;
	countYR = 1;
	
	countZ = 1;
	
	cycle = 0;
	
	for (int x = 0; x < fix_total; x++) {fixA[x] = 0.0; fixB[x] = 0.0; fixC[x] = 0.0; fixD[x] = 0.0;}
	//from ZBandpass, so I can use enums with it
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kPlateC::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		kPlateC::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	double downRate = GetSampleRate()/cycleEnd;
	//we now shift gears between 44.1k and 48k so our tone is the same, slight changes in delay times
	
	double inputPad = GetParameter( kParam_One );
	double regen = (GetParameter(kParam_Two)*0.0415)+0.1;
	regen = (regen*0.0001)+0.00024;
	double iirAmount = (GetParameter( kParam_Three )*0.1)+0.04;
	iirAmount = (iirAmount*1000.0)/downRate;
	double earlyVolume = pow(GetParameter( kParam_Four ),2)*0.5; //predelay to a half-second
	int adjPredelay = (downRate * earlyVolume);
	double wet = GetParameter( kParam_Five )*2.0;
	double dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0;
	if (wet < 0.0) wet = 0.0;
	if (dry > 1.0) dry = 1.0;
	if (dry < 0.0) dry = 0.0;
	//this reverb makes 50% full dry AND full wet, not crossfaded.
	//that's so it can be on submixes without cutting back dry channel when adjusted:
	//unless you go super heavy, you are only adjusting the added verb loudness.
	
	fixA[fix_freq] = 21.0/downRate;
	fixA[fix_reso] = 0.0020736;
	fixD[fix_freq] = 25.0/downRate;
	fixD[fix_reso] = 0.0016384;
	fixB[fix_freq] = (fixA[fix_freq] + fixA[fix_freq] + fixD[fix_freq]) / 3.0;
	fixB[fix_reso] = 0.0019285;
	fixC[fix_freq] = (fixA[fix_freq] + fixD[fix_freq] + fixD[fix_freq]) / 3.0;
	fixC[fix_reso] = 0.0017834;
	
	double K = tan(M_PI * fixA[fix_freq]);
	double norm = 1.0 / (1.0 + K / fixA[fix_reso] + K * K);
	fixA[fix_a0] = K / fixA[fix_reso] * norm;
	fixA[fix_a1] = 0.0;
	fixA[fix_a2] = -fixA[fix_a0];
	fixA[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixA[fix_b2] = (1.0 - K / fixA[fix_reso] + K * K) * norm;
	//stereo biquad bandpasses we can put into the reverb matrix
	
	K = tan(M_PI * fixB[fix_freq]);
	norm = 1.0 / (1.0 + K / fixB[fix_reso] + K * K);
	fixB[fix_a0] = K / fixB[fix_reso] * norm;
	fixB[fix_a1] = 0.0;
	fixB[fix_a2] = -fixB[fix_a0];
	fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixB[fix_b2] = (1.0 - K / fixB[fix_reso] + K * K) * norm;
	//stereo biquad bandpasses we can put into the reverb matrix
	
	K = tan(M_PI * fixC[fix_freq]);
	norm = 1.0 / (1.0 + K / fixC[fix_reso] + K * K);
	fixC[fix_a0] = K / fixC[fix_reso] * norm;
	fixC[fix_a1] = 0.0;
	fixC[fix_a2] = -fixC[fix_a0];
	fixC[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixC[fix_b2] = (1.0 - K / fixC[fix_reso] + K * K) * norm;
	//stereo biquad bandpasses we can put into the reverb matrix
	
	K = tan(M_PI * fixD[fix_freq]);
	norm = 1.0 / (1.0 + K / fixD[fix_reso] + K * K);
	fixD[fix_a0] = K / fixD[fix_reso] * norm;
	fixD[fix_a1] = 0.0;
	fixD[fix_a2] = -fixD[fix_a0];
	fixD[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixD[fix_b2] = (1.0 - K / fixD[fix_reso] + K * K) * norm;
	//stereo biquad bandpasses we can put into the reverb matrix
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a reverb sample
			if (inputPad < 1.0) {
				inputSampleL *= inputPad;
				inputSampleR *= inputPad;
			}
			double outSample;
			outSample = (inputSampleL + prevInDL)*0.5;
			prevInDL = inputSampleL; inputSampleL = outSample;
			outSample = (inputSampleR + prevInDR)*0.5;
			prevInDR = inputSampleR; inputSampleR = outSample;
			//10k filter on input
			
			iirAL = (iirAL * (1.0 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL = inputSampleL - iirAL;
			iirAR = (iirAR * (1.0 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR = inputSampleR - iirAR;
			//600hz highpass on input
			
			inputSampleL *= 0.25; inputSampleR *= 0.25;
			if (gainIn < 0.0078125) gainIn = 0.0078125; if (gainIn > 1.0) gainIn = 1.0;
			//gain of 1,0 gives you a super-clean one, gain of 2 is obviously compressing
			//smaller number is maximum clamping, if too small it'll take a while to bounce back
			inputSampleL *= gainIn; inputSampleR *= gainIn;
			gainIn += sin((fabs(inputSampleL*4)>1)?4:fabs(inputSampleL*4))*pow(inputSampleL,4);
			gainIn += sin((fabs(inputSampleR*4)>1)?4:fabs(inputSampleR*4))*pow(inputSampleR,4);
			//4.71239 radians sined will turn to -1 which is the maximum gain reduction speed
			inputSampleL *= 2.0; inputSampleR *= 2.0;
			//curve! To get a compressed effect that matches a certain other plugin
			//that is too overprocessed for its own good :)

			//begin overdrive
			if (inputSampleL > 1.4137166941154) inputSampleL = 1.4137166941154;
			if (inputSampleL < -1.4137166941154) inputSampleL = -1.4137166941154;
			if (inputSampleL > 0.0) inputSampleL = (inputSampleL/2.0)*(2.8274333882308-inputSampleL);
			else inputSampleL = -(inputSampleL/-2.0)*(2.8274333882308+inputSampleL);
			//BigFastSin channel stage
			if (inputSampleR > 1.4137166941154) inputSampleR = 1.4137166941154;
			if (inputSampleR < -1.4137166941154) inputSampleR = -1.4137166941154;
			if (inputSampleR > 0.0) inputSampleR = (inputSampleR/2.0)*(2.8274333882308-inputSampleR);
			else inputSampleR = -(inputSampleR/-2.0)*(2.8274333882308+inputSampleR);
			//end overdrive
			
			iirBL = (iirBL * (1.0 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL = inputSampleL - iirBL;
			iirBR = (iirBR * (1.0 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR = inputSampleR - iirBR;
			//600hz highpass on input
			
			outSample = (inputSampleL + prevInEL)*0.5;
			prevInEL = inputSampleL; inputSampleL = outSample;
			outSample = (inputSampleR + prevInER)*0.5;
			prevInER = inputSampleR; inputSampleR = outSample;
			//10k filter on input
			
			//begin allpasses
			double oeAL = inputSampleL - (eAL[(earlyAL+1)-((earlyAL+1 > earlyA)?earlyA+1:0)]*0.5);
			double oeBL = inputSampleL - (eBL[(earlyBL+1)-((earlyBL+1 > earlyB)?earlyB+1:0)]*0.5);
			double oeCL = inputSampleL - (eCL[(earlyCL+1)-((earlyCL+1 > earlyC)?earlyC+1:0)]*0.5);
			double oeCR = inputSampleR - (eCR[(earlyCR+1)-((earlyCR+1 > earlyC)?earlyC+1:0)]*0.5);
			double oeFR = inputSampleR - (eFR[(earlyFR+1)-((earlyFR+1 > earlyF)?earlyF+1:0)]*0.5);
			double oeIR = inputSampleR - (eIR[(earlyIR+1)-((earlyIR+1 > earlyI)?earlyI+1:0)]*0.5);
			
			eAL[earlyAL] = oeAL; oeAL *= 0.5;
			eBL[earlyBL] = oeBL; oeBL *= 0.5;
			eCL[earlyCL] = oeCL; oeCL *= 0.5;
			eCR[earlyCR] = oeCR; oeCR *= 0.5;
			eFR[earlyFR] = oeFR; oeFR *= 0.5;
			eIR[earlyIR] = oeIR; oeIR *= 0.5;
			
			earlyAL++; if (earlyAL < 0 || earlyAL > earlyA) earlyAL = 0;
			earlyBL++; if (earlyBL < 0 || earlyBL > earlyB) earlyBL = 0;
			earlyCL++; if (earlyCL < 0 || earlyCL > earlyC) earlyCL = 0;
			earlyCR++; if (earlyCR < 0 || earlyCR > earlyC) earlyCR = 0;
			earlyFR++; if (earlyFR < 0 || earlyFR > earlyF) earlyFR = 0;
			earlyIR++; if (earlyIR < 0 || earlyIR > earlyI) earlyIR = 0;
			
			oeAL += eAL[earlyAL-((earlyAL > earlyA)?earlyA+1:0)];
			oeBL += eBL[earlyBL-((earlyBL > earlyB)?earlyB+1:0)];
			oeCL += eCL[earlyCL-((earlyCL > earlyC)?earlyC+1:0)];
			oeCR += eCR[earlyCR-((earlyCR > earlyC)?earlyC+1:0)];
			oeFR += eFR[earlyFR-((earlyFR > earlyF)?earlyF+1:0)];
			oeIR += eIR[earlyIR-((earlyIR > earlyI)?earlyI+1:0)];
			
			double oeDL = ((oeBL+oeCL)-oeAL) - (eDL[(earlyDL+1)-((earlyDL+1 > earlyD)?earlyD+1:0)]*0.5);
			double oeEL = ((oeAL+oeCL)-oeBL) - (eEL[(earlyEL+1)-((earlyEL+1 > earlyE)?earlyE+1:0)]*0.5);
			double oeFL = ((oeAL+oeBL)-oeCL) - (eFL[(earlyFL+1)-((earlyFL+1 > earlyF)?earlyF+1:0)]*0.5);
			double oeBR = ((oeFR+oeIR)-oeCR) - (eBR[(earlyBR+1)-((earlyBR+1 > earlyB)?earlyB+1:0)]*0.5);
			double oeER = ((oeCR+oeIR)-oeFR) - (eER[(earlyER+1)-((earlyER+1 > earlyE)?earlyE+1:0)]*0.5);
			double oeHR = ((oeCR+oeFR)-oeIR) - (eHR[(earlyHR+1)-((earlyHR+1 > earlyH)?earlyH+1:0)]*0.5);
			
			eDL[earlyDL] = oeDL; oeDL *= 0.5;
			eEL[earlyEL] = oeEL; oeEL *= 0.5;
			eFL[earlyFL] = oeFL; oeFL *= 0.5;
			eBR[earlyBR] = oeBR; oeBR *= 0.5;
			eER[earlyER] = oeER; oeER *= 0.5;
			eHR[earlyHR] = oeHR; oeHR *= 0.5;
			
			earlyDL++; if (earlyDL < 0 || earlyDL > earlyD) earlyDL = 0;
			earlyEL++; if (earlyEL < 0 || earlyEL > earlyE) earlyEL = 0;
			earlyFL++; if (earlyFL < 0 || earlyFL > earlyF) earlyFL = 0;
			earlyBR++; if (earlyBR < 0 || earlyBR > earlyB) earlyBR = 0;
			earlyER++; if (earlyER < 0 || earlyER > earlyE) earlyER = 0;
			earlyHR++; if (earlyHR < 0 || earlyHR > earlyH) earlyHR = 0;
			
			oeDL += eDL[earlyDL-((earlyDL > earlyD)?earlyD+1:0)];
			oeEL += eEL[earlyEL-((earlyEL > earlyE)?earlyE+1:0)];
			oeFL += eFL[earlyFL-((earlyFL > earlyF)?earlyF+1:0)];
			oeBR += eBR[earlyBR-((earlyBR > earlyB)?earlyB+1:0)];
			oeER += eER[earlyER-((earlyER > earlyE)?earlyE+1:0)];
			oeHR += eHR[earlyHR-((earlyHR > earlyH)?earlyH+1:0)];
			
			double oeGL = ((oeEL + oeFL) - oeDL) - (eGL[(earlyGL+1)-((earlyGL+1 > earlyG)?earlyG+1:0)]*0.5);
			double oeHL = ((oeDL + oeFL) - oeEL) - (eHL[(earlyHL+1)-((earlyHL+1 > earlyH)?earlyH+1:0)]*0.5);
			double oeIL = ((oeDL + oeEL) - oeFL) - (eIL[(earlyIL+1)-((earlyIL+1 > earlyI)?earlyI+1:0)]*0.5);
			double oeAR = ((oeER + oeHR) - oeBR) - (eAR[(earlyAR+1)-((earlyAR+1 > earlyA)?earlyA+1:0)]*0.5);
			double oeDR = ((oeBR + oeHR) - oeER) - (eDR[(earlyDR+1)-((earlyDR+1 > earlyD)?earlyD+1:0)]*0.5);
			double oeGR = ((oeBR + oeER) - oeHR) - (eGR[(earlyGR+1)-((earlyGR+1 > earlyG)?earlyG+1:0)]*0.5);
			
			eGL[earlyGL] = oeGL; oeGL *= 0.5;
			eHL[earlyHL] = oeHL; oeHL *= 0.5;
			eIL[earlyIL] = oeIL; oeIL *= 0.5;
			eAR[earlyAR] = oeAR; oeAR *= 0.5;
			eDR[earlyDR] = oeDR; oeDR *= 0.5;
			eGR[earlyGR] = oeGR; oeGR *= 0.5;
			
			earlyGL++; if (earlyGL < 0 || earlyGL > earlyG) earlyGL = 0;
			earlyHL++; if (earlyHL < 0 || earlyHL > earlyH) earlyHL = 0;
			earlyIL++; if (earlyIL < 0 || earlyIL > earlyI) earlyIL = 0;
			earlyAR++; if (earlyAR < 0 || earlyAR > earlyA) earlyAR = 0;
			earlyDR++; if (earlyDR < 0 || earlyDR > earlyD) earlyDR = 0;
			earlyGR++; if (earlyGR < 0 || earlyGR > earlyG) earlyGR = 0;
			
			oeGL += eGL[earlyGL-((earlyGL > earlyG)?earlyG+1:0)];
			oeHL += eHL[earlyHL-((earlyHL > earlyH)?earlyH+1:0)];
			oeIL += eIL[earlyIL-((earlyIL > earlyI)?earlyI+1:0)];
			oeAR += eAR[earlyAR-((earlyAR > earlyA)?earlyA+1:0)];
			oeDR += eDR[earlyDR-((earlyDR > earlyD)?earlyD+1:0)];
			oeGR += eGR[earlyGR-((earlyGR > earlyG)?earlyG+1:0)];
			
			//allpasses predelay
			 aZL[countZ] = (oeGL + oeHL + oeIL)*0.25;
			 aZR[countZ] = (oeAR + oeDR + oeGR)*0.25;
			 countZ++; if (countZ < 0 || countZ > adjPredelay) countZ = 0;
			 inputSampleL = aZL[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			 inputSampleR = aZR[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			 //end allpasses
			
			aAL[countAL] = inputSampleL + (feedbackAL * regen);
			aBL[countBL] = inputSampleL + (feedbackBL * regen);
			aCL[countCL] = inputSampleL + (feedbackCL * regen);
			aDL[countDL] = inputSampleL + (feedbackDL * regen);
			aEL[countEL] = inputSampleL + (feedbackEL * regen);
			
			aER[countER] = inputSampleR + (feedbackER * regen);
			aJR[countJR] = inputSampleR + (feedbackJR * regen);
			aOR[countOR] = inputSampleR + (feedbackOR * regen);
			aTR[countTR] = inputSampleR + (feedbackTR * regen);
			aYR[countYR] = inputSampleR + (feedbackYR * regen);
			
			countAL++; if (countAL < 0 || countAL > delayA) countAL = 0;
			countBL++; if (countBL < 0 || countBL > delayB) countBL = 0;
			countCL++; if (countCL < 0 || countCL > delayC) countCL = 0;
			countDL++; if (countDL < 0 || countDL > delayD) countDL = 0;
			countEL++; if (countEL < 0 || countEL > delayE) countEL = 0;
			
			countER++; if (countER < 0 || countER > delayE) countER = 0;
			countJR++; if (countJR < 0 || countJR > delayJ) countJR = 0;
			countOR++; if (countOR < 0 || countOR > delayO) countOR = 0;
			countTR++; if (countTR < 0 || countTR > delayT) countTR = 0;
			countYR++; if (countYR < 0 || countYR > delayY) countYR = 0;
			
			double outAL = aAL[countAL-((countAL > delayA)?delayA+1:0)];
			double outBL = aBL[countBL-((countBL > delayB)?delayB+1:0)];
			double outCL = aCL[countCL-((countCL > delayC)?delayC+1:0)];
			double outDL = aDL[countDL-((countDL > delayD)?delayD+1:0)];
			double outEL = aEL[countEL-((countEL > delayE)?delayE+1:0)];
			
			double outER = aER[countER-((countER > delayE)?delayE+1:0)];
			double outJR = aJR[countJR-((countJR > delayJ)?delayJ+1:0)];
			double outOR = aOR[countOR-((countOR > delayO)?delayO+1:0)];
			double outTR = aTR[countTR-((countTR > delayT)?delayT+1:0)];
			double outYR = aYR[countYR-((countYR > delayY)?delayY+1:0)];
			
			//-------- one
			
			outSample = (outAL * fixA[fix_a0]) + fixA[fix_sL1];
			fixA[fix_sL1] = (outAL * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sL2];
			fixA[fix_sL2] = (outAL * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
			outAL = outSample; //fixed biquad
			
			outSample = (outER * fixA[fix_a0]) + fixA[fix_sR1];
			fixA[fix_sR1] = (outER * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sR2];
			fixA[fix_sR2] = (outER * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
			outER = outSample; //fixed biquad
						
			//-------- filtered (one path in five, feeding the rest of the matrix
			
			aFL[countFL] = ((outAL*3.0) - ((outBL + outCL + outDL + outEL)*2.0));
			aGL[countGL] = ((outBL*3.0) - ((outAL + outCL + outDL + outEL)*2.0));
			aHL[countHL] = ((outCL*3.0) - ((outAL + outBL + outDL + outEL)*2.0));
			aIL[countIL] = ((outDL*3.0) - ((outAL + outBL + outCL + outEL)*2.0));
			aJL[countJL] = ((outEL*3.0) - ((outAL + outBL + outCL + outDL)*2.0));
			
			aDR[countDR] = ((outER*3.0) - ((outJR + outOR + outTR + outYR)*2.0));
			aIR[countIR] = ((outJR*3.0) - ((outER + outOR + outTR + outYR)*2.0));
			aNR[countNR] = ((outOR*3.0) - ((outER + outJR + outTR + outYR)*2.0));
			aSR[countSR] = ((outTR*3.0) - ((outER + outJR + outOR + outYR)*2.0));
			aXR[countXR] = ((outYR*3.0) - ((outER + outJR + outOR + outTR)*2.0));
			
			countFL++; if (countFL < 0 || countFL > delayF) countFL = 0;
			countGL++; if (countGL < 0 || countGL > delayG) countGL = 0;
			countHL++; if (countHL < 0 || countHL > delayH) countHL = 0;
			countIL++; if (countIL < 0 || countIL > delayI) countIL = 0;
			countJL++; if (countJL < 0 || countJL > delayJ) countJL = 0;
			
			countDR++; if (countDR < 0 || countDR > delayD) countDR = 0;
			countIR++; if (countIR < 0 || countIR > delayI) countIR = 0;
			countNR++; if (countNR < 0 || countNR > delayN) countNR = 0;
			countSR++; if (countSR < 0 || countSR > delayS) countSR = 0;
			countXR++; if (countXR < 0 || countXR > delayX) countXR = 0;
			
			double outFL = aFL[countFL-((countFL > delayF)?delayF+1:0)];
			double outGL = aGL[countGL-((countGL > delayG)?delayG+1:0)];
			double outHL = aHL[countHL-((countHL > delayH)?delayH+1:0)];
			double outIL = aIL[countIL-((countIL > delayI)?delayI+1:0)];
			double outJL = aJL[countJL-((countJL > delayJ)?delayJ+1:0)];
			
			double outDR = aDR[countDR-((countDR > delayD)?delayD+1:0)];
			double outIR = aIR[countIR-((countIR > delayI)?delayI+1:0)];
			double outNR = aNR[countNR-((countNR > delayN)?delayN+1:0)];
			double outSR = aSR[countSR-((countSR > delayS)?delayS+1:0)];
			double outXR = aXR[countXR-((countXR > delayX)?delayX+1:0)];
			
			//-------- mulch
			
			outSample = (outFL * fixB[fix_a0]) + fixB[fix_sL1];
			fixB[fix_sL1] = (outFL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
			fixB[fix_sL2] = (outFL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
			outFL = outSample; //fixed biquad
			
			outSample = (outDR * fixB[fix_a0]) + fixB[fix_sR1];
			fixB[fix_sR1] = (outDR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
			fixB[fix_sR2] = (outDR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
			outDR = outSample; //fixed biquad			
						
			//-------- two
			
			aKL[countKL] = ((outFL*3.0) - ((outGL + outHL + outIL + outJL)*2.0));
			aLL[countLL] = ((outGL*3.0) - ((outFL + outHL + outIL + outJL)*2.0));
			aML[countML] = ((outHL*3.0) - ((outFL + outGL + outIL + outJL)*2.0));
			aNL[countNL] = ((outIL*3.0) - ((outFL + outGL + outHL + outJL)*2.0));
			aOL[countOL] = ((outJL*3.0) - ((outFL + outGL + outHL + outIL)*2.0));
			
			aCR[countCR] = ((outDR*3.0) - ((outIR + outNR + outSR + outXR)*2.0));
			aHR[countHR] = ((outIR*3.0) - ((outDR + outNR + outSR + outXR)*2.0));
			aMR[countMR] = ((outNR*3.0) - ((outDR + outIR + outSR + outXR)*2.0));
			aRR[countRR] = ((outSR*3.0) - ((outDR + outIR + outNR + outXR)*2.0));
			aWR[countWR] = ((outXR*3.0) - ((outDR + outIR + outNR + outSR)*2.0));
			
			countKL++; if (countKL < 0 || countKL > delayK) countKL = 0;
			countLL++; if (countLL < 0 || countLL > delayL) countLL = 0;
			countML++; if (countML < 0 || countML > delayM) countML = 0;
			countNL++; if (countNL < 0 || countNL > delayN) countNL = 0;
			countOL++; if (countOL < 0 || countOL > delayO) countOL = 0;
			
			countCR++; if (countCR < 0 || countCR > delayC) countCR = 0;
			countHR++; if (countHR < 0 || countHR > delayH) countHR = 0;
			countMR++; if (countMR < 0 || countMR > delayM) countMR = 0;
			countRR++; if (countRR < 0 || countRR > delayR) countRR = 0;
			countWR++; if (countWR < 0 || countWR > delayW) countWR = 0;
			
			double outKL = aKL[countKL-((countKL > delayK)?delayK+1:0)];
			double outLL = aLL[countLL-((countLL > delayL)?delayL+1:0)];
			double outML = aML[countML-((countML > delayM)?delayM+1:0)];
			double outNL = aNL[countNL-((countNL > delayN)?delayN+1:0)];
			double outOL = aOL[countOL-((countOL > delayO)?delayO+1:0)];
			
			double outCR = aCR[countCR-((countCR > delayC)?delayC+1:0)];
			double outHR = aHR[countHR-((countHR > delayH)?delayH+1:0)];
			double outMR = aMR[countMR-((countMR > delayM)?delayM+1:0)];
			double outRR = aRR[countRR-((countRR > delayR)?delayR+1:0)];
			double outWR = aWR[countWR-((countWR > delayW)?delayW+1:0)];
			
			//-------- mulch
			
			outSample = (outKL * fixC[fix_a0]) + fixC[fix_sL1];
			fixC[fix_sL1] = (outKL * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
			fixC[fix_sL2] = (outKL * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
			outKL = outSample; //fixed biquad
			
			outSample = (outCR * fixC[fix_a0]) + fixC[fix_sR1];
			fixC[fix_sR1] = (outCR * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sR2];
			fixC[fix_sR2] = (outCR * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
			outCR = outSample; //fixed biquad			
			
			//-------- three
			
			aPL[countPL] = ((outKL*3.0) - ((outLL + outML + outNL + outOL)*2.0));
			aQL[countQL] = ((outLL*3.0) - ((outKL + outML + outNL + outOL)*2.0));
			aRL[countRL] = ((outML*3.0) - ((outKL + outLL + outNL + outOL)*2.0));
			aSL[countSL] = ((outNL*3.0) - ((outKL + outLL + outML + outOL)*2.0));
			aTL[countTL] = ((outOL*3.0) - ((outKL + outLL + outML + outNL)*2.0));
			
			aBR[countBR] = ((outCR*3.0) - ((outHR + outMR + outRR + outWR)*2.0));
			aGR[countGR] = ((outHR*3.0) - ((outCR + outMR + outRR + outWR)*2.0));
			aLR[countLR] = ((outMR*3.0) - ((outCR + outHR + outRR + outWR)*2.0));
			aQR[countQR] = ((outRR*3.0) - ((outCR + outHR + outMR + outWR)*2.0));
			aVR[countVR] = ((outWR*3.0) - ((outCR + outHR + outMR + outRR)*2.0));
			
			countPL++; if (countPL < 0 || countPL > delayP) countPL = 0;
			countQL++; if (countQL < 0 || countQL > delayQ) countQL = 0;
			countRL++; if (countRL < 0 || countRL > delayR) countRL = 0;
			countSL++; if (countSL < 0 || countSL > delayS) countSL = 0;
			countTL++; if (countTL < 0 || countTL > delayT) countTL = 0;
			
			countBR++; if (countBR < 0 || countBR > delayB) countBR = 0;
			countGR++; if (countGR < 0 || countGR > delayG) countGR = 0;
			countLR++; if (countLR < 0 || countLR > delayL) countLR = 0;
			countQR++; if (countQR < 0 || countQR > delayQ) countQR = 0;
			countVR++; if (countVR < 0 || countVR > delayV) countVR = 0;
			
			double outPL = aPL[countPL-((countPL > delayP)?delayP+1:0)];
			double outQL = aQL[countQL-((countQL > delayQ)?delayQ+1:0)];
			double outRL = aRL[countRL-((countRL > delayR)?delayR+1:0)];
			double outSL = aSL[countSL-((countSL > delayS)?delayS+1:0)];
			double outTL = aTL[countTL-((countTL > delayT)?delayT+1:0)];
			
			double outBR = aBR[countBR-((countBR > delayB)?delayB+1:0)];
			double outGR = aGR[countGR-((countGR > delayG)?delayG+1:0)];
			double outLR = aLR[countLR-((countLR > delayL)?delayL+1:0)];
			double outQR = aQR[countQR-((countQR > delayQ)?delayQ+1:0)];
			double outVR = aVR[countVR-((countVR > delayV)?delayV+1:0)];
			
			//-------- mulch
			
			outSample = (outPL * fixD[fix_a0]) + fixD[fix_sL1];
			fixD[fix_sL1] = (outPL * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
			fixD[fix_sL2] = (outPL * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
			outPL = outSample; //fixed biquad
			
			outSample = (outBR * fixD[fix_a0]) + fixD[fix_sR1];
			fixD[fix_sR1] = (outBR * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sR2];
			fixD[fix_sR2] = (outBR * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
			outBR = outSample; //fixed biquad			
			
			outSample = (outQL + prevMulchDL)*0.5;
			prevMulchDL = outQL; outQL = outSample;
			outSample = (outGR + prevMulchDR)*0.5;
			prevMulchDR = outGR; outGR = outSample;
			
			//-------- four
			
			aUL[countUL] = ((outPL*3.0) - ((outQL + outRL + outSL + outTL)*2.0));
			aVL[countVL] = ((outQL*3.0) - ((outPL + outRL + outSL + outTL)*2.0));
			aWL[countWL] = ((outRL*3.0) - ((outPL + outQL + outSL + outTL)*2.0));
			aXL[countXL] = ((outSL*3.0) - ((outPL + outQL + outRL + outTL)*2.0));
			aYL[countYL] = ((outTL*3.0) - ((outPL + outQL + outRL + outSL)*2.0));
			
			aAR[countAR] = ((outBR*3.0) - ((outGR + outLR + outQR + outVR)*2.0));
			aFR[countFR] = ((outGR*3.0) - ((outBR + outLR + outQR + outVR)*2.0));
			aKR[countKR] = ((outLR*3.0) - ((outBR + outGR + outQR + outVR)*2.0));
			aPR[countPR] = ((outQR*3.0) - ((outBR + outGR + outLR + outVR)*2.0));
			aUR[countUR] = ((outVR*3.0) - ((outBR + outGR + outLR + outQR)*2.0));
			
			countUL++; if (countUL < 0 || countUL > delayU) countUL = 0;
			countVL++; if (countVL < 0 || countVL > delayV) countVL = 0;
			countWL++; if (countWL < 0 || countWL > delayW) countWL = 0;
			countXL++; if (countXL < 0 || countXL > delayX) countXL = 0;
			countYL++; if (countYL < 0 || countYL > delayY) countYL = 0;
			
			countAR++; if (countAR < 0 || countAR > delayA) countAR = 0;
			countFR++; if (countFR < 0 || countFR > delayF) countFR = 0;
			countKR++; if (countKR < 0 || countKR > delayK) countKR = 0;
			countPR++; if (countPR < 0 || countPR > delayP) countPR = 0;
			countUR++; if (countUR < 0 || countUR > delayU) countUR = 0;
			
			double outUL = aUL[countUL-((countUL > delayU)?delayU+1:0)];
			double outVL = aVL[countVL-((countVL > delayV)?delayV+1:0)];
			double outWL = aWL[countWL-((countWL > delayW)?delayW+1:0)];
			double outXL = aXL[countXL-((countXL > delayX)?delayX+1:0)];
			double outYL = aYL[countYL-((countYL > delayY)?delayY+1:0)];
			
			double outAR = aAR[countAR-((countAR > delayA)?delayA+1:0)];
			double outFR = aFR[countFR-((countFR > delayF)?delayF+1:0)];
			double outKR = aKR[countKR-((countKR > delayK)?delayK+1:0)];
			double outPR = aPR[countPR-((countPR > delayP)?delayP+1:0)];
			double outUR = aUR[countUR-((countUR > delayU)?delayU+1:0)];
			
			//-------- mulch
			
			outSample = (outVL + prevMulchEL)*0.5;
			prevMulchEL = outVL; outVL = outSample;
			outSample = (outFR + prevMulchER)*0.5;
			prevMulchER = outFR; outFR = outSample;
			
			//-------- five
			
			feedbackER = ((outUL*3.0) - ((outVL + outWL + outXL + outYL)*2.0));
			feedbackAL = ((outAR*3.0) - ((outFR + outKR + outPR + outUR)*2.0));
			feedbackJR = ((outVL*3.0) - ((outUL + outWL + outXL + outYL)*2.0));
			feedbackBL = ((outFR*3.0) - ((outAR + outKR + outPR + outUR)*2.0));
			feedbackCL = ((outWL*3.0) - ((outUL + outVL + outXL + outYL)*2.0));
			feedbackOR = ((outKR*3.0) - ((outAR + outFR + outPR + outUR)*2.0));
			feedbackDL = ((outXL*3.0) - ((outUL + outVL + outWL + outYL)*2.0));
			feedbackTR = ((outPR*3.0) - ((outAR + outFR + outKR + outUR)*2.0));
			feedbackEL = ((outYL*3.0) - ((outUL + outVL + outWL + outXL)*2.0));
			feedbackYR = ((outUR*3.0) - ((outAR + outFR + outKR + outPR)*2.0));
			//which we need to feed back into the input again, a bit
			
			inputSampleL = (outUL + outVL + outWL + outXL + outYL)*0.0016;
			inputSampleR = (outAR + outFR + outKR + outPR + outUR)*0.0016;
			//and take the final combined sum of outputs, corrected for Householder gain
			
			inputSampleL *= 0.5; inputSampleR *= 0.5;
			if (gainOutL < 0.0078125) gainOutL = 0.0078125; if (gainOutL > 1.0) gainOutL = 1.0;
			if (gainOutR < 0.0078125) gainOutR = 0.0078125; if (gainOutR > 1.0) gainOutR = 1.0;
			//gain of 1,0 gives you a super-clean one, gain of 2 is obviously compressing
			//smaller number is maximum clamping, if too small it'll take a while to bounce back
			inputSampleL *= gainOutL; inputSampleR *= gainOutR;
			gainOutL += sin((fabs(inputSampleL*4)>1)?4:fabs(inputSampleL*4))*pow(inputSampleL,4);
			gainOutR += sin((fabs(inputSampleR*4)>1)?4:fabs(inputSampleR*4))*pow(inputSampleR,4);
			//4.71239 radians sined will turn to -1 which is the maximum gain reduction speed
			inputSampleL *= 2.0; inputSampleR *= 2.0;
			//curve! To get a compressed effect that matches a certain other plugin
			//that is too overprocessed for its own good :)			

			outSample = (inputSampleL + prevOutDL)*0.5;
			prevOutDL = inputSampleL; inputSampleL = outSample;
			outSample = (inputSampleR + prevOutDR)*0.5;
			prevOutDR = inputSampleR; inputSampleR = outSample;
			outSample = (inputSampleL + prevOutEL)*0.5;
			prevOutEL = inputSampleL; inputSampleL = outSample;
			outSample = (inputSampleR + prevOutER)*0.5;
			prevOutER = inputSampleR; inputSampleR = outSample;
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		
		if (inputSampleL > 2.8) inputSampleL = 2.8;
		if (inputSampleL < -2.8) inputSampleL = -2.8;
		if (inputSampleR > 2.8) inputSampleR = 2.8;
		if (inputSampleR < -2.8) inputSampleR = -2.8;//clip BigFastArcSin harder
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*2.0)/(2.8274333882308-inputSampleL);
		else inputSampleL = -(inputSampleL*-2.0)/(2.8274333882308+inputSampleL);
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*2.0)/(2.8274333882308-inputSampleR);
		else inputSampleR = -(inputSampleR*-2.0)/(2.8274333882308+inputSampleR);
		//BigFastArcSin output stage
		
		
		if (wet < 1.0) {inputSampleL *= wet; inputSampleR *= wet;}
		if (dry < 1.0) {drySampleL *= dry; drySampleR *= dry;}
		inputSampleL += drySampleL; inputSampleR += drySampleR;
		//this is our submix verb dry/wet: 0.5 is BOTH at FULL VOLUME
		//purpose is that, if you're adding verb, you're not altering other balances
		
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

