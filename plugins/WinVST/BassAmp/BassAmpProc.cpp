/* ========================================
 *  BassAmp - BassAmp.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BassAmp_H
#include "BassAmp.h"
#endif

void BassAmp::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double contHigh = A;
	double dry = B;
	double contDub = C*1.3;
	double contSub = D/2.0;
	
	double driveone = pow(contHigh*3.0,2);
	double driveoutput = contHigh;
	double iirAmount = 0.344/overallscale;
	double BassGain = contDub * 0.1;
	double HeadBumpFreq = (BassGain+0.0001)/overallscale;
	double BassOutGain = contDub * 0.2;
	double SubGain = contSub * 0.1;
	double SubBumpFreq = (SubGain+0.0001)/overallscale;
	double SubOutGain = contSub * 0.3;
	double iirHPAmount = 0.0000014/overallscale;
    
    while (--sampleFrames >= 0)
    {
		double LinputSample = *in1;
		double RinputSample = *in2;
		if (fabs(LinputSample)<1.18e-37) LinputSample = fpdL * 1.18e-37;
		if (fabs(RinputSample)<1.18e-37) RinputSample = fpdR * 1.18e-37;
		
		LataDrySample = LinputSample;
		LataHalfDrySample = LataHalfwaySample = (LinputSample + LataLast1Sample + (LataLast2Sample*ataK1) + (LataLast3Sample*ataK2) + (LataLast4Sample*ataK6) + (LataLast5Sample*ataK7) + (LataLast6Sample*ataK8)) / 2.0;
		LataLast6Sample = LataLast5Sample; LataLast5Sample = LataLast4Sample; LataLast4Sample = LataLast3Sample; LataLast3Sample = LataLast2Sample; LataLast2Sample = LataLast1Sample; LataLast1Sample = LinputSample;
		//setting up oversampled special antialiasing
		RataDrySample = RinputSample;
		RataHalfDrySample = RataHalfwaySample = (RinputSample + RataLast1Sample + (RataLast2Sample*ataK1) + (RataLast3Sample*ataK2) + (RataLast4Sample*ataK6) + (RataLast5Sample*ataK7) + (RataLast6Sample*ataK8)) / 2.0;
		RataLast6Sample = RataLast5Sample; RataLast5Sample = RataLast4Sample; RataLast4Sample = RataLast3Sample; RataLast3Sample = RataLast2Sample; RataLast2Sample = RataLast1Sample; RataLast1Sample = RinputSample;
		//setting up oversampled special antialiasing
		
		double correction;
		double subtractSample;
		double tempSample; //used repeatedly in different places
		
		//begin first half- change inputSample -> ataHalfwaySample, ataDrySample -> ataHalfDrySample
		
		double LataHalfwayLowpass;
		double RataHalfwayLowpass;
		if (flip)
		{
			tempSample = subtractSample = LataHalfwaySample;
			LiirDriveSampleA = (LiirDriveSampleA * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleA; correction = LiirDriveSampleA;
			LiirDriveSampleC = (LiirDriveSampleC * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleC; correction += LiirDriveSampleC;
			LiirDriveSampleE = (LiirDriveSampleE * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleE; correction += LiirDriveSampleE;
			LataHalfwaySample -= correction;
			LataHalfwayLowpass = subtractSample - LataHalfwaySample;
			
			tempSample = subtractSample = RataHalfwaySample;
			RiirDriveSampleA = (RiirDriveSampleA * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleA; correction = RiirDriveSampleA;
			RiirDriveSampleC = (RiirDriveSampleC * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleC; correction += RiirDriveSampleC;
			RiirDriveSampleE = (RiirDriveSampleE * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleE; correction += RiirDriveSampleE;
			RataHalfwaySample -= correction;
			RataHalfwayLowpass = subtractSample - RataHalfwaySample;
		}
		else
		{
			tempSample = subtractSample = LataHalfwaySample;
			LiirDriveSampleB = (LiirDriveSampleB * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleB; correction = LiirDriveSampleB;
			LiirDriveSampleD = (LiirDriveSampleD * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleD; correction += LiirDriveSampleD;
			LiirDriveSampleF = (LiirDriveSampleF * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleF; correction += RiirDriveSampleF;
			LataHalfwaySample -= correction;			
			LataHalfwayLowpass = subtractSample - LataHalfwaySample;
			
			tempSample = subtractSample = RataHalfwaySample;
			RiirDriveSampleB = (RiirDriveSampleB * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleB; correction = RiirDriveSampleB;
			RiirDriveSampleD = (RiirDriveSampleD * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleD; correction += RiirDriveSampleD;
			RiirDriveSampleF = (RiirDriveSampleF * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleF; correction += RiirDriveSampleF;
			RataHalfwaySample -= correction;			
			RataHalfwayLowpass = subtractSample - RataHalfwaySample;
		}
		//highpass section
		if (LataHalfwaySample > 1.0) {LataHalfwaySample = 1.0;}
		if (LataHalfwaySample < -1.0) {LataHalfwaySample = -1.0;}
		if (RataHalfwaySample > 1.0) {RataHalfwaySample = 1.0;}
		if (RataHalfwaySample < -1.0) {RataHalfwaySample = -1.0;}
		double drivetwo = driveone;
		while (drivetwo > 0.60)
		{
			drivetwo -= 0.60;
			LataHalfwaySample -= (LataHalfwaySample * (fabs(LataHalfwaySample) * 0.60) * (fabs(LataHalfwaySample) * 0.60) );
			RataHalfwaySample -= (RataHalfwaySample * (fabs(RataHalfwaySample) * 0.60) * (fabs(RataHalfwaySample) * 0.60) );
			LataHalfwaySample *= (1.0+0.60);
			RataHalfwaySample *= (1.0+0.60);
		}
		//that's taken care of the really high gain stuff
		
		LataHalfwaySample -= (LataHalfwaySample * (fabs(LataHalfwaySample) * drivetwo) * (fabs(LataHalfwaySample) * drivetwo) );
		RataHalfwaySample -= (RataHalfwaySample * (fabs(RataHalfwaySample) * drivetwo) * (fabs(RataHalfwaySample) * drivetwo) );
		LataHalfwaySample *= (1.0+drivetwo);
		RataHalfwaySample *= (1.0+drivetwo);
		
		
		//end first half
		//begin second half- inputSample and ataDrySample handled separately here
		
		double LataLowpass;
		double RataLowpass;
		if (flip)
		{
			tempSample = subtractSample = LinputSample;
			LiirDriveSampleA = (LiirDriveSampleA * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleA; correction = LiirDriveSampleA;
			LiirDriveSampleC = (LiirDriveSampleC * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleC; correction += LiirDriveSampleC;
			LiirDriveSampleE = (LiirDriveSampleE * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleE; correction += LiirDriveSampleE;
			LinputSample -= correction;
			LataLowpass = subtractSample - LinputSample;

			tempSample = subtractSample = RinputSample;
			RiirDriveSampleA = (RiirDriveSampleA * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleA; correction = RiirDriveSampleA;
			RiirDriveSampleC = (RiirDriveSampleC * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleC; correction += RiirDriveSampleC;
			RiirDriveSampleE = (RiirDriveSampleE * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleE; correction += RiirDriveSampleE;
			RinputSample -= correction;
			RataLowpass = subtractSample - RinputSample;
		}
		else
		{
			tempSample = subtractSample = LinputSample;
			LiirDriveSampleB = (LiirDriveSampleB * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleB; correction = LiirDriveSampleB;
			LiirDriveSampleD = (LiirDriveSampleD * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleD; correction += LiirDriveSampleD;
			LiirDriveSampleF = (LiirDriveSampleF * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleF; correction += LiirDriveSampleF;
			LinputSample -= correction;
			LataLowpass = subtractSample - LinputSample;

			tempSample = subtractSample = RinputSample;
			RiirDriveSampleB = (RiirDriveSampleB * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleB; correction = RiirDriveSampleB;
			RiirDriveSampleD = (RiirDriveSampleD * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleD; correction += RiirDriveSampleD;
			RiirDriveSampleF = (RiirDriveSampleF * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleF; correction += RiirDriveSampleF;
			RinputSample -= correction;
			RataLowpass = subtractSample - RinputSample;
		}
		//highpass section
		if (LinputSample > 1.0) {LinputSample = 1.0;}
		if (LinputSample < -1.0) {LinputSample = -1.0;}
		if (RinputSample > 1.0) {RinputSample = 1.0;}
		if (RinputSample < -1.0) {RinputSample = -1.0;}
		drivetwo = driveone;
		while (drivetwo > 0.60)
		{
			drivetwo -= 0.60;
			LinputSample -= (LinputSample * (fabs(LinputSample) * 0.60) * (fabs(LinputSample) * 0.60) );
			RinputSample -= (RinputSample * (fabs(RinputSample) * 0.60) * (fabs(RinputSample) * 0.60) );
			LinputSample *= (1.0+0.60);
			RinputSample *= (1.0+0.60);
		}
		//that's taken care of the really high gain stuff
		
		LinputSample -= (LinputSample * (fabs(LinputSample) * drivetwo) * (fabs(LinputSample) * drivetwo) );
		RinputSample -= (RinputSample * (fabs(RinputSample) * drivetwo) * (fabs(RinputSample) * drivetwo) );
		LinputSample *= (1.0+drivetwo);
		RinputSample *= (1.0+drivetwo);
		//end second half
		
		
		LataHalfDrySample = (LataDrySample*ataK3)+(LataHalfDrySample*ataK4);
		LataHalfDiffSample = (LataHalfwaySample - LataHalfDrySample)/2.0;
		LataLastDiffSample = LataDiffSample*ataK5;
		LataDiffSample = (LinputSample - LataDrySample)/2.0;
		LataDiffSample += LataHalfDiffSample;
		LataDiffSample -= LataLastDiffSample;
		LinputSample = LataDrySample;
		LinputSample += LataDiffSample;
		//apply processing as difference to non-oversampled raw input
		RataHalfDrySample = (RataDrySample*ataK3)+(RataHalfDrySample*ataK4);
		RataHalfDiffSample = (RataHalfwaySample - RataHalfDrySample)/2.0;
		RataLastDiffSample = RataDiffSample*ataK5;
		RataDiffSample = (RinputSample - RataDrySample)/2.0;
		RataDiffSample += RataHalfDiffSample;
		RataDiffSample -= RataLastDiffSample;
		RinputSample = RataDrySample;
		RinputSample += RataDiffSample;
		//apply processing as difference to non-oversampled raw input
		
		//now we've got inputSample as the Drive top-end output, and we have ataLowpass and ataHalfwayLowpass
		LataLowpass += LataHalfwayLowpass; //and combined them. Now we make sub-octaves
		RataLowpass += RataHalfwayLowpass; //and combined them. Now we make sub-octaves
		
		double randy = (double(fpdL)/UINT32_MAX)*0.0555; //0 to 1 the noise, may not be needed		
		
		switch (bflip)
		{
			case 1:				
				LiirHeadBumpA += (LataLowpass * BassGain);
				LiirHeadBumpA -= (LiirHeadBumpA * LiirHeadBumpA * LiirHeadBumpA * HeadBumpFreq);
				LiirHeadBumpA = ((1.0-randy) * LiirHeadBumpA) + (randy * 0.5 * LiirHeadBumpB) + (randy * 0.5 * LiirHeadBumpC);
				
				RiirHeadBumpA += (RataLowpass * BassGain);
				RiirHeadBumpA -= (RiirHeadBumpA * RiirHeadBumpA * RiirHeadBumpA * HeadBumpFreq);
				RiirHeadBumpA = ((1.0-randy) * RiirHeadBumpA) + (randy * 0.5 * RiirHeadBumpB) + (randy * 0.5 * RiirHeadBumpC);
				break;
			case 2:
				LiirHeadBumpB += (LataLowpass * BassGain);
				LiirHeadBumpB -= (LiirHeadBumpB * LiirHeadBumpB * LiirHeadBumpB * HeadBumpFreq);
				LiirHeadBumpB = (randy * 0.5 * LiirHeadBumpA) + ((1.0-randy) * LiirHeadBumpB) + (randy * 0.5 * LiirHeadBumpC);

				RiirHeadBumpB += (RataLowpass * BassGain);
				RiirHeadBumpB -= (RiirHeadBumpB * RiirHeadBumpB * RiirHeadBumpB * HeadBumpFreq);
				RiirHeadBumpB = (randy * 0.5 * RiirHeadBumpA) + ((1.0-randy) * RiirHeadBumpB) + (randy * 0.5 * RiirHeadBumpC);
				break;
			case 3:
				LiirHeadBumpC += (LataLowpass * BassGain);
				LiirHeadBumpC -= (LiirHeadBumpC * LiirHeadBumpC * LiirHeadBumpC * HeadBumpFreq);
				LiirHeadBumpC = (randy * 0.5 * LiirHeadBumpA) + (randy * 0.5 * LiirHeadBumpB) + ((1.0-randy) * LiirHeadBumpC);

				RiirHeadBumpC += (RataLowpass * BassGain);
				RiirHeadBumpC -= (RiirHeadBumpC * RiirHeadBumpC * RiirHeadBumpC * HeadBumpFreq);
				RiirHeadBumpC = (randy * 0.5 * RiirHeadBumpA) + (randy * 0.5 * RiirHeadBumpB) + ((1.0-randy) * RiirHeadBumpC);
				break;
		}
		double LHeadBump = LiirHeadBumpA + LiirHeadBumpB + LiirHeadBumpC;
		double RHeadBump = RiirHeadBumpA + RiirHeadBumpB + RiirHeadBumpC;
		
		double LSubBump = fabs(LHeadBump);
		double RSubBump = fabs(RHeadBump);
		
		if (LHeadBump > 0)
		{if (LWasNegative){LSubOctave = !LSubOctave;} LWasNegative = false;}
		else {LWasNegative = true;}
		//set up polarities for sub-bass version
		if (RHeadBump > 0)
		{if (RWasNegative){RSubOctave = !RSubOctave;} RWasNegative = false;}
		else {RWasNegative = true;}
		//set up polarities for sub-bass version
		
		if (LSubOctave == false) {LSubBump = -LSubBump;}
		if (RSubOctave == false) {RSubBump = -RSubBump;}
		
		switch (bflip)
		{
			case 1:				
				LiirSubBumpA += (LSubBump * SubGain);
				LiirSubBumpA -= (LiirSubBumpA * LiirSubBumpA * LiirSubBumpA * SubBumpFreq);
				LiirSubBumpA = ((1.0-randy) * LiirSubBumpA) + (randy * 0.5 * LiirSubBumpB) + (randy * 0.5 * LiirSubBumpC);

				RiirSubBumpA += (RSubBump * SubGain);
				RiirSubBumpA -= (RiirSubBumpA * RiirSubBumpA * RiirSubBumpA * SubBumpFreq);
				RiirSubBumpA = ((1.0-randy) * RiirSubBumpA) + (randy * 0.5 * RiirSubBumpB) + (randy * 0.5 * RiirSubBumpC);
				break;
			case 2:
				LiirSubBumpB += (LSubBump * SubGain);
				LiirSubBumpB -= (LiirSubBumpB * LiirSubBumpB * LiirSubBumpB * SubBumpFreq);
				LiirSubBumpB = (randy * 0.5 * LiirSubBumpA) + ((1.0-randy) * LiirSubBumpB) + (randy * 0.5 * LiirSubBumpC);

				RiirSubBumpB += (RSubBump * SubGain);
				RiirSubBumpB -= (RiirSubBumpB * RiirSubBumpB * RiirSubBumpB * SubBumpFreq);
				RiirSubBumpB = (randy * 0.5 * RiirSubBumpA) + ((1.0-randy) * RiirSubBumpB) + (randy * 0.5 * RiirSubBumpC);
				break;
			case 3:
				LiirSubBumpC += (LSubBump * SubGain);
				LiirSubBumpC -= (LiirSubBumpC * LiirSubBumpC * LiirSubBumpC * SubBumpFreq);
				LiirSubBumpC = (randy * 0.5 * LiirSubBumpA) + (randy * 0.5 * LiirSubBumpB) + ((1.0-randy) * LiirSubBumpC);

				RiirSubBumpC += (RSubBump * SubGain);
				RiirSubBumpC -= (RiirSubBumpC * RiirSubBumpC * RiirSubBumpC * SubBumpFreq);
				RiirSubBumpC = (randy * 0.5 * RiirSubBumpA) + (randy * 0.5 * RiirSubBumpB) + ((1.0-randy) * RiirSubBumpC);
				break;
		}
		LSubBump = LiirSubBumpA + LiirSubBumpB + LiirSubBumpC;
		RSubBump = RiirSubBumpA + RiirSubBumpB + RiirSubBumpC;
		
		flip = !flip;
		bflip++;
		if (bflip < 1 || bflip > 3) bflip = 1;
		
		LinputSample *= driveoutput;
		RinputSample *= driveoutput;
		
		
		correction = LataDrySample*dry;
		correction += (LHeadBump * BassOutGain);
		correction += (LSubBump * SubOutGain);
		LinputSample += correction;
		
		correction = RataDrySample*dry;
		correction += (RHeadBump * BassOutGain);
		correction += (RSubBump * SubOutGain);
		RinputSample += correction;
		
		tempSample = LinputSample;		
		LiirSampleA = (LiirSampleA * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleA; correction = LiirSampleA;
		LiirSampleB = (LiirSampleB * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleB; correction += LiirSampleB;
		LiirSampleC = (LiirSampleC * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleC; correction += LiirSampleC;
		LiirSampleD = (LiirSampleD * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleD; correction += LiirSampleD;
		LiirSampleE = (LiirSampleE * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleE; correction += LiirSampleE;
		LiirSampleF = (LiirSampleF * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleF; correction += LiirSampleF;
		LiirSampleG = (LiirSampleG * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleG; correction += LiirSampleG;
		LiirSampleH = (LiirSampleH * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleH; correction += LiirSampleH;
		LiirSampleI = (LiirSampleI * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleI; correction += LiirSampleI;
		LiirSampleJ = (LiirSampleJ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleJ; correction += LiirSampleJ;
		LiirSampleK = (LiirSampleK * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleK; correction += LiirSampleK;
		LiirSampleL = (LiirSampleL * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleL; correction += LiirSampleL;
		LiirSampleM = (LiirSampleM * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleM; correction += LiirSampleM;
		LiirSampleN = (LiirSampleN * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleN; correction += LiirSampleN;
		LiirSampleO = (LiirSampleO * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleO; correction += LiirSampleO;
		LiirSampleP = (LiirSampleP * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleP; correction += LiirSampleP;
		LiirSampleQ = (LiirSampleQ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleQ; correction += LiirSampleQ;
		LiirSampleR = (LiirSampleR * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleR; correction += LiirSampleR;
		LiirSampleS = (LiirSampleS * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleS; correction += LiirSampleS;
		LiirSampleT = (LiirSampleT * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleT; correction += LiirSampleT;
		LiirSampleU = (LiirSampleU * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleU; correction += LiirSampleU;
		LiirSampleV = (LiirSampleV * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleV; correction += LiirSampleV;
		LiirSampleW = (LiirSampleW * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleW; correction += LiirSampleW;
		LiirSampleX = (LiirSampleX * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleX; correction += LiirSampleX;
		LiirSampleY = (LiirSampleY * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleY; correction += LiirSampleY;
		LiirSampleZ = (LiirSampleZ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleZ; correction += LiirSampleZ;
		//do the IIR on a dummy sample, and store up the correction in a variable at the same scale as the very low level
		//numbers being used. Don't keep doing it against the possibly high level signal number.
		LinputSample -= correction;
		//apply stored up tiny corrections
		
		tempSample = RinputSample;		
		RiirSampleA = (RiirSampleA * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleA; correction = RiirSampleA;
		RiirSampleB = (RiirSampleB * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleB; correction += RiirSampleB;
		RiirSampleC = (RiirSampleC * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleC; correction += RiirSampleC;
		RiirSampleD = (RiirSampleD * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleD; correction += RiirSampleD;
		RiirSampleE = (RiirSampleE * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleE; correction += RiirSampleE;
		RiirSampleF = (RiirSampleF * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleF; correction += RiirSampleF;
		RiirSampleG = (RiirSampleG * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleG; correction += RiirSampleG;
		RiirSampleH = (RiirSampleH * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleH; correction += RiirSampleH;
		RiirSampleI = (RiirSampleI * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleI; correction += RiirSampleI;
		RiirSampleJ = (RiirSampleJ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleJ; correction += RiirSampleJ;
		RiirSampleK = (RiirSampleK * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleK; correction += RiirSampleK;
		RiirSampleL = (RiirSampleL * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleL; correction += RiirSampleL;
		RiirSampleM = (RiirSampleM * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleM; correction += RiirSampleM;
		RiirSampleN = (RiirSampleN * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleN; correction += RiirSampleN;
		RiirSampleO = (RiirSampleO * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleO; correction += RiirSampleO;
		RiirSampleP = (RiirSampleP * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleP; correction += RiirSampleP;
		RiirSampleQ = (RiirSampleQ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleQ; correction += RiirSampleQ;
		RiirSampleR = (RiirSampleR * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleR; correction += RiirSampleR;
		RiirSampleS = (RiirSampleS * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleS; correction += RiirSampleS;
		RiirSampleT = (RiirSampleT * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleT; correction += RiirSampleT;
		RiirSampleU = (RiirSampleU * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleU; correction += RiirSampleU;
		RiirSampleV = (RiirSampleV * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleV; correction += RiirSampleV;
		RiirSampleW = (RiirSampleW * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleW; correction += RiirSampleW;
		RiirSampleX = (RiirSampleX * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleX; correction += RiirSampleX;
		RiirSampleY = (RiirSampleY * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleY; correction += RiirSampleY;
		RiirSampleZ = (RiirSampleZ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleZ; correction += RiirSampleZ;
		//do the IIR on a dummy sample, and store up the correction in a variable at the same scale as the very low level
		//numbers being used. Don't keep doing it against the possibly high level signal number.
		RinputSample -= correction;
		//apply stored up tiny corrections
				
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)LinputSample, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		LinputSample += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)RinputSample, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		RinputSample += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		*out1 = LinputSample;
		*out2 = RinputSample;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void BassAmp::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double contHigh = A;
	double dry = B;
	double contDub = C*1.3;
	double contSub = D/2.0;
	
	double driveone = pow(contHigh*3.0,2);
	double driveoutput = contHigh;
	double iirAmount = 0.344/overallscale;
	double BassGain = contDub * 0.1;
	double HeadBumpFreq = (BassGain+0.0001)/overallscale;
	double BassOutGain = contDub * 0.2;
	double SubGain = contSub * 0.1;
	double SubBumpFreq = (SubGain+0.0001)/overallscale;
	double SubOutGain = contSub * 0.3;
	double iirHPAmount = 0.0000014/overallscale;
    
    while (--sampleFrames >= 0)
    {
		double LinputSample = *in1;
		double RinputSample = *in2;
		if (fabs(LinputSample)<1.18e-43) LinputSample = fpdL * 1.18e-43;
		if (fabs(RinputSample)<1.18e-43) RinputSample = fpdR * 1.18e-43;
		
		LataDrySample = LinputSample;
		LataHalfDrySample = LataHalfwaySample = (LinputSample + LataLast1Sample + (LataLast2Sample*ataK1) + (LataLast3Sample*ataK2) + (LataLast4Sample*ataK6) + (LataLast5Sample*ataK7) + (LataLast6Sample*ataK8)) / 2.0;
		LataLast6Sample = LataLast5Sample; LataLast5Sample = LataLast4Sample; LataLast4Sample = LataLast3Sample; LataLast3Sample = LataLast2Sample; LataLast2Sample = LataLast1Sample; LataLast1Sample = LinputSample;
		//setting up oversampled special antialiasing
		RataDrySample = RinputSample;
		RataHalfDrySample = RataHalfwaySample = (RinputSample + RataLast1Sample + (RataLast2Sample*ataK1) + (RataLast3Sample*ataK2) + (RataLast4Sample*ataK6) + (RataLast5Sample*ataK7) + (RataLast6Sample*ataK8)) / 2.0;
		RataLast6Sample = RataLast5Sample; RataLast5Sample = RataLast4Sample; RataLast4Sample = RataLast3Sample; RataLast3Sample = RataLast2Sample; RataLast2Sample = RataLast1Sample; RataLast1Sample = RinputSample;
		//setting up oversampled special antialiasing
		
		double correction;
		double subtractSample;
		double tempSample; //used repeatedly in different places
		
		//begin first half- change inputSample -> ataHalfwaySample, ataDrySample -> ataHalfDrySample
		
		double LataHalfwayLowpass;
		double RataHalfwayLowpass;
		if (flip)
		{
			tempSample = subtractSample = LataHalfwaySample;
			LiirDriveSampleA = (LiirDriveSampleA * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleA; correction = LiirDriveSampleA;
			LiirDriveSampleC = (LiirDriveSampleC * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleC; correction += LiirDriveSampleC;
			LiirDriveSampleE = (LiirDriveSampleE * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleE; correction += LiirDriveSampleE;
			LataHalfwaySample -= correction;
			LataHalfwayLowpass = subtractSample - LataHalfwaySample;
			
			tempSample = subtractSample = RataHalfwaySample;
			RiirDriveSampleA = (RiirDriveSampleA * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleA; correction = RiirDriveSampleA;
			RiirDriveSampleC = (RiirDriveSampleC * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleC; correction += RiirDriveSampleC;
			RiirDriveSampleE = (RiirDriveSampleE * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleE; correction += RiirDriveSampleE;
			RataHalfwaySample -= correction;
			RataHalfwayLowpass = subtractSample - RataHalfwaySample;
		}
		else
		{
			tempSample = subtractSample = LataHalfwaySample;
			LiirDriveSampleB = (LiirDriveSampleB * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleB; correction = LiirDriveSampleB;
			LiirDriveSampleD = (LiirDriveSampleD * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleD; correction += LiirDriveSampleD;
			LiirDriveSampleF = (LiirDriveSampleF * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleF; correction += RiirDriveSampleF;
			LataHalfwaySample -= correction;			
			LataHalfwayLowpass = subtractSample - LataHalfwaySample;
			
			tempSample = subtractSample = RataHalfwaySample;
			RiirDriveSampleB = (RiirDriveSampleB * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleB; correction = RiirDriveSampleB;
			RiirDriveSampleD = (RiirDriveSampleD * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleD; correction += RiirDriveSampleD;
			RiirDriveSampleF = (RiirDriveSampleF * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleF; correction += RiirDriveSampleF;
			RataHalfwaySample -= correction;			
			RataHalfwayLowpass = subtractSample - RataHalfwaySample;
		}
		//highpass section
		if (LataHalfwaySample > 1.0) {LataHalfwaySample = 1.0;}
		if (LataHalfwaySample < -1.0) {LataHalfwaySample = -1.0;}
		if (RataHalfwaySample > 1.0) {RataHalfwaySample = 1.0;}
		if (RataHalfwaySample < -1.0) {RataHalfwaySample = -1.0;}
		double drivetwo = driveone;
		while (drivetwo > 0.60)
		{
			drivetwo -= 0.60;
			LataHalfwaySample -= (LataHalfwaySample * (fabs(LataHalfwaySample) * 0.60) * (fabs(LataHalfwaySample) * 0.60) );
			RataHalfwaySample -= (RataHalfwaySample * (fabs(RataHalfwaySample) * 0.60) * (fabs(RataHalfwaySample) * 0.60) );
			LataHalfwaySample *= (1.0+0.60);
			RataHalfwaySample *= (1.0+0.60);
		}
		//that's taken care of the really high gain stuff
		
		LataHalfwaySample -= (LataHalfwaySample * (fabs(LataHalfwaySample) * drivetwo) * (fabs(LataHalfwaySample) * drivetwo) );
		RataHalfwaySample -= (RataHalfwaySample * (fabs(RataHalfwaySample) * drivetwo) * (fabs(RataHalfwaySample) * drivetwo) );
		LataHalfwaySample *= (1.0+drivetwo);
		RataHalfwaySample *= (1.0+drivetwo);
		
		
		//end first half
		//begin second half- inputSample and ataDrySample handled separately here
		
		double LataLowpass;
		double RataLowpass;
		if (flip)
		{
			tempSample = subtractSample = LinputSample;
			LiirDriveSampleA = (LiirDriveSampleA * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleA; correction = LiirDriveSampleA;
			LiirDriveSampleC = (LiirDriveSampleC * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleC; correction += LiirDriveSampleC;
			LiirDriveSampleE = (LiirDriveSampleE * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleE; correction += LiirDriveSampleE;
			LinputSample -= correction;
			LataLowpass = subtractSample - LinputSample;
			
			tempSample = subtractSample = RinputSample;
			RiirDriveSampleA = (RiirDriveSampleA * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleA; correction = RiirDriveSampleA;
			RiirDriveSampleC = (RiirDriveSampleC * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleC; correction += RiirDriveSampleC;
			RiirDriveSampleE = (RiirDriveSampleE * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleE; correction += RiirDriveSampleE;
			RinputSample -= correction;
			RataLowpass = subtractSample - RinputSample;
		}
		else
		{
			tempSample = subtractSample = LinputSample;
			LiirDriveSampleB = (LiirDriveSampleB * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleB; correction = LiirDriveSampleB;
			LiirDriveSampleD = (LiirDriveSampleD * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleD; correction += LiirDriveSampleD;
			LiirDriveSampleF = (LiirDriveSampleF * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= LiirDriveSampleF; correction += LiirDriveSampleF;
			LinputSample -= correction;
			LataLowpass = subtractSample - LinputSample;
			
			tempSample = subtractSample = RinputSample;
			RiirDriveSampleB = (RiirDriveSampleB * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleB; correction = RiirDriveSampleB;
			RiirDriveSampleD = (RiirDriveSampleD * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleD; correction += RiirDriveSampleD;
			RiirDriveSampleF = (RiirDriveSampleF * (1.0-iirAmount)) + (tempSample * iirAmount); tempSample -= RiirDriveSampleF; correction += RiirDriveSampleF;
			RinputSample -= correction;
			RataLowpass = subtractSample - RinputSample;
		}
		//highpass section
		if (LinputSample > 1.0) {LinputSample = 1.0;}
		if (LinputSample < -1.0) {LinputSample = -1.0;}
		if (RinputSample > 1.0) {RinputSample = 1.0;}
		if (RinputSample < -1.0) {RinputSample = -1.0;}
		drivetwo = driveone;
		while (drivetwo > 0.60)
		{
			drivetwo -= 0.60;
			LinputSample -= (LinputSample * (fabs(LinputSample) * 0.60) * (fabs(LinputSample) * 0.60) );
			RinputSample -= (RinputSample * (fabs(RinputSample) * 0.60) * (fabs(RinputSample) * 0.60) );
			LinputSample *= (1.0+0.60);
			RinputSample *= (1.0+0.60);
		}
		//that's taken care of the really high gain stuff
		
		LinputSample -= (LinputSample * (fabs(LinputSample) * drivetwo) * (fabs(LinputSample) * drivetwo) );
		RinputSample -= (RinputSample * (fabs(RinputSample) * drivetwo) * (fabs(RinputSample) * drivetwo) );
		LinputSample *= (1.0+drivetwo);
		RinputSample *= (1.0+drivetwo);
		//end second half
		
		
		LataHalfDrySample = (LataDrySample*ataK3)+(LataHalfDrySample*ataK4);
		LataHalfDiffSample = (LataHalfwaySample - LataHalfDrySample)/2.0;
		LataLastDiffSample = LataDiffSample*ataK5;
		LataDiffSample = (LinputSample - LataDrySample)/2.0;
		LataDiffSample += LataHalfDiffSample;
		LataDiffSample -= LataLastDiffSample;
		LinputSample = LataDrySample;
		LinputSample += LataDiffSample;
		//apply processing as difference to non-oversampled raw input
		RataHalfDrySample = (RataDrySample*ataK3)+(RataHalfDrySample*ataK4);
		RataHalfDiffSample = (RataHalfwaySample - RataHalfDrySample)/2.0;
		RataLastDiffSample = RataDiffSample*ataK5;
		RataDiffSample = (RinputSample - RataDrySample)/2.0;
		RataDiffSample += RataHalfDiffSample;
		RataDiffSample -= RataLastDiffSample;
		RinputSample = RataDrySample;
		RinputSample += RataDiffSample;
		//apply processing as difference to non-oversampled raw input
		
		//now we've got inputSample as the Drive top-end output, and we have ataLowpass and ataHalfwayLowpass
		LataLowpass += LataHalfwayLowpass; //and combined them. Now we make sub-octaves
		RataLowpass += RataHalfwayLowpass; //and combined them. Now we make sub-octaves
		
		double randy = (double(fpdL)/UINT32_MAX)*0.0555; //0 to 1 the noise, may not be needed		
		
		switch (bflip)
		{
			case 1:				
				LiirHeadBumpA += (LataLowpass * BassGain);
				LiirHeadBumpA -= (LiirHeadBumpA * LiirHeadBumpA * LiirHeadBumpA * HeadBumpFreq);
				LiirHeadBumpA = ((1.0-randy) * LiirHeadBumpA) + (randy * LiirHeadBumpB) + (randy * LiirHeadBumpC);
				
				RiirHeadBumpA += (RataLowpass * BassGain);
				RiirHeadBumpA -= (RiirHeadBumpA * RiirHeadBumpA * RiirHeadBumpA * HeadBumpFreq);
				RiirHeadBumpA = ((1.0-randy) * RiirHeadBumpA) + (randy * RiirHeadBumpB) + (randy * RiirHeadBumpC);
				break;
			case 2:
				LiirHeadBumpB += (LataLowpass * BassGain);
				LiirHeadBumpB -= (LiirHeadBumpB * LiirHeadBumpB * LiirHeadBumpB * HeadBumpFreq);
				LiirHeadBumpB = (randy * LiirHeadBumpA) + ((1.0-randy) * LiirHeadBumpB) + (randy * LiirHeadBumpC);
				
				RiirHeadBumpB += (RataLowpass * BassGain);
				RiirHeadBumpB -= (RiirHeadBumpB * RiirHeadBumpB * RiirHeadBumpB * HeadBumpFreq);
				RiirHeadBumpB = (randy * RiirHeadBumpA) + ((1.0-randy) * RiirHeadBumpB) + (randy * RiirHeadBumpC);
				break;
			case 3:
				LiirHeadBumpC += (LataLowpass * BassGain);
				LiirHeadBumpC -= (LiirHeadBumpC * LiirHeadBumpC * LiirHeadBumpC * HeadBumpFreq);
				LiirHeadBumpC = (randy * LiirHeadBumpA) + (randy * LiirHeadBumpB) + ((1.0-randy) * LiirHeadBumpC);
				
				RiirHeadBumpC += (RataLowpass * BassGain);
				RiirHeadBumpC -= (RiirHeadBumpC * RiirHeadBumpC * RiirHeadBumpC * HeadBumpFreq);
				RiirHeadBumpC = (randy * RiirHeadBumpA) + (randy * RiirHeadBumpB) + ((1.0-randy) * RiirHeadBumpC);
				break;
		}
		double LHeadBump = LiirHeadBumpA + LiirHeadBumpB + LiirHeadBumpC;
		double RHeadBump = RiirHeadBumpA + RiirHeadBumpB + RiirHeadBumpC;
		
		double LSubBump = fabs(LHeadBump);
		double RSubBump = fabs(RHeadBump);
		
		if (LHeadBump > 0)
		{if (LWasNegative){LSubOctave = !LSubOctave;} LWasNegative = false;}
		else {LWasNegative = true;}
		//set up polarities for sub-bass version
		if (RHeadBump > 0)
		{if (RWasNegative){RSubOctave = !RSubOctave;} RWasNegative = false;}
		else {RWasNegative = true;}
		//set up polarities for sub-bass version
		
		if (LSubOctave == false) {LSubBump = -LSubBump;}
		if (RSubOctave == false) {RSubBump = -RSubBump;}
		
		switch (bflip)
		{
			case 1:				
				LiirSubBumpA += (LSubBump * SubGain);
				LiirSubBumpA -= (LiirSubBumpA * LiirSubBumpA * LiirSubBumpA * SubBumpFreq);
				LiirSubBumpA = ((1.0-randy) * LiirSubBumpA) + (randy * LiirSubBumpB) + (randy * LiirSubBumpC);
				
				RiirSubBumpA += (RSubBump * SubGain);
				RiirSubBumpA -= (RiirSubBumpA * RiirSubBumpA * RiirSubBumpA * SubBumpFreq);
				RiirSubBumpA = ((1.0-randy) * RiirSubBumpA) + (randy * RiirSubBumpB) + (randy * RiirSubBumpC);
				break;
			case 2:
				LiirSubBumpB += (LSubBump * SubGain);
				LiirSubBumpB -= (LiirSubBumpB * LiirSubBumpB * LiirSubBumpB * SubBumpFreq);
				LiirSubBumpB = (randy * LiirSubBumpA) + ((1.0-randy) * LiirSubBumpB) + (randy * LiirSubBumpC);
				
				RiirSubBumpB += (RSubBump * SubGain);
				RiirSubBumpB -= (RiirSubBumpB * RiirSubBumpB * RiirSubBumpB * SubBumpFreq);
				RiirSubBumpB = (randy * RiirSubBumpA) + ((1.0-randy) * RiirSubBumpB) + (randy * RiirSubBumpC);
				break;
			case 3:
				LiirSubBumpC += (LSubBump * SubGain);
				LiirSubBumpC -= (LiirSubBumpC * LiirSubBumpC * LiirSubBumpC * SubBumpFreq);
				LiirSubBumpC = (randy * LiirSubBumpA) + (randy * LiirSubBumpB) + ((1.0-randy) * LiirSubBumpC);
				
				RiirSubBumpC += (RSubBump * SubGain);
				RiirSubBumpC -= (RiirSubBumpC * RiirSubBumpC * RiirSubBumpC * SubBumpFreq);
				RiirSubBumpC = (randy * RiirSubBumpA) + (randy * RiirSubBumpB) + ((1.0-randy) * RiirSubBumpC);
				break;
		}
		LSubBump = LiirSubBumpA + LiirSubBumpB + LiirSubBumpC;
		RSubBump = RiirSubBumpA + RiirSubBumpB + RiirSubBumpC;
		
		flip = !flip;
		bflip++;
		if (bflip < 1 || bflip > 3) bflip = 1;
		
		LinputSample *= driveoutput;
		RinputSample *= driveoutput;
		
		
		correction = LataDrySample*dry;
		correction += (LHeadBump * BassOutGain);
		correction += (LSubBump * SubOutGain);
		LinputSample += correction;
		
		correction = RataDrySample*dry;
		correction += (RHeadBump * BassOutGain);
		correction += (RSubBump * SubOutGain);
		RinputSample += correction;
		
		tempSample = LinputSample;		
		LiirSampleA = (LiirSampleA * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleA; correction = LiirSampleA;
		LiirSampleB = (LiirSampleB * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleB; correction += LiirSampleB;
		LiirSampleC = (LiirSampleC * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleC; correction += LiirSampleC;
		LiirSampleD = (LiirSampleD * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleD; correction += LiirSampleD;
		LiirSampleE = (LiirSampleE * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleE; correction += LiirSampleE;
		LiirSampleF = (LiirSampleF * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleF; correction += LiirSampleF;
		LiirSampleG = (LiirSampleG * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleG; correction += LiirSampleG;
		LiirSampleH = (LiirSampleH * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleH; correction += LiirSampleH;
		LiirSampleI = (LiirSampleI * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleI; correction += LiirSampleI;
		LiirSampleJ = (LiirSampleJ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleJ; correction += LiirSampleJ;
		LiirSampleK = (LiirSampleK * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleK; correction += LiirSampleK;
		LiirSampleL = (LiirSampleL * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleL; correction += LiirSampleL;
		LiirSampleM = (LiirSampleM * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleM; correction += LiirSampleM;
		LiirSampleN = (LiirSampleN * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleN; correction += LiirSampleN;
		LiirSampleO = (LiirSampleO * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleO; correction += LiirSampleO;
		LiirSampleP = (LiirSampleP * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleP; correction += LiirSampleP;
		LiirSampleQ = (LiirSampleQ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleQ; correction += LiirSampleQ;
		LiirSampleR = (LiirSampleR * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleR; correction += LiirSampleR;
		LiirSampleS = (LiirSampleS * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleS; correction += LiirSampleS;
		LiirSampleT = (LiirSampleT * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleT; correction += LiirSampleT;
		LiirSampleU = (LiirSampleU * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleU; correction += LiirSampleU;
		LiirSampleV = (LiirSampleV * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleV; correction += LiirSampleV;
		LiirSampleW = (LiirSampleW * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleW; correction += LiirSampleW;
		LiirSampleX = (LiirSampleX * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleX; correction += LiirSampleX;
		LiirSampleY = (LiirSampleY * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleY; correction += LiirSampleY;
		LiirSampleZ = (LiirSampleZ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= LiirSampleZ; correction += LiirSampleZ;
		//do the IIR on a dummy sample, and store up the correction in a variable at the same scale as the very low level
		//numbers being used. Don't keep doing it against the possibly high level signal number.
		LinputSample -= correction;
		//apply stored up tiny corrections
		
		tempSample = RinputSample;		
		RiirSampleA = (RiirSampleA * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleA; correction = RiirSampleA;
		RiirSampleB = (RiirSampleB * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleB; correction += RiirSampleB;
		RiirSampleC = (RiirSampleC * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleC; correction += RiirSampleC;
		RiirSampleD = (RiirSampleD * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleD; correction += RiirSampleD;
		RiirSampleE = (RiirSampleE * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleE; correction += RiirSampleE;
		RiirSampleF = (RiirSampleF * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleF; correction += RiirSampleF;
		RiirSampleG = (RiirSampleG * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleG; correction += RiirSampleG;
		RiirSampleH = (RiirSampleH * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleH; correction += RiirSampleH;
		RiirSampleI = (RiirSampleI * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleI; correction += RiirSampleI;
		RiirSampleJ = (RiirSampleJ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleJ; correction += RiirSampleJ;
		RiirSampleK = (RiirSampleK * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleK; correction += RiirSampleK;
		RiirSampleL = (RiirSampleL * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleL; correction += RiirSampleL;
		RiirSampleM = (RiirSampleM * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleM; correction += RiirSampleM;
		RiirSampleN = (RiirSampleN * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleN; correction += RiirSampleN;
		RiirSampleO = (RiirSampleO * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleO; correction += RiirSampleO;
		RiirSampleP = (RiirSampleP * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleP; correction += RiirSampleP;
		RiirSampleQ = (RiirSampleQ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleQ; correction += RiirSampleQ;
		RiirSampleR = (RiirSampleR * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleR; correction += RiirSampleR;
		RiirSampleS = (RiirSampleS * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleS; correction += RiirSampleS;
		RiirSampleT = (RiirSampleT * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleT; correction += RiirSampleT;
		RiirSampleU = (RiirSampleU * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleU; correction += RiirSampleU;
		RiirSampleV = (RiirSampleV * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleV; correction += RiirSampleV;
		RiirSampleW = (RiirSampleW * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleW; correction += RiirSampleW;
		RiirSampleX = (RiirSampleX * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleX; correction += RiirSampleX;
		RiirSampleY = (RiirSampleY * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleY; correction += RiirSampleY;
		RiirSampleZ = (RiirSampleZ * (1.0-iirHPAmount)) + (tempSample * iirHPAmount); tempSample -= RiirSampleZ; correction += RiirSampleZ;
		//do the IIR on a dummy sample, and store up the correction in a variable at the same scale as the very low level
		//numbers being used. Don't keep doing it against the possibly high level signal number.
		RinputSample -= correction;
		//apply stored up tiny corrections
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)LinputSample, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//LinputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//frexp((double)RinputSample, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//RinputSample += ((double(fpd)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = LinputSample;
		*out2 = RinputSample;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
