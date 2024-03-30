/* ========================================
 *  ContentHideD - ContentHideD.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ContentHideD_H
#include "ContentHideD.h"
#endif

void ContentHideD::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		
		double duck = 0.0;
		demotimer -= (1.0/overallscale);
		
		if (demotimer < 441.0) duck = 1.0 - (demotimer/441.0);
		if (demotimer > 44100.0) duck = ((demotimer-44100.0)/441.0);
		if (duck > 1.0) duck = 1.0; duck = sin((1.0-duck)*1.57);
		
		if ((demotimer < 1) || (demotimer > 441000.0)) {
			double randy = (double(fpd)/(double)UINT32_MAX); //0 to 1 the noise, may not be needed		
			demotimer = 100000.0*(randy+2.0);
		}
		
		inputSampleL *= duck;
		inputSampleR *= duck;
		
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void ContentHideD::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		
		double duck = 0.0;
		demotimer -= (1.0/overallscale);
		
		if (demotimer < 441.0) duck = 1.0 - (demotimer/441.0);
		if (demotimer > 44100.0) duck = ((demotimer-44100.0)/441.0);
		if (duck > 1.0) duck = 1.0; duck = sin((1.0-duck)*1.57);
		
		if ((demotimer < 1) || (demotimer > 441000.0)) {
			double randy = (double(fpd)/(double)UINT32_MAX); //0 to 1 the noise, may not be needed		
			demotimer = 100000.0*(randy+2.0);
		}
		
		inputSampleL *= duck;
		inputSampleR *= duck;
		
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
