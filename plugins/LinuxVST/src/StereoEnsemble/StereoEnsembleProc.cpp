/* ========================================
 *  StereoEnsemble - StereoEnsemble.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __StereoEnsemble_H
#include "StereoEnsemble.h"
#endif

void StereoEnsemble::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	double delayfactor = 0.66 + (A/3.0);
	
	double outlevel = B;
	
	dryL = 1.0 - (outlevel * 0.65);
	dryR = 1.0 - (outlevel * 0.65);
	
	maxdelayA = (int)(7481.0 * delayfactor);
	maxdelayB = (int)(7523.0 * delayfactor);
	
	maxdelayC = (int)(5779.0 * delayfactor);
	maxdelayD = (int)(5737.0 * delayfactor);
	
	maxdelayE = (int)(4831.0 * delayfactor);
	maxdelayF = (int)(4861.0 * delayfactor);
	
	maxdelayG = (int)(3109.0 * delayfactor);
	maxdelayH = (int)(3079.0 * delayfactor);
	
	maxdelayI = (int)(2203.0 * delayfactor);
	maxdelayJ = (int)(2213.0 * delayfactor);
	
	maxdelayK = (int)(1327.0 * delayfactor);
	maxdelayL = (int)(1321.0 * delayfactor);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a ensemble sample
			double drySampleL = inputSampleL;
			double drySampleR = inputSampleR;
			double outL = 0.0;
			double outR = 0.0;
			double temp = 0.0;
			
			dA[oneA] = inputSampleL;
			oneA--; if (oneA < 0 || oneA > maxdelayA) {oneA = maxdelayA;} temp = dA[oneA];
			outL += (temp*levelA);
			
			dB[oneB] = inputSampleR;
			oneB--; if (oneB < 0 || oneB > maxdelayB) {oneB = maxdelayB;} temp += dB[oneB];
			outR += (temp*levelB);
			
			dC[oneC] = inputSampleL;
			oneC--; if (oneC < 0 || oneC > maxdelayC) {oneC = maxdelayC;} temp = dC[oneC];
			outL += (temp*levelC);
			
			dD[oneD] = inputSampleR;
			oneD--; if (oneD < 0 || oneD > maxdelayD) {oneD = maxdelayD;} temp += dD[oneD];
			outR += (temp*levelD);
			
			dE[oneE] = inputSampleL;
			oneE--; if (oneE < 0 || oneE > maxdelayE) {oneE = maxdelayE;} temp = dE[oneE];
			outL += (temp*levelE);
			
			dF[oneF] = inputSampleR;
			oneF--; if (oneF < 0 || oneF > maxdelayF) {oneF = maxdelayF;} temp += dF[oneF];
			outR += (temp*levelF);
			
			dG[oneG] = inputSampleL;
			oneG--; if (oneG < 0 || oneG > maxdelayG) {oneG = maxdelayG;} temp = dG[oneG];
			outL += (temp*levelG);
			
			dH[oneH] = inputSampleR;
			oneH--; if (oneH < 0 || oneH > maxdelayH) {oneH = maxdelayH;} temp += dH[oneH];
			outR += (temp*levelH);
			
			dI[oneI] = inputSampleL;
			oneI--; if (oneI < 0 || oneI > maxdelayI) {oneI = maxdelayI;} temp = dI[oneI];
			outL += (temp*levelI);
			
			dJ[oneJ] = inputSampleR;
			oneJ--; if (oneJ < 0 || oneJ > maxdelayJ) {oneJ = maxdelayJ;} temp += dJ[oneJ];
			outR += (temp*levelJ);
			
			dK[oneK] = inputSampleL;
			oneK--; if (oneK < 0 || oneK > maxdelayK) {oneK = maxdelayK;} temp = dK[oneK];
			outL += (temp*levelK);
			
			dL[oneL] = inputSampleR;
			oneL--; if (oneL < 0 || oneL > maxdelayL) {oneL = maxdelayL;} temp += dL[oneL];
			outR += (temp*levelL);
			
			inputSampleL = (outL * outlevel) + (drySampleL * dryL);
			inputSampleR = (outR * outlevel) + (drySampleR * dryR);
			
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void StereoEnsemble::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check
	
	double delayfactor = 0.66 + (A/3.0);
	
	double outlevel = B;
	
	dryL = 1.0 - (outlevel * 0.65);
	dryR = 1.0 - (outlevel * 0.65);
	
	maxdelayA = (int)(7481.0 * delayfactor);
	maxdelayB = (int)(7523.0 * delayfactor);
	
	maxdelayC = (int)(5779.0 * delayfactor);
	maxdelayD = (int)(5737.0 * delayfactor);
	
	maxdelayE = (int)(4831.0 * delayfactor);
	maxdelayF = (int)(4861.0 * delayfactor);
	
	maxdelayG = (int)(3109.0 * delayfactor);
	maxdelayH = (int)(3079.0 * delayfactor);
	
	maxdelayI = (int)(2203.0 * delayfactor);
	maxdelayJ = (int)(2213.0 * delayfactor);
	
	maxdelayK = (int)(1327.0 * delayfactor);
	maxdelayL = (int)(1321.0 * delayfactor);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		cycle++;
		if (cycle == cycleEnd) { //hit the end point and we do a ensemble sample
			double drySampleL = inputSampleL;
			double drySampleR = inputSampleR;
			double outL = 0.0;
			double outR = 0.0;
			double temp = 0.0;
			
			dA[oneA] = inputSampleL;
			oneA--; if (oneA < 0 || oneA > maxdelayA) {oneA = maxdelayA;} temp = dA[oneA];
			outL += (temp*levelA);
			
			dB[oneB] = inputSampleR;
			oneB--; if (oneB < 0 || oneB > maxdelayB) {oneB = maxdelayB;} temp += dB[oneB];
			outR += (temp*levelB);
			
			dC[oneC] = inputSampleL;
			oneC--; if (oneC < 0 || oneC > maxdelayC) {oneC = maxdelayC;} temp = dC[oneC];
			outL += (temp*levelC);
			
			dD[oneD] = inputSampleR;
			oneD--; if (oneD < 0 || oneD > maxdelayD) {oneD = maxdelayD;} temp += dD[oneD];
			outR += (temp*levelD);
			
			dE[oneE] = inputSampleL;
			oneE--; if (oneE < 0 || oneE > maxdelayE) {oneE = maxdelayE;} temp = dE[oneE];
			outL += (temp*levelE);
			
			dF[oneF] = inputSampleR;
			oneF--; if (oneF < 0 || oneF > maxdelayF) {oneF = maxdelayF;} temp += dF[oneF];
			outR += (temp*levelF);
			
			dG[oneG] = inputSampleL;
			oneG--; if (oneG < 0 || oneG > maxdelayG) {oneG = maxdelayG;} temp = dG[oneG];
			outL += (temp*levelG);
			
			dH[oneH] = inputSampleR;
			oneH--; if (oneH < 0 || oneH > maxdelayH) {oneH = maxdelayH;} temp += dH[oneH];
			outR += (temp*levelH);
			
			dI[oneI] = inputSampleL;
			oneI--; if (oneI < 0 || oneI > maxdelayI) {oneI = maxdelayI;} temp = dI[oneI];
			outL += (temp*levelI);
			
			dJ[oneJ] = inputSampleR;
			oneJ--; if (oneJ < 0 || oneJ > maxdelayJ) {oneJ = maxdelayJ;} temp += dJ[oneJ];
			outR += (temp*levelJ);
			
			dK[oneK] = inputSampleL;
			oneK--; if (oneK < 0 || oneK > maxdelayK) {oneK = maxdelayK;} temp = dK[oneK];
			outL += (temp*levelK);
			
			dL[oneL] = inputSampleR;
			oneL--; if (oneL < 0 || oneL > maxdelayL) {oneL = maxdelayL;} temp += dL[oneL];
			outR += (temp*levelL);
			
			inputSampleL = (outL * outlevel) + (drySampleL * dryL);
			inputSampleR = (outR * outlevel) + (drySampleR * dryR);
			
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
