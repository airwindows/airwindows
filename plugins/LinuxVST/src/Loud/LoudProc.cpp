/* ========================================
 *  Loud - Loud.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Loud_H
#include "Loud.h"
#endif

void Loud::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double boost = pow(A+1.0,5);
	double output = B;
	double wet = C;
	double dry = 1.0-wet;
	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	double clamp;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
				if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		drySampleL = inputSampleL;
		drySampleR = inputSampleR;

		//begin L
		inputSampleL *= boost;
		clamp = inputSampleL - lastSampleL;
		
		if (clamp > 0)
		{
			inputSampleL = -(inputSampleL - 1.0);
			inputSampleL *= 1.2566108;
			if (inputSampleL < 0.0) inputSampleL = 0.0;
			if (inputSampleL > 3.141527) inputSampleL = 3.141527;
			inputSampleL = sin(inputSampleL) * overallscale;
			if (clamp > inputSampleL) clamp = inputSampleL;
		}
		
		if (clamp < 0)
		{
			inputSampleL += 1.0;
			inputSampleL *= 1.2566108;
			if (inputSampleL < 0.0) inputSampleL = 0.0;
			if (inputSampleL > 3.141527) inputSampleL = 3.141527;
			inputSampleL = -sin(inputSampleL) * overallscale;
			if (clamp < inputSampleL) clamp = inputSampleL;
		}
		
		inputSampleL = lastSampleL + clamp;
		lastSampleL = inputSampleL;
		//finished L
		
		//begin R
		inputSampleR *= boost;
		clamp = inputSampleR - lastSampleR;
		
		if (clamp > 0)
		{
			inputSampleR = -(inputSampleR - 1.0);
			inputSampleR *= 1.2566108;
			if (inputSampleR < 0.0) inputSampleR = 0.0;
			if (inputSampleR > 3.141527) inputSampleR = 3.141527;
			inputSampleR = sin(inputSampleR) * overallscale;
			if (clamp > inputSampleR) clamp = inputSampleR;
		}
		
		if (clamp < 0)
		{
			inputSampleR += 1.0;
			inputSampleR *= 1.2566108;
			if (inputSampleR < 0.0) inputSampleR = 0.0;
			if (inputSampleR > 3.141527) inputSampleR = 3.141527;
			inputSampleR = -sin(inputSampleR) * overallscale;
			if (clamp < inputSampleR) clamp = inputSampleR;
		}
		
		inputSampleR = lastSampleR + clamp;
		lastSampleR = inputSampleR;
		//finished R
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		if (wet < 1.0) {
			inputSampleL = (drySampleL*dry)+(inputSampleL*wet);
			inputSampleR = (drySampleR*dry)+(inputSampleR*wet);
		}
		//nice little output stage template: if we have another scale of floating point
		//number, we really don't want to meaninglessly multiply that by 1.0.
		
		
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

void Loud::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double boost = pow(A+1.0,5);
	double output = B;
	double wet = C;
	double dry = 1.0-wet;
	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	double clamp;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		//begin L
		inputSampleL *= boost;
		clamp = inputSampleL - lastSampleL;
		
		if (clamp > 0)
		{
			inputSampleL = -(inputSampleL - 1.0);
			inputSampleL *= 1.2566108;
			if (inputSampleL < 0.0) inputSampleL = 0.0;
			if (inputSampleL > 3.141527) inputSampleL = 3.141527;
			inputSampleL = sin(inputSampleL) * overallscale;
			if (clamp > inputSampleL) clamp = inputSampleL;
		}
		
		if (clamp < 0)
		{
			inputSampleL += 1.0;
			inputSampleL *= 1.2566108;
			if (inputSampleL < 0.0) inputSampleL = 0.0;
			if (inputSampleL > 3.141527) inputSampleL = 3.141527;
			inputSampleL = -sin(inputSampleL) * overallscale;
			if (clamp < inputSampleL) clamp = inputSampleL;
		}
		
		inputSampleL = lastSampleL + clamp;
		lastSampleL = inputSampleL;
		//finished L
		
		//begin R
		inputSampleR *= boost;
		clamp = inputSampleR - lastSampleR;
		
		if (clamp > 0)
		{
			inputSampleR = -(inputSampleR - 1.0);
			inputSampleR *= 1.2566108;
			if (inputSampleR < 0.0) inputSampleR = 0.0;
			if (inputSampleR > 3.141527) inputSampleR = 3.141527;
			inputSampleR = sin(inputSampleR) * overallscale;
			if (clamp > inputSampleR) clamp = inputSampleR;
		}
		
		if (clamp < 0)
		{
			inputSampleR += 1.0;
			inputSampleR *= 1.2566108;
			if (inputSampleR < 0.0) inputSampleR = 0.0;
			if (inputSampleR > 3.141527) inputSampleR = 3.141527;
			inputSampleR = -sin(inputSampleR) * overallscale;
			if (clamp < inputSampleR) clamp = inputSampleR;
		}
		
		inputSampleR = lastSampleR + clamp;
		lastSampleR = inputSampleR;
		//finished R
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		if (wet < 1.0) {
			inputSampleL = (drySampleL*dry)+(inputSampleL*wet);
			inputSampleR = (drySampleR*dry)+(inputSampleR*wet);
		}
		//nice little output stage template: if we have another scale of floating point
		//number, we really don't want to meaninglessly multiply that by 1.0.
		
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