/*
*	File:		AngleEQ.cpp
*	
*	Version:	1.0
* 
*	Created:	3/19/25
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
	AngleEQ.cpp
	
=============================================================================*/
#include "AngleEQ.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, AngleEQ)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AngleEQ::AngleEQ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AngleEQ::AngleEQ(AudioUnit component)
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
//	AngleEQ::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AngleEQ::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AngleEQ::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AngleEQ::GetParameterInfo(AudioUnitScope		inScope,
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
//	AngleEQ::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AngleEQ::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AngleEQ::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			AngleEQ::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	AngleEQ::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult AngleEQ::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____AngleEQEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AngleEQ::AngleEQKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		AngleEQ::AngleEQKernel::Reset()
{
	for(int count = 0; count < 36; count++) {
		iirHPosition[count] = 0.0;
		iirHAngle[count] = 0.0;
		iirMHPosition[count] = 0.0;
		iirMHAngle[count] = 0.0;
		iirMLPosition[count] = 0.0;
		iirMLAngle[count] = 0.0;
		iirLPosition[count] = 0.0;
		iirLAngle[count] = 0.0;
	}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	AngleEQ::AngleEQKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		AngleEQ::AngleEQKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();

	double highVol = pow(GetParameter( kParam_A )*2.0,2.0);
	double midVol = pow(GetParameter( kParam_B )*2.0,2.0);
	double lowVol = pow(GetParameter( kParam_C )*2.0,2.0);
	
	double hFreq = pow(GetParameter( kParam_D ),overallscale);
	double mhFreq = pow(GetParameter( kParam_E ),overallscale);
	double mlFreq = pow(GetParameter( kParam_F ),overallscale+3.0);
	double lFreq = pow(GetParameter( kParam_G ),overallscale+3.0);
	
	double dry = 2.0-(GetParameter( kParam_H )*2.0);
	if (dry > 1.0) dry = 1.0; //full dry for use with inv, to 0.0 at full wet
	double wet = (GetParameter( kParam_H )*2.0)-1.0; //inv-dry-wet for highpass
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		double midSample = inputSample;
		
		double highSample = inputSample;
		for(int count = 0; count < (3.0+(hFreq*32.0)); count++) {
			iirHAngle[count] = (iirHAngle[count]*(1.0-hFreq))+((inputSample-iirHPosition[count])*hFreq);
			inputSample = ((iirHPosition[count]+(iirHAngle[count]*hFreq))*(1.0-hFreq))+(inputSample*hFreq);
			iirHPosition[count] = ((iirHPosition[count]+(iirHAngle[count]*hFreq))*(1.0-hFreq))+(inputSample*hFreq);
			highSample -= (inputSample * (1.0/(3.0+(hFreq*32.0))) );
		} //highpass point of treble band
		
		inputSample = midSample; //restore for second highpass
		for(int count = 0; count < (3.0+(mlFreq*32.0)); count++) {
			iirMHAngle[count] = (iirMHAngle[count]*(1.0-mlFreq))+((inputSample-iirMHPosition[count])*mlFreq);
			inputSample = ((iirMHPosition[count]+(iirMHAngle[count]*mlFreq))*(1.0-mlFreq))+(inputSample*mlFreq);
			iirMHPosition[count] = ((iirMHPosition[count]+(iirMHAngle[count]*mlFreq))*(1.0-mlFreq))+(inputSample*mlFreq);
			midSample -= (inputSample * (1.0/(3.0+(mlFreq*32.0))) );
		} //highpass point of mid-to-low band
		
		for(int count = 0; count < (3.0+(mhFreq*32.0)); count++) {
			iirMLAngle[count] = (iirMLAngle[count]*(1.0-mhFreq))+((midSample-iirMLPosition[count])*mhFreq);
			midSample = ((iirMLPosition[count]+(iirMLAngle[count]*mhFreq))*(1.0-mhFreq))+(midSample*mhFreq);
			iirMLPosition[count] = ((iirMLPosition[count]+(iirMLAngle[count]*mhFreq))*(1.0-mhFreq))+(midSample*mhFreq);
		} //lowpass point of mid-to-high band
		
		double lowSample = inputSample;
		for(int count = 0; count < (3.0+(lFreq*32.0)); count++) {
			iirLAngle[count] = (iirLAngle[count]*(1.0-lFreq))+((lowSample-iirLPosition[count])*lFreq);
			lowSample = ((iirLPosition[count]+(iirLAngle[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
			iirLPosition[count] = ((iirLPosition[count]+(iirLAngle[count]*lFreq))*(1.0-lFreq))+(lowSample*lFreq);
		} //lowpass point of low band
		
		inputSample = (drySample*dry)+(((highSample*highVol)+(midSample*midVol)+(lowSample*lowVol))*wet);
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

