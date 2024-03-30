/* ========================================
 *  ConsoleXSubIn - ConsoleXSubIn.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleXSubIn_H
#include "ConsoleXSubIn.h"
#endif

void ConsoleXSubIn::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	biquad[biq_freq] = 25000.0/getSampleRate();	
    biquad[biq_reso] = 0.60134489;
	double K = tan(M_PI * biquad[biq_freq]);
	double norm = 1.0 / (1.0 + K / biquad[biq_reso] + K * K);
	biquad[biq_a0] = K * K * norm;
	biquad[biq_a1] = 2.0 * biquad[biq_a0];
	biquad[biq_a2] = biquad[biq_a0];
	biquad[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	biquad[biq_b2] = (1.0 - K / biquad[biq_reso] + K * K) * norm;
	//ultrasonic nonlinear filter
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (biquad[biq_freq] < 0.5) {
			double nlBiq = fabs(biquad[biq_a0]*(1.0+(inputSampleL*0.25))); if (nlBiq > 1.0) nlBiq = 1.0;
			double tmp = (inputSampleL * nlBiq) + biquad[biq_sL1];
			biquad[biq_sL1] = (inputSampleL * biquad[biq_a1]) - (tmp * biquad[biq_b1]) + biquad[biq_sL2];
			biquad[biq_sL2] = (inputSampleL * nlBiq) - (tmp * biquad[biq_b2]);
			inputSampleL = tmp;
			nlBiq = fabs(biquad[biq_a0]*(1.0+(inputSampleR*0.25))); if (nlBiq > 1.0) nlBiq = 1.0;
			tmp = (inputSampleR * nlBiq) + biquad[biq_sR1];
			biquad[biq_sR1] = (inputSampleR * biquad[biq_a1]) - (tmp * biquad[biq_b1]) + biquad[biq_sR2];
			biquad[biq_sR2] = (inputSampleR * nlBiq) - (tmp * biquad[biq_b2]);
			inputSampleR = tmp;
			//ultrasonic filter before anything else is done
		}
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 0.6180339887498949));
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 0.6180339887498949));
		inputSampleL *= 1.6180339887498949;
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 0.6180339887498949));
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 0.6180339887498949));
		inputSampleR *= 1.6180339887498949;
		//ConsoleXSubIn is purely a decode that is followed with encode on ConsoleXSubOut
		//It allows for another filtering stage for steep distributed ultrasonic filtering
		//but pointedly has no controls: it just has to be there, first on the submix
		
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

void ConsoleXSubIn::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	biquad[biq_freq] = 25000.0/getSampleRate();	
    biquad[biq_reso] = 0.60134489;
	double K = tan(M_PI * biquad[biq_freq]);
	double norm = 1.0 / (1.0 + K / biquad[biq_reso] + K * K);
	biquad[biq_a0] = K * K * norm;
	biquad[biq_a1] = 2.0 * biquad[biq_a0];
	biquad[biq_a2] = biquad[biq_a0];
	biquad[biq_b1] = 2.0 * (K * K - 1.0) * norm;
	biquad[biq_b2] = (1.0 - K / biquad[biq_reso] + K * K) * norm;
	//ultrasonic nonlinear filter
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (biquad[biq_freq] < 0.5) {
			double nlBiq = fabs(biquad[biq_a0]*(1.0+(inputSampleL*0.25))); if (nlBiq > 1.0) nlBiq = 1.0;
			double tmp = (inputSampleL * nlBiq) + biquad[biq_sL1];
			biquad[biq_sL1] = (inputSampleL * biquad[biq_a1]) - (tmp * biquad[biq_b1]) + biquad[biq_sL2];
			biquad[biq_sL2] = (inputSampleL * nlBiq) - (tmp * biquad[biq_b2]);
			inputSampleL = tmp;
			nlBiq = fabs(biquad[biq_a0]*(1.0+(inputSampleR*0.25))); if (nlBiq > 1.0) nlBiq = 1.0;
			tmp = (inputSampleR * nlBiq) + biquad[biq_sR1];
			biquad[biq_sR1] = (inputSampleR * biquad[biq_a1]) - (tmp * biquad[biq_b1]) + biquad[biq_sR2];
			biquad[biq_sR2] = (inputSampleR * nlBiq) - (tmp * biquad[biq_b2]);
			inputSampleR = tmp;
			//ultrasonic filter before anything else is done
		}
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 0.6180339887498949));
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 0.6180339887498949));
		inputSampleL *= 1.6180339887498949;
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 0.6180339887498949));
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 0.6180339887498949));
		inputSampleR *= 1.6180339887498949;
		//ConsoleXSubIn is purely a decode that is followed with encode on ConsoleXSubOut
		//It allows for another filtering stage for steep distributed ultrasonic filtering
		//but pointedly has no controls: it just has to be there, first on the submix
		
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
