/*
*	File:		DeCrackle.cpp
*	
*	Version:	1.0
* 
*	Created:	2/20/25
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
	DeCrackle.cpp
	
=============================================================================*/
#include "DeCrackle.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(DeCrackle)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeCrackle::DeCrackle
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DeCrackle::DeCrackle(AudioUnit component)
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
//	DeCrackle::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeCrackle::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeCrackle::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeCrackle::GetParameterInfo(AudioUnitScope		inScope,
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
//	DeCrackle::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeCrackle::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 DeCrackle::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	DeCrackle::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DeCrackle::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	DeCrackle::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult DeCrackle::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____DeCrackleEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeCrackle::DeCrackleKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		DeCrackle::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for(int x = 0; x < kshort+2; x++) {aAL[x] = 0.0; aAR[x] = 0.0;}
	for(int x = 0; x < kshort+2; x++) {aBL[x] = 0.0; aBR[x] = 0.0;}
	for(int x = 0; x < kshort+2; x++) {aCL[x] = 0.0;}
	count = 1;
	for(int x = 0; x < 17; x++) {
		iirLSample[x] = 0.0;
		iirRSample[x] = 0.0;
		iirCSample[x] = 0.0;
		iirLAngle[x] = 0.0;
		iirRAngle[x] = 0.0;
	}
	iirTargetL = 0.0;
	iirTargetR = 0.0;
	iirClickL = 0.0;
	iirClickR = 0.0;
	prevSampleL = 0.0;
	prevSampleR = 0.0;
	prevSurfaceL = 0.0;
	prevSurfaceR = 0.0;
	prevOutL = 0.0;
	prevOutR = 0.0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DeCrackle::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		DeCrackle::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	int offset = (int)(overallscale*1.1);
	double maxHeight = 1.0*overallscale;
	double filterOut = pow((GetParameter( kParam_A )*0.618)+0.1,2.0)/overallscale;
	double filterRef = pow(((1.0-GetParameter( kParam_B ))*0.618)+0.1,2.0)/overallscale;
	double iirCut = (pow(1.0-GetParameter( kParam_B ),2.0)*0.2)/overallscale;
	int adjDelay = (int)(GetParameter( kParam_B )*((double)(kshort/8)*overallscale))-2.0;
	if (adjDelay > kshort - 1) adjDelay = kshort - 1; //don't break on silly samplerates
	int halfTrig = fmin(0.5+pow(GetParameter( kParam_B ),3.0),0.999)*adjDelay;
	int halfRaw = 0.5*adjDelay;
	int halfBez = 0.5*adjDelay;
	double threshold = pow(GetParameter( kParam_C )*0.618,2.0)-0.1;
	double surface = (1.0-pow(1.0-GetParameter( kParam_D ),3.0))*0.9;
	double wet = GetParameter( kParam_E );
	int options = 0; if (wet < 0.001) options = 1;
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL -= prevSampleL; prevSampleL = drySampleL;
		inputSampleR -= prevSampleR; prevSampleR = drySampleR;
		inputSampleL *= 16.0; inputSampleR *= 16.0;
		
		double bezL = drySampleL;
		for(int x = 0; x < 6; x++) {
			iirLAngle[x] = (iirLAngle[x]*(1.0-filterOut))+((bezL-iirLSample[x])*filterOut);
			bezL = ((iirLSample[x]+(iirLAngle[x]*filterOut))*(1.0-filterOut))+(bezL*filterOut);
			iirLSample[x] = ((iirLSample[x]+(iirLAngle[x]*filterOut))*(1.0-filterOut))+(bezL*filterOut);
		} //lowpass audio band
		double bezR = drySampleR;
		for(int x = 0; x < 6; x++) {
			iirRAngle[x] = (iirRAngle[x]*(1.0-filterOut))+((bezR-iirRSample[x])*filterOut);
			bezR = ((iirRSample[x]+(iirRAngle[x]*filterOut))*(1.0-filterOut))+(bezR*filterOut);
			iirRSample[x] = ((iirRSample[x]+(iirRAngle[x]*filterOut))*(1.0-filterOut))+(bezR*filterOut);
		} //lowpass audio band
		
		double rect = fabs(drySampleL*drySampleR*64.0);
		for(int x = 0; x < 6; x++) {
			rect = fabs((iirCSample[x]*(1.0-filterRef))+(rect*filterRef));
			iirCSample[x] = (iirCSample[x]*(1.0-filterRef))+(rect*filterRef);
		} //lowpass rectified CV band
		
		aAL[count] = drySampleL;
		aAR[count] = drySampleR;
		aBL[count] = bezL;
		aBR[count] = bezR;
		aCL[count] = rect;
		
		count++; if (count < 0 || count > adjDelay) count = 0;
		double near = rect;
		double far = aCL[count-((count > adjDelay)?adjDelay+1:0)];
		//near and far are the smoothed rectified version
		
		double prevL = aAL[(count+halfRaw+offset)-(((count+halfRaw+offset) > adjDelay)?adjDelay+1:0)];
		double prevR = aAR[(count+halfRaw+offset)-(((count+halfRaw+offset) > adjDelay)?adjDelay+1:0)];
		double outL = aAL[(count+halfRaw)-(((count+halfRaw) > adjDelay)?adjDelay+1:0)];
		double outR = aAR[(count+halfRaw)-(((count+halfRaw) > adjDelay)?adjDelay+1:0)];
		double outBezL = aBL[(count+halfBez)-(((count+halfBez) > adjDelay)?adjDelay+1:0)];
		double outBezR = aBR[(count+halfBez)-(((count+halfBez) > adjDelay)?adjDelay+1:0)];
		//these are the various outputs to fade between
		double trigL = aAL[(count+halfTrig)-(((count+halfTrig) > adjDelay)?adjDelay+1:0)];
		double trigR = aAR[(count+halfTrig)-(((count+halfTrig) > adjDelay)?adjDelay+1:0)];
		
		double deClickL = pow(fmax((fabs(trigL)-threshold)-fmax(near,far),0.0)*16.0,3.0);
		double deClickR = pow(fmax((fabs(trigR)-threshold)-fmax(near,far),0.0)*16.0,3.0);
		iirTargetL = fmax(iirTargetL-iirCut,0.0);
		iirTargetR = fmax(iirTargetR-iirCut,0.0); //taper down at iirCut speed
		if (deClickL > iirTargetL) iirTargetL = fmin(deClickL,maxHeight);
		if (deClickR > iirTargetR) iirTargetR = fmin(deClickR,maxHeight); //beyond 1.0 to stretch window
		if (deClickR*0.618 > iirTargetL) iirTargetL = fmin(deClickR*0.618,maxHeight);
		if (deClickL*0.618 > iirTargetR) iirTargetR = fmin(deClickL*0.618,maxHeight); //opposite channel
		iirClickL = fmin(iirClickL+iirCut,iirTargetL);
		iirClickR = fmin(iirClickR+iirCut,iirTargetR); //taper up on attack
		inputSampleL = (outBezL*fmin(iirClickL, 1.0)) + (outL*(1.0-fmin(iirClickL, 1.0)));
		inputSampleR = (outBezR*fmin(iirClickR, 1.0)) + (outR*(1.0-fmin(iirClickR, 1.0)));
		//declicking is applied: crossfade between darkened and normal
		
		if (wet < 1.0 && wet > 0.0) {
			inputSampleL = (inputSampleL*wet)+(outL*(1.0-wet));
			inputSampleR = (inputSampleR*wet)+(outR*(1.0-wet));
		} //neither full wet, nor the 'clicks only' special case below
		
		if (wet == 0.0) {
			inputSampleL = outL - inputSampleL;
			inputSampleR = outR - inputSampleR;
		} //clicks only at full dry
		
		double recordVolume = fmax(fmax(near,far),fmax(prevL,prevR))+0.001; //engage only at quietest
		double surfaceL = sin(fmin((fabs(outL-prevL)/recordVolume)*surface, 3.14159265358979))*0.5;
		double surfaceR = sin(fmin((fabs(outR-prevR)/recordVolume)*surface, 3.14159265358979))*0.5;
		double gateOnAudio = fmax(surface-(recordVolume*surface*4.0),0.0);
		if (surface > 0.0 && wet > 0.0) {
			inputSampleL = (prevOutL*surfaceL)+(inputSampleL*(1.0-surfaceL));
			inputSampleR = (prevOutR*surfaceR)+(inputSampleR*(1.0-surfaceR));
			inputSampleL = (prevOutL*gateOnAudio)+(inputSampleL*(1.0-gateOnAudio));
			inputSampleR = (prevOutR*gateOnAudio)+(inputSampleR*(1.0-gateOnAudio));
			prevOutL = (prevOutL*gateOnAudio)+(inputSampleL*(1.0-gateOnAudio));
			prevOutR = (prevOutR*gateOnAudio)+(inputSampleR*(1.0-gateOnAudio));
		} //if we're also adding surface noise reduction that goes here
		
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

