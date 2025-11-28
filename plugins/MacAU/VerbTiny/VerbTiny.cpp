/*
*	File:		VerbTiny.cpp
*	
*	Version:	1.0
* 
*	Created:	11/10/25
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
	VerbTiny.cpp
	
=============================================================================*/
#include "VerbTiny.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(VerbTiny)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VerbTiny::VerbTiny
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
VerbTiny::VerbTiny(AudioUnit component)
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
//	VerbTiny::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			VerbTiny::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VerbTiny::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			VerbTiny::GetParameterInfo(AudioUnitScope		inScope,
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
//	VerbTiny::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			VerbTiny::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 VerbTiny::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	VerbTiny::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			VerbTiny::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	VerbTiny::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult VerbTiny::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____VerbTinyEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VerbTiny::VerbTinyKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		VerbTiny::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for(int x = 0; x < d4A+2; x++) {a4AL[x] = 0.0; a4AR[x] = 0.0;}
	for(int x = 0; x < d4B+2; x++) {a4BL[x] = 0.0; a4BR[x] = 0.0;}
	for(int x = 0; x < d4C+2; x++) {a4CL[x] = 0.0; a4CR[x] = 0.0;}
	for(int x = 0; x < d4D+2; x++) {a4DL[x] = 0.0; a4DR[x] = 0.0;}
	for(int x = 0; x < d4E+2; x++) {a4EL[x] = 0.0; a4ER[x] = 0.0;}
	for(int x = 0; x < d4F+2; x++) {a4FL[x] = 0.0; a4FR[x] = 0.0;}
	for(int x = 0; x < d4G+2; x++) {a4GL[x] = 0.0; a4GR[x] = 0.0;}
	for(int x = 0; x < d4H+2; x++) {a4HL[x] = 0.0; a4HR[x] = 0.0;}
	for(int x = 0; x < d4I+2; x++) {a4IL[x] = 0.0; a4IR[x] = 0.0;}
	for(int x = 0; x < d4J+2; x++) {a4JL[x] = 0.0; a4JR[x] = 0.0;}
	for(int x = 0; x < d4K+2; x++) {a4KL[x] = 0.0; a4KR[x] = 0.0;}
	for(int x = 0; x < d4L+2; x++) {a4LL[x] = 0.0; a4LR[x] = 0.0;}
	for(int x = 0; x < d4M+2; x++) {a4ML[x] = 0.0; a4MR[x] = 0.0;}
	for(int x = 0; x < d4N+2; x++) {a4NL[x] = 0.0; a4NR[x] = 0.0;}
	for(int x = 0; x < d4O+2; x++) {a4OL[x] = 0.0; a4OR[x] = 0.0;}
	for(int x = 0; x < d4P+2; x++) {a4PL[x] = 0.0; a4PR[x] = 0.0;}
	c4AL = c4BL = c4CL = c4DL = c4EL = c4FL = c4GL = c4HL = 1;
	c4IL = c4JL = c4KL = c4LL = c4ML = c4NL = c4OL = c4PL = 1;
	c4AR = c4BR = c4CR = c4DR = c4ER = c4FR = c4GR = c4HR = 1;
	c4IR = c4JR = c4KR = c4LR = c4MR = c4NR = c4OR = c4PR = 1;
	f4AL = f4BL = f4CL = f4DL = 0.0;
	f4DR = f4HR = f4LR = f4PR = 0.0;
	
	for(int x = 0; x < d4A+2; x++) {b4AL[x] = 0.0; b4AR[x] = 0.0;}
	for(int x = 0; x < d4B+2; x++) {b4BL[x] = 0.0; b4BR[x] = 0.0;}
	for(int x = 0; x < d4C+2; x++) {b4CL[x] = 0.0; b4CR[x] = 0.0;}
	for(int x = 0; x < d4D+2; x++) {b4DL[x] = 0.0; b4DR[x] = 0.0;}
	for(int x = 0; x < d4E+2; x++) {b4EL[x] = 0.0; b4ER[x] = 0.0;}
	for(int x = 0; x < d4F+2; x++) {b4FL[x] = 0.0; b4FR[x] = 0.0;}
	for(int x = 0; x < d4G+2; x++) {b4GL[x] = 0.0; b4GR[x] = 0.0;}
	for(int x = 0; x < d4H+2; x++) {b4HL[x] = 0.0; b4HR[x] = 0.0;}
	for(int x = 0; x < d4I+2; x++) {b4IL[x] = 0.0; b4IR[x] = 0.0;}
	for(int x = 0; x < d4J+2; x++) {b4JL[x] = 0.0; b4JR[x] = 0.0;}
	for(int x = 0; x < d4K+2; x++) {b4KL[x] = 0.0; b4KR[x] = 0.0;}
	for(int x = 0; x < d4L+2; x++) {b4LL[x] = 0.0; b4LR[x] = 0.0;}
	for(int x = 0; x < d4M+2; x++) {b4ML[x] = 0.0; b4MR[x] = 0.0;}
	for(int x = 0; x < d4N+2; x++) {b4NL[x] = 0.0; b4NR[x] = 0.0;}
	for(int x = 0; x < d4O+2; x++) {b4OL[x] = 0.0; b4OR[x] = 0.0;}
	for(int x = 0; x < d4P+2; x++) {b4PL[x] = 0.0; b4PR[x] = 0.0;}
	g4AL = g4BL = g4CL = g4DL = 0.0;
	g4DR = g4HR = g4LR = g4PR = 0.0;
	
	for (int x = 0; x < bez_total; x++) {
		bez[x] = 0.0;
		bezF[x] = 0.0;
	}
	bez[bez_cycle] = 1.0;
	bezF[bez_cycle] = 1.0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	VerbTiny::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		VerbTiny::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	
	double reg4n = 0.03125+((1.0-pow(1.0-GetParameter( kParam_A ),2.0))*0.03125);
	double attenuate = 1.0 - (1.0-pow(1.0-GetParameter( kParam_A ),2.0));
	double derez = pow(GetParameter( kParam_B ),2.0);
	derez = fmin(fmax(derez/overallscale,0.0001),1.0);
	int bezFraction = (int)(1.0/derez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	derez = 1.0 / bezFraction;
	bezTrim = 1.0-(derez*bezTrim);
	//the revision more accurately connects the bezier curves
	double derezFreq = pow(GetParameter( kParam_C ),2.0);
	derezFreq = fmin(fmax(derezFreq/overallscale,0.0001),1.0);
	int bezFreqFraction = (int)(1.0/derezFreq);
	double bezFreqTrim = (double)bezFreqFraction/(bezFreqFraction+1.0);
	derezFreq = 1.0 / bezFreqFraction;
	bezFreqTrim = 1.0-(derezFreq*bezFreqTrim);
	//the revision more accurately connects the bezier curves
	double wider = GetParameter( kParam_D )*2.0;
	double wet = GetParameter( kParam_E );
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += (inputSampleL*attenuate*derez);
		bez[bez_SampR] += (inputSampleR*attenuate*derez);
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			double mainSampleL = bez[bez_SampL];
			double dualmonoSampleL =  bez[bez_SampR];
			//workaround involves keeping the cross-matrix system,
			//but for initial layering, each side gets each version
			//making blends never quite line up as exactly the same.
			
			//left verbs
			a4AL[c4AL] = mainSampleL + (f4DR * reg4n);
			a4BL[c4BL] = mainSampleL + (f4HR * reg4n);
			a4CL[c4CL] = mainSampleL + (f4LR * reg4n);
			a4DL[c4DL] = mainSampleL + (f4PR * reg4n);
			b4AL[c4AL] = dualmonoSampleL + (g4AL * reg4n);
			b4BL[c4BL] = dualmonoSampleL + (g4BL * reg4n);
			b4CL[c4CL] = dualmonoSampleL + (g4CL * reg4n);
			b4DL[c4DL] = dualmonoSampleL + (g4DL * reg4n);
			
			c4AL++; if (c4AL < 0 || c4AL > d4A) c4AL = 0;
			c4BL++; if (c4BL < 0 || c4BL > d4B) c4BL = 0;
			c4CL++; if (c4CL < 0 || c4CL > d4C) c4CL = 0;
			c4DL++; if (c4DL < 0 || c4DL > d4D) c4DL = 0;
			
			double hA = a4AL[c4AL-((c4AL > d4A)?d4A+1:0)];
			double hB = a4BL[c4BL-((c4BL > d4B)?d4B+1:0)];
			double hC = a4CL[c4CL-((c4CL > d4C)?d4C+1:0)];
			double hD = a4DL[c4DL-((c4DL > d4D)?d4D+1:0)];
			a4EL[c4EL] = hA - (hB + hC + hD);
			a4FL[c4FL] = hB - (hA + hC + hD);
			a4GL[c4GL] = hC - (hA + hB + hD);
			a4HL[c4HL] = hD - (hA + hB + hC);
			hA = b4AL[c4AL-((c4AL > d4A)?d4A+1:0)];
			hB = b4BL[c4BL-((c4BL > d4B)?d4B+1:0)];
			hC = b4CL[c4CL-((c4CL > d4C)?d4C+1:0)];
			hD = b4DL[c4DL-((c4DL > d4D)?d4D+1:0)];
			b4EL[c4EL] = hA - (hB + hC + hD);
			b4FL[c4FL] = hB - (hA + hC + hD);
			b4GL[c4GL] = hC - (hA + hB + hD);
			b4HL[c4HL] = hD - (hA + hB + hC);
			
			c4EL++; if (c4EL < 0 || c4EL > d4E) c4EL = 0;
			c4FL++; if (c4FL < 0 || c4FL > d4F) c4FL = 0;
			c4GL++; if (c4GL < 0 || c4GL > d4G) c4GL = 0;
			c4HL++; if (c4HL < 0 || c4HL > d4H) c4HL = 0;
			
			hA = a4EL[c4EL-((c4EL > d4E)?d4E+1:0)];
			hB = a4FL[c4FL-((c4FL > d4F)?d4F+1:0)];
			hC = a4GL[c4GL-((c4GL > d4G)?d4G+1:0)];
			hD = a4HL[c4HL-((c4HL > d4H)?d4H+1:0)];
			a4IL[c4IL] = hA - (hB + hC + hD);
			a4JL[c4JL] = hB - (hA + hC + hD);
			a4KL[c4KL] = hC - (hA + hB + hD);
			a4LL[c4LL] = hD - (hA + hB + hC);
			hA = b4EL[c4EL-((c4EL > d4E)?d4E+1:0)];
			hB = b4FL[c4FL-((c4FL > d4F)?d4F+1:0)];
			hC = b4GL[c4GL-((c4GL > d4G)?d4G+1:0)];
			hD = b4HL[c4HL-((c4HL > d4H)?d4H+1:0)];
			b4IL[c4IL] = hA - (hB + hC + hD);
			b4JL[c4JL] = hB - (hA + hC + hD);
			b4KL[c4KL] = hC - (hA + hB + hD);
			b4LL[c4LL] = hD - (hA + hB + hC);
			
			c4IL++; if (c4IL < 0 || c4IL > d4I) c4IL = 0;
			c4JL++; if (c4JL < 0 || c4JL > d4J) c4JL = 0;
			c4KL++; if (c4KL < 0 || c4KL > d4K) c4KL = 0;
			c4LL++; if (c4LL < 0 || c4LL > d4L) c4LL = 0;
			
			hA = a4IL[c4IL-((c4IL > d4I)?d4I+1:0)];
			hB = a4JL[c4JL-((c4JL > d4J)?d4J+1:0)];
			hC = a4KL[c4KL-((c4KL > d4K)?d4K+1:0)];
			hD = a4LL[c4LL-((c4LL > d4L)?d4L+1:0)];
			a4ML[c4ML] = hA - (hB + hC + hD);
			a4NL[c4NL] = hB - (hA + hC + hD);
			a4OL[c4OL] = hC - (hA + hB + hD);
			a4PL[c4PL] = hD - (hA + hB + hC);
			hA = b4IL[c4IL-((c4IL > d4I)?d4I+1:0)];
			hB = b4JL[c4JL-((c4JL > d4J)?d4J+1:0)];
			hC = b4KL[c4KL-((c4KL > d4K)?d4K+1:0)];
			hD = b4LL[c4LL-((c4LL > d4L)?d4L+1:0)];
			b4ML[c4ML] = hA - (hB + hC + hD);
			b4NL[c4NL] = hB - (hA + hC + hD);
			b4OL[c4OL] = hC - (hA + hB + hD);
			b4PL[c4PL] = hD - (hA + hB + hC);
			
			c4ML++; if (c4ML < 0 || c4ML > d4M) c4ML = 0;
			c4NL++; if (c4NL < 0 || c4NL > d4N) c4NL = 0;
			c4OL++; if (c4OL < 0 || c4OL > d4O) c4OL = 0;
			c4PL++; if (c4PL < 0 || c4PL > d4P) c4PL = 0;
			
			hA = a4ML[c4ML-((c4ML > d4M)?d4M+1:0)];
			hB = a4NL[c4NL-((c4NL > d4N)?d4N+1:0)];
			hC = a4OL[c4OL-((c4OL > d4O)?d4O+1:0)];
			hD = a4PL[c4PL-((c4PL > d4P)?d4P+1:0)];
			f4AL = hA - (hB + hC + hD);				
			f4BL = hB - (hA + hC + hD);
			f4CL = hC - (hA + hB + hD);
			f4DL = hD - (hA + hB + hC);//not actually crosschannel yet
			mainSampleL = (hA + hB + hC + hD)*0.125;
			
			hA = b4ML[c4ML-((c4ML > d4M)?d4M+1:0)];
			hB = b4NL[c4NL-((c4NL > d4N)?d4N+1:0)];
			hC = b4OL[c4OL-((c4OL > d4O)?d4O+1:0)];
			hD = b4PL[c4PL-((c4PL > d4P)?d4P+1:0)];
			g4AL = hA - (hB + hC + hD);
			g4BL = hB - (hA + hC + hD);
			g4CL = hC - (hA + hB + hD);
			g4DL = hD - (hA + hB + hC);			
			dualmonoSampleL = (hA + hB + hC + hD)*0.125;
			
			double mainSampleR = bez[bez_SampR]; //begin primary reverb
			double dualmonoSampleR =  bez[bez_SampL];
			
			//right verbs
			a4DR[c4DR] = mainSampleR + (f4AL * reg4n);
			a4HR[c4HR] = mainSampleR + (f4BL * reg4n);
			a4LR[c4LR] = mainSampleR + (f4CL * reg4n);
			a4PR[c4PR] = mainSampleR + (f4DL * reg4n);
			b4DR[c4DR] = dualmonoSampleR + (g4DR * reg4n);
			b4HR[c4HR] = dualmonoSampleR + (g4HR * reg4n);
			b4LR[c4LR] = dualmonoSampleR + (g4LR * reg4n);
			b4PR[c4PR] = dualmonoSampleR + (g4PR * reg4n);
			
			c4DR++; if (c4DR < 0 || c4DR > d4D) c4DR = 0;
			c4HR++; if (c4HR < 0 || c4HR > d4H) c4HR = 0;
			c4LR++; if (c4LR < 0 || c4LR > d4L) c4LR = 0;
			c4PR++; if (c4PR < 0 || c4PR > d4P) c4PR = 0;
			
			hA = a4DR[c4DR-((c4DR > d4D)?d4D+1:0)];
			hB = a4HR[c4HR-((c4HR > d4H)?d4H+1:0)];
			hC = a4LR[c4LR-((c4LR > d4L)?d4L+1:0)];
			hD = a4PR[c4PR-((c4PR > d4P)?d4P+1:0)];
			a4CR[c4CR] = hA - (hB + hC + hD);
			a4GR[c4GR] = hB - (hA + hC + hD);
			a4KR[c4KR] = hC - (hA + hB + hD);
			a4OR[c4OR] = hD - (hA + hB + hC);
			hA = b4DR[c4DR-((c4DR > d4D)?d4D+1:0)];
			hB = b4HR[c4HR-((c4HR > d4H)?d4H+1:0)];
			hC = b4LR[c4LR-((c4LR > d4L)?d4L+1:0)];
			hD = b4PR[c4PR-((c4PR > d4P)?d4P+1:0)];
			b4CR[c4CR] = hA - (hB + hC + hD);
			b4GR[c4GR] = hB - (hA + hC + hD);
			b4KR[c4KR] = hC - (hA + hB + hD);
			b4OR[c4OR] = hD - (hA + hB + hC);
			
			c4CR++; if (c4CR < 0 || c4CR > d4C) c4CR = 0;
			c4GR++; if (c4GR < 0 || c4GR > d4G) c4GR = 0;
			c4KR++; if (c4KR < 0 || c4KR > d4K) c4KR = 0;
			c4OR++; if (c4OR < 0 || c4OR > d4O) c4OR = 0;
			
			hA = a4CR[c4CR-((c4CR > d4C)?d4C+1:0)];
			hB = a4GR[c4GR-((c4GR > d4G)?d4G+1:0)];
			hC = a4KR[c4KR-((c4KR > d4K)?d4K+1:0)];
			hD = a4OR[c4OR-((c4OR > d4O)?d4O+1:0)];
			a4BR[c4BR] = hA - (hB + hC + hD);
			a4FR[c4FR] = hB - (hA + hC + hD);
			a4JR[c4JR] = hC - (hA + hB + hD);
			a4NR[c4NR] = hD - (hA + hB + hC);
			hA = b4CR[c4CR-((c4CR > d4C)?d4C+1:0)];
			hB = b4GR[c4GR-((c4GR > d4G)?d4G+1:0)];
			hC = b4KR[c4KR-((c4KR > d4K)?d4K+1:0)];
			hD = b4OR[c4OR-((c4OR > d4O)?d4O+1:0)];
			b4BR[c4BR] = hA - (hB + hC + hD);
			b4FR[c4FR] = hB - (hA + hC + hD);
			b4JR[c4JR] = hC - (hA + hB + hD);
			b4NR[c4NR] = hD - (hA + hB + hC);
			
			c4BR++; if (c4BR < 0 || c4BR > d4B) c4BR = 0;
			c4FR++; if (c4FR < 0 || c4FR > d4F) c4FR = 0;
			c4JR++; if (c4JR < 0 || c4JR > d4J) c4JR = 0;
			c4NR++; if (c4NR < 0 || c4NR > d4N) c4NR = 0;
			
			hA = a4BR[c4BR-((c4BR > d4B)?d4B+1:0)];
			hB = a4FR[c4FR-((c4FR > d4F)?d4F+1:0)];
			hC = a4JR[c4JR-((c4JR > d4J)?d4J+1:0)];
			hD = a4NR[c4NR-((c4NR > d4N)?d4N+1:0)];
			a4AR[c4AR] = hA - (hB + hC + hD);
			a4ER[c4ER] = hB - (hA + hC + hD);
			a4IR[c4IR] = hC - (hA + hB + hD);
			a4MR[c4MR] = hD - (hA + hB + hC);
			hA = b4BR[c4BR-((c4BR > d4B)?d4B+1:0)];
			hB = b4FR[c4FR-((c4FR > d4F)?d4F+1:0)];
			hC = b4JR[c4JR-((c4JR > d4J)?d4J+1:0)];
			hD = b4NR[c4NR-((c4NR > d4N)?d4N+1:0)];
			b4AR[c4AR] = hA - (hB + hC + hD);
			b4ER[c4ER] = hB - (hA + hC + hD);
			b4IR[c4IR] = hC - (hA + hB + hD);
			b4MR[c4MR] = hD - (hA + hB + hC);
			
			c4AR++; if (c4AR < 0 || c4AR > d4A) c4AR = 0;
			c4ER++; if (c4ER < 0 || c4ER > d4E) c4ER = 0;
			c4IR++; if (c4IR < 0 || c4IR > d4I) c4IR = 0;
			c4MR++; if (c4MR < 0 || c4MR > d4M) c4MR = 0;
			
			hA = a4AR[c4AR-((c4AR > d4A)?d4A+1:0)];
			hB = a4ER[c4ER-((c4ER > d4E)?d4E+1:0)];
			hC = a4IR[c4IR-((c4IR > d4I)?d4I+1:0)];
			hD = a4MR[c4MR-((c4MR > d4M)?d4M+1:0)];
			f4DR = hA - (hB + hC + hD);
			f4HR = hB - (hA + hC + hD);
			f4LR = hC - (hA + hB + hD);
			f4PR = hD - (hA + hB + hC);
			mainSampleR = (hA + hB + hC + hD)*0.125;
			
			hA = b4AR[c4AR-((c4AR > d4A)?d4A+1:0)];
			hB = b4ER[c4ER-((c4ER > d4E)?d4E+1:0)];
			hC = b4IR[c4IR-((c4IR > d4I)?d4I+1:0)];
			hD = b4MR[c4MR-((c4MR > d4M)?d4M+1:0)];
			g4DR = hA - (hB + hC + hD);				
			g4HR = hB - (hA + hC + hD);
			g4LR = hC - (hA + hB + hD);
			g4PR = hD - (hA + hB + hC);
			dualmonoSampleR = (hA + hB + hC + hD)*0.125;
			//dual mono version is wider = 1.0 at the center
			//with mainsample 0.0 and 2.0 (only at the edges)
			//with mainsample out of phase when over 1.0
			//couldn't re-do the arrays perfectly, so instead
			//we keep exactly the same cross-matrix,
			//but we flip the sides we're using for initial reverb.
			//then, dualmono remains totally dualmono, and blend a bit in for wideness.
			
			if (wider < 1.0) {
				inputSampleL = (dualmonoSampleR*wider) + (mainSampleL*(1.0-wider));
				inputSampleR = (dualmonoSampleL*wider) + (mainSampleR*(1.0-wider));
			} else {
				inputSampleL = (dualmonoSampleR*(2.0-wider)) + (mainSampleL*(wider-1.0));
				inputSampleR = (dualmonoSampleL*(2.0-wider)) + (-mainSampleR*(wider-1.0));
			}
			
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
		inputSampleL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*-0.25;
		inputSampleR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*-0.25;
		
		bezF[bez_cycle] += derezFreq;
		bezF[bez_SampL] += (inputSampleL * derezFreq);
		bezF[bez_SampR] += (inputSampleR * derezFreq);
		if (bezF[bez_cycle] > 1.0) { //hit the end point and we do a filter sample
			bezF[bez_cycle] = 0.0;
			bezF[bez_CL] = bezF[bez_BL];
			bezF[bez_BL] = bezF[bez_AL];
			bezF[bez_AL] = bezF[bez_SampL];
			bezF[bez_SampL] = 0.0;
			bezF[bez_CR] = bezF[bez_BR];
			bezF[bez_BR] = bezF[bez_AR];
			bezF[bez_AR] = bezF[bez_SampR];
			bezF[bez_SampR] = 0.0;
		}
		X = bezF[bez_cycle]*bezFreqTrim;
		double CBLfreq = (bezF[bez_CL]*(1.0-X))+(bezF[bez_BL]*X);
		double BALfreq = (bezF[bez_BL]*(1.0-X))+(bezF[bez_AL]*X);
		inputSampleL = (bezF[bez_BL]+(CBLfreq*(1.0-X))+(BALfreq*X))*0.5;
		double CBRfreq = (bezF[bez_CR]*(1.0-X))+(bezF[bez_BR]*X);
		double BARfreq = (bezF[bez_BR]*(1.0-X))+(bezF[bez_AR]*X);
		inputSampleR = (bezF[bez_BR]+(CBRfreq*(1.0-X))+(BARfreq*X))*0.5;
		//filtering the reverb separately, after making it
		
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