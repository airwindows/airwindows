/*
 *	File:		StereoEnsemble.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	2/7/22
 *	
 *	Copyright:  Copyright © 2022 Airwindows, Airwindows uses the MIT license
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
 StereoEnsemble.cpp
 
 =============================================================================*/
#include "StereoEnsemble.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, StereoEnsemble)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StereoEnsemble::StereoEnsemble
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StereoEnsemble::StereoEnsemble(AudioUnit component)
: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StereoEnsemble::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StereoEnsemble::GetParameterValueStrings(AudioUnitScope		inScope,
																 AudioUnitParameterID	inParameterID,
																 CFArrayRef *		outStrings)
{
	
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StereoEnsemble::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StereoEnsemble::GetParameterInfo(AudioUnitScope		inScope,
														 AudioUnitParameterID	inParameterID,
														 AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;
	
	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
	|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
			case kParam_One:
                AUBase::FillInParameterName (outParameterInfo, kParameterOneName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
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
//	StereoEnsemble::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StereoEnsemble::GetPropertyInfo (AudioUnitPropertyID	inID,
														 AudioUnitScope		inScope,
														 AudioUnitElement	inElement,
														 UInt32 &		outDataSize,
														 Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 StereoEnsemble::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	StereoEnsemble::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StereoEnsemble::GetProperty(	AudioUnitPropertyID inID,
													AudioUnitScope 		inScope,
													AudioUnitElement 	inElement,
													void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	StereoEnsemble::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult StereoEnsemble::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____StereoEnsembleEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StereoEnsemble::StereoEnsembleKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		StereoEnsemble::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for(int count = 0; count < 7490; count++) {dA[count] = 0.0;}
	for(int count = 0; count < 7532; count++) {dB[count] = 0.0;}

	for(int count = 0; count < 5788; count++) {dC[count] = 0.0;}
	for(int count = 0; count < 5746; count++) {dD[count] = 0.0;}

	for(int count = 0; count < 4840; count++) {dE[count] = 0.0;}
	for(int count = 0; count < 4870; count++) {dF[count] = 0.0;}

	for(int count = 0; count < 3118; count++) {dG[count] = 0.0;}
	for(int count = 0; count < 3088; count++) {dH[count] = 0.0;}

	for(int count = 0; count < 2212; count++) {dI[count] = 0.0;}
	for(int count = 0; count < 2222; count++) {dJ[count] = 0.0;}

	for(int count = 0; count < 1336; count++) {dK[count] = 0.0;}
	for(int count = 0; count < 1330; count++) {dL[count] = 0.0;}
	
	oneA = 1;
	oneB = 1;
	oneC = 1;
	oneD = 1;
	oneE = 1;
	oneF = 1;
	oneG = 1;
	oneH = 1;
	oneI = 1;
	oneJ = 1;
	oneK = 1;
	oneL = 1;
		
	levelA = 0.75;
	levelB = 0.15;

	levelC = 0.15;
	levelD = 0.75;
	
	levelE = 0.63;
	levelF = 0.27;
	
	levelG = 0.27;
	levelH = 0.63;
	
	levelI = 0.48;
	levelJ = 0.32;
	
	levelK = 0.32;
	levelL = 0.48;
		
	for(int count = 0; count < 6; count++) {lastRefL[count] = 0.0;lastRefR[count] = 0.0;}
	cycle = 0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StereoEnsemble::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		StereoEnsemble::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	double delayfactor = 0.66 + (GetParameter( kParam_One )/3.0);
	
	double outlevel = GetParameter( kParam_Two );
	
	dryL = 1.0 - (outlevel * 0.65);
	dryR = 1.0 - (outlevel * 0.65);
	
	maxdelayA = (int)(7481.0 * delayfactor);
	maxdelayB = (int)(7523.0 * delayfactor);

	maxdelayC = (int)(5779.0 * delayfactor);
	maxdelayD = (int)(5737.0 * delayfactor);
	
	maxdelayE = (int)(4831.0 * delayfactor);
	maxdelayF = (int)(4861.0 * delayfactor);
	
	maxdelayG = (int)(3109.0 * delayfactor);
	maxdelayH = (int)(3079.0 * delayfactor);
	
	maxdelayI = (int)(2203.0 * delayfactor);
	maxdelayJ = (int)(2213.0 * delayfactor);
	
	maxdelayK = (int)(1327.0 * delayfactor);
	maxdelayL = (int)(1321.0 * delayfactor);
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a ensemble sample
			double drySampleL = inputSampleL;
			double drySampleR = inputSampleR;
			double outL = 0.0;
			double outR = 0.0;
			double temp = 0.0;
			
			dA[oneA] = inputSampleL;
			oneA--; if (oneA < 0 || oneA > maxdelayA) {oneA = maxdelayA;} temp = dA[oneA];
			outL += (temp*levelA);
			
			dB[oneB] = inputSampleR;
			oneB--; if (oneB < 0 || oneB > maxdelayB) {oneB = maxdelayB;} temp += dB[oneB];
			outR += (temp*levelB);
			
			dC[oneC] = inputSampleL;
			oneC--; if (oneC < 0 || oneC > maxdelayC) {oneC = maxdelayC;} temp = dC[oneC];
			outL += (temp*levelC);
			
			dD[oneD] = inputSampleR;
			oneD--; if (oneD < 0 || oneD > maxdelayD) {oneD = maxdelayD;} temp += dD[oneD];
			outR += (temp*levelD);
			
			dE[oneE] = inputSampleL;
			oneE--; if (oneE < 0 || oneE > maxdelayE) {oneE = maxdelayE;} temp = dE[oneE];
			outL += (temp*levelE);
			
			dF[oneF] = inputSampleR;
			oneF--; if (oneF < 0 || oneF > maxdelayF) {oneF = maxdelayF;} temp += dF[oneF];
			outR += (temp*levelF);
			
			dG[oneG] = inputSampleL;
			oneG--; if (oneG < 0 || oneG > maxdelayG) {oneG = maxdelayG;} temp = dG[oneG];
			outL += (temp*levelG);
			
			dH[oneH] = inputSampleR;
			oneH--; if (oneH < 0 || oneH > maxdelayH) {oneH = maxdelayH;} temp += dH[oneH];
			outR += (temp*levelH);
			
			dI[oneI] = inputSampleL;
			oneI--; if (oneI < 0 || oneI > maxdelayI) {oneI = maxdelayI;} temp = dI[oneI];
			outL += (temp*levelI);
			
			dJ[oneJ] = inputSampleR;
			oneJ--; if (oneJ < 0 || oneJ > maxdelayJ) {oneJ = maxdelayJ;} temp += dJ[oneJ];
			outR += (temp*levelJ);
			
			dK[oneK] = inputSampleL;
			oneK--; if (oneK < 0 || oneK > maxdelayK) {oneK = maxdelayK;} temp = dK[oneK];
			outL += (temp*levelK);
			
			dL[oneL] = inputSampleR;
			oneL--; if (oneL < 0 || oneL > maxdelayL) {oneL = maxdelayL;} temp += dL[oneL];
			outR += (temp*levelL);
			
			inputSampleL = (outL * outlevel) + (drySampleL * dryL);
			inputSampleR = (outR * outlevel) + (drySampleR * dryR);
			
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		
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

