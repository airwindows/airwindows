/* ========================================
 *  TPDFWide - TPDFWide.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __TPDFWide_H
#include "TPDFWide.h"
#endif

void TPDFWide::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;

		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		double ditherL = -1.0;
		ditherL += (rand()/(double)RAND_MAX);
		ditherL += (rand()/(double)RAND_MAX);
		//TPDF: two 0-1 random noises
		
		double ditherR = -1.0;
		ditherR += (rand()/(double)RAND_MAX);
		ditherR += (rand()/(double)RAND_MAX);
		//TPDF: two 0-1 random noises
		
		if (fabs(ditherL-ditherR) < 0.5) {
			ditherL = -1.0;
			ditherL += (rand()/(double)RAND_MAX);
			ditherL += (rand()/(double)RAND_MAX);
		}
		
		if (fabs(ditherL-ditherR) < 0.5) {
			ditherR = -1.0;
			ditherR += (rand()/(double)RAND_MAX);
			ditherR += (rand()/(double)RAND_MAX);
		}
		
		if (fabs(ditherL-ditherR) < 0.5) {
			ditherL = -1.0;
			ditherL += (rand()/(double)RAND_MAX);
			ditherL += (rand()/(double)RAND_MAX);
		}
		
		inputSampleL = floor(inputSampleL+ditherL);
		inputSampleR = floor(inputSampleR+ditherR);
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void TPDFWide::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		
		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		double ditherL = -1.0;
		ditherL += (rand()/(double)RAND_MAX);
		ditherL += (rand()/(double)RAND_MAX);
		//TPDF: two 0-1 random noises
		
		double ditherR = -1.0;
		ditherR += (rand()/(double)RAND_MAX);
		ditherR += (rand()/(double)RAND_MAX);
		//TPDF: two 0-1 random noises
		
		if (fabs(ditherL-ditherR) < 0.5) {
			ditherL = -1.0;
			ditherL += (rand()/(double)RAND_MAX);
			ditherL += (rand()/(double)RAND_MAX);
		}
		
		if (fabs(ditherL-ditherR) < 0.5) {
			ditherR = -1.0;
			ditherR += (rand()/(double)RAND_MAX);
			ditherR += (rand()/(double)RAND_MAX);
		}
		
		if (fabs(ditherL-ditherR) < 0.5) {
			ditherL = -1.0;
			ditherL += (rand()/(double)RAND_MAX);
			ditherL += (rand()/(double)RAND_MAX);
		}
		
		inputSampleL = floor(inputSampleL+ditherL);
		inputSampleR = floor(inputSampleR+ditherR);
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
