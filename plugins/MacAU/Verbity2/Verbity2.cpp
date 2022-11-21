/*
*	File:		Verbity2.cpp
*	
*	Version:	1.0
* 
*	Created:	9/3/22
*	
*	Copyright:  Copyright © 2022 Airwindows, Airwindows uses the MIT license
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

COMPONENT_ENTRY(Verbity2)


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
void		Verbity2::Verbity2Kernel::Reset()
{
	iirA = 0.0;
	iirB = 0.0;
	
	for(int count = 0; count < 5190; count++) {aZ[count] = 0.0;}
	
	for(int count = 0; count < 5004; count++) {aA[count] = 0.0;} //AFKPU
	for(int count = 0; count < 4952; count++) {aF[count] = 0.0;}
	for(int count = 0; count < 4920; count++) {aK[count] = 0.0;}
	for(int count = 0; count < 4800; count++) {aP[count] = 0.0;}
	for(int count = 0; count < 4752; count++) {aU[count] = 0.0;}
	
	for(int count = 0; count < 4350; count++) {aB[count] = 0.0;} //BGLQV
	for(int count = 0; count < 4158; count++) {aG[count] = 0.0;}
	for(int count = 0; count < 3930; count++) {aL[count] = 0.0;}
	for(int count = 0; count < 3530; count++) {aQ[count] = 0.0;}
	for(int count = 0; count < 3330; count++) {aV[count] = 0.0;}
	
	for(int count = 0; count < 3324; count++) {aC[count] = 0.0;} //CHMRW
	for(int count = 0; count < 2792; count++) {aH[count] = 0.0;}
	for(int count = 0; count < 2768; count++) {aM[count] = 0.0;}
	for(int count = 0; count < 2390; count++) {aR[count] = 0.0;}
	for(int count = 0; count < 2348; count++) {aW[count] = 0.0;}
	
	for(int count = 0; count < 2142; count++) {aD[count] = 0.0;} //DINSX
	for(int count = 0; count < 1812; count++) {aI[count] = 0.0;}
	for(int count = 0; count < 1734; count++) {aN[count] = 0.0;}
	for(int count = 0; count < 1172; count++) {aS[count] = 0.0;}
	for(int count = 0; count < 788; count++) {aX[count] = 0.0;}
	
	for(int count = 0; count < 678; count++) {aE[count] = 0.0;} //EJOTY
	for(int count = 0; count < 644; count++) {aJ[count] = 0.0;}
	for(int count = 0; count < 440; count++) {aO[count] = 0.0;}
	for(int count = 0; count < 350; count++) {aT[count] = 0.0;}
	for(int count = 0; count < 282; count++) {aY[count] = 0.0;}
	
	feedbackA = 0.0;
	feedbackB = 0.0;
	feedbackC = 0.0;
	feedbackD = 0.0;
	feedbackE = 0.0;
	
	previousA = 0.0;
	previousB = 0.0;
	previousC = 0.0;
	previousD = 0.0;
	previousE = 0.0;
	
	for(int count = 0; count < 6; count++) {lastRef[count] = 0.0;}
		
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
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Verbity2::Verbity2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Verbity2::Verbity2Kernel::Process(	const Float32 	*inSourceP,
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
	
	//mulch is tone control, darken to obscure the Chrome Oxide, not as much highpass
	Float64 size = (pow(GetParameter( kParam_One ),2.0)*0.9)+0.1;
	Float64 regen = (1.0-pow(1.0-GetParameter( kParam_Two ),3.0))*0.00032;
	Float64 mulchSetting = 1.0-pow(1.0-GetParameter( kParam_Three ),2); //modify taper
	Float64 lowpass = (1.0-(pow(mulchSetting,2)*0.75))/sqrt(overallscale);
	Float64 highpass = (0.01+(mulchSetting*0.05))/sqrt(overallscale);
	Float64 interpolateMax = 0.06+(mulchSetting*0.3);
	Float64 wet = GetParameter( kParam_Four )*2.0;
	Float64 dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0;
	if (wet < 0.0) wet = 0.0;
	if (dry > 1.0) dry = 1.0;
	if (dry < 0.0) dry = 0.0;
	//this reverb makes 50% full dry AND full wet, not crossfaded.
	//that's so it can be on submixes without cutting back dry channel when adjusted:
	//unless you go super heavy, you are only adjusting the added verb loudness.
	
	delayZ = 5189.0*size; //z can be predelay
	
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
	delayM= 2767.0*size;
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
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		if (fabs(iirA)<1.18e-37) iirA = 0.0;
		iirA = (iirA*(1.0-highpass))+(inputSample*highpass); inputSample -= iirA;
		//first filter is highpass
		
		double interpolate = interpolateMax + (interpolateMax * (double(fpd)/UINT32_MAX));
		//we have our feedback soften also working as Chrome Oxide soften/noise
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a reverb sample
			feedbackA = (feedbackA*(1.0-interpolate))+(previousA*interpolate); previousA = feedbackA;
			feedbackB = (feedbackB*(1.0-interpolate))+(previousB*interpolate); previousB = feedbackB;
			feedbackC = (feedbackC*(1.0-interpolate))+(previousC*interpolate); previousC = feedbackC;
			feedbackD = (feedbackD*(1.0-interpolate))+(previousD*interpolate); previousD = feedbackD;
			feedbackE = (feedbackE*(1.0-interpolate))+(previousE*interpolate); previousE = feedbackE;

			aZ[countZ] = inputSample;
			countZ++; if (countZ < 0 || countZ > delayZ) countZ = 0;
			inputSample = aZ[countZ-((countZ > delayZ)?delayZ+1:0)];
			//predelay 
			
			aA[countA] = inputSample + (feedbackA * (regen*(1.0-fabs(feedbackA*regen))));
			aB[countB] = inputSample + (feedbackB * (regen*(1.0-fabs(feedbackB*regen))));
			aC[countC] = inputSample + (feedbackC * (regen*(1.0-fabs(feedbackC*regen))));
			aD[countD] = inputSample + (feedbackD * (regen*(1.0-fabs(feedbackD*regen))));
			aE[countE] = inputSample + (feedbackE * (regen*(1.0-fabs(feedbackE*regen))));
			
			countA++; if (countA < 0 || countA > delayA) countA = 0;
			countB++; if (countB < 0 || countB > delayB) countB = 0;
			countC++; if (countC < 0 || countC > delayC) countC = 0;
			countD++; if (countD < 0 || countD > delayD) countD = 0;
			countE++; if (countE < 0 || countE > delayE) countE = 0;
			
			double outA = aA[countA-((countA > delayA)?delayA+1:0)];
			double outB = aB[countB-((countB > delayB)?delayB+1:0)];
			double outC = aC[countC-((countC > delayC)?delayC+1:0)];
			double outD = aD[countD-((countD > delayD)?delayD+1:0)];
			double outE = aE[countE-((countE > delayE)?delayE+1:0)];
			
			//-------- one
			
			aF[countF] = ((outA*3.0) - ((outB + outC + outD + outE)*2.0));
			aG[countG] = ((outB*3.0) - ((outA + outC + outD + outE)*2.0));
			aH[countH] = ((outC*3.0) - ((outA + outB + outD + outE)*2.0));
			aI[countI] = ((outD*3.0) - ((outA + outB + outC + outE)*2.0));
			aJ[countJ] = ((outE*3.0) - ((outA + outB + outC + outD)*2.0));
			
			countF++; if (countF < 0 || countF > delayF) countF = 0;
			countG++; if (countG < 0 || countG > delayG) countG = 0;
			countH++; if (countH < 0 || countH > delayH) countH = 0;
			countI++; if (countI < 0 || countI > delayI) countI = 0;
			countJ++; if (countJ < 0 || countJ > delayJ) countJ = 0;
			
			double outF = aF[countF-((countF > delayF)?delayF+1:0)];
			double outG = aG[countG-((countG > delayG)?delayG+1:0)];
			double outH = aH[countH-((countH > delayH)?delayH+1:0)];
			double outI = aI[countI-((countI > delayI)?delayI+1:0)];
			double outJ = aJ[countJ-((countJ > delayJ)?delayJ+1:0)];
			
			//-------- two
			
			aK[countK] = ((outF*3.0) - ((outG + outH + outI + outJ)*2.0));
			aL[countL] = ((outG*3.0) - ((outF + outH + outI + outJ)*2.0));
			aM[countM] = ((outH*3.0) - ((outF + outG + outI + outJ)*2.0));
			aN[countN] = ((outI*3.0) - ((outF + outG + outH + outJ)*2.0));
			aO[countO] = ((outJ*3.0) - ((outF + outG + outH + outI)*2.0));
			
			countK++; if (countK < 0 || countK > delayK) countK = 0;
			countL++; if (countL < 0 || countL > delayL) countL = 0;
			countM++; if (countM < 0 || countM > delayM) countM = 0;
			countN++; if (countN < 0 || countN > delayN) countN = 0;
			countO++; if (countO < 0 || countO > delayO) countO = 0;
			
			double outK = aK[countK-((countK > delayK)?delayK+1:0)];
			double outL = aL[countL-((countL > delayL)?delayL+1:0)];
			double outM = aM[countM-((countM > delayM)?delayM+1:0)];
			double outN = aN[countN-((countN > delayN)?delayN+1:0)];
			double outO = aO[countO-((countO > delayO)?delayO+1:0)];
			
			//-------- three
			
			aP[countP] = ((outK*3.0) - ((outL + outM + outN + outO)*2.0));
			aQ[countQ] = ((outL*3.0) - ((outK + outM + outN + outO)*2.0));
			aR[countR] = ((outM*3.0) - ((outK + outL + outN + outO)*2.0));
			aS[countS] = ((outN*3.0) - ((outK + outL + outM + outO)*2.0));
			aT[countT] = ((outO*3.0) - ((outK + outL + outM + outN)*2.0));
			
			countP++; if (countP < 0 || countP > delayP) countP = 0;
			countQ++; if (countQ < 0 || countQ > delayQ) countQ = 0;
			countR++; if (countR < 0 || countR > delayR) countR = 0;
			countS++; if (countS < 0 || countS > delayS) countS = 0;
			countT++; if (countT < 0 || countT > delayT) countT = 0;
			
			double outP = aP[countP-((countP > delayP)?delayP+1:0)];
			double outQ = aQ[countQ-((countQ > delayQ)?delayQ+1:0)];
			double outR = aR[countR-((countR > delayR)?delayR+1:0)];
			double outS = aS[countS-((countS > delayS)?delayS+1:0)];
			double outT = aT[countT-((countT > delayT)?delayT+1:0)];
		
			//-------- four
			
			aU[countU] = ((outP*3.0) - ((outQ + outR + outS + outT)*2.0));
			aV[countV] = ((outQ*3.0) - ((outP + outR + outS + outT)*2.0));
			aW[countW] = ((outR*3.0) - ((outP + outQ + outS + outT)*2.0));
			aX[countX] = ((outS*3.0) - ((outP + outQ + outR + outT)*2.0));
			aY[countY] = ((outT*3.0) - ((outP + outQ + outR + outS)*2.0));
			
			countU++; if (countU < 0 || countU > delayU) countU = 0;
			countV++; if (countV < 0 || countV > delayV) countV = 0;
			countW++; if (countW < 0 || countW > delayW) countW = 0;
			countX++; if (countX < 0 || countX > delayX) countX = 0;
			countY++; if (countY < 0 || countY > delayY) countY = 0;
			
			double outU = aU[countU-((countU > delayU)?delayU+1:0)];
			double outV = aV[countV-((countV > delayV)?delayV+1:0)];
			double outW = aW[countW-((countW > delayW)?delayW+1:0)];
			double outX = aX[countX-((countX > delayX)?delayX+1:0)];
			double outY = aY[countY-((countY > delayY)?delayY+1:0)];
			
			//-------- five
			
			
			feedbackA = ((outU*3.0) - ((outV + outW + outX + outY)*2.0));
			feedbackB = ((outV*3.0) - ((outU + outW + outX + outY)*2.0));
			feedbackC = ((outW*3.0) - ((outU + outV + outX + outY)*2.0));
			feedbackD = ((outX*3.0) - ((outU + outV + outW + outY)*2.0));
			feedbackE = ((outY*3.0) - ((outU + outV + outW + outX)*2.0));
			//which we need to feed back into the input again, a bit
			
			inputSample = (outU + outV + outW + outX + outY)*0.0016;
			//and take the final combined sum of outputs, corrected for Householder gain
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
		
		if (fabs(iirB)<1.18e-37) iirB = 0.0;
		iirB = (iirB*(1.0-lowpass))+(inputSample*lowpass); inputSample = iirB;
		//second filter
		
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

