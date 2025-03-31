/*
*	File:		DubPlate2.cpp
*	
*	Version:	1.0
* 
*	Created:	3/21/25
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
	DubPlate2.cpp
	
=============================================================================*/
#include "DubPlate2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, DubPlate2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DubPlate2::DubPlate2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DubPlate2::DubPlate2(AudioUnit component)
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
//	DubPlate2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DubPlate2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DubPlate2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DubPlate2::GetParameterInfo(AudioUnitScope		inScope,
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
//	DubPlate2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DubPlate2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 DubPlate2::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	DubPlate2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			DubPlate2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	DubPlate2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult DubPlate2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____DubPlate2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DubPlate2::DubPlate2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		DubPlate2::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for (int x = 0; x < bax_total; x++) {baxH[x] = 0.0;baxL[x] = 0.0;}
	flip = false;
	iirA = 0.0;
	iirB = 0.0;
	iirC = 0.0;
	iirD = 0.0;
	lastSinewAL = 0.0;
	lastSinewAR = 0.0;
	lastSinewBL = 0.0;
	lastSinewBR = 0.0;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DubPlate2::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		DubPlate2::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	
	double inputGain = pow(GetParameter( kParam_A )*2.0,2.0);
	double trebleGain = pow(GetParameter( kParam_B )*2.0,2.0);
	double trebleFreq = ((2000.0*trebleGain)+200.0)/GetSampleRate();
	if (trebleFreq > 0.45) trebleFreq = 0.45;
	baxH[bax_freq] = trebleFreq;
    baxH[bax_reso] = 0.57735026919; //bessel second order
	double K = tan(M_PI * baxH[bax_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / baxH[bax_reso] + K * K);
	baxH[bax_a0] = K * K * norm;
	baxH[bax_a1] = 2.0 * baxH[bax_a0];
	baxH[bax_a2] = baxH[bax_a0];
	baxH[bax_b1] = 2.0 * (K * K - 1.0) * norm;
	baxH[bax_b2] = (1.0 - K / baxH[bax_reso] + K * K) * norm;
	//end bax highpass
	double bassGain = pow(GetParameter( kParam_C )*2.0,2.0);
	double bassFreq = pow((1.0-GetParameter( kParam_C ))*2.0,2.0);
	bassFreq = ((2000.0*bassFreq)+200.0)/GetSampleRate();
	if (bassFreq > 0.45) bassFreq = 0.45;
	baxL[bax_freq] = bassFreq;
    baxL[bax_reso] = 0.57735026919; //bessel second order
	K = tan(M_PI * baxL[bax_freq]); //lowpass
	norm = 1.0 / (1.0 + K / baxL[bax_reso] + K * K);
	baxL[bax_a0] = K * K * norm;
	baxL[bax_a1] = 2.0 * baxL[bax_a0];
	baxL[bax_a2] = baxL[bax_a0];
	baxL[bax_b1] = 2.0 * (K * K - 1.0) * norm;
	baxL[bax_b2] = (1.0 - K / baxL[bax_reso] + K * K) * norm;
	//end bax lowpass
	
	double iirSide = 0.01862 / overallscale;
	double iirMid = 0.01102 / overallscale;
	double threshSinewA = 0.1442 / overallscale;
	double threshSinewB = 0.0274 / overallscale;	
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL = sin(fmax(fmin(inputSampleL*inputGain,M_PI_2),-M_PI_2));
		inputSampleR = sin(fmax(fmin(inputSampleR*inputGain,M_PI_2),-M_PI_2));
		//encode Console5: good cleanness
		double trebleSampleL;
		double trebleSampleR;
		double bassSampleL;
		double bassSampleR;
		if (flip) {
			trebleSampleL = (inputSampleL * baxH[bax_a0]) + baxH[bax_sLA1];
			baxH[bax_sLA1] = (inputSampleL * baxH[bax_a1]) - (trebleSampleL * baxH[bax_b1]) + baxH[bax_sLA2];
			baxH[bax_sLA2] = (inputSampleL * baxH[bax_a2]) - (trebleSampleL * baxH[bax_b2]);
			trebleSampleL = inputSampleL - trebleSampleL;
			trebleSampleR = (inputSampleR * baxH[bax_a0]) + baxH[bax_sRA1];
			baxH[bax_sRA1] = (inputSampleR * baxH[bax_a1]) - (trebleSampleR * baxH[bax_b1]) + baxH[bax_sRA2];
			baxH[bax_sRA2] = (inputSampleR * baxH[bax_a2]) - (trebleSampleR * baxH[bax_b2]);
			trebleSampleR = inputSampleR - trebleSampleR;
			bassSampleL = (inputSampleL * baxL[bax_a0]) + baxL[bax_sLA1];
			baxL[bax_sLA1] = (inputSampleL * baxL[bax_a1]) - (bassSampleL * baxL[bax_b1]) + baxL[bax_sLA2];
			baxL[bax_sLA2] = (inputSampleL * baxL[bax_a2]) - (bassSampleL * baxL[bax_b2]);
			bassSampleR = (inputSampleR * baxL[bax_a0]) + baxL[bax_sRA1];
			baxL[bax_sRA1] = (inputSampleR * baxL[bax_a1]) - (bassSampleR * baxL[bax_b1]) + baxL[bax_sRA2];
			baxL[bax_sRA2] = (inputSampleR * baxL[bax_a2]) - (bassSampleR * baxL[bax_b2]);
		} else {
			trebleSampleL = (inputSampleL * baxH[bax_a0]) + baxH[bax_sLB1];
			baxH[bax_sLB1] = (inputSampleL * baxH[bax_a1]) - (trebleSampleL * baxH[bax_b1]) + baxH[bax_sLB2];
			baxH[bax_sLB2] = (inputSampleL * baxH[bax_a2]) - (trebleSampleL * baxH[bax_b2]);
			trebleSampleL = inputSampleL - trebleSampleL;
			trebleSampleR = (inputSampleR * baxH[bax_a0]) + baxH[bax_sRB1];
			baxH[bax_sRB1] = (inputSampleR * baxH[bax_a1]) - (trebleSampleR * baxH[bax_b1]) + baxH[bax_sRB2];
			baxH[bax_sRB2] = (inputSampleR * baxH[bax_a2]) - (trebleSampleR * baxH[bax_b2]);
			trebleSampleR = inputSampleR - trebleSampleR;
			bassSampleL = (inputSampleL * baxL[bax_a0]) + baxL[bax_sLB1];
			baxL[bax_sLB1] = (inputSampleL * baxL[bax_a1]) - (bassSampleL * baxL[bax_b1]) + baxL[bax_sLB2];
			baxL[bax_sLB2] = (inputSampleL * baxL[bax_a2]) - (bassSampleL * baxL[bax_b2]);
			bassSampleR = (inputSampleR * baxL[bax_a0]) + baxL[bax_sRB1];
			baxL[bax_sRB1] = (inputSampleR * baxL[bax_a1]) - (bassSampleR * baxL[bax_b1]) + baxL[bax_sRB2];
			baxL[bax_sRB2] = (inputSampleR * baxL[bax_a2]) - (bassSampleR * baxL[bax_b2]);
		}
		
		trebleSampleL *= trebleGain;
		trebleSampleR *= trebleGain;
		bassSampleL *= bassGain;
		bassSampleR *= bassGain;
		inputSampleL = bassSampleL + trebleSampleL; //interleaved biquad
		inputSampleR = bassSampleR + trebleSampleR; //interleaved biquad
		
		inputSampleL = asin(fmax(fmin(inputSampleL,0.99999),-0.99999));
		inputSampleR = asin(fmax(fmin(inputSampleR,0.99999),-0.99999));
		//amplitude aspect
		
		double mid = inputSampleL + inputSampleR;
		double side = inputSampleL - inputSampleR;
		//assign mid and side.Between these sections, you can do mid/side processing
		double temp = side;
		double correction;
		
		if (flip) {
			iirA = (iirA * (1.0 - iirSide)) + (temp * iirSide); temp -= iirA; correction = iirA;
		} else {
			iirB = (iirB * (1.0 - iirSide)) + (temp * iirSide); temp -= iirB; correction = iirB;
		}
		iirC = (iirC * (1.0 - iirSide)) + (temp * iirSide); temp -= iirC; correction += (iirC * 0.162);

		side -= sin(correction);
		
		iirD = (iirD * (1.0 - iirMid)) + (mid * iirMid);
		mid -= sin(iirD);
		//gonna cut those lows a hair
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		//unassign mid and side
		
		temp = inputSampleL;
		double sinew = threshSinewA * cos(lastSinewAL*lastSinewAL);
		if (inputSampleL - lastSinewAL > sinew) temp = lastSinewAL + sinew;
		if (-(inputSampleL - lastSinewAL) > sinew) temp = lastSinewAL - sinew;
		lastSinewAL = temp;
		if (lastSinewAL > 1.0) lastSinewAL = 1.0;
		if (lastSinewAL < -1.0) lastSinewAL = -1.0;
		inputSampleL = (inputSampleL * 0.5)+(lastSinewAL * 0.5);
		sinew = threshSinewB * cos(lastSinewBL*lastSinewBL);
		if (inputSampleL - lastSinewBL > sinew) temp = lastSinewBL + sinew;
		if (-(inputSampleL - lastSinewBL) > sinew) temp = lastSinewBL - sinew;
		lastSinewBL = temp;
		if (lastSinewBL > 1.0) lastSinewBL = 1.0;
		if (lastSinewBL < -1.0) lastSinewBL = -1.0;
		inputSampleL = (inputSampleL * 0.5)+(lastSinewBL * 0.5);
		
		temp = inputSampleR;
		sinew = threshSinewA * cos(lastSinewAR*lastSinewAR);
		if (inputSampleR - lastSinewAR > sinew) temp = lastSinewAR + sinew;
		if (-(inputSampleR - lastSinewAR) > sinew) temp = lastSinewAR - sinew;
		lastSinewAR = temp;
		if (lastSinewAR > 1.0) lastSinewAR = 1.0;
		if (lastSinewAR < -1.0) lastSinewAR = -1.0;
		inputSampleR = (inputSampleR * 0.5)+(lastSinewAR * 0.5);
		sinew = threshSinewB * cos(lastSinewBR*lastSinewBR);
		if (inputSampleR - lastSinewBR > sinew) temp = lastSinewBR + sinew;
		if (-(inputSampleR - lastSinewBR) > sinew) temp = lastSinewBR - sinew;
		lastSinewBR = temp;
		if (lastSinewBR > 1.0) lastSinewBR = 1.0;
		if (lastSinewBR < -1.0) lastSinewBR = -1.0;
		inputSampleR = (inputSampleR * 0.5)+(lastSinewBR * 0.5);
		//run Sinew to stop excess slews, two layers to make it more audible
		
		flip = !flip; //both for Baxandall3 and DubPlate highpass

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

