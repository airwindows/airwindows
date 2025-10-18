/*
*	File:		ConsoleX2Buss.cpp
*	
*	Version:	1.0
* 
*	Created:	10/7/25
*	
*	Copyright:  Copyright � 2025 Airwindows, Airwindows uses the MIT license
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
	ConsoleX2Buss.cpp
	
=============================================================================*/
#include "ConsoleX2Buss.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, ConsoleX2Buss)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Buss::ConsoleX2Buss
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ConsoleX2Buss::ConsoleX2Buss(AudioUnit component)
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
//	ConsoleX2Buss::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Buss::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Buss::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Buss::GetParameterInfo(AudioUnitScope		inScope,
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
 				outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterAUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
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
 				outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterEUnit;
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
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
 				outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterIUnit;
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
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
 				outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterMUnit;
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
//	ConsoleX2Buss::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Buss::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 ConsoleX2Buss::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	ConsoleX2Buss::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Buss::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ConsoleX2Buss::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ConsoleX2Buss::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ConsoleX2BussEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Buss::ConsoleX2BussKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		ConsoleX2Buss::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
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
	highLIIR = 0.0;
	highRIIR = 0.0;
	midLIIR = 0.0;
	midRIIR = 0.0;
	lowLIIR = 0.0;
	lowRIIR = 0.0;
	//SmoothEQ2
	
	for (int x = 0; x < bez_total; x++) {bezCompF[x] = 0.0;bezCompS[x] = 0.0;}
	bezCompF[bez_cycle] = 1.0; bezMaxF = 0.0;
	bezCompS[bez_cycle] = 1.0; bezGate = 2.0;
	//Dynamics2
	
	for(int count = 0; count < 22; count++) {
		iirHPositionL[count] = 0.0;
		iirHAngleL[count] = 0.0;
		iirHPositionR[count] = 0.0;
		iirHAngleR[count] = 0.0;
	}
	hBypass = false;
	
	for(int count = 0; count < 14; count++) {
		iirLPositionL[count] = 0.0;
		iirLAngleL[count] = 0.0;
		iirLPositionR[count] = 0.0;
		iirLAngleR[count] = 0.0;
	}
	lBypass = false;	
	//Cabs2
	
	for(int count = 0; count < dscBuf+2; count++) {
		dBaL[count] = 0.0;
		dBaR[count] = 0.0;
	}
	dBaPosL = 0.0;
	dBaPosR = 0.0;
	dBaXL = 1;
	dBaXR = 1;
	//Discontapeity
		
	lFreqA = 1.0; lFreqB = 1.0;
	hFreqA = 0.0; hFreqB = 0.0;
	panA = 0.5; panB = 0.5;
	inTrimA = 0.5; inTrimB = 0.5;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Buss::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		ConsoleX2Buss::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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

	double trebleGain = (GetParameter( kParam_A )-0.5)*2.0;
	trebleGain = 1.0+(trebleGain*fabs(trebleGain)*fabs(trebleGain));
	double highmidGain = (GetParameter( kParam_B )-0.5)*2.0;
	highmidGain = 1.0+(highmidGain*fabs(highmidGain)*fabs(highmidGain));
	double lowmidGain = (GetParameter( kParam_C )-0.5)*2.0;
	lowmidGain = 1.0+(lowmidGain*fabs(lowmidGain)*fabs(lowmidGain));
	double bassGain = (GetParameter( kParam_D )-0.5)*2.0;
	bassGain = 1.0+(bassGain*fabs(bassGain)*fabs(bassGain));
	
	double trebleRef = GetParameter( kParam_E )-0.5;
	double highmidRef = GetParameter( kParam_F )-0.5;
	double lowmidRef = GetParameter( kParam_G )-0.5;
	double bassRef = GetParameter( kParam_H )-0.5;
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
	double K = 2.0-cos(omega);
	double highCoef = -sqrt((K*K)-1.0)+K;
	highA[biq_reso] = 2.24697960 * highQ;
	highB[biq_reso] = 0.80193774 * highQ;
	highC[biq_reso] = 0.55495813 * highQ;
	
	midA[biq_freq] = ((pow(midF,3)*20000.0)/GetSampleRate());
	midC[biq_freq] = midB[biq_freq] = midA[biq_freq] = fmax(fmin(midA[biq_freq],0.4999),0.00025);	
	double midFreq = pow(midF,3)*20000.0;
	omega = 2.0*M_PI*(midFreq/GetSampleRate());
	K = 2.0-cos(omega);
	double midCoef = -sqrt((K*K)-1.0)+K;
	midA[biq_reso] = 2.24697960 * midQ;
	midB[biq_reso] = 0.80193774 * midQ;
	midC[biq_reso] = 0.55495813 * midQ;
	
	lowA[biq_freq] = ((pow(bassF,3)*20000.0)/GetSampleRate());
	lowC[biq_freq] = lowB[biq_freq] = lowA[biq_freq] = fmax(fmin(lowA[biq_freq],0.4999),0.00025);
	double lowFreq = pow(bassF,3)*20000.0;
	omega = 2.0*M_PI*(lowFreq/GetSampleRate());
	K = 2.0-cos(omega);
	double lowCoef = -sqrt((K*K)-1.0)+K;
	lowA[biq_reso] = 2.24697960 * lowQ;
	lowB[biq_reso] = 0.80193774 * lowQ;
	lowC[biq_reso] = 0.55495813 * lowQ;
	
	K = tan(M_PI * highA[biq_freq]);
	double norm = 1.0 / (1.0 + K / highA[biq_reso] + K * K);
	highA[biq_a0] = K * K * norm;
	highA[biq_a1] = 2.0 * highA[biq_a0];
	highA[biq_a2] = highA[biq_a0];
	highA[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	highA[biq_b2] = (1.0 - K / highA[biq_reso] + K * K) * norm;
	K = tan(M_PI * highB[biq_freq]);
	norm = 1.0 / (1.0 + K / highB[biq_reso] + K * K);
	highB[biq_a0] = K * K * norm;
	highB[biq_a1] = 2.0 * highB[biq_a0];
	highB[biq_a2] = highB[biq_a0];
	highB[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	highB[biq_b2] = (1.0 - K / highB[biq_reso] + K * K) * norm;
	K = tan(M_PI * highC[biq_freq]);
	norm = 1.0 / (1.0 + K / highC[biq_reso] + K * K);
	highC[biq_a0] = K * K * norm;
	highC[biq_a1] = 2.0 * highC[biq_a0];
	highC[biq_a2] = highC[biq_a0];
	highC[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	highC[biq_b2] = (1.0 - K / highC[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * midA[biq_freq]);
	norm = 1.0 / (1.0 + K / midA[biq_reso] + K * K);
	midA[biq_a0] = K * K * norm;
	midA[biq_a1] = 2.0 * midA[biq_a0];
	midA[biq_a2] = midA[biq_a0];
	midA[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	midA[biq_b2] = (1.0 - K / midA[biq_reso] + K * K) * norm;
	K = tan(M_PI * midB[biq_freq]);
	norm = 1.0 / (1.0 + K / midB[biq_reso] + K * K);
	midB[biq_a0] = K * K * norm;
	midB[biq_a1] = 2.0 * midB[biq_a0];
	midB[biq_a2] = midB[biq_a0];
	midB[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	midB[biq_b2] = (1.0 - K / midB[biq_reso] + K * K) * norm;
	K = tan(M_PI * midC[biq_freq]);
	norm = 1.0 / (1.0 + K / midC[biq_reso] + K * K);
	midC[biq_a0] = K * K * norm;
	midC[biq_a1] = 2.0 * midC[biq_a0];
	midC[biq_a2] = midC[biq_a0];
	midC[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	midC[biq_b2] = (1.0 - K / midC[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * lowA[biq_freq]);
	norm = 1.0 / (1.0 + K / lowA[biq_reso] + K * K);
	lowA[biq_a0] = K * K * norm;
	lowA[biq_a1] = 2.0 * lowA[biq_a0];
	lowA[biq_a2] = lowA[biq_a0];
	lowA[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	lowA[biq_b2] = (1.0 - K / lowA[biq_reso] + K * K) * norm;
	K = tan(M_PI * lowB[biq_freq]);
	norm = 1.0 / (1.0 + K / lowB[biq_reso] + K * K);
	lowB[biq_a0] = K * K * norm;
	lowB[biq_a1] = 2.0 * lowB[biq_a0];
	lowB[biq_a2] = lowB[biq_a0];
	lowB[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	lowB[biq_b2] = (1.0 - K / lowB[biq_reso] + K * K) * norm;
	K = tan(M_PI * lowC[biq_freq]);
	norm = 1.0 / (1.0 + K / lowC[biq_reso] + K * K);
	lowC[biq_a0] = K * K * norm;
	lowC[biq_a1] = 2.0 * lowC[biq_a0];
	lowC[biq_a2] = lowC[biq_a0];
	lowC[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	lowC[biq_b2] = (1.0 - K / lowC[biq_reso] + K * K) * norm;
	//SmoothEQ2
	
	double bezCThresh = pow(1.0-GetParameter( kParam_I ), 6.0) * 8.0;
	double bezRez = pow(1.0-GetParameter( kParam_J ), 8.0) / overallscale; 
	double sloRez = pow(1.0-GetParameter( kParam_K ),12.0) / overallscale;
	sloRez = fmin(fmax(sloRez-(bezRez*0.5),0.00001),1.0);
	bezRez = fmin(fmax(bezRez,0.0001),1.0);
	double gate = pow(pow(GetParameter( kParam_L ),4.0),sqrt(bezCThresh+1.0));
	//Dynamics2
	
	lFreqA = lFreqB; lFreqB = pow(fmax(GetParameter( kParam_M ),0.002),overallscale); //the lowpass
	hFreqA = hFreqB; hFreqB = pow(GetParameter( kParam_N ),overallscale+2.0); //the highpass
	//Cabs2
	
	double moreDiscontinuity = fmax(pow(GetParameter( kParam_O )*0.42,3.0)*overallscale,0.00001);
	double moreTapeHack = (GetParameter( kParam_O )*1.4152481)+1.2;
	//Discontapeity
	
	panA = panB; panB = GetParameter( kParam_P )*1.57079633;
	inTrimA = inTrimB; inTrimB = GetParameter( kParam_Q )*2.0;
	//Console

	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 0.6180339887498949));
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 0.6180339887498949));
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 0.6180339887498949));
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 0.6180339887498949));
		
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
		
		highLIIR = (highLIIR*highCoef) + (trebleL*(1.0-highCoef));
		highmidL = highLIIR; trebleL -= highmidL;
		
		midLIIR = (midLIIR*midCoef) + (highmidL*(1.0-midCoef));
		lowmidL = midLIIR; highmidL -= lowmidL;
		
		lowLIIR = (lowLIIR*lowCoef) + (lowmidL*(1.0-lowCoef));
		bassL = lowLIIR; lowmidL -= bassL;
		
		inputSampleL = (bassL*bassGain) + (lowmidL*lowmidGain) + (highmidL*highmidGain) + (trebleL*trebleGain);		
		//fourth stage of three crossovers is the exponential filters
		
		
		double trebleR = inputSampleR;		
		outSample = (trebleR * highA[biq_a0]) + highA[biq_sR1];
		highA[biq_sR1] = (trebleR * highA[biq_a1]) - (outSample * highA[biq_b1]) + highA[biq_sR2];
		highA[biq_sR2] = (trebleR * highA[biq_a2]) - (outSample * highA[biq_b2]);
		double highmidR = outSample; trebleR -= highmidR;
		
		outSample = (highmidR * midA[biq_a0]) + midA[biq_sR1];
		midA[biq_sR1] = (highmidR * midA[biq_a1]) - (outSample * midA[biq_b1]) + midA[biq_sR2];
		midA[biq_sR2] = (highmidR * midA[biq_a2]) - (outSample * midA[biq_b2]);
		double lowmidR = outSample; highmidR -= lowmidR;
		
		outSample = (lowmidR * lowA[biq_a0]) + lowA[biq_sR1];
		lowA[biq_sR1] = (lowmidR * lowA[biq_a1]) - (outSample * lowA[biq_b1]) + lowA[biq_sR2];
		lowA[biq_sR2] = (lowmidR * lowA[biq_a2]) - (outSample * lowA[biq_b2]);
		double bassR = outSample; lowmidR -= bassR;
		
		trebleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);
		//first stage of three crossovers
		
		outSample = (trebleR * highB[biq_a0]) + highB[biq_sR1];
		highB[biq_sR1] = (trebleR * highB[biq_a1]) - (outSample * highB[biq_b1]) + highB[biq_sR2];
		highB[biq_sR2] = (trebleR * highB[biq_a2]) - (outSample * highB[biq_b2]);
		highmidR = outSample; trebleR -= highmidR;
		
		outSample = (highmidR * midB[biq_a0]) + midB[biq_sR1];
		midB[biq_sR1] = (highmidR * midB[biq_a1]) - (outSample * midB[biq_b1]) + midB[biq_sR2];
		midB[biq_sR2] = (highmidR * midB[biq_a2]) - (outSample * midB[biq_b2]);
		lowmidR = outSample; highmidR -= lowmidR;
		
		outSample = (lowmidR * lowB[biq_a0]) + lowB[biq_sR1];
		lowB[biq_sR1] = (lowmidR * lowB[biq_a1]) - (outSample * lowB[biq_b1]) + lowB[biq_sR2];
		lowB[biq_sR2] = (lowmidR * lowB[biq_a2]) - (outSample * lowB[biq_b2]);
		bassR = outSample; lowmidR -= bassR;
		
		trebleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);
		//second stage of three crossovers
		
		outSample = (trebleR * highC[biq_a0]) + highC[biq_sR1];
		highC[biq_sR1] = (trebleR * highC[biq_a1]) - (outSample * highC[biq_b1]) + highC[biq_sR2];
		highC[biq_sR2] = (trebleR * highC[biq_a2]) - (outSample * highC[biq_b2]);
		highmidR = outSample; trebleR -= highmidR;
		
		outSample = (highmidR * midC[biq_a0]) + midC[biq_sR1];
		midC[biq_sR1] = (highmidR * midC[biq_a1]) - (outSample * midC[biq_b1]) + midC[biq_sR2];
		midC[biq_sR2] = (highmidR * midC[biq_a2]) - (outSample * midC[biq_b2]);
		lowmidR = outSample; highmidR -= lowmidR;
		
		outSample = (lowmidR * lowC[biq_a0]) + lowC[biq_sR1];
		lowC[biq_sR1] = (lowmidR * lowC[biq_a1]) - (outSample * lowC[biq_b1]) + lowC[biq_sR2];
		lowC[biq_sR2] = (lowmidR * lowC[biq_a2]) - (outSample * lowC[biq_b2]);
		bassR = outSample; lowmidR -= bassR;
		
		trebleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);
		//third stage of three crossovers
		
		highRIIR = (highRIIR*highCoef) + (trebleR*(1.0-highCoef));
		highmidR = highRIIR; trebleR -= highmidR;
		
		midRIIR = (midRIIR*midCoef) + (highmidR*(1.0-midCoef));
		lowmidR = midRIIR; highmidR -= lowmidR;
		
		lowRIIR = (lowRIIR*lowCoef) + (lowmidR*(1.0-lowCoef));
		bassR = lowRIIR; lowmidR -= bassR;
		
		inputSampleR = (bassR*bassGain) + (lowmidR*lowmidGain) + (highmidR*highmidGain) + (trebleR*trebleGain);		
		//fourth stage of three crossovers is the exponential filters
		//SmoothEQ2
		
		if (fmax(fabs(inputSampleL),fabs(inputSampleR)) > gate+(sloRez*bezGate)) bezGate = ((bezGate*overallscale*3.0)+3.0)*(0.25/overallscale);
		else bezGate = fmax(0.0, bezGate-(sloRez*sloRez));
		
		if (bezCThresh > 0.0) {
			inputSampleL *= ((bezCThresh*0.5)+1.0);
			inputSampleR *= ((bezCThresh*0.5)+1.0);
		}
		
		bezCompF[bez_cycle] += bezRez;
		bezCompF[bez_SampL] += (fabs(inputSampleL) * bezRez);
		bezCompF[bez_SampR] += (fabs(inputSampleR) * bezRez);
		bezMaxF = fmax(bezMaxF,fmax(fabs(inputSampleL),fabs(inputSampleR)));
		
		if (bezCompF[bez_cycle] > 1.0) {
			bezCompF[bez_cycle] -= 1.0;
			
			if (bezMaxF < gate) bezCompF[bez_SampL] = bezMaxF/gate; //note: SampL is a control voltage,
			if (bezCompF[bez_SampL]<gate) bezCompF[bez_SampL] = 0.0; //not a bipolar audio signal
			bezCompF[bez_CL] = bezCompF[bez_BL];
			bezCompF[bez_BL] = bezCompF[bez_AL];
			bezCompF[bez_AL] = bezCompF[bez_SampL];
			bezCompF[bez_SampL] = 0.0;
			
			if (bezMaxF < gate) bezCompF[bez_SampR] = bezMaxF/gate; //note: SampR is a control voltage,
			if (bezCompF[bez_SampR]<gate) bezCompF[bez_SampR] = 0.0; //not a bipolar audio signal
			bezCompF[bez_CR] = bezCompF[bez_BR];
			bezCompF[bez_BR] = bezCompF[bez_AR];
			bezCompF[bez_AR] = bezCompF[bez_SampR];
			bezCompF[bez_SampR] = 0.0;
			
			bezMaxF = 0.0;
		}
		bezCompS[bez_cycle] += sloRez;
		bezCompS[bez_SampL] += (fabs(inputSampleL) * sloRez); //note: SampL is a control voltage
		bezCompS[bez_SampR] += (fabs(inputSampleR) * sloRez); //note: SampR is a control voltage
		if (bezCompS[bez_cycle] > 1.0) {
			bezCompS[bez_cycle] -= 1.0;
			
			if (bezCompS[bez_SampL]<gate) bezCompS[bez_SampL] = 0.0;
			bezCompS[bez_CL] = bezCompS[bez_BL];
			bezCompS[bez_BL] = bezCompS[bez_AL];
			bezCompS[bez_AL] = bezCompS[bez_SampL];
			bezCompS[bez_SampL] = 0.0;
			
			if (bezCompS[bez_SampR]<gate) bezCompS[bez_SampR] = 0.0;
			bezCompS[bez_CR] = bezCompS[bez_BR];
			bezCompS[bez_BR] = bezCompS[bez_AR];
			bezCompS[bez_AR] = bezCompS[bez_SampR];
			bezCompS[bez_SampR] = 0.0;
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
		
		double CBFR = (bezCompF[bez_CR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BR]*bezCompF[bez_cycle]);
		double BAFR = (bezCompF[bez_BR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AR]*bezCompF[bez_cycle]);
		double CBAFR = (bezCompF[bez_BR]+(CBFR*(1.0-bezCompF[bez_cycle]))+(BAFR*bezCompF[bez_cycle]))*0.5;
		double CBSR = (bezCompS[bez_CR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BR]*bezCompS[bez_cycle]);
		double BASR = (bezCompS[bez_BR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AR]*bezCompS[bez_cycle]);
		double CBASR = (bezCompS[bez_BR]+(CBSR*(1.0-bezCompS[bez_cycle]))+(BASR*bezCompS[bez_cycle]))*0.5;
		CBAMax = fmax(CBASR,CBAFR); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		CBAFade = ((CBASR*-CBAMax)+(CBAFR*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleR *= 1.0-(fmin(((CBASR*(1.0-CBAFade))+(CBAFR*CBAFade))*bezCThresh,1.0));
		
		if (bezGate < 1.0 && gate > 0.0) {inputSampleL *= bezGate; inputSampleR *= bezGate;}
		//Dynamics2
		
		const double temp = (double)nSampleFrames/inFramesToProcess;
		const double hFreq = (hFreqA*temp)+(hFreqB*(1.0-temp));
		if (hFreq > 0.0) {
			double lowSampleL = inputSampleL;
			double lowSampleR = inputSampleR;
			for(int count = 0; count < 21; count++) {
				iirHAngleL[count] = (iirHAngleL[count]*(1.0-hFreq))+((lowSampleL-iirHPositionL[count])*hFreq);
				lowSampleL = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(lowSampleL*hFreq);
				iirHPositionL[count] = ((iirHPositionL[count]+(iirHAngleL[count]*hFreq))*(1.0-hFreq))+(lowSampleL*hFreq);
				inputSampleL -= (lowSampleL * (1.0/21.0));//left
				iirHAngleR[count] = (iirHAngleR[count]*(1.0-hFreq))+((lowSampleR-iirHPositionR[count])*hFreq);
				lowSampleR = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(lowSampleR*hFreq);
				iirHPositionR[count] = ((iirHPositionR[count]+(iirHAngleR[count]*hFreq))*(1.0-hFreq))+(lowSampleR*hFreq);
				inputSampleR -= (lowSampleR * (1.0/21.0));//right
			} //the highpass
			hBypass = false;
		} else {
			if (!hBypass) {
				hBypass = true;
				for(int count = 0; count < 22; count++) {
					iirHPositionL[count] = 0.0;
					iirHAngleL[count] = 0.0;
					iirHPositionR[count] = 0.0;
					iirHAngleR[count] = 0.0;
				}
			} //blank out highpass if jut switched off
		}
		const double lFreq = (lFreqA*temp)+(lFreqB*(1.0-temp));
		if (lFreq < 1.0) {
			for(int count = 0; count < 13; count++) {
				iirLAngleL[count] = (iirLAngleL[count]*(1.0-lFreq))+((inputSampleL-iirLPositionL[count])*lFreq);
				inputSampleL = ((iirLPositionL[count]+(iirLAngleL[count]*lFreq))*(1.0-lFreq))+(inputSampleL*lFreq);
				iirLPositionL[count] = ((iirLPositionL[count]+(iirLAngleL[count]*lFreq))*(1.0-lFreq))+(inputSampleL*lFreq);//left
				iirLAngleR[count] = (iirLAngleR[count]*(1.0-lFreq))+((inputSampleR-iirLPositionR[count])*lFreq);
				inputSampleR = ((iirLPositionR[count]+(iirLAngleR[count]*lFreq))*(1.0-lFreq))+(inputSampleR*lFreq);
				iirLPositionR[count] = ((iirLPositionR[count]+(iirLAngleR[count]*lFreq))*(1.0-lFreq))+(inputSampleR*lFreq);//right
			} //the lowpass
			lBypass = false;
		} else {
			if (!lBypass) {
				lBypass = true;
				for(int count = 0; count < 14; count++) {
					iirLPositionL[count] = 0.0;
					iirLAngleL[count] = 0.0;
					iirLPositionR[count] = 0.0;
					iirLAngleR[count] = 0.0;
				}
			} //blank out lowpass if just switched off
		}
		//Cabs2
		
		
		//begin Discontinuity section
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
		inputSampleL = fmax(fmin(inputSampleL*moreTapeHack,2.305929007734908),-2.305929007734908);
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
		inputSampleL *= 0.9239;
		//end TapeHack section
		
		//begin Discontinuity section
		inputSampleR *= moreDiscontinuity;
		dBaR[dBaXR] = inputSampleR; dBaPosR *= 0.5; dBaPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBaPosR = fmin(dBaPosR,1.0);
		dBdly = floor(dBaPosR*dscBuf);
		dBi = (dBaPosR*dscBuf)-dBdly;
		inputSampleR = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
		inputSampleR /= moreDiscontinuity;
		//end Discontinuity section, begin TapeHack section
		inputSampleR = fmax(fmin(inputSampleR*moreTapeHack,2.305929007734908),-2.305929007734908);
		addtwo = inputSampleR * inputSampleR;
		empower = inputSampleR * addtwo; // inputSampleR to the third power
		inputSampleR -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleR += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleR -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleR += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleR -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		inputSampleR *= 0.9239;
		//end TapeHack section
		//Discontapeity

		double gainR = (panA*temp)+(panB*(1.0-temp));
		double gainL = 1.57079633-gainR;
		gainR = sin(gainR); gainL = sin(gainL);
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		if (gain > 1.0) gain *= gain;
		if (gain < 1.0) gain = 1.0-pow(1.0-gain,2);
		gain *= 2.0;
		
		inputSampleL = inputSampleL * gainL * gain;
		inputSampleR = inputSampleR * gainR * gain;
		//applies pan section, and smoothed fader gain
		
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

