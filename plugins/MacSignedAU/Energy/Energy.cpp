/*
*	File:		Energy.cpp
*	
*	Version:	1.0
* 
*	Created:	10/2/10
*	
*	Copyright:  Copyright © 2010 Airwindows, Airwindows uses the MIT license
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
	Energy.h
	
=============================================================================*/
#include "Energy.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Energy)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy::Energy
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Energy::Energy(AudioUnit component)
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
//	Energy::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Energy::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Energy::GetParameterInfo(AudioUnitScope		inScope,
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
//	Energy::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Energy::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Energy::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Energy::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____EnergyEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy::EnergyKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Energy::EnergyKernel::Reset()
{
	duoEven = 0.0;
	duoOdd = 0.0;
	duoFactor = 0.0;
	flip = false;

	tripletA = 0.0;
	tripletB = 0.0;
	tripletC = 0.0;
	tripletFactor = 0.0;
	countA = 1;

	quadA = 0.0;
	quadB = 0.0;
	quadC = 0.0;
	quadD = 0.0;
	quadFactor = 0.0;
	countB = 1;
	
	quintA = 0.0;
	quintB = 0.0;
	quintC = 0.0;
	quintD = 0.0;
	quintE = 0.0;
	quintFactor = 0.0;
	countC = 1;
	
	sextA = 0.0;
	sextB = 0.0;
	sextC = 0.0;
	sextD = 0.0;
	sextE = 0.0;
	sextF = 0.0;
	sextFactor = 0.0;
	countD = 1;
	
	septA = 0.0;
	septB = 0.0;
	septC = 0.0;
	septD = 0.0;
	septE = 0.0;
	septF = 0.0;
	septG = 0.0;
	septFactor = 0.0;
	countE = 1;
	
	octA = 0.0;
	octB = 0.0;
	octC = 0.0;
	octD = 0.0;
	octE = 0.0;
	octF = 0.0;
	octG = 0.0;
	octH = 0.0;
	octFactor = 0.0;
	countF = 1;
	
	nintA = 0.0;
	nintB = 0.0;
	nintC = 0.0;
	nintD = 0.0;
	nintE = 0.0;
	nintF = 0.0;
	nintG = 0.0;
	nintH = 0.0;
	nintI = 0.0;
	nintFactor = 0.0;
	countG = 1;
	
	PrevH = 0.0;
	PrevG = 0.0;
	PrevF = 0.0;
	PrevE = 0.0;
	PrevD = 0.0;
	PrevC = 0.0;
	PrevB = 0.0;
	PrevA = 0.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Energy::EnergyKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Energy::EnergyKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels,
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
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
		
		double correction = 0.0;
		
		duoFactor = PrevA - inputSample;
		if (flip)
			{
			duoEven += duoFactor;
			duoOdd -= duoFactor;
			duoFactor = duoEven * duoIntensity;
			}
		else
			{
			duoOdd += duoFactor;
			duoEven -= duoFactor;
			duoFactor = duoOdd * duoIntensity;
			}
		duoOdd = (duoOdd - ((duoOdd - duoEven)/256.0)) / 2.0;
		duoEven = (duoEven - ((duoEven - duoOdd)/256.0)) / 2.0;
		correction = correction + duoFactor;
		flip = !flip;
		//finished duo section
		
		
		if (countA < 1 || countA > 3) countA = 1;
		switch (countA)
		{
			case 1:
				tripletFactor = PrevB - inputSample;
				tripletA += tripletFactor;
				tripletC -= tripletFactor;
				tripletFactor = tripletA * tripletIntensity;
			break;
			case 2:
				tripletFactor = PrevB - inputSample;
				tripletB += tripletFactor;
				tripletA -= tripletFactor;
				tripletFactor = tripletB * tripletIntensity;
			break;
			case 3:
				tripletFactor = PrevB - inputSample;
				tripletC += tripletFactor;
				tripletB -= tripletFactor;
				tripletFactor = tripletC * tripletIntensity;
			break;
		}
		tripletA /= 2.0;
		tripletB /= 2.0;
		tripletC /= 2.0;
		correction = correction + tripletFactor;
		countA++;
		//finished triplet section- 15K
		
		if (countB < 1 || countB > 4) countB = 1;
		switch (countB)
		{
			case 1:
				quadFactor = PrevC - inputSample;
				quadA += quadFactor;
				quadD -= quadFactor;
				quadFactor = quadA * quadIntensity;
			break;
			case 2:
				quadFactor = PrevC - inputSample;
				quadB += quadFactor;
				quadA -= quadFactor;
				quadFactor = quadB * quadIntensity;
			break;
			case 3:
				quadFactor = PrevC - inputSample;
				quadC += quadFactor;
				quadB -= quadFactor;
				quadFactor = quadC * quadIntensity;
			break;
			case 4:
				quadFactor = PrevC - inputSample;
				quadD += quadFactor;
				quadC -= quadFactor;
				quadFactor = quadD * quadIntensity;
			break;
		}
		quadA /= 2.0;
		quadB /= 2.0;
		quadC /= 2.0;
		quadD /= 2.0;
		correction = correction + quadFactor;
		countB++;
		//finished quad section- 10K
		
		if (countC < 1 || countC > 5) countC = 1;
		switch (countC)
		{
			case 1:
				quintFactor = PrevD - inputSample;
				quintA += quintFactor;
				quintE -= quintFactor;
				quintFactor = quintA * quintIntensity;
			break;
			case 2:
				quintFactor = PrevD - inputSample;
				quintB += quintFactor;
				quintA -= quintFactor;
				quintFactor = quintB * quintIntensity;
			break;
			case 3:
				quintFactor = PrevD - inputSample;
				quintC += quintFactor;
				quintB -= quintFactor;
				quintFactor = quintC * quintIntensity;
			break;
			case 4:
				quintFactor = PrevD - inputSample;
				quintD += quintFactor;
				quintC -= quintFactor;
				quintFactor = quintD * quintIntensity;
			break;
			case 5:
				quintFactor = PrevD - inputSample;
				quintE += quintFactor;
				quintD -= quintFactor;
				quintFactor = quintE * quintIntensity;
			break;
		}
		quintA /= 2.0;
		quintB /= 2.0;
		quintC /= 2.0;
		quintD /= 2.0;
		quintE /= 2.0;
		correction = correction + quintFactor;
		countC++;
		//finished quint section- 8K
		
		if (countD < 1 || countD > 6) countD = 1;
		switch (countD)
		{
			case 1:
				sextFactor = PrevE - inputSample;
				sextA += sextFactor;
				sextF -= sextFactor;
				sextFactor = sextA * sextIntensity;
			break;
			case 2:
				sextFactor = PrevE - inputSample;
				sextB += sextFactor;
				sextA -= sextFactor;
				sextFactor = sextB * sextIntensity;
			break;
			case 3:
				sextFactor = PrevE - inputSample;
				sextC += sextFactor;
				sextB -= sextFactor;
				sextFactor = sextC * sextIntensity;
			break;
			case 4:
				sextFactor = PrevE - inputSample;
				sextD += sextFactor;
				sextC -= sextFactor;
				sextFactor = sextD * sextIntensity;
			break;
			case 5:
				sextFactor = PrevE - inputSample;
				sextE += sextFactor;
				sextD -= sextFactor;
				sextFactor = sextE * sextIntensity;
			break;
			case 6:
				sextFactor = PrevE - inputSample;
				sextF += sextFactor;
				sextE -= sextFactor;
				sextFactor = sextF * sextIntensity;
			break;
		}
		sextA /= 2.0;
		sextB /= 2.0;
		sextC /= 2.0;
		sextD /= 2.0;
		sextE /= 2.0;
		sextF /= 2.0;
		correction = correction + sextFactor;
		countD++;
		//finished sext section- 6K
		
		if (countE < 1 || countE > 7) countE = 1;
		switch (countE)
		{
			case 1:
				septFactor = PrevF - inputSample;
				septA += septFactor;
				septG -= septFactor;
				septFactor = septA * septIntensity;
			break;
			case 2:
				septFactor = PrevF - inputSample;
				septB += septFactor;
				septA -= septFactor;
				septFactor = septB * septIntensity;
			break;
			case 3:
				septFactor = PrevF - inputSample;
				septC += septFactor;
				septB -= septFactor;
				septFactor = septC * septIntensity;
			break;
			case 4:
				septFactor = PrevF - inputSample;
				septD += septFactor;
				septC -= septFactor;
				septFactor = septD * septIntensity;
			break;
			case 5:
				septFactor = PrevF - inputSample;
				septE += septFactor;
				septD -= septFactor;
				septFactor = septE * septIntensity;
			break;
			case 6:
				septFactor = PrevF - inputSample;
				septF += septFactor;
				septE -= septFactor;
				septFactor = septF * septIntensity;
			break;
			case 7:
				septFactor = PrevF - inputSample;
				septG += septFactor;
				septF -= septFactor;
				septFactor = septG * septIntensity;
			break;
		}
		septA /= 2.0;
		septB /= 2.0;
		septC /= 2.0;
		septD /= 2.0;
		septE /= 2.0;
		septF /= 2.0;
		septG /= 2.0;
		correction = correction + septFactor;
		countE++;
		//finished sept section- 5K
		
		if (countF < 1 || countF > 8) countF = 1;
		switch (countF)
		{
			case 1:
				octFactor = PrevG - inputSample;
				octA += octFactor;
				octH -= octFactor;
				octFactor = octA * octIntensity;
			break;
			case 2:
				octFactor = PrevG - inputSample;
				octB += octFactor;
				octA -= octFactor;
				octFactor = octB * octIntensity;
			break;
			case 3:
				octFactor = PrevG - inputSample;
				octC += octFactor;
				octB -= octFactor;
				octFactor = octC * octIntensity;
			break;
			case 4:
				octFactor = PrevG - inputSample;
				octD += octFactor;
				octC -= octFactor;
				octFactor = octD * octIntensity;
			break;
			case 5:
				octFactor = PrevG - inputSample;
				octE += octFactor;
				octD -= octFactor;
				octFactor = octE * octIntensity;
			break;
			case 6:
				octFactor = PrevG - inputSample;
				octF += octFactor;
				octE -= octFactor;
				octFactor = octF * octIntensity;
			break;
			case 7:
				octFactor = PrevG - inputSample;
				octG += octFactor;
				octF -= octFactor;
				octFactor = octG * octIntensity;
			break;
			case 8:
				octFactor = PrevG - inputSample;
				octH += octFactor;
				octG -= octFactor;
				octFactor = octH * octIntensity;
			break;
		}
		octA /= 2.0;
		octB /= 2.0;
		octC /= 2.0;
		octD /= 2.0;
		octE /= 2.0;
		octF /= 2.0;
		octG /= 2.0;
		octH /= 2.0;
		correction = correction + octFactor;
		countF++;
		//finished oct section- 4K
		
		if (countG < 1 || countG > 9) countG = 1;
		switch (countG)
		{
			case 1:
				nintFactor = PrevH - inputSample;
				nintA += nintFactor;
				nintI -= nintFactor;
				nintFactor = nintA * nintIntensity;
			break;
			case 2:
				nintFactor = PrevH - inputSample;
				nintB += nintFactor;
				nintA -= nintFactor;
				nintFactor = nintB * nintIntensity;
			break;
			case 3:
				nintFactor = PrevH - inputSample;
				nintC += nintFactor;
				nintB -= nintFactor;
				nintFactor = nintC * nintIntensity;
			break;
			case 4:
				nintFactor = PrevH - inputSample;
				nintD += nintFactor;
				nintC -= nintFactor;
				nintFactor = nintD * nintIntensity;
			break;
			case 5:
				nintFactor = PrevH - inputSample;
				nintE += nintFactor;
				nintD -= nintFactor;
				nintFactor = nintE * nintIntensity;
			break;
			case 6:
				nintFactor = PrevH - inputSample;
				nintF += nintFactor;
				nintE -= nintFactor;
				nintFactor = nintF * nintIntensity;
			break;
			case 7:
				nintFactor = PrevH - inputSample;
				nintG += nintFactor;
				nintF -= nintFactor;
				nintFactor = nintG * nintIntensity;
			break;
			case 8:
				nintFactor = PrevH - inputSample;
				nintH += nintFactor;
				nintG -= nintFactor;
				nintFactor = nintH * nintIntensity;
			break;
			case 9:
				nintFactor = PrevH - inputSample;
				nintI += nintFactor;
				nintH -= nintFactor;
				nintFactor = nintI * nintIntensity;
			break;
		}
		nintA /= 2.0;
		nintB /= 2.0;
		nintC /= 2.0;
		nintD /= 2.0;
		nintE /= 2.0;
		nintF /= 2.0;
		nintG /= 2.0;
		nintH /= 2.0;
		nintI /= 2.0;
		correction = correction + nintFactor;
		countG++;
		//finished nint section- 3K
				
		PrevH = PrevG;
		PrevG = PrevF;
		PrevF = PrevE;
		PrevE = PrevD;
		PrevD = PrevC;
		PrevC = PrevB;
		PrevB = PrevA;
		PrevA = inputSample;
		
		inputSample += (correction * mix);
		//we don't need a drySample because we never touched inputSample
		//so, this provides the inv/dry/wet control all by itself
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}