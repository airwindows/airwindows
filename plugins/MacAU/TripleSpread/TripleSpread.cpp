/*
*	File:		TripleSpread.cpp
*	
*	Version:	1.0
* 
*	Created:	10/2/20
*	
*	Copyright:  Copyright © 2020 Airwindows, Airwindows uses the MIT license
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
	TripleSpread.cpp
	
=============================================================================*/
#include "TripleSpread.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(TripleSpread)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TripleSpread::TripleSpread
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TripleSpread::TripleSpread(AudioUnit component)
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
//	TripleSpread::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			TripleSpread::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TripleSpread::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			TripleSpread::GetParameterInfo(AudioUnitScope		inScope,
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
//	TripleSpread::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			TripleSpread::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 TripleSpread::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	TripleSpread::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			TripleSpread::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	TripleSpread::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult TripleSpread::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____TripleSpreadEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TripleSpread::TripleSpreadKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		TripleSpread::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for (int count = 0; count < 131074; count++) {pL[count] = 0; pR[count] = 0;}
	for (int count = 0; count < 257; count++) {
		offsetL[count] = 0; pastzeroL[count] = 0; previousL[count] = 0; thirdL[count] = 0; fourthL[count] = 0;
		offsetR[count] = 0; pastzeroR[count] = 0; previousR[count] = 0; thirdR[count] = 0; fourthR[count] = 0;
	}
	crossesL = 0;
	realzeroesL = 0;
	tempL = 0;
	lasttempL = 0;
	thirdtempL = 0;
	fourthtempL = 0;
	sincezerocrossL = 0;
	airPrevL = 0.0;
	airEvenL = 0.0;
	airOddL = 0.0;
	airFactorL = 0.0;
	positionL = 0.0;
	splicingL = false;	
	
	crossesR = 0;
	realzeroesR = 0;
	tempR = 0;
	lasttempR = 0;
	thirdtempR = 0;
	fourthtempR = 0;
	sincezerocrossR = 0;
	airPrevR = 0.0;
	airEvenR = 0.0;
	airOddR = 0.0;
	airFactorR = 0.0;
	positionR = 0.0;
	splicingR = false;	
	
	gcount = 0;
	lastwidth = 16386;
	flip = false;
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TripleSpread::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		TripleSpread::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
													const AudioBufferList & inBuffer,
                                                    AudioBufferList & outBuffer,
                                                    UInt32 			inFramesToProcess)
{
	Float32 * inputL = (Float32*)(inBuffer.mBuffers[0].mData);
	Float32 * inputR = (Float32*)(inBuffer.mBuffers[1].mData);
	Float32 * outputL = (Float32*)(outBuffer.mBuffers[0].mData);
	Float32 * outputR = (Float32*)(outBuffer.mBuffers[1].mData);
	UInt32 nSampleFrames = inFramesToProcess;
	
	Float64 speed = GetParameter( kParam_One )*0.041666666666667;
	SInt32 width = (SInt32)(65536-((1-pow(1-GetParameter( kParam_Two ),2))*65530.0));
	Float64 bias = pow(GetParameter( kParam_Two ),3);
	Float64 wet = GetParameter( kParam_Three );
	
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		airFactorL = airPrevL - inputSampleL;
		if (flip) {airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;}
		else {airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;}
		airOddL = (airOddL - ((airOddL - airEvenL)/256.0)) / 1.0001;
		airEvenL = (airEvenL - ((airEvenL - airOddL)/256.0)) / 1.0001;
		airPrevL = inputSampleL;
		inputSampleL += airFactorL;
		
		airFactorR = airPrevR - inputSampleR;
		if (flip) {airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;}
		else {airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;}
		airOddR = (airOddR - ((airOddR - airEvenR)/256.0)) / 1.0001;
		airEvenR = (airEvenR - ((airEvenR - airOddR)/256.0)) / 1.0001;
		airPrevR = inputSampleR;
		inputSampleR += airFactorR;
		
		flip = !flip;
		//air, compensates for loss of highs of interpolation
		
		if (lastwidth != width){crossesL = 0; realzeroesL = 0; crossesR = 0; realzeroesR = 0; lastwidth = width;}
		//global: changing this resets both channels
		
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
		//this is a wrap on the overall buffers, so count, one and two are also common to both channels
		
		pL[count+width] = pL[count] = (int)((inputSampleL*8388352.0));
		pR[count+width] = pR[count] = (int)((inputSampleR*8388352.0));
		//double buffer -8388352 to 8388352 is equal to 24 bit linear space
		
		if ((pL[countone] > 0 && pL[count] < 0)||(pL[countone] < 0 && pL[count] > 0)) //source crossed zero
		{
			crossesL++;
			realzeroesL++;
			if (crossesL > 256) {crossesL = 0;} //wrap crosses to keep adding new crosses
			if (realzeroesL > 256) {realzeroesL = 256;} //don't wrap realzeroes, full buffer, use all
			offsetL[crossesL] = count;
			pastzeroL[crossesL] = pL[count];
			previousL[crossesL] = pL[countone];
			thirdL[crossesL] = pL[counttwo];
			//we load the zero crosses register with crosses to examine
		} //we just put in a source zero cross in the registry
		
		if ((pR[countone] > 0 && pR[count] < 0)||(pR[countone] < 0 && pR[count] > 0)) //source crossed zero
		{
			crossesR++;
			realzeroesR++;
			if (crossesR > 256) {crossesR = 0;} //wrap crosses to keep adding new crosses
			if (realzeroesR > 256) {realzeroesR = 256;} //don't wrap realzeroes, full buffer, use all
			offsetR[crossesR] = count;
			pastzeroR[crossesR] = pR[count];
			previousR[crossesR] = pR[countone];
			thirdR[crossesR] = pR[counttwo];
			//we load the zero crosses register with crosses to examine
		} //we just put in a source zero cross in the registry
		//in this we don't update count at all, so we can run them one after another because this is
		//feeding the system, not tracking the output of two parallel but non-matching output taps
		
		positionL -= speed; //this is individual to each channel!
		
		if (positionL > width) { //we just caught up to the buffer end
			if (realzeroesL > 0) { //we just caught up to the buffer end with zero crosses in the bin
				positionL = 0;
				double diff = 99999999.0; 
				int best = 0; //these can be local, I think
				int scan;
				for(scan = (realzeroesL-1); scan >= 0; scan--) {
					int scanone = scan + crossesL;
					if (scanone > 256){scanone -= 256;}
					//try to track the real most recent ones more closely
					double howdiff = (double)((tempL - pastzeroL[scanone]) + (lasttempL - previousL[scanone]) + (thirdtempL - thirdL[scanone]) + (fourthtempL - fourthL[scanone]));
					//got difference factor between things
					howdiff -= (double)(scan*bias);
					//try to bias in favor of more recent crosses
					if (howdiff < diff) {diff = howdiff; best = scanone;}
				} //now we have 'best' as the closest match to the current rate of zero cross and positioning- a splice.
				positionL = offsetL[best]-sincezerocrossL;
				crossesL = 0;
				realzeroesL = 0;
				splicingL = true; //we just kicked the delay tap back, changing positionL
			} else { //we just caught up to the buffer end with no crosses- glitch speeds.
				positionL -= width;
				crossesL = 0;
				realzeroesL = 0;
				splicingL = true; //so, hard splice it.
			}
		}
		
		if (positionL < 0) { //we just caught up to the dry tap.
			if (realzeroesL > 0) { //we just caught up to the dry tap with zero crosses in the bin
				positionL = 0;
				double diff = 99999999.0; 
				int best = 0; //these can be local, I think
				int scan;
				for(scan = (realzeroesL-1); scan >= 0; scan--) {
					int scanone = scan + crossesL;
					if (scanone > 256){scanone -= 256;}
					//try to track the real most recent ones more closely
					double howdiff = (double)((tempL - pastzeroL[scanone]) + (lasttempL - previousL[scanone]) + (thirdtempL - thirdL[scanone]) + (fourthtempL - fourthL[scanone]));
					//got difference factor between things
					howdiff -= (double)(scan*bias);
					//try to bias in favor of more recent crosses
					if (howdiff < diff) {diff = howdiff; best = scanone;}
				} //now we have 'best' as the closest match to the current rate of zero cross and positioning- a splice.
				positionL = offsetL[best]-sincezerocrossL;
				crossesL = 0;
				realzeroesL = 0;
				splicingL = true; //we just kicked the delay tap back, changing positionL
			} else { //we just caught up to the dry tap with no crosses- glitch speeds.
				positionL += width;
				crossesL = 0;
				realzeroesL = 0;
				splicingL = true; //so, hard splice it.
			}
		}
		
		positionR += speed; //this is individual to each channel and inverted for R!
		
		if (positionR > width) { //we just caught up to the buffer end
			if (realzeroesR > 0) { //we just caught up to the buffer end with zero crosses in the bin
				positionR = 0;
				double diff = 99999999.0; 
				int best = 0; //these can be local, I think
				int scan;
				for(scan = (realzeroesR-1); scan >= 0; scan--) {
					int scanone = scan + crossesR;
					if (scanone > 256){scanone -= 256;}
					//try to track the real most recent ones more closely
					double howdiff = (double)((tempR - pastzeroR[scanone]) + (lasttempR - previousR[scanone]) + (thirdtempR - thirdR[scanone]) + (fourthtempR - fourthR[scanone]));
					//got difference factor between things
					howdiff -= (double)(scan*bias);
					//try to bias in favor of more recent crosses
					if (howdiff < diff) {diff = howdiff; best = scanone;}
				} //now we have 'best' as the closest match to the current rate of zero cross and positioning- a splice.
				positionR = offsetR[best]-sincezerocrossR;
				crossesR = 0;
				realzeroesR = 0;
				splicingR = true; //we just kicked the delay tap back, changing positionL
			} else { //we just caught up to the buffer end with no crosses- glitch speeds.
				positionR -= width;
				crossesR = 0;
				realzeroesR = 0;
				splicingR = true; //so, hard splice it.
			}
		}
		
		if (positionR < 0) { //we just caught up to the dry tap.
			if (realzeroesR > 0) { //we just caught up to the dry tap with zero crosses in the bin
				positionR = 0;
				double diff = 99999999.0; 
				int best = 0; //these can be local, I think
				int scan;
				for(scan = (realzeroesR-1); scan >= 0; scan--) {
					int scanone = scan + crossesR;
					if (scanone > 256){scanone -= 256;}
					//try to track the real most recent ones more closely
					double howdiff = (double)((tempR - pastzeroR[scanone]) + (lasttempR - previousR[scanone]) + (thirdtempR - thirdR[scanone]) + (fourthtempR - fourthR[scanone]));
					//got difference factor between things
					howdiff -= (double)(scan*bias);
					//try to bias in favor of more recent crosses
					if (howdiff < diff) {diff = howdiff; best = scanone;}
				} //now we have 'best' as the closest match to the current rate of zero cross and positioning- a splice.
				positionR = offsetR[best]-sincezerocrossR;
				crossesR = 0;
				realzeroesR = 0;
				splicingR = true; //we just kicked the delay tap back, changing positionL
			} else { //we just caught up to the dry tap with no crosses- glitch speeds.
				positionR += width;
				crossesR = 0;
				realzeroesR = 0;
				splicingR = true; //so, hard splice it.
			}
		}
		
		
		count = gcount - (int)floor(positionL);
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
		tempL = 0;
		tempL += (int)(pL[count] * (1-(positionL-floor(positionL)))); //less as value moves away from .0
		tempL += pL[count+1]; //we can assume always using this in one way or another?
		tempL += (int)(pL[count+2] * (positionL-floor(positionL))); //greater as value moves away from .0
		tempL -= (int)(((pL[count]-pL[count+1])-(pL[count+1]-pL[count+2]))/50); //interpolation hacks 'r us		
		tempL /= 2; //gotta make temp be the same level scale as buffer
		//now we have our delay tap, which is going to do our pitch shifting
		if (abs(tempL) > 8388352.0){tempL = (lasttempL + (lasttempL - thirdtempL));}
		//kill ticks of bad buffer mojo by sticking with the trajectory. Ugly hack *shrug*
		
		sincezerocrossL++;
		if (sincezerocrossL < 0 || sincezerocrossL > width){sincezerocrossL = 0;} //just a sanity check
		if (splicingL){tempL = (tempL + (lasttempL + (lasttempL - thirdtempL)))/2; splicingL = false;}
		//do a smoother transition by taking the sample of transition and going half with it
		
		if ((lasttempL > 0 && tempL < 0)||(lasttempL < 0 && tempL > 0)) //delay tap crossed zero
		{
			sincezerocrossL = 0;
		} //we just restarted counting from the delay tap zero cross
		
		count = gcount - (int)floor(positionR);
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
		
		
		tempR = 0;
		tempR += (int)(pR[count] * (1-(positionR-floor(positionR)))); //less as value moves away from .0
		tempR += pR[count+1]; //we can assume always using this in one way or another?
		tempR += (int)(pR[count+2] * (positionR-floor(positionR))); //greater as value moves away from .0
		tempR -= (int)(((pR[count]-pR[count+1])-(pR[count+1]-pR[count+2]))/50); //interpolation hacks 'r us		
		tempR /= 2; //gotta make temp be the same level scale as buffer
		//now we have our delay tap, which is going to do our pitch shifting
		if (abs(tempR) > 8388352.0){tempR = (lasttempR + (lasttempR - thirdtempR));}
		//kill ticks of bad buffer mojo by sticking with the trajectory. Ugly hack *shrug*
		
		sincezerocrossR++;
		if (sincezerocrossR < 0 || sincezerocrossR > width){sincezerocrossR = 0;} //just a sanity check
		if (splicingR){tempR = (tempR + (lasttempR + (lasttempR - thirdtempR)))/2; splicingR = false;}
		//do a smoother transition by taking the sample of transition and going half with it
		
		if ((lasttempR > 0 && tempR < 0)||(lasttempR < 0 && tempR > 0)) //delay tap crossed zero
		{
			sincezerocrossR = 0;
		} //we just restarted counting from the delay tap zero cross
		
		fourthtempL = thirdtempL;
		thirdtempL = lasttempL;
		lasttempL = tempL;
		
		fourthtempR = thirdtempR;
		thirdtempR = lasttempR;
		lasttempR = tempR;
		
		double mid = (inputSampleL + inputSampleR)*(1-wet);
		double side = inputSampleL - inputSampleR;
		//assign mid and side.Between these sections, you can do mid/side processing
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		//unassign mid and side		
		
		inputSampleL = ( drySampleL * (1-wet))+((double)(tempL/(8388352.0))*wet);
		if (inputSampleL > 4.0) inputSampleL = 4.0;
		if (inputSampleL < -4.0) inputSampleL = -4.0;
		
		inputSampleR = ( drySampleR * (1-wet))+((double)(tempR/(8388352.0))*wet);
		if (inputSampleR > 4.0) inputSampleR = 4.0;
		if (inputSampleR < -4.0) inputSampleR = -4.0;
		//this plugin can throw insane outputs so we'll put in a hard clip
		
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

