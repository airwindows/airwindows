/* ========================================
 *  AutoPan - AutoPan.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __AutoPan_H
#include "AutoPan.h"
#endif

void AutoPan::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double drift = (pow(A,4)*0.01)/overallscale;
	double offset = 3.141592653589793 + (B*6.283185307179586);
	double panlaw = 1.0-pow(C,2);
	double wet = D;

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		rate += (oldfpd*drift);
		if (rate > 6.283185307179586) {
			rate = 0.0;
			oldfpd = 0.4294967295+(fpdL*0.0000000000618);
		} else if (rate < 0.0) {
			rate = 6.283185307179586;
			oldfpd = 0.4294967295+(fpdL*0.0000000000618);
		}
		
		inputSampleL *= (sin(rate)+1.0);
		inputSampleR *= (sin(rate+offset)+1.0);
		
		double mid = (inputSampleL + inputSampleR)*panlaw;
		double side = inputSampleL - inputSampleR;
		//assign mid and side.Between these sections, you can do mid/side processing
		
		inputSampleL = (mid+side)/4.0;
		inputSampleR = (mid-side)/4.0;
		//unassign mid and side, and compensate for the sin()+1 volume boost
		
		if (wet != 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
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

void AutoPan::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double drift = (pow(A,4)*0.01)/overallscale;
	double offset = 3.141592653589793 + (B*6.283185307179586);
	double panlaw = 1.0-pow(C,2);
	double wet = D;

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		rate += (oldfpd*drift);
		if (rate > 6.283185307179586) {
			rate = 0.0;
			oldfpd = 0.4294967295+(fpdL*0.0000000000618);
		} else if (rate < 0.0) {
			rate = 6.283185307179586;
			oldfpd = 0.4294967295+(fpdL*0.0000000000618);
		}
		
		inputSampleL *= (sin(rate)+1.0);
		inputSampleR *= (sin(rate+offset)+1.0);
		
		double mid = (inputSampleL + inputSampleR)*panlaw;
		double side = inputSampleL - inputSampleR;
		//assign mid and side.Between these sections, you can do mid/side processing
		
		inputSampleL = (mid+side)/4.0;
		inputSampleR = (mid-side)/4.0;
		//unassign mid and side, and compensate for the sin()+1 volume boost
		
		if (wet != 1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		
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
