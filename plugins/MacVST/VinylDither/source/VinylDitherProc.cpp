/* ========================================
 *  VinylDither - VinylDither.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __VinylDither_H
#include "VinylDither.h"
#endif

void VinylDither::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double absSample;

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
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		absSample = ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[0] += absSample; nsL[0] /= 2; absSample -= nsL[0];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[1] += absSample; nsL[1] /= 2; absSample -= nsL[1];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[2] += absSample; nsL[2] /= 2; absSample -= nsL[2];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[3] += absSample; nsL[3] /= 2; absSample -= nsL[3];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[4] += absSample; nsL[4] /= 2; absSample -= nsL[4];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[5] += absSample; nsL[5] /= 2; absSample -= nsL[5];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[6] += absSample; nsL[6] /= 2; absSample -= nsL[6];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[7] += absSample; nsL[7] /= 2; absSample -= nsL[7];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[8] += absSample; nsL[8] /= 2; absSample -= nsL[8];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[9] += absSample; nsL[9] /= 2; absSample -= nsL[9];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[10] += absSample; nsL[10] /= 2; absSample -= nsL[10];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[11] += absSample; nsL[11] /= 2; absSample -= nsL[11];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[12] += absSample; nsL[12] /= 2; absSample -= nsL[12];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[13] += absSample; nsL[13] /= 2; absSample -= nsL[13];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[14] += absSample; nsL[14] /= 2; absSample -= nsL[14];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[15] += absSample; nsL[15] /= 2; absSample -= nsL[15];
		//install noise and then shape it
		absSample += inputSampleL;
		
		if (NSOddL > 0) NSOddL -= 0.97;
		if (NSOddL < 0) NSOddL += 0.97;
		
		NSOddL -= (NSOddL * NSOddL * NSOddL * 0.475);
		
		NSOddL += prevL;
		absSample += (NSOddL*0.475);
		prevL = floor(absSample) - inputSampleL;
		inputSampleL = floor(absSample);
		//TenNines dither L
		
		absSample = ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[0] += absSample; nsR[0] /= 2; absSample -= nsR[0];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[1] += absSample; nsR[1] /= 2; absSample -= nsR[1];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[2] += absSample; nsR[2] /= 2; absSample -= nsR[2];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[3] += absSample; nsR[3] /= 2; absSample -= nsR[3];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[4] += absSample; nsR[4] /= 2; absSample -= nsR[4];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[5] += absSample; nsR[5] /= 2; absSample -= nsR[5];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[6] += absSample; nsR[6] /= 2; absSample -= nsR[6];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[7] += absSample; nsR[7] /= 2; absSample -= nsR[7];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[8] += absSample; nsR[8] /= 2; absSample -= nsR[8];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[9] += absSample; nsR[9] /= 2; absSample -= nsR[9];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[10] += absSample; nsR[10] /= 2; absSample -= nsR[10];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[11] += absSample; nsR[11] /= 2; absSample -= nsR[11];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[12] += absSample; nsR[12] /= 2; absSample -= nsR[12];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[13] += absSample; nsR[13] /= 2; absSample -= nsR[13];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[14] += absSample; nsR[14] /= 2; absSample -= nsR[14];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[15] += absSample; nsR[15] /= 2; absSample -= nsR[15];
		//install noise and then shape it
		absSample += inputSampleR;
		
		if (NSOddR > 0) NSOddR -= 0.97;
		if (NSOddR < 0) NSOddR += 0.97;
		
		NSOddR -= (NSOddR * NSOddR * NSOddR * 0.475);
		
		NSOddR += prevR;
		absSample += (NSOddR*0.475);
		prevR = floor(absSample) - inputSampleR;
		inputSampleR = floor(absSample);
		//TenNines dither R
		
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

void VinylDither::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double absSample;

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
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		inputSampleL *= scaleFactor;
		inputSampleR *= scaleFactor;
		//0-1 is now one bit, now we dither
		
		absSample = ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[0] += absSample; nsL[0] /= 2; absSample -= nsL[0];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[1] += absSample; nsL[1] /= 2; absSample -= nsL[1];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[2] += absSample; nsL[2] /= 2; absSample -= nsL[2];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[3] += absSample; nsL[3] /= 2; absSample -= nsL[3];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[4] += absSample; nsL[4] /= 2; absSample -= nsL[4];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[5] += absSample; nsL[5] /= 2; absSample -= nsL[5];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[6] += absSample; nsL[6] /= 2; absSample -= nsL[6];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[7] += absSample; nsL[7] /= 2; absSample -= nsL[7];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[8] += absSample; nsL[8] /= 2; absSample -= nsL[8];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[9] += absSample; nsL[9] /= 2; absSample -= nsL[9];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[10] += absSample; nsL[10] /= 2; absSample -= nsL[10];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[11] += absSample; nsL[11] /= 2; absSample -= nsL[11];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[12] += absSample; nsL[12] /= 2; absSample -= nsL[12];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[13] += absSample; nsL[13] /= 2; absSample -= nsL[13];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[14] += absSample; nsL[14] /= 2; absSample -= nsL[14];
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		absSample += ((double(fpdL)/UINT32_MAX) - 0.5);
		nsL[15] += absSample; nsL[15] /= 2; absSample -= nsL[15];
		//install noise and then shape it
		absSample += inputSampleL;
		
		if (NSOddL > 0) NSOddL -= 0.97;
		if (NSOddL < 0) NSOddL += 0.97;
		
		NSOddL -= (NSOddL * NSOddL * NSOddL * 0.475);
		
		NSOddL += prevL;
		absSample += (NSOddL*0.475);
		prevL = floor(absSample) - inputSampleL;
		inputSampleL = floor(absSample);
		//TenNines dither L
		
		absSample = ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[0] += absSample; nsR[0] /= 2; absSample -= nsR[0];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[1] += absSample; nsR[1] /= 2; absSample -= nsR[1];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[2] += absSample; nsR[2] /= 2; absSample -= nsR[2];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[3] += absSample; nsR[3] /= 2; absSample -= nsR[3];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[4] += absSample; nsR[4] /= 2; absSample -= nsR[4];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[5] += absSample; nsR[5] /= 2; absSample -= nsR[5];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[6] += absSample; nsR[6] /= 2; absSample -= nsR[6];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[7] += absSample; nsR[7] /= 2; absSample -= nsR[7];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[8] += absSample; nsR[8] /= 2; absSample -= nsR[8];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[9] += absSample; nsR[9] /= 2; absSample -= nsR[9];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[10] += absSample; nsR[10] /= 2; absSample -= nsR[10];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[11] += absSample; nsR[11] /= 2; absSample -= nsR[11];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[12] += absSample; nsR[12] /= 2; absSample -= nsR[12];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[13] += absSample; nsR[13] /= 2; absSample -= nsR[13];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[14] += absSample; nsR[14] /= 2; absSample -= nsR[14];
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		absSample += ((double(fpdR)/UINT32_MAX) - 0.5);
		nsR[15] += absSample; nsR[15] /= 2; absSample -= nsR[15];
		//install noise and then shape it
		absSample += inputSampleR;
		
		if (NSOddR > 0) NSOddR -= 0.97;
		if (NSOddR < 0) NSOddR += 0.97;
		
		NSOddR -= (NSOddR * NSOddR * NSOddR * 0.475);
		
		NSOddR += prevR;
		absSample += (NSOddR*0.475);
		prevR = floor(absSample) - inputSampleR;
		inputSampleR = floor(absSample);
		//TenNines dither R
		
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