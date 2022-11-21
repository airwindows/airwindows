/* ========================================
 *  C5RawChannel - C5RawChannel.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __C5RawChannel_H
#include "C5RawChannel.h"
#endif

void C5RawChannel::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	
	double centering = A * 0.5;
	centering = 1.0 - pow(centering,5);
	//we can set our centering force from zero to rather high, but
	//there's a really intense taper on it forcing it to mostly be almost 1.0.
	//If it's literally 1.0, we don't even apply it, and you get the original
	//Xmas Morning bugged-out Console5, which is the default setting for Raw Console5.
	double differenceL;
	double differenceR;
	
	double inputSampleL;
	double inputSampleR;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		differenceL = lastSampleChannelL - inputSampleL;
		lastSampleChannelL = inputSampleL;
		//derive slew part off direct sample measurement + from last time
		differenceR = lastSampleChannelR - inputSampleR;
		lastSampleChannelR = inputSampleR;
		//derive slew part off direct sample measurement + from last time
		
		if (differenceL > 1.0) differenceL = 1.0;
		if (differenceL < -1.0) differenceL = -1.0;
		if (differenceR > 1.0) differenceR = 1.0;
		if (differenceR < -1.0) differenceR = -1.0;

		inputSampleL = lastFXChannelL + asin(differenceL);
		lastFXChannelL = inputSampleL;
		if (centering < 1.0) lastFXChannelL *= centering;
		//if we're using the crude centering force, it's applied here
		inputSampleR = lastFXChannelR + asin(differenceR);
		lastFXChannelR = inputSampleR;
		if (centering < 1.0) lastFXChannelR *= centering;
		//if we're using the crude centering force, it's applied here

		if (lastFXChannelL > 1.0) lastFXChannelL = 1.0;
		if (lastFXChannelL < -1.0) lastFXChannelL = -1.0;
		if (lastFXChannelR > 1.0) lastFXChannelR = 1.0;
		if (lastFXChannelR < -1.0) lastFXChannelR = -1.0;
		//build new signal off what was present in output last time
		//slew aspect
		
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//amplitude aspect
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//amplitude aspect
		
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

void C5RawChannel::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	
	double centering = A * 0.5;
	centering = 1.0 - pow(centering,5);
	//we can set our centering force from zero to rather high, but
	//there's a really intense taper on it forcing it to mostly be almost 1.0.
	//If it's literally 1.0, we don't even apply it, and you get the original
	//Xmas Morning bugged-out Console5, which is the default setting for Raw Console5.
	double differenceL;
	double differenceR;
	
	double inputSampleL;
	double inputSampleR;

    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		differenceL = lastSampleChannelL - inputSampleL;
		lastSampleChannelL = inputSampleL;
		//derive slew part off direct sample measurement + from last time
		differenceR = lastSampleChannelR - inputSampleR;
		lastSampleChannelR = inputSampleR;
		//derive slew part off direct sample measurement + from last time
		
		if (differenceL > 1.0) differenceL = 1.0;
		if (differenceL < -1.0) differenceL = -1.0;
		if (differenceR > 1.0) differenceR = 1.0;
		if (differenceR < -1.0) differenceR = -1.0;
		
		inputSampleL = lastFXChannelL + asin(differenceL);
		lastFXChannelL = inputSampleL;
		if (centering < 1.0) lastFXChannelL *= centering;
		//if we're using the crude centering force, it's applied here
		inputSampleR = lastFXChannelR + asin(differenceR);
		lastFXChannelR = inputSampleR;
		if (centering < 1.0) lastFXChannelR *= centering;
		//if we're using the crude centering force, it's applied here
		
		if (lastFXChannelL > 1.0) lastFXChannelL = 1.0;
		if (lastFXChannelL < -1.0) lastFXChannelL = -1.0;
		if (lastFXChannelR > 1.0) lastFXChannelR = 1.0;
		if (lastFXChannelR < -1.0) lastFXChannelR = -1.0;
		//build new signal off what was present in output last time
		//slew aspect
		
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//amplitude aspect
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//amplitude aspect
		
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