/*
*	File:		BezEQ4.cpp
*	
*	Version:	1.0
* 
*	Created:	5/28/26
*	
*	Copyright:  Copyright © 2026 Airwindows, Airwindows uses the MIT license
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
	BezEQ4.cpp
	
=============================================================================*/
#include "BezEQ4.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, BezEQ4)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BezEQ4::BezEQ4
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BezEQ4::BezEQ4(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
	SetParameter(kParam_D, kDefaultValue_ParamD );
	SetParameter(kParam_E, kDefaultValue_ParamE );
	SetParameter(kParam_F, kDefaultValue_ParamF );
	SetParameter(kParam_G, kDefaultValue_ParamG );
	SetParameter(kParam_H, kDefaultValue_ParamH );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BezEQ4::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BezEQ4::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BezEQ4::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BezEQ4::GetParameterInfo(AudioUnitScope		inScope,
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
            case kParam_B:
                AUBase::FillInParameterName (outParameterInfo, kParameterBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamB;
                break;
            case kParam_C:
                AUBase::FillInParameterName (outParameterInfo, kParameterCName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamC;
                break;
            case kParam_D:
                AUBase::FillInParameterName (outParameterInfo, kParameterDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamD;
                break;
            case kParam_E:
                AUBase::FillInParameterName (outParameterInfo, kParameterEName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamE;
                break;
            case kParam_F:
                AUBase::FillInParameterName (outParameterInfo, kParameterFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamF;
                break;
            case kParam_G:
                AUBase::FillInParameterName (outParameterInfo, kParameterGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamG;
                break;
            case kParam_H:
                AUBase::FillInParameterName (outParameterInfo, kParameterHName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamH;
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
//	BezEQ4::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BezEQ4::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BezEQ4::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BezEQ4::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	BezEQ4::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult BezEQ4::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____BezEQ4EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BezEQ4::BezEQ4Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BezEQ4::BezEQ4Kernel::Reset()
{
	for (int x = 0; x < bez_EQtotal; x++) {for (int y = 0; y < 3; y++) bezEQ[x][y] = 0.0;}
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BezEQ4::BezEQ4Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BezEQ4::BezEQ4Kernel::Process(	const Float32 	*inSourceP,
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
	
	//begin BezEQ4 control setup
	double highGain = (pow(GetParameter( kParam_A )+0.5,3.0)+2.0)*0.3333333;
	double hmidGain = (pow(GetParameter( kParam_B )+0.5,3.0)+2.0)*0.3333333;
	double lmidGain = (pow(GetParameter( kParam_C )+0.5,3.0)+2.0)*0.3333333;
	double bassGain = (pow(GetParameter( kParam_D )+0.5,3.0)+2.0)*0.3333333;
	double trebleRef = GetParameter( kParam_E )-0.3;
	double highmidRef = GetParameter( kParam_F )-0.5;
	double lowmidRef = GetParameter( kParam_G )-0.7;
	double bassRef = GetParameter( kParam_H )-0.9;
	double HMderez = 0.75 + (trebleRef*0.25);
	double LMderez = 0.25 + (bassRef*0.25);
	double Mderez = ((HMderez+LMderez+highmidRef+lowmidRef)*0.25);
	HMderez /= overallscale; Mderez /= overallscale; LMderez /= overallscale;
	if (HMderez > 1.0) HMderez = 1.0;
	int stepped = 999999; if (HMderez > 0.000001) stepped = (int)(1.0/HMderez);
	HMderez = 0.99999999 / stepped;
	double HMtrim = 1.0-(HMderez*((double)stepped/(stepped+1.0)));
	stepped = 999999; if (Mderez > 0.000001) stepped = (int)(1.0/Mderez);
	Mderez = 0.99999999 / stepped;
	double Mtrim = 1.0-(Mderez*((double)stepped/(stepped+1.0)));
	stepped = 999999; if (LMderez > 0.000001) stepped = (int)(1.0/LMderez);
	LMderez = 0.99999999 / stepped;
	double LMtrim = 1.0-(LMderez*((double)stepped/(stepped+1.0)));
	double highQ = 0.84+((highmidRef-trebleRef)*0.1666666);
	double midQ = 0.84+((lowmidRef-highmidRef)*0.1666666);
	double lowQ = 0.84+((bassRef-lowmidRef)*0.1666666);
	//end BezEQ4 control setup
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpd * 1.18e-17;
		
		//begin BezEQ4
		for (int p = 0; p < 3; p++) {
			double high = inputSampleL;
			bezEQ[bez_HMcycle][p] += HMderez;
			bezEQ[bez_HMAL][p] += (high * HMderez);
			if (bezEQ[bez_HMcycle][p] > HMtrim) {
				bezEQ[bez_HMDL][p] = bezEQ[bez_HMCL][p]; bezEQ[bez_HMCL][p] = bezEQ[bez_HMBL][p]; 
				bezEQ[bez_HMBL][p] = bezEQ[bez_HMAL][p]*(0.5-(HMderez*0.082));
				bezEQ[bez_HMAL][p] = 0.0; bezEQ[bez_HMcycle][p] = 0.0;
			}
			double X = bezEQ[bez_HMcycle][p];
			double hmid = bezEQ[bez_HMCL][p]+(bezEQ[bez_HMDL][p]*(1.0-X)*(1.0-X));
			hmid += (bezEQ[bez_HMCL][p]*2.0*(1.0-X)*X)+(bezEQ[bez_HMBL][p]*X*X);
			hmid *= highQ;
			high -= hmid; //final high and hmid
			bezEQ[bez_Mcycle][p] += Mderez;
			bezEQ[bez_MAL][p] += (hmid * Mderez);
			if (bezEQ[bez_Mcycle][p] > Mtrim) {
				bezEQ[bez_MDL][p] = bezEQ[bez_MCL][p]; bezEQ[bez_MCL][p] = bezEQ[bez_MBL][p]; 
				bezEQ[bez_MBL][p] = bezEQ[bez_MAL][p]*(0.5-(Mderez*0.082));
				bezEQ[bez_MAL][p] = 0.0; bezEQ[bez_Mcycle][p] = 0.0;
			}
			X = bezEQ[bez_Mcycle][p];
			double lmid = bezEQ[bez_MCL][p]+(bezEQ[bez_MDL][p]*(1.0-X)*(1.0-X));
			lmid += (bezEQ[bez_MCL][p]*2.0*(1.0-X)*X)+(bezEQ[bez_MBL][p]*X*X);
			lmid *= midQ;
			hmid -= lmid; //final hmid and lmid
			bezEQ[bez_LMcycle][p] += LMderez;
			bezEQ[bez_LMAL][p] += (lmid * LMderez);
			if (bezEQ[bez_LMcycle][p] > LMtrim) {
				bezEQ[bez_LMDL][p] = bezEQ[bez_LMCL][p]; bezEQ[bez_LMCL][p] = bezEQ[bez_LMBL][p]; 
				bezEQ[bez_LMBL][p] = bezEQ[bez_LMAL][p]*(0.5-(LMderez*0.082));
				bezEQ[bez_LMAL][p] = 0.0; bezEQ[bez_LMcycle][p] = 0.0;
			}
			X = bezEQ[bez_LMcycle][p];
			double bass = bezEQ[bez_LMCL][p]+(bezEQ[bez_LMDL][p]*(1.0-X)*(1.0-X));
			bass += (bezEQ[bez_LMCL][p]*2.0*(1.0-X)*X)+(bezEQ[bez_LMBL][p]*X*X);
			bass *= lowQ;
			lmid -= bass; //final lmid and bass
			inputSampleL = (high*highGain)+(hmid*hmidGain)+(lmid*lmidGain)+(bass*bassGain);
		}
		//end BezEQ4
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += ((double(fpd)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSampleL;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

