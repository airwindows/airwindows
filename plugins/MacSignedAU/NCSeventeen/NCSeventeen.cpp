/*
*	File:		NCSeventeen.cpp
*	
*	Version:	1.0
* 
*	Created:	9/27/10
*	
*	Copyright:  Copyright © 2010 Airwindows, Airwindows uses the MIT license
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
	NCSeventeen.h
	
=============================================================================*/
#include "NCSeventeen.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, NCSeventeen)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NCSeventeen::NCSeventeen
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NCSeventeen::NCSeventeen(AudioUnit component)
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
//	NCSeventeen::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		NCSeventeen::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NCSeventeen::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		NCSeventeen::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 24.0;
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
//	NCSeventeen::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		NCSeventeen::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NCSeventeen::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		NCSeventeen::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NCSeventeen::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult NCSeventeen::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____NCSeventeenEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NCSeventeen::NCSeventeenKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		NCSeventeen::NCSeventeenKernel::Reset()
{
	lastSample = 0.0;
	iirSampleA = 0.0;
	iirSampleB = 0.0;
	flip = false;
	basslev = 0.0;
	treblev = 0.0;
	cheblev = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NCSeventeen::NCSeventeenKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		NCSeventeen::NCSeventeenKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels,
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;	
	Float64 inP2;
	Float64 chebyshev;
	Float64 overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	Float64 IIRscaleback = 0.0004716;
	Float64 bassScaleback = 0.0002364;
	Float64 trebleScaleback = 0.0005484;
	Float64 addBassBuss = 0.000243;
	Float64 addTrebBuss = 0.000407;
	Float64 addShortBuss = 0.000326;
	IIRscaleback /= overallscale;
	bassScaleback /= overallscale;
	trebleScaleback /= overallscale;
	addBassBuss /= overallscale;
	addTrebBuss /= overallscale;
	addShortBuss /= overallscale;
	Float64 limitingBass = 0.39;
	Float64 limitingTreb = 0.6;
	Float64 limiting = 0.36;
	Float64 maxfeedBass = 0.972;
	Float64 maxfeedTreb = 0.972;
	Float64 maxfeed = 0.975;
	Float64 bridgerectifier;
	double inputSample;
	Float64 lowSample;
	Float64 highSample;
	Float64 distSample;
	Float64 minusSample;
	Float64 plusSample;
	Float64 gain = pow(10.0,GetParameter( kParam_One )/20);
	Float64 outgain = GetParameter( kParam_Two );

	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		inputSample *= gain;

		if (flip)
			{
			iirSampleA = (iirSampleA * 0.9) + (inputSample * 0.1);
			lowSample = iirSampleA;
			}
		else
			{
			iirSampleB = (iirSampleB * 0.9) + (inputSample * 0.1);
			lowSample = iirSampleB;
			}
		highSample = inputSample - lowSample;
		flip = !flip;
		//we now have two bands and the original source

		//inputSample = lowSample;
		inP2 = lowSample * lowSample;
		if (inP2 > 1.0) inP2 = 1.0; if (inP2 < -1.0) inP2 = -1.0;
		chebyshev = (2 * inP2);
		chebyshev *= basslev;
		//second harmonic max +1
		if (basslev > 0) basslev -= bassScaleback;
		if (basslev < 0) basslev += bassScaleback;
		//this is ShortBuss, IIRscaleback is the decay speed. *2 for second harmonic, and so on
		bridgerectifier = fabs(lowSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (lowSample > 0.0) distSample = bridgerectifier;
		else distSample = -bridgerectifier;
		minusSample = lowSample - distSample;
		plusSample = lowSample + distSample;
		if (minusSample > maxfeedBass) minusSample = maxfeedBass;
		if (plusSample > maxfeedBass) plusSample = maxfeedBass;
		if (plusSample < -maxfeedBass) plusSample = -maxfeedBass;
		if (minusSample < -maxfeedBass) minusSample = -maxfeedBass;
		if (lowSample > distSample) basslev += (minusSample*addBassBuss);
		if (lowSample < -distSample) basslev -= (plusSample*addBassBuss);
		if (basslev > 1.0)  basslev = 1.0;
		if (basslev < -1.0) basslev = -1.0;
		bridgerectifier = fabs(lowSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (lowSample > 0.0) lowSample = bridgerectifier;
		else lowSample = -bridgerectifier;
		//apply the distortion transform for reals
		lowSample /= (1.0+fabs(basslev*limitingBass));
		lowSample += chebyshev;
		//apply the correction measures
		
		//inputSample = highSample;
		inP2 = highSample * highSample;
		if (inP2 > 1.0) inP2 = 1.0; if (inP2 < -1.0) inP2 = -1.0;
		chebyshev = (2 * inP2);
		chebyshev *= treblev;
		//second harmonic max +1
		if (treblev > 0) treblev -= trebleScaleback;
		if (treblev < 0) treblev += trebleScaleback;
		//this is ShortBuss, IIRscaleback is the decay speed. *2 for second harmonic, and so on
		bridgerectifier = fabs(highSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (highSample > 0.0) distSample = bridgerectifier;
		else distSample = -bridgerectifier;
		minusSample = highSample - distSample;
		plusSample = highSample + distSample;
		if (minusSample > maxfeedTreb) minusSample = maxfeedTreb;
		if (plusSample > maxfeedTreb) plusSample = maxfeedTreb;
		if (plusSample < -maxfeedTreb) plusSample = -maxfeedTreb;
		if (minusSample < -maxfeedTreb) minusSample = -maxfeedTreb;
		if (highSample > distSample) treblev += (minusSample*addTrebBuss);
		if (highSample < -distSample) treblev -= (plusSample*addTrebBuss);
		if (treblev > 1.0)  treblev = 1.0;
		if (treblev < -1.0) treblev = -1.0;
		bridgerectifier = fabs(highSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (highSample > 0.0) highSample = bridgerectifier;
		else highSample = -bridgerectifier;
		//apply the distortion transform for reals
		highSample /= (1.0+fabs(treblev*limitingTreb));
		highSample += chebyshev;
		//apply the correction measures
		
		inputSample = lowSample + highSample;

		inP2 = inputSample * inputSample;
		if (inP2 > 1.0) inP2 = 1.0; if (inP2 < -1.0) inP2 = -1.0;
		chebyshev = (2 * inP2);
		chebyshev *= cheblev;
		//third harmonic max -1
		if (cheblev > 0) cheblev -= IIRscaleback;
		if (cheblev < 0) cheblev += IIRscaleback;
		//this is ShortBuss, IIRscaleback is the decay speed. *2 for second harmonic, and so on
		bridgerectifier = fabs(inputSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0.0) distSample = bridgerectifier;
		else distSample = -bridgerectifier;
		minusSample = inputSample - distSample;
		plusSample = inputSample + distSample;
		if (minusSample > maxfeed) minusSample = maxfeed;
		if (plusSample > maxfeed) plusSample = maxfeed;
		if (plusSample < -maxfeed) plusSample = -maxfeed;
		if (minusSample < -maxfeed) minusSample = -maxfeed;
		if (inputSample > distSample) cheblev += (minusSample*addShortBuss);
		if (inputSample < -distSample) cheblev -= (plusSample*addShortBuss);
		if (cheblev > 1.0)  cheblev = 1.0;
		if (cheblev < -1.0) cheblev = -1.0;
		bridgerectifier = fabs(inputSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0.0) inputSample = bridgerectifier;
		else inputSample = -bridgerectifier;
		//apply the distortion transform for reals
		inputSample /= (1.0+fabs(cheblev*limiting));
		inputSample += chebyshev;
		//apply the correction measures
		
		if (outgain < 1.0) {
			inputSample *= outgain;
		}
		
		if (inputSample > 0.95) inputSample = 0.95;
		if (inputSample < -0.95) inputSample = -0.95;
		//iron bar
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		//built in output trim and dry/wet by default
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

