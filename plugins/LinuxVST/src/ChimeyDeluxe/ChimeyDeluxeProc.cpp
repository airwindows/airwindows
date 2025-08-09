/* ========================================
 *  ChimeyDeluxe - ChimeyDeluxe.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ChimeyDeluxe_H
#include "ChimeyDeluxe.h"
#endif

void ChimeyDeluxe::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double drive = 1.0;
	double pad = 1.0;
	angG[1] = A+0.5;
	if (pad > angG[1]) pad = angG[1];
	if (drive < angG[1]) drive = angG[1];
	angG[0] = (1.0+angG[1])*0.5; //if boost, boost half
	if (angG[0] > angG[1]) angG[0] = angG[1];
	angG[2] = B+0.5;
	if (pad > angG[2]) pad = angG[2];
	if (drive < angG[2]) drive = angG[2];
	angG[3] = C+0.5;
	if (pad > angG[3]) pad = angG[3];
	if (drive < angG[3]) drive = angG[3];
	angG[4] = D+0.5;
	if (pad > angG[4]) pad = angG[4];
	if (drive < angG[4]) drive = angG[4];
	angG[5] = E+0.5;
	if (pad > angG[5]) pad = angG[5];
	if (drive < angG[5]) drive = angG[5];
	angG[6] = F+0.5;
	if (pad > angG[6]) pad = angG[6];
	if (drive < angG[6]) drive = angG[6];
	angG[7] = G+0.5;
	if (pad > angG[7]) pad = angG[7];
	if (drive < angG[7]) drive = angG[7];
	angG[8] = H+0.5;
	if (pad > angG[8]) pad = angG[8];
	if (drive < angG[8]) drive = angG[8];
	angG[9] = I+0.5;
	if (pad > angG[9]) pad = angG[9];
	if (drive < angG[9]) drive = angG[9];
	angG[10] = J+0.5;
	if (pad > angG[10]) pad = angG[10];
	if (drive < angG[10]) drive = angG[10];
	angG[11] = (angG[10]+1.0)*0.5;
	angG[12] = (angG[11]+1.0)*0.5;
	double tune = 0.618+(overallscale*0.0055);	
	double threshold = 1.0-(drive*0.23);
	double adjSpd = ((drive*120.0)+50.0)*overallscale;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		for (int x = 0; x < 16; x++) {
			double fr = (0.92/overallscale)+(overallscale*0.01);
			double band = inputSampleL; inputSampleL = 0.0;
			for (int y = 0; y < 12; y++) {
				angAL[x][y] = (angAL[x][y]*(1.0-fr)) + ((band-angSL[x][y])*fr);
				double temp = band; band = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSL[x][y] = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleL += ((temp-band)*angG[y]);
				fr *= tune;
			}
			inputSampleL += band;
			inputSampleL *= threshold;
			inputSampleL *= (muCompL/threshold);
			if (fabs(inputSampleL) > threshold)
			{
				muCompL *= muSpdL;
				if (threshold/fabs(inputSampleL) < threshold) muCompL += threshold*fabs(inputSampleL);
				else muCompL -= threshold/fabs(inputSampleL);
				muCompL /= muSpdL;
			} else {
				muCompL *= (muSpdL*muSpdL);
				muCompL += ((1.1+threshold)-fabs(inputSampleL));
				muCompL /= (muSpdL*muSpdL);
			}
			muCompL = fmax(fmin(muCompL,1.0),threshold);
			inputSampleL *= (muCompL*muCompL);
			muSpdL = fmax(fmin(((muSpdL*(muSpdL-1.0))+(fabs(inputSampleL*adjSpd)))/muSpdL,adjSpd*2.0),adjSpd);
		}
		inputSampleL = sin(fmin(fmax(inputSampleL,-M_PI_2),M_PI_2));
				
		for (int x = 0; x < 16; x++) {
			double fr = (0.92/overallscale)+(overallscale*0.01);
			double band = inputSampleR; inputSampleR = 0.0;
			for (int y = 0; y < 12; y++) {
				angAR[x][y] = (angAR[x][y]*(1.0-fr)) + ((band-angSR[x][y])*fr);
				double temp = band; band = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSR[x][y] = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleR += ((temp-band)*angG[y]);
				fr *= tune;
			}
			inputSampleR += band;
			inputSampleR *= threshold;
			inputSampleR *= (muCompR/threshold);
			if (fabs(inputSampleR) > threshold)
			{
				muCompR *= muSpdR;
				if (threshold/fabs(inputSampleR) < threshold) muCompR += threshold*fabs(inputSampleR);
				else muCompR -= threshold/fabs(inputSampleR);
				muCompR /= muSpdR;
			} else {
				muCompR *= (muSpdR*muSpdR);
				muCompR += ((1.1+threshold)-fabs(inputSampleR));
				muCompR /= (muSpdR*muSpdR);
			}
			muCompR = fmax(fmin(muCompR,1.0),threshold);
			inputSampleR *= (muCompR*muCompR);
			muSpdR = fmax(fmin(((muSpdR*(muSpdR-1.0))+(fabs(inputSampleR*adjSpd)))/muSpdR,adjSpd*2.0),adjSpd);
		}
		inputSampleR = sin(fmin(fmax(inputSampleR,-M_PI_2),M_PI_2));
				
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

void ChimeyDeluxe::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double drive = 1.0;
	double pad = 1.0;
	angG[1] = A+0.5;
	if (pad > angG[1]) pad = angG[1];
	if (drive < angG[1]) drive = angG[1];
	angG[0] = (1.0+angG[1])*0.5; //if boost, boost half
	if (angG[0] > angG[1]) angG[0] = angG[1];
	angG[2] = B+0.5;
	if (pad > angG[2]) pad = angG[2];
	if (drive < angG[2]) drive = angG[2];
	angG[3] = C+0.5;
	if (pad > angG[3]) pad = angG[3];
	if (drive < angG[3]) drive = angG[3];
	angG[4] = D+0.5;
	if (pad > angG[4]) pad = angG[4];
	if (drive < angG[4]) drive = angG[4];
	angG[5] = E+0.5;
	if (pad > angG[5]) pad = angG[5];
	if (drive < angG[5]) drive = angG[5];
	angG[6] = F+0.5;
	if (pad > angG[6]) pad = angG[6];
	if (drive < angG[6]) drive = angG[6];
	angG[7] = G+0.5;
	if (pad > angG[7]) pad = angG[7];
	if (drive < angG[7]) drive = angG[7];
	angG[8] = H+0.5;
	if (pad > angG[8]) pad = angG[8];
	if (drive < angG[8]) drive = angG[8];
	angG[9] = I+0.5;
	if (pad > angG[9]) pad = angG[9];
	if (drive < angG[9]) drive = angG[9];
	angG[10] = J+0.5;
	if (pad > angG[10]) pad = angG[10];
	if (drive < angG[10]) drive = angG[10];
	angG[11] = (angG[10]+1.0)*0.5;
	angG[12] = (angG[11]+1.0)*0.5;
	double tune = 0.618+(overallscale*0.0055);	
	double threshold = 1.0-(drive*0.23);
	double adjSpd = ((drive*120.0)+50.0)*overallscale;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		for (int x = 0; x < 16; x++) {
			double fr = (0.92/overallscale)+(overallscale*0.01);
			double band = inputSampleL; inputSampleL = 0.0;
			for (int y = 0; y < 12; y++) {
				angAL[x][y] = (angAL[x][y]*(1.0-fr)) + ((band-angSL[x][y])*fr);
				double temp = band; band = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSL[x][y] = ((angSL[x][y]+(angAL[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleL += ((temp-band)*angG[y]);
				fr *= tune;
			}
			inputSampleL += band;
			inputSampleL *= threshold;
			inputSampleL *= (muCompL/threshold);
			if (fabs(inputSampleL) > threshold)
			{
				muCompL *= muSpdL;
				if (threshold/fabs(inputSampleL) < threshold) muCompL += threshold*fabs(inputSampleL);
				else muCompL -= threshold/fabs(inputSampleL);
				muCompL /= muSpdL;
			} else {
				muCompL *= (muSpdL*muSpdL);
				muCompL += ((1.1+threshold)-fabs(inputSampleL));
				muCompL /= (muSpdL*muSpdL);
			}
			muCompL = fmax(fmin(muCompL,1.0),threshold);
			inputSampleL *= (muCompL*muCompL);
			muSpdL = fmax(fmin(((muSpdL*(muSpdL-1.0))+(fabs(inputSampleL*adjSpd)))/muSpdL,adjSpd*2.0),adjSpd);
		}
		inputSampleL = sin(fmin(fmax(inputSampleL,-M_PI_2),M_PI_2));
		
		for (int x = 0; x < 16; x++) {
			double fr = (0.92/overallscale)+(overallscale*0.01);
			double band = inputSampleR; inputSampleR = 0.0;
			for (int y = 0; y < 12; y++) {
				angAR[x][y] = (angAR[x][y]*(1.0-fr)) + ((band-angSR[x][y])*fr);
				double temp = band; band = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				angSR[x][y] = ((angSR[x][y]+(angAR[x][y]*fr)) * (1.0-fr))+(band*fr);
				inputSampleR += ((temp-band)*angG[y]);
				fr *= tune;
			}
			inputSampleR += band;
			inputSampleR *= threshold;
			inputSampleR *= (muCompR/threshold);
			if (fabs(inputSampleR) > threshold)
			{
				muCompR *= muSpdR;
				if (threshold/fabs(inputSampleR) < threshold) muCompR += threshold*fabs(inputSampleR);
				else muCompR -= threshold/fabs(inputSampleR);
				muCompR /= muSpdR;
			} else {
				muCompR *= (muSpdR*muSpdR);
				muCompR += ((1.1+threshold)-fabs(inputSampleR));
				muCompR /= (muSpdR*muSpdR);
			}
			muCompR = fmax(fmin(muCompR,1.0),threshold);
			inputSampleR *= (muCompR*muCompR);
			muSpdR = fmax(fmin(((muSpdR*(muSpdR-1.0))+(fabs(inputSampleR*adjSpd)))/muSpdR,adjSpd*2.0),adjSpd);
		}
		inputSampleR = sin(fmin(fmax(inputSampleR,-M_PI_2),M_PI_2));
		
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
