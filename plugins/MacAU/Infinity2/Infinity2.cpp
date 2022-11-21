/*
*	File:		Infinity2.cpp
*	
*	Version:	1.0
* 
*	Created:	4/26/21
*	
*	Copyright:  Copyright © 2021 Airwindows, Airwindows uses the MIT license
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
	Infinity2.cpp
	
=============================================================================*/
#include "Infinity2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Infinity2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Infinity2::Infinity2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Infinity2::Infinity2(AudioUnit component)
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
//	Infinity2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Infinity2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Infinity2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Infinity2::GetParameterInfo(AudioUnitScope		inScope,
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
//	Infinity2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Infinity2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Infinity2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Infinity2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Infinity2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Infinity2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Infinity2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Infinity2::Infinity2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Infinity2::Infinity2Kernel::Reset()
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
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Infinity2::Infinity2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Infinity2::Infinity2Kernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	biquadC[0] = biquadB[0] = biquadA[0] = ((pow(GetParameter( kParam_One ),2)*9900.0)+100.0) / GetSampleRate();
	biquadA[1] = 0.618033988749894848204586;
	biquadB[1] = (GetParameter( kParam_One )*0.5)+0.118033988749894848204586;
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
	
	Float64 size = (pow(GetParameter( kParam_Two ),2)*99.0)+1.0;
	Float64 damping = pow(GetParameter( kParam_Three ),2)*0.5;
	Float64 rawPass = GetParameter( kParam_Four );
	Float64 feedback = 1.0-(pow(1.0-GetParameter( kParam_Five ),4));
	Float64 wet = GetParameter( kParam_Six );
	
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
	
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		double tempSample = biquadA[2]*inputSample+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
		biquadA[8] = biquadA[7]; biquadA[7] = inputSample; inputSample = tempSample; 
		biquadA[10] = biquadA[9]; biquadA[9] = inputSample; //DF1
				
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
		
		if (rawPass !=1.0) {
			allpassI = (allpassI * rawPass) + (drySample * (1.0-rawPass));
			allpassJ = (allpassJ * rawPass) + (drySample * (1.0-rawPass));
			allpassK = (allpassK * rawPass) + (drySample * (1.0-rawPass));
			allpassL = (allpassL * rawPass) + (drySample * (1.0-rawPass));
		}
		
		aA[countA] = allpassI + (feedbackA*feedback);
		aB[countB] = allpassJ + (feedbackB*feedback);
		aC[countC] = allpassK + (feedbackC*feedback);
		aD[countD] = allpassL + (feedbackD*feedback);
		aE[countE] = allpassI + (feedbackE*feedback);
		aF[countF] = allpassJ + (feedbackF*feedback);
		aG[countG] = allpassK + (feedbackG*feedback);
		aH[countH] = allpassL + (feedbackH*feedback);
		
		countA++; if (countA < 0 || countA > delayA) {countA = 0;}
		countB++; if (countB < 0 || countB > delayB) {countB = 0;}
		countC++; if (countC < 0 || countC > delayC) {countC = 0;}
		countD++; if (countD < 0 || countD > delayD) {countD = 0;}
		countE++; if (countE < 0 || countE > delayE) {countE = 0;}
		countF++; if (countF < 0 || countF > delayF) {countF = 0;}
		countG++; if (countG < 0 || countG > delayG) {countG = 0;}
		countH++; if (countH < 0 || countH > delayH) {countH = 0;}
		//the Householder matrices
		
		Float64 infiniteA = (aA[countA-((countA > delayA)?delayA+1:0)] * (1-(damping-floor(damping))) );
		infiniteA += (aA[countA+1-((countA+1 > delayA)?delayA+1:0)] * ((damping-floor(damping))) );
		Float64 infiniteB = (aB[countB-((countB > delayB)?delayB+1:0)] * (1-(damping-floor(damping))) );
		infiniteB += (aB[countB+1-((countB+1 > delayB)?delayB+1:0)] * ((damping-floor(damping))) );
		Float64 infiniteC = (aC[countC-((countC > delayC)?delayC+1:0)] * (1-(damping-floor(damping))) );
		infiniteC += (aC[countC+1-((countC+1 > delayC)?delayC+1:0)] * ((damping-floor(damping))) );
		Float64 infiniteD = (aD[countD-((countD > delayD)?delayD+1:0)] * (1-(damping-floor(damping))) );
		infiniteD += (aD[countD+1-((countD+1 > delayD)?delayD+1:0)] * ((damping-floor(damping))) );
		
		Float64 infiniteE = (aE[countE-((countE > delayE)?delayE+1:0)] * (1-(damping-floor(damping))) );
		infiniteE += (aE[countE+1-((countE+1 > delayE)?delayE+1:0)] * ((damping-floor(damping))) );
		Float64 infiniteF = (aF[countF-((countF > delayF)?delayF+1:0)] * (1-(damping-floor(damping))) );
		infiniteF += (aF[countF+1-((countF+1 > delayF)?delayF+1:0)] * ((damping-floor(damping))) );
		Float64 infiniteG = (aG[countG-((countG > delayG)?delayG+1:0)] * (1-(damping-floor(damping))) );
		infiniteG += (aG[countG+1-((countG+1 > delayG)?delayG+1:0)] * ((damping-floor(damping))) );
		Float64 infiniteH = (aH[countH-((countH > delayH)?delayH+1:0)] * (1-(damping-floor(damping))) );
		infiniteH += (aH[countH+1-((countH+1 > delayH)?delayH+1:0)] * ((damping-floor(damping))) );
		
		Float64 dialBackA = 0.5;
		Float64 dialBackE = 0.5;
		Float64 dialBackDry = 0.5;
		if (fabs(infiniteA)>0.4) dialBackA -= ((fabs(infiniteA)-0.4)*0.2);
		if (fabs(infiniteE)>0.4) dialBackE -= ((fabs(infiniteE)-0.4)*0.2);
		if (fabs(drySample)>0.4) dialBackDry -= ((fabs(drySample)-0.4)*0.2);
		//we're compressing things subtly so we can feed energy in and not overload
		
		feedbackA = (infiniteA - (infiniteB + infiniteC + infiniteD))*dialBackA;
		feedbackB = (infiniteB - (infiniteA + infiniteC + infiniteD))*dialBackDry;
		feedbackC = (infiniteC - (infiniteA + infiniteB + infiniteD))*dialBackDry;
		feedbackD = (infiniteD - (infiniteA + infiniteB + infiniteC))*dialBackDry;
		
		feedbackE = (infiniteE - (infiniteF + infiniteG + infiniteH))*dialBackE;
		feedbackF = (infiniteF - (infiniteE + infiniteG + infiniteH))*dialBackDry;
		feedbackG = (infiniteG - (infiniteE + infiniteF + infiniteH))*dialBackDry;
		feedbackH = (infiniteH - (infiniteE + infiniteF + infiniteG))*dialBackDry;
		
		inputSample = (infiniteA + infiniteB + infiniteC + infiniteD + infiniteE + infiniteF + infiniteG + infiniteH)/8.0;
		
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