/* ========================================
 *  Swell - Swell.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Swell_H
#include "Swell.h"
#endif

void Swell::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double thresholdOn = pow(A,2) * B;
	double speedOn = (pow(B,2)*0.001)/overallscale;
	double thresholdOff = thresholdOn * B;
	double speedOff = (sin(B)*0.01)/overallscale;
	double wet = C;
	//removed extra dry variable	
	
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		if (fabs(inputSampleL) > thresholdOn && louderL == false) louderL = true;
		if (fabs(inputSampleL) < thresholdOff && louderL == true) louderL = false;
		if (louderL == true) swellL = (swellL * (1.0 - speedOn)) + speedOn;
		else swellL *= (1.0 - speedOff);
		//both poles are a Zeno's arrow: approach but never get to either 1.0 or 0.0
		inputSampleL *= swellL;
		
		if (fabs(inputSampleR) > thresholdOn && louderR == false) louderR = true;
		if (fabs(inputSampleR) < thresholdOff && louderR == true) louderR = false;
		if (louderR == true) swellR = (swellR * (1.0 - speedOn)) + speedOn;
		else swellR *= (1.0 - speedOff);
		//both poles are a Zeno's arrow: approach but never get to either 1.0 or 0.0
		inputSampleR *= swellR;
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		
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

void Swell::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double thresholdOn = pow(A,2) * B;
	double speedOn = (pow(B,2)*0.001)/overallscale;
	double thresholdOff = thresholdOn * B;
	double speedOff = (sin(B)*0.01)/overallscale;
	double wet = C;
	//removed extra dry variable	

	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		if (fabs(inputSampleL) > thresholdOn && louderL == false) louderL = true;
		if (fabs(inputSampleL) < thresholdOff && louderL == true) louderL = false;
		if (louderL == true) swellL = (swellL * (1.0 - speedOn)) + speedOn;
		else swellL *= (1.0 - speedOff);
		//both poles are a Zeno's arrow: approach but never get to either 1.0 or 0.0
		inputSampleL *= swellL;
		
		if (fabs(inputSampleR) > thresholdOn && louderR == false) louderR = true;
		if (fabs(inputSampleR) < thresholdOff && louderR == true) louderR = false;
		if (louderR == true) swellR = (swellR * (1.0 - speedOn)) + speedOn;
		else swellR *= (1.0 - speedOff);
		//both poles are a Zeno's arrow: approach but never get to either 1.0 or 0.0
		inputSampleR *= swellR;
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		
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