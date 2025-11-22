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

AUDIOCOMPONENT_ENTRY(AUBaseFactory, VerbTiny)


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
	e4AL = e4BL = e4CL = e4DL = e4EL = e4FL = e4GL = e4HL = 1;
	e4IL = e4JL = e4KL = e4LL = e4ML = e4NL = e4OL = e4PL = 1;
	e4AR = e4BR = e4CR = e4DR = e4ER = e4FR = e4GR = e4HR = 1;
	e4IR = e4JR = e4KR = e4LR = e4MR = e4NR = e4OR = e4PR = 1;
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
			double mainSampleR = bez[bez_SampR]; //begin primary reverb
			
			a4AL[c4AL] = mainSampleL + (f4AL * reg4n);
			a4BL[c4BL] = mainSampleL + (f4BL * reg4n);
			a4CL[c4CL] = mainSampleL + (f4CL * reg4n);
			a4DL[c4DL] = mainSampleL + (f4DL * reg4n);
			
			c4AL++; if (c4AL < 0 || c4AL > d4A) c4AL = 0;
			c4BL++; if (c4BL < 0 || c4BL > d4B) c4BL = 0;
			c4CL++; if (c4CL < 0 || c4CL > d4C) c4CL = 0;
			c4DL++; if (c4DL < 0 || c4DL > d4D) c4DL = 0;
			
			double o4AL = a4AL[c4AL-((c4AL > d4A)?d4A+1:0)];
			double o4BL = a4BL[c4BL-((c4BL > d4B)?d4B+1:0)];
			double o4CL = a4CL[c4CL-((c4CL > d4C)?d4C+1:0)];
			double o4DL = a4DL[c4DL-((c4DL > d4D)?d4D+1:0)];
			
			a4DR[c4DR] = mainSampleR + (f4DR * reg4n);
			a4HR[c4HR] = mainSampleR + (f4HR * reg4n);
			a4LR[c4LR] = mainSampleR + (f4LR * reg4n);
			a4PR[c4PR] = mainSampleR + (f4PR * reg4n);
			
			c4DR++; if (c4DR < 0 || c4DR > d4D) c4DR = 0;
			c4HR++; if (c4HR < 0 || c4HR > d4H) c4HR = 0;
			c4LR++; if (c4LR < 0 || c4LR > d4L) c4LR = 0;
			c4PR++; if (c4PR < 0 || c4PR > d4P) c4PR = 0;
			
			double o4DR = a4DR[c4DR-((c4DR > d4D)?d4D+1:0)];
			double o4HR = a4HR[c4HR-((c4HR > d4H)?d4H+1:0)];
			double o4LR = a4LR[c4LR-((c4LR > d4L)?d4L+1:0)];
			double o4PR = a4PR[c4PR-((c4PR > d4P)?d4P+1:0)];
			
			//-------- one
			
			a4EL[c4EL] = o4AL - (o4BL + o4CL + o4DL);
			a4FL[c4FL] = o4BL - (o4AL + o4CL + o4DL);
			a4GL[c4GL] = o4CL - (o4AL + o4BL + o4DL);
			a4HL[c4HL] = o4DL - (o4AL + o4BL + o4CL);
			
			c4EL++; if (c4EL < 0 || c4EL > d4E) c4EL = 0;
			c4FL++; if (c4FL < 0 || c4FL > d4F) c4FL = 0;
			c4GL++; if (c4GL < 0 || c4GL > d4G) c4GL = 0;
			c4HL++; if (c4HL < 0 || c4HL > d4H) c4HL = 0;
			
			double o4EL = a4EL[c4EL-((c4EL > d4E)?d4E+1:0)];
			double o4FL = a4FL[c4FL-((c4FL > d4F)?d4F+1:0)];
			double o4GL = a4GL[c4GL-((c4GL > d4G)?d4G+1:0)];
			double o4HL = a4HL[c4HL-((c4HL > d4H)?d4H+1:0)];
			
			a4CR[c4CR] = o4DR - (o4HR + o4LR + o4PR);
			a4GR[c4GR] = o4HR - (o4DR + o4LR + o4PR);
			a4KR[c4KR] = o4LR - (o4DR + o4HR + o4PR);
			a4OR[c4OR] = o4PR - (o4DR + o4HR + o4LR);
			
			c4CR++; if (c4CR < 0 || c4CR > d4C) c4CR = 0;
			c4GR++; if (c4GR < 0 || c4GR > d4G) c4GR = 0;
			c4KR++; if (c4KR < 0 || c4KR > d4K) c4KR = 0;
			c4OR++; if (c4OR < 0 || c4OR > d4O) c4OR = 0;
			
			double o4CR = a4CR[c4CR-((c4CR > d4C)?d4C+1:0)];
			double o4GR = a4GR[c4GR-((c4GR > d4G)?d4G+1:0)];
			double o4KR = a4KR[c4KR-((c4KR > d4K)?d4K+1:0)];
			double o4OR = a4OR[c4OR-((c4OR > d4O)?d4O+1:0)];
			
			//-------- two
			
			a4IL[c4IL] = o4EL - (o4FL + o4GL + o4HL);
			a4JL[c4JL] = o4FL - (o4EL + o4GL + o4HL);
			a4KL[c4KL] = o4GL - (o4EL + o4FL + o4HL);
			a4LL[c4LL] = o4HL - (o4EL + o4FL + o4GL);
			
			c4IL++; if (c4IL < 0 || c4IL > d4I) c4IL = 0;
			c4JL++; if (c4JL < 0 || c4JL > d4J) c4JL = 0;
			c4KL++; if (c4KL < 0 || c4KL > d4K) c4KL = 0;
			c4LL++; if (c4LL < 0 || c4LL > d4L) c4LL = 0;
			
			double o4IL = a4IL[c4IL-((c4IL > d4I)?d4I+1:0)];
			double o4JL = a4JL[c4JL-((c4JL > d4J)?d4J+1:0)];
			double o4KL = a4KL[c4KL-((c4KL > d4K)?d4K+1:0)];
			double o4LL = a4LL[c4LL-((c4LL > d4L)?d4L+1:0)];
			
			a4BR[c4BR] = o4CR - (o4GR + o4KR + o4OR);
			a4FR[c4FR] = o4GR - (o4CR + o4KR + o4OR);
			a4JR[c4JR] = o4KR - (o4CR + o4GR + o4OR);
			a4NR[c4NR] = o4OR - (o4CR + o4GR + o4KR);
			
			c4BR++; if (c4BR < 0 || c4BR > d4B) c4BR = 0;
			c4FR++; if (c4FR < 0 || c4FR > d4F) c4FR = 0;
			c4JR++; if (c4JR < 0 || c4JR > d4J) c4JR = 0;
			c4NR++; if (c4NR < 0 || c4NR > d4N) c4NR = 0;
			
			double o4BR = a4BR[c4BR-((c4BR > d4B)?d4B+1:0)];
			double o4FR = a4FR[c4FR-((c4FR > d4F)?d4F+1:0)];
			double o4JR = a4JR[c4JR-((c4JR > d4J)?d4J+1:0)];
			double o4NR = a4NR[c4NR-((c4NR > d4N)?d4N+1:0)];
			
			//-------- three
			
			a4ML[c4ML] = o4IL - (o4JL + o4KL + o4LL);
			a4NL[c4NL] = o4JL - (o4IL + o4KL + o4LL);
			a4OL[c4OL] = o4KL - (o4IL + o4JL + o4LL);
			a4PL[c4PL] = o4LL - (o4IL + o4JL + o4KL);
			
			c4ML++; if (c4ML < 0 || c4ML > d4M) c4ML = 0;
			c4NL++; if (c4NL < 0 || c4NL > d4N) c4NL = 0;
			c4OL++; if (c4OL < 0 || c4OL > d4O) c4OL = 0;
			c4PL++; if (c4PL < 0 || c4PL > d4P) c4PL = 0;
			
			double o4ML = a4ML[c4ML-((c4ML > d4M)?d4M+1:0)];
			double o4NL = a4NL[c4NL-((c4NL > d4N)?d4N+1:0)];
			double o4OL = a4OL[c4OL-((c4OL > d4O)?d4O+1:0)];
			double o4PL = a4PL[c4PL-((c4PL > d4P)?d4P+1:0)];
			
			a4AR[c4AR] = o4BR - (o4FR + o4JR + o4NR);
			a4ER[c4ER] = o4FR - (o4BR + o4JR + o4NR);
			a4IR[c4IR] = o4JR - (o4BR + o4FR + o4NR);
			a4MR[c4MR] = o4NR - (o4BR + o4FR + o4JR);
			
			c4AR++; if (c4AR < 0 || c4AR > d4A) c4AR = 0;
			c4ER++; if (c4ER < 0 || c4ER > d4E) c4ER = 0;
			c4IR++; if (c4IR < 0 || c4IR > d4I) c4IR = 0;
			c4MR++; if (c4MR < 0 || c4MR > d4M) c4MR = 0;
			
			double o4AR = a4AR[c4AR-((c4AR > d4A)?d4A+1:0)];
			double o4ER = a4ER[c4ER-((c4ER > d4E)?d4E+1:0)];
			double o4IR = a4IR[c4IR-((c4IR > d4I)?d4I+1:0)];
			double o4MR = a4MR[c4MR-((c4MR > d4M)?d4M+1:0)];
			
			//-------- four
			
			f4AL = o4AR - (o4ER + o4IR + o4MR);				
			f4BL = o4ER - (o4AR + o4IR + o4MR);
			f4CL = o4IR - (o4AR + o4ER + o4MR);
			f4DL = o4MR - (o4AR + o4ER + o4IR);
			
			f4DR = o4ML - (o4NL + o4OL + o4PL);
			f4HR = o4NL - (o4ML + o4OL + o4PL);
			f4LR = o4OL - (o4ML + o4NL + o4PL);
			f4PR = o4PL - (o4ML + o4NL + o4OL);
			
			mainSampleL = (o4ML + o4NL + o4OL + o4PL)*0.125;
			mainSampleR = (o4AR + o4ER + o4IR + o4MR)*0.125;
			
			//we still have an untouched inputsample, turn it into dual mono
			//won't need to redeclare any of the o4 temp variables, just re-use
			
			double dualmonoSampleL =  bez[bez_SampR];
			double dualmonoSampleR =  bez[bez_SampL];
			//workaround involves keeping the cross-matrix system,
			//but for initial layering, each side gets each version
			//making blends never quite line up as exactly the same.
			
			b4AL[e4AL] = dualmonoSampleL + (g4AL * reg4n);
			b4BL[e4BL] = dualmonoSampleL + (g4BL * reg4n);
			b4CL[e4CL] = dualmonoSampleL + (g4CL * reg4n);
			b4DL[e4DL] = dualmonoSampleL + (g4DL * reg4n);
			
			e4AL++; if (e4AL < 0 || e4AL > d4A) e4AL = 0;
			e4BL++; if (e4BL < 0 || e4BL > d4B) e4BL = 0;
			e4CL++; if (e4CL < 0 || e4CL > d4C) e4CL = 0;
			e4DL++; if (e4DL < 0 || e4DL > d4D) e4DL = 0;
			
			o4AL = b4AL[e4AL-((e4AL > d4A)?d4A+1:0)];
			o4BL = b4BL[e4BL-((e4BL > d4B)?d4B+1:0)];
			o4CL = b4CL[e4CL-((e4CL > d4C)?d4C+1:0)];
			o4DL = b4DL[e4DL-((e4DL > d4D)?d4D+1:0)];
			
			b4DR[e4DR] = dualmonoSampleR + (g4DR * reg4n);
			b4HR[e4HR] = dualmonoSampleR + (g4HR * reg4n);
			b4LR[e4LR] = dualmonoSampleR + (g4LR * reg4n);
			b4PR[e4PR] = dualmonoSampleR + (g4PR * reg4n);
			
			e4DR++; if (e4DR < 0 || e4DR > d4D) e4DR = 0;
			e4HR++; if (e4HR < 0 || e4HR > d4H) e4HR = 0;
			e4LR++; if (e4LR < 0 || e4LR > d4L) e4LR = 0;
			e4PR++; if (e4PR < 0 || e4PR > d4P) e4PR = 0;
			
			o4DR = b4DR[e4DR-((e4DR > d4D)?d4D+1:0)];
			o4HR = b4HR[e4HR-((e4HR > d4H)?d4H+1:0)];
			o4LR = b4LR[e4LR-((e4LR > d4L)?d4L+1:0)];
			o4PR = b4PR[e4PR-((e4PR > d4P)?d4P+1:0)];
			
			//-------- one
			
			b4EL[e4EL] = o4AL - (o4BL + o4CL + o4DL);
			b4FL[e4FL] = o4BL - (o4AL + o4CL + o4DL);
			b4GL[e4GL] = o4CL - (o4AL + o4BL + o4DL);
			b4HL[e4HL] = o4DL - (o4AL + o4BL + o4CL);
			
			e4EL++; if (e4EL < 0 || e4EL > d4E) e4EL = 0;
			e4FL++; if (e4FL < 0 || e4FL > d4F) e4FL = 0;
			e4GL++; if (e4GL < 0 || e4GL > d4G) e4GL = 0;
			e4HL++; if (e4HL < 0 || e4HL > d4H) e4HL = 0;
			
			o4EL = b4EL[e4EL-((e4EL > d4E)?d4E+1:0)];
			o4FL = b4FL[e4FL-((e4FL > d4F)?d4F+1:0)];
			o4GL = b4GL[e4GL-((e4GL > d4G)?d4G+1:0)];
			o4HL = b4HL[e4HL-((e4HL > d4H)?d4H+1:0)];
		
			b4CR[e4CR] = o4DR - (o4HR + o4LR + o4PR);
			b4GR[e4GR] = o4HR - (o4DR + o4LR + o4PR);
			b4KR[e4KR] = o4LR - (o4DR + o4HR + o4PR);
			b4OR[e4OR] = o4PR - (o4DR + o4HR + o4LR);
			
			e4CR++; if (e4CR < 0 || e4CR > d4C) e4CR = 0;
			e4GR++; if (e4GR < 0 || e4GR > d4G) e4GR = 0;
			e4KR++; if (e4KR < 0 || e4KR > d4K) e4KR = 0;
			e4OR++; if (e4OR < 0 || e4OR > d4O) e4OR = 0;
			
			o4CR = b4CR[e4CR-((e4CR > d4C)?d4C+1:0)];
			o4GR = b4GR[e4GR-((e4GR > d4G)?d4G+1:0)];
			o4KR = b4KR[e4KR-((e4KR > d4K)?d4K+1:0)];
			o4OR = b4OR[e4OR-((e4OR > d4O)?d4O+1:0)];
			
			//-------- two
			
			b4IL[e4IL] = o4EL - (o4FL + o4GL + o4HL);
			b4JL[e4JL] = o4FL - (o4EL + o4GL + o4HL);
			b4KL[e4KL] = o4GL - (o4EL + o4FL + o4HL);
			b4LL[e4LL] = o4HL - (o4EL + o4FL + o4GL);
			
			e4IL++; if (e4IL < 0 || e4IL > d4I) e4IL = 0;
			e4JL++; if (e4JL < 0 || e4JL > d4J) e4JL = 0;
			e4KL++; if (e4KL < 0 || e4KL > d4K) e4KL = 0;
			e4LL++; if (e4LL < 0 || e4LL > d4L) e4LL = 0;
			
			o4IL = b4IL[e4IL-((e4IL > d4I)?d4I+1:0)];
			o4JL = b4JL[e4JL-((e4JL > d4J)?d4J+1:0)];
			o4KL = b4KL[e4KL-((e4KL > d4K)?d4K+1:0)];
			o4LL = b4LL[e4LL-((e4LL > d4L)?d4L+1:0)];
			
			b4BR[e4BR] = o4CR - (o4GR + o4KR + o4OR);
			b4FR[e4FR] = o4GR - (o4CR + o4KR + o4OR);
			b4JR[e4JR] = o4KR - (o4CR + o4GR + o4OR);
			b4NR[e4NR] = o4OR - (o4CR + o4GR + o4KR);
			
			e4BR++; if (e4BR < 0 || e4BR > d4B) e4BR = 0;
			e4FR++; if (e4FR < 0 || e4FR > d4F) e4FR = 0;
			e4JR++; if (e4JR < 0 || e4JR > d4J) e4JR = 0;
			e4NR++; if (e4NR < 0 || e4NR > d4N) e4NR = 0;
			
			o4BR = b4BR[e4BR-((e4BR > d4B)?d4B+1:0)];
			o4FR = b4FR[e4FR-((e4FR > d4F)?d4F+1:0)];
			o4JR = b4JR[e4JR-((e4JR > d4J)?d4J+1:0)];
			o4NR = b4NR[e4NR-((e4NR > d4N)?d4N+1:0)];
			
			//-------- three
			
			b4ML[e4ML] = o4IL - (o4JL + o4KL + o4LL);
			b4NL[e4NL] = o4JL - (o4IL + o4KL + o4LL);
			b4OL[e4OL] = o4KL - (o4IL + o4JL + o4LL);
			b4PL[e4PL] = o4LL - (o4IL + o4JL + o4KL);
			
			e4ML++; if (e4ML < 0 || e4ML > d4M) e4ML = 0;
			e4NL++; if (e4NL < 0 || e4NL > d4N) e4NL = 0;
			e4OL++; if (e4OL < 0 || e4OL > d4O) e4OL = 0;
			e4PL++; if (e4PL < 0 || e4PL > d4P) e4PL = 0;
			
			o4ML = b4ML[e4ML-((e4ML > d4M)?d4M+1:0)];
			o4NL = b4NL[e4NL-((e4NL > d4N)?d4N+1:0)];
			o4OL = b4OL[e4OL-((e4OL > d4O)?d4O+1:0)];
			o4PL = b4PL[e4PL-((e4PL > d4P)?d4P+1:0)];
			
			b4AR[e4AR] = o4BR - (o4FR + o4JR + o4NR);
			b4ER[e4ER] = o4FR - (o4BR + o4JR + o4NR);
			b4IR[e4IR] = o4JR - (o4BR + o4FR + o4NR);
			b4MR[e4MR] = o4NR - (o4BR + o4FR + o4JR);
			
			e4AR++; if (e4AR < 0 || e4AR > d4A) e4AR = 0;
			e4ER++; if (e4ER < 0 || e4ER > d4E) e4ER = 0;
			e4IR++; if (e4IR < 0 || e4IR > d4I) e4IR = 0;
			e4MR++; if (e4MR < 0 || e4MR > d4M) e4MR = 0;
			
			o4AR = b4AR[e4AR-((e4AR > d4A)?d4A+1:0)];
			o4ER = b4ER[e4ER-((e4ER > d4E)?d4E+1:0)];
			o4IR = b4IR[e4IR-((e4IR > d4I)?d4I+1:0)];
			o4MR = b4MR[e4MR-((e4MR > d4M)?d4M+1:0)];
			
			//-------- four
			
			g4DR = o4AR - (o4ER + o4IR + o4MR);				
			g4HR = o4ER - (o4AR + o4IR + o4MR);
			g4LR = o4IR - (o4AR + o4ER + o4MR);
			g4PR = o4MR - (o4AR + o4ER + o4IR);
			
			g4AL = o4ML - (o4NL + o4OL + o4PL);
			g4BL = o4NL - (o4ML + o4OL + o4PL);
			g4CL = o4OL - (o4ML + o4NL + o4PL);
			g4DL = o4PL - (o4ML + o4NL + o4OL);
			
			dualmonoSampleR = (o4ML + o4NL + o4OL + o4PL)*0.125;
			dualmonoSampleL = (o4AR + o4ER + o4IR + o4MR)*0.125;
			//dual mono version is wider = 1.0 at the center
			//with mainsample 0.0 and 2.0 (only at the edges)
			//with mainsample out of phase when over 1.0
			//couldn't re-do the arrays perfectly, so instead
			//we keep exactly the same cross-matrix,
			//but we flip the sides we're using for initial reverb.
			//then, dualmono remains totally dualmono, and blend a bit in for wideness.
			
			if (wider < 1.0) {
				inputSampleL = (dualmonoSampleL*wider) + (mainSampleL*(1.0-wider));
				inputSampleR = (dualmonoSampleR*wider) + (mainSampleR*(1.0-wider));
			} else {
				inputSampleL = (dualmonoSampleL*(2.0-wider)) + (mainSampleL*(wider-1.0));
				inputSampleR = (dualmonoSampleR*(2.0-wider)) + (-mainSampleR*(wider-1.0));
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