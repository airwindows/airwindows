/*
*	File:		Elation.cpp
*	
*	Version:	1.0
* 
*	Created:	1/11/20
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
	Elation.cpp
	
=============================================================================*/
#include "Elation.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Elation)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Elation::Elation
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Elation::Elation(AudioUnit component)
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
//	Elation::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Elation::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Elation::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Elation::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.maxValue = 3.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 3.0;
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
//	Elation::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Elation::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Elation::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Elation::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Elation::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Elation::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ElationEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Elation::ElationKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Elation::ElationKernel::Reset()
{
	for(int count = 0; count < 34; count++) {b[count] = 0;}
	compA = 1.0; compB = 1.0; flip = false; previous = 0.0;
	compC = 1.0; compD = 1.0; previousB = 0.0;
	lastSample = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Elation::ElationKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Elation::ElationKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;

	Float64 wet = GetParameter( kParam_One );
	Float64 sqdrive = GetParameter( kParam_Two );
	if (sqdrive > 1.0) sqdrive *= sqdrive;
	sqdrive = sqrt(sqdrive);
	Float64 indrive = GetParameter( kParam_Three );
	Float64 compthreshold = (6.0 - indrive)/6.0;
	Float64 recoveryspd = indrive / 32.0;
	if (indrive > 1.0) indrive *= indrive;
	indrive *= (1.0+(0.226*sqdrive));
	//no gain loss of convolution for APIcolypse
	//calibrate this to match noise level with character at 1.0
	//you get for instance 0.819 and 1.0-0.819 is 0.181
	Float64 randy;
	Float64 outlevel = GetParameter( kParam_Four );
	Float64 threshSample;
	Float64 abSample;
	Float64 outputSample;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;

		inputSample *= indrive;
		
		abSample = fabs(inputSample);
		if (abSample > previous) threshSample = previous;
		else threshSample = abSample;
		//calibrated to match gain through convolution and -0.3 correction
		//quick lil compression
		if (threshSample > 2.0)
		{
			compA = (compA + (1.0 / threshSample)) / 2.0;
			compB = (compB + (1.0 / threshSample)) / 2.0;
		}
		
		if (flip)
		{
			inputSample *= compA;
			threshSample *= compA;
			if (threshSample > compthreshold)
			{compA = (compA + compA + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compA < 1.0) compA += pow((1.0-compA)*recoveryspd,7);}
		}
		else
		{
			inputSample *= compB;
			threshSample *= compB;
			if (threshSample > compthreshold)
			{compB = (compB + compB + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compB < 1.0) compB += pow((1.0-compB)*recoveryspd,7);}			
		}
		previous = abSample;
		//now the convolution
		if (sqdrive > 0.0){
			b[33] = b[32]; b[32] = b[31]; 
			b[31] = b[30]; b[30] = b[29]; b[29] = b[28]; b[28] = b[27]; b[27] = b[26]; b[26] = b[25]; b[25] = b[24]; b[24] = b[23]; 
			b[23] = b[22]; b[22] = b[21]; b[21] = b[20]; b[20] = b[19]; b[19] = b[18]; b[18] = b[17]; b[17] = b[16]; b[16] = b[15]; 
			b[15] = b[14]; b[14] = b[13]; b[13] = b[12]; b[12] = b[11]; b[11] = b[10]; b[10] = b[9]; b[9] = b[8]; b[8] = b[7]; 
			b[7] = b[6]; b[6] = b[5]; b[5] = b[4]; b[4] = b[3]; b[3] = b[2]; b[2] = b[1]; b[1] = b[0]; b[0] = inputSample * sqdrive;
			//inputSample -= ((b[1] * (0.25867935358656502  - (0.00045755657070112*fabs(b[1]))))*threshold);
			inputSample -= (b[1] * (0.25867935358656502  - (0.00045755657070112*fabs(b[1]))));
			inputSample += (b[2] * (0.11509367290253694  - (0.00017494270657228*fabs(b[2]))));
			inputSample -= (b[3] * (0.06709853575891785  - (0.00058913102597723*fabs(b[3]))));
			inputSample += (b[4] * (0.01871006356851681  - (0.00003387358004645*fabs(b[4]))));
			inputSample -= (b[5] * (0.00794797957360465  - (0.00044224784691203*fabs(b[5]))));
			inputSample -= (b[6] * (0.01956921817394220  - (0.00006718936750076*fabs(b[6]))));
			inputSample += (b[7] * (0.01682120257195205  + (0.00032857446292230*fabs(b[7]))));
			inputSample -= (b[8] * (0.03401069039824205  - (0.00013634182872897*fabs(b[8]))));
			inputSample += (b[9] * (0.02369950268232634  + (0.00023112685751657*fabs(b[9]))));
			inputSample -= (b[10] * (0.03477071178117132  - (0.00018029792231600*fabs(b[10]))));
			inputSample += (b[11] * (0.02024369717958201  + (0.00017337813374202*fabs(b[11]))));
			inputSample -= (b[12] * (0.02819087729102172  - (0.00021438538665420*fabs(b[12]))));
			inputSample += (b[13] * (0.01147946743141303  + (0.00014424066034649*fabs(b[13]))));
			inputSample -= (b[14] * (0.01894777011468867  - (0.00021549146262408*fabs(b[14]))));
			inputSample += (b[15] * (0.00301370330346873  + (0.00013527460148394*fabs(b[15]))));
			inputSample -= (b[16] * (0.01067147835815486  - (0.00020960689910868*fabs(b[16]))));
			inputSample -= (b[17] * (0.00402715397506384  - (0.00014421582712470*fabs(b[17]))));
			inputSample -= (b[18] * (0.00502221703392005  - (0.00019805767015024*fabs(b[18]))));
			inputSample -= (b[19] * (0.00808788533308497  - (0.00016095444141931*fabs(b[19]))));
			inputSample -= (b[20] * (0.00232696588842683  - (0.00018384470981829*fabs(b[20]))));
			inputSample -= (b[21] * (0.00943950821324531  - (0.00017098987347593*fabs(b[21]))));
			inputSample -= (b[22] * (0.00193709517200834  - (0.00018151995939591*fabs(b[22]))));
			inputSample -= (b[23] * (0.00899713952612659  - (0.00017385835059948*fabs(b[23]))));
			inputSample -= (b[24] * (0.00280584331659089  - (0.00017742164162470*fabs(b[24]))));
			inputSample -= (b[25] * (0.00780381001954970  - (0.00018002500755708*fabs(b[25]))));
			inputSample -= (b[26] * (0.00400370310490333  - (0.00017471691087957*fabs(b[26]))));
			inputSample -= (b[27] * (0.00661527728186928  - (0.00018137323370347*fabs(b[27]))));
			inputSample -= (b[28] * (0.00496545526864518  - (0.00017681872601767*fabs(b[28]))));
			inputSample -= (b[29] * (0.00580728820997532  - (0.00018186220389790*fabs(b[29]))));
			inputSample -= (b[30] * (0.00549309984725666  - (0.00017722985399075*fabs(b[30]))));
			inputSample -= (b[31] * (0.00542194777529239  - (0.00018486900185338*fabs(b[31]))));
			inputSample -= (b[32] * (0.00565992080998939  - (0.00018005824393118*fabs(b[32]))));
			inputSample -= (b[33] * (0.00532121562846656  - (0.00018643189636216*fabs(b[33]))));}
		//we apply the first samples of the impulse- dynamically adjusted.
		
		abSample = fabs(inputSample);
		if (abSample > previousB) threshSample = previousB;
		else threshSample = abSample;
		//calibrated to match gain through convolution and -0.3 correction
		//quick lil compression
		if (threshSample > 2.0)
		{
			compC = (compC + (1.0 / threshSample)) / 2.0;
			compD = (compD + (1.0 / threshSample)) / 2.0;
		}
		
		if (flip)
		{
			inputSample *= compC;
			threshSample *= compC;
			if (threshSample > compthreshold)
			{compC = (compC + compC + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compC < 1.0) compC += pow((1.0-compC)*recoveryspd,7);}
		}
		else
		{
			inputSample *= compD;
			threshSample *= compD;
			if (threshSample > compthreshold)
			{compD = (compD + compD + (compthreshold / threshSample)) / 3.0;}
			else
			{if (compD < 1.0) compD += pow((1.0-compD)*recoveryspd,7);}			
		}
		previousB = abSample;
		//another dose of compression, please
		
		flip = !flip;
		
		randy = ((double(fpd)/UINT32_MAX)*0.054);
		
		outputSample = ((((inputSample*(1-randy))+(lastSample*randy))*wet)+(drySample*(1.0-wet))) * outlevel;
		lastSample = inputSample;

		inputSample = outputSample;
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

