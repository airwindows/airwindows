/*
*	File:		GrindAmp.cpp
*	
*	Version:	1.0
* 
*	Created:	3/28/22
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
	GrindAmp.cpp
	
=============================================================================*/
#include "GrindAmp.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(GrindAmp)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GrindAmp::GrindAmp
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GrindAmp::GrindAmp(AudioUnit component)
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
//	GrindAmp::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			GrindAmp::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GrindAmp::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			GrindAmp::GetParameterInfo(AudioUnitScope		inScope,
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
//	GrindAmp::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			GrindAmp::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GrindAmp::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			GrindAmp::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	GrindAmp::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult GrindAmp::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____GrindAmpEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GrindAmp::GrindAmpKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		GrindAmp::GrindAmpKernel::Reset()
{
	smoothA = 0.0;
	smoothB = 0.0;
	smoothC = 0.0;
	smoothD = 0.0;
	smoothE = 0.0;
	smoothF = 0.0;
	smoothG = 0.0;
	smoothH = 0.0;
	smoothI = 0.0;
	smoothJ = 0.0;
	smoothK = 0.0;
	secondA = 0.0;
	secondB = 0.0;
	secondC = 0.0;
	secondD = 0.0;
	secondE = 0.0;
	secondF = 0.0;
	secondG = 0.0;
	secondH = 0.0;
	secondI = 0.0;
	secondJ = 0.0;
	secondK = 0.0;
	thirdA = 0.0;
	thirdB = 0.0;
	thirdC = 0.0;
	thirdD = 0.0;
	thirdE = 0.0;
	thirdF = 0.0;
	thirdG = 0.0;
	thirdH = 0.0;
	thirdI = 0.0;
	thirdJ = 0.0;
	thirdK = 0.0;
	iirSampleA = 0.0;
	iirSampleB = 0.0;
	iirSampleC = 0.0;
	iirSampleD = 0.0;
	iirSampleE = 0.0;
	iirSampleF = 0.0;
	iirSampleG = 0.0;
	iirSampleH = 0.0;
	iirSampleI = 0.0;
	iirLowpass = 0.0;
	iirSub = 0.0;
	storeSample = 0.0; //amp
	
	for(int fcount = 0; fcount < 90; fcount++) {b[fcount] = 0;}
	smoothCabA = 0.0; smoothCabB = 0.0; lastCabSample = 0.0; //cab
	
	for (int fcount = 0; fcount < 9; fcount++) {lastRef[fcount] = 0.0;}
	cycle = 0; //undersampling
	
	for (int x = 0; x < fix_total; x++) {
		fixA[x] = 0.0;
		fixB[x] = 0.0;
		fixC[x] = 0.0;
		fixD[x] = 0.0;
		fixE[x] = 0.0;
		fixF[x] = 0.0;
	}	//filtering
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GrindAmp::GrindAmpKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		GrindAmp::GrindAmpKernel::Process(	const Float32 	*inSourceP,
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
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check		
	
	double inputlevel = pow(GetParameter( kParam_One ),2);
	double samplerate = GetSampleRate();
	double trimEQ = 1.1-GetParameter( kParam_Two );
	double toneEQ = trimEQ/1.2;
	trimEQ /= 50.0;
	trimEQ += 0.165;
	double EQ = ((trimEQ-(toneEQ/6.1)) / samplerate)*22050.0;
	double BEQ = ((trimEQ+(toneEQ/2.1)) / samplerate)*22050.0;
	double outputlevel = GetParameter( kParam_Three );
	double wet = GetParameter( kParam_Four );
	double bassdrive = 1.57079633*(2.5-toneEQ);
	
	double cutoff = (18000.0+(GetParameter( kParam_Two )*1000.0)) / GetSampleRate();
	if (cutoff > 0.49) cutoff = 0.49; //don't crash if run at 44.1k
	if (cutoff < 0.001) cutoff = 0.001; //or if cutoff's too low
	
	fixF[fix_freq] = fixE[fix_freq] = fixD[fix_freq] = fixC[fix_freq] = fixB[fix_freq] = fixA[fix_freq] = cutoff;
	
    fixA[fix_reso] = 4.46570214;
	fixB[fix_reso] = 1.51387132;
	fixC[fix_reso] = 0.93979296;
	fixD[fix_reso] = 0.70710678;
	fixE[fix_reso] = 0.52972649;
	fixF[fix_reso] = 0.50316379;
	
	double K = tan(M_PI * fixA[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fixA[fix_reso] + K * K);
	fixA[fix_a0] = K * K * norm;
	fixA[fix_a1] = 2.0 * fixA[fix_a0];
	fixA[fix_a2] = fixA[fix_a0];
	fixA[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixA[fix_b2] = (1.0 - K / fixA[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixB[fix_freq]);
	norm = 1.0 / (1.0 + K / fixB[fix_reso] + K * K);
	fixB[fix_a0] = K * K * norm;
	fixB[fix_a1] = 2.0 * fixB[fix_a0];
	fixB[fix_a2] = fixB[fix_a0];
	fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixB[fix_b2] = (1.0 - K / fixB[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixC[fix_freq]);
	norm = 1.0 / (1.0 + K / fixC[fix_reso] + K * K);
	fixC[fix_a0] = K * K * norm;
	fixC[fix_a1] = 2.0 * fixC[fix_a0];
	fixC[fix_a2] = fixC[fix_a0];
	fixC[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixC[fix_b2] = (1.0 - K / fixC[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixD[fix_freq]);
	norm = 1.0 / (1.0 + K / fixD[fix_reso] + K * K);
	fixD[fix_a0] = K * K * norm;
	fixD[fix_a1] = 2.0 * fixD[fix_a0];
	fixD[fix_a2] = fixD[fix_a0];
	fixD[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixD[fix_b2] = (1.0 - K / fixD[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixE[fix_freq]);
	norm = 1.0 / (1.0 + K / fixE[fix_reso] + K * K);
	fixE[fix_a0] = K * K * norm;
	fixE[fix_a1] = 2.0 * fixE[fix_a0];
	fixE[fix_a2] = fixE[fix_a0];
	fixE[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixE[fix_b2] = (1.0 - K / fixE[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixF[fix_freq]);
	norm = 1.0 / (1.0 + K / fixF[fix_reso] + K * K);
	fixF[fix_a0] = K * K * norm;
	fixF[fix_a1] = 2.0 * fixF[fix_a0];
	fixF[fix_a2] = fixF[fix_a0];
	fixF[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixF[fix_b2] = (1.0 - K / fixF[fix_reso] + K * K) * norm;
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		double drySample = inputSample;

		double outSample = (inputSample * fixA[fix_a0]) + fixA[fix_sL1];
		fixA[fix_sL1] = (inputSample * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sL2];
		fixA[fix_sL2] = (inputSample * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		inputSample *= inputlevel;
		iirSampleA = (iirSampleA * (1 - EQ)) + (inputSample *  EQ);
		inputSample = inputSample - (iirSampleA*0.92);
		//highpass
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		double bridgerectifier = fabs(inputSample);
		double inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothA + (secondA*inverse) + (thirdA*bridgerectifier) + inputSample);
		thirdA = secondA;
		secondA = smoothA;
		smoothA = inputSample;
		double basscatch = inputSample = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSample * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSample * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSample * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		iirSampleB = (iirSampleB * (1 - EQ)) + (inputSample *  EQ);
		inputSample = inputSample - (iirSampleB*0.79);
		//highpass
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		//overdrive
		bridgerectifier = fabs(inputSample);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothB + (secondB*inverse) + (thirdB*bridgerectifier) + inputSample);
		thirdB = secondB;
		secondB = smoothB;
		smoothB = inputSample;
		inputSample = bridgerectifier;
		//three-sample averaging lowpass

		iirSampleC = (iirSampleC * (1 - BEQ)) + (basscatch * BEQ);
		basscatch = iirSampleC*bassdrive;
		bridgerectifier = fabs(basscatch);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatch > 0.0) basscatch = bridgerectifier;
		else basscatch = -bridgerectifier;
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothC + (secondC*inverse) + (thirdC*bridgerectifier) + inputSample);
		thirdC = secondC;
		secondC = smoothC;
		smoothC = inputSample;
		inputSample = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSample * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSample * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSample * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleD = (iirSampleD * (1 - BEQ)) + (basscatch * BEQ);
		basscatch = iirSampleD*bassdrive;
		bridgerectifier = fabs(basscatch);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatch > 0.0) basscatch = bridgerectifier;
		else basscatch = -bridgerectifier;
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothD + (secondD*inverse) + (thirdD*bridgerectifier) + inputSample);
		thirdD = secondD;
		secondD = smoothD;
		smoothD = inputSample;
		inputSample = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSample * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSample * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSample * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		iirSampleE = (iirSampleE * (1 - BEQ)) + (basscatch * BEQ);
		basscatch = iirSampleE*bassdrive;
		bridgerectifier = fabs(basscatch);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatch > 0.0) basscatch = bridgerectifier;
		else basscatch = -bridgerectifier;
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothE + (secondE*inverse) + (thirdE*bridgerectifier) + inputSample);
		thirdE = secondE;
		secondE = smoothE;
		smoothE = inputSample;
		inputSample = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleF = (iirSampleF * (1 - BEQ)) + (basscatch * BEQ);
		basscatch = iirSampleF*bassdrive;
		bridgerectifier = fabs(basscatch);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatch > 0.0) basscatch = bridgerectifier;
		else basscatch = -bridgerectifier;
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothF + (secondF*inverse) + (thirdF*bridgerectifier) + inputSample);
		thirdF = secondF;
		secondF = smoothF;
		smoothF = inputSample;
		inputSample = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSample * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSample * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSample * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		iirSampleG = (iirSampleG * (1 - BEQ)) + (basscatch * BEQ);
		basscatch = iirSampleG*bassdrive;
		bridgerectifier = fabs(basscatch);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatch > 0.0) basscatch = bridgerectifier;
		else basscatch = -bridgerectifier;
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothG + (secondG*inverse) + (thirdG*bridgerectifier) + inputSample);
		thirdG = secondG;
		secondG = smoothG;
		smoothG = inputSample;
		inputSample = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleH = (iirSampleH * (1 - BEQ)) + (basscatch * BEQ);
		basscatch = iirSampleH*bassdrive;
		bridgerectifier = fabs(basscatch);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatch > 0.0) basscatch = bridgerectifier;
		else basscatch = -bridgerectifier;
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothH + (secondH*inverse) + (thirdH*bridgerectifier) + inputSample);
		thirdH = secondH;
		secondH = smoothH;
		smoothH = inputSample;
		inputSample = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSample * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSample * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSample * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		iirSampleI = (iirSampleI * (1 - BEQ)) + (basscatch * BEQ);
		basscatch = iirSampleI*bassdrive;
		bridgerectifier = fabs(basscatch);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatch > 0.0) basscatch = bridgerectifier;
		else basscatch = -bridgerectifier;
		if (inputSample > 1.0) inputSample = 1.0; if (inputSample < -1.0) inputSample = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothI + (secondI*inverse) + (thirdI*bridgerectifier) + inputSample);
		thirdI = secondI;
		secondI = smoothI;
		smoothI = inputSample;
		inputSample = bridgerectifier;
		//three-sample averaging lowpass
		bridgerectifier = fabs(inputSample);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothJ + (secondJ*inverse) + (thirdJ*bridgerectifier) + inputSample);
		thirdJ = secondJ;
		secondJ = smoothJ;
		smoothJ = inputSample;
		inputSample = bridgerectifier;
		//three-sample averaging lowpass
		bridgerectifier = fabs(inputSample);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothK + (secondK*inverse) + (thirdK*bridgerectifier) + inputSample);
		thirdK = secondK;
		secondK = smoothK;
		smoothK = inputSample;
		inputSample = bridgerectifier;
		//three-sample averaging lowpass
		
		basscatch /= 2.0;
		inputSample = (inputSample*toneEQ)+basscatch;
		//extra lowpass for 4*12" speakers		
		
		bridgerectifier = fabs(inputSample*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0) inputSample = bridgerectifier;
		else inputSample = -bridgerectifier;
		inputSample += basscatch;
		//split bass between overdrive and clean
		inputSample /= (1.0+toneEQ);
		
		double randy = ((double(fpd)/UINT32_MAX)*0.061);
		inputSample = ((inputSample*(1-randy))+(storeSample*randy))*outputlevel;
		storeSample = inputSample;
		
		if (wet !=1.0) {
			inputSample = (inputSample * wet) + (drySample * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		//amp
		
		cycle++;
		if (cycle == cycleEnd) {
			
			//drySample = inputSample;
			double temp = (inputSample + smoothCabA)/3.0;
			smoothCabA = inputSample;
			inputSample = temp;
			
			b[83] = b[82]; b[82] = b[81]; b[81] = b[80]; b[80] = b[79]; 
			b[79] = b[78]; b[78] = b[77]; b[77] = b[76]; b[76] = b[75]; b[75] = b[74]; b[74] = b[73]; b[73] = b[72]; b[72] = b[71]; 
			b[71] = b[70]; b[70] = b[69]; b[69] = b[68]; b[68] = b[67]; b[67] = b[66]; b[66] = b[65]; b[65] = b[64]; b[64] = b[63]; 
			b[63] = b[62]; b[62] = b[61]; b[61] = b[60]; b[60] = b[59]; b[59] = b[58]; b[58] = b[57]; b[57] = b[56]; b[56] = b[55]; 
			b[55] = b[54]; b[54] = b[53]; b[53] = b[52]; b[52] = b[51]; b[51] = b[50]; b[50] = b[49]; b[49] = b[48]; b[48] = b[47]; 
			b[47] = b[46]; b[46] = b[45]; b[45] = b[44]; b[44] = b[43]; b[43] = b[42]; b[42] = b[41]; b[41] = b[40]; b[40] = b[39]; 
			b[39] = b[38]; b[38] = b[37]; b[37] = b[36]; b[36] = b[35]; b[35] = b[34]; b[34] = b[33]; b[33] = b[32]; b[32] = b[31]; 
			b[31] = b[30]; b[30] = b[29]; b[29] = b[28]; b[28] = b[27]; b[27] = b[26]; b[26] = b[25]; b[25] = b[24]; b[24] = b[23]; 
			b[23] = b[22]; b[22] = b[21]; b[21] = b[20]; b[20] = b[19]; b[19] = b[18]; b[18] = b[17]; b[17] = b[16]; b[16] = b[15]; 
			b[15] = b[14]; b[14] = b[13]; b[13] = b[12]; b[12] = b[11]; b[11] = b[10]; b[10] = b[9]; b[9] = b[8]; b[8] = b[7]; 
			b[7] = b[6]; b[6] = b[5]; b[5] = b[4]; b[4] = b[3]; b[3] = b[2]; b[2] = b[1]; b[1] = b[0]; b[0] = inputSample;
			inputSample += (b[1] * (1.29550481610475132  + (0.19713872057074355*fabs(b[1]))));
			inputSample += (b[2] * (1.42302569895462616  + (0.30599505521284787*fabs(b[2]))));
			inputSample += (b[3] * (1.28728195804197565  + (0.23168333460446133*fabs(b[3]))));
			inputSample += (b[4] * (0.88553784290822690  + (0.14263256172918892*fabs(b[4]))));
			inputSample += (b[5] * (0.37129054918432319  + (0.00150040944205920*fabs(b[5]))));
			inputSample -= (b[6] * (0.12150959412556320  + (0.32776273620569107*fabs(b[6]))));
			inputSample -= (b[7] * (0.44900065463203775  + (0.74101214925298819*fabs(b[7]))));
			inputSample -= (b[8] * (0.54058781908186482  + (1.07821707459008387*fabs(b[8]))));
			inputSample -= (b[9] * (0.49361966401791391  + (1.23540109014850508*fabs(b[9]))));
			inputSample -= (b[10] * (0.39819495093078133  + (1.11247213730917749*fabs(b[10]))));
			inputSample -= (b[11] * (0.31379279985435521  + (0.80330360359638298*fabs(b[11]))));
			inputSample -= (b[12] * (0.30744359242808555  + (0.42132528876858205*fabs(b[12]))));
			inputSample -= (b[13] * (0.33943170284673974  + (0.09183418349389982*fabs(b[13]))));
			inputSample -= (b[14] * (0.33838775119286391  - (0.06453051658561271*fabs(b[14]))));
			inputSample -= (b[15] * (0.30682305697961665  - (0.09549380253249232*fabs(b[15]))));
			inputSample -= (b[16] * (0.23408741339295336  - (0.08083404732361277*fabs(b[16]))));
			inputSample -= (b[17] * (0.10411746814025019  + (0.00253651281245780*fabs(b[17]))));
			inputSample += (b[18] * (0.00133623776084696  - (0.04447267870865820*fabs(b[18]))));
			inputSample += (b[19] * (0.02461903992114161  + (0.07530671732655550*fabs(b[19]))));
			inputSample += (b[20] * (0.02086715842475373  + (0.22795860236804899*fabs(b[20]))));
			inputSample += (b[21] * (0.02761433637100917  + (0.26108320417844094*fabs(b[21]))));
			inputSample += (b[22] * (0.04475285369162533  + (0.19160705011061663*fabs(b[22]))));
			inputSample += (b[23] * (0.09447338372862381  + (0.03681550508743799*fabs(b[23]))));
			inputSample += (b[24] * (0.13445890343722280  - (0.13713036462146147*fabs(b[24]))));
			inputSample += (b[25] * (0.13872868945088121  - (0.22401242373298191*fabs(b[25]))));
			inputSample += (b[26] * (0.14915650097434549  - (0.26718804981526367*fabs(b[26]))));
			inputSample += (b[27] * (0.12766643217091783  - (0.27745664795660430*fabs(b[27]))));
			inputSample += (b[28] * (0.03675849788393101  - (0.18338278173550679*fabs(b[28]))));
			inputSample -= (b[29] * (0.06307306864232835  + (0.06089480869040766*fabs(b[29]))));
			inputSample -= (b[30] * (0.14947389348962944  + (0.04642103054798480*fabs(b[30]))));
			inputSample -= (b[31] * (0.25235266566401526  + (0.08423062596460507*fabs(b[31]))));
			inputSample -= (b[32] * (0.33496344048679683  + (0.09808328256677995*fabs(b[32]))));
			inputSample -= (b[33] * (0.36590030482175445  + (0.10622650888958179*fabs(b[33]))));
			inputSample -= (b[34] * (0.35015197011464372  + (0.08982043516016047*fabs(b[34]))));
			inputSample -= (b[35] * (0.26808437585665090  + (0.00735561860229533*fabs(b[35]))));
			inputSample -= (b[36] * (0.11624318543291220  - (0.07142484314510467*fabs(b[36]))));
			inputSample += (b[37] * (0.05617084165377551  + (0.11785854050350089*fabs(b[37]))));
			inputSample += (b[38] * (0.20540028692589385  + (0.20479174663329586*fabs(b[38]))));
			inputSample += (b[39] * (0.30455415003043818  + (0.29074864580096849*fabs(b[39]))));
			inputSample += (b[40] * (0.33810750937829476  + (0.29182307921316802*fabs(b[40]))));
			inputSample += (b[41] * (0.31936133365277430  + (0.26535537727394987*fabs(b[41]))));
			inputSample += (b[42] * (0.27388548321981876  + (0.19735049990538350*fabs(b[42]))));
			inputSample += (b[43] * (0.21454597517994098  + (0.06415909270247236*fabs(b[43]))));
			inputSample += (b[44] * (0.15001045817707717  - (0.03831118543404573*fabs(b[44]))));
			inputSample += (b[45] * (0.07283437284653138  - (0.09281952429543777*fabs(b[45]))));
			inputSample -= (b[46] * (0.03917872184241358  + (0.14306291461398810*fabs(b[46]))));
			inputSample -= (b[47] * (0.16695932032148642  + (0.19138995946950504*fabs(b[47]))));
			inputSample -= (b[48] * (0.27055854466909462  + (0.22531296466343192*fabs(b[48]))));
			inputSample -= (b[49] * (0.33256357307578271  + (0.23305840475692102*fabs(b[49]))));
			inputSample -= (b[50] * (0.33459770116834442  + (0.24091822618917569*fabs(b[50]))));
			inputSample -= (b[51] * (0.27156687236338090  + (0.24062938573512443*fabs(b[51]))));
			inputSample -= (b[52] * (0.17197093288412094  + (0.19083085091993421*fabs(b[52]))));
			inputSample -= (b[53] * (0.06738628195910543  + (0.10268609751019808*fabs(b[53]))));
			inputSample += (b[54] * (0.00222429218204290  + (0.01439664435720548*fabs(b[54]))));
			inputSample += (b[55] * (0.01346992803494091  + (0.15947137113534526*fabs(b[55]))));
			inputSample -= (b[56] * (0.02038911881377448  - (0.26763170752416160*fabs(b[56]))));
			inputSample -= (b[57] * (0.08233579178189687  - (0.29415931086406055*fabs(b[57]))));
			inputSample -= (b[58] * (0.15447855089824883  - (0.26489186990840807*fabs(b[58]))));
			inputSample -= (b[59] * (0.20518281113362655  - (0.16135382257522859*fabs(b[59]))));
			inputSample -= (b[60] * (0.22244686050232007  + (0.00847180390247432*fabs(b[60]))));
			inputSample -= (b[61] * (0.21849243134998034  + (0.14460595245753741*fabs(b[61]))));
			inputSample -= (b[62] * (0.20256105734574054  + (0.18932793221831667*fabs(b[62]))));
			inputSample -= (b[63] * (0.18604070054295399  + (0.17250665610927965*fabs(b[63]))));
			inputSample -= (b[64] * (0.17222844322058231  + (0.12992472027850357*fabs(b[64]))));
			inputSample -= (b[65] * (0.14447856616566443  + (0.09089219002147308*fabs(b[65]))));
			inputSample -= (b[66] * (0.10385520794251019  + (0.08600465834570559*fabs(b[66]))));
			inputSample -= (b[67] * (0.07124435678265063  + (0.09071532210549428*fabs(b[67]))));
			inputSample -= (b[68] * (0.05216857461197572  + (0.06794061706070262*fabs(b[68]))));
			inputSample -= (b[69] * (0.05235381920184123  + (0.02818101717909346*fabs(b[69]))));
			inputSample -= (b[70] * (0.07569701245553526  - (0.00634228544764946*fabs(b[70]))));
			inputSample -= (b[71] * (0.10320125382718826  - (0.02751486906644141*fabs(b[71]))));
			inputSample -= (b[72] * (0.12122120969079088  - (0.05434007312178933*fabs(b[72]))));
			inputSample -= (b[73] * (0.13438969117200902  - (0.09135218559713874*fabs(b[73]))));
			inputSample -= (b[74] * (0.13534390437529981  - (0.10437672041458675*fabs(b[74]))));
			inputSample -= (b[75] * (0.11424128854188388  - (0.08693450726462598*fabs(b[75]))));
			inputSample -= (b[76] * (0.08166894518596159  - (0.06949989431475120*fabs(b[76]))));
			inputSample -= (b[77] * (0.04293976378555305  - (0.05718625137421843*fabs(b[77]))));
			inputSample += (b[78] * (0.00933076320644409  + (0.01728285211520138*fabs(b[78]))));
			inputSample += (b[79] * (0.06450430362918153  - (0.02492994833691022*fabs(b[79]))));
			inputSample += (b[80] * (0.10187400687649277  - (0.03578455940532403*fabs(b[80]))));
			inputSample += (b[81] * (0.11039763294094571  - (0.03995523517573508*fabs(b[81]))));
			inputSample += (b[82] * (0.08557960776024547  - (0.03482514309492527*fabs(b[82]))));
			inputSample += (b[83] * (0.02730881850805332  - (0.00514750108411127*fabs(b[83]))));
			
			temp = (inputSample + smoothCabB)/3.0;
			smoothCabB = inputSample;
			inputSample = temp/4.0;
			
			
			randy = ((double(fpd)/UINT32_MAX)*0.044);
			drySample = ((((inputSample*(1-randy))+(lastCabSample*randy))*wet)+(drySample*(1.0-wet)))*outputlevel;
			lastCabSample = inputSample;
			inputSample = drySample; //cab
			
			if (cycleEnd == 4) {
				lastRef[0] = lastRef[4]; //start from previous last
				lastRef[2] = (lastRef[0] + inputSample)/2; //half
				lastRef[1] = (lastRef[0] + lastRef[2])/2; //one quarter
				lastRef[3] = (lastRef[2] + inputSample)/2; //three quarters
				lastRef[4] = inputSample; //full
			}
			if (cycleEnd == 3) {
				lastRef[0] = lastRef[3]; //start from previous last
				lastRef[2] = (lastRef[0]+lastRef[0]+inputSample)/3; //third
				lastRef[1] = (lastRef[0]+inputSample+inputSample)/3; //two thirds
				lastRef[3] = inputSample; //full
			}
			if (cycleEnd == 2) {
				lastRef[0] = lastRef[2]; //start from previous last
				lastRef[1] = (lastRef[0] + inputSample)/2; //half
				lastRef[2] = inputSample; //full
			}
			if (cycleEnd == 1) lastRef[0] = inputSample;
			cycle = 0; //reset
			inputSample = lastRef[cycle];
		} else {
			inputSample = lastRef[cycle];
			//we are going through our references now
		}
		
		switch (cycleEnd) //multi-pole average using lastRef[] variables
		{
			case 4:
				lastRef[8] = inputSample; inputSample = (inputSample+lastRef[7])*0.5;
				lastRef[7] = lastRef[8]; //continue, do not break
			case 3:
				lastRef[8] = inputSample; inputSample = (inputSample+lastRef[6])*0.5;
				lastRef[6] = lastRef[8]; //continue, do not break
			case 2:
				lastRef[8] = inputSample; inputSample = (inputSample+lastRef[5])*0.5;
				lastRef[5] = lastRef[8]; //continue, do not break
			case 1:
				break; //no further averaging
		} //undersampling
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
 
	}
}

