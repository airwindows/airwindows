/* ========================================
 *  Fracture - Fracture.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Fracture_H
#include "Fracture.h"
#endif

void Fracture::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	
	double density = A*4;
	double fracture = (((B*2.999)+1)*3.14159265358979);
	double output = C;
	double wet = D;
	double dry = 1.0-wet;
	double bridgerectifier;
	density = density * fabs(density);
	
	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;

		inputSampleL *= density;
		inputSampleR *= density;

		bridgerectifier = fabs(inputSampleL)*fracture;
		if (bridgerectifier > fracture) bridgerectifier = fracture;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		//blend according to density control

		bridgerectifier = fabs(inputSampleR)*fracture;
		if (bridgerectifier > fracture) bridgerectifier = fracture;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		//blend according to density control
		
		inputSampleL *= output;
		inputSampleR *= output;
		
		inputSampleL = (drySampleL * dry)+(inputSampleL * wet);
		inputSampleR = (drySampleR * dry)+(inputSampleR * wet);		
		
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

void Fracture::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double density = A*4;
	double fracture = (((B*2.999)+1)*3.14159265358979);
	double output = C;
	double wet = D;
	double dry = 1.0-wet;
	double bridgerectifier;
	density = density * fabs(density);
	
	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;	

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		inputSampleL *= density;
		inputSampleR *= density;
		
		bridgerectifier = fabs(inputSampleL)*fracture;
		if (bridgerectifier > fracture) bridgerectifier = fracture;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		//blend according to density control
		
		bridgerectifier = fabs(inputSampleR)*fracture;
		if (bridgerectifier > fracture) bridgerectifier = fracture;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		//blend according to density control
		
		inputSampleL *= output;
		inputSampleR *= output;
		
		inputSampleL = (drySampleL * dry)+(inputSampleL * wet);
		inputSampleR = (drySampleR * dry)+(inputSampleR * wet);		
		
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