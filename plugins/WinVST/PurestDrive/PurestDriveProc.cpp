/* ========================================
 *  PurestDrive - PurestDrive.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PurestDrive_H
#include "PurestDrive.h"
#endif

void PurestDrive::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];


	double intensity = A;
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	double apply;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		inputSampleL = sin(inputSampleL);
		//basic distortion factor
		apply = (fabs(previousSampleL + inputSampleL) / 2.0) * intensity;
		//saturate less if previous sample was undistorted and low level, or if it was
		//inverse polarity. Lets through highs and brightness more.
		inputSampleL = (drySampleL * (1.0 - apply)) + (inputSampleL * apply);		
		//dry-wet control for intensity also has FM modulation to clean up highs
		previousSampleL = sin(drySampleL);
		//apply the sine while storing previous sample
		
		inputSampleR = sin(inputSampleR);
		//basic distortion factor
		apply = (fabs(previousSampleR + inputSampleR) / 2.0) * intensity;
		//saturate less if previous sample was undistorted and low level, or if it was
		//inverse polarity. Lets through highs and brightness more.
		inputSampleR = (drySampleR * (1.0 - apply)) + (inputSampleR * apply);		
		//dry-wet control for intensity also has FM modulation to clean up highs
		previousSampleR = sin(drySampleR);
		//apply the sine while storing previous sample
		
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

void PurestDrive::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	
	double intensity = A;
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	double apply;


    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		inputSampleL = sin(inputSampleL);
		//basic distortion factor
		apply = (fabs(previousSampleL + inputSampleL) / 2.0) * intensity;
		//saturate less if previous sample was undistorted and low level, or if it was
		//inverse polarity. Lets through highs and brightness more.
		inputSampleL = (drySampleL * (1.0 - apply)) + (inputSampleL * apply);		
		//dry-wet control for intensity also has FM modulation to clean up highs
		previousSampleL = sin(drySampleL);
		//apply the sine while storing previous sample
		
		inputSampleR = sin(inputSampleR);
		//basic distortion factor
		apply = (fabs(previousSampleR + inputSampleR) / 2.0) * intensity;
		//saturate less if previous sample was undistorted and low level, or if it was
		//inverse polarity. Lets through highs and brightness more.
		inputSampleR = (drySampleR * (1.0 - apply)) + (inputSampleR * apply);		
		//dry-wet control for intensity also has FM modulation to clean up highs
		previousSampleR = sin(drySampleR);
		//apply the sine while storing previous sample
		
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