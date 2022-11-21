/*
*	File:		IronOxideClassic2.cpp
*	
*	Version:	1.0
* 
*	Created:	3/10/21
*	
*	Copyright:  Copyright © 2021 Airwindows, Airwindows uses the MIT license
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
	IronOxideClassic2.cpp
	
=============================================================================*/
#include "IronOxideClassic2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, IronOxideClassic2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxideClassic2::IronOxideClassic2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IronOxideClassic2::IronOxideClassic2(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxideClassic2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			IronOxideClassic2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxideClassic2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			IronOxideClassic2::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -18.0;
                outParameterInfo.maxValue = 18.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
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
//	IronOxideClassic2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			IronOxideClassic2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxideClassic2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			IronOxideClassic2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	IronOxideClassic2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult IronOxideClassic2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____IronOxideClassic2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxideClassic2::IronOxideClassic2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		IronOxideClassic2::IronOxideClassic2Kernel::Reset()
{
	for (int x = 0; x < 11; x++) {biquadA[x] = 0.0;biquadB[x] = 0.0;}
	for (int temp = 0; temp < 263; temp++) {d[temp] = 0.0;}
	for(int count = 0; count < 6; count++) {lastRef[count] = 0.0;}
	cycle = 0;
	gcount = 0;
	fastIIRA = fastIIRB = slowIIRA = slowIIRB = 0.0;
	iirSampleA = iirSampleB = 0.0;
	flip = true;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IronOxideClassic2::IronOxideClassic2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		IronOxideClassic2::IronOxideClassic2Kernel::Process(	const Float32 	*inSourceP,
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
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	Float64 inputgain = pow(10.0,GetParameter( kParam_One )/20.0);
	Float64 outputgain = pow(10.0,GetParameter( kParam_Three )/20.0);
	Float64 ips = GetParameter( kParam_Two ) * 1.1;
	//slight correction to dial in convincing ips settings
	if (ips < 1 || ips > 200) ips=33.0;
	//sanity checks are always key
	Float64 iirAmount = ips/430.0; //for low leaning
	Float64 fastTaper = ips/15.0;
	Float64 slowTaper = 2.0/(ips*ips);
	
	iirAmount /= overallscale;
	fastTaper /= overallscale;
	slowTaper /= overallscale;
	//now that we have this, we must multiply it back up
	fastTaper *= cycleEnd;
	slowTaper *= cycleEnd;
	//because we're only running that part one sample in two, or three, or four
	fastTaper += 1.0;
	slowTaper += 1.0;
	
	biquadA[0] = 24000.0 / GetSampleRate();
    biquadA[1] = 1.618033988749894848204586;
	biquadB[0] = 24000.0 / GetSampleRate();
    biquadB[1] = 0.618033988749894848204586;
	
	double K = tan(M_PI * biquadA[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadB[0]); //lowpass
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = K * K * norm;
	biquadB[3] = 2.0 * biquadB[2];
	biquadB[4] = biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		if (flip)
		{
			if (fabs(iirSampleA)<1.18e-37) iirSampleA = 0.0;
			iirSampleA = (iirSampleA * (1 - iirAmount)) + (inputSample * iirAmount);
			inputSample -= iirSampleA;
		}
		else
		{
			if (fabs(iirSampleB)<1.18e-37) iirSampleB = 0.0;
			iirSampleB = (iirSampleB * (1 - iirAmount)) + (inputSample * iirAmount);
			inputSample -= iirSampleB;
		}
		//do IIR highpass for leaning out
		
		if (biquadA[0] < 0.49999) {
			double tempSample = biquadA[2]*inputSample+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
			biquadA[8] = biquadA[7]; biquadA[7] = inputSample; inputSample = tempSample; 
			biquadA[10] = biquadA[9]; biquadA[9] = inputSample; //DF1
		}		
		
		if (inputgain != 1.0) inputSample *= inputgain;
		
		double bridgerectifier = fabs(inputSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0.0) inputSample = bridgerectifier;
		else inputSample = -bridgerectifier;		
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a tape sample
		
		if (gcount < 0 || gcount > 131) gcount = 131;
		int count = gcount;
		//increment the counter
		
		double temp;
		d[count+131] = d[count] = inputSample;
			
		if (flip)
		{
			if (fabs(fastIIRA)<1.18e-37) fastIIRA = 0.0;
			if (fabs(slowIIRA)<1.18e-37) slowIIRA = 0.0;
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
			if (fabs(fastIIRB)<1.18e-37) fastIIRB = 0.0;
			if (fabs(slowIIRB)<1.18e-37) slowIIRB = 0.0;
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
			
			if (cycleEnd == 4) {
				lastRef[0] = lastRef[4]; //start from previous last
				lastRef[2] = (lastRef[0] + inputSample)/2; //half
				lastRef[1] = (lastRef[0] + lastRef[2])/2; //one quarter
				lastRef[3] = (lastRef[2] + inputSample)/2; //three quarters
				lastRef[4] = inputSample; //full
			}
			if (cycleEnd == 3) {
				lastRef[0] = lastRef[3]; //start from previous last
				lastRef[2] = (lastRef[0]+lastRef[0]+inputSample)/3; //third
				lastRef[1] = (lastRef[0]+inputSample+inputSample)/3; //two thirds
				lastRef[3] = inputSample; //full
			}
			if (cycleEnd == 2) {
				lastRef[0] = lastRef[2]; //start from previous last
				lastRef[1] = (lastRef[0] + inputSample)/2; //half
				lastRef[2] = inputSample; //full
			}
			if (cycleEnd == 1) lastRef[0] = inputSample;
			cycle = 0; //reset
			inputSample = lastRef[cycle];
		} else {
			inputSample = lastRef[cycle];
			//we are going through our references now
		}
		
		
		bridgerectifier = fabs(inputSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		//can use as an output limiter
		if (inputSample > 0.0) inputSample = bridgerectifier;
		else inputSample = -bridgerectifier;
		//second stage of overdrive to prevent overs and allow bloody loud extremeness
		
		if (biquadB[0] < 0.49999) {
			double tempSample = biquadB[2]*inputSample+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSample; inputSample = tempSample; 
			biquadB[10] = biquadB[9]; biquadB[9] = inputSample; //DF1
		}
		
		if (outputgain != 1.0) inputSample *= outputgain;
		flip = !flip;
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

