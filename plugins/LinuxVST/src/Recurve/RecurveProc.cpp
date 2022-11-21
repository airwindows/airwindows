/* ========================================
 *  Recurve - Recurve.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Recurve_H
#include "Recurve.h"
#endif

void Recurve::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= 0.5;
		inputSampleR *= 0.5;
		
		if (gain < 0.0078125) gain = 0.0078125; if (gain > 2.0) gain = 2.0;
		//gain of 1,0 gives you a super-clean one, gain of 2 is obviously compressing
		//smaller number is maximum clamping, if too small it'll take a while to bounce back
		inputSampleL *= gain; inputSampleR *= gain;
		
		gain += sin((fabs(inputSampleL*4)>1)?4:fabs(inputSampleL*4))*pow(inputSampleL,4);
		gain += sin((fabs(inputSampleR*4)>1)?4:fabs(inputSampleR*4))*pow(inputSampleR,4);
		//4.71239 radians sined will turn to -1 which is the maximum gain reduction speed
		
		inputSampleL *= 2.0;
		inputSampleR *= 2.0;
		
		if (inputSampleL > 0.9999) inputSampleL = 0.9999;
		if (inputSampleR > 0.9999) inputSampleR = 0.9999;
		if (inputSampleL < -0.9999) inputSampleL = -0.9999;
		if (inputSampleR < -0.9999) inputSampleR = -0.9999;
		//iron bar so people can play with this as a loudenator
		
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

void Recurve::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= 0.5;
		inputSampleR *= 0.5;
		
		if (gain < 0.0078125) gain = 0.0078125; if (gain > 2.0) gain = 2.0;
		//gain of 1,0 gives you a super-clean one, gain of 2 is obviously compressing
		//smaller number is maximum clamping, if too small it'll take a while to bounce back
		inputSampleL *= gain; inputSampleR *= gain;
		
		gain += sin((fabs(inputSampleL*4)>1)?4:fabs(inputSampleL*4))*pow(inputSampleL,4);
		gain += sin((fabs(inputSampleR*4)>1)?4:fabs(inputSampleR*4))*pow(inputSampleR,4);
		//4.71239 radians sined will turn to -1 which is the maximum gain reduction speed
		
		inputSampleL *= 2.0;
		inputSampleR *= 2.0;
		
		if (inputSampleL > 0.9999) inputSampleL = 0.9999;
		if (inputSampleR > 0.9999) inputSampleR = 0.9999;
		if (inputSampleL < -0.9999) inputSampleL = -0.9999;
		if (inputSampleR < -0.9999) inputSampleR = -0.9999;
		//iron bar so people can play with this as a loudenator
		
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
