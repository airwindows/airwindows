/*
*	File:		StudioTan.cpp
*	
*	Version:	1.0
* 
*	Created:	1/9/19
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
	StudioTan.cpp
	
=============================================================================*/
#include "StudioTan.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, StudioTan)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StudioTan::StudioTan
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StudioTan::StudioTan(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StudioTan::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StudioTan::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
	if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_One)) //ID must be actual name of parameter identifier, not number
	{
		if (outStrings == NULL) return noErr;
		CFStringRef strings [] =
		{
			kMenuItem_ST,
			kMenuItem_DM,
			kMenuItem_NJ,
			kMenuItem_STCD,
			kMenuItem_DMCD,
			kMenuItem_NJCD,
		};
		*outStrings = CFArrayCreate (
									 NULL,
									 (const void **) strings,
									 (sizeof (strings) / sizeof (strings [0])),
									 NULL
									 );
		return noErr;
	}
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StudioTan::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StudioTan::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = kST;
                outParameterInfo.maxValue = kNJCD;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
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
//	StudioTan::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StudioTan::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StudioTan::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			StudioTan::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	StudioTan::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult StudioTan::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____StudioTanEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StudioTan::StudioTanKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		StudioTan::StudioTanKernel::Reset()
{
	byn[0] = 1000.0;
	byn[1] = 301.0;
	byn[2] = 176.0;
	byn[3] = 125.0;
	byn[4] = 97.0;
	byn[5] = 79.0;
	byn[6] = 67.0;
	byn[7] = 58.0;
	byn[8] = 51.0;
	byn[9] = 46.0;
	byn[10] = 1000.0;
	noiseShaping = 0.0;
	lastSample = 0.0;
	lastSample2 = 0.0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	StudioTan::StudioTanKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		StudioTan::StudioTanKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	int processing = (int) GetParameter( kParam_One );
	bool highres = true; //for 24 bit: false for 16 bit
	bool brightfloor = true; //for Studio Tan: false for Dither Me Timbers
	bool benford = true; //for Not Just Another Dither: false for newer two
	bool cutbins = false; //for NJAD: only attenuate bins if one gets very full
	switch (processing)
	{
		case 0: benford = false; break; //Studio Tan 24
		case 1: benford = false; brightfloor = false; break; //Dither Me Timbers 24
		case 2: break; //Not Just Another Dither 24
		case 3: benford = false; highres = false; break; //Studio Tan 16
		case 4: benford = false; brightfloor = false; highres = false; break; //Dither Me Timbers 16
		case 5: highres = false; break; //Not Just Another Dither 16
	}
	
	while (nSampleFrames-- > 0) {
		double inputSample;
		double outputSample;
		double drySample;
		
		if (highres) inputSample = *sourceP * 8388608.0;
		else inputSample = *sourceP * 32768.0;
		//shared input stage
		
		if (benford) {
			//begin Not Just Another Dither
			cutbins = false;
			drySample = inputSample;
			inputSample -= noiseShaping;
			double benfordize = floor(inputSample);
			while (benfordize >= 1.0) {benfordize /= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			int hotbinA = floor(benfordize);
			//hotbin becomes the Benford bin value for this number floored
			double totalA = 0;
			if ((hotbinA > 0) && (hotbinA < 10))
			{
				byn[hotbinA] += 1;
				if (byn[hotbinA] > 982) cutbins = true;
				totalA += (301-byn[1]);
				totalA += (176-byn[2]);
				totalA += (125-byn[3]);
				totalA += (97-byn[4]);
				totalA += (79-byn[5]);
				totalA += (67-byn[6]);
				totalA += (58-byn[7]);
				totalA += (51-byn[8]);
				totalA += (46-byn[9]);
				byn[hotbinA] -= 1;
			} else {hotbinA = 10;}
			//produce total number- smaller is closer to Benford real
			
			benfordize = ceil(inputSample);
			while (benfordize >= 1.0) {benfordize /= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			int hotbinB = floor(benfordize);
			//hotbin becomes the Benford bin value for this number ceiled
			double totalB = 0;
			if ((hotbinB > 0) && (hotbinB < 10))
			{
				byn[hotbinB] += 1;
				if (byn[hotbinB] > 982) cutbins = true;
				totalB += (301-byn[1]);
				totalB += (176-byn[2]);
				totalB += (125-byn[3]);
				totalB += (97-byn[4]);
				totalB += (79-byn[5]);
				totalB += (67-byn[6]);
				totalB += (58-byn[7]);
				totalB += (51-byn[8]);
				totalB += (46-byn[9]);
				byn[hotbinB] -= 1;
			} else {hotbinB = 10;}
			//produce total number- smaller is closer to Benford real
			
			if (totalA < totalB)
			{
				byn[hotbinA] += 1;
				outputSample = floor(inputSample);
			}
			else
			{
				byn[hotbinB] += 1;
				outputSample = floor(inputSample+1);
			}
			//assign the relevant one to the delay line
			//and floor/ceil signal accordingly
			if (cutbins) {
				byn[1] *= 0.99;
				byn[2] *= 0.99;
				byn[3] *= 0.99;
				byn[4] *= 0.99;
				byn[5] *= 0.99;
				byn[6] *= 0.99;
				byn[7] *= 0.99;
				byn[8] *= 0.99;
				byn[9] *= 0.99;
				byn[10] *= 0.99; //catchall for garbage data
			}
			noiseShaping += outputSample - drySample;
			
			//end Not Just Another Dither
		} else {		
			//begin StudioTan or Dither Me Timbers
			if (brightfloor) {
				lastSample -= (noiseShaping*0.8);
				if ((lastSample+lastSample) <= (inputSample+lastSample2)) outputSample = floor(lastSample); //StudioTan
				else outputSample = floor(lastSample+1.0); //round down or up based on whether it softens treble angles
			} else {
				lastSample -= (noiseShaping*0.11);
				if ((lastSample+lastSample) >= (inputSample+lastSample2)) outputSample = floor(lastSample); //DitherMeTimbers
				else outputSample = floor(lastSample+1.0); //round down or up based on whether it softens treble angles
			}
			noiseShaping += outputSample;
			noiseShaping -= lastSample; //apply noise shaping
			lastSample2 = lastSample;
			lastSample = inputSample; //we retain three samples in a row
			//end StudioTan or Dither Me Timbers
		}
		
		//shared output stage
		double noiseSuppress = fabs(inputSample);
		if (noiseShaping > noiseSuppress) noiseShaping = noiseSuppress;
		if (noiseShaping < -noiseSuppress) noiseShaping = -noiseSuppress;
		
		Float32 ironBar;
		if (highres) ironBar = outputSample / 8388608.0;
		else ironBar = outputSample / 32768.0;
		
		if (ironBar > 1.0) ironBar = 1.0;
		if (ironBar < -1.0) ironBar = -1.0;
		*destP = ironBar;
		
		sourceP += inNumChannels;
		destP += inNumChannels;
	}
}



