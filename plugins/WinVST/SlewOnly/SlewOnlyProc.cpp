/* ========================================
 *  SlewOnly - SlewOnly.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SlewOnly_H
#include "SlewOnly.h"
#endif

void SlewOnly::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double inputSampleL;
	double inputSampleR;
	double outputSampleL;
	double outputSampleR;
	double trim = 2.302585092994045684017991; //natural logarithm of 10
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		outputSampleL = (inputSampleL - lastSampleL)*trim;
		outputSampleR = (inputSampleR - lastSampleR)*trim;
		lastSampleL = inputSampleL;
		lastSampleR = inputSampleR;
		if (outputSampleL > 1.0) outputSampleL = 1.0;
		if (outputSampleR > 1.0) outputSampleR = 1.0;
		if (outputSampleL < -1.0) outputSampleL = -1.0;		
		if (outputSampleR < -1.0) outputSampleR = -1.0;		
		
		*out1 = outputSampleL;
		*out2 = outputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void SlewOnly::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double inputSampleL;
	double inputSampleR;
	double outputSampleL;
	double outputSampleR;
	double trim = 2.302585092994045684017991; //natural logarithm of 10
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		
		outputSampleL = (inputSampleL - lastSampleL)*trim;
		outputSampleR = (inputSampleR - lastSampleR)*trim;
		lastSampleL = inputSampleL;
		lastSampleR = inputSampleR;
		if (outputSampleL > 1.0) outputSampleL = 1.0;
		if (outputSampleR > 1.0) outputSampleR = 1.0;
		if (outputSampleL < -1.0) outputSampleL = -1.0;		
		if (outputSampleR < -1.0) outputSampleR = -1.0;		
		
		*out1 = outputSampleL;
		*out2 = outputSampleR;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}