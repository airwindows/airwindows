/* ========================================
 *  Weight - Weight.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Weight_H
#include "Weight.h"
#endif

void Weight::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double targetFreq = A;
	//gives us a 0-1 value like the VST will be. For the VST, start with 0-1 and
	//have the plugin display the number as 20-120.
	
	targetFreq = ((targetFreq+0.53)*0.2)/sqrt(overallscale);
	//must use square root of what the real scale would be, to get correct output
	double alpha = pow(targetFreq,4);
	double out = B;
	double resControl = (out*0.05)+0.2;
	double beta = (alpha * pow(resControl,2));	
	alpha += ((1.0-beta)*pow(targetFreq,3)); //correct for droop in frequency
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Weight
		double trend;
		double forecast;
		for (int i = 0; i < 8; i++) {
			trend = (beta * (inputSampleL - previousSampleL[i]) + ((0.999-beta) * previousTrendL[i]));
			forecast = previousSampleL[i] + previousTrendL[i];
			inputSampleL = (alpha * inputSampleL) + ((0.999-alpha) * forecast);
			previousSampleL[i] = inputSampleL; previousTrendL[i] = trend;
			
			trend = (beta * (inputSampleR - previousSampleR[i]) + ((0.999-beta) * previousTrendR[i]));
			forecast = previousSampleR[i] + previousTrendR[i];
			inputSampleR = (alpha * inputSampleR) + ((0.999-alpha) * forecast);
			previousSampleR[i] = inputSampleR; previousTrendR[i] = trend;
		}
		//inputSample is now the bass boost to be added
		
		inputSampleL *= out;
		inputSampleR *= out;
		inputSampleL += drySampleL;
		inputSampleR += drySampleR;
		
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

void Weight::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double targetFreq = A;
	//gives us a 0-1 value like the VST will be. For the VST, start with 0-1 and
	//have the plugin display the number as 20-120.
	
	targetFreq = ((targetFreq+0.53)*0.2)/sqrt(overallscale);
	//must use square root of what the real scale would be, to get correct output
	double alpha = pow(targetFreq,4);
	double out = B;
	double resControl = (out*0.05)+0.2;
	double beta = (alpha * pow(resControl,2));	
	alpha += ((1.0-beta)*pow(targetFreq,3)); //correct for droop in frequency
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Weight
		double trend;
		double forecast;
		for (int i = 0; i < 8; i++) {
			trend = (beta * (inputSampleL - previousSampleL[i]) + ((0.999-beta) * previousTrendL[i]));
			forecast = previousSampleL[i] + previousTrendL[i];
			inputSampleL = (alpha * inputSampleL) + ((0.999-alpha) * forecast);
			previousSampleL[i] = inputSampleL; previousTrendL[i] = trend;
			
			trend = (beta * (inputSampleR - previousSampleR[i]) + ((0.999-beta) * previousTrendR[i]));
			forecast = previousSampleR[i] + previousTrendR[i];
			inputSampleR = (alpha * inputSampleR) + ((0.999-alpha) * forecast);
			previousSampleR[i] = inputSampleR; previousTrendR[i] = trend;
		}
		//inputSample is now the bass boost to be added
		
		inputSampleL *= out;
		inputSampleR *= out;
		inputSampleL += drySampleL;
		inputSampleR += drySampleR;
		
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
