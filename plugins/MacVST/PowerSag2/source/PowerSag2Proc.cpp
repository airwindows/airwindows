/* ========================================
 *  PowerSag2 - PowerSag2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __PowerSag2_H
#include "PowerSag2.h"
#endif

void PowerSag2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double depth = pow(A,4);
	int offset = (int)(depth * 16383) + 1;	
	double wet = (B*2.0)-1.0;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (gcount < 0 || gcount > 16384) {gcount = 16384;}		
		dL[gcount] = fabs(inputSampleL);
		dR[gcount] = fabs(inputSampleR);
		controlL += dL[gcount];
		controlR += dR[gcount];
		controlL -= dL[gcount+offset-((gcount+offset > 16384)?16384:0)];
		controlR -= dR[gcount+offset-((gcount+offset > 16384)?16384:0)];
		gcount--;
		
		if (controlL > offset) controlL = offset; if (controlL < 0.0) controlL = 0.0;				
		if (controlR > offset) controlR = offset; if (controlR < 0.0) controlR = 0.0;				
		
		double burst = inputSampleL * (controlL / sqrt(offset));
		double clamp = inputSampleL / ((burst == 0.0)?1.0:burst);
		
		if (clamp > 1.0) clamp = 1.0; if (clamp < 0.0) clamp = 0.0;
		inputSampleL *= clamp;
		double difference = drySampleL - inputSampleL;
		if (wet < 0.0) drySampleL *= (wet+1.0);
		inputSampleL = drySampleL - (difference * wet);
		
		burst = inputSampleR * (controlR / sqrt(offset));
		clamp = inputSampleR / ((burst == 0.0)?1.0:burst);
		
		if (clamp > 1.0) clamp = 1.0; if (clamp < 0.0) clamp = 0.0;
		inputSampleR *= clamp;
		difference = drySampleR - inputSampleR;
		if (wet < 0.0) drySampleR *= (wet+1.0);
		inputSampleR = drySampleR - (difference * wet);
		
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

void PowerSag2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double depth = pow(A,4);
	int offset = (int)(depth * 16383) + 1;	
	double wet = (B*2.0)-1.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (gcount < 0 || gcount > 16384) {gcount = 16384;}		
		dL[gcount] = fabs(inputSampleL);
		dR[gcount] = fabs(inputSampleR);
		controlL += dL[gcount];
		controlR += dR[gcount];
		controlL -= dL[gcount+offset-((gcount+offset > 16384)?16384:0)];
		controlR -= dR[gcount+offset-((gcount+offset > 16384)?16384:0)];
		gcount--;
		
		if (controlL > offset) controlL = offset; if (controlL < 0.0) controlL = 0.0;				
		if (controlR > offset) controlR = offset; if (controlR < 0.0) controlR = 0.0;				
		
		double burst = inputSampleL * (controlL / sqrt(offset));
		double clamp = inputSampleL / ((burst == 0.0)?1.0:burst);
		
		if (clamp > 1.0) clamp = 1.0; if (clamp < 0.0) clamp = 0.0;
		inputSampleL *= clamp;
		double difference = drySampleL - inputSampleL;
		if (wet < 0.0) drySampleL *= (wet+1.0);
		inputSampleL = drySampleL - (difference * wet);
		
		burst = inputSampleR * (controlR / sqrt(offset));
		clamp = inputSampleR / ((burst == 0.0)?1.0:burst);
		
		if (clamp > 1.0) clamp = 1.0; if (clamp < 0.0) clamp = 0.0;
		inputSampleR *= clamp;
		difference = drySampleR - inputSampleR;
		if (wet < 0.0) drySampleR *= (wet+1.0);
		inputSampleR = drySampleR - (difference * wet);
		
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
