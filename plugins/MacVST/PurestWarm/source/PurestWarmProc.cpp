/* ========================================
 *  PurestWarm - PurestWarm.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __PurestWarm_H
#include "PurestWarm.h"
#endif

void PurestWarm::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];


	int polarity = (int) ( A * 1.999 );
	
	double inputSampleL;
	double inputSampleR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (polarity == 1)
		{
			if (inputSampleL < 0) 
			{
				inputSampleL = -(sin(-inputSampleL*1.57079634)/1.57079634);
				//stereo 32 bit dither, made small and tidy.
				int expon; frexpf((float)inputSampleL, &expon);
				double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				inputSampleL += (dither-fpNShapeL); fpNShapeL = dither;
				frexpf((float)inputSampleR, &expon);
				dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				inputSampleR += (dither-fpNShapeR); fpNShapeR = dither;
				//end 32 bit dither
			}
			if (inputSampleR < 0) 
			{
				inputSampleR = -(sin(-inputSampleR*1.57079634)/1.57079634);
				//stereo 32 bit dither, made small and tidy.
				int expon; frexpf((float)inputSampleL, &expon);
				double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				inputSampleL += (dither-fpNShapeL); fpNShapeL = dither;
				frexpf((float)inputSampleR, &expon);
				dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				inputSampleR += (dither-fpNShapeR); fpNShapeR = dither;
				//end 32 bit dither
			}
		} else {
			if (inputSampleL > 0) 
			{
				inputSampleL = sin(inputSampleL*1.57079634)/1.57079634;
				//stereo 32 bit dither, made small and tidy.
				int expon; frexpf((float)inputSampleL, &expon);
				double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				inputSampleL += (dither-fpNShapeL); fpNShapeL = dither;
				frexpf((float)inputSampleR, &expon);
				dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				inputSampleR += (dither-fpNShapeR); fpNShapeR = dither;
				//end 32 bit dither
			} 
			
			if (inputSampleR > 0) 
			{
				inputSampleR = sin(inputSampleR*1.57079634)/1.57079634;
				//stereo 32 bit dither, made small and tidy.
				int expon; frexpf((float)inputSampleL, &expon);
				double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				inputSampleL += (dither-fpNShapeL); fpNShapeL = dither;
				frexpf((float)inputSampleR, &expon);
				dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				inputSampleR += (dither-fpNShapeR); fpNShapeR = dither;
				//end 32 bit dither
			}			
		}
		//that's it. Only applies on one half of the waveform, other half is passthrough untouched.

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void PurestWarm::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	
	int polarity = (int) ( A * 1.999 );

	double inputSampleL;
	double inputSampleR;

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (polarity == 1)
		{
			if (inputSampleL < 0) 
			{
				inputSampleL = -(sin(-inputSampleL*1.57079634)/1.57079634);
				//stereo 64 bit dither, made small and tidy.
				int expon; frexp((double)inputSampleL, &expon);
				double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				dither /= 536870912.0; //needs this to scale to 64 bit zone
				inputSampleL += (dither-fpNShapeL); fpNShapeL = dither;
				frexp((double)inputSampleR, &expon);
				dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				dither /= 536870912.0; //needs this to scale to 64 bit zone
				inputSampleR += (dither-fpNShapeR); fpNShapeR = dither;
				//end 64 bit dither
			}
			
			if (inputSampleR < 0) 
			{
				inputSampleR = -(sin(-inputSampleR*1.57079634)/1.57079634);
				//stereo 64 bit dither, made small and tidy.
				int expon; frexp((double)inputSampleL, &expon);
				double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				dither /= 536870912.0; //needs this to scale to 64 bit zone
				inputSampleL += (dither-fpNShapeL); fpNShapeL = dither;
				frexp((double)inputSampleR, &expon);
				dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				dither /= 536870912.0; //needs this to scale to 64 bit zone
				inputSampleR += (dither-fpNShapeR); fpNShapeR = dither;
				//end 64 bit dither
			}
		} else {
			if (inputSampleL > 0) 
			{
				inputSampleL = sin(inputSampleL*1.57079634)/1.57079634;
				//stereo 64 bit dither, made small and tidy.
				int expon; frexp((double)inputSampleL, &expon);
				double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				dither /= 536870912.0; //needs this to scale to 64 bit zone
				inputSampleL += (dither-fpNShapeL); fpNShapeL = dither;
				frexp((double)inputSampleR, &expon);
				dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				dither /= 536870912.0; //needs this to scale to 64 bit zone
				inputSampleR += (dither-fpNShapeR); fpNShapeR = dither;
				//end 64 bit dither
			}
			if (inputSampleR > 0) 
			{
				inputSampleR = sin(inputSampleR*1.57079634)/1.57079634;
				//stereo 64 bit dither, made small and tidy.
				int expon; frexp((double)inputSampleL, &expon);
				double dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				dither /= 536870912.0; //needs this to scale to 64 bit zone
				inputSampleL += (dither-fpNShapeL); fpNShapeL = dither;
				frexp((double)inputSampleR, &expon);
				dither = (rand()/(RAND_MAX*7.737125245533627e+25))*pow(2,expon+62);
				dither /= 536870912.0; //needs this to scale to 64 bit zone
				inputSampleR += (dither-fpNShapeR); fpNShapeR = dither;
				//end 64 bit dither
			}
		}
		//that's it. Only applies on one half of the waveform, other half is passthrough untouched.
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}