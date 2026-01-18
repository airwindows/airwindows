/*
*	File:		SoftClock3.cpp
*	
*	Version:	1.0
* 
*	Created:	1/8/26
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
	SoftClock3.cpp
	
=============================================================================*/
#include "SoftClock3.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(SoftClock3)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SoftClock3::SoftClock3
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SoftClock3::SoftClock3(AudioUnit component)
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
//	SoftClock3::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SoftClock3::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SoftClock3::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SoftClock3::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				switch ((int)GetParameter( kParam_B )){
					case 0:outParameterInfo.unitName = CFSTR("0"); break;
					case 1:	outParameterInfo.unitName = CFSTR("1"); break;
					case 2:outParameterInfo.unitName = CFSTR("2"); break;
					case 3:outParameterInfo.unitName = CFSTR("3"); break;
					case 4:outParameterInfo.unitName = CFSTR("4"); break;
					case 5:outParameterInfo.unitName = CFSTR("5"); break;
					case 6:outParameterInfo.unitName = CFSTR("6"); break;
					case 7:outParameterInfo.unitName = CFSTR("7"); break;
					case 8:outParameterInfo.unitName = CFSTR("8"); break;
					case 9:outParameterInfo.unitName = CFSTR("9"); break;
					case 10:outParameterInfo.unitName = CFSTR("10"); break;
					case 11:outParameterInfo.unitName = CFSTR("11"); break;
					case 12:outParameterInfo.unitName = CFSTR("11"); break;
					case 13:outParameterInfo.unitName = CFSTR("11"); break;
					case 14:outParameterInfo.unitName = CFSTR("11"); break;
					case 15:outParameterInfo.unitName = CFSTR("13"); break;
					case 16:outParameterInfo.unitName = CFSTR("16"); break;
					case 17:outParameterInfo.unitName = CFSTR("13"); break;
					case 18:outParameterInfo.unitName = CFSTR("13"); break;
					case 19:outParameterInfo.unitName = CFSTR("17"); break;
					case 20:outParameterInfo.unitName = CFSTR("17"); break;
					case 21:outParameterInfo.unitName = CFSTR("17"); break;
					case 22:outParameterInfo.unitName = CFSTR("17"); break;
					case 23:outParameterInfo.unitName = CFSTR("19"); break;
					case 24:outParameterInfo.unitName = CFSTR("24"); break;
					case 25:outParameterInfo.unitName = CFSTR("19"); break;
					case 26:outParameterInfo.unitName = CFSTR("19"); break;
					case 27:outParameterInfo.unitName = CFSTR("19"); break;
					case 28:outParameterInfo.unitName = CFSTR("23"); break;
					case 29:outParameterInfo.unitName = CFSTR("23"); break;
					case 30:outParameterInfo.unitName = CFSTR("23"); break;
					case 31:outParameterInfo.unitName = CFSTR("23"); break;
					case 32:outParameterInfo.unitName = CFSTR("32"); break;
					default: break;			
				}			
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 1;
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
//	SoftClock3::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SoftClock3::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 SoftClock3::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	SoftClock3::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			SoftClock3::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	SoftClock3::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult SoftClock3::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____SoftClock3EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SoftClock3::SoftClock3Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		SoftClock3::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
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
	currentBPM = 0;
	step = 0.0;
	swagger = 0.0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	SoftClock3::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		SoftClock3::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	double triplet = 0.0;
	if (GetParameter( kParam_D ) > 0) triplet = 1.0;
	double accent = GetParameter( kParam_E );
	
	if (bpm != currentBPM) {
		currentBPM = bpm;
		step = 20.0;
		double trial = 250.0;
		while (trial > bpm) {step -= 1.0; trial = (0.016666666666*pow(step,M_PI))+(step*M_PI)+20;}
		//we now have the nearest 'flow' energy state UNDER the real bpm, and will step up more slowly
		while (trial < bpm) {step += 0.01; trial = (0.016666666666*pow(step,M_PI))+(step*M_PI)+20;}
		//we now have the nearest tenth of the energy state and have overshot
		while (trial > bpm) {step -= 0.001; trial = (0.016666666666*pow(step,M_PI))+(step*M_PI)+20;}
		//we now have the nearest hundredth to the correct energy state, and can throw away 'trial'
		step = step-((int)step); //and calculate speed based on the fractional remainder of 'step'
		step = ((0.5-fabs(step-0.5))*2.0) * 0.75;
		swagger = pow((0.5-fabs(step-0.5))*2.0,1.618033988749894) * 0.5 * accent;
		//and turn it into the 'speed' control
		//with step 0.0 being 'flow', 0.25 being 'groove', 0.5 being 'edge' and 0.75 being 'tude'
		//turned into a linear speed control where edge is top step and flow being lowest step.
	}	
	double swing = (triplet+swagger)*bpmTarget*0.33333;
	//swing makes beats hit LATER, so the One is 0.0
	int beatMax = beatTable[beatCode];
	//only some counts are literal, others are ways to do prime grooves with different subrhythms
	for (int x = 0; x < (beatMax+1); x++) {
		beatAccent[x] = ((double)fabs((double)beatMax-((double)x*2.0)))/(double)(beatMax*1.618033988749894);
		if (x % 2 > 0) beatSwing[x] = (swagger*(1.0-beatAccent[x]));
		else beatSwing[x] = swing;
	} //this makes the non-accented beats drop down to quiet and back up to half volume
	//we're also decoupling swing from triplet feel unless actually playing triplets
	//otherwise, it's part of how you hear accents and tempos
	if (beatCode > 0) beatAccent[1] = 0.9; beatSwing[1] = 0.0; //first note is an accent at full crank
	switch (beatCode)
	{
		case 0:	break; //not used
		case 1:	break; //1
		case 2:	break; //2
		case 3:	break; //3
		case 4: beatAccent[3]=0.9;
			beatSwing[3]=0.0; break; //4-22
		case 5:	beatAccent[4]=0.9;
			beatSwing[4]=0.0; break; //5-32
		case 6:	beatAccent[4]=0.9;
			beatSwing[4]=0.0; break; //6-33
		case 7:	beatAccent[5]=0.9;
			beatSwing[5]=0.0; break; //7-43
		case 8:	beatAccent[5]=0.9;
			beatSwing[5]=0.0; break; //8-44
		case 9:	beatAccent[4]=0.9; beatAccent[7]=0.8;
			beatSwing[4]=0.0; beatSwing[7]=0.0; break; //9-333
		case 10: beatAccent[6]=0.9;
			beatSwing[6]=0.0; break; //10-55
		case 11: beatAccent[4]=0.9;	beatAccent[7]=0.8;	beatAccent[10]=0.7;
			beatSwing[4]=0.0; beatSwing[7]=0.0; beatSwing[10]=0.0; break; //11-3332
		case 12: beatAccent[5]=0.9;	beatAccent[9]=0.8;
			beatSwing[5]=0.0; beatSwing[9]=0.0; break; //11-443
		case 13: beatAccent[6]=0.9;	beatAccent[11]=0.8;
			beatSwing[6]=0.0; beatSwing[11]=0.0; break; //11-551
		case 14: beatAccent[7]=0.9;
			beatSwing[7]=0.0; break; //11-65
		case 15: beatAccent[4]=0.9;	beatAccent[7]=0.8;	beatAccent[10]=0.7;
			beatSwing[4]=0.0; beatSwing[7]=0.0; beatSwing[10]=0.0; break; //13-3334
		case 16: beatAccent[9]=0.9;
			beatSwing[9]=0.0; break; //16-88
		case 17: beatAccent[5]=0.9;	beatAccent[9]=0.8;
			beatSwing[5]=0.0; beatSwing[9]=0.0; break; //13-445
		case 18: beatAccent[6]=0.9;	beatAccent[11]=0.8;
			beatSwing[6]=0.0; beatSwing[11]=0.0; break; //13-553
		case 19: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;
			beatSwing[5]=0.0; beatSwing[9]=0.0; beatSwing[13]=0.0; beatSwing[17]=0.0; break; //17-44441
		case 20: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=0.0; beatSwing[11]=0.0; beatSwing[16]=0.0; break; //17-5552
		case 21: beatAccent[8]=0.9;	beatAccent[15]=0.8;
			beatSwing[8]=0.0; beatSwing[15]=0.0; break; //17-773
		case 22: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //17-881
		case 23: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;
			beatSwing[5]=0.0; beatSwing[9]=0.0; beatSwing[13]=0.0; beatSwing[17]=0.0; break; //19-44443
		case 24: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //24-888
		case 25: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=0.0; beatSwing[11]=0.0; beatSwing[16]=0.0; break; //19-5554
		case 26: beatAccent[8]=0.9;	beatAccent[15]=0.8;
			beatSwing[8]=0.0; beatSwing[15]=0.0; break; //19-775
		case 27: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //19-883
		case 28: beatAccent[5]=0.9;	beatAccent[9]=0.85;	beatAccent[13]=0.8;	beatAccent[17]=0.75;	beatAccent[21]=0.7;
			beatSwing[5]=0.0; beatSwing[9]=0.0; beatSwing[13]=0.0; beatSwing[17]=0.0; beatSwing[21]=0.0; break; //23-444443
		case 29: beatAccent[6]=0.9;	beatAccent[11]=0.8;	beatAccent[16]=0.7;
			beatSwing[6]=0.0; beatSwing[11]=0.0; beatSwing[16]=0.0; break; //23-5558
		case 30: beatAccent[8]=0.9;	beatAccent[15]=0.8;	beatAccent[22]=0.7;
			beatSwing[8]=0.0; beatSwing[15]=0.0; beatSwing[22]=0.0; break; //23-7772
		case 31: beatAccent[9]=0.9;	beatAccent[17]=0.8;
			beatSwing[9]=0.0; beatSwing[17]=0.0; break; //23-887
		case 32: beatAccent[9]=0.9;	beatAccent[17]=0.8;	beatAccent[25]=0.7;
			beatSwing[9]=0.0; beatSwing[17]=0.0; beatSwing[25]=0.0; break; //32-8888
		default: break;			
	}			
	double chaseSpeed = ((step*0.000125)+0.0000125)/overallscale;
	double rootSpeed = 0.9999-(chaseSpeed*((2.0*step)+2.0));
	double pulseWidth = (0.085+(accent*0.04)-((1.0-step)*0.06))/chaseSpeed;
	
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
		inputSampleL = inputSampleR = sin(sin(sinePos)*inc*5.0);
		
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

