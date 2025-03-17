/*
*	File:		SoftClock.cpp
*	
*	Version:	1.0
* 
*	Created:	3/5/25
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
	SoftClock.cpp
	
=============================================================================*/
#include "SoftClock.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(SoftClock)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SoftClock::SoftClock
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SoftClock::SoftClock(AudioUnit component)
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
//	SoftClock::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SoftClock::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SoftClock::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SoftClock::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 40;
                outParameterInfo.maxValue = 240;
                outParameterInfo.defaultValue = kDefaultValue_ParamA;
                break;
            case kParam_B:
                AUBase::FillInParameterName (outParameterInfo, kParameterBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 32;
                outParameterInfo.defaultValue = kDefaultValue_ParamB;
                break;
            case kParam_C:
                AUBase::FillInParameterName (outParameterInfo, kParameterCName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 16;
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
//	SoftClock::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SoftClock::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 SoftClock::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	SoftClock::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SoftClock::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	SoftClock::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult SoftClock::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____SoftClockEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SoftClock::SoftClockKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		SoftClock::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	sinePos = 0.0;
	barPos = 0.0;
	beatPos = 0;
	for (int x = 0; x < 34; x++) {beatAccent[x] = 0.0; beatSwing[x] = 0.0;}
	inc = 0.0;
	beatTable[0]=0;
	beatTable[1]=1;
	beatTable[2]=2;
	beatTable[3]=3;
	beatTable[4]=4;
	beatTable[5]=5;
	beatTable[6]=6;
	beatTable[7]=7;
	beatTable[8]=8;
	beatTable[9]=9;
	beatTable[10]=10;
	beatTable[11]=11;
	beatTable[12]=11;
	beatTable[13]=11;
	beatTable[14]=11;
	beatTable[15]=13;
	beatTable[16]=16;
	beatTable[17]=13;
	beatTable[18]=13;
	beatTable[19]=17;
	beatTable[20]=17;
	beatTable[21]=17;
	beatTable[22]=17;
	beatTable[23]=19;
	beatTable[24]=24;
	beatTable[25]=19;
	beatTable[26]=19;
	beatTable[27]=19;
	beatTable[28]=23;
	beatTable[29]=23;
	beatTable[30]=23;
	beatTable[31]=23;
	beatTable[32]=32;
	beatTable[33]=32;
	beatTable[34]=32;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SoftClock::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		SoftClock::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	int bpm = GetParameter( kParam_A );
	int beatCode = GetParameter( kParam_B );
	double notes = (double)fmax(GetParameter( kParam_C )/4.0, 0.125);
	double bpmTarget = (GetSampleRate()*60.0)/((double)bpm*notes);
	double swing = GetParameter( kParam_D )*bpmTarget*0.66666;
	double bigbeat = GetParameter( kParam_E )*bpmTarget*0.33333;
	//swing makes beats hit LATER, so the One is 0.0
	//bigbeat makes snares hit either on the beat or later
	int beatMax = beatTable[beatCode];
	//only some counts are literal, others are ways to do prime grooves with different subrhythms
	for (int x = 0; x < (beatMax+1); x++) {
		beatAccent[x] = ((double)fabs((double)beatMax-((double)x*2.0)))/(double)(beatMax*1.618033988749894);
		if (x % 2 > 0) beatSwing[x] = 0.0;
		else beatSwing[x] = swing;
	} //this makes the non-accented beats drop down to quiet and back up to half volume
	if (beatCode > 0) beatAccent[1] = 1.0; beatSwing[1] = 0.0; //first note is an accent at full crank
	switch (beatCode)
	{
		case 0:	break; //not used
		case 1:	break; //1
		case 2:	break; //2
		case 3:	break; //3
		case 4: beatAccent[3]=0.9;
			beatSwing[3]=bigbeat; break; //4-22
		case 5:	beatAccent[4]=0.9;
			beatSwing[4]=bigbeat; break; //5-32
		case 6:	beatAccent[4]=0.9;
			beatSwing[4]=bigbeat; break; //6-33
		case 7:	beatAccent[5]=0.9;
			beatSwing[5]=bigbeat; break; //7-43
		case 8:	beatAccent[5]=0.9;
			beatSwing[5]=bigbeat; break; //8-44
		case 9:	beatAccent[4]=0.9; beatAccent[7]=0.8;
			beatSwing[4]=bigbeat; beatSwing[7]=bigbeat; break; //9-333
		case 10: beatAccent[6]=0.9;
			beatSwing[6]=bigbeat; break; //10-55
		case 11: beatAccent[4]=0.9;	beatAccent[7]=0.8;	beatAccent[10]=0.7;
			beatSwing[4]=bigbeat; beatSwing[7]=bigbeat; beatSwing[10]=bigbeat; break; //11-3332
		case 12: beatAccent[5]=0.9;	beatAccent[9]=0.8;
			beatSwing[5]=bigbeat; beatSwing[9]=bigbeat; break; //11-443
		case 13: beatAccent[6]=0.9;	beatAccent[11]=0.8;
			beatSwing[6]=bigbeat; beatSwing[11]=bigbeat; break; //11-551
		case 14: beatAccent[7]=0.9;
			beatSwing[7]=bigbeat; break; //11-65
		case 15: beatAccent[4]=0.9;	beatAccent[7]=0.8;	beatAccent[10]=0.7;
			beatSwing[4]=bigbeat; beatSwing[7]=bigbeat; beatSwing[10]=bigbeat; break; //13-3334
		case 16: beatAccent[9]=0.9;
			beatSwing[9]=bigbeat; break; //16-88
		case 17: beatAccent[5]=0.9;	beatAccent[9]=0.8;
			beatSwing[5]=bigbeat; beatSwing[9]=bigbeat; break; //13-445
		case 18: beatAccent[6]=0.9;	beatAccent[11]=0.8;
			beatSwing[6]=bigbeat; beatSwing[11]=bigbeat; break; //13-553
		case 19: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;
			beatSwing[5]=bigbeat; beatSwing[9]=bigbeat; beatSwing[13]=bigbeat; beatSwing[17]=bigbeat; break; //17-44441
		case 20: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=bigbeat; beatSwing[11]=bigbeat; beatSwing[16]=bigbeat; break; //17-5552
		case 21: beatAccent[8]=0.9;	beatAccent[15]=0.8;
			beatSwing[8]=bigbeat; beatSwing[15]=bigbeat; break; //17-773
		case 22: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=bigbeat; beatSwing[17]=bigbeat; break; //17-881
		case 23: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;
			beatSwing[5]=bigbeat; beatSwing[9]=bigbeat; beatSwing[13]=bigbeat; beatSwing[17]=bigbeat; break; //19-44443
		case 24: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=bigbeat; beatSwing[17]=bigbeat; break; //24-888
		case 25: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=bigbeat; beatSwing[11]=bigbeat; beatSwing[16]=bigbeat; break; //19-5554
		case 26: beatAccent[8]=0.9;	beatAccent[15]=0.8;
			beatSwing[8]=bigbeat; beatSwing[15]=bigbeat; break; //19-775
		case 27: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=bigbeat; beatSwing[17]=bigbeat; break; //19-883
		case 28: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;	beatAccent[21]=0.7;
			beatSwing[5]=bigbeat; beatSwing[9]=bigbeat; beatSwing[13]=bigbeat; beatSwing[17]=bigbeat; beatSwing[21]=bigbeat; break; //23-444443
		case 29: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=bigbeat; beatSwing[11]=bigbeat; beatSwing[16]=bigbeat; break; //23-5558
		case 30: beatAccent[8]=0.9;	beatAccent[15]=0.8;	beatAccent[22]=0.7;
			beatSwing[8]=bigbeat; beatSwing[15]=bigbeat; beatSwing[22]=bigbeat; break; //23-7772
		case 31: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=bigbeat; beatSwing[17]=bigbeat; break; //23-887
		case 32: beatAccent[9]=0.9;	beatAccent[17]=0.8;	beatAccent[25]=0.7;
			beatSwing[9]=bigbeat; beatSwing[17]=bigbeat; beatSwing[25]=bigbeat; break; //32-8888
		default: break;			
	}			
	double accent = 1.0-pow(1.0-GetParameter( kParam_F ),2);
	double chaseSpeed = ((GetParameter( kParam_H )*0.00016)+0.000016)/overallscale;
	double rootSpeed = 1.0-(chaseSpeed*((1.0-GetParameter( kParam_H ))+0.5)*4.0);
	double pulseWidth = ((GetParameter( kParam_G )*0.2)-((1.0-GetParameter( kParam_H ))*0.03))/chaseSpeed;

	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		barPos += 1.0;
		if (barPos>bpmTarget) {
			barPos=0.0;
			beatPos++;
			if (beatPos>beatMax) beatPos=1;
		}
		if ((barPos < (pulseWidth+beatSwing[beatPos])) && (barPos > beatSwing[beatPos]))
			inc = (((beatAccent[beatPos]*accent)+(1.0-accent))*chaseSpeed)+(inc*(1.0-chaseSpeed));
		else
			inc *= rootSpeed;
		sinePos += inc;
		if (sinePos > 6.283185307179586) sinePos -= 6.283185307179586;
		inputSampleL = inputSampleR = sin(sin(sinePos)*inc*8.0);
		
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

