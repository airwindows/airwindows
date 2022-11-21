/* ========================================
 *  TapeDither - TapeDither.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TapeDither_H
#include "TapeDither.h"
#endif

void TapeDither::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double currentDitherL;
	double currentDitherR;
	int processing = (VstInt32)( A * 1.999 );
	bool highres = false;
	if (processing == 1) highres = true;
	float scaleFactor;
	if (highres) scaleFactor = 8388608.0;
	else scaleFactor = 32768.0;
	float derez = B;
	if (derez > 0.0) scaleFactor *= pow(1.0-derez,6);
	if (scaleFactor < 0.0001) scaleFactor = 0.0001;
	float outScale = scaleFactor;
	if (outScale < 8.0) outScale = 8.0;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		
		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		currentDitherL = (double(fpdL)/UINT32_MAX);
		currentDitherR = (double(fpdR)/UINT32_MAX);
		
		inputSampleL += currentDitherL;
		inputSampleR += currentDitherR;
		inputSampleL -= previousDither4L;
		inputSampleR -= previousDither4R;
		
		inputSampleL = floor(inputSampleL);
		inputSampleR = floor(inputSampleR);

		previousDither4L = previousDither3L;
		previousDither3L = previousDither2L;
		previousDither2L = previousDither1L;
		previousDither1L = currentDitherL;

		previousDither4R = previousDither3R;
		previousDither3R = previousDither2R;
		previousDither2R = previousDither1R;
		previousDither1R = currentDitherR;
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;

		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//pseudorandom number updater

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void TapeDither::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double currentDitherL;
	double currentDitherR;
	int processing = (VstInt32)( A * 1.999 );
	bool highres = false;
	if (processing == 1) highres = true;
	float scaleFactor;
	if (highres) scaleFactor = 8388608.0;
	else scaleFactor = 32768.0;
	float derez = B;
	if (derez > 0.0) scaleFactor *= pow(1.0-derez,6);
	if (scaleFactor < 0.0001) scaleFactor = 0.0001;
	float outScale = scaleFactor;
	if (outScale < 8.0) outScale = 8.0;
    
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		
		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		currentDitherL = (double(fpdL)/UINT32_MAX);
		currentDitherR = (double(fpdR)/UINT32_MAX);
		
		inputSampleL += currentDitherL;
		inputSampleR += currentDitherR;
		inputSampleL -= previousDither4L;
		inputSampleR -= previousDither4R;
		
		inputSampleL = floor(inputSampleL);
		inputSampleR = floor(inputSampleR);
		
		previousDither4L = previousDither3L;
		previousDither3L = previousDither2L;
		previousDither2L = previousDither1L;
		previousDither1L = currentDitherL;
		
		previousDither4R = previousDither3R;
		previousDither3R = previousDither2R;
		previousDither2R = previousDither1R;
		previousDither1R = currentDitherR;
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//pseudorandom number updater

		*out1 = inputSampleL;
		*out2 = inputSampleR;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}