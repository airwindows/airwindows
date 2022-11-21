/*
*	File:		LeadAmp.cpp
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
	LeadAmp.cpp
	
=============================================================================*/
#include "LeadAmp.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(LeadAmp)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	LeadAmp::LeadAmp
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LeadAmp::LeadAmp(AudioUnit component)
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
//	LeadAmp::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			LeadAmp::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	LeadAmp::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			LeadAmp::GetParameterInfo(AudioUnitScope		inScope,
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
//	LeadAmp::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			LeadAmp::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	LeadAmp::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			LeadAmp::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	LeadAmp::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult LeadAmp::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____LeadAmpEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	LeadAmp::LeadAmpKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		LeadAmp::LeadAmpKernel::Reset()
{
	lastSample = 0.0;
	storeSample = 0.0;
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
	iirSampleA = 0.0;
	iirSampleB = 0.0;
	iirSampleC = 0.0;
	iirSampleD = 0.0;
	iirSampleE = 0.0;
	iirSampleF = 0.0;
	iirSampleG = 0.0;
	iirSampleH = 0.0;
	iirSampleI = 0.0;
	iirSampleJ = 0.0;
	iirSampleK = 0.0;
	iirLowpass = 0.0;
	iirSpkA = 0.0;
	iirSpkB = 0.0;
	iirSub = 0.0;
	register int fcount;
	for (fcount = 0; fcount < 257; fcount++) {Odd[fcount] = 0.0; Even[fcount] = 0.0;}
	count = 0;
	flip = false; //amp
	
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
//	LeadAmp::LeadAmpKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		LeadAmp::LeadAmpKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	double bassfill = GetParameter( kParam_One );
	double startlevel = bassfill;
	double samplerate = GetSampleRate();
	double basstrim = bassfill / 10.0;
	double toneEQ = (GetParameter( kParam_Two ) / samplerate)*22050.0;
	double EQ = (basstrim / samplerate)*22050.0;
	double outputlevel = GetParameter( kParam_Three );
	double wet = GetParameter( kParam_Four );
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check		
	
	double bleed = outputlevel/16.0;
	double bassfactor = 1.0-(basstrim*basstrim);
	double BEQ = (bleed / samplerate)*22050.0;
	int diagonal = (int)(0.000861678*samplerate);
	if (diagonal > 127) diagonal = 127;
	int side = (int)(diagonal/1.4142135623730951);
	int down = (side + diagonal)/2;
	//now we've got down, side and diagonal as offsets and we also use three successive samples upfront
	
	double cutoff = (15000.0+(GetParameter( kParam_Two )*10000.0)) / GetSampleRate();
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
				
		double basscut = 0.99;
		//we're going to be shifting this as the stages progress
		double inputlevel = startlevel;
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleA = (iirSampleA * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleA*basscut);
		//highpass
		double bridgerectifier = fabs(inputSample);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0) inputSample = bridgerectifier;
		else inputSample = -bridgerectifier;
		//overdrive
		bridgerectifier = (smoothA + inputSample);
		smoothA = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		
		outSample = (inputSample * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSample * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSample * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleB = (iirSampleB * (1.0 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleB*basscut);
		//highpass
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//overdrive
		bridgerectifier = (smoothB + inputSample);
		smoothB = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSample * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSample * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSample * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleC = (iirSampleC * (1.0 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleC*basscut);
		//highpass
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//overdrive
		bridgerectifier = (smoothC + inputSample);
		smoothC = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleD = (iirSampleD * (1.0 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleD*basscut);
		//highpass
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//overdrive
		bridgerectifier = (smoothD + inputSample);
		smoothD = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSample * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSample * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSample * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleE = (iirSampleE * (1.0 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleE*basscut);
		//highpass
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//overdrive
		bridgerectifier = (smoothE + inputSample);
		smoothE = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleF = (iirSampleF * (1.0 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleF*basscut);
		//highpass
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//overdrive
		bridgerectifier = (smoothF + inputSample);
		smoothF = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSample * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSample * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSample * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleG = (iirSampleG * (1.0 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleG*basscut);
		//highpass
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//overdrive
		bridgerectifier = (smoothG + inputSample);
		smoothG = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleH = (iirSampleH * (1.0 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleH*basscut);
		//highpass
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//overdrive
		bridgerectifier = (smoothH + inputSample);
		smoothH = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSample * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSample * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSample * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleI = (iirSampleI * (1.0 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleI*basscut);
		//highpass
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//overdrive
		bridgerectifier = (smoothI + inputSample);
		smoothI = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleJ = (iirSampleJ * (1.0 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleJ*basscut);
		//highpass
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//overdrive
		bridgerectifier = (smoothJ + inputSample);
		smoothJ = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleK = (iirSampleK * (1.0 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleK*basscut);
		//highpass
		if (inputSample > 1.57079633) inputSample = 1.57079633;
		if (inputSample < -1.57079633) inputSample = -1.57079633;
		inputSample = sin(inputSample);
		//overdrive
		bridgerectifier = (smoothK + inputSample);
		smoothK = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		iirLowpass = (iirLowpass * (1.0 - toneEQ)) + (inputSample * toneEQ);
		inputSample = iirLowpass;
		//lowpass. The only one of this type.
		
		iirSpkA = (iirSpkA * (1.0 -  BEQ)) + (inputSample * BEQ);
		//extra lowpass for 4*12" speakers
		if (count < 0 || count > 128) {count = 128;}
		double resultB = 0.0;
		if (flip)
		{
			Odd[count+128] = Odd[count] = iirSpkA;
			resultB = (Odd[count+down] + Odd[count+side] + Odd[count+diagonal]);
		}
		else
		{
			Even[count+128] = Even[count] = iirSpkA;
			resultB = (Even[count+down] + Even[count+side] + Even[count+diagonal]);
		}
		count--;
		iirSpkB = (iirSpkB * (1 - BEQ)) + (resultB * BEQ);
		inputSample += (iirSpkB*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSub = (iirSub * (1 - BEQ)) + (inputSample * BEQ);
		inputSample += (iirSub * bassfill);
		
		
		bridgerectifier = fabs(inputSample*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0.0) inputSample = bridgerectifier;
		else inputSample = -bridgerectifier;
		
		double randy = ((double(fpd)/UINT32_MAX)*0.084);
		inputSample = ((inputSample*(1.0-randy))+(storeSample*randy))*outputlevel;
		storeSample = inputSample;
		flip = !flip;
		
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
			
			b[85] = b[84]; b[84] = b[83]; b[83] = b[82]; b[82] = b[81]; b[81] = b[80]; b[80] = b[79]; 
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
			inputSample += (b[1] * (1.30406584776167445  - (0.01410622186823351*fabs(b[1]))));
			inputSample += (b[2] * (1.09350974154373559  + (0.34478044709202327*fabs(b[2]))));
			inputSample += (b[3] * (0.52285510059938256  + (0.84225842837363574*fabs(b[3]))));
			inputSample -= (b[4] * (0.00018126260714707  - (1.02446537989058117*fabs(b[4]))));
			inputSample -= (b[5] * (0.34943699771860115  - (0.84094709567790016*fabs(b[5]))));
			inputSample -= (b[6] * (0.53068048407937285  - (0.49231169327705593*fabs(b[6]))));
			inputSample -= (b[7] * (0.48631669406792399  - (0.08965111766223610*fabs(b[7]))));
			inputSample -= (b[8] * (0.28099201947014130  + (0.23921137841068607*fabs(b[8]))));
			inputSample -= (b[9] * (0.10333290012666248  + (0.35058962687321482*fabs(b[9]))));
			inputSample -= (b[10] * (0.06605032198166226  + (0.23447405567823365*fabs(b[10]))));
			inputSample -= (b[11] * (0.10485808661261729  + (0.05025985449763527*fabs(b[11]))));
			inputSample -= (b[12] * (0.13231190973014911  - (0.05484648240248013*fabs(b[12]))));
			inputSample -= (b[13] * (0.12926184767180304  - (0.04054223744746116*fabs(b[13]))));
			inputSample -= (b[14] * (0.13802696739839460  + (0.01876754906568237*fabs(b[14]))));
			inputSample -= (b[15] * (0.16548980700926913  + (0.06772130758771169*fabs(b[15]))));
			inputSample -= (b[16] * (0.14469310965751475  + (0.10590928840978781*fabs(b[16]))));
			inputSample -= (b[17] * (0.07838457396093310  + (0.13120101199677947*fabs(b[17]))));
			inputSample -= (b[18] * (0.05123031606187391  + (0.13883400806512292*fabs(b[18]))));
			inputSample -= (b[19] * (0.08906103481939850  + (0.07840461228402337*fabs(b[19]))));
			inputSample -= (b[20] * (0.13939265522625241  + (0.01194366471800457*fabs(b[20]))));
			inputSample -= (b[21] * (0.14957600717294034  + (0.07687598594361914*fabs(b[21]))));
			inputSample -= (b[22] * (0.14112708654047090  + (0.20118461131186977*fabs(b[22]))));
			inputSample -= (b[23] * (0.14961020766492997  + (0.30005716443826147*fabs(b[23]))));
			inputSample -= (b[24] * (0.16130382224652270  + (0.40459872030013055*fabs(b[24]))));
			inputSample -= (b[25] * (0.15679868471080052  + (0.47292767226083465*fabs(b[25]))));
			inputSample -= (b[26] * (0.16456530552807727  + (0.45182121471666481*fabs(b[26]))));
			inputSample -= (b[27] * (0.16852385701909278  + (0.38272684270752266*fabs(b[27]))));
			inputSample -= (b[28] * (0.13317562760966850  + (0.28829580273670768*fabs(b[28]))));
			inputSample -= (b[29] * (0.09396196532150952  + (0.18886898332071317*fabs(b[29]))));
			inputSample -= (b[30] * (0.10133496956734221  + (0.11158788414137354*fabs(b[30]))));
			inputSample -= (b[31] * (0.16097596389376778  + (0.02621299102374547*fabs(b[31]))));
			inputSample -= (b[32] * (0.21419006394821866  - (0.03585678078834797*fabs(b[32]))));
			inputSample -= (b[33] * (0.21273234570555244  - (0.02574469802924526*fabs(b[33]))));
			inputSample -= (b[34] * (0.16934948798707830  + (0.01354331184333835*fabs(b[34]))));
			inputSample -= (b[35] * (0.11970436472852493  + (0.04242183865883427*fabs(b[35]))));
			inputSample -= (b[36] * (0.09329023656747724  + (0.06890873292358397*fabs(b[36]))));
			inputSample -= (b[37] * (0.10255328436608116  + (0.11482972519137427*fabs(b[37]))));
			inputSample -= (b[38] * (0.13883223352796811  + (0.18016014431438840*fabs(b[38]))));
			inputSample -= (b[39] * (0.16532844286979087  + (0.24521957638633446*fabs(b[39]))));
			inputSample -= (b[40] * (0.16254607738965438  + (0.25669472097572482*fabs(b[40]))));
			inputSample -= (b[41] * (0.15353207135544752  + (0.15048064682912729*fabs(b[41]))));
			inputSample -= (b[42] * (0.13039046390746015  - (0.00200335414623601*fabs(b[42]))));
			inputSample -= (b[43] * (0.06707245032180627  - (0.06498125592578702*fabs(b[43]))));
			inputSample += (b[44] * (0.01427326441869788  + (0.01940451360783622*fabs(b[44]))));
			inputSample += (b[45] * (0.06151238306578224  - (0.07335755969763329*fabs(b[45]))));
			inputSample += (b[46] * (0.04685840498892526  - (0.14258849371688248*fabs(b[46]))));
			inputSample -= (b[47] * (0.00950136304466093  + (0.14379354707665129*fabs(b[47]))));
			inputSample -= (b[48] * (0.06245771575493557  + (0.07639718586346110*fabs(b[48]))));
			inputSample -= (b[49] * (0.07159593175777741  - (0.00595536565276915*fabs(b[49]))));
			inputSample -= (b[50] * (0.03167929390245019  - (0.03856769526301793*fabs(b[50]))));
			inputSample += (b[51] * (0.01890898565110766  + (0.00760539424271147*fabs(b[51]))));
			inputSample += (b[52] * (0.04926161137832240  - (0.06411014430053390*fabs(b[52]))));
			inputSample += (b[53] * (0.05768814623421683  - (0.15068618173358578*fabs(b[53]))));
			inputSample += (b[54] * (0.06144258297076708  - (0.21200636329120301*fabs(b[54]))));
			inputSample += (b[55] * (0.06348341960185613  - (0.19620269813094307*fabs(b[55]))));
			inputSample += (b[56] * (0.04877736350310589  - (0.11864999881200111*fabs(b[56]))));
			inputSample += (b[57] * (0.01010950997574472  - (0.02630070679113791*fabs(b[57]))));
			inputSample -= (b[58] * (0.02929178864801191  - (0.04439260202207482*fabs(b[58]))));
			inputSample -= (b[59] * (0.03484517126321562  - (0.04508635396034735*fabs(b[59]))));
			inputSample -= (b[60] * (0.00547176780437610  - (0.00205637806941426*fabs(b[60]))));
			inputSample += (b[61] * (0.02278296865283977  - (0.00063732526427685*fabs(b[61]))));
			inputSample += (b[62] * (0.02688982591366477  + (0.05333738901586284*fabs(b[62]))));
			inputSample += (b[63] * (0.01942012754957055  + (0.10942832669749143*fabs(b[63]))));
			inputSample += (b[64] * (0.01572585258756565  + (0.11189204189054594*fabs(b[64]))));
			inputSample += (b[65] * (0.01490550715016034  + (0.04449822818925343*fabs(b[65]))));
			inputSample += (b[66] * (0.01715683226376727  - (0.06944648050933899*fabs(b[66]))));
			inputSample += (b[67] * (0.02822659878011318  - (0.17843652160132820*fabs(b[67]))));
			inputSample += (b[68] * (0.03758307610456144  - (0.21986013433664692*fabs(b[68]))));
			inputSample += (b[69] * (0.03275008021608433  - (0.15869878676112170*fabs(b[69]))));
			inputSample += (b[70] * (0.01855749786752354  - (0.02337224995718105*fabs(b[70]))));
			inputSample += (b[71] * (0.00217095395782931  + (0.10971764224593601*fabs(b[71]))));
			inputSample -= (b[72] * (0.01851381451105007  - (0.17214910008793413*fabs(b[72]))));
			inputSample -= (b[73] * (0.04722574936345419  - (0.14341588977845254*fabs(b[73]))));
			inputSample -= (b[74] * (0.07151540514482006  - (0.04684695724814321*fabs(b[74]))));
			inputSample -= (b[75] * (0.06827195484995092  + (0.07022207121861397*fabs(b[75]))));
			inputSample -= (b[76] * (0.03290227240464227  + (0.16328400808152735*fabs(b[76]))));
			inputSample += (b[77] * (0.01043861198275382  - (0.20184486126076279*fabs(b[77]))));
			inputSample += (b[78] * (0.03236563559476477  - (0.17125821306380920*fabs(b[78]))));
			inputSample += (b[79] * (0.02040121529932702  - (0.09103660189829657*fabs(b[79]))));
			inputSample -= (b[80] * (0.00509649513318102  + (0.01170360991547489*fabs(b[80]))));
			inputSample -= (b[81] * (0.01388353426600228  - (0.03588955538451771*fabs(b[81]))));
			inputSample -= (b[82] * (0.00523671715033842  - (0.07068798057534148*fabs(b[82]))));
			inputSample += (b[83] * (0.00665852487721137  + (0.11666210640054926*fabs(b[83]))));
			inputSample += (b[84] * (0.01593540832939290  + (0.15844892856402149*fabs(b[84]))));
			inputSample += (b[85] * (0.02080509201836796  + (0.17186274420065850*fabs(b[85]))));
			
			temp = (inputSample + smoothCabB)/3.0;
			smoothCabB = inputSample;
			inputSample = temp/4.0;
			
			randy = ((double(fpd)/UINT32_MAX)*0.079);
			drySample = ((((inputSample*(1.0-randy))+(lastCabSample*randy))*wet)+(drySample*(1.0-wet)))*outputlevel;
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

