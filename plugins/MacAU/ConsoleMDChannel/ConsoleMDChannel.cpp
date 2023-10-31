/*
*	File:		ConsoleMDChannel.cpp
*	
*	Version:	1.0
* 
*	Created:	10/30/23
*	
*	Copyright:  Copyright © 2023 Airwindows, Airwindows uses the MIT license
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
	ConsoleMDChannel.cpp
	
=============================================================================*/
#include "ConsoleMDChannel.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(ConsoleMDChannel)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleMDChannel::ConsoleMDChannel
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ConsoleMDChannel::ConsoleMDChannel(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
	SetParameter(kParam_Five, kDefaultValue_ParamFive );
	SetParameter(kParam_Six, kDefaultValue_ParamSix );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleMDChannel::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleMDChannel::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleMDChannel::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleMDChannel::GetParameterInfo(AudioUnitScope		inScope,
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
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
                break;
           case kParam_Four:
                AUBase::FillInParameterName (outParameterInfo, kParameterFourName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFour;
                break;
           case kParam_Five:
                AUBase::FillInParameterName (outParameterInfo, kParameterFiveName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFive;
                break;
           case kParam_Six:
                AUBase::FillInParameterName (outParameterInfo, kParameterSixName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSix;
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
//	ConsoleMDChannel::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleMDChannel::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 ConsoleMDChannel::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	ConsoleMDChannel::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ConsoleMDChannel::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ConsoleMDChannel::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ConsoleMDChannel::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ConsoleMDChannelEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleMDChannel::ConsoleMDChannelKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		ConsoleMDChannel::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for (int x = 0; x < 17; x++) pearA[x] = 0.0;
	for (int x = 0; x < 21; x++) pearB[x] = 0.0;
	for(int count = 0; count < 2004; count++) {mpkL[count] = 0.0; mpkR[count] = 0.0;}
	for(int count = 0; count < 65; count++) {f[count] = 0.0;}
	prevfreqMPeak = -1;
	prevamountMPeak = -1;
	mpc = 1;	
	bassA = bassB = 0.0;
	gainA = gainB = 1.0;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ConsoleMDChannel::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		ConsoleMDChannel::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	overallscale *= GetSampleRate(); //will be over 1.0848 when over 48k
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 3) cycleEnd = 3;
	
	double fatTreble = (GetParameter( kParam_One )*6.0)-3.0;
	bassA = bassB;
	bassB = (GetParameter( kParam_Four )*6.0)-3.0;
	//these should stack to go up to -3.0 to 3.0
	if (fatTreble < 0.0) fatTreble /= 3.0;
	if (bassB < 0.0) bassB /= 3.0;
	//and then become -1.0 to 3.0;
	//there will be successive sin/cos stages w. dry/wet in these
	double freqTreble = 0.853;
	double freqMid = 0.026912;
	switch (cycleEnd)
	{
		case 1: //base sample rate, no change
			break;
		case 2: //96k tier
			freqTreble = 0.4265;
			freqMid = 0.013456;
			break;
		case 3: //192k tier
			freqTreble = 0.21325;
			freqMid = 0.006728;
			break;
	}
	
	//begin ResEQ2 Mid Boost
	double freqMPeak = pow(GetParameter( kParam_Two )+0.16,3);
	double amountMPeak = pow(GetParameter( kParam_Three ),2);
	int maxMPeak = (amountMPeak*63.0)+1;
	if ((freqMPeak != prevfreqMPeak)||(amountMPeak != prevamountMPeak)) {
		for (int x = 0; x < maxMPeak; x++) {
			if (((double)x*freqMPeak) < M_PI_4) f[x] = sin(((double)x*freqMPeak)*4.0)*freqMPeak*sin(((double)(maxMPeak-x)/(double)maxMPeak)*M_PI_2);
			else f[x] = cos((double)x*freqMPeak)*freqMPeak*sin(((double)(maxMPeak-x)/(double)maxMPeak)*M_PI_2);
		}
		prevfreqMPeak = freqMPeak; prevamountMPeak = amountMPeak;
	}//end ResEQ2 Mid Boost
	
	int bitshiftL = 0;
	int bitshiftR = 0;
	double panControl = (GetParameter( kParam_Five )*2.0)-1.0; //-1.0 to 1.0
	double panAttenuation = (1.0-fabs(panControl));
	int panBits = 20; //start centered
	if (panAttenuation > 0.0) panBits = floor(1.0 / panAttenuation);
	if (panControl > 0.25) bitshiftL += panBits;
	if (panControl < -0.25) bitshiftR += panBits;
	if (bitshiftL < 0) bitshiftL = 0; if (bitshiftL > 17) bitshiftL = 17;
	if (bitshiftR < 0) bitshiftR = 0; if (bitshiftR > 17) bitshiftR = 17;
	double gainL = 1.0;
	double gainR = 1.0;
	switch (bitshiftL)
	{
		case 17: gainL = 0.0; break;
		case 16: gainL = 0.0000152587890625; break;
		case 15: gainL = 0.000030517578125; break;
		case 14: gainL = 0.00006103515625; break;
		case 13: gainL = 0.0001220703125; break;
		case 12: gainL = 0.000244140625; break;
		case 11: gainL = 0.00048828125; break;
		case 10: gainL = 0.0009765625; break;
		case 9: gainL = 0.001953125; break;
		case 8: gainL = 0.00390625; break;
		case 7: gainL = 0.0078125; break;
		case 6: gainL = 0.015625; break;
		case 5: gainL = 0.03125; break;
		case 4: gainL = 0.0625; break;
		case 3: gainL = 0.125; break;
		case 2: gainL = 0.25; break;
		case 1: gainL = 0.5; break;
		case 0: break;
	}
	switch (bitshiftR)
	{
		case 17: gainR = 0.0; break;
		case 16: gainR = 0.0000152587890625; break;
		case 15: gainR = 0.000030517578125; break;
		case 14: gainR = 0.00006103515625; break;
		case 13: gainR = 0.0001220703125; break;
		case 12: gainR = 0.000244140625; break;
		case 11: gainR = 0.00048828125; break;
		case 10: gainR = 0.0009765625; break;
		case 9: gainR = 0.001953125; break;
		case 8: gainR = 0.00390625; break;
		case 7: gainR = 0.0078125; break;
		case 6: gainR = 0.015625; break;
		case 5: gainR = 0.03125; break;
		case 4: gainR = 0.0625; break;
		case 3: gainR = 0.125; break;
		case 2: gainR = 0.25; break;
		case 1: gainR = 0.5; break;
		case 0: break;
	}
	
	gainA = gainB;
	gainB = GetParameter( kParam_Six )*2.0; //smoothed master fader from Z2 filters
	//BitShiftGain pre gain trim goes here
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double temp = (double)nSampleFrames/inFramesToProcess;
		double gain = (gainA*temp)+(gainB*(1.0-temp));
		double bass = (bassA*temp)+(bassB*(1.0-temp));
		inputSampleL *= gain;
		inputSampleR *= gain;
		//for MCI consoles, the fader is before the EQ, which overdrives easily.
		//so we put the main fader here.
		
		//begin Pear filter stages
		double bassL = inputSampleL;
		double bassR = inputSampleR;
		double slew = ((bassL - pearA[0]) + pearA[1])*freqTreble*0.5;
		pearA[0] = bassL = (freqTreble * bassL) + ((1.0-freqTreble) * (pearA[0] + pearA[1]));
		pearA[1] = slew; slew = ((bassR - pearA[2]) + pearA[3])*freqTreble*0.5;
		pearA[2] = bassR = (freqTreble * bassR) + ((1.0-freqTreble) * (pearA[2] + pearA[3]));
		pearA[3] = slew; slew = ((bassL - pearA[4]) + pearA[5])*freqTreble*0.5;
		pearA[4] = bassL = (freqTreble * bassL) + ((1.0-freqTreble) * (pearA[4] + pearA[5]));
		pearA[5] = slew; slew = ((bassR - pearA[6]) + pearA[7])*freqTreble*0.5;
		pearA[6] = bassR = (freqTreble * bassR) + ((1.0-freqTreble) * (pearA[6] + pearA[7]));
		pearA[7] = slew; slew = ((bassL - pearA[8]) + pearA[9])*freqTreble*0.5;
		pearA[8] = bassL = (freqTreble * bassL) + ((1.0-freqTreble) * (pearA[8] + pearA[9]));
		pearA[9] = slew; slew = ((bassR - pearA[10]) + pearA[11])*freqTreble*0.5;
		pearA[10] = bassR = (freqTreble * bassR) + ((1.0-freqTreble) * (pearA[10] + pearA[11]));
		pearA[11] = slew; slew = ((bassL - pearA[12]) + pearA[13])*freqTreble*0.5;
		pearA[12] = bassL = (freqTreble * bassL) + ((1.0-freqTreble) * (pearA[12] + pearA[13]));
		pearA[13] = slew; slew = ((bassR - pearA[14]) + pearA[15])*freqTreble*0.5;
		pearA[14] = bassR = (freqTreble * bassR) + ((1.0-freqTreble) * (pearA[14] + pearA[15]));
		pearA[15] = slew;
		//unrolled mid/treble crossover (called bass to use fewer variables)		
		double trebleL = inputSampleL - bassL; inputSampleL = bassL;
		double trebleR = inputSampleR - bassR; inputSampleR = bassR;
		//at this point 'bass' is actually still mid and bass
		slew = ((bassL - pearB[0]) + pearB[1])*freqMid*0.5;
		pearB[0] = bassL = (freqMid * bassL) + ((1.0-freqMid) * (pearB[0] + pearB[1]));
		pearB[1] = slew; slew = ((bassR - pearB[2]) + pearB[3])*freqMid*0.5;
		pearB[2] = bassR = (freqMid * bassR) + ((1.0-freqMid) * (pearB[2] + pearB[3]));
		pearB[3] = slew; slew = ((bassL - pearB[4]) + pearB[5])*freqMid*0.5;
		pearB[4] = bassL = (freqMid * bassL) + ((1.0-freqMid) * (pearB[4] + pearB[5]));
		pearB[5] = slew; slew = ((bassR - pearB[6]) + pearB[7])*freqMid*0.5;
		pearB[6] = bassR = (freqMid * bassR) + ((1.0-freqMid) * (pearB[6] + pearB[7]));
		pearB[7] = slew; slew = ((bassL - pearB[8]) + pearB[9])*freqMid*0.5;
		pearB[8] = bassL = (freqMid * bassL) + ((1.0-freqMid) * (pearB[8] + pearB[9]));
		pearB[9] = slew; slew = ((bassR - pearB[10]) + pearB[11])*freqMid*0.5;
		pearB[10] = bassR = (freqMid * bassR) + ((1.0-freqMid) * (pearB[10] + pearB[11]));
		pearB[11] = slew; slew = ((bassL - pearB[12]) + pearB[13])*freqMid*0.5;
		pearB[12] = bassL = (freqMid * bassL) + ((1.0-freqMid) * (pearB[12] + pearB[13]));
		pearB[13] = slew; slew = ((bassR - pearB[14]) + pearB[15])*freqMid*0.5;
		pearB[14] = bassR = (freqMid * bassR) + ((1.0-freqMid) * (pearB[14] + pearB[15]));
		pearB[15] = slew; slew = ((bassL - pearB[16]) + pearB[17])*freqMid*0.5;
		pearB[16] = bassL = (freqMid * bassL) + ((1.0-freqMid) * (pearB[16] + pearB[17]));
		pearB[17] = slew; slew = ((bassR - pearB[18]) + pearB[19])*freqMid*0.5;
		pearB[18] = bassR = (freqMid * bassR) + ((1.0-freqMid) * (pearB[18] + pearB[19]));
		pearB[19] = slew;
		double midL = inputSampleL - bassL;
		double midR = inputSampleR - bassR;
		//we now have three bands out of two pear filters
		
		double w = 0.0; //filter into bands, apply the sin/cos to each band
		if (fatTreble > 0.0) {
			w = fatTreble; if (w > 1.0) w = 1.0;
			trebleL = (trebleL*(1.0-w)) + (sin(trebleL*M_PI_2)*w);
			trebleR = (trebleR*(1.0-w)) + (sin(trebleR*M_PI_2)*w);
			if (fatTreble > 1.0) {
				w = fatTreble-1.0; if (w > 1.0) w = 1.0;
				trebleL = (trebleL*(1.0-w)) + (sin(trebleL*M_PI_2)*w);
				trebleR = (trebleR*(1.0-w)) + (sin(trebleR*M_PI_2)*w);
				if (fatTreble > 2.0) {
					w = fatTreble-2.0;
					trebleL = (trebleL*(1.0-w)) + (sin(trebleL*M_PI_2)*w);
					trebleR = (trebleR*(1.0-w)) + (sin(trebleR*M_PI_2)*w);
				} //sine stages for EQ or compression
			}
		}
		if (fatTreble < 0.0) {
			if (trebleL > 1.0) trebleL = 1.0; if (trebleL < -1.0) trebleL = -1.0;
			if (trebleR > 1.0) trebleR = 1.0; if (trebleR < -1.0) trebleR = -1.0;
			w = -fatTreble; if (w > 1.0) w = 1.0;
			if (trebleL > 0) trebleL = (trebleL*(1.0-w))+((1.0-cos(trebleL))*sin(w));
			else trebleL = (trebleL*(1.0-w))+((-1.0+cos(-trebleL))*sin(w));
			if (trebleR > 0) trebleR = (trebleR*(1.0-w))+((1.0-cos(trebleR))*sin(w));
			else trebleR = (trebleR*(1.0-w))+((-1.0+cos(-trebleR))*sin(w));
		} //cosine stages for EQ or expansion
		
		//begin ResEQ2 Mid Boost
		mpc++; if (mpc < 1 || mpc > 2001) mpc = 1;
		mpkL[mpc] = midL;
		mpkR[mpc] = midR;
		double midMPeakL = 0.0;
		double midMPeakR = 0.0;
		for (int x = 0; x < maxMPeak; x++) {
			int y = x*cycleEnd;
			switch (cycleEnd)
			{
				case 1: 
					midMPeakL += (mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x]);
					midMPeakR += (mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x]); break;
				case 2: 
					midMPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5);
					midMPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5); y--;
					midMPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5);
					midMPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.5); break;
				case 3: 
					midMPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333);
					midMPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333); y--;
					midMPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333);
					midMPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333); y--;
					midMPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333);
					midMPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.333); break;
				case 4: 
					midMPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midMPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); y--;
					midMPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midMPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); y--;
					midMPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midMPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); y--;
					midMPeakL += ((mpkL[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25);
					midMPeakR += ((mpkR[(mpc-y)+((mpc-y < 1)?2001:0)] * f[x])*0.25); //break
			}
		}
		midL = (midMPeakL*amountMPeak)+((1.5-amountMPeak>1.0)?midL:midL*(1.5-amountMPeak));
		midR = (midMPeakR*amountMPeak)+((1.5-amountMPeak>1.0)?midR:midR*(1.5-amountMPeak));
		//end ResEQ2 Mid Boost
		
		if (bassL > 1.0) bassL = 1.0; if (bassL < -1.0) bassL = -1.0;
		if (bassR > 1.0) bassR = 1.0; if (bassR < -1.0) bassR = -1.0;
		if (bass > 0.0) {
			w = bass; if (w > 1.0) w = 1.0;
			bassL = (bassL*(1.0-w)) + (sin(bassL*M_PI_2)*w*1.6);
			bassR = (bassR*(1.0-w)) + (sin(bassR*M_PI_2)*w*1.6);
			if (bass > 1.0) {
				w = bass-1.0; if (w > 1.0) w = 1.0;
				bassL = (bassL*(1.0-w)) + (sin(bassL*M_PI_2)*w*1.4);
				bassR = (bassR*(1.0-w)) + (sin(bassR*M_PI_2)*w*1.4);
				if (bass > 2.0) {
					w = bass-2.0;
					bassL = (bassL*(1.0-w)) + (sin(bassL*M_PI_2)*w*1.2);
					bassR = (bassR*(1.0-w)) + (sin(bassR*M_PI_2)*w*1.2);
				} //sine stages for EQ or compression
			}
		}
		if (bass < 0.0) {
			w = -bass; if (w > 1.0) w = 1.0;	
			if (bassL > 0) bassL = (bassL*(1.0-w))+((1.0-cos(bassL))*sin(w));
			else bassL = (bassL*(1.0-w))+((-1.0+cos(-bassL))*sin(w));
			if (bassR > 0) bassR = (bassR*(1.0-w))+((1.0-cos(bassR))*sin(w));
			else bassR = (bassR*(1.0-w))+((-1.0+cos(-bassR))*sin(w));
		} //cosine stages for EQ or expansion
		//the sin() is further restricting output when fully attenuated
				
		inputSampleL = (bassL + midL + trebleL)*gainL;
		inputSampleR = (bassR + midR + trebleR)*gainR;
		//applies BitShiftPan pan section
		
		//begin sin() style Channel processing
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleL = sin(inputSampleL);
		inputSampleR = sin(inputSampleR);
		
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

