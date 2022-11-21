/* ========================================
 *  SurgeTide - SurgeTide.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SurgeTide_H
#include "SurgeTide.h"
#endif

void SurgeTide::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	
	double intensity = A;
	double attack = ((B+0.1)*0.0005)/overallscale;
	double decay = ((B+0.001)*0.00005)/overallscale;
	double wet = C;
	//removed extra dry variable
	double inputSense;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;

		inputSampleL *= 8.0;
		inputSampleR *= 8.0;
		inputSampleL *= intensity;
		inputSampleR *= intensity;
		
		inputSense = fabs(inputSampleL);
		if (fabs(inputSampleR) > inputSense)
			inputSense = fabs(inputSampleR);
		
		if (chaseC < inputSense) chaseA += attack;
		if (chaseC > inputSense) chaseA -= decay;
		
		if (chaseA > decay) chaseA = decay;
		if (chaseA < -attack) chaseA = -attack;
		
		chaseB += (chaseA/overallscale);
		
		if (chaseB > decay) chaseB = decay;
		if (chaseB < -attack) chaseB = -attack;
		
		chaseC += (chaseB/overallscale);
		if (chaseC > 1.0) chaseC = 1.0;
		if (chaseC < 0.0) chaseC = 0.0;
		
		inputSampleL *= chaseC;
		inputSampleL = drySampleL - (inputSampleL * intensity);
		inputSampleL = (drySampleL * (1.0-wet)) + (inputSampleL * wet);
		
		inputSampleR *= chaseC;
		inputSampleR = drySampleR - (inputSampleR * intensity);
		inputSampleR = (drySampleR * (1.0-wet)) + (inputSampleR * wet);
		
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

void SurgeTide::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	
	double intensity = A;
	double attack = ((B+0.1)*0.0005)/overallscale;
	double decay = ((B+0.001)*0.00005)/overallscale;
	double wet = C;
	//removed extra dry variable
	double inputSense;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;

		inputSampleL *= 8.0;
		inputSampleR *= 8.0;
		inputSampleL *= intensity;
		inputSampleR *= intensity;
		
		inputSense = fabs(inputSampleL);
		if (fabs(inputSampleR) > inputSense)
			inputSense = fabs(inputSampleR);
		
		if (chaseC < inputSense) chaseA += attack;
		if (chaseC > inputSense) chaseA -= decay;
		
		if (chaseA > decay) chaseA = decay;
		if (chaseA < -attack) chaseA = -attack;
		
		chaseB += (chaseA/overallscale);
		
		if (chaseB > decay) chaseB = decay;
		if (chaseB < -attack) chaseB = -attack;
		
		chaseC += (chaseB/overallscale);
		if (chaseC > 1.0) chaseC = 1.0;
		if (chaseC < 0.0) chaseC = 0.0;
		
		inputSampleL *= chaseC;
		inputSampleL = drySampleL - (inputSampleL * intensity);
		inputSampleL = (drySampleL * (1.0-wet)) + (inputSampleL * wet);
		
		inputSampleR *= chaseC;
		inputSampleR = drySampleR - (inputSampleR * intensity);
		inputSampleR = (drySampleR * (1.0-wet)) + (inputSampleR * wet);		

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