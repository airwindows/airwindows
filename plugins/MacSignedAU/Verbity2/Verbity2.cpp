/*
*	File:		Verbity2.cpp
*	
*	Version:	1.0
* 
*	Created:	2/1/23
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
	Verbity2.cpp
	
=============================================================================*/
#include "Verbity2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Verbity2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Verbity2::Verbity2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Verbity2::Verbity2(AudioUnit component)
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
//	Verbity2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Verbity2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Verbity2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Verbity2::GetParameterInfo(AudioUnitScope		inScope,
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
//	Verbity2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Verbity2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 Verbity2::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	Verbity2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Verbity2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Verbity2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Verbity2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Verbity2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Verbity2::Verbity2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Verbity2::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	iirAL = 0.0;
	iirBL = 0.0;
	
	iirAR = 0.0;
	iirBR = 0.0;
	
	//for(int count = 0; count < 5190; count++) {aZL[count] = 0.0; aZR[count] = 0.0;}
	
	for(int count = 0; count < 5004; count++) {aAL[count] = 0.0; aAR[count] = 0.0;} //AFKPU
	for(int count = 0; count < 4952; count++) {aFL[count] = 0.0; aFR[count] = 0.0;}
	for(int count = 0; count < 4920; count++) {aKL[count] = 0.0; aKR[count] = 0.0;}
	for(int count = 0; count < 4800; count++) {aPL[count] = 0.0; aPR[count] = 0.0;}
	for(int count = 0; count < 4752; count++) {aUL[count] = 0.0; aUR[count] = 0.0;}
	
	for(int count = 0; count < 4350; count++) {aBL[count] = 0.0; aBR[count] = 0.0;} //BGLQV
	for(int count = 0; count < 4158; count++) {aGL[count] = 0.0; aGR[count] = 0.0;}
	for(int count = 0; count < 3930; count++) {aLL[count] = 0.0; aLR[count] = 0.0;}
	for(int count = 0; count < 3530; count++) {aQL[count] = 0.0; aQR[count] = 0.0;}
	for(int count = 0; count < 3330; count++) {aVL[count] = 0.0; aVR[count] = 0.0;}
	
	for(int count = 0; count < 3324; count++) {aCL[count] = 0.0; aCR[count] = 0.0;} //CHMRW
	for(int count = 0; count < 2792; count++) {aHL[count] = 0.0; aHR[count] = 0.0;}
	for(int count = 0; count < 2768; count++) {aML[count] = 0.0; aMR[count] = 0.0;}
	for(int count = 0; count < 2390; count++) {aRL[count] = 0.0; aRR[count] = 0.0;}
	for(int count = 0; count < 2348; count++) {aWL[count] = 0.0; aWR[count] = 0.0;}
	
	for(int count = 0; count < 2142; count++) {aDL[count] = 0.0; aDR[count] = 0.0;} //DINSX
	for(int count = 0; count < 1812; count++) {aIL[count] = 0.0; aIR[count] = 0.0;}
	for(int count = 0; count < 1734; count++) {aNL[count] = 0.0; aNR[count] = 0.0;}
	for(int count = 0; count < 1172; count++) {aSL[count] = 0.0; aSR[count] = 0.0;}
	for(int count = 0; count < 788; count++) {aXL[count] = 0.0; aXR[count] = 0.0;}
	
	for(int count = 0; count < 678; count++) {aEL[count] = 0.0; aER[count] = 0.0;} //EJOTY
	for(int count = 0; count < 644; count++) {aJL[count] = 0.0; aJR[count] = 0.0;}
	for(int count = 0; count < 440; count++) {aOL[count] = 0.0; aOR[count] = 0.0;}
	for(int count = 0; count < 350; count++) {aTL[count] = 0.0; aTR[count] = 0.0;}
	for(int count = 0; count < 282; count++) {aYL[count] = 0.0; aYR[count] = 0.0;}
	
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
	
	feedbackAR = 0.0;
	feedbackBR = 0.0;
	feedbackCR = 0.0;
	feedbackDR = 0.0;
	feedbackER = 0.0;
	
	previousAR = 0.0;
	previousBR = 0.0;
	previousCR = 0.0;
	previousDR = 0.0;
	previousER = 0.0;
	
	for(int count = 0; count < 6; count++) {lastRefL[count] = 0.0; lastRefR[count] = 0.0;}
	
	countA = 1;
	countB = 1;
	countC = 1;
	countD = 1;	
	countE = 1;
	countF = 1;
	countG = 1;
	countH = 1;
	countI = 1;
	countJ = 1;
	countK = 1;
	countL = 1;
	countM = 1;
	countN = 1;
	countO = 1;
	countP = 1;
	countQ = 1;
	countR = 1;
	countS = 1;
	countT = 1;
	countU = 1;
	countV = 1;
	countW = 1;
	countX = 1;
	countY = 1;
	countZ = 1;
	
	cycle = 0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;	
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Verbity2::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		Verbity2::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	
	//mulch is tone control, darken to obscure the Chrome Oxide, not as much highpass
	Float64 size = (pow(GetParameter( kParam_One ),2.0)*0.9)+0.1;
	Float64 regen = (1.0-pow(1.0-GetParameter( kParam_Two ),3.0))*0.00032;
	Float64 mulchSetting = GetParameter( kParam_Three );
	Float64 lowpass = (1.0-(mulchSetting*0.75))/sqrt(overallscale);
	Float64 highpass = (0.007+(mulchSetting*0.022))/sqrt(overallscale);
	Float64 interpolateMax = 0.07+(mulchSetting*0.4);
	Float64 wet = GetParameter( kParam_Four )*2.0;
	Float64 dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0;
	if (wet < 0.0) wet = 0.0;
	if (dry > 1.0) dry = 1.0;
	if (dry < 0.0) dry = 0.0;
	//this reverb makes 50% full dry AND full wet, not crossfaded.
	//that's so it can be on submixes without cutting back dry channel when adjusted:
	//unless you go super heavy, you are only adjusting the added verb loudness.
	
	//delayZ = 5189.0*size; //z can be predelay
	
	delayA = 5003.0*size;
	delayF = 4951.0*size;
	delayK = 4919.0*size;
	delayP = 4799.0*size;
	delayU = 4751.0*size;
	
	delayB = 4349.0*size;
	delayG = 4157.0*size;
	delayL = 3929.0*size;
	delayQ = 3529.0*size;
	delayV = 3329.0*size;
	
	delayC = 3323.0*size;
	delayH = 2791.0*size;
	delayM = 2767.0*size;
	delayR = 2389.0*size;
	delayW = 2347.0*size;
	
	delayD = 2141.0*size;
	delayI = 1811.0*size;
	delayN = 1733.0*size;
	delayS = 1171.0*size;
	delayX = 787.0*size;
	
	delayE = 677.0*size;
	delayJ = 643.0*size;
	delayO = 439.0*size;
	delayT = 349.0*size;
	delayY = 281.0*size; //balanced primes of order two
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (fabs(iirAL)<1.18e-37) iirAL = 0.0;
		iirAL = (iirAL*(1.0-highpass))+(inputSampleL*highpass); inputSampleL -= iirAL;
		if (fabs(iirAR)<1.18e-37) iirAR = 0.0;
		iirAR = (iirAR*(1.0-highpass))+(inputSampleR*highpass); inputSampleR -= iirAR;
		//first filter is highpass
		
		double interpolateL = interpolateMax + (interpolateMax * (double(fpdL)/UINT32_MAX));
		double interpolateR = interpolateMax + (interpolateMax * (double(fpdR)/UINT32_MAX));
		//we have our feedback soften also working as Chrome Oxide soften/noise
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a reverb sample
			feedbackAL = (feedbackAL*(1.0-interpolateL))+(previousAL*interpolateL); previousAL = feedbackAL;
			feedbackBL = (feedbackBL*(1.0-interpolateL))+(previousBL*interpolateL); previousBL = feedbackBL;
			feedbackCL = (feedbackCL*(1.0-interpolateL))+(previousCL*interpolateL); previousCL = feedbackCL;
			feedbackDL = (feedbackDL*(1.0-interpolateL))+(previousDL*interpolateL); previousDL = feedbackDL;
			feedbackEL = (feedbackEL*(1.0-interpolateL))+(previousEL*interpolateL); previousEL = feedbackEL;
			feedbackAR = (feedbackAR*(1.0-interpolateR))+(previousAR*interpolateR); previousAR = feedbackAR;
			feedbackBR = (feedbackBR*(1.0-interpolateR))+(previousBR*interpolateR); previousBR = feedbackBR;
			feedbackCR = (feedbackCR*(1.0-interpolateR))+(previousCR*interpolateR); previousCR = feedbackCR;
			feedbackDR = (feedbackDR*(1.0-interpolateR))+(previousDR*interpolateR); previousDR = feedbackDR;
			feedbackER = (feedbackER*(1.0-interpolateR))+(previousER*interpolateR); previousER = feedbackER;
			
			//	aZL[countZ] = inputSampleL;
			//	aZR[countZ] = inputSampleR;
			//	countZ++; if (countZ < 0 || countZ > delayZ) countZ = 0;
			//	inputSampleL = aZL[countZ-((countZ > delayZ)?delayZ+1:0)];
			//	inputSampleR = aZR[countZ-((countZ > delayZ)?delayZ+1:0)];
			//predelay 
			
			aAL[countA] = inputSampleL + (feedbackAL * (regen*(1.0-fabs(feedbackAL*regen))));
			aBL[countB] = inputSampleL + (feedbackBL * (regen*(1.0-fabs(feedbackBL*regen))));
			aCL[countC] = inputSampleL + (feedbackCL * (regen*(1.0-fabs(feedbackCL*regen))));
			aDL[countD] = inputSampleL + (feedbackDL * (regen*(1.0-fabs(feedbackDL*regen))));
			aEL[countE] = inputSampleL + (feedbackEL * (regen*(1.0-fabs(feedbackEL*regen))));
			
			aAR[countA] = inputSampleR + (feedbackAR * (regen*(1.0-fabs(feedbackAR*regen))));
			aBR[countB] = inputSampleR + (feedbackBR * (regen*(1.0-fabs(feedbackBR*regen))));
			aCR[countC] = inputSampleR + (feedbackCR * (regen*(1.0-fabs(feedbackCR*regen))));
			aDR[countD] = inputSampleR + (feedbackDR * (regen*(1.0-fabs(feedbackDR*regen))));
			aER[countE] = inputSampleR + (feedbackER * (regen*(1.0-fabs(feedbackER*regen))));
			
			countA++; if (countA < 0 || countA > delayA) countA = 0;
			countB++; if (countB < 0 || countB > delayB) countB = 0;
			countC++; if (countC < 0 || countC > delayC) countC = 0;
			countD++; if (countD < 0 || countD > delayD) countD = 0;
			countE++; if (countE < 0 || countE > delayE) countE = 0;
			
			double outAL = aAL[countA-((countA > delayA)?delayA+1:0)];
			double outBL = aBL[countB-((countB > delayB)?delayB+1:0)];
			double outCL = aCL[countC-((countC > delayC)?delayC+1:0)];
			double outDL = aDL[countD-((countD > delayD)?delayD+1:0)];
			double outEL = aEL[countE-((countE > delayE)?delayE+1:0)];
			
			double outAR = aAR[countA-((countA > delayA)?delayA+1:0)];
			double outBR = aBR[countB-((countB > delayB)?delayB+1:0)];
			double outCR = aCR[countC-((countC > delayC)?delayC+1:0)];
			double outDR = aDR[countD-((countD > delayD)?delayD+1:0)];
			double outER = aER[countE-((countE > delayE)?delayE+1:0)];
			
			//-------- one
			
			aFL[countF] = ((outAL*3.0) - ((outBL + outCL + outDL + outEL)*2.0));
			aGL[countG] = ((outBL*3.0) - ((outAL + outCL + outDL + outEL)*2.0));
			aHL[countH] = ((outCL*3.0) - ((outAL + outBL + outDL + outEL)*2.0));
			aIL[countI] = ((outDL*3.0) - ((outAL + outBL + outCL + outEL)*2.0));
			aJL[countJ] = ((outEL*3.0) - ((outAL + outBL + outCL + outDL)*2.0));
			
			aFR[countF] = ((outAR*3.0) - ((outBR + outCR + outDR + outER)*2.0));
			aGR[countG] = ((outBR*3.0) - ((outAR + outCR + outDR + outER)*2.0));
			aHR[countH] = ((outCR*3.0) - ((outAR + outBR + outDR + outER)*2.0));
			aIR[countI] = ((outDR*3.0) - ((outAR + outBR + outCR + outER)*2.0));
			aJR[countJ] = ((outER*3.0) - ((outAR + outBR + outCR + outDR)*2.0));
			
			countF++; if (countF < 0 || countF > delayF) countF = 0;
			countG++; if (countG < 0 || countG > delayG) countG = 0;
			countH++; if (countH < 0 || countH > delayH) countH = 0;
			countI++; if (countI < 0 || countI > delayI) countI = 0;
			countJ++; if (countJ < 0 || countJ > delayJ) countJ = 0;
			
			double outFL = aFL[countF-((countF > delayF)?delayF+1:0)];
			double outGL = aGL[countG-((countG > delayG)?delayG+1:0)];
			double outHL = aHL[countH-((countH > delayH)?delayH+1:0)];
			double outIL = aIL[countI-((countI > delayI)?delayI+1:0)];
			double outJL = aJL[countJ-((countJ > delayJ)?delayJ+1:0)];
			
			double outFR = aFR[countF-((countF > delayF)?delayF+1:0)];
			double outGR = aGR[countG-((countG > delayG)?delayG+1:0)];
			double outHR = aHR[countH-((countH > delayH)?delayH+1:0)];
			double outIR = aIR[countI-((countI > delayI)?delayI+1:0)];
			double outJR = aJR[countJ-((countJ > delayJ)?delayJ+1:0)];
			
			//-------- two
			
			aKL[countK] = ((outFL*3.0) - ((outGL + outHL + outIL + outJL)*2.0));
			aLL[countL] = ((outGL*3.0) - ((outFL + outHL + outIL + outJL)*2.0));
			aML[countM] = ((outHL*3.0) - ((outFL + outGL + outIL + outJL)*2.0));
			aNL[countN] = ((outIL*3.0) - ((outFL + outGL + outHL + outJL)*2.0));
			aOL[countO] = ((outJL*3.0) - ((outFL + outGL + outHL + outIL)*2.0));
			
			aKR[countK] = ((outFR*3.0) - ((outGR + outHR + outIR + outJR)*2.0));
			aLR[countL] = ((outGR*3.0) - ((outFR + outHR + outIR + outJR)*2.0));
			aMR[countM] = ((outHR*3.0) - ((outFR + outGR + outIR + outJR)*2.0));
			aNR[countN] = ((outIR*3.0) - ((outFR + outGR + outHR + outJR)*2.0));
			aOR[countO] = ((outJR*3.0) - ((outFR + outGR + outHR + outIR)*2.0));
			
			countK++; if (countK < 0 || countK > delayK) countK = 0;
			countL++; if (countL < 0 || countL > delayL) countL = 0;
			countM++; if (countM < 0 || countM > delayM) countM = 0;
			countN++; if (countN < 0 || countN > delayN) countN = 0;
			countO++; if (countO < 0 || countO > delayO) countO = 0;
			
			double outKL = aKL[countK-((countK > delayK)?delayK+1:0)];
			double outLL = aLL[countL-((countL > delayL)?delayL+1:0)];
			double outML = aML[countM-((countM > delayM)?delayM+1:0)];
			double outNL = aNL[countN-((countN > delayN)?delayN+1:0)];
			double outOL = aOL[countO-((countO > delayO)?delayO+1:0)];
			
			double outKR = aKR[countK-((countK > delayK)?delayK+1:0)];
			double outLR = aLR[countL-((countL > delayL)?delayL+1:0)];
			double outMR = aMR[countM-((countM > delayM)?delayM+1:0)];
			double outNR = aNR[countN-((countN > delayN)?delayN+1:0)];
			double outOR = aOR[countO-((countO > delayO)?delayO+1:0)];
			
			//-------- three
			
			aPL[countP] = ((outKL*3.0) - ((outLL + outML + outNL + outOL)*2.0));
			aQL[countQ] = ((outLL*3.0) - ((outKL + outML + outNL + outOL)*2.0));
			aRL[countR] = ((outML*3.0) - ((outKL + outLL + outNL + outOL)*2.0));
			aSL[countS] = ((outNL*3.0) - ((outKL + outLL + outML + outOL)*2.0));
			aTL[countT] = ((outOL*3.0) - ((outKL + outLL + outML + outNL)*2.0));
			
			aPR[countP] = ((outKR*3.0) - ((outLR + outMR + outNR + outOR)*2.0));
			aQR[countQ] = ((outLR*3.0) - ((outKR + outMR + outNR + outOR)*2.0));
			aRR[countR] = ((outMR*3.0) - ((outKR + outLR + outNR + outOR)*2.0));
			aSR[countS] = ((outNR*3.0) - ((outKR + outLR + outMR + outOR)*2.0));
			aTR[countT] = ((outOR*3.0) - ((outKR + outLR + outMR + outNR)*2.0));
			
			countP++; if (countP < 0 || countP > delayP) countP = 0;
			countQ++; if (countQ < 0 || countQ > delayQ) countQ = 0;
			countR++; if (countR < 0 || countR > delayR) countR = 0;
			countS++; if (countS < 0 || countS > delayS) countS = 0;
			countT++; if (countT < 0 || countT > delayT) countT = 0;
			
			double outPL = aPL[countP-((countP > delayP)?delayP+1:0)];
			double outQL = aQL[countQ-((countQ > delayQ)?delayQ+1:0)];
			double outRL = aRL[countR-((countR > delayR)?delayR+1:0)];
			double outSL = aSL[countS-((countS > delayS)?delayS+1:0)];
			double outTL = aTL[countT-((countT > delayT)?delayT+1:0)];
			
			double outPR = aPR[countP-((countP > delayP)?delayP+1:0)];
			double outQR = aQR[countQ-((countQ > delayQ)?delayQ+1:0)];
			double outRR = aRR[countR-((countR > delayR)?delayR+1:0)];
			double outSR = aSR[countS-((countS > delayS)?delayS+1:0)];
			double outTR = aTR[countT-((countT > delayT)?delayT+1:0)];
			
			//-------- four
			
			aUL[countU] = ((outPL*3.0) - ((outQL + outRL + outSL + outTL)*2.0));
			aVL[countV] = ((outQL*3.0) - ((outPL + outRL + outSL + outTL)*2.0));
			aWL[countW] = ((outRL*3.0) - ((outPL + outQL + outSL + outTL)*2.0));
			aXL[countX] = ((outSL*3.0) - ((outPL + outQL + outRL + outTL)*2.0));
			aYL[countY] = ((outTL*3.0) - ((outPL + outQL + outRL + outSL)*2.0));
			
			aUR[countU] = ((outPR*3.0) - ((outQR + outRR + outSR + outTR)*2.0));
			aVR[countV] = ((outQR*3.0) - ((outPR + outRR + outSR + outTR)*2.0));
			aWR[countW] = ((outRR*3.0) - ((outPR + outQR + outSR + outTR)*2.0));
			aXR[countX] = ((outSR*3.0) - ((outPR + outQR + outRR + outTR)*2.0));
			aYR[countY] = ((outTR*3.0) - ((outPR + outQR + outRR + outSR)*2.0));
			
			countU++; if (countU < 0 || countU > delayU) countU = 0;
			countV++; if (countV < 0 || countV > delayV) countV = 0;
			countW++; if (countW < 0 || countW > delayW) countW = 0;
			countX++; if (countX < 0 || countX > delayX) countX = 0;
			countY++; if (countY < 0 || countY > delayY) countY = 0;
			
			double outUL = aUL[countU-((countU > delayU)?delayU+1:0)];
			double outVL = aVL[countV-((countV > delayV)?delayV+1:0)];
			double outWL = aWL[countW-((countW > delayW)?delayW+1:0)];
			double outXL = aXL[countX-((countX > delayX)?delayX+1:0)];
			double outYL = aYL[countY-((countY > delayY)?delayY+1:0)];
			
			double outUR = aUR[countU-((countU > delayU)?delayU+1:0)];
			double outVR = aVR[countV-((countV > delayV)?delayV+1:0)];
			double outWR = aWR[countW-((countW > delayW)?delayW+1:0)];
			double outXR = aXR[countX-((countX > delayX)?delayX+1:0)];
			double outYR = aYR[countY-((countY > delayY)?delayY+1:0)];
			
			//-------- five
			
			
			feedbackAR = ((outUL*3.0) - ((outVL + outWL + outXL + outYL)*2.0));
			feedbackBL = ((outVL*3.0) - ((outUL + outWL + outXL + outYL)*2.0));
			feedbackCR = ((outWL*3.0) - ((outUL + outVL + outXL + outYL)*2.0));
			feedbackDL = ((outXL*3.0) - ((outUL + outVL + outWL + outYL)*2.0));
			feedbackER = ((outYL*3.0) - ((outUL + outVL + outWL + outXL)*2.0));
			
			feedbackAL = ((outUR*3.0) - ((outVR + outWR + outXR + outYR)*2.0));
			feedbackBR = ((outVR*3.0) - ((outUR + outWR + outXR + outYR)*2.0));
			feedbackCL = ((outWR*3.0) - ((outUR + outVR + outXR + outYR)*2.0));
			feedbackDR = ((outXR*3.0) - ((outUR + outVR + outWR + outYR)*2.0));
			feedbackEL = ((outYR*3.0) - ((outUR + outVR + outWR + outXR)*2.0));
			//which we need to feed back into the input again, a bit
			
			inputSampleL = (outUL + outVL + outWL + outXL + outYL)*0.0016;
			inputSampleR = (outUR + outVR + outWR + outXR + outYR)*0.0016;
			//and take the final combined sum of outputs, corrected for Householder gain
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
		
		if (fabs(iirBL)<1.18e-37) iirBL = 0.0;
		iirBL = (iirBL*(1.0-lowpass))+(inputSampleL*lowpass); inputSampleL = iirBL;
		if (fabs(iirBR)<1.18e-37) iirBR = 0.0;
		iirBR = (iirBR*(1.0-lowpass))+(inputSampleR*lowpass); inputSampleR = iirBR;
		//second filter
		
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

