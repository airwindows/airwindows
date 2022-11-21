/*
*	File:		Apicolypse.cpp
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
	Apicolypse.cpp
	
=============================================================================*/
#include "Apicolypse.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Apicolypse)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Apicolypse::Apicolypse
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Apicolypse::Apicolypse(AudioUnit component)
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
//	Apicolypse::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Apicolypse::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Apicolypse::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Apicolypse::GetParameterInfo(AudioUnitScope		inScope,
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
//	Apicolypse::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Apicolypse::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Apicolypse::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Apicolypse::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Apicolypse::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Apicolypse::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ApicolypseEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Apicolypse::ApicolypseKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Apicolypse::ApicolypseKernel::Reset()
{
	for(int count = 0; count < 34; count++) {b[count] = 0;}
	lastSample = 0.0;
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Apicolypse::ApicolypseKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Apicolypse::ApicolypseKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	Float64 threshold = GetParameter( kParam_One );
	Float64 hardness;
	Float64 breakup = (1.0-(threshold/2.0))*3.14159265358979;
	Float64 bridgerectifier;
	Float64 sqdrive = GetParameter( kParam_Two );
	if (sqdrive > 1.0) sqdrive *= sqdrive;
	sqdrive = sqrt(sqdrive);
	Float64 indrive = GetParameter( kParam_Three );
	if (indrive > 1.0) indrive *= indrive;
	indrive *= (1.0-(0.008*sqdrive));
	//no gain loss of convolution for APIcolypse
	//calibrate this to match noise level with character at 1.0
	//you get for instance 0.819 and 1.0-0.819 is 0.181
	Float64 randy;
	Float64 outlevel = GetParameter( kParam_Four );
	
	if (threshold < 1) hardness = 1.0 / (1.0-threshold);
	else hardness = 999999999999999999999.0;
	//set up hardness to exactly fill gap between threshold and 0db
	//if threshold is literally 1 then hardness is infinite, so we make it very big
	
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		inputSample *= indrive;
		//calibrated to match gain through convolution and -0.3 correction
		if (sqdrive > 0.0){
			b[33] = b[32]; b[32] = b[31]; 
			b[31] = b[30]; b[30] = b[29]; b[29] = b[28]; b[28] = b[27]; b[27] = b[26]; b[26] = b[25]; b[25] = b[24]; b[24] = b[23]; 
			b[23] = b[22]; b[22] = b[21]; b[21] = b[20]; b[20] = b[19]; b[19] = b[18]; b[18] = b[17]; b[17] = b[16]; b[16] = b[15]; 
			b[15] = b[14]; b[14] = b[13]; b[13] = b[12]; b[12] = b[11]; b[11] = b[10]; b[10] = b[9]; b[9] = b[8]; b[8] = b[7]; 
			b[7] = b[6]; b[6] = b[5]; b[5] = b[4]; b[4] = b[3]; b[3] = b[2]; b[2] = b[1]; b[1] = b[0]; b[0] = inputSample * sqdrive;
			inputSample += (b[1] * (0.09299870608542582  - (0.00009582362368873*fabs(b[1]))));
			inputSample -= (b[2] * (0.11947847710741009  - (0.00004500891602770*fabs(b[2]))));
			inputSample += (b[3] * (0.09071606264761795  + (0.00005639498984741*fabs(b[3]))));
			inputSample -= (b[4] * (0.08561982770836980  - (0.00004964855606916*fabs(b[4]))));
			inputSample += (b[5] * (0.06440549220820363  + (0.00002428052139507*fabs(b[5]))));
			inputSample -= (b[6] * (0.05987991812840746  + (0.00000101867082290*fabs(b[6]))));
			inputSample += (b[7] * (0.03980233135839382  + (0.00003312430049041*fabs(b[7]))));
			inputSample -= (b[8] * (0.03648402630896925  - (0.00002116186381142*fabs(b[8]))));
			inputSample += (b[9] * (0.01826860869525248  + (0.00003115110025396*fabs(b[9]))));
			inputSample -= (b[10] * (0.01723968622495364  - (0.00002450634121718*fabs(b[10]))));
			inputSample += (b[11] * (0.00187588812316724  + (0.00002838206198968*fabs(b[11]))));
			inputSample -= (b[12] * (0.00381796423957237  - (0.00003155815499462*fabs(b[12]))));
			inputSample -= (b[13] * (0.00852092214496733  - (0.00001702651162392*fabs(b[13]))));
			inputSample += (b[14] * (0.00315560292270588  + (0.00002547861676047*fabs(b[14]))));
			inputSample -= (b[15] * (0.01258630914496868  - (0.00004555319243213*fabs(b[15]))));
			inputSample += (b[16] * (0.00536435648963575  + (0.00001812393657101*fabs(b[16]))));
			inputSample -= (b[17] * (0.01272975658159178  - (0.00004103775306121*fabs(b[17]))));
			inputSample += (b[18] * (0.00403818975172755  + (0.00003764615492871*fabs(b[18]))));
			inputSample -= (b[19] * (0.01042617366897483  - (0.00003605210426041*fabs(b[19]))));
			inputSample += (b[20] * (0.00126599583390057  + (0.00004305458668852*fabs(b[20]))));
			inputSample -= (b[21] * (0.00747876207688339  - (0.00003731207018977*fabs(b[21]))));
			inputSample -= (b[22] * (0.00149873689175324  - (0.00005086601800791*fabs(b[22]))));
			inputSample -= (b[23] * (0.00503221309488033  - (0.00003636086782783*fabs(b[23]))));
			inputSample -= (b[24] * (0.00342998224655821  - (0.00004103091180506*fabs(b[24]))));
			inputSample -= (b[25] * (0.00355585977903117  - (0.00003698982145400*fabs(b[25]))));
			inputSample -= (b[26] * (0.00437201792934817  - (0.00002720235666939*fabs(b[26]))));
			inputSample -= (b[27] * (0.00299217874451556  - (0.00004446954727956*fabs(b[27]))));
			inputSample -= (b[28] * (0.00457924652487249  - (0.00003859065778860*fabs(b[28]))));
			inputSample -= (b[29] * (0.00298182934892027  - (0.00002064710931733*fabs(b[29]))));
			inputSample -= (b[30] * (0.00438838441540584  - (0.00005223008424866*fabs(b[30]))));
			inputSample -= (b[31] * (0.00323984218794705  - (0.00003397987535887*fabs(b[31]))));
			inputSample -= (b[32] * (0.00407693981307314  - (0.00003935772436894*fabs(b[32]))));
			inputSample -= (b[33] * (0.00350435348467321  - (0.00005525463935338*fabs(b[33]))));}
		//we apply the first 28 samples of the Neve impulse- dynamically adjusted.
		if (fabs(inputSample) > threshold)
		{
			bridgerectifier = (fabs(inputSample)-threshold)*hardness;
			//skip flat area if any, scale to distortion limit
			if (bridgerectifier > breakup) bridgerectifier = breakup;
			//max value for sine function, 'breakup' modeling for trashed console tone
			//more hardness = more solidness behind breakup modeling. more softness, more 'grunge' and sag
			bridgerectifier = sin(bridgerectifier)/hardness;
			//do the sine factor, scale back to proper amount
			if (inputSample > 0) inputSample = bridgerectifier+threshold;
			else inputSample = -(bridgerectifier+threshold);
		}
		//otherwise we leave it untouched by the overdrive stuff
		randy = ((double(fpd)/UINT32_MAX)*0.033);
		inputSample = ((inputSample*(1-randy))+(lastSample*randy)) * outlevel;
		lastSample = inputSample;

		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

