/*
*	File:		kRockstar2.cpp
*	
*	Version:	1.0
* 
*	Created:	4/9/26
*	
*	Copyright:  Copyright © 2026 Airwindows, Airwindows uses the MIT license
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
	kRockstar2.cpp
	
=============================================================================*/
#include "kRockstar2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, kRockstar2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kRockstar2::kRockstar2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
kRockstar2::kRockstar2(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
	SetParameter(kParam_D, kDefaultValue_ParamD );
	SetParameter(kParam_E, kDefaultValue_ParamE );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kRockstar2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kRockstar2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kRockstar2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kRockstar2::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamE;
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
//	kRockstar2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kRockstar2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 kRockstar2::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	kRockstar2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kRockstar2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	kRockstar2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult kRockstar2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____kRockstar2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kRockstar2::kRockstar2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		kRockstar2::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for(int x = 0; x < d3A+2; x++) {a3AL[x] = 0.0; a3AR[x] = 0.0;}
	for(int x = 0; x < d3B+2; x++) {a3BL[x] = 0.0; a3BR[x] = 0.0;}
	for(int x = 0; x < d3C+2; x++) {a3CL[x] = 0.0; a3CR[x] = 0.0;}
	for(int x = 0; x < d3D+2; x++) {a3DL[x] = 0.0; a3DR[x] = 0.0;}
	for(int x = 0; x < d3E+2; x++) {a3EL[x] = 0.0; a3ER[x] = 0.0;}
	for(int x = 0; x < d3F+2; x++) {a3FL[x] = 0.0; a3FR[x] = 0.0;}
	for(int x = 0; x < d3G+2; x++) {a3GL[x] = 0.0; a3GR[x] = 0.0;}
	for(int x = 0; x < d3H+2; x++) {a3HL[x] = 0.0; a3HR[x] = 0.0;}
	for(int x = 0; x < d3I+2; x++) {a3IL[x] = 0.0; a3IR[x] = 0.0;}
	c3AL = c3BL = c3CL = c3DL = c3EL = c3FL = c3GL = c3HL = c3IL = 1;
	c3AR = c3BR = c3CR = c3DR = c3ER = c3FR = c3GR = c3HR = c3IR = 1;
	earlyZero = 0;
	
	ld3G = early[27]; 
	ld3H = early[28]; 
	ld3D = early[29]; 
	ld3A = early[30]; 
	ld3E = early[31]; 
	ld3I = early[32];
	ld3F = early[33];
	ld3B = early[34]; 
	ld3C = early[35];
	
	for(int x = 0; x < d6A+2; x++) {a6AL[x] = 0.0; a6AR[x] = 0.0;}
	for(int x = 0; x < d6B+2; x++) {a6BL[x] = 0.0; a6BR[x] = 0.0;}
	for(int x = 0; x < d6C+2; x++) {a6CL[x] = 0.0; a6CR[x] = 0.0;}
	for(int x = 0; x < d6D+2; x++) {a6DL[x] = 0.0; a6DR[x] = 0.0;}
	for(int x = 0; x < d6E+2; x++) {a6EL[x] = 0.0; a6ER[x] = 0.0;}
	for(int x = 0; x < d6F+2; x++) {a6FL[x] = 0.0; a6FR[x] = 0.0;}
	for(int x = 0; x < d6G+2; x++) {a6GL[x] = 0.0; a6GR[x] = 0.0;}
	for(int x = 0; x < d6H+2; x++) {a6HL[x] = 0.0; a6HR[x] = 0.0;}
	for(int x = 0; x < d6I+2; x++) {a6IL[x] = 0.0; a6IR[x] = 0.0;}
	for(int x = 0; x < d6J+2; x++) {a6JL[x] = 0.0; a6JR[x] = 0.0;}
	for(int x = 0; x < d6K+2; x++) {a6KL[x] = 0.0; a6KR[x] = 0.0;}
	for(int x = 0; x < d6L+2; x++) {a6LL[x] = 0.0; a6LR[x] = 0.0;}
	for(int x = 0; x < d6M+2; x++) {a6ML[x] = 0.0; a6MR[x] = 0.0;}
	for(int x = 0; x < d6N+2; x++) {a6NL[x] = 0.0; a6NR[x] = 0.0;}
	for(int x = 0; x < d6O+2; x++) {a6OL[x] = 0.0; a6OR[x] = 0.0;}
	for(int x = 0; x < d6P+2; x++) {a6PL[x] = 0.0; a6PR[x] = 0.0;}
	for(int x = 0; x < d6Q+2; x++) {a6QL[x] = 0.0; a6QR[x] = 0.0;}
	for(int x = 0; x < d6R+2; x++) {a6RL[x] = 0.0; a6RR[x] = 0.0;}
	for(int x = 0; x < d6S+2; x++) {a6SL[x] = 0.0; a6SR[x] = 0.0;}
	for(int x = 0; x < d6T+2; x++) {a6TL[x] = 0.0; a6TR[x] = 0.0;}
	for(int x = 0; x < d6U+2; x++) {a6UL[x] = 0.0; a6UR[x] = 0.0;}
	for(int x = 0; x < d6V+2; x++) {a6VL[x] = 0.0; a6VR[x] = 0.0;}
	for(int x = 0; x < d6W+2; x++) {a6WL[x] = 0.0; a6WR[x] = 0.0;}
	for(int x = 0; x < d6X+2; x++) {a6XL[x] = 0.0; a6XR[x] = 0.0;}
	for(int x = 0; x < d6Y+2; x++) {a6YL[x] = 0.0; a6YR[x] = 0.0;}
	for(int x = 0; x < d6ZA+2; x++) {a6ZAL[x] = 0.0; a6ZAR[x] = 0.0;}
	for(int x = 0; x < d6ZB+2; x++) {a6ZBL[x] = 0.0; a6ZBR[x] = 0.0;}
	for(int x = 0; x < d6ZC+2; x++) {a6ZCL[x] = 0.0; a6ZCR[x] = 0.0;}
	for(int x = 0; x < d6ZD+2; x++) {a6ZDL[x] = 0.0; a6ZDR[x] = 0.0;}
	for(int x = 0; x < d6ZE+2; x++) {a6ZEL[x] = 0.0; a6ZER[x] = 0.0;}
	for(int x = 0; x < d6ZF+2; x++) {a6ZFL[x] = 0.0; a6ZFR[x] = 0.0;}
	for(int x = 0; x < d6ZG+2; x++) {a6ZGL[x] = 0.0; a6ZGR[x] = 0.0;}
	for(int x = 0; x < d6ZH+2; x++) {a6ZHL[x] = 0.0; a6ZHR[x] = 0.0;}
	for(int x = 0; x < d6ZI+2; x++) {a6ZIL[x] = 0.0; a6ZIR[x] = 0.0;}
	for(int x = 0; x < d6ZJ+2; x++) {a6ZJL[x] = 0.0; a6ZJR[x] = 0.0;}
	for(int x = 0; x < d6ZK+2; x++) {a6ZKL[x] = 0.0; a6ZKR[x] = 0.0;}
	c6AL = c6BL = c6CL = c6DL = c6EL = c6FL = c6GL = c6HL = c6IL = 1;
	c6JL = c6KL = c6LL = c6ML = c6NL = c6OL = c6PL = c6QL = c6RL = 1;
	c6SL = c6TL = c6UL = c6VL = c6WL = c6XL = c6YL = c6ZAL = c6ZBL = 1;
	c6ZCL = c6ZDL = c6ZEL = c6ZFL = c6ZGL = c6ZHL = c6ZIL = c6ZJL = c6ZKL = 1;
	c6AR = c6BR = c6CR = c6DR = c6ER = c6FR = c6GR = c6HR = c6IR = 1;
	c6JR = c6KR = c6LR = c6MR = c6NR = c6OR = c6PR = c6QR = c6RR = 1;
	c6SR = c6TR = c6UR = c6VR = c6WR = c6XR = c6YR = c6ZAR = c6ZBR = 1;
	c6ZCR = c6ZDR = c6ZER = c6ZFR = c6ZGR = c6ZHR = c6ZIR = c6ZJR = c6ZKR = 1;
	f6AL = f6BL = f6CL = f6DL = f6EL = f6FL = 0.0;
	f6FR = f6LR = f6RR = f6XR = f6ZER = f6ZKR = 0.0;
	fhAL = fhBL = fhCL = fhDL = fhEL = fhFL = 0.0;
	flAL = flBL = flCL = flDL = flEL = flFL = 0.0;
	fhAR = fhBR = fhCR = fhDR = fhER = fhFR = 0.0;
	flAR = flBR = flCR = flDR = flER = flFR = 0.0;
	hA = hB = hC = hD = hE = hF = 0.0;
	
	for (int x = 0; x < bez_total; x++) {
		bez[x] = 0.0;
	}
	bez[bez_cycle] = 1.0;
	
	firstDryL = firstAvgL = 0.0f;
	lsA = lsB = lsC = lsD = lsE = lsF = lsG = lsH = lsI = lsJ = lsK = lsL = lsM = 0.0f;
	lsN = lsO = lsP = lsQ = lsR = lsS = lsT = lsU = lsV = lsW = lsX = lsY = lsZ = 0.0f;
	lsa = lsb = lsc = lsd = lse = lsf = lsg = lsh = lsi = lsj = lsk = lsl = lsm = 0.0f;
	lsn = lso = lsp = lsq = lsr = lss = lst = lsu = lsv = lsw = lsx = lsy = lsz = 0.0f;	
	firstDryR = firstAvgR = 0.0f;
	rsA = rsB = rsC = rsD = rsE = rsF = rsG = rsH = rsI = rsJ = rsK = rsL = rsM = 0.0f;
	rsN = rsO = rsP = rsQ = rsR = rsS = rsT = rsU = rsV = rsW = rsX = rsY = rsZ = 0.0f;
	rsa = rsb = rsc = rsd = rse = rsf = rsg = rsh = rsi = rsj = rsk = rsl = rsm = 0.0f;
	rsn = rso = rsp = rsq = rsr = rss = rst = rsu = rsv = rsw = rsx = rsy = rsz = 0.0f;	
	firstdryl = firstavgl = 0.0f;
	firstdryr = firstavgr = 0.0f;
	
	prevDistance = 0;	
		
	roomNoiseL = 0.0;
	roomNoiseR = 0.0006;//this becomes regen, in practice
	//it will chase what you set regen to, rather than chasing 0
	roomTimerL = 0;
	roomTimerR = 0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kRockstar2::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		kRockstar2::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	overallscale /= 48000.0;
	overallscale *= GetSampleRate();

	double reg6n = (1.0-pow(1.0-GetParameter( kParam_A ),3.0))*0.0078125;
	double regenMax = 0.0078125;
	int start = (int)(GetParameter( kParam_B ) * 27.0);
	
	double downRez = ((GetParameter( kParam_C )*(overallscale/(overallscale+0.99999)))/overallscale);
	downRez = fmin(fmax(downRez,0.0005),1.0/overallscale);

	int bezFraction = (int)(1.0/downRez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	downRez = 0.99999999 / bezFraction;
	bezTrim = 1.0-(downRez*bezTrim);
	double distance = pow(GetParameter( kParam_D )*5.0f,2.0f);
	int distanceSteps = (int)distance; //25 maximum
	distance -= (double)distanceSteps; 
	double wet = GetParameter( kParam_E );
		
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += downRez;
		bez[bez_SampL] += (inputSampleL * downRez);
		bez[bez_SampR] += (inputSampleR * downRez);
		if (bez[bez_cycle] > bezTrim) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			
			double earlyFloor = fabs(inputSampleL) + fabs(inputSampleR);
			switch(earlyZero){
				case 0:
					earlyFloor = earlyFloor + fabs(a3AL[c3AL]) + fabs(a3AR[c3AR]);
					if (ld3A != early[start+3] && earlyFloor < 0.06125)
					{
						for(int x = 0; x < d3A+2; x++) {a3AL[x] = 0.0; a3AR[x] = 0.0;}
						ld3A = early[start+3];
					} break;
				case 1:
					earlyFloor = earlyFloor + fabs(a3BL[c3BL]) + fabs(a3BR[c3BR]);
					if (ld3B != early[start+7] && earlyFloor < 0.06125)
					{
						for(int x = 0; x < d3B+2; x++) {a3BL[x] = 0.0; a3BR[x] = 0.0;}
						ld3B = early[start+7];
					} break;
				case 2:
					earlyFloor = earlyFloor + fabs(a3CL[c3CL]) + fabs(a3CR[c3CR]);
					if (ld3C != early[start+8] && earlyFloor < 0.06125)
					{
						for(int x = 0; x < d3C+2; x++) {a3CL[x] = 0.0; a3CR[x] = 0.0;}
						ld3C = early[start+8];
					} break;
				case 3:
					earlyFloor = earlyFloor + fabs(a3DL[c3DL]) + fabs(a3DR[c3DR]);
					if (ld3D != early[start+2] && earlyFloor < 0.06125)
					{
						for(int x = 0; x < d3D+2; x++) {a3DL[x] = 0.0; a3DR[x] = 0.0;}
						ld3D = early[start+2];
					} break;
				case 4:
					earlyFloor = earlyFloor + fabs(a3EL[c3EL]) + fabs(a3ER[c3ER]);
					if (ld3E != early[start+4] && earlyFloor < 0.06125)
					{
						for(int x = 0; x < d3E+2; x++) {a3EL[x] = 0.0; a3ER[x] = 0.0;}
						ld3E = early[start+4];
					} break;
				case 5:
					earlyFloor = earlyFloor + fabs(a3FL[c3FL]) + fabs(a3FR[c3FR]);
					if (ld3F != early[start+6] && earlyFloor < 0.06125)
					{
						for(int x = 0; x < d3F+2; x++) {a3FL[x] = 0.0; a3FR[x] = 0.0;}
						ld3F = early[start+6];
					} break;
				case 6:
					earlyFloor = earlyFloor + fabs(a3GL[c3GL]) + fabs(a3GR[c3GR]);
					if (ld3G != early[start] && earlyFloor < 0.06125)
					{
						for(int x = 0; x < d3G+2; x++) {a3GL[x] = 0.0; a3GR[x] = 0.0;}
						ld3G = early[start];
					} break;
				case 7:
					earlyFloor = earlyFloor + fabs(a3HL[c3HL]) + fabs(a3HR[c3HR]);
					if (ld3H != early[start+1] && earlyFloor < 0.06125)
					{
						for(int x = 0; x < d3H+2; x++) {a3HL[x] = 0.0; a3HR[x] = 0.0;}
						ld3H = early[start+1];
					} break;
				case 8:
					earlyFloor = earlyFloor + fabs(a3IL[c3IL]) + fabs(a3IR[c3IR]);
					if (ld3I != early[start+5] && earlyFloor < 0.06125)
					{
						for(int x = 0; x < d3I+2; x++) {a3IL[x] = 0.0; a3IR[x] = 0.0;}
						ld3I = early[start+5];
					} break;
			}
			earlyZero++; if (earlyZero > 8) earlyZero = 0;
			
			inputSampleL = (bez[bez_SampL]);
			inputSampleR = (bez[bez_SampR]);
			
			a3AL[c3AL] = inputSampleL;
			a3BL[c3BL] = inputSampleL;
			a3CL[c3CL] = inputSampleL;
			a3CR[c3CR] = inputSampleR;
			a3FR[c3FR] = inputSampleR;
			a3IR[c3IR] = inputSampleR;
			
			c3AL++; c3BL++; c3CL++; c3CR++; c3FR++; c3IR++; 
			
			if (c3AL > ld3A) c3AL = 0;
			if (c3BL > ld3B) c3BL = 0;
			if (c3CL > ld3C) c3CL = 0;
			if (c3CR > ld3C) c3CR = 0;
			if (c3FR > ld3F) c3FR = 0;
			if (c3IR > ld3I) c3IR = 0;
			
			hA = a3AL[c3AL-((c3AL > ld3A)?c3AL+1:0)];
			hB = a3BL[c3BL-((c3BL > ld3B)?c3BL+1:0)];
			hC = a3CL[c3CL-((c3CL > ld3C)?c3CL+1:0)];
			hD = a3CR[c3CR-((c3CR > ld3C)?c3CR+1:0)];
			hE = a3FR[c3FR-((c3FR > ld3F)?c3FR+1:0)];
			hF = a3IR[c3IR-((c3IR > ld3I)?c3IR+1:0)];
			
			a3DL[c3DL] = hB + hC;
			a3EL[c3EL] = hA + hC;
			a3FL[c3FL] = hA + hB;
			a3BR[c3BR] = hE + hF;
			a3ER[c3ER] = hD + hF;
			a3HR[c3HR] = hD + hE;
			
			a3DL[c3DL] = (((hB + hC) * -2.0) + hA);
			a3EL[c3EL] = (((hA + hC) * -2.0) + hB);
			a3FL[c3FL] = (((hA + hB) * -2.0) + hC);
			a3BR[c3BR] = (((hE + hF) * -2.0) + hD);
			a3ER[c3ER] = (((hD + hF) * -2.0) + hE);
			a3HR[c3HR] = (((hD + hE) * -2.0) + hF);
			
			c3DL++; c3EL++; c3FL++; c3BR++; c3ER++; c3HR++; 
			
			if (c3DL > ld3D) c3DL = 0;
			if (c3EL > ld3E) c3EL = 0;
			if (c3FL > ld3F) c3FL = 0;
			if (c3BR > ld3B) c3BR = 0;
			if (c3ER > ld3E) c3ER = 0;
			if (c3HR > ld3H) c3HR = 0;
			
			hA = a3DL[c3DL-((c3DL > ld3D)?c3DL+1:0)];
			hB = a3EL[c3EL-((c3EL > ld3E)?c3EL+1:0)];
			hC = a3FL[c3FL-((c3FL > ld3F)?c3FL+1:0)];
			hD = a3BR[c3BR-((c3BR > ld3B)?c3BR+1:0)];
			hE = a3ER[c3ER-((c3ER > ld3E)?c3ER+1:0)];
			hF = a3HR[c3HR-((c3HR > ld3H)?c3HR+1:0)];
			
			a3GL[c3GL] = (((hB + hC) * -2.0) + hA);
			a3HL[c3HL] = (((hA + hC) * -2.0) + hB);
			a3IL[c3IL] = (((hA + hB) * -2.0) + hC);
			a3AR[c3AR] = (((hE + hF) * -2.0) + hD);
			a3DR[c3DR] = (((hD + hF) * -2.0) + hE);
			a3GR[c3GR] = (((hD + hE) * -2.0) + hF);
			
			c3GL++; c3HL++; c3IL++; c3AR++; c3DR++; c3GR++; 
			
			if (c3GL > ld3G) c3GL = 0;
			if (c3HL > ld3H) c3HL = 0;
			if (c3IL > ld3I) c3IL = 0;
			if (c3AR > ld3A) c3AR = 0;
			if (c3DR > ld3D) c3DR = 0;
			if (c3GR > ld3G) c3GR = 0;
			
			hA = a3GL[c3GL-((c3GL > ld3G)?c3GL+1:0)];
			hB = a3HL[c3HL-((c3HL > ld3H)?c3HL+1:0)];
			hC = a3IL[c3IL-((c3IL > ld3I)?c3IL+1:0)];
			hD = a3AR[c3AR-((c3AR > ld3A)?c3AR+1:0)];
			hE = a3DR[c3DR-((c3DR > ld3D)?c3DR+1:0)];
			hF = a3GR[c3GR-((c3GR > ld3G)?c3GR+1:0)];
			
			double earlyReflectionL = (((hB + hC) * -2.0) + hA)*-0.0625;
			double earlyReflectionR = (((hE + hF) * -2.0) + hD)*-0.0625;
			
			double nonlin = 1.0-fmin(fmax(fabs(inputSampleL),fabs(inputSampleR)),1.0);
			nonlin = 1.0-(nonlin*nonlin*nonlin);
			
			inputSampleL -= earlyReflectionL;
			inputSampleR -= earlyReflectionR;
			
			roomTimerL++;
			if (roomTimerL > 13) {
				roomTimerL = 0;
				if (roomNoiseL > 0.0) roomNoiseL -= fabs((fpdL / (double)UINT32_MAX)-0.5)*0.125;
				else roomNoiseL += fabs((fpdL / (double)UINT32_MAX)-0.5)*0.125;
			} else roomNoiseL += ((fpdL / (double)UINT32_MAX)-0.5)*0.125;
			//roomNoiseL governs the added 'room tone' noise. 0.125 gives you not an overwhelming
			//volume of it, less will be quieter. The 13 is part of VoiceOfTheStarship, and
			//larger allows more subs into the noise: it'll determine the apparent room scale,
			//with subs content implying a HUGE space, no lows implying a small space.
			
			roomTimerR++;
			if (roomTimerR > 4) {
				roomTimerR = 0;
				if (roomNoiseR > fmin(reg6n+(nonlin*0.0002),regenMax)) roomNoiseR -= fabs((fpdR / (double)UINT32_MAX)-0.5)*0.000007629;
				else roomNoiseR += fabs((fpdR / (double)UINT32_MAX)-0.5)*0.000007629;
			} else roomNoiseR += ((fpdR / (double)UINT32_MAX)-0.5)*0.000007629;
			//roomNoiseR governs the activity of the air in this space. Consider it in terms of
			//winds at MPH: the RoomTimer number is (int)sqrt(wind MPH) which governs the intensity
			//of gusts and fluctuations (avoid hurricane force inside realistic rooms)
			//and the scaling of roomNoiseR (as added to reg6n) is sqrt(wind MPH)*0.00002
			//but check against pure tones, don't let it dirty them up too much!
			f6BL += roomNoiseL;
			a6AL[c6AL] = fma(f6BL,roomNoiseR,inputSampleL);
			a6BL[c6BL] = fma(f6CL,roomNoiseR,inputSampleL);
			a6CL[c6CL] = fma(f6DL,roomNoiseR,inputSampleL);
			a6DL[c6DL] = fma(f6EL,roomNoiseR,inputSampleL);
			a6EL[c6EL] = fma(f6FL,roomNoiseR,inputSampleL);
			a6FL[c6FL] = fma(f6AL,roomNoiseR,inputSampleL);
			
			f6LR += roomNoiseL;
			a6FR[c6FR] = fma(f6LR,roomNoiseR,inputSampleR);
			a6LR[c6LR] = fma(f6RR,roomNoiseR,inputSampleR);
			a6RR[c6RR] = fma(f6XR,roomNoiseR,inputSampleR);
			a6XR[c6XR] = fma(f6ZER,roomNoiseR,inputSampleR);
			a6ZER[c6ZER] = fma(f6ZKR,roomNoiseR,inputSampleR);
			a6ZKR[c6ZKR] = fma(f6FR,roomNoiseR,inputSampleR);
			
			//left verb
			
			c6AL++; c6BL++; c6CL++; c6DL++; c6EL++; c6FL++; 
			
			if (c6AL > d6A) c6AL = 0;
			if (c6BL > d6B) c6BL = 0;
			if (c6CL > d6C) c6CL = 0;
			if (c6DL > d6D) c6DL = 0;
			if (c6EL > d6E) c6EL = 0;
			if (c6FL > d6F) c6FL = 0;
			
			hA = a6AL[c6AL-((c6AL > d6A)?d6A+1:0)];
			hB = a6BL[c6BL-((c6BL > d6B)?d6B+1:0)];
			hC = a6CL[c6CL-((c6CL > d6C)?d6C+1:0)];
			hD = a6DL[c6DL-((c6DL > d6D)?d6D+1:0)];
			hE = a6EL[c6EL-((c6EL > d6E)?d6E+1:0)];
			hF = a6FL[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6GL[c6GL] = hB + hC + hD + hE + hF;
			a6HL[c6HL] = (hA + hC + hF) - (hD + hE);
			a6IL[c6IL] = (hA + hB + hD) - (hE + hF);
			a6JL[c6JL] = (hA + hC + hE) - (hB + hF);
			a6KL[c6KL] = (hA + hD + hF) - (hB + hC);
			a6LL[c6LL] = (hA + hB + hE) - (hC + hD);
			
			c6GL++; c6HL++; c6IL++; c6JL++; c6KL++; c6LL++; 
			
			if (c6GL > d6G) c6GL = 0;
			if (c6HL > d6H) c6HL = 0;
			if (c6IL > d6I) c6IL = 0;
			if (c6JL > d6J) c6JL = 0;
			if (c6KL > d6K) c6KL = 0;
			if (c6LL > d6L) c6LL = 0;
			
			hA = a6GL[c6GL-((c6GL > d6G)?d6G+1:0)];
			hB = a6HL[c6HL-((c6HL > d6H)?d6H+1:0)];
			hC = a6IL[c6IL-((c6IL > d6I)?d6I+1:0)];
			hD = a6JL[c6JL-((c6JL > d6J)?d6J+1:0)];
			hE = a6KL[c6KL-((c6KL > d6K)?d6K+1:0)];
			hF = a6LL[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6ML[c6ML] = hB + hC + hD + hE + hF;
			a6NL[c6NL] = (hA + hC + hF) - (hD + hE);
			a6OL[c6OL] = (hA + hB + hD) - (hE + hF);
			a6PL[c6PL] = (hA + hC + hE) - (hB + hF);
			a6QL[c6QL] = (hA + hD + hF) - (hB + hC);
			a6RL[c6RL] = (hA + hB + hE) - (hC + hD);
			
			
			c6ML++; c6NL++; c6OL++; c6PL++; c6QL++; c6RL++; 
			
			if (c6ML > d6M) c6ML = 0;
			if (c6NL > d6N) c6NL = 0;
			if (c6OL > d6O) c6OL = 0;
			if (c6PL > d6P) c6PL = 0;
			if (c6QL > d6Q) c6QL = 0;
			if (c6RL > d6R) c6RL = 0;
			
			hA = a6ML[c6ML-((c6ML > d6M)?d6M+1:0)];
			hB = a6NL[c6NL-((c6NL > d6N)?d6N+1:0)];
			hC = a6OL[c6OL-((c6OL > d6O)?d6O+1:0)];
			hD = a6PL[c6PL-((c6PL > d6P)?d6P+1:0)];
			hE = a6QL[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			hF = a6RL[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6SL[c6SL] = hB + hC + hD + hE + hF;
			a6TL[c6TL] = (hA + hC + hF) - (hD + hE);
			a6UL[c6UL] = (hA + hB + hD) - (hE + hF);
			a6VL[c6VL] = (hA + hC + hE) - (hB + hF);
			a6WL[c6WL] = (hA + hD + hF) - (hB + hC);
			a6XL[c6XL] = (hA + hB + hE) - (hC + hD);
			
			
			c6SL++; c6TL++; c6UL++; c6VL++; c6WL++; c6XL++; 
			
			if (c6SL > d6S) c6SL = 0;
			if (c6TL > d6T) c6TL = 0;
			if (c6UL > d6U) c6UL = 0;
			if (c6VL > d6V) c6VL = 0;
			if (c6WL > d6W) c6WL = 0;
			if (c6XL > d6X) c6XL = 0;
			
			hA = a6SL[c6SL-((c6SL > d6S)?d6S+1:0)];
			hB = a6TL[c6TL-((c6TL > d6T)?d6T+1:0)];
			hC = a6UL[c6UL-((c6UL > d6U)?d6U+1:0)];
			hD = a6VL[c6VL-((c6VL > d6V)?d6V+1:0)];
			hE = a6WL[c6WL-((c6WL > d6W)?d6W+1:0)];
			hF = a6XL[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6YL[c6YL] = hB + hC + hD + hE + hF;
			a6ZAL[c6ZAL] = (hA + hC + hF) - (hD + hE);
			a6ZBL[c6ZBL] = (hA + hB + hD) - (hE + hF);
			a6ZCL[c6ZCL] = (hA + hC + hE) - (hB + hF);
			a6ZDL[c6ZDL] = (hA + hD + hF) - (hB + hC);
			a6ZEL[c6ZEL] = (hA + hB + hE) - (hC + hD);
			
			c6YL++; c6ZAL++; c6ZBL++; c6ZCL++; c6ZDL++; c6ZEL++; 
			
			if (c6YL > d6Y) c6YL = 0;
			if (c6ZAL > d6ZA) c6ZAL = 0;
			if (c6ZBL > d6ZB) c6ZBL = 0;
			if (c6ZCL > d6ZC) c6ZCL = 0;
			if (c6ZDL > d6ZD) c6ZDL = 0;
			if (c6ZEL > d6ZE) c6ZEL = 0;
			
			hA = a6YL[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			hB = a6ZAL[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			hC = a6ZBL[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			hD = a6ZCL[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			hE = a6ZDL[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			hF = a6ZEL[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6ZFL[c6ZFL] = hB + hC + hD + hE + hF;
			a6ZGL[c6ZGL] = (hA + hC + hF) - (hD + hE);
			a6ZHL[c6ZHL] = (hA + hB + hD) - (hE + hF);
			a6ZIL[c6ZIL] = (hA + hC + hE) - (hB + hF);
			a6ZJL[c6ZJL] = (hA + hD + hF) - (hB + hC);
			a6ZKL[c6ZKL] = (hA + hB + hE) - (hC + hD);
			
			c6ZFL++; c6ZGL++; c6ZHL++; c6ZIL++; c6ZJL++; c6ZKL++; 
			
			if (c6ZFL > d6ZF) c6ZFL = 0;
			if (c6ZGL > d6ZG) c6ZGL = 0;
			if (c6ZHL > d6ZH) c6ZHL = 0;
			if (c6ZIL > d6ZI) c6ZIL = 0;
			if (c6ZJL > d6ZJ) c6ZJL = 0;
			if (c6ZKL > d6ZK) c6ZKL = 0;
			
			hA = a6ZFL[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			hB = a6ZGL[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			hC = a6ZHL[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			hD = a6ZIL[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			hE = a6ZJL[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			hF = a6ZKL[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			f6FR = hB + hC + hD + hE + hF;
			f6LR = (hA + hC + hF) - (hD + hE);
			f6RR = (hA + hB + hD) - (hE + hF);
			f6XR = (hA + hC + hE) - (hB + hF);
			f6ZER = (hA + hD + hF) - (hB + hC);
			f6ZKR = (hA + hB + hE) - (hC + hD);
			
			inputSampleL = (hB + hC + hD + hE + hF)*0.015625;
			
			//right verb
			
			c6FR++; c6LR++; c6RR++; c6XR++; c6ZER++; c6ZKR++; 
			
			if (c6FR > d6F) c6FR = 0;
			if (c6LR > d6L) c6LR = 0;
			if (c6RR > d6R) c6RR = 0;
			if (c6XR > d6X) c6XR = 0;
			if (c6ZER > d6ZE) c6ZER = 0;
			if (c6ZKR > d6ZK) c6ZKR = 0;
			
			hA = a6FR[c6FR-((c6FR > d6F)?d6F+1:0)];
			hB = a6LR[c6LR-((c6LR > d6L)?d6L+1:0)];
			hC = a6RR[c6RR-((c6RR > d6R)?d6R+1:0)];
			hD = a6XR[c6XR-((c6XR > d6X)?d6X+1:0)];
			hE = a6ZER[c6ZER-((c6ZER > d6ZE)?d6ZE+1:0)];
			hF = a6ZKR[c6ZKR-((c6ZKR > d6ZK)?d6ZK+1:0)];

			a6ER[c6ER] = hB + hC + hD + hE + hF;
			a6KR[c6KR] = (hA + hC + hF) - (hD + hE);
			a6QR[c6QR] = (hA + hB + hD) - (hE + hF);
			a6WR[c6WR] = (hA + hC + hE) - (hB + hF);
			a6ZDR[c6ZDR] = (hA + hD + hF) - (hB + hC);
			a6ZJR[c6ZJR] = (hA + hB + hE) - (hC + hD);
			
			c6ER++; c6KR++; c6QR++; c6WR++; c6ZDR++; c6ZJR++; 
			
			if (c6ER > d6E) c6ER = 0;
			if (c6KR > d6K) c6KR = 0;
			if (c6QR > d6Q) c6QR = 0;
			if (c6WR > d6W) c6WR = 0;
			if (c6ZDR > d6ZD) c6ZDR = 0;
			if (c6ZJR > d6ZJ) c6ZJR = 0;
			
			hA = a6ER[c6ER-((c6ER > d6E)?d6E+1:0)];
			hB = a6KR[c6KR-((c6KR > d6K)?d6K+1:0)];
			hC = a6QR[c6QR-((c6QR > d6Q)?d6Q+1:0)];
			hD = a6WR[c6WR-((c6WR > d6W)?d6W+1:0)];
			hE = a6ZDR[c6ZDR-((c6ZDR > d6ZD)?d6ZD+1:0)];
			hF = a6ZJR[c6ZJR-((c6ZJR > d6ZJ)?d6ZJ+1:0)];
			
			a6DR[c6DR] = hB + hC + hD + hE + hF;
			a6JR[c6JR] = (hA + hC + hF) - (hD + hE);
			a6PR[c6PR] = (hA + hB + hD) - (hE + hF);
			a6VR[c6VR] = (hA + hC + hE) - (hB + hF);
			a6ZCR[c6ZCR] = (hA + hD + hF) - (hB + hC);
			a6ZIR[c6ZIR] = (hA + hB + hE) - (hC + hD);
			
			c6DR++; c6JR++; c6PR++; c6VR++; c6ZCR++; c6ZIR++; 
			
			if (c6DR > d6D) c6DR = 0;
			if (c6JR > d6J) c6JR = 0;
			if (c6PR > d6P) c6PR = 0;
			if (c6VR > d6V) c6VR = 0;
			if (c6ZCR > d6ZC) c6ZCR = 0;
			if (c6ZIR > d6ZI) c6ZIR = 0;
			
			hA = a6DR[c6DR-((c6DR > d6D)?d6D+1:0)];
			hB = a6JR[c6JR-((c6JR > d6J)?d6J+1:0)];
			hC = a6PR[c6PR-((c6PR > d6P)?d6P+1:0)];
			hD = a6VR[c6VR-((c6VR > d6V)?d6V+1:0)];
			hE = a6ZCR[c6ZCR-((c6ZCR > d6ZC)?d6ZC+1:0)];
			hF = a6ZIR[c6ZIR-((c6ZIR > d6ZI)?d6ZI+1:0)];
			
			a6CR[c6CR] = hB + hC + hD + hE + hF;
			a6IR[c6IR] = (hA + hC + hF) - (hD + hE);
			a6OR[c6OR] = (hA + hB + hD) - (hE + hF);
			a6UR[c6UR] = (hA + hC + hE) - (hB + hF);
			a6ZBR[c6ZBR] = (hA + hD + hF) - (hB + hC);
			a6ZHR[c6ZHR] = (hA + hB + hE) - (hC + hD);
			
			c6CR++; c6IR++; c6OR++; c6UR++; c6ZBR++; c6ZHR++; 
			
			if (c6CR > d6C) c6CR = 0;
			if (c6IR > d6I) c6IR = 0;
			if (c6OR > d6O) c6OR = 0;
			if (c6UR > d6U) c6UR = 0;
			if (c6ZBR > d6ZB) c6ZBR = 0;
			if (c6ZHR > d6ZH) c6ZHR = 0;
			
			hA = a6CR[c6CR-((c6CR > d6C)?d6C+1:0)];
			hB = a6IR[c6IR-((c6IR > d6I)?d6I+1:0)];
			hC = a6OR[c6OR-((c6OR > d6O)?d6O+1:0)];
			hD = a6UR[c6UR-((c6UR > d6U)?d6U+1:0)];
			hE = a6ZBR[c6ZBR-((c6ZBR > d6ZB)?d6ZB+1:0)];
			hF = a6ZHR[c6ZHR-((c6ZHR > d6ZH)?d6ZH+1:0)];
			
			a6BR[c6BR] = hB + hC + hD + hE + hF;
			a6HR[c6HR] = (hA + hC + hF) - (hD + hE);
			a6NR[c6NR] = (hA + hB + hD) - (hE + hF);
			a6TR[c6TR] = (hA + hC + hE) - (hB + hF);
			a6ZAR[c6ZAR] = (hA + hD + hF) - (hB + hC);
			a6ZGR[c6ZGR] = (hA + hB + hE) - (hC + hD);
			
			c6BR++; c6HR++; c6NR++; c6TR++; c6ZBR++; c6ZGR++; 
			
			if (c6BR > d6B) c6BR = 0;
			if (c6HR > d6H) c6HR = 0;
			if (c6NR > d6N) c6NR = 0;
			if (c6TR > d6T) c6TR = 0;
			if (c6ZBR > d6ZB) c6ZBR = 0;
			if (c6ZGR > d6ZG) c6ZGR = 0;
			
			hA = a6BR[c6BR-((c6BR > d6B)?d6B+1:0)];
			hB = a6HR[c6HR-((c6HR > d6H)?d6H+1:0)];
			hC = a6NR[c6NR-((c6NR > d6N)?d6N+1:0)];
			hD = a6TR[c6TR-((c6TR > d6T)?d6T+1:0)];
			hE = a6ZAR[c6ZAR-((c6ZAR > d6ZA)?d6ZA+1:0)];
			hF = a6ZGR[c6ZGR-((c6ZGR > d6ZG)?d6ZG+1:0)];
			
			a6AR[c6AR] = hB + hC + hD + hE + hF;
			a6GR[c6GR] = (hA + hC + hF) - (hD + hE);
			a6MR[c6MR] = (hA + hB + hD) - (hE + hF);
			a6SR[c6SR] = (hA + hC + hE) - (hB + hF);
			a6YR[c6YR] = (hA + hD + hF) - (hB + hC);
			a6ZFR[c6ZFR] = (hA + hB + hE) - (hC + hD);
			
			c6AR++; c6GR++; c6MR++; c6SR++; c6YR++; c6ZFR++; 
			
			if (c6AR > d6A) c6AR = 0;
			if (c6GR > d6G) c6GR = 0;
			if (c6MR > d6M) c6MR = 0;
			if (c6SR > d6S) c6SR = 0;
			if (c6YR > d6Y) c6YR = 0;
			if (c6ZFR > d6ZF) c6ZFR = 0;
			
			hA = a6AR[c6AR-((c6AR > d6A)?d6A+1:0)];
			hB = a6GR[c6GR-((c6GR > d6G)?d6G+1:0)];
			hC = a6MR[c6MR-((c6MR > d6M)?d6M+1:0)];
			hD = a6SR[c6SR-((c6SR > d6S)?d6S+1:0)];
			hE = a6YR[c6YR-((c6YR > d6Y)?d6Y+1:0)];
			hF = a6ZFR[c6ZFR-((c6ZFR > d6ZF)?d6ZF+1:0)];
			
			f6AL = hB + hC + hD + hE + hF;
			f6BL = (hA + hC + hF) - (hD + hE);
			f6CL = (hA + hB + hD) - (hE + hF);
			f6DL = (hA + hC + hE) - (hB + hF);
			f6EL = (hA + hD + hF) - (hB + hC);
			f6FL = (hA + hB + hE) - (hC + hD);
			
			fhDL *= 0.25f; f6DL -= fhDL*0.125f; fhDL += f6DL;
			fhDR *= 0.25f; f6XR -= fhDR*0.125f; fhDR += f6XR;
			fhEL *= 0.25f; f6EL -= fhEL*0.125f; fhEL += f6EL;
			fhER *= 0.25f; f6ZER -= fhER*0.125f; fhER += f6ZER;
			fhFL *= 0.25f; f6FL -= fhFL*0.125f; fhFL += f6FL;
			fhFR *= 0.25f; f6ZKR -= fhFR*0.125f; fhFR += f6ZKR;
			f6BL = (f6BL+flBL)*0.5f; flBL = f6BL;
			f6LR = (f6LR+flBR)*0.5f; flBR = f6LR;
			f6AL = (f6AL+flAL)*0.5f; flAL = f6AL;
			f6FR = (f6FR+flAR)*0.5f; flAR = f6FR;
			
			inputSampleR = (hB + hC + hD + hE + hF)*0.015625;
			
			//begin just the distance filter, lowercase for inside the undersampling
			firstdryl = inputSampleL; //start by doing the interpolation
			inputSampleL += firstavgl; inputSampleL *= 0.5f; firstavgl = inputSampleL;
			inputSampleL = (firstdryl*(1.0f-distance)) + (inputSampleL*distance);
			firstdryr = inputSampleR; //start by doing the interpolation
			inputSampleR += firstavgr; inputSampleR *= 0.5f; firstavgr = inputSampleR;
			inputSampleR = (firstdryr*(1.0f-distance)) + (inputSampleR*distance);
			//having done at least one interpolation we can now do the integer number of stages
			switch (25-distanceSteps)
			{ //apply the stack of filter steps to produce the distance filter
				case  0: inputSampleL += lsz; inputSampleL *= 0.5; lsz = inputSampleL; inputSampleR += rsz; inputSampleR *= 0.5; rsz = inputSampleR;
				case  1: inputSampleL += lsy; inputSampleL *= 0.5; lsy = inputSampleL; inputSampleR += rsy; inputSampleR *= 0.5; rsy = inputSampleR;
				case  2: inputSampleL += lsx; inputSampleL *= 0.5; lsx = inputSampleL; inputSampleR += rsx; inputSampleR *= 0.5; rsx = inputSampleR;
				case  3: inputSampleL += lsw; inputSampleL *= 0.5; lsw = inputSampleL; inputSampleR += rsw; inputSampleR *= 0.5; rsw = inputSampleR;
				case  4: inputSampleL += lsv; inputSampleL *= 0.5; lsv = inputSampleL; inputSampleR += rsv; inputSampleR *= 0.5; rsv = inputSampleR;
				case  5: inputSampleL += lsu; inputSampleL *= 0.5; lsu = inputSampleL; inputSampleR += rsu; inputSampleR *= 0.5; rsu = inputSampleR;
				case  6: inputSampleL += lst; inputSampleL *= 0.5; lst = inputSampleL; inputSampleR += rst; inputSampleR *= 0.5; rst = inputSampleR;
				case  7: inputSampleL += lss; inputSampleL *= 0.5; lss = inputSampleL; inputSampleR += rss; inputSampleR *= 0.5; rss = inputSampleR;
				case  8: inputSampleL += lsr; inputSampleL *= 0.5; lsr = inputSampleL; inputSampleR += rsr; inputSampleR *= 0.5; rsr = inputSampleR;
				case  9: inputSampleL += lsq; inputSampleL *= 0.5; lsq = inputSampleL; inputSampleR += rsq; inputSampleR *= 0.5; rsq = inputSampleR;
				case 10: inputSampleL += lsp; inputSampleL *= 0.5; lsp = inputSampleL; inputSampleR += rsp; inputSampleR *= 0.5; rsp = inputSampleR;
				case 11: inputSampleL += lso; inputSampleL *= 0.5; lso = inputSampleL; inputSampleR += rso; inputSampleR *= 0.5; rso = inputSampleR;
				case 12: inputSampleL += lsn; inputSampleL *= 0.5; lsn = inputSampleL; inputSampleR += rsn; inputSampleR *= 0.5; rsn = inputSampleR;
				case 13: inputSampleL += lsm; inputSampleL *= 0.5; lsm = inputSampleL; inputSampleR += rsm; inputSampleR *= 0.5; rsm = inputSampleR;
				case 14: inputSampleL += lsl; inputSampleL *= 0.5; lsl = inputSampleL; inputSampleR += rsl; inputSampleR *= 0.5; rsl = inputSampleR;
				case 15: inputSampleL += lsk; inputSampleL *= 0.5; lsk = inputSampleL; inputSampleR += rsk; inputSampleR *= 0.5; rsk = inputSampleR;
				case 16: inputSampleL += lsj; inputSampleL *= 0.5; lsj = inputSampleL; inputSampleR += rsj; inputSampleR *= 0.5; rsj = inputSampleR;
				case 17: inputSampleL += lsi; inputSampleL *= 0.5; lsi = inputSampleL; inputSampleR += rsi; inputSampleR *= 0.5; rsi = inputSampleR;
				case 18: inputSampleL += lsh; inputSampleL *= 0.5; lsh = inputSampleL; inputSampleR += rsh; inputSampleR *= 0.5; rsh = inputSampleR;
				case 19: inputSampleL += lsg; inputSampleL *= 0.5; lsg = inputSampleL; inputSampleR += rsg; inputSampleR *= 0.5; rsg = inputSampleR;
				case 20: inputSampleL += lsf; inputSampleL *= 0.5; lsf = inputSampleL; inputSampleR += rsf; inputSampleR *= 0.5; rsf = inputSampleR;
				case 21: inputSampleL += lse; inputSampleL *= 0.5; lse = inputSampleL; inputSampleR += rse; inputSampleR *= 0.5; rse = inputSampleR;
				case 22: inputSampleL += lsd; inputSampleL *= 0.5; lsd = inputSampleL; inputSampleR += rsd; inputSampleR *= 0.5; rsd = inputSampleR;
				case 23: inputSampleL += lsc; inputSampleL *= 0.5; lsc = inputSampleL; inputSampleR += rsc; inputSampleR *= 0.5; rsc = inputSampleR;
				case 24: inputSampleL += lsb; inputSampleL *= 0.5; lsb = inputSampleL; inputSampleR += rsb; inputSampleR *= 0.5; rsb = inputSampleR;
				case 25: inputSampleL += lsa; inputSampleL *= 0.5; lsa = inputSampleL; inputSampleR += rsa; inputSampleR *= 0.5; rsa = inputSampleR;
				case 26: break;
			}
			//end distance filter
			
			inputSampleL += earlyReflectionL;
			inputSampleR += earlyReflectionR;
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double X = bez[bez_cycle];
		inputSampleL = (bez[bez_BL]+(bez[bez_CL]*(1.0-X)*(1.0-X))+(bez[bez_BL]*2.0*(1.0-X)*X)+(bez[bez_AL]*X*X))*-0.0625;
		inputSampleR = (bez[bez_BR]+(bez[bez_CR]*(1.0-X)*(1.0-X))+(bez[bez_BR]*2.0*(1.0-X)*X)+(bez[bez_AR]*X*X))*-0.0625;
		
		if (prevDistance < distanceSteps) {
			switch (prevDistance)
			{
				case  0: rsa = lsa = 0.0f; rsA = inputSampleR; lsA = inputSampleL; 
				case  1: rsb = lsb = 0.0f; rsB = inputSampleR; lsB = inputSampleL; 
				case  2: rsc = lsc = 0.0f; rsC = inputSampleR; lsC = inputSampleL;
				case  3: rsd = lsd = 0.0f; rsD = inputSampleR; lsD = inputSampleL;
				case  4: rse = lse = 0.0f; rsE = inputSampleR; lsE = inputSampleL; 
				case  5: rsf = lsf = 0.0f; rsF = inputSampleR; lsF = inputSampleL; 
				case  6: rsg = lsg = 0.0f; rsG = inputSampleR; lsG = inputSampleL; 
				case  7: rsh = lsh = 0.0f; rsH = inputSampleR; lsH = inputSampleL;
				case  8: rsi = lsi = 0.0f; rsI = inputSampleR; lsI = inputSampleL;
				case  9: rsj = lsj = 0.0f; rsJ = inputSampleR; lsJ = inputSampleL;
				case 10: rsk = lsk = 0.0f; rsK = inputSampleR; lsK = inputSampleL;
				case 11: rsl = lsl = 0.0f; rsL = inputSampleR; lsL = inputSampleL;
				case 12: rsm = lsm = 0.0f; rsM = inputSampleR; lsM = inputSampleL;
				case 13: rsn = lsn = 0.0f; rsN = inputSampleR; lsN = inputSampleL;
				case 14: rso = lso = 0.0f; rsO = inputSampleR; lsO = inputSampleL;
				case 15: rsp = lsp = 0.0f; rsP = inputSampleR; lsP = inputSampleL;
				case 16: rsq = lsq = 0.0f; rsQ = inputSampleR; lsQ = inputSampleL;
				case 17: rsr = lsr = 0.0f; rsR = inputSampleR; lsR = inputSampleL;
				case 18: rss = lss = 0.0f; rsS = inputSampleR; lsS = inputSampleL;
				case 19: rst = lst = 0.0f; rsT = inputSampleR; lsT = inputSampleL;
				case 20: rsu = lsu = 0.0f; rsU = inputSampleR; lsU = inputSampleL;
				case 21: rsv = lsv = 0.0f; rsV = inputSampleR; lsV = inputSampleL;
				case 22: rsw = lsw = 0.0f; rsW = inputSampleR; lsW = inputSampleL;
				case 23: rsx = lsx = 0.0f; rsX = inputSampleR; lsX = inputSampleL;
				case 24: rsy = lsy = 0.0f; rsY = inputSampleR; lsY = inputSampleL;
				case 25: rsz = lsz = 0.0f; rsZ = inputSampleR; lsZ = inputSampleL;
				case 26: break;
			}
		 prevDistance = distanceSteps;
		}
		
		//begin just the distance filter capitalized for outside the undersampling
		firstDryL = inputSampleL; //start by doing the interpolation
		inputSampleL += firstAvgL; inputSampleL *= 0.5f; firstAvgL = inputSampleL;
		inputSampleL = (firstDryL*(1.0f-distance)) + (inputSampleL*distance);
		firstDryR = inputSampleR; //start by doing the interpolation
		inputSampleR += firstAvgR; inputSampleR *= 0.5f; firstAvgR = inputSampleR;
		inputSampleR = (firstDryR*(1.0f-distance)) + (inputSampleR*distance);
		//having done at least one interpolation we can now do the integer number of stages				
		switch (25-distanceSteps)
		{ //apply the stack of filter steps to produce the distance filter
			case 0: inputSampleL += lsZ; inputSampleL *= 0.5; lsZ = inputSampleL; inputSampleR += rsZ; inputSampleR *= 0.5; rsZ = inputSampleR;
			case 1: inputSampleL += lsY; inputSampleL *= 0.5; lsY = inputSampleL; inputSampleR += rsY; inputSampleR *= 0.5; rsY = inputSampleR;
			case 2: inputSampleL += lsX; inputSampleL *= 0.5; lsX = inputSampleL; inputSampleR += rsX; inputSampleR *= 0.5; rsX = inputSampleR;
			case 3: inputSampleL += lsW; inputSampleL *= 0.5; lsW = inputSampleL; inputSampleR += rsW; inputSampleR *= 0.5; rsW = inputSampleR;
			case 4: inputSampleL += lsV; inputSampleL *= 0.5; lsV = inputSampleL; inputSampleR += rsV; inputSampleR *= 0.5; rsV = inputSampleR;
			case 5: inputSampleL += lsU; inputSampleL *= 0.5; lsU = inputSampleL; inputSampleR += rsU; inputSampleR *= 0.5; rsU = inputSampleR;
			case 6: inputSampleL += lsT; inputSampleL *= 0.5; lsT = inputSampleL; inputSampleR += rsT; inputSampleR *= 0.5; rsT = inputSampleR;
			case 7: inputSampleL += lsS; inputSampleL *= 0.5; lsS = inputSampleL; inputSampleR += rsS; inputSampleR *= 0.5; rsS = inputSampleR;
			case 8: inputSampleL += lsR; inputSampleL *= 0.5; lsR = inputSampleL; inputSampleR += rsR; inputSampleR *= 0.5; rsR = inputSampleR;
			case 9: inputSampleL += lsQ; inputSampleL *= 0.5; lsQ = inputSampleL; inputSampleR += rsQ; inputSampleR *= 0.5; rsQ = inputSampleR;
			case 10: inputSampleL += lsP; inputSampleL *= 0.5; lsP = inputSampleL; inputSampleR += rsP; inputSampleR *= 0.5; rsP = inputSampleR;
			case 11: inputSampleL += lsO; inputSampleL *= 0.5; lsO = inputSampleL; inputSampleR += rsO; inputSampleR *= 0.5; rsO = inputSampleR;
			case 12: inputSampleL += lsN; inputSampleL *= 0.5; lsN = inputSampleL; inputSampleR += rsN; inputSampleR *= 0.5; rsN = inputSampleR;
			case 13: inputSampleL += lsM; inputSampleL *= 0.5; lsM = inputSampleL; inputSampleR += rsM; inputSampleR *= 0.5; rsM = inputSampleR;
			case 14: inputSampleL += lsL; inputSampleL *= 0.5; lsL = inputSampleL; inputSampleR += rsL; inputSampleR *= 0.5; rsL = inputSampleR;
			case 15: inputSampleL += lsK; inputSampleL *= 0.5; lsK = inputSampleL; inputSampleR += rsK; inputSampleR *= 0.5; rsK = inputSampleR;
			case 16: inputSampleL += lsJ; inputSampleL *= 0.5; lsJ = inputSampleL; inputSampleR += rsJ; inputSampleR *= 0.5; rsJ = inputSampleR;
			case 17: inputSampleL += lsI; inputSampleL *= 0.5; lsI = inputSampleL; inputSampleR += rsI; inputSampleR *= 0.5; rsI = inputSampleR;
			case 18: inputSampleL += lsH; inputSampleL *= 0.5; lsH = inputSampleL; inputSampleR += rsH; inputSampleR *= 0.5; rsH = inputSampleR;
			case 19: inputSampleL += lsG; inputSampleL *= 0.5; lsG = inputSampleL; inputSampleR += rsG; inputSampleR *= 0.5; rsG = inputSampleR;
			case 20: inputSampleL += lsF; inputSampleL *= 0.5; lsF = inputSampleL; inputSampleR += rsF; inputSampleR *= 0.5; rsF = inputSampleR;
			case 21: inputSampleL += lsE; inputSampleL *= 0.5; lsE = inputSampleL; inputSampleR += rsE; inputSampleR *= 0.5; rsE = inputSampleR;
			case 22: inputSampleL += lsD; inputSampleL *= 0.5; lsD = inputSampleL; inputSampleR += rsD; inputSampleR *= 0.5; rsD = inputSampleR;
			case 23: inputSampleL += lsC; inputSampleL *= 0.5; lsC = inputSampleL; inputSampleR += rsC; inputSampleR *= 0.5; rsC = inputSampleR;
			case 24: inputSampleL += lsB; inputSampleL *= 0.5; lsB = inputSampleL; inputSampleR += rsB; inputSampleR *= 0.5; rsB = inputSampleR;
			case 25: inputSampleL += lsA; inputSampleL *= 0.5; lsA = inputSampleL; inputSampleR += rsA; inputSampleR *= 0.5; rsA = inputSampleR;
			case 26: break;
		}
		//end distance filter
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
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