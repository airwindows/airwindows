/* ========================================
 *  BassKit - BassKit.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BassKit_H
#include "BassKit.h"
#endif

void BassKit::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double ataLowpass;
	double randy;
	double invrandy;
	double HeadBump = 0.0;
	double BassGain = A * 0.1;
	double HeadBumpFreq = ((B*0.1)+0.02)/overallscale;
	double iirAmount = HeadBumpFreq/44.1;
	double BassOutGain = ((C*2.0)-1.0)*fabs(((C*2.0)-1.0));
	double SubBump = 0.0;
	double SubOutGain = ((D*2.0)-1.0)*fabs(((D*2.0)-1.0))*4.0;
	double clamp = 0.0;
	double fuzz = 0.111;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		ataLowpass = (inputSampleL + inputSampleR) / 2.0;
		iirDriveSampleA = (iirDriveSampleA * (1.0 - HeadBumpFreq)) + (ataLowpass * HeadBumpFreq); ataLowpass = iirDriveSampleA;
		iirDriveSampleB = (iirDriveSampleB * (1.0 - HeadBumpFreq)) + (ataLowpass * HeadBumpFreq); ataLowpass = iirDriveSampleB;
		
		
		oscGate += fabs(ataLowpass * 10.0);
		oscGate -= 0.001;
		if (oscGate > 1.0) oscGate = 1.0;
		if (oscGate < 0) oscGate = 0;
		//got a value that only goes down low when there's silence or near silence on input
		clamp = 1.0-oscGate;
		clamp *= 0.00001;
		//set up the thing to choke off oscillations- belt and suspenders affair
		
		if (ataLowpass > 0)
		{if (WasNegative){SubOctave = !SubOctave;} WasNegative = false;}
		else {WasNegative = true;}
		//set up polarities for sub-bass version
		randy = (double(fpdL)/UINT32_MAX)*fuzz; //0 to 1 the noise, may not be needed
		invrandy = (1.0-randy);
		randy /= 2.0;
		//set up the noise
		
		iirSampleA = (iirSampleA * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleA;
		iirSampleB = (iirSampleB * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleB;
		iirSampleC = (iirSampleC * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleC;
		iirSampleD = (iirSampleD * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleD;
		iirSampleE = (iirSampleE * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleE;
		iirSampleF = (iirSampleF * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleF;
		iirSampleG = (iirSampleG * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleG;
		iirSampleH = (iirSampleH * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleH;
		iirSampleI = (iirSampleI * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleI;
		iirSampleJ = (iirSampleJ * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleJ;
		iirSampleK = (iirSampleK * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleK;
		iirSampleL = (iirSampleL * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleL;
		iirSampleM = (iirSampleM * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleM;
		iirSampleN = (iirSampleN * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleN;
		iirSampleO = (iirSampleO * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleO;
		iirSampleP = (iirSampleP * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleP;
		iirSampleQ = (iirSampleQ * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleQ;
		iirSampleR = (iirSampleR * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleR;
		iirSampleS = (iirSampleS * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleS;
		iirSampleT = (iirSampleT * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleT;
		iirSampleU = (iirSampleU * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleU;
		iirSampleV = (iirSampleV * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleV;
		
		switch (bflip)
		{
			case 1:				
				iirHeadBumpA += (ataLowpass * BassGain);
				iirHeadBumpA -= (iirHeadBumpA * iirHeadBumpA * iirHeadBumpA * HeadBumpFreq);
				iirHeadBumpA = (invrandy * iirHeadBumpA) + (randy * iirHeadBumpB) + (randy * iirHeadBumpC);
				if (iirHeadBumpA > 0) iirHeadBumpA -= clamp;
				if (iirHeadBumpA < 0) iirHeadBumpA += clamp;
				HeadBump = iirHeadBumpA;
				break;
			case 2:
				iirHeadBumpB += (ataLowpass * BassGain);
				iirHeadBumpB -= (iirHeadBumpB * iirHeadBumpB * iirHeadBumpB * HeadBumpFreq);
				iirHeadBumpB = (randy * iirHeadBumpA) + (invrandy * iirHeadBumpB) + (randy * iirHeadBumpC);
				if (iirHeadBumpB > 0) iirHeadBumpB -= clamp;
				if (iirHeadBumpB < 0) iirHeadBumpB += clamp;
				HeadBump = iirHeadBumpB;
				break;
			case 3:
				iirHeadBumpC += (ataLowpass * BassGain);
				iirHeadBumpC -= (iirHeadBumpC * iirHeadBumpC * iirHeadBumpC * HeadBumpFreq);
				iirHeadBumpC = (randy * iirHeadBumpA) + (randy * iirHeadBumpB) + (invrandy * iirHeadBumpC);
				if (iirHeadBumpC > 0) iirHeadBumpC -= clamp;
				if (iirHeadBumpC < 0) iirHeadBumpC += clamp;
				HeadBump = iirHeadBumpC;
				break;
		}
		
		iirSampleW = (iirSampleW * (1.0 - iirAmount)) + (HeadBump * iirAmount); HeadBump -= iirSampleW;
		iirSampleX = (iirSampleX * (1.0 - iirAmount)) + (HeadBump * iirAmount); HeadBump -= iirSampleX;
		
		SubBump = HeadBump;
		iirSampleY = (iirSampleY * (1.0 - iirAmount)) + (SubBump * iirAmount); SubBump -= iirSampleY;
		
		iirDriveSampleC = (iirDriveSampleC * (1.0 - HeadBumpFreq)) + (SubBump * HeadBumpFreq); SubBump = iirDriveSampleC;
		iirDriveSampleD = (iirDriveSampleD * (1.0 - HeadBumpFreq)) + (SubBump * HeadBumpFreq); SubBump = iirDriveSampleD;
		
		
		SubBump = fabs(SubBump);
		if (SubOctave == false) {SubBump = -SubBump;}
		
		switch (bflip)
		{
			case 1:				
				iirSubBumpA += SubBump;// * BassGain);
				iirSubBumpA -= (iirSubBumpA * iirSubBumpA * iirSubBumpA * HeadBumpFreq);
				iirSubBumpA = (invrandy * iirSubBumpA) + (randy * iirSubBumpB) + (randy * iirSubBumpC);
				if (iirSubBumpA > 0) iirSubBumpA -= clamp;
				if (iirSubBumpA < 0) iirSubBumpA += clamp;
				SubBump = iirSubBumpA;
				break;
			case 2:
				iirSubBumpB += SubBump;// * BassGain);
				iirSubBumpB -= (iirSubBumpB * iirSubBumpB * iirSubBumpB * HeadBumpFreq);
				iirSubBumpB = (randy * iirSubBumpA) + (invrandy * iirSubBumpB) + (randy * iirSubBumpC);
				if (iirSubBumpB > 0) iirSubBumpB -= clamp;
				if (iirSubBumpB < 0) iirSubBumpB += clamp;
				SubBump = iirSubBumpB;
				break;
			case 3:
				iirSubBumpC += SubBump;// * BassGain);
				iirSubBumpC -= (iirSubBumpC * iirSubBumpC * iirSubBumpC * HeadBumpFreq);
				iirSubBumpC = (randy * iirSubBumpA) + (randy * iirSubBumpB) + (invrandy * iirSubBumpC);
				if (iirSubBumpC > 0) iirSubBumpC -= clamp;
				if (iirSubBumpC < 0) iirSubBumpC += clamp;
				SubBump = iirSubBumpC;
				break;
		}
		
		iirSampleZ = (iirSampleZ * (1.0 - HeadBumpFreq)) + (SubBump * HeadBumpFreq); SubBump = iirSampleZ;
		iirDriveSampleE = (iirDriveSampleE * (1.0 - iirAmount)) + (SubBump * iirAmount); SubBump = iirDriveSampleE;
		iirDriveSampleF = (iirDriveSampleF * (1.0 - iirAmount)) + (SubBump * iirAmount); SubBump = iirDriveSampleF;
		
		
		inputSampleL += (HeadBump * BassOutGain);
		inputSampleL += (SubBump * SubOutGain);
		
		inputSampleR += (HeadBump * BassOutGain);
		inputSampleR += (SubBump * SubOutGain);
		
		
		flip = !flip;
		bflip++;
		if (bflip < 1 || bflip > 3) bflip = 1;
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void BassKit::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double ataLowpass;
	double randy;
	double invrandy;
	double HeadBump = 0.0;
	double BassGain = A * 0.1;
	double HeadBumpFreq = ((B*0.1)+0.02)/overallscale;
	double iirAmount = HeadBumpFreq/44.1;
	double BassOutGain = ((C*2.0)-1.0)*fabs(((C*2.0)-1.0));
	double SubBump = 0.0;
	double SubOutGain = ((D*2.0)-1.0)*fabs(((D*2.0)-1.0))*4.0;
	double clamp = 0.0;
	double fuzz = 0.111;
		
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		ataLowpass = (inputSampleL + inputSampleR) / 2.0;
		iirDriveSampleA = (iirDriveSampleA * (1.0 - HeadBumpFreq)) + (ataLowpass * HeadBumpFreq); ataLowpass = iirDriveSampleA;
		iirDriveSampleB = (iirDriveSampleB * (1.0 - HeadBumpFreq)) + (ataLowpass * HeadBumpFreq); ataLowpass = iirDriveSampleB;
		
		
		oscGate += fabs(ataLowpass * 10.0);
		oscGate -= 0.001;
		if (oscGate > 1.0) oscGate = 1.0;
		if (oscGate < 0) oscGate = 0;
		//got a value that only goes down low when there's silence or near silence on input
		clamp = 1.0-oscGate;
		clamp *= 0.00001;
		//set up the thing to choke off oscillations- belt and suspenders affair
		
		if (ataLowpass > 0)
		{if (WasNegative){SubOctave = !SubOctave;} WasNegative = false;}
		else {WasNegative = true;}
		//set up polarities for sub-bass version
		randy = (double(fpdL)/UINT32_MAX)*fuzz; //0 to 1 the noise, may not be needed
		invrandy = (1.0-randy);
		randy /= 2.0;
		//set up the noise
		
		iirSampleA = (iirSampleA * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleA;
		iirSampleB = (iirSampleB * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleB;
		iirSampleC = (iirSampleC * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleC;
		iirSampleD = (iirSampleD * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleD;
		iirSampleE = (iirSampleE * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleE;
		iirSampleF = (iirSampleF * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleF;
		iirSampleG = (iirSampleG * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleG;
		iirSampleH = (iirSampleH * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleH;
		iirSampleI = (iirSampleI * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleI;
		iirSampleJ = (iirSampleJ * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleJ;
		iirSampleK = (iirSampleK * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleK;
		iirSampleL = (iirSampleL * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleL;
		iirSampleM = (iirSampleM * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleM;
		iirSampleN = (iirSampleN * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleN;
		iirSampleO = (iirSampleO * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleO;
		iirSampleP = (iirSampleP * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleP;
		iirSampleQ = (iirSampleQ * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleQ;
		iirSampleR = (iirSampleR * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleR;
		iirSampleS = (iirSampleS * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleS;
		iirSampleT = (iirSampleT * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleT;
		iirSampleU = (iirSampleU * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleU;
		iirSampleV = (iirSampleV * (1.0 - iirAmount)) + (ataLowpass * iirAmount); ataLowpass -= iirSampleV;
		
		switch (bflip)
		{
			case 1:				
				iirHeadBumpA += (ataLowpass * BassGain);
				iirHeadBumpA -= (iirHeadBumpA * iirHeadBumpA * iirHeadBumpA * HeadBumpFreq);
				iirHeadBumpA = (invrandy * iirHeadBumpA) + (randy * iirHeadBumpB) + (randy * iirHeadBumpC);
				if (iirHeadBumpA > 0) iirHeadBumpA -= clamp;
				if (iirHeadBumpA < 0) iirHeadBumpA += clamp;
				HeadBump = iirHeadBumpA;
				break;
			case 2:
				iirHeadBumpB += (ataLowpass * BassGain);
				iirHeadBumpB -= (iirHeadBumpB * iirHeadBumpB * iirHeadBumpB * HeadBumpFreq);
				iirHeadBumpB = (randy * iirHeadBumpA) + (invrandy * iirHeadBumpB) + (randy * iirHeadBumpC);
				if (iirHeadBumpB > 0) iirHeadBumpB -= clamp;
				if (iirHeadBumpB < 0) iirHeadBumpB += clamp;
				HeadBump = iirHeadBumpB;
				break;
			case 3:
				iirHeadBumpC += (ataLowpass * BassGain);
				iirHeadBumpC -= (iirHeadBumpC * iirHeadBumpC * iirHeadBumpC * HeadBumpFreq);
				iirHeadBumpC = (randy * iirHeadBumpA) + (randy * iirHeadBumpB) + (invrandy * iirHeadBumpC);
				if (iirHeadBumpC > 0) iirHeadBumpC -= clamp;
				if (iirHeadBumpC < 0) iirHeadBumpC += clamp;
				HeadBump = iirHeadBumpC;
				break;
		}
		
		iirSampleW = (iirSampleW * (1.0 - iirAmount)) + (HeadBump * iirAmount); HeadBump -= iirSampleW;
		iirSampleX = (iirSampleX * (1.0 - iirAmount)) + (HeadBump * iirAmount); HeadBump -= iirSampleX;
		
		SubBump = HeadBump;
		iirSampleY = (iirSampleY * (1.0 - iirAmount)) + (SubBump * iirAmount); SubBump -= iirSampleY;
		
		iirDriveSampleC = (iirDriveSampleC * (1.0 - HeadBumpFreq)) + (SubBump * HeadBumpFreq); SubBump = iirDriveSampleC;
		iirDriveSampleD = (iirDriveSampleD * (1.0 - HeadBumpFreq)) + (SubBump * HeadBumpFreq); SubBump = iirDriveSampleD;
		
		
		SubBump = fabs(SubBump);
		if (SubOctave == false) {SubBump = -SubBump;}
		
		switch (bflip)
		{
			case 1:				
				iirSubBumpA += SubBump;// * BassGain);
				iirSubBumpA -= (iirSubBumpA * iirSubBumpA * iirSubBumpA * HeadBumpFreq);
				iirSubBumpA = (invrandy * iirSubBumpA) + (randy * iirSubBumpB) + (randy * iirSubBumpC);
				if (iirSubBumpA > 0) iirSubBumpA -= clamp;
				if (iirSubBumpA < 0) iirSubBumpA += clamp;
				SubBump = iirSubBumpA;
				break;
			case 2:
				iirSubBumpB += SubBump;// * BassGain);
				iirSubBumpB -= (iirSubBumpB * iirSubBumpB * iirSubBumpB * HeadBumpFreq);
				iirSubBumpB = (randy * iirSubBumpA) + (invrandy * iirSubBumpB) + (randy * iirSubBumpC);
				if (iirSubBumpB > 0) iirSubBumpB -= clamp;
				if (iirSubBumpB < 0) iirSubBumpB += clamp;
				SubBump = iirSubBumpB;
				break;
			case 3:
				iirSubBumpC += SubBump;// * BassGain);
				iirSubBumpC -= (iirSubBumpC * iirSubBumpC * iirSubBumpC * HeadBumpFreq);
				iirSubBumpC = (randy * iirSubBumpA) + (randy * iirSubBumpB) + (invrandy * iirSubBumpC);
				if (iirSubBumpC > 0) iirSubBumpC -= clamp;
				if (iirSubBumpC < 0) iirSubBumpC += clamp;
				SubBump = iirSubBumpC;
				break;
		}
		
		iirSampleZ = (iirSampleZ * (1.0 - HeadBumpFreq)) + (SubBump * HeadBumpFreq); SubBump = iirSampleZ;
		iirDriveSampleE = (iirDriveSampleE * (1.0 - iirAmount)) + (SubBump * iirAmount); SubBump = iirDriveSampleE;
		iirDriveSampleF = (iirDriveSampleF * (1.0 - iirAmount)) + (SubBump * iirAmount); SubBump = iirDriveSampleF;
		
		
		inputSampleL += (HeadBump * BassOutGain);
		inputSampleL += (SubBump * SubOutGain);
		
		inputSampleR += (HeadBump * BassOutGain);
		inputSampleR += (SubBump * SubOutGain);
		
		
		flip = !flip;
		bflip++;
		if (bflip < 1 || bflip > 3) bflip = 1;
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
