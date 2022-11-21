/*
*	File:		ResEQ.cpp
*	
*	Version:	1.0
* 
*	Created:	8/1/07
*	
*	Copyright:  Copyright © 2007 Airwindows, Airwindows uses the MIT license
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
	ResEQ.h
	
=============================================================================*/
#include "ResEQ.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, ResEQ)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResEQ::ResEQ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ResEQ::ResEQ(AudioUnit component)
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
	SetParameter(kParam_Seven, kDefaultValue_ParamSeven );
	SetParameter(kParam_Eight, kDefaultValue_ParamEight );
	SetParameter(kParam_Nine, kDefaultValue_ParamNine );
        
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResEQ::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		ResEQ::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResEQ::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		ResEQ::GetParameterInfo(AudioUnitScope		inScope,
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
           case kParam_Seven:
                AUBase::FillInParameterName (outParameterInfo, kParameterSevenName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSeven;
                break;
           case kParam_Eight:
                AUBase::FillInParameterName (outParameterInfo, kParameterEightName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamEight;
                break;
           case kParam_Nine:
                AUBase::FillInParameterName (outParameterInfo, kParameterNineName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamNine;
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
//	ResEQ::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		ResEQ::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResEQ::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		ResEQ::GetProperty(	AudioUnitPropertyID inID,
									   AudioUnitScope 		inScope,
									   AudioUnitElement 	inElement,
									   void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResEQ::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ResEQ::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ResEQEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResEQ::ResEQKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ResEQ::ResEQKernel::Reset()
{
	register UInt32 count;
	for(count = 0; count < 60; count++) {b[count] = 0.0; f[count] = 0.0;}
	framenumber = 0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResEQ::ResEQKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ResEQ::ResEQKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, // for version 2 AudioUnits inNumChannels is always 1
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	Float64 overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	Float64 v1 = GetParameter( kParam_One );
	Float64 v2 = GetParameter( kParam_Two );
	Float64 v3 = GetParameter( kParam_Three );
	Float64 v4 = GetParameter( kParam_Four );
	Float64 v5 = GetParameter( kParam_Five );
	Float64 v6 = GetParameter( kParam_Six );
	Float64 v7 = GetParameter( kParam_Seven );
	Float64 v8 = GetParameter( kParam_Eight );
	Float64 f1 = pow(v1,2);
	Float64 f2 = pow(v2,2);
	Float64 f3 = pow(v3,2);
	Float64 f4 = pow(v4,2);
	Float64 f5 = pow(v5,2);
	Float64 f6 = pow(v6,2);
	Float64 f7 = pow(v7,2);
	Float64 f8 = pow(v8,2);
	Float64 wet = GetParameter( kParam_Nine );
	Float64 falloff;
	v1 += 0.2; v2 += 0.2; v3 += 0.2; v4 += 0.2; v5 += 0.2; v6 += 0.2; v7 += 0.2; v8 += 0.2;
	v1 /= overallscale;
	v2 /= overallscale;
	v3 /= overallscale;
	v4 /= overallscale;
	v5 /= overallscale;
	v6 /= overallscale;
	v7 /= overallscale;
	v8 /= overallscale;
	//each process frame we'll update some of the kernel frames. That way we don't have to crunch the whole thing at once,
	//and we can load a LOT more resonant peaks into the kernel.
	
	//amount = 0.005 + (amount * 0.99);
	
	
	//mAudioUnit->SetParameter( kParam_Nine, amount );
	
	//		mAudioUnit->Globals()->SetParameter( kParam_Nine, amount );
	
	//		AudioUnitParameter param;
	//		param.mAudioUnit	= mAudioUnit->GetComponentInstance();	
	//		param.mScope 		= kAudioUnitScope_Global;
	//		param.mParameterID	= kParam_Nine;
	//		AUParameterListenerNotify( NULL, NULL, &param);
	
	
	
	
	
	framenumber += 1; if (framenumber > 59) framenumber = 1;
	falloff = sin(framenumber / 19.098992);
	f[framenumber] = 0.0;
	if ((framenumber * f1) < 1.57079633) f[framenumber]  += (sin((framenumber * f1)*2.0) * falloff * v1);
	else f[framenumber]  += (cos(framenumber * f1) * falloff * v1);
	if ((framenumber * f2) < 1.57079633) f[framenumber]  += (sin((framenumber * f2)*2.0) * falloff * v2);
	else f[framenumber]  += (cos(framenumber * f2) * falloff * v2);
	if ((framenumber * f3) < 1.57079633) f[framenumber]  += (sin((framenumber * f3)*2.0) * falloff * v3);
	else f[framenumber]  += (cos(framenumber * f3) * falloff * v3);
	if ((framenumber * f4) < 1.57079633) f[framenumber]  += (sin((framenumber * f4)*2.0) * falloff * v4);
	else f[framenumber]  += (cos(framenumber * f4) * falloff * v4);
	if ((framenumber * f5) < 1.57079633) f[framenumber]  += (sin((framenumber * f5)*2.0) * falloff * v5);
	else f[framenumber]  += (cos(framenumber * f5) * falloff * v5);
	if ((framenumber * f6) < 1.57079633) f[framenumber]  += (sin((framenumber * f6)*2.0) * falloff * v6);
	else f[framenumber]  += (cos(framenumber * f6) * falloff * v6);
	if ((framenumber * f7) < 1.57079633) f[framenumber]  += (sin((framenumber * f7)*2.0) * falloff * v7);
	else f[framenumber]  += (cos(framenumber * f7) * falloff * v7);
	if ((framenumber * f8) < 1.57079633) f[framenumber]  += (sin((framenumber * f8)*2.0) * falloff * v8);
	else f[framenumber]  += (cos(framenumber * f8) * falloff * v8);

	framenumber += 1; if (framenumber > 59) framenumber = 1;
	falloff = sin(framenumber / 19.098992);
	f[framenumber] = 0.0;
	if ((framenumber * f1) < 1.57079633) f[framenumber]  += (sin((framenumber * f1)*2.0) * falloff * v1);
	else f[framenumber]  += (cos(framenumber * f1) * falloff * v1);
	if ((framenumber * f2) < 1.57079633) f[framenumber]  += (sin((framenumber * f2)*2.0) * falloff * v2);
	else f[framenumber]  += (cos(framenumber * f2) * falloff * v2);
	if ((framenumber * f3) < 1.57079633) f[framenumber]  += (sin((framenumber * f3)*2.0) * falloff * v3);
	else f[framenumber]  += (cos(framenumber * f3) * falloff * v3);
	if ((framenumber * f4) < 1.57079633) f[framenumber]  += (sin((framenumber * f4)*2.0) * falloff * v4);
	else f[framenumber]  += (cos(framenumber * f4) * falloff * v4);
	if ((framenumber * f5) < 1.57079633) f[framenumber]  += (sin((framenumber * f5)*2.0) * falloff * v5);
	else f[framenumber]  += (cos(framenumber * f5) * falloff * v5);
	if ((framenumber * f6) < 1.57079633) f[framenumber]  += (sin((framenumber * f6)*2.0) * falloff * v6);
	else f[framenumber]  += (cos(framenumber * f6) * falloff * v6);
	if ((framenumber * f7) < 1.57079633) f[framenumber]  += (sin((framenumber * f7)*2.0) * falloff * v7);
	else f[framenumber]  += (cos(framenumber * f7) * falloff * v7);
	if ((framenumber * f8) < 1.57079633) f[framenumber]  += (sin((framenumber * f8)*2.0) * falloff * v8);
	else f[framenumber]  += (cos(framenumber * f8) * falloff * v8);

	//done updating the kernel for this go-round
		
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
b[59] = b[58]; b[58] = b[57]; b[57] = b[56]; b[56] = b[55]; b[55] = b[54]; b[54] = b[53]; b[53] = b[52]; b[52] = b[51]; b[51] = b[50]; b[50] = b[49]; b[49] = b[48];
b[48] = b[47]; b[47] = b[46]; b[46] = b[45]; b[45] = b[44]; b[44] = b[43]; b[43] = b[42]; b[42] = b[41]; b[41] = b[40]; b[40] = b[39]; b[39] = b[38];
b[38] = b[37]; b[37] = b[36]; b[36] = b[35]; b[35] = b[34]; b[34] = b[33]; b[33] = b[32]; b[32] = b[31]; b[31] = b[30]; b[30] = b[29]; b[29] = b[28]; 
b[28] = b[27]; b[27] = b[26]; b[26] = b[25]; b[25] = b[24]; b[24] = b[23]; b[23] = b[22]; b[22] = b[21]; b[21] = b[20]; b[20] = b[19]; b[19] = b[18];
b[18] = b[17]; b[17] = b[16]; b[16] = b[15]; b[15] = b[14]; b[14] = b[13]; b[13] = b[12]; b[12] = b[11]; b[11] = b[10]; b[10] = b[9]; b[9] = b[8]; b[8] = b[7]; 
b[7] = b[6]; b[6] = b[5]; b[5] = b[4]; b[4] = b[3]; b[3] = b[2]; b[2] = b[1]; b[1] = b[0]; b[0] = inputSample;
		
		inputSample = (b[1] * f[1]);
		inputSample += (b[2] * f[2]);
		inputSample += (b[3] * f[3]);
		inputSample += (b[4] * f[4]);
		inputSample += (b[5] * f[5]);
		inputSample += (b[6] * f[6]);
		inputSample += (b[7] * f[7]);
		inputSample += (b[8] * f[8]);
		inputSample += (b[9] * f[9]);
		inputSample += (b[10] * f[10]);
		inputSample += (b[11] * f[11]);
		inputSample += (b[12] * f[12]);
		inputSample += (b[13] * f[13]);
		inputSample += (b[14] * f[14]);
		inputSample += (b[15] * f[15]);
		inputSample += (b[16] * f[16]);
		inputSample += (b[17] * f[17]);
		inputSample += (b[18] * f[18]);
		inputSample += (b[19] * f[19]);
		inputSample += (b[20] * f[20]);
		inputSample += (b[21] * f[21]);
		inputSample += (b[22] * f[22]);
		inputSample += (b[23] * f[23]);
		inputSample += (b[24] * f[24]);
		inputSample += (b[25] * f[25]);
		inputSample += (b[26] * f[26]);
		inputSample += (b[27] * f[27]);
		inputSample += (b[28] * f[28]);
		inputSample += (b[29] * f[29]);
		inputSample += (b[30] * f[30]);
		inputSample += (b[31] * f[31]);
		inputSample += (b[32] * f[32]);
		inputSample += (b[33] * f[33]);
		inputSample += (b[34] * f[34]);
		inputSample += (b[35] * f[35]);
		inputSample += (b[36] * f[36]);
		inputSample += (b[37] * f[37]);
		inputSample += (b[38] * f[38]);
		inputSample += (b[39] * f[39]);
		inputSample += (b[40] * f[40]);
		inputSample += (b[41] * f[41]);
		inputSample += (b[42] * f[42]);
		inputSample += (b[43] * f[43]);
		inputSample += (b[44] * f[44]);
		inputSample += (b[45] * f[45]);
		inputSample += (b[46] * f[46]);
		inputSample += (b[47] * f[47]);
		inputSample += (b[48] * f[48]);
		inputSample += (b[49] * f[49]);
		inputSample += (b[50] * f[50]);
		inputSample += (b[51] * f[51]);
		inputSample += (b[52] * f[52]);
		inputSample += (b[53] * f[53]);
		inputSample += (b[54] * f[54]);
		inputSample += (b[55] * f[55]);
		inputSample += (b[56] * f[56]);
		inputSample += (b[57] * f[57]);
		inputSample += (b[58] * f[58]);
		inputSample += (b[59] * f[59]);
		inputSample /= 12.0;
		//inlined- this is our little EQ kernel. Longer will give better tightness on bass frequencies.
		inputSample = (drySample*(1.0-wet))+(inputSample*wet);

		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		//end 32 bit floating point dither
		

		*destP = inputSample;
		sourceP += inNumChannels;
		destP += inNumChannels;
	}
}

