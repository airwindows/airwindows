/* ========================================
 *  MidSide - MidSide.h
 *  Copyright (c) 2016 airwindows, All rights reserved
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
		
		//stereo 32 bit dither, made small and tidy.
		int expon; frexpf((float)mid, &expon);
		double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
		mid += (dither-fpNShapeL); fpNShapeL = dither;
		frexpf((float)side, &expon);
		dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
		side += (dither-fpNShapeR); fpNShapeR = dither;
		//end 32 bit dither
		
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
		int expon; frexp((double)mid, &expon);
		double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
		dither /= 536870912.0; //needs this to scale to 64 bit zone
		mid += (dither-fpNShapeL); fpNShapeL = dither;
		frexp((double)side, &expon);
		dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
		dither /= 536870912.0; //needs this to scale to 64 bit zone
		side += (dither-fpNShapeR); fpNShapeR = dither;
		//end 64 bit dither
		
		*out1 = mid;
		*out2 = side;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}