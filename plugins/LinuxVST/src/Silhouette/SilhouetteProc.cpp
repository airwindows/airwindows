/* ========================================
 *  Silhouette - Silhouette.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#include "Silhouette.h"
#endif

void Silhouette::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double wet = pow(A,5);
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin L channel
		double bridgerectifier = fabs(inputSampleL)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = 1.0-cos(bridgerectifier);
		if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		
		double silhouette = (double(fpdL)/UINT32_MAX);
		silhouette -= 0.5;
		silhouette *= 2.0;
		silhouette *= fabs(inputSampleL);
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		double smoother = (double(fpdL)/UINT32_MAX);
		smoother -= 0.5;
		smoother *= 2.0;
		smoother *= fabs(lastSampleL);
		lastSampleL = inputSampleL;
		
		silhouette += smoother;
		
		bridgerectifier = fabs(silhouette)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (silhouette > 0.0) silhouette = bridgerectifier;
		else silhouette = -bridgerectifier;
		
		inputSampleL = (silhouette + outSampleL) / 2.0;
		outSampleL = silhouette;
		//end L channel
		
		//begin R channel
		bridgerectifier = fabs(inputSampleR)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = 1.0-cos(bridgerectifier);
		if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		
		silhouette = (double(fpdR)/UINT32_MAX);
		silhouette -= 0.5;
		silhouette *= 2.0;
		silhouette *= fabs(inputSampleR);
		
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		smoother = (double(fpdR)/UINT32_MAX);
		smoother -= 0.5;
		smoother *= 2.0;
		smoother *= fabs(lastSampleR);
		lastSampleR = inputSampleR;
		
		silhouette += smoother;
		
		bridgerectifier = fabs(silhouette)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (silhouette > 0.0) silhouette = bridgerectifier;
		else silhouette = -bridgerectifier;
		
		inputSampleR = (silhouette + outSampleR) / 2.0;
		outSampleR = silhouette;
		//end R channel
				
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void Silhouette::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double wet = pow(A,5);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin L channel
		double bridgerectifier = fabs(inputSampleL)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = 1.0-cos(bridgerectifier);
		if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		
		double silhouette = (double(fpdL)/UINT32_MAX);
		silhouette -= 0.5;
		silhouette *= 2.0;
		silhouette *= fabs(inputSampleL);
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		double smoother = (double(fpdL)/UINT32_MAX);
		smoother -= 0.5;
		smoother *= 2.0;
		smoother *= fabs(lastSampleL);
		lastSampleL = inputSampleL;
		
		silhouette += smoother;
		
		bridgerectifier = fabs(silhouette)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (silhouette > 0.0) silhouette = bridgerectifier;
		else silhouette = -bridgerectifier;
		
		inputSampleL = (silhouette + outSampleL) / 2.0;
		outSampleL = silhouette;
		//end L channel
		
		//begin R channel
		bridgerectifier = fabs(inputSampleR)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = 1.0-cos(bridgerectifier);
		if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		
		silhouette = (double(fpdR)/UINT32_MAX);
		silhouette -= 0.5;
		silhouette *= 2.0;
		silhouette *= fabs(inputSampleR);
		
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		smoother = (double(fpdR)/UINT32_MAX);
		smoother -= 0.5;
		smoother *= 2.0;
		smoother *= fabs(lastSampleR);
		lastSampleR = inputSampleR;
		
		silhouette += smoother;
		
		bridgerectifier = fabs(silhouette)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (silhouette > 0.0) silhouette = bridgerectifier;
		else silhouette = -bridgerectifier;
		
		inputSampleR = (silhouette + outSampleR) / 2.0;
		outSampleR = silhouette;
		//end R channel
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
