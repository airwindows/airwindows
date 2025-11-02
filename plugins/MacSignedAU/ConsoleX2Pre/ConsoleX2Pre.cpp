/*
*	File:		ConsoleX2Pre.cpp
*	
*	Version:	1.0
* 
*	Created:	10/7/25
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
	ConsoleX2Pre.cpp
	
=============================================================================*/
#include "ConsoleX2Pre.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, ConsoleX2Pre)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Pre::ConsoleX2Pre
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ConsoleX2Pre::ConsoleX2Pre(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
	SetParameter(kParam_D, kDefaultValue_ParamD );
	SetParameter(kParam_E, kDefaultValue_ParamE );
	SetParameter(kParam_F, kDefaultValue_ParamF );
	SetParameter(kParam_G, kDefaultValue_ParamG );
	SetParameter(kParam_H, kDefaultValue_ParamH );
	SetParameter(kParam_I, kDefaultValue_ParamI );
	SetParameter(kParam_J, kDefaultValue_ParamJ );
	SetParameter(kParam_K, kDefaultValue_ParamK );
	SetParameter(kParam_L, kDefaultValue_ParamL );
	SetParameter(kParam_M, kDefaultValue_ParamM );
	SetParameter(kParam_N, kDefaultValue_ParamN );
	SetParameter(kParam_O, kDefaultValue_ParamO );
	SetParameter(kParam_P, kDefaultValue_ParamP );
	SetParameter(kParam_Q, kDefaultValue_ParamQ );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Pre::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Pre::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Pre::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Pre::GetParameterInfo(AudioUnitScope		inScope,
                                                        AudioUnitParameterID	inParameterID,
                                                        AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_A:
                AUBase::FillInParameterName (outParameterInfo, kParameterAName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 4;
                outParameterInfo.defaultValue = kDefaultValue_ParamA;
                break;
            case kParam_B:
                AUBase::FillInParameterName (outParameterInfo, kParameterBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamB;
                break;
            case kParam_C:
                AUBase::FillInParameterName (outParameterInfo, kParameterCName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterCUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamC;
                break;
            case kParam_D:
                AUBase::FillInParameterName (outParameterInfo, kParameterDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamD;
                break;
            case kParam_E:
                AUBase::FillInParameterName (outParameterInfo, kParameterEName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamE;
                break;
            case kParam_F:
                AUBase::FillInParameterName (outParameterInfo, kParameterFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamF;
                break;
            case kParam_G:
                AUBase::FillInParameterName (outParameterInfo, kParameterGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterGUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamG;
                break;
            case kParam_H:
                AUBase::FillInParameterName (outParameterInfo, kParameterHName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamH;
                break;
            case kParam_I:
                AUBase::FillInParameterName (outParameterInfo, kParameterIName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamI;
                break;
            case kParam_J:
                AUBase::FillInParameterName (outParameterInfo, kParameterJName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamJ;
                break;
            case kParam_K:
                AUBase::FillInParameterName (outParameterInfo, kParameterKName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterKUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamK;
                break;
            case kParam_L:
                AUBase::FillInParameterName (outParameterInfo, kParameterLName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamL;
                break;
            case kParam_M:
                AUBase::FillInParameterName (outParameterInfo, kParameterMName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
				outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamM;
                break;
            case kParam_N:
                AUBase::FillInParameterName (outParameterInfo, kParameterNName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamN;
                break;
            case kParam_O:
                AUBase::FillInParameterName (outParameterInfo, kParameterOName, false);
  				outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterOUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamO;
                break;
            case kParam_P:
                AUBase::FillInParameterName (outParameterInfo, kParameterPName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamP;
                break;
            case kParam_Q:
                AUBase::FillInParameterName (outParameterInfo, kParameterQName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamQ;
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
//	ConsoleX2Pre::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Pre::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Pre::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Pre::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ConsoleX2Pre::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ConsoleX2Pre::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ConsoleX2PreEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Pre::ConsoleX2PreKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ConsoleX2Pre::ConsoleX2PreKernel::Reset()
{
	for (int x = 0; x < biq_total; x++) {
		highA[x] = 0.0;
		highB[x] = 0.0;
		highC[x] = 0.0;
		midA[x] = 0.0;
		midB[x] = 0.0;
		midC[x] = 0.0;
		lowA[x] = 0.0;
		lowB[x] = 0.0;
		lowC[x] = 0.0;
	}
	highIIR = 0.0;
	midIIR = 0.0;
	lowIIR = 0.0;
	//SmoothEQ2
	
	for (int x = 0; x < bez_total; x++) {bezCompF[x] = 0.0;bezCompS[x] = 0.0;}
	bezCompF[bez_cycle] = 1.0; bezMaxF = 0.0;
	bezCompS[bez_cycle] = 1.0; bezGate = 2.0;
	//Dynamics2
	
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
	
	for (int x = 0; x < 33; x++) {avg32L[x] = 0.0;}
	for (int x = 0; x < 17; x++) {avg16L[x] = 0.0;}
	for (int x = 0; x < 9; x++) {avg8L[x] = 0.0;}
	for (int x = 0; x < 5; x++) {avg4L[x] = 0.0;}
	for (int x = 0; x < 3; x++) {avg2L[x] = 0.0;}
	avgPos = 0;
	lastSlewL = 0.0;
	lastSlewpleL = 0.0;
	//preTapeHack
	
	lFreqA = 1.0; lFreqB = 1.0;
	hFreqA = 0.0; hFreqB = 0.0;
	inTrimA = 0.5; inTrimB = 0.5;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Pre::ConsoleX2PreKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ConsoleX2Pre::ConsoleX2PreKernel::Process(	const Float32 	*inSourceP,
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
	
	double moreTapeHack = (GetParameter( kParam_B )*2.0)+1.0;
	switch ((int)GetParameter( kParam_A )){
		case 0: moreTapeHack *= 0.5; break;
		case 1: break;
		case 2: moreTapeHack *= 2.0; break;
		case 3: moreTapeHack *= 4.0; break;
		case 4: moreTapeHack *= 8.0; break;
	}
	double moreDiscontinuity = fmax(pow(GetParameter( kParam_B )*0.42,3.0)*overallscale,0.00001);
	//Discontapeity	
	
	double trebleGain = (GetParameter( kParam_C )-0.5)*2.0;
	trebleGain = 1.0+(trebleGain*fabs(trebleGain)*fabs(trebleGain));
	double highmidGain = (GetParameter( kParam_D )-0.5)*2.0;
	highmidGain = 1.0+(highmidGain*fabs(highmidGain)*fabs(highmidGain));
	double lowmidGain = (GetParameter( kParam_E )-0.5)*2.0;
	lowmidGain = 1.0+(lowmidGain*fabs(lowmidGain)*fabs(lowmidGain));
	double bassGain = (GetParameter( kParam_F )-0.5)*2.0;
	bassGain = 1.0+(bassGain*fabs(bassGain)*fabs(bassGain));
	
	double trebleRef = GetParameter( kParam_G )-0.5;
	double highmidRef = GetParameter( kParam_H )-0.5;
	double lowmidRef = GetParameter( kParam_I )-0.5;
	double bassRef = GetParameter( kParam_J )-0.5;
	double highF = 0.75 + ((trebleRef+trebleRef+trebleRef+highmidRef)*0.125);
	double bassF = 0.25 + ((lowmidRef+bassRef+bassRef+bassRef)*0.125);
	double midF = (highF*0.5) + (bassF*0.5) + ((highmidRef+lowmidRef)*0.125);
	
	double highQ = fmax(fmin(1.0+(highmidRef-trebleRef),4.0),0.125);
	double midQ = fmax(fmin(1.0+(lowmidRef-highmidRef),4.0),0.125);
	double lowQ = fmax(fmin(1.0+(bassRef-lowmidRef),4.0),0.125);
	
	highA[biq_freq] = ((pow(highF,3)*20000.0)/GetSampleRate());
	highC[biq_freq] = highB[biq_freq] = highA[biq_freq] = fmax(fmin(highA[biq_freq],0.4999),0.00025);
	double highFreq = pow(highF,3)*20000.0;
	double omega = 2.0*M_PI*(highFreq/GetSampleRate());
	double biqK = 2.0-cos(omega);
	double highCoef = -sqrt((biqK*biqK)-1.0)+biqK;
	highA[biq_reso] = 2.24697960 * highQ;
	highB[biq_reso] = 0.80193774 * highQ;
	highC[biq_reso] = 0.55495813 * highQ;
	
	midA[biq_freq] = ((pow(midF,3)*20000.0)/GetSampleRate());
	midC[biq_freq] = midB[biq_freq] = midA[biq_freq] = fmax(fmin(midA[biq_freq],0.4999),0.00025);	
	double midFreq = pow(midF,3)*20000.0;
	omega = 2.0*M_PI*(midFreq/GetSampleRate());
	biqK = 2.0-cos(omega);
	double midCoef = -sqrt((biqK*biqK)-1.0)+biqK;
	midA[biq_reso] = 2.24697960 * midQ;
	midB[biq_reso] = 0.80193774 * midQ;
	midC[biq_reso] = 0.55495813 * midQ;
	
	lowA[biq_freq] = ((pow(bassF,3)*20000.0)/GetSampleRate());
	lowC[biq_freq] = lowB[biq_freq] = lowA[biq_freq] = fmax(fmin(lowA[biq_freq],0.4999),0.00025);
	double lowFreq = pow(bassF,3)*20000.0;
	omega = 2.0*M_PI*(lowFreq/GetSampleRate());
	biqK = 2.0-cos(omega);
	double lowCoef = -sqrt((biqK*biqK)-1.0)+biqK;
	lowA[biq_reso] = 2.24697960 * lowQ;
	lowB[biq_reso] = 0.80193774 * lowQ;
	lowC[biq_reso] = 0.55495813 * lowQ;
	
	biqK = tan(M_PI * highA[biq_freq]);
	double norm = 1.0 / (1.0 + biqK / highA[biq_reso] + biqK * biqK);
	highA[biq_a0] = biqK * biqK * norm;
	highA[biq_a1] = 2.0 * highA[biq_a0];
	highA[biq_a2] = highA[biq_a0];
	highA[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highA[biq_b2] = (1.0 - biqK / highA[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * highB[biq_freq]);
	norm = 1.0 / (1.0 + biqK / highB[biq_reso] + biqK * biqK);
	highB[biq_a0] = biqK * biqK * norm;
	highB[biq_a1] = 2.0 * highB[biq_a0];
	highB[biq_a2] = highB[biq_a0];
	highB[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highB[biq_b2] = (1.0 - biqK / highB[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * highC[biq_freq]);
	norm = 1.0 / (1.0 + biqK / highC[biq_reso] + biqK * biqK);
	highC[biq_a0] = biqK * biqK * norm;
	highC[biq_a1] = 2.0 * highC[biq_a0];
	highC[biq_a2] = highC[biq_a0];
	highC[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highC[biq_b2] = (1.0 - biqK / highC[biq_reso] + biqK * biqK) * norm;
	
	biqK = tan(M_PI * midA[biq_freq]);
	norm = 1.0 / (1.0 + biqK / midA[biq_reso] + biqK * biqK);
	midA[biq_a0] = biqK * biqK * norm;
	midA[biq_a1] = 2.0 * midA[biq_a0];
	midA[biq_a2] = midA[biq_a0];
	midA[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	midA[biq_b2] = (1.0 - biqK / midA[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * midB[biq_freq]);
	norm = 1.0 / (1.0 + biqK / midB[biq_reso] + biqK * biqK);
	midB[biq_a0] = biqK * biqK * norm;
	midB[biq_a1] = 2.0 * midB[biq_a0];
	midB[biq_a2] = midB[biq_a0];
	midB[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	midB[biq_b2] = (1.0 - biqK / midB[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * midC[biq_freq]);
	norm = 1.0 / (1.0 + biqK / midC[biq_reso] + biqK * biqK);
	midC[biq_a0] = biqK * biqK * norm;
	midC[biq_a1] = 2.0 * midC[biq_a0];
	midC[biq_a2] = midC[biq_a0];
	midC[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	midC[biq_b2] = (1.0 - biqK / midC[biq_reso] + biqK * biqK) * norm;
	
	biqK = tan(M_PI * lowA[biq_freq]);
	norm = 1.0 / (1.0 + biqK / lowA[biq_reso] + biqK * biqK);
	lowA[biq_a0] = biqK * biqK * norm;
	lowA[biq_a1] = 2.0 * lowA[biq_a0];
	lowA[biq_a2] = lowA[biq_a0];
	lowA[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowA[biq_b2] = (1.0 - biqK / lowA[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * lowB[biq_freq]);
	norm = 1.0 / (1.0 + biqK / lowB[biq_reso] + biqK * biqK);
	lowB[biq_a0] = biqK * biqK * norm;
	lowB[biq_a1] = 2.0 * lowB[biq_a0];
	lowB[biq_a2] = lowB[biq_a0];
	lowB[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowB[biq_b2] = (1.0 - biqK / lowB[biq_reso] + biqK * biqK) * norm;
	biqK = tan(M_PI * lowC[biq_freq]);
	norm = 1.0 / (1.0 + biqK / lowC[biq_reso] + biqK * biqK);
	lowC[biq_a0] = biqK * biqK * norm;
	lowC[biq_a1] = 2.0 * lowC[biq_a0];
	lowC[biq_a2] = lowC[biq_a0];
	lowC[biq_b1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowC[biq_b2] = (1.0 - biqK / lowC[biq_reso] + biqK * biqK) * norm;
	//SmoothEQ2
	
	double bezCThresh = pow(1.0-GetParameter( kParam_K ), 6.0) * 8.0;
	double bezRez = pow(1.0-GetParameter( kParam_L ), 8.0) / overallscale; 
	double sloRez = pow(1.0-GetParameter( kParam_M ),12.0) / overallscale;
	sloRez = fmin(fmax(sloRez-(bezRez*0.5),0.00001),1.0);
	bezRez = fmin(fmax(bezRez,0.0001),1.0);
	double gate = pow(pow(GetParameter( kParam_N ),4.0),sqrt(bezCThresh+1.0));
	//Dynamics2
	
	lFreqA = lFreqB; lFreqB = pow(fmax(GetParameter( kParam_O ),0.002),overallscale); //the lowpass
	hFreqA = hFreqB; hFreqB = pow(GetParameter( kParam_P ),overallscale+2.0); //the highpass
	//Cabs2
	
	inTrimA = inTrimB; inTrimB = GetParameter( kParam_Q )*2.0;
	//Console

	while (nSampleFrames-- > 0) {
		double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpd * 1.18e-17;
		
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
		} avgPos++;
		lastSlewL += fabs(lastSlewpleL-inputSampleL); lastSlewpleL = inputSampleL;
		double avgSlewL = fmin(lastSlewL,1.0);
		lastSlewL = fmax(lastSlewL*0.78,2.39996322972865332223); //look up Golden Angle, it's cool
		inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
		
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
		
		double trebleL = inputSampleL;		
		double outSample = (trebleL * highA[biq_a0]) + highA[biq_sL1];
		highA[biq_sL1] = (trebleL * highA[biq_a1]) - (outSample * highA[biq_b1]) + highA[biq_sL2];
		highA[biq_sL2] = (trebleL * highA[biq_a2]) - (outSample * highA[biq_b2]);
		double highmidL = outSample; trebleL -= highmidL;
		
		outSample = (highmidL * midA[biq_a0]) + midA[biq_sL1];
		midA[biq_sL1] = (highmidL * midA[biq_a1]) - (outSample * midA[biq_b1]) + midA[biq_sL2];
		midA[biq_sL2] = (highmidL * midA[biq_a2]) - (outSample * midA[biq_b2]);
		double lowmidL = outSample; highmidL -= lowmidL;
		
		outSample = (lowmidL * lowA[biq_a0]) + lowA[biq_sL1];
		lowA[biq_sL1] = (lowmidL * lowA[biq_a1]) - (outSample * lowA[biq_b1]) + lowA[biq_sL2];
		lowA[biq_sL2] = (lowmidL * lowA[biq_a2]) - (outSample * lowA[biq_b2]);
		double bassL = outSample; lowmidL -= bassL;
		
		trebleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);
		//first stage of three crossovers
		
		outSample = (trebleL * highB[biq_a0]) + highB[biq_sL1];
		highB[biq_sL1] = (trebleL * highB[biq_a1]) - (outSample * highB[biq_b1]) + highB[biq_sL2];
		highB[biq_sL2] = (trebleL * highB[biq_a2]) - (outSample * highB[biq_b2]);
		highmidL = outSample; trebleL -= highmidL;
		
		outSample = (highmidL * midB[biq_a0]) + midB[biq_sL1];
		midB[biq_sL1] = (highmidL * midB[biq_a1]) - (outSample * midB[biq_b1]) + midB[biq_sL2];
		midB[biq_sL2] = (highmidL * midB[biq_a2]) - (outSample * midB[biq_b2]);
		lowmidL = outSample; highmidL -= lowmidL;
		
		outSample = (lowmidL * lowB[biq_a0]) + lowB[biq_sL1];
		lowB[biq_sL1] = (lowmidL * lowB[biq_a1]) - (outSample * lowB[biq_b1]) + lowB[biq_sL2];
		lowB[biq_sL2] = (lowmidL * lowB[biq_a2]) - (outSample * lowB[biq_b2]);
		bassL = outSample; lowmidL -= bassL;
		
		trebleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);
		//second stage of three crossovers
		
		outSample = (trebleL * highC[biq_a0]) + highC[biq_sL1];
		highC[biq_sL1] = (trebleL * highC[biq_a1]) - (outSample * highC[biq_b1]) + highC[biq_sL2];
		highC[biq_sL2] = (trebleL * highC[biq_a2]) - (outSample * highC[biq_b2]);
		highmidL = outSample; trebleL -= highmidL;
		
		outSample = (highmidL * midC[biq_a0]) + midC[biq_sL1];
		midC[biq_sL1] = (highmidL * midC[biq_a1]) - (outSample * midC[biq_b1]) + midC[biq_sL2];
		midC[biq_sL2] = (highmidL * midC[biq_a2]) - (outSample * midC[biq_b2]);
		lowmidL = outSample; highmidL -= lowmidL;
		
		outSample = (lowmidL * lowC[biq_a0]) + lowC[biq_sL1];
		lowC[biq_sL1] = (lowmidL * lowC[biq_a1]) - (outSample * lowC[biq_b1]) + lowC[biq_sL2];
		lowC[biq_sL2] = (lowmidL * lowC[biq_a2]) - (outSample * lowC[biq_b2]);
		bassL = outSample; lowmidL -= bassL;
		
		trebleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);
		//third stage of three crossovers
		
		highIIR = (highIIR*highCoef) + (trebleL*(1.0-highCoef));
		highmidL = highIIR; trebleL -= highmidL;
		
		midIIR = (midIIR*midCoef) + (highmidL*(1.0-midCoef));
		lowmidL = midIIR; highmidL -= lowmidL;
		
		lowIIR = (lowIIR*lowCoef) + (lowmidL*(1.0-lowCoef));
		bassL = lowIIR; lowmidL -= bassL;
		
		inputSampleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);		
		//fourth stage of three crossovers is the exponential filters
		//SmoothEQ2
		
		if (fabs(inputSampleL) > gate+(sloRez*bezGate)) bezGate = ((bezGate*overallscale*3.0)+3.0)*(0.25/overallscale);
		else bezGate = fmax(0.0, bezGate-(sloRez*sloRez));
		
		if (bezCThresh > 0.0) inputSampleL *= ((bezCThresh*0.5)+1.0);
		
		bezCompF[bez_cycle] += bezRez;
		bezCompF[bez_SampL] += (fabs(inputSampleL) * bezRez);
		bezMaxF = fmax(bezMaxF,fabs(inputSampleL));
		if (bezCompF[bez_cycle] > 1.0) {
			if (bezMaxF < gate) bezCompF[bez_SampL] = bezMaxF/gate; //note: SampL is a control voltage,
			if (bezCompF[bez_SampL]<gate) bezCompF[bez_SampL] = 0.0; //not a bipolar audio signal
			bezCompF[bez_cycle] -= 1.0;
			bezCompF[bez_CL] = bezCompF[bez_BL];
			bezCompF[bez_BL] = bezCompF[bez_AL];
			bezCompF[bez_AL] = bezCompF[bez_SampL];
			bezCompF[bez_SampL] = 0.0;
			bezMaxF = 0.0;
		}
		bezCompS[bez_cycle] += sloRez;
		bezCompS[bez_SampL] += (fabs(inputSampleL) * sloRez); //note: SampL is a control voltage.
		if (bezCompS[bez_cycle] > 1.0) {
			if (bezCompS[bez_SampL]<gate) bezCompS[bez_SampL] = 0.0;
			bezCompS[bez_cycle] -= 1.0;
			bezCompS[bez_CL] = bezCompS[bez_BL];
			bezCompS[bez_BL] = bezCompS[bez_AL];
			bezCompS[bez_AL] = bezCompS[bez_SampL];
			bezCompS[bez_SampL] = 0.0;
		}
		double CBFL = (bezCompF[bez_CL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BL]*bezCompF[bez_cycle]);
		double BAFL = (bezCompF[bez_BL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AL]*bezCompF[bez_cycle]);
		double CBAFL = (bezCompF[bez_BL]+(CBFL*(1.0-bezCompF[bez_cycle]))+(BAFL*bezCompF[bez_cycle]))*0.5;
		double CBSL = (bezCompS[bez_CL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BL]*bezCompS[bez_cycle]);
		double BASL = (bezCompS[bez_BL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AL]*bezCompS[bez_cycle]);
		double CBASL = (bezCompS[bez_BL]+(CBSL*(1.0-bezCompS[bez_cycle]))+(BASL*bezCompS[bez_cycle]))*0.5;
		
		double CBAMax = fmax(CBASL,CBAFL); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		double CBAFade = ((CBASL*-CBAMax)+(CBAFL*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleL *= 1.0-(fmin(((CBASL*(1.0-CBAFade))+(CBAFL*CBAFade))*bezCThresh,1.0));
		
		if (bezGate < 1.0 && gate > 0.0) inputSampleL *= bezGate;
		//Dynamics2
		
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

