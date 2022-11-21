/* ========================================
 *  Desk - Desk.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Desk_H
#include "Desk.h"
#endif

void Desk::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double gain = 0.135;
	double slewgain = 0.208;	
	double prevslew = 0.333;
	double balanceB = 0.0001;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	slewgain *= overallscale;
	prevslew *= overallscale;
	balanceB /= overallscale;
	double balanceA = 1.0 - balanceB;
	double slew;
	double bridgerectifier;
	double combsample;
	

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
		
		//begin L
		slew = inputSampleL - lastSampleL;
		lastSampleL = inputSampleL;
		//Set up direct reference for slew
		
		bridgerectifier = fabs(slew*slewgain);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (slew > 0) slew = bridgerectifier/slewgain;
		else slew = -(bridgerectifier/slewgain);
		
		inputSampleL = (lastOutSampleL*balanceA) + (lastSampleL*balanceB) + slew;
		//go from last slewed, but include some raw values
		lastOutSampleL = inputSampleL;
		//Set up slewed reference
		
		combsample = fabs(drySampleL*lastSampleL);
		if (combsample > 1.0) combsample = 1.0;
		//bailout for very high input gains
		inputSampleL -= (lastSlewL * combsample * prevslew);
		lastSlewL = slew;
		//slew interaction with previous slew
		
		inputSampleL *= gain;
		bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		
		if (inputSampleL > 0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		//drive section
		inputSampleL /= gain;
		//end L
		
		//begin R
		slew = inputSampleR - lastSampleR;
		lastSampleR = inputSampleR;
		//Set up direct reference for slew
		
		bridgerectifier = fabs(slew*slewgain);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (slew > 0) slew = bridgerectifier/slewgain;
		else slew = -(bridgerectifier/slewgain);
		
		inputSampleR = (lastOutSampleR*balanceA) + (lastSampleR*balanceB) + slew;
		//go from last slewed, but include some raw values
		lastOutSampleR = inputSampleR;
		//Set up slewed reference
		
		combsample = fabs(drySampleR*lastSampleR);
		if (combsample > 1.0) combsample = 1.0;
		//bailout for very high input gains
		inputSampleR -= (lastSlewR * combsample * prevslew);
		lastSlewR = slew;
		//slew interaction with previous slew
		
		inputSampleR *= gain;
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		
		if (inputSampleR > 0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		//drive section
		inputSampleR /= gain;
		//end R
		
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

void Desk::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double gain = 0.135;
	double slewgain = 0.208;	
	double prevslew = 0.333;
	double balanceB = 0.0001;
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	slewgain *= overallscale;
	prevslew *= overallscale;
	balanceB /= overallscale;
	double balanceA = 1.0 - balanceB;
	double slew;
	double bridgerectifier;
	double combsample;
	

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

		//begin L
		slew = inputSampleL - lastSampleL;
		lastSampleL = inputSampleL;
		//Set up direct reference for slew
		
		bridgerectifier = fabs(slew*slewgain);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (slew > 0) slew = bridgerectifier/slewgain;
		else slew = -(bridgerectifier/slewgain);
		
		inputSampleL = (lastOutSampleL*balanceA) + (lastSampleL*balanceB) + slew;
		//go from last slewed, but include some raw values
		lastOutSampleL = inputSampleL;
		//Set up slewed reference
		
		combsample = fabs(drySampleL*lastSampleL);
		if (combsample > 1.0) combsample = 1.0;
		//bailout for very high input gains
		inputSampleL -= (lastSlewL * combsample * prevslew);
		lastSlewL = slew;
		//slew interaction with previous slew
		
		inputSampleL *= gain;
		bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		
		if (inputSampleL > 0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		//drive section
		inputSampleL /= gain;
		//end L
		
		//begin R
		slew = inputSampleR - lastSampleR;
		lastSampleR = inputSampleR;
		//Set up direct reference for slew
		
		bridgerectifier = fabs(slew*slewgain);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (slew > 0) slew = bridgerectifier/slewgain;
		else slew = -(bridgerectifier/slewgain);
		
		inputSampleR = (lastOutSampleR*balanceA) + (lastSampleR*balanceB) + slew;
		//go from last slewed, but include some raw values
		lastOutSampleR = inputSampleR;
		//Set up slewed reference
		
		combsample = fabs(drySampleR*lastSampleR);
		if (combsample > 1.0) combsample = 1.0;
		//bailout for very high input gains
		inputSampleR -= (lastSlewR * combsample * prevslew);
		lastSlewR = slew;
		//slew interaction with previous slew
		
		inputSampleR *= gain;
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		
		if (inputSampleR > 0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		//drive section
		inputSampleR /= gain;
		//end R

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