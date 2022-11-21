/*
 *	File:		Energy2.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	6/28/21
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
 Energy2.cpp
 
 =============================================================================*/
#include "Energy2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Energy2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy2::Energy2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Energy2::Energy2(AudioUnit component)
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
	SetParameter(kParam_Seven, kDefaultValue_ParamSeven );
	SetParameter(kParam_Eight, kDefaultValue_ParamEight );
	SetParameter(kParam_Nine, kDefaultValue_ParamNine );
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Energy2::GetParameterValueStrings(AudioUnitScope		inScope,
														  AudioUnitParameterID	inParameterID,
														  CFArrayRef *		outStrings)
{
	
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Energy2::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
                break;
			case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFour;
                break;
			case kParam_Five:
                AUBase::FillInParameterName (outParameterInfo, kParameterFiveName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFive;
                break;
			case kParam_Six:
                AUBase::FillInParameterName (outParameterInfo, kParameterSixName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSix;
                break;
			case kParam_Seven:
                AUBase::FillInParameterName (outParameterInfo, kParameterSevenName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSeven;
                break;
			case kParam_Eight:
                AUBase::FillInParameterName (outParameterInfo, kParameterEightName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamEight;
                break;
			case kParam_Nine:
                AUBase::FillInParameterName (outParameterInfo, kParameterNineName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamNine;
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
//	Energy2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Energy2::GetPropertyInfo (AudioUnitPropertyID	inID,
												  AudioUnitScope		inScope,
												  AudioUnitElement	inElement,
												  UInt32 &		outDataSize,
												  Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Energy2::GetProperty(	AudioUnitPropertyID inID,
											 AudioUnitScope 		inScope,
											 AudioUnitElement 	inElement,
											 void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Energy2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Energy2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Energy2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy2::Energy2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Energy2::Energy2Kernel::Reset()
{
	duoEven = duoOdd = 0.0;
	duoFactor = 0.0;
	flip = false;
	
	tripletA = tripletB = tripletC = 0.0;
	tripletFactor = 0.0;
	countA = 1;
	
	quadA = quadB = quadC = quadD = 0.0;
	quadFactor = 0.0;
	countB = 1;
	
	quintA = quintB = quintC = quintD = quintE = 0.0;
	quintFactor = 0.0;
	countC = 1;
	
	sextA = sextB = sextC = 0.0;
	sextD = sextE = sextF = 0.0;
	sextFactor = 0.0;
	countD = 1;
	
	septA = septB = septC = septD = 0.0;
	septE = septF = septG = 0.0;
	septFactor = 0.0;
	countE = 1;
	
	octA = octB = octC = octD = 0.0;
	octE = octF = octG = octH = 0.0;
	octFactor = 0.0;
	countF = 1;
	
	nintA = nintB = nintC = nintD = nintE = 0.0;
	nintF = nintG = nintH = nintI = 0.0;
	nintFactor = 0.0;
	countG = 1;
	
	PrevH = PrevG = PrevF = PrevE = 0.0;
	PrevD = PrevC = PrevB = PrevA = 0.0;
	
	for(int count = 0; count < 9; count++) {lastRef[count] = 0.0;}
	cycle = 0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy2::Energy2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Energy2::Energy2Kernel::Process(	const Float32 	*inSourceP,
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
	
	Float64 duoIntensity = -pow(GetParameter( kParam_One ),3);
	Float64 tripletIntensity = -pow(GetParameter( kParam_Two ),3);
	Float64 quadIntensity = -pow(GetParameter( kParam_Three ),3);
	Float64 quintIntensity = -pow(GetParameter( kParam_Four ),3);
	Float64 sextIntensity = -pow(GetParameter( kParam_Five ),3);
	Float64 septIntensity = -pow(GetParameter( kParam_Six ),3);
	Float64 octIntensity = -pow(GetParameter( kParam_Seven ),3);
	Float64 nintIntensity = -pow(GetParameter( kParam_Eight ),3);
	Float64 mix = GetParameter( kParam_Nine );
	//all types of air band are running in parallel, not series
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do an Energy sample
			double correction = 0.0;
			
			if (fabs(duoIntensity) > 0.0001) {
				duoFactor = PrevA - inputSample;
				if (flip)
				{
					duoEven += duoFactor; duoOdd -= duoFactor;
					duoFactor = duoEven * duoIntensity;
				} else {
					duoOdd += duoFactor; duoEven -= duoFactor;
					duoFactor = duoOdd * duoIntensity;
				}
				duoOdd = (duoOdd - ((duoOdd - duoEven)/256.0)) / 2.0;
				duoEven = (duoEven - ((duoEven - duoOdd)/256.0)) / 2.0;
				correction = correction + duoFactor;
				flip = !flip;
			}//finished duo section
			
			if (fabs(tripletIntensity) > 0.0001) {
				if (countA < 1 || countA > 3) countA = 1;
				switch (countA)
				{
					case 1:
						tripletFactor = PrevB - inputSample;
						tripletA += tripletFactor; tripletC -= tripletFactor;
						tripletFactor = tripletA * tripletIntensity;
						break;
					case 2:
						tripletFactor = PrevB - inputSample;
						tripletB += tripletFactor; tripletA -= tripletFactor;
						tripletFactor = tripletB * tripletIntensity;
						break;
					case 3:
						tripletFactor = PrevB - inputSample;
						tripletC += tripletFactor; tripletB -= tripletFactor;
						tripletFactor = tripletC * tripletIntensity;
						break;
				}
				tripletA /= 2.0; tripletB /= 2.0; tripletC /= 2.0;
				correction = correction + tripletFactor;
				countA++;
			}//finished triplet section- 15K
			
			if (fabs(quadIntensity) > 0.0001) {
				if (countB < 1 || countB > 4) countB = 1;
				switch (countB)
				{
					case 1:
						quadFactor = PrevC - inputSample;
						quadA += quadFactor; quadD -= quadFactor;
						quadFactor = quadA * quadIntensity;
						break;
					case 2:
						quadFactor = PrevC - inputSample;
						quadB += quadFactor; quadA -= quadFactor;
						quadFactor = quadB * quadIntensity;
						break;
					case 3:
						quadFactor = PrevC - inputSample;
						quadC += quadFactor; quadB -= quadFactor;
						quadFactor = quadC * quadIntensity;
						break;
					case 4:
						quadFactor = PrevC - inputSample;
						quadD += quadFactor; quadC -= quadFactor;
						quadFactor = quadD * quadIntensity;
						break;
				}
				quadA /= 2.0; quadB /= 2.0; quadC /= 2.0; quadD /= 2.0;
				correction = correction + quadFactor;
				countB++;
			}//finished quad section- 10K
			
			if (fabs(quintIntensity) > 0.0001) {
				if (countC < 1 || countC > 5) countC = 1;
				switch (countC)
				{
					case 1:
						quintFactor = PrevD - inputSample;
						quintA += quintFactor; quintE -= quintFactor;
						quintFactor = quintA * quintIntensity;
						break;
					case 2:
						quintFactor = PrevD - inputSample;
						quintB += quintFactor; quintA -= quintFactor;
						quintFactor = quintB * quintIntensity;
						break;
					case 3:
						quintFactor = PrevD - inputSample;
						quintC += quintFactor; quintB -= quintFactor;
						quintFactor = quintC * quintIntensity;
						break;
					case 4:
						quintFactor = PrevD - inputSample;
						quintD += quintFactor; quintC -= quintFactor;
						quintFactor = quintD * quintIntensity;
						break;
					case 5:
						quintFactor = PrevD - inputSample;
						quintE += quintFactor; quintD -= quintFactor;
						quintFactor = quintE * quintIntensity;
						break;
				}
				quintA /= 2.0; quintB /= 2.0; quintC /= 2.0; quintD /= 2.0; quintE /= 2.0;
				correction = correction + quintFactor;
				countC++;
			}//finished quint section- 8K
			
			if (fabs(sextIntensity) > 0.0001) {
				if (countD < 1 || countD > 6) countD = 1;
				switch (countD)
				{
					case 1:
						sextFactor = PrevE - inputSample;
						sextA += sextFactor; sextF -= sextFactor;
						sextFactor = sextA * sextIntensity;
						break;
					case 2:
						sextFactor = PrevE - inputSample;
						sextB += sextFactor; sextA -= sextFactor;
						sextFactor = sextB * sextIntensity;
						break;
					case 3:
						sextFactor = PrevE - inputSample;
						sextC += sextFactor; sextB -= sextFactor;
						sextFactor = sextC * sextIntensity;
						break;
					case 4:
						sextFactor = PrevE - inputSample;
						sextD += sextFactor; sextC -= sextFactor;
						sextFactor = sextD * sextIntensity;
						break;
					case 5:
						sextFactor = PrevE - inputSample;
						sextE += sextFactor; sextD -= sextFactor;
						sextFactor = sextE * sextIntensity;
						break;
					case 6:
						sextFactor = PrevE - inputSample;
						sextF += sextFactor; sextE -= sextFactor;
						sextFactor = sextF * sextIntensity;
						break;
				}
				sextA /= 2.0; sextB /= 2.0; sextC /= 2.0;
				sextD /= 2.0; sextE /= 2.0; sextF /= 2.0;
				correction = correction + sextFactor;
				countD++;
			}//finished sext section- 6K
			
			if (fabs(septIntensity) > 0.0001) {
				if (countE < 1 || countE > 7) countE = 1;
				switch (countE)
				{
					case 1:
						septFactor = PrevF - inputSample;
						septA += septFactor; septG -= septFactor;
						septFactor = septA * septIntensity;
						break;
					case 2:
						septFactor = PrevF - inputSample;
						septB += septFactor; septA -= septFactor;
						septFactor = septB * septIntensity;
						break;
					case 3:
						septFactor = PrevF - inputSample;
						septC += septFactor; septB -= septFactor;
						septFactor = septC * septIntensity;
						break;
					case 4:
						septFactor = PrevF - inputSample;
						septD += septFactor; septC -= septFactor;
						septFactor = septD * septIntensity;
						break;
					case 5:
						septFactor = PrevF - inputSample;
						septE += septFactor; septD -= septFactor;
						septFactor = septE * septIntensity;
						break;
					case 6:
						septFactor = PrevF - inputSample;
						septF += septFactor; septE -= septFactor;
						septFactor = septF * septIntensity;
						break;
					case 7:
						septFactor = PrevF - inputSample;
						septG += septFactor; septF -= septFactor;
						septFactor = septG * septIntensity;
						break;
				}
				septA /= 2.0; septB /= 2.0; septC /= 2.0; septD /= 2.0;
				septE /= 2.0; septF /= 2.0; septG /= 2.0;
				correction = correction + septFactor;
				countE++;
			}//finished sept section- 5K
			
			if (fabs(octIntensity) > 0.0001) {
				if (countF < 1 || countF > 8) countF = 1;
				switch (countF)
				{
					case 1:
						octFactor = PrevG - inputSample;
						octA += octFactor; octH -= octFactor;
						octFactor = octA * octIntensity;
						break;
					case 2:
						octFactor = PrevG - inputSample;
						octB += octFactor; octA -= octFactor;
						octFactor = octB * octIntensity;
						break;
					case 3:
						octFactor = PrevG - inputSample;
						octC += octFactor; octB -= octFactor;
						octFactor = octC * octIntensity;
						break;
					case 4:
						octFactor = PrevG - inputSample;
						octD += octFactor; octC -= octFactor;
						octFactor = octD * octIntensity;
						break;
					case 5:
						octFactor = PrevG - inputSample;
						octE += octFactor; octD -= octFactor;
						octFactor = octE * octIntensity;
						break;
					case 6:
						octFactor = PrevG - inputSample;
						octF += octFactor; octE -= octFactor;
						octFactor = octF * octIntensity;
						break;
					case 7:
						octFactor = PrevG - inputSample;
						octG += octFactor; octF -= octFactor;
						octFactor = octG * octIntensity;
						break;
					case 8:
						octFactor = PrevG - inputSample;
						octH += octFactor; octG -= octFactor;
						octFactor = octH * octIntensity;
						break;
				}
				octA /= 2.0; octB /= 2.0; octC /= 2.0; octD /= 2.0;
				octE /= 2.0; octF /= 2.0; octG /= 2.0; octH /= 2.0;
				correction = correction + octFactor;
				countF++;
			}//finished oct section- 4K
			
			if (fabs(nintIntensity) > 0.0001) {
				if (countG < 1 || countG > 9) countG = 1;
				switch (countG)
				{
					case 1:
						nintFactor = PrevH - inputSample;
						nintA += nintFactor; nintI -= nintFactor;
						nintFactor = nintA * nintIntensity;
						break;
					case 2:
						nintFactor = PrevH - inputSample;
						nintB += nintFactor; nintA -= nintFactor;
						nintFactor = nintB * nintIntensity;
						break;
					case 3:
						nintFactor = PrevH - inputSample;
						nintC += nintFactor; nintB -= nintFactor;
						nintFactor = nintC * nintIntensity;
						break;
					case 4:
						nintFactor = PrevH - inputSample;
						nintD += nintFactor; nintC -= nintFactor;
						nintFactor = nintD * nintIntensity;
						break;
					case 5:
						nintFactor = PrevH - inputSample;
						nintE += nintFactor; nintD -= nintFactor;
						nintFactor = nintE * nintIntensity;
						break;
					case 6:
						nintFactor = PrevH - inputSample;
						nintF += nintFactor; nintE -= nintFactor;
						nintFactor = nintF * nintIntensity;
						break;
					case 7:
						nintFactor = PrevH - inputSample;
						nintG += nintFactor; nintF -= nintFactor;
						nintFactor = nintG * nintIntensity;
						break;
					case 8:
						nintFactor = PrevH - inputSample;
						nintH += nintFactor; nintG -= nintFactor;
						nintFactor = nintH * nintIntensity;
						break;
					case 9:
						nintFactor = PrevH - inputSample;
						nintI += nintFactor; nintH -= nintFactor;
						nintFactor = nintI * nintIntensity;
						break;
				}
				nintA /= 2.0; nintB /= 2.0; nintC /= 2.0; nintD /= 2.0; nintE /= 2.0;
				nintF /= 2.0; nintG /= 2.0; nintH /= 2.0; nintI /= 2.0;
				correction = correction + nintFactor;
				countG++;
			}//finished nint section- 3K
			
			PrevH = PrevG; PrevG = PrevF; PrevF = PrevE; PrevE = PrevD;
			PrevD = PrevC; PrevC = PrevB; PrevB = PrevA; PrevA = inputSample;
			
			inputSample = correction * mix;
			
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
		
		inputSample += drySample;
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

