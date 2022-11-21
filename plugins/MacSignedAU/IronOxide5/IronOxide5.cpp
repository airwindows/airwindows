/*
*	File:		IronOxide5.cpp
*	
*	Version:	1.0
* 
*	Created:	4/21/17
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
	IronOxide5.cpp
	
=============================================================================*/
#include "IronOxide5.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, IronOxide5)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxide5::IronOxide5
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IronOxide5::IronOxide5(AudioUnit component)
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
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxide5::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			IronOxide5::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxide5::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			IronOxide5::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -18.0;
                outParameterInfo.maxValue = 18.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
				outParameterInfo.unitName = kParameterTwoUnit;
				outParameterInfo.minValue = 1.5;
                outParameterInfo.maxValue = 150.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_CustomUnit;
				outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
				outParameterInfo.unitName = kParameterThreeUnit;
				outParameterInfo.minValue = 1.5;
                outParameterInfo.maxValue = 150.0;
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -18.0;
                outParameterInfo.maxValue = 18.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSix;
                break;
			case kParam_Seven:
                AUBase::FillInParameterName (outParameterInfo, kParameterSevenName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSeven;
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
//	IronOxide5::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			IronOxide5::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxide5::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			IronOxide5::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	IronOxide5::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult IronOxide5::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____IronOxide5EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxide5::IronOxide5Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		IronOxide5::IronOxide5Kernel::Reset()
{
	int temp;
	for (temp = 0; temp < 263; temp++) {d[temp] = 0.0;}
	gcount = 0;
	fastIIRA = fastIIRB = slowIIRA = slowIIRB = 0.0;
	fastIIHA = fastIIHB = slowIIHA = slowIIHB = 0.0;
	iirSamplehA = iirSamplehB = 0.0;
	iirSampleA = iirSampleB = 0.0;
	prevInputSample = 0.0;
	flip = false;
	for (temp = 0; temp < 99; temp++) {fl[temp] = 0.0;}
	fstoredcount = 0;	
	sweep = 0.0;
	rateof = 0.5;
	nextmax = 0.5;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxide5::IronOxide5Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		IronOxide5::IronOxide5Kernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	Float64 inputgain = pow(10.0,GetParameter( kParam_One )/20.0);
	Float64 outputgain = pow(10.0,GetParameter( kParam_Six )/20.0);
	Float64 ips = GetParameter( kParam_Two ) * 1.1;
	//slight correction to dial in convincing ips settings
	if (ips < 1 || ips > 200){ips=33.0;}
	//sanity checks are always key
	Float64 tempRandy = 0.04+(0.11/sqrt(ips));
	Float64 randy;
	Float64 lps = GetParameter( kParam_Three ) * 1.1;
	//slight correction to dial in convincing ips settings
	if (lps < 1 || lps > 200){lps=33.0;}
	//sanity checks are always key
	Float64 iirAmount = lps/430.0; //for low leaning
	Float64 bridgerectifier;
	Float64 fastTaper = ips/15.0;
	Float64 slowTaper = 2.0/(lps*lps);
	Float64 lowspeedscale = (5.0/ips);
	double inputSample;
	Float64 drySample;
	SInt32 count;
	SInt32 flutcount;
	Float64 flutterrandy;
	Float64 temp;
	Float64 overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	Float64 depth = pow(GetParameter( kParam_Four ),2)*overallscale;
	Float64 fluttertrim = 0.00581/overallscale;
	Float64 sweeptrim = (0.0005*depth)/overallscale;
	Float64 offset;	
	Float64 tupi = 3.141592653589793238 * 2.0;
	Float64 newrate = 0.006/overallscale;
	Float64 oldrate = 1.0-newrate;	
	if (overallscale == 0) {fastTaper += 1.0; slowTaper += 1.0;}
	else
	{
		iirAmount /= overallscale;
		lowspeedscale *= overallscale;
		fastTaper = 1.0 + (fastTaper / overallscale);
		slowTaper = 1.0 + (slowTaper / overallscale);
	}
	Float64 noise = GetParameter( kParam_Five ) * 0.5;
	Float64 invdrywet = GetParameter( kParam_Seven );
	Float64 dry = 1.0;
	if (invdrywet > 0.0) dry -= invdrywet;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		drySample = inputSample;
		
		flutterrandy = (double(fpd)/UINT32_MAX);
		//part of flutter section
		//now we've got a random flutter, so we're messing with the pitch before tape effects go on
		if (fstoredcount < 0 || fstoredcount > 30) {fstoredcount = 30;}
		flutcount = fstoredcount;
		fl[flutcount+31] = fl[flutcount] = inputSample;
		offset = (1.0 + sin(sweep)) * depth;
		flutcount += (int)floor(offset);
		bridgerectifier = (fl[flutcount] * (1-(offset-floor(offset))));
		bridgerectifier += (fl[flutcount+1] * (offset-floor(offset)));
		rateof = (nextmax * newrate) + (rateof * oldrate);
		sweep += rateof * fluttertrim;
		sweep += sweep * sweeptrim;
		if (sweep >= tupi){sweep = 0.0; nextmax = 0.02 + (flutterrandy*0.98);}
		fstoredcount--;
		inputSample = bridgerectifier;
		//apply to input signal, interpolate samples
		//all the funky renaming is just trying to fix how I never reassigned the control numbers
		
		if (flip)
		{
			iirSampleA = (iirSampleA * (1 - iirAmount)) + (inputSample * iirAmount);
			inputSample -= iirSampleA;
		}
		else
		{
			iirSampleB = (iirSampleB * (1 - iirAmount)) + (inputSample * iirAmount);
			inputSample -= iirSampleB;
		}
		//do IIR highpass for leaning out
		inputSample *= inputgain;
		bridgerectifier = fabs(inputSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0.0) inputSample = bridgerectifier;
		else inputSample = -bridgerectifier;
		//preliminary gain stage using antialiasing
		
		//over to the Iron Oxide shaping code using inputsample
		if (gcount < 0 || gcount > 131) {gcount = 131;}
		count = gcount;
		//increment the counter
		
		d[count+131] = d[count] = inputSample;
		if (flip)
		{
			fastIIRA = fastIIRA/fastTaper;
			slowIIRA = slowIIRA/slowTaper;
			//scale stuff down
			fastIIRA += d[count];
			count += 3;
			temp = d[count+127];
			temp += d[count+113];
			temp += d[count+109];
			temp += d[count+107];
			temp += d[count+103];
			temp += d[count+101];
			temp += d[count+97];
			temp += d[count+89];
			temp += d[count+83];
			temp /= 2;
			temp += d[count+79];
			temp += d[count+73];
			temp += d[count+71];
			temp += d[count+67];
			temp += d[count+61];
			temp += d[count+59];
			temp += d[count+53];
			temp += d[count+47];
			temp += d[count+43];
			temp += d[count+41];
			temp += d[count+37];
			temp += d[count+31];
			temp += d[count+29];
			temp /= 2;
			temp += d[count+23];
			temp += d[count+19];
			temp += d[count+17];
			temp += d[count+13];
			temp += d[count+11];
			temp /= 2;
			temp += d[count+7];
			temp += d[count+5];
			temp += d[count+3];
			temp /= 2;
			temp += d[count+2];
			temp += d[count+1];
			slowIIRA += (temp/128);
			inputSample = fastIIRA - (slowIIRA / slowTaper);
		}
		else
		{
			fastIIRB = fastIIRB/fastTaper;
			slowIIRB = slowIIRB/slowTaper;
			//scale stuff down
			fastIIRB += d[count];
			count += 3;
			temp = d[count+127];
			temp += d[count+113];
			temp += d[count+109];
			temp += d[count+107];
			temp += d[count+103];
			temp += d[count+101];
			temp += d[count+97];
			temp += d[count+89];
			temp += d[count+83];
			temp /= 2;
			temp += d[count+79];
			temp += d[count+73];
			temp += d[count+71];
			temp += d[count+67];
			temp += d[count+61];
			temp += d[count+59];
			temp += d[count+53];
			temp += d[count+47];
			temp += d[count+43];
			temp += d[count+41];
			temp += d[count+37];
			temp += d[count+31];
			temp += d[count+29];
			temp /= 2;
			temp += d[count+23];
			temp += d[count+19];
			temp += d[count+17];
			temp += d[count+13];
			temp += d[count+11];
			temp /= 2;
			temp += d[count+7];
			temp += d[count+5];
			temp += d[count+3];
			temp /= 2;
			temp += d[count+2];
			temp += d[count+1];
			slowIIRB += (temp/128);
			inputSample = fastIIRB - (slowIIRB / slowTaper);
		}
		inputSample /= fastTaper;
		inputSample /= lowspeedscale;
		//inputsample side
		//post-center code on inputSample and halfwaySample in parallel
		//begin raw sample- inputSample and ataDrySample handled separately here
		bridgerectifier = fabs(inputSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		//can use as an output limiter
		if (inputSample > 0.0) inputSample = bridgerectifier;
		else inputSample = -bridgerectifier;
		//second stage of overdrive to prevent overs and allow bloody loud extremeness		
		
		randy = (0.55 + tempRandy + ((double(fpd)/UINT32_MAX)*tempRandy))*noise; //0 to 2
		inputSample *= (1.0 - randy);
		inputSample += (prevInputSample*randy);
		prevInputSample = drySample;
		
		flip = !flip;
		
		//begin invdrywet block with outputgain
		if (outputgain != 1.0) inputSample *= outputgain;
		if (invdrywet != 1.0) inputSample *= invdrywet;
		if (dry != 1.0) drySample *= dry;
		if (fabs(drySample) > 0.0) inputSample += drySample;
		//end invdrywet block with outputgain
				
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

