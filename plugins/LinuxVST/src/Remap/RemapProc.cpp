/* ========================================
 *  Remap - Remap.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Remap_H
#include "Remap.h"
#endif

void Remap::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double drive = A * 0.5;
	double gain = (drive+0.2)*8;
	double out = B;
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double gaintrimL = fabs(inputSampleL);
		double gaintrimR = fabs(inputSampleR);

		double bridgerectifierL = gaintrimL*gain;
		double bridgerectifierR = gaintrimR*gain;

		if (gaintrimL > 1.0) gaintrimL = 1.0;
		else gaintrimL *= gaintrimL;
		
		if (gaintrimR > 1.0) gaintrimR = 1.0;
		else gaintrimR *= gaintrimR;
		
		if (inputSampleL > 1.57079633) bridgerectifierL = 1.0-(1.0-sin(bridgerectifierL));
		else bridgerectifierL = sin(bridgerectifierL);
		
		if (inputSampleR > 1.57079633) bridgerectifierR = 1.0-(1.0-sin(bridgerectifierR));
		else bridgerectifierR = sin(bridgerectifierR);
		
		if (inputSampleL > 0) inputSampleL -= (bridgerectifierL*gaintrimL*drive);
		else inputSampleL += (bridgerectifierL*gaintrimL*drive);
		
		if (inputSampleR > 0) inputSampleR -= (bridgerectifierR*gaintrimR*drive);
		else inputSampleR += (bridgerectifierR*gaintrimR*drive);
		
		if (out < 1.0) {
			inputSampleL *= out;
			inputSampleR *= out;
		}
		
		if (wet < 1.0) {
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Remap::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double drive = A * 0.5;
	double gain = (drive+0.2)*8;
	double out = B;
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double gaintrimL = fabs(inputSampleL);
		double gaintrimR = fabs(inputSampleR);
		
		double bridgerectifierL = gaintrimL*gain;
		double bridgerectifierR = gaintrimR*gain;
		
		if (gaintrimL > 1.0) gaintrimL = 1.0;
		else gaintrimL *= gaintrimL;
		
		if (gaintrimR > 1.0) gaintrimR = 1.0;
		else gaintrimR *= gaintrimR;
		
		if (inputSampleL > 1.57079633) bridgerectifierL = 1.0-(1.0-sin(bridgerectifierL));
		else bridgerectifierL = sin(bridgerectifierL);
		
		if (inputSampleR > 1.57079633) bridgerectifierR = 1.0-(1.0-sin(bridgerectifierR));
		else bridgerectifierR = sin(bridgerectifierR);
		
		if (inputSampleL > 0) inputSampleL -= (bridgerectifierL*gaintrimL*drive);
		else inputSampleL += (bridgerectifierL*gaintrimL*drive);
		
		if (inputSampleR > 0) inputSampleR -= (bridgerectifierR*gaintrimR*drive);
		else inputSampleR += (bridgerectifierR*gaintrimR*drive);
		
		if (out < 1.0) {
			inputSampleL *= out;
			inputSampleR *= out;
		}
		
		if (wet < 1.0) {
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
