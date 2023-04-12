/*
*	File:		Righteous4.cpp
*	
*	Version:	1.0
* 
*	Created:	4/8/18
*	
*	Copyright:  Copyright © 2018 Airwindows, Airwindows uses the MIT license
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
	Righteous4.cpp
	
=============================================================================*/
#include "Righteous4.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Righteous4)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Righteous4::Righteous4
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Righteous4::Righteous4(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_One, kDefaultValue_ParamOne );
	SetParameter(kParam_Two, kDefaultValue_ParamTwo );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Righteous4::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Righteous4::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParam_Two)) //ID must be actual name of parameter identifier, not number
	{
		if (outStrings == NULL) return noErr;
		CFStringRef strings [] =
		{
			kMenuItem_16bit,
			kMenuItem_24bit,
			kMenuItem_32bit,
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
//	Righteous4::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Righteous4::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Decibels;
                outParameterInfo.minValue = -28.0;
                outParameterInfo.maxValue = -4.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamOne;
                break;
			case kParam_Two:
                AUBase::FillInParameterName (outParameterInfo, kParameterTwoName, false);
				outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
				outParameterInfo.minValue = k16bit;
				outParameterInfo.maxValue = k32bit;
                outParameterInfo.defaultValue = kDefaultValue_ParamTwo;
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
//	Righteous4::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Righteous4::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Righteous4::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Righteous4::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Righteous4::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Righteous4::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Righteous4EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Righteous4::Righteous4Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Righteous4::Righteous4Kernel::Reset()
{
	midSampleA = 0.0;
	midSampleB = 0.0;
	midSampleC = 0.0;
	midSampleD = 0.0;
	midSampleE = 0.0;
	midSampleF = 0.0;
	midSampleG = 0.0;
	midSampleH = 0.0;
	midSampleI = 0.0;
	midSampleJ = 0.0;
	midSampleK = 0.0;
	midSampleL = 0.0;
	midSampleM = 0.0;
	midSampleN = 0.0;
	midSampleO = 0.0;
	midSampleP = 0.0;
	midSampleQ = 0.0;
	midSampleR = 0.0;
	midSampleS = 0.0;
	midSampleT = 0.0;
	midSampleU = 0.0;
	midSampleV = 0.0;
	midSampleW = 0.0;
	midSampleX = 0.0;
	midSampleY = 0.0;
	midSampleZ = 0.0;
	
	byn[0] = 1000;
	byn[1] = 301;
	byn[2] = 176;
	byn[3] = 125;
	byn[4] = 97;
	byn[5] = 79;
	byn[6] = 67;
	byn[7] = 58;
	byn[8] = 51;
	byn[9] = 46;
	byn[10] = 1000;
	noiseShaping = 0.0;
	
	lastSample = 0.0;
	IIRsample = 0.0;
	gwPrev = 0.0;
	gwA = 0.0;
	gwB = 0.0;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Righteous4::Righteous4Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Righteous4::Righteous4Kernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	double fpOld = 0.618033988749894848204586; //golden ratio!
	double fpNew = 1.0 - fpOld;
	Float64 overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	Float64 IIRscaleback = 0.0002597;//scaleback of harmonic avg
	IIRscaleback /= overallscale;
	IIRscaleback = 1.0 - IIRscaleback;
		
	Float64 target = GetParameter( kParam_One );
	target += 17; //gives us scaled distortion factor based on test conditions
	target = pow(10.0,target/20.0); //we will multiply and divide by this
	//ShortBuss section
	if (target == 0) target = 1; //insanity check
	
	int bitDepth = (int) GetParameter( kParam_Two ); // +1 for Reaper bug workaround
	
	Float64 fusswithscale = 149940.0; //corrected
	Float64 cutofffreq = 20; //was 46/2.0
	Float64 midAmount = (cutofffreq)/fusswithscale;
	midAmount /= overallscale;
	Float64 midaltAmount = 1.0 - midAmount;
	Float64 gwAfactor = 0.718;
	gwAfactor -= (overallscale*0.05); //0.2 at 176K, 0.1 at 88.2K, 0.05 at 44.1K
	//reduce slightly to not less than 0.5 to increase effect
	Float64 gwBfactor = 1.0 - gwAfactor;
	Float64 softness = 0.2135;
	Float64 hardness = 1.0 - softness;
	Float64 refclip = pow(10.0,-0.0058888);	
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		Float64 drySample = inputSample;
		
		//begin the whole distortion dealiebop
		inputSample /= target;
		
		//running shortbuss on direct sample
		IIRsample *= IIRscaleback;
		Float64 secondharmonic = sin((2 * inputSample * inputSample) * IIRsample);
		//secondharmonic is calculated before IIRsample is updated, to delay reaction
		
		double bridgerectifier = inputSample;
		if (bridgerectifier > 1.2533141373155) bridgerectifier = 1.2533141373155;
		if (bridgerectifier < -1.2533141373155) bridgerectifier = -1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		bridgerectifier = sin(bridgerectifier * fabs(bridgerectifier)) / ((bridgerectifier == 0.0) ?1:fabs(bridgerectifier));
		if (inputSample > bridgerectifier) IIRsample += ((inputSample - bridgerectifier)*0.0009);
		if (inputSample < -bridgerectifier) IIRsample += ((inputSample + bridgerectifier)*0.0009);
		//manipulate IIRSample
		inputSample = bridgerectifier;
		//apply the distortion transform for reals. Has been converted back to -1/1
		
		//apply resonant highpass
		Float64 tempSample = inputSample;
		midSampleA = (midSampleA * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleA; Float64 correction = midSampleA;
		midSampleB = (midSampleB * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleB; correction += midSampleB;
		midSampleC = (midSampleC * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleC; correction += midSampleC;
		midSampleD = (midSampleD * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleD; correction += midSampleD;
		midSampleE = (midSampleE * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleE; correction += midSampleE;
		midSampleF = (midSampleF * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleF; correction += midSampleF;
		midSampleG = (midSampleG * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleG; correction += midSampleG;
		midSampleH = (midSampleH * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleH; correction += midSampleH;
		midSampleI = (midSampleI * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleI; correction += midSampleI;
		midSampleJ = (midSampleJ * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleJ; correction += midSampleJ;
		midSampleK = (midSampleK * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleK; correction += midSampleK;
		midSampleL = (midSampleL * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleL; correction += midSampleL;
		midSampleM = (midSampleM * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleM; correction += midSampleM;
		midSampleN = (midSampleN * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleN; correction += midSampleN;
		midSampleO = (midSampleO * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleO; correction += midSampleO;
		midSampleP = (midSampleP * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleP; correction += midSampleP;
		midSampleQ = (midSampleQ * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleQ; correction += midSampleQ;
		midSampleR = (midSampleR * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleR; correction += midSampleR;
		midSampleS = (midSampleS * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleS; correction += midSampleS;
		midSampleT = (midSampleT * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleT; correction += midSampleT;
		midSampleU = (midSampleU * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleU; correction += midSampleU;
		midSampleV = (midSampleV * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleV; correction += midSampleV;
		midSampleW = (midSampleW * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleW; correction += midSampleW;
		midSampleX = (midSampleX * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleX; correction += midSampleX;
		midSampleY = (midSampleY * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleY; correction += midSampleY;
		midSampleZ = (midSampleZ * midaltAmount) + (tempSample * midAmount); tempSample -= midSampleZ; correction += midSampleZ;
		correction *= fabs(secondharmonic);
		//scale it directly by second harmonic: DC block is now adding harmonics too
		correction -= secondharmonic*fpOld;
		//apply the shortbuss processing to output DCblock by subtracting it 
		//we are not a peak limiter! not using it to clip or nothin'
		//adding it inversely, it's the same as adding to inputsample only we are accumulating 'stuff' in 'correction'
		inputSample -= correction;
		if (inputSample < 0) inputSample = (inputSample * fpNew) - (sin(-inputSample)*fpOld);
		//lastly, class A clipping on the negative to combat the one-sidedness
		//uses bloom/antibloom to dial in previous unconstrained behavior
		//end the whole distortion dealiebop
		inputSample *= target;
		//begin simplified Groove Wear, outside the scaling
		//varies depending on what sample rate you're at:
		//high sample rate makes it more airy
		gwB = gwA; gwA = tempSample = (inputSample-gwPrev);
		tempSample *= gwAfactor;
		tempSample += (gwB * gwBfactor);
		correction = (inputSample-gwPrev) - tempSample;
		gwPrev = inputSample;		
		inputSample -= correction;		
		//simplified Groove Wear.
		
		//begin simplified ADClip
		drySample = inputSample;
		if (lastSample >= refclip)
		{
			if (inputSample < refclip)
			{
				lastSample = ((refclip*hardness) + (inputSample * softness));
			}
			else lastSample = refclip;
		}
		
		if (lastSample <= -refclip)
		{
			if (inputSample > -refclip)
			{
				lastSample = ((-refclip*hardness) + (inputSample * softness));
			}
			else lastSample = -refclip;
		}
		
		if (inputSample > refclip)
		{
			if (lastSample < refclip)
			{
				inputSample = ((refclip*hardness) + (lastSample * softness));
			}
			else inputSample = refclip;
		}
		
		if (inputSample < -refclip)
		{
			if (lastSample > -refclip)
			{
				inputSample = ((-refclip*hardness) + (lastSample * softness));
			}
			else inputSample = -refclip;
		}
		lastSample = drySample;
				
		//output dither section
		if (bitDepth == 3) {
			//begin 32 bit floating point dither
			int expon; frexpf((float)inputSample, &expon);
			fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
			inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
			//end 32 bit floating point dither
		} else {
			//entire Naturalize section used when not on 32 bit out
			
			inputSample -= noiseShaping;
			if (bitDepth == 2) inputSample *= 8388608.0; //go to dither at 24 bit
			if (bitDepth == 1) inputSample *= 32768.0; //go to dither at 16 bit
			
			Float64 benfordize = floor(inputSample);
			while (benfordize >= 1.0) {benfordize /= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			int hotbinA = floor(benfordize);
			//hotbin becomes the Benford bin value for this number floored
			Float64 totalA = 0;
			if ((hotbinA > 0) && (hotbinA < 10))
			{
				byn[hotbinA] += 1;
				totalA += (301-byn[1]);
				totalA += (176-byn[2]);
				totalA += (125-byn[3]);
				totalA += (97-byn[4]);
				totalA += (79-byn[5]);
				totalA += (67-byn[6]);
				totalA += (58-byn[7]);
				totalA += (51-byn[8]);
				totalA += (46-byn[9]);
				byn[hotbinA] -= 1;
			} else {hotbinA = 10;}
			//produce total number- smaller is closer to Benford real
			
			benfordize = ceil(inputSample);
			while (benfordize >= 1.0) {benfordize /= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			if (benfordize < 1.0) {benfordize *= 10;}
			int hotbinB = floor(benfordize);
			//hotbin becomes the Benford bin value for this number ceiled
			Float64 totalB = 0;
			if ((hotbinB > 0) && (hotbinB < 10))
			{
				byn[hotbinB] += 1;
				totalB += (301-byn[1]);
				totalB += (176-byn[2]);
				totalB += (125-byn[3]);
				totalB += (97-byn[4]);
				totalB += (79-byn[5]);
				totalB += (67-byn[6]);
				totalB += (58-byn[7]);
				totalB += (51-byn[8]);
				totalB += (46-byn[9]);
				byn[hotbinB] -= 1;
			} else {hotbinB = 10;}
			//produce total number- smaller is closer to Benford real
			
			if (totalA < totalB)
			{
				byn[hotbinA] += 1;
				inputSample = floor(inputSample);
			}
			else
			{
				byn[hotbinB] += 1;
				inputSample = ceil(inputSample);
			}
			//assign the relevant one to the delay line
			//and floor/ceil signal accordingly
			
			totalA = byn[1] + byn[2] + byn[3] + byn[4] + byn[5] + byn[6] + byn[7] + byn[8] + byn[9];
			totalA /= 1000;
			totalA = 1; // spotted by Laserbat: this 'scaling back' code doesn't. It always divides by the fallback of 1. Old NJAD doesn't scale back the things we're comparing against. Kept to retain known behavior, use the one in StudioTan and Monitoring for a tuned-as-intended NJAD.
			byn[1] /= totalA;
			byn[2] /= totalA;
			byn[3] /= totalA;
			byn[4] /= totalA;
			byn[5] /= totalA;
			byn[6] /= totalA;
			byn[7] /= totalA;
			byn[8] /= totalA;
			byn[9] /= totalA;
			byn[10] /= 2; //catchall for garbage data
			
			if (bitDepth == 2) inputSample /= 8388608.0;
			if (bitDepth == 1) inputSample /= 32768.0;
			noiseShaping += inputSample - drySample;
		}
		
		if (inputSample > refclip) inputSample = refclip;
		if (inputSample < -refclip) inputSample = -refclip;
		//iron bar prohibits any overs
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

