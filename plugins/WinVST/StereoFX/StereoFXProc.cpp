/* ========================================
 *  StereoFX - StereoFX.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __StereoFX_H
#include "StereoFX.h"
#endif

void StereoFX::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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
	double mid;
	double side;
	//High Impact section
	double stereowide = A;
	double centersquish = C;
	double density = stereowide * 2.4;
	double sustain = 1.0 - (1.0/(1.0 + (density/7.0)));
	//this way, enhance increases up to 50% and then mid falls off beyond that
	double bridgerectifier;
	double count;
	//Highpass section
	double iirAmount = pow(B,3)/overallscale;
	double tight = -0.33333333333333;
	double offset;
	//we are setting it up so that to either extreme we can get an audible sound,
	//but sort of scaled so small adjustments don't shift the cutoff frequency yet.
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		//assign working variables		
		mid = inputSampleL + inputSampleR;
		side = inputSampleL - inputSampleR;
		//assign mid and side. Now, High Impact code
		count = density;
		while (count > 1.0)
		{
			bridgerectifier = fabs(side)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			bridgerectifier = sin(bridgerectifier);
			if (side > 0.0) side = bridgerectifier;
			else side = -bridgerectifier;
			count = count - 1.0;
		}
		//we have now accounted for any really high density settings.
		bridgerectifier = fabs(side)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (side > 0) side = (side*(1-count))+(bridgerectifier*count);
		else side = (side*(1-count))-(bridgerectifier*count);
		//blend according to density control
		//done first density. Next, sustain-reducer
		bridgerectifier = fabs(side)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (1-cos(bridgerectifier))*3.141592653589793;
		if (side > 0) side = (side*(1-sustain))+(bridgerectifier*sustain);
		else side = (side*(1-sustain))-(bridgerectifier*sustain);
		//done with High Impact code
		
		//now, Highpass code
		offset = 0.666666666666666 + ((1-fabs(side))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		if (flip)
		{
			iirSampleA = (iirSampleA * (1 - (offset * iirAmount))) + (side * (offset * iirAmount));
			side = side - iirSampleA;
		}
		else
		{
			iirSampleB = (iirSampleB * (1 - (offset * iirAmount))) + (side * (offset * iirAmount));
			side = side - iirSampleB;
		}
		//done with Highpass code
		
		bridgerectifier = fabs(mid)/1.273239544735162;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier)*1.273239544735162;
		if (mid > 0) mid = (mid*(1-centersquish))+(bridgerectifier*centersquish);
		else mid = (mid*(1-centersquish))-(bridgerectifier*centersquish);
		//done with the mid saturating section.
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		
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

void StereoFX::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
	double mid;
	double side;
	//High Impact section
	double stereowide = A;
	double centersquish = C;
	double density = stereowide * 2.4;
	double sustain = 1.0 - (1.0/(1.0 + (density/7.0)));
	//this way, enhance increases up to 50% and then mid falls off beyond that
	double bridgerectifier;
	double count;
	//Highpass section
	double iirAmount = pow(B,3)/overallscale;
	double tight = -0.33333333333333;
	double offset;
	//we are setting it up so that to either extreme we can get an audible sound,
	//but sort of scaled so small adjustments don't shift the cutoff frequency yet.
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		//assign working variables		
		mid = inputSampleL + inputSampleR;
		side = inputSampleL - inputSampleR;
		//assign mid and side. Now, High Impact code
		count = density;
		while (count > 1.0)
		{
			bridgerectifier = fabs(side)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			bridgerectifier = sin(bridgerectifier);
			if (side > 0.0) side = bridgerectifier;
			else side = -bridgerectifier;
			count = count - 1.0;
		}
		//we have now accounted for any really high density settings.
		bridgerectifier = fabs(side)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		bridgerectifier = sin(bridgerectifier);
		if (side > 0) side = (side*(1-count))+(bridgerectifier*count);
		else side = (side*(1-count))-(bridgerectifier*count);
		//blend according to density control
		//done first density. Next, sustain-reducer
		bridgerectifier = fabs(side)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (1-cos(bridgerectifier))*3.141592653589793;
		if (side > 0) side = (side*(1-sustain))+(bridgerectifier*sustain);
		else side = (side*(1-sustain))-(bridgerectifier*sustain);
		//done with High Impact code
		
		//now, Highpass code
		offset = 0.666666666666666 + ((1-fabs(side))*tight);
		if (offset < 0) offset = 0;
		if (offset > 1) offset = 1;
		if (flip)
		{
			iirSampleA = (iirSampleA * (1 - (offset * iirAmount))) + (side * (offset * iirAmount));
			side = side - iirSampleA;
		}
		else
		{
			iirSampleB = (iirSampleB * (1 - (offset * iirAmount))) + (side * (offset * iirAmount));
			side = side - iirSampleB;
		}
		//done with Highpass code
		
		bridgerectifier = fabs(mid)/1.273239544735162;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier)*1.273239544735162;
		if (mid > 0) mid = (mid*(1-centersquish))+(bridgerectifier*centersquish);
		else mid = (mid*(1-centersquish))-(bridgerectifier*centersquish);
		//done with the mid saturating section.
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		
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