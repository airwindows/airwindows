/* ========================================
 *  Golem - Golem.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Golem_H
#include "Golem.h"
#endif

void Golem::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	int phase = (int)((C * 5.999)+1);
	double balance = ((A*2.0)-1.0) / 2.0;
	double gainL = 0.5 - balance;
	double gainR = 0.5 + balance;
	double range = 30.0;
	if (phase == 3) range = 700.0;
	if (phase == 4) range = 700.0;
	double offset = pow((B*2.0)-1.0,5) * range;
	if (phase > 4) offset = 0.0;
	if (phase > 5)
	{
		gainL = 0.5;
		gainR = 0.5;
	}
	int near = (int)floor(fabs(offset));
	double farLevel = fabs(offset) - near;
	int far = near + 1;
	double nearLevel = 1.0 - farLevel;
	
	double inputSampleL;
	double inputSampleR;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		//assign working variables
		
		if (phase == 2) inputSampleL = -inputSampleL;
		if (phase == 4) inputSampleL = -inputSampleL;
		
		inputSampleL *= gainL;
		inputSampleR *= gainR;
		
		if (count < 1 || count > 2048) {count = 2048;}
		
		if (offset > 0)
		{
			p[count+2048] = p[count] = inputSampleL;
			inputSampleL = p[count+near]*nearLevel;
			inputSampleL += p[count+far]*farLevel;
			
			//consider adding third sample just to bring out superhighs subtly, like old interpolation hacks
			//or third and fifth samples, ditto
			
		}
		
		if (offset < 0)
		{
			p[count+2048] = p[count] = inputSampleR;
			inputSampleR = p[count+near]*nearLevel;
			inputSampleR += p[count+far]*farLevel;
		}
		
		count -= 1;
		
		inputSampleL = inputSampleL + inputSampleR;
		inputSampleR = inputSampleL;
		//the output is totally mono
		
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

void Golem::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	int phase = (int)((C * 5.999)+1);
	double balance = ((A*2.0)-1.0) / 2.0;
	double gainL = 0.5 - balance;
	double gainR = 0.5 + balance;
	double range = 30.0;
	if (phase == 3) range = 700.0;
	if (phase == 4) range = 700.0;
	double offset = pow((B*2.0)-1.0,5) * range;
	if (phase > 4) offset = 0.0;
	if (phase > 5)
	{
		gainL = 0.5;
		gainR = 0.5;
	}
	int near = (int)floor(fabs(offset));
	double farLevel = fabs(offset) - near;
	int far = near + 1;
	double nearLevel = 1.0 - farLevel;
	
	double inputSampleL;
	double inputSampleR;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		//assign working variables
		
		if (phase == 2) inputSampleL = -inputSampleL;
		if (phase == 4) inputSampleL = -inputSampleL;
		
		inputSampleL *= gainL;
		inputSampleR *= gainR;
		
		if (count < 1 || count > 2048) {count = 2048;}
		
		if (offset > 0)
		{
			p[count+2048] = p[count] = inputSampleL;
			inputSampleL = p[count+near]*nearLevel;
			inputSampleL += p[count+far]*farLevel;
			
			//consider adding third sample just to bring out superhighs subtly, like old interpolation hacks
			//or third and fifth samples, ditto
			
		}
		
		if (offset < 0)
		{
			p[count+2048] = p[count] = inputSampleR;
			inputSampleR = p[count+near]*nearLevel;
			inputSampleR += p[count+far]*farLevel;
		}
		
		count -= 1;
		
		inputSampleL = inputSampleL + inputSampleR;
		inputSampleR = inputSampleL;
		//the output is totally mono
		
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