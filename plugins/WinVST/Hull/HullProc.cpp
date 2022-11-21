/* ========================================
 *  Hull - Hull.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Hull_H
#include "Hull.h"
#endif

void Hull::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 96000.0;
	overallscale *= getSampleRate();
	if (overallscale > 1.0) overallscale = 1.0; //and if you go for 192k, rather than crash
	//it just cuts out the maximum (2000) depth of averaging you can get
	double hullSetting = pow(A,3)*overallscale;
	int limitA = (hullSetting*2000.0)+1.0;
	double divisorA = 1.0/limitA;
	int limitB = (hullSetting*1000.0)+1.0;
	double divisorB = 1.0/limitB;
	int limitC = sqrt(hullSetting*2000.0)+1.0;
	double divisorC = 1.0/limitC;
	double wet = -((B*2.0)-1.0); //functions as dark/bright
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bPointer--; if (bPointer < 0) bPointer += 2000;
		bL[bPointer] = bL[bPointer+2000] = inputSampleL;
		bR[bPointer] = bR[bPointer+2000] = inputSampleR;
		
		int x = bPointer;
		double longAverageL = 0.0;
		double longAverageR = 0.0;
		double shortAverageL = 0.0;
		double shortAverageR = 0.0;
		while (x < bPointer+limitB) {
			shortAverageL += bL[x];
			shortAverageR += bR[x];
			longAverageL += bL[x];
			longAverageR += bR[x];
			x++;
		} //once shorter average is reached, continue to longer
		while (x < bPointer+limitA) {
			longAverageL += bL[x];
			longAverageR += bR[x];
			x++;
		}
		longAverageL *= divisorA;
		longAverageR *= divisorA;
		shortAverageL *= divisorB;
		shortAverageR *= divisorB;
		
		
		cPointer--; if (cPointer < 0) cPointer += 50;
		cL[cPointer] = cL[cPointer+50] = shortAverageL+(shortAverageL-longAverageL);
		cR[cPointer] = cR[cPointer+50] = shortAverageR+(shortAverageR-longAverageR);
		
		x = cPointer;
		double shortestAverageL = 0.0;
		double shortestAverageR = 0.0;
		while (x < cPointer+limitC) {
			shortestAverageL += cL[x];
			shortestAverageR += cR[x];
			x++;
		}
		shortestAverageL *= divisorC;
		shortestAverageR *= divisorC;
		
		if (wet > 0.0) {
			inputSampleL = (shortestAverageL * wet)+(drySampleL * (1.0-wet)); //dark
			inputSampleR = (shortestAverageR * wet)+(drySampleR * (1.0-wet)); //dark
		} else {
			inputSampleL = ((inputSampleL-shortestAverageL) * fabs(wet))+(drySampleL * (1.0+wet)); //bright
			inputSampleR = ((inputSampleR-shortestAverageR) * fabs(wet))+(drySampleR * (1.0+wet)); //bright
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

void Hull::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 96000.0;
	overallscale *= getSampleRate();
	if (overallscale > 1.0) overallscale = 1.0; //and if you go for 192k, rather than crash
	//it just cuts out the maximum (2000) depth of averaging you can get
	double hullSetting = pow(A,3)*overallscale;
	int limitA = (hullSetting*2000.0)+1.0;
	double divisorA = 1.0/limitA;
	int limitB = (hullSetting*1000.0)+1.0;
	double divisorB = 1.0/limitB;
	int limitC = sqrt(hullSetting*2000.0)+1.0;
	double divisorC = 1.0/limitC;
	double wet = -((B*2.0)-1.0); //functions as dark/bright
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		bPointer--; if (bPointer < 0) bPointer += 2000;
		bL[bPointer] = bL[bPointer+2000] = inputSampleL;
		bR[bPointer] = bR[bPointer+2000] = inputSampleR;
		
		int x = bPointer;
		double longAverageL = 0.0;
		double longAverageR = 0.0;
		double shortAverageL = 0.0;
		double shortAverageR = 0.0;
		while (x < bPointer+limitB) {
			shortAverageL += bL[x];
			shortAverageR += bR[x];
			longAverageL += bL[x];
			longAverageR += bR[x];
			x++;
		} //once shorter average is reached, continue to longer
		while (x < bPointer+limitA) {
			longAverageL += bL[x];
			longAverageR += bR[x];
			x++;
		}
		longAverageL *= divisorA;
		longAverageR *= divisorA;
		shortAverageL *= divisorB;
		shortAverageR *= divisorB;
		
		
		cPointer--; if (cPointer < 0) cPointer += 50;
		cL[cPointer] = cL[cPointer+50] = shortAverageL+(shortAverageL-longAverageL);
		cR[cPointer] = cR[cPointer+50] = shortAverageR+(shortAverageR-longAverageR);
		
		x = cPointer;
		double shortestAverageL = 0.0;
		double shortestAverageR = 0.0;
		while (x < cPointer+limitC) {
			shortestAverageL += cL[x];
			shortestAverageR += cR[x];
			x++;
		}
		shortestAverageL *= divisorC;
		shortestAverageR *= divisorC;
		
		if (wet > 0.0) {
			inputSampleL = (shortestAverageL * wet)+(drySampleL * (1.0-wet)); //dark
			inputSampleR = (shortestAverageR * wet)+(drySampleR * (1.0-wet)); //dark
		} else {
			inputSampleL = ((inputSampleL-shortestAverageL) * fabs(wet))+(drySampleL * (1.0+wet)); //bright
			inputSampleR = ((inputSampleR-shortestAverageR) * fabs(wet))+(drySampleR * (1.0+wet)); //bright
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
