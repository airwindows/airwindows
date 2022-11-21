/*
*	File:		Hermepass.cpp
*	
*	Version:	1.0
* 
*	Created:	3/15/17
*	
*	Copyright:  Copyright © 2017 Airwindows, Airwindows uses the MIT license
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
	Hermepass.cpp
	
=============================================================================*/
#include "Hermepass.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Hermepass)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Hermepass::Hermepass
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hermepass::Hermepass(AudioUnit component)
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
//	Hermepass::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Hermepass::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Hermepass::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Hermepass::GetParameterInfo(AudioUnitScope		inScope,
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
//	Hermepass::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Hermepass::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Hermepass::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Hermepass::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Hermepass::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Hermepass::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____HermepassEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Hermepass::HermepassKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Hermepass::HermepassKernel::Reset()
{
	iirA = 0.0;
	iirB = 0.0;
	iirC = 0.0;
	iirD = 0.0;
	iirE = 0.0;
	iirF = 0.0;
	iirG = 0.0;
	iirH = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
	fpFlip = true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Hermepass::HermepassKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Hermepass::HermepassKernel::Process(	const Float32 	*inSourceP,
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
	double fpOld = 0.618033988749894848204586; //golden ratio!
	double fpNew = 1.0 - fpOld;

	Float64 rangescale = 0.1 / overallscale;
	
	Float64 cutoff = pow(GetParameter( kParam_One ),3);
	Float64 slope = pow(GetParameter( kParam_Two ),3) * 6.0;
	
	Float64 newA = cutoff * rangescale;
	Float64 newB = newA; //other part of interleaved IIR is the same
	
	Float64 newC = cutoff * rangescale; //first extra pole is the same
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	Float64 newD = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	Float64 newE = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	Float64 newF = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	Float64 newG = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * fpNew);
	Float64 newH = cutoff * rangescale;
	//converge toward the unvarying fixed cutoff value
	
	Float64 oldA = 1.0 - newA;
	Float64 oldB = 1.0 - newB;
	Float64 oldC = 1.0 - newC;
	Float64 oldD = 1.0 - newD;
	Float64 oldE = 1.0 - newE;
	Float64 oldF = 1.0 - newF;
	Float64 oldG = 1.0 - newG;
	Float64 oldH = 1.0 - newH;
	
	Float64 polesC;
	Float64 polesD;
	Float64 polesE;
	Float64 polesF;
	Float64 polesG;
	Float64 polesH;
	
	polesC = slope; if (slope > 1.0) polesC = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesD = slope; if (slope > 1.0) polesD = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesE = slope; if (slope > 1.0) polesE = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesF = slope; if (slope > 1.0) polesF = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesG = slope; if (slope > 1.0) polesG = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	polesH = slope; if (slope > 1.0) polesH = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	//each one will either be 0.0, the fractional slope value, or 1
	
	double inputSample;
	Float64 tempSample;
	Float64 correction;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		tempSample = inputSample;

		if (fpFlip) {
			iirA = (iirA * oldA) + (tempSample * newA); tempSample -= iirA; correction = iirA;
		} else {
			iirB = (iirB * oldB) + (tempSample * newB); tempSample -= iirB; correction = iirB;
		}
		iirC = (iirC * oldC) + (tempSample * newC); tempSample -= iirC;
		iirD = (iirD * oldD) + (tempSample * newD); tempSample -= iirD;
		iirE = (iirE * oldE) + (tempSample * newE); tempSample -= iirE;
		iirF = (iirF * oldF) + (tempSample * newF); tempSample -= iirF;
		iirG = (iirG * oldG) + (tempSample * newG); tempSample -= iirG;
		iirH = (iirH * oldH) + (tempSample * newH); tempSample -= iirH;
		//set up all the iir filters in case they are used
		
		if (polesC == 1.0) correction += iirC; if (polesC > 0.0 && polesC < 1.0) correction += (iirC * polesC);
		if (polesD == 1.0) correction += iirD; if (polesD > 0.0 && polesD < 1.0) correction += (iirD * polesD);
		if (polesE == 1.0) correction += iirE; if (polesE > 0.0 && polesE < 1.0) correction += (iirE * polesE);
		if (polesF == 1.0) correction += iirF; if (polesF > 0.0 && polesF < 1.0) correction += (iirF * polesF);
		if (polesG == 1.0) correction += iirG; if (polesG > 0.0 && polesG < 1.0) correction += (iirG * polesG);
		if (polesH == 1.0) correction += iirH; if (polesH > 0.0 && polesH < 1.0) correction += (iirH * polesH);
		//each of these are added directly if they're fully engaged,
		//multiplied by 0-1 if they are the interpolated one, or skipped if they are beyond the interpolated one.
		//the purpose is to do all the math at the floating point exponent nearest to the tiny value in use.
		//also, it's formatted that way to easily substitute the next variable: this could be written as a loop
		//with everything an array value. However, this makes just as much sense for this few poles.
		
		inputSample -= correction;
		fpFlip = !fpFlip;
	
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

