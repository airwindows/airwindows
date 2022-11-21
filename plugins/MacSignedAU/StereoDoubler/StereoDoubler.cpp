/*
 *	File:		StereoDoubler.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	2/7/22
 *	
 *	Copyright:  Copyright © 2022 Airwindows, Airwindows uses the MIT license
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
 StereoDoubler.cpp
 
 =============================================================================*/
#include "StereoDoubler.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, StereoDoubler)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StereoDoubler::StereoDoubler
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StereoDoubler::StereoDoubler(AudioUnit component)
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
//	StereoDoubler::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StereoDoubler::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
	
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StereoDoubler::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StereoDoubler::GetParameterInfo(AudioUnitScope		inScope,
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
//	StereoDoubler::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StereoDoubler::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 StereoDoubler::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	StereoDoubler::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StereoDoubler::GetProperty(	AudioUnitPropertyID inID,
												   AudioUnitScope 		inScope,
												   AudioUnitElement 	inElement,
												   void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	StereoDoubler::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult StereoDoubler::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____StereoDoublerEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StereoDoubler::StereoDoublerKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		StereoDoubler::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for(int count = 0; count < 5000; count++) {pL[count] = 0.0; pR[count] = 0.0;}
	for(int count = 0; count < 8; count++)
	{tempL[count] = 0.0; positionL[count] = 0.0; lastpositionL[count] = 0.0; trackingL[count] = 0.0;}
	for(int count = 0; count < 8; count++)
	{tempR[count] = 0.0; positionR[count] = 0.0; lastpositionR[count] = 0.0; trackingR[count] = 0.0;}
	gcountL = 0;
	lastcountL = 0;
	gcountR = 0;
	lastcountR = 0;
	prevwidth = 643;
	activeL = 0;
	bestspliceL = 4;
	activeR = 0;
	bestspliceR = 4;
	bestyetL = 1.0;
	bestyetR = 1.0;
	airPrevL = 0.0;
	airEvenL = 0.0;
	airOddL = 0.0;
	airFactorL = 0.0;
	airPrevR = 0.0;
	airEvenR = 0.0;
	airOddR = 0.0;
	airFactorR = 0.0;
	flip = false;
	
	for(int count = 0; count < 6; count++) {lastRefL[count] = 0.0;lastRefR[count] = 0.0;}
	cycle = 0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StereoDoubler::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		StereoDoubler::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
												  const AudioBufferList & inBuffer,
												  AudioBufferList & outBuffer,
												  UInt32 			inFramesToProcess)
{
	Float32 * inputL = (Float32*)(inBuffer.mBuffers[0].mData);
	Float32 * inputR = (Float32*)(inBuffer.mBuffers[1].mData);
	Float32 * outputL = (Float32*)(outBuffer.mBuffers[0].mData);
	Float32 * outputR = (Float32*)(outBuffer.mBuffers[1].mData);
	UInt32 nSampleFrames = inFramesToProcess;
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	Float64 trim = GetParameter( kParam_One );
	trim *= fabs(trim);
	trim /= 40;
	Float64 speedL = trim+1.0;
	Float64 speedR = (-trim)+1.0;
	if (speedL < 0.0) speedL = 0.0;
	if (speedR < 0.0) speedR = 0.0;
	
	int width = 2300;
	if (prevwidth != width)
	{
		positionL[0] = 0;
		positionL[1] = (int)(width/3);
		positionL[2] = (int)((width/3)*2);
		positionL[3] = (int)(width/5);
		positionL[4] = (int)((width/5)*2);
		positionL[5] = (int)((width/5)*3);
		positionL[6] = (int)((width/5)*4);
		positionL[7] = (int)(width/2);
		positionR[0] = 0;
		positionR[1] = (int)(width/3);
		positionR[2] = (int)((width/3)*2);
		positionR[3] = (int)(width/5);
		positionR[4] = (int)((width/5)*2);
		positionR[5] = (int)((width/5)*3);
		positionR[6] = (int)((width/5)*4);
		positionR[7] = (int)(width/2);
		prevwidth = width;
	}
	Float64 adjust = 1100;
	Float64 wet = GetParameter( kParam_Two );
	int gplusL;
	int lastplusL;
	int gplusR;
	int lastplusR;
	Float64 posplusL;
	Float64 lastposplusL;
	Float64 posplusR;
	Float64 lastposplusR;
	Float64 depth;
	Float64 crossfade;
	int count;
	int bcountL;
	int bcountR;
	int base;
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a doubler sample
			double drySampleL = inputSampleL;
			double drySampleR = inputSampleR;
			
			//assign working variables
			airFactorL = airPrevL - inputSampleL;
			if (flip) {airEvenL += airFactorL; airOddL -= airFactorL; airFactorL = airEvenL;}
			else {airOddL += airFactorL; airEvenL -= airFactorL; airFactorL = airOddL;}
			airOddL = (airOddL - ((airOddL - airEvenL)/256.0)) / 1.0001;
			airEvenL = (airEvenL - ((airEvenL - airOddL)/256.0)) / 1.0001;
			airPrevL = inputSampleL;
			inputSampleL += airFactorL;
			//left
			airFactorR = airPrevR - inputSampleR;
			if (flip) {airEvenR += airFactorR; airOddR -= airFactorR; airFactorR = airEvenR;}
			else {airOddR += airFactorR; airEvenR -= airFactorR; airFactorR = airOddR;}
			airOddR = (airOddR - ((airOddR - airEvenR)/256.0)) / 1.0001;
			airEvenR = (airEvenR - ((airEvenR - airOddR)/256.0)) / 1.0001;
			airPrevR = inputSampleR;
			inputSampleR += airFactorR;
			//right
			flip = !flip;
			//air, compensates for loss of highs in flanger's interpolation
			
			gcountL++;
			gcountR++;
			for(count = 0; count < 8; count++) {positionL[count] += speedL; positionR[count] += speedR;}
			
			gplusL = gcountL+(int)adjust;
			lastplusL = lastcountL+(int)adjust;
			if (gplusL > width) {gplusL -= width;}
			if (lastplusL > width) {lastplusL -= width;}
			
			gplusR = gcountR+(int)adjust;
			lastplusR = lastcountR+(int)adjust;
			if (gplusR > width) {gplusR -= width;}
			if (lastplusR > width) {lastplusR -= width;}
			
			if (trackingL[activeL] == 0.0)
			{
				posplusL = positionL[activeL]+adjust;
				lastposplusL = lastpositionL[activeL]+adjust;
				if (posplusL > width) {posplusL -= width;}
				if (lastposplusL > width) {lastposplusL -= width;}
				if ((gplusL > positionL[activeL]) && (lastplusL < lastpositionL[activeL])) {trackingL[activeL] = 1.0;}
				if ((posplusL > gcountL) && (lastposplusL < lastcountL)) {trackingL[activeL] = 1.0;}
				//fire splice based on whether somebody moved past somebody else just now
			}
			
			if (trackingR[activeR] == 0.0)
			{
				posplusR = positionR[activeR]+adjust;
				lastposplusR = lastpositionR[activeR]+adjust;
				if (posplusR > width) {posplusR -= width;}
				if (lastposplusR > width) {lastposplusR -= width;}
				if ((gplusR > positionR[activeR]) && (lastplusR < lastpositionR[activeR])) {trackingR[activeR] = 1.0;}
				if ((posplusR > gcountR) && (lastposplusR < lastcountR)) {trackingR[activeR] = 1.0;}
				//fire splice based on whether somebody moved past somebody else just now
			}
			
			for(count = 0; count < 8; count++) 
			{
				if (positionL[count] > width) {positionL[count] -= width;}
				if (positionR[count] > width) {positionR[count] -= width;}
				lastpositionL[count] = positionL[count];
				lastpositionR[count] = positionR[count];
			}
			if (gcountL < 0 || gcountL > width) {gcountL -= width;}
			lastcountL = bcountL = gcountL;
			if (gcountR < 0 || gcountR > width) {gcountR -= width;}
			lastcountR = bcountR = gcountR;
			
			pL[bcountL+width] = pL[bcountL] = inputSampleL;
			pR[bcountR+width] = pR[bcountR] = inputSampleR;
			
			
			for(count = 0; count < 8; count++)
			{
				base = (int)floor(positionL[count]);
				tempL[count] = (pL[base] * (1-(positionL[count]-base))); //less as value moves away from .0
				tempL[count] += pL[base+1]; //we can assume always using this in one way or another?
				tempL[count] += (pL[base+2] * (positionL[count]-base)); //greater as value moves away from .0
				tempL[count] -= (((pL[base]-pL[base+1])-(pL[base+1]-pL[base+2]))/50); //interpolation hacks 'r us		
				tempL[count] /= 2;
				
				base = (int)floor(positionR[count]);
				tempR[count] = (pR[base] * (1-(positionR[count]-base))); //less as value moves away from .0
				tempR[count] += pR[base+1]; //we can assume always using this in one way or another?
				tempR[count] += (pR[base+2] * (positionR[count]-base)); //greater as value moves away from .0
				tempR[count] -= (((pR[base]-pR[base+1])-(pR[base+1]-pR[base+2]))/50); //interpolation hacks 'r us		
				tempR[count] /= 2;
			}
			
			
			if (trackingL[activeL] > 0.0)
			{
				crossfade = sin(trackingL[bestspliceL]*1.57);
				inputSampleL = (tempL[activeL]*crossfade)+(tempL[bestspliceL]*(1.0-crossfade));
				
				for(count = 0; count < 8; count++)
				{
					depth = (0.5-fabs(tempL[activeL]-tempL[count]));
					if ((depth > 0) && (count != activeL))
					{trackingL[count] -= (depth/adjust);
						bestspliceL = count;}
					//take down the splicings but skip the current one
				}
				bestyetL = 1.0;
				for(count = 0; count < 8; count++)
				{
					if ((trackingL[count] < bestyetL)&&(count != activeL))
					{bestspliceL = count; bestyetL = trackingL[count];}
				}
				
				if (trackingL[bestspliceL] < 0.0)
				{
					for(count = 0; count < 8; count++)
					{trackingL[count] = 1.0;}
					activeL = bestspliceL;
					trackingL[activeL] = 0.0;
				}
			}
			else
			{inputSampleL = tempL[activeL];}
			
			if (trackingR[activeR] > 0.0)
			{
				crossfade = sin(trackingR[bestspliceR]*1.57);
				inputSampleR = (tempR[activeR]*crossfade)+(tempR[bestspliceR]*(1.0-crossfade));
				
				for(count = 0; count < 8; count++)
				{
					depth = (0.5-fabs(tempR[activeR]-tempR[count]));
					if ((depth > 0) && (count != activeR))
					{trackingR[count] -= (depth/adjust);
						bestspliceR = count;}
					//take down the splicings but skip the current one
				}
				bestyetR = 1.0;
				for(count = 0; count < 8; count++)
				{
					if ((trackingR[count] < bestyetR)&&(count != activeR))
					{bestspliceR = count; bestyetR = trackingR[count];}
				}
				
				if (trackingR[bestspliceR] < 0.0)
				{
					for(count = 0; count < 8; count++)
					{trackingR[count] = 1.0;}
					activeR = bestspliceR;
					trackingR[activeR] = 0.0;
				}					
			}
			else
			{inputSampleR = tempR[activeR];}
			
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		
		
		
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

