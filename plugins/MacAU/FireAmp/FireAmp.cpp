/*
 *	File:		FireAmp.cpp
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
 FireAmp.cpp
 
 =============================================================================*/
#include "FireAmp.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(FireAmp)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FireAmp::FireAmp
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
FireAmp::FireAmp(AudioUnit component)
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
//	FireAmp::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FireAmp::GetParameterValueStrings(AudioUnitScope		inScope,
														  AudioUnitParameterID	inParameterID,
														  CFArrayRef *		outStrings)
{
	
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FireAmp::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FireAmp::GetParameterInfo(AudioUnitScope		inScope,
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
//	FireAmp::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FireAmp::GetPropertyInfo (AudioUnitPropertyID	inID,
												  AudioUnitScope		inScope,
												  AudioUnitElement	inElement,
												  UInt32 &		outDataSize,
												  Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FireAmp::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			FireAmp::GetProperty(	AudioUnitPropertyID inID,
											 AudioUnitScope 		inScope,
											 AudioUnitElement 	inElement,
											 void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	FireAmp::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult FireAmp::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____FireAmpEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FireAmp::FireAmpKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		FireAmp::FireAmpKernel::Reset()
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
	smoothL = 0.0;
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
	iirSampleL = 0.0;
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
//	FireAmp::FireAmpKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		FireAmp::FireAmpKernel::Process(	const Float32 	*inSourceP,
											Float32		 	*inDestP,
											UInt32 			inFramesToProcess,
											UInt32			inNumChannels, 
											bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	double bassfill = GetParameter( kParam_One );
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
	
	double startlevel = bassfill;
	double samplerate = GetSampleRate();
	double basstrim = bassfill / 16.0;
	double toneEQ = (GetParameter( kParam_Two ) / samplerate)*22050.0;
	double EQ = (basstrim / samplerate)*22050.0;
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
		
		if (inputSample > 1.0) inputSample = 1.0;
		if (inputSample < -1.0) inputSample = -1.0;
		double basscut = 0.98;
		//we're going to be shifting this as the stages progress
		double inputlevel = startlevel;
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleA = (iirSampleA * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleA*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		double bridgerectifier = (smoothA + inputSample);
		smoothA = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleB = (iirSampleB * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleB*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothB + inputSample);
		smoothB = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		
		outSample = (inputSample * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSample * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSample * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleC = (iirSampleC * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleC*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothC + inputSample);
		smoothC = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleD = (iirSampleD * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleD*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothD + inputSample);
		smoothD = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSample * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSample * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSample * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleE = (iirSampleE * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleE*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothE + inputSample);
		smoothE = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleF = (iirSampleF * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleF*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothF + inputSample);
		smoothF = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSample * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSample * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSample * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleG = (iirSampleG * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleG*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothG + inputSample);
		smoothG = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleH = (iirSampleH * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleH*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothH + inputSample);
		smoothH = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSample * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSample * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSample * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleI = (iirSampleI * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleI*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothI + inputSample);
		smoothI = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleJ = (iirSampleJ * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleJ*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothJ + inputSample);
		smoothJ = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSample * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSample * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSample * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleK = (iirSampleK * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleK*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothK + inputSample);
		smoothK = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		inputSample *= inputlevel;
		inputlevel = ((inputlevel * 7.0)+1.0)/8.0;
		iirSampleL = (iirSampleL * (1 - EQ)) + (inputSample *  EQ);
		basscut *= bassfactor;
		inputSample = inputSample - (iirSampleL*basscut);
		//highpass
		inputSample -= (inputSample * (fabs(inputSample) * 0.654) * (fabs(inputSample) * 0.654) );
		//overdrive
		bridgerectifier = (smoothL + inputSample);
		smoothL = inputSample;
		inputSample = bridgerectifier;
		//two-sample averaging lowpass
		
		iirLowpass = (iirLowpass * (1 - toneEQ)) + (inputSample * toneEQ);
		inputSample = iirLowpass;
		//lowpass. The only one of this type.
		
		iirSpkA = (iirSpkA * (1 -  BEQ)) + (inputSample * BEQ);
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
		inputSample += (iirSpkB * bleed);
		//extra lowpass for 4*12" speakers
		
		bridgerectifier = fabs(inputSample*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSample > 0) inputSample = bridgerectifier;
		else inputSample = -bridgerectifier;
		
		iirSub = (iirSub * (1 - BEQ)) + (inputSample * BEQ);
		inputSample += (iirSub * bassfill * outputlevel);
		
		double randy = ((rand()/(double)RAND_MAX)*0.053);
		inputSample = ((inputSample*(1-randy))+(storeSample*randy))*outputlevel;
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
			
			b[84] = b[83]; b[83] = b[82]; b[82] = b[81]; b[81] = b[80]; b[80] = b[79]; 
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
			inputSample += (b[1] * (1.31698250313308396  - (0.08140616497621633*fabs(b[1]))));
			inputSample += (b[2] * (1.47229016949915326  - (0.27680278993637253*fabs(b[2]))));
			inputSample += (b[3] * (1.30410109086044956  - (0.35629113432046489*fabs(b[3]))));
			inputSample += (b[4] * (0.81766210474551260  - (0.26808782337659753*fabs(b[4]))));
			inputSample += (b[5] * (0.19868872545506663  - (0.11105517193919669*fabs(b[5]))));
			inputSample -= (b[6] * (0.39115909132567039  - (0.12630622002682679*fabs(b[6]))));
			inputSample -= (b[7] * (0.76881891559343574  - (0.40879849500403143*fabs(b[7]))));
			inputSample -= (b[8] * (0.87146861782680340  - (0.59529560488000599*fabs(b[8]))));
			inputSample -= (b[9] * (0.79504575932563670  - (0.60877047551611796*fabs(b[9]))));
			inputSample -= (b[10] * (0.61653017622406314  - (0.47662851438557335*fabs(b[10]))));
			inputSample -= (b[11] * (0.40718195794382067  - (0.24955839378539713*fabs(b[11]))));
			inputSample -= (b[12] * (0.31794900040616203  - (0.04169792259600613*fabs(b[12]))));
			inputSample -= (b[13] * (0.41075032540217843  + (0.00368483996076280*fabs(b[13]))));
			inputSample -= (b[14] * (0.56901352922170667  - (0.11027360805893105*fabs(b[14]))));
			inputSample -= (b[15] * (0.62443222391889264  - (0.22198075154245228*fabs(b[15]))));
			inputSample -= (b[16] * (0.53462856723129204  - (0.22933544545324852*fabs(b[16]))));
			inputSample -= (b[17] * (0.34441703361995046  - (0.12956809502269492*fabs(b[17]))));
			inputSample -= (b[18] * (0.13947052337867882  + (0.00339775055962799*fabs(b[18]))));
			inputSample += (b[19] * (0.03771252648928484  - (0.10863931549251718*fabs(b[19]))));
			inputSample += (b[20] * (0.18280210770271693  - (0.17413646599296417*fabs(b[20]))));
			inputSample += (b[21] * (0.24621986701761467  - (0.14547053270435095*fabs(b[21]))));
			inputSample += (b[22] * (0.22347075142737360  - (0.02493869490104031*fabs(b[22]))));
			inputSample += (b[23] * (0.14346348482123716  + (0.11284054747963246*fabs(b[23]))));
			inputSample += (b[24] * (0.00834364862916028  + (0.24284684053733926*fabs(b[24]))));
			inputSample -= (b[25] * (0.11559740296078347  - (0.32623054435304538*fabs(b[25]))));
			inputSample -= (b[26] * (0.18067604561283060  - (0.32311481551122478*fabs(b[26]))));
			inputSample -= (b[27] * (0.22927997789035612  - (0.26991539052832925*fabs(b[27]))));
			inputSample -= (b[28] * (0.28487666578669446  - (0.22437227250279349*fabs(b[28]))));
			inputSample -= (b[29] * (0.31992973037153838  - (0.15289876100963865*fabs(b[29]))));
			inputSample -= (b[30] * (0.35174606303520733  - (0.05656293023086628*fabs(b[30]))));
			inputSample -= (b[31] * (0.36894898011375254  + (0.04333925421463558*fabs(b[31]))));
			inputSample -= (b[32] * (0.32567576055307507  + (0.14594589410921388*fabs(b[32]))));
			inputSample -= (b[33] * (0.27440135050585784  + (0.15529667398122521*fabs(b[33]))));
			inputSample -= (b[34] * (0.21998973785078091  + (0.05083553737157104*fabs(b[34]))));
			inputSample -= (b[35] * (0.10323624876862457  - (0.04651829594199963*fabs(b[35]))));
			inputSample += (b[36] * (0.02091603687851074  + (0.12000046818439322*fabs(b[36]))));
			inputSample += (b[37] * (0.11344930914138468  + (0.17697142512225839*fabs(b[37]))));
			inputSample += (b[38] * (0.22766779627643968  + (0.13645102964003858*fabs(b[38]))));
			inputSample += (b[39] * (0.38378309953638229  - (0.01997653307333791*fabs(b[39]))));
			inputSample += (b[40] * (0.52789400804568076  - (0.21409137428422448*fabs(b[40]))));
			inputSample += (b[41] * (0.55444630296938280  - (0.32331980931576626*fabs(b[41]))));
			inputSample += (b[42] * (0.42333237669264601  - (0.26855847463044280*fabs(b[42]))));
			inputSample += (b[43] * (0.21942831522035078  - (0.12051365248820624*fabs(b[43]))));
			inputSample -= (b[44] * (0.00584169427830633  - (0.03706970171280329*fabs(b[44]))));
			inputSample -= (b[45] * (0.24279799124660351  - (0.17296440491477982*fabs(b[45]))));
			inputSample -= (b[46] * (0.40173760787507085  - (0.21717989835163351*fabs(b[46]))));
			inputSample -= (b[47] * (0.43930035724188155  - (0.16425928481378199*fabs(b[47]))));
			inputSample -= (b[48] * (0.41067765934041811  - (0.10390115786636855*fabs(b[48]))));
			inputSample -= (b[49] * (0.34409235547165967  - (0.07268159377411920*fabs(b[49]))));
			inputSample -= (b[50] * (0.26542883122568151  - (0.05483457497365785*fabs(b[50]))));
			inputSample -= (b[51] * (0.22024754776138800  - (0.06484897950087598*fabs(b[51]))));
			inputSample -= (b[52] * (0.20394367993632415  - (0.08746309731952180*fabs(b[52]))));
			inputSample -= (b[53] * (0.17565242431124092  - (0.07611309538078760*fabs(b[53]))));
			inputSample -= (b[54] * (0.10116623231246825  - (0.00642818706295112*fabs(b[54]))));
			inputSample -= (b[55] * (0.00782648272053632  + (0.08004141267685004*fabs(b[55]))));
			inputSample += (b[56] * (0.05059046006747323  - (0.12436676387548490*fabs(b[56]))));
			inputSample += (b[57] * (0.06241531553254467  - (0.11530779547021434*fabs(b[57]))));
			inputSample += (b[58] * (0.04952694587101836  - (0.08340945324333944*fabs(b[58]))));
			inputSample += (b[59] * (0.00843873294401687  - (0.03279659052562903*fabs(b[59]))));
			inputSample -= (b[60] * (0.05161338949440241  - (0.03428181149163798*fabs(b[60]))));
			inputSample -= (b[61] * (0.08165520146902012  - (0.08196746092283110*fabs(b[61]))));
			inputSample -= (b[62] * (0.06639532849935320  - (0.09797462781896329*fabs(b[62]))));
			inputSample -= (b[63] * (0.02953430910661621  - (0.09175612938515763*fabs(b[63]))));
			inputSample += (b[64] * (0.00741058547442938  + (0.05442091048731967*fabs(b[64]))));
			inputSample += (b[65] * (0.01832866125391727  + (0.00306243693643687*fabs(b[65]))));
			inputSample += (b[66] * (0.00526964230373573  - (0.04364102661136410*fabs(b[66]))));
			inputSample -= (b[67] * (0.00300984373848200  + (0.09742737841278880*fabs(b[67]))));
			inputSample -= (b[68] * (0.00413616769576694  + (0.14380661694523073*fabs(b[68]))));
			inputSample -= (b[69] * (0.00588769034931419  + (0.16012843578892538*fabs(b[69]))));
			inputSample -= (b[70] * (0.00688588239450581  + (0.14074464279305798*fabs(b[70]))));
			inputSample -= (b[71] * (0.02277307992926315  + (0.07914752191801366*fabs(b[71]))));
			inputSample -= (b[72] * (0.04627166091180877  - (0.00192787268067208*fabs(b[72]))));
			inputSample -= (b[73] * (0.05562045897455786  - (0.05932868727665747*fabs(b[73]))));
			inputSample -= (b[74] * (0.05134243784922165  - (0.08245334798868090*fabs(b[74]))));
			inputSample -= (b[75] * (0.04719409472239919  - (0.07498680629253825*fabs(b[75]))));
			inputSample -= (b[76] * (0.05889738914266415  - (0.06116127018043697*fabs(b[76]))));
			inputSample -= (b[77] * (0.09428363535111127  - (0.06535868867863834*fabs(b[77]))));
			inputSample -= (b[78] * (0.15181756953225126  - (0.08982979655234427*fabs(b[78]))));
			inputSample -= (b[79] * (0.20878969456036670  - (0.10761070891499538*fabs(b[79]))));
			inputSample -= (b[80] * (0.22647885581813790  - (0.08462542510349125*fabs(b[80]))));
			inputSample -= (b[81] * (0.19723482443646323  - (0.02665160920736287*fabs(b[81]))));
			inputSample -= (b[82] * (0.16441643451155163  + (0.02314691954338197*fabs(b[82]))));
			inputSample -= (b[83] * (0.15201914054931515  + (0.04424903493886839*fabs(b[83]))));
			inputSample -= (b[84] * (0.15454370641307855  + (0.04223203797913008*fabs(b[84]))));
			
			temp = (inputSample + smoothCabB)/3.0;
			smoothCabB = inputSample;
			inputSample = temp/4.0;
			
			
			randy = ((double(fpd)/UINT32_MAX)*0.057);
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

