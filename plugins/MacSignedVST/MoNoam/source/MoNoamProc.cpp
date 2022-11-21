/* ========================================
 *  MoNoam - MoNoam.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __MoNoam_H
#include "MoNoam.h"
#endif

void MoNoam::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	int processing = (VstInt32)( A * 7.999 );
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double mid; mid = inputSampleL + inputSampleR;
		double side; side = inputSampleL - inputSampleR;
		
		if (processing == kMONO || processing == kMONOR || processing == kMONOL) side = 0.0;
		if (processing == kSIDE || processing == kSIDEM || processing == kSIDER || processing == kSIDEL) mid = 0.0;
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		
		if (processing == kSIDEM || processing == kSIDER || processing == kSIDEL) inputSampleL = -inputSampleL;
		
		if (processing == kMONOR || processing == kSIDER) inputSampleL = 0.0; 
		if (processing == kMONOL || processing == kSIDEL) inputSampleR = 0.0; 		
		
		if (processing == kBYPASS) {inputSampleL = *in1; inputSampleR = *in2;}

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

void MoNoam::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	int processing = (VstInt32)( A * 7.999 );

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
				
		double mid; mid = inputSampleL + inputSampleR;
		double side; side = inputSampleL - inputSampleR;
		
		if (processing == kMONO || processing == kMONOR || processing == kMONOL) side = 0.0;
		if (processing == kSIDE || processing == kSIDEM || processing == kSIDER || processing == kSIDEL) mid = 0.0;
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		
		if (processing == kSIDEM || processing == kSIDER || processing == kSIDEL) inputSampleL = -inputSampleL;
		
		if (processing == kMONOR || processing == kSIDER) inputSampleL = 0.0; 
		if (processing == kMONOL || processing == kSIDEL) inputSampleR = 0.0; 		
		
		if (processing == kBYPASS) {inputSampleL = *in1; inputSampleR = *in2;}
		
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
