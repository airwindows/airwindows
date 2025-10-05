/*
*	File:		MasteringMono.cpp
*	
*	Version:	1.0
* 
*	Created:	9/25/25
*	
*	Copyright:  Copyright © 2025 Airwindows, Airwindows uses the MIT license
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
	MasteringMono.cpp
	
=============================================================================*/
#include "MasteringMono.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, MasteringMono)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MasteringMono::MasteringMono
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MasteringMono::MasteringMono(AudioUnit component)
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
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MasteringMono::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MasteringMono::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_F)) //ID must be actual name of parameter identifier, not number
	{
		if (outStrings == NULL) return noErr;
		CFStringRef strings [] =
		{
			kMenuItem_Dark,
			kMenuItem_TenNines,
			kMenuItem_TPDFWide,
			kMenuItem_PaulWide,
			kMenuItem_NJAD,
			kMenuItem_Bypass,
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
//	MasteringMono::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MasteringMono::GetParameterInfo(AudioUnitScope		inScope,
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
				outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kDark;
                outParameterInfo.maxValue = kBypass;
                outParameterInfo.defaultValue = kDefaultValue_ParamF;
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
//	MasteringMono::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MasteringMono::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MasteringMono::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			MasteringMono::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	MasteringMono::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult MasteringMono::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____MasteringMonoEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MasteringMono::MasteringMonoKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		MasteringMono::MasteringMonoKernel::Reset()
{
	for (int x = 0; x < air_total; x++) air[x] = 0.0;
	for (int x = 0; x < kal_total; x++) {kalM[x] = 0.0;kalS[x] = 0.0;}
	
	lastSinewL = 0.0;
	
	lastSampleL = 0.0;
	wasPosClipL = false;
	wasNegClipL = false;
	for (int x = 0; x < 16; x++) {intermediateL[x] = 0.0;}
	
	quantA = 0;
	quantB = 1;
	expectedSlew = 0.0;
	testA = 0.0;
	testB = 0.0;
	correction = 0.0;
	shapedSampleL = 0.0;
	currentDither = 0.0;
	ditherL = 0.0;
	cutbinsL = false;
	hotbinA = 0;
	hotbinB = 0;
	benfordize = 0.0;
	totalA = 0.0;
	totalB = 0.0;
	outputSample = 0.0;
	expon = 0; //internal dither variables
	//these didn't like to be defined inside a case statement
	
	NSOddL = 0.0; NSEvenL = 0.0; prevShapeL = 0.0;
	flip = true; //Ten Nines
	for(int count = 0; count < 99; count++) {
		darkSampleL[count] = 0;
	} //Dark
	previousDitherL = 0.0; //PaulWide
	bynL[0] = 1000.0;
	bynL[1] = 301.0;
	bynL[2] = 176.0;
	bynL[3] = 125.0;
	bynL[4] = 97.0;
	bynL[5] = 79.0;
	bynL[6] = 67.0;
	bynL[7] = 58.0;
	bynL[8] = 51.0;
	bynL[9] = 46.0;
	bynL[10] = 1000.0;
	noiseShapingL = 0.0; //NJAD
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	MasteringMono::MasteringMonoKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		MasteringMono::MasteringMonoKernel::Process(	const Float32 	*inSourceP,
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
	
	double threshSinew = (0.25+((1.0-GetParameter( kParam_A ))*0.333))/overallscale;
	double depthSinew = 1.0-pow(1.0-GetParameter( kParam_A ),2.0);
	
	double trebleZoom = GetParameter( kParam_B )-0.5;
	long double trebleGain = (trebleZoom*fabs(trebleZoom))+1.0;
	if (trebleGain > 1.0) trebleGain = pow(trebleGain,3.0+sqrt(overallscale));
	//this boost is necessary to adapt to higher sample rates
	
	double midZoom = GetParameter( kParam_C )-0.5;
	long double midGain = (midZoom*fabs(midZoom))+1.0;
	double kalMid = 0.35-(GetParameter( kParam_C )*0.25); //crossover frequency between mid/bass
	double kalSub = 0.45+(GetParameter( kParam_C )*0.25); //crossover frequency between bass/sub
	
	double bassZoom = (GetParameter( kParam_D )*0.5)-0.25;
	long double bassGain = (-bassZoom*fabs(bassZoom))+1.0; //control inverted
	long double subGain = ((GetParameter( kParam_D )*0.25)-0.125)+1.0;
	if (subGain < 1.0) subGain = 1.0; //very small sub shift, only pos.
	
	long double driveIn = (GetParameter( kParam_E )-0.5)+1.0;
	long double driveOut = (-(GetParameter( kParam_E )-0.5)*fabs(GetParameter( kParam_E )-0.5))+1.0;
	
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	int dither = (int) GetParameter( kParam_F );
	int depth = (int)(17.0*overallscale);
	if (depth < 3) depth = 3; if (depth > 98) depth = 98; //for Dark
	
	while (nSampleFrames-- > 0) {
		long double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		inputSampleL *= driveIn;
		long double drySampleL = inputSampleL;
		
		//begin Air3L
		air[pvSL4] = air[pvAL4] - air[pvAL3]; air[pvSL3] = air[pvAL3] - air[pvAL2];
		air[pvSL2] = air[pvAL2] - air[pvAL1]; air[pvSL1] = air[pvAL1] - inputSampleL;
		air[accSL3] = air[pvSL4] - air[pvSL3]; air[accSL2] = air[pvSL3] - air[pvSL2];
		air[accSL1] = air[pvSL2] - air[pvSL1];
		air[acc2SL2] = air[accSL3] - air[accSL2]; air[acc2SL1] = air[accSL2] - air[accSL1];		
		air[outAL] = -(air[pvAL1] + air[pvSL3] + air[acc2SL2] - ((air[acc2SL2] + air[acc2SL1])*0.5));
		air[gainAL] *= 0.5; air[gainAL] += fabs(drySampleL-air[outAL])*0.5;
		if (air[gainAL] > 0.3*sqrt(overallscale)) air[gainAL] = 0.3*sqrt(overallscale);
		air[pvAL4] = air[pvAL3]; air[pvAL3] = air[pvAL2];
		air[pvAL2] = air[pvAL1]; air[pvAL1] = (air[gainAL] * air[outAL]) + drySampleL;
		long double midL = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
		long double temp = (midL + air[gndavgL])*0.5; air[gndavgL] = midL; midL = temp;
		long double trebleL = drySampleL-midL;
		//end Air3L
		
		//begin KalmanML
		temp = midL;
		kalM[prevSlewL3] += kalM[prevSampL3] - kalM[prevSampL2]; kalM[prevSlewL3] *= 0.5;
		kalM[prevSlewL2] += kalM[prevSampL2] - kalM[prevSampL1]; kalM[prevSlewL2] *= 0.5;
		kalM[prevSlewL1] += kalM[prevSampL1] - midL; kalM[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kalM[accSlewL2] += kalM[prevSlewL3] - kalM[prevSlewL2]; kalM[accSlewL2] *= 0.5;
		kalM[accSlewL1] += kalM[prevSlewL2] - kalM[prevSlewL1]; kalM[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalM[accSlewL3] += (kalM[accSlewL2] - kalM[accSlewL1]); kalM[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kalM[kalOutL] += kalM[prevSampL1] + kalM[prevSlewL2] + kalM[accSlewL3]; kalM[kalOutL] *= 0.5;
		
		//resynthesizing predicted result (all iir smoothed)
		kalM[kalGainL] += fabs(temp-kalM[kalOutL])*kalMid*8.0; kalM[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalM[kalGainL] > kalMid*0.5) kalM[kalGainL] = kalMid*0.5;
		//attempts to avoid explosions
		kalM[kalOutL] += (temp*(1.0-(0.68+(kalMid*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalM[prevSampL3] = kalM[prevSampL2]; kalM[prevSampL2] = kalM[prevSampL1];
		kalM[prevSampL1] = (kalM[kalGainL] * kalM[kalOutL]) + ((1.0-kalM[kalGainL])*temp);
		//feed the chain of previous samples
		long double bassL = (kalM[kalOutL]+kalM[kalAvgL])*0.5;
		kalM[kalAvgL] = kalM[kalOutL];
		midL -= bassL;
		//end KalmanML
				
		//begin KalmanSL
		temp = bassL;
		kalS[prevSlewL3] += kalS[prevSampL3] - kalS[prevSampL2]; kalS[prevSlewL3] *= 0.5;
		kalS[prevSlewL2] += kalS[prevSampL2] - kalS[prevSampL1]; kalS[prevSlewL2] *= 0.5;
		kalS[prevSlewL1] += kalS[prevSampL1] - bassL; kalS[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kalS[accSlewL2] += kalS[prevSlewL3] - kalS[prevSlewL2]; kalS[accSlewL2] *= 0.5;
		kalS[accSlewL1] += kalS[prevSlewL2] - kalS[prevSlewL1]; kalS[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kalS[accSlewL3] += (kalS[accSlewL2] - kalS[accSlewL1]); kalS[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kalS[kalOutL] += kalS[prevSampL1] + kalS[prevSlewL2] + kalS[accSlewL3]; kalS[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kalS[kalGainL] += fabs(temp-kalS[kalOutL])*kalSub*8.0; kalS[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kalS[kalGainL] > kalSub*0.5) kalS[kalGainL] = kalSub*0.5;
		//attempts to avoid explosions
		kalS[kalOutL] += (temp*(1.0-(0.68+(kalSub*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kalS[prevSampL3] = kalS[prevSampL2]; kalS[prevSampL2] = kalS[prevSampL1];
		kalS[prevSampL1] = (kalS[kalGainL] * kalS[kalOutL]) + ((1.0-kalS[kalGainL])*temp);
		//feed the chain of previous samples
		long double subL = (kalS[kalOutL]+kalS[kalAvgL])*0.5;
		kalS[kalAvgL] = kalS[kalOutL];
		bassL -= subL;
		//end KalmanSL
		
		inputSampleL = (subL*subGain);
		
		if (bassZoom > 0.0) {
			double closer = bassL * 1.57079633;
			if (closer > 1.57079633) closer = 1.57079633;
			if (closer < -1.57079633) closer = -1.57079633;
			bassL = (bassL*(1.0-bassZoom))+(sin(closer)*bassZoom);
		} //zooming in will make the body of the sound louder: it's just Density
		if (bassZoom < 0.0) {
			double farther = fabs(bassL) * 1.57079633;
			if (farther > 1.57079633) farther = 1.0;
			else farther = 1.0-cos(farther);
			if (bassL > 0.0) bassL = (bassL*(1.0+bassZoom))-(farther*bassZoom*1.57079633);
			if (bassL < 0.0) bassL = (bassL*(1.0+bassZoom))+(farther*bassZoom*1.57079633);			
		} //zooming out boosts the hottest peaks but cuts back softer stuff
		inputSampleL += (bassL*bassGain);
		
		if (midZoom > 0.0) {
			double closer = midL * 1.57079633;
			if (closer > 1.57079633) closer = 1.57079633;
			if (closer < -1.57079633) closer = -1.57079633;
			midL = (midL*(1.0-midZoom))+(sin(closer)*midZoom);
		} //zooming in will make the body of the sound louder: it's just Density
		if (midZoom < 0.0) {
			double farther = fabs(midL) * 1.57079633;
			if (farther > 1.57079633) farther = 1.0;
			else farther = 1.0-cos(farther);
			if (midL > 0.0) midL = (midL*(1.0+midZoom))-(farther*midZoom*1.57079633);
			if (midL < 0.0) midL = (midL*(1.0+midZoom))+(farther*midZoom*1.57079633);			
		} //zooming out boosts the hottest peaks but cuts back softer stuff
		inputSampleL += (midL*midGain);
		
		if (trebleZoom > 0.0) {
			double closer = trebleL * 1.57079633;
			if (closer > 1.57079633) closer = 1.57079633;
			if (closer < -1.57079633) closer = -1.57079633;
			trebleL = (trebleL*(1.0-trebleZoom))+(sin(closer)*trebleZoom);
		} //zooming in will make the body of the sound louder: it's just Density
		if (trebleZoom < 0.0) {
			double farther = fabs(trebleL) * 1.57079633;
			if (farther > 1.57079633) farther = 1.0;
			else farther = 1.0-cos(farther);
			if (trebleL > 0.0) trebleL = (trebleL*(1.0+trebleZoom))-(farther*trebleZoom*1.57079633);
			if (trebleL < 0.0) trebleL = (trebleL*(1.0+trebleZoom))+(farther*trebleZoom*1.57079633);			
		} //zooming out boosts the hottest peaks but cuts back softer stuff
		inputSampleL += (trebleL*trebleGain);
		
		inputSampleL *= driveOut;
		
		//begin ClipOnly2 as a little, compressed chunk that can be dropped into code
		if (inputSampleL > 4.0) inputSampleL = 4.0; if (inputSampleL < -4.0) inputSampleL = -4.0;
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=0.7058208+(inputSampleL*0.2609148);
			else lastSampleL = 0.2491717+(lastSampleL*0.7390851);
		} wasPosClipL = false;
		if (inputSampleL>0.9549925859) {wasPosClipL=true;inputSampleL=0.7058208+(lastSampleL*0.2609148);}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=-0.7058208+(inputSampleL*0.2609148);
			else lastSampleL=-0.2491717+(lastSampleL*0.7390851);
		} wasNegClipL = false;
		if (inputSampleL<-0.9549925859) {wasNegClipL=true;inputSampleL=-0.7058208+(lastSampleL*0.2609148);}
		intermediateL[spacing] = inputSampleL;
        inputSampleL = lastSampleL; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateL[x-1] = intermediateL[x];
		lastSampleL = intermediateL[0]; //run a little buffer to handle this
		//end ClipOnly2 as a little, compressed chunk that can be dropped into code
		
		temp = inputSampleL;
		long double sinew = threshSinew * cos(lastSinewL*lastSinewL);
		if (inputSampleL - lastSinewL > sinew) temp = lastSinewL + sinew;
		if (-(inputSampleL - lastSinewL) > sinew) temp = lastSinewL - sinew;
		lastSinewL = temp;
		inputSampleL = (inputSampleL * (1.0-depthSinew))+(lastSinewL*depthSinew);
		//run Sinew to stop excess slews, but run a dry/wet to allow a range of brights
		
		switch (dither) {
			case 1:
				//begin Dark		
				inputSampleL *= 8388608.0; //we will apply the 24 bit Dark
				//We are doing it first Left, then Right, because the loops may run faster if
				//they aren't too jammed full of variables. This means re-running code.
				
				//begin left
				quantA = floor(inputSampleL);
				quantB = floor(inputSampleL+1.0);
				//to do this style of dither, we quantize in either direction and then
				//do a reconstruction of what the result will be for each choice.
				//We then evaluate which one we like, and keep a history of what we previously had
				
				expectedSlew = 0;
				for(int x = 0; x < depth; x++) {
					expectedSlew += (darkSampleL[x+1] - darkSampleL[x]);
				}
				expectedSlew /= depth; //we have an average of all recent slews
				//we are doing that to voice the thing down into the upper mids a bit
				//it mustn't just soften the brightest treble, it must smooth high mids too
				
				testA = fabs((darkSampleL[0] - quantA) - expectedSlew);
				testB = fabs((darkSampleL[0] - quantB) - expectedSlew);
				
				if (testA < testB) inputSampleL = quantA;
				else inputSampleL = quantB;
				//select whichever one departs LEAST from the vector of averaged
				//reconstructed previous final samples. This will force a kind of dithering
				//as it'll make the output end up as smooth as possible
				
				for(int x = depth; x >=0; x--) {
					darkSampleL[x+1] = darkSampleL[x];
				}
				darkSampleL[0] = inputSampleL;
				//end Dark left
								
				inputSampleL /= 8388608.0;
				break; //Dark (Monitoring2)
			case 2:
				//begin Dark	 for Ten Nines
				inputSampleL *= 8388608.0; //we will apply the 24 bit Dark
				//We are doing it first Left, then Right, because the loops may run faster if
				//they aren't too jammed full of variables. This means re-running code.
				
				//begin L
				correction = 0;
				if (flip) {
					NSOddL = (NSOddL * 0.9999999999) + prevShapeL;
					NSEvenL = (NSEvenL * 0.9999999999) - prevShapeL;
					correction = NSOddL;
				} else {
					NSOddL = (NSOddL * 0.9999999999) - prevShapeL;
					NSEvenL = (NSEvenL * 0.9999999999) + prevShapeL;
					correction = NSEvenL;
				}
				shapedSampleL = inputSampleL+correction;
				//end Ten Nines L
				
				//begin Dark L
				quantA = floor(shapedSampleL);
				quantB = floor(shapedSampleL+1.0);
				//to do this style of dither, we quantize in either direction and then
				//do a reconstruction of what the result will be for each choice.
				//We then evaluate which one we like, and keep a history of what we previously had
				
				expectedSlew = 0;
				for(int x = 0; x < depth; x++) {
					expectedSlew += (darkSampleL[x+1] - darkSampleL[x]);
				}
				expectedSlew /= depth; //we have an average of all recent slews
				//we are doing that to voice the thing down into the upper mids a bit
				//it mustn't just soften the brightest treble, it must smooth high mids too
				
				testA = fabs((darkSampleL[0] - quantA) - expectedSlew);
				testB = fabs((darkSampleL[0] - quantB) - expectedSlew);
				
				if (testA < testB) inputSampleL = quantA;
				else inputSampleL = quantB;
				//select whichever one departs LEAST from the vector of averaged
				//reconstructed previous final samples. This will force a kind of dithering
				//as it'll make the output end up as smooth as possible
				
				for(int x = depth; x >=0; x--) {
					darkSampleL[x+1] = darkSampleL[x];
				}
				darkSampleL[0] = inputSampleL;
				//end Dark L
				
				prevShapeL = (floor(shapedSampleL) - inputSampleL)*0.9999999999;
				//end Ten Nines L

				flip = !flip;
				
				inputSampleL /= 8388608.0;
				break; //Ten Nines (which goes into Dark in Monitoring3)
			case 3:
				inputSampleL *= 8388608.0;
				
				ditherL = -1.0;
				ditherL += (double(fpdL)/UINT32_MAX);
				fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
				ditherL += (double(fpdL)/UINT32_MAX);
				//TPDF: two 0-1 random noises
												
				inputSampleL = floor(inputSampleL+ditherL);
				
				inputSampleL /= 8388608.0;
				break; //TPDF
			case 4:
				inputSampleL *= 8388608.0;
				//Paul Frindle: It's true that the dither itself can sound different 
				//if it's given a different freq response and you get to hear it. 
				//The one we use most is triangular single pole high pass dither. 
				//It's not freq bent enough to sound odd, but is slightly less audible than 
				//flat dither. It can also be easily made by taking one sample of dither 
				//away from the previous one - this gives you the triangular PDF and the 
				//filtering in one go :-)
				
				currentDither = (double(fpdL)/UINT32_MAX);
				ditherL = currentDither;
				ditherL -= previousDitherL;
				previousDitherL = currentDither;
				//TPDF: 0-1 random noise
				
				inputSampleL = floor(inputSampleL+ditherL);
				
				inputSampleL /= 8388608.0;
				break; //PaulDither
			case 5:
				inputSampleL *= 8388608.0;
				cutbinsL = false;
				drySampleL = inputSampleL;//re-using in NJAD
				inputSampleL -= noiseShapingL;
				//NJAD L
				benfordize = floor(inputSampleL);
				while (benfordize >= 1.0) benfordize /= 10;
				while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;
				hotbinA = floor(benfordize);
				//hotbin becomes the Benford bin value for this number floored
				totalA = 0.0;
				if ((hotbinA > 0) && (hotbinA < 10))
				{
					bynL[hotbinA] += 1; if (bynL[hotbinA] > 982) cutbinsL = true;
					totalA += (301-bynL[1]); totalA += (176-bynL[2]); totalA += (125-bynL[3]);
					totalA += (97-bynL[4]); totalA += (79-bynL[5]); totalA += (67-bynL[6]);
					totalA += (58-bynL[7]); totalA += (51-bynL[8]); totalA += (46-bynL[9]); bynL[hotbinA] -= 1;
				} else hotbinA = 10;
				//produce total number- smaller is closer to Benford real
				benfordize = ceil(inputSampleL);
				while (benfordize >= 1.0) benfordize /= 10;
				while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;
				hotbinB = floor(benfordize);
				//hotbin becomes the Benford bin value for this number ceiled
				totalB = 0.0;
				if ((hotbinB > 0) && (hotbinB < 10))
				{
					bynL[hotbinB] += 1; if (bynL[hotbinB] > 982) cutbinsL = true;
					totalB += (301-bynL[1]); totalB += (176-bynL[2]); totalB += (125-bynL[3]);
					totalB += (97-bynL[4]); totalB += (79-bynL[5]); totalB += (67-bynL[6]);
					totalB += (58-bynL[7]); totalB += (51-bynL[8]); totalB += (46-bynL[9]); bynL[hotbinB] -= 1;
				} else hotbinB = 10;
				//produce total number- smaller is closer to Benford real
				if (totalA < totalB) {bynL[hotbinA] += 1; outputSample = floor(inputSampleL);}
				else {bynL[hotbinB] += 1; outputSample = floor(inputSampleL+1);}
				//assign the relevant one to the delay line
				//and floor/ceil signal accordingly
				if (cutbinsL) {
					bynL[1] *= 0.99; bynL[2] *= 0.99; bynL[3] *= 0.99; bynL[4] *= 0.99; bynL[5] *= 0.99; 
					bynL[6] *= 0.99; bynL[7] *= 0.99; bynL[8] *= 0.99; bynL[9] *= 0.99; bynL[10] *= 0.99; 
				}
				noiseShapingL += outputSample - drySampleL;			
				if (noiseShapingL > fabs(inputSampleL)) noiseShapingL = fabs(inputSampleL);
				if (noiseShapingL < -fabs(inputSampleL)) noiseShapingL = -fabs(inputSampleL);
				inputSampleL /= 8388608.0;
				if (inputSampleL > 1.0) inputSampleL = 1.0;
				if (inputSampleL < -1.0) inputSampleL = -1.0;
				//finished NJAD L
				break; //NJAD (Monitoring. Brightest)
			case 6:
				//begin 32 bit stereo floating point dither
				frexpf((float)inputSampleL, &expon);
				fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
				inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
				//end 32 bit stereo floating point dither
				break; //Bypass for saving floating point files directly
		}
				
		*destP = inputSampleL;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

