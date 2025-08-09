/*
*	File:		kGuitarHall2.cpp
*	
*	Version:	1.0
* 
*	Created:	6/30/25
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
	kGuitarHall2.cpp
	
=============================================================================*/
#include "kGuitarHall2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(kGuitarHall2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kGuitarHall2::kGuitarHall2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
kGuitarHall2::kGuitarHall2(AudioUnit component)
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
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kGuitarHall2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kGuitarHall2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kGuitarHall2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kGuitarHall2::GetParameterInfo(AudioUnitScope		inScope,
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
            case kParam_F:
                AUBase::FillInParameterName (outParameterInfo, kParameterFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamF;
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
//	kGuitarHall2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kGuitarHall2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 kGuitarHall2::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	kGuitarHall2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			kGuitarHall2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	kGuitarHall2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult kGuitarHall2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____kGuitarHall2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kGuitarHall2::kGuitarHall2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		kGuitarHall2::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
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
		
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	
	for (int x = 0; x < pear_total; x++) pear[x] = 0.0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	kGuitarHall2::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		kGuitarHall2::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	
	double fdb6ck = (0.0009765625+0.0009765625+0.001953125)*0.3333333;
	double reg6n = (1.0-pow(1.0-GetParameter( kParam_A ),3.0))*fdb6ck;
	
	double derez = GetParameter( kParam_B )*2.0;
	bool stepped = true; // Revised Bezier Undersampling
	if (derez > 1.0) {  // has full rez at center, stepped
		stepped = false; // to left, continuous to right
		derez = 1.0-(derez-1.0);
	} //if it's set up like that it's the revised algorithm
	derez = fmin(fmax(derez/overallscale,0.0005),1.0);
	int bezFraction = (int)(1.0/derez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	if (stepped) { //this hard-locks derez to exact subdivisions of 1.0
		derez = 1.0 / bezFraction;
		bezTrim = 1.0-(derez*bezTrim);
	} else { //this makes it match the 1.0 case using stepped
		derez /= (2.0/pow(overallscale,0.5-((overallscale-1.0)*0.0375)));
		bezTrim = 1.0-pow(derez*0.5,1.0/(derez*0.5));
	} //the revision more accurately connects the bezier curves
	
	double freq = GetParameter( kParam_C )+0.02;
	double earlyLoudness = GetParameter( kParam_D );
	int start = (int)(GetParameter( kParam_E ) * 27.0);
	int ld3G = early[start]; 
	int ld3H = early[start+1]; 
	int ld3D = early[start+2]; 
	int ld3A = early[start+3]; 
	int ld3E = early[start+4]; 
	int ld3I = early[start+5];
	int ld3F = early[start+6];
	int ld3B = early[start+7]; 
	int ld3C = early[start+8];
	double wet = GetParameter( kParam_F );
		
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += ((inputSampleL+bez[bez_InL]) * derez);
		bez[bez_SampR] += ((inputSampleR+bez[bez_InR]) * derez);
		bez[bez_InL] = inputSampleL; bez[bez_InR] = inputSampleR;
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			if (stepped) bez[bez_cycle] = 0.0;
			else bez[bez_cycle] -= 1.0;
						
			inputSampleL = (bez[bez_SampL]+bez[bez_AvgInSampL])*0.5;
			bez[bez_AvgInSampL] = bez[bez_SampL];
			inputSampleR = (bez[bez_SampR]+bez[bez_AvgInSampR])*0.5;
			bez[bez_AvgInSampR] = bez[bez_SampR];
			
			a3AL[c3AL] = inputSampleL;// + (f3AL * reg3n);
			a3BL[c3BL] = inputSampleL;// + (f3BL * reg3n);
			a3CL[c3CL] = inputSampleL;// + (f3CL * reg3n);
			
			a3CR[c3CR] = inputSampleR;// + (f3CR * reg3n);
			a3FR[c3FR] = inputSampleR;// + (f3FR * reg3n);
			a3IR[c3IR] = inputSampleR;// + (f3IR * reg3n);
			
			c3AL++; if (c3AL < 0 || c3AL > ld3A) c3AL = 0;
			c3BL++; if (c3BL < 0 || c3BL > ld3B) c3BL = 0;
			c3CL++; if (c3CL < 0 || c3CL > ld3C) c3CL = 0;
			c3CR++; if (c3CR < 0 || c3CR > ld3C) c3CR = 0;
			c3FR++; if (c3FR < 0 || c3FR > ld3F) c3FR = 0;
			c3IR++; if (c3IR < 0 || c3IR > ld3I) c3IR = 0;
			
			double o3AL = a3AL[c3AL-((c3AL > ld3A)?c3AL+1:0)];
			double o3BL = a3BL[c3BL-((c3BL > ld3B)?c3BL+1:0)];
			double o3CL = a3CL[c3CL-((c3CL > ld3C)?c3CL+1:0)];
			double o3CR = a3CR[c3CR-((c3CR > ld3C)?c3CR+1:0)];
			double o3FR = a3FR[c3FR-((c3FR > ld3F)?c3FR+1:0)];
			double o3IR = a3IR[c3IR-((c3IR > ld3I)?c3IR+1:0)];
			
			a3DL[c3DL] = (((o3BL + o3CL) * -2.0) + o3AL);
			a3EL[c3EL] = (((o3AL + o3CL) * -2.0) + o3BL);
			a3FL[c3FL] = (((o3AL + o3BL) * -2.0) + o3CL);
			a3BR[c3BR] = (((o3FR + o3IR) * -2.0) + o3CR);
			a3ER[c3ER] = (((o3CR + o3IR) * -2.0) + o3FR);
			a3HR[c3HR] = (((o3CR + o3FR) * -2.0) + o3IR);
			
			c3DL++; if (c3DL < 0 || c3DL > ld3D) c3DL = 0;
			c3EL++; if (c3EL < 0 || c3EL > ld3E) c3EL = 0;
			c3FL++; if (c3FL < 0 || c3FL > ld3F) c3FL = 0;
			c3BR++; if (c3BR < 0 || c3BR > ld3B) c3BR = 0;
			c3ER++; if (c3ER < 0 || c3ER > ld3E) c3ER = 0;
			c3HR++; if (c3HR < 0 || c3HR > ld3H) c3HR = 0;
			
			double o3DL = a3DL[c3DL-((c3DL > ld3D)?c3DL+1:0)];
			double o3EL = a3EL[c3EL-((c3EL > ld3E)?c3EL+1:0)];
			double o3FL = a3FL[c3FL-((c3FL > ld3F)?c3FL+1:0)];
			double o3BR = a3BR[c3BR-((c3BR > ld3B)?c3BR+1:0)];
			double o3ER = a3ER[c3ER-((c3ER > ld3E)?c3ER+1:0)];
			double o3HR = a3HR[c3HR-((c3HR > ld3H)?c3HR+1:0)];
			
			a3GL[c3GL] = (((o3EL + o3FL) * -2.0) + o3DL);
			a3HL[c3HL] = (((o3DL + o3FL) * -2.0) + o3EL);
			a3IL[c3IL] = (((o3DL + o3EL) * -2.0) + o3FL);
			a3AR[c3AR] = (((o3ER + o3HR) * -2.0) + o3BR);
			a3DR[c3DR] = (((o3BR + o3HR) * -2.0) + o3ER);
			a3GR[c3GR] = (((o3BR + o3ER) * -2.0) + o3HR);
			
			c3GL++; if (c3GL < 0 || c3GL > ld3G) c3GL = 0;
			c3HL++; if (c3HL < 0 || c3HL > ld3H) c3HL = 0;
			c3IL++; if (c3IL < 0 || c3IL > ld3I) c3IL = 0;
			c3AR++; if (c3AR < 0 || c3AR > ld3A) c3AR = 0;
			c3DR++; if (c3DR < 0 || c3DR > ld3D) c3DR = 0;
			c3GR++; if (c3GR < 0 || c3GR > ld3G) c3GR = 0;
			
			double o3GL = a3GL[c3GL-((c3GL > ld3G)?c3GL+1:0)];
			double o3HL = a3HL[c3HL-((c3HL > ld3H)?c3HL+1:0)];
			double o3IL = a3IL[c3IL-((c3IL > ld3I)?c3IL+1:0)];
			double o3AR = a3AR[c3AR-((c3AR > ld3A)?c3AR+1:0)];
			double o3DR = a3DR[c3DR-((c3DR > ld3D)?c3DR+1:0)];
			double o3GR = a3GR[c3GR-((c3GR > ld3G)?c3GR+1:0)];
			
			double inputSampleL = (o3GL + o3HL + o3IL)*0.03125;
			double inputSampleR = (o3AR + o3DR + o3GR)*0.03125;
			
			double earlyReflectionL = inputSampleL;
			double earlyReflectionR = inputSampleR;
			
			if (freq < 1.0) {
				double di = fabs(freq*(1.0+(inputSampleL*0.125))); if (di > 1.0) di = 1.0;
				double slew = ((inputSampleL - pear[prevSampL1]) + pear[prevSlewL1])*di*0.5;
				pear[prevSampL1] = inputSampleL = (di * inputSampleL) + ((1.0-di) * (pear[prevSampL1] + pear[prevSlewL1]));
				pear[prevSlewL1] = slew;
				di = fabs(freq*(1.0+(inputSampleL))); if (di > 1.0) di = 1.0;
				slew = ((inputSampleL - pear[prevSampL2]) + pear[prevSlewL2])*di*0.5;
				pear[prevSampL2] = inputSampleL = (di * inputSampleL) + ((1.0-di) * (pear[prevSampL2] + pear[prevSlewL2]));
				pear[prevSlewL2] = slew;
				di = fabs(freq*(1.0+(inputSampleL))); if (di > 1.0) di = 1.0;
				slew = ((inputSampleL - pear[prevSampL3]) + pear[prevSlewL3])*di*0.5;
				pear[prevSampL3] = inputSampleL = (di * inputSampleL) + ((1.0-di) * (pear[prevSampL3] + pear[prevSlewL3]));
				pear[prevSlewL3] = slew;
				
				di = fabs(freq*(1.0+(inputSampleR*0.125))); if (di > 1.0) di = 1.0;
				slew = ((inputSampleR - pear[prevSampR1]) + pear[prevSlewR1])*di*0.5;
				pear[prevSampR1] = inputSampleR = (di * inputSampleR) + ((1.0-di) * (pear[prevSampR1] + pear[prevSlewR1]));
				pear[prevSlewR1] = slew;
				di = fabs(freq*(1.0+(inputSampleR))); if (di > 1.0) di = 1.0;
				slew = ((inputSampleR - pear[prevSampR2]) + pear[prevSlewR2])*di*0.5;
				pear[prevSampR2] = inputSampleR = (di * inputSampleR) + ((1.0-di) * (pear[prevSampR2] + pear[prevSlewR2]));
				pear[prevSlewR2] = slew;
				di = fabs(freq*(1.0+(inputSampleR))); if (di > 1.0) di = 1.0;
				slew = ((inputSampleR - pear[prevSampR3]) + pear[prevSlewR3])*di*0.5;
				pear[prevSampR3] = inputSampleR = (di * inputSampleR) + ((1.0-di) * (pear[prevSampR3] + pear[prevSlewR3]));
				pear[prevSlewR3] = slew;
			}
			
			a6AL[c6AL] = inputSampleL + (f6BL * reg6n);
			a6BL[c6BL] = inputSampleL + (f6CL * reg6n);
			a6CL[c6CL] = inputSampleL + (f6DL * reg6n);
			a6DL[c6DL] = inputSampleL + (f6EL * reg6n);
			a6EL[c6EL] = inputSampleL + (f6FL * reg6n);
			a6FL[c6FL] = inputSampleL + (f6AL * reg6n);
			
			c6AL++; if (c6AL < 0 || c6AL > d6A) c6AL = 0;
			c6BL++; if (c6BL < 0 || c6BL > d6B) c6BL = 0;
			c6CL++; if (c6CL < 0 || c6CL > d6C) c6CL = 0;
			c6DL++; if (c6DL < 0 || c6DL > d6D) c6DL = 0;
			c6EL++; if (c6EL < 0 || c6EL > d6E) c6EL = 0;
			c6FL++; if (c6FL < 0 || c6FL > d6F) c6FL = 0;
			
			double o6AL = a6AL[c6AL-((c6AL > d6A)?d6A+1:0)];
			double o6BL = a6BL[c6BL-((c6BL > d6B)?d6B+1:0)];
			double o6CL = a6CL[c6CL-((c6CL > d6C)?d6C+1:0)];
			double o6DL = a6DL[c6DL-((c6DL > d6D)?d6D+1:0)];
			double o6EL = a6EL[c6EL-((c6EL > d6E)?d6E+1:0)];
			double o6FL = a6FL[c6FL-((c6FL > d6F)?d6F+1:0)];
			
			a6FR[c6FR] = inputSampleR + (f6LR * reg6n);
			a6LR[c6LR] = inputSampleR + (f6RR * reg6n);
			a6RR[c6RR] = inputSampleR + (f6XR * reg6n);
			a6XR[c6XR] = inputSampleR + (f6ZER * reg6n);
			a6ZER[c6ZER] = inputSampleR + (f6ZKR * reg6n);
			a6ZKR[c6ZKR] = inputSampleR + (f6FR * reg6n);
			
			c6FR++; if (c6FR < 0 || c6FR > d6F) c6FR = 0;
			c6LR++; if (c6LR < 0 || c6LR > d6L) c6LR = 0;
			c6RR++; if (c6RR < 0 || c6RR > d6R) c6RR = 0;
			c6XR++; if (c6XR < 0 || c6XR > d6X) c6XR = 0;
			c6ZER++; if (c6ZER < 0 || c6ZER > d6ZE) c6ZER = 0;
			c6ZKR++; if (c6ZKR < 0 || c6ZKR > d6ZK) c6ZKR = 0;
			
			double o6FR = a6FR[c6FR-((c6FR > d6F)?d6F+1:0)];
			double o6LR = a6LR[c6LR-((c6LR > d6L)?d6L+1:0)];
			double o6RR = a6RR[c6RR-((c6RR > d6R)?d6R+1:0)];
			double o6XR = a6XR[c6XR-((c6XR > d6X)?d6X+1:0)];
			double o6ZER = a6ZER[c6ZER-((c6ZER > d6ZE)?d6ZE+1:0)];
			double o6ZKR = a6ZKR[c6ZKR-((c6ZKR > d6ZK)?d6ZK+1:0)];
			
			//-------- one
			
			a6GL[c6GL] = ((o6AL*2.0) - (o6BL + o6CL + o6DL + o6EL + o6FL));
			a6HL[c6HL] = ((o6BL*2.0) - (o6AL + o6CL + o6DL + o6EL + o6FL));
			a6IL[c6IL] = ((o6CL*2.0) - (o6AL + o6BL + o6DL + o6EL + o6FL));
			a6JL[c6JL] = ((o6DL*2.0) - (o6AL + o6BL + o6CL + o6EL + o6FL));
			a6KL[c6KL] = ((o6EL*2.0) - (o6AL + o6BL + o6CL + o6DL + o6FL));
			a6LL[c6LL] = ((o6FL*2.0) - (o6AL + o6BL + o6CL + o6DL + o6EL));
			
			c6GL++; if (c6GL < 0 || c6GL > d6G) c6GL = 0;
			c6HL++; if (c6HL < 0 || c6HL > d6H) c6HL = 0;
			c6IL++; if (c6IL < 0 || c6IL > d6I) c6IL = 0;
			c6JL++; if (c6JL < 0 || c6JL > d6J) c6JL = 0;
			c6KL++; if (c6KL < 0 || c6KL > d6K) c6KL = 0;
			c6LL++; if (c6LL < 0 || c6LL > d6L) c6LL = 0;
			
			double o6GL = a6GL[c6GL-((c6GL > d6G)?d6G+1:0)];
			double o6HL = a6HL[c6HL-((c6HL > d6H)?d6H+1:0)];
			double o6IL = a6IL[c6IL-((c6IL > d6I)?d6I+1:0)];
			double o6JL = a6JL[c6JL-((c6JL > d6J)?d6J+1:0)];
			double o6KL = a6KL[c6KL-((c6KL > d6K)?d6K+1:0)];
			double o6LL = a6LL[c6LL-((c6LL > d6L)?d6L+1:0)];
			
			a6ER[c6ER] = ((o6FR*2.0) - (o6LR + o6RR + o6XR + o6ZER + o6ZKR));
			a6KR[c6KR] = ((o6LR*2.0) - (o6FR + o6RR + o6XR + o6ZER + o6ZKR));
			a6QR[c6QR] = ((o6RR*2.0) - (o6FR + o6LR + o6XR + o6ZER + o6ZKR));
			a6WR[c6WR] = ((o6XR*2.0) - (o6FR + o6LR + o6RR + o6ZER + o6ZKR));
			a6ZDR[c6ZDR] = ((o6ZER*2.0) - (o6FR + o6LR + o6RR + o6XR + o6ZKR));			
			a6ZJR[c6ZJR] = ((o6ZKR*2.0) - (o6FR + o6LR + o6RR + o6XR + o6ZER));			
			
			c6ER++; if (c6ER < 0 || c6ER > d6E) c6ER = 0;
			c6KR++; if (c6KR < 0 || c6KR > d6K) c6KR = 0;
			c6QR++; if (c6QR < 0 || c6QR > d6Q) c6QR = 0;
			c6WR++; if (c6WR < 0 || c6WR > d6W) c6WR = 0;
			c6ZDR++; if (c6ZDR < 0 || c6ZDR > d6ZD) c6ZDR = 0;
			c6ZJR++; if (c6ZJR < 0 || c6ZJR > d6ZJ) c6ZJR = 0;
			
			double o6ER = a6ER[c6ER-((c6ER > d6E)?d6E+1:0)];
			double o6KR = a6KR[c6KR-((c6KR > d6K)?d6K+1:0)];
			double o6QR = a6QR[c6QR-((c6QR > d6Q)?d6Q+1:0)];
			double o6WR = a6WR[c6WR-((c6WR > d6W)?d6W+1:0)];
			double o6ZDR = a6ZDR[c6ZDR-((c6ZDR > d6ZD)?d6ZD+1:0)];
			double o6ZJR = a6ZJR[c6ZJR-((c6ZJR > d6ZJ)?d6ZJ+1:0)];
			
			//-------- two
			
			a6ML[c6ML] = ((o6GL*2.0) - (o6HL + o6IL + o6JL + o6KL + o6LL));
			a6NL[c6NL] = ((o6HL*2.0) - (o6GL + o6IL + o6JL + o6KL + o6LL));
			a6OL[c6OL] = ((o6IL*2.0) - (o6GL + o6HL + o6JL + o6KL + o6LL));
			a6PL[c6PL] = ((o6JL*2.0) - (o6GL + o6HL + o6IL + o6KL + o6LL));
			a6QL[c6QL] = ((o6KL*2.0) - (o6GL + o6HL + o6IL + o6JL + o6LL));
			a6RL[c6RL] = ((o6LL*2.0) - (o6GL + o6HL + o6IL + o6JL + o6KL));
			
			c6ML++; if (c6ML < 0 || c6ML > d6M) c6ML = 0;
			c6NL++; if (c6NL < 0 || c6NL > d6N) c6NL = 0;
			c6OL++; if (c6OL < 0 || c6OL > d6O) c6OL = 0;
			c6PL++; if (c6PL < 0 || c6PL > d6P) c6PL = 0;
			c6QL++; if (c6QL < 0 || c6QL > d6Q) c6QL = 0;
			c6RL++; if (c6RL < 0 || c6RL > d6R) c6RL = 0;
			
			double o6ML = a6ML[c6ML-((c6ML > d6M)?d6M+1:0)];
			double o6NL = a6NL[c6NL-((c6NL > d6N)?d6N+1:0)];
			double o6OL = a6OL[c6OL-((c6OL > d6O)?d6O+1:0)];
			double o6PL = a6PL[c6PL-((c6PL > d6P)?d6P+1:0)];
			double o6QL = a6QL[c6QL-((c6QL > d6Q)?d6Q+1:0)];
			double o6RL = a6RL[c6RL-((c6RL > d6R)?d6R+1:0)];
			
			a6DR[c6DR] = ((o6ER*2.0) - (o6KR + o6QR + o6WR + o6ZDR + o6ZJR));
			a6JR[c6JR] = ((o6KR*2.0) - (o6ER + o6QR + o6WR + o6ZDR + o6ZJR));
			a6PR[c6PR] = ((o6QR*2.0) - (o6ER + o6KR + o6WR + o6ZDR + o6ZJR));
			a6VR[c6VR] = ((o6WR*2.0) - (o6ER + o6KR + o6QR + o6ZDR + o6ZJR));
			a6ZCR[c6ZCR] = ((o6ZDR*2.0) - (o6ER + o6KR + o6QR + o6WR + o6ZJR));
			a6ZIR[c6ZIR] = ((o6ZJR*2.0) - (o6ER + o6KR + o6QR + o6WR + o6ZDR));
			
			c6DR++; if (c6DR < 0 || c6DR > d6D) c6DR = 0;
			c6JR++; if (c6JR < 0 || c6JR > d6J) c6JR = 0;
			c6PR++; if (c6PR < 0 || c6PR > d6P) c6PR = 0;
			c6VR++; if (c6VR < 0 || c6VR > d6V) c6VR = 0;
			c6ZCR++; if (c6ZCR < 0 || c6ZCR > d6ZC) c6ZCR = 0;
			c6ZIR++; if (c6ZIR < 0 || c6ZIR > d6ZI) c6ZIR = 0;
			
			double o6DR = a6DR[c6DR-((c6DR > d6D)?d6D+1:0)];
			double o6JR = a6JR[c6JR-((c6JR > d6J)?d6J+1:0)];
			double o6PR = a6PR[c6PR-((c6PR > d6P)?d6P+1:0)];
			double o6VR = a6VR[c6VR-((c6VR > d6V)?d6V+1:0)];
			double o6ZCR = a6ZCR[c6ZCR-((c6ZCR > d6ZC)?d6ZC+1:0)];
			double o6ZIR = a6ZIR[c6ZIR-((c6ZIR > d6ZI)?d6ZI+1:0)];
			
			//-------- three
			
			a6SL[c6SL] = ((o6ML*2.0) - (o6NL + o6OL + o6PL + o6QL + o6RL));
			a6TL[c6TL] = ((o6NL*2.0) - (o6ML + o6OL + o6PL + o6QL + o6RL));
			a6UL[c6UL] = ((o6OL*2.0) - (o6ML + o6NL + o6PL + o6QL + o6RL));
			a6VL[c6VL] = ((o6PL*2.0) - (o6ML + o6NL + o6OL + o6QL + o6RL));
			a6WL[c6WL] = ((o6QL*2.0) - (o6ML + o6NL + o6OL + o6PL + o6RL));
			a6XL[c6XL] = ((o6RL*2.0) - (o6ML + o6NL + o6OL + o6PL + o6QL));
			
			c6SL++; if (c6SL < 0 || c6SL > d6S) c6SL = 0;
			c6TL++; if (c6TL < 0 || c6TL > d6T) c6TL = 0;
			c6UL++; if (c6UL < 0 || c6UL > d6U) c6UL = 0;
			c6VL++; if (c6VL < 0 || c6VL > d6V) c6VL = 0;
			c6WL++; if (c6WL < 0 || c6WL > d6W) c6WL = 0;
			c6XL++; if (c6XL < 0 || c6XL > d6X) c6XL = 0;
			
			double o6SL = a6SL[c6SL-((c6SL > d6S)?d6S+1:0)];
			double o6TL = a6TL[c6TL-((c6TL > d6T)?d6T+1:0)];
			double o6UL = a6UL[c6UL-((c6UL > d6U)?d6U+1:0)];
			double o6VL = a6VL[c6VL-((c6VL > d6V)?d6V+1:0)];
			double o6WL = a6WL[c6WL-((c6WL > d6W)?d6W+1:0)];
			double o6XL = a6XL[c6XL-((c6XL > d6X)?d6X+1:0)];
			
			a6CR[c6CR] = ((o6DR*2.0) - (o6JR + o6PR + o6VR + o6ZCR + o6ZIR));
			a6IR[c6IR] = ((o6JR*2.0) - (o6DR + o6PR + o6VR + o6ZCR + o6ZIR));
			a6OR[c6OR] = ((o6PR*2.0) - (o6DR + o6JR + o6VR + o6ZCR + o6ZIR));
			a6UR[c6UR] = ((o6VR*2.0) - (o6DR + o6JR + o6PR + o6ZCR + o6ZIR));
			a6ZBR[c6ZBR] = ((o6ZCR*2.0) - (o6DR + o6JR + o6PR + o6VR + o6ZIR));
			a6ZHR[c6ZHR] = ((o6ZIR*2.0) - (o6DR + o6JR + o6PR + o6VR + o6ZCR));
			
			c6CR++; if (c6CR < 0 || c6CR > d6C) c6CR = 0;
			c6IR++; if (c6IR < 0 || c6IR > d6I) c6IR = 0;
			c6OR++; if (c6OR < 0 || c6OR > d6O) c6OR = 0;
			c6UR++; if (c6UR < 0 || c6UR > d6U) c6UR = 0;
			c6ZBR++; if (c6ZBR < 0 || c6ZBR > d6ZB) c6ZBR = 0;
			c6ZHR++; if (c6ZHR < 0 || c6ZHR > d6ZH) c6ZHR = 0;
			
			double o6CR = a6CR[c6CR-((c6CR > d6C)?d6C+1:0)];
			double o6IR = a6IR[c6IR-((c6IR > d6I)?d6I+1:0)];
			double o6OR = a6OR[c6OR-((c6OR > d6O)?d6O+1:0)];
			double o6UR = a6UR[c6UR-((c6UR > d6U)?d6U+1:0)];
			double o6ZBR = a6ZBR[c6ZBR-((c6ZBR > d6ZB)?d6ZB+1:0)];
			double o6ZHR = a6ZHR[c6ZHR-((c6ZHR > d6ZH)?d6ZH+1:0)];
			
			//-------- four
			
			a6YL[c6YL] = ((o6SL*2.0) - (o6TL + o6UL + o6VL + o6WL + o6XL));
			a6ZAL[c6ZAL] = ((o6TL*2.0) - (o6SL + o6UL + o6VL + o6WL + o6XL));
			a6ZBL[c6ZBL] = ((o6UL*2.0) - (o6SL + o6TL + o6VL + o6WL + o6XL));
			a6ZCL[c6ZCL] = ((o6VL*2.0) - (o6SL + o6TL + o6UL + o6WL + o6XL));
			a6ZDL[c6ZDL] = ((o6WL*2.0) - (o6SL + o6TL + o6UL + o6VL + o6XL));
			a6ZEL[c6ZEL] = ((o6XL*2.0) - (o6SL + o6TL + o6UL + o6VL + o6WL));
			
			c6YL++; if (c6YL < 0 || c6YL > d6Y) c6YL = 0;
			c6ZAL++; if (c6ZAL < 0 || c6ZAL > d6ZA) c6ZAL = 0;
			c6ZBL++; if (c6ZBL < 0 || c6ZBL > d6ZB) c6ZBL = 0;
			c6ZCL++; if (c6ZCL < 0 || c6ZCL > d6ZC) c6ZCL = 0;
			c6ZDL++; if (c6ZDL < 0 || c6ZDL > d6ZD) c6ZDL = 0;
			c6ZEL++; if (c6ZEL < 0 || c6ZEL > d6ZE) c6ZEL = 0;
			
			double o6YL = a6YL[c6YL-((c6YL > d6Y)?d6Y+1:0)];
			double o6ZAL = a6ZAL[c6ZAL-((c6ZAL > d6ZA)?d6ZA+1:0)];
			double o6ZBL = a6ZBL[c6ZBL-((c6ZBL > d6ZB)?d6ZB+1:0)];
			double o6ZCL = a6ZCL[c6ZCL-((c6ZCL > d6ZC)?d6ZC+1:0)];
			double o6ZDL = a6ZDL[c6ZDL-((c6ZDL > d6ZD)?d6ZD+1:0)];
			double o6ZEL = a6ZEL[c6ZEL-((c6ZEL > d6ZE)?d6ZE+1:0)];
			
			a6BR[c6BR] = ((o6CR*2.0) - (o6IR + o6OR + o6UR + o6ZBR + o6ZHR));
			a6HR[c6HR] = ((o6IR*2.0) - (o6CR + o6OR + o6UR + o6ZBR + o6ZHR));
			a6NR[c6NR] = ((o6OR*2.0) - (o6CR + o6IR + o6UR + o6ZBR + o6ZHR));
			a6TR[c6TR] = ((o6UR*2.0) - (o6CR + o6IR + o6OR + o6ZBR + o6ZHR));
			a6ZAR[c6ZAR] = ((o6ZBR*2.0) - (o6CR + o6IR + o6OR + o6UR + o6ZHR));
			a6ZGR[c6ZGR] = ((o6ZHR*2.0) - (o6CR + o6IR + o6OR + o6UR + o6ZBR));
			
			c6BR++; if (c6BR < 0 || c6BR > d6B) c6BR = 0;
			c6HR++; if (c6HR < 0 || c6HR > d6H) c6HR = 0;
			c6NR++; if (c6NR < 0 || c6NR > d6N) c6NR = 0;
			c6TR++; if (c6TR < 0 || c6TR > d6T) c6TR = 0;
			c6ZBR++; if (c6ZBR < 0 || c6ZBR > d6ZB) c6ZBR = 0;
			c6ZGR++; if (c6ZGR < 0 || c6ZGR > d6ZG) c6ZGR = 0;
			
			double o6BR = a6BR[c6BR-((c6BR > d6B)?d6B+1:0)];
			double o6HR = a6HR[c6HR-((c6HR > d6H)?d6H+1:0)];
			double o6NR = a6NR[c6NR-((c6NR > d6N)?d6N+1:0)];
			double o6TR = a6TR[c6TR-((c6TR > d6T)?d6T+1:0)];
			double o6ZAR = a6ZAR[c6ZAR-((c6ZAR > d6ZA)?d6ZA+1:0)];
			double o6ZGR = a6ZGR[c6ZGR-((c6ZGR > d6ZG)?d6ZG+1:0)];
			
			//-------- five
			
			a6ZFL[c6ZFL] = ((o6YL*2.0) - (o6ZAL + o6ZBL + o6ZCL + o6ZDL + o6ZEL));
			a6ZGL[c6ZGL] = ((o6ZAL*2.0) - (o6YL + o6ZBL + o6ZCL + o6ZDL + o6ZEL));
			a6ZHL[c6ZHL] = ((o6ZBL*2.0) - (o6YL + o6ZAL + o6ZCL + o6ZDL + o6ZEL));
			a6ZIL[c6ZIL] = ((o6ZCL*2.0) - (o6YL + o6ZAL + o6ZBL + o6ZDL + o6ZEL));
			a6ZJL[c6ZJL] = ((o6ZDL*2.0) - (o6YL + o6ZAL + o6ZBL + o6ZCL + o6ZEL));
			a6ZKL[c6ZKL] = ((o6ZEL*2.0) - (o6YL + o6ZAL + o6ZBL + o6ZCL + o6ZDL));
			
			c6ZFL++; if (c6ZFL < 0 || c6ZFL > d6ZF) c6ZFL = 0;
			c6ZGL++; if (c6ZGL < 0 || c6ZGL > d6ZG) c6ZGL = 0;
			c6ZHL++; if (c6ZHL < 0 || c6ZHL > d6ZH) c6ZHL = 0;
			c6ZIL++; if (c6ZIL < 0 || c6ZIL > d6ZI) c6ZIL = 0;
			c6ZJL++; if (c6ZJL < 0 || c6ZJL > d6ZJ) c6ZJL = 0;
			c6ZKL++; if (c6ZKL < 0 || c6ZKL > d6ZK) c6ZKL = 0;
			
			double o6ZFL = a6ZFL[c6ZFL-((c6ZFL > d6ZF)?d6ZF+1:0)];
			double o6ZGL = a6ZGL[c6ZGL-((c6ZGL > d6ZG)?d6ZG+1:0)];
			double o6ZHL = a6ZHL[c6ZHL-((c6ZHL > d6ZH)?d6ZH+1:0)];
			double o6ZIL = a6ZIL[c6ZIL-((c6ZIL > d6ZI)?d6ZI+1:0)];
			double o6ZJL = a6ZJL[c6ZJL-((c6ZJL > d6ZJ)?d6ZJ+1:0)];
			double o6ZKL = a6ZKL[c6ZKL-((c6ZKL > d6ZK)?d6ZK+1:0)];
			
			a6AR[c6AR] = ((o6BR*2.0) - (o6HR + o6NR + o6TR + o6ZAR + o6ZGR));
			a6GR[c6GR] = ((o6HR*2.0) - (o6BR + o6NR + o6TR + o6ZAR + o6ZGR));
			a6MR[c6MR] = ((o6NR*2.0) - (o6BR + o6HR + o6TR + o6ZAR + o6ZGR));
			a6SR[c6SR] = ((o6TR*2.0) - (o6BR + o6HR + o6NR + o6ZAR + o6ZGR));
			a6YR[c6YR] = ((o6ZAR*2.0) - (o6BR + o6HR + o6NR + o6TR + o6ZGR));
			a6ZFR[c6ZFR] = ((o6ZGR*2.0) - (o6BR + o6HR + o6NR + o6TR + o6ZAR));
			
			c6AR++; if (c6AR < 0 || c6AR > d6A) c6AR = 0;
			c6GR++; if (c6GR < 0 || c6GR > d6G) c6GR = 0;
			c6MR++; if (c6MR < 0 || c6MR > d6M) c6MR = 0;
			c6SR++; if (c6SR < 0 || c6SR > d6S) c6SR = 0;
			c6YR++; if (c6YR < 0 || c6YR > d6Y) c6YR = 0;
			c6ZFR++; if (c6ZFR < 0 || c6ZFR > d6ZF) c6ZFR = 0;
			
			double o6AR = a6AR[c6AR-((c6AR > d6A)?d6A+1:0)];
			double o6GR = a6GR[c6GR-((c6GR > d6G)?d6G+1:0)];
			double o6MR = a6MR[c6MR-((c6MR > d6M)?d6M+1:0)];
			double o6SR = a6SR[c6SR-((c6SR > d6S)?d6S+1:0)];
			double o6YR = a6YR[c6YR-((c6YR > d6Y)?d6Y+1:0)];
			double o6ZFR = a6ZFR[c6ZFR-((c6ZFR > d6ZF)?d6ZF+1:0)];
			
			//-------- six
			
			f6AL = ((o6AR*2.0) - (o6GR + o6MR + o6SR + o6YR + o6ZFR));
			f6BL = ((o6GR*2.0) - (o6AR + o6MR + o6SR + o6YR + o6ZFR));
			f6CL = ((o6MR*2.0) - (o6AR + o6GR + o6SR + o6YR + o6ZFR));
			f6DL = ((o6SR*2.0) - (o6AR + o6GR + o6MR + o6YR + o6ZFR));
			f6EL = ((o6YR*2.0) - (o6AR + o6GR + o6MR + o6SR + o6ZFR));
			f6FL = ((o6ZFR*2.0) - (o6AR + o6GR + o6MR + o6SR + o6YR));
			
			f6FR = ((o6ZFL*2.0) - (o6ZGL + o6ZHL + o6ZIL + o6ZJL + o6ZKL));
			f6LR = ((o6ZGL*2.0) - (o6ZFL + o6ZHL + o6ZIL + o6ZJL + o6ZKL));
			f6RR = ((o6ZHL*2.0) - (o6ZFL + o6ZGL + o6ZIL + o6ZJL + o6ZKL));
			f6XR = ((o6ZIL*2.0) - (o6ZFL + o6ZGL + o6ZHL + o6ZJL + o6ZKL));
			f6ZER = ((o6ZJL*2.0) - (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZKL));
			f6ZKR = ((o6ZKL*2.0) - (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZJL));
			
			inputSampleL = (o6ZFL + o6ZGL + o6ZHL + o6ZIL + o6ZJL + o6ZKL)*0.001953125;
			inputSampleR = (o6AR + o6GR + o6MR + o6SR + o6YR + o6ZFR)*0.001953125;
			
			f6AL = (f6AL+f6AL+f6AL+fabs(avg6L))*0.25; avg6L = f6AL;
			f6FR = (f6FR+f6FR+f6FR+fabs(avg6R))*0.25; avg6R = f6FR;
			//manipulating deep reverb tail for realism
			
			inputSampleL += (earlyReflectionL * earlyLoudness);
			inputSampleR += (earlyReflectionR * earlyLoudness);
			
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
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*-0.0625;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*-0.0625;
		inputSampleL = CBAL+bez[bez_AvgOutSampL]; bez[bez_AvgOutSampL] = CBAL;
		inputSampleR = CBAR+bez[bez_AvgOutSampR]; bez[bez_AvgOutSampR] = CBAR;
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
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