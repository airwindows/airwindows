/* ========================================
 *  TPDFDither - TPDFDither.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __TPDFDither_H
#include "TPDFDither.h"
#endif

void TPDFDither::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

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
		if (fabs(inputSampleL)<1.18e-37) inputSampleL = fpd * 1.18e-37;
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		if (fabs(inputSampleR)<1.18e-37) inputSampleR = fpd * 1.18e-37;
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;

		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		inputSampleL -= 1.0;
		inputSampleR -= 1.0;
		
		inputSampleL += (double(fpd)/UINT32_MAX);
		inputSampleR += (double(fpd)/UINT32_MAX);
		
		inputSampleL += (double(fpd)/UINT32_MAX);
		inputSampleR += (double(fpd)/UINT32_MAX);
		
		inputSampleL = floor(inputSampleL);
		inputSampleR = floor(inputSampleR);
		//TPDF: two 0-1 random noises
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void TPDFDither::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

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
		if (fabs(inputSampleL)<1.18e-43) inputSampleL = fpd * 1.18e-43;
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		if (fabs(inputSampleR)<1.18e-43) inputSampleR = fpd * 1.18e-43;
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		
		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		inputSampleL -= 1.0;
		inputSampleR -= 1.0;
		
		inputSampleL += (double(fpd)/UINT32_MAX);
		inputSampleR += (double(fpd)/UINT32_MAX);
		
		inputSampleL += (double(fpd)/UINT32_MAX);
		inputSampleR += (double(fpd)/UINT32_MAX);
		
		inputSampleL = floor(inputSampleL);
		inputSampleR = floor(inputSampleR);
		//TPDF: two 0-1 random noises
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}