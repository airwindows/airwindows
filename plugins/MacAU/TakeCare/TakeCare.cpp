/*
*	File:		TakeCare.cpp
*	
*	Version:	1.0
* 
*	Created:	7/29/25
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
	TakeCare.cpp
	
=============================================================================*/
#include "TakeCare.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(TakeCare)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TakeCare::TakeCare
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TakeCare::TakeCare(AudioUnit component)
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
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TakeCare::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			TakeCare::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TakeCare::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			TakeCare::GetParameterInfo(AudioUnitScope		inScope,
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
//	TakeCare::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			TakeCare::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 TakeCare::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	TakeCare::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			TakeCare::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	TakeCare::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult TakeCare::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____TakeCareEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TakeCare::TakeCareKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		TakeCare::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for(int x = 0; x < 32767+2; x++) {a3AL[x] = 0.0; a3AR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3BL[x] = 0.0; a3BR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3CL[x] = 0.0; a3CR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3DL[x] = 0.0; a3DR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3EL[x] = 0.0; a3ER[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3FL[x] = 0.0; a3FR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3GL[x] = 0.0; a3GR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3HL[x] = 0.0; a3HR[x] = 0.0;}
	for(int x = 0; x < 32767+2; x++) {a3IL[x] = 0.0; a3IR[x] = 0.0;}
	c3AL = c3BL = c3CL = c3DL = c3EL = c3FL = c3GL = c3HL = c3IL = 1;
	c3AR = c3BR = c3CR = c3DR = c3ER = c3FR = c3GR = c3HR = c3IR = 1;
	f3AL = f3BL = f3CL = 0.0;
	f3CR = f3FR = f3IR = 0.0;
	avg3L = avg3R = 0.0;
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	
	rotate = 0.0;
	oldfpd = 0.4294967295;
	
	buf = 8192;
	vibDepth = 0.0;
	outA = 1.0;
	outB = 1.0;
	wetA = 1.0;
	wetB = 1.0;
	derezA = 0.5;
	derezB = 0.5;

	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	lastSampleR = 0.0;
	wasPosClipR = false;
	wasNegClipR = false;

	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TakeCare::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		TakeCare::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	double vibSpeed = pow(GetParameter( kParam_A ),5.0) * 0.1;
	double vibRandom = pow(GetParameter( kParam_B ),3.0) * 0.99;
	double targetDepth = pow(GetParameter( kParam_C ),2.0) * 0.5;
	double reg3n = GetParameter( kParam_D )*0.0625;
	
	derezA = derezB; derezB = GetParameter( kParam_E )*2.0;
	bool stepped = true; // Revised Bezier Undersampling
	if (derezB > 1.0) {  // has full rez at center, stepped
		stepped = false; // to left, continuous to right
		derezB = 1.0-(derezB-1.0);
	} //if it's set up like that it's the revised algorithm
	derezB = fmin(fmax(derezB/overallscale,0.0005),1.0);
	int bezFraction = (int)(1.0/derezB);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	if (stepped) { //this hard-locks derez to exact subdivisions of 1.0
		derezB = 1.0 / bezFraction;
		bezTrim = 1.0-(derezB*bezTrim);
	} else { //this makes it match the 1.0 case using stepped
		derezB /= (2.0/pow(overallscale,0.5-((overallscale-1.0)*0.0375)));
		bezTrim = 1.0-pow(derezB*0.5,1.0/(derezB*0.5));
	} //the revision more accurately connects the bezier curves
	
	int targetBuf = (pow(GetParameter( kParam_F ),3.0)*32510.0)+256;
	outA = outB; outB = GetParameter( kParam_G );
	wetA = wetB; wetB = 1.0-pow(1.0-GetParameter( kParam_H ),2.0);
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		double temp = (double)nSampleFrames/inFramesToProcess;
		double derez = (derezA*temp)+(derezB*(1.0-temp));
		double out = (outA*temp)+(outB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
				
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
			
			rotate += (oldfpd*vibSpeed);
			if (rotate > (M_PI*2.0)) {
				rotate -= (M_PI*2.0);
				oldfpd = (1.0-vibRandom) + ((fpdL*0.000000000618)*vibRandom);
			}
			vibDepth = (vibDepth*0.99)+(targetDepth*0.01);
			if (buf < targetBuf) buf++;
			if (buf > targetBuf) buf--;
			
			double mA = (sin(rotate)+1.0)*vibDepth*buf;
			double mB = (sin(rotate+(M_PI/9.0))+1.0)*vibDepth*buf;
			double mC = (sin(rotate+((M_PI/9.0)*2.0))+1.0)*vibDepth*buf;
			double mD = (sin(rotate+((M_PI/9.0)*3.0))+1.0)*vibDepth*buf;
			double mE = (sin(rotate+((M_PI/9.0)*4.0))+1.0)*vibDepth*buf;
			double mF = (sin(rotate+((M_PI/9.0)*5.0))+1.0)*vibDepth*buf;
			double mG = (sin(rotate+((M_PI/9.0)*6.0))+1.0)*vibDepth*buf;
			double mH = (sin(rotate+((M_PI/9.0)*7.0))+1.0)*vibDepth*buf;
			double mI = (sin(rotate+((M_PI/9.0)*8.0))+1.0)*vibDepth*buf;		

			inputSampleL = sin(fmin(fmax(inputSampleL*0.5,-M_PI_2),M_PI_2));
			inputSampleR = sin(fmin(fmax(inputSampleR*0.5,-M_PI_2),M_PI_2));
			
			a3AL[c3AL] = inputSampleL + fmin(fmax(f3AL*reg3n,-M_PI),M_PI);
			a3BL[c3BL] = inputSampleL + fmin(fmax(f3BL*reg3n,-M_PI),M_PI);
			a3CL[c3CL] = inputSampleL + fmin(fmax(f3CL*reg3n,-M_PI),M_PI);
			
			a3CR[c3CR] = inputSampleR + fmin(fmax(f3CR*reg3n,-M_PI),M_PI);
			a3FR[c3FR] = inputSampleR + fmin(fmax(f3FR*reg3n,-M_PI),M_PI);
			a3IR[c3IR] = inputSampleR + fmin(fmax(f3IR*reg3n,-M_PI),M_PI);
			
			c3AL++; if (c3AL < 0 || c3AL > buf) c3AL = 0;
			c3BL++; if (c3BL < 0 || c3BL > buf) c3BL = 0;
			c3CL++; if (c3CL < 0 || c3CL > buf) c3CL = 0;
			c3CR++; if (c3CR < 0 || c3CR > buf) c3CR = 0;
			c3FR++; if (c3FR < 0 || c3FR > buf) c3FR = 0;
			c3IR++; if (c3IR < 0 || c3IR > buf) c3IR = 0;
			
			double o3AL = a3AL[(int)(c3AL+mA-((c3AL+mA>buf)?buf+1:0))] *(1.0-(mA-floor(mA)));
			o3AL += (a3AL[(int)(c3AL+mA+1-((c3AL+mA+1>buf)?buf+1:0))] *((mA-floor(mA))));
			double o3BL = a3BL[(int)(c3BL+mB-((c3BL+mB>buf)?buf+1:0))] *(1.0-(mB-floor(mB)));
			o3BL += (a3BL[(int)(c3BL+mB+1-((c3BL+mB+1>buf)?buf+1:0))] *((mB-floor(mB))));
			double o3CL = a3CL[(int)(c3CL+mC-((c3CL+mC>buf)?buf+1:0))] *(1.0-(mC-floor(mC)));
			o3CL += (a3CL[(int)(c3CL+mC+1-((c3CL+mC+1>buf)?buf+1:0))] *((mC-floor(mC))));
			double o3CR = a3CR[(int)(c3CR+mC-((c3CR+mC>buf)?buf+1:0))] *(1.0-(mC-floor(mC)));
			o3CR += (a3CR[(int)(c3CR+mC+1-((c3CR+mC+1>buf)?buf+1:0))] *((mC-floor(mC))));
			double o3FR = a3FR[(int)(c3FR+mF-((c3FR+mF>buf)?buf+1:0))] *(1.0-(mF-floor(mF)));
			o3FR += (a3FR[(int)(c3FR+mF+1-((c3FR+mF+1>buf)?buf+1:0))] *((mF-floor(mF))));
			double o3IR = a3IR[(int)(c3IR+mI-((c3IR+mI>buf)?buf+1:0))] *(1.0-(mI-floor(mI)));
			o3IR += (a3IR[(int)(c3IR+mI+1-((c3IR+mI+1>buf)?buf+1:0))] *((mI-floor(mI))));
			
			a3DL[c3DL] = (((o3BL + o3CL) * -2.0) + o3AL);
			a3EL[c3EL] = (((o3AL + o3CL) * -2.0) + o3BL);
			a3FL[c3FL] = (((o3AL + o3BL) * -2.0) + o3CL);
			a3BR[c3BR] = (((o3FR + o3IR) * -2.0) + o3CR);
			a3ER[c3ER] = (((o3CR + o3IR) * -2.0) + o3FR);
			a3HR[c3HR] = (((o3CR + o3FR) * -2.0) + o3IR);
			
			c3DL++; if (c3DL < 0 || c3DL > buf) c3DL = 0;
			c3EL++; if (c3EL < 0 || c3EL > buf) c3EL = 0;
			c3FL++; if (c3FL < 0 || c3FL > buf) c3FL = 0;
			c3BR++; if (c3BR < 0 || c3BR > buf) c3BR = 0;
			c3ER++; if (c3ER < 0 || c3ER > buf) c3ER = 0;
			c3HR++; if (c3HR < 0 || c3HR > buf) c3HR = 0;
						
			double o3DL = a3DL[(int)(c3DL+mD-((c3DL+mD>buf)?buf+1:0))] *(1.0-(mD-floor(mD)));
			o3DL += (a3DL[(int)(c3DL+mD+1-((c3DL+mD+1>buf)?buf+1:0))] *((mD-floor(mD))));
			double o3EL = a3EL[(int)(c3EL+mE-((c3EL+mE>buf)?buf+1:0))] *(1.0-(mE-floor(mE)));
			o3EL += (a3EL[(int)(c3EL+mE+1-((c3EL+mE+1>buf)?buf+1:0))] *((mE-floor(mE))));
			double o3FL = a3FL[(int)(c3FL+mF-((c3FL+mF>buf)?buf+1:0))] *(1.0-(mF-floor(mF)));
			o3FL += (a3FL[(int)(c3FL+mF+1-((c3FL+mF+1>buf)?buf+1:0))] *((mF-floor(mF))));
			double o3BR = a3BR[(int)(c3BR+mB-((c3BR+mB>buf)?buf+1:0))] *(1.0-(mB-floor(mB)));
			o3BR += (a3BR[(int)(c3BR+mB+1-((c3BR+mB+1>buf)?buf+1:0))] *((mB-floor(mB))));
			double o3ER = a3ER[(int)(c3ER+mE-((c3ER+mE>buf)?buf+1:0))] *(1.0-(mE-floor(mE)));
			o3ER += (a3ER[(int)(c3ER+mE+1-((c3ER+mE+1>buf)?buf+1:0))] *((mE-floor(mE))));
			double o3HR = a3HR[(int)(c3HR+mH-((c3HR+mH>buf)?buf+1:0))] *(1.0-(mH-floor(mH)));
			o3HR += (a3HR[(int)(c3HR+mH+1-((c3HR+mH+1>buf)?buf+1:0))] *((mH-floor(mH))));
			
			a3GL[c3GL] = (((o3EL + o3FL) * -2.0) + o3DL);
			a3HL[c3HL] = (((o3DL + o3FL) * -2.0) + o3EL);
			a3IL[c3IL] = (((o3DL + o3EL) * -2.0) + o3FL);
			a3AR[c3AR] = (((o3ER + o3HR) * -2.0) + o3BR);
			a3DR[c3DR] = (((o3BR + o3HR) * -2.0) + o3ER);
			a3GR[c3GR] = (((o3BR + o3ER) * -2.0) + o3HR);
			
			
			c3GL++; if (c3GL < 0 || c3GL > buf) c3GL = 0;
			c3HL++; if (c3HL < 0 || c3HL > buf) c3HL = 0;
			c3IL++; if (c3IL < 0 || c3IL > buf) c3IL = 0;
			c3AR++; if (c3AR < 0 || c3AR > buf) c3AR = 0;
			c3DR++; if (c3DR < 0 || c3DR > buf) c3DR = 0;
			c3GR++; if (c3GR < 0 || c3GR > buf) c3GR = 0;
			
			double o3GL = a3GL[(int)(c3GL+mG-((c3GL+mG>buf)?buf+1:0))] *(1.0-(mG-floor(mG)));
			o3GL += (a3GL[(int)(c3GL+mG+1-((c3GL+mG+1>buf)?buf+1:0))] *((mG-floor(mG))));
			double o3HL = a3HL[(int)(c3HL+mH-((c3HL+mH>buf)?buf+1:0))] *(1.0-(mH-floor(mH)));
			o3HL += (a3HL[(int)(c3HL+mH+1-((c3HL+mH+1>buf)?buf+1:0))] *((mH-floor(mH))));
			double o3IL = a3IL[(int)(c3IL+mI-((c3IL+mI>buf)?buf+1:0))] *(1.0-(mI-floor(mI)));
			o3IL += (a3IL[(int)(c3IL+mI+1-((c3IL+mI+1>buf)?buf+1:0))] *((mI-floor(mI))));
			double o3AR = a3AR[(int)(c3AR+mA-((c3AR+mA>buf)?buf+1:0))] *(1.0-(mA-floor(mA)));
			o3AR += (a3AR[(int)(c3AR+mA+1-((c3AR+mA+1>buf)?buf+1:0))] *((mA-floor(mA))));
			double o3DR = a3DR[(int)(c3DR+mD-((c3DR+mD>buf)?buf+1:0))] *(1.0-(mD-floor(mD)));
			o3DR += (a3DR[(int)(c3DR+mD+1-((c3DR+mD+1>buf)?buf+1:0))] *((mD-floor(mD))));
			double o3GR = a3GR[(int)(c3GR+mG-((c3GR+mG>buf)?buf+1:0))] *(1.0-(mG-floor(mG)));
			o3GR += (a3GR[(int)(c3GR+mG+1-((c3GR+mG+1>buf)?buf+1:0))] *((mG-floor(mG))));
			
			f3AL = (((o3GR + o3HR) * -2.0) + o3IR);				
			f3BL = (((o3GR + o3HR) * -2.0) + o3IR);
			f3CL = (((o3GR + o3HR) * -2.0) + o3IR);
			
			f3CR = (((o3AL + o3DL) * -2.0) + o3GL);
			f3FR = (((o3AL + o3DL) * -2.0) + o3GL);
			f3IR = (((o3AL + o3DL) * -2.0) + o3GL);
			
			double inputSampleL = (o3GL + o3HL + o3IL)*0.03125;
			double inputSampleR = (o3AR + o3DR + o3GR)*0.03125;
			
			f3AL = (f3AL+f3AL+f3AL+fabs(avg3L))*0.25; avg3L = f3AL;
			f3CR = (f3CR+f3CR+f3CR+fabs(avg3R))*0.25; avg3R = f3CR;
			//manipulating deep reverb tail for realism	
			
			//begin ClipOnly stereo as a little, compressed chunk that can be dropped into undersampled code
			inputSampleL = fmin(fmax(inputSampleL,-4.0),4.0);
			if (wasPosClipL == true) { //current will be over
				if (inputSampleL<lastSampleL) lastSampleL=0.79+(inputSampleL*0.2);
				else lastSampleL = 0.2+(lastSampleL*0.79);
			} wasPosClipL = false;
			if (inputSampleL>0.99) {wasPosClipL=true;inputSampleL=0.79+(lastSampleL*0.2);}
			if (wasNegClipL == true) { //current will be -over
				if (inputSampleL > lastSampleL) lastSampleL=-0.79+(inputSampleL*0.2);
				else lastSampleL=-0.2+(lastSampleL*0.79);
			} wasNegClipL = false;
			if (inputSampleL<-0.99) {wasNegClipL=true;inputSampleL=-0.79+(lastSampleL*0.2);}
			lastSampleL = inputSampleL;
			inputSampleR = fmin(fmax(inputSampleR,-4.0),4.0);
			if (wasPosClipR == true) { //current will be over
				if (inputSampleR<lastSampleR) lastSampleR=0.79+(inputSampleR*0.2);
				else lastSampleR = 0.2+(lastSampleR*0.79);
			} wasPosClipR = false;
			if (inputSampleR>0.99) {wasPosClipR=true;inputSampleR=0.79+(lastSampleR*0.2);}
			if (wasNegClipR == true) { //current will be -over
				if (inputSampleR > lastSampleR) lastSampleR=-0.79+(inputSampleR*0.2);
				else lastSampleR=-0.2+(lastSampleR*0.79);
			} wasNegClipR = false;
			if (inputSampleR<-0.99) {wasNegClipR=true;inputSampleR=-0.79+(lastSampleR*0.2);}
			lastSampleR = inputSampleR;
			//end ClipOnly stereo as a little, compressed chunk that can be dropped into undersampled code							
			
			inputSampleL = asin(inputSampleL*0.7);
			inputSampleR = asin(inputSampleR*0.7);
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		
		double X = bez[bez_cycle] * bezTrim;
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*-0.25;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*-0.25;
		inputSampleL = CBAL+bez[bez_AvgOutSampL]; bez[bez_AvgOutSampL] = CBAL;
		inputSampleR = CBAR+bez[bez_AvgOutSampR]; bez[bez_AvgOutSampR] = CBAR;
		
		if (out != 1.0) {
			inputSampleL *= out;
			inputSampleR *= out;
		}
				
		if (wet != 1.0) {
		 inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
		 inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
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

