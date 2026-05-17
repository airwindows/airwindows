/*
*	File:		kCyberCity.cpp
*	
*	Version:	1.0
* 
*	Created:	5/11/26
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
	kCyberCity.cpp
	
=============================================================================*/
#include "kCyberCity.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, kCyberCity)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kCyberCity::kCyberCity
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
kCyberCity::kCyberCity(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kCyberCity::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kCyberCity::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kCyberCity::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kCyberCity::GetParameterInfo(AudioUnitScope		inScope,
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
//	kCyberCity::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kCyberCity::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 kCyberCity::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	kCyberCity::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kCyberCity::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	kCyberCity::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult kCyberCity::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____kCyberCityEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kCyberCity::kCyberCityKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		kCyberCity::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
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
	avg6L = avg6R = 0.0;	
	
	for (int x = 0; x < bez_total; x++) {
		bez[x] = 0.0;
	}
	bez[bez_cycle] = 1.0;
	
	for (int x = 0; x < 33; x++) {avg32L[x] = 0.0; post32L[x] = 0.0; avg32R[x] = 0.0; post32R[x] = 0.0;}
	for (int x = 0; x < 17; x++) {avg16L[x] = 0.0; post16L[x] = 0.0; avg16R[x] = 0.0; post16R[x] = 0.0;}
	for (int x = 0; x < 9; x++) {avg8L[x] = 0.0; post8L[x] = 0.0; avg8R[x] = 0.0; post8R[x] = 0.0;}
	for (int x = 0; x < 5; x++) {avg4L[x] = 0.0; post4L[x] = 0.0; avg4R[x] = 0.0; post4R[x] = 0.0;}
	for (int x = 0; x < 3; x++) {avg2L[x] = 0.0; post2L[x] = 0.0; avg2R[x] = 0.0; post2R[x] = 0.0;}
	avgPos = 0;
	lastDarkL = 0.0; lastDarkR = 0.0;
	//preTapeHack
	
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
//	kCyberCity::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		kCyberCity::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	int slewsing = floor(overallscale*2.0);
	if (slewsing < 2) slewsing = 2; if (slewsing > 32) slewsing = 32;	
	double reg6n = (1.0-pow(1.0-GetParameter( kParam_A ),3.0))*0.0013425;
	double regenMax = 0.0013425;
	double derez = 1.0;
	derez = fmin(fmax(derez/overallscale,0.0005),1.0);
	int bezFraction = (int)(1.0/derez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	derez = 1.0 / bezFraction;
	bezTrim = 1.0-(derez*bezTrim);
	
	int start = (int)(GetParameter( kParam_B ) * 27.0);
	double wet = GetParameter( kParam_C );
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += (inputSampleL * derez);
		bez[bez_SampR] += (inputSampleR * derez);
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
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
			
			c3AL++; if (c3AL > ld3A) c3AL = 0;
			c3BL++; if (c3BL > ld3B) c3BL = 0;
			c3CL++; if (c3CL > ld3C) c3CL = 0;
			c3CR++; if (c3CR > ld3C) c3CR = 0;
			c3FR++; if (c3FR > ld3F) c3FR = 0;
			c3IR++; if (c3IR > ld3I) c3IR = 0;
			
			double hA = a3AL[c3AL-((c3AL > ld3A)?c3AL+1:0)];
			double hB = a3BL[c3BL-((c3BL > ld3B)?c3BL+1:0)];
			double hC = a3CL[c3CL-((c3CL > ld3C)?c3CL+1:0)];
			double hD = a3CR[c3CR-((c3CR > ld3C)?c3CR+1:0)];
			double hE = a3FR[c3FR-((c3FR > ld3F)?c3FR+1:0)];
			double hF = a3IR[c3IR-((c3IR > ld3I)?c3IR+1:0)];
			
			a3DL[c3DL] = (((hB + hC) * -2.0) + hA);
			a3EL[c3EL] = (((hA + hC) * -2.0) + hB);
			a3FL[c3FL] = (((hA + hB) * -2.0) + hC);
			a3BR[c3BR] = (((hE + hF) * -2.0) + hD);
			a3ER[c3ER] = (((hD + hF) * -2.0) + hE);
			a3HR[c3HR] = (((hD + hE) * -2.0) + hF);
			
			c3DL++; if (c3DL > ld3D) c3DL = 0;
			c3EL++; if (c3EL > ld3E) c3EL = 0;
			c3FL++; if (c3FL > ld3F) c3FL = 0;
			c3BR++; if (c3BR > ld3B) c3BR = 0;
			c3ER++; if (c3ER > ld3E) c3ER = 0;
			c3HR++; if (c3HR > ld3H) c3HR = 0;
			
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
			
			c3GL++; if (c3GL > ld3G) c3GL = 0;
			c3HL++; if (c3HL > ld3H) c3HL = 0;
			c3IL++; if (c3IL > ld3I) c3IL = 0;
			c3AR++; if (c3AR > ld3A) c3AR = 0;
			c3DR++; if (c3DR > ld3D) c3DR = 0;
			c3GR++; if (c3GR > ld3G) c3GR = 0;
			
			hA = a3GL[c3GL-((c3GL > ld3G)?c3GL+1:0)];
			hB = a3HL[c3HL-((c3HL > ld3H)?c3HL+1:0)];
			hC = a3IL[c3IL-((c3IL > ld3I)?c3IL+1:0)];
			hD = a3AR[c3AR-((c3AR > ld3A)?c3AR+1:0)];
			hE = a3DR[c3DR-((c3DR > ld3D)?c3DR+1:0)];
			hF = a3GR[c3GR-((c3GR > ld3G)?c3GR+1:0)];
			
			double earlyReflectionL = (((hB + hC) * -2.0) + hA)*-0.0625;
			double earlyReflectionR = (((hE + hF) * -2.0) + hD)*-0.0625;
			
			double applyNonlin = 1.0-fmin(fmax(fabs(inputSampleL),fabs(inputSampleR)),1.0);
			applyNonlin = 1.0-(applyNonlin*applyNonlin*applyNonlin);
			
			inputSampleL -= earlyReflectionL;
			inputSampleR -= earlyReflectionR;
			
			roomTimerL++;
			if (roomTimerL > 19) {
				roomTimerL = 0;
				if (roomNoiseL > 0.0) roomNoiseL -= fabs((fpdL / (double)UINT32_MAX)-0.5)*0.08625;
				else roomNoiseL += fabs((fpdL / (double)UINT32_MAX)-0.5)*0.08625;
			} else roomNoiseL += ((fpdL / (double)UINT32_MAX)-0.5)*0.08625;
			//roomNoiseL governs the added 'room tone' noise. 0.125 gives you not an overwhelming
			//volume of it, less will be quieter. The 13 is part of VoiceOfTheStarship, and
			//larger allows more subs into the noise: it'll determine the apparent room scale,
			//with subs content implying a HUGE space, no lows implying a small space.
			
			roomTimerR++;
			if (roomTimerR > 7) {
				roomTimerR = 0;
				if (roomNoiseR > fmin(reg6n+(applyNonlin*0.00058),regenMax)) roomNoiseR -= fabs((fpdR / (double)UINT32_MAX)-0.5)*0.000021;
				else roomNoiseR += fabs((fpdR / (double)UINT32_MAX)-0.5)*0.000021;
			} else roomNoiseR += ((fpdR / (double)UINT32_MAX)-0.5)*0.000021;
			//roomNoiseR governs the activity of the air in this space. Consider it in terms of
			//winds at MPH: the RoomTimer number is (int)sqrt(wind MPH) which governs the intensity
			//of gusts and fluctuations (avoid hurricane force inside realistic rooms)
			//and the scaling of roomNoiseR (as added to reg6n) is sqrt(wind MPH)*0.00002
			//but check against pure tones, don't let it dirty them up too much!
			
			a6AL[c6AL] = inputSampleL + ((f6BL+roomNoiseL) * roomNoiseR);
			a6BL[c6BL] = inputSampleL + (f6CL * roomNoiseR);
			a6CL[c6CL] = inputSampleL + (f6DL * roomNoiseR);
			a6DL[c6DL] = inputSampleL + (f6EL * roomNoiseR);
			a6EL[c6EL] = inputSampleL + (f6FL * roomNoiseR);
			a6FL[c6FL] = inputSampleL + (f6AL * roomNoiseR);
			
			a6FR[c6FR] = inputSampleR + ((f6LR+roomNoiseL) * roomNoiseR);
			a6LR[c6LR] = inputSampleR + (f6RR * roomNoiseR);
			a6RR[c6RR] = inputSampleR + (f6XR * roomNoiseR);
			a6XR[c6XR] = inputSampleR + (f6ZER * roomNoiseR);
			a6ZER[c6ZER] = inputSampleR + (f6ZKR * roomNoiseR);
			a6ZKR[c6ZKR] = inputSampleR + (f6FR * roomNoiseR);
			
			//left verb
			
			c6AL++; if (c6AL > d6A) c6AL = 0;
			c6BL++; if (c6BL > d6B) c6BL = 0;
			c6CL++; if (c6CL > d6C) c6CL = 0;
			c6DL++; if (c6DL > d6D) c6DL = 0;
			c6EL++; if (c6EL > d6E) c6EL = 0;
			c6FL++; if (c6FL > d6F) c6FL = 0;
			
			hA = a6AL[c6AL-((c6AL > d6A)?d6A+1:0)];
			hB = a6BL[c6BL-((c6BL > d6B)?d6B+1:0)];
			hC = a6CL[c6CL-((c6CL > d6C)?d6C+1:0)];
			hD = a6DL[c6DL-((c6DL > d6D)?d6D+1:0)];
			hE = a6EL[c6EL-((c6EL > d6E)?d6E+1:0)];
			hF = a6FL[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6GL[c6GL] = ((hA*2.0) - (hB + hC + hD + hE + hF));
			a6HL[c6HL] = ((hB*2.0) - (hA + hC + hD + hE + hF));
			a6IL[c6IL] = ((hC*2.0) - (hA + hB + hD + hE + hF));
			a6JL[c6JL] = ((hD*2.0) - (hA + hB + hC + hE + hF));
			a6KL[c6KL] = ((hE*2.0) - (hA + hB + hC + hD + hF));
			a6LL[c6LL] = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			c6GL++; if (c6GL > d6G) c6GL = 0;
			c6HL++; if (c6HL > d6H) c6HL = 0;
			c6IL++; if (c6IL > d6I) c6IL = 0;
			c6JL++; if (c6JL > d6J) c6JL = 0;
			c6KL++; if (c6KL > d6K) c6KL = 0;
			c6LL++; if (c6LL > d6L) c6LL = 0;
			
			hA = a6GL[c6GL-((c6GL > d6G)?d6G+1:0)];
			hB = a6HL[c6HL-((c6HL > d6H)?d6H+1:0)];
			hC = a6IL[c6IL-((c6IL > d6I)?d6I+1:0)];
			hD = a6JL[c6JL-((c6JL > d6J)?d6J+1:0)];
			hE = a6KL[c6KL-((c6KL > d6K)?d6K+1:0)];
			hF = a6LL[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6ML[c6ML] = ((hA*2.0) - (hB + hC + hD + hE + hF));
			a6NL[c6NL] = ((hB*2.0) - (hA + hC + hD + hE + hF));
			a6OL[c6OL] = ((hC*2.0) - (hA + hB + hD + hE + hF));
			a6PL[c6PL] = ((hD*2.0) - (hA + hB + hC + hE + hF));
			a6QL[c6QL] = ((hE*2.0) - (hA + hB + hC + hD + hF));
			a6RL[c6RL] = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			c6ML++; if (c6ML > d6M) c6ML = 0;
			c6NL++; if (c6NL > d6N) c6NL = 0;
			c6OL++; if (c6OL > d6O) c6OL = 0;
			c6PL++; if (c6PL > d6P) c6PL = 0;
			c6QL++; if (c6QL > d6Q) c6QL = 0;
			c6RL++; if (c6RL > d6R) c6RL = 0;
			
			hA = a6ML[c6ML-((c6ML > d6M)?d6M+1:0)];
			hB = a6NL[c6NL-((c6NL > d6N)?d6N+1:0)];
			hC = a6OL[c6OL-((c6OL > d6O)?d6O+1:0)];
			hD = a6PL[c6PL-((c6PL > d6P)?d6P+1:0)];
			hE = a6QL[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			hF = a6RL[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6SL[c6SL] = ((hA*2.0) - (hB + hC + hD + hE + hF));
			a6TL[c6TL] = ((hB*2.0) - (hA + hC + hD + hE + hF));
			a6UL[c6UL] = ((hC*2.0) - (hA + hB + hD + hE + hF));
			a6VL[c6VL] = ((hD*2.0) - (hA + hB + hC + hE + hF));
			a6WL[c6WL] = ((hE*2.0) - (hA + hB + hC + hD + hF));
			a6XL[c6XL] = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			c6SL++; if (c6SL > d6S) c6SL = 0;
			c6TL++; if (c6TL > d6T) c6TL = 0;
			c6UL++; if (c6UL > d6U) c6UL = 0;
			c6VL++; if (c6VL > d6V) c6VL = 0;
			c6WL++; if (c6WL > d6W) c6WL = 0;
			c6XL++; if (c6XL > d6X) c6XL = 0;
			
			hA = a6SL[c6SL-((c6SL > d6S)?d6S+1:0)];
			hB = a6TL[c6TL-((c6TL > d6T)?d6T+1:0)];
			hC = a6UL[c6UL-((c6UL > d6U)?d6U+1:0)];
			hD = a6VL[c6VL-((c6VL > d6V)?d6V+1:0)];
			hE = a6WL[c6WL-((c6WL > d6W)?d6W+1:0)];
			hF = a6XL[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6YL[c6YL] = ((hA*2.0) - (hB + hC + hD + hE + hF));
			a6ZAL[c6ZAL] = ((hB*2.0) - (hA + hC + hD + hE + hF));
			a6ZBL[c6ZBL] = ((hC*2.0) - (hA + hB + hD + hE + hF));
			a6ZCL[c6ZCL] = ((hD*2.0) - (hA + hB + hC + hE + hF));
			a6ZDL[c6ZDL] = ((hE*2.0) - (hA + hB + hC + hD + hF));
			a6ZEL[c6ZEL] = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			c6YL++; if (c6YL > d6Y) c6YL = 0;
			c6ZAL++; if (c6ZAL > d6ZA) c6ZAL = 0;
			c6ZBL++; if (c6ZBL > d6ZB) c6ZBL = 0;
			c6ZCL++; if (c6ZCL > d6ZC) c6ZCL = 0;
			c6ZDL++; if (c6ZDL > d6ZD) c6ZDL = 0;
			c6ZEL++; if (c6ZEL > d6ZE) c6ZEL = 0;
			
			hA = a6YL[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			hB = a6ZAL[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			hC = a6ZBL[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			hD = a6ZCL[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			hE = a6ZDL[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			hF = a6ZEL[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6ZFL[c6ZFL] = ((hA*2.0) - (hB + hC + hD + hE + hF));
			a6ZGL[c6ZGL] = ((hB*2.0) - (hA + hC + hD + hE + hF));
			a6ZHL[c6ZHL] = ((hC*2.0) - (hA + hB + hD + hE + hF));
			a6ZIL[c6ZIL] = ((hD*2.0) - (hA + hB + hC + hE + hF));
			a6ZJL[c6ZJL] = ((hE*2.0) - (hA + hB + hC + hD + hF));
			a6ZKL[c6ZKL] = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			c6ZFL++; if (c6ZFL > d6ZF) c6ZFL = 0;
			c6ZGL++; if (c6ZGL > d6ZG) c6ZGL = 0;
			c6ZHL++; if (c6ZHL > d6ZH) c6ZHL = 0;
			c6ZIL++; if (c6ZIL > d6ZI) c6ZIL = 0;
			c6ZJL++; if (c6ZJL > d6ZJ) c6ZJL = 0;
			c6ZKL++; if (c6ZKL > d6ZK) c6ZKL = 0;
			
			hA = a6ZFL[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			hB = a6ZGL[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			hC = a6ZHL[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			hD = a6ZIL[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			hE = a6ZJL[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			hF = a6ZKL[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			f6FR = (((hA*2.0) - (hB + hC + hD + hE + hF))+avg6R)*0.5;
			avg6R = ((hA*2.0) - (hB + hC + hD + hE + hF));
			f6LR = ((hB*2.0) - (hA + hC + hD + hE + hF));
			f6RR = ((hC*2.0) - (hA + hB + hD + hE + hF));
			f6XR = ((hD*2.0) - (hA + hB + hC + hE + hF));
			f6ZER = ((hE*2.0) - (hA + hB + hC + hD + hF));
			f6ZKR = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			inputSampleL = ((hA*2.0) - (hB + hC + hD + hE + hF))*0.001953125;
			
			//right verb
			
			c6FR++; if (c6FR > d6F) c6FR = 0;
			c6LR++; if (c6LR > d6L) c6LR = 0;
			c6RR++; if (c6RR > d6R) c6RR = 0;
			c6XR++; if (c6XR > d6X) c6XR = 0;
			c6ZER++; if (c6ZER > d6ZE) c6ZER = 0;
			c6ZKR++; if (c6ZKR > d6ZK) c6ZKR = 0;
			
			hA = a6FR[c6FR-((c6FR > d6F)?d6F+1:0)];
			hB = a6LR[c6LR-((c6LR > d6L)?d6L+1:0)];
			hC = a6RR[c6RR-((c6RR > d6R)?d6R+1:0)];
			hD = a6XR[c6XR-((c6XR > d6X)?d6X+1:0)];
			hE = a6ZER[c6ZER-((c6ZER > d6ZE)?d6ZE+1:0)];
			hF = a6ZKR[c6ZKR-((c6ZKR > d6ZK)?d6ZK+1:0)];
			
			a6ER[c6ER] = ((hA*2.0) - (hB + hC + hD + hE + hF));
			a6KR[c6KR] = ((hB*2.0) - (hA + hC + hD + hE + hF));
			a6QR[c6QR] = ((hC*2.0) - (hA + hB + hD + hE + hF));
			a6WR[c6WR] = ((hD*2.0) - (hA + hB + hC + hE + hF));
			a6ZDR[c6ZDR] = ((hE*2.0) - (hA + hB + hC + hD + hF));			
			a6ZJR[c6ZJR] = ((hF*2.0) - (hA + hB + hC + hD + hE));			
			
			c6ER++; if (c6ER > d6E) c6ER = 0;
			c6KR++; if (c6KR > d6K) c6KR = 0;
			c6QR++; if (c6QR > d6Q) c6QR = 0;
			c6WR++; if (c6WR > d6W) c6WR = 0;
			c6ZDR++; if (c6ZDR > d6ZD) c6ZDR = 0;
			c6ZJR++; if (c6ZJR > d6ZJ) c6ZJR = 0;
			
			hA = a6ER[c6ER-((c6ER > d6E)?d6E+1:0)];
			hB = a6KR[c6KR-((c6KR > d6K)?d6K+1:0)];
			hC = a6QR[c6QR-((c6QR > d6Q)?d6Q+1:0)];
			hD = a6WR[c6WR-((c6WR > d6W)?d6W+1:0)];
			hE = a6ZDR[c6ZDR-((c6ZDR > d6ZD)?d6ZD+1:0)];
			hF = a6ZJR[c6ZJR-((c6ZJR > d6ZJ)?d6ZJ+1:0)];
			
			a6DR[c6DR] = ((hA*2.0) - (hB + hC + hD + hE + hF));
			a6JR[c6JR] = ((hB*2.0) - (hA + hC + hD + hE + hF));
			a6PR[c6PR] = ((hC*2.0) - (hA + hB + hD + hE + hF));
			a6VR[c6VR] = ((hD*2.0) - (hA + hB + hC + hE + hF));
			a6ZCR[c6ZCR] = ((hE*2.0) - (hA + hB + hC + hD + hF));
			a6ZIR[c6ZIR] = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			c6DR++; if (c6DR > d6D) c6DR = 0;
			c6JR++; if (c6JR > d6J) c6JR = 0;
			c6PR++; if (c6PR > d6P) c6PR = 0;
			c6VR++; if (c6VR > d6V) c6VR = 0;
			c6ZCR++; if (c6ZCR > d6ZC) c6ZCR = 0;
			c6ZIR++; if (c6ZIR > d6ZI) c6ZIR = 0;
			
			hA = a6DR[c6DR-((c6DR > d6D)?d6D+1:0)];
			hB = a6JR[c6JR-((c6JR > d6J)?d6J+1:0)];
			hC = a6PR[c6PR-((c6PR > d6P)?d6P+1:0)];
			hD = a6VR[c6VR-((c6VR > d6V)?d6V+1:0)];
			hE = a6ZCR[c6ZCR-((c6ZCR > d6ZC)?d6ZC+1:0)];
			hF = a6ZIR[c6ZIR-((c6ZIR > d6ZI)?d6ZI+1:0)];
			
			a6CR[c6CR] = ((hA*2.0) - (hB + hC + hD + hE + hF));
			a6IR[c6IR] = ((hB*2.0) - (hA + hC + hD + hE + hF));
			a6OR[c6OR] = ((hC*2.0) - (hA + hB + hD + hE + hF));
			a6UR[c6UR] = ((hD*2.0) - (hA + hB + hC + hE + hF));
			a6ZBR[c6ZBR] = ((hE*2.0) - (hA + hB + hC + hD + hF));
			a6ZHR[c6ZHR] = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			c6CR++; if (c6CR > d6C) c6CR = 0;
			c6IR++; if (c6IR > d6I) c6IR = 0;
			c6OR++; if (c6OR > d6O) c6OR = 0;
			c6UR++; if (c6UR > d6U) c6UR = 0;
			c6ZBR++; if (c6ZBR > d6ZB) c6ZBR = 0;
			c6ZHR++; if (c6ZHR > d6ZH) c6ZHR = 0;
			
			hA = a6CR[c6CR-((c6CR > d6C)?d6C+1:0)];
			hB = a6IR[c6IR-((c6IR > d6I)?d6I+1:0)];
			hC = a6OR[c6OR-((c6OR > d6O)?d6O+1:0)];
			hD = a6UR[c6UR-((c6UR > d6U)?d6U+1:0)];
			hE = a6ZBR[c6ZBR-((c6ZBR > d6ZB)?d6ZB+1:0)];
			hF = a6ZHR[c6ZHR-((c6ZHR > d6ZH)?d6ZH+1:0)];
			
			a6BR[c6BR] = ((hA*2.0) - (hB + hC + hD + hE + hF));
			a6HR[c6HR] = ((hB*2.0) - (hA + hC + hD + hE + hF));
			a6NR[c6NR] = ((hC*2.0) - (hA + hB + hD + hE + hF));
			a6TR[c6TR] = ((hD*2.0) - (hA + hB + hC + hE + hF));
			a6ZAR[c6ZAR] = ((hE*2.0) - (hA + hB + hC + hD + hF));
			a6ZGR[c6ZGR] = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			c6BR++; if (c6BR > d6B) c6BR = 0;
			c6HR++; if (c6HR > d6H) c6HR = 0;
			c6NR++; if (c6NR > d6N) c6NR = 0;
			c6TR++; if (c6TR > d6T) c6TR = 0;
			c6ZBR++; if (c6ZBR > d6ZB) c6ZBR = 0;
			c6ZGR++; if (c6ZGR > d6ZG) c6ZGR = 0;
			
			hA = a6BR[c6BR-((c6BR > d6B)?d6B+1:0)];
			hB = a6HR[c6HR-((c6HR > d6H)?d6H+1:0)];
			hC = a6NR[c6NR-((c6NR > d6N)?d6N+1:0)];
			hD = a6TR[c6TR-((c6TR > d6T)?d6T+1:0)];
			hE = a6ZAR[c6ZAR-((c6ZAR > d6ZA)?d6ZA+1:0)];
			hF = a6ZGR[c6ZGR-((c6ZGR > d6ZG)?d6ZG+1:0)];
			
			a6AR[c6AR] = ((hA*2.0) - (hB + hC + hD + hE + hF));
			a6GR[c6GR] = ((hB*2.0) - (hA + hC + hD + hE + hF));
			a6MR[c6MR] = ((hC*2.0) - (hA + hB + hD + hE + hF));
			a6SR[c6SR] = ((hD*2.0) - (hA + hB + hC + hE + hF));
			a6YR[c6YR] = ((hE*2.0) - (hA + hB + hC + hD + hF));
			a6ZFR[c6ZFR] = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			c6AR++; if (c6AR > d6A) c6AR = 0;
			c6GR++; if (c6GR > d6G) c6GR = 0;
			c6MR++; if (c6MR > d6M) c6MR = 0;
			c6SR++; if (c6SR > d6S) c6SR = 0;
			c6YR++; if (c6YR > d6Y) c6YR = 0;
			c6ZFR++; if (c6ZFR > d6ZF) c6ZFR = 0;
			
			hA = a6AR[c6AR-((c6AR > d6A)?d6A+1:0)];
			hB = a6GR[c6GR-((c6GR > d6G)?d6G+1:0)];
			hC = a6MR[c6MR-((c6MR > d6M)?d6M+1:0)];
			hD = a6SR[c6SR-((c6SR > d6S)?d6S+1:0)];
			hE = a6YR[c6YR-((c6YR > d6Y)?d6Y+1:0)];
			hF = a6ZFR[c6ZFR-((c6ZFR > d6ZF)?d6ZF+1:0)];
			
			f6AL = (((hA*2.0) - (hB + hC + hD + hE + hF))+avg6L)*0.5;
			avg6L = ((hA*2.0) - (hB + hC + hD + hE + hF));
			f6BL = ((hB*2.0) - (hA + hC + hD + hE + hF));
			f6CL = ((hC*2.0) - (hA + hB + hD + hE + hF));
			f6DL = ((hD*2.0) - (hA + hB + hC + hE + hF));
			f6EL = ((hE*2.0) - (hA + hB + hC + hD + hF));
			f6FL = ((hF*2.0) - (hA + hB + hC + hD + hE));
			
			inputSampleR = ((hA*2.0) - (hB + hC + hD + hE + hF))*0.001953125;
			
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
		double X = bez[bez_cycle]*bezTrim;
		inputSampleL = (bez[bez_BL]+(bez[bez_CL]*(1.0-X)*(1.0-X))+(bez[bez_BL]*2.0*(1.0-X)*X)+(bez[bez_AL]*X*X))*-0.0625;
		inputSampleR = (bez[bez_BR]+(bez[bez_CR]*(1.0-X)*(1.0-X))+(bez[bez_BR]*2.0*(1.0-X)*X)+(bez[bez_AR]*X*X))*-0.0625;
		
		//begin TapeHack2
		double darkSampleL = inputSampleL;
		double darkSampleR = inputSampleR;
		if (avgPos > 31) avgPos = 0;
		if (slewsing > 31) {
			avg32L[avgPos] = darkSampleL; avg32R[avgPos] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 32; x++) {darkSampleL += avg32L[x]; darkSampleR += avg32R[x];}
			darkSampleL /= 32.0; darkSampleR /= 32.0;
		} if (slewsing > 15) {
			avg16L[avgPos%16] = darkSampleL; avg16R[avgPos%16] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 16; x++) {darkSampleL += avg16L[x]; darkSampleR += avg16R[x];}
			darkSampleL /= 16.0; darkSampleR /= 16.0;
		} if (slewsing > 7) {
			avg8L[avgPos%8] = darkSampleL; avg8R[avgPos%8] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 8; x++) {darkSampleL += avg8L[x]; darkSampleR += avg8R[x];}
			darkSampleL /= 8.0; darkSampleR /= 8.0;
		} if (slewsing > 3) {
			avg4L[avgPos%4] = darkSampleL; avg4R[avgPos%4] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 4; x++) {darkSampleL += avg4L[x]; darkSampleR += avg4R[x];}
			darkSampleL /= 4.0; darkSampleR /= 4.0;
		} if (slewsing > 1) {
			avg2L[avgPos%2] = darkSampleL; avg2R[avgPos%2] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 2; x++) {darkSampleL += avg2L[x]; darkSampleR += avg2R[x];}
			darkSampleL /= 2.0; darkSampleR /= 2.0; 
		} //only update avgPos after the post-distortion filter stage
		double avgSlewL = fmin(fabs(lastDarkL-inputSampleL)*0.09384*overallscale,1.0);
		avgSlewL = 1.0-(1.0-avgSlewL*1.0-avgSlewL);
		inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
		lastDarkL = darkSampleL;
		double avgSlewR = fmin(fabs(lastDarkR-inputSampleR)*0.09384*overallscale,1.0);
		avgSlewR = 1.0-(1.0-avgSlewR*1.0-avgSlewR);
		inputSampleR = (inputSampleR*(1.0-avgSlewR)) + (darkSampleR*avgSlewR);
		lastDarkR = darkSampleR;
		
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