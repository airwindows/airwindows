/*
*	File:		ConsoleHPre.cpp
*	
*	Version:	1.0
* 
*	Created:	11/4/25
*	
*	Copyright:  Copyright © 2025 Airwindows, Airwindows uses the MIT license
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
	ConsoleHPre.cpp
	
=============================================================================*/
#include "ConsoleHPre.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, ConsoleHPre)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHPre::ConsoleHPre
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ConsoleHPre::ConsoleHPre(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_TRM, kDefaultValue_ParamTRM );
	SetParameter(kParam_MOR, kDefaultValue_ParamMOR );
	
	SetParameter(kParam_HIG, kDefaultValue_ParamHIG );
	SetParameter(kParam_MID, kDefaultValue_ParamMID );
	SetParameter(kParam_LOW, kDefaultValue_ParamLOW );
	SetParameter(kParam_CRS, kDefaultValue_ParamCRS );
	
	SetParameter(kParam_TRF, kDefaultValue_ParamTRF );
	SetParameter(kParam_TRG, kDefaultValue_ParamTRG );
	SetParameter(kParam_TRB, kDefaultValue_ParamTRB );
	SetParameter(kParam_HMF, kDefaultValue_ParamHMF );
	SetParameter(kParam_HMG, kDefaultValue_ParamHMG );
	SetParameter(kParam_HMB, kDefaultValue_ParamHMB );
	SetParameter(kParam_LMF, kDefaultValue_ParamLMF );
	SetParameter(kParam_LMG, kDefaultValue_ParamLMG );
	SetParameter(kParam_LMB, kDefaultValue_ParamLMB );
	SetParameter(kParam_BSF, kDefaultValue_ParamBSF );
	SetParameter(kParam_BSG, kDefaultValue_ParamBSG );
	SetParameter(kParam_BSB, kDefaultValue_ParamBSB );
	
	SetParameter(kParam_THR, kDefaultValue_ParamTHR );
	SetParameter(kParam_ATK, kDefaultValue_ParamATK );
	SetParameter(kParam_RLS, kDefaultValue_ParamRLS );
	SetParameter(kParam_GAT, kDefaultValue_ParamGAT );
	
	SetParameter(kParam_LOP, kDefaultValue_ParamLOP );
	SetParameter(kParam_HIP, kDefaultValue_ParamHIP );
	SetParameter(kParam_FAD, kDefaultValue_ParamFAD );
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHPre::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleHPre::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHPre::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleHPre::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_TRM:
                AUBase::FillInParameterName (outParameterInfo, kParameterTRMName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 4;
                outParameterInfo.defaultValue = kDefaultValue_ParamTRM;
                break;
            case kParam_MOR:
                AUBase::FillInParameterName (outParameterInfo, kParameterMORName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamMOR;
                break;
				
            case kParam_HIG:
                AUBase::FillInParameterName (outParameterInfo, kParameterHIGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterHIGUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHIG;
                break;
            case kParam_MID:
                AUBase::FillInParameterName (outParameterInfo, kParameterMIDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamMID;
                break;
            case kParam_LOW:
                AUBase::FillInParameterName (outParameterInfo, kParameterLOWName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLOW;
                break;
            case kParam_CRS:
                AUBase::FillInParameterName (outParameterInfo, kParameterCRSName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamCRS;
                break;
				
            case kParam_TRF:
                AUBase::FillInParameterName (outParameterInfo, kParameterTRFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterTRFUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTRF;
                break;
            case kParam_TRG:
                AUBase::FillInParameterName (outParameterInfo, kParameterTRGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTRG;
                break;
            case kParam_TRB:
                AUBase::FillInParameterName (outParameterInfo, kParameterTRBName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTRB;
                break;
            case kParam_HMF:
                AUBase::FillInParameterName (outParameterInfo, kParameterHMFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHMF;
                break;
            case kParam_HMG:
                AUBase::FillInParameterName (outParameterInfo, kParameterHMGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHMG;
                break;
            case kParam_HMB:
                AUBase::FillInParameterName (outParameterInfo, kParameterHMBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHMB;
                break;
            case kParam_LMF:
                AUBase::FillInParameterName (outParameterInfo, kParameterLMFName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
				outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLMF;
                break;
            case kParam_LMG:
                AUBase::FillInParameterName (outParameterInfo, kParameterLMGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLMG;
                break;
            case kParam_LMB:
                AUBase::FillInParameterName (outParameterInfo, kParameterLMBName, false);
  				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLMB;
                break;
            case kParam_BSF:
                AUBase::FillInParameterName (outParameterInfo, kParameterBSFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamBSF;
                break;
            case kParam_BSG:
                AUBase::FillInParameterName (outParameterInfo, kParameterBSGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamBSG;
                break;
            case kParam_BSB:
                AUBase::FillInParameterName (outParameterInfo, kParameterBSBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamBSB;
                break;
				
            case kParam_THR:
                AUBase::FillInParameterName (outParameterInfo, kParameterTHRName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterTHRUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTHR;
                break;
            case kParam_ATK:
                AUBase::FillInParameterName (outParameterInfo, kParameterATKName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamATK;
                break;
            case kParam_RLS:
                AUBase::FillInParameterName (outParameterInfo, kParameterRLSName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamRLS;
                break;
            case kParam_GAT:
                AUBase::FillInParameterName (outParameterInfo, kParameterGATName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamGAT;
                break;
				
            case kParam_LOP:
                AUBase::FillInParameterName (outParameterInfo, kParameterLOPName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterLOPUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLOP;
                break;
            case kParam_HIP:
                AUBase::FillInParameterName (outParameterInfo, kParameterHIPName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHIP;
                break;
            case kParam_FAD:
                AUBase::FillInParameterName (outParameterInfo, kParameterFADName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFAD;
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
//	ConsoleHPre::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleHPre::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHPre::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleHPre::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ConsoleHPre::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ConsoleHPre::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ConsoleHPreEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHPre::ConsoleHPreKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ConsoleHPre::ConsoleHPreKernel::Reset()
{
	for (int x = 0; x < biq_total; x++) {
		highFast[x] = 0.0;
		lowFast[x] = 0.0;
	}
	highFastLIIR = 0.0;
	lowFastLIIR = 0.0;
	//SmoothEQ3
	
	for (int x = 0; x < biqs_total; x++) {
		high[x] = 0.0;
		hmid[x] = 0.0;
		lmid[x] = 0.0;
		bass[x] = 0.0;
	}
	//HipCrush with four bands
	
	for (int x = 0; x < bez_total; x++) bezComp[x] = 0.0;
	bezComp[bez_cycle] = 1.0; bezMax = 0.0; bezMin = 0.0;
	bezGate = 2.0;
	//Dynamics3
	
	for(int count = 0; count < 22; count++) {
		iirHPosition[count] = 0.0;
		iirHAngle[count] = 0.0;
	}
	hBypass = false;
	
	for(int count = 0; count < 14; count++) {
		iirLPosition[count] = 0.0;
		iirLAngle[count] = 0.0;
	}
	lBypass = false;
	//Cabs2
	
	for(int count = 0; count < dscBuf+2; count++) {
		dBaL[count] = 0.0;
	}
	dBaPosL = 0.0;
	dBaXL = 1;
	//Discontapeity
	
	for (int x = 0; x < 33; x++) {avg32L[x] = 0.0; post32L[x] = 0.0;}
	for (int x = 0; x < 17; x++) {avg16L[x] = 0.0; post16L[x] = 0.0;}
	for (int x = 0; x < 9; x++) {avg8L[x] = 0.0; post8L[x] = 0.0;}
	for (int x = 0; x < 5; x++) {avg4L[x] = 0.0; post4L[x] = 0.0;}
	for (int x = 0; x < 3; x++) {avg2L[x] = 0.0; post2L[x] = 0.0;}
	avgPos = 0;
	lastDarkL = 0.0;
	//preTapeHack
	
	lFreqA = 1.0; lFreqB = 1.0;
	hFreqA = 0.0; hFreqB = 0.0;
	inTrimA = 0.5; inTrimB = 0.5;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleHPre::ConsoleHPreKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ConsoleHPre::ConsoleHPreKernel::Process(	const Float32 	*inSourceP,
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
	int spacing = floor(overallscale*2.0);
	if (spacing < 2) spacing = 2; if (spacing > 32) spacing = 32;
	
	double moreTapeHack = (GetParameter( kParam_MOR )*2.0)+1.0;
	bool tapehackOff = (GetParameter( kParam_MOR ) == 0.0);
	switch ((int)GetParameter( kParam_TRM )){
		case 0: moreTapeHack *= 0.5; break;
		case 1: break;
		case 2: moreTapeHack *= 2.0; break;
		case 3: moreTapeHack *= 4.0; break;
		case 4: moreTapeHack *= 8.0; break;
	}
	double moreDiscontinuity = fmax(pow(GetParameter( kParam_MOR )*0.42,3.0)*overallscale,0.00001);
	//Discontapeity	
	
	double trebleGain = (GetParameter( kParam_HIG )-0.5)*2.0;
	trebleGain = 1.0+(trebleGain*fabs(trebleGain)*fabs(trebleGain));
	double midGain = (GetParameter( kParam_MID )-0.5)*2.0;
	midGain = 1.0+(midGain*fabs(midGain)*fabs(midGain));
	double bassGain = (GetParameter( kParam_LOW )-0.5)*2.0;
	bassGain = 1.0+(bassGain*fabs(bassGain)*fabs(bassGain));
	//separate from filtering stage, this is amplitude, centered on 1.0 unity gain
	double highCoef = 0.0;
	double lowCoef = 0.0;
	double omega = 0.0;
	double biqK = 0.0;
	double norm = 0.0;
	
	bool eqOff = (trebleGain == 1.0 && midGain == 1.0 && bassGain == 1.0);
	//we get to completely bypass EQ if we're truly not using it. The mechanics of it mean that
	//it cancels out to bit-identical anyhow, but we get to skip the calculation
	if (!eqOff) {
		//SmoothEQ3 is how to get 3rd order steepness at very low CPU.
		//because sample rate varies, you could also vary the crossovers
		//you can't vary Q because math is simplified to take advantage of
		//how the accurate Q value for this filter is always exactly 1.0.
		highFast[biq_freq] = (4000.0/GetSampleRate());
		omega = 2.0*M_PI*(4000.0/GetSampleRate()); //mid-high crossover freq
		biqK = 2.0 - cos(omega);
		highCoef = -sqrt(biqK*biqK - 1.0) + biqK;
		lowFast[biq_freq] = (200.0/GetSampleRate());
		omega = 2.0*M_PI*(200.0/GetSampleRate()); //low-mid crossover freq
		biqK = 2.0 - cos(omega);
		lowCoef = -sqrt(biqK*biqK - 1.0) + biqK;
		//exponential IIR filter as part of an accurate 3rd order Butterworth filter 
		biqK = tan(M_PI * highFast[biq_freq]);
		norm = 1.0 / (1.0 + biqK + biqK*biqK);
		highFast[biq_a0] = biqK * biqK * norm;
		highFast[biq_a1] = 2.0 * highFast[biq_a0];
		highFast[biq_a2] = highFast[biq_a0];
		highFast[biq_b1] = 2.0 * (biqK*biqK - 1.0) * norm;
		highFast[biq_b2] = (1.0 - biqK + biqK*biqK) * norm;
		biqK = tan(M_PI * lowFast[biq_freq]);
		norm = 1.0 / (1.0 + biqK + biqK*biqK);
		lowFast[biq_a0] = biqK * biqK * norm;
		lowFast[biq_a1] = 2.0 * lowFast[biq_a0];
		lowFast[biq_a2] = lowFast[biq_a0];
		lowFast[biq_b1] = 2.0 * (biqK*biqK - 1.0) * norm;
		lowFast[biq_b2] = (1.0 - biqK + biqK*biqK) * norm;
		//custom biquad setup with Q = 1.0 gets to omit some divides	
	}
	//SmoothEQ3
	
	double crossFade = GetParameter( kParam_CRS );
	bool hipcrushOff = (crossFade == 0.0);
	if (!hipcrushOff) {
		high[biqs_freq] = (((pow(GetParameter( kParam_TRF ),2.0)*16000.0)+1000.0)/GetSampleRate());
		if (high[biqs_freq] < 0.0001) high[biqs_freq] = 0.0001;
		high[biqs_bit] = (GetParameter( kParam_TRB )*2.0)-1.0;
		high[biqs_level] = (1.0-pow(1.0-GetParameter( kParam_TRG ),2.0))*1.618033988749894848204586;
		high[biqs_reso] = pow(GetParameter( kParam_TRG )+0.618033988749894848204586,2.0);
		biqK = tan(M_PI * high[biqs_freq]);
		norm = 1.0 / (1.0 + biqK / (high[biqs_reso]*0.618033988749894848204586) + biqK * biqK);
		high[biqs_a0] = biqK / (high[biqs_reso]*0.618033988749894848204586) * norm;
		high[biqs_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
		high[biqs_b2] = (1.0 - biqK / (high[biqs_reso]*0.618033988749894848204586) + biqK * biqK) * norm;
		norm = 1.0 / (1.0 + biqK / (high[biqs_reso]*1.618033988749894848204586) + biqK * biqK);
		high[biqs_c0] = biqK / (high[biqs_reso]*1.618033988749894848204586) * norm;
		high[biqs_d1] = 2.0 * (biqK * biqK - 1.0) * norm;
		high[biqs_d2] = (1.0 - biqK / (high[biqs_reso]*1.618033988749894848204586) + biqK * biqK) * norm;
		//high
		
		hmid[biqs_freq] = (((pow(GetParameter( kParam_HMF ),3.0)*7000.0)+300.0)/GetSampleRate());
		if (hmid[biqs_freq] < 0.0001) hmid[biqs_freq] = 0.0001;
		hmid[biqs_bit] = (GetParameter( kParam_HMB )*2.0)-1.0;
		hmid[biqs_level] = (1.0-pow(1.0-GetParameter( kParam_HMG ),2.0))*1.618033988749894848204586;
		hmid[biqs_reso] = pow(GetParameter( kParam_HMG )+0.618033988749894848204586,2.0);
		biqK = tan(M_PI * hmid[biqs_freq]);
		norm = 1.0 / (1.0 + biqK / (hmid[biqs_reso]*0.618033988749894848204586) + biqK * biqK);
		hmid[biqs_a0] = biqK / (hmid[biqs_reso]*0.618033988749894848204586) * norm;
		hmid[biqs_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
		hmid[biqs_b2] = (1.0 - biqK / (hmid[biqs_reso]*0.618033988749894848204586) + biqK * biqK) * norm;
		norm = 1.0 / (1.0 + biqK / (hmid[biqs_reso]*1.618033988749894848204586) + biqK * biqK);
		hmid[biqs_c0] = biqK / (hmid[biqs_reso]*1.618033988749894848204586) * norm;
		hmid[biqs_d1] = 2.0 * (biqK * biqK - 1.0) * norm;
		hmid[biqs_d2] = (1.0 - biqK / (hmid[biqs_reso]*1.618033988749894848204586) + biqK * biqK) * norm;
		//hmid
		
		lmid[biqs_freq] = (((pow(GetParameter( kParam_LMF ),3.0)*3000.0)+40.0)/GetSampleRate());
		if (lmid[biqs_freq] < 0.00001) lmid[biqs_freq] = 0.00001;
		lmid[biqs_bit] = (GetParameter( kParam_LMB )*2.0)-1.0;
		lmid[biqs_level] = (1.0-pow(1.0-GetParameter( kParam_LMG ),2.0))*1.618033988749894848204586;
		lmid[biqs_reso] = pow(GetParameter( kParam_LMG )+0.618033988749894848204586,2.0);
		biqK = tan(M_PI * lmid[biqs_freq]);
		norm = 1.0 / (1.0 + biqK / (lmid[biqs_reso]*0.618033988749894848204586) + biqK * biqK);
		lmid[biqs_a0] = biqK / (lmid[biqs_reso]*0.618033988749894848204586) * norm;
		lmid[biqs_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
		lmid[biqs_b2] = (1.0 - biqK / (lmid[biqs_reso]*0.618033988749894848204586) + biqK * biqK) * norm;
		norm = 1.0 / (1.0 + biqK / (lmid[biqs_reso]*1.618033988749894848204586) + biqK * biqK);
		lmid[biqs_c0] = biqK / (lmid[biqs_reso]*1.618033988749894848204586) * norm;
		lmid[biqs_d1] = 2.0 * (biqK * biqK - 1.0) * norm;
		lmid[biqs_d2] = (1.0 - biqK / (lmid[biqs_reso]*1.618033988749894848204586) + biqK * biqK) * norm;
		//lmid
		
		bass[biqs_freq] = (((pow(GetParameter( kParam_BSF ),4.0)*1000.0)+20.0)/GetSampleRate());
		if (bass[biqs_freq] < 0.00001) bass[biqs_freq] = 0.00001;
		bass[biqs_bit] = (GetParameter( kParam_BSB )*2.0)-1.0;
		bass[biqs_level] = (1.0-pow(1.0-GetParameter( kParam_BSG ),2.0))*1.618033988749894848204586;
		bass[biqs_reso] = pow(GetParameter( kParam_BSG )+0.618033988749894848204586,2.0);
		biqK = tan(M_PI * bass[biqs_freq]);
		norm = 1.0 / (1.0 + biqK / (bass[biqs_reso]*0.618033988749894848204586) + biqK * biqK);
		bass[biqs_a0] = biqK / (bass[biqs_reso]*0.618033988749894848204586) * norm;
		bass[biqs_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
		bass[biqs_b2] = (1.0 - biqK / (bass[biqs_reso]*0.618033988749894848204586) + biqK * biqK) * norm;
		norm = 1.0 / (1.0 + biqK / (bass[biqs_reso]*1.618033988749894848204586) + biqK * biqK);
		bass[biqs_c0] = biqK / (bass[biqs_reso]*1.618033988749894848204586) * norm;
		bass[biqs_d1] = 2.0 * (biqK * biqK - 1.0) * norm;
		bass[biqs_d2] = (1.0 - biqK / (bass[biqs_reso]*1.618033988749894848204586) + biqK * biqK) * norm;
		//bass
	}
	//HipCrush with four bands
	
	double bezThresh = pow(1.0-GetParameter( kParam_THR ), 4.0) * 8.0;
	double bezRez = pow(1.0-GetParameter( kParam_ATK ), 4.0) / overallscale; 
	double sloRez = pow(1.0-GetParameter( kParam_RLS ), 4.0) / overallscale;
	double gate = pow(GetParameter( kParam_GAT ),4.0);
	bezRez = fmin(fmax(bezRez,0.0001),1.0);
	sloRez = fmin(fmax(sloRez,0.0001),1.0);
	//Dynamics3
	
	lFreqA = lFreqB; lFreqB = pow(fmax(GetParameter( kParam_LOP ),0.002),overallscale); //the lowpass
	hFreqA = hFreqB; hFreqB = pow(GetParameter( kParam_HIP ),overallscale+2.0); //the highpass
	//Cabs2
	
	inTrimA = inTrimB; inTrimB = GetParameter( kParam_FAD )*2.0;
	//Console
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpd * 1.18e-17;
		
		inputSampleL *= moreTapeHack;
		//trim control gets to work even when MORE is off
		
		if (!tapehackOff) {
			double darkSampleL = inputSampleL;
			if (avgPos > 31) avgPos = 0;
			if (spacing > 31) {
				avg32L[avgPos] = darkSampleL;
				darkSampleL = 0.0;
				for (int x = 0; x < 32; x++) {darkSampleL += avg32L[x];}
				darkSampleL /= 32.0;
			} if (spacing > 15) {
				avg16L[avgPos%16] = darkSampleL;
				darkSampleL = 0.0;
				for (int x = 0; x < 16; x++) {darkSampleL += avg16L[x];}
				darkSampleL /= 16.0;
			} if (spacing > 7) {
				avg8L[avgPos%8] = darkSampleL;
				darkSampleL = 0.0;
				for (int x = 0; x < 8; x++) {darkSampleL += avg8L[x];}
				darkSampleL /= 8.0;
			} if (spacing > 3) {
				avg4L[avgPos%4] = darkSampleL;
				darkSampleL = 0.0;
				for (int x = 0; x < 4; x++) {darkSampleL += avg4L[x];}
				darkSampleL /= 4.0;
			} if (spacing > 1) {
				avg2L[avgPos%2] = darkSampleL;
				darkSampleL = 0.0;
				for (int x = 0; x < 2; x++) {darkSampleL += avg2L[x];}
				darkSampleL /= 2.0; 
			} //only update after the post-distortion filter stage
			
			double avgSlewL = fmin(fabs(lastDarkL-inputSampleL)*0.12*overallscale,1.0);
			avgSlewL = 1.0-(1.0-avgSlewL*1.0-avgSlewL);
			inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
			lastDarkL = darkSampleL;
			
			//begin Discontinuity section
			inputSampleL *= moreTapeHack;
			inputSampleL *= moreDiscontinuity;
			dBaL[dBaXL] = inputSampleL; dBaPosL *= 0.5; dBaPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
			dBaPosL = fmin(dBaPosL,1.0);
			int dBdly = floor(dBaPosL*dscBuf);
			double dBi = (dBaPosL*dscBuf)-dBdly;
			inputSampleL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
			dBdly++; inputSampleL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
			dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
			inputSampleL /= moreDiscontinuity;
			//end Discontinuity section, begin TapeHack section
			inputSampleL = fmax(fmin(inputSampleL,2.305929007734908),-2.305929007734908);
			double addtwo = inputSampleL * inputSampleL;
			double empower = inputSampleL * addtwo; // inputSampleL to the third power
			inputSampleL -= (empower / 6.0);
			empower *= addtwo; // to the fifth power
			inputSampleL += (empower / 69.0);
			empower *= addtwo; //seventh
			inputSampleL -= (empower / 2530.08);
			empower *= addtwo; //ninth
			inputSampleL += (empower / 224985.6);
			empower *= addtwo; //eleventh
			inputSampleL -= (empower / 9979200.0f);
			//this is a degenerate form of a Taylor Series to approximate sin()
			//end TapeHack section
			//Discontapeity
			
			darkSampleL = inputSampleL;
			if (avgPos > 31) avgPos = 0;
			if (spacing > 31) {
				post32L[avgPos] = darkSampleL;
				darkSampleL = 0.0;
				for (int x = 0; x < 32; x++) {darkSampleL += post32L[x];}
				darkSampleL /= 32.0;
			} if (spacing > 15) {
				post16L[avgPos%16] = darkSampleL;
				darkSampleL = 0.0;
				for (int x = 0; x < 16; x++) {darkSampleL += post16L[x];}
				darkSampleL /= 16.0;
			} if (spacing > 7) {
				post8L[avgPos%8] = darkSampleL;
				darkSampleL = 0.0;
				for (int x = 0; x < 8; x++) {darkSampleL += post8L[x];}
				darkSampleL /= 8.0;
			} if (spacing > 3) {
				post4L[avgPos%4] = darkSampleL;
				darkSampleL = 0.0;
				for (int x = 0; x < 4; x++) {darkSampleL += post4L[x];}
				darkSampleL /= 4.0;
			} if (spacing > 1) {
				post2L[avgPos%2] = darkSampleL;
				darkSampleL = 0.0;
				for (int x = 0; x < 2; x++) {darkSampleL += post2L[x];}
				darkSampleL /= 2.0; 
			} avgPos++;
			inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
			//use the previously calculated depth of the filter			
		}
		
		double smoothEQL = inputSampleL;
		
		if (!eqOff) {
			double trebleFastL = inputSampleL;		
			double outSample = (trebleFastL * highFast[biq_a0]) + highFast[biq_sL1];
			highFast[biq_sL1] = (trebleFastL * highFast[biq_a1]) - (outSample * highFast[biq_b1]) + highFast[biq_sL2];
			highFast[biq_sL2] = (trebleFastL * highFast[biq_a2]) - (outSample * highFast[biq_b2]);
			double midFastL = outSample; trebleFastL -= midFastL;
			outSample = (midFastL * lowFast[biq_a0]) + lowFast[biq_sL1];
			lowFast[biq_sL1] = (midFastL * lowFast[biq_a1]) - (outSample * lowFast[biq_b1]) + lowFast[biq_sL2];
			lowFast[biq_sL2] = (midFastL * lowFast[biq_a2]) - (outSample * lowFast[biq_b2]);
			double bassFastL = outSample; midFastL -= bassFastL;
			trebleFastL = (bassFastL*bassGain) + (midFastL*midGain) + (trebleFastL*trebleGain);
			//first stage of two crossovers is biquad of exactly 1.0 Q
			highFastLIIR = (highFastLIIR*highCoef) + (trebleFastL*(1.0-highCoef));
			midFastL = highFastLIIR; trebleFastL -= midFastL;
			lowFastLIIR = (lowFastLIIR*lowCoef) + (midFastL*(1.0-lowCoef));
			bassFastL = lowFastLIIR; midFastL -= bassFastL;
			smoothEQL = (bassFastL*bassGain) + (midFastL*midGain) + (trebleFastL*trebleGain);		
			//second stage of two crossovers is the exponential filters
			//this produces a slightly steeper Butterworth filter very cheaply
		}
		//SmoothEQ3
		
		double parametricL = 0.0;
		
		if (!hipcrushOff) {
			//begin Stacked Biquad With Reversed Neutron Flow L
			high[biqs_outL] = inputSampleL * fabs(high[biqs_level]);
			high[biqs_temp] = (high[biqs_outL] * high[biqs_a0]) + high[biqs_aL1];
			high[biqs_aL1] = high[biqs_aL2] - (high[biqs_temp]*high[biqs_b1]);
			high[biqs_aL2] = (high[biqs_outL] * -high[biqs_a0]) - (high[biqs_temp]*high[biqs_b2]);
			high[biqs_outL] = high[biqs_temp];
			if (high[biqs_bit] != 0.0) {
				double bitFactor = high[biqs_bit];
				bool crushGate = (bitFactor < 0.0);
				bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
				high[biqs_outL] *= bitFactor;		
				high[biqs_outL] = floor(high[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
				high[biqs_outL] /= bitFactor;
			}
			high[biqs_temp] = (high[biqs_outL] * high[biqs_c0]) + high[biqs_cL1];
			high[biqs_cL1] = high[biqs_cL2] - (high[biqs_temp]*high[biqs_d1]);
			high[biqs_cL2] = (high[biqs_outL] * -high[biqs_c0]) - (high[biqs_temp]*high[biqs_d2]);
			high[biqs_outL] = high[biqs_temp];
			high[biqs_outL] *= high[biqs_level];
			//end Stacked Biquad With Reversed Neutron Flow L
			
			//begin Stacked Biquad With Reversed Neutron Flow L
			hmid[biqs_outL] = inputSampleL * fabs(hmid[biqs_level]);
			hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_a0]) + hmid[biqs_aL1];
			hmid[biqs_aL1] = hmid[biqs_aL2] - (hmid[biqs_temp]*hmid[biqs_b1]);
			hmid[biqs_aL2] = (hmid[biqs_outL] * -hmid[biqs_a0]) - (hmid[biqs_temp]*hmid[biqs_b2]);
			hmid[biqs_outL] = hmid[biqs_temp];
			if (hmid[biqs_bit] != 0.0) {
				double bitFactor = hmid[biqs_bit];
				bool crushGate = (bitFactor < 0.0);
				bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
				hmid[biqs_outL] *= bitFactor;		
				hmid[biqs_outL] = floor(hmid[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
				hmid[biqs_outL] /= bitFactor;
			}
			hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_c0]) + hmid[biqs_cL1];
			hmid[biqs_cL1] = hmid[biqs_cL2] - (hmid[biqs_temp]*hmid[biqs_d1]);
			hmid[biqs_cL2] = (hmid[biqs_outL] * -hmid[biqs_c0]) - (hmid[biqs_temp]*hmid[biqs_d2]);
			hmid[biqs_outL] = hmid[biqs_temp];
			hmid[biqs_outL] *= hmid[biqs_level];
			//end Stacked Biquad With Reversed Neutron Flow L
			
			//begin Stacked Biquad With Reversed Neutron Flow L
			lmid[biqs_outL] = inputSampleL * fabs(lmid[biqs_level]);
			lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_a0]) + lmid[biqs_aL1];
			lmid[biqs_aL1] = lmid[biqs_aL2] - (lmid[biqs_temp]*lmid[biqs_b1]);
			lmid[biqs_aL2] = (lmid[biqs_outL] * -lmid[biqs_a0]) - (lmid[biqs_temp]*lmid[biqs_b2]);
			lmid[biqs_outL] = lmid[biqs_temp];
			if (lmid[biqs_bit] != 0.0) {
				double bitFactor = lmid[biqs_bit];
				bool crushGate = (bitFactor < 0.0);
				bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
				lmid[biqs_outL] *= bitFactor;		
				lmid[biqs_outL] = floor(lmid[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
				lmid[biqs_outL] /= bitFactor;
			}
			lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_c0]) + lmid[biqs_cL1];
			lmid[biqs_cL1] = lmid[biqs_cL2] - (lmid[biqs_temp]*lmid[biqs_d1]);
			lmid[biqs_cL2] = (lmid[biqs_outL] * -lmid[biqs_c0]) - (lmid[biqs_temp]*lmid[biqs_d2]);
			lmid[biqs_outL] = lmid[biqs_temp];
			lmid[biqs_outL] *= lmid[biqs_level];
			//end Stacked Biquad With Reversed Neutron Flow L
			
			//begin Stacked Biquad With Reversed Neutron Flow L
			bass[biqs_outL] = inputSampleL * fabs(bass[biqs_level]);
			bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_a0]) + bass[biqs_aL1];
			bass[biqs_aL1] = bass[biqs_aL2] - (bass[biqs_temp]*bass[biqs_b1]);
			bass[biqs_aL2] = (bass[biqs_outL] * -bass[biqs_a0]) - (bass[biqs_temp]*bass[biqs_b2]);
			bass[biqs_outL] = bass[biqs_temp];
			if (bass[biqs_bit] != 0.0) {
				double bitFactor = bass[biqs_bit];
				bool crushGate = (bitFactor < 0.0);
				bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
				bass[biqs_outL] *= bitFactor;		
				bass[biqs_outL] = floor(bass[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
				bass[biqs_outL] /= bitFactor;
			}
			bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_c0]) + bass[biqs_cL1];
			bass[biqs_cL1] = bass[biqs_cL2] - (bass[biqs_temp]*bass[biqs_d1]);
			bass[biqs_cL2] = (bass[biqs_outL] * -bass[biqs_c0]) - (bass[biqs_temp]*bass[biqs_d2]);
			bass[biqs_outL] = bass[biqs_temp];
			bass[biqs_outL] *= bass[biqs_level];
			parametricL = high[biqs_outL] + hmid[biqs_outL] + lmid[biqs_outL] + bass[biqs_outL];
			//end Stacked Biquad With Reversed Neutron Flow L
		}
		//end HipCrush as four band
		
		if (fabs(inputSampleL) > gate) bezGate = overallscale/fmin(bezRez,sloRez);
		else bezGate = bezGate = fmax(0.000001, bezGate-fmin(bezRez,sloRez));
		
		if (bezThresh > 0.0) {
			inputSampleL *= (bezThresh+1.0);
			smoothEQL *= (bezThresh+1.0);
			parametricL *= (bezThresh+1.0);
		} //makeup gain		
		
		double ctrl = fabs(inputSampleL);
		bezMax = fmax(bezMax,ctrl);
		bezMin = fmax(bezMin-sloRez,ctrl);
		bezComp[bez_cycle] += bezRez;
		bezComp[bez_Ctrl] += (bezMin * bezRez);
		
		if (bezComp[bez_cycle] > 1.0) {
			if (bezGate < 1.0) bezComp[bez_Ctrl] /= bezGate;
			bezComp[bez_cycle] -= 1.0;
			bezComp[bez_C] = bezComp[bez_B];
			bezComp[bez_B] = bezComp[bez_A];
			bezComp[bez_A] = bezComp[bez_Ctrl];
			bezComp[bez_Ctrl] = 0.0;
			bezMax = 0.0;
		}
		double CB = (bezComp[bez_C]*(1.0-bezComp[bez_cycle]))+(bezComp[bez_B]*bezComp[bez_cycle]);
		double BA = (bezComp[bez_B]*(1.0-bezComp[bez_cycle]))+(bezComp[bez_A]*bezComp[bez_cycle]);
		double CBA = (bezComp[bez_B]+(CB*(1.0-bezComp[bez_cycle]))+(BA*bezComp[bez_cycle]))*0.5;
		//switch over to the EQed or HipCrushed sound and compress
		inputSampleL = (smoothEQL * (1.0-crossFade)) + (parametricL * crossFade);
		
		if (bezThresh > 0.0) {
			inputSampleL *= 1.0-(fmin(CBA*bezThresh,1.0));
		}
		//Dynamics3, but with crossfade over EQ or HipCrush
		
		const double temp = (double)nSampleFrames/inFramesToProcess;
		const double hFreq = (hFreqA*temp)+(hFreqB*(1.0-temp));
		if (hFreq > 0.0) {
			double lowSample = inputSampleL;
			for(int count = 0; count < 21; count++) {
				iirHAngle[count] = (iirHAngle[count]*(1.0-hFreq))+((lowSample-iirHPosition[count])*hFreq);
				lowSample = ((iirHPosition[count]+(iirHAngle[count]*hFreq))*(1.0-hFreq))+(lowSample*hFreq);
				iirHPosition[count] = ((iirHPosition[count]+(iirHAngle[count]*hFreq))*(1.0-hFreq))+(lowSample*hFreq);
				inputSampleL -= (lowSample * (1.0/21.0));
			} //the highpass
			hBypass = false;
		} else {
			if (!hBypass) {
				hBypass = true;
				for(int count = 0; count < 22; count++) {
					iirHPosition[count] = 0.0;
					iirHAngle[count] = 0.0;
				}
			} //blank out highpass if jut switched off
		}
		const double lFreq = (lFreqA*temp)+(lFreqB*(1.0-temp));
		if (lFreq < 1.0) {
			for(int count = 0; count < 13; count++) {
				iirLAngle[count] = (iirLAngle[count]*(1.0-lFreq))+((inputSampleL-iirLPosition[count])*lFreq);
				inputSampleL = ((iirLPosition[count]+(iirLAngle[count]*lFreq))*(1.0-lFreq))+(inputSampleL*lFreq);
				iirLPosition[count] = ((iirLPosition[count]+(iirLAngle[count]*lFreq))*(1.0-lFreq))+(inputSampleL*lFreq);
			} //the lowpass
			lBypass = false;
		} else {
			if (!lBypass) {
				lBypass = true;
				for(int count = 0; count < 14; count++) {
					iirLPosition[count] = 0.0;
					iirLAngle[count] = 0.0;
				}
			} //blank out lowpass if just switched off
		}
		//Cabs2
		
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		if (gain > 1.0) gain *= gain;
		if (gain < 1.0) gain = 1.0-pow(1.0-gain,2);
		
		inputSampleL *= gain;
		//applies smoothed fader gain
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSampleL;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

