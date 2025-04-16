/* ========================================
 *  SmoothEQ - SmoothEQ.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SmoothEQ_H
#include "SmoothEQ.h"
#endif

void SmoothEQ::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double trebleGain = A*2.0;
	double midGain = B*2.0;
	double bassGain = C*2.0; //amount ends up being pow(gain,3)
	//simple three band to adjust
	
	besselA[biq_freq] = pow(D,2) * (0.25/overallscale) * 1.9047076123;
	if (besselA[biq_freq] < 0.00025) besselA[biq_freq] = 0.00025;
	if (besselA[biq_freq] > 0.4999) besselA[biq_freq] = 0.4999;
	besselA[biq_reso] = 1.02331395383;
	besselB[biq_freq] = pow(D,2) * (0.25/overallscale) * 1.68916826762;
	if (besselB[biq_freq] < 0.00025) besselB[biq_freq] = 0.00025;
	if (besselB[biq_freq] > 0.4999) besselB[biq_freq] = 0.4999;
	besselB[biq_reso] = 0.611194546878;
	besselC[biq_freq] = pow(D,2) * (0.25/overallscale) * 1.60391912877;
	if (besselC[biq_freq] < 0.00025) besselC[biq_freq] = 0.00025;
	if (besselC[biq_freq] > 0.4999) besselC[biq_freq] = 0.4999;
	besselC[biq_reso] = 0.510317824749;
	
	besselD[biq_freq] = pow(E,4) * (0.25/overallscale) * 1.9047076123;
	if (besselD[biq_freq] < 0.00025) besselD[biq_freq] = 0.00025;
	if (besselD[biq_freq] > 0.4999) besselD[biq_freq] = 0.4999;
	besselD[biq_reso] = 1.02331395383;
	besselE[biq_freq] = pow(E,4) * (0.25/overallscale) * 1.68916826762;
	if (besselE[biq_freq] < 0.00025) besselE[biq_freq] = 0.00025;
	if (besselE[biq_freq] > 0.4999) besselE[biq_freq] = 0.4999;
	besselE[biq_reso] = 0.611194546878;
	besselF[biq_freq] = pow(E,4) * (0.25/overallscale) * 1.60391912877;
	if (besselF[biq_freq] < 0.00025) besselF[biq_freq] = 0.00025;
	if (besselF[biq_freq] > 0.4999) besselF[biq_freq] = 0.4999;
	besselF[biq_reso] = 0.510317824749;
	
	double K = tan(M_PI * besselA[biq_freq]);
	double norm = 1.0 / (1.0 + K / besselA[biq_reso] + K * K);
	besselA[biq_a0] = K * K * norm;
	besselA[biq_a1] = 2.0 * besselA[biq_a0];
	besselA[biq_a2] = besselA[biq_a0];
	besselA[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselA[biq_b2] = (1.0 - K / besselA[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselB[biq_freq]);
	norm = 1.0 / (1.0 + K / besselB[biq_reso] + K * K);
	besselB[biq_a0] = K * K * norm;
	besselB[biq_a1] = 2.0 * besselB[biq_a0];
	besselB[biq_a2] = besselB[biq_a0];
	besselB[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselB[biq_b2] = (1.0 - K / besselB[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselC[biq_freq]);
	norm = 1.0 / (1.0 + K / besselC[biq_reso] + K * K);
	besselC[biq_a0] = K * K * norm;
	besselC[biq_a1] = 2.0 * besselC[biq_a0];
	besselC[biq_a2] = besselC[biq_a0];
	besselC[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselC[biq_b2] = (1.0 - K / besselC[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselD[biq_freq]);
	norm = 1.0 / (1.0 + K / besselD[biq_reso] + K * K);
	besselD[biq_a0] = K * K * norm;
	besselD[biq_a1] = 2.0 * besselD[biq_a0];
	besselD[biq_a2] = besselD[biq_a0];
	besselD[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselD[biq_b2] = (1.0 - K / besselD[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselE[biq_freq]);
	norm = 1.0 / (1.0 + K / besselE[biq_reso] + K * K);
	besselE[biq_a0] = K * K * norm;
	besselE[biq_a1] = 2.0 * besselE[biq_a0];
	besselE[biq_a2] = besselE[biq_a0];
	besselE[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselE[biq_b2] = (1.0 - K / besselE[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselF[biq_freq]);
	norm = 1.0 / (1.0 + K / besselF[biq_reso] + K * K);
	besselF[biq_a0] = K * K * norm;
	besselF[biq_a1] = 2.0 * besselF[biq_a0];
	besselF[biq_a2] = besselF[biq_a0];
	besselF[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselF[biq_b2] = (1.0 - K / besselF[biq_reso] + K * K) * norm;
	
	while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		double trebleL = inputSampleL;		
		double outSample = (trebleL * besselA[biq_a0]) + besselA[biq_sL1];
		besselA[biq_sL1] = (trebleL * besselA[biq_a1]) - (outSample * besselA[biq_b1]) + besselA[biq_sL2];
		besselA[biq_sL2] = (trebleL * besselA[biq_a2]) - (outSample * besselA[biq_b2]);
		double midL = outSample; trebleL -= midL;
		outSample = (midL * besselD[biq_a0]) + besselD[biq_sL1];
		besselD[biq_sL1] = (midL * besselD[biq_a1]) - (outSample * besselD[biq_b1]) + besselD[biq_sL2];
		besselD[biq_sL2] = (midL * besselD[biq_a2]) - (outSample * besselD[biq_b2]);
		double bassL = outSample; midL -= bassL;
		trebleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		outSample = (trebleL * besselB[biq_a0]) + besselB[biq_sL1];
		besselB[biq_sL1] = (trebleL * besselB[biq_a1]) - (outSample * besselB[biq_b1]) + besselB[biq_sL2];
		besselB[biq_sL2] = (trebleL * besselB[biq_a2]) - (outSample * besselB[biq_b2]);
		midL = outSample; trebleL -= midL;
		outSample = (midL * besselE[biq_a0]) + besselE[biq_sL1];
		besselE[biq_sL1] = (midL * besselE[biq_a1]) - (outSample * besselE[biq_b1]) + besselE[biq_sL2];
		besselE[biq_sL2] = (midL * besselE[biq_a2]) - (outSample * besselE[biq_b2]);
		bassL = outSample; midL -= bassL;
		trebleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		outSample = (trebleL * besselC[biq_a0]) + besselC[biq_sL1];
		besselC[biq_sL1] = (trebleL * besselC[biq_a1]) - (outSample * besselC[biq_b1]) + besselC[biq_sL2];
		besselC[biq_sL2] = (trebleL * besselC[biq_a2]) - (outSample * besselC[biq_b2]);
		midL = outSample; trebleL -= midL;
		outSample = (midL * besselF[biq_a0]) + besselF[biq_sL1];
		besselF[biq_sL1] = (midL * besselF[biq_a1]) - (outSample * besselF[biq_b1]) + besselF[biq_sL2];
		besselF[biq_sL2] = (midL * besselF[biq_a2]) - (outSample * besselF[biq_b2]);
		bassL = outSample; midL -= bassL;
		inputSampleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		
		double trebleR = inputSampleR;		
		outSample = (trebleR * besselA[biq_a0]) + besselA[biq_sR1];
		besselA[biq_sR1] = (trebleR * besselA[biq_a1]) - (outSample * besselA[biq_b1]) + besselA[biq_sR2];
		besselA[biq_sR2] = (trebleR * besselA[biq_a2]) - (outSample * besselA[biq_b2]);
		double midR = outSample; trebleR -= midR;
		outSample = (midR * besselD[biq_a0]) + besselD[biq_sR1];
		besselD[biq_sR1] = (midR * besselD[biq_a1]) - (outSample * besselD[biq_b1]) + besselD[biq_sR2];
		besselD[biq_sR2] = (midR * besselD[biq_a2]) - (outSample * besselD[biq_b2]);
		double bassR = outSample; midR -= bassR;
		trebleR = (bassR*bassGain) + (midR*midGain) + (trebleR*trebleGain);
		outSample = (trebleR * besselB[biq_a0]) + besselB[biq_sR1];
		besselB[biq_sR1] = (trebleR * besselB[biq_a1]) - (outSample * besselB[biq_b1]) + besselB[biq_sR2];
		besselB[biq_sR2] = (trebleR * besselB[biq_a2]) - (outSample * besselB[biq_b2]);
		midR = outSample; trebleR -= midR;
		outSample = (midR * besselE[biq_a0]) + besselE[biq_sR1];
		besselE[biq_sR1] = (midR * besselE[biq_a1]) - (outSample * besselE[biq_b1]) + besselE[biq_sR2];
		besselE[biq_sR2] = (midR * besselE[biq_a2]) - (outSample * besselE[biq_b2]);
		bassR = outSample; midR -= bassR;
		trebleR = (bassR*bassGain) + (midR*midGain) + (trebleR*trebleGain);
		outSample = (trebleR * besselC[biq_a0]) + besselC[biq_sR1];
		besselC[biq_sR1] = (trebleR * besselC[biq_a1]) - (outSample * besselC[biq_b1]) + besselC[biq_sR2];
		besselC[biq_sR2] = (trebleR * besselC[biq_a2]) - (outSample * besselC[biq_b2]);
		midR = outSample; trebleR -= midR;
		outSample = (midR * besselF[biq_a0]) + besselF[biq_sR1];
		besselF[biq_sR1] = (midR * besselF[biq_a1]) - (outSample * besselF[biq_b1]) + besselF[biq_sR2];
		besselF[biq_sR2] = (midR * besselF[biq_a2]) - (outSample * besselF[biq_b2]);
		bassR = outSample; midR -= bassR;
		inputSampleR = (bassR*bassGain) + (midR*midGain) + (trebleR*trebleGain);
		
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

void SmoothEQ::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double trebleGain = A*2.0;
	double midGain = B*2.0;
	double bassGain = C*2.0; //amount ends up being pow(gain,3)
	//simple three band to adjust
	
	besselA[biq_freq] = pow(D,2) * (0.25/overallscale) * 1.9047076123;
	if (besselA[biq_freq] < 0.00025) besselA[biq_freq] = 0.00025;
	if (besselA[biq_freq] > 0.4999) besselA[biq_freq] = 0.4999;
	besselA[biq_reso] = 1.02331395383;
	besselB[biq_freq] = pow(D,2) * (0.25/overallscale) * 1.68916826762;
	if (besselB[biq_freq] < 0.00025) besselB[biq_freq] = 0.00025;
	if (besselB[biq_freq] > 0.4999) besselB[biq_freq] = 0.4999;
	besselB[biq_reso] = 0.611194546878;
	besselC[biq_freq] = pow(D,2) * (0.25/overallscale) * 1.60391912877;
	if (besselC[biq_freq] < 0.00025) besselC[biq_freq] = 0.00025;
	if (besselC[biq_freq] > 0.4999) besselC[biq_freq] = 0.4999;
	besselC[biq_reso] = 0.510317824749;
	
	besselD[biq_freq] = pow(E,4) * (0.25/overallscale) * 1.9047076123;
	if (besselD[biq_freq] < 0.00025) besselD[biq_freq] = 0.00025;
	if (besselD[biq_freq] > 0.4999) besselD[biq_freq] = 0.4999;
	besselD[biq_reso] = 1.02331395383;
	besselE[biq_freq] = pow(E,4) * (0.25/overallscale) * 1.68916826762;
	if (besselE[biq_freq] < 0.00025) besselE[biq_freq] = 0.00025;
	if (besselE[biq_freq] > 0.4999) besselE[biq_freq] = 0.4999;
	besselE[biq_reso] = 0.611194546878;
	besselF[biq_freq] = pow(E,4) * (0.25/overallscale) * 1.60391912877;
	if (besselF[biq_freq] < 0.00025) besselF[biq_freq] = 0.00025;
	if (besselF[biq_freq] > 0.4999) besselF[biq_freq] = 0.4999;
	besselF[biq_reso] = 0.510317824749;
	
	double K = tan(M_PI * besselA[biq_freq]);
	double norm = 1.0 / (1.0 + K / besselA[biq_reso] + K * K);
	besselA[biq_a0] = K * K * norm;
	besselA[biq_a1] = 2.0 * besselA[biq_a0];
	besselA[biq_a2] = besselA[biq_a0];
	besselA[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselA[biq_b2] = (1.0 - K / besselA[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselB[biq_freq]);
	norm = 1.0 / (1.0 + K / besselB[biq_reso] + K * K);
	besselB[biq_a0] = K * K * norm;
	besselB[biq_a1] = 2.0 * besselB[biq_a0];
	besselB[biq_a2] = besselB[biq_a0];
	besselB[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselB[biq_b2] = (1.0 - K / besselB[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselC[biq_freq]);
	norm = 1.0 / (1.0 + K / besselC[biq_reso] + K * K);
	besselC[biq_a0] = K * K * norm;
	besselC[biq_a1] = 2.0 * besselC[biq_a0];
	besselC[biq_a2] = besselC[biq_a0];
	besselC[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselC[biq_b2] = (1.0 - K / besselC[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselD[biq_freq]);
	norm = 1.0 / (1.0 + K / besselD[biq_reso] + K * K);
	besselD[biq_a0] = K * K * norm;
	besselD[biq_a1] = 2.0 * besselD[biq_a0];
	besselD[biq_a2] = besselD[biq_a0];
	besselD[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselD[biq_b2] = (1.0 - K / besselD[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselE[biq_freq]);
	norm = 1.0 / (1.0 + K / besselE[biq_reso] + K * K);
	besselE[biq_a0] = K * K * norm;
	besselE[biq_a1] = 2.0 * besselE[biq_a0];
	besselE[biq_a2] = besselE[biq_a0];
	besselE[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselE[biq_b2] = (1.0 - K / besselE[biq_reso] + K * K) * norm;
	K = tan(M_PI * besselF[biq_freq]);
	norm = 1.0 / (1.0 + K / besselF[biq_reso] + K * K);
	besselF[biq_a0] = K * K * norm;
	besselF[biq_a1] = 2.0 * besselF[biq_a0];
	besselF[biq_a2] = besselF[biq_a0];
	besselF[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	besselF[biq_b2] = (1.0 - K / besselF[biq_reso] + K * K) * norm;
	
	while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double trebleL = inputSampleL;		
		double outSample = (trebleL * besselA[biq_a0]) + besselA[biq_sL1];
		besselA[biq_sL1] = (trebleL * besselA[biq_a1]) - (outSample * besselA[biq_b1]) + besselA[biq_sL2];
		besselA[biq_sL2] = (trebleL * besselA[biq_a2]) - (outSample * besselA[biq_b2]);
		double midL = outSample; trebleL -= midL;
		outSample = (midL * besselD[biq_a0]) + besselD[biq_sL1];
		besselD[biq_sL1] = (midL * besselD[biq_a1]) - (outSample * besselD[biq_b1]) + besselD[biq_sL2];
		besselD[biq_sL2] = (midL * besselD[biq_a2]) - (outSample * besselD[biq_b2]);
		double bassL = outSample; midL -= bassL;
		trebleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		outSample = (trebleL * besselB[biq_a0]) + besselB[biq_sL1];
		besselB[biq_sL1] = (trebleL * besselB[biq_a1]) - (outSample * besselB[biq_b1]) + besselB[biq_sL2];
		besselB[biq_sL2] = (trebleL * besselB[biq_a2]) - (outSample * besselB[biq_b2]);
		midL = outSample; trebleL -= midL;
		outSample = (midL * besselE[biq_a0]) + besselE[biq_sL1];
		besselE[biq_sL1] = (midL * besselE[biq_a1]) - (outSample * besselE[biq_b1]) + besselE[biq_sL2];
		besselE[biq_sL2] = (midL * besselE[biq_a2]) - (outSample * besselE[biq_b2]);
		bassL = outSample; midL -= bassL;
		trebleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		outSample = (trebleL * besselC[biq_a0]) + besselC[biq_sL1];
		besselC[biq_sL1] = (trebleL * besselC[biq_a1]) - (outSample * besselC[biq_b1]) + besselC[biq_sL2];
		besselC[biq_sL2] = (trebleL * besselC[biq_a2]) - (outSample * besselC[biq_b2]);
		midL = outSample; trebleL -= midL;
		outSample = (midL * besselF[biq_a0]) + besselF[biq_sL1];
		besselF[biq_sL1] = (midL * besselF[biq_a1]) - (outSample * besselF[biq_b1]) + besselF[biq_sL2];
		besselF[biq_sL2] = (midL * besselF[biq_a2]) - (outSample * besselF[biq_b2]);
		bassL = outSample; midL -= bassL;
		inputSampleL = (bassL*bassGain) + (midL*midGain) + (trebleL*trebleGain);
		
		double trebleR = inputSampleR;		
		outSample = (trebleR * besselA[biq_a0]) + besselA[biq_sR1];
		besselA[biq_sR1] = (trebleR * besselA[biq_a1]) - (outSample * besselA[biq_b1]) + besselA[biq_sR2];
		besselA[biq_sR2] = (trebleR * besselA[biq_a2]) - (outSample * besselA[biq_b2]);
		double midR = outSample; trebleR -= midR;
		outSample = (midR * besselD[biq_a0]) + besselD[biq_sR1];
		besselD[biq_sR1] = (midR * besselD[biq_a1]) - (outSample * besselD[biq_b1]) + besselD[biq_sR2];
		besselD[biq_sR2] = (midR * besselD[biq_a2]) - (outSample * besselD[biq_b2]);
		double bassR = outSample; midR -= bassR;
		trebleR = (bassR*bassGain) + (midR*midGain) + (trebleR*trebleGain);
		outSample = (trebleR * besselB[biq_a0]) + besselB[biq_sR1];
		besselB[biq_sR1] = (trebleR * besselB[biq_a1]) - (outSample * besselB[biq_b1]) + besselB[biq_sR2];
		besselB[biq_sR2] = (trebleR * besselB[biq_a2]) - (outSample * besselB[biq_b2]);
		midR = outSample; trebleR -= midR;
		outSample = (midR * besselE[biq_a0]) + besselE[biq_sR1];
		besselE[biq_sR1] = (midR * besselE[biq_a1]) - (outSample * besselE[biq_b1]) + besselE[biq_sR2];
		besselE[biq_sR2] = (midR * besselE[biq_a2]) - (outSample * besselE[biq_b2]);
		bassR = outSample; midR -= bassR;
		trebleR = (bassR*bassGain) + (midR*midGain) + (trebleR*trebleGain);
		outSample = (trebleR * besselC[biq_a0]) + besselC[biq_sR1];
		besselC[biq_sR1] = (trebleR * besselC[biq_a1]) - (outSample * besselC[biq_b1]) + besselC[biq_sR2];
		besselC[biq_sR2] = (trebleR * besselC[biq_a2]) - (outSample * besselC[biq_b2]);
		midR = outSample; trebleR -= midR;
		outSample = (midR * besselF[biq_a0]) + besselF[biq_sR1];
		besselF[biq_sR1] = (midR * besselF[biq_a1]) - (outSample * besselF[biq_b1]) + besselF[biq_sR2];
		besselF[biq_sR2] = (midR * besselF[biq_a2]) - (outSample * besselF[biq_b2]);
		bassR = outSample; midR -= bassR;
		inputSampleR = (bassR*bassGain) + (midR*midGain) + (trebleR*trebleGain);
		
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
