/* ========================================
 *  SoftGate - SoftGate.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SoftGate_H
#include "SoftGate.h"
#endif

void SoftGate::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double threshold = pow(A,6);
	double recovery = pow((B*0.5),6);
	recovery /= overallscale;
	double baseline = pow(C,6);
	double invrec = 1.0 - recovery;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		storedL[1] = storedL[0];
		storedL[0] = inputSampleL;
		diffL = storedL[0] - storedL[1];
		
		storedR[1] = storedR[0];
		storedR[0] = inputSampleR;
		diffR = storedR[0] - storedR[1];
		
		if (gate > 0) {gate = ((gate-baseline) * invrec) + baseline;}
		
		if ((fabs(diffR) > threshold) || (fabs(diffL) > threshold)) {gate = 1.1;}
		else {gate = (gate * invrec); if (threshold > 0) {gate += ((fabs(inputSampleL)/threshold) * recovery);gate += ((fabs(inputSampleR)/threshold) * recovery);}}
		
		if (gate < 0) gate = 0;
		
		if (gate < 1.0)
		{
			storedL[0] = storedL[1] + (diffL * gate);		
			storedR[0] = storedR[1] + (diffR * gate);
		}
		
		if (gate < 1) {
			inputSampleL = (inputSampleL * gate) + (storedL[0] * (1.0-gate));
			inputSampleR = (inputSampleR * gate) + (storedR[0] * (1.0-gate));
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

void SoftGate::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double threshold = pow(A,6);
	double recovery = pow((B*0.5),6);
	recovery /= overallscale;
	double baseline = pow(C,6);
	double invrec = 1.0 - recovery;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		storedL[1] = storedL[0];
		storedL[0] = inputSampleL;
		diffL = storedL[0] - storedL[1];
		
		storedR[1] = storedR[0];
		storedR[0] = inputSampleR;
		diffR = storedR[0] - storedR[1];
		
		if (gate > 0) {gate = ((gate-baseline) * invrec) + baseline;}
		
		if ((fabs(diffR) > threshold) || (fabs(diffL) > threshold)) {gate = 1.1;}
		else {gate = (gate * invrec); if (threshold > 0) {gate += ((fabs(inputSampleL)/threshold) * recovery);gate += ((fabs(inputSampleR)/threshold) * recovery);}}
		
		if (gate < 0) gate = 0;
		
		if (gate < 1.0)
		{
			storedL[0] = storedL[1] + (diffL * gate);		
			storedR[0] = storedR[1] + (diffR * gate);
		}
		
		if (gate < 1) {
			inputSampleL = (inputSampleL * gate) + (storedL[0] * (1.0-gate));
			inputSampleR = (inputSampleR * gate) + (storedR[0] * (1.0-gate));
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
