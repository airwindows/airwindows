/* ========================================
 *  HighGlossDither - HighGlossDither.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __HighGlossDither_H
#include "HighGlossDither.h"
#endif

void HighGlossDither::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	int hotbinA;

	double inputSampleL;
	double inputSampleR;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= 8388608.0;
		inputSampleR *= 8388608.0;
		//0-1 is now one bit, now we dither
		
		Position += 1;
		//Note- uses integer overflow as a 'mod' operator
		hotbinA = Position * Position;
		hotbinA = hotbinA % 170003; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 17011; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 1709; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 173; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 17;
		hotbinA *= 0.0635;
		if (flip) hotbinA = -hotbinA;
		
		inputSampleL += hotbinA;
		inputSampleR += hotbinA;
		inputSampleL = floor(inputSampleL);
		inputSampleR = floor(inputSampleR);
		//Quadratic dither
		
		flip = !flip;
		
		inputSampleL /= 8388608.0;
		inputSampleR /= 8388608.0;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void HighGlossDither::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	int hotbinA;
	
	double inputSampleL;
	double inputSampleR;

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		inputSampleL *= 8388608.0;
		inputSampleR *= 8388608.0;
		//0-1 is now one bit, now we dither
		
		Position += 1;
		//Note- uses integer overflow as a 'mod' operator
		hotbinA = Position * Position;
		hotbinA = hotbinA % 170003; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 17011; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 1709; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 173; //% is C++ mod operator
		hotbinA *= hotbinA;
		hotbinA = hotbinA % 17;
		hotbinA *= 0.0635;
		if (flip) hotbinA = -hotbinA;
		
		inputSampleL += hotbinA;
		inputSampleR += hotbinA;
		inputSampleL = floor(inputSampleL);
		inputSampleR = floor(inputSampleR);
		//Quadratic dither
		
		flip = !flip;
		
		inputSampleL /= 8388608.0;
		inputSampleR /= 8388608.0;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}