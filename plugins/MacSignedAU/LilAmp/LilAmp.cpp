/*
*	File:		LilAmp.cpp
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
	LilAmp.cpp
	
=============================================================================*/
#include "LilAmp.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, LilAmp)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	LilAmp::LilAmp
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
LilAmp::LilAmp(AudioUnit component)
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
//	LilAmp::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			LilAmp::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	LilAmp::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			LilAmp::GetParameterInfo(AudioUnitScope		inScope,
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
//	LilAmp::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			LilAmp::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	LilAmp::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			LilAmp::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	LilAmp::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult LilAmp::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____LilAmpEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	LilAmp::LilAmpKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		LilAmp::LilAmpKernel::Reset()
{
	lastSample = 0.0;
	storeSample = 0.0;
	lastSlew = 0.0;
	iirSampleA = 0.0;
	iirSampleB = 0.0;
	iirSampleC = 0.0;
	iirSampleD = 0.0;
	iirSampleE = 0.0;
	OddA = 0.0;
	OddB = 0.0;
	OddC = 0.0;
	OddD = 0.0;
	OddE = 0.0;
	EvenA = 0.0;
	EvenB = 0.0;
	EvenC = 0.0;
	EvenD = 0.0;
	EvenE = 0.0;
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
//	LilAmp::LilAmpKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		LilAmp::LilAmpKernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;

	double inputlevel = GetParameter( kParam_One )*6.0;
	double EQ = (GetParameter( kParam_Two )/ GetSampleRate())*22050;
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
	
	double skewlevel = pow(basstrim,2) * outputlevel;
	
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
		skew *= skewlevel;
		inputSample *= basstrim;
		inputSample *= inputlevel;
		double offset = (1.0 - EQ) + (fabs(inputSample)*EQ);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		iirSampleA = (iirSampleA * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = inputSample - iirSampleA;
		//highpass
		bridgerectifier = fabs(inputSample) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skew;
		if (inputSample > 0) inputSample = (inputSample*(-0.57079633+skew))+(bridgerectifier*(1.57079633+skew));
		else inputSample = (inputSample*(-0.57079633+skew))-(bridgerectifier*(1.57079633+skew));
		//overdrive
		iirSampleC = (iirSampleC * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = iirSampleC;
		//lowpass. Use offset from before gain stage
		//finished first gain stage
		
		outSample = (inputSample * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSample * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSample * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		inputSample *= inputlevel;
		offset = (1.0 + offset) / 2.0;
		iirSampleB = (iirSampleB * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = inputSample - iirSampleB;
		//highpass
		bridgerectifier = fabs(inputSample) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skew;
		if (inputSample > 0) inputSample = (inputSample*(-0.57079633+skew))+(bridgerectifier*(1.57079633+skew));
		else inputSample = (inputSample*(-0.57079633+skew))-(bridgerectifier*(1.57079633+skew));
		//overdrive
		iirSampleD = (iirSampleD * (1 - (offset * EQ))) + (inputSample * (offset * EQ));
		inputSample = iirSampleD;
		//lowpass. Use offset from before gain stage
		
		if (flip)
		{
			OddD = OddC; OddC = OddB; OddB = OddA; OddA = inputSample;
			inputSample = (OddA + OddB + OddC + OddD) / 4.0;
		}
		else
		{
			EvenD = EvenC; EvenC = EvenB; EvenB = EvenA; EvenA = inputSample;
			inputSample = (EvenA + EvenB + EvenC + EvenD) / 4.0;
		}
		
		outSample = (inputSample * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSample * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSample * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		inputSample *= inputlevel;
		bridgerectifier = fabs(inputSample) + skew;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSample > 0) inputSample = (inputSample*-0.57079633)+(bridgerectifier*1.57079633);
		else inputSample = (inputSample*-0.57079633)-(bridgerectifier*1.57079633);
		//output stage has less gain, no highpass, straight lowpass
		iirSampleE = (iirSampleE * (1 - EQ)) + (inputSample * EQ);
		inputSample = iirSampleE;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSample * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSample * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSample * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
				
		inputSample = sin(inputSample*outputlevel);
		
		outSample = (inputSample * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSample * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSample * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSample = outSample; //fixed biquad filtering ultrasonics
		
		double randy = ((double(fpd)/UINT32_MAX)*0.034);
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
			
			b[82] = b[81]; b[81] = b[80]; b[80] = b[79]; 
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
			inputSample += (b[1] * (1.42133070619855229  - (0.18270903813104500*fabs(b[1]))));
			inputSample += (b[2] * (1.47209686171873821  - (0.27954009590498585*fabs(b[2]))));
			inputSample += (b[3] * (1.34648011331265294  - (0.47178343556301960*fabs(b[3]))));
			inputSample += (b[4] * (0.82133804036124580  - (0.41060189990353935*fabs(b[4]))));
			inputSample += (b[5] * (0.21628057120466901  - (0.26062442734317454*fabs(b[5]))));
			inputSample -= (b[6] * (0.30306716082877883  + (0.10067648425439185*fabs(b[6]))));
			inputSample -= (b[7] * (0.69484313178531876  - (0.09655574841702286*fabs(b[7]))));
			inputSample -= (b[8] * (0.88320822356980833  - (0.26501644327144314*fabs(b[8]))));
			inputSample -= (b[9] * (0.81326147029423723  - (0.31115926837054075*fabs(b[9]))));
			inputSample -= (b[10] * (0.56728759049069222  - (0.23304233545561287*fabs(b[10]))));
			inputSample -= (b[11] * (0.33340326645198737  - (0.12361361388240180*fabs(b[11]))));
			inputSample -= (b[12] * (0.20280263733605616  - (0.03531960962500105*fabs(b[12]))));
			inputSample -= (b[13] * (0.15864533788751345  + (0.00355160825317868*fabs(b[13]))));
			inputSample -= (b[14] * (0.12544767480555119  + (0.01979010423176500*fabs(b[14]))));
			inputSample -= (b[15] * (0.06666788902658917  + (0.00188830739903378*fabs(b[15]))));
			inputSample += (b[16] * (0.02977793355081072  + (0.02304216615605394*fabs(b[16]))));
			inputSample += (b[17] * (0.12821526330916558  + (0.02636238376777800*fabs(b[17]))));
			inputSample += (b[18] * (0.19933812710210136  - (0.02932657234709721*fabs(b[18]))));
			inputSample += (b[19] * (0.18346460191225772  - (0.12859581955080629*fabs(b[19]))));
			inputSample -= (b[20] * (0.00088697526755385  + (0.15855257539277415*fabs(b[20]))));
			inputSample -= (b[21] * (0.28904286712096761  + (0.06226286786982616*fabs(b[21]))));
			inputSample -= (b[22] * (0.49133546282552537  - (0.06512851581813534*fabs(b[22]))));
			inputSample -= (b[23] * (0.52908013030763046  - (0.13606992188523465*fabs(b[23]))));
			inputSample -= (b[24] * (0.45897241332311706  - (0.15527194946346906*fabs(b[24]))));
			inputSample -= (b[25] * (0.35535938629924352  - (0.13634771941703441*fabs(b[25]))));
			inputSample -= (b[26] * (0.26185269405237693  - (0.08736651482771546*fabs(b[26]))));
			inputSample -= (b[27] * (0.19997351944186473  - (0.01714565029656306*fabs(b[27]))));
			inputSample -= (b[28] * (0.18894054145105646  + (0.04557612705740050*fabs(b[28]))));
			inputSample -= (b[29] * (0.24043993691153928  + (0.05267500387081067*fabs(b[29]))));
			inputSample -= (b[30] * (0.29191852873822671  + (0.01922151122971644*fabs(b[30]))));
			inputSample -= (b[31] * (0.29399783430587761  - (0.02238952856106585*fabs(b[31]))));
			inputSample -= (b[32] * (0.26662219155294159  - (0.07760819463416335*fabs(b[32]))));
			inputSample -= (b[33] * (0.20881206667122221  - (0.11930017354479640*fabs(b[33]))));
			inputSample -= (b[34] * (0.12916658879944876  - (0.11798638949823513*fabs(b[34]))));
			inputSample -= (b[35] * (0.07678815166012012  - (0.06826864734598684*fabs(b[35]))));
			inputSample -= (b[36] * (0.08568505484529348  - (0.00510459741104792*fabs(b[36]))));
			inputSample -= (b[37] * (0.13613615872486634  + (0.02288223583971244*fabs(b[37]))));
			inputSample -= (b[38] * (0.17426657494209266  + (0.02723737220296440*fabs(b[38]))));
			inputSample -= (b[39] * (0.17343619261009030  + (0.01412920547179825*fabs(b[39]))));
			inputSample -= (b[40] * (0.14548368977428555  - (0.02640418940455951*fabs(b[40]))));
			inputSample -= (b[41] * (0.10485295885802372  - (0.06334665781931498*fabs(b[41]))));
			inputSample -= (b[42] * (0.06632268974717079  - (0.05960343688612868*fabs(b[42]))));
			inputSample -= (b[43] * (0.06915692039882040  - (0.03541337869596061*fabs(b[43]))));
			inputSample -= (b[44] * (0.11889611687783583  - (0.02250608307287119*fabs(b[44]))));
			inputSample -= (b[45] * (0.14598456370320673  + (0.00280345943128246*fabs(b[45]))));
			inputSample -= (b[46] * (0.12312084125613143  + (0.04947283933434576*fabs(b[46]))));
			inputSample -= (b[47] * (0.11379940289994711  + (0.06590080966570636*fabs(b[47]))));
			inputSample -= (b[48] * (0.12963290754003182  + (0.02597647654256477*fabs(b[48]))));
			inputSample -= (b[49] * (0.12723837402978638  - (0.04942071966927938*fabs(b[49]))));
			inputSample -= (b[50] * (0.09185015882996231  - (0.10420810015956679*fabs(b[50]))));
			inputSample -= (b[51] * (0.04011592913036545  - (0.10234174227772008*fabs(b[51]))));
			inputSample += (b[52] * (0.00992597785057113  + (0.05674042373836896*fabs(b[52]))));
			inputSample += (b[53] * (0.04921452178306781  - (0.00222698867111080*fabs(b[53]))));
			inputSample += (b[54] * (0.06096504883783566  - (0.04040426549982253*fabs(b[54]))));
			inputSample += (b[55] * (0.04113530718724200  - (0.04190143593049960*fabs(b[55]))));
			inputSample += (b[56] * (0.01292699017654650  - (0.01121994018532499*fabs(b[56]))));
			inputSample -= (b[57] * (0.00437123132431870  - (0.02482497612289103*fabs(b[57]))));
			inputSample -= (b[58] * (0.02090571264211918  - (0.03732746039260295*fabs(b[58]))));
			inputSample -= (b[59] * (0.04749751678612051  - (0.02960060937328099*fabs(b[59]))));
			inputSample -= (b[60] * (0.07675095194206227  - (0.02241927084099648*fabs(b[60]))));
			inputSample -= (b[61] * (0.08879414028581609  - (0.01144281133042115*fabs(b[61]))));
			inputSample -= (b[62] * (0.07378854974999530  + (0.02518742701599147*fabs(b[62]))));
			inputSample -= (b[63] * (0.04677309194488959  + (0.08984657372223502*fabs(b[63]))));
			inputSample -= (b[64] * (0.02911874044176449  + (0.14202665940555093*fabs(b[64]))));
			inputSample -= (b[65] * (0.02103564720234969  + (0.14640411976171003*fabs(b[65]))));
			inputSample -= (b[66] * (0.01940626429101940  + (0.10867274382865903*fabs(b[66]))));
			inputSample -= (b[67] * (0.03965401793931531  + (0.04775225375522835*fabs(b[67]))));
			inputSample -= (b[68] * (0.08102486457314527  - (0.03204447425666343*fabs(b[68]))));
			inputSample -= (b[69] * (0.11794849372825778  - (0.12755667382696789*fabs(b[69]))));
			inputSample -= (b[70] * (0.11946469076758266  - (0.20151394599125422*fabs(b[70]))));
			inputSample -= (b[71] * (0.07404630324668053  - (0.21300634351769704*fabs(b[71]))));
			inputSample -= (b[72] * (0.00477584437144086  - (0.16864707684978708*fabs(b[72]))));
			inputSample += (b[73] * (0.05924822014377220  + (0.09394651445109450*fabs(b[73]))));
			inputSample += (b[74] * (0.10060989907457370  + (0.00419196431884887*fabs(b[74]))));
			inputSample += (b[75] * (0.10817907203844988  - (0.07459664480796091*fabs(b[75]))));
			inputSample += (b[76] * (0.08701102204768002  - (0.11129477437630560*fabs(b[76]))));
			inputSample += (b[77] * (0.05673785623180162  - (0.10638640242375266*fabs(b[77]))));
			inputSample += (b[78] * (0.02944190197442081  - (0.08499792583420167*fabs(b[78]))));
			inputSample += (b[79] * (0.01570145445652971  - (0.06190456843465320*fabs(b[79]))));
			inputSample += (b[80] * (0.02770233032476748  - (0.04573713136865480*fabs(b[80]))));
			inputSample += (b[81] * (0.05417160459175360  - (0.03965651064634598*fabs(b[81]))));
			inputSample += (b[82] * (0.06080831637644498  - (0.02909500789113911*fabs(b[82]))));
			
			temp = (inputSample + smoothCabB)/3.0;
			smoothCabB = inputSample;
			inputSample = temp/4.0;
			
			
			randy = ((double(fpd)/UINT32_MAX)*0.085);
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

