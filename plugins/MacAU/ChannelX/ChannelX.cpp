/*
 *	File:		ChannelX.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	7/15/26
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
 ChannelX.cpp
 
 =============================================================================*/
#include "ChannelX.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(ChannelX)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ChannelX::ChannelX
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ChannelX::ChannelX(AudioUnit component)
: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
	SetParameter(kParam_Three, kDefaultValue_ParamThree );
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ChannelX::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ChannelX::GetParameterValueStrings(AudioUnitScope		inScope,
														   AudioUnitParameterID	inParameterID,
														   CFArrayRef *		outStrings)
{
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_One)) //ID must be actual name of parameter identifier, not number
	{
		if (outStrings == NULL) return noErr;
		CFStringRef strings [] =
		{
			kMenuItem_Neve,
			kMenuItem_API,
			kMenuItem_SSL,
			kMenuItem_Teac,
			kMenuItem_Mackie,
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
//	ChannelX::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ChannelX::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.minValue = kNeve;
                outParameterInfo.maxValue = kMackie;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
            case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 200.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
                break;
            case kParam_Three:
                AUBase::FillInParameterName (outParameterInfo, kParameterThreeName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamThree;
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
//	ChannelX::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ChannelX::GetPropertyInfo (AudioUnitPropertyID	inID,
												   AudioUnitScope		inScope,
												   AudioUnitElement	inElement,
												   UInt32 &		outDataSize,
												   Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ChannelX::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			ChannelX::GetProperty(	AudioUnitPropertyID inID,
											  AudioUnitScope 		inScope,
											  AudioUnitElement 	inElement,
											  void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	ChannelX::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ChannelX::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____ChannelXEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ChannelX::ChannelXKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ChannelX::ChannelXKernel::Reset()
{
	lastSampleA = lastSampleB = lastSampleC = 0.0;
	rbSampleLA = rbSampleLB = 0.0;
	flip = false;
	for (int x = 0; x < bip_total; x++) bip[x] = 0.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ChannelX::ChannelXKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		ChannelX::ChannelXKernel::Process(	const Float32 	*inSourceP,
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
	int console = (int) GetParameter( kParam_One );
	double density = GetParameter( kParam_Two )/100.0; //0-2
	double phattity = density - 1.0;
	if (density > 1.0) density = 1.0; //max out at full wet for Spiral aspect
	if (phattity < 0.0) phattity = 0.0; //
	double nonLin = 5.0-density; //number is smaller for more intense, larger for more subtle
	double output = GetParameter( kParam_Three );
	double rbAmount = 0.005832;
	double threshold = 0.33362176;
	switch (console)
	{
		case 1: rbAmount = 0.005832; threshold = 0.33362176; break; //Neve
		case 2: rbAmount = 0.004096; threshold = 0.59969536; break; //API
		case 3: rbAmount = 0.004913; threshold = 0.84934656; break; //SSL
		case 4: rbAmount = 0.009216; threshold = 0.149; break; //Teac
		case 5: rbAmount = 0.011449; threshold = 0.092; break; //Mackie
	}
	rbAmount /= overallscale; //we've learned not to try and adjust threshold for sample rate
	
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *sourceP;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpd * 1.18e-17;
		
		double dielectricScale = fabs(2.0-((inputSampleL+nonLin)/nonLin));
		if (flip) {
			rbSampleLA = (rbSampleLA * (1.0-(rbAmount*dielectricScale)))+(inputSampleL*rbAmount*dielectricScale);
			if (fabs(rbSampleLA)>1.18e-37) {
				double bip_delta = inputSampleL; //delta can be just local and re-used
				inputSampleL *= rbSampleLA*0.96;
				inputSampleL = ((inputSampleL*inputSampleL) * -0.166666666666666)*(inputSampleL*inputSampleL) + inputSampleL;
				inputSampleL /= rbSampleLA*0.96;
				bip[bip_drbLA] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
				bip[bip_drbLB] = bip[bip_praLA]-bip[bip_drbLA]; bip[bip_praLA] = bip[bip_drbLA];//velocity
				bip[bip_drbLC] = bip[bip_praLB]-bip[bip_drbLB]; bip[bip_praLB] = bip[bip_drbLB];//acceleration
				bip[bip_drbLD] = bip[bip_praLC]-bip[bip_drbLC]; bip[bip_praLC] = bip[bip_drbLC];//jerk
				double bip_drbE = bip[bip_praLD]-bip[bip_drbLD]; bip[bip_praLD] = bip[bip_drbLD];//snap
				inputSampleL *= (1.0+(fabs(bip[bip_drbLC])*0.0618)+(fabs(bip[bip_drbLD])*-0.05982)+(fabs(bip_drbE)*0.0206));
			}
			inputSampleL -= rbSampleLA*0.92;
		} else {
			rbSampleLB = (rbSampleLB * (1.0-(rbAmount*dielectricScale)))+(inputSampleL*rbAmount*dielectricScale);
			if (fabs(rbSampleLB)>1.18e-37) {
				double bip_delta = inputSampleL; //delta can be just local and re-used
				inputSampleL *= rbSampleLB*0.96;
				inputSampleL = ((inputSampleL*inputSampleL) * -0.166666666666666)*(inputSampleL*inputSampleL) + inputSampleL;
				inputSampleL /= rbSampleLB*0.96;
				bip[bip_drbLA] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
				bip[bip_drbLB] = bip[bip_prbLA]-bip[bip_drbLA]; bip[bip_prbLA] = bip[bip_drbLA];//velocity
				bip[bip_drbLC] = bip[bip_prbLB]-bip[bip_drbLB]; bip[bip_prbLB] = bip[bip_drbLB];//acceleration
				bip[bip_drbLD] = bip[bip_prbLC]-bip[bip_drbLC]; bip[bip_prbLC] = bip[bip_drbLC];//jerk
				double bip_drbE = bip[bip_prbLD]-bip[bip_drbLD]; bip[bip_prbLD] = bip[bip_drbLD];//snap
				inputSampleL *= (1.0+(fabs(bip[bip_drbLC])*0.0618)+(fabs(bip[bip_drbLD])*-0.05982)+(fabs(bip_drbE)*0.0206));
			}
			inputSampleL -= rbSampleLB*0.92;
		}
		//highpass section including capacitor modeling nonlinearity
		
		double bip_delta = inputSampleL; //delta can be just local and re-used
		double drySampleL = inputSampleL;
		inputSampleL = fmax(fmin(inputSampleL,1.0),-1.0);
		long double phatSample = inputSampleL * 1.57079633; //phatSample is sin()
		long double X = phatSample; X *= X; //long double for even
		long double temp = phatSample * X; //the initial multiplies
		phatSample -= temp*0.16666666666666666666666666666666666; temp *= X;
		phatSample += temp*0.00833333333333333333333333333333333; temp *= X;
		phatSample -= temp*0.00019841269841269841269841269841269; temp *= X;
		phatSample += temp*0.00000275573192239858906525573192239; temp *= X;
		phatSample -= temp*0.00000002505210838544171877521083854; temp *= X;
		phatSample += temp*0.00000000016059043836821614599392377; temp *= X;
		phatSample -= temp*0.00000000000076471637318198164759011; temp *= X;
		phatSample += temp*0.00000000000000281145725434552076319; temp *= X;
		phatSample -= temp*0.00000000000000000822063524662432971; temp *= X;
		phatSample += temp*0.00000000000000000001957294106339126;
		//retain mantissa of a long double increasing power function
		//long double probably doesn't handle more than 36 digits or so
		
		inputSampleL *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output, or 1.57079633 for pure sine 'phat' version
		long double distSample = inputSampleL * fabs(inputSampleL); //distSample is Spiral
		X = distSample; X *= X; //long double for even
		temp = distSample * X; //the initial multiplies
		distSample -= temp*0.16666666666666666666666666666666666; temp *= X;
		distSample += temp*0.00833333333333333333333333333333333; temp *= X;
		distSample -= temp*0.00019841269841269841269841269841269; temp *= X;
		distSample += temp*0.00000275573192239858906525573192239; temp *= X;
		distSample -= temp*0.00000002505210838544171877521083854; temp *= X;
		distSample += temp*0.00000000016059043836821614599392377; temp *= X;
		distSample -= temp*0.00000000000076471637318198164759011; temp *= X;
		distSample += temp*0.00000000000000281145725434552076319; temp *= X;
		distSample -= temp*0.00000000000000000822063524662432971; temp *= X;
		distSample += temp*0.00000000000000000001957294106339126;
		//retain mantissa of a long double increasing power function
		//long double probably doesn't handle more than 36 digits or so
		distSample /= ((fabs(inputSampleL) == 0.0) ?1.0:fabs(inputSampleL));
		inputSampleL = distSample; //purest form is full Spiral
		
		if (density < 1.0) inputSampleL = (drySampleL*(1-density))+(distSample*density); //fade Spiral aspect
		if (phattity > 0.0) inputSampleL = (inputSampleL*(1-phattity))+(phatSample*phattity); //apply original Density on top
		
		bip[bip_dvA] = bip_delta - inputSampleL; // these are derivatives: raw clip is position
		bip[bip_dvB] = bip[bip_pvA]-bip[bip_dvA]; bip[bip_pvA] = bip[bip_dvA];//velocity
		bip[bip_dvC] = bip[bip_pvB]-bip[bip_dvB]; bip[bip_pvB] = bip[bip_dvB];//acceleration
		bip[bip_dvD] = bip[bip_pvC]-bip[bip_dvC]; bip[bip_pvC] = bip[bip_dvC];//jerk
		double bip_dvE = bip[bip_pvD]-bip[bip_dvD]; bip[bip_pvD] = bip[bip_dvD];//snap
		inputSampleL *= (1.0+(fabs(bip[bip_dvC])*0.0618)+(fabs(bip[bip_dvD])*-0.05982)+(fabs(bip_dvE)*0.0206));
		
		double clamp = (lastSampleB - lastSampleC) * 0.381966011250105;
		clamp -= (lastSampleA - lastSampleB) * 0.6180339887498948482045;
		clamp += inputSampleL - lastSampleA; //regular slew clamping added
		
		lastSampleC = lastSampleB;
		lastSampleB = lastSampleA;
		lastSampleA = inputSampleL; //now our output relates off lastSampleB
		
		if (clamp > threshold)
			inputSampleL = lastSampleB + threshold;
		if (-clamp > threshold)
			inputSampleL = lastSampleB - threshold;
		//slew section		
		lastSampleA = (lastSampleA*0.381966011250105)+(inputSampleL*0.6180339887498948482045);
		//split the difference between raw and smoothed for buffer
		
		flip = !flip;
		
		if (output < 1.0)
		{
			inputSampleL *= output;
		}
		
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += ((double(fpd)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSampleL;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

