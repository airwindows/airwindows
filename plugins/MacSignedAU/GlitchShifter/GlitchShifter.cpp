/*
*	File:		GlitchShifter.cpp
*	
*	Version:	1.0
* 
*	Created:	2/19/07
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
	GlitchShifter.h
	
=============================================================================*/
#include "GlitchShifter.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, GlitchShifter)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GlitchShifter::GlitchShifter
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GlitchShifter::GlitchShifter(AudioUnit component)
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
//	GlitchShifter::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		GlitchShifter::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GlitchShifter::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		GlitchShifter::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = -12;
                outParameterInfo.maxValue = 12;
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
//	GlitchShifter::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		GlitchShifter::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GlitchShifter::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		GlitchShifter::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GlitchShifter::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult GlitchShifter::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____GlitchShifterEffectKernel


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GlitchShifter::GlitchShifterKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		GlitchShifter::GlitchShifterKernel::Reset()
{
	for (int count = 0; count < 131074; count++) {p[count] = 0;}
	for (int count = 0; count < 257; count++) {offset[count] = 0; pastzero[count] = 0; previous[count] = 0; third[count] = 0; fourth[count] = 0;}
	gcount = 0;
	crosses = 0;
	realzeroes = 0;
	temp = 0;
	lasttemp = 0;
	thirdtemp = 0;
	fourthtemp = 0;
	lastwidth = 16386;
	sincezerocross = 0;
	airPrev = 0.0;
	airEven = 0.0;
	airOdd = 0.0;
	airFactor = 0.0;
	position = 0.0;
	flip = false;
	splicing = false;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GlitchShifter::GlitchShifterKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		GlitchShifter::GlitchShifterKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, // for version 2 AudioUnits inNumChannels is always 1
                                                    bool			&ioSilence )
{

	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	Float64 note = GetParameter( kParam_One );
	Float64 trim = GetParameter( kParam_Two );
	Float64 speed = ((1.0/12.0)*note)+trim;	
	if (speed < 0) speed *= 0.5;
	//include sanity check- maximum pitch lowering cannot be to 0 hz.
	SInt32 width = (SInt32)(65536-((1-pow(1-GetParameter( kParam_Three ),2))*65530.0));
	Float64 bias = pow(GetParameter( kParam_Three ),3);
	Float64 feedback = GetParameter( kParam_Four )/1.5;
	Float64 wet = GetParameter( kParam_Five );
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;

		airFactor = airPrev - inputSample;
		if (flip) {airEven += airFactor; airOdd -= airFactor; airFactor = airEven;}
		else {airOdd += airFactor; airEven -= airFactor; airFactor = airOdd;}
		airOdd = (airOdd - ((airOdd - airEven)/256.0)) / 1.0001;
		airEven = (airEven - ((airEven - airOdd)/256.0)) / 1.0001;
		airPrev = inputSample;
		inputSample += airFactor;
		flip = !flip;
		//air, compensates for loss of highs of interpolation
		
		if (lastwidth != width){crosses = 0; realzeroes = 0; lastwidth = width;}
		
		gcount++;
		if (gcount < 0 || gcount > width) {gcount = 0;}
		int count = gcount;
		int countone = count-1;
		int counttwo = count-2;
		while (count < 0){count += width;}
		while (countone < 0){countone += width;}
		while (counttwo < 0){counttwo += width;}
		while (count > width){count -= width;} //this can only happen with very insane variables
		while (countone > width){countone -= width;}
		while (counttwo > width){counttwo -= width;}
		//yay sanity checks
		//now we have counts zero, one, two, none of which have sanity checked values
		//we are tracking most recent samples and must SUBTRACT.
		
		p[count+width] = p[count] = (SInt32)((inputSample*8388352.0)+(Float64)(lasttemp*feedback));
		//double buffer -8388352 to 8388352 is equal to 24 bit linear space

		if ((p[countone] > 0 && p[count] < 0)||(p[countone] < 0 && p[count] > 0)) //source crossed zero
			{
				crosses++;
				realzeroes++;
				if (crosses > 256) {crosses = 0;} //wrap crosses to keep adding new crosses
				if (realzeroes > 256) {realzeroes = 256;} //don't wrap realzeroes, full buffer, use all
				offset[crosses] = count;
				pastzero[crosses] = p[count];
				previous[crosses] = p[countone];
				third[crosses] = p[counttwo];
				//we load the zero crosses register with crosses to examine
				//the purpose is amassing situations to compare potential zero splice points
				//and switch at the moment the wave angles are most similar
			} //we just put in a source zero cross in the registry

		position -= speed;
		
		if (position > width) {
			if (realzeroes > 0) { //we just caught up to the buffer end with zero crosses in the bin
				position = 0;
				Float64 diff = 99999999.0;
				int best = 0;
				int scan;
				for(scan = (realzeroes-1); scan >= 0; scan--) {
					int scanone = scan + crosses;
					if (scanone > 256){scanone -= 256;}
					//try to track the real most recent ones more closely
					Float64 howdiff = (Float64)((temp - pastzero[scanone]) + (lasttemp - previous[scanone]) + (thirdtemp - third[scanone]) + (fourthtemp - fourth[scanone]));
					//got difference factor between things
					howdiff -= (Float64)(scan*bias);
					//try to bias in favor of more recent crosses
					if (howdiff < diff) {diff = howdiff; best = scanone;}
				} //now we have 'best' as the closest match to the current rate of zero cross and positioning- a splice.
				position = offset[best]-sincezerocross;
				crosses = 0;
				realzeroes = 0;
				splicing = true; //we just kicked the delay tap back
			} else {
				position -= width; //with no zero crosses in the bin
				crosses = 0;
				realzeroes = 0;
				splicing = true; //so, hard splice it.
			}
		}
		
		if (position < 0) { //we just caught up to the dry tap
			if (realzeroes > 0) { //we just caught up to the dry tap with zero crosses in the bin
				position = 0;
				Float64 diff = 99999999.0;
				int best = 0;
				int scan;
				for(scan = (realzeroes-1); scan >= 0; scan--) {
					int scanone = scan + crosses;
					if (scanone > 256){scanone -= 256;}
					//try to track the real most recent ones more closely
					Float64 howdiff = (Float64)((temp - pastzero[scanone]) + (lasttemp - previous[scanone]) + (thirdtemp - third[scanone]) + (fourthtemp - fourth[scanone]));
					//got difference factor between things
					howdiff -= (Float64)(scan*bias);
					//try to bias in favor of more recent crosses
					if (howdiff < diff) {diff = howdiff; best = scanone;}
				} //now we have 'best' as the closest match to the current rate of zero cross and positioning- a splice.
				position = offset[best]-sincezerocross;
				crosses = 0;
				realzeroes = 0;
				splicing = true; //we just kicked the delay tap back
			} else {
				position += width; //with no zero crosses in the bin
				crosses = 0;
				realzeroes = 0;
				splicing = true; //so, hard splice it.
			}
		}

		count = gcount - (int)floor(position);
		//we go back because the buffer goes forward this time
		countone = count+1;
		counttwo = count+2;
		//now we have counts zero, one, two, none of which have sanity checked values
		//we are interpolating, we ADD
		while (count < 0){count += width;}
		while (countone < 0){countone += width;}
		while (counttwo < 0){counttwo += width;}
		while (count > width){count -= width;} //this can only happen with very insane variables
		while (countone > width){countone -= width;}
		while (counttwo > width){counttwo -= width;}
		//here's where we do our shift against the rotating buffer
		temp = 0;
		temp += (SInt32)(p[count] * (1-(position-floor(position)))); //less as value moves away from .0
		temp += p[count+1]; //we can assume always using this in one way or another?
		temp += (SInt32)(p[count+2] * (position-floor(position))); //greater as value moves away from .0
		temp -= (SInt32)(((p[count]-p[count+1])-(p[count+1]-p[count+2]))/50); //interpolation hacks 'r us		
		temp /= 2; //gotta make temp be the same level scale as buffer
		//now we have our delay tap, which is going to do our pitch shifting
		
		if (abs(temp) > 8388352.0){temp = (lasttemp + (lasttemp - thirdtemp));}
		//kill ticks of bad buffer mojo by sticking with the trajectory. Ugly hack *shrug*
		
		sincezerocross++;
		if (sincezerocross < 0 || sincezerocross > width){sincezerocross = 0;} //just a sanity check
		if (splicing){temp = (temp + (lasttemp + (lasttemp - thirdtemp)))/2; splicing = false;}
		//do a smoother transition by taking the sample of transition and going half with it

		if ((lasttemp > 0 && temp < 0)||(lasttemp < 0 && temp > 0)) //delay tap crossed zero
		{
			sincezerocross = 0;
		} //we just restarted counting from the delay tap zero cross
		
		fourthtemp = thirdtemp;
		thirdtemp = lasttemp;
		lasttemp = temp;
		
		inputSample = ( *sourceP * (1-wet))+((Float64)(temp/(8388352.0))*wet);
		
		if (inputSample > 4.0) inputSample = 4.0;
		if (inputSample < -4.0) inputSample = -4.0;
		//this plugin can throw insane outputs so we'll put in a hard clip
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		destP += inNumChannels;
		sourceP += inNumChannels;
	}
}

