/*
*	File:		X2Buss.cpp
*	
*	Version:	1.0
* 
*	Created:	10/31/25
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
	X2Buss.cpp
	
=============================================================================*/
#include "X2Buss.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, X2Buss)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	X2Buss::X2Buss
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
X2Buss::X2Buss(AudioUnit component)
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
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	X2Buss::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			X2Buss::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	X2Buss::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			X2Buss::GetParameterInfo(AudioUnitScope		inScope,
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
//	X2Buss::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			X2Buss::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 X2Buss::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	X2Buss::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			X2Buss::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	X2Buss::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult X2Buss::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____X2BussEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	X2Buss::X2BussKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		X2Buss::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
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
	bezCompS[bez_cycle] = 1.0;
	//Dynamics2
	
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	lastSampleR = 0.0;
	wasPosClipR = false;
	wasNegClipR = false;
	for (int x = 0; x < 17; x++) {intermediateL[x] = 0.0; intermediateR[x] = 0.0;}
	for (int x = 0; x < 33; x++) {slewL[x] = 0.0; slewR[x] = 0.0;}
	
	inTrimA = 0.5; inTrimB = 0.5;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	X2Buss::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		X2Buss::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
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
	
	double bezCThresh = pow(1.0-GetParameter( kParam_I ), 6.0) * 8.0;
	double bezRez = pow(1.0-GetParameter( kParam_I ), 12.360679774997898) / overallscale;
	bezRez = fmin(fmax(bezRez,0.00001),1.0);
	int stepped = 999999; if (bezRez > 0.000001) stepped = (int)(1.0/bezRez);
	bezRez = 1.0 / stepped;
	double bezTrim = 1.0-(bezRez*((double)stepped/(stepped+1.0)));
	double sloRez = pow(1.0-GetParameter( kParam_I ),10.0) / overallscale;
	sloRez = fmin(fmax(sloRez,0.00001),1.0);
	stepped = 999999; if (sloRez > 0.000001) stepped = (int)(1.0/sloRez);
	sloRez = 1.0 / stepped;
	double sloTrim = 1.0-(sloRez*((double)stepped/(stepped+1.0)));
	//Dynamics2
	
	inTrimA = inTrimB; inTrimB = GetParameter( kParam_J )*2.0;
	//Console
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
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
			bezCompF[bez_CL] = bezCompF[bez_BL];
			bezCompF[bez_BL] = bezCompF[bez_AL];
			bezCompF[bez_AL] = bezCompF[bez_SampL];
			bezCompF[bez_SampL] = 0.0;
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
			bezCompS[bez_CL] = bezCompS[bez_BL];
			bezCompS[bez_BL] = bezCompS[bez_AL];
			bezCompS[bez_AL] = bezCompS[bez_SampL];
			bezCompS[bez_SampL] = 0.0;
			bezCompS[bez_CR] = bezCompS[bez_BR];
			bezCompS[bez_BR] = bezCompS[bez_AR];
			bezCompS[bez_AR] = bezCompS[bez_SampR];
			bezCompS[bez_SampR] = 0.0;
		}
		double X = bezCompF[bez_cycle]*bezTrim;
		double CBAFL = bezCompF[bez_BL]+(bezCompF[bez_CL]*(1.0-X)*(1.0-X))+(bezCompF[bez_BL]*2.0*(1.0-X)*X)+(bezCompF[bez_AL]*X*X);
		CBAFL *= 0.5;
		X = bezCompS[bez_cycle]*sloTrim;
		double CBASL = bezCompS[bez_BL]+(bezCompS[bez_CL]*(1.0-X)*(1.0-X))+(bezCompS[bez_BL]*2.0*(1.0-X)*X)+(bezCompS[bez_AL]*X*X);
		CBASL *= 0.5;
		double CBAMax = fmax(CBASL,CBAFL); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		double CBAFade = ((CBASL*-CBAMax)+(CBAFL*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleL *= 1.0-(fmin(((CBASL*(1.0-CBAFade))+(CBAFL*CBAFade))*bezCThresh,1.0));
		X = bezCompF[bez_cycle]*bezTrim;
		double CBAFR = bezCompF[bez_BR]+(bezCompF[bez_CR]*(1.0-X)*(1.0-X))+(bezCompF[bez_BR]*2.0*(1.0-X)*X)+(bezCompF[bez_AR]*X*X);
		CBAFR *= 0.5;
		X = bezCompS[bez_cycle]*sloTrim;
		double CBASR = bezCompS[bez_BR]+(bezCompS[bez_CR]*(1.0-X)*(1.0-X))+(bezCompS[bez_BR]*2.0*(1.0-X)*X)+(bezCompS[bez_AR]*X*X);
		CBASR *= 0.5;
		CBAMax = fmax(CBASR,CBAFR); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		CBAFade = ((CBASR*-CBAMax)+(CBAFR*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleR *= 1.0-(fmin(((CBASR*(1.0-CBAFade))+(CBAFR*CBAFade))*bezCThresh,1.0));
		//Dynamics2
		
		const double temp = (double)nSampleFrames/inFramesToProcess;
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		inputSampleL = inputSampleL * gain;
		inputSampleR = inputSampleR * gain;
		//applies pan section, and smoothed fader gain
		
		//begin ClipOnly3 as a little, compressed chunk that can be dropped into code
		double noise = 1.0-((double(fpdL)/UINT32_MAX)*0.076);
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=(0.9085097*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = 0.94; //~-0.2dB to nearly match ClipOnly and ClipOnly2
		} wasPosClipL = false;
		if (inputSampleL>0.9085097) {wasPosClipL=true;inputSampleL=(0.9085097*noise)+(lastSampleL*(1.0-noise));}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=(-0.9085097*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = -0.94;
		} wasNegClipL = false;
		if (inputSampleL<-0.9085097) {wasNegClipL=true;inputSampleL=(-0.9085097*noise)+(lastSampleL*(1.0-noise));}
		slewL[spacing*2] = fabs(lastSampleL-inputSampleL);
		for (int x = spacing*2; x > 0; x--) slewL[x-1] = slewL[x];
		intermediateL[spacing] = inputSampleL; inputSampleL = lastSampleL;
		//latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateL[x-1] = intermediateL[x];} lastSampleL = intermediateL[0];
		if (wasPosClipL || wasNegClipL) {
			for (int x = spacing; x > 0; x--) lastSampleL += intermediateL[x];
			lastSampleL /= spacing;
		} double finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewL[x]) finalSlew = slewL[x];
		double postclip = 0.94 / (1.0+(finalSlew*1.3986013));
		if (inputSampleL > postclip) inputSampleL = postclip; if (inputSampleL < -postclip) inputSampleL = -postclip;
		
		noise = 1.0-((double(fpdR)/UINT32_MAX)*0.076);
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=(0.9085097*noise)+(inputSampleR*(1.0-noise));
			else lastSampleR = 0.94; //~-0.2dB to nearly match ClipOnly and ClipOnly2
		} wasPosClipR = false;
		if (inputSampleR>0.9085097) {wasPosClipR=true;inputSampleR=(0.9085097*noise)+(lastSampleR*(1.0-noise));}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=(-0.9085097*noise)+(inputSampleR*(1.0-noise));
			else lastSampleR = -0.94;
		} wasNegClipR = false;
		if (inputSampleR<-0.9085097) {wasNegClipR=true;inputSampleR=(-0.9085097*noise)+(lastSampleR*(1.0-noise));}
		slewR[spacing*2] = fabs(lastSampleR-inputSampleR);
		for (int x = spacing*2; x > 0; x--) slewR[x-1] = slewR[x];
		intermediateR[spacing] = inputSampleR; inputSampleR = lastSampleR;
		//latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateR[x-1] = intermediateR[x];} lastSampleR = intermediateR[0];
		if (wasPosClipR || wasNegClipR) {
			for (int x = spacing; x > 0; x--) lastSampleR += intermediateR[x];
			lastSampleR /= spacing;
		} finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewR[x]) finalSlew = slewR[x];
		postclip = 0.94 / (1.0+(finalSlew*1.3986013));
		if (inputSampleR > postclip) inputSampleR = postclip; if (inputSampleR < -postclip) inputSampleR = -postclip;
		//end ClipOnly3 as a little, compressed chunk that can be dropped into code
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
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

