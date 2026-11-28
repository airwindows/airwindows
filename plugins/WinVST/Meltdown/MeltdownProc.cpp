/* ========================================
 *  Meltdown - Meltdown.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Meltdown_H
#include "Meltdown.h"
#endif

void Meltdown::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
	VstInt32 nPrescanFrames = sampleFrames;
	float* prescanL  =  inputs[0];
    float* prescanR  =  inputs[1];

	double meltdown = 1.0-pow(1.0-A,3.0);
	double highestBufferGainL = 0.0;
	double highestBufferGainR = 0.0;
	int highestGainPosL = 0;
	int highestGainPosR = 0;
	VstInt32 prescanWindow = 0.1618033988749894848204586*sampleFrames;
	if (prevBufferGainL > 0.0) prevBufferGainL = 1.0/prevBufferGainL;
	if (prevBufferGainR > 0.0) prevBufferGainR = 1.0/prevBufferGainR;
	while (nPrescanFrames-- > 0) {
		if (prevBufferGainL < fabs(*prescanL) && (nPrescanFrames < prescanWindow)) {
			prevBufferGainL = fabs(*prescanL);
		}
		if (prevBufferGainR < fabs(*prescanR) && (nPrescanFrames < prescanWindow)) {
			prevBufferGainR = fabs(*prescanR);
		}
		if (highestBufferGainL < fabs(*prescanL)) {
			highestBufferGainL = fabs(*prescanL);
			highestGainPosL = sampleFrames - nPrescanFrames;
		}
		if (highestBufferGainR < fabs(*prescanR)) {
			highestBufferGainR = fabs(*prescanR);
			highestGainPosR = sampleFrames - nPrescanFrames;
		}
		prescanL++;
		prescanR++;
	}
	if (highestBufferGainL < 1.0-meltdown) highestBufferGainL = 1.0-meltdown;
	if (highestBufferGainR < 1.0-meltdown) highestBufferGainR = 1.0-meltdown;
	if (highestBufferGainL > 0.0) highestBufferGainL = 1.0/highestBufferGainL;
	if (highestBufferGainR > 0.0) highestBufferGainR = 1.0/highestBufferGainR;
	if (prevBufferGainL > 0.0) prevBufferGainL = 1.0/prevBufferGainL;
	if (prevBufferGainR > 0.0) prevBufferGainR = 1.0/prevBufferGainR;
	
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		double rampGainL = highestBufferGainL;
		double rampGainR = highestBufferGainR;
		if (abs(inFramesToProcess-sampleFrames) < highestGainPosL) {
			rampGainL = prevBufferGainL+((highestBufferGainL-prevBufferGainL)*(double)((inFramesToProcess-sampleFrames)/highestGainPosL));
		}
		if (abs(inFramesToProcess-sampleFrames) < highestGainPosR) {
			rampGainR = prevBufferGainR+((highestBufferGainR-prevBufferGainR)*(double)((inFramesToProcess-sampleFrames)/highestGainPosR));
		}
		inputSampleL = (fmin(fmax(inputSampleL*rampGainL,-1.0),1.0)*meltdown)+(inputSampleL*(1.0-meltdown));
		inputSampleR = (fmin(fmax(inputSampleR*rampGainR,-1.0),1.0)*meltdown)+(inputSampleR*(1.0-meltdown));
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff))  * 3.553e-44l *  pow(2,expon+62));
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
	prevBufferGainL = highestBufferGainL;
	prevBufferGainR = highestBufferGainR;
}

void Meltdown::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
	VstInt32 nPrescanFrames = sampleFrames;
	double* prescanL  =  inputs[0];
    double* prescanR  =  inputs[1];
	
	double meltdown = 1.0-pow(1.0-A,3.0);
	double highestBufferGainL = 0.0;
	double highestBufferGainR = 0.0;
	int highestGainPosL = 0;
	int highestGainPosR = 0;
	VstInt32 prescanWindow = 0.1618033988749894848204586*sampleFrames;
	if (prevBufferGainL > 0.0) prevBufferGainL = 1.0/prevBufferGainL;
	if (prevBufferGainR > 0.0) prevBufferGainR = 1.0/prevBufferGainR;
	while (nPrescanFrames-- > 0) {
		if (prevBufferGainL < fabs(*prescanL) && (nPrescanFrames < prescanWindow)) {
			prevBufferGainL = fabs(*prescanL);
		}
		if (prevBufferGainR < fabs(*prescanR) && (nPrescanFrames < prescanWindow)) {
			prevBufferGainR = fabs(*prescanR);
		}
		if (highestBufferGainL < fabs(*prescanL)) {
			highestBufferGainL = fabs(*prescanL);
			highestGainPosL = sampleFrames - nPrescanFrames;
		}
		if (highestBufferGainR < fabs(*prescanR)) {
			highestBufferGainR = fabs(*prescanR);
			highestGainPosR = sampleFrames - nPrescanFrames;
		}
		prescanL++;
		prescanR++;
	}
	if (highestBufferGainL < 1.0-meltdown) highestBufferGainL = 1.0-meltdown;
	if (highestBufferGainR < 1.0-meltdown) highestBufferGainR = 1.0-meltdown;
	if (highestBufferGainL > 0.0) highestBufferGainL = 1.0/highestBufferGainL;
	if (highestBufferGainR > 0.0) highestBufferGainR = 1.0/highestBufferGainR;
	if (prevBufferGainL > 0.0) prevBufferGainL = 1.0/prevBufferGainL;
	if (prevBufferGainR > 0.0) prevBufferGainR = 1.0/prevBufferGainR;
	
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double rampGainL = highestBufferGainL;
		double rampGainR = highestBufferGainR;
		if (abs(inFramesToProcess-sampleFrames) < highestGainPosL) {
			rampGainL = prevBufferGainL+((highestBufferGainL-prevBufferGainL)*(double)((inFramesToProcess-sampleFrames)/highestGainPosL));
		}
		if (abs(inFramesToProcess-sampleFrames) < highestGainPosR) {
			rampGainR = prevBufferGainR+((highestBufferGainR-prevBufferGainR)*(double)((inFramesToProcess-sampleFrames)/highestGainPosR));
		}
		inputSampleL = (fmin(fmax(inputSampleL*rampGainL,-1.0),1.0)*meltdown)+(inputSampleL*(1.0-meltdown));
		inputSampleR = (fmin(fmax(inputSampleR*rampGainR,-1.0),1.0)*meltdown)+(inputSampleR*(1.0-meltdown));
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l  * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
	prevBufferGainL = highestBufferGainL;
	prevBufferGainR = highestBufferGainR;
}
