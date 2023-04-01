/*
*	File:		Huge.cpp
*	
*	Version:	1.0
* 
*	Created:	3/27/23
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
	Huge.cpp
	
=============================================================================*/
#include "Huge.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Huge)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Huge::Huge
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Huge::Huge(AudioUnit component)
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
//	Huge::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Huge::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Huge::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Huge::GetParameterInfo(AudioUnitScope		inScope,
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
//	Huge::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Huge::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Huge::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Huge::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Huge::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Huge::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____HugeEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Huge::HugeKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Huge::HugeKernel::Reset()
{
	prev3 = prev5 = prev7 = prev9 = prev11 = prev13 = prevOut = 0.0;
	limit3 = limit5 = limit7 = limit9 = limit11 = limit13 = 1.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Huge::HugeKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Huge::HugeKernel::Process(	const Float32 	*inSourceP,
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
	
	double huge = GetParameter( kParam_One );
	double threshold = (GetParameter( kParam_Two ) * 0.05) + 0.05;
	double attack = (threshold * 8.0) / overallscale;
	double release = (threshold * 8.0) / overallscale;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;

		inputSample *= huge;
		
		double inP3 = inputSample * inputSample * inputSample;
		double outP3 = ((4.0*inP3)-(3.0*inputSample))*-0.36;
		double inP5 = inP3 * inputSample * inputSample;
		double outP5 = ((16.0*inP5)-(20.0*inP3)+(5.0*inputSample))*0.2025;
		double inP7 = inP5 * inputSample * inputSample;
		double outP7 = ((64.0*inP7)-(112.0*inP5)+(56.0*inP3)-(7.0*inputSample))*-0.1444;
		double inP9 = inP7 * inputSample * inputSample;
		double outP9 = ((256.0*inP9)-(576.0*inP7)+(432.0*inP5)-(120.0*inP3)+(9.0*inputSample))*0.1225;
		double inP11 = inP9 * inputSample * inputSample;
		double outP11 = ((1024.0*inP11)-(2816.0*inP9)+(2816.0*inP7)-(1232.0*inP5)+(220.0*inP3)-(11.0*inputSample))*-0.1024;
		double inP13 = inP11 * inputSample * inputSample;
		double outP13 = ((4096.0*inP13)-(13312.0*inP11)+(16640.0*inP9)-(9984.0*inP7)+(2912.0*inP5)-(364.0*inP3)+(13.0*inputSample))*0.09;

		double slew3 = fabs(outP3 - prev3); prev3 = outP3;
		double slew5 = fabs(outP5 - prev5); prev5 = outP5;
		double slew7 = fabs(outP7 - prev7); prev7 = outP7;
		double slew9 = fabs(outP9 - prev9); prev9 = outP9;
		double slew11 = fabs(outP11 - prev11); prev11 = outP11;
		double slew13 = fabs(outP13 - prev13); prev13 = outP13;
		
		if (slew3 < threshold) limit3 += release;
		else limit3 -= attack;
		if (slew5 < threshold) limit5 += release;
		else limit5 -= attack;
		if (slew7 < threshold) limit7 += release;
		else limit7 -= attack;
		if (slew9 < threshold) limit9 += release;
		else limit9 -= attack;
		if (slew11 < threshold) limit11 += release;
		else limit11 -= attack;
		if (slew13 < threshold) limit13 += release;
		else limit13 -= attack;
		
		if (limit3 > 1.0)  {limit3 = 1.0;}
		if (limit5 > 1.0)  {limit3 = 1.0; limit5 = 1.0;}
		if (limit7 > 1.0)  {limit3 = 1.0; limit5 = 1.0; limit7 = 1.0;}
		if (limit9 > 1.0)  {limit3 = 1.0; limit5 = 1.0; limit7 = 1.0; limit9 = 1.0;}
		if (limit11 > 1.0) {limit3 = 1.0; limit5 = 1.0; limit7 = 1.0; limit9 = 1.0; limit11 = 1.0;}
		if (limit13 > 1.0) {limit3 = 1.0; limit5 = 1.0; limit7 = 1.0; limit9 = 1.0; limit11 = 1.0; limit13 = 1.0;}
		if (limit3 < 0.0) {limit3 = 0.0; limit5 = 0.0; limit7 = 0.0; limit9 = 0.0; limit11 = 0.0; limit13 = 0.0;}
		if (limit5 < 0.0) {limit5 = 0.0; limit7 = 0.0; limit9 = 0.0; limit11 = 0.0; limit13 = 0.0;}
		if (limit7 < 0.0) {limit7 = 0.0; limit9 = 0.0; limit11 = 0.0; limit13 = 0.0;}
		if (limit9 < 0.0) {limit9 = 0.0; limit11 = 0.0; limit13 = 0.0;}
		if (limit11 < 0.0) {limit11 = 0.0; limit13 = 0.0;}
		if (limit13 < 0.0) {limit13 = 0.0;}
		
		double chebyshev = (outP3 * limit3);
		chebyshev += (outP5 * limit5);
		chebyshev += (outP7 * limit7);
		chebyshev += (outP9 * limit9);
		chebyshev += (outP11 * limit11);
		chebyshev += (outP13 * limit13);
		inputSample += ((chebyshev+prevOut)*0.5);
		prevOut = chebyshev;
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

