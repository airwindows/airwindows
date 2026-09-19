/*
*	File:		Ultralight2.cpp
*	
*	Version:	1.0
* 
*	Created:	9/13/26
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
	Ultralight2.cpp
	
=============================================================================*/
#include "Ultralight2.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Ultralight2)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight2::Ultralight2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Ultralight2::Ultralight2(AudioUnit component)
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
//	Ultralight2::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight2::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight2::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight2::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.maxValue = 1.0;//Distance output lowpass
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
//	Ultralight2::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight2::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight2::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight2::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Ultralight2::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Ultralight2::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____Ultralight2EffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight2::Ultralight2Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Ultralight2::Ultralight2Kernel::Reset()
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
	hA = hB = hC = hD = hE = 0.0f;
	
	firstDry = firstAvg = 0.0f;
	psA = psB = psC = psD = psE = psF = psG = psH = psI = psJ = psK = psL = psM = 0.0f;
	psN = psO = psP = psQ = psR = psS = psT = psU = psV = psW = psX = psY = psZ = 0.0f;
	psa = psb = psc = psd = pse = psf = psg = psh = psi = psj = psk = psl = psm = 0.0f;
	psn = pso = psp = psq = psr = pss = pst = psu = psv = psw = psx = psy = psz = 0.0f;	
	prevDistance = 0;
	pointCycle = pointAL = pointBL = 0.0f;
	
	fpd = 1.0; while (fpd < 16386) fpd = rand()*UINT32_MAX;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight2::Ultralight2Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Ultralight2::Ultralight2Kernel::Process(	const Float32 	*inSourceP,
                                                    Float32		 	*inDestP,
                                                    UInt32 			inFramesToProcess,
                                                    UInt32			inNumChannels, 
                                                    bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	float overallscale = 1.0f;
	overallscale /= 48000.0f; //we are treating both 44.1k and 48k as '1X'
	overallscale *= GetSampleRate(); //and expecting to get 2X, 4X as needed
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
	//this plugin applies distance filter to dry AND wet, so that when testing
	//out possible spaces for the game engine, you can hear what it'll sound like
	//in practice when both the source sound and the verb are distant.
	float distance = pow(GetParameter( kParam_F )*7.28f,2.0f); //0 to 52.9984
	int distanceSteps = (int)distance; //52 maximum
	distance -= (float)distanceSteps; //0.9984
	//distance is not a property of the reverb node, it's a property of
	//the game audio engine, where every source can be localized
	
	while (nSampleFrames-- > 0) {
		float inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		pointCycle += pointDerez;
		if (pointCycle > 0.9999999f) {
			//this is sample rate handling for the plugin, so that
			//a more primitive reverb node can run at any rate.
			
			float drySample = inputSample;
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
			oQ = fma(oQ,0.5f,aQ[cQ]);
			oR = fma(oR,0.5f,aR[cR]);
			oS = fma(oS,0.5f,aS[cS]);
			oT = fma(oT,0.0f,aT[cT]); //not allpass but can use vector fma()
			
			if (bypassPre) oQ = inputSample; //plugin only
			
			aA[cA] = fma(fA,damping,oQ);
			aB[cB] = fma(fB,damping,oQ);
			aC[cC] = fma(fC,damping,oQ);
			aD[cD] = fma(fD,damping,oQ);
			cA++; cB++; cC++; cD++;
			if (cA > d4A) cA = 0;
			if (cB > d4B) cB = 0;
			if (cC > d4C) cC = 0;
			if (cD > d4D) cD = 0;
			hA = aA[cA]; hB = aB[cB]; hC = aC[cC]; hD = aD[cD];
			hE = -(hA + hB + hC + hD);
			aE[cE] = fma(hA,2.0f,hE);
			aF[cF] = fma(hB,2.0f,hE);
			aG[cG] = fma(hC,2.0f,hE);
			aH[cH] = fma(hD,2.0f,hE);
			cE++; cF++; cG++; cH++;
			if (cE > d4E) cE = 0;
			if (cF > d4F) cF = 0;
			if (cG > d4G) cG = 0;
			if (cH > d4H) cH = 0;
			hA = aE[cE]; hB = aF[cF]; hC = aG[cG]; hD = aH[cH];
			hE = -(hA + hB + hC + hD);
			aI[cI] = fma(hA,2.0f,hE);
			aJ[cJ] = fma(hB,2.0f,hE);
			aK[cK] = fma(hC,2.0f,hE);
			aL[cL] = fma(hD,2.0f,hE);
			cI++; cJ++; cK++; cL++;
			if (cI > d4I) cI = 0;
			if (cJ > d4J) cJ = 0;
			if (cK > d4K) cK = 0;
			if (cL > d4L) cL = 0;
			hA = aI[cI]; hB = aJ[cJ]; hC = aK[cK]; hD = aL[cL];
			hE = -(hA + hB + hC + hD);
			aM[cM] = fma(hA,2.0f,hE);
			aN[cN] = fma(hB,2.0f,hE);
			aO[cO] = fma(hC,2.0f,hE);
			aP[cP] = fma(hD,2.0f,hE);
			cM++; cN++; cO++; cP++;
			if (cM > d4M) cM = 0;
			if (cN > d4N) cN = 0;
			if (cO > d4O) cO = 0;
			if (cP > d4P) cP = 0;
			hA = aM[cM]; hB = aN[cN]; hC = aO[cO]; hD = aP[cP];
			hE = -(hA + hB + hC + hD);
			fA = fma(hA,2.0f,hE);				
			fB = fma(hB,2.0f,hE);
			fC = fma(hC,2.0f,hE);
			fD = fma(hD,2.0f,hE);
			switch (unSolid)
			{ //in engine, this'll be a const and the compiler might optimize it away
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
			{ //in engine, this'll be a const and the compiler might optimize it away
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
			inputSample = (hA + hB + hC + hD)*0.0625f;
			//end of just the reverb node part
			
			if (bypassVerb) inputSample = oQ; //plugin only
			//dry/wet is not part of the game engine reverb node
			inputSample = (inputSample * wet)+(drySample * (1.0f-wet));
			//plugin only: game engine one would have just the reverb,
			//and every sound source could have the following Distance filter,
			//including the localized reverb nodes. The reason it's here
			//is so the plugin can preview how dry+reverb, both distant, will sound
			
			//begin just the distance filter part
			firstDry = inputSample; //start by doing the interpolation
			inputSample += firstAvg; inputSample *= 0.5f; firstAvg = inputSample;
			inputSample = (firstDry*(1.0f-distance)) + (inputSample*distance);
			//having done at least one interpolation we can now do the integer number of stages
			if (prevDistance < distanceSteps) {
				switch (prevDistance)
				{
					case  0: psA = inputSample; //if we've changed the setting
					case  1: psB = inputSample; //reset the ones being put back into play
					case  2: psC = inputSample; //this won't be happening constantly,
					case  3: psD = inputSample; //but since we don't have an array
					case  4: psE = inputSample; //this is the structure by which we
					case  5: psF = inputSample; //get the new averagings going
					case  6: psG = inputSample; //without pops or clicks
					case  7: psH = inputSample;
					case  8: psI = inputSample;
					case  9: psJ = inputSample;
					case 10: psK = inputSample;
					case 11: psL = inputSample;
					case 12: psM = inputSample;
					case 13: psN = inputSample;
					case 14: psO = inputSample;
					case 15: psP = inputSample;
					case 16: psQ = inputSample;
					case 17: psR = inputSample;
					case 18: psS = inputSample;
					case 19: psT = inputSample;
					case 20: psU = inputSample;
					case 21: psV = inputSample;
					case 22: psW = inputSample;
					case 23: psX = inputSample;
					case 24: psY = inputSample;
					case 25: psZ = inputSample;
					case 26: psa = inputSample;
					case 27: psb = inputSample;
					case 28: psc = inputSample;
					case 29: psd = inputSample;
					case 30: pse = inputSample;
					case 31: psf = inputSample;
					case 32: psg = inputSample;
					case 33: psh = inputSample;
					case 34: psi = inputSample;
					case 35: psj = inputSample;
					case 36: psk = inputSample;
					case 37: psl = inputSample;
					case 38: psm = inputSample;
					case 39: psn = inputSample;
					case 40: pso = inputSample;
					case 41: psp = inputSample;
					case 42: psq = inputSample;
					case 43: psr = inputSample;
					case 44: pss = inputSample;
					case 45: pst = inputSample;
					case 46: psu = inputSample;
					case 47: psv = inputSample;
					case 48: psw = inputSample;
					case 49: psx = inputSample;
					case 50: psy = inputSample;
					case 51: psz = inputSample;
					case 52: prevDistance = distanceSteps;
				}
			}
			switch (52-distanceSteps)
			{ //apply the stack of filter steps to produce the distance filter
				case  0: inputSample += psz; inputSample *= 0.5f; psz = inputSample;
				case  1: inputSample += psy; inputSample *= 0.5f; psy = inputSample;
				case  2: inputSample += psx; inputSample *= 0.5f; psx = inputSample;
				case  3: inputSample += psw; inputSample *= 0.5f; psw = inputSample;
				case  4: inputSample += psv; inputSample *= 0.5f; psv = inputSample;
				case  5: inputSample += psu; inputSample *= 0.5f; psu = inputSample;
				case  6: inputSample += pst; inputSample *= 0.5f; pst = inputSample;
				case  7: inputSample += pss; inputSample *= 0.5f; pss = inputSample;
				case  8: inputSample += psr; inputSample *= 0.5f; psr = inputSample;
				case  9: inputSample += psq; inputSample *= 0.5f; psq = inputSample;
				case 10: inputSample += psp; inputSample *= 0.5f; psp = inputSample;
				case 11: inputSample += pso; inputSample *= 0.5f; pso = inputSample;
				case 12: inputSample += psn; inputSample *= 0.5f; psn = inputSample;
				case 13: inputSample += psm; inputSample *= 0.5f; psm = inputSample;
				case 14: inputSample += psl; inputSample *= 0.5f; psl = inputSample;
				case 15: inputSample += psk; inputSample *= 0.5f; psk = inputSample;
				case 16: inputSample += psj; inputSample *= 0.5f; psj = inputSample;
				case 17: inputSample += psi; inputSample *= 0.5f; psi = inputSample;
				case 18: inputSample += psh; inputSample *= 0.5f; psh = inputSample;
				case 19: inputSample += psg; inputSample *= 0.5f; psg = inputSample;
				case 20: inputSample += psf; inputSample *= 0.5f; psf = inputSample;
				case 21: inputSample += pse; inputSample *= 0.5f; pse = inputSample;
				case 22: inputSample += psd; inputSample *= 0.5f; psd = inputSample;
				case 23: inputSample += psc; inputSample *= 0.5f; psc = inputSample;
				case 24: inputSample += psb; inputSample *= 0.5f; psb = inputSample;
				case 25: inputSample += psa; inputSample *= 0.5f; psa = inputSample;
				case 26: inputSample += psZ; inputSample *= 0.5f; psZ = inputSample;
				case 27: inputSample += psY; inputSample *= 0.5f; psY = inputSample;
				case 28: inputSample += psX; inputSample *= 0.5f; psX = inputSample;
				case 29: inputSample += psW; inputSample *= 0.5f; psW = inputSample;
				case 30: inputSample += psV; inputSample *= 0.5f; psV = inputSample;
				case 31: inputSample += psU; inputSample *= 0.5f; psU = inputSample;
				case 32: inputSample += psT; inputSample *= 0.5f; psT = inputSample;
				case 33: inputSample += psS; inputSample *= 0.5f; psS = inputSample;
				case 34: inputSample += psR; inputSample *= 0.5f; psR = inputSample;
				case 35: inputSample += psQ; inputSample *= 0.5f; psQ = inputSample;
				case 36: inputSample += psP; inputSample *= 0.5f; psP = inputSample;
				case 37: inputSample += psO; inputSample *= 0.5f; psO = inputSample;
				case 38: inputSample += psN; inputSample *= 0.5f; psN = inputSample;
				case 39: inputSample += psM; inputSample *= 0.5f; psM = inputSample;
				case 40: inputSample += psL; inputSample *= 0.5f; psL = inputSample;
				case 41: inputSample += psK; inputSample *= 0.5f; psK = inputSample;
				case 42: inputSample += psJ; inputSample *= 0.5f; psJ = inputSample;
				case 43: inputSample += psI; inputSample *= 0.5f; psI = inputSample;
				case 44: inputSample += psH; inputSample *= 0.5f; psH = inputSample;
				case 45: inputSample += psG; inputSample *= 0.5f; psG = inputSample;
				case 46: inputSample += psF; inputSample *= 0.5f; psF = inputSample;
				case 47: inputSample += psE; inputSample *= 0.5f; psE = inputSample;
				case 48: inputSample += psD; inputSample *= 0.5f; psD = inputSample;
				case 49: inputSample += psC; inputSample *= 0.5f; psC = inputSample;
				case 50: inputSample += psB; inputSample *= 0.5f; psB = inputSample;
				case 51: inputSample += psA; inputSample *= 0.5f; psA = inputSample;
				case 52: break;
			}
			//end distance filter
			
			pointAL = pointBL;
			pointBL = inputSample;
			pointCycle = 0.0f; //interpolation point values
			//this is the sample rate adjustment for the plugin version
			//game engine may well always run at 44.1k or whatever			
		}
		inputSample = pointAL+((pointBL-pointAL)*pointCycle);
		//this is the linear interpolation between reverb points
		//governed by sample rate, to work as a plugin
		
		//begin 32 bit floating point dither
		//int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		//inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

