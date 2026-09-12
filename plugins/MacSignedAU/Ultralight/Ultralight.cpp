/*
 *	File:		Ultralight.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	8/26/26
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
 Ultralight.cpp
 
 =============================================================================*/
#include "Ultralight.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, Ultralight)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight::Ultralight
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Ultralight::Ultralight(AudioUnit component)
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
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight::GetParameterValueStrings(AudioUnitScope		inScope,
															 AudioUnitParameterID	inParameterID,
															 CFArrayRef *		outStrings)
{
	
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.maxValue = 9;//RT60
                outParameterInfo.defaultValue = kDefaultValue_ParamA;
                break;
            case kParam_B:
                AUBase::FillInParameterName (outParameterInfo, kParameterBName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 4;//All/Comb from 0-4
                outParameterInfo.defaultValue = kDefaultValue_ParamB;
                break;
            case kParam_C:
                AUBase::FillInParameterName (outParameterInfo, kParameterCName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 4;//WallDns highpass 0-4
                outParameterInfo.defaultValue = kDefaultValue_ParamC;
                break;
            case kParam_D:
                AUBase::FillInParameterName (outParameterInfo, kParameterDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = 0;
                outParameterInfo.maxValue = 4;//Reflect lowpass 0-4
                outParameterInfo.defaultValue = kDefaultValue_ParamD;
                break;
            case kParam_E:
                AUBase::FillInParameterName (outParameterInfo, kParameterEName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.flags |= kAudioUnitParameterFlag_DisplayLogarithmic;
				outParameterInfo.minValue = 1; //can't display log from zero
                outParameterInfo.maxValue = 4096;//predelay
                outParameterInfo.defaultValue = kDefaultValue_ParamE;
                break;
            case kParam_F:
                AUBase::FillInParameterName (outParameterInfo, kParameterFName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;//dry/wet
                outParameterInfo.defaultValue = kDefaultValue_ParamF;
                break;
            case kParam_G:
                AUBase::FillInParameterName (outParameterInfo, kParameterGName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;//Distance output lowpass
                outParameterInfo.defaultValue = kDefaultValue_ParamG;
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
//	Ultralight::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight::GetPropertyInfo (AudioUnitPropertyID	inID,
													 AudioUnitScope		inScope,
													 AudioUnitElement	inElement,
													 UInt32 &		outDataSize,
													 Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Ultralight::GetProperty(	AudioUnitPropertyID inID,
												AudioUnitScope 		inScope,
												AudioUnitElement 	inElement,
												void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Ultralight::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Ultralight::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____UltralightEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Ultralight::UltralightKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Ultralight::UltralightKernel::Reset()
{
	for(int x = 0; x < predelay+2; x++) aZ[x] = 0.0f;
	cZ = 1;
	
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
	cA = cB = cC = cD = cE = cF = cG = cH = 1;
	cI = cJ = cK = cL = cM = cN = cO = cP = 1;
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
//	Ultralight::UltralightKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		Ultralight::UltralightKernel::Process(	const Float32 	*inSourceP,
												  Float32		 	*inDestP,
												  UInt32 			inFramesToProcess,
												  UInt32			inNumChannels, 
												  bool			&ioSilence )
{
	UInt32 nSampleFrames = inFramesToProcess;
	const Float32 *sourceP = inSourceP;
	Float32 *destP = inDestP;
	double overallscale = 1.0;
	overallscale /= 48000.0; //we are treating both 44.1k and 48k as '1X'
	overallscale *= GetSampleRate(); //and expecting to get 2X, 4X as needed
	float pointDerez = 1.0f / overallscale; //forcing it to be simple
	
	//these become presets and are built into the reverb when used
	float rtSixty = 0.0625f-(GetParameter( kParam_A )*0.003125f);
	int furnish = 4-GetParameter( kParam_B ); //0 through 4 are valid
	int wallDns = GetParameter( kParam_C ); //0 through 4 are valid
	int reflect = GetParameter( kParam_D ); //0 through 4 are valid
	int preDlay = GetParameter( kParam_E ); //0 through 4096 are valid
	//making five additional const ints adjusting the node in question
		
	double wet = GetParameter( kParam_F );
	float distance = pow(GetParameter( kParam_G )*7.28f,2.0f); //0 to 52.9984
	int distanceSteps = (int)distance; //52 maximum
	distance -= (float)distanceSteps; //0.9984
	
	while (nSampleFrames-- > 0) {
		double inputSample = *sourceP;
		if (fabs(inputSample)<1.18e-23) inputSample = fpd * 1.18e-17;
		
		pointCycle += pointDerez;
		if (pointCycle > 0.9999999) {
			//this is sample rate handling for the plugin, so that
			//a more primitive reverb node can run at any rate.
			
			double drySample = inputSample;
			//predelay for plugin: not suitable for game engine,
			//as it doesn't handle changing delays gracefully.
			//However, it might be worth designing a FIXED predelay for
			//each verb node, on the grounds that if you're right next to it,
			//you'll hear maximum delay before the sound bounces off the first wall.
			//As such it'd always be the same delay, and so it'd not pose a problem,
			//even for sound events that are moving around in the verb area.
			//it'd also mean you could set zero for other purposes.
			aZ[cZ] = inputSample;
			cZ++; if (cZ < 0 || cZ > preDlay) cZ = 0;
			inputSample = aZ[cZ-((cZ > preDlay)?preDlay+1:0)];
			//end predelay
			
			//begin just what's in reverb node
			
			if (furnish < 4) {
				hA = inputSample - (aA[(cA+1)-((cA+1 > d4A)?d4A+1:0)]*0.5);
				hA += (fA * rtSixty); aA[cA] = hA; hA *= 0.5;
				cA++;
				if (cA > d4A) cA = 0;
				hA += aA[cA];
			} else {
				aA[cA] = inputSample + (fA * rtSixty);
				cA++;
				if (cA > d4A) cA = 0;
				hA = aA[cA];
			}
			if (furnish < 3) {
				hB = inputSample - (aB[(cB+1)-((cB+1 > d4B)?d4B+1:0)]*0.5);
				hB += (fB * rtSixty); aB[cB] = hB; hB *= 0.5;
				cB++;
				if (cB > d4B) cB = 0;
				hB += aB[cB];
			} else {
				aB[cB] = inputSample + (fB * rtSixty);
				cB++;
				if (cB > d4B) cB = 0;
				hB = aB[cB];
			}
			if (furnish < 2) {
				hC = inputSample - (aC[(cC+1)-((cC+1 > d4C)?d4C+1:0)]*0.5);
				hC += (fC * rtSixty); aC[cC] = hC; hC *= 0.5;
				cC++;
				if (cC > d4C) cC = 0;
				hC += aC[cC];
			} else {
				aC[cC] = inputSample + (fC * rtSixty);
				cC++;
				if (cC > d4C) cC = 0;
				hC = aC[cC];
			}
			if (furnish < 1) {
				hD = inputSample - (aD[(cD+1)-((cD+1 > d4D)?d4D+1:0)]*0.5);
				hD += (fD * rtSixty); aD[cD] = hD; hD *= 0.5;
				cD++;
				if (cD > d4D) cD = 0;
				hD += aD[cD];
			} else {
				aD[cD] = inputSample + (fD * rtSixty);
				cD++;
				if (cD > d4D) cD = 0;
				hD = aD[cD];
			}
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
			switch (reflect)
			{
				case 0:
					fA = (fA+flA)*0.5f; flA = fA;
				case 1:
					fB = (fB+flB)*0.5f; flB = fB;
				case 2:
					fC = (fC+flC)*0.5f; flC = fC;
				case 3:
					fD = (fD+flC)*0.5f; flD = fD;
				case 4:
					break; //This applies ALL processing
					//after the starting point, meaning the
					//cases intentionally fall through
			}
			switch (wallDns)
			{
				case 0:
					fhA *= 0.5f; fA -= fhA*0.0625f; fhA += fA;
				case 1:
					fhB *= 0.5f; fB -= fhB*0.0625f; fhB += fB;
				case 2:
					fhC *= 0.5f; fC -= fhC*0.0625f; fhC += fC;
				case 3:
					fhD *= 0.5f; fD -= fhD*0.0625f; fhD += fD;
				case 4:
					break; //This applies ALL processing
					//after the starting point, meaning the
					//cases intentionally fall through
			}
			inputSample = (hA + hB + hC + hD)*0.0625;
			//end of just the reverb node part
			
			//dry/wet is not part of the game engine reverb node
			inputSample = (inputSample * wet)+(drySample * (1.0-wet));
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
				case  0: inputSample += psz; inputSample *= 0.5; psz = inputSample;
				case  1: inputSample += psy; inputSample *= 0.5; psy = inputSample;
				case  2: inputSample += psx; inputSample *= 0.5; psx = inputSample;
				case  3: inputSample += psw; inputSample *= 0.5; psw = inputSample;
				case  4: inputSample += psv; inputSample *= 0.5; psv = inputSample;
				case  5: inputSample += psu; inputSample *= 0.5; psu = inputSample;
				case  6: inputSample += pst; inputSample *= 0.5; pst = inputSample;
				case  7: inputSample += pss; inputSample *= 0.5; pss = inputSample;
				case  8: inputSample += psr; inputSample *= 0.5; psr = inputSample;
				case  9: inputSample += psq; inputSample *= 0.5; psq = inputSample;
				case 10: inputSample += psp; inputSample *= 0.5; psp = inputSample;
				case 11: inputSample += pso; inputSample *= 0.5; pso = inputSample;
				case 12: inputSample += psn; inputSample *= 0.5; psn = inputSample;
				case 13: inputSample += psm; inputSample *= 0.5; psm = inputSample;
				case 14: inputSample += psl; inputSample *= 0.5; psl = inputSample;
				case 15: inputSample += psk; inputSample *= 0.5; psk = inputSample;
				case 16: inputSample += psj; inputSample *= 0.5; psj = inputSample;
				case 17: inputSample += psi; inputSample *= 0.5; psi = inputSample;
				case 18: inputSample += psh; inputSample *= 0.5; psh = inputSample;
				case 19: inputSample += psg; inputSample *= 0.5; psg = inputSample;
				case 20: inputSample += psf; inputSample *= 0.5; psf = inputSample;
				case 21: inputSample += pse; inputSample *= 0.5; pse = inputSample;
				case 22: inputSample += psd; inputSample *= 0.5; psd = inputSample;
				case 23: inputSample += psc; inputSample *= 0.5; psc = inputSample;
				case 24: inputSample += psb; inputSample *= 0.5; psb = inputSample;
				case 25: inputSample += psa; inputSample *= 0.5; psa = inputSample;
				case 26: inputSample += psZ; inputSample *= 0.5; psZ = inputSample;
				case 27: inputSample += psY; inputSample *= 0.5; psY = inputSample;
				case 28: inputSample += psX; inputSample *= 0.5; psX = inputSample;
				case 29: inputSample += psW; inputSample *= 0.5; psW = inputSample;
				case 30: inputSample += psV; inputSample *= 0.5; psV = inputSample;
				case 31: inputSample += psU; inputSample *= 0.5; psU = inputSample;
				case 32: inputSample += psT; inputSample *= 0.5; psT = inputSample;
				case 33: inputSample += psS; inputSample *= 0.5; psS = inputSample;
				case 34: inputSample += psR; inputSample *= 0.5; psR = inputSample;
				case 35: inputSample += psQ; inputSample *= 0.5; psQ = inputSample;
				case 36: inputSample += psP; inputSample *= 0.5; psP = inputSample;
				case 37: inputSample += psO; inputSample *= 0.5; psO = inputSample;
				case 38: inputSample += psN; inputSample *= 0.5; psN = inputSample;
				case 39: inputSample += psM; inputSample *= 0.5; psM = inputSample;
				case 40: inputSample += psL; inputSample *= 0.5; psL = inputSample;
				case 41: inputSample += psK; inputSample *= 0.5; psK = inputSample;
				case 42: inputSample += psJ; inputSample *= 0.5; psJ = inputSample;
				case 43: inputSample += psI; inputSample *= 0.5; psI = inputSample;
				case 44: inputSample += psH; inputSample *= 0.5; psH = inputSample;
				case 45: inputSample += psG; inputSample *= 0.5; psG = inputSample;
				case 46: inputSample += psF; inputSample *= 0.5; psF = inputSample;
				case 47: inputSample += psE; inputSample *= 0.5; psE = inputSample;
				case 48: inputSample += psD; inputSample *= 0.5; psD = inputSample;
				case 49: inputSample += psC; inputSample *= 0.5; psC = inputSample;
				case 50: inputSample += psB; inputSample *= 0.5; psB = inputSample;
				case 51: inputSample += psA; inputSample *= 0.5; psA = inputSample;
				case 52: break;
			}
			//end distance filter
			
			pointAL = pointBL;
			pointBL = inputSample;
			pointCycle = 0.0; //interpolation point values
			//this is the sample rate adjustment for the plugin version
			//game engine may well always run at 44.1k or whatever			
		}
		inputSample = pointAL+((pointBL-pointAL)*pointCycle);
		//this is the linear interpolation between reverb points
		//governed by sample rate, to work as a plugin
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)inputSample, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*destP = inputSample;
		
		sourceP += inNumChannels; destP += inNumChannels;
	}
}

