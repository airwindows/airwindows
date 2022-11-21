/*
*	File:		NonlinearSpace.cpp
*	
*	Version:	1.0
* 
*	Created:	12/21/14
*	
*	Copyright:  Copyright © 2014 Airwindows, Airwindows uses the MIT license
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
	NonlinearSpace.cpp
	
=============================================================================*/
#include "NonlinearSpace.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, NonlinearSpace)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NonlinearSpace::NonlinearSpace
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
NonlinearSpace::NonlinearSpace(AudioUnit component)
: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	SetParameter(kParam_Four, kDefaultValue_ParamFour );
	SetParameter(kParam_Five, kDefaultValue_ParamFive );
	SetParameter(kParam_Six, kDefaultValue_ParamSix );
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NonlinearSpace::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			NonlinearSpace::GetParameterValueStrings(AudioUnitScope		inScope,
														AudioUnitParameterID	inParameterID,
														CFArrayRef *		outStrings)
{
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_One)) //ID must be actual name of parameter identifier, not number
	{
		if (outStrings == NULL) return noErr;
		CFStringRef strings [] =
		{
			kMenuItem_16,
			kMenuItem_32,
			kMenuItem_44,
			kMenuItem_48,
			kMenuItem_64,
			kMenuItem_88,
			kMenuItem_96,
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
//	NonlinearSpace::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			NonlinearSpace::GetParameterInfo(AudioUnitScope		inScope,
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
				outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = k16;
                outParameterInfo.maxValue = k96;
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
			case kParam_Five:
                AUBase::FillInParameterName (outParameterInfo, kParameterFiveName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = -1.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamFive;
                break;
			case kParam_Six:
                AUBase::FillInParameterName (outParameterInfo, kParameterSixName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamSix;
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
//	NonlinearSpace::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			NonlinearSpace::GetPropertyInfo (AudioUnitPropertyID	inID,
												AudioUnitScope		inScope,
												AudioUnitElement	inElement,
												UInt32 &		outDataSize,
												Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 NonlinearSpace::SupportedNumChannels(const AUChannelInfo ** outInfo)
{
	if (outInfo != NULL)
	{
		static AUChannelInfo info;
		info.inChannels = 2;
		info.outChannels = 2;
		*outInfo = &info;
	}
	
	return 1;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	NonlinearSpace::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			NonlinearSpace::GetProperty(	AudioUnitPropertyID inID,
										   AudioUnitScope 		inScope,
										   AudioUnitElement 	inElement,
										   void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	NonlinearSpace::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult NonlinearSpace::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____NonlinearSpaceEffectKernel

//-----------------------------------------------------------------------------------------
// this is called the reset the DSP state (clear buffers, reset counters, etc.)
ComponentResult		NonlinearSpace::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	int count;
	for(count = 0; count < 2347; count++) {dMid[count] = 0.0;}
	for(count = 0; count < 1333; count++) {dSide[count] = 0.0;}
	for(count = 0; count < 5923; count++) {dLeft[count] = 0.0;}
	for(count = 0; count < 5925; count++) {dRight[count] = 0.0;}
	
	for(count = 0; count < 7574; count++) {dpreR[count] = 0.0;}
	for(count = 0; count < 7574; count++) {dpreL[count] = 0.0;}
	
	for(count = 0; count < 7574; count++) {dA[count] = 0.0;}
	for(count = 0; count < 7308; count++) {dB[count] = 0.0;}
	for(count = 0; count < 7178; count++) {dC[count] = 0.0;}
	for(count = 0; count < 6908; count++) {dD[count] = 0.0;}
	for(count = 0; count < 6780; count++) {dE[count] = 0.0;}
	for(count = 0; count < 6522; count++) {dF[count] = 0.0;}
	for(count = 0; count < 5982; count++) {dG[count] = 0.0;}
	for(count = 0; count < 5564; count++) {dH[count] = 0.0;}
	for(count = 0; count < 5298; count++) {dI[count] = 0.0;}
	for(count = 0; count < 4904; count++) {dJ[count] = 0.0;}
	for(count = 0; count < 4760; count++) {dK[count] = 0.0;}
	for(count = 0; count < 4490; count++) {dL[count] = 0.0;}
	for(count = 0; count < 4392; count++) {dM[count] = 0.0;}
	for(count = 0; count < 4230; count++) {dN[count] = 0.0;}
	for(count = 0; count < 4154; count++) {dO[count] = 0.0;}
	for(count = 0; count < 3990; count++) {dP[count] = 0.0;}
	for(count = 0; count < 3660; count++) {dQ[count] = 0.0;}
	for(count = 0; count < 3408; count++) {dR[count] = 0.0;}
	for(count = 0; count < 3252; count++) {dS[count] = 0.0;}
	for(count = 0; count < 3000; count++) {dT[count] = 0.0;}
	for(count = 0; count < 2918; count++) {dU[count] = 0.0;}
	for(count = 0; count < 2750; count++) {dV[count] = 0.0;}
	for(count = 0; count < 2504; count++) {dW[count] = 0.0;}
	for(count = 0; count < 2424; count++) {dX[count] = 0.0;}
	for(count = 0; count < 2147; count++) {dY[count] = 0.0;}
	for(count = 0; count < 2089; count++) {dZ[count] = 0.0;}
	
	oneMid = 1; delayMid = 2346; maxdelayMid = 2346;
	oneSide = 1; delaySide = 1332; maxdelaySide = 1332;
	oneLeft = 1; delayLeft = 5922; maxdelayLeft = 5922;
	oneRight = 1; delayRight = 5924; maxdelayRight = 5924;
	onepre = 1; delaypre = 7573; maxdelaypre = 7573;
	
	oneA = 1; twoA = 2; treA = 3; delayA = 7573; maxdelayA = 7573;
	oneB = 1; twoB = 2; treB = 3; delayB = 7307; maxdelayB = 7307;
	oneC = 1; twoC = 2; treC = 3; delayC = 7177; maxdelayC = 7177;
	oneD = 1; twoD = 2; treD = 3; delayD = 6907; maxdelayD = 6907;
	oneE = 1; twoE = 2; treE = 3; delayE = 6779; maxdelayE = 6779;
	oneF = 1; twoF = 2; treF = 3; delayF = 6521; maxdelayF = 6521;
	oneG = 1; twoG = 2; treG = 3; delayG = 5981; maxdelayG = 5981;
	oneH = 1; twoH = 2; treH = 3; delayH = 5563; maxdelayH = 5563;
	oneI = 1; twoI = 2; treI = 3; delayI = 5297; maxdelayI = 5297;
	oneJ = 1; twoJ = 2; treJ = 3; delayJ = 4903; maxdelayJ = 4903;
	oneK = 1; twoK = 2; treK = 3; delayK = 4759; maxdelayK = 4759;
	oneL = 1; twoL = 2; treL = 3; delayL = 4489; maxdelayL = 4489;
	oneM = 1; twoM = 2; treM = 3; delayM = 4391; maxdelayM = 4391;
	oneN = 1; twoN = 2; treN = 3; delayN = 4229; maxdelayN = 4229;
	oneO = 1; twoO = 2; treO = 3; delayO = 4153; maxdelayO = 4153;
	oneP = 1; twoP = 2; treP = 3; delayP = 3989; maxdelayP = 3989;
	oneQ = 1; twoQ = 2; treQ = 3; delayQ = 3659; maxdelayQ = 3659;
	oneR = 1; twoR = 2; treR = 3; delayR = 3407; maxdelayR = 3407;
	oneS = 1; twoS = 2; treS = 3; delayS = 3251; maxdelayS = 3251;
	oneT = 1; twoT = 2; treT = 3; delayT = 2999; maxdelayT = 2999;
	oneU = 1; twoU = 2; treU = 3; delayU = 2917; maxdelayU = 2917;
	oneV = 1; twoV = 2; treV = 3; delayV = 2749; maxdelayV = 2749;
	oneW = 1; twoW = 2; treW = 3; delayW = 2503; maxdelayW = 2503;
	oneX = 1; twoX = 2; treX = 3; delayX = 2423; maxdelayX = 2423;
	oneY = 1; twoY = 2; treY = 3; delayY = 2146; maxdelayY = 2146;
	oneZ = 1; twoZ = 2; treZ = 3; delayZ = 2088; maxdelayZ = 2088;
	
	avgInputL = 0.0;
	avgInputR = 0.0;
	avgOutputL = 0.0;
	avgOutputR = 0.0;
	avg2InputL = 0.0;
	avg2InputR = 0.0;
	avg2OutputL = 0.0;
	avg2OutputR = 0.0;
	a2vgInputL = 0.0;
	a2vgInputR = 0.0;
	a2vgOutputL = 0.0;
	a2vgOutputR = 0.0;
	a2vg2InputL = 0.0;
	a2vg2InputR = 0.0;
	a2vg2OutputL = 0.0;
	a2vg2OutputR = 0.0;
	
	lowpassSampleAA = 0.0;
	lowpassSampleAB = 0.0;
	lowpassSampleBA = 0.0;
	lowpassSampleBB = 0.0;
	lowpassSampleCA = 0.0;
	lowpassSampleCB = 0.0;
	lowpassSampleDA = 0.0;
	lowpassSampleDB = 0.0;
	lowpassSampleE = 0.0;
	lowpassSampleF = 0.0;
	lowpassSampleG = 0.0;
	
	rowpassSampleAA = 0.0;
	rowpassSampleAB = 0.0;
	rowpassSampleBA = 0.0;
	rowpassSampleBB = 0.0;
	rowpassSampleCA = 0.0;
	rowpassSampleCB = 0.0;
	rowpassSampleDA = 0.0;
	rowpassSampleDB = 0.0;
	rowpassSampleE = 0.0;
	rowpassSampleF = 0.0;
	rowpassSampleG = 0.0;
	
	interpolA = 0.0;
	interpolB = 0.0;
	interpolC = 0.0;
	interpolD = 0.0;
	interpolE = 0.0;
	interpolF = 0.0;
	interpolG = 0.0;
	interpolH = 0.0;
	interpolI = 0.0;
	interpolJ = 0.0;
	interpolK = 0.0;
	interpolL = 0.0;
	interpolM = 0.0;
	interpolN = 0.0;
	interpolO = 0.0;
	interpolP = 0.0;
	interpolQ = 0.0;
	interpolR = 0.0;
	interpolS = 0.0;
	interpolT = 0.0;
	interpolU = 0.0;
	interpolV = 0.0;
	interpolW = 0.0;
	interpolX = 0.0;
	interpolY = 0.0;
	interpolZ = 0.0;
	
	pitchshiftA = 1.0 / maxdelayA;
	pitchshiftB = 1.0 / maxdelayB;
	pitchshiftC = 1.0 / maxdelayC;
	pitchshiftD = 1.0 / maxdelayD;
	pitchshiftE = 1.0 / maxdelayE;
	pitchshiftF = 1.0 / maxdelayF;
	pitchshiftG = 1.0 / maxdelayG;
	pitchshiftH = 1.0 / maxdelayH;
	pitchshiftI = 1.0 / maxdelayI;
	pitchshiftJ = 1.0 / maxdelayJ;
	pitchshiftK = 1.0 / maxdelayK;
	pitchshiftL = 1.0 / maxdelayL;
	pitchshiftM = 1.0 / maxdelayM;
	pitchshiftN = 1.0 / maxdelayN;
	pitchshiftO = 1.0 / maxdelayO;
	pitchshiftP = 1.0 / maxdelayP;
	pitchshiftQ = 1.0 / maxdelayQ;
	pitchshiftR = 1.0 / maxdelayR;
	pitchshiftS = 1.0 / maxdelayS;
	pitchshiftT = 1.0 / maxdelayT;
	pitchshiftU = 1.0 / maxdelayU;
	pitchshiftV = 1.0 / maxdelayV;
	pitchshiftW = 1.0 / maxdelayW;
	pitchshiftX = 1.0 / maxdelayX;
	pitchshiftY = 1.0 / maxdelayY;
	pitchshiftZ = 1.0 / maxdelayZ;
	
	nonlin = 0.0;
	
	verboutL = 0.0;
	verboutR = 0.0;
	iirCCSampleL = 0.0;
	iirCCSampleR = 0.0;
	iirSampleL = 0.0;
	iirSampleR = 0.0;
	savedRoomsize = -1.0; //force update to begin
	countdown = -1;
	flip = true;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	FarNonlinearSpace3::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		NonlinearSpace::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
										  const AudioBufferList & inBuffer,
										  AudioBufferList & outBuffer,
										  UInt32 			inFramesToProcess)
{
	
	Float32 * inputL = (Float32*)(inBuffer.mBuffers[0].mData);
	Float32 * inputR = (Float32*)(inBuffer.mBuffers[1].mData);
	Float32 * outputL = (Float32*)(outBuffer.mBuffers[0].mData);
	Float32 * outputR = (Float32*)(outBuffer.mBuffers[1].mData);
	UInt32 nSampleFrames = inFramesToProcess;
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	double mid;
	double side;
	Float64 overallscale = 1.0;
	int samplerate = (int) GetParameter( kParam_One );
	switch (samplerate)
	{
		case 1: overallscale *= (16/44.1); break; //16
		case 2: overallscale *= (32/44.1); break; //32
		case 3: overallscale *= 1.0; break; //44.1
		case 4: overallscale *= (48/44.1); break; //48
		case 5: overallscale *= (64/44.1); break; //64
		case 6: overallscale *= 2.0; break; //88.2
		case 7: overallscale *= (96/44.1); break; //96
	}
	nonlin *= 0.001; //scale suitably to apply to our liveness value
	Float64 basefeedback = 0.45 + (nonlin * pow(GetParameter( kParam_Five ),3)); //nonlin from previous sample, positive adds liveness when loud
	nonlin = 0.0; //reset it here for setting up again next time
	Float64 tankfeedback = basefeedback + (pow(GetParameter( kParam_Two ),2) * 0.05);
	//liveness
	if (tankfeedback > 0.5) tankfeedback = 0.5;
	if (tankfeedback < 0.4) tankfeedback = 0.4;
	Float64 iirAmountC = 1.0-pow(1.0-GetParameter( kParam_Three ),2);
	//most of the range is up at the top end
	iirAmountC += (iirAmountC/overallscale);
	iirAmountC /= 2.0;
	if (iirAmountC > 1.1) iirAmountC = 1.1;
	//lowpass, check to see if it's working reasonably at 96K
	Float64 iirAmount = (((1.0-pow(GetParameter( kParam_Four ),2)) * 0.09)/overallscale)+0.001;
	if (iirAmount > 1.0) iirAmount = 1.0;
	if (iirAmount < 0.001) iirAmount = 0.001;
	Float64 wetness = GetParameter( kParam_Six );
	Float64 dryness = 1.0 - wetness;
	Float64 roomsize = overallscale*0.203;
	Float64 lean = 0.125;
	Float64 invlean = 1.0 - lean;
	Float64 pspeed = 0.145;
	Float64 outcouple = 0.5 - tankfeedback;
	Float64 constallpass = 0.618033988749894848204586; //golden ratio!
	Float64 temp;
	int allpasstemp;
	Float64 predelay = 0.222 * overallscale;
	
	//reverb setup
	
	delayA = (int(maxdelayA * roomsize));
	delayB = (int(maxdelayB * roomsize));
	delayC = (int(maxdelayC * roomsize));
	delayD = (int(maxdelayD * roomsize));
	delayE = (int(maxdelayE * roomsize));
	delayF = (int(maxdelayF * roomsize));
	delayG = (int(maxdelayG * roomsize));
	delayH = (int(maxdelayH * roomsize));
	delayI = (int(maxdelayI * roomsize));
	delayJ = (int(maxdelayJ * roomsize));
	delayK = (int(maxdelayK * roomsize));
	delayL = (int(maxdelayL * roomsize));
	delayM = (int(maxdelayM * roomsize));
	delayN = (int(maxdelayN * roomsize));
	delayO = (int(maxdelayO * roomsize));
	delayP = (int(maxdelayP * roomsize));
	delayQ = (int(maxdelayQ * roomsize));
	delayR = (int(maxdelayR * roomsize));
	delayS = (int(maxdelayS * roomsize));
	delayT = (int(maxdelayT * roomsize));
	delayU = (int(maxdelayU * roomsize));
	delayV = (int(maxdelayV * roomsize));
	delayW = (int(maxdelayW * roomsize));
	delayX = (int(maxdelayX * roomsize));
	delayY = (int(maxdelayY * roomsize));
	delayZ = (int(maxdelayZ * roomsize));
	delayMid = (int(maxdelayMid * roomsize));
	delaySide = (int(maxdelaySide * roomsize));
	delayLeft = (int(maxdelayLeft * roomsize));
	delayRight = (int(maxdelayRight * roomsize));
	delaypre = (int(maxdelaypre * predelay));
	
	while (nSampleFrames-- > 0) {
		inputSampleL = *inputL;
		inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		
		dpreL[onepre] = inputSampleL;
		dpreR[onepre] = inputSampleR;
		onepre--; if (onepre < 0 || onepre > delaypre) {onepre = delaypre;}
		inputSampleL = (dpreL[onepre]);
		inputSampleR = (dpreR[onepre]);
		//predelay
		
		interpolA += pitchshiftA*pspeed;
		interpolB += pitchshiftB*pspeed;
		interpolC += pitchshiftC*pspeed;
		interpolD += pitchshiftD*pspeed;
		interpolE += pitchshiftE*pspeed;
		interpolF += pitchshiftF*pspeed;
		interpolG += pitchshiftG*pspeed;
		interpolH += pitchshiftH*pspeed;
		interpolI += pitchshiftI*pspeed;
		interpolJ += pitchshiftJ*pspeed;
		interpolK += pitchshiftK*pspeed;
		interpolL += pitchshiftL*pspeed;
		interpolM += pitchshiftM*pspeed;
		interpolN += pitchshiftN*pspeed;
		interpolO += pitchshiftO*pspeed;
		interpolP += pitchshiftP*pspeed;
		interpolQ += pitchshiftQ*pspeed;
		interpolR += pitchshiftR*pspeed;
		interpolS += pitchshiftS*pspeed;
		interpolT += pitchshiftT*pspeed;
		interpolU += pitchshiftU*pspeed;
		interpolV += pitchshiftV*pspeed;
		interpolW += pitchshiftW*pspeed;
		interpolX += pitchshiftX*pspeed;
		interpolY += pitchshiftY*pspeed;
		interpolZ += pitchshiftZ*pspeed;
		//increment all the sub-sample offsets for the pitch shifting of combs
		
		if (interpolA > 1.0) {pitchshiftA = -fabs(pitchshiftA); interpolA += pitchshiftA*pspeed;}
		if (interpolB > 1.0) {pitchshiftB = -fabs(pitchshiftB); interpolB += pitchshiftB*pspeed;}
		if (interpolC > 1.0) {pitchshiftC = -fabs(pitchshiftC); interpolC += pitchshiftC*pspeed;}
		if (interpolD > 1.0) {pitchshiftD = -fabs(pitchshiftD); interpolD += pitchshiftD*pspeed;}
		if (interpolE > 1.0) {pitchshiftE = -fabs(pitchshiftE); interpolE += pitchshiftE*pspeed;}
		if (interpolF > 1.0) {pitchshiftF = -fabs(pitchshiftF); interpolF += pitchshiftF*pspeed;}
		if (interpolG > 1.0) {pitchshiftG = -fabs(pitchshiftG); interpolG += pitchshiftG*pspeed;}
		if (interpolH > 1.0) {pitchshiftH = -fabs(pitchshiftH); interpolH += pitchshiftH*pspeed;}
		if (interpolI > 1.0) {pitchshiftI = -fabs(pitchshiftI); interpolI += pitchshiftI*pspeed;}
		if (interpolJ > 1.0) {pitchshiftJ = -fabs(pitchshiftJ); interpolJ += pitchshiftJ*pspeed;}
		if (interpolK > 1.0) {pitchshiftK = -fabs(pitchshiftK); interpolK += pitchshiftK*pspeed;}
		if (interpolL > 1.0) {pitchshiftL = -fabs(pitchshiftL); interpolL += pitchshiftL*pspeed;}
		if (interpolM > 1.0) {pitchshiftM = -fabs(pitchshiftM); interpolM += pitchshiftM*pspeed;}
		if (interpolN > 1.0) {pitchshiftN = -fabs(pitchshiftN); interpolN += pitchshiftN*pspeed;}
		if (interpolO > 1.0) {pitchshiftO = -fabs(pitchshiftO); interpolO += pitchshiftO*pspeed;}
		if (interpolP > 1.0) {pitchshiftP = -fabs(pitchshiftP); interpolP += pitchshiftP*pspeed;}
		if (interpolQ > 1.0) {pitchshiftQ = -fabs(pitchshiftQ); interpolQ += pitchshiftQ*pspeed;}
		if (interpolR > 1.0) {pitchshiftR = -fabs(pitchshiftR); interpolR += pitchshiftR*pspeed;}
		if (interpolS > 1.0) {pitchshiftS = -fabs(pitchshiftS); interpolS += pitchshiftS*pspeed;}
		if (interpolT > 1.0) {pitchshiftT = -fabs(pitchshiftT); interpolT += pitchshiftT*pspeed;}
		if (interpolU > 1.0) {pitchshiftU = -fabs(pitchshiftU); interpolU += pitchshiftU*pspeed;}
		if (interpolV > 1.0) {pitchshiftV = -fabs(pitchshiftV); interpolV += pitchshiftV*pspeed;}
		if (interpolW > 1.0) {pitchshiftW = -fabs(pitchshiftW); interpolW += pitchshiftW*pspeed;}
		if (interpolX > 1.0) {pitchshiftX = -fabs(pitchshiftX); interpolX += pitchshiftX*pspeed;}
		if (interpolY > 1.0) {pitchshiftY = -fabs(pitchshiftY); interpolY += pitchshiftY*pspeed;}
		if (interpolZ > 1.0) {pitchshiftZ = -fabs(pitchshiftZ); interpolZ += pitchshiftZ*pspeed;}
		
		if (interpolA < 0.0) {pitchshiftA = fabs(pitchshiftA); interpolA += pitchshiftA*pspeed;}
		if (interpolB < 0.0) {pitchshiftB = fabs(pitchshiftB); interpolB += pitchshiftB*pspeed;}
		if (interpolC < 0.0) {pitchshiftC = fabs(pitchshiftC); interpolC += pitchshiftC*pspeed;}
		if (interpolD < 0.0) {pitchshiftD = fabs(pitchshiftD); interpolD += pitchshiftD*pspeed;}
		if (interpolE < 0.0) {pitchshiftE = fabs(pitchshiftE); interpolE += pitchshiftE*pspeed;}
		if (interpolF < 0.0) {pitchshiftF = fabs(pitchshiftF); interpolF += pitchshiftF*pspeed;}
		if (interpolG < 0.0) {pitchshiftG = fabs(pitchshiftG); interpolG += pitchshiftG*pspeed;}
		if (interpolH < 0.0) {pitchshiftH = fabs(pitchshiftH); interpolH += pitchshiftH*pspeed;}
		if (interpolI < 0.0) {pitchshiftI = fabs(pitchshiftI); interpolI += pitchshiftI*pspeed;}
		if (interpolJ < 0.0) {pitchshiftJ = fabs(pitchshiftJ); interpolJ += pitchshiftJ*pspeed;}
		if (interpolK < 0.0) {pitchshiftK = fabs(pitchshiftK); interpolK += pitchshiftK*pspeed;}
		if (interpolL < 0.0) {pitchshiftL = fabs(pitchshiftL); interpolL += pitchshiftL*pspeed;}
		if (interpolM < 0.0) {pitchshiftM = fabs(pitchshiftM); interpolM += pitchshiftM*pspeed;}
		if (interpolN < 0.0) {pitchshiftN = fabs(pitchshiftN); interpolN += pitchshiftN*pspeed;}
		if (interpolO < 0.0) {pitchshiftO = fabs(pitchshiftO); interpolO += pitchshiftO*pspeed;}
		if (interpolP < 0.0) {pitchshiftP = fabs(pitchshiftP); interpolP += pitchshiftP*pspeed;}
		if (interpolQ < 0.0) {pitchshiftQ = fabs(pitchshiftQ); interpolQ += pitchshiftQ*pspeed;}
		if (interpolR < 0.0) {pitchshiftR = fabs(pitchshiftR); interpolR += pitchshiftR*pspeed;}
		if (interpolS < 0.0) {pitchshiftS = fabs(pitchshiftS); interpolS += pitchshiftS*pspeed;}
		if (interpolT < 0.0) {pitchshiftT = fabs(pitchshiftT); interpolT += pitchshiftT*pspeed;}
		if (interpolU < 0.0) {pitchshiftU = fabs(pitchshiftU); interpolU += pitchshiftU*pspeed;}
		if (interpolV < 0.0) {pitchshiftV = fabs(pitchshiftV); interpolV += pitchshiftV*pspeed;}
		if (interpolW < 0.0) {pitchshiftW = fabs(pitchshiftW); interpolW += pitchshiftW*pspeed;}
		if (interpolX < 0.0) {pitchshiftX = fabs(pitchshiftX); interpolX += pitchshiftX*pspeed;}
		if (interpolY < 0.0) {pitchshiftY = fabs(pitchshiftY); interpolY += pitchshiftY*pspeed;}
		if (interpolZ < 0.0) {pitchshiftZ = fabs(pitchshiftZ); interpolZ += pitchshiftZ*pspeed;}
		//all of the sanity checks for interpol for all combs
		
		if (verboutR > 1.0) verboutR = 1.0;
		if (verboutR < -1.0) verboutR = -1.0;
		if (verboutL > 1.0) verboutL = 1.0;
		if (verboutL < -1.0) verboutL = -1.0;
		
		inputSampleL += verboutR;
		inputSampleR += verboutL;
		verboutL = 0.0;
		verboutR = 0.0;
		//here we add in the cross-coupling- output of L tank to R, output of R tank to L
		
		
		mid = inputSampleL + inputSampleR;
		side = inputSampleL - inputSampleR;
		//assign mid and side.	
		
		allpasstemp = oneMid - 1;
		if (allpasstemp < 0 || allpasstemp > delayMid) {allpasstemp = delayMid;}
		mid -= dMid[allpasstemp]*constallpass;
		dMid[oneMid] = mid;
		mid *= constallpass;
		oneMid--; if (oneMid < 0 || oneMid > delayMid) {oneMid = delayMid;}
		mid += (dMid[oneMid]);
		nonlin += fabs(dMid[oneMid]);
		//allpass filter mid		
		
		allpasstemp = oneSide - 1;
		if (allpasstemp < 0 || allpasstemp > delaySide) {allpasstemp = delaySide;}
		side -= dSide[allpasstemp]*constallpass;
		dSide[oneSide] = side;
		side *= constallpass;
		oneSide--; if (oneSide < 0 || oneSide > delaySide) {oneSide = delaySide;}
		side += (dSide[oneSide]);
		nonlin += fabs(dSide[oneSide]);
		//allpass filter side
		
		//here we do allpasses on the mid and side
		
		allpasstemp = oneLeft - 1;
		if (allpasstemp < 0 || allpasstemp > delayLeft) {allpasstemp = delayLeft;}
		inputSampleL -= dLeft[allpasstemp]*constallpass;
		dLeft[oneLeft] = verboutL;
		inputSampleL *= constallpass;
		oneLeft--; if (oneLeft < 0 || oneLeft > delayLeft) {oneLeft = delayLeft;}
		inputSampleL += (dLeft[oneLeft]);
		nonlin += fabs(dLeft[oneLeft]);
		//allpass filter left
		
		
		allpasstemp = oneRight - 1;
		if (allpasstemp < 0 || allpasstemp > delayRight) {allpasstemp = delayRight;}
		inputSampleR -= dRight[allpasstemp]*constallpass;
		dRight[oneRight] = verboutR;
		inputSampleR *= constallpass;
		oneRight--; if (oneRight < 0 || oneRight > delayRight) {oneRight = delayRight;}
		inputSampleR += (dRight[oneRight]);
		nonlin += fabs(dRight[oneRight]);
		//allpass filter right
		
		
		inputSampleL += (mid+side)/2.0;
		inputSampleR += (mid-side)/2.0;
		//here we get back to a L/R topology by adding the mid/side in parallel with L/R
		
		
		
		temp = (dA[oneA]*interpolA );
		temp += (dA[treA]*( 1.0 - interpolA ));
		temp += ((dA[twoA]));
		dA[treA] = (temp*tankfeedback);
		dA[treA] += inputSampleL;
		oneA--; if (oneA < 0 || oneA > delayA) {oneA = delayA;}
		twoA--; if (twoA < 0 || twoA > delayA) {twoA = delayA;}
		treA--; if (treA < 0 || treA > delayA) {treA = delayA;}
		temp = (dA[oneA]*interpolA );
		temp += (dA[treA]*( 1.0 - interpolA ));
		temp *=  (invlean + (lean*fabs(dA[twoA])));
   		verboutL += temp;
		//comb filter A
		temp = (dC[oneC]*interpolC );
		temp += (dC[treC]*( 1.0 - interpolC ));
		temp += ((dC[twoC]));
		dC[treC] = (temp*tankfeedback);
		dC[treC] += inputSampleL;
		oneC--; if (oneC < 0 || oneC > delayC) {oneC = delayC;}
		twoC--; if (twoC < 0 || twoC > delayC) {twoC = delayC;}
		treC--; if (treC < 0 || treC > delayC) {treC = delayC;}
		temp = (dC[oneC]*interpolC );
		temp += (dC[treC]*( 1.0 - interpolC ));
		temp *=  (invlean + (lean*fabs(dC[twoC])));
   		verboutL += temp;
		//comb filter C
		temp = (dE[oneE]*interpolE );
		temp += (dE[treE]*( 1.0 - interpolE ));
		temp += ((dE[twoE]));
		dE[treE] = (temp*tankfeedback);
		dE[treE] += inputSampleL;
		oneE--; if (oneE < 0 || oneE > delayE) {oneE = delayE;}
		twoE--; if (twoE < 0 || twoE > delayE) {twoE = delayE;}
		treE--; if (treE < 0 || treE > delayE) {treE = delayE;}
		temp = (dE[oneE]*interpolE );
		temp += (dE[treE]*( 1.0 - interpolE ));
		temp *=  (invlean + (lean*fabs(dE[twoE])));
   		verboutL += temp;
		//comb filter E
		temp = (dG[oneG]*interpolG );
		temp += (dG[treG]*( 1.0 - interpolG ));
		temp += ((dG[twoG]));
		dG[treG] = (temp*tankfeedback);
		dG[treG] += inputSampleL;
		oneG--; if (oneG < 0 || oneG > delayG) {oneG = delayG;}
		twoG--; if (twoG < 0 || twoG > delayG) {twoG = delayG;}
		treG--; if (treG < 0 || treG > delayG) {treG = delayG;}
		temp = (dG[oneG]*interpolG );
		temp += (dG[treG]*( 1.0 - interpolG ));
		temp *=  (invlean + (lean*fabs(dG[twoG])));
   		verboutL += temp;
		//comb filter G
		temp = (dI[oneI]*interpolI );
		temp += (dI[treI]*( 1.0 - interpolI ));
		temp += ((dI[twoI]));
		dI[treI] = (temp*tankfeedback);
		dI[treI] += inputSampleL;
		oneI--; if (oneI < 0 || oneI > delayI) {oneI = delayI;}
		twoI--; if (twoI < 0 || twoI > delayI) {twoI = delayI;}
		treI--; if (treI < 0 || treI > delayI) {treI = delayI;}
		temp = (dI[oneI]*interpolI );
		temp += (dI[treI]*( 1.0 - interpolI ));
		temp *=  (invlean + (lean*fabs(dI[twoI])));
  		verboutL += temp;
		//comb filter I
		temp = (dK[oneK]*interpolK );
		temp += (dK[treK]*( 1.0 - interpolK ));
		temp += ((dK[twoK]));
		dK[treK] = (temp*tankfeedback);
		dK[treK] += inputSampleL;
		oneK--; if (oneK < 0 || oneK > delayK) {oneK = delayK;}
		twoK--; if (twoK < 0 || twoK > delayK) {twoK = delayK;}
		treK--; if (treK < 0 || treK > delayK) {treK = delayK;}
		temp = (dK[oneK]*interpolK );
		temp += (dK[treK]*( 1.0 - interpolK ));
		temp *=  (invlean + (lean*fabs(dK[twoK])));
  		verboutL += temp;
		//comb filter K
		temp = (dM[oneM]*interpolM );
		temp += (dM[treM]*( 1.0 - interpolM ));
		temp += ((dM[twoM]));
		dM[treM] = (temp*tankfeedback);
		dM[treM] += inputSampleL;
		oneM--; if (oneM < 0 || oneM > delayM) {oneM = delayM;}
		twoM--; if (twoM < 0 || twoM > delayM) {twoM = delayM;}
		treM--; if (treM < 0 || treM > delayM) {treM = delayM;}
		temp = (dM[oneM]*interpolM );
		temp += (dM[treM]*( 1.0 - interpolM ));
		temp *=  (invlean + (lean*fabs(dM[twoM])));
   		verboutL += temp;
		//comb filter M
		temp = (dO[oneO]*interpolO );
		temp += (dO[treO]*( 1.0 - interpolO ));
		temp += ((dO[twoO]));
		dO[treO] = (temp*tankfeedback);
		dO[treO] += inputSampleL;
		oneO--; if (oneO < 0 || oneO > delayO) {oneO = delayO;}
		twoO--; if (twoO < 0 || twoO > delayO) {twoO = delayO;}
		treO--; if (treO < 0 || treO > delayO) {treO = delayO;}
		temp = (dO[oneO]*interpolO );
		temp += (dO[treO]*( 1.0 - interpolO ));
		temp *=  (invlean + (lean*fabs(dO[twoO])));
  		verboutL += temp;
		//comb filter O
		temp = (dQ[oneQ]*interpolQ );
		temp += (dQ[treQ]*( 1.0 - interpolQ ));
		temp += ((dQ[twoQ]));
		dQ[treQ] = (temp*tankfeedback);
		dQ[treQ] += inputSampleL;
		oneQ--; if (oneQ < 0 || oneQ > delayQ) {oneQ = delayQ;}
		twoQ--; if (twoQ < 0 || twoQ > delayQ) {twoQ = delayQ;}
		treQ--; if (treQ < 0 || treQ > delayQ) {treQ = delayQ;}
		temp = (dQ[oneQ]*interpolQ );
		temp += (dQ[treQ]*( 1.0 - interpolQ ));
		temp *=  (invlean + (lean*fabs(dQ[twoQ])));
  		verboutL += temp;
		//comb filter Q
		temp = (dS[oneS]*interpolS );
		temp += (dS[treS]*( 1.0 - interpolS ));
		temp += ((dS[twoS]));
		dS[treS] = (temp*tankfeedback);
		dS[treS] += inputSampleL;
		oneS--; if (oneS < 0 || oneS > delayS) {oneS = delayS;}
		twoS--; if (twoS < 0 || twoS > delayS) {twoS = delayS;}
		treS--; if (treS < 0 || treS > delayS) {treS = delayS;}
		temp = (dS[oneS]*interpolS );
		temp += (dS[treS]*( 1.0 - interpolS ));
		temp *=  (invlean + (lean*fabs(dS[twoS])));
   		verboutL += temp;
		//comb filter S
		temp = (dU[oneU]*interpolU );
		temp += (dU[treU]*( 1.0 - interpolU ));
		temp += ((dU[twoU]));
		dU[treU] = (temp*tankfeedback);
		dU[treU] += inputSampleL;
		oneU--; if (oneU < 0 || oneU > delayU) {oneU = delayU;}
		twoU--; if (twoU < 0 || twoU > delayU) {twoU = delayU;}
		treU--; if (treU < 0 || treU > delayU) {treU = delayU;}
		temp = (dU[oneU]*interpolU );
		temp += (dU[treU]*( 1.0 - interpolU ));
		temp *=  (invlean + (lean*fabs(dU[twoU])));
  		verboutL += temp;
		//comb filter U
		temp = (dW[oneW]*interpolW );
		temp += (dW[treW]*( 1.0 - interpolW ));
		temp += ((dW[twoW]));
		dW[treW] = (temp*tankfeedback);
		dW[treW] += inputSampleL;
		oneW--; if (oneW < 0 || oneW > delayW) {oneW = delayW;}
		twoW--; if (twoW < 0 || twoW > delayW) {twoW = delayW;}
		treW--; if (treW < 0 || treW > delayW) {treW = delayW;}
		temp = (dW[oneW]*interpolW );
		temp += (dW[treW]*( 1.0 - interpolW ));
		temp *=  (invlean + (lean*fabs(dW[twoW])));
  		verboutL += temp;
		//comb filter W
		temp = (dY[oneY]*interpolY );
		temp += (dY[treY]*( 1.0 - interpolY ));
		temp += ((dY[twoY]));
		dY[treY] = (temp*tankfeedback);
		dY[treY] += inputSampleL;
		oneY--; if (oneY < 0 || oneY > delayY) {oneY = delayY;}
		twoY--; if (twoY < 0 || twoY > delayY) {twoY = delayY;}
		treY--; if (treY < 0 || treY > delayY) {treY = delayY;}
		temp = (dY[oneY]*interpolY );
		temp += (dY[treY]*( 1.0 - interpolY ));
		temp *=  (invlean + (lean*fabs(dY[twoY])));
  		verboutL += temp;
		//comb filter Y
		//here we do the L delay tank, every other letter A C E G I
		
		temp = (dB[oneB]*interpolB );
		temp += (dB[treB]*( 1.0 - interpolB ));
		temp += ((dB[twoB]));
		dB[treB] = (temp*tankfeedback);
		dB[treB] += inputSampleR;
		oneB--; if (oneB < 0 || oneB > delayB) {oneB = delayB;}
		twoB--; if (twoB < 0 || twoB > delayB) {twoB = delayB;}
		treB--; if (treB < 0 || treB > delayB) {treB = delayB;}
		temp = (dB[oneB]*interpolB );
		temp += (dB[treB]*( 1.0 - interpolB ));
		temp *=  (invlean + (lean*fabs(dB[twoB])));
		verboutR += temp;
		//comb filter B		
		temp = (dD[oneD]*interpolD );
		temp += (dD[treD]*( 1.0 - interpolD ));
		temp += ((dD[twoD]));
		dD[treD] = (temp*tankfeedback);
		dD[treD] += inputSampleR;
		oneD--; if (oneD < 0 || oneD > delayD) {oneD = delayD;}
		twoD--; if (twoD < 0 || twoD > delayD) {twoD = delayD;}
		treD--; if (treD < 0 || treD > delayD) {treD = delayD;}
		temp = (dD[oneD]*interpolD );
		temp += (dD[treD]*( 1.0 - interpolD ));
		temp *=  (invlean + (lean*fabs(dD[twoD])));
   		verboutR += temp;
		//comb filter D
		temp = (dF[oneF]*interpolF );
		temp += (dF[treF]*( 1.0 - interpolF ));
		temp += ((dF[twoF]));
		dF[treF] = (temp*tankfeedback);
		dF[treF] += inputSampleR;
		oneF--; if (oneF < 0 || oneF > delayF) {oneF = delayF;}
		twoF--; if (twoF < 0 || twoF > delayF) {twoF = delayF;}
		treF--; if (treF < 0 || treF > delayF) {treF = delayF;}
		temp = (dF[oneF]*interpolF );
		temp += (dF[treF]*( 1.0 - interpolF ));
		temp *=  (invlean + (lean*fabs(dF[twoF])));
   		verboutR += temp;
		//comb filter F
		temp = (dH[oneH]*interpolH );
		temp += (dH[treH]*( 1.0 - interpolH ));
		temp += ((dH[twoH]));
		dH[treH] = (temp*tankfeedback);
		dH[treH] += inputSampleR;
		oneH--; if (oneH < 0 || oneH > delayH) {oneH = delayH;}
		twoH--; if (twoH < 0 || twoH > delayH) {twoH = delayH;}
		treH--; if (treH < 0 || treH > delayH) {treH = delayH;}
		temp = (dH[oneH]*interpolH );
		temp += (dH[treH]*( 1.0 - interpolH ));
		temp *=  (invlean + (lean*fabs(dH[twoH])));
   		verboutR += temp;
		//comb filter H
		temp = (dJ[oneJ]*interpolJ );
		temp += (dJ[treJ]*( 1.0 - interpolJ ));
		temp += ((dJ[twoJ]));
		dJ[treJ] = (temp*tankfeedback);
		dJ[treJ] += inputSampleR;
		oneJ--; if (oneJ < 0 || oneJ > delayJ) {oneJ = delayJ;}
		twoJ--; if (twoJ < 0 || twoJ > delayJ) {twoJ = delayJ;}
		treJ--; if (treJ < 0 || treJ > delayJ) {treJ = delayJ;}
		temp = (dJ[oneJ]*interpolJ );
		temp += (dJ[treJ]*( 1.0 - interpolJ ));
		temp *=  (invlean + (lean*fabs(dJ[twoJ])));
   		verboutR += temp;
		//comb filter J
		temp = (dL[oneL]*interpolL );
		temp += (dL[treL]*( 1.0 - interpolL ));
		temp += ((dL[twoL]));
		dL[treL] = (temp*tankfeedback);
		dL[treL] += inputSampleR;
		oneL--; if (oneL < 0 || oneL > delayL) {oneL = delayL;}
		twoL--; if (twoL < 0 || twoL > delayL) {twoL = delayL;}
		treL--; if (treL < 0 || treL > delayL) {treL = delayL;}
		temp = (dL[oneL]*interpolL );
		temp += (dL[treL]*( 1.0 - interpolL ));
		temp *=  (invlean + (lean*fabs(dL[twoL])));
   		verboutR += temp;
		//comb filter L
		temp = (dN[oneN]*interpolN );
		temp += (dN[treN]*( 1.0 - interpolN ));
		temp += ((dN[twoN]));
		dN[treN] = (temp*tankfeedback);
		dN[treN] += inputSampleR;
		oneN--; if (oneN < 0 || oneN > delayN) {oneN = delayN;}
		twoN--; if (twoN < 0 || twoN > delayN) {twoN = delayN;}
		treN--; if (treN < 0 || treN > delayN) {treN = delayN;}
		temp = (dN[oneN]*interpolN );
		temp += (dN[treN]*( 1.0 - interpolN ));
		temp *=  (invlean + (lean*fabs(dN[twoN])));
   		verboutR += temp;
		//comb filter N
		temp = (dP[oneP]*interpolP );
		temp += (dP[treP]*( 1.0 - interpolP ));
		temp += ((dP[twoP]));
		dP[treP] = (temp*tankfeedback);
		dP[treP] += inputSampleR;
		oneP--; if (oneP < 0 || oneP > delayP) {oneP = delayP;}
		twoP--; if (twoP < 0 || twoP > delayP) {twoP = delayP;}
		treP--; if (treP < 0 || treP > delayP) {treP = delayP;}
		temp = (dP[oneP]*interpolP );
		temp += (dP[treP]*( 1.0 - interpolP ));
		temp *=  (invlean + (lean*fabs(dP[twoP])));
   		verboutR += temp;
		//comb filter P
		temp = (dR[oneR]*interpolR );
		temp += (dR[treR]*( 1.0 - interpolR ));
		temp += ((dR[twoR]));
		dR[treR] = (temp*tankfeedback);
		dR[treR] += inputSampleR;
		oneR--; if (oneR < 0 || oneR > delayR) {oneR = delayR;}
		twoR--; if (twoR < 0 || twoR > delayR) {twoR = delayR;}
		treR--; if (treR < 0 || treR > delayR) {treR = delayR;}
		temp = (dR[oneR]*interpolR );
		temp += (dR[treR]*( 1.0 - interpolR ));
		temp *=  (invlean + (lean*fabs(dR[twoR])));
   		verboutR += temp;
		//comb filter R
		temp = (dT[oneT]*interpolT );
		temp += (dT[treT]*( 1.0 - interpolT ));
		temp += ((dT[twoT]));
		dT[treT] = (temp*tankfeedback);
		dT[treT] += inputSampleR;
		oneT--; if (oneT < 0 || oneT > delayT) {oneT = delayT;}
		twoT--; if (twoT < 0 || twoT > delayT) {twoT = delayT;}
		treT--; if (treT < 0 || treT > delayT) {treT = delayT;}
		temp = (dT[oneT]*interpolT );
		temp += (dT[treT]*( 1.0 - interpolT ));
		temp *=  (invlean + (lean*fabs(dT[twoT])));
   		verboutR += temp;
		//comb filter T
		temp = (dV[oneV]*interpolV );
		temp += (dV[treV]*( 1.0 - interpolV ));
		temp += ((dV[twoV]));
		dV[treV] = (temp*tankfeedback);
		dV[treV] += inputSampleR;
		oneV--; if (oneV < 0 || oneV > delayV) {oneV = delayV;}
		twoV--; if (twoV < 0 || twoV > delayV) {twoV = delayV;}
		treV--; if (treV < 0 || treV > delayV) {treV = delayV;}
		temp = (dV[oneV]*interpolV );
		temp += (dV[treV]*( 1.0 - interpolV ));
		temp *=  (invlean + (lean*fabs(dV[twoV])));
   		verboutR += temp;
		//comb filter V
		temp = (dX[oneX]*interpolX );
		temp += (dX[treX]*( 1.0 - interpolX ));
		temp += ((dX[twoX]));
		dX[treX] = (temp*tankfeedback);
		dX[treX] += inputSampleR;
		oneX--; if (oneX < 0 || oneX > delayX) {oneX = delayX;}
		twoX--; if (twoX < 0 || twoX > delayX) {twoX = delayX;}
		treX--; if (treX < 0 || treX > delayX) {treX = delayX;}
		temp = (dX[oneX]*interpolX );
		temp += (dX[treX]*( 1.0 - interpolX ));
		temp *=  (invlean + (lean*fabs(dX[twoX])));
   		verboutR += temp;
		//comb filter X
		temp = (dZ[oneZ]*interpolZ );
		temp += (dZ[treZ]*( 1.0 - interpolZ ));
		temp += ((dZ[twoZ]));
		dZ[treZ] = (temp*tankfeedback);
		dZ[treZ] += inputSampleR;
		oneZ--; if (oneZ < 0 || oneZ > delayZ) {oneZ = delayZ;}
		twoZ--; if (twoZ < 0 || twoZ > delayZ) {twoZ = delayZ;}
		treZ--; if (treZ < 0 || treZ > delayZ) {treZ = delayZ;}
		temp = (dZ[oneZ]*interpolZ );
		temp += (dZ[treZ]*( 1.0 - interpolZ ));
		temp *=  (invlean + (lean*fabs(dZ[twoZ])));
   		verboutR += temp;
		//comb filter Z
		//here we do the R delay tank, every other letter B D F H J
		
		verboutL /= 8;
		verboutR /= 8;
		
		iirSampleL = (iirSampleL * (1 - iirAmount)) + (verboutL * iirAmount);
		verboutL = verboutL - iirSampleL;
		
		iirSampleR = (iirSampleR * (1 - iirAmount)) + (verboutR * iirAmount);
		verboutR = verboutR - iirSampleR;
		//we need to highpass the crosscoupling, it's making DC runaway
		
		verboutL *=  (invlean + (lean*fabs(verboutL)));
		verboutR *=  (invlean + (lean*fabs(verboutR)));
		//scale back the verb tank the same way we scaled the combs
		
		inputSampleL = verboutL;
		inputSampleR = verboutR;
		
		//EQ lowpass is after all processing like the compressor that might produce hash
		if (flip)
		{
			lowpassSampleAA = (lowpassSampleAA * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
			inputSampleL = lowpassSampleAA;
			lowpassSampleBA = (lowpassSampleBA * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
			inputSampleL = lowpassSampleBA;
			lowpassSampleCA = (lowpassSampleCA * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
			inputSampleL = lowpassSampleCA;
			lowpassSampleDA = (lowpassSampleDA * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
			inputSampleL = lowpassSampleDA;
			lowpassSampleE = (lowpassSampleE * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
			inputSampleL = lowpassSampleE;
		}
		else
		{
			lowpassSampleAB = (lowpassSampleAB * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
			inputSampleL = lowpassSampleAB;
			lowpassSampleBB = (lowpassSampleBB * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
			inputSampleL = lowpassSampleBB;
			lowpassSampleCB = (lowpassSampleCB * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
			inputSampleL = lowpassSampleCB;
			lowpassSampleDB = (lowpassSampleDB * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
			inputSampleL = lowpassSampleDB;
			lowpassSampleF = (lowpassSampleF * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
			inputSampleL = lowpassSampleF;			
		}
		lowpassSampleG = (lowpassSampleG * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
		inputSampleL = (lowpassSampleG * (1 - iirAmountC)) + (inputSampleL * iirAmountC);
		
		
		if (flip)
		{
			rowpassSampleAA = (rowpassSampleAA * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
			inputSampleR = rowpassSampleAA;
			rowpassSampleBA = (rowpassSampleBA * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
			inputSampleR = rowpassSampleBA;
			rowpassSampleCA = (rowpassSampleCA * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
			inputSampleR = rowpassSampleCA;
			rowpassSampleDA = (rowpassSampleDA * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
			inputSampleR = rowpassSampleDA;
			rowpassSampleE = (rowpassSampleE * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
			inputSampleR = rowpassSampleE;
		}
		else
		{
			rowpassSampleAB = (rowpassSampleAB * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
			inputSampleR = rowpassSampleAB;
			rowpassSampleBB = (rowpassSampleBB * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
			inputSampleR = rowpassSampleBB;
			rowpassSampleCB = (rowpassSampleCB * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
			inputSampleR = rowpassSampleCB;
			rowpassSampleDB = (rowpassSampleDB * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
			inputSampleR = rowpassSampleDB;
			rowpassSampleF = (rowpassSampleF * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
			inputSampleR = rowpassSampleF;			
		}
		rowpassSampleG = (rowpassSampleG * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
		inputSampleR = (rowpassSampleG * (1 - iirAmountC)) + (inputSampleR * iirAmountC);
		
		iirCCSampleL = (iirCCSampleL * (1 - iirAmount)) + (verboutL * iirAmount);
		verboutL = verboutL - iirCCSampleL;
		
		iirCCSampleR = (iirCCSampleR * (1 - iirAmount)) + (verboutR * iirAmount);
		verboutR = verboutR - iirCCSampleR;
		//we need to highpass the crosscoupling, it's making DC runaway
		
		verboutL *=  (invlean + (lean*fabs(verboutL)));
		verboutR *=  (invlean + (lean*fabs(verboutR)));
		//scale back the crosscouple the same way we scaled the combs
		verboutL = (inputSampleL) * outcouple;
		verboutR = (inputSampleR) * outcouple;
		//send it off to the input again
		
		nonlin += fabs(verboutL);
		nonlin += fabs(verboutR);//post highpassing and a lot of processing
		
		drySampleL *= dryness;
		drySampleR *= dryness;
		
		inputSampleL *= wetness;
		inputSampleR *= wetness;
		
		drySampleL += inputSampleL;
		drySampleR += inputSampleR;
		//here we combine the tanks with the dry signal
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		flip = !flip;
		
		*outputL = drySampleL;
		*outputR = drySampleR;
		//here we mix the reverb output with the dry input
		
		inputL += 1;
		inputR += 1;
		outputL += 1;
		outputR += 1;
	}
	return noErr;
}

