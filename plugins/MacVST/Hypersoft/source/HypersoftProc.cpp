/* ========================================
 *  Hypersoft - Hypersoft.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Hypersoft_H
#include "Hypersoft.h"
#endif

void Hypersoft::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double inputGain = A*2.0;
	if (inputGain > 1.0) inputGain *= inputGain; else inputGain = 1.0-pow(1.0-inputGain,2);
	//this is the fader curve from ConsoleX with 0.5 being unity gain
	int stages = (int)(B*12.0)+2;
	//each stage brings in an additional layer of harmonics on the waveshaping
	double bright = (1.0-C)*0.15;
	//higher slews suppress these higher harmonics when they are sure to just alias
	double outputGain = D*2.0;
	if (outputGain > 1.0) outputGain *= outputGain; else outputGain = 1.0-pow(1.0-outputGain,2);
	outputGain *= 0.68;
	//this is the fader curve from ConsoleX, rescaled to work with Hypersoft
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= inputGain;
		inputSampleR *= inputGain;
		
		inputSampleL = sin(inputSampleL); inputSampleL += (sin(inputSampleL*2.0)/2.0);
		inputSampleR = sin(inputSampleR); inputSampleR += (sin(inputSampleR*2.0)/2.0);
		for (int count = 2; count<stages; count++){
			inputSampleL += ((sin(inputSampleL*(double)count)/(double)pow(count,3))*fmax(0.0,1.0-fabs((inputSampleL-lastSampleL)*bright*(double)(count*count))));
			inputSampleR += ((sin(inputSampleR*(double)count)/(double)pow(count,3))*fmax(0.0,1.0-fabs((inputSampleR-lastSampleR)*bright*(double)(count*count))));
		}
		lastSampleL = inputSampleL;
		lastSampleR = inputSampleR;
		
		inputSampleL *= outputGain;
		inputSampleR *= outputGain;
		
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

void Hypersoft::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double inputGain = A*2.0;
	if (inputGain > 1.0) inputGain *= inputGain; else inputGain = 1.0-pow(1.0-inputGain,2);
	//this is the fader curve from ConsoleX with 0.5 being unity gain
	int stages = (int)(B*12.0)+2;
	//each stage brings in an additional layer of harmonics on the waveshaping
	double bright = (1.0-C)*0.15;
	//higher slews suppress these higher harmonics when they are sure to just alias
	double outputGain = D*2.0;
	if (outputGain > 1.0) outputGain *= outputGain; else outputGain = 1.0-pow(1.0-outputGain,2);
	outputGain *= 0.68;
	//this is the fader curve from ConsoleX, rescaled to work with Hypersoft
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= inputGain;
		inputSampleR *= inputGain;
		
		inputSampleL = sin(inputSampleL); inputSampleL += (sin(inputSampleL*2.0)/2.0);
		inputSampleR = sin(inputSampleR); inputSampleR += (sin(inputSampleR*2.0)/2.0);
		for (int count = 2; count<stages; count++){
			inputSampleL += ((sin(inputSampleL*(double)count)/(double)pow(count,3))*fmax(0.0,1.0-fabs((inputSampleL-lastSampleL)*bright*(double)(count*count))));
			inputSampleR += ((sin(inputSampleR*(double)count)/(double)pow(count,3))*fmax(0.0,1.0-fabs((inputSampleR-lastSampleR)*bright*(double)(count*count))));
		}
		lastSampleL = inputSampleL;
		lastSampleR = inputSampleR;
		
		inputSampleL *= outputGain;
		inputSampleR *= outputGain;
		
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
