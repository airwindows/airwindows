/*
 *	File:		Air2.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	7/5/21
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
 Air2.cpp
 
 =============================================================================*/
#include "Air2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Air2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Air2::Air2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Air2::Air2(AudioUnit component)
: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
	SetParameter(kParam_Five, kDefaultValue_ParamFive );
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Air2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Air2::GetParameterValueStrings(AudioUnitScope		inScope,
													   AudioUnitParameterID	inParameterID,
													   CFArrayRef *		outStrings)
{
	
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Air2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Air2::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
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
//	Air2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Air2::GetPropertyInfo (AudioUnitPropertyID	inID,
											   AudioUnitScope		inScope,
											   AudioUnitElement	inElement,
											   UInt32 &		outDataSize,
											   Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Air2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Air2::GetProperty(	AudioUnitPropertyID inID,
										  AudioUnitScope 		inScope,
										  AudioUnitElement 	inElement,
										  void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Air2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Air2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Air2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Air2::Air2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Air2::Air2Kernel::Reset()
{
	airPrevA = 0.0;
	airEvenA = 0.0;
	airOddA = 0.0;
	airFactorA = 0.0;
	flipA = false;
	airPrevB = 0.0;
	airEvenB = 0.0;
	airOddB = 0.0;
	airFactorB = 0.0;
	flipB = false;
	airPrevC = 0.0;
	airEvenC = 0.0;
	airOddC = 0.0;
	airFactorC = 0.0;
	flop = false;
	tripletPrev = 0.0;
	tripletMid = 0.0;
	tripletA = 0.0;
	tripletB = 0.0;
	tripletC = 0.0;
	tripletFactor = 0.0;
	count = 1;
	postsine = sin(1.0);
	
	for(int c = 0; c < 9; c++) {lastRef[c] = 0.0;}
	cycle = 0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Air2::Air2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Air2::Air2Kernel::Process(	const Float32 	*inSourceP,
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
	
	Float64 hiIntensity = GetParameter( kParam_One );
	Float64 tripletIntensity = GetParameter( kParam_Two );
	Float64 airIntensity = GetParameter( kParam_Three );
	if (hiIntensity < 0.0) hiIntensity *= 0.57525;
	if (tripletIntensity < 0.0) tripletIntensity *= 0.71325;
	if (airIntensity < 0.0) airIntensity *= 0.5712;
	hiIntensity = -pow(hiIntensity,3);
	tripletIntensity = -pow(tripletIntensity,3);
	airIntensity = -pow(airIntensity,3);
	Float64 hiQ = 1.5+fabs(hiIntensity*0.5);
	Float64 tripletQ = 1.5+fabs(tripletIntensity*0.5);
	Float64 airQ = 1.5+fabs(airIntensity*0.5);
	Float64 intensity = (pow(GetParameter( kParam_Four ),3)*4.0) + 0.0001;
	Float64 mix = GetParameter( kParam_Five );
	Float64 drymix = (1.0-mix)*4.0;
	if (drymix > 1.0) drymix = 1.0;
	
	//all types of air band are running in parallel, not series
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do an Air sample
			double correction = 0.0;
			if (fabs(hiIntensity) > 0.0001) {
				airFactorC = airPrevC - inputSample;
				if (flop)
				{
					airEvenC += airFactorC; airOddC -= airFactorC;
					airFactorC = airEvenC * hiIntensity;
				}
				else
				{
					airOddC += airFactorC; airEvenC -= airFactorC;
					airFactorC = airOddC * hiIntensity;
				}
				airOddC = (airOddC - ((airOddC - airEvenC)/256.0)) / hiQ;
				airEvenC = (airEvenC - ((airEvenC - airOddC)/256.0)) / hiQ;
				airPrevC = inputSample;
				correction = correction + airFactorC;
				flop = !flop;
			}//22k
			
			if (fabs(tripletIntensity) > 0.0001) {
				tripletFactor = tripletPrev - inputSample;
				if (count < 1 || count > 3) count = 1;
				switch (count)
				{
					case 1:
						tripletA += tripletFactor; tripletC -= tripletFactor;
						tripletFactor = tripletA * tripletIntensity;
						tripletPrev = tripletMid; tripletMid = inputSample;
						break;
					case 2:
						tripletB += tripletFactor; tripletA -= tripletFactor;
						tripletFactor = tripletB * tripletIntensity;
						tripletPrev = tripletMid; tripletMid = inputSample;
						break;
					case 3:
						tripletC += tripletFactor; tripletB -= tripletFactor;
						tripletFactor = tripletC * tripletIntensity;
						tripletPrev = tripletMid; tripletMid = inputSample;
						break;
				}
				tripletA /= tripletQ; tripletB /= tripletQ; tripletC /= tripletQ;
				correction = correction + tripletFactor;
				count++;
			}//15K
			
			if (fabs(airIntensity) > 0.0001) {
				if (flop)
				{
					airFactorA = airPrevA - inputSample;
					if (flipA)
					{
						airEvenA += airFactorA; airOddA -= airFactorA;
						airFactorA = airEvenA * airIntensity;
					}
					else
					{
						airOddA += airFactorA; airEvenA -= airFactorA;
						airFactorA = airOddA * airIntensity;
					}
					airOddA = (airOddA - ((airOddA - airEvenA)/256.0)) / airQ;
					airEvenA = (airEvenA - ((airEvenA - airOddA)/256.0)) / airQ;
					airPrevA = inputSample;
					correction = correction + airFactorA;
					flipA = !flipA;
				}
				else
				{
					airFactorB = airPrevB - inputSample;
					if (flipB)
					{
						airEvenB += airFactorB; airOddB -= airFactorB;
						airFactorB = airEvenB * airIntensity;
					}
					else
					{
						airOddB += airFactorB; airEvenB -= airFactorB;
						airFactorB = airOddB * airIntensity;
					}
					airOddB = (airOddB - ((airOddB - airEvenB)/256.0)) / airQ;
					airEvenB = (airEvenB - ((airEvenB - airOddB)/256.0)) / airQ;
					airPrevB = inputSample;
					correction = correction + airFactorB;
					flipB = !flipB;
				}
			}//11k
			
			correction *= intensity;
			correction -= 1.0;
			double bridgerectifier = fabs(correction);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (correction > 0) correction = bridgerectifier;
			else correction = -bridgerectifier;
			correction += postsine;
			correction /= intensity;
			inputSample = correction * 4.0 * mix;
			
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
		switch (cycleEnd) //multi-pole average using lastRef[] variables
		{
			case 4:
				lastRef[8] = inputSample; inputSample = (inputSample+lastRef[7])*0.5;
				lastRef[7] = lastRef[8]; //continue, do not break
			case 3:
				lastRef[8] = inputSample; inputSample = (inputSample+lastRef[6])*0.5;
				lastRef[6] = lastRef[8]; //continue, do not break
			case 2:
				lastRef[8] = inputSample; inputSample = (inputSample+lastRef[5])*0.5;
				lastRef[5] = lastRef[8]; //continue, do not break
			case 1:
				break; //no further averaging
		}
		
		if (drymix < 1.0) drySample *= drymix;
		inputSample += drySample;
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

