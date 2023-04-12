/* ========================================
 *  NotJustAnotherCD - NotJustAnotherCD.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __NotJustAnotherCD_H
#include "NotJustAnotherCD.h"
#endif

void NotJustAnotherCD::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double inputSampleL;
	double inputSampleR;
	
	double benfordize;
	int hotbinA;
	int hotbinB;
	double totalA;
	double totalB;
	float drySampleL;
	float drySampleR;
    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;

		inputSampleL -= noiseShapingL;
		inputSampleR -= noiseShapingR;
		
		inputSampleL *= 32768.0;
		inputSampleR *= 32768.0;
		//0-1 is now one bit, now we dither
		
		//begin L
		benfordize = floor(inputSampleL);
		while (benfordize >= 1.0) {benfordize /= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		hotbinA = floor(benfordize);
		//hotbin becomes the Benford bin value for this number floored
		totalA = 0;
		if ((hotbinA > 0) && (hotbinA < 10))
		{
			bynL[hotbinA] += 1;
			totalA += (301-bynL[1]);
			totalA += (176-bynL[2]);
			totalA += (125-bynL[3]);
			totalA += (97-bynL[4]);
			totalA += (79-bynL[5]);
			totalA += (67-bynL[6]);
			totalA += (58-bynL[7]);
			totalA += (51-bynL[8]);
			totalA += (46-bynL[9]);
			bynL[hotbinA] -= 1;
		} else {hotbinA = 10;}
		//produce total number- smaller is closer to Benford real
		
		benfordize = ceil(inputSampleL);
		while (benfordize >= 1.0) {benfordize /= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		hotbinB = floor(benfordize);
		//hotbin becomes the Benford bin value for this number ceiled
		totalB = 0;
		if ((hotbinB > 0) && (hotbinB < 10))
		{
			bynL[hotbinB] += 1;
			totalB += (301-bynL[1]);
			totalB += (176-bynL[2]);
			totalB += (125-bynL[3]);
			totalB += (97-bynL[4]);
			totalB += (79-bynL[5]);
			totalB += (67-bynL[6]);
			totalB += (58-bynL[7]);
			totalB += (51-bynL[8]);
			totalB += (46-bynL[9]);
			bynL[hotbinB] -= 1;
		} else {hotbinB = 10;}
		//produce total number- smaller is closer to Benford real
		
		if (totalA < totalB)
		{
			bynL[hotbinA] += 1;
			inputSampleL = floor(inputSampleL);
		}
		else
		{
			bynL[hotbinB] += 1;
			inputSampleL = ceil(inputSampleL);
		}
		//assign the relevant one to the delay line
		//and floor/ceil signal accordingly
		
		totalA = bynL[1] + bynL[2] + bynL[3] + bynL[4] + bynL[5] + bynL[6] + bynL[7] + bynL[8] + bynL[9];
		totalA /= 1000;
		totalA = 1; // spotted by Laserbat: this 'scaling back' code doesn't. It always divides by the fallback of 1. Old NJAD doesn't scale back the things we're comparing against. Kept to retain known behavior, use the one in StudioTan and Monitoring for a tuned-as-intended NJAD.
		bynL[1] /= totalA;
		bynL[2] /= totalA;
		bynL[3] /= totalA;
		bynL[4] /= totalA;
		bynL[5] /= totalA;
		bynL[6] /= totalA;
		bynL[7] /= totalA;
		bynL[8] /= totalA;
		bynL[9] /= totalA;
		bynL[10] /= 2; //catchall for garbage data
		//end L
		
		//begin R
		benfordize = floor(inputSampleR);
		while (benfordize >= 1.0) {benfordize /= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		hotbinA = floor(benfordize);
		//hotbin becomes the Benford bin value for this number floored
		totalA = 0;
		if ((hotbinA > 0) && (hotbinA < 10))
		{
			bynR[hotbinA] += 1;
			totalA += (301-bynR[1]);
			totalA += (176-bynR[2]);
			totalA += (125-bynR[3]);
			totalA += (97-bynR[4]);
			totalA += (79-bynR[5]);
			totalA += (67-bynR[6]);
			totalA += (58-bynR[7]);
			totalA += (51-bynR[8]);
			totalA += (46-bynR[9]);
			bynR[hotbinA] -= 1;
		} else {hotbinA = 10;}
		//produce total number- smaller is closer to Benford real
		
		benfordize = ceil(inputSampleR);
		while (benfordize >= 1.0) {benfordize /= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		hotbinB = floor(benfordize);
		//hotbin becomes the Benford bin value for this number ceiled
		totalB = 0;
		if ((hotbinB > 0) && (hotbinB < 10))
		{
			bynR[hotbinB] += 1;
			totalB += (301-bynR[1]);
			totalB += (176-bynR[2]);
			totalB += (125-bynR[3]);
			totalB += (97-bynR[4]);
			totalB += (79-bynR[5]);
			totalB += (67-bynR[6]);
			totalB += (58-bynR[7]);
			totalB += (51-bynR[8]);
			totalB += (46-bynR[9]);
			bynR[hotbinB] -= 1;
		} else {hotbinB = 10;}
		//produce total number- smaller is closer to Benford real
		
		if (totalA < totalB)
		{
			bynR[hotbinA] += 1;
			inputSampleR = floor(inputSampleR);
		}
		else
		{
			bynR[hotbinB] += 1;
			inputSampleR = ceil(inputSampleR);
		}
		//assign the relevant one to the delay line
		//and floor/ceil signal accordingly
		
		totalA = bynR[1] + bynR[2] + bynR[3] + bynR[4] + bynR[5] + bynR[6] + bynR[7] + bynR[8] + bynR[9];
		totalA /= 1000;
		totalA = 1; // spotted by Laserbat: this 'scaling back' code doesn't. It always divides by the fallback of 1. Old NJAD doesn't scale back the things we're comparing against. Kept to retain known behavior, use the one in StudioTan and Monitoring for a tuned-as-intended NJAD.
		bynR[1] /= totalA;
		bynR[2] /= totalA;
		bynR[3] /= totalA;
		bynR[4] /= totalA;
		bynR[5] /= totalA;
		bynR[6] /= totalA;
		bynR[7] /= totalA;
		bynR[8] /= totalA;
		bynR[9] /= totalA;
		bynR[10] /= 2; //catchall for garbage data
		//end R
		
		inputSampleL /= 32768.0;
		inputSampleR /= 32768.0;
		
		noiseShapingL += inputSampleL - drySampleL;
		noiseShapingR += inputSampleR - drySampleR;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void NotJustAnotherCD::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	
	double inputSampleL;
	double inputSampleR;
	
	double benfordize;
	int hotbinA;
	int hotbinB;
	double totalA;
	double totalB;
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
		
		
		inputSampleL -= noiseShapingL;
		inputSampleR -= noiseShapingR;
		
		inputSampleL *= 32768.0;
		inputSampleR *= 32768.0;
		//0-1 is now one bit, now we dither
		
		//begin L
		benfordize = floor(inputSampleL);
		while (benfordize >= 1.0) {benfordize /= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		hotbinA = floor(benfordize);
		//hotbin becomes the Benford bin value for this number floored
		totalA = 0;
		if ((hotbinA > 0) && (hotbinA < 10))
		{
			bynL[hotbinA] += 1;
			totalA += (301-bynL[1]);
			totalA += (176-bynL[2]);
			totalA += (125-bynL[3]);
			totalA += (97-bynL[4]);
			totalA += (79-bynL[5]);
			totalA += (67-bynL[6]);
			totalA += (58-bynL[7]);
			totalA += (51-bynL[8]);
			totalA += (46-bynL[9]);
			bynL[hotbinA] -= 1;
		} else {hotbinA = 10;}
		//produce total number- smaller is closer to Benford real
		
		benfordize = ceil(inputSampleL);
		while (benfordize >= 1.0) {benfordize /= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		hotbinB = floor(benfordize);
		//hotbin becomes the Benford bin value for this number ceiled
		totalB = 0;
		if ((hotbinB > 0) && (hotbinB < 10))
		{
			bynL[hotbinB] += 1;
			totalB += (301-bynL[1]);
			totalB += (176-bynL[2]);
			totalB += (125-bynL[3]);
			totalB += (97-bynL[4]);
			totalB += (79-bynL[5]);
			totalB += (67-bynL[6]);
			totalB += (58-bynL[7]);
			totalB += (51-bynL[8]);
			totalB += (46-bynL[9]);
			bynL[hotbinB] -= 1;
		} else {hotbinB = 10;}
		//produce total number- smaller is closer to Benford real
		
		if (totalA < totalB)
		{
			bynL[hotbinA] += 1;
			inputSampleL = floor(inputSampleL);
		}
		else
		{
			bynL[hotbinB] += 1;
			inputSampleL = ceil(inputSampleL);
		}
		//assign the relevant one to the delay line
		//and floor/ceil signal accordingly
		
		totalA = bynL[1] + bynL[2] + bynL[3] + bynL[4] + bynL[5] + bynL[6] + bynL[7] + bynL[8] + bynL[9];
		totalA /= 1000;
		totalA = 1; // spotted by Laserbat: this 'scaling back' code doesn't. It always divides by the fallback of 1. Old NJAD doesn't scale back the things we're comparing against. Kept to retain known behavior, use the one in StudioTan and Monitoring for a tuned-as-intended NJAD.
		bynL[1] /= totalA;
		bynL[2] /= totalA;
		bynL[3] /= totalA;
		bynL[4] /= totalA;
		bynL[5] /= totalA;
		bynL[6] /= totalA;
		bynL[7] /= totalA;
		bynL[8] /= totalA;
		bynL[9] /= totalA;
		bynL[10] /= 2; //catchall for garbage data
		//end L
		
		//begin R
		benfordize = floor(inputSampleR);
		while (benfordize >= 1.0) {benfordize /= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		hotbinA = floor(benfordize);
		//hotbin becomes the Benford bin value for this number floored
		totalA = 0;
		if ((hotbinA > 0) && (hotbinA < 10))
		{
			bynR[hotbinA] += 1;
			totalA += (301-bynR[1]);
			totalA += (176-bynR[2]);
			totalA += (125-bynR[3]);
			totalA += (97-bynR[4]);
			totalA += (79-bynR[5]);
			totalA += (67-bynR[6]);
			totalA += (58-bynR[7]);
			totalA += (51-bynR[8]);
			totalA += (46-bynR[9]);
			bynR[hotbinA] -= 1;
		} else {hotbinA = 10;}
		//produce total number- smaller is closer to Benford real
		
		benfordize = ceil(inputSampleR);
		while (benfordize >= 1.0) {benfordize /= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		if (benfordize < 1.0) {benfordize *= 10;}
		hotbinB = floor(benfordize);
		//hotbin becomes the Benford bin value for this number ceiled
		totalB = 0;
		if ((hotbinB > 0) && (hotbinB < 10))
		{
			bynR[hotbinB] += 1;
			totalB += (301-bynR[1]);
			totalB += (176-bynR[2]);
			totalB += (125-bynR[3]);
			totalB += (97-bynR[4]);
			totalB += (79-bynR[5]);
			totalB += (67-bynR[6]);
			totalB += (58-bynR[7]);
			totalB += (51-bynR[8]);
			totalB += (46-bynR[9]);
			bynR[hotbinB] -= 1;
		} else {hotbinB = 10;}
		//produce total number- smaller is closer to Benford real
		
		if (totalA < totalB)
		{
			bynR[hotbinA] += 1;
			inputSampleR = floor(inputSampleR);
		}
		else
		{
			bynR[hotbinB] += 1;
			inputSampleR = ceil(inputSampleR);
		}
		//assign the relevant one to the delay line
		//and floor/ceil signal accordingly
		
		totalA = bynR[1] + bynR[2] + bynR[3] + bynR[4] + bynR[5] + bynR[6] + bynR[7] + bynR[8] + bynR[9];
		totalA /= 1000;
		totalA = 1; // spotted by Laserbat: this 'scaling back' code doesn't. It always divides by the fallback of 1. Old NJAD doesn't scale back the things we're comparing against. Kept to retain known behavior, use the one in StudioTan and Monitoring for a tuned-as-intended NJAD.
		bynR[1] /= totalA;
		bynR[2] /= totalA;
		bynR[3] /= totalA;
		bynR[4] /= totalA;
		bynR[5] /= totalA;
		bynR[6] /= totalA;
		bynR[7] /= totalA;
		bynR[8] /= totalA;
		bynR[9] /= totalA;
		bynR[10] /= 2; //catchall for garbage data
		//end R
		
		inputSampleL /= 32768.0;
		inputSampleR /= 32768.0;
		
		noiseShapingL += inputSampleL - drySampleL;
		noiseShapingR += inputSampleR - drySampleR;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}