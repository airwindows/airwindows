/*
 *	File:		CreamCoat.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	5/23/24
 *	
 *	Copyright:  Copyright © 2024 Airwindows, Airwindows uses the MIT license
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
 CreamCoat.cpp
 
 =============================================================================*/
#include "CreamCoat.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

AUDIOCOMPONENT_ENTRY(AUBaseFactory, CreamCoat)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	CreamCoat::CreamCoat
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
CreamCoat::CreamCoat(AudioUnit component)
: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_A, kDefaultValue_ParamA );
	SetParameter(kParam_B, kDefaultValue_ParamB );
	SetParameter(kParam_C, kDefaultValue_ParamC );
	SetParameter(kParam_D, kDefaultValue_ParamD );
	SetParameter(kParam_E, kDefaultValue_ParamE );
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	CreamCoat::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			CreamCoat::GetParameterValueStrings(AudioUnitScope		inScope,
															AudioUnitParameterID	inParameterID,
															CFArrayRef *		outStrings)
{
	
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	CreamCoat::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			CreamCoat::GetParameterInfo(AudioUnitScope		inScope,
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
            case kParam_E:
                AUBase::FillInParameterName (outParameterInfo, kParameterEName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Generic;
                outParameterInfo.minValue = 0.0;
                outParameterInfo.maxValue = 1.0;
                outParameterInfo.defaultValue = kDefaultValue_ParamE;
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
//	CreamCoat::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			CreamCoat::GetPropertyInfo (AudioUnitPropertyID	inID,
													AudioUnitScope		inScope,
													AudioUnitElement	inElement,
													UInt32 &		outDataSize,
													Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// state that plugin supports only stereo-in/stereo-out processing
UInt32 CreamCoat::SupportedNumChannels(const AUChannelInfo ** outInfo)
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
//	CreamCoat::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult			CreamCoat::GetProperty(	AudioUnitPropertyID inID,
											   AudioUnitScope 		inScope,
											   AudioUnitElement 	inElement,
											   void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

//	CreamCoat::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult CreamCoat::Initialize()
{
    ComponentResult result = AUEffectBase::Initialize();
    if (result == noErr)
        Reset(kAudioUnitScope_Global, 0);
    return result;
}

#pragma mark ____CreamCoatEffectKernel



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	CreamCoat::CreamCoatKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		CreamCoat::Reset(AudioUnitScope inScope, AudioUnitElement inElement)
{
	for(int count = 0; count < kshortA+2; count++) {aAL[count] = 0.0; aAR[count] = 0.0;}
	for(int count = 0; count < kshortB+2; count++) {aBL[count] = 0.0; aBR[count] = 0.0;}
	for(int count = 0; count < kshortC+2; count++) {aCL[count] = 0.0; aCR[count] = 0.0;}
	for(int count = 0; count < kshortD+2; count++) {aDL[count] = 0.0; aDR[count] = 0.0;}
	for(int count = 0; count < kshortE+2; count++) {aEL[count] = 0.0; aER[count] = 0.0;}
	for(int count = 0; count < kshortF+2; count++) {aFL[count] = 0.0; aFR[count] = 0.0;}
	for(int count = 0; count < kshortG+2; count++) {aGL[count] = 0.0; aGR[count] = 0.0;}
	for(int count = 0; count < kshortH+2; count++) {aHL[count] = 0.0; aHR[count] = 0.0;}
	for(int count = 0; count < kshortI+2; count++) {aIL[count] = 0.0; aIR[count] = 0.0;}
	for(int count = 0; count < kshortJ+2; count++) {aJL[count] = 0.0; aJR[count] = 0.0;}
	for(int count = 0; count < kshortK+2; count++) {aKL[count] = 0.0; aKR[count] = 0.0;}
	for(int count = 0; count < kshortL+2; count++) {aLL[count] = 0.0; aLR[count] = 0.0;}
	for(int count = 0; count < kshortM+2; count++) {aML[count] = 0.0; aMR[count] = 0.0;}
	for(int count = 0; count < kshortN+2; count++) {aNL[count] = 0.0; aNR[count] = 0.0;}
	for(int count = 0; count < kshortO+2; count++) {aOL[count] = 0.0; aOR[count] = 0.0;}
	for(int count = 0; count < kshortP+2; count++) {aPL[count] = 0.0; aPR[count] = 0.0;}
	
	for(int count = 0; count < predelay+2; count++) {aZL[count] = 0.0; aZR[count] = 0.0;}

	feedbackAL = 0.0;
	feedbackBL = 0.0;
	feedbackCL = 0.0;
	feedbackDL = 0.0;
	
	previousAL = 0.0;
	previousBL = 0.0;
	previousCL = 0.0;
	previousDL = 0.0;
	previousEL = 0.0;
	
	feedbackDR = 0.0;
	feedbackHR = 0.0;
	feedbackLR = 0.0;
	feedbackPR = 0.0;
	
	previousAR = 0.0;
	previousBR = 0.0;
	previousCR = 0.0;
	previousDR = 0.0;
	previousER = 0.0;
		
	countAL = 1;
	countBL = 1;
	countCL = 1;
	countDL = 1;	
	countEL = 1;
	countFL = 1;
	countGL = 1;
	countHL = 1;
	countIL = 1;
	countJL = 1;
	countKL = 1;
	countLL = 1;
	countML = 1;
	countNL = 1;
	countOL = 1;
	countPL = 1;
	
	countAR = 1;
	countBR = 1;
	countCR = 1;
	countDR = 1;	
	countER = 1;
	countFR = 1;
	countGR = 1;
	countHR = 1;
	countIR = 1;
	countJR = 1;
	countKR = 1;
	countLR = 1;
	countMR = 1;
	countNR = 1;
	countOR = 1;
	countPR = 1;
	
	countZ = 1;
	
	shortA = 336;
	shortB = 1660;
	shortC = 386;
	shortD = 623;
	shortE = 693;
	shortF = 1079;
	shortG = 891;
	shortH = 1574;
	shortI = 24;
	shortJ = 2641;
	shortK = 1239;
	shortL = 775;
	shortM = 11;
	shortN = 3104;
	shortO = 55;
	shortP = 2366;
	prevclearcoat = -1;
	
	for (int x = 0; x < bez_total; x++) bez[x] = 0.0;
	bez[bez_cycle] = 1.0;
	
	fpdL = 1.0; while (fpdL < 16386) fpdL = rand()*UINT32_MAX;
	fpdR = 1.0; while (fpdR < 16386) fpdR = rand()*UINT32_MAX;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	CreamCoat::ProcessBufferLists
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus		CreamCoat::ProcessBufferLists(AudioUnitRenderActionFlags & ioActionFlags,
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
		for(int count = 0; count < kshortA+2; count++) {aAL[count] = 0.0; aAR[count] = 0.0;}
		for(int count = 0; count < kshortB+2; count++) {aBL[count] = 0.0; aBR[count] = 0.0;}
		for(int count = 0; count < kshortC+2; count++) {aCL[count] = 0.0; aCR[count] = 0.0;}
		for(int count = 0; count < kshortD+2; count++) {aDL[count] = 0.0; aDR[count] = 0.0;}
		for(int count = 0; count < kshortE+2; count++) {aEL[count] = 0.0; aER[count] = 0.0;}
		for(int count = 0; count < kshortF+2; count++) {aFL[count] = 0.0; aFR[count] = 0.0;}
		for(int count = 0; count < kshortG+2; count++) {aGL[count] = 0.0; aGR[count] = 0.0;}
		for(int count = 0; count < kshortH+2; count++) {aHL[count] = 0.0; aHR[count] = 0.0;}
		for(int count = 0; count < kshortI+2; count++) {aIL[count] = 0.0; aIR[count] = 0.0;}
		for(int count = 0; count < kshortJ+2; count++) {aJL[count] = 0.0; aJR[count] = 0.0;}
		for(int count = 0; count < kshortK+2; count++) {aKL[count] = 0.0; aKR[count] = 0.0;}
		for(int count = 0; count < kshortL+2; count++) {aLL[count] = 0.0; aLR[count] = 0.0;}
		for(int count = 0; count < kshortM+2; count++) {aML[count] = 0.0; aMR[count] = 0.0;}
		for(int count = 0; count < kshortN+2; count++) {aNL[count] = 0.0; aNR[count] = 0.0;}
		for(int count = 0; count < kshortO+2; count++) {aOL[count] = 0.0; aOR[count] = 0.0;}
		for(int count = 0; count < kshortP+2; count++) {aPL[count] = 0.0; aPR[count] = 0.0;}		
		countAL = 1;
		countBL = 1;
		countCL = 1;
		countDL = 1;	
		countEL = 1;
		countFL = 1;
		countGL = 1;
		countHL = 1;
		countIL = 1;
		countJL = 1;
		countKL = 1;
		countLL = 1;
		countML = 1;
		countNL = 1;
		countOL = 1;
		countPL = 1;
		
		countAR = 1;
		countBR = 1;
		countCR = 1;
		countDR = 1;	
		countER = 1;
		countFR = 1;
		countGR = 1;
		countHR = 1;
		countIR = 1;
		countJR = 1;
		countKR = 1;
		countLR = 1;
		countMR = 1;
		countNR = 1;
		countOR = 1;
		countPR = 1;
		switch (clearcoat)
		{
			case 0:
				shortA = 65; shortB = 124; shortC = 83; shortD = 180; shortE = 200; shortF = 291; shortG = 108; shortH = 189; shortI = 73; shortJ = 410; shortK = 479; shortL = 310; shortM = 11; shortN = 928; shortO = 23; shortP = 654; break; //5 to 51 ms, 96 seat room. Scarcity, 1 in 125324
				//Short96
			case 1:
				shortA = 114; shortB = 205; shortC = 498; shortD = 195; shortE = 205; shortF = 318; shortG = 143; shortH = 254; shortI = 64; shortJ = 721; shortK = 512; shortL = 324; shortM = 11; shortN = 782; shortO = 26; shortP = 394; break; //7 to 52 ms, 107 seat club. Scarcity, 1 in 65537
				//Short107
			case 2:
				shortA = 118; shortB = 272; shortC = 292; shortD = 145; shortE = 200; shortF = 241; shortG = 204; shortH = 504; shortI = 50; shortJ = 678; shortK = 424; shortL = 412; shortM = 11; shortN = 1124; shortO = 47; shortP = 766; break; //8 to 58 ms, 135 seat club. Scarcity, 1 in 196272
				//Short135
			case 3:
				shortA = 19; shortB = 474; shortC = 301; shortD = 275; shortE = 260; shortF = 321; shortG = 371; shortH = 571; shortI = 50; shortJ = 410; shortK = 697; shortL = 414; shortM = 11; shortN = 986; shortO = 47; shortP = 522; break; //7 to 61 ms, 143 seat club. Scarcity, 1 in 165738
				//Short143
			case 4:
				shortA = 112; shortB = 387; shortC = 452; shortD = 289; shortE = 173; shortF = 476; shortG = 321; shortH = 593; shortI = 73; shortJ = 343; shortK = 829; shortL = 91; shortM = 11; shortN = 1055; shortO = 43; shortP = 862; break; //8 to 66 ms, 166 seat club. Scarcity, 1 in 158437
				//Short166
			case 5:
				shortA = 60; shortB = 368; shortC = 295; shortD = 272; shortE = 210; shortF = 284; shortG = 326; shortH = 830; shortI = 125; shortJ = 236; shortK = 737; shortL = 486; shortM = 11; shortN = 1178; shortO = 75; shortP = 902; break; //9 to 70 ms, 189 seat club. Scarcity, 1 in 94790
				//Short189
			case 6:
				shortA = 73; shortB = 311; shortC = 472; shortD = 251; shortE = 134; shortF = 509; shortG = 393; shortH = 591; shortI = 124; shortJ = 1070; shortK = 340; shortL = 525; shortM = 11; shortN = 1367; shortO = 75; shortP = 816; break; //7 to 79 ms, 225 seat club. Scarcity, 1 in 257803
				//Short225
			case 7:
				shortA = 159; shortB = 518; shortC = 514; shortD = 165; shortE = 275; shortF = 494; shortG = 296; shortH = 667; shortI = 75; shortJ = 1101; shortK = 116; shortL = 414; shortM = 11; shortN = 1261; shortO = 79; shortP = 998; break; //11 to 80 ms, 252 seat club. Scarcity, 1 in 175192
				//Short252
			case 8:
				shortA = 41; shortB = 741; shortC = 274; shortD = 59; shortE = 306; shortF = 332; shortG = 291; shortH = 767; shortI = 42; shortJ = 881; shortK = 959; shortL = 422; shortM = 11; shortN = 1237; shortO = 45; shortP = 958; break; //8 to 83 ms, 255 seat club. Scarcity, 1 in 185708
				//Short255
			case 9:
				shortA = 251; shortB = 437; shortC = 783; shortD = 189; shortE = 130; shortF = 272; shortG = 244; shortH = 761; shortI = 128; shortJ = 1190; shortK = 320; shortL = 491; shortM = 11; shortN = 1409; shortO = 58; shortP = 455; break; //10 to 93 ms, 323 seat club. Scarcity, 1 in 334044
				//Short323
			case 10:
				shortA = 316; shortB = 510; shortC = 1087; shortD = 349; shortE = 359; shortF = 74; shortG = 79; shortH = 1269; shortI = 34; shortJ = 693; shortK = 749; shortL = 511; shortM = 11; shortN = 1751; shortO = 93; shortP = 403; break; //9 to 110 ms, 427 seat theater. Scarcity, 1 in 200715
				//Short427
			case 11:
				shortA = 254; shortB = 651; shortC = 845; shortD = 316; shortE = 373; shortF = 267; shortG = 182; shortH = 857; shortI = 215; shortJ = 1535; shortK = 1127; shortL = 315; shortM = 11; shortN = 1649; shortO = 97; shortP = 829; break; //15 to 110 ms, 470 seat theater. Scarcity, 1 in 362673
				//Short470
			case 12:
				shortA = 113; shortB = 101; shortC = 673; shortD = 357; shortE = 340; shortF = 229; shortG = 278; shortH = 1008; shortI = 265; shortJ = 1890; shortK = 155; shortL = 267; shortM = 11; shortN = 2233; shortO = 116; shortP = 600; break; //11 to 131 ms, 606 seat theater. Scarcity, 1 in 238058
				//Short606
			case 13:
				shortA = 218; shortB = 1058; shortC = 862; shortD = 505; shortE = 297; shortF = 580; shortG = 532; shortH = 1387; shortI = 120; shortJ = 576; shortK = 1409; shortL = 473; shortM = 11; shortN = 1991; shortO = 76; shortP = 685; break; //14 to 132 ms, 643 seat theater. Scarcity, 1 in 193432
				//Short643
			case 14:
				shortA = 78; shortB = 760; shortC = 982; shortD = 528; shortE = 445; shortF = 1128; shortG = 130; shortH = 708; shortI = 22; shortJ = 2144; shortK = 354; shortL = 1169; shortM = 11; shortN = 2782; shortO = 58; shortP = 1515; break; //5 to 159 ms, 809 seat hall. Scarcity, 1 in 212274
				//Short809
			case 15:
				shortA = 330; shortB = 107; shortC = 1110; shortD = 371; shortE = 620; shortF = 143; shortG = 1014; shortH = 1763; shortI = 184; shortJ = 2068; shortK = 1406; shortL = 595; shortM = 11; shortN = 2639; shortO = 33; shortP = 1594; break; //10 to 171 ms, 984 seat hall. Scarcity, 1 in 226499
				//Short984
			case 16:
			default:
				shortA = 336; shortB = 1660; shortC = 386; shortD = 623; shortE = 693; shortF = 1079; shortG = 891; shortH = 1574; shortI = 24; shortJ = 2641; shortK = 1239; shortL = 775; shortM = 11; shortN = 3104; shortO = 55; shortP = 2366; break; //24 to 203 ms, 1541 seat hall. Scarcity, 1 in 275025
				//Short1541
		}
		prevclearcoat = clearcoat;
	}
	
	
	double regen = (1.0-pow(1.0-GetParameter( kParam_B ),2.0))*0.0625;
	double derez = GetParameter( kParam_C )/overallscale;
	if (derez < 0.0005) derez = 0.0005; if (derez > 1.0) derez = 1.0;
	derez = 1.0 / ((int)(1.0/derez));
	//this hard-locks derez to exact subdivisions of 1.0
	int adjPredelay = predelay*GetParameter( kParam_D )*derez;	
	double wet = GetParameter( kParam_E )*2.0;
	double dry = 2.0 - wet;
	if (wet > 1.0) wet = 1.0; else wet *= wet;
	if (wet < 0.0) wet = 0.0;
	if (dry > 1.0) dry = 1.0;
	if (dry < 0.0) dry = 0.0;
	//this reverb makes 50% full dry AND full wet, not crossfaded.
	//that's so it can be on submixes without cutting back dry channel when adjusted:
	//unless you go super heavy, you are only adjusting the added verb loudness.
	
	while (nSampleFrames-- > 0) {
		double inputSampleL = *inputL;
		double inputSampleR = *inputR;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
				
		bez[bez_cycle] += derez;
		bez[bez_SampL] += ((inputSampleL+bez[bez_InL]) * derez);
		bez[bez_SampR] += ((inputSampleR+bez[bez_InR]) * derez);
		bez[bez_InL] = inputSampleL; bez[bez_InR] = inputSampleR;
		if (bez[bez_cycle] > 1.0) { //hit the end point and we do a reverb sample
			bez[bez_cycle] = 0.0;
			
			//predelay
			aZL[countZ] = bez[bez_SampL];
			aZR[countZ] = bez[bez_SampR];
			countZ++; if (countZ < 0 || countZ > adjPredelay) countZ = 0;
			bez[bez_SampL] = aZL[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			bez[bez_SampR] = aZR[countZ-((countZ > adjPredelay)?adjPredelay+1:0)];
			//end predelay
			
			aAL[countAL] = (bez[bez_SampL]+bez[bez_UnInL]) + (feedbackAL * regen);
			aBL[countBL] = (bez[bez_SampL]+bez[bez_UnInL]) + (feedbackBL * regen);
			aCL[countCL] = (bez[bez_SampL]+bez[bez_UnInL]) + (feedbackCL * regen);
			aDL[countDL] = (bez[bez_SampL]+bez[bez_UnInL]) + (feedbackDL * regen);
			bez[bez_UnInL] = bez[bez_SampL];
			
			aDR[countDR] = (bez[bez_SampR]+bez[bez_UnInR]) + (feedbackDR * regen);
			aHR[countHR] = (bez[bez_SampR]+bez[bez_UnInR]) + (feedbackHR * regen);
			aLR[countLR] = (bez[bez_SampR]+bez[bez_UnInR]) + (feedbackLR * regen);
			aPR[countPR] = (bez[bez_SampR]+bez[bez_UnInR]) + (feedbackPR * regen);
			bez[bez_UnInR] = bez[bez_SampR];
			
			countAL++; if (countAL < 0 || countAL > shortA) countAL = 0;
			countBL++; if (countBL < 0 || countBL > shortB) countBL = 0;
			countCL++; if (countCL < 0 || countCL > shortC) countCL = 0;
			countDL++; if (countDL < 0 || countDL > shortD) countDL = 0;
			
			countDR++; if (countDR < 0 || countDR > shortD) countDR = 0;
			countHR++; if (countHR < 0 || countHR > shortH) countHR = 0;
			countLR++; if (countLR < 0 || countLR > shortL) countLR = 0;
			countPR++; if (countPR < 0 || countPR > shortP) countPR = 0;
			
			double outAL = aAL[countAL-((countAL > shortA)?shortA+1:0)];
			double outBL = aBL[countBL-((countBL > shortB)?shortB+1:0)];
			double outCL = aCL[countCL-((countCL > shortC)?shortC+1:0)];
			double outDL = aDL[countDL-((countDL > shortD)?shortD+1:0)];
			
			double outDR = aDR[countDR-((countDR > shortD)?shortD+1:0)];
			double outHR = aHR[countHR-((countHR > shortH)?shortH+1:0)];
			double outLR = aLR[countLR-((countLR > shortL)?shortL+1:0)];
			double outPR = aPR[countPR-((countPR > shortP)?shortP+1:0)];
			
			aEL[countEL] = outAL - (outBL + outCL + outDL);
			aFL[countFL] = outBL - (outAL + outCL + outDL);
			aGL[countGL] = outCL - (outAL + outBL + outDL);
			aHL[countHL] = outDL - (outAL + outBL + outCL);
			
			aCR[countCR] = outDR - (outHR + outLR + outPR);
			aGR[countGR] = outHR - (outDR + outLR + outPR);
			aKR[countKR] = outLR - (outDR + outHR + outPR);
			aOR[countOR] = outPR - (outDR + outHR + outLR);
			
			countEL++; if (countEL < 0 || countEL > shortE) countEL = 0;
			countFL++; if (countFL < 0 || countFL > shortF) countFL = 0;
			countGL++; if (countGL < 0 || countGL > shortG) countGL = 0;
			countHL++; if (countHL < 0 || countHL > shortH) countHL = 0;
			
			countCR++; if (countCR < 0 || countCR > shortC) countCR = 0;
			countGR++; if (countGR < 0 || countGR > shortG) countGR = 0;
			countKR++; if (countKR < 0 || countKR > shortK) countKR = 0;
			countOR++; if (countOR < 0 || countOR > shortO) countOR = 0;
			
			double outEL = aEL[countEL-((countEL > shortE)?shortE+1:0)];
			double outFL = aFL[countFL-((countFL > shortF)?shortF+1:0)];
			double outGL = aGL[countGL-((countGL > shortG)?shortG+1:0)];
			double outHL = aHL[countHL-((countHL > shortH)?shortH+1:0)];
			
			double outCR = aCR[countCR-((countCR > shortC)?shortC+1:0)];
			double outGR = aGR[countGR-((countGR > shortG)?shortG+1:0)];
			double outKR = aKR[countKR-((countKR > shortK)?shortK+1:0)];
			double outOR = aOR[countOR-((countOR > shortO)?shortO+1:0)];
			
			aIL[countIL] = outEL - (outFL + outGL + outHL);
			aJL[countJL] = outFL - (outEL + outGL + outHL);
			aKL[countKL] = outGL - (outEL + outFL + outHL);
			aLL[countLL] = outHL - (outEL + outFL + outGL);
			
			aBR[countBR] = outCR - (outGR + outKR + outOR);
			aFR[countFR] = outGR - (outCR + outKR + outOR);
			aJR[countJR] = outKR - (outCR + outGR + outOR);
			aNR[countNR] = outOR - (outCR + outGR + outKR);
			
			countIL++; if (countIL < 0 || countIL > shortI) countIL = 0;
			countJL++; if (countJL < 0 || countJL > shortJ) countJL = 0;
			countKL++; if (countKL < 0 || countKL > shortK) countKL = 0;
			countLL++; if (countLL < 0 || countLL > shortL) countLL = 0;
			
			countBR++; if (countBR < 0 || countBR > shortB) countBR = 0;
			countFR++; if (countFR < 0 || countFR > shortF) countFR = 0;
			countJR++; if (countJR < 0 || countJR > shortJ) countJR = 0;
			countNR++; if (countNR < 0 || countNR > shortN) countNR = 0;
			
			double outIL = aIL[countIL-((countIL > shortI)?shortI+1:0)];
			double outJL = aJL[countJL-((countJL > shortJ)?shortJ+1:0)];
			double outKL = aKL[countKL-((countKL > shortK)?shortK+1:0)];
			double outLL = aLL[countLL-((countLL > shortL)?shortL+1:0)];
			
			double outBR = aBR[countBR-((countBR > shortB)?shortB+1:0)];
			double outFR = aFR[countFR-((countFR > shortF)?shortF+1:0)];
			double outJR = aJR[countJR-((countJR > shortJ)?shortJ+1:0)];
			double outNR = aNR[countNR-((countNR > shortN)?shortN+1:0)];
			
			aML[countML] = outIL - (outJL + outKL + outLL);
			aNL[countNL] = outJL - (outIL + outKL + outLL);
			aOL[countOL] = outKL - (outIL + outJL + outLL);
			aPL[countPL] = outLL - (outIL + outJL + outKL);
			
			aAR[countAR] = outBR - (outFR + outJR + outNR);
			aER[countER] = outFR - (outBR + outJR + outNR);
			aIR[countIR] = outJR - (outBR + outFR + outNR);
			aMR[countMR] = outNR - (outBR + outFR + outJR);
			
			countML++; if (countML < 0 || countML > shortM) countML = 0;
			countNL++; if (countNL < 0 || countNL > shortN) countNL = 0;
			countOL++; if (countOL < 0 || countOL > shortO) countOL = 0;
			countPL++; if (countPL < 0 || countPL > shortP) countPL = 0;
			
			countAR++; if (countAR < 0 || countAR > shortA) countAR = 0;
			countER++; if (countER < 0 || countER > shortE) countER = 0;
			countIR++; if (countIR < 0 || countIR > shortI) countIR = 0;
			countMR++; if (countMR < 0 || countMR > shortM) countMR = 0;
			
			double outML = aML[countML-((countML > shortM)?shortM+1:0)];
			double outNL = aNL[countNL-((countNL > shortN)?shortN+1:0)];
			double outOL = aOL[countOL-((countOL > shortO)?shortO+1:0)];
			double outPL = aPL[countPL-((countPL > shortP)?shortP+1:0)];
			
			double outAR = aAR[countAR-((countAR > shortA)?shortA+1:0)];
			double outER = aER[countER-((countER > shortE)?shortE+1:0)];
			double outIR = aIR[countIR-((countIR > shortI)?shortI+1:0)];
			double outMR = aMR[countMR-((countMR > shortM)?shortM+1:0)];			
			
			feedbackAL = outML - (outNL + outOL + outPL);
			feedbackDR = outAR - (outER + outIR + outMR);
			feedbackBL = outNL - (outML + outOL + outPL);
			feedbackHR = outER - (outAR + outIR + outMR);
			feedbackCL = outOL - (outML + outNL + outPL);
			feedbackLR = outIR - (outAR + outER + outMR);
			feedbackDL = outPL - (outML + outNL + outOL);
			feedbackPR = outMR - (outAR + outER + outIR);
			//which we need to feed back into the input again, a bit
			
			inputSampleL = (outML + outNL + outOL + outPL)/32.0;
			inputSampleR = (outAR + outER + outIR + outMR)/32.0;
			//and take the final combined sum of outputs, corrected for Householder gain and averaging
			
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double CBL = (bez[bez_CL]*(1.0-bez[bez_cycle]))+(bez[bez_BL]*bez[bez_cycle]);
		double CBR = (bez[bez_CR]*(1.0-bez[bez_cycle]))+(bez[bez_BR]*bez[bez_cycle]);
		double BAL = (bez[bez_BL]*(1.0-bez[bez_cycle]))+(bez[bez_AL]*bez[bez_cycle]);
		double BAR = (bez[bez_BR]*(1.0-bez[bez_cycle]))+(bez[bez_AR]*bez[bez_cycle]);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-bez[bez_cycle]))+(BAL*bez[bez_cycle]))*0.125;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-bez[bez_cycle]))+(BAR*bez[bez_cycle]))*0.125;
		inputSampleL = CBAL;
		inputSampleR = CBAR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		if (wet < 1.0) {inputSampleL *= wet; inputSampleR *= wet;}
		if (dry < 1.0) {drySampleL *= dry; drySampleR *= dry;}
		inputSampleL += drySampleL; inputSampleR += drySampleR;
		//this is our submix verb dry/wet: 0.5 is BOTH at FULL VOLUME
		//purpose is that, if you're adding verb, you're not altering other balances
		
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

