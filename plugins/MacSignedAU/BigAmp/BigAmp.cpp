/*
 *	File:		BigAmp.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	3/21/22
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
 BigAmp.cpp
 
 =============================================================================*/
#include "BigAmp.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, BigAmp)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BigAmp::BigAmp
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BigAmp::BigAmp(AudioUnit component)
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
//	BigAmp::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BigAmp::GetParameterValueStrings(AudioUnitScope		inScope,
														 AudioUnitParameterID	inParameterID,
														 CFArrayRef *		outStrings)
{
	
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BigAmp::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BigAmp::GetParameterInfo(AudioUnitScope		inScope,
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
//	BigAmp::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BigAmp::GetPropertyInfo (AudioUnitPropertyID	inID,
												 AudioUnitScope		inScope,
												 AudioUnitElement	inElement,
												 UInt32 &		outDataSize,
												 Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BigAmp::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			BigAmp::GetProperty(	AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 	inElement,
											void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	BigAmp::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult BigAmp::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____BigAmpEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BigAmp::BigAmpKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BigAmp::BigAmpKernel::Reset()
{
	lastSample = 0.0;
	storeSample = 0.0;
	lastSlew = 0.0;
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
	for (int fcount = 0; fcount < 257; fcount++) {Odd[fcount] = 0.0; Even[fcount] = 0.0;}
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
//	BigAmp::BigAmpKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		BigAmp::BigAmpKernel::Process(	const Float32 	*inSourceP,
										  Float32		 	*inDestP,
										  UInt32 			inFramesToProcess,
										  UInt32			inNumChannels, 
										  bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	
	double bassfill = GetParameter( kParam_One );
	double basstrim = GetParameter( kParam_Two );
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
	double inputlevel = bassfill*3.0;
	
	double samplerate = GetSampleRate();
	double EQ = (GetParameter( kParam_Two )/ samplerate)*22050.0;
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
		
		double skew = (inputSample - lastSample);
		lastSample = inputSample;
		//skew will be direction/angle
		double bridgerectifier = fabs(skew);
		if (bridgerectifier > 3.1415926) bridgerectifier = 3.1415926;
		//for skew we want it to go to zero effect again, so we use full range of the sine
		bridgerectifier = sin(bridgerectifier);
		if (skew > 0) skew = bridgerectifier;
		else skew = -bridgerectifier;
		//skew is now sined and clamped and then re-amplified again
		
		skew *= inputSample;
		skew = (skew+(skew*basstrim))/2.0;
		inputSample *= basstrim;
		double basscut = basstrim;
		//we're going to be shifting this as the stages progress
		
		outSample = (inputSample * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSample * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSample * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		double offset = (1.0 - EQ) + (fabs(inputSample)*EQ);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSampleA = (iirSampleA * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = inputSample - (iirSampleA*basscut);
		//highpass
		bridgerectifier = fabs(inputSample) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSample > 0) inputSample = (inputSample*(-0.57079633+skew))+(bridgerectifier*(1.57079633+skew));
		else inputSample = (inputSample*(-0.57079633+skew))-(bridgerectifier*(1.57079633+skew));
		//overdrive
		iirSampleB = (iirSampleB * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = inputSample - (iirSampleB*basscut);
		basscut /= 2.0;
		//highpass. Use offset from before gain stage
		//finished first gain stage
		
		outSample = (inputSample * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSample * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSample * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		skew /= 2.0;
		offset = (1.0 + offset) / 2.0;
		bridgerectifier = fabs(inputSample) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSample > 0) inputSample = (inputSample*(-0.57079633+skew))+(bridgerectifier*(1.57079633+skew));
		else inputSample = (inputSample*(-0.57079633+skew))-(bridgerectifier*(1.57079633+skew));
		//overdrive
		iirSampleC = (iirSampleC * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = inputSample - (iirSampleC*basscut);
		basscut /= 2.0;
		//highpass.
		iirSampleD = (iirSampleD * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = iirSampleD;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSample * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSample * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSample * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		skew /= 2.0;
		offset = (1.0 + offset) / 2.0;
		bridgerectifier = fabs(inputSample) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSample > 0) inputSample = (inputSample*(-0.57079633+skew))+(bridgerectifier*(1.57079633+skew));
		else inputSample = (inputSample*(-0.57079633+skew))-(bridgerectifier*(1.57079633+skew));
		//overdrive
		iirSampleE = (iirSampleE * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = inputSample - (iirSampleE*basscut);
		//we don't need to do basscut again, that was the last one
		//highpass.
		iirSampleF = (iirSampleF * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = iirSampleF;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSample * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSample * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSample * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		skew /= 2.0;
		offset = (1.0 + offset) / 2.0;
		bridgerectifier = fabs(inputSample) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSample > 0) inputSample = (inputSample*(-0.57079633+skew))+(bridgerectifier*(1.57079633+skew));
		else inputSample = (inputSample*(-0.57079633+skew))-(bridgerectifier*(1.57079633+skew));
		//output stage has less gain, no highpass, straight lowpass
		iirSampleG = (iirSampleG * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = iirSampleG;
		//lowpass. Use offset from before gain stage
		
		iirSampleH = (iirSampleH * (1 - (offset * BEQ))) + (inputSample * (offset * BEQ));
		//extra lowpass for 4*12" speakers
		if (count < 0 || count > 128) count = 128;
		double resultB = 0.0;
		
		if (flip)
		{
			Odd[count+128] = Odd[count] = iirSampleH;
			resultB = (Odd[count+down] + Odd[count+side] + Odd[count+diagonal]);
		} else {
			Even[count+128] = Even[count] = iirSampleH;
			resultB = (Even[count+down] + Even[count+side] + Even[count+diagonal]);
		}
		count--;
		
		iirSampleI = (iirSampleI * (1 - (offset * BEQ))) + (resultB * (offset * BEQ));
		inputSample += (iirSampleI*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSampleJ = (iirSampleJ * (1 - (offset * BEQ))) + (inputSample * (offset * BEQ));
		inputSample += (iirSampleJ * bassfill);
		
		inputSample = sin(inputSample*outputlevel);
		
		double randy = ((double(fpd)/UINT32_MAX)*0.04);
		inputSample = ((inputSample*(1-randy))+(storeSample*randy))*outputlevel;
		storeSample = inputSample;
		
		outSample = (inputSample * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSample * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSample * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
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
			
			b[81] = b[80]; b[80] = b[79]; 
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
			
			inputSample += (b[1] * (1.35472031405494242  + (0.00220914099195157*fabs(b[1]))));
			inputSample += (b[2] * (1.63534207755253003  - (0.11406232654509685*fabs(b[2]))));
			inputSample += (b[3] * (1.82334575691525869  - (0.42647194712964054*fabs(b[3]))));
			inputSample += (b[4] * (1.86156386235405868  - (0.76744187887586590*fabs(b[4]))));
			inputSample += (b[5] * (1.67332739338852599  - (0.95161997324293013*fabs(b[5]))));
			inputSample += (b[6] * (1.25054130794899021  - (0.98410433514572859*fabs(b[6]))));
			inputSample += (b[7] * (0.70049121047281737  - (0.87375612110718992*fabs(b[7]))));
			inputSample += (b[8] * (0.15291791448081560  - (0.61195266024519046*fabs(b[8]))));
			inputSample -= (b[9] * (0.37301992914152693  + (0.16755422915252094*fabs(b[9]))));
			inputSample -= (b[10] * (0.76568539228498433  - (0.28554435228965386*fabs(b[10]))));
			inputSample -= (b[11] * (0.95726568749937369  - (0.61659719162806048*fabs(b[11]))));
			inputSample -= (b[12] * (1.01273552193911032  - (0.81827288407943954*fabs(b[12]))));
			inputSample -= (b[13] * (0.93920108117234447  - (0.80077111864205874*fabs(b[13]))));
			inputSample -= (b[14] * (0.79831898832953974  - (0.65814750339694406*fabs(b[14]))));
			inputSample -= (b[15] * (0.64200088100452313  - (0.46135833001232618*fabs(b[15]))));
			inputSample -= (b[16] * (0.48807302802822128  - (0.15506178974799034*fabs(b[16]))));
			inputSample -= (b[17] * (0.36545171501947982  + (0.16126103769376721*fabs(b[17]))));
			inputSample -= (b[18] * (0.31469581455759105  + (0.32250870039053953*fabs(b[18]))));
			inputSample -= (b[19] * (0.36893534817945800  + (0.25409418897237473*fabs(b[19]))));
			inputSample -= (b[20] * (0.41092557722975687  + (0.13114730488878301*fabs(b[20]))));
			inputSample -= (b[21] * (0.38584044480710594  + (0.06825323739722661*fabs(b[21]))));
			inputSample -= (b[22] * (0.33378434007178670  + (0.04144255489164217*fabs(b[22]))));
			inputSample -= (b[23] * (0.26144203061699706  + (0.06031313105098152*fabs(b[23]))));
			inputSample -= (b[24] * (0.25818342000920502  + (0.03642289242586355*fabs(b[24]))));
			inputSample -= (b[25] * (0.28096018498822661  + (0.00976973667327174*fabs(b[25]))));
			inputSample -= (b[26] * (0.25845682019095384  + (0.02749015358080831*fabs(b[26]))));
			inputSample -= (b[27] * (0.26655607865953096  - (0.00329839675455690*fabs(b[27]))));
			inputSample -= (b[28] * (0.30590085026938518  - (0.07375043215328811*fabs(b[28]))));
			inputSample -= (b[29] * (0.32875683916470899  - (0.12454134857516502*fabs(b[29]))));
			inputSample -= (b[30] * (0.38166643180506560  - (0.19973911428609989*fabs(b[30]))));
			inputSample -= (b[31] * (0.49068186937289598  - (0.34785166842136384*fabs(b[31]))));
			inputSample -= (b[32] * (0.60274753867622777  - (0.48685038872711034*fabs(b[32]))));
			inputSample -= (b[33] * (0.65944678627090636  - (0.49844657885975518*fabs(b[33]))));
			inputSample -= (b[34] * (0.64488955808717285  - (0.40514406499806987*fabs(b[34]))));
			inputSample -= (b[35] * (0.55818730353434354  - (0.28029870614987346*fabs(b[35]))));
			inputSample -= (b[36] * (0.43110859113387556  - (0.15373504582939335*fabs(b[36]))));
			inputSample -= (b[37] * (0.37726894966096269  - (0.11570983506028532*fabs(b[37]))));
			inputSample -= (b[38] * (0.39953242355200935  - (0.17879231130484088*fabs(b[38]))));
			inputSample -= (b[39] * (0.36726676379100875  - (0.22013553023983223*fabs(b[39]))));
			inputSample -= (b[40] * (0.27187029469227386  - (0.18461171768478427*fabs(b[40]))));
			inputSample -= (b[41] * (0.21109334552321635  - (0.14497481318083569*fabs(b[41]))));
			inputSample -= (b[42] * (0.19808797405293213  - (0.14916579928186940*fabs(b[42]))));
			inputSample -= (b[43] * (0.16287926785495671  - (0.15146098461120627*fabs(b[43]))));
			inputSample -= (b[44] * (0.11086621477163359  - (0.13182973443924018*fabs(b[44]))));
			inputSample -= (b[45] * (0.07531043236890560  - (0.08062172796472888*fabs(b[45]))));
			inputSample -= (b[46] * (0.01747364473230771  + (0.02201865873632456*fabs(b[46]))));
			inputSample += (b[47] * (0.03080279125662693  - (0.08721756240972101*fabs(b[47]))));
			inputSample += (b[48] * (0.02354148659185142  - (0.06376361763053796*fabs(b[48]))));
			inputSample -= (b[49] * (0.02835772372098715  + (0.00589978513642627*fabs(b[49]))));
			inputSample -= (b[50] * (0.08983370744565244  - (0.02350960427706536*fabs(b[50]))));
			inputSample -= (b[51] * (0.14148947620055380  - (0.03329826628693369*fabs(b[51]))));
			inputSample -= (b[52] * (0.17576502674572581  - (0.06507546651241880*fabs(b[52]))));
			inputSample -= (b[53] * (0.17168865666573860  - (0.07734801128437317*fabs(b[53]))));
			inputSample -= (b[54] * (0.14107027738292105  - (0.03136459344220402*fabs(b[54]))));
			inputSample -= (b[55] * (0.12287163395380074  + (0.01933408169185258*fabs(b[55]))));
			inputSample -= (b[56] * (0.12276622398112971  + (0.01983508766241737*fabs(b[56]))));
			inputSample -= (b[57] * (0.12349721440213673  - (0.01111031415304768*fabs(b[57]))));
			inputSample -= (b[58] * (0.08649454142716655  + (0.02252815645513927*fabs(b[58]))));
			inputSample -= (b[59] * (0.00953083685474757  + (0.13778878548343007*fabs(b[59]))));
			inputSample += (b[60] * (0.06045983158868478  - (0.23966318224935096*fabs(b[60]))));
			inputSample += (b[61] * (0.09053229817093242  - (0.27190119941572544*fabs(b[61]))));
			inputSample += (b[62] * (0.08112662178843048  - (0.22456862606452327*fabs(b[62]))));
			inputSample += (b[63] * (0.07503525686243730  - (0.14330154410548213*fabs(b[63]))));
			inputSample += (b[64] * (0.07372595404399729  - (0.06185193766408734*fabs(b[64]))));
			inputSample += (b[65] * (0.06073789200080433  + (0.01261857435786178*fabs(b[65]))));
			inputSample += (b[66] * (0.04616712695742254  + (0.05851771967084609*fabs(b[66]))));
			inputSample += (b[67] * (0.01036235510345900  + (0.08286534414423796*fabs(b[67]))));
			inputSample -= (b[68] * (0.03708389413229191  - (0.06695282381039531*fabs(b[68]))));
			inputSample -= (b[69] * (0.07092204876981217  - (0.01915829199112784*fabs(b[69]))));
			inputSample -= (b[70] * (0.09443579589460312  + (0.01210082455316246*fabs(b[70]))));
			inputSample -= (b[71] * (0.07824038577769601  + (0.06121988546065113*fabs(b[71]))));
			inputSample -= (b[72] * (0.00854730633079399  + (0.14468518752295506*fabs(b[72]))));
			inputSample += (b[73] * (0.06845589924191028  - (0.18902431382592944*fabs(b[73]))));
			inputSample += (b[74] * (0.10351569998375465  - (0.13204443060279647*fabs(b[74]))));
			inputSample += (b[75] * (0.10513368758532179  - (0.02993199294485649*fabs(b[75]))));
			inputSample += (b[76] * (0.08896978950235003  + (0.04074499273825906*fabs(b[76]))));
			inputSample += (b[77] * (0.03697537734050980  + (0.09217751130846838*fabs(b[77]))));
			inputSample -= (b[78] * (0.04014322441280276  - (0.14062297149365666*fabs(b[78]))));
			inputSample -= (b[79] * (0.10505934581398618  - (0.16988861157275814*fabs(b[79]))));
			inputSample -= (b[80] * (0.13937661651676272  - (0.15083294570551492*fabs(b[80]))));
			inputSample -= (b[81] * (0.13183458845108439  - (0.06657454442471208*fabs(b[81]))));
			
			temp = (inputSample + smoothCabB)/3.0;
			smoothCabB = inputSample;
			inputSample = temp/4.0;
			
			
			randy = ((double(fpd)/UINT32_MAX)*0.05);
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

