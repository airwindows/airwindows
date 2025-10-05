/*
*	File:		Slew4.cpp
*	
*	Version:	1.0
* 
*	Created:	9/24/25
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
	Slew4.cpp
	
=============================================================================*/
#include "Slew4.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Slew4)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Slew4::Slew4
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Slew4::Slew4(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Slew4::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Slew4::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Slew4::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Slew4::GetParameterInfo(AudioUnitScope		inScope,
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
//	Slew4::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Slew4::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Slew4::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Slew4::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Slew4::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Slew4::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Slew4EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Slew4::Slew4Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Slew4::Slew4Kernel::Reset()
{
	for(int x = 0; x < 23; x++) {
		for (int y = 0; y < 11; y++) slew[x][y] = 0.0;
	}
	slewCount = 0;
	sustainSlew = 0.0;
	secondSample = 0.0;
	thirdSample = 0.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Slew4::Slew4Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Slew4::Slew4Kernel::Process(	const Float32 	*inSourceP,
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

	double compresity = pow(GetParameter( kParam_A )*0.95,2.0)*8.0*overallscale;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;
		
		sustainSlew += fabs((inputSample-secondSample)-(secondSample-thirdSample));
		sustainSlew *= 0.9; inputSample = thirdSample; //two sample lookahead
		double depthAmt = fmin(sin(fmin(sustainSlew,M_PI_2))*compresity,9.0);
		int z = floor(depthAmt);
		double depthTrim = depthAmt - (double)z;
		
		for (int y = 0; y < z; y++) {
			switch (y) {
				case 0:
					slew[slewCount%2][y] = inputSample;
					inputSample = (slew[0][y] + slew[1][y]) * 0.5;
					break;
				case 1:
					slew[slewCount%4][y] = inputSample;
					inputSample = (slew[0][y]+slew[1][y]+slew[2][y]+slew[3][y]) * 0.25;
					break;
				case 2:
					slew[slewCount%6][y] = inputSample;
					inputSample = slew[0][y]+slew[1][y]+slew[2][y];
					inputSample += slew[3][y]+slew[4][y]+slew[5][y];
					inputSample *= 0.1666666666;
					break;
				case 3:
					slew[slewCount%8][y] = inputSample;
					inputSample = slew[0][y]+slew[1][y]+slew[2][y]+slew[3][y];
					inputSample += slew[4][y]+slew[5][y]+slew[6][y]+slew[7][y];
					inputSample *= 0.125;
					break;
				case 4:
					slew[slewCount%10][y] = inputSample;
					inputSample = slew[0][y]+slew[1][y]+slew[2][y]+slew[3][y]+slew[4][y];
					inputSample += slew[5][y]+slew[6][y]+slew[7][y]+slew[8][y]+slew[9][y];
					inputSample *= 0.1;
					break;
				case 5:
					slew[slewCount%12][y] = inputSample;
					inputSample = slew[0][y]+slew[1][y]+slew[2][y]+slew[3][y]+slew[4][y]+slew[5][y];
					inputSample += slew[6][y]+slew[7][y]+slew[8][y]+slew[9][y]+slew[10][y]+slew[11][y];
					inputSample *= 0.0833333333;
					break;
				case 6:
					slew[slewCount%14][y] = inputSample;
					inputSample = slew[0][y]+slew[1][y]+slew[2][y]+slew[3][y]+slew[4][y]+slew[5][y]+slew[6][y];
					inputSample += slew[7][y]+slew[8][y]+slew[9][y]+slew[10][y]+slew[11][y]+slew[12][y]+slew[13][y];
					inputSample *= 0.0714285;
					break;
				case 7:
					slew[slewCount%16][y] = inputSample;
					inputSample = slew[0][y]+slew[1][y]+slew[2][y]+slew[3][y]+slew[4][y]+slew[5][y]+slew[6][y]+slew[7][y];
					inputSample += slew[8][y]+slew[9][y]+slew[10][y]+slew[11][y]+slew[12][y]+slew[13][y]+slew[14][y]+slew[15][y];
					inputSample *= 0.0625;
					break;
				case 8:
					slew[slewCount%18][y] = inputSample;
					inputSample = slew[0][y]+slew[1][y]+slew[2][y]+slew[3][y]+slew[4][y]+slew[5][y]+slew[6][y]+slew[7][y]+slew[8][y];
					inputSample += slew[9][y]+slew[10][y]+slew[11][y]+slew[12][y]+slew[13][y]+slew[14][y]+slew[15][y]+slew[16][y]+slew[17][y];
					inputSample *= 0.05555555;
					break;
				case 9:
					slew[slewCount%20][y] = inputSample;
					inputSample = slew[0][y]+slew[1][y]+slew[2][y]+slew[3][y]+slew[4][y]+slew[5][y]+slew[6][y]+slew[7][y]+slew[8][y]+slew[9][y];
					inputSample += slew[10][y]+slew[11][y]+slew[12][y]+slew[13][y]+slew[14][y]+slew[15][y]+slew[16][y]+slew[17][y]+slew[18][y]+slew[19][y];
					inputSample *= 0.05;
					break;
			} //generate sets of rectangular window averages to layer
		} //all of which have a 'node' (silence) at Nyquist to combine
		
		double remainderSample = 0.0;
		switch (z) {
			case 0:
				slew[slewCount%2][z] = inputSample;
				remainderSample = (slew[0][z] + slew[1][z]) * 0.5;
				break;
			case 1:
				slew[slewCount%4][z] = inputSample;
				remainderSample = (slew[0][z]+slew[1][z]+slew[2][z]+slew[3][z]) * 0.25;
				break;
			case 2:
				slew[slewCount%6][z] = inputSample;
				remainderSample = slew[0][z]+slew[1][z]+slew[2][z];
				remainderSample += slew[3][z]+slew[4][z]+slew[5][z];
				remainderSample *= 0.1666666666;
				break;
			case 3:
				slew[slewCount%8][z] = inputSample;
				remainderSample = slew[0][z]+slew[1][z]+slew[2][z]+slew[3][z];
				remainderSample += slew[4][z]+slew[5][z]+slew[6][z]+slew[7][z];
				remainderSample *= 0.125;
				break;
			case 4:
				slew[slewCount%10][z] = inputSample;
				remainderSample = slew[0][z]+slew[1][z]+slew[2][z]+slew[3][z]+slew[4][z];
				remainderSample	+= slew[5][z]+slew[6][z]+slew[7][z]+slew[8][z]+slew[9][z];
				remainderSample *= 0.1;
				break;
			case 5:
				slew[slewCount%12][z] = inputSample;
				remainderSample = slew[0][z]+slew[1][z]+slew[2][z]+slew[3][z]+slew[4][z]+slew[5][z];
				remainderSample += slew[6][z]+slew[7][z]+slew[8][z]+slew[9][z]+slew[10][z]+slew[11][z];
				remainderSample *= 0.0833333333;
				break;
			case 6:
				slew[slewCount%14][z] = inputSample;
				remainderSample = slew[0][z]+slew[1][z]+slew[2][z]+slew[3][z]+slew[4][z]+slew[5][z]+slew[6][z];
				remainderSample += slew[7][z]+slew[8][z]+slew[9][z]+slew[10][z]+slew[11][z]+slew[12][z]+slew[13][z];
				remainderSample *= 0.0714285;
				break;
			case 7:
				slew[slewCount%16][z] = inputSample;
				remainderSample = slew[0][z]+slew[1][z]+slew[2][z]+slew[3][z]+slew[4][z]+slew[5][z]+slew[6][z]+slew[7][z];
				remainderSample += slew[8][z]+slew[9][z]+slew[10][z]+slew[11][z]+slew[12][z]+slew[13][z]+slew[14][z]+slew[15][z];
				remainderSample *= 0.0625;
				break;
			case 8:
				slew[slewCount%18][z] = inputSample;
				remainderSample = slew[0][z]+slew[1][z]+slew[2][z]+slew[3][z]+slew[4][z]+slew[5][z]+slew[6][z]+slew[7][z]+slew[8][z];
				remainderSample += slew[9][z]+slew[10][z]+slew[11][z]+slew[12][z]+slew[13][z]+slew[14][z]+slew[15][z]+slew[16][z]+slew[17][z];
				remainderSample *= 0.0555555;
				break;
			case 9:
				slew[slewCount%20][z] = inputSample;
				remainderSample = slew[0][z]+slew[1][z]+slew[2][z]+slew[3][z]+slew[4][z]+slew[5][z]+slew[6][z]+slew[7][z]+slew[8][z]+slew[9][z];
				remainderSample += slew[10][z]+slew[11][z]+slew[12][z]+slew[13][z]+slew[14][z]+slew[15][z]+slew[16][z]+slew[17][z]+slew[18][z]+slew[19][z];
				remainderSample *= 0.05;
				break;
		} //generate the final fractional amount
		
		for (int y = z+1; y < 9; y++) {
			switch (y) {
				case 0:
					slew[slewCount%2][y] = inputSample; break;
				case 1:
					slew[slewCount%4][y] = inputSample; break;
				case 2:
					slew[slewCount%6][y] = inputSample; break;
				case 3:
					slew[slewCount%8][y] = inputSample; break;
				case 4:
					slew[slewCount%10][y] = inputSample; break;
				case 5:
					slew[slewCount%12][y] = inputSample; break;
				case 6:
					slew[slewCount%14][y] = inputSample; break;
				case 7:
					slew[slewCount%16][y] = inputSample; break;
				case 8:
					slew[slewCount%18][y] = inputSample; break;
				case 9:
					slew[slewCount%20][y] = inputSample; break;
			}
		} //clean up the little buffers when not being actively used
		
		slewCount++; if (slewCount > 619315200) slewCount = 0;
		thirdSample = secondSample; secondSample = drySample;
		
		inputSample = (inputSample*(1.0-depthTrim))+(remainderSample*depthTrim);
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

