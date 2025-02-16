/* ========================================
 *  SquareRoot - SquareRoot.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SquareRoot_H
#include "SquareRoot.h"
#endif

void SquareRoot::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double in = A*2.0; in *= in;
	double soar = 0.3-(B*0.3);
	double out = C*2.0; out *= out;
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (in !=1.0) {
			inputSampleL *= in;
			inputSampleR *= in;
		}
		
		double temp = fabs(inputSampleL)+(soar*soar);
		if (inputSampleL > 0.0) inputSampleL = fmax((sqrt(1.0/temp)*temp)-soar,0.0);
		if (inputSampleL < 0.0) inputSampleL = fmin((-sqrt(1.0/temp)*temp)+soar,0.0);
		
		temp = fabs(inputSampleR)+(soar*soar);
		if (inputSampleR > 0.0) inputSampleR = fmax((sqrt(1.0/temp)*temp)-soar,0.0);
		if (inputSampleR < 0.0) inputSampleR = fmin((-sqrt(1.0/temp)*temp)+soar,0.0);
		
		if (out !=1.0) {
			inputSampleL *= out;
			inputSampleR *= out;
		}
		
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

void SquareRoot::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double in = A*2.0; in *= in;
	double soar = 0.3-(B*0.3);
	double out = C*2.0; out *= out;
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (in !=1.0) {
			inputSampleL *= in;
			inputSampleR *= in;
		}
		
		double temp = fabs(inputSampleL)+(soar*soar);
		if (inputSampleL > 0.0) inputSampleL = fmax((sqrt(1.0/temp)*temp)-soar,0.0);
		if (inputSampleL < 0.0) inputSampleL = fmin((-sqrt(1.0/temp)*temp)+soar,0.0);
		
		temp = fabs(inputSampleR)+(soar*soar);
		if (inputSampleR > 0.0) inputSampleR = fmax((sqrt(1.0/temp)*temp)-soar,0.0);
		if (inputSampleR < 0.0) inputSampleR = fmin((-sqrt(1.0/temp)*temp)+soar,0.0);
		
		if (out !=1.0) {
			inputSampleL *= out;
			inputSampleR *= out;
		}
		
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
