/* ========================================
 *  Density3 - Density3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Density3_H
#include "Density3.h"
#endif

void Density3::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double density = A*5.0;
	double iirAmount = pow(B,3)/overallscale;
	if (iirAmount == 0.0) {iirSampleL = 0.0; iirSampleR = 0.0;}
	double output = C;
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		iirSampleL = (iirSampleL * (1.0 - iirAmount)) + (inputSampleL * iirAmount);
		inputSampleL -= iirSampleL;
		iirSampleR = (iirSampleR * (1.0 - iirAmount)) + (inputSampleR * iirAmount);
		inputSampleR -= iirSampleR;
		
		double altered = inputSampleL;
		if (density > 1.0) {
			altered = fmax(fmin(inputSampleL*density*M_PI_2,M_PI_2),-M_PI_2);
			double X = altered*altered;
			double temp = altered*X;
			altered -= (temp / 6.0); temp *= X;
			altered += (temp / 120.0); temp *= X;
			altered -= (temp / 5040.0); temp *= X;
			altered += (temp / 362880.0); temp *= X;
			altered -= (temp / 39916800.0);
		}
		if (density < 1.0) {
			altered = fmax(fmin(inputSampleL,1.0),-1.0);
			double polarity = altered;
			double X = inputSampleL * altered;
			double temp = X;
			altered = (temp / 2.0); temp *= X;
			altered -= (temp / 24.0); temp *= X;
			altered += (temp / 720.0); temp *= X;
			altered -= (temp / 40320.0); temp *= X;
			altered += (temp / 3628800.0);
			altered *= ((polarity<0.0)?-1.0:1.0);
		}
		if (density > 2.0) inputSampleL = altered;
		else inputSampleL = (inputSampleL*(1.0-fabs(density-1.0)))+(altered*fabs(density-1.0));
		
		altered = inputSampleR;
		if (density > 1.0) {
			altered = fmax(fmin(inputSampleR*density*M_PI_2,M_PI_2),-M_PI_2);
			double X = altered*altered;
			double temp = altered*X;
			altered -= (temp / 6.0); temp *= X;
			altered += (temp / 120.0); temp *= X;
			altered -= (temp / 5040.0); temp *= X;
			altered += (temp / 362880.0); temp *= X;
			altered -= (temp / 39916800.0);
		}
		if (density < 1.0) {
			altered = fmax(fmin(inputSampleR,1.0),-1.0);
			double polarity = altered;
			double X = inputSampleR * altered;
			double temp = X;
			altered = (temp / 2.0); temp *= X;
			altered -= (temp / 24.0); temp *= X;
			altered += (temp / 720.0); temp *= X;
			altered -= (temp / 40320.0); temp *= X;
			altered += (temp / 3628800.0);
			altered *= ((polarity<0.0)?-1.0:1.0);
		}
		if (density > 2.0) inputSampleR = altered;
		else inputSampleR = (inputSampleR*(1.0-fabs(density-1.0)))+(altered*fabs(density-1.0));
		
		inputSampleL = (drySampleL*(1.0-wet))+(inputSampleL*output*wet);
		inputSampleR = (drySampleR*(1.0-wet))+(inputSampleR*output*wet);
		
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

void Density3::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double density = A*5.0;
	double iirAmount = pow(B,3)/overallscale;
	if (iirAmount == 0.0) {iirSampleL = 0.0; iirSampleR = 0.0;}
	double output = C;
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		iirSampleL = (iirSampleL * (1.0 - iirAmount)) + (inputSampleL * iirAmount);
		inputSampleL -= iirSampleL;
		iirSampleR = (iirSampleR * (1.0 - iirAmount)) + (inputSampleR * iirAmount);
		inputSampleR -= iirSampleR;
		
		double altered = inputSampleL;
		if (density > 1.0) {
			altered = fmax(fmin(inputSampleL*density*M_PI_2,M_PI_2),-M_PI_2);
			double X = altered*altered;
			double temp = altered*X;
			altered -= (temp / 6.0); temp *= X;
			altered += (temp / 120.0); temp *= X;
			altered -= (temp / 5040.0); temp *= X;
			altered += (temp / 362880.0); temp *= X;
			altered -= (temp / 39916800.0);
		}
		if (density < 1.0) {
			altered = fmax(fmin(inputSampleL,1.0),-1.0);
			double polarity = altered;
			double X = inputSampleL * altered;
			double temp = X;
			altered = (temp / 2.0); temp *= X;
			altered -= (temp / 24.0); temp *= X;
			altered += (temp / 720.0); temp *= X;
			altered -= (temp / 40320.0); temp *= X;
			altered += (temp / 3628800.0);
			altered *= ((polarity<0.0)?-1.0:1.0);
		}
		if (density > 2.0) inputSampleL = altered;
		else inputSampleL = (inputSampleL*(1.0-fabs(density-1.0)))+(altered*fabs(density-1.0));
		
		altered = inputSampleR;
		if (density > 1.0) {
			altered = fmax(fmin(inputSampleR*density*M_PI_2,M_PI_2),-M_PI_2);
			double X = altered*altered;
			double temp = altered*X;
			altered -= (temp / 6.0); temp *= X;
			altered += (temp / 120.0); temp *= X;
			altered -= (temp / 5040.0); temp *= X;
			altered += (temp / 362880.0); temp *= X;
			altered -= (temp / 39916800.0);
		}
		if (density < 1.0) {
			altered = fmax(fmin(inputSampleR,1.0),-1.0);
			double polarity = altered;
			double X = inputSampleR * altered;
			double temp = X;
			altered = (temp / 2.0); temp *= X;
			altered -= (temp / 24.0); temp *= X;
			altered += (temp / 720.0); temp *= X;
			altered -= (temp / 40320.0); temp *= X;
			altered += (temp / 3628800.0);
			altered *= ((polarity<0.0)?-1.0:1.0);
		}
		if (density > 2.0) inputSampleR = altered;
		else inputSampleR = (inputSampleR*(1.0-fabs(density-1.0)))+(altered*fabs(density-1.0));
		
		inputSampleL = (drySampleL*(1.0-wet))+(inputSampleL*output*wet);
		inputSampleR = (drySampleR*(1.0-wet))+(inputSampleR*output*wet);
		
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
