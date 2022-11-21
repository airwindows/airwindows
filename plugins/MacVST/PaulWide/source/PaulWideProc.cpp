/* ========================================
 *  PaulWide - PaulWide.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PaulWide_H
#include "PaulWide.h"
#endif

void PaulWide::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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
		//Paul Frindle: It's true that the dither itself can sound different 
		//if it's given a different freq response and you get to hear it. 
		//The one we use most is triangular single pole high pass dither. 
		//It not freq bent enough sound odd, but is slightly less audible than 
		//flat dither. It can also be easily made by taking one sample of dither 
		//away from the previous one - this gives you the triangular PDF and the 
		//filtering in one go :-)
		
		double currentDither = (double(fpdL)/UINT32_MAX);
		double ditherL = currentDither;
		ditherL -= previousDitherL;
		previousDitherL = currentDither;
		//TPDF: two 0-1 random noises
		
		currentDither = (double(fpdR)/UINT32_MAX);
		double ditherR = currentDither;
		ditherR -= previousDitherR;
		previousDitherR = currentDither;
		//TPDF: two 0-1 random noises
		
		if (fabs(ditherL-ditherR) < 0.5) {
			fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
			currentDither = (double(fpdL)/UINT32_MAX);
			ditherL = currentDither;
			ditherL -= previousDitherL;
			previousDitherL = currentDither;
		}
		
		if (fabs(ditherL-ditherR) < 0.5) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
			currentDither = (double(fpdR)/UINT32_MAX);
			ditherR = currentDither;
			ditherR -= previousDitherR;
			previousDitherR = currentDither;
		}
		
		if (fabs(ditherL-ditherR) < 0.5) {
			fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
			currentDither = (double(fpdL)/UINT32_MAX);
			ditherL = currentDither;
			ditherL -= previousDitherL;
			previousDitherL = currentDither;
		}
		
		inputSampleL = floor(inputSampleL+ditherL);
		inputSampleR = floor(inputSampleR+ditherR);
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//pseudorandom number updater

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void PaulWide::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
		//Paul Frindle: It's true that the dither itself can sound different 
		//if it's given a different freq response and you get to hear it. 
		//The one we use most is triangular single pole high pass dither. 
		//It not freq bent enough sound odd, but is slightly less audible than 
		//flat dither. It can also be easily made by taking one sample of dither 
		//away from the previous one - this gives you the triangular PDF and the 
		//filtering in one go :-)
		
		double currentDither = (double(fpdL)/UINT32_MAX);
		double ditherL = currentDither;
		ditherL -= previousDitherL;
		previousDitherL = currentDither;
		//TPDF: two 0-1 random noises
		
		currentDither = (double(fpdR)/UINT32_MAX);
		double ditherR = currentDither;
		ditherR -= previousDitherR;
		previousDitherR = currentDither;
		//TPDF: two 0-1 random noises
		
		if (fabs(ditherL-ditherR) < 0.5) {
			fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
			currentDither = (double(fpdL)/UINT32_MAX);
			ditherL = currentDither;
			ditherL -= previousDitherL;
			previousDitherL = currentDither;
		}
		
		if (fabs(ditherL-ditherR) < 0.5) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
			currentDither = (double(fpdR)/UINT32_MAX);
			ditherR = currentDither;
			ditherR -= previousDitherR;
			previousDitherR = currentDither;
		}
		
		if (fabs(ditherL-ditherR) < 0.5) {
			fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
			currentDither = (double(fpdL)/UINT32_MAX);
			ditherL = currentDither;
			ditherL -= previousDitherL;
			previousDitherL = currentDither;
		}
		
		inputSampleL = floor(inputSampleL+ditherL);
		inputSampleR = floor(inputSampleR+ditherR);
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//pseudorandom number updater

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
