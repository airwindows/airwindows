/* ========================================
 *  SampleDelay - SampleDelay.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SampleDelay_H
#include "SampleDelay.h"
#endif

void SampleDelay::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double coarse = A * (4410.0*overallscale);
	double fine = B * 10.0;
	double subsample = C;
	int offset = floor(coarse + fine);
	if (offset > 16380) offset = 16380; //insanity check
	int maxtime = 16384;
	double phase = (D*2.0)-1.0;
	double dryLevel = 1.0-fabs(phase);
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (gcount < 0 || gcount > maxtime) {gcount = maxtime;}
		int count = gcount;
		pL[count] = inputSampleL;
		pR[count] = inputSampleR;
		count += offset;
		inputSampleL = pL[count-((count > maxtime)?maxtime+1:0)]*(1.0 - subsample);
		inputSampleR = pR[count-((count > maxtime)?maxtime+1:0)]*(1.0 - subsample);
		inputSampleL += pL[count+1-((count+1 > maxtime)?maxtime+1:0)]*subsample;
		inputSampleR += pR[count+1-((count+1 > maxtime)?maxtime+1:0)]*subsample;
		gcount--;
		
		if (phase < 1.0) {
			inputSampleL *= phase;
			inputSampleR *= phase;
		}
		
		if (dryLevel > 0.0) {
			inputSampleL += (drySampleL * dryLevel);
			inputSampleR += (drySampleR * dryLevel);
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

void SampleDelay::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double coarse = A * (4410.0*overallscale);
	double fine = B * 10.0;
	double subsample = C;
	int offset = floor(coarse + fine);
	if (offset > 16380) offset = 16380; //insanity check
	int maxtime = 16384;
	double phase = (D*2.0)-1.0;
	double dryLevel = 1.0-fabs(phase);
	
	while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (gcount < 0 || gcount > maxtime) {gcount = maxtime;}
		int count = gcount;
		pL[count] = inputSampleL;
		pR[count] = inputSampleR;
		count += offset;
		inputSampleL = pL[count-((count > maxtime)?maxtime+1:0)]*(1.0 - subsample);
		inputSampleR = pR[count-((count > maxtime)?maxtime+1:0)]*(1.0 - subsample);
		inputSampleL += pL[count+1-((count+1 > maxtime)?maxtime+1:0)]*subsample;
		inputSampleR += pR[count+1-((count+1 > maxtime)?maxtime+1:0)]*subsample;
		gcount--;
		
		if (phase < 1.0) {
			inputSampleL *= phase;
			inputSampleR *= phase;
		}
		
		if (dryLevel > 0.0) {
			inputSampleL += (drySampleL * dryLevel);
			inputSampleR += (drySampleR * dryLevel);
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
