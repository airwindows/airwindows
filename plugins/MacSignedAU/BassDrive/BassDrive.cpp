/*
*	File:		BassDrive.cpp
*	
*	Version:	1.0
* 
*	Created:	5/24/19
*	
*	Copyright:  Copyright © 2019 Airwindows, Airwindows uses the MIT license
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
	BassDrive.cpp
	
=============================================================================*/
#include "BassDrive.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, BassDrive)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassDrive::BassDrive
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BassDrive::BassDrive(AudioUnit component)
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
//	BassDrive::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassDrive::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassDrive::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassDrive::GetParameterInfo(AudioUnitScope		inScope,
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
//	BassDrive::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassDrive::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassDrive::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BassDrive::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	BassDrive::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult BassDrive::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____BassDriveEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassDrive::BassDriveKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BassDrive::BassDriveKernel::Reset()
{
	for (int fcount = 0; fcount < 7; fcount++)
	{
		presenceInA[fcount] = 0.0;
		presenceOutA[fcount] = 0.0;
		highInA[fcount] = 0.0;
		highOutA[fcount] = 0.0;
		midInA[fcount] = 0.0;
		midOutA[fcount] = 0.0;
		lowInA[fcount] = 0.0;
		lowOutA[fcount] = 0.0;
		presenceInB[fcount] = 0.0;
		presenceOutB[fcount] = 0.0;
		highInB[fcount] = 0.0;
		highOutB[fcount] = 0.0;
		midInB[fcount] = 0.0;
		midOutB[fcount] = 0.0;
		lowInB[fcount] = 0.0;
		lowOutB[fcount] = 0.0;
	}
	flip = false;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BassDrive::BassDriveKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BassDrive::BassDriveKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	Float64 sum;
	Float64 presence = pow(GetParameter( kParam_One ),5) * 8.0;
	Float64 high = pow(GetParameter( kParam_Two ),3) * 4.0;
	Float64 mid = pow(GetParameter( kParam_Three ),2);
	Float64 low = GetParameter( kParam_Four ) / 4.0;
	Float64 drive = GetParameter( kParam_Five ) * 2.0;
	Float64 bridgerectifier;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;

		sum = 0.0;
		
		
		if (flip)
		{
			presenceInA[0] = presenceInA[1]; presenceInA[1] = presenceInA[2]; presenceInA[2] = presenceInA[3];
			presenceInA[3] = presenceInA[4]; presenceInA[4] = presenceInA[5]; presenceInA[5] = presenceInA[6]; 
			presenceInA[6] = inputSample * presence; presenceOutA[2] = presenceOutA[3];
			presenceOutA[3] = presenceOutA[4]; presenceOutA[4] = presenceOutA[5]; presenceOutA[5] = presenceOutA[6]; 
			presenceOutA[6] = (presenceInA[0] + presenceInA[6]) + 1.9152966321 * (presenceInA[1] + presenceInA[5]) 
			- (presenceInA[2] + presenceInA[4]) - 3.8305932641 * presenceInA[3]
			+ ( -0.2828214615 * presenceOutA[2]) + (  0.2613069963 * presenceOutA[3])
			+ ( -0.8628193852 * presenceOutA[4]) + (  0.5387164389 * presenceOutA[5]);
			bridgerectifier = fabs(presenceOutA[6]);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (presenceOutA[6] > 0.0){sum += bridgerectifier;}
			else {sum -= bridgerectifier;}			
			//presence section
			
			highInA[0] = highInA[1]; highInA[1] = highInA[2]; highInA[2] = highInA[3];
			highInA[3] = highInA[4]; highInA[4] = highInA[5]; highInA[5] = highInA[6]; 
			bridgerectifier = fabs(inputSample) * high;
			if (bridgerectifier > 1.57079633) {bridgerectifier = 1.57079633;}
			bridgerectifier = sin(bridgerectifier);
			if (inputSample > 0.0) {highInA[6] = bridgerectifier;}
			else {highInA[6] = -bridgerectifier;}
			highInA[6] *= high; highOutA[2] = highOutA[3];
			highOutA[3] = highOutA[4]; highOutA[4] = highOutA[5]; highOutA[5] = highOutA[6]; 
			highOutA[6] = (highInA[0] + highInA[6]) -   0.5141967433 * (highInA[1] + highInA[5]) 
			- (highInA[2] + highInA[4]) +   1.0283934866 * highInA[3]
			+ ( -0.2828214615 * highOutA[2]) + (  1.0195930909 * highOutA[3])
			+ ( -1.9633013869 * highOutA[4]) + (  2.1020162751 * highOutA[5]);
			bridgerectifier = fabs(highOutA[6]);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (highOutA[6] > 0.0){sum += bridgerectifier;}
			else {sum -= bridgerectifier;}			
			//high section
			
			midInA[0] = midInA[1]; midInA[1] = midInA[2]; midInA[2] = midInA[3];
			midInA[3] = midInA[4]; midInA[4] = midInA[5]; midInA[5] = midInA[6]; 
			bridgerectifier = fabs(inputSample) * mid;
			if (bridgerectifier > 1.57079633) {bridgerectifier = 1.57079633;}
			bridgerectifier = sin(bridgerectifier);
			if (inputSample > 0.0) {midInA[6] = bridgerectifier;}
			else {midInA[6] = -bridgerectifier;}
			midInA[6] *= mid; midOutA[2] = midOutA[3];
			midOutA[3] = midOutA[4]; midOutA[4] = midOutA[5]; midOutA[5] = midOutA[6]; 
			midOutA[6] = (midInA[0] + midInA[6]) - 1.1790257790 * (midInA[1] + midInA[5]) 
			- (midInA[2] + midInA[4]) + 2.3580515580 * midInA[3]
			+ ( -0.6292082828 * midOutA[2]) + (  2.7785843605 * midOutA[3])
			+ ( -4.6638295236 * midOutA[4]) + (  3.5142515802 * midOutA[5]);
			sum += midOutA[6];
			//mid section
			
			lowInA[0] = lowInA[1]; lowInA[1] = lowInA[2]; lowInA[2] = lowInA[3];
			lowInA[3] = lowInA[4]; lowInA[4] = lowInA[5]; lowInA[5] = lowInA[6]; 
			bridgerectifier = fabs(inputSample) * low;
			if (bridgerectifier > 1.57079633) {bridgerectifier = 1.57079633;}
			bridgerectifier = sin(bridgerectifier);
			if (inputSample > 0.0) {lowInA[6] = bridgerectifier;}
			else {lowInA[6] = -bridgerectifier;}
			lowInA[6] *= low; lowOutA[2] = lowOutA[3];
			lowOutA[3] = lowOutA[4]; lowOutA[4] = lowOutA[5]; lowOutA[5] = lowOutA[6]; 
			lowOutA[6] = (lowInA[0] + lowInA[6]) - 1.9193504547 * (lowInA[1] + lowInA[5]) 
			- (lowInA[2] + lowInA[4]) + 3.8387009093 * lowInA[3]
			+ ( -0.9195964462 * lowOutA[2]) + (  3.7538173833 * lowOutA[3])
			+ ( -5.7487775603 * lowOutA[4]) + (  3.9145559258 * lowOutA[5]);
			sum += lowOutA[6];
			//low section
		}
		else
		{
			presenceInB[0] = presenceInB[1]; presenceInB[1] = presenceInB[2]; presenceInB[2] = presenceInB[3];
			presenceInB[3] = presenceInB[4]; presenceInB[4] = presenceInB[5]; presenceInB[5] = presenceInB[6]; 
			presenceInB[6] = inputSample * presence; presenceOutB[2] = presenceOutB[3];
			presenceOutB[3] = presenceOutB[4]; presenceOutB[4] = presenceOutB[5]; presenceOutB[5] = presenceOutB[6]; 
			presenceOutB[6] = (presenceInB[0] + presenceInB[6]) + 1.9152966321 * (presenceInB[1] + presenceInB[5]) 
			- (presenceInB[2] + presenceInB[4]) - 3.8305932641 * presenceInB[3]
			+ ( -0.2828214615 * presenceOutB[2]) + (  0.2613069963 * presenceOutB[3])
			+ ( -0.8628193852 * presenceOutB[4]) + (  0.5387164389 * presenceOutB[5]);
			bridgerectifier = fabs(presenceOutB[6]);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (presenceOutB[6] > 0.0){sum += bridgerectifier;}
			else {sum -= bridgerectifier;}			
			//presence section
			
			highInB[0] = highInB[1]; highInB[1] = highInB[2]; highInB[2] = highInB[3];
			highInB[3] = highInB[4]; highInB[4] = highInB[5]; highInB[5] = highInB[6]; 
			bridgerectifier = fabs(inputSample) * high;
			if (bridgerectifier > 1.57079633) {bridgerectifier = 1.57079633;}
			bridgerectifier = sin(bridgerectifier);
			if (inputSample > 0.0) {highInB[6] = bridgerectifier;}
			else {highInB[6] = -bridgerectifier;}
			highInB[6] *= high; highOutB[2] = highOutB[3];
			highOutB[3] = highOutB[4]; highOutB[4] = highOutB[5]; highOutB[5] = highOutB[6]; 
			highOutB[6] = (highInB[0] + highInB[6]) -   0.5141967433 * (highInB[1] + highInB[5]) 
			- (highInB[2] + highInB[4]) +   1.0283934866 * highInB[3]
			+ ( -0.2828214615 * highOutB[2]) + (  1.0195930909 * highOutB[3])
			+ ( -1.9633013869 * highOutB[4]) + (  2.1020162751 * highOutB[5]);
			bridgerectifier = fabs(highOutB[6]);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (highOutB[6] > 0.0){sum += bridgerectifier;}
			else {sum -= bridgerectifier;}			
			//high section
			
			midInB[0] = midInB[1]; midInB[1] = midInB[2]; midInB[2] = midInB[3];
			midInB[3] = midInB[4]; midInB[4] = midInB[5]; midInB[5] = midInB[6]; 
			bridgerectifier = fabs(inputSample) * mid;
			if (bridgerectifier > 1.57079633) {bridgerectifier = 1.57079633;}
			bridgerectifier = sin(bridgerectifier);
			if (inputSample > 0.0) {midInB[6] = bridgerectifier;}
			else {midInB[6] = -bridgerectifier;}
			midInB[6] *= mid; midOutB[2] = midOutB[3];
			midOutB[3] = midOutB[4]; midOutB[4] = midOutB[5]; midOutB[5] = midOutB[6]; 
			midOutB[6] = (midInB[0] + midInB[6]) - 1.1790257790 * (midInB[1] + midInB[5]) 
			- (midInB[2] + midInB[4]) + 2.3580515580 * midInB[3]
			+ ( -0.6292082828 * midOutB[2]) + (  2.7785843605 * midOutB[3])
			+ ( -4.6638295236 * midOutB[4]) + (  3.5142515802 * midOutB[5]);
			sum += midOutB[6];
			//mid section
			
			lowInB[0] = lowInB[1]; lowInB[1] = lowInB[2]; lowInB[2] = lowInB[3];
			lowInB[3] = lowInB[4]; lowInB[4] = lowInB[5]; lowInB[5] = lowInB[6]; 
			bridgerectifier = fabs(inputSample) * low;
			if (bridgerectifier > 1.57079633) {bridgerectifier = 1.57079633;}
			bridgerectifier = sin(bridgerectifier);
			if (inputSample > 0.0) {lowInB[6] = bridgerectifier;}
			else {lowInB[6] = -bridgerectifier;}
			lowInB[6] *= low; lowOutB[2] = lowOutB[3];
			lowOutB[3] = lowOutB[4]; lowOutB[4] = lowOutB[5]; lowOutB[5] = lowOutB[6]; 
			lowOutB[6] = (lowInB[0] + lowInB[6]) - 1.9193504547 * (lowInB[1] + lowInB[5]) 
			- (lowInB[2] + lowInB[4]) + 3.8387009093 * lowInB[3]
			+ ( -0.9195964462 * lowOutB[2]) + (  3.7538173833 * lowOutB[3])
			+ ( -5.7487775603 * lowOutB[4]) + (  3.9145559258 * lowOutB[5]);
			sum += lowOutB[6];
			//low section
		}
		
		inputSample = fabs(sum) * drive;
		if (inputSample > 1.57079633) {inputSample = 1.57079633;}
		inputSample = sin(inputSample);
		if (sum < 0) inputSample = -inputSample;
		
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

