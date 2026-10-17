/*
*	File:		Echojam.cpp
*	
*	Version:	1.0
* 
*	Created:	9/22/26
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
	Echojam.cpp
	
=============================================================================*/
#include "Echojam.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(Echojam)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Echojam::Echojam
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Echojam::Echojam(AudioUnit component)
	: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
	SetParameter(kParam_D, kDefaultValue_ParamD );
         
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Echojam::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Echojam::GetParameterValueStrings(AudioUnitScope		inScope,
                                                                AudioUnitParameterID	inParameterID,
                                                                CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Echojam::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Echojam::GetParameterInfo(AudioUnitScope		inScope,
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
                outParameterInfo.maxValue = 16;
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
//	Echojam::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Echojam::GetPropertyInfo (AudioUnitPropertyID	inID,
                                                        AudioUnitScope		inScope,
                                                        AudioUnitElement	inElement,
                                                        UInt32 &		outDataSize,
                                                        Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 Echojam::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	Echojam::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			Echojam::GetProperty(	AudioUnitPropertyID inID,
                                                        AudioUnitScope 		inScope,
                                                        AudioUnitElement 	inElement,
                                                        void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	Echojam::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult Echojam::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____EchojamEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Echojam::EchojamKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		Echojam::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for(int x = 0; x < d4A+2; x++) {lA[x] = 0.0; rA[x] = 0.0;}
	for(int x = 0; x < d4B+2; x++) {lB[x] = 0.0; rB[x] = 0.0;}
	for(int x = 0; x < d4C+2; x++) {lC[x] = 0.0; rC[x] = 0.0;}
	for(int x = 0; x < d4D+2; x++) {lD[x] = 0.0; rD[x] = 0.0;}
	for(int x = 0; x < d4E+2; x++) {lE[x] = 0.0; rE[x] = 0.0;}
	for(int x = 0; x < d4F+2; x++) {lF[x] = 0.0; rF[x] = 0.0;}
	for(int x = 0; x < d4G+2; x++) {lG[x] = 0.0; rG[x] = 0.0;}
	for(int x = 0; x < d4H+2; x++) {lH[x] = 0.0; rH[x] = 0.0;}
	for(int x = 0; x < d4I+2; x++) {lI[x] = 0.0; rI[x] = 0.0;}
	for(int x = 0; x < d4J+2; x++) {lJ[x] = 0.0; rJ[x] = 0.0;}
	for(int x = 0; x < d4K+2; x++) {lK[x] = 0.0; rK[x] = 0.0;}
	for(int x = 0; x < d4L+2; x++) {lL[x] = 0.0; rL[x] = 0.0;}
	for(int x = 0; x < d4M+2; x++) {lM[x] = 0.0; rM[x] = 0.0;}
	for(int x = 0; x < d4N+2; x++) {lN[x] = 0.0; rN[x] = 0.0;}
	for(int x = 0; x < d4O+2; x++) {lO[x] = 0.0; rO[x] = 0.0;}
	for(int x = 0; x < d4P+2; x++) {lP[x] = 0.0; rP[x] = 0.0;}

	cLA = cLB = cLC = cLD = cLE = cLF = cLG = cLH = 1;
	cLI = cLJ = cLK = cLL = cLM = cLN = cLO = cLP = 1;
	
	cRA = cRB = cRC = cRD = cRE = cRF = cRG = cRH = 1;
	cRI = cRJ = cRK = cRL = cRM = cRN = cRO = cRP = 1;
	
	fAL = fBL = fCL = fDL = 0.0f;
	hAL = hBL = hCL = hDL = hEL = 0.0f;
	
	fDR = fHR = fLR = fPR = 0.0f;
	hAR = hBR = hCR = hDR = hER = 0.0f;
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	
	shortA = 17; shortB = 19; shortC = 23; shortD = 29; shortE = 19; shortF = 106; shortG = 41;
	shortH = 6361; shortI = 47; shortJ = 53; shortK = 59; shortL = 61; shortM = 67; shortN = 5824;
	shortO = 6257; shortP = 653; //1 to 419 ms, 5286 seat arena
	prevclearcoat = -1;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Echojam::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		Echojam::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
	overallscale /= 44100.0;
	overallscale *= GetSampleRate();
	
	int clearcoat = GetParameter( kParam_A );
	if (clearcoat != prevclearcoat) {
		for(int x = 0; x < d4A+2; x++) {lA[x] = 0.0f; rA[x] = 0.0f;}
		for(int x = 0; x < d4B+2; x++) {lB[x] = 0.0f; rB[x] = 0.0f;}
		for(int x = 0; x < d4C+2; x++) {lC[x] = 0.0f; rC[x] = 0.0f;}
		for(int x = 0; x < d4D+2; x++) {lD[x] = 0.0f; rD[x] = 0.0f;}
		for(int x = 0; x < d4E+2; x++) {lE[x] = 0.0f; rE[x] = 0.0f;}
		for(int x = 0; x < d4F+2; x++) {lF[x] = 0.0f; rF[x] = 0.0f;}
		for(int x = 0; x < d4G+2; x++) {lG[x] = 0.0f; rG[x] = 0.0f;}
		for(int x = 0; x < d4H+2; x++) {lH[x] = 0.0f; rH[x] = 0.0f;}
		for(int x = 0; x < d4I+2; x++) {lI[x] = 0.0f; rI[x] = 0.0f;}
		for(int x = 0; x < d4J+2; x++) {lJ[x] = 0.0f; rJ[x] = 0.0f;}
		for(int x = 0; x < d4K+2; x++) {lK[x] = 0.0f; rK[x] = 0.0f;}
		for(int x = 0; x < d4L+2; x++) {lL[x] = 0.0f; rL[x] = 0.0f;}
		for(int x = 0; x < d4M+2; x++) {lM[x] = 0.0f; rM[x] = 0.0f;}
		for(int x = 0; x < d4N+2; x++) {lN[x] = 0.0f; rN[x] = 0.0f;}
		for(int x = 0; x < d4O+2; x++) {lO[x] = 0.0f; rO[x] = 0.0f;}
		for(int x = 0; x < d4P+2; x++) {lP[x] = 0.0f; rP[x] = 0.0f;}
		cLA = cLB = cLC = cLD = cLE = cLF = cLG = cLH = 1;
		cLI = cLJ = cLK = cLL = cLM = cLN = cLO = cLP = 1;
		cRA = cRB = cRC = cRD = cRE = cRF = cRG = cRH = 1;
		cRI = cRJ = cRK = cRL = cRM = cRN = cRO = cRP = 1;
		
		//Echojam
		switch (clearcoat)
		{
			case 0:
				shortA = 17; shortB = 19; shortC = 23; shortD = 29; shortE = 19; shortF = 106; shortG = 41; shortH = 6361; shortI = 47; shortJ = 53; shortK = 59; shortL = 61; shortM = 67; shortN = 5824; shortO = 6257; shortP = 653; break; //1 to 419 ms, 5286 seat arena
			case 1:
				shortA = 17; shortB = 7450; shortC = 1692; shortD = 29; shortE = 64; shortF = 37; shortG = 41; shortH = 43; shortI = 47; shortJ = 4838; shortK = 776; shortL = 61; shortM = 67; shortN = 6307; shortO = 1158; shortP = 79; break; //2 to 423 ms, 5392 seat arena  
			case 2:
				shortA = 17; shortB = 19; shortC = 23; shortD = 29; shortE = 6111; shortF = 6237; shortG = 41; shortH = 5973; shortI = 47; shortJ = 53; shortK = 59; shortL = 768; shortM = 67; shortN = 71; shortO = 73; shortP = 6822; break; //1 to 436 ms, 5711 seat arena  
			case 3:
				shortA = 17; shortB = 19; shortC = 23; shortD = 29; shortE = 5841; shortF = 37; shortG = 41; shortH = 43; shortI = 794; shortJ = 53; shortK = 59; shortL = 6160; shortM = 7230; shortN = 71; shortO = 73; shortP = 6473; break; //1 to 436 ms, 5721 seat arena  
			case 4:
				shortA = 17; shortB = 3; shortC = 6854; shortD = 6650; shortE = 31; shortF = 6854; shortG = 41; shortH = 765; shortI = 47; shortJ = 53; shortK = 59; shortL = 61; shortM = 67; shortN = 71; shortO = 73; shortP = 809; break; //2 to 463 ms, 6453 seat arena  
			case 5:
				shortA = 756; shortB = 19; shortC = 6998; shortD = 29; shortE = 31; shortF = 37; shortG = 6808; shortH = 43; shortI = 47; shortJ = 53; shortK = 59; shortL = 61; shortM = 6978; shortN = 71; shortO = 269; shortP = 79; break; //3 to 472 ms, 6729 seat arena  
			case 6:
				shortA = 5824; shortB = 652; shortC = 739; shortD = 6086; shortE = 31; shortF = 37; shortG = 41; shortH = 6852; shortI = 47; shortJ = 75; shortK = 59; shortL = 61; shortM = 67; shortN = 71; shortO = 73; shortP = 6589; break; //3 to 444 ms, 5975 seat arena  
			case 7:
				shortA = 17; shortB = 19; shortC = 6906; shortD = 29; shortE = 6361; shortF = 37; shortG = 6098; shortH = 43; shortI = 47; shortJ = 53; shortK = 59; shortL = 765; shortM = 67; shortN = 71; shortO = 6133; shortP = 79; break; //2 to 457 ms, 6291 seat arena  
			case 8:
				shortA = 6617; shortB = 19; shortC = 315; shortD = 29; shortE = 31; shortF = 5982; shortG = 41; shortH = 676; shortI = 47; shortJ = 53; shortK = 7069; shortL = 61; shortM = 67; shortN = 71; shortO = 73; shortP = 79; break; //2 to 461 ms, 6401 seat arena  
			case 9:
				shortA = 17; shortB = 19; shortC = 6171; shortD = 29; shortE = 31; shortF = 37; shortG = 41; shortH = 43; shortI = 7355; shortJ = 53; shortK = 906; shortL = 61; shortM = 7307; shortN = 71; shortO = 1911; shortP = 79; break; //2 to 473 ms, 6729 seat arena  
			case 10:
				shortA = 744; shortB = 649; shortC = 433; shortD = 29; shortE = 31; shortF = 37; shortG = 41; shortH = 6974; shortI = 47; shortJ = 53; shortK = 59; shortL = 61; shortM = 7203; shortN = 71; shortO = 6478; shortP = 79; break; //3 to 483 ms, 7027 seat arena  
			case 11:
				shortA = 848; shortB = 19; shortC = 23; shortD = 29; shortE = 7136; shortF = 7187; shortG = 41; shortH = 43; shortI = 726; shortJ = 53; shortK = 59; shortL = 7175; shortM = 67; shortN = 71; shortO = 73; shortP = 79; break; //3 to 489 ms, 7203 seat arena  
			case 12:
				shortA = 17; shortB = 19; shortC = 23; shortD = 6428; shortE = 31; shortF = 7198; shortG = 7253; shortH = 7087; shortI = 47; shortJ = 53; shortK = 59; shortL = 878; shortM = 67; shortN = 71; shortO = 13; shortP = 79; break; //2 to 489 ms, 7206 seat arena  
			case 13:
				shortA = 962; shortB = 19; shortC = 23; shortD = 29; shortE = 31; shortF = 37; shortG = 41; shortH = 7221; shortI = 47; shortJ = 53; shortK = 59; shortL = 6437; shortM = 67; shortN = 71; shortO = 7104; shortP = 79; break; //2 to 492 ms, 7281 seat arena  
			case 14:
				shortA = 17; shortB = 19; shortC = 4995; shortD = 29; shortE = 20; shortF = 37; shortG = 41; shortH = 5153; shortI = 47; shortJ = 53; shortK = 5325; shortL = 61; shortM = 67; shortN = 71; shortO = 958; shortP = 7443; break; //2 to 519 ms, 8101 seat arena  
			case 15:
				shortA = 17; shortB = 19; shortC = 23; shortD = 5994; shortE = 5965; shortF = 37; shortG = 41; shortH = 6324; shortI = 47; shortJ = 664; shortK = 59; shortL = 61; shortM = 5693; shortN = 71; shortO = 73; shortP = 79; break; //2 to 423 ms, 5399 seat arena  
			case 16:
			default:
				shortA = 6654; shortB = 19; shortC = 23; shortD = 29; shortE = 31; shortF = 37; shortG = 41; shortH = 43; shortI = 47; shortJ = 21; shortK = 59; shortL = 740; shortM = 67; shortN = 6819; shortO = 6662; shortP = 79; break; //2 to 473 ms, 6725 seat arena  
		} //Echojam
		prevclearcoat = clearcoat;
	}
	double regen = (1.0-pow(1.0-GetParameter( kParam_B ),2.0))*0.0625f;
	double derez = pow(GetParameter( kParam_C ),overallscale);
	derez = fmin(fmax(derez,0.0001),1.0);
	int bezFraction = (int)(1.0/derez);
	double bezTrim = (double)bezFraction/(bezFraction+1.0);
	derez = 1.0 / bezFraction;
	bezTrim = 1.0-(derez*bezTrim);
	//the revision more accurately connects the bezier curves
	double wet = 1.0-pow(1.0-GetParameter( kParam_D ),2.0);
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bez[bez_cycle] += derez;
		bez[bez_SampL] += (inputSampleL*derez);
		bez[bez_SampR] += (inputSampleR*derez);
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			//left verbs
			inputSampleL = bez[bez_SampL];
			inputSampleR = bez[bez_SampR];
			
			lA[cLA] = fma(fAL,regen,inputSampleL);
			lB[cLB] = fma(fBL,regen,inputSampleL);
			lC[cLC] = fma(fCL,regen,inputSampleL);
			lD[cLD] = fma(fDL,regen,inputSampleL);
			rD[cRD] = fma(fDR,regen,inputSampleR);
			rH[cRH] = fma(fHR,regen,inputSampleR);
			rL[cRL] = fma(fLR,regen,inputSampleR);
			rP[cRP] = fma(fPR,regen,inputSampleR);
			cLA++; cLB++; cLC++; cLD++;
			cRD++; cRH++; cRL++; cRP++;
			if (cLA > shortA) cLA = 0;
			if (cLB > shortB) cLB = 0;
			if (cLC > shortC) cLC = 0;
			if (cLD > shortD) cLD = 0;
			if (cRD > shortD) cRD = 0;
			if (cRH > shortH) cRH = 0;
			if (cRL > shortL) cRL = 0;
			if (cRP > shortP) cRP = 0;
			hAL = lA[cLA]; hBL = lB[cLB]; hCL = lC[cLC]; hDL = lD[cLD];
			hAR = rD[cRD]; hBR = rH[cRH]; hCR = rL[cRL]; hDR = rP[cRP];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lE[cLE] = fma(hAL,2.0f,hEL);
			lF[cLF] = fma(hBL,2.0f,hEL);
			lG[cLG] = fma(hCL,2.0f,hEL);
			lH[cLH] = fma(hDL,2.0f,hEL);
			rC[cRC] = fma(hAR,2.0f,hER);
			rG[cRG] = fma(hBR,2.0f,hER);
			rK[cRK] = fma(hCR,2.0f,hER);
			rO[cRO] = fma(hDR,2.0f,hER);
			cLE++; cLF++; cLG++; cLH++;
			cRC++; cRG++; cRK++; cRO++;
			if (cLE > shortE) cLE = 0;
			if (cLF > shortF) cLF = 0;
			if (cLG > shortG) cLG = 0;
			if (cLH > shortH) cLH = 0;
			if (cRC > shortC) cRC = 0;
			if (cRG > shortG) cRG = 0;
			if (cRK > shortK) cRK = 0;
			if (cRO > shortO) cRO = 0;
			hAL = lE[cLE]; hBL = lF[cLF]; hCL = lG[cLG]; hDL = lH[cLH];
			hAR = rC[cRC]; hBR = rG[cRG]; hCR = rK[cRK]; hDR = rO[cRO];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lI[cLI] = fma(hAL,2.0f,hEL);
			lJ[cLJ] = fma(hBL,2.0f,hEL);
			lK[cLK] = fma(hCL,2.0f,hEL);
			lL[cLL] = fma(hDL,2.0f,hEL);
			rB[cRB] = fma(hAR,2.0f,hER);
			rF[cRF] = fma(hBR,2.0f,hER);
			rJ[cRJ] = fma(hCR,2.0f,hER);
			rN[cRN] = fma(hDR,2.0f,hER);
			cLI++; cLJ++; cLK++; cLL++;
			cRB++; cRF++; cRJ++; cRN++;
			if (cLI > shortI) cLI = 0;
			if (cLJ > shortJ) cLJ = 0;
			if (cLK > shortK) cLK = 0;
			if (cLL > shortL) cLL = 0;
			if (cRB > shortB) cRB = 0;
			if (cRF > shortF) cRF = 0;
			if (cRJ > shortJ) cRJ = 0;
			if (cRN > shortN) cRN = 0;
			hAL = lI[cLI]; hBL = lJ[cLJ]; hCL = lK[cLK]; hDL = lL[cLL];
			hAR = rB[cRB]; hBR = rF[cRF]; hCR = rJ[cRJ]; hDR = rN[cRN];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			lM[cLM] = fma(hAL,2.0f,hEL);
			lN[cLN] = fma(hBL,2.0f,hEL);
			lO[cLO] = fma(hCL,2.0f,hEL);
			lP[cLP] = fma(hDL,2.0f,hEL);
			rA[cRA] = fma(hAR,2.0f,hER);
			rE[cRE] = fma(hBR,2.0f,hER);
			rI[cRI] = fma(hCR,2.0f,hER);
			rM[cRM] = fma(hDR,2.0f,hER);
			cLM++; cLN++; cLO++; cLP++;
			cRA++; cRE++; cRI++; cRM++;
			if (cLM > shortM) cLM = 0;
			if (cLN > shortN) cLN = 0;
			if (cLO > shortO) cLO = 0;
			if (cLP > shortP) cLP = 0;
			if (cRA > shortA) cRA = 0;
			if (cRE > shortE) cRE = 0;
			if (cRI > shortI) cRI = 0;
			if (cRM > shortM) cRM = 0;
			hAL = lM[cLM]; hBL = lN[cLN]; hCL = lO[cLO]; hDL = lP[cLP];
			hAR = rA[cRA]; hBR = rE[cRE]; hCR = rI[cRI]; hDR = rM[cRM];
			hEL = -(hAL + hBL + hCL + hDL);
			hER = -(hAR + hBR + hCR + hDR);
			fAL = fma(hAR,2.0f,hER);				
			fBL = fma(hBR,2.0f,hER);
			fCL = fma(hCR,2.0f,hER);
			fDL = fma(hDR,2.0f,hER);
			fDR = fma(hAL,2.0f,hEL);				
			fHR = fma(hBL,2.0f,hEL);
			fLR = fma(hCL,2.0f,hEL);
			fPR = fma(hDL,2.0f,hEL);
			inputSampleL = (hAL + hBL + hCL + hDL)*0.0625f;
			inputSampleR = (hAR + hBR + hCR + hDR)*0.0625f;
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double X = bez[bez_cycle]*bezTrim;
		double CBL = (bez[bez_CL]*(1.0-X))+(bez[bez_BL]*X);
		double CBR = (bez[bez_CR]*(1.0-X))+(bez[bez_BR]*X);
		double BAL = (bez[bez_BL]*(1.0-X))+(bez[bez_AL]*X);
		double BAR = (bez[bez_BR]*(1.0-X))+(bez[bez_AR]*X);
		inputSampleL = (bez[bez_BL]+(CBL*(1.0-X))+(BAL*X))*0.125;
		inputSampleR = (bez[bez_BR]+(CBR*(1.0-X))+(BAR*X))*0.125;
		
		inputSampleL = (inputSampleL * wet)+(drySampleL * (1.0-wet));
		inputSampleR = (inputSampleR * wet)+(drySampleR * (1.0-wet));
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
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

