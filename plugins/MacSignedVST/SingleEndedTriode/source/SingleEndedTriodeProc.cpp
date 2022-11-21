/* ========================================
 *  SingleEndedTriode - SingleEndedTriode.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SingleEndedTriode_H
#include "SingleEndedTriode.h"
#endif

void SingleEndedTriode::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	double intensity = pow(A,2)*8.0;
	double triode = intensity;
	intensity +=0.001;
	double softcrossover = pow(B,3)/8.0;
	double hardcrossover = pow(C,7)/8.0;
	double wet = D;
	//removed extra dry variable
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (triode > 0.0)
		{
			inputSampleL *= intensity;
			inputSampleR *= intensity;
			inputSampleL -= 0.5;
			inputSampleR -= 0.5;
			
			double bridgerectifier = fabs(inputSampleL);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleL > 0) inputSampleL = bridgerectifier;
			else inputSampleL = -bridgerectifier;

			bridgerectifier = fabs(inputSampleR);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleR > 0) inputSampleR = bridgerectifier;
			else inputSampleR = -bridgerectifier;

			inputSampleL += postsine;
			inputSampleR += postsine;
			inputSampleL /= intensity;
			inputSampleR /= intensity;
		}
		
		if (softcrossover > 0.0)
		{
			double bridgerectifier = fabs(inputSampleL);
			if (bridgerectifier > 0.0) bridgerectifier -= (softcrossover*(bridgerectifier+sqrt(bridgerectifier)));
			if (bridgerectifier < 0.0) bridgerectifier = 0;
			if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
			else inputSampleL = -bridgerectifier;				

			bridgerectifier = fabs(inputSampleR);
			if (bridgerectifier > 0.0) bridgerectifier -= (softcrossover*(bridgerectifier+sqrt(bridgerectifier)));
			if (bridgerectifier < 0.0) bridgerectifier = 0;
			if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
			else inputSampleR = -bridgerectifier;				
		}
		
		
		if (hardcrossover > 0.0)
		{
			double bridgerectifier = fabs(inputSampleL);
			bridgerectifier -= hardcrossover;
			if (bridgerectifier < 0.0) bridgerectifier = 0.0;
			if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
			else inputSampleL = -bridgerectifier;				

			bridgerectifier = fabs(inputSampleR);
			bridgerectifier -= hardcrossover;
			if (bridgerectifier < 0.0) bridgerectifier = 0.0;
			if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
			else inputSampleR = -bridgerectifier;				
		}
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
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
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void SingleEndedTriode::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	double intensity = pow(A,2)*8.0;
	double triode = intensity;
	intensity +=0.001;
	double softcrossover = pow(B,3)/8.0;
	double hardcrossover = pow(C,7)/8.0;
	double wet = D;
	//removed extra dry variable

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (triode > 0.0)
		{
			inputSampleL *= intensity;
			inputSampleR *= intensity;
			inputSampleL -= 0.5;
			inputSampleR -= 0.5;
			
			double bridgerectifier = fabs(inputSampleL);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleL > 0) inputSampleL = bridgerectifier;
			else inputSampleL = -bridgerectifier;
			
			bridgerectifier = fabs(inputSampleR);
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleR > 0) inputSampleR = bridgerectifier;
			else inputSampleR = -bridgerectifier;
			
			inputSampleL += postsine;
			inputSampleR += postsine;
			inputSampleL /= intensity;
			inputSampleR /= intensity;
		}
		
		if (softcrossover > 0.0)
		{
			double bridgerectifier = fabs(inputSampleL);
			if (bridgerectifier > 0.0) bridgerectifier -= (softcrossover*(bridgerectifier+sqrt(bridgerectifier)));
			if (bridgerectifier < 0.0) bridgerectifier = 0;
			if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
			else inputSampleL = -bridgerectifier;				
			
			bridgerectifier = fabs(inputSampleR);
			if (bridgerectifier > 0.0) bridgerectifier -= (softcrossover*(bridgerectifier+sqrt(bridgerectifier)));
			if (bridgerectifier < 0.0) bridgerectifier = 0;
			if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
			else inputSampleR = -bridgerectifier;				
		}
		
		
		if (hardcrossover > 0.0)
		{
			double bridgerectifier = fabs(inputSampleL);
			bridgerectifier -= hardcrossover;
			if (bridgerectifier < 0.0) bridgerectifier = 0.0;
			if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
			else inputSampleL = -bridgerectifier;				
			
			bridgerectifier = fabs(inputSampleR);
			bridgerectifier -= hardcrossover;
			if (bridgerectifier < 0.0) bridgerectifier = 0.0;
			if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
			else inputSampleR = -bridgerectifier;				
		}
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
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
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}