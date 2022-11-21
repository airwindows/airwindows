/* ========================================
 *  MidSide - MidSide.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __MidSide_H
#include "MidSide.h"
#endif

void MidSide::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];


	double inputSampleL;
	double inputSampleR;
	
	double mid;
	double side;
	
	double sidegain = A * 2.0;
	double midgain = 2.0 - sidegain;
    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		mid = inputSampleL + inputSampleR;
		side = inputSampleL - inputSampleR;
		
		mid *= midgain;
		side *= sidegain;
		
		//begin 32 bit floating point dither
		int expon; frexpf((float)mid, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		mid += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		frexpf((float)side, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		side += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		
		*out1 = mid;
		*out2 = side;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void MidSide::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];


	double inputSampleL;
	double inputSampleR;

	double mid;
	double side;
	
	double sidegain = A * 2.0;
	double midgain = 2.0 - sidegain;
    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		mid = inputSampleL + inputSampleR;
		side = inputSampleL - inputSampleR;
		
		mid *= midgain;
		side *= sidegain;
		
		//stereo 64 bit dither, made small and tidy.
		//int expon; frexpf((float)mid, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//mid += ((double(fpdL)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 32 bit floating point dither
		//frexpf((float)side, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//side += ((double(fpdR)-uint32_t(0x7fffffff)) * 5.5e-36l * pow(2,expon+62));
		//end 64 bit dither
		
		*out1 = mid;
		*out2 = side;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}