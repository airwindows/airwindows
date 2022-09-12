/*
*	File:		Chamber2.cpp
*	
*	Version:	1.0
* 
*	Created:	8/26/22
*	
*	Copyright:  Copyright © 2022 Airwindows, All Rights Reserved
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
	Chamber2.cpp
	
=============================================================================*/
#include "Chamber2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Chamber2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Chamber2::Chamber2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Chamber2::Chamber2(AudioUnit component)
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
//	Chamber2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Chamber2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Chamber2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Chamber2::GetParameterInfo(AudioUnitScope		inScope,
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
//	Chamber2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Chamber2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Chamber2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Chamber2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Chamber2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Chamber2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Chamber2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Chamber2::Chamber2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Chamber2::Chamber2Kernel::Reset()
{
	for(int count = 0; count < 9999; count++) {aE[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aF[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aG[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aH[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aA[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aB[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aC[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aD[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aI[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aJ[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aK[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aL[count] = 0.0;}
	for(int count = 0; count < 9999; count++) {aM[count] = 0.0;}
	
	feedbackA = 0.0;
	feedbackB = 0.0;
	feedbackC = 0.0;
	feedbackD = 0.0;
	previousA = 0.0;
	previousB = 0.0;
	previousC = 0.0;
	previousD = 0.0;
	
	for(int count = 0; count < 9; count++) {lastRef[count] = 0.0;}
	cycle = 0;
	
	countI = 1;
	countJ = 1;
	countK = 1;
	countL = 1;
	countM = 1;
	
	countA = 1;
	countB = 1;
	countC = 1;
	countD = 1;	
	
	countE = 1;
	countF = 1;
	countG = 1;
	countH = 1;
	
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Chamber2::Chamber2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Chamber2::Chamber2Kernel::Process(	const Float32 	*inSourceP,
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
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	Float64 size = (GetParameter( kParam_One )*0.9)+0.1;
	Float64 regen = (1.0-(pow(1.0-GetParameter( kParam_Two ),2)))*0.123;
	Float64 echoScale = 1.0-(pow(GetParameter( kParam_Three ),3));
	Float64 echo = 0.618033988749894848204586+((1.0-0.618033988749894848204586)*echoScale);
	Float64 interpolate = (1.0-echo)*0.381966011250105;
	//this now goes from Chamber, to all the reverb delays being exactly the same
	//much larger usage of RAM due to the larger reverb delays everywhere, but
	//ability to go to an unusual variation on blurred delay.
	Float64 wet = GetParameter( kParam_Four )*2.0;
	Float64 dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0;
	if (wet < 0.0) wet = 0.0;
	if (dry > 1.0) dry = 1.0;
	if (dry < 0.0) dry = 0.0;
	//this reverb makes 50% full dry AND full wet, not crossfaded.
	//that's so it can be on submixes without cutting back dry channel when adjusted:
	//unless you go super heavy, you are only adjusting the added verb loudness.
	
	delayE = delayM = 9900*size;
	delayF = delayE*echo; 
	delayG = delayF*echo;
	delayH = delayG*echo;
	delayA = delayH*echo;
	delayB = delayA*echo;
	delayC = delayB*echo;
	delayD = delayC*echo;
	delayI = delayD*echo;
	delayJ = delayI*echo;
	delayK = delayJ*echo;
	delayL = delayK*echo;
	//initially designed around the Fibonnaci series, Chamber uses
	//delay coefficients that are all related to the Golden Ratio,
	//Turns out that as you continue to sustain them, it turns from a
	//chunky slapback effect into a smoother reverb tail that can
	//sustain infinitely.	
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a reverb sample
			
			aM[countM] = inputSample;
			countM++; if (countM < 0 || countM > delayM) countM = 0;
			inputSample = aM[countM-((countM > delayM)?delayM+1:0)];
			//predelay to make the first echo still be an echo even when blurred
			
			feedbackA = (feedbackA*(1.0-interpolate))+(previousA*interpolate); previousA = feedbackA;
			feedbackB = (feedbackB*(1.0-interpolate))+(previousB*interpolate); previousB = feedbackB;
			feedbackC = (feedbackC*(1.0-interpolate))+(previousC*interpolate); previousC = feedbackC;
			feedbackD = (feedbackD*(1.0-interpolate))+(previousD*interpolate); previousD = feedbackD;
			
			aI[countI] = inputSample + (feedbackA * regen);
			aJ[countJ] = inputSample + (feedbackB * regen);
			aK[countK] = inputSample + (feedbackC * regen);
			aL[countL] = inputSample + (feedbackD * regen);
			
			countI++; if (countI < 0 || countI > delayI) countI = 0;
			countJ++; if (countJ < 0 || countJ > delayJ) countJ = 0;
			countK++; if (countK < 0 || countK > delayK) countK = 0;
			countL++; if (countL < 0 || countL > delayL) countL = 0;
			
			Float64 outI = aI[countI-((countI > delayI)?delayI+1:0)];
			Float64 outJ = aJ[countJ-((countJ > delayJ)?delayJ+1:0)];
			Float64 outK = aK[countK-((countK > delayK)?delayK+1:0)];
			Float64 outL = aL[countL-((countL > delayL)?delayL+1:0)];
			//first block: now we have four outputs
			
			aA[countA] = (outI - (outJ + outK + outL));
			aB[countB] = (outJ - (outI + outK + outL));
			aC[countC] = (outK - (outI + outJ + outL));
			aD[countD] = (outL - (outI + outJ + outK));
			
			countA++; if (countA < 0 || countA > delayA) countA = 0;
			countB++; if (countB < 0 || countB > delayB) countB = 0;
			countC++; if (countC < 0 || countC > delayC) countC = 0;
			countD++; if (countD < 0 || countD > delayD) countD = 0;
			
			Float64 outA = aA[countA-((countA > delayA)?delayA+1:0)];
			Float64 outB = aB[countB-((countB > delayB)?delayB+1:0)];
			Float64 outC = aC[countC-((countC > delayC)?delayC+1:0)];
			Float64 outD = aD[countD-((countD > delayD)?delayD+1:0)];
			//second block: four more outputs
			
			aE[countE] = (outA - (outB + outC + outD));
			aF[countF] = (outB - (outA + outC + outD));
			aG[countG] = (outC - (outA + outB + outD));
			aH[countH] = (outD - (outA + outB + outC));
			
			countE++; if (countE < 0 || countE > delayE) countE = 0;
			countF++; if (countF < 0 || countF > delayF) countF = 0;
			countG++; if (countG < 0 || countG > delayG) countG = 0;
			countH++; if (countH < 0 || countH > delayH) countH = 0;
			
			Float64 outE = aE[countE-((countE > delayE)?delayE+1:0)];
			Float64 outF = aF[countF-((countF > delayF)?delayF+1:0)];
			Float64 outG = aG[countG-((countG > delayG)?delayG+1:0)];
			Float64 outH = aH[countH-((countH > delayH)?delayH+1:0)];
			//third block: final outputs
			
			feedbackA = (outE - (outF + outG + outH));
			feedbackB = (outF - (outE + outG + outH));
			feedbackC = (outG - (outE + outF + outH));
			feedbackD = (outH - (outE + outF + outG));
			//which we need to feed back into the input again, a bit
			
			inputSample = (outE + outF + outG + outH)/8.0;
			//and take the final combined sum of outputs
			if (cycleEnd == 4) {
				lastRef[0] = lastRef[4]; //start from previous last
				lastRef[2] = (lastRef[0] + inputSample)/2; //half
				lastRef[1] = (lastRef[0] + lastRef[2])/2; //one quarter
				lastRef[3] = (lastRef[2] + inputSample)/2; //three quarters
				lastRef[4] = inputSample; //full
			}
			if (cycleEnd == 3) {
				lastRef[0] = lastRef[3]; //start from previous last
				lastRef[2] = (lastRef[0]+lastRef[0]+inputSample)/3; //third
				lastRef[1] = (lastRef[0]+inputSample+inputSample)/3; //two thirds
				lastRef[3] = inputSample; //full
			}
			if (cycleEnd == 2) {
				lastRef[0] = lastRef[2]; //start from previous last
				lastRef[1] = (lastRef[0] + inputSample)/2; //half
				lastRef[2] = inputSample; //full
			}
			if (cycleEnd == 1) lastRef[0] = inputSample;
			cycle = 0; //reset
			inputSample = lastRef[cycle];
		} else {
			inputSample = lastRef[cycle];
			//we are going through our references now
		}
		switch (cycleEnd) //multi-pole average using lastRef[] variables
		{
			case 4:
				lastRef[8] = inputSample; inputSample = (inputSample+lastRef[7])*0.5;
				lastRef[7] = lastRef[8]; //continue, do not break
			case 3:
				lastRef[8] = inputSample; inputSample = (inputSample+lastRef[6])*0.5;
				lastRef[6] = lastRef[8]; //continue, do not break
			case 2:
				lastRef[8] = inputSample; inputSample = (inputSample+lastRef[5])*0.5;
				lastRef[5] = lastRef[8]; //continue, do not break
			case 1:
				break; //no further averaging
		}
				
		if (wet < 1.0) inputSample *= wet;
		if (dry < 1.0) drySample *= dry;
		inputSample += drySample;
		//this is our submix verb dry/wet: 0.5 is BOTH at FULL VOLUME
		//purpose is that, if you're adding verb, you're not altering other balances
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}
