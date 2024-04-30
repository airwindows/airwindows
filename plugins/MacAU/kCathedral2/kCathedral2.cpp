/*
*	File:		kCathedral2.cpp
*	
*	Version:	1.0
* 
*	Created:	1/31/24
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
	kCathedral2.cpp
	
=============================================================================*/
#include "kCathedral2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(kCathedral2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kCathedral2::kCathedral2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
kCathedral2::kCathedral2(AudioUnit component)
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
//	kCathedral2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kCathedral2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kCathedral2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kCathedral2::GetParameterInfo(AudioUnitScope		inScope,
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
//	kCathedral2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kCathedral2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 kCathedral2::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	kCathedral2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kCathedral2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	kCathedral2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult kCathedral2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____kCathedral2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kCathedral2::kCathedral2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		kCathedral2::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	gainOutL = gainOutR = 1.0;
	
	for(int count = 0; count < shortA+2; count++) {eAL[count] = 0.0; eAR[count] = 0.0;}
	for(int count = 0; count < shortB+2; count++) {eBL[count] = 0.0; eBR[count] = 0.0;}
	for(int count = 0; count < shortC+2; count++) {eCL[count] = 0.0; eCR[count] = 0.0;}
	for(int count = 0; count < shortD+2; count++) {eDL[count] = 0.0; eDR[count] = 0.0;}
	for(int count = 0; count < shortE+2; count++) {eEL[count] = 0.0; eER[count] = 0.0;}
	for(int count = 0; count < shortF+2; count++) {eFL[count] = 0.0; eFR[count] = 0.0;}
	for(int count = 0; count < shortG+2; count++) {eGL[count] = 0.0; eGR[count] = 0.0;}
	for(int count = 0; count < shortH+2; count++) {eHL[count] = 0.0; eHR[count] = 0.0;}
	for(int count = 0; count < shortI+2; count++) {eIL[count] = 0.0; eIR[count] = 0.0;}
	for(int count = 0; count < shortJ+2; count++) {eJL[count] = 0.0; eJR[count] = 0.0;}
	for(int count = 0; count < shortK+2; count++) {eKL[count] = 0.0; eKR[count] = 0.0;}
	for(int count = 0; count < shortL+2; count++) {eLL[count] = 0.0; eLR[count] = 0.0;}
	for(int count = 0; count < shortM+2; count++) {eML[count] = 0.0; eMR[count] = 0.0;}
	for(int count = 0; count < shortN+2; count++) {eNL[count] = 0.0; eNR[count] = 0.0;}
	for(int count = 0; count < shortO+2; count++) {eOL[count] = 0.0; eOR[count] = 0.0;}
	for(int count = 0; count < shortP+2; count++) {ePL[count] = 0.0; ePR[count] = 0.0;}		
	
	
	shortAL = 1;
	shortBL = 1;
	shortCL = 1;
	shortDL = 1;	
	shortEL = 1;
	shortFL = 1;
	shortGL = 1;
	shortHL = 1;
	shortIL = 1;
	shortJL = 1;
	shortKL = 1;
	shortLL = 1;
	shortML = 1;
	shortNL = 1;
	shortOL = 1;
	shortPL = 1;
	
	shortAR = 1;
	shortBR = 1;
	shortCR = 1;
	shortDR = 1;	
	shortER = 1;
	shortFR = 1;
	shortGR = 1;
	shortHR = 1;
	shortIR = 1;
	shortJR = 1;
	shortKR = 1;
	shortLR = 1;
	shortMR = 1;
	shortNR = 1;
	shortOR = 1;
	shortPR = 1;
	
	
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
	
	for(int count = 0; count < predelay+2; count++) {aZL[count] = 0.0; aZR[count] = 0.0;}
	for(int count = 0; count < vlfpredelay+2; count++) {aVLFL[count] = 0.0; aVLFR[count] = 0.0;}
	
	feedbackAL = 0.0;
	feedbackBL = 0.0;
	feedbackCL = 0.0;
	feedbackDL = 0.0;
	feedbackEL = 0.0;
	
	feedbackER = 0.0;
	feedbackJR = 0.0;
	feedbackOR = 0.0;
	feedbackTR = 0.0;
	feedbackYR = 0.0;
		
	for(int count = 0; count < 6; count++) {lastRefL[count] = 0.0; lastRefR[count] = 0.0;}
		
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
	countVLF = 1;
	
	cycle = 0;
	
	for (int x = 0; x < pear_total; x++) {pearA[x] = 0.0; pearB[x] = 0.0; pearC[x] = 0.0; pearD[x] = 0.0; pearE[x] = 0.0; pearF[x] = 0.0;}
	//from PearEQ
	
	vibratoL = vibAL = vibAR = vibBL = vibBR = 0.0;
	vibratoR = M_PI_4;
		
	subAL = subAR = subBL = subBR = subCL = subCR = 0.0;
	sbAL = sbAR = sbBL = sbBR = sbCL = sbCR = 0.0;
	//from SubTight
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kCathedral2::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		kCathedral2::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	int adjPredelay = predelay;
	int adjSubDelay = vlfpredelay;
	
	int pearStages = 5;
	double pear = 0.388;
	double pearScaled = (pear*0.388)/(double)cycleEnd;
	
	double wet = GetParameter( kParam_One )*2.0; 
	double dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0;
	if (wet < 0.0) wet = 0.0;
	if (dry > 1.0) dry = 1.0;
	if (dry < 0.0) dry = 0.0;
	//this reverb makes 50% full dry AND full wet, not crossfaded.
	//that's so it can be on submixes without cutting back dry channel when adjusted:
	//unless you go super heavy, you are only adjusting the added verb loudness.
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a reverb sample
			//predelay
			aZL[countZ] = inputSampleL;
			aZR[countZ] = inputSampleR;
			countZ++; if (countZ < 0 || countZ > adjPredelay) countZ = 0;
			inputSampleL = aZL[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			inputSampleR = aZR[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			//end predelay
			
			//begin SubTight section
			double outSampleL = inputSampleL * 0.00187;
			double outSampleR = inputSampleR * 0.00187;
			double scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (subAL+(sin(subAL-outSampleL)*scale));
			subAL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (subAR+(sin(subAR-outSampleR)*scale));
			subAR = outSampleR*scale;
			scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (subBL+(sin(subBL-outSampleL)*scale));
			subBL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (subBR+(sin(subBR-outSampleR)*scale));
			subBR = outSampleR*scale;
			scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (subCL+(sin(subCL-outSampleL)*scale));
			subCL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (subCR+(sin(subCR-outSampleR)*scale));
			subCR = outSampleR*scale;
			outSampleL = -outSampleL; outSampleR = -outSampleR;
			if (outSampleL > 0.25) outSampleL = 0.25; if (outSampleL < -0.25) outSampleL = -0.25;
			if (outSampleR > 0.25) outSampleR = 0.25; if (outSampleR < -0.25) outSampleR = -0.25;
			outSampleL *= 16.0;
			outSampleR *= 16.0;
			inputSampleL -= outSampleL;
			inputSampleR -= outSampleR;
			//end SubTight section
			
			double earlyAL = inputSampleL - (eAL[(shortAL+1)-((shortAL+1 > shortA)?shortA+1:0)]*0.618033988749894848204586);
			double earlyBL = inputSampleL - (eBL[(shortBL+1)-((shortBL+1 > shortB)?shortB+1:0)]*0.618033988749894848204586);
			double earlyCL = inputSampleL - (eCL[(shortCL+1)-((shortCL+1 > shortC)?shortC+1:0)]*0.618033988749894848204586);
			double earlyDL = inputSampleL - (eDL[(shortDL+1)-((shortDL+1 > shortD)?shortD+1:0)]*0.618033988749894848204586);
			double earlyDR = inputSampleR - (eDR[(shortDR+1)-((shortDR+1 > shortD)?shortD+1:0)]*0.618033988749894848204586);
			double earlyHR = inputSampleR - (eHR[(shortHR+1)-((shortHR+1 > shortH)?shortH+1:0)]*0.618033988749894848204586);
			double earlyLR = inputSampleR - (eLR[(shortLR+1)-((shortLR+1 > shortL)?shortL+1:0)]*0.618033988749894848204586);
			double earlyPR = inputSampleR - (ePR[(shortPR+1)-((shortPR+1 > shortP)?shortP+1:0)]*0.618033988749894848204586);
						
			eAL[shortAL] = earlyAL; earlyAL *= 0.618033988749894848204586;
			eBL[shortBL] = earlyBL; earlyBL *= 0.618033988749894848204586;
			eCL[shortCL] = earlyCL; earlyCL *= 0.618033988749894848204586;
			eDL[shortDL] = earlyDL; earlyDL *= 0.618033988749894848204586;
			eDR[shortDR] = earlyDR; earlyDR *= 0.618033988749894848204586;
			eHR[shortHR] = earlyHR; earlyHR *= 0.618033988749894848204586;
			eLR[shortLR] = earlyLR; earlyLR *= 0.618033988749894848204586;
			ePR[shortPR] = earlyPR; earlyPR *= 0.618033988749894848204586;
			
			shortAL++; if (shortAL < 0 || shortAL > shortA) shortAL = 0;
			shortBL++; if (shortBL < 0 || shortBL > shortB) shortBL = 0;
			shortCL++; if (shortCL < 0 || shortCL > shortC) shortCL = 0;
			shortDL++; if (shortDL < 0 || shortDL > shortD) shortDL = 0;
			shortDR++; if (shortDR < 0 || shortDR > shortD) shortDR = 0;
			shortHR++; if (shortHR < 0 || shortHR > shortH) shortHR = 0;
			shortLR++; if (shortLR < 0 || shortLR > shortL) shortLR = 0;
			shortPR++; if (shortPR < 0 || shortPR > shortP) shortPR = 0;
			
			earlyAL += eAL[shortAL-((shortAL > shortA)?shortA+1:0)];
			earlyBL += eBL[shortBL-((shortBL > shortB)?shortB+1:0)];
			earlyCL += eCL[shortCL-((shortCL > shortC)?shortC+1:0)];
			earlyDL += eDL[shortDL-((shortDL > shortD)?shortD+1:0)];
			earlyDR += eDR[shortDR-((shortDR > shortD)?shortD+1:0)];
			earlyHR += eHR[shortHR-((shortHR > shortH)?shortH+1:0)];
			earlyLR += eLR[shortLR-((shortLR > shortL)?shortL+1:0)];
			earlyPR += ePR[shortPR-((shortPR > shortP)?shortP+1:0)];
			
			double earlyEL = (earlyAL - (earlyBL + earlyCL + earlyDL)) - (eEL[(shortEL+1)-((shortEL+1 > shortE)?shortE+1:0)]*0.618033988749894848204586);
			double earlyFL = (earlyBL - (earlyAL + earlyCL + earlyDL)) - (eFL[(shortFL+1)-((shortFL+1 > shortF)?shortF+1:0)]*0.618033988749894848204586);
			double earlyGL = (earlyCL - (earlyAL + earlyBL + earlyDL)) - (eGL[(shortGL+1)-((shortGL+1 > shortG)?shortG+1:0)]*0.618033988749894848204586);
			double earlyHL = (earlyDL - (earlyAL + earlyBL + earlyCL)) - (eHL[(shortHL+1)-((shortHL+1 > shortH)?shortH+1:0)]*0.618033988749894848204586);
			double earlyCR = (earlyDR - (earlyHR + earlyLR + earlyPR)) - (eCR[(shortCR+1)-((shortCR+1 > shortC)?shortC+1:0)]*0.618033988749894848204586);
			double earlyGR = (earlyHR - (earlyDR + earlyLR + earlyPR)) - (eGR[(shortGR+1)-((shortGR+1 > shortG)?shortG+1:0)]*0.618033988749894848204586);
			double earlyKR = (earlyLR - (earlyDR + earlyHR + earlyPR)) - (eKR[(shortKR+1)-((shortKR+1 > shortK)?shortK+1:0)]*0.618033988749894848204586);
			double earlyOR = (earlyPR - (earlyDR + earlyHR + earlyLR)) - (eOR[(shortOR+1)-((shortOR+1 > shortO)?shortO+1:0)]*0.618033988749894848204586);
			
			eEL[shortEL] = earlyEL; earlyEL *= 0.618033988749894848204586;
			eFL[shortFL] = earlyFL; earlyFL *= 0.618033988749894848204586;
			eGL[shortGL] = earlyGL; earlyGL *= 0.618033988749894848204586;
			eHL[shortHL] = earlyHL; earlyHL *= 0.618033988749894848204586;
			eCR[shortCR] = earlyCR; earlyCR *= 0.618033988749894848204586;
			eGR[shortGR] = earlyGR; earlyGR *= 0.618033988749894848204586;
			eKR[shortKR] = earlyKR; earlyKR *= 0.618033988749894848204586;
			eOR[shortOR] = earlyOR; earlyOR *= 0.618033988749894848204586;
			
			shortEL++; if (shortEL < 0 || shortEL > shortE) shortEL = 0;
			shortFL++; if (shortFL < 0 || shortFL > shortF) shortFL = 0;
			shortGL++; if (shortGL < 0 || shortGL > shortG) shortGL = 0;
			shortHL++; if (shortHL < 0 || shortHL > shortH) shortHL = 0;
			shortCR++; if (shortCR < 0 || shortCR > shortC) shortCR = 0;
			shortGR++; if (shortGR < 0 || shortGR > shortG) shortGR = 0;
			shortKR++; if (shortKR < 0 || shortKR > shortK) shortKR = 0;
			shortOR++; if (shortOR < 0 || shortOR > shortO) shortOR = 0;
			
			earlyEL += eEL[shortEL-((shortEL > shortE)?shortE+1:0)];
			earlyFL += eFL[shortFL-((shortFL > shortF)?shortF+1:0)];
			earlyGL += eGL[shortGL-((shortGL > shortG)?shortG+1:0)];
			earlyHL += eHL[shortHL-((shortHL > shortH)?shortH+1:0)];
			earlyCR += eCR[shortCR-((shortCR > shortC)?shortC+1:0)];
			earlyGR += eGR[shortGR-((shortGR > shortG)?shortG+1:0)];
			earlyKR += eKR[shortKR-((shortKR > shortK)?shortK+1:0)];
			earlyOR += eOR[shortOR-((shortOR > shortO)?shortO+1:0)];
			
			double earlyIL = (earlyEL - (earlyFL + earlyGL + earlyHL)) - (eIL[(shortIL+1)-((shortIL+1 > shortI)?shortI+1:0)]*0.618033988749894848204586);
			double earlyJL = (earlyFL - (earlyEL + earlyGL + earlyHL)) - (eJL[(shortJL+1)-((shortJL+1 > shortJ)?shortJ+1:0)]*0.618033988749894848204586);
			double earlyKL = (earlyGL - (earlyEL + earlyFL + earlyHL)) - (eKL[(shortKL+1)-((shortKL+1 > shortK)?shortK+1:0)]*0.618033988749894848204586);
			double earlyLL = (earlyHL - (earlyEL + earlyFL + earlyGL)) - (eLL[(shortLL+1)-((shortLL+1 > shortL)?shortL+1:0)]*0.618033988749894848204586);
			double earlyBR = (earlyCR - (earlyGR + earlyKR + earlyOR)) - (eBR[(shortBR+1)-((shortBR+1 > shortB)?shortB+1:0)]*0.618033988749894848204586);
			double earlyFR = (earlyGR - (earlyCR + earlyKR + earlyOR)) - (eFR[(shortFR+1)-((shortFR+1 > shortF)?shortF+1:0)]*0.618033988749894848204586);
			double earlyJR = (earlyKR - (earlyCR + earlyGR + earlyOR)) - (eJR[(shortJR+1)-((shortJR+1 > shortJ)?shortJ+1:0)]*0.618033988749894848204586);
			double earlyNR = (earlyOR - (earlyCR + earlyGR + earlyKR)) - (eNR[(shortNR+1)-((shortNR+1 > shortN)?shortN+1:0)]*0.618033988749894848204586);
			
			eIL[shortIL] = earlyIL; earlyIL *= 0.618033988749894848204586;
			eJL[shortJL] = earlyJL; earlyJL *= 0.618033988749894848204586;
			eKL[shortKL] = earlyKL; earlyKL *= 0.618033988749894848204586;
			eLL[shortLL] = earlyLL; earlyLL *= 0.618033988749894848204586;
			eBR[shortBR] = earlyBR; earlyBR *= 0.618033988749894848204586;
			eFR[shortFR] = earlyFR; earlyFR *= 0.618033988749894848204586;
			eJR[shortJR] = earlyJR; earlyJR *= 0.618033988749894848204586;
			eNR[shortNR] = earlyNR; earlyNR *= 0.618033988749894848204586;
			
			shortIL++; if (shortIL < 0 || shortIL > shortI) shortIL = 0;
			shortJL++; if (shortJL < 0 || shortJL > shortJ) shortJL = 0;
			shortKL++; if (shortKL < 0 || shortKL > shortK) shortKL = 0;
			shortLL++; if (shortLL < 0 || shortLL > shortL) shortLL = 0;
			shortBR++; if (shortBR < 0 || shortBR > shortB) shortBR = 0;
			shortFR++; if (shortFR < 0 || shortFR > shortF) shortFR = 0;
			shortJR++; if (shortJR < 0 || shortJR > shortJ) shortJR = 0;
			shortNR++; if (shortNR < 0 || shortNR > shortN) shortNR = 0;
			
			earlyIL += eIL[shortIL-((shortIL > shortI)?shortI+1:0)];
			earlyJL += eJL[shortJL-((shortJL > shortJ)?shortJ+1:0)];
			earlyKL += eKL[shortKL-((shortKL > shortK)?shortK+1:0)];
			earlyLL += eLL[shortLL-((shortLL > shortL)?shortL+1:0)];
			earlyBR += eBR[shortBR-((shortBR > shortB)?shortB+1:0)];
			earlyFR += eFR[shortFR-((shortFR > shortF)?shortF+1:0)];
			earlyJR += eJR[shortJR-((shortJR > shortJ)?shortJ+1:0)];
			earlyNR += eNR[shortNR-((shortNR > shortN)?shortN+1:0)];
			
			double earlyML = (earlyIL - (earlyJL + earlyKL + earlyLL)) - (eML[(shortML+1)-((shortML+1 > shortM)?shortM+1:0)]*0.618033988749894848204586);
			double earlyNL = (earlyJL - (earlyIL + earlyKL + earlyLL)) - (eNL[(shortNL+1)-((shortNL+1 > shortN)?shortN+1:0)]*0.618033988749894848204586);
			double earlyOL = (earlyKL - (earlyIL + earlyJL + earlyLL)) - (eOL[(shortOL+1)-((shortOL+1 > shortO)?shortO+1:0)]*0.618033988749894848204586);
			double earlyPL = (earlyLL - (earlyIL + earlyJL + earlyKL)) - (ePL[(shortPL+1)-((shortPL+1 > shortP)?shortP+1:0)]*0.618033988749894848204586);
			double earlyAR = (earlyBR - (earlyFR + earlyJR + earlyNR)) - (eAR[(shortAR+1)-((shortAR+1 > shortA)?shortA+1:0)]*0.618033988749894848204586);
			double earlyER = (earlyFR - (earlyBR + earlyJR + earlyNR)) - (eER[(shortER+1)-((shortER+1 > shortE)?shortE+1:0)]*0.618033988749894848204586);
			double earlyIR = (earlyJR - (earlyBR + earlyFR + earlyNR)) - (eIR[(shortIR+1)-((shortIR+1 > shortI)?shortI+1:0)]*0.618033988749894848204586);
			double earlyMR = (earlyNR - (earlyBR + earlyFR + earlyJR)) - (eMR[(shortMR+1)-((shortMR+1 > shortM)?shortM+1:0)]*0.618033988749894848204586);
			
			eML[shortML] = earlyML; earlyML *= 0.618033988749894848204586;
			eNL[shortNL] = earlyNL; earlyNL *= 0.618033988749894848204586;
			eOL[shortOL] = earlyOL; earlyOL *= 0.618033988749894848204586;
			ePL[shortPL] = earlyPL; earlyPL *= 0.618033988749894848204586;
			eAR[shortAR] = earlyAR; earlyAR *= 0.618033988749894848204586;
			eER[shortER] = earlyER; earlyER *= 0.618033988749894848204586;
			eIR[shortIR] = earlyIR; earlyIR *= 0.618033988749894848204586;
			eMR[shortMR] = earlyMR; earlyMR *= 0.618033988749894848204586;
			
			shortML++; if (shortML < 0 || shortML > shortM) shortML = 0;
			shortNL++; if (shortNL < 0 || shortNL > shortN) shortNL = 0;
			shortOL++; if (shortOL < 0 || shortOL > shortO) shortOL = 0;
			shortPL++; if (shortPL < 0 || shortPL > shortP) shortPL = 0;
			shortAR++; if (shortAR < 0 || shortAR > shortA) shortAR = 0;
			shortER++; if (shortER < 0 || shortER > shortE) shortER = 0;
			shortIR++; if (shortIR < 0 || shortIR > shortI) shortIR = 0;
			shortMR++; if (shortMR < 0 || shortMR > shortM) shortMR = 0;
			
			earlyML += eML[shortML-((shortML > shortM)?shortM+1:0)];
			earlyNL += eNL[shortNL-((shortNL > shortN)?shortN+1:0)];
			earlyOL += eOL[shortOL-((shortOL > shortO)?shortO+1:0)];
			earlyPL += ePL[shortPL-((shortPL > shortP)?shortP+1:0)];
			earlyAR += eAR[shortAR-((shortAR > shortA)?shortA+1:0)];
			earlyER += eER[shortER-((shortER > shortE)?shortE+1:0)];
			earlyIR += eIR[shortIR-((shortIR > shortI)?shortI+1:0)];
			earlyMR += eMR[shortMR-((shortMR > shortM)?shortM+1:0)];
			
			double earlyReflectionsL = -(earlyML + earlyNL + earlyOL + earlyPL)/8.0;
			double earlyReflectionsR = -(earlyAR + earlyER + earlyIR + earlyMR)/8.0;
			//and take the final combined sum of outputs, corrected for Householder gain
						
			//VLF predelay
			aVLFL[countVLF] = outSampleL;
			aVLFR[countVLF] = outSampleR;
			countVLF++; if (countVLF < 0 || countVLF > adjSubDelay) countVLF = 0;
			outSampleL = aVLFL[countVLF-((countVLF > adjSubDelay)?adjSubDelay+1:0)] * 2.0;
			outSampleR = aVLFR[countVLF-((countVLF > adjSubDelay)?adjSubDelay+1:0)] * 2.0;
			//end VLF predelay
						
			inputSampleL += outSampleL;
			inputSampleR += outSampleR;
			//having re-added our VLF delayed channel we can now re-use outSample
			
			aAL[countAL] = inputSampleL + (feedbackAL * 0.000285);
			aBL[countBL] = inputSampleL + (feedbackBL * 0.000285);
			aCL[countCL] = inputSampleL + (feedbackCL * 0.000285);
			aDL[countDL] = inputSampleL + (feedbackDL * 0.000285);
			aEL[countEL] = inputSampleL + (feedbackEL * 0.000285);
			
			aER[countER] = inputSampleR + (feedbackER * 0.000285);
			aJR[countJR] = inputSampleR + (feedbackJR * 0.000285);
			aOR[countOR] = inputSampleR + (feedbackOR * 0.000285);
			aTR[countTR] = inputSampleR + (feedbackTR * 0.000285);
			aYR[countYR] = inputSampleR + (feedbackYR * 0.000285);
			
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
						
			//-------- four
			
			aVL[countVL] = ((outQL*3.0) - ((outPL + outRL + outSL + outTL)*2.0));
			aWL[countWL] = ((outRL*3.0) - ((outPL + outQL + outSL + outTL)*2.0));
			aXL[countXL] = ((outSL*3.0) - ((outPL + outQL + outRL + outTL)*2.0));
			aYL[countYL] = ((outTL*3.0) - ((outPL + outQL + outRL + outSL)*2.0));
			
			aAR[countAR] = ((outBR*3.0) - ((outGR + outLR + outQR + outVR)*2.0));
			aFR[countFR] = ((outGR*3.0) - ((outBR + outLR + outQR + outVR)*2.0));
			aKR[countKR] = ((outLR*3.0) - ((outBR + outGR + outQR + outVR)*2.0));
			aPR[countPR] = ((outQR*3.0) - ((outBR + outGR + outLR + outVR)*2.0));
			
			double outUL = ((outPL*3.0) - ((outQL + outRL + outSL + outTL)*2.0)) - (aUL[(countUL+1)-((countUL+1 > delayU)?delayU+1:0)]*0.618033988749894848204586);
			double outUR = ((outVR*3.0) - ((outBR + outGR + outLR + outQR)*2.0)) - (aUR[(countUR+1)-((countUR+1 > delayU)?delayU+1:0)]*0.618033988749894848204586);
			aUL[countUL] = outUL; outUL *= 0.618033988749894848204586;
			aUR[countUR] = outUR; outUR *= 0.618033988749894848204586;
			countUL++; if (countUL < 0 || countUL > delayU) countUL = 0;
			countUR++; if (countUR < 0 || countUR > delayU) countUR = 0;
			outUL += aUL[countUL-((countUL > delayU)?delayU+1:0)];
			outUR += aUR[countUR-((countUR > delayU)?delayU+1:0)];
			//the 11-length delay slot becomes a sole allpass
			
			vibBL = vibAL; vibAL = outUL;
			vibBR = vibAR; vibAR = outUR; //tiny two sample delay chains
			vibratoL += fpdL * 0.5e-13; if (vibratoL > M_PI*2.0) vibratoL -= M_PI*2.0;
			vibratoR += fpdR * 0.5e-13; if (vibratoR > M_PI*2.0) vibratoR -= M_PI*2.0;
			double quadL = sin(vibratoL)+1.0;
			double quadR = sin(vibratoR)+1.0;
			//quadrature delay points play back from a position in delay chains
			if (quadL < 1.0) outUL = (outUL*(1.0-quadL))+(vibAL*quadL);
			else outUL = (vibAL*(1.0-(quadL-1.0)))+(vibBL*(quadL-1.0));
			if (quadR < 1.0) outUR = (outUR*(1.0-quadR))+(vibAR*quadR);
			else outUR = (vibAR*(1.0-(quadR-1.0)))+(vibBR*(quadR-1.0));
			//also, pitch drift this allpass slot for very subtle motion
			
			countVL++; if (countVL < 0 || countVL > delayV) countVL = 0;
			countWL++; if (countWL < 0 || countWL > delayW) countWL = 0;
			countXL++; if (countXL < 0 || countXL > delayX) countXL = 0;
			countYL++; if (countYL < 0 || countYL > delayY) countYL = 0;
			
			countAR++; if (countAR < 0 || countAR > delayA) countAR = 0;
			countFR++; if (countFR < 0 || countFR > delayF) countFR = 0;
			countKR++; if (countKR < 0 || countKR > delayK) countKR = 0;
			countPR++; if (countPR < 0 || countPR > delayP) countPR = 0;
			
			double outVL = aVL[countVL-((countVL > delayV)?delayV+1:0)];
			double outWL = aWL[countWL-((countWL > delayW)?delayW+1:0)];
			double outXL = aXL[countXL-((countXL > delayX)?delayX+1:0)];
			double outYL = aYL[countYL-((countYL > delayY)?delayY+1:0)];
			
			double outAR = aAR[countAR-((countAR > delayA)?delayA+1:0)];
			double outFR = aFR[countFR-((countFR > delayF)?delayF+1:0)];
			double outKR = aKR[countKR-((countKR > delayK)?delayK+1:0)];
			double outPR = aPR[countPR-((countPR > delayP)?delayP+1:0)];
			
			//-------- five
			
			feedbackER = ((outUL*3.0) - ((outVL + outWL + outXL + outYL)*2.0));
			feedbackAL = ((outAR*3.0) - ((outFR + outKR + outPR + outUR)*2.0));
			for (int x = 0; x < 1; x += 4) {
				double slew = ((feedbackAL - pearA[x]) + pearA[x+1])*pear*0.5;
				pearA[x] = feedbackAL = (pear * feedbackAL) + ((1.0-pear) * (pearA[x] + pearA[x+1]));
				pearA[x+1] = slew;
				slew = ((feedbackER - pearA[x+2]) + pearA[x+3])*pear*0.5;
				pearA[x+2] = feedbackER = (pear * feedbackER) + ((1.0-pear) * (pearA[x+2] + pearA[x+3]));
				pearA[x+3] = slew;
			}			
			
			feedbackBL = ((outVL*3.0) - ((outUL + outWL + outXL + outYL)*2.0));
			feedbackJR = ((outFR*3.0) - ((outAR + outKR + outPR + outUR)*2.0));
			for (int x = 0; x < pearStages; x += 4) {
				double slew = ((feedbackBL - pearB[x]) + pearB[x+1])*pear*0.5;
				pearB[x] = feedbackBL = (pear * feedbackBL) + ((1.0-pear) * (pearB[x] + pearB[x+1]));
				pearB[x+1] = slew;
				slew = ((feedbackJR - pearB[x+2]) + pearB[x+3])*pear*0.5;
				pearB[x+2] = feedbackJR = (pear * feedbackJR) + ((1.0-pear) * (pearB[x+2] + pearB[x+3]));
				pearB[x+3] = slew;
			}			
			
			feedbackCL = ((outWL*3.0) - ((outUL + outVL + outXL + outYL)*2.0));
			feedbackOR = ((outKR*3.0) - ((outAR + outFR + outPR + outUR)*2.0));
			for (int x = 0; x < pearStages; x += 4) {
				double slew = ((feedbackCL - pearC[x]) + pearC[x+1])*pear*0.5;
				pearC[x] = feedbackCL = (pear * feedbackCL) + ((1.0-pear) * (pearC[x] + pearC[x+1]));
				pearC[x+1] = slew;
				slew = ((feedbackOR - pearC[x+2]) + pearC[x+3])*pear*0.5;
				pearC[x+2] = feedbackOR = (pear * feedbackOR) + ((1.0-pear) * (pearC[x+2] + pearC[x+3]));
				pearC[x+3] = slew;
			}			
			
			feedbackDL = ((outXL*3.0) - ((outUL + outVL + outWL + outYL)*2.0));
			feedbackTR = ((outPR*3.0) - ((outAR + outFR + outKR + outUR)*2.0));
			for (int x = 0; x < pearStages; x += 4) {
				double slew = ((feedbackDL - pearD[x]) + pearD[x+1])*pear*0.5;
				pearD[x] = feedbackDL = (pear * feedbackDL) + ((1.0-pear) * (pearD[x] + pearD[x+1]));
				pearD[x+1] = slew;
				slew = ((feedbackTR - pearD[x+2]) + pearD[x+3])*pear*0.5;
				pearD[x+2] = feedbackTR = (pear * feedbackTR) + ((1.0-pear) * (pearD[x+2] + pearD[x+3]));
				pearD[x+3] = slew;
			}			
			
			feedbackEL = ((outYL*3.0) - ((outUL + outVL + outWL + outXL)*2.0));
			feedbackYR = ((outUR*3.0) - ((outAR + outFR + outKR + outPR)*2.0));
			for (int x = 0; x < pearStages; x += 4) {
				double slew = ((feedbackEL - pearE[x]) + pearE[x+1])*pear*0.5;
				pearE[x] = feedbackEL = (pear * feedbackEL) + ((1.0-pear) * (pearE[x] + pearE[x+1]));
				pearE[x+1] = slew;
				slew = ((feedbackYR - pearE[x+2]) + pearE[x+3])*pear*0.5;
				pearE[x+2] = feedbackYR = (pear * feedbackYR) + ((1.0-pear) * (pearE[x+2] + pearE[x+3]));
				pearE[x+3] = slew;
			}			
			//which we need to feed back into the input again, a bit
						 
			inputSampleL = (outUL + outVL + outWL + outXL + outYL)*0.0004;
			inputSampleR = (outAR + outFR + outKR + outPR + outUR)*0.0004;
			//and take the final combined sum of outputs, corrected for Householder gain

			//begin SubBoost section
			outSampleL = inputSampleL * 0.00186;
			outSampleR = inputSampleR * 0.00186;
			scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (sbAL+(sin(sbAL-outSampleL)*scale));
			sbAL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (sbAR+(sin(sbAR-outSampleR)*scale));
			sbAR = outSampleR*scale;
			scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (sbBL+(sin(sbBL-outSampleL)*scale));
			sbBL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (sbBR+(sin(sbBR-outSampleR)*scale));
			sbBR = outSampleR*scale;
			scale = 0.5+fabs(outSampleL*0.5);
			outSampleL = (sbCL+(sin(sbCL-outSampleL)*scale));
			sbCL = outSampleL*scale;
			scale = 0.5+fabs(outSampleR*0.5);
			outSampleR = (sbCR+(sin(sbCR-outSampleR)*scale));
			sbCR = outSampleR*scale;
			outSampleL = -outSampleL; outSampleR = -outSampleR;
			if (outSampleL > 0.25) outSampleL = 0.25; if (outSampleL < -0.25) outSampleL = -0.25;
			if (outSampleR > 0.25) outSampleR = 0.25; if (outSampleR < -0.25) outSampleR = -0.25;
			outSampleL *= 32.0;
			outSampleR *= 32.0;
			inputSampleL += outSampleL;
			inputSampleR += outSampleR;
			//end SubBoost section
			
			inputSampleL += (earlyReflectionsL*0.25);
			inputSampleR += (earlyReflectionsR*0.25);			
			
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
		
		for (int x = 0; x < 1; x += 4) {
			double slew = ((inputSampleL - pearF[x]) + pearF[x+1])*pearScaled*0.5;
			pearF[x] = inputSampleL = (pearScaled * inputSampleL) + ((1.0-pearScaled) * (pearF[x] + pearF[x+1]));
			pearF[x+1] = slew;
			slew = ((inputSampleR - pearF[x+2]) + pearF[x+3])*pearScaled*0.5;
			pearF[x+2] = inputSampleR = (pearScaled * inputSampleR) + ((1.0-pearScaled) * (pearF[x+2] + pearF[x+3]));
			pearF[x+3] = slew;
		}
				
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