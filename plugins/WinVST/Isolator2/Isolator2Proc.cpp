/* ========================================
 *  Isolator2 - Isolator2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#include "Isolator2.h"
#endif

void Isolator2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	biquadA[biq_freq] = pow(A,(2.0*sqrt(overallscale)))*0.4999;
	if (biquadA[biq_freq] < 0.0005) biquadA[biq_freq] = 0.0005;
	biquadG[biq_freq] = biquadF[biq_freq] = biquadE[biq_freq] = biquadD[biq_freq] = biquadC[biq_freq] = biquadB[biq_freq] = biquadA[biq_freq];
	
	double reso = pow(B,2);
	double resoBoost = reso+1.0;
	reso = 1.0-reso;
	
	biquadA[biq_reso] = 4.46570214 * resoBoost;
	biquadB[biq_reso] = 1.51387132 * resoBoost;
	biquadC[biq_reso] = 0.93979296 * resoBoost;
	biquadD[biq_reso] = 0.70710678 * resoBoost;
	biquadE[biq_reso] = 0.59051105 * resoBoost;
	biquadF[biq_reso] = 0.52972649 * resoBoost;
	biquadG[biq_reso] = 0.50316379 * resoBoost;
	
	biquadA[biq_aA0] = biquadA[biq_aB0];
	biquadA[biq_aA1] = biquadA[biq_aB1];
	biquadA[biq_aA2] = biquadA[biq_aB2];
	biquadA[biq_bA1] = biquadA[biq_bB1];
	biquadA[biq_bA2] = biquadA[biq_bB2];
	
	biquadB[biq_aA0] = biquadB[biq_aB0];
	biquadB[biq_aA1] = biquadB[biq_aB1];
	biquadB[biq_aA2] = biquadB[biq_aB2];
	biquadB[biq_bA1] = biquadB[biq_bB1];
	biquadB[biq_bA2] = biquadB[biq_bB2];
	
	biquadC[biq_aA0] = biquadC[biq_aB0];
	biquadC[biq_aA1] = biquadC[biq_aB1];
	biquadC[biq_aA2] = biquadC[biq_aB2];
	biquadC[biq_bA1] = biquadC[biq_bB1];
	biquadC[biq_bA2] = biquadC[biq_bB2];
	
	biquadD[biq_aA0] = biquadD[biq_aB0];
	biquadD[biq_aA1] = biquadD[biq_aB1];
	biquadD[biq_aA2] = biquadD[biq_aB2];
	biquadD[biq_bA1] = biquadD[biq_bB1];
	biquadD[biq_bA2] = biquadD[biq_bB2];
	
	biquadE[biq_aA0] = biquadE[biq_aB0];
	biquadE[biq_aA1] = biquadE[biq_aB1];
	biquadE[biq_aA2] = biquadE[biq_aB2];
	biquadE[biq_bA1] = biquadE[biq_bB1];
	biquadE[biq_bA2] = biquadE[biq_bB2];
	
	biquadF[biq_aA0] = biquadF[biq_aB0];
	biquadF[biq_aA1] = biquadF[biq_aB1];
	biquadF[biq_aA2] = biquadF[biq_aB2];
	biquadF[biq_bA1] = biquadF[biq_bB1];
	biquadF[biq_bA2] = biquadF[biq_bB2];
	
	biquadG[biq_aA0] = biquadG[biq_aB0];
	biquadG[biq_aA1] = biquadG[biq_aB1];
	biquadG[biq_aA2] = biquadG[biq_aB2];
	biquadG[biq_bA1] = biquadG[biq_bB1];
	biquadG[biq_bA2] = biquadG[biq_bB2];
	
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.	
	double K = tan(M_PI * biquadA[biq_freq]);
	double norm = 1.0 / (1.0 + K / biquadA[biq_reso] + K * K);
	biquadA[biq_aB0] = K * K * norm;
	biquadA[biq_aB1] = 2.0 * biquadA[biq_aB0];
	biquadA[biq_aB2] = biquadA[biq_aB0];
	biquadA[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadA[biq_bB2] = (1.0 - K / biquadA[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadB[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadB[biq_reso] + K * K);
	biquadB[biq_aB0] = K * K * norm;
	biquadB[biq_aB1] = 2.0 * biquadB[biq_aB0];
	biquadB[biq_aB2] = biquadB[biq_aB0];
	biquadB[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadB[biq_bB2] = (1.0 - K / biquadB[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadC[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadC[biq_reso] + K * K);
	biquadC[biq_aB0] = K * K * norm;
	biquadC[biq_aB1] = 2.0 * biquadC[biq_aB0];
	biquadC[biq_aB2] = biquadC[biq_aB0];
	biquadC[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadC[biq_bB2] = (1.0 - K / biquadC[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadD[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadD[biq_reso] + K * K);
	biquadD[biq_aB0] = K * K * norm;
	biquadD[biq_aB1] = 2.0 * biquadD[biq_aB0];
	biquadD[biq_aB2] = biquadD[biq_aB0];
	biquadD[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadD[biq_bB2] = (1.0 - K / biquadD[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadE[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadE[biq_reso] + K * K);
	biquadE[biq_aB0] = K * K * norm;
	biquadE[biq_aB1] = 2.0 * biquadE[biq_aB0];
	biquadE[biq_aB2] = biquadE[biq_aB0];
	biquadE[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadE[biq_bB2] = (1.0 - K / biquadE[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadF[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadF[biq_reso] + K * K);
	biquadF[biq_aB0] = K * K * norm;
	biquadF[biq_aB1] = 2.0 * biquadF[biq_aB0];
	biquadF[biq_aB2] = biquadF[biq_aB0];
	biquadF[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadF[biq_bB2] = (1.0 - K / biquadF[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadG[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadG[biq_reso] + K * K);
	biquadG[biq_aB0] = K * K * norm;
	biquadG[biq_aB1] = 2.0 * biquadG[biq_aB0];
	biquadG[biq_aB2] = biquadG[biq_aB0];
	biquadG[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadG[biq_bB2] = (1.0 - K / biquadG[biq_reso] + K * K) * norm;
	
	bool bypass = (A == 1.0);
	highA = highB;
	highB = C*reso;
	if (highB > 0.0) bypass = false;
	lowA = lowB;
	lowB = D*reso;
	if (lowB < 1.0) bypass = false;

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		
		biquadA[biq_a0] = (biquadA[biq_aA0]*temp)+(biquadA[biq_aB0]*(1.0-temp));
		biquadA[biq_a1] = (biquadA[biq_aA1]*temp)+(biquadA[biq_aB1]*(1.0-temp));
		biquadA[biq_a2] = (biquadA[biq_aA2]*temp)+(biquadA[biq_aB2]*(1.0-temp));
		biquadA[biq_b1] = (biquadA[biq_bA1]*temp)+(biquadA[biq_bB1]*(1.0-temp));
		biquadA[biq_b2] = (biquadA[biq_bA2]*temp)+(biquadA[biq_bB2]*(1.0-temp));
		
		biquadB[biq_a0] = (biquadB[biq_aA0]*temp)+(biquadB[biq_aB0]*(1.0-temp));
		biquadB[biq_a1] = (biquadB[biq_aA1]*temp)+(biquadB[biq_aB1]*(1.0-temp));
		biquadB[biq_a2] = (biquadB[biq_aA2]*temp)+(biquadB[biq_aB2]*(1.0-temp));
		biquadB[biq_b1] = (biquadB[biq_bA1]*temp)+(biquadB[biq_bB1]*(1.0-temp));
		biquadB[biq_b2] = (biquadB[biq_bA2]*temp)+(biquadB[biq_bB2]*(1.0-temp));
		
		biquadC[biq_a0] = (biquadC[biq_aA0]*temp)+(biquadC[biq_aB0]*(1.0-temp));
		biquadC[biq_a1] = (biquadC[biq_aA1]*temp)+(biquadC[biq_aB1]*(1.0-temp));
		biquadC[biq_a2] = (biquadC[biq_aA2]*temp)+(biquadC[biq_aB2]*(1.0-temp));
		biquadC[biq_b1] = (biquadC[biq_bA1]*temp)+(biquadC[biq_bB1]*(1.0-temp));
		biquadC[biq_b2] = (biquadC[biq_bA2]*temp)+(biquadC[biq_bB2]*(1.0-temp));
		
		biquadD[biq_a0] = (biquadD[biq_aA0]*temp)+(biquadD[biq_aB0]*(1.0-temp));
		biquadD[biq_a1] = (biquadD[biq_aA1]*temp)+(biquadD[biq_aB1]*(1.0-temp));
		biquadD[biq_a2] = (biquadD[biq_aA2]*temp)+(biquadD[biq_aB2]*(1.0-temp));
		biquadD[biq_b1] = (biquadD[biq_bA1]*temp)+(biquadD[biq_bB1]*(1.0-temp));
		biquadD[biq_b2] = (biquadD[biq_bA2]*temp)+(biquadD[biq_bB2]*(1.0-temp));
		
		biquadE[biq_a0] = (biquadE[biq_aA0]*temp)+(biquadE[biq_aB0]*(1.0-temp));
		biquadE[biq_a1] = (biquadE[biq_aA1]*temp)+(biquadE[biq_aB1]*(1.0-temp));
		biquadE[biq_a2] = (biquadE[biq_aA2]*temp)+(biquadE[biq_aB2]*(1.0-temp));
		biquadE[biq_b1] = (biquadE[biq_bA1]*temp)+(biquadE[biq_bB1]*(1.0-temp));
		biquadE[biq_b2] = (biquadE[biq_bA2]*temp)+(biquadE[biq_bB2]*(1.0-temp));
		
		biquadF[biq_a0] = (biquadF[biq_aA0]*temp)+(biquadF[biq_aB0]*(1.0-temp));
		biquadF[biq_a1] = (biquadF[biq_aA1]*temp)+(biquadF[biq_aB1]*(1.0-temp));
		biquadF[biq_a2] = (biquadF[biq_aA2]*temp)+(biquadF[biq_aB2]*(1.0-temp));
		biquadF[biq_b1] = (biquadF[biq_bA1]*temp)+(biquadF[biq_bB1]*(1.0-temp));
		biquadF[biq_b2] = (biquadF[biq_bA2]*temp)+(biquadF[biq_bB2]*(1.0-temp));
		
		biquadG[biq_a0] = (biquadG[biq_aA0]*temp)+(biquadG[biq_aB0]*(1.0-temp));
		biquadG[biq_a1] = (biquadG[biq_aA1]*temp)+(biquadG[biq_aB1]*(1.0-temp));
		biquadG[biq_a2] = (biquadG[biq_aA2]*temp)+(biquadG[biq_aB2]*(1.0-temp));
		biquadG[biq_b1] = (biquadG[biq_bA1]*temp)+(biquadG[biq_bB1]*(1.0-temp));
		biquadG[biq_b2] = (biquadG[biq_bA2]*temp)+(biquadG[biq_bB2]*(1.0-temp));
		
		//this is the interpolation code for the biquad
		double high = (highA*temp)+(highB*(1.0-temp));
		double low = (lowA*temp)+(lowB*(1.0-temp));
		
		double outSample = (inputSampleL * biquadA[biq_a0]) + biquadA[biq_sL1];
		biquadA[biq_sL1] = (inputSampleL * biquadA[biq_a1]) - (outSample * biquadA[biq_b1]) + biquadA[biq_sL2];
		biquadA[biq_sL2] = (inputSampleL * biquadA[biq_a2]) - (outSample * biquadA[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadA[biq_a0]) + biquadA[biq_sR1];
		biquadA[biq_sR1] = (inputSampleR * biquadA[biq_a1]) - (outSample * biquadA[biq_b1]) + biquadA[biq_sR2];
		biquadA[biq_sR2] = (inputSampleR * biquadA[biq_a2]) - (outSample * biquadA[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadB[biq_a0]) + biquadB[biq_sL1];
		biquadB[biq_sL1] = (inputSampleL * biquadB[biq_a1]) - (outSample * biquadB[biq_b1]) + biquadB[biq_sL2];
		biquadB[biq_sL2] = (inputSampleL * biquadB[biq_a2]) - (outSample * biquadB[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadB[biq_a0]) + biquadB[biq_sR1];
		biquadB[biq_sR1] = (inputSampleR * biquadB[biq_a1]) - (outSample * biquadB[biq_b1]) + biquadB[biq_sR2];
		biquadB[biq_sR2] = (inputSampleR * biquadB[biq_a2]) - (outSample * biquadB[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadC[biq_a0]) + biquadC[biq_sL1];
		biquadC[biq_sL1] = (inputSampleL * biquadC[biq_a1]) - (outSample * biquadC[biq_b1]) + biquadC[biq_sL2];
		biquadC[biq_sL2] = (inputSampleL * biquadC[biq_a2]) - (outSample * biquadC[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadC[biq_a0]) + biquadC[biq_sR1];
		biquadC[biq_sR1] = (inputSampleR * biquadC[biq_a1]) - (outSample * biquadC[biq_b1]) + biquadC[biq_sR2];
		biquadC[biq_sR2] = (inputSampleR * biquadC[biq_a2]) - (outSample * biquadC[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadD[biq_a0]) + biquadD[biq_sL1];
		biquadD[biq_sL1] = (inputSampleL * biquadD[biq_a1]) - (outSample * biquadD[biq_b1]) + biquadD[biq_sL2];
		biquadD[biq_sL2] = (inputSampleL * biquadD[biq_a2]) - (outSample * biquadD[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadD[biq_a0]) + biquadD[biq_sR1];
		biquadD[biq_sR1] = (inputSampleR * biquadD[biq_a1]) - (outSample * biquadD[biq_b1]) + biquadD[biq_sR2];
		biquadD[biq_sR2] = (inputSampleR * biquadD[biq_a2]) - (outSample * biquadD[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadE[biq_a0]) + biquadE[biq_sL1];
		biquadE[biq_sL1] = (inputSampleL * biquadE[biq_a1]) - (outSample * biquadE[biq_b1]) + biquadE[biq_sL2];
		biquadE[biq_sL2] = (inputSampleL * biquadE[biq_a2]) - (outSample * biquadE[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadE[biq_a0]) + biquadE[biq_sR1];
		biquadE[biq_sR1] = (inputSampleR * biquadE[biq_a1]) - (outSample * biquadE[biq_b1]) + biquadE[biq_sR2];
		biquadE[biq_sR2] = (inputSampleR * biquadE[biq_a2]) - (outSample * biquadE[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadF[biq_a0]) + biquadF[biq_sL1];
		biquadF[biq_sL1] = (inputSampleL * biquadF[biq_a1]) - (outSample * biquadF[biq_b1]) + biquadF[biq_sL2];
		biquadF[biq_sL2] = (inputSampleL * biquadF[biq_a2]) - (outSample * biquadF[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadF[biq_a0]) + biquadF[biq_sR1];
		biquadF[biq_sR1] = (inputSampleR * biquadF[biq_a1]) - (outSample * biquadF[biq_b1]) + biquadF[biq_sR2];
		biquadF[biq_sR2] = (inputSampleR * biquadF[biq_a2]) - (outSample * biquadF[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadG[biq_a0]) + biquadG[biq_sL1];
		biquadG[biq_sL1] = (inputSampleL * biquadG[biq_a1]) - (outSample * biquadG[biq_b1]) + biquadG[biq_sL2];
		biquadG[biq_sL2] = (inputSampleL * biquadG[biq_a2]) - (outSample * biquadG[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadG[biq_a0]) + biquadG[biq_sR1];
		biquadG[biq_sR1] = (inputSampleR * biquadG[biq_a1]) - (outSample * biquadG[biq_b1]) + biquadG[biq_sR2];
		biquadG[biq_sR2] = (inputSampleR * biquadG[biq_a2]) - (outSample * biquadG[biq_b2]);
		inputSampleR = outSample;
		
		if (bypass) {
			inputSampleL = drySampleL;
			inputSampleR = drySampleR;
		} else {
			inputSampleL = (inputSampleL * low) + ((drySampleL - inputSampleL)*high);
			inputSampleR = (inputSampleR * low) + ((drySampleR - inputSampleR)*high);
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

void Isolator2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	biquadA[biq_freq] = pow(A,(2.0*sqrt(overallscale)))*0.4999;
	if (biquadA[biq_freq] < 0.0005) biquadA[biq_freq] = 0.0005;
	biquadG[biq_freq] = biquadF[biq_freq] = biquadE[biq_freq] = biquadD[biq_freq] = biquadC[biq_freq] = biquadB[biq_freq] = biquadA[biq_freq];
	
	double reso = pow(B,2);
	double resoBoost = reso+1.0;
	reso = 1.0-reso;
	
	biquadA[biq_reso] = 4.46570214 * resoBoost;
	biquadB[biq_reso] = 1.51387132 * resoBoost;
	biquadC[biq_reso] = 0.93979296 * resoBoost;
	biquadD[biq_reso] = 0.70710678 * resoBoost;
	biquadE[biq_reso] = 0.59051105 * resoBoost;
	biquadF[biq_reso] = 0.52972649 * resoBoost;
	biquadG[biq_reso] = 0.50316379 * resoBoost;
	
	biquadA[biq_aA0] = biquadA[biq_aB0];
	biquadA[biq_aA1] = biquadA[biq_aB1];
	biquadA[biq_aA2] = biquadA[biq_aB2];
	biquadA[biq_bA1] = biquadA[biq_bB1];
	biquadA[biq_bA2] = biquadA[biq_bB2];
	
	biquadB[biq_aA0] = biquadB[biq_aB0];
	biquadB[biq_aA1] = biquadB[biq_aB1];
	biquadB[biq_aA2] = biquadB[biq_aB2];
	biquadB[biq_bA1] = biquadB[biq_bB1];
	biquadB[biq_bA2] = biquadB[biq_bB2];
	
	biquadC[biq_aA0] = biquadC[biq_aB0];
	biquadC[biq_aA1] = biquadC[biq_aB1];
	biquadC[biq_aA2] = biquadC[biq_aB2];
	biquadC[biq_bA1] = biquadC[biq_bB1];
	biquadC[biq_bA2] = biquadC[biq_bB2];
	
	biquadD[biq_aA0] = biquadD[biq_aB0];
	biquadD[biq_aA1] = biquadD[biq_aB1];
	biquadD[biq_aA2] = biquadD[biq_aB2];
	biquadD[biq_bA1] = biquadD[biq_bB1];
	biquadD[biq_bA2] = biquadD[biq_bB2];
	
	biquadE[biq_aA0] = biquadE[biq_aB0];
	biquadE[biq_aA1] = biquadE[biq_aB1];
	biquadE[biq_aA2] = biquadE[biq_aB2];
	biquadE[biq_bA1] = biquadE[biq_bB1];
	biquadE[biq_bA2] = biquadE[biq_bB2];
	
	biquadF[biq_aA0] = biquadF[biq_aB0];
	biquadF[biq_aA1] = biquadF[biq_aB1];
	biquadF[biq_aA2] = biquadF[biq_aB2];
	biquadF[biq_bA1] = biquadF[biq_bB1];
	biquadF[biq_bA2] = biquadF[biq_bB2];
	
	biquadG[biq_aA0] = biquadG[biq_aB0];
	biquadG[biq_aA1] = biquadG[biq_aB1];
	biquadG[biq_aA2] = biquadG[biq_aB2];
	biquadG[biq_bA1] = biquadG[biq_bB1];
	biquadG[biq_bA2] = biquadG[biq_bB2];
	
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.	
	double K = tan(M_PI * biquadA[biq_freq]);
	double norm = 1.0 / (1.0 + K / biquadA[biq_reso] + K * K);
	biquadA[biq_aB0] = K * K * norm;
	biquadA[biq_aB1] = 2.0 * biquadA[biq_aB0];
	biquadA[biq_aB2] = biquadA[biq_aB0];
	biquadA[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadA[biq_bB2] = (1.0 - K / biquadA[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadB[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadB[biq_reso] + K * K);
	biquadB[biq_aB0] = K * K * norm;
	biquadB[biq_aB1] = 2.0 * biquadB[biq_aB0];
	biquadB[biq_aB2] = biquadB[biq_aB0];
	biquadB[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadB[biq_bB2] = (1.0 - K / biquadB[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadC[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadC[biq_reso] + K * K);
	biquadC[biq_aB0] = K * K * norm;
	biquadC[biq_aB1] = 2.0 * biquadC[biq_aB0];
	biquadC[biq_aB2] = biquadC[biq_aB0];
	biquadC[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadC[biq_bB2] = (1.0 - K / biquadC[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadD[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadD[biq_reso] + K * K);
	biquadD[biq_aB0] = K * K * norm;
	biquadD[biq_aB1] = 2.0 * biquadD[biq_aB0];
	biquadD[biq_aB2] = biquadD[biq_aB0];
	biquadD[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadD[biq_bB2] = (1.0 - K / biquadD[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadE[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadE[biq_reso] + K * K);
	biquadE[biq_aB0] = K * K * norm;
	biquadE[biq_aB1] = 2.0 * biquadE[biq_aB0];
	biquadE[biq_aB2] = biquadE[biq_aB0];
	biquadE[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadE[biq_bB2] = (1.0 - K / biquadE[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadF[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadF[biq_reso] + K * K);
	biquadF[biq_aB0] = K * K * norm;
	biquadF[biq_aB1] = 2.0 * biquadF[biq_aB0];
	biquadF[biq_aB2] = biquadF[biq_aB0];
	biquadF[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadF[biq_bB2] = (1.0 - K / biquadF[biq_reso] + K * K) * norm;
	
	K = tan(M_PI * biquadG[biq_freq]);
	norm = 1.0 / (1.0 + K / biquadG[biq_reso] + K * K);
	biquadG[biq_aB0] = K * K * norm;
	biquadG[biq_aB1] = 2.0 * biquadG[biq_aB0];
	biquadG[biq_aB2] = biquadG[biq_aB0];
	biquadG[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
	biquadG[biq_bB2] = (1.0 - K / biquadG[biq_reso] + K * K) * norm;
	
	bool bypass = (A == 1.0);
	highA = highB;
	highB = C*reso;
	if (highB > 0.0) bypass = false;
	lowA = lowB;
	lowB = D*reso;
	if (lowB < 1.0) bypass = false;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		
		biquadA[biq_a0] = (biquadA[biq_aA0]*temp)+(biquadA[biq_aB0]*(1.0-temp));
		biquadA[biq_a1] = (biquadA[biq_aA1]*temp)+(biquadA[biq_aB1]*(1.0-temp));
		biquadA[biq_a2] = (biquadA[biq_aA2]*temp)+(biquadA[biq_aB2]*(1.0-temp));
		biquadA[biq_b1] = (biquadA[biq_bA1]*temp)+(biquadA[biq_bB1]*(1.0-temp));
		biquadA[biq_b2] = (biquadA[biq_bA2]*temp)+(biquadA[biq_bB2]*(1.0-temp));
		
		biquadB[biq_a0] = (biquadB[biq_aA0]*temp)+(biquadB[biq_aB0]*(1.0-temp));
		biquadB[biq_a1] = (biquadB[biq_aA1]*temp)+(biquadB[biq_aB1]*(1.0-temp));
		biquadB[biq_a2] = (biquadB[biq_aA2]*temp)+(biquadB[biq_aB2]*(1.0-temp));
		biquadB[biq_b1] = (biquadB[biq_bA1]*temp)+(biquadB[biq_bB1]*(1.0-temp));
		biquadB[biq_b2] = (biquadB[biq_bA2]*temp)+(biquadB[biq_bB2]*(1.0-temp));
		
		biquadC[biq_a0] = (biquadC[biq_aA0]*temp)+(biquadC[biq_aB0]*(1.0-temp));
		biquadC[biq_a1] = (biquadC[biq_aA1]*temp)+(biquadC[biq_aB1]*(1.0-temp));
		biquadC[biq_a2] = (biquadC[biq_aA2]*temp)+(biquadC[biq_aB2]*(1.0-temp));
		biquadC[biq_b1] = (biquadC[biq_bA1]*temp)+(biquadC[biq_bB1]*(1.0-temp));
		biquadC[biq_b2] = (biquadC[biq_bA2]*temp)+(biquadC[biq_bB2]*(1.0-temp));
		
		biquadD[biq_a0] = (biquadD[biq_aA0]*temp)+(biquadD[biq_aB0]*(1.0-temp));
		biquadD[biq_a1] = (biquadD[biq_aA1]*temp)+(biquadD[biq_aB1]*(1.0-temp));
		biquadD[biq_a2] = (biquadD[biq_aA2]*temp)+(biquadD[biq_aB2]*(1.0-temp));
		biquadD[biq_b1] = (biquadD[biq_bA1]*temp)+(biquadD[biq_bB1]*(1.0-temp));
		biquadD[biq_b2] = (biquadD[biq_bA2]*temp)+(biquadD[biq_bB2]*(1.0-temp));
		
		biquadE[biq_a0] = (biquadE[biq_aA0]*temp)+(biquadE[biq_aB0]*(1.0-temp));
		biquadE[biq_a1] = (biquadE[biq_aA1]*temp)+(biquadE[biq_aB1]*(1.0-temp));
		biquadE[biq_a2] = (biquadE[biq_aA2]*temp)+(biquadE[biq_aB2]*(1.0-temp));
		biquadE[biq_b1] = (biquadE[biq_bA1]*temp)+(biquadE[biq_bB1]*(1.0-temp));
		biquadE[biq_b2] = (biquadE[biq_bA2]*temp)+(biquadE[biq_bB2]*(1.0-temp));
		
		biquadF[biq_a0] = (biquadF[biq_aA0]*temp)+(biquadF[biq_aB0]*(1.0-temp));
		biquadF[biq_a1] = (biquadF[biq_aA1]*temp)+(biquadF[biq_aB1]*(1.0-temp));
		biquadF[biq_a2] = (biquadF[biq_aA2]*temp)+(biquadF[biq_aB2]*(1.0-temp));
		biquadF[biq_b1] = (biquadF[biq_bA1]*temp)+(biquadF[biq_bB1]*(1.0-temp));
		biquadF[biq_b2] = (biquadF[biq_bA2]*temp)+(biquadF[biq_bB2]*(1.0-temp));
		
		biquadG[biq_a0] = (biquadG[biq_aA0]*temp)+(biquadG[biq_aB0]*(1.0-temp));
		biquadG[biq_a1] = (biquadG[biq_aA1]*temp)+(biquadG[biq_aB1]*(1.0-temp));
		biquadG[biq_a2] = (biquadG[biq_aA2]*temp)+(biquadG[biq_aB2]*(1.0-temp));
		biquadG[biq_b1] = (biquadG[biq_bA1]*temp)+(biquadG[biq_bB1]*(1.0-temp));
		biquadG[biq_b2] = (biquadG[biq_bA2]*temp)+(biquadG[biq_bB2]*(1.0-temp));
		
		//this is the interpolation code for the biquad
		double high = (highA*temp)+(highB*(1.0-temp));
		double low = (lowA*temp)+(lowB*(1.0-temp));
		
		double outSample = (inputSampleL * biquadA[biq_a0]) + biquadA[biq_sL1];
		biquadA[biq_sL1] = (inputSampleL * biquadA[biq_a1]) - (outSample * biquadA[biq_b1]) + biquadA[biq_sL2];
		biquadA[biq_sL2] = (inputSampleL * biquadA[biq_a2]) - (outSample * biquadA[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadA[biq_a0]) + biquadA[biq_sR1];
		biquadA[biq_sR1] = (inputSampleR * biquadA[biq_a1]) - (outSample * biquadA[biq_b1]) + biquadA[biq_sR2];
		biquadA[biq_sR2] = (inputSampleR * biquadA[biq_a2]) - (outSample * biquadA[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadB[biq_a0]) + biquadB[biq_sL1];
		biquadB[biq_sL1] = (inputSampleL * biquadB[biq_a1]) - (outSample * biquadB[biq_b1]) + biquadB[biq_sL2];
		biquadB[biq_sL2] = (inputSampleL * biquadB[biq_a2]) - (outSample * biquadB[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadB[biq_a0]) + biquadB[biq_sR1];
		biquadB[biq_sR1] = (inputSampleR * biquadB[biq_a1]) - (outSample * biquadB[biq_b1]) + biquadB[biq_sR2];
		biquadB[biq_sR2] = (inputSampleR * biquadB[biq_a2]) - (outSample * biquadB[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadC[biq_a0]) + biquadC[biq_sL1];
		biquadC[biq_sL1] = (inputSampleL * biquadC[biq_a1]) - (outSample * biquadC[biq_b1]) + biquadC[biq_sL2];
		biquadC[biq_sL2] = (inputSampleL * biquadC[biq_a2]) - (outSample * biquadC[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadC[biq_a0]) + biquadC[biq_sR1];
		biquadC[biq_sR1] = (inputSampleR * biquadC[biq_a1]) - (outSample * biquadC[biq_b1]) + biquadC[biq_sR2];
		biquadC[biq_sR2] = (inputSampleR * biquadC[biq_a2]) - (outSample * biquadC[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadD[biq_a0]) + biquadD[biq_sL1];
		biquadD[biq_sL1] = (inputSampleL * biquadD[biq_a1]) - (outSample * biquadD[biq_b1]) + biquadD[biq_sL2];
		biquadD[biq_sL2] = (inputSampleL * biquadD[biq_a2]) - (outSample * biquadD[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadD[biq_a0]) + biquadD[biq_sR1];
		biquadD[biq_sR1] = (inputSampleR * biquadD[biq_a1]) - (outSample * biquadD[biq_b1]) + biquadD[biq_sR2];
		biquadD[biq_sR2] = (inputSampleR * biquadD[biq_a2]) - (outSample * biquadD[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadE[biq_a0]) + biquadE[biq_sL1];
		biquadE[biq_sL1] = (inputSampleL * biquadE[biq_a1]) - (outSample * biquadE[biq_b1]) + biquadE[biq_sL2];
		biquadE[biq_sL2] = (inputSampleL * biquadE[biq_a2]) - (outSample * biquadE[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadE[biq_a0]) + biquadE[biq_sR1];
		biquadE[biq_sR1] = (inputSampleR * biquadE[biq_a1]) - (outSample * biquadE[biq_b1]) + biquadE[biq_sR2];
		biquadE[biq_sR2] = (inputSampleR * biquadE[biq_a2]) - (outSample * biquadE[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadF[biq_a0]) + biquadF[biq_sL1];
		biquadF[biq_sL1] = (inputSampleL * biquadF[biq_a1]) - (outSample * biquadF[biq_b1]) + biquadF[biq_sL2];
		biquadF[biq_sL2] = (inputSampleL * biquadF[biq_a2]) - (outSample * biquadF[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadF[biq_a0]) + biquadF[biq_sR1];
		biquadF[biq_sR1] = (inputSampleR * biquadF[biq_a1]) - (outSample * biquadF[biq_b1]) + biquadF[biq_sR2];
		biquadF[biq_sR2] = (inputSampleR * biquadF[biq_a2]) - (outSample * biquadF[biq_b2]);
		inputSampleR = outSample;
		
		outSample = (inputSampleL * biquadG[biq_a0]) + biquadG[biq_sL1];
		biquadG[biq_sL1] = (inputSampleL * biquadG[biq_a1]) - (outSample * biquadG[biq_b1]) + biquadG[biq_sL2];
		biquadG[biq_sL2] = (inputSampleL * biquadG[biq_a2]) - (outSample * biquadG[biq_b2]);
		inputSampleL = outSample;
		outSample = (inputSampleR * biquadG[biq_a0]) + biquadG[biq_sR1];
		biquadG[biq_sR1] = (inputSampleR * biquadG[biq_a1]) - (outSample * biquadG[biq_b1]) + biquadG[biq_sR2];
		biquadG[biq_sR2] = (inputSampleR * biquadG[biq_a2]) - (outSample * biquadG[biq_b2]);
		inputSampleR = outSample;
		
		if (bypass) {
			inputSampleL = drySampleL;
			inputSampleR = drySampleR;
		} else {
			inputSampleL = (inputSampleL * low) + ((drySampleL - inputSampleL)*high);
			inputSampleR = (inputSampleR * low) + ((drySampleR - inputSampleR)*high);
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
