/* ========================================
 *  DigitalBlack - DigitalBlack.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DigitalBlack_H
#include "DigitalBlack.h"
#endif

void DigitalBlack::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double threshold = (pow(A,4)/3)+0.00018;
	double release = 0.0064 / overallscale;
	double wet = B;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		

		if (inputSampleL > 0) {
			if (WasNegativeL == true) ZeroCrossL = 0;
			WasNegativeL = false;
		} else {
			ZeroCrossL += 1;
			WasNegativeL = true;
		}
		if (inputSampleR > 0) {
			if (WasNegativeR == true) ZeroCrossR = 0;
			WasNegativeR = false;
		} else {
			ZeroCrossR += 1;
			WasNegativeR = true;
		}
		
		if (ZeroCrossL > 6000) ZeroCrossL = 6000;
		if (ZeroCrossR > 6000) ZeroCrossR = 6000;
		
		if (fabs(inputSampleL) > threshold) {
			if (gaterollerL < ZeroCrossL) gaterollerL = ZeroCrossL;
		} else {
			gaterollerL -= release;
		}
		if (fabs(inputSampleR) > threshold) {
			if (gaterollerR < ZeroCrossR) gaterollerR = ZeroCrossR;
		} else {
			gaterollerR -= release;
		}
		
		if (gaterollerL < 0.0) gaterollerL = 0.0;
		if (gaterollerR < 0.0) gaterollerR = 0.0;
		
		double gate = 1.0;
		if (gaterollerL < 1.0) gate = gaterollerL;
		
		double bridgerectifier = 1-cos(fabs(inputSampleL));
		
		if (inputSampleL > 0) inputSampleL = (inputSampleL*gate)+(bridgerectifier*(1-gate));
		else inputSampleL = (inputSampleL*gate)-(bridgerectifier*(1-gate));
		
		if ((gate == 0.0) && (fabs(inputSampleL*3) < threshold)) inputSampleL = 0.0;
		
		gate = 1.0;
		if (gaterollerR < 1.0) gate = gaterollerR;
		
		bridgerectifier = 1-cos(fabs(inputSampleR));
		
		if (inputSampleR > 0) inputSampleR = (inputSampleR*gate)+(bridgerectifier*(1-gate));
		else inputSampleR = (inputSampleR*gate)-(bridgerectifier*(1-gate));
		
		if ((gate == 0.0) && (fabs(inputSampleR*3) < threshold)) inputSampleR = 0.0;
				
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void DigitalBlack::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double threshold = (pow(A,4)/3)+0.00018;
	double release = 0.0064 / overallscale;
	double wet = B;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		
		if (inputSampleL > 0) {
			if (WasNegativeL == true) ZeroCrossL = 0;
			WasNegativeL = false;
		} else {
			ZeroCrossL += 1;
			WasNegativeL = true;
		}
		if (inputSampleR > 0) {
			if (WasNegativeR == true) ZeroCrossR = 0;
			WasNegativeR = false;
		} else {
			ZeroCrossR += 1;
			WasNegativeR = true;
		}
		
		if (ZeroCrossL > 6000) ZeroCrossL = 6000;
		if (ZeroCrossR > 6000) ZeroCrossR = 6000;
		
		if (fabs(inputSampleL) > threshold) {
			if (gaterollerL < ZeroCrossL) gaterollerL = ZeroCrossL;
		} else {
			gaterollerL -= release;
		}
		if (fabs(inputSampleR) > threshold) {
			if (gaterollerR < ZeroCrossR) gaterollerR = ZeroCrossR;
		} else {
			gaterollerR -= release;
		}
		
		if (gaterollerL < 0.0) gaterollerL = 0.0;
		if (gaterollerR < 0.0) gaterollerR = 0.0;
		
		double gate = 1.0;
		if (gaterollerL < 1.0) gate = gaterollerL;
		
		double bridgerectifier = 1-cos(fabs(inputSampleL));
		
		if (inputSampleL > 0) inputSampleL = (inputSampleL*gate)+(bridgerectifier*(1-gate));
		else inputSampleL = (inputSampleL*gate)-(bridgerectifier*(1-gate));
		
		if ((gate == 0.0) && (fabs(inputSampleL*3) < threshold)) inputSampleL = 0.0;
		
		gate = 1.0;
		if (gaterollerR < 1.0) gate = gaterollerR;
		
		bridgerectifier = 1-cos(fabs(inputSampleR));
		
		if (inputSampleR > 0) inputSampleR = (inputSampleR*gate)+(bridgerectifier*(1-gate));
		else inputSampleR = (inputSampleR*gate)-(bridgerectifier*(1-gate));
		
		if ((gate == 0.0) && (fabs(inputSampleR*3) < threshold)) inputSampleR = 0.0;
		
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
