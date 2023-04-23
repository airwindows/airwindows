/*
*	File:		MV2.cpp
*	
*	Version:	1.0
* 
*	Created:	2/27/23
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
	MV2.cpp
	
=============================================================================*/
#include "MV2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(MV2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MV2::MV2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MV2::MV2(AudioUnit component)
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
//	MV2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MV2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MV2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MV2::GetParameterInfo(AudioUnitScope		inScope,
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
//	MV2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MV2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MV2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MV2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	MV2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult MV2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____MV2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MV2::MV2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		MV2::MV2Kernel::Reset()
{
	int count;
	for(count = 0; count < 15149; count++) {aA[count] = 0.0;}
	for(count = 0; count < 14617; count++) {aB[count] = 0.0;}
	for(count = 0; count < 14357; count++) {aC[count] = 0.0;}
	for(count = 0; count < 13817; count++) {aD[count] = 0.0;}
	for(count = 0; count < 13561; count++) {aE[count] = 0.0;}
	for(count = 0; count < 13045; count++) {aF[count] = 0.0;}
	for(count = 0; count < 11965; count++) {aG[count] = 0.0;}
	for(count = 0; count < 11129; count++) {aH[count] = 0.0;}
	for(count = 0; count < 10597; count++) {aI[count] = 0.0;}
	for(count = 0; count < 9809; count++) {aJ[count] = 0.0;}
	for(count = 0; count < 9521; count++) {aK[count] = 0.0;}
	for(count = 0; count < 8981; count++) {aL[count] = 0.0;}
	for(count = 0; count < 8785; count++) {aM[count] = 0.0;}
	for(count = 0; count < 8461; count++) {aN[count] = 0.0;}
	for(count = 0; count < 8309; count++) {aO[count] = 0.0;}
	for(count = 0; count < 7981; count++) {aP[count] = 0.0;}
	for(count = 0; count < 7321; count++) {aQ[count] = 0.0;}
	for(count = 0; count < 6817; count++) {aR[count] = 0.0;}
	for(count = 0; count < 6505; count++) {aS[count] = 0.0;}
	for(count = 0; count < 6001; count++) {aT[count] = 0.0;}
	for(count = 0; count < 5837; count++) {aU[count] = 0.0;}
	for(count = 0; count < 5501; count++) {aV[count] = 0.0;}
	for(count = 0; count < 5009; count++) {aW[count] = 0.0;}
	for(count = 0; count < 4849; count++) {aX[count] = 0.0;}
	for(count = 0; count < 4295; count++) {aY[count] = 0.0;}
	for(count = 0; count < 4179; count++) {aZ[count] = 0.0;}	
	
	alpA = 1; delayA = 7573; avgA = 0.0;
	alpB = 1; delayB = 7307; avgB = 0.0;
	alpC = 1; delayC = 7177; avgC = 0.0;
	alpD = 1; delayD = 6907; avgD = 0.0;
	alpE = 1; delayE = 6779; avgE = 0.0;
	alpF = 1; delayF = 6521; avgF = 0.0;
	alpG = 1; delayG = 5981; avgG = 0.0;
	alpH = 1; delayH = 5563; avgH = 0.0;
	alpI = 1; delayI = 5297; avgI = 0.0;
	alpJ = 1; delayJ = 4903; avgJ = 0.0;
	alpK = 1; delayK = 4759; avgK = 0.0;
	alpL = 1; delayL = 4489; avgL = 0.0;
	alpM = 1; delayM = 4391; avgM = 0.0;
	alpN = 1; delayN = 4229; avgN = 0.0;
	alpO = 1; delayO = 4153; avgO = 0.0;
	alpP = 1; delayP = 3989; avgP = 0.0;
	alpQ = 1; delayQ = 3659; avgQ = 0.0;
	alpR = 1; delayR = 3407; avgR = 0.0;
	alpS = 1; delayS = 3251; avgS = 0.0;
	alpT = 1; delayT = 2999; avgT = 0.0;
	alpU = 1; delayU = 2917; avgU = 0.0;
	alpV = 1; delayV = 2749; avgV = 0.0;
	alpW = 1; delayW = 2503; avgW = 0.0;
	alpX = 1; delayX = 2423; avgX = 0.0;
	alpY = 1; delayY = 2146; avgY = 0.0;
	alpZ = 1; delayZ = 2088; avgZ = 0.0;
	
	feedback = 0.0;
	
	for(int count = 0; count < 6; count++) {lastRef[count] = 0.0;}
	cycle = 0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MV2::MV2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		MV2::MV2Kernel::Process(	const Float32 	*inSourceP,
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
	
	int allpasstemp;
	Float64 avgtemp;
	int stage = GetParameter( kParam_One ) * 27.0;
	int damp = (1.0-GetParameter( kParam_Two )) * stage;
	Float64 feedbacklevel = GetParameter( kParam_Three );
	if (feedbacklevel <= 0.0625) feedbacklevel = 0.0;
	if (feedbacklevel > 0.0625 && feedbacklevel <= 0.125) feedbacklevel = 0.0625; //-24db
	if (feedbacklevel > 0.125 && feedbacklevel <= 0.25) feedbacklevel = 0.125; //-18db
	if (feedbacklevel > 0.25 && feedbacklevel <= 0.5) feedbacklevel = 0.25; //-12db
	if (feedbacklevel > 0.5 && feedbacklevel <= 0.99) feedbacklevel = 0.5; //-6db
	if (feedbacklevel > 0.99) feedbacklevel = 1.0;
	//we're forcing even the feedback level to be Midiverb-ized
	Float64 gain = GetParameter( kParam_Four );
	Float64 wet = GetParameter( kParam_Five );
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a reverb sample
			
			
			
			inputSample += feedback;
			
			inputSample = sin(inputSample);
			
			
			switch (stage){
				case 27:
				case 26:
					allpasstemp = alpA - 1;
					if (allpasstemp < 0 || allpasstemp > delayA) {allpasstemp = delayA;}
					inputSample -= aA[allpasstemp]*0.5;
					aA[alpA] = inputSample;
					inputSample *= 0.5;
					alpA--; if (alpA < 0 || alpA > delayA) {alpA = delayA;}
					inputSample += (aA[alpA]);
					if (damp > 26) {
						avgtemp = inputSample;
						inputSample += avgA;
						inputSample *= 0.5;
						avgA = avgtemp;
					}
					//allpass filter A		
				case 25:
					allpasstemp = alpB - 1;
					if (allpasstemp < 0 || allpasstemp > delayB) {allpasstemp = delayB;}
					inputSample -= aB[allpasstemp]*0.5;
					aB[alpB] = inputSample;
					inputSample *= 0.5;
					alpB--; if (alpB < 0 || alpB > delayB) {alpB = delayB;}
					inputSample += (aB[alpB]);
					if (damp > 25) {
						avgtemp = inputSample;
						inputSample += avgB;
						inputSample *= 0.5;
						avgB = avgtemp;
					}
					//allpass filter B
				case 24:
					allpasstemp = alpC - 1;
					if (allpasstemp < 0 || allpasstemp > delayC) {allpasstemp = delayC;}
					inputSample -= aC[allpasstemp]*0.5;
					aC[alpC] = inputSample;
					inputSample *= 0.5;
					alpC--; if (alpC < 0 || alpC > delayC) {alpC = delayC;}
					inputSample += (aC[alpC]);
					if (damp > 24) {
						avgtemp = inputSample;
						inputSample += avgC;
						inputSample *= 0.5;
						avgC = avgtemp;
					}
					//allpass filter C
				case 23:
					allpasstemp = alpD - 1;
					if (allpasstemp < 0 || allpasstemp > delayD) {allpasstemp = delayD;}
					inputSample -= aD[allpasstemp]*0.5;
					aD[alpD] = inputSample;
					inputSample *= 0.5;
					alpD--; if (alpD < 0 || alpD > delayD) {alpD = delayD;}
					inputSample += (aD[alpD]);
					if (damp > 23) {
						avgtemp = inputSample;
						inputSample += avgD;
						inputSample *= 0.5;
						avgD = avgtemp;
					}
					//allpass filter D
				case 22:
					allpasstemp = alpE - 1;
					if (allpasstemp < 0 || allpasstemp > delayE) {allpasstemp = delayE;}
					inputSample -= aE[allpasstemp]*0.5;
					aE[alpE] = inputSample;
					inputSample *= 0.5;
					alpE--; if (alpE < 0 || alpE > delayE) {alpE = delayE;}
					inputSample += (aE[alpE]);
					if (damp > 22) {
						avgtemp = inputSample;
						inputSample += avgE;
						inputSample *= 0.5;
						avgE = avgtemp;
					}
					//allpass filter E
				case 21:
					allpasstemp = alpF - 1;
					if (allpasstemp < 0 || allpasstemp > delayF) {allpasstemp = delayF;}
					inputSample -= aF[allpasstemp]*0.5;
					aF[alpF] = inputSample;
					inputSample *= 0.5;
					alpF--; if (alpF < 0 || alpF > delayF) {alpF = delayF;}
					inputSample += (aF[alpF]);
					if (damp > 21) {
						avgtemp = inputSample;
						inputSample += avgF;
						inputSample *= 0.5;
						avgF = avgtemp;
					}
					//allpass filter F
				case 20:
					allpasstemp = alpG - 1;
					if (allpasstemp < 0 || allpasstemp > delayG) {allpasstemp = delayG;}
					inputSample -= aG[allpasstemp]*0.5;
					aG[alpG] = inputSample;
					inputSample *= 0.5;
					alpG--; if (alpG < 0 || alpG > delayG) {alpG = delayG;}
					inputSample += (aG[alpG]);
					if (damp > 20) {
						avgtemp = inputSample;
						inputSample += avgG;
						inputSample *= 0.5;
						avgG = avgtemp;
					}
					//allpass filter G
				case 19:
					allpasstemp = alpH - 1;
					if (allpasstemp < 0 || allpasstemp > delayH) {allpasstemp = delayH;}
					inputSample -= aH[allpasstemp]*0.5;
					aH[alpH] = inputSample;
					inputSample *= 0.5;
					alpH--; if (alpH < 0 || alpH > delayH) {alpH = delayH;}
					inputSample += (aH[alpH]);
					if (damp > 19) {
						avgtemp = inputSample;
						inputSample += avgH;
						inputSample *= 0.5;
						avgH = avgtemp;
					}
					//allpass filter H
				case 18:
					allpasstemp = alpI - 1;
					if (allpasstemp < 0 || allpasstemp > delayI) {allpasstemp = delayI;}
					inputSample -= aI[allpasstemp]*0.5;
					aI[alpI] = inputSample;
					inputSample *= 0.5;
					alpI--; if (alpI < 0 || alpI > delayI) {alpI = delayI;}
					inputSample += (aI[alpI]);
					if (damp > 18) {
						avgtemp = inputSample;
						inputSample += avgI;
						inputSample *= 0.5;
						avgI = avgtemp;
					}
					//allpass filter I
				case 17:
					allpasstemp = alpJ - 1;
					if (allpasstemp < 0 || allpasstemp > delayJ) {allpasstemp = delayJ;}
					inputSample -= aJ[allpasstemp]*0.5;
					aJ[alpJ] = inputSample;
					inputSample *= 0.5;
					alpJ--; if (alpJ < 0 || alpJ > delayJ) {alpJ = delayJ;}
					inputSample += (aJ[alpJ]);
					if (damp > 17) {
						avgtemp = inputSample;
						inputSample += avgJ;
						inputSample *= 0.5;
						avgJ = avgtemp;
					}
					//allpass filter J
				case 16:
					allpasstemp = alpK - 1;
					if (allpasstemp < 0 || allpasstemp > delayK) {allpasstemp = delayK;}
					inputSample -= aK[allpasstemp]*0.5;
					aK[alpK] = inputSample;
					inputSample *= 0.5;
					alpK--; if (alpK < 0 || alpK > delayK) {alpK = delayK;}
					inputSample += (aK[alpK]);
					if (damp > 16) {
						avgtemp = inputSample;
						inputSample += avgK;
						inputSample *= 0.5;
						avgK = avgtemp;
					}
					//allpass filter K
				case 15:
					allpasstemp = alpL - 1;
					if (allpasstemp < 0 || allpasstemp > delayL) {allpasstemp = delayL;}
					inputSample -= aL[allpasstemp]*0.5;
					aL[alpL] = inputSample;
					inputSample *= 0.5;
					alpL--; if (alpL < 0 || alpL > delayL) {alpL = delayL;}
					inputSample += (aL[alpL]);
					if (damp > 15) {
						avgtemp = inputSample;
						inputSample += avgL;
						inputSample *= 0.5;
						avgL = avgtemp;
					}
					//allpass filter L
				case 14:
					allpasstemp = alpM - 1;
					if (allpasstemp < 0 || allpasstemp > delayM) {allpasstemp = delayM;}
					inputSample -= aM[allpasstemp]*0.5;
					aM[alpM] = inputSample;
					inputSample *= 0.5;
					alpM--; if (alpM < 0 || alpM > delayM) {alpM = delayM;}
					inputSample += (aM[alpM]);
					if (damp > 14) {
						avgtemp = inputSample;
						inputSample += avgM;
						inputSample *= 0.5;
						avgM = avgtemp;
					}
					//allpass filter M
				case 13:
					allpasstemp = alpN - 1;
					if (allpasstemp < 0 || allpasstemp > delayN) {allpasstemp = delayN;}
					inputSample -= aN[allpasstemp]*0.5;
					aN[alpN] = inputSample;
					inputSample *= 0.5;
					alpN--; if (alpN < 0 || alpN > delayN) {alpN = delayN;}
					inputSample += (aN[alpN]);
					if (damp > 13) {
						avgtemp = inputSample;
						inputSample += avgN;
						inputSample *= 0.5;
						avgN = avgtemp;
					}
					//allpass filter N
				case 12:
					allpasstemp = alpO - 1;
					if (allpasstemp < 0 || allpasstemp > delayO) {allpasstemp = delayO;}
					inputSample -= aO[allpasstemp]*0.5;
					aO[alpO] = inputSample;
					inputSample *= 0.5;
					alpO--; if (alpO < 0 || alpO > delayO) {alpO = delayO;}
					inputSample += (aO[alpO]);
					if (damp > 12) {
						avgtemp = inputSample;
						inputSample += avgO;
						inputSample *= 0.5;
						avgO = avgtemp;
					}
					//allpass filter O
				case 11:
					allpasstemp = alpP - 1;
					if (allpasstemp < 0 || allpasstemp > delayP) {allpasstemp = delayP;}
					inputSample -= aP[allpasstemp]*0.5;
					aP[alpP] = inputSample;
					inputSample *= 0.5;
					alpP--; if (alpP < 0 || alpP > delayP) {alpP = delayP;}
					inputSample += (aP[alpP]);
					if (damp > 11) {
						avgtemp = inputSample;
						inputSample += avgP;
						inputSample *= 0.5;
						avgP = avgtemp;
					}
					//allpass filter P
				case 10:
					allpasstemp = alpQ - 1;
					if (allpasstemp < 0 || allpasstemp > delayQ) {allpasstemp = delayQ;}
					inputSample -= aQ[allpasstemp]*0.5;
					aQ[alpQ] = inputSample;
					inputSample *= 0.5;
					alpQ--; if (alpQ < 0 || alpQ > delayQ) {alpQ = delayQ;}
					inputSample += (aQ[alpQ]);
					if (damp > 10) {
						avgtemp = inputSample;
						inputSample += avgQ;
						inputSample *= 0.5;
						avgQ = avgtemp;
					}
					//allpass filter Q
				case 9:
					allpasstemp = alpR - 1;
					if (allpasstemp < 0 || allpasstemp > delayR) {allpasstemp = delayR;}
					inputSample -= aR[allpasstemp]*0.5;
					aR[alpR] = inputSample;
					inputSample *= 0.5;
					alpR--; if (alpR < 0 || alpR > delayR) {alpR = delayR;}
					inputSample += (aR[alpR]);
					if (damp > 9) {
						avgtemp = inputSample;
						inputSample += avgR;
						inputSample *= 0.5;
						avgR = avgtemp;
					}
					//allpass filter R
				case 8:
					allpasstemp = alpS - 1;
					if (allpasstemp < 0 || allpasstemp > delayS) {allpasstemp = delayS;}
					inputSample -= aS[allpasstemp]*0.5;
					aS[alpS] = inputSample;
					inputSample *= 0.5;
					alpS--; if (alpS < 0 || alpS > delayS) {alpS = delayS;}
					inputSample += (aS[alpS]);
					if (damp > 8) {
						avgtemp = inputSample;
						inputSample += avgS;
						inputSample *= 0.5;
						avgS = avgtemp;
					}
					//allpass filter S
				case 7:
					allpasstemp = alpT - 1;
					if (allpasstemp < 0 || allpasstemp > delayT) {allpasstemp = delayT;}
					inputSample -= aT[allpasstemp]*0.5;
					aT[alpT] = inputSample;
					inputSample *= 0.5;
					alpT--; if (alpT < 0 || alpT > delayT) {alpT = delayT;}
					inputSample += (aT[alpT]);
					if (damp > 7) {
						avgtemp = inputSample;
						inputSample += avgT;
						inputSample *= 0.5;
						avgT = avgtemp;
					}
					//allpass filter T
				case 6:
					allpasstemp = alpU - 1;
					if (allpasstemp < 0 || allpasstemp > delayU) {allpasstemp = delayU;}
					inputSample -= aU[allpasstemp]*0.5;
					aU[alpU] = inputSample;
					inputSample *= 0.5;
					alpU--; if (alpU < 0 || alpU > delayU) {alpU = delayU;}
					inputSample += (aU[alpU]);
					if (damp > 6) {
						avgtemp = inputSample;
						inputSample += avgU;
						inputSample *= 0.5;
						avgU = avgtemp;
					}
					//allpass filter U
				case 5:
					allpasstemp = alpV - 1;
					if (allpasstemp < 0 || allpasstemp > delayV) {allpasstemp = delayV;}
					inputSample -= aV[allpasstemp]*0.5;
					aV[alpV] = inputSample;
					inputSample *= 0.5;
					alpV--; if (alpV < 0 || alpV > delayV) {alpV = delayV;}
					inputSample += (aV[alpV]);
					if (damp > 5) {
						avgtemp = inputSample;
						inputSample += avgV;
						inputSample *= 0.5;
						avgV = avgtemp;
					}
					//allpass filter V
				case 4:
					allpasstemp = alpW - 1;
					if (allpasstemp < 0 || allpasstemp > delayW) {allpasstemp = delayW;}
					inputSample -= aW[allpasstemp]*0.5;
					aW[alpW] = inputSample;
					inputSample *= 0.5;
					alpW--; if (alpW < 0 || alpW > delayW) {alpW = delayW;}
					inputSample += (aW[alpW]);
					if (damp > 4) {
						avgtemp = inputSample;
						inputSample += avgW;
						inputSample *= 0.5;
						avgW = avgtemp;
					}
					//allpass filter W
				case 3:
					allpasstemp = alpX - 1;
					if (allpasstemp < 0 || allpasstemp > delayX) {allpasstemp = delayX;}
					inputSample -= aX[allpasstemp]*0.5;
					aX[alpX] = inputSample;
					inputSample *= 0.5;
					alpX--; if (alpX < 0 || alpX > delayX) {alpX = delayX;}
					inputSample += (aX[alpX]);
					if (damp > 3) {
						avgtemp = inputSample;
						inputSample += avgX;
						inputSample *= 0.5;
						avgX = avgtemp;
					}
					//allpass filter X
				case 2:
					allpasstemp = alpY - 1;
					if (allpasstemp < 0 || allpasstemp > delayY) {allpasstemp = delayY;}
					inputSample -= aY[allpasstemp]*0.5;
					aY[alpY] = inputSample;
					inputSample *= 0.5;
					alpY--; if (alpY < 0 || alpY > delayY) {alpY = delayY;}
					inputSample += (aY[alpY]);
					if (damp > 2) {
						avgtemp = inputSample;
						inputSample += avgY;
						inputSample *= 0.5;
						avgY = avgtemp;
					}
					//allpass filter Y
				case 1:
					allpasstemp = alpZ - 1;
					if (allpasstemp < 0 || allpasstemp > delayZ) {allpasstemp = delayZ;}
					inputSample -= aZ[allpasstemp]*0.5;
					aZ[alpZ] = inputSample;
					inputSample *= 0.5;
					alpZ--; if (alpZ < 0 || alpZ > delayZ) {alpZ = delayZ;}
					inputSample += (aZ[alpZ]);
					if (damp > 1) {
						avgtemp = inputSample;
						inputSample += avgZ;
						inputSample *= 0.5;
						avgZ = avgtemp;
					}
					//allpass filter Z
			}
			
			feedback = inputSample * feedbacklevel;
			
			if (gain != 1.0) {
				inputSample *= gain;
			}
			//we can pad with the gain to tame distortyness from the PurestConsole code
			
			if (inputSample > 1.0) inputSample = 1.0;
			if (inputSample < -1.0) inputSample = -1.0;
			//without this, you can get a NaN condition where it spits out DC offset at full blast!
			
			inputSample = asin(inputSample);
			
			
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
		
		
		if (wet !=1.0) {
			inputSample = (inputSample * wet) + (drySample * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

