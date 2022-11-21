/* ========================================
 *  Infrasonic - Infrasonic.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Infrasonic_H
#include "Infrasonic.h"
#endif

void Infrasonic::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	biquadE[0] = biquadD[0] = biquadC[0] = biquadB[0] = biquadA[0] = 20.0 / getSampleRate();
    biquadA[1] = 0.50623256;
	biquadB[1] = 0.56116312;
	biquadC[1] = 0.70710678;
	biquadD[1] = 1.10134463;
	biquadE[1] = 3.19622661; //tenth order Butterworth out of five biquads
	
	double K = tan(M_PI * biquadA[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = norm;
	biquadA[3] = -2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadA[0]);
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = norm;
	biquadB[3] = -2.0 * biquadB[2];
	biquadB[4] = biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;
	
	K = tan(M_PI * biquadC[0]);
	norm = 1.0 / (1.0 + K / biquadC[1] + K * K);
	biquadC[2] = norm;
	biquadC[3] = -2.0 * biquadC[2];
	biquadC[4] = biquadC[2];
	biquadC[5] = 2.0 * (K * K - 1.0) * norm;
	biquadC[6] = (1.0 - K / biquadC[1] + K * K) * norm;
	
	K = tan(M_PI * biquadD[0]);
	norm = 1.0 / (1.0 + K / biquadD[1] + K * K);
	biquadD[2] = norm;
	biquadD[3] = -2.0 * biquadD[2];
	biquadD[4] = biquadD[2];
	biquadD[5] = 2.0 * (K * K - 1.0) * norm;
	biquadD[6] = (1.0 - K / biquadD[1] + K * K) * norm;
	
	K = tan(M_PI * biquadE[0]);
	norm = 1.0 / (1.0 + K / biquadE[1] + K * K);
	biquadE[2] = norm;
	biquadE[3] = -2.0 * biquadE[2];
	biquadE[4] = biquadE[2];
	biquadE[5] = 2.0 * (K * K - 1.0) * norm;
	biquadE[6] = (1.0 - K / biquadE[1] + K * K) * norm;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double outSampleL = biquadA[2]*inputSampleL+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
		biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; inputSampleL = outSampleL; biquadA[10] = biquadA[9]; biquadA[9] = inputSampleL; //DF1 left
		
		outSampleL = biquadB[2]*inputSampleL+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
		biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; inputSampleL = outSampleL; biquadB[10] = biquadB[9]; biquadB[9] = inputSampleL; //DF1 left
		
		outSampleL = biquadC[2]*inputSampleL+biquadC[3]*biquadC[7]+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
		biquadC[8] = biquadC[7]; biquadC[7] = inputSampleL; inputSampleL = outSampleL; biquadC[10] = biquadC[9]; biquadC[9] = inputSampleL; //DF1 left
		
		outSampleL = biquadD[2]*inputSampleL+biquadD[3]*biquadD[7]+biquadD[4]*biquadD[8]-biquadD[5]*biquadD[9]-biquadD[6]*biquadD[10];
		biquadD[8] = biquadD[7]; biquadD[7] = inputSampleL; inputSampleL = outSampleL; biquadD[10] = biquadD[9]; biquadD[9] = inputSampleL; //DF1 left
		
		outSampleL = biquadE[2]*inputSampleL+biquadE[3]*biquadE[7]+biquadE[4]*biquadE[8]-biquadE[5]*biquadE[9]-biquadE[6]*biquadE[10];
		biquadE[8] = biquadE[7]; biquadE[7] = inputSampleL; inputSampleL = outSampleL; biquadE[10] = biquadE[9]; biquadE[9] = inputSampleL; //DF1 left
		
		double outSampleR = biquadA[2]*inputSampleR+biquadA[3]*biquadA[11]+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
		biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; inputSampleR = outSampleR; biquadA[14] = biquadA[13]; biquadA[13] = inputSampleR; //DF1 right
		
		outSampleR = biquadB[2]*inputSampleR+biquadB[3]*biquadB[11]+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
		biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; inputSampleR = outSampleR; biquadB[14] = biquadB[13]; biquadB[13] = inputSampleR; //DF1 right
		
		outSampleR = biquadC[2]*inputSampleR+biquadC[3]*biquadC[11]+biquadC[4]*biquadC[12]-biquadC[5]*biquadC[13]-biquadC[6]*biquadC[14];
		biquadC[12] = biquadC[11]; biquadC[11] = inputSampleR; inputSampleR = outSampleR; biquadC[14] = biquadC[13]; biquadC[13] = inputSampleR; //DF1 right
		
		outSampleR = biquadD[2]*inputSampleR+biquadD[3]*biquadD[11]+biquadD[4]*biquadD[12]-biquadD[5]*biquadD[13]-biquadD[6]*biquadD[14];
		biquadD[12] = biquadD[11]; biquadD[11] = inputSampleR; inputSampleR = outSampleR; biquadD[14] = biquadD[13]; biquadD[13] = inputSampleR; //DF1 right
		
		outSampleR = biquadE[2]*inputSampleR+biquadE[3]*biquadE[11]+biquadE[4]*biquadE[12]-biquadE[5]*biquadE[13]-biquadE[6]*biquadE[14];
		biquadE[12] = biquadE[11]; biquadE[11] = inputSampleR; inputSampleR = outSampleR; biquadE[14] = biquadE[13]; biquadE[13] = inputSampleR; //DF1 right
		
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

void Infrasonic::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	biquadE[0] = biquadD[0] = biquadC[0] = biquadB[0] = biquadA[0] = 20.0 / getSampleRate();
    biquadA[1] = 0.50623256;
	biquadB[1] = 0.56116312;
	biquadC[1] = 0.70710678;
	biquadD[1] = 1.10134463;
	biquadE[1] = 3.19622661; //tenth order Butterworth out of five biquads
	
	double K = tan(M_PI * biquadA[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = norm;
	biquadA[3] = -2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadA[0]);
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = norm;
	biquadB[3] = -2.0 * biquadB[2];
	biquadB[4] = biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;
	
	K = tan(M_PI * biquadC[0]);
	norm = 1.0 / (1.0 + K / biquadC[1] + K * K);
	biquadC[2] = norm;
	biquadC[3] = -2.0 * biquadC[2];
	biquadC[4] = biquadC[2];
	biquadC[5] = 2.0 * (K * K - 1.0) * norm;
	biquadC[6] = (1.0 - K / biquadC[1] + K * K) * norm;
	
	K = tan(M_PI * biquadD[0]);
	norm = 1.0 / (1.0 + K / biquadD[1] + K * K);
	biquadD[2] = norm;
	biquadD[3] = -2.0 * biquadD[2];
	biquadD[4] = biquadD[2];
	biquadD[5] = 2.0 * (K * K - 1.0) * norm;
	biquadD[6] = (1.0 - K / biquadD[1] + K * K) * norm;
	
	K = tan(M_PI * biquadE[0]);
	norm = 1.0 / (1.0 + K / biquadE[1] + K * K);
	biquadE[2] = norm;
	biquadE[3] = -2.0 * biquadE[2];
	biquadE[4] = biquadE[2];
	biquadE[5] = 2.0 * (K * K - 1.0) * norm;
	biquadE[6] = (1.0 - K / biquadE[1] + K * K) * norm;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double outSampleL = biquadA[2]*inputSampleL+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
		biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; inputSampleL = outSampleL; biquadA[10] = biquadA[9]; biquadA[9] = inputSampleL; //DF1 left
		
		outSampleL = biquadB[2]*inputSampleL+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
		biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; inputSampleL = outSampleL; biquadB[10] = biquadB[9]; biquadB[9] = inputSampleL; //DF1 left
		
		outSampleL = biquadC[2]*inputSampleL+biquadC[3]*biquadC[7]+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
		biquadC[8] = biquadC[7]; biquadC[7] = inputSampleL; inputSampleL = outSampleL; biquadC[10] = biquadC[9]; biquadC[9] = inputSampleL; //DF1 left
		
		outSampleL = biquadD[2]*inputSampleL+biquadD[3]*biquadD[7]+biquadD[4]*biquadD[8]-biquadD[5]*biquadD[9]-biquadD[6]*biquadD[10];
		biquadD[8] = biquadD[7]; biquadD[7] = inputSampleL; inputSampleL = outSampleL; biquadD[10] = biquadD[9]; biquadD[9] = inputSampleL; //DF1 left
		
		outSampleL = biquadE[2]*inputSampleL+biquadE[3]*biquadE[7]+biquadE[4]*biquadE[8]-biquadE[5]*biquadE[9]-biquadE[6]*biquadE[10];
		biquadE[8] = biquadE[7]; biquadE[7] = inputSampleL; inputSampleL = outSampleL; biquadE[10] = biquadE[9]; biquadE[9] = inputSampleL; //DF1 left
		
		double outSampleR = biquadA[2]*inputSampleR+biquadA[3]*biquadA[11]+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
		biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; inputSampleR = outSampleR; biquadA[14] = biquadA[13]; biquadA[13] = inputSampleR; //DF1 right
		
		outSampleR = biquadB[2]*inputSampleR+biquadB[3]*biquadB[11]+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
		biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; inputSampleR = outSampleR; biquadB[14] = biquadB[13]; biquadB[13] = inputSampleR; //DF1 right
		
		outSampleR = biquadC[2]*inputSampleR+biquadC[3]*biquadC[11]+biquadC[4]*biquadC[12]-biquadC[5]*biquadC[13]-biquadC[6]*biquadC[14];
		biquadC[12] = biquadC[11]; biquadC[11] = inputSampleR; inputSampleR = outSampleR; biquadC[14] = biquadC[13]; biquadC[13] = inputSampleR; //DF1 right
		
		outSampleR = biquadD[2]*inputSampleR+biquadD[3]*biquadD[11]+biquadD[4]*biquadD[12]-biquadD[5]*biquadD[13]-biquadD[6]*biquadD[14];
		biquadD[12] = biquadD[11]; biquadD[11] = inputSampleR; inputSampleR = outSampleR; biquadD[14] = biquadD[13]; biquadD[13] = inputSampleR; //DF1 right
		
		outSampleR = biquadE[2]*inputSampleR+biquadE[3]*biquadE[11]+biquadE[4]*biquadE[12]-biquadE[5]*biquadE[13]-biquadE[6]*biquadE[14];
		biquadE[12] = biquadE[11]; biquadE[11] = inputSampleR; inputSampleR = outSampleR; biquadE[14] = biquadE[13]; biquadE[13] = inputSampleR; //DF1 right
		
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
