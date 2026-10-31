/*
*	File:		Ultralight3.cpp
*	
*	Version:	1.0
* 
*	Created:	10/12/26
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
	Ultralight3.cpp
	
=============================================================================*/
#include "Ultralight3.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Ultralight3)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight3::Ultralight3
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Ultralight3::Ultralight3(AudioUnit component)
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
	SetParameter(kParam_G, kDefaultValue_ParamG );
	SetParameter(kParam_H, kDefaultValue_ParamH );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight3::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight3::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight3::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight3::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 9;//Damping
                outParameterInfo.defaultValue = kDefaultValue_ParamA;
                break;
            case kParam_B:
                AUBase::FillInParameterName (outParameterInfo, kParameterBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 4;//WallDns highpass 0-4
                outParameterInfo.defaultValue = kDefaultValue_ParamB;
                break;
            case kParam_C:
                AUBase::FillInParameterName (outParameterInfo, kParameterCName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 4;//Reflect lowpass 0-4
                outParameterInfo.defaultValue = kDefaultValue_ParamC;
                break;
            case kParam_D:
                AUBase::FillInParameterName (outParameterInfo, kParameterDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 998;//predelay
                outParameterInfo.defaultValue = kDefaultValue_ParamD;
                break;
            case kParam_E:
                AUBase::FillInParameterName (outParameterInfo, kParameterEName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;//dry/wet
                outParameterInfo.defaultValue = kDefaultValue_ParamE;
                break;
            case kParam_F:
                AUBase::FillInParameterName (outParameterInfo, kParameterFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;//front/back
                outParameterInfo.defaultValue = kDefaultValue_ParamF;
                break;
            case kParam_G:
                AUBase::FillInParameterName (outParameterInfo, kParameterGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;//bottom/top
                outParameterInfo.defaultValue = kDefaultValue_ParamG;
                break;
            case kParam_H:
                AUBase::FillInParameterName (outParameterInfo, kParameterHName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;//Distance output lowpass
                outParameterInfo.defaultValue = kDefaultValue_ParamH;
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
//	Ultralight3::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight3::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 Ultralight3::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	Ultralight3::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight3::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Ultralight3::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Ultralight3::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Ultralight3EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight3::Ultralight3Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Ultralight3::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for(int x = 0; x < d4A+2; x++) aA[x] = 0.0f;
	for(int x = 0; x < d4B+2; x++) aB[x] = 0.0f;
	for(int x = 0; x < d4C+2; x++) aC[x] = 0.0f;
	for(int x = 0; x < d4D+2; x++) aD[x] = 0.0f;
	for(int x = 0; x < d4E+2; x++) aE[x] = 0.0f;
	for(int x = 0; x < d4F+2; x++) aF[x] = 0.0f;
	for(int x = 0; x < d4G+2; x++) aG[x] = 0.0f;
	for(int x = 0; x < d4H+2; x++) aH[x] = 0.0f;
	for(int x = 0; x < d4I+2; x++) aI[x] = 0.0f;
	for(int x = 0; x < d4J+2; x++) aJ[x] = 0.0f;
	for(int x = 0; x < d4K+2; x++) aK[x] = 0.0f;
	for(int x = 0; x < d4L+2; x++) aL[x] = 0.0f;
	for(int x = 0; x < d4M+2; x++) aM[x] = 0.0f;
	for(int x = 0; x < d4N+2; x++) aN[x] = 0.0f;
	for(int x = 0; x < d4O+2; x++) aO[x] = 0.0f;
	for(int x = 0; x < d4P+2; x++) aP[x] = 0.0f;
	for(int x = 0; x < d4Q+2; x++) aQ[x] = 0.0f;
	for(int x = 0; x < d4R+2; x++) aR[x] = 0.0f;
	for(int x = 0; x < d4S+2; x++) aS[x] = 0.0f;
	for(int x = 0; x < d4T+2; x++) aT[x] = 0.0f;
	cA = cB = cC = cD = cE = cF = cG = cH = 1;
	cI = cJ = cK = cL = cM = cN = cO = cP = 1;
	cQ = cR = cS = cT = 1;
	oQ = oR = oS = oT = 0.0f;
	fA = fB = fC = fD = 0.0f;
	fhA = fhB = fhC = fhD = 0.0f;
	flA = flB = flC = flD = 0.0f;
	hA = hB = hC = hD = hE = hF = hG = hH = hI = 0.0f;
	
	fdLA = fdLB = fdLC = fdLD = fdLE = fdLF = fdLG = fdLH = 0.0;
	fdRA = fdRB = fdRC = fdRD = fdRE = fdRF = fdRG = fdRH = 0.0;

	pointCycle = pointAL = pointBL = pointAR = pointBR = 0.0f;	
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight3::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		Ultralight3::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
													const AudioBufferList & inBuffer,
                                                    AudioBufferList & outBuffer,
                                                    UInt32 			inFramesToProcess)
{
	Float32 * inputL = (Float32*)(inBuffer.mBuffers[0].mData);
	Float32 * inputR = (Float32*)(inBuffer.mBuffers[1].mData);
	Float32 * outputL = (Float32*)(outBuffer.mBuffers[0].mData);
	Float32 * outputR = (Float32*)(outBuffer.mBuffers[1].mData);
	UInt32 nSampleFrames = inFramesToProcess;
	double overallscale = 1.0;
	overallscale /= 48000.0;
	overallscale *= GetSampleRate();
	float pointDerez = 1.0f / overallscale; //forcing it to be simple
	
	bool bypassPre = (GetParameter( kParam_D ) < 1); //plugin only
	//the hardcoded verb objects use set matrix delay sizes, then the following four extras:
	float X = GetParameter( kParam_D );
	int dpT = prime[(int)X];//only a delay, not an allpass
	X = sqrt(X*81.408f);
	int dpQ = prime[(int)X];//final allpass
	X = sqrt(X*81.408f);
	int dpR = prime[(int)X];//second allpass
	X = sqrt(X*81.408f);
	int dpS = prime[(int)X];//first allpass
	//this produces four ints that declare arrays like the matrix delays,
	//which replaces the plugin versions set up to scale up to the maximum predelay
	//so, here's how we gnerate const int d4Q-R-S-T with T first, then QRS
	
	//X = the number in Param D; d4T = prime[(int)X];
	//X = sqrt(X*81.408); d4Q = prime[(int)X];
	//X = sqrt(X*81.408); d4R = prime[(int)X];
	//X = sqrt(X*81.408); d4S = prime[(int)X];
	
	bool bypassVerb = (GetParameter( kParam_A ) < 1); //plugin only
	//and then the following three adjustments for RT60 and tone shaping.
	float damping = 0.0625f-((GetParameter( kParam_A ))*0.003125f); //0 bypass, 1-9 valid
	int unSolid = 4-GetParameter( kParam_B ); //0 bypass, 1-4 are valid
	int unReflect = 4-GetParameter( kParam_C ); //0 bypass, 1-4 are valid
	//lastly, the verb object gets three more ints, d4U-V-W, damping-unsolid-unreflect.
	//it must generate its damping value from infinite sustain minus the damping int,
	//then unsolid/unreflect just selects which path to highpass/lowpass, or not.
	
	//damping = 0.0625f-(d4U*0.003125f); //will be 1-9
	//use d4V for unSolid, use d4W for unReflect
	
	float wet = GetParameter( kParam_E ); //plugin only
	float back = GetParameter( kParam_F ); //plugin only
	float ceiling = GetParameter( kParam_G ); //plugin only
	
	//this plugin applies distance filter to dry AND wet, so that when testing
	//out possible spaces for the game engine, you can hear what it'll sound like
	//in practice when both the source sound and the verb are distant.
	float distance = pow(1.0f-GetParameter( kParam_H ),overallscale+1.0f);
	//distance is not a property of the reverb node, it's a property of
	//the game audio engine, where every source can be localized
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		pointCycle += pointDerez;
		if (pointCycle > 0.9999999f) {
			//this is sample rate handling for the plugin, so that
			//a more primitive reverb node can run at any rate.
			
			float drySampleL = inputSampleL;
			float drySampleR = inputSampleR;
			float inputSample = (inputSampleL + inputSampleR) * 0.5f;
			//predelay for plugin: not suitable for game engine,
			//as it doesn't handle changing delays gracefully.
			//However, we are going to run a FIXED predelay for
			//each verb node, on the grounds that if you're right next to it,
			//you'll hear maximum delay before the sound bounces off the first wall.
			//As such it'd always be the same delay, and so it'd not pose a problem,
			//even for sound events that are moving around in the verb area.
			//This is it, combined with three inline allpasses that are tuned
			//independently to the predelay, set up so the compiler can
			//vectorize the lot of them as SIMD operations.
			//Note that some old AMD processors may perform worse here.
			
			oQ = oR - (aQ[(cQ+1)-((cQ+1 > dpQ)?dpQ+1:0)]*0.5f);
			oR = oS - (aR[(cR+1)-((cR+1 > dpR)?dpR+1:0)]*0.5f);
			oS = oT - (aS[(cS+1)-((cS+1 > dpS)?dpS+1:0)]*0.5f);
			oT = inputSample; //SIMDable, but aT[] is just a simple delay
			aQ[cQ] = oQ; aR[cR] = oR; aS[cS] = oS; aT[cT] = oT;
			cQ++; cR++; cS++; cT++;
			if (cQ > dpQ) cQ = 0;
			if (cR > dpR) cR = 0;
			if (cS > dpS) cS = 0;
			if (cT > dpT) cT = 0;
			oQ = oQ*0.5f + aQ[cQ];
			oR = oR*0.5f + aR[cR];
			oS = oS*0.5f + aS[cS];
			oT = oT*0.0f + aT[cT]; //not allpass but can use vector fma
			
			if (bypassPre) oQ = inputSample; //plugin only
			
			aA[cA] = fA*damping + oQ;
			aB[cB] = fB*damping + oQ;
			aC[cC] = fC*damping + oQ;
			aD[cD] = fD*damping + oQ;
			cA++; cB++; cC++; cD++;
			if (cA > d4A) cA = 0;
			if (cB > d4B) cB = 0;
			if (cC > d4C) cC = 0;
			if (cD > d4D) cD = 0;
			hA = aA[cA]; hB = aB[cB]; hC = aC[cC]; hD = aD[cD];
			hI = -(hA + hB + hC + hD);
			aE[cE] = hA*2.0f + hI;
			aF[cF] = hB*2.0f + hI;
			aG[cG] = hC*2.0f + hI;
			aH[cH] = hD*2.0f + hI;
			cE++; cF++; cG++; cH++;
			if (cE > d4E) cE = 0;
			if (cF > d4F) cF = 0;
			if (cG > d4G) cG = 0;
			if (cH > d4H) cH = 0;
			hA = aE[cE]; hB = aF[cF]; hC = aG[cG]; hD = aH[cH];
			hI = -(hA + hB + hC + hD);
			aI[cI] = hA*2.0f + hI;
			aJ[cJ] = hB*2.0f + hI;
			aK[cK] = hC*2.0f + hI;
			aL[cL] = hD*2.0f + hI;
			cI++; cJ++; cK++; cL++;
			if (cI > d4I) cI = 0;
			if (cJ > d4J) cJ = 0;
			if (cK > d4K) cK = 0;
			if (cL > d4L) cL = 0;
			hA = aI[cI]; hB = aJ[cJ]; hC = aK[cK]; hD = aL[cL];
			hI = -(hA + hB + hC + hD); //we have our ground level 4 outputs
			aM[cM] = hA*2.0f + hI; //function of the reverb is unaffected,
			aN[cN] = hB*2.0f + hI; //but on the first pass through, this
			aO[cO] = hC*2.0f + hI; //will be slightly quicker and produce
			aP[cP] = hD*2.0f + hI; //fewer early reflections.
			cM++; cN++; cO++; cP++;
			if (cM > d4M) cM = 0;
			if (cN > d4N) cN = 0;
			if (cO > d4O) cO = 0;
			if (cP > d4P) cP = 0;
			hE = aM[cM]; hF = aN[cN]; hG = aO[cO]; hH = aP[cP];
			hI = -(hE + hF + hG + hH); //we have our overhead 4 outputs
			fA = hE*2.0f + hI; //using these as the added four outputs means
			fB = hF*2.0f + hI; //there's a whole other layer of delays in there
			fC = hG*2.0f + hI; //that are distinct delays which don't match
			fD = hH*2.0f + hI; //the other four. Verb still unaffected.
						
			hA *= 0.25f; hB *= 0.25f; hC *= 0.25f; hD *= 0.25f;
			hE *= 0.125f; hF *= 0.125f; hG *= 0.125f; hH *= 0.125f;
			//hA through hD are now the floor reverb outputs. These are
			//fewer echoes and earlier reflections than the final ones.
			//hE through hH are now the ceiling outputs. These are
			//the final outputs with more reflections, added delays.
			//They all use the full matrix to get to feedback, so it ends
			//up as the same reverb tail no matter where you tap in.
			//The difference in gain is because of level differences from
			//different places in the matrix.
			
			//In stereo, you can monitor either pair, or fold them down
			//to left and right in the plugin. In an actual game engine,
			//these are positioned in a cube, so spatialization is what
			//folds them down to stereo or binaural or whatever you need.
			//end of just the reverb node part
			
			switch (unSolid)
			{
				case 0:
					fhA *= 0.25f; fA -= fhA*0.125f; fhA += fA;
				case 1:
					fhB *= 0.25f; fB -= fhB*0.125f; fhB += fB;
				case 2:
					fhC *= 0.25f; fC -= fhC*0.125f; fhC += fC;
				case 3:
					fhD *= 0.25f; fD -= fhD*0.125f; fhD += fD;
				case 4:
					break;
			}
			switch (unReflect)
			{
				case 0:
					fD = (fD+flD)*0.5f; flD = fD;
				case 1:
					fC = (fC+flC)*0.5f; flC = fC;
				case 2:
					fB = (fB+flB)*0.5f; flB = fB;
				case 3:
					fA = (fA+flA)*0.5f; flA = fA;
				case 4:
					break;
			}
			
			if (bypassVerb) inputSampleL = inputSampleR = oQ; //plugin only
			inputSampleL = (hC * back * (1.0f-ceiling))+(hA * (1.0f-ceiling) * (1.0f-back));
			inputSampleR = (hD * back * (1.0f-ceiling))+(hB * (1.0f-ceiling) * (1.0f-back));
			inputSampleL += (hG * back * ceiling)+(hE * ceiling * (1.0f-back));
			inputSampleR += (hH * back * ceiling)+(hF * ceiling * (1.0f-back));
			
			//dry/wet is not part of the game engine reverb node
			inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0f-wet));
			inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0f-wet));
			//plugin only: game engine one would have just the reverb,
			//and every sound source could have the FastDistance2 filter,
			//including the localized reverb nodes. The reason it's here
			//is so the plugin can preview how dry+reverb, both distant, will sound.
			
			
			pointAL = pointBL;
			pointBL = inputSampleL;
			pointAR = pointBR;
			pointBR = inputSampleR;
			pointCycle = 0.0f; //interpolation point values
			//this is the sample rate adjustment for the plugin version	
		}
		inputSampleL = pointAL+((pointBL-pointAL)*pointCycle);
		inputSampleR = pointAR+((pointBR-pointAR)*pointCycle);
		//this is the linear interpolation between reverb points
		//governed by sample rate, to work as a plugin.
		//For use in a game engine, I suggest running the whole reverb
		//system undersampled in the event of high sample rate, all the nodes and
		//each node's FastDistance2 filter. Even simple linear interpolation will
		//handle this just fine, or the gane engine can run one instance of my
		//bezier undersampling like in my plugin reverbs. In pretty much every
		//case, the game system will NOT be running high sample rate, and it's
		//super wasted on distance-filtered reverbs.
		
		//begin FastDistance2
		inputSampleL *= 0.5f; //because of the ending fdC+fdD, output is
		inputSampleR *= 0.5f; //6dB up from input. In game this can be built
		//into the reverb probe's input as that will have a distance attenuation
		//already as part of its normal operation.
		//for FastDistance2, the alterations from a pure cascading stack
		//of SVFs is that the first one's at a slightly different frequency
		//to more gracefully go from very close to medium distance, and the
		//output is the sum of the last two stages, which causes a subtle roll-off
		//right up at the very top of the frequency range while also altering
		//the shape of the filter to be more like original FastDistance.
		fdLE = distance*0.875f*(inputSampleL-fdLA-fdLE) + fdLE;
		fdLF = distance*(fdLA-fdLB-fdLF) + fdLF;
		fdLG = distance*(fdLB-fdLC-fdLG) + fdLG;
		fdLH = distance*(fdLC-fdLD-fdLH) + fdLH;
		
		fdLA = distance*0.875f*fdLE + fdLA;
		fdLB = distance*fdLF + fdLB;
		fdLC = distance*fdLG + fdLC;
		fdLD = distance*fdLH + fdLD;
		
		inputSampleL = fdLC+fdLD; 

		fdRE = distance*0.875f*(inputSampleR-fdRA-fdRE) + fdRE;
		fdRF = distance*(fdRA-fdRB-fdRF) + fdRF;
		fdRG = distance*(fdRB-fdRC-fdRG) + fdRG;
		fdRH = distance*(fdRC-fdRD-fdRH) + fdRH;
		
		fdRA = distance*0.875f*fdRE + fdRA;
		fdRB = distance*fdRF + fdRB;
		fdRC = distance*fdRG + fdRC;
		fdRD = distance*fdRH + fdRD;
		
		inputSampleR = fdRC+fdRD; 
		//end FastDistance2
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		*outputL = inputSampleL;
		*outputR = inputSampleR;
		//direct stereo out
		
		inputL += 1;
		inputR += 1;
		outputL += 1;
		outputR += 1;
	}
	return noErr;
}

