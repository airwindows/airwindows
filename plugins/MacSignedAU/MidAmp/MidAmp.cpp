/*
*	File:		MidAmp.cpp
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
	MidAmp.cpp
	
=============================================================================*/
#include "MidAmp.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, MidAmp)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MidAmp::MidAmp
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MidAmp::MidAmp(AudioUnit component)
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
//	MidAmp::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MidAmp::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MidAmp::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MidAmp::GetParameterInfo(AudioUnitScope		inScope,
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
//	MidAmp::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MidAmp::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MidAmp::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MidAmp::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	MidAmp::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult MidAmp::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____MidAmpEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MidAmp::MidAmpKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		MidAmp::MidAmpKernel::Reset()
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
//	MidAmp::MidAmpKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		MidAmp::MidAmpKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;

	Float64 inputlevel = GetParameter( kParam_One )*3.0;
	Float64 samplerate = GetSampleRate();
	Float64 EQ = (GetParameter( kParam_Two )/ samplerate)*22050.0;
	Float64 basstrim = GetParameter( kParam_Two );
	Float64 outputlevel = GetParameter( kParam_Three );
	double wet = GetParameter( kParam_Four );
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= samplerate;
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check		
	
	Float64 bleed = outputlevel/6.0;
	Float64 BEQ = (bleed / samplerate)*44100.0;
	Float64 resultB;
	int side = (int)(0.0006092985*samplerate);
	if (side > 126) side = 126;
	int down = side + 1;
	inputlevel = (inputlevel + (inputlevel*basstrim))/2.0;
	
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
		
		outSample = (inputSample * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSample * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSample * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		double offset = (1.0 - EQ) + (fabs(inputSample)*EQ);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSampleA = (iirSampleA * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = inputSample - (iirSampleA*0.8);
		//highpass
		bridgerectifier = fabs(inputSample) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skew;
		if (inputSample > 0) inputSample = (inputSample*(-0.57079633+skew))+(bridgerectifier*(1.57079633+skew));
		else inputSample = (inputSample*(-0.57079633+skew))-(bridgerectifier*(1.57079633+skew));
		//overdrive
		iirSampleC = (iirSampleC * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = inputSample - (iirSampleC*0.6);
		//highpass
		skew /= 1.57079633;
		//finished first gain stage
		
		outSample = (inputSample * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSample * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSample * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		offset = (1.0 + offset) / 2.0;
		bridgerectifier = fabs(inputSample) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSample > 0) inputSample = (inputSample*(-0.57079633+skew))+(bridgerectifier*(1.57079633+skew));
		else inputSample = (inputSample*(-0.57079633+skew))-(bridgerectifier*(1.57079633+skew));
		//overdrive
		iirSampleG = (iirSampleG * (1 - EQ)) + (inputSample * EQ);
		inputSample = inputSample - (iirSampleG*0.4);
		//highpass. no further basscut, no more highpasses
		iirSampleD = (iirSampleD * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = iirSampleD;
		skew /= 1.57079633;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSample * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSample * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSample * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		bridgerectifier = fabs(inputSample) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSample > 0) inputSample = (inputSample*(-0.57079633+skew))+(bridgerectifier*(1.57079633+skew));
		else inputSample = (inputSample*(-0.57079633+skew))-(bridgerectifier*(1.57079633+skew));
		//output stage has less gain, no highpass, straight lowpass
		iirSampleE = (iirSampleE * (1 - EQ)) + (inputSample * EQ);
		inputSample = iirSampleE;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSample * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSample * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSample * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		iirSampleF = (iirSampleF * (1 - (offset * BEQ))) + (inputSample * (offset * BEQ));
		//extra lowpass for 4*12" speakers
		if (count < 0 || count > 128) {count = 128;}
		if (flip)
		{
			Odd[count+128] = Odd[count] = iirSampleF;
			resultB = (Odd[count+down] + Odd[count+side]);
		}
		else
		{
			Even[count+128] = Even[count] = iirSampleF;
			resultB = (Even[count+down] + Even[count+side]);
		}
		count--;
		
		
		iirSampleB = (iirSampleB * (1 - (offset * BEQ))) + (resultB * (offset * BEQ));
		inputSample += (iirSampleB*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSampleH = (iirSampleH * (1 - (offset * BEQ))) + (inputSample * (offset * BEQ));
		inputSample += iirSampleH;
		
inputSample = sin(inputSample*outputlevel);		
		double randy = ((double(fpd)/UINT32_MAX)*0.047);
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
			
			b[87] = b[86]; b[86] = b[85]; b[85] = b[84]; b[84] = b[83]; b[83] = b[82]; b[82] = b[81]; b[81] = b[80]; b[80] = b[79]; 
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
			inputSample += (b[1] * (1.31819680801404560  + (0.00362521700518292*fabs(b[1]))));
			inputSample += (b[2] * (1.37738284126127919  + (0.14134596126256205*fabs(b[2]))));
			inputSample += (b[3] * (1.09957637225311622  + (0.33199581815501555*fabs(b[3]))));
			inputSample += (b[4] * (0.62025358899656258  + (0.37476042042088142*fabs(b[4]))));
			inputSample += (b[5] * (0.12926194402137478  + (0.24702655568406759*fabs(b[5]))));
			inputSample -= (b[6] * (0.28927985011367602  - (0.13289168298307708*fabs(b[6]))));
			inputSample -= (b[7] * (0.56518146339033448  - (0.11026641793526121*fabs(b[7]))));
			inputSample -= (b[8] * (0.59843200696815069  - (0.10139909232154271*fabs(b[8]))));
			inputSample -= (b[9] * (0.45219971861789204  - (0.13313355255903159*fabs(b[9]))));
			inputSample -= (b[10] * (0.32520490032331351  - (0.29009061730364216*fabs(b[10]))));
			inputSample -= (b[11] * (0.29773131872442909  - (0.45307495356996669*fabs(b[11]))));
			inputSample -= (b[12] * (0.31738895975218867  - (0.43198591958928922*fabs(b[12]))));
			inputSample -= (b[13] * (0.33336150604703757  - (0.24240412850274029*fabs(b[13]))));
			inputSample -= (b[14] * (0.32461638442042151  - (0.02779297492397464*fabs(b[14]))));
			inputSample -= (b[15] * (0.27812829473787770  + (0.15565718905032455*fabs(b[15]))));
			inputSample -= (b[16] * (0.19413454458668097  + (0.32087693535188599*fabs(b[16]))));
			inputSample -= (b[17] * (0.12378036344480114  + (0.37736575956794161*fabs(b[17]))));
			inputSample -= (b[18] * (0.12550494837257106  + (0.25593811142722300*fabs(b[18]))));
			inputSample -= (b[19] * (0.17725736033713696  + (0.07708896413593636*fabs(b[19]))));
			inputSample -= (b[20] * (0.22023699647700670  - (0.01600371273599124*fabs(b[20]))));
			inputSample -= (b[21] * (0.21987645486953747  + (0.00973336938352798*fabs(b[21]))));
			inputSample -= (b[22] * (0.15014276479707978  + (0.11602269600138954*fabs(b[22]))));
			inputSample -= (b[23] * (0.05176520203073560  + (0.20383164255692698*fabs(b[23]))));
			inputSample -= (b[24] * (0.04276687165294867  + (0.17785002166834518*fabs(b[24]))));
			inputSample -= (b[25] * (0.15951546388137597  + (0.06748854885822464*fabs(b[25]))));
			inputSample -= (b[26] * (0.30211952144352616  - (0.03440494237025149*fabs(b[26]))));
			inputSample -= (b[27] * (0.36462803375134506  - (0.05874284362202409*fabs(b[27]))));
			inputSample -= (b[28] * (0.32283960219377539  + (0.01189623197958362*fabs(b[28]))));
			inputSample -= (b[29] * (0.19245178663350720  + (0.11088858383712991*fabs(b[29]))));
			inputSample += (b[30] * (0.00681589563349590  - (0.16314250963457660*fabs(b[30]))));
			inputSample += (b[31] * (0.20927798345622584  - (0.16952981620487462*fabs(b[31]))));
			inputSample += (b[32] * (0.25638846543430976  - (0.11462562122281153*fabs(b[32]))));
			inputSample += (b[33] * (0.04584495673888605  + (0.04669671229804190*fabs(b[33]))));
			inputSample -= (b[34] * (0.25221561978187662  - (0.19250758741703761*fabs(b[34]))));
			inputSample -= (b[35] * (0.35662801992424953  - (0.12244680002787561*fabs(b[35]))));
			inputSample -= (b[36] * (0.21498114329314663  + (0.12152120956991189*fabs(b[36]))));
			inputSample += (b[37] * (0.00968605571673376  - (0.30597812512858558*fabs(b[37]))));
			inputSample += (b[38] * (0.18029119870614621  - (0.31569386468576782*fabs(b[38]))));
			inputSample += (b[39] * (0.22744437185251629  - (0.18028438460422197*fabs(b[39]))));
			inputSample += (b[40] * (0.09725687638959078  + (0.05479918522830433*fabs(b[40]))));
			inputSample -= (b[41] * (0.17970389267353537  - (0.29222750363124067*fabs(b[41]))));
			inputSample -= (b[42] * (0.42371969704763018  - (0.34924957781842314*fabs(b[42]))));
			inputSample -= (b[43] * (0.43313266755788055  - (0.11503731970288061*fabs(b[43]))));
			inputSample -= (b[44] * (0.22178165627851801  + (0.25002925550036226*fabs(b[44]))));
			inputSample -= (b[45] * (0.00410198176852576  + (0.43283281457037676*fabs(b[45]))));
			inputSample += (b[46] * (0.09072426344812032  - (0.35318250460706446*fabs(b[46]))));
			inputSample += (b[47] * (0.08405839183965140  - (0.16936391987143717*fabs(b[47]))));
			inputSample -= (b[48] * (0.01110419756114383  - (0.01247164991313877*fabs(b[48]))));
			inputSample -= (b[49] * (0.18593334646855278  - (0.14513260199423966*fabs(b[49]))));
			inputSample -= (b[50] * (0.33665010871497486  - (0.14456206192169668*fabs(b[50]))));
			inputSample -= (b[51] * (0.32644968491439380  + (0.01594380759082303*fabs(b[51]))));
			inputSample -= (b[52] * (0.14855437679485431  + (0.23555511219002742*fabs(b[52]))));
			inputSample += (b[53] * (0.05113019250820622  - (0.35556617126595202*fabs(b[53]))));
			inputSample += (b[54] * (0.12915754942362243  - (0.28571671825750300*fabs(b[54]))));
			inputSample += (b[55] * (0.07406865846069306  - (0.10543886479975995*fabs(b[55]))));
			inputSample -= (b[56] * (0.03669573814193980  - (0.03194267657582078*fabs(b[56]))));
			inputSample -= (b[57] * (0.13429103278009327  - (0.06145796486786051*fabs(b[57]))));
			inputSample -= (b[58] * (0.17884021749974641  - (0.00156626902982124*fabs(b[58]))));
			inputSample -= (b[59] * (0.16138212225178239  + (0.09402070836837134*fabs(b[59]))));
			inputSample -= (b[60] * (0.10867028245257521  + (0.15407963447815898*fabs(b[60]))));
			inputSample -= (b[61] * (0.06312416389213464  + (0.11241095544179526*fabs(b[61]))));
			inputSample -= (b[62] * (0.05826376574081994  - (0.03635253545701986*fabs(b[62]))));
			inputSample -= (b[63] * (0.07991631148258237  - (0.18041947557579863*fabs(b[63]))));
			inputSample -= (b[64] * (0.07777397532506500  - (0.20817156738202205*fabs(b[64]))));
			inputSample -= (b[65] * (0.03812528734394271  - (0.13679963125162486*fabs(b[65]))));
			inputSample += (b[66] * (0.00204900323943951  + (0.04009000730101046*fabs(b[66]))));
			inputSample += (b[67] * (0.01779599498119764  - (0.04218637577942354*fabs(b[67]))));
			inputSample += (b[68] * (0.00950301949319113  - (0.07908911305044238*fabs(b[68]))));
			inputSample -= (b[69] * (0.04283600714814891  + (0.02716262334097985*fabs(b[69]))));
			inputSample -= (b[70] * (0.14478320837041933  - (0.08823515277628832*fabs(b[70]))));
			inputSample -= (b[71] * (0.23250267035795688  - (0.15334197814956568*fabs(b[71]))));
			inputSample -= (b[72] * (0.22369031446225857  - (0.08550989980799503*fabs(b[72]))));
			inputSample -= (b[73] * (0.11142757883989868  + (0.08321482928259660*fabs(b[73]))));
			inputSample += (b[74] * (0.02752318631713307  - (0.25252906099212968*fabs(b[74]))));
			inputSample += (b[75] * (0.11940028414727490  - (0.34358127205009553*fabs(b[75]))));
			inputSample += (b[76] * (0.12702057126698307  - (0.31808560130583663*fabs(b[76]))));
			inputSample += (b[77] * (0.03639067777025356  - (0.17970282734717846*fabs(b[77]))));
			inputSample -= (b[78] * (0.11389848143835518  + (0.00470616711331971*fabs(b[78]))));
			inputSample -= (b[79] * (0.23024072979374310  - (0.09772245468884058*fabs(b[79]))));
			inputSample -= (b[80] * (0.24389015061112601  - (0.09600959885615798*fabs(b[80]))));
			inputSample -= (b[81] * (0.16680269075295703  - (0.05194978963662898*fabs(b[81]))));
			inputSample -= (b[82] * (0.05108041495077725  - (0.01796071525570735*fabs(b[82]))));
			inputSample += (b[83] * (0.06489835353859555  - (0.00808013770331126*fabs(b[83]))));
			inputSample += (b[84] * (0.15481511440233464  - (0.02674063848284838*fabs(b[84]))));
			inputSample += (b[85] * (0.18620867857907253  - (0.01786423699465214*fabs(b[85]))));
			inputSample += (b[86] * (0.13879832139055756  + (0.01584446839973597*fabs(b[86]))));
			inputSample += (b[87] * (0.04878235109120615  + (0.02962866516075816*fabs(b[87]))));
			
			temp = (inputSample + smoothCabB)/3.0;
			smoothCabB = inputSample;
			inputSample = temp/4.0;
			
			
			randy = ((double(fpd)/UINT32_MAX)*0.039);
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

