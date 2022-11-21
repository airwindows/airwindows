/*
*	File:		Reverb.cpp
*	
*	Version:	1.0
* 
*	Created:	12/8/20
*	
*	Copyright:  Copyright © 2020 Airwindows, Airwindows uses the MIT license
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
	Reverb.cpp
	
=============================================================================*/
#include "Reverb.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Reverb)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Reverb::Reverb
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Reverb::Reverb(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Reverb::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Reverb::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Reverb::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Reverb::GetParameterInfo(AudioUnitScope		inScope,
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
//	Reverb::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Reverb::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Reverb::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Reverb::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Reverb::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Reverb::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ReverbEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Reverb::ReverbKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Reverb::ReverbKernel::Reset()
{
	for (int x = 0; x < 11; x++) {biquadA[x] = 0.0;biquadB[x] = 0.0;biquadC[x] = 0.0;}
	
	feedbackA = 0.0;
	feedbackB = 0.0;
	feedbackC = 0.0;
	feedbackD = 0.0;
	feedbackE = 0.0;
	feedbackF = 0.0;
	feedbackG = 0.0;
	feedbackH = 0.0;
	
	int count;
	for(count = 0; count < 8110; count++) {aA[count] = 0.0;}
	for(count = 0; count < 7510; count++) {aB[count] = 0.0;}
	for(count = 0; count < 7310; count++) {aC[count] = 0.0;}
	for(count = 0; count < 6910; count++) {aD[count] = 0.0;}
	for(count = 0; count < 6310; count++) {aE[count] = 0.0;}
	for(count = 0; count < 6110; count++) {aF[count] = 0.0;}
	for(count = 0; count < 5510; count++) {aG[count] = 0.0;}
	for(count = 0; count < 4910; count++) {aH[count] = 0.0;}
	//maximum value needed will be delay * 100, plus 206 (absolute max vibrato depth)
	for(count = 0; count < 4510; count++) {aI[count] = 0.0;}
	for(count = 0; count < 4310; count++) {aJ[count] = 0.0;}
	for(count = 0; count < 3910; count++) {aK[count] = 0.0;}
	for(count = 0; count < 3310; count++) {aL[count] = 0.0;}
	//maximum value will be delay * 100
	for(count = 0; count < 3110; count++) {aM[count] = 0.0;}	
	//maximum value will be delay * 100
	countA = 1; delayA = 79;
	countB = 1; delayB = 73;
	countC = 1; delayC = 71;
	countD = 1; delayD = 67;	
	countE = 1; delayE = 61;
	countF = 1; delayF = 59;
	countG = 1; delayG = 53;
	countH = 1; delayH = 47;
	//the householder matrices
	countI = 1; delayI = 43;
	countJ = 1; delayJ = 41;
	countK = 1; delayK = 37;
	countL = 1; delayL = 31;
	//the allpasses
	countM = 1; delayM = 29;
	//the predelay
	depthA = 0.003251;
	depthB = 0.002999;
	depthC = 0.002917;
	depthD = 0.002749;
	depthE = 0.002503;
	depthF = 0.002423;
	depthG = 0.002146;
	depthH = 0.002088;
	//the individual vibrato rates for the delays
	vibA = rand()*-2147483647;
	vibB = rand()*-2147483647;
	vibC = rand()*-2147483647;
	vibD = rand()*-2147483647;
	vibE = rand()*-2147483647;
	vibF = rand()*-2147483647;
	vibG = rand()*-2147483647;
	vibH = rand()*-2147483647;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Reverb::ReverbKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Reverb::ReverbKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	Float64 big = GetParameter( kParam_One );
	Float64 wet = GetParameter( kParam_Two );	
	Float64 vibSpeed = 0.1;
	Float64 vibDepth = 7.0;
	Float64 size = (pow(big,2)*75.0)+25.0;
	Float64 depthFactor = 1.0-pow((1.0-(0.82-(((1.0-big)*0.7)+(size*0.002)))),4);	
	Float64 blend = 0.955-(size*0.007);
	Float64 regen = depthFactor * 0.5;
	
	delayA = 79*size;
	delayB = 73*size;
	delayC = 71*size;
	delayD = 67*size;
	delayE = 61*size;
	delayF = 59*size;
	delayG = 53*size;
	delayH = 47*size;
	delayI = 43*size;
	delayJ = 41*size;
	delayK = 37*size;
	delayL = 31*size;
	delayM = 29*size;
	
	biquadC[0] = biquadB[0] = biquadA[0] = (10000.0-(big*wet*3000.0))/GetSampleRate();
	biquadA[1] = 1.618033988749894848204586;
	biquadB[1] = 0.618033988749894848204586;
    biquadC[1] = 0.5;
	
	double K = tan(M_PI * biquadA[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadA[0]);
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = K * K * norm;
	biquadB[3] = 2.0 * biquadB[2];
	biquadB[4] = biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;
	
	K = tan(M_PI * biquadC[0]);
	norm = 1.0 / (1.0 + K / biquadC[1] + K * K);
	biquadC[2] = K * K * norm;
	biquadC[3] = 2.0 * biquadC[2];
	biquadC[4] = biquadC[2];
	biquadC[5] = 2.0 * (K * K - 1.0) * norm;
	biquadC[6] = (1.0 - K / biquadC[1] + K * K) * norm;	
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		aM[countM] = inputSample;
		countM++; if (countM < 0 || countM > delayM) {countM = 0;}
		inputSample = aM[countM];
		//predelay
		
		double tempSample = biquadA[2]*inputSample+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
		biquadA[8] = biquadA[7]; biquadA[7] = inputSample; inputSample = tempSample; 
		biquadA[10] = biquadA[9]; biquadA[9] = inputSample; //DF1
		
		inputSample *= wet;
		//we're going to use this as a kind of balance since the reverb buildup can be so large
		
		inputSample = sin(inputSample);
		
		double allpassI = inputSample;
		double allpassJ = inputSample;
		double allpassK = inputSample;
		double allpassL = inputSample;
		
		
		int allpasstemp = countI + 1;
		if (allpasstemp < 0 || allpasstemp > delayI) {allpasstemp = 0;}
		allpassI -= aI[allpasstemp]*0.5;
		aI[countI] = allpassI;
		allpassI *= 0.5;
		countI++; if (countI < 0 || countI > delayI) {countI = 0;}		
		allpassI += (aI[countI]);
		
		allpasstemp = countJ + 1;
		if (allpasstemp < 0 || allpasstemp > delayJ) {allpasstemp = 0;}
		allpassJ -= aJ[allpasstemp]*0.5;
		aJ[countJ] = allpassJ;
		allpassJ *= 0.5;
		countJ++; if (countJ < 0 || countJ > delayJ) {countJ = 0;}		
		allpassJ += (aJ[countJ]);
		
		allpasstemp = countK + 1;
		if (allpasstemp < 0 || allpasstemp > delayK) {allpasstemp = 0;}
		allpassK -= aK[allpasstemp]*0.5;
		aK[countK] = allpassK;
		allpassK *= 0.5;
		countK++; if (countK < 0 || countK > delayK) {countK = 0;}		
		allpassK += (aK[countK]);
		
		allpasstemp = countL + 1;
		if (allpasstemp < 0 || allpasstemp > delayL) {allpasstemp = 0;}
		allpassL -= aL[allpasstemp]*0.5;
		aL[countL] = allpassL;
		allpassL *= 0.5;
		countL++; if (countL < 0 || countL > delayL) {countL = 0;}		
		allpassL += (aL[countL]);		
		//the big allpass in front of everything
		
		
		aA[countA] = allpassL + feedbackA;
		aB[countB] = allpassK + feedbackB;
		aC[countC] = allpassJ + feedbackC;
		aD[countD] = allpassI + feedbackD;
		aE[countE] = allpassI + feedbackE;
		aF[countF] = allpassJ + feedbackF;
		aG[countG] = allpassK + feedbackG;
		aH[countH] = allpassL + feedbackH;
		
		countA++; if (countA < 0 || countA > delayA) {countA = 0;}
		countB++; if (countB < 0 || countB > delayB) {countB = 0;}
		countC++; if (countC < 0 || countC > delayC) {countC = 0;}
		countD++; if (countD < 0 || countD > delayD) {countD = 0;}
		countE++; if (countE < 0 || countE > delayE) {countE = 0;}
		countF++; if (countF < 0 || countF > delayF) {countF = 0;}
		countG++; if (countG < 0 || countG > delayG) {countG = 0;}
		countH++; if (countH < 0 || countH > delayH) {countH = 0;}
		//the Householder matrices
		
		vibA += (depthA * vibSpeed);
		vibB += (depthB * vibSpeed);
		vibC += (depthC * vibSpeed);
		vibD += (depthD * vibSpeed);
		vibE += (depthE * vibSpeed);
		vibF += (depthF * vibSpeed);
		vibG += (depthG * vibSpeed);
		vibH += (depthH * vibSpeed);
		
		Float64 offsetA = (sin(vibA)+1.0)*vibDepth;
		Float64 offsetB = (sin(vibB)+1.0)*vibDepth;
		Float64 offsetC = (sin(vibC)+1.0)*vibDepth;
		Float64 offsetD = (sin(vibD)+1.0)*vibDepth;
		Float64 offsetE = (sin(vibE)+1.0)*vibDepth;
		Float64 offsetF = (sin(vibF)+1.0)*vibDepth;
		Float64 offsetG = (sin(vibG)+1.0)*vibDepth;
		Float64 offsetH = (sin(vibH)+1.0)*vibDepth;
		
		int workingA = countA + offsetA;
		int workingB = countB + offsetB;
		int workingC = countC + offsetC;
		int workingD = countD + offsetD;
		int workingE = countE + offsetE;
		int workingF = countF + offsetF;
		int workingG = countG + offsetG;
		int workingH = countH + offsetH;
		
		Float64 interpolA = (aA[workingA-((workingA > delayA)?delayA+1:0)] * (1-(offsetA-floor(offsetA))) );
		interpolA += (aA[workingA+1-((workingA+1 > delayA)?delayA+1:0)] * ((offsetA-floor(offsetA))) );
		
		Float64 interpolB = (aB[workingB-((workingB > delayB)?delayB+1:0)] * (1-(offsetB-floor(offsetB))) );
		interpolB += (aB[workingB+1-((workingB+1 > delayB)?delayB+1:0)] * ((offsetB-floor(offsetB))) );
		
		Float64 interpolC = (aC[workingC-((workingC > delayC)?delayC+1:0)] * (1-(offsetC-floor(offsetC))) );
		interpolC += (aC[workingC+1-((workingC+1 > delayC)?delayC+1:0)] * ((offsetC-floor(offsetC))) );
		
		Float64 interpolD = (aD[workingD-((workingD > delayD)?delayD+1:0)] * (1-(offsetD-floor(offsetD))) );
		interpolD += (aD[workingD+1-((workingD+1 > delayD)?delayD+1:0)] * ((offsetD-floor(offsetD))) );
		
		Float64 interpolE = (aE[workingE-((workingE > delayE)?delayE+1:0)] * (1-(offsetE-floor(offsetE))) );
		interpolE += (aE[workingE+1-((workingE+1 > delayE)?delayE+1:0)] * ((offsetE-floor(offsetE))) );
		
		Float64 interpolF = (aF[workingF-((workingF > delayF)?delayF+1:0)] * (1-(offsetF-floor(offsetF))) );
		interpolF += (aF[workingF+1-((workingF+1 > delayF)?delayF+1:0)] * ((offsetF-floor(offsetF))) );
		
		Float64 interpolG = (aG[workingG-((workingG > delayG)?delayG+1:0)] * (1-(offsetG-floor(offsetG))) );
		interpolG += (aG[workingG+1-((workingG+1 > delayG)?delayG+1:0)] * ((offsetG-floor(offsetG))) );
		
		Float64 interpolH = (aH[workingH-((workingH > delayH)?delayH+1:0)] * (1-(offsetH-floor(offsetH))) );
		interpolH += (aH[workingH+1-((workingH+1 > delayH)?delayH+1:0)] * ((offsetH-floor(offsetH))) );
		
		interpolA = ((1.0-blend)*interpolA)+(aA[workingA-((workingA > delayA)?delayA+1:0)]*blend);
		interpolB = ((1.0-blend)*interpolB)+(aB[workingB-((workingB > delayB)?delayB+1:0)]*blend);
		interpolC = ((1.0-blend)*interpolC)+(aC[workingC-((workingC > delayC)?delayC+1:0)]*blend);
		interpolD = ((1.0-blend)*interpolD)+(aD[workingD-((workingD > delayD)?delayD+1:0)]*blend);
		interpolE = ((1.0-blend)*interpolE)+(aE[workingE-((workingE > delayE)?delayE+1:0)]*blend);
		interpolF = ((1.0-blend)*interpolF)+(aF[workingF-((workingF > delayF)?delayF+1:0)]*blend);
		interpolG = ((1.0-blend)*interpolG)+(aG[workingG-((workingG > delayG)?delayG+1:0)]*blend);
		interpolH = ((1.0-blend)*interpolH)+(aH[workingH-((workingH > delayH)?delayH+1:0)]*blend);
				
		feedbackA = (interpolA - (interpolB + interpolC + interpolD)) * regen;
		feedbackB = (interpolB - (interpolA + interpolC + interpolD)) * regen;
		feedbackC = (interpolC - (interpolA + interpolB + interpolD)) * regen;
		feedbackD = (interpolD - (interpolA + interpolB + interpolC)) * regen;
		
		feedbackE = (interpolE - (interpolF + interpolG + interpolH)) * regen;
		feedbackF = (interpolF - (interpolE + interpolG + interpolH)) * regen;
		feedbackG = (interpolG - (interpolE + interpolF + interpolH)) * regen;
		feedbackH = (interpolH - (interpolE + interpolF + interpolG)) * regen;
		
		inputSample = (interpolA + interpolB + interpolC + interpolD + interpolE + interpolF + interpolG + interpolH)/8.0;
		
		tempSample = biquadB[2]*inputSample+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
		biquadB[8] = biquadB[7]; biquadB[7] = inputSample; inputSample = tempSample; 
		biquadB[10] = biquadB[9]; biquadB[9] = inputSample; //DF1
		
		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
		//without this, you can get a NaN condition where it spits out DC offset at full blast!
		
		inputSample = asin(inputSample);
		
		tempSample = biquadC[2]*inputSample+biquadC[3]*biquadC[7]+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
		biquadC[8] = biquadC[7]; biquadC[7] = inputSample; inputSample = tempSample; 
		biquadC[10] = biquadC[9]; biquadC[9] = inputSample; //DF1
		
		if (wet !=1.0) {
			inputSample += (drySample * (1.0-wet));
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