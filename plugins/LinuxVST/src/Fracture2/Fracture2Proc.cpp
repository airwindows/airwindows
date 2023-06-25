/* ========================================
 *  Fracture2 - Fracture2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Fracture2_H
#include "Fracture2.h"
#endif

void Fracture2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	densityA = densityB;
	densityB = A*10.0; //0.0 to 10.0
	int stages = B*8.0;
	thresholdA = thresholdB;
	thresholdB = C;
	outputA = outputB;
	outputB = D;
	wetA = wetB;
	wetB = E;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double density = (densityA*temp)+(densityB*(1.0-temp));
		double threshold = (thresholdA*temp)+(thresholdB*(1.0-temp));
		double output = (outputA*temp)+(outputB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
		
		inputSampleL *= density;
		inputSampleR *= density;
		
		for (int x = 0; x < stages; x++) {
			inputSampleL *= (fabs(inputSampleL)+1.0);
			inputSampleR *= (fabs(inputSampleR)+1.0);
		}
		
		if (inputSampleL > M_PI_2) {
			inputSampleL = (sin(inputSampleL)*threshold)+(1.0*(1.0-threshold));
		} else if (inputSampleL < -M_PI_2) {
			inputSampleL = (sin(inputSampleL)*threshold)+(-1.0*(1.0-threshold));
		} else {
			inputSampleL = sin(inputSampleL);
		}
		if (inputSampleR > M_PI_2) {
			inputSampleR = (sin(inputSampleR)*threshold)+(1.0*(1.0-threshold));
		} else if (inputSampleR < -M_PI_2) {
			inputSampleR = (sin(inputSampleR)*threshold)+(-1.0*(1.0-threshold));
		} else {
			inputSampleR = sin(inputSampleR);
		}
		
		inputSampleL *= output;
		inputSampleR *= output;
		
		if (wet !=1.0) {
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void Fracture2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	densityA = densityB;
	densityB = A*10.0; //0.0 to 10.0
	int stages = B*8.0;
	thresholdA = thresholdB;
	thresholdB = C;
	outputA = outputB;
	outputB = D;
	wetA = wetB;
	wetB = E;
		
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double density = (densityA*temp)+(densityB*(1.0-temp));
		double threshold = (thresholdA*temp)+(thresholdB*(1.0-temp));
		double output = (outputA*temp)+(outputB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
		
		inputSampleL *= density;
		inputSampleR *= density;
		
		for (int x = 0; x < stages; x++) {
			inputSampleL *= (fabs(inputSampleL)+1.0);
			inputSampleR *= (fabs(inputSampleR)+1.0);
		}
		
		if (inputSampleL > M_PI_2) {
			inputSampleL = (sin(inputSampleL)*threshold)+(1.0*(1.0-threshold));
		} else if (inputSampleL < -M_PI_2) {
			inputSampleL = (sin(inputSampleL)*threshold)+(-1.0*(1.0-threshold));
		} else {
			inputSampleL = sin(inputSampleL);
		}
		if (inputSampleR > M_PI_2) {
			inputSampleR = (sin(inputSampleR)*threshold)+(1.0*(1.0-threshold));
		} else if (inputSampleR < -M_PI_2) {
			inputSampleR = (sin(inputSampleR)*threshold)+(-1.0*(1.0-threshold));
		} else {
			inputSampleR = sin(inputSampleR);
		}
		
		inputSampleL *= output;
		inputSampleR *= output;
		
		if (wet !=1.0) {
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
