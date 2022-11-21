/* ========================================
 *  Average - Average.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Average_H
#include "Average.h"
#endif

void Average::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	
	double correctionSample;
	double accumulatorSampleL;
	double accumulatorSampleR;
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	
	double overallscale = (A * 9.0)+1.0;
	double wet = B;
	//removed extra dry variable
	double gain = overallscale;
	
	if (gain > 1.0) {f[0] = 1.0; gain -= 1.0;} else {f[0] = gain; gain = 0.0;}
	if (gain > 1.0) {f[1] = 1.0; gain -= 1.0;} else {f[1] = gain; gain = 0.0;}
	if (gain > 1.0) {f[2] = 1.0; gain -= 1.0;} else {f[2] = gain; gain = 0.0;}
	if (gain > 1.0) {f[3] = 1.0; gain -= 1.0;} else {f[3] = gain; gain = 0.0;}
	if (gain > 1.0) {f[4] = 1.0; gain -= 1.0;} else {f[4] = gain; gain = 0.0;}
	if (gain > 1.0) {f[5] = 1.0; gain -= 1.0;} else {f[5] = gain; gain = 0.0;}
	if (gain > 1.0) {f[6] = 1.0; gain -= 1.0;} else {f[6] = gain; gain = 0.0;}
	if (gain > 1.0) {f[7] = 1.0; gain -= 1.0;} else {f[7] = gain; gain = 0.0;}
	if (gain > 1.0) {f[8] = 1.0; gain -= 1.0;} else {f[8] = gain; gain = 0.0;}
	if (gain > 1.0) {f[9] = 1.0; gain -= 1.0;} else {f[9] = gain; gain = 0.0;}
	//there, now we have a neat little moving average with remainders
	
	if (overallscale < 1.0) overallscale = 1.0;
	f[0] /= overallscale;
	f[1] /= overallscale;
	f[2] /= overallscale;
	f[3] /= overallscale;
	f[4] /= overallscale;
	f[5] /= overallscale;
	f[6] /= overallscale;
	f[7] /= overallscale;
	f[8] /= overallscale;
	f[9] /= overallscale;
	//and now it's neatly scaled, too
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;

		bL[9] = bL[8]; bL[8] = bL[7]; bL[7] = bL[6]; bL[6] = bL[5];
		bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1];
		bL[1] = bL[0]; bL[0] = accumulatorSampleL = inputSampleL;

		bR[9] = bR[8]; bR[8] = bR[7]; bR[7] = bR[6]; bR[6] = bR[5];
		bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1];
		bR[1] = bR[0]; bR[0] = accumulatorSampleR = inputSampleR;
		//primitive way of doing this: for larger batches of samples, you might
		//try using a circular buffer like in a reverb. If you add the new sample
		//and subtract the one on the end you can keep a running tally of the samples
		//between. Beware of tiny floating-point math errors eventually screwing up
		//your system, though!
		
		accumulatorSampleL *= f[0];
		accumulatorSampleL += (bL[1] * f[1]);
		accumulatorSampleL += (bL[2] * f[2]);
		accumulatorSampleL += (bL[3] * f[3]);
		accumulatorSampleL += (bL[4] * f[4]);
		accumulatorSampleL += (bL[5] * f[5]);
		accumulatorSampleL += (bL[6] * f[6]);
		accumulatorSampleL += (bL[7] * f[7]);
		accumulatorSampleL += (bL[8] * f[8]);
		accumulatorSampleL += (bL[9] * f[9]);

		accumulatorSampleR *= f[0];
		accumulatorSampleR += (bR[1] * f[1]);
		accumulatorSampleR += (bR[2] * f[2]);
		accumulatorSampleR += (bR[3] * f[3]);
		accumulatorSampleR += (bR[4] * f[4]);
		accumulatorSampleR += (bR[5] * f[5]);
		accumulatorSampleR += (bR[6] * f[6]);
		accumulatorSampleR += (bR[7] * f[7]);
		accumulatorSampleR += (bR[8] * f[8]);
		accumulatorSampleR += (bR[9] * f[9]);
		//we are doing our repetitive calculations on a separate value
		
		correctionSample = inputSampleL - accumulatorSampleL;
		//we're gonna apply the total effect of all these calculations as a single subtract
		inputSampleL -= correctionSample;

		correctionSample = inputSampleR - accumulatorSampleR;
		inputSampleR -= correctionSample;
		//our one math operation on the input data coming in
		
		if (wet < 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//dry/wet control only applies if you're using it. We don't do a multiply by 1.0
		//if it 'won't change anything' but our sample might be at a very different scaling
		//in the floating point system.
		
		
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

void Average::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];	
	double correctionSample;
	double accumulatorSampleL;
	double accumulatorSampleR;
	double drySampleL;
	double drySampleR;
	double inputSampleL;
	double inputSampleR;
	
	double overallscale = (A * 9.0)+1.0;
	double wet = B;
	//removed extra dry variable
	double gain = overallscale;
	
	if (gain > 1.0) {f[0] = 1.0; gain -= 1.0;} else {f[0] = gain; gain = 0.0;}
	if (gain > 1.0) {f[1] = 1.0; gain -= 1.0;} else {f[1] = gain; gain = 0.0;}
	if (gain > 1.0) {f[2] = 1.0; gain -= 1.0;} else {f[2] = gain; gain = 0.0;}
	if (gain > 1.0) {f[3] = 1.0; gain -= 1.0;} else {f[3] = gain; gain = 0.0;}
	if (gain > 1.0) {f[4] = 1.0; gain -= 1.0;} else {f[4] = gain; gain = 0.0;}
	if (gain > 1.0) {f[5] = 1.0; gain -= 1.0;} else {f[5] = gain; gain = 0.0;}
	if (gain > 1.0) {f[6] = 1.0; gain -= 1.0;} else {f[6] = gain; gain = 0.0;}
	if (gain > 1.0) {f[7] = 1.0; gain -= 1.0;} else {f[7] = gain; gain = 0.0;}
	if (gain > 1.0) {f[8] = 1.0; gain -= 1.0;} else {f[8] = gain; gain = 0.0;}
	if (gain > 1.0) {f[9] = 1.0; gain -= 1.0;} else {f[9] = gain; gain = 0.0;}
	//there, now we have a neat little moving average with remainders
	
	if (overallscale < 1.0) overallscale = 1.0;
	f[0] /= overallscale;
	f[1] /= overallscale;
	f[2] /= overallscale;
	f[3] /= overallscale;
	f[4] /= overallscale;
	f[5] /= overallscale;
	f[6] /= overallscale;
	f[7] /= overallscale;
	f[8] /= overallscale;
	f[9] /= overallscale;
	//and now it's neatly scaled, too
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		bL[9] = bL[8]; bL[8] = bL[7]; bL[7] = bL[6]; bL[6] = bL[5];
		bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1];
		bL[1] = bL[0]; bL[0] = accumulatorSampleL = inputSampleL;
		
		bR[9] = bR[8]; bR[8] = bR[7]; bR[7] = bR[6]; bR[6] = bR[5];
		bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1];
		bR[1] = bR[0]; bR[0] = accumulatorSampleR = inputSampleR;
		//primitive way of doing this: for larger batches of samples, you might
		//try using a circular buffer like in a reverb. If you add the new sample
		//and subtract the one on the end you can keep a running tally of the samples
		//between. Beware of tiny floating-point math errors eventually screwing up
		//your system, though!
		
		accumulatorSampleL *= f[0];
		accumulatorSampleL += (bL[1] * f[1]);
		accumulatorSampleL += (bL[2] * f[2]);
		accumulatorSampleL += (bL[3] * f[3]);
		accumulatorSampleL += (bL[4] * f[4]);
		accumulatorSampleL += (bL[5] * f[5]);
		accumulatorSampleL += (bL[6] * f[6]);
		accumulatorSampleL += (bL[7] * f[7]);
		accumulatorSampleL += (bL[8] * f[8]);
		accumulatorSampleL += (bL[9] * f[9]);
		
		accumulatorSampleR *= f[0];
		accumulatorSampleR += (bR[1] * f[1]);
		accumulatorSampleR += (bR[2] * f[2]);
		accumulatorSampleR += (bR[3] * f[3]);
		accumulatorSampleR += (bR[4] * f[4]);
		accumulatorSampleR += (bR[5] * f[5]);
		accumulatorSampleR += (bR[6] * f[6]);
		accumulatorSampleR += (bR[7] * f[7]);
		accumulatorSampleR += (bR[8] * f[8]);
		accumulatorSampleR += (bR[9] * f[9]);
		//we are doing our repetitive calculations on a separate value
		
		correctionSample = inputSampleL - accumulatorSampleL;
		//we're gonna apply the total effect of all these calculations as a single subtract
		inputSampleL -= correctionSample;
		
		correctionSample = inputSampleR - accumulatorSampleR;
		inputSampleR -= correctionSample;
		//our one math operation on the input data coming in
		
		if (wet < 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//dry/wet control only applies if you're using it. We don't do a multiply by 1.0
		//if it 'won't change anything' but our sample might be at a very different scaling
		//in the floating point system.
		
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