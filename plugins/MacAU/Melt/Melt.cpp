/*
*	File:		Melt.cpp
*	
*	Version:	1.0
* 
*	Created:	1/12/17
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
	Melt.cpp
	
=============================================================================*/
#include "Melt.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Melt)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Melt::Melt
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Melt::Melt(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Melt::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Melt::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Melt::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Melt::GetParameterInfo(AudioUnitScope		inScope,
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
//	Melt::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Melt::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Melt::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Melt::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Melt::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Melt::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____MeltEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Melt::MeltKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Melt::MeltKernel::Reset()
{
	for(int count = 0; count < 32001; count++) {d[count] = 0;}
	for(int count = 0; count < 31; count++) {minTap[count] = 0; maxTap[count] = 0; position[count] = 1; stepTap[count] = 1;}
	combine = 0;
	scalefactor = 0.999;
	stepCount = 0;
	slowCount = 0;
	gcount = 0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Melt::MeltKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Melt::MeltKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	Float64 rate = 1 / (pow(GetParameter( kParam_One ),2) + 0.001);
	Float64 depthB = (GetParameter( kParam_Two ) * 139.5)+2;
	Float64 depthA = depthB * (1.0 - GetParameter( kParam_One ));
	Float64 output = GetParameter( kParam_Three ) * 0.05;
	Float64 wet = GetParameter( kParam_Four );
	Float64 dry = 1.0-wet;
	
	minTap[0] = floor(2 * depthA); maxTap[0] = floor(2 * depthB);
	minTap[1] = floor(3 * depthA); maxTap[1] = floor(3 * depthB);
	minTap[2] = floor(5 * depthA); maxTap[2] = floor(5 * depthB);
	minTap[3] = floor(7 * depthA); maxTap[3] = floor(7 * depthB);
	minTap[4] = floor(11 * depthA); maxTap[4] = floor(11 * depthB);
	minTap[5] = floor(13 * depthA); maxTap[5] = floor(13 * depthB);
	minTap[6] = floor(17 * depthA); maxTap[6] = floor(17 * depthB);
	minTap[7] = floor(19 * depthA); maxTap[7] = floor(19 * depthB);
	minTap[8] = floor(23 * depthA); maxTap[8] = floor(23 * depthB);
	minTap[9] = floor(29 * depthA); maxTap[9] = floor(29 * depthB);
	minTap[10] = floor(31 * depthA); maxTap[10] = floor(31 * depthB);
	minTap[11] = floor(37 * depthA); maxTap[11] = floor(37 * depthB);
	minTap[12] = floor(41 * depthA); maxTap[12] = floor(41 * depthB);
	minTap[13] = floor(43 * depthA); maxTap[13] = floor(43 * depthB);
	minTap[14] = floor(47 * depthA); maxTap[14] = floor(47 * depthB);
	minTap[15] = floor(53 * depthA); maxTap[15] = floor(53 * depthB);
	minTap[16] = floor(59 * depthA); maxTap[16] = floor(59 * depthB);
	minTap[17] = floor(61 * depthA); maxTap[17] = floor(61 * depthB);
	minTap[18] = floor(67 * depthA); maxTap[18] = floor(67 * depthB);
	minTap[19] = floor(71 * depthA); maxTap[19] = floor(71 * depthB);
	minTap[20] = floor(73 * depthA); maxTap[20] = floor(73 * depthB);
	minTap[21] = floor(79 * depthA); maxTap[21] = floor(79 * depthB);
	minTap[22] = floor(83 * depthA); maxTap[22] = floor(83 * depthB);
	minTap[23] = floor(89 * depthA); maxTap[23] = floor(89 * depthB);
	minTap[24] = floor(97 * depthA); maxTap[24] = floor(97 * depthB);
	minTap[25] = floor(101 * depthA); maxTap[25] = floor(101 * depthB);
	minTap[26] = floor(103 * depthA); maxTap[26] = floor(103 * depthB);
	minTap[27] = floor(107 * depthA); maxTap[27] = floor(107 * depthB);
	minTap[28] = floor(109 * depthA); maxTap[28] = floor(109 * depthB);
	minTap[29] = floor(113 * depthA); maxTap[29] = floor(113 * depthB);
	minTap[30] = floor(117 * depthA); maxTap[30] = floor(117 * depthB);	
	double drySample;
	double inputSample;
	
	while (nSampleFrames-- > 0) {
		inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		drySample = inputSample;
		
		if (gcount < 0 || gcount > 16000) {gcount = 16000;}
		d[gcount+16000] = d[gcount] = inputSample;
		
		if (slowCount > rate || slowCount < 0) {
			slowCount = 0;
			stepCount++;
			if (stepCount > 29 || stepCount < 0) {stepCount = 0;}
			position[stepCount] += stepTap[stepCount];
			if (position[stepCount] < minTap[stepCount]) {
				position[stepCount] = minTap[stepCount];
				stepTap[stepCount] = 1;
			}
			if (position[stepCount] > maxTap[stepCount]) {
				position[stepCount] = maxTap[stepCount];
				stepTap[stepCount] = -1;
			}
		}
		scalefactor *= 0.9999;
		scalefactor += (100.0 - fabs(combine)) * 0.000001;
		
		combine *= scalefactor;
		combine -= (d[gcount+position[29]]);
		combine += (d[gcount+position[28]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[27]]);
		combine += (d[gcount+position[26]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[25]]);
		combine += (d[gcount+position[24]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[23]]);
		combine += (d[gcount+position[22]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[21]]);
		combine += (d[gcount+position[20]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[19]]);
		combine += (d[gcount+position[18]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[17]]);
		combine += (d[gcount+position[16]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[15]]);
		combine += (d[gcount+position[14]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[13]]);
		combine += (d[gcount+position[12]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[11]]);
		combine += (d[gcount+position[10]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[9]]);
		combine += (d[gcount+position[8]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[7]]);
		combine += (d[gcount+position[6]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[5]]);
		combine += (d[gcount+position[4]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[3]]);
		combine += (d[gcount+position[2]]);
		
		combine *= scalefactor;
		combine -= (d[gcount+position[1]]);
		combine += (d[gcount+position[0]]);
		
		gcount--;
		slowCount++;
		
		inputSample = combine;
		
		if (output < 1.0) inputSample *= output;
		if (wet < 1.0) inputSample = (drySample * dry)+(inputSample*wet);
		//nice little output stage template: if we have another scale of floating point
		//number, we really don't want to meaninglessly multiply that by 1.0.
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

