/* ========================================
 *  NotJustAnotherDither - NotJustAnotherDither.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __NotJustAnotherDither_H
#include "NotJustAnotherDither.h"
#endif

void NotJustAnotherDither::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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

		//begin Not Just Another Dither		
		bool cutbinsL; cutbinsL = false;
		bool cutbinsR; cutbinsR = false;
		double drySampleL; drySampleL = inputSampleL;
		double drySampleR; drySampleR = inputSampleR;
		inputSampleL -= noiseShapingL;
		inputSampleR -= noiseShapingR;
		
		//NJAD L
		double benfordize; benfordize = floor(inputSampleL);
		while (benfordize >= 1.0) benfordize /= 10;
		while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;
		int hotbinA; hotbinA = floor(benfordize);
		//hotbin becomes the Benford bin value for this number floored
		double totalA; totalA = 0;
		if ((hotbinA > 0) && (hotbinA < 10))
		{
			bynL[hotbinA] += 1; if (bynL[hotbinA] > 982) cutbinsL = true;
			totalA += (301-bynL[1]); totalA += (176-bynL[2]); totalA += (125-bynL[3]);
			totalA += (97-bynL[4]); totalA += (79-bynL[5]); totalA += (67-bynL[6]);
			totalA += (58-bynL[7]); totalA += (51-bynL[8]); totalA += (46-bynL[9]);
			bynL[hotbinA] -= 1;
		} else hotbinA = 10;
		//produce total number- smaller is closer to Benford real
		
		benfordize = ceil(inputSampleL);
		while (benfordize >= 1.0) benfordize /= 10;
		while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;
		int hotbinB; hotbinB = floor(benfordize);
		//hotbin becomes the Benford bin value for this number ceiled
		double totalB; totalB = 0;
		if ((hotbinB > 0) && (hotbinB < 10))
		{
			bynL[hotbinB] += 1; if (bynL[hotbinB] > 982) cutbinsL = true;
			totalB += (301-bynL[1]); totalB += (176-bynL[2]); totalB += (125-bynL[3]);
			totalB += (97-bynL[4]); totalB += (79-bynL[5]); totalB += (67-bynL[6]);
			totalB += (58-bynL[7]); totalB += (51-bynL[8]); totalB += (46-bynL[9]);
			bynL[hotbinB] -= 1;
		} else hotbinB = 10;
		//produce total number- smaller is closer to Benford real
		
		double outputSample;
		if (totalA < totalB) {bynL[hotbinA] += 1; outputSample = floor(inputSampleL);}
		else {bynL[hotbinB] += 1; outputSample = floor(inputSampleL+1);}
		//assign the relevant one to the delay line
		//and floor/ceil signal accordingly
		if (cutbinsL) {
			bynL[1] *= 0.99; bynL[2] *= 0.99; bynL[3] *= 0.99; bynL[4] *= 0.99; bynL[5] *= 0.99; 
			bynL[6] *= 0.99; bynL[7] *= 0.99; bynL[8] *= 0.99; bynL[9] *= 0.99; bynL[10] *= 0.99; 
		}
		noiseShapingL += outputSample - drySampleL;			
		if (noiseShapingL > fabs(inputSampleL)) noiseShapingL = fabs(inputSampleL);
		if (noiseShapingL < -fabs(inputSampleL)) noiseShapingL = -fabs(inputSampleL);		
		//finished NJAD L
		
		//NJAD R
		benfordize = floor(inputSampleR);
		while (benfordize >= 1.0) benfordize /= 10;
		while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;		
		hotbinA = floor(benfordize);
		//hotbin becomes the Benford bin value for this number floored
		totalA = 0;
		if ((hotbinA > 0) && (hotbinA < 10))
		{
			bynR[hotbinA] += 1; if (bynR[hotbinA] > 982) cutbinsR = true;
			totalA += (301-bynR[1]); totalA += (176-bynR[2]); totalA += (125-bynR[3]);
			totalA += (97-bynR[4]); totalA += (79-bynR[5]); totalA += (67-bynR[6]);
			totalA += (58-bynR[7]); totalA += (51-bynR[8]); totalA += (46-bynR[9]);
			bynR[hotbinA] -= 1;
		} else hotbinA = 10;
		//produce total number- smaller is closer to Benford real
		
		benfordize = ceil(inputSampleR);
		while (benfordize >= 1.0) benfordize /= 10;
		while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;		
		hotbinB = floor(benfordize);
		//hotbin becomes the Benford bin value for this number ceiled
		totalB = 0;
		if ((hotbinB > 0) && (hotbinB < 10))
		{
			bynR[hotbinB] += 1; if (bynR[hotbinB] > 982) cutbinsR = true;
			totalB += (301-bynR[1]); totalB += (176-bynR[2]); totalB += (125-bynR[3]);
			totalB += (97-bynR[4]); totalB += (79-bynR[5]); totalB += (67-bynR[6]);
			totalB += (58-bynR[7]); totalB += (51-bynR[8]); totalB += (46-bynR[9]);
			bynR[hotbinB] -= 1;
		} else hotbinB = 10;
		//produce total number- smaller is closer to Benford real
		
		if (totalA < totalB) {bynR[hotbinA] += 1; outputSample = floor(inputSampleR);}
		else {bynR[hotbinB] += 1; outputSample = floor(inputSampleR+1);}
		//assign the relevant one to the delay line
		//and floor/ceil signal accordingly
		
		if (cutbinsR) {
			bynR[1] *= 0.99; bynR[2] *= 0.99; bynR[3] *= 0.99; bynR[4] *= 0.99; bynR[5] *= 0.99; 
			bynR[6] *= 0.99; bynR[7] *= 0.99; bynR[8] *= 0.99; bynR[9] *= 0.99; bynR[10] *= 0.99; 
		}
		noiseShapingR += outputSample - drySampleR;			
		if (noiseShapingR > fabs(inputSampleR)) noiseShapingR = fabs(inputSampleR);
		if (noiseShapingR < -fabs(inputSampleR)) noiseShapingR = -fabs(inputSampleR);
		//finished NJAD R
		
		
		inputSampleL /= outScale;
		inputSampleR /= outScale;

		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void NotJustAnotherDither::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
		
		//begin Not Just Another Dither		
		bool cutbinsL; cutbinsL = false;
		bool cutbinsR; cutbinsR = false;
		double drySampleL; drySampleL = inputSampleL;
		double drySampleR; drySampleR = inputSampleR;
		inputSampleL -= noiseShapingL;
		inputSampleR -= noiseShapingR;
		
		//NJAD L
		double benfordize; benfordize = floor(inputSampleL);
		while (benfordize >= 1.0) benfordize /= 10;
		while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;
		int hotbinA; hotbinA = floor(benfordize);
		//hotbin becomes the Benford bin value for this number floored
		double totalA; totalA = 0;
		if ((hotbinA > 0) && (hotbinA < 10))
		{
			bynL[hotbinA] += 1; if (bynL[hotbinA] > 982) cutbinsL = true;
			totalA += (301-bynL[1]); totalA += (176-bynL[2]); totalA += (125-bynL[3]);
			totalA += (97-bynL[4]); totalA += (79-bynL[5]); totalA += (67-bynL[6]);
			totalA += (58-bynL[7]); totalA += (51-bynL[8]); totalA += (46-bynL[9]);
			bynL[hotbinA] -= 1;
		} else hotbinA = 10;
		//produce total number- smaller is closer to Benford real
		
		benfordize = ceil(inputSampleL);
		while (benfordize >= 1.0) benfordize /= 10;
		while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;
		int hotbinB; hotbinB = floor(benfordize);
		//hotbin becomes the Benford bin value for this number ceiled
		double totalB; totalB = 0;
		if ((hotbinB > 0) && (hotbinB < 10))
		{
			bynL[hotbinB] += 1; if (bynL[hotbinB] > 982) cutbinsL = true;
			totalB += (301-bynL[1]); totalB += (176-bynL[2]); totalB += (125-bynL[3]);
			totalB += (97-bynL[4]); totalB += (79-bynL[5]); totalB += (67-bynL[6]);
			totalB += (58-bynL[7]); totalB += (51-bynL[8]); totalB += (46-bynL[9]);
			bynL[hotbinB] -= 1;
		} else hotbinB = 10;
		//produce total number- smaller is closer to Benford real
		
		double outputSampleL;
		if (totalA < totalB) {bynL[hotbinA] += 1; outputSampleL = floor(inputSampleL);}
		else {bynL[hotbinB] += 1; outputSampleL = floor(inputSampleL+1);}
		//assign the relevant one to the delay line
		//and floor/ceil signal accordingly
		if (cutbinsL) {
			bynL[1] *= 0.99; bynL[2] *= 0.99; bynL[3] *= 0.99; bynL[4] *= 0.99; bynL[5] *= 0.99; 
			bynL[6] *= 0.99; bynL[7] *= 0.99; bynL[8] *= 0.99; bynL[9] *= 0.99; bynL[10] *= 0.99; 
		}
		noiseShapingL += outputSampleL - drySampleL;			
		if (noiseShapingL > fabs(inputSampleL)) noiseShapingL = fabs(inputSampleL);
		if (noiseShapingL < -fabs(inputSampleL)) noiseShapingL = -fabs(inputSampleL);		
		//finished NJAD L
		
		//NJAD R
		benfordize = floor(inputSampleR);
		while (benfordize >= 1.0) benfordize /= 10;
		while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;		
		hotbinA = floor(benfordize);
		//hotbin becomes the Benford bin value for this number floored
		totalA = 0;
		if ((hotbinA > 0) && (hotbinA < 10))
		{
			bynR[hotbinA] += 1; if (bynR[hotbinA] > 982) cutbinsR = true;
			totalA += (301-bynR[1]); totalA += (176-bynR[2]); totalA += (125-bynR[3]);
			totalA += (97-bynR[4]); totalA += (79-bynR[5]); totalA += (67-bynR[6]);
			totalA += (58-bynR[7]); totalA += (51-bynR[8]); totalA += (46-bynR[9]);
			bynR[hotbinA] -= 1;
		} else hotbinA = 10;
		//produce total number- smaller is closer to Benford real
		
		benfordize = ceil(inputSampleR);
		while (benfordize >= 1.0) benfordize /= 10;
		while (benfordize < 1.0 && benfordize > 0.0000001) benfordize *= 10;		
		hotbinB = floor(benfordize);
		//hotbin becomes the Benford bin value for this number ceiled
		totalB = 0;
		if ((hotbinB > 0) && (hotbinB < 10))
		{
			bynR[hotbinB] += 1; if (bynR[hotbinB] > 982) cutbinsR = true;
			totalB += (301-bynR[1]); totalB += (176-bynR[2]); totalB += (125-bynR[3]);
			totalB += (97-bynR[4]); totalB += (79-bynR[5]); totalB += (67-bynR[6]);
			totalB += (58-bynR[7]); totalB += (51-bynR[8]); totalB += (46-bynR[9]);
			bynR[hotbinB] -= 1;
		} else hotbinB = 10;
		//produce total number- smaller is closer to Benford real
		
		double outputSampleR;
		if (totalA < totalB) {bynR[hotbinA] += 1; outputSampleR = floor(inputSampleR);}
		else {bynR[hotbinB] += 1; outputSampleR = floor(inputSampleR+1);}
		//assign the relevant one to the delay line
		//and floor/ceil signal accordingly
		
		if (cutbinsR) {
			bynR[1] *= 0.99; bynR[2] *= 0.99; bynR[3] *= 0.99; bynR[4] *= 0.99; bynR[5] *= 0.99; 
			bynR[6] *= 0.99; bynR[7] *= 0.99; bynR[8] *= 0.99; bynR[9] *= 0.99; bynR[10] *= 0.99; 
		}
		noiseShapingR += outputSampleR - drySampleR;			
		if (noiseShapingR > fabs(inputSampleR)) noiseShapingR = fabs(inputSampleR);
		if (noiseShapingR < -fabs(inputSampleR)) noiseShapingR = -fabs(inputSampleR);
		//finished NJAD R
		
		
		inputSampleL = outputSampleL / outScale;
		inputSampleR = outputSampleR / outScale;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}