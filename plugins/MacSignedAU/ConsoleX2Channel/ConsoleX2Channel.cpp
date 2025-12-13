/*
*	File:		ConsoleX2Channel.cpp
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
	ConsoleX2Channel.cpp
	
=============================================================================*/
#include "ConsoleX2Channel.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, ConsoleX2Channel)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Channel::ConsoleX2Channel
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ConsoleX2Channel::ConsoleX2Channel(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_TRM, kDefaultValue_ParamTRM );
	SetParameter(kParam_MOR, kDefaultValue_ParamMOR );
	SetParameter(kParam_HIG, kDefaultValue_ParamHIG );
	SetParameter(kParam_HMG, kDefaultValue_ParamHMG );
	SetParameter(kParam_LMG, kDefaultValue_ParamLMG );
	SetParameter(kParam_BSG, kDefaultValue_ParamBSG );
	SetParameter(kParam_HIF, kDefaultValue_ParamHIF );
	SetParameter(kParam_HMF, kDefaultValue_ParamHMF );
	SetParameter(kParam_LMF, kDefaultValue_ParamLMF );
	SetParameter(kParam_BSF, kDefaultValue_ParamBSF );
	SetParameter(kParam_THR, kDefaultValue_ParamTHR );
	SetParameter(kParam_ATK, kDefaultValue_ParamATK );
	SetParameter(kParam_RLS, kDefaultValue_ParamRLS );
	SetParameter(kParam_GAT, kDefaultValue_ParamGAT );
	SetParameter(kParam_LOP, kDefaultValue_ParamLOP );
	SetParameter(kParam_HIP, kDefaultValue_ParamHIP );
	SetParameter(kParam_PAN, kDefaultValue_ParamPAN );
	SetParameter(kParam_FAD, kDefaultValue_ParamFAD );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Channel::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Channel::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Channel::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Channel::GetParameterInfo(AudioUnitScope		inScope,
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
            case kParam_HMG:
                AUBase::FillInParameterName (outParameterInfo, kParameterHMGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHMG;
                break;
            case kParam_LMG:
                AUBase::FillInParameterName (outParameterInfo, kParameterLMGName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLMG;
                break;
            case kParam_BSG:
                AUBase::FillInParameterName (outParameterInfo, kParameterBSGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamBSG;
                break;
            case kParam_HIF:
                AUBase::FillInParameterName (outParameterInfo, kParameterHIFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.unitName = kParameterHIFUnit;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHIF;
                break;
            case kParam_HMF:
                AUBase::FillInParameterName (outParameterInfo, kParameterHMFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamHMF;
                break;
            case kParam_LMF:
                AUBase::FillInParameterName (outParameterInfo, kParameterLMFName, false);
 				outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamLMF;
                break;
            case kParam_BSF:
                AUBase::FillInParameterName (outParameterInfo, kParameterBSFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamBSF;
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
            case kParam_PAN:
                AUBase::FillInParameterName (outParameterInfo, kParameterPANName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamPAN;
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
//	ConsoleX2Channel::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Channel::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 ConsoleX2Channel::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	ConsoleX2Channel::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleX2Channel::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ConsoleX2Channel::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ConsoleX2Channel::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ConsoleX2ChannelEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Channel::ConsoleX2ChannelKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		ConsoleX2Channel::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
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
	
	for (int x = 0; x < bez_total; x++) bezComp[x] = 0.0;
	bezComp[bez_cycle] = 1.0; bezMax = 0.0; bezMin = 0.0;
	bezGate = 2.0;
	//Dynamics3
	
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
	
	for (int x = 0; x < 33; x++) {avg32L[x] = 0.0; avg32R[x] = 0.0;}
	for (int x = 0; x < 17; x++) {avg16L[x] = 0.0; avg16R[x] = 0.0;}
	for (int x = 0; x < 9; x++) {avg8L[x] = 0.0; avg8R[x] = 0.0;}
	for (int x = 0; x < 5; x++) {avg4L[x] = 0.0; avg4R[x] = 0.0;}
	for (int x = 0; x < 3; x++) {avg2L[x] = 0.0; avg2R[x] = 0.0;}
	avgPos = 0;
	lastSlewL = 0.0; lastSlewR = 0.0;
	lastSlewpleL = 0.0; lastSlewpleR = 0.0;
	//preTapeHack
	
	lFreqA = 1.0; lFreqB = 1.0;
	hFreqA = 0.0; hFreqB = 0.0;
	panA = 0.5; panB = 0.5;
	inTrimA = 0.5; inTrimB = 0.5;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleX2Channel::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		ConsoleX2Channel::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	double highmidGain = (GetParameter( kParam_HMG )-0.5)*2.0;
	highmidGain = 1.0+(highmidGain*fabs(highmidGain)*fabs(highmidGain));
	double lowmidGain = (GetParameter( kParam_LMG )-0.5)*2.0;
	lowmidGain = 1.0+(lowmidGain*fabs(lowmidGain)*fabs(lowmidGain));
	double bassGain = (GetParameter( kParam_BSG )-0.5)*2.0;
	bassGain = 1.0+(bassGain*fabs(bassGain)*fabs(bassGain));
	double highCoef = 0.0;
	double midCoef = 0.0;
	double lowCoef = 0.0;
	
	bool eqOff = (trebleGain == 1.0 && highmidGain == 1.0 && lowmidGain == 1.0 && bassGain == 1.0);
	//we get to completely bypass EQ if we're truly not using it. The mechanics of it mean that
	//it cancels out to bit-identical anyhow, but we get to skip the calculation
	if (!eqOff) {
		double trebleRef = GetParameter( kParam_HIF )-0.5;
		double highmidRef = GetParameter( kParam_HMF )-0.5;
		double lowmidRef = GetParameter( kParam_LMF )-0.5;
		double bassRef = GetParameter( kParam_BSF )-0.5;
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
		highCoef = -sqrt((biqK*biqK)-1.0)+biqK;
		highA[biq_reso] = 2.24697960 * highQ;
		highB[biq_reso] = 0.80193774 * highQ;
		highC[biq_reso] = 0.55495813 * highQ;
		
		midA[biq_freq] = ((pow(midF,3)*20000.0)/GetSampleRate());
		midC[biq_freq] = midB[biq_freq] = midA[biq_freq] = fmax(fmin(midA[biq_freq],0.4999),0.00025);	
		double midFreq = pow(midF,3)*20000.0;
		omega = 2.0*M_PI*(midFreq/GetSampleRate());
		biqK = 2.0-cos(omega);
		midCoef = -sqrt((biqK*biqK)-1.0)+biqK;
		midA[biq_reso] = 2.24697960 * midQ;
		midB[biq_reso] = 0.80193774 * midQ;
		midC[biq_reso] = 0.55495813 * midQ;
		
		lowA[biq_freq] = ((pow(bassF,3)*20000.0)/GetSampleRate());
		lowC[biq_freq] = lowB[biq_freq] = lowA[biq_freq] = fmax(fmin(lowA[biq_freq],0.4999),0.00025);
		double lowFreq = pow(bassF,3)*20000.0;
		omega = 2.0*M_PI*(lowFreq/GetSampleRate());
		biqK = 2.0-cos(omega);
		lowCoef = -sqrt((biqK*biqK)-1.0)+biqK;
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
	}
	//SmoothEQ2
	
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
		
	panA = panB; panB = GetParameter( kParam_PAN )*1.57079633;
	inTrimA = inTrimB; inTrimB = GetParameter( kParam_FAD )*2.0;
	//Console
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= moreTapeHack;
		inputSampleR *= moreTapeHack;
		//trim control gets to work even when MORE is off

		if (!tapehackOff) {
			double darkSampleL = inputSampleL;
			double darkSampleR = inputSampleR;
			if (avgPos > 31) avgPos = 0;
			if (spacing > 31) {
				avg32L[avgPos] = darkSampleL; avg32R[avgPos] = darkSampleR;
				darkSampleL = 0.0; darkSampleR = 0.0;
				for (int x = 0; x < 32; x++) {darkSampleL += avg32L[x]; darkSampleR += avg32R[x];}
				darkSampleL /= 32.0; darkSampleR /= 32.0;
			} if (spacing > 15) {
				avg16L[avgPos%16] = darkSampleL; avg16R[avgPos%16] = darkSampleR;
				darkSampleL = 0.0; darkSampleR = 0.0;
				for (int x = 0; x < 16; x++) {darkSampleL += avg16L[x]; darkSampleR += avg16R[x];}
				darkSampleL /= 16.0; darkSampleR /= 16.0;
			} if (spacing > 7) {
				avg8L[avgPos%8] = darkSampleL; avg8R[avgPos%8] = darkSampleR;
				darkSampleL = 0.0; darkSampleR = 0.0;
				for (int x = 0; x < 8; x++) {darkSampleL += avg8L[x]; darkSampleR += avg8R[x];}
				darkSampleL /= 8.0; darkSampleR /= 8.0;
			} if (spacing > 3) {
				avg4L[avgPos%4] = darkSampleL; avg4R[avgPos%4] = darkSampleR;
				darkSampleL = 0.0; darkSampleR = 0.0;
				for (int x = 0; x < 4; x++) {darkSampleL += avg4L[x]; darkSampleR += avg4R[x];}
				darkSampleL /= 4.0; darkSampleR /= 4.0;
			} if (spacing > 1) {
				avg2L[avgPos%2] = darkSampleL; avg2R[avgPos%2] = darkSampleR;
				darkSampleL = 0.0; darkSampleR = 0.0;
				for (int x = 0; x < 2; x++) {darkSampleL += avg2L[x]; darkSampleR += avg2R[x];}
				darkSampleL /= 2.0; darkSampleR /= 2.0; 
			} avgPos++;
			lastSlewL += fabs(lastSlewpleL-inputSampleL); lastSlewpleL = inputSampleL;
			double avgSlewL = fmin(lastSlewL*lastSlewL*(0.0635-(overallscale*0.0018436)),1.0);
			lastSlewL = fmax(lastSlewL*0.78,2.39996322972865332223);
			lastSlewR += fabs(lastSlewpleR-inputSampleR); lastSlewpleR = inputSampleR;
			double avgSlewR = fmin(lastSlewR*lastSlewR*(0.0635-(overallscale*0.0018436)),1.0);
			lastSlewR = fmax(lastSlewR*0.78,2.39996322972865332223); //look up Golden Angle, it's cool
			inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
			inputSampleR = (inputSampleR*(1.0-avgSlewR)) + (darkSampleR*avgSlewR);
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
			inputSampleR = fmax(fmin(inputSampleR,2.305929007734908),-2.305929007734908);
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
			//end TapeHack section
			//Discontapeity
		}
		
		if (!eqOff) {
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
		}
		//SmoothEQ2
		
		if (bezThresh > 0.0) {
			if (fmax(fabs(inputSampleL),fabs(inputSampleR)) > gate) bezGate = overallscale/fmin(bezRez,sloRez);
			else bezGate = bezGate = fmax(0.000001, bezGate-fmin(bezRez,sloRez));
			inputSampleL *= (bezThresh+1.0);
			inputSampleR *= (bezThresh+1.0);
			double ctrl = fmax(fabs(inputSampleL),fabs(inputSampleR));
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
			inputSampleL *= 1.0-(fmin(CBA*bezThresh,1.0));
			inputSampleR *= 1.0-(fmin(CBA*bezThresh,1.0));
		} else bezComp[bez_Ctrl] = 0.0;
		//Dynamics3
		
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
				
		double gainR = (panA*temp)+(panB*(1.0-temp));
		double gainL = 1.57079633-gainR;
		gainR = sin(gainR); gainL = sin(gainL);
		
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		if (gain > 1.0) gain *= gain;
		if (gain < 1.0) gain = 1.0-pow(1.0-gain,2);
		
		inputSampleL = inputSampleL * gainL * gain;
		inputSampleR = inputSampleR * gainR * gain;
		//applies pan section, and smoothed fader gain
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 1.618033988749895));
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 1.618033988749895));
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 1.618033988749895));
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 1.618033988749895));
		
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

