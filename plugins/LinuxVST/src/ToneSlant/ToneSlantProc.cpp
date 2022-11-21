/* ========================================
 *  ToneSlant - ToneSlant.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ToneSlant_H
#include "ToneSlant.h"
#endif

void ToneSlant::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];


	double inputSampleL;
	double inputSampleR;
	double correctionSampleL;
	double correctionSampleR;
	double accumulatorSampleL;
	double accumulatorSampleR;
	double drySampleL;
	double drySampleR;
	double overallscale = (A*99.0)+1.0;
	double applySlant = (B*2.0)-1.0;
	
	
	f[0] = 1.0 / overallscale;
	//count to f(gain) which will be 0. f(0) is x1
	for (int count = 1; count < 102; count++) {
		if (count <= overallscale) {
			f[count] = (1.0 - (count / overallscale)) / overallscale;
			//recalc the filter and don't change the buffer it'll apply to
		} else {
			bL[count] = 0.0; //blank the unused buffer so when we return to it, no pops
			bR[count] = 0.0; //blank the unused buffer so when we return to it, no pops
		}
	}
    
    while (--sampleFrames >= 0)
    {
		for (int count = overallscale; count >= 0; count--) {
			bL[count+1] = bL[count];
			bR[count+1] = bR[count];
		}
		
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		bL[0] = accumulatorSampleL = drySampleL = inputSampleL;
		bR[0] = accumulatorSampleR = drySampleR = inputSampleR;
		
		accumulatorSampleL *= f[0];
		accumulatorSampleR *= f[0];

		for (int count = 1; count < overallscale; count++) {
			accumulatorSampleL += (bL[count] * f[count]);
			accumulatorSampleR += (bR[count] * f[count]);
		}
		
		correctionSampleL = inputSampleL - (accumulatorSampleL*2.0);
		correctionSampleR = inputSampleR - (accumulatorSampleR*2.0);
		//we're gonna apply the total effect of all these calculations as a single subtract
		
		inputSampleL += (correctionSampleL * applySlant);
		inputSampleR += (correctionSampleR * applySlant);
		//our one math operation on the input data coming in

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

void ToneSlant::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];


	double inputSampleL;
	double inputSampleR;
	double correctionSampleL;
	double correctionSampleR;
	double accumulatorSampleL;
	double accumulatorSampleR;
	double drySampleL;
	double drySampleR;
	double overallscale = (A*99.0)+1.0;
	double applySlant = (B*2.0)-1.0;
	
	f[0] = 1.0 / overallscale;
	//count to f(gain) which will be 0. f(0) is x1
	for (int count = 1; count < 102; count++) {
		if (count <= overallscale) {
			f[count] = (1.0 - (count / overallscale)) / overallscale;
			//recalc the filter and don't change the buffer it'll apply to
		} else {
			bL[count] = 0.0; //blank the unused buffer so when we return to it, no pops
			bR[count] = 0.0; //blank the unused buffer so when we return to it, no pops
		}
	}
	
    while (--sampleFrames >= 0)
    {
		for (int count = overallscale; count >= 0; count--) {
			bL[count+1] = bL[count];
			bR[count+1] = bR[count];
		}
		
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		bL[0] = accumulatorSampleL = drySampleL = inputSampleL;
		bR[0] = accumulatorSampleR = drySampleR = inputSampleR;
		
		accumulatorSampleL *= f[0];
		accumulatorSampleR *= f[0];
		
		for (int count = 1; count < overallscale; count++) {
			accumulatorSampleL += (bL[count] * f[count]);
			accumulatorSampleR += (bR[count] * f[count]);
		}
		
		correctionSampleL = inputSampleL - (accumulatorSampleL*2.0);
		correctionSampleR = inputSampleR - (accumulatorSampleR*2.0);
		//we're gonna apply the total effect of all these calculations as a single subtract
		
		inputSampleL += (correctionSampleL * applySlant);
		inputSampleR += (correctionSampleR * applySlant);
		//our one math operation on the input data coming in
		
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