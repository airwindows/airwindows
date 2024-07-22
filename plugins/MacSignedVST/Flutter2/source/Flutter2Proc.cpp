/* ========================================
 *  Flutter2 - Flutter2.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Flutter2_H
#include "Flutter2.h"
#endif

void Flutter2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double flutDepth = pow(A,4)*overallscale*90;
	if (flutDepth > 498.0) flutDepth = 498.0;
	double flutFrequency = (0.02*pow(B,3))/overallscale;
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Flutter
		if (gcount < 0 || gcount > 999) gcount = 999;
		
		dL[gcount] = inputSampleL;
		int count = gcount;
		double offset = flutDepth + (flutDepth * sin(sweepL));
		sweepL += nextmaxL * flutFrequency;
		if (sweepL > (M_PI*2.0)) {sweepL -= M_PI*2.0; nextmaxL = 0.24 + (fpdL / (double)UINT32_MAX * 0.74);}
		count += (int)floor(offset);
		inputSampleL = (dL[count-((count > 999)?1000:0)] * (1-(offset-floor(offset))));
		inputSampleL += (dL[count+1-((count+1 > 999)?1000:0)] * (offset-floor(offset)));
		
		dR[gcount] = inputSampleR;
		count = gcount;
		offset = flutDepth + (flutDepth * sin(sweepR));
		sweepR += nextmaxR * flutFrequency;
		if (sweepR > (M_PI*2.0)) {sweepR -= M_PI*2.0; nextmaxR = 0.24 + (fpdR / (double)UINT32_MAX * 0.74);}
		count += (int)floor(offset);
		inputSampleR = (dR[count-((count > 999)?1000:0)] * (1-(offset-floor(offset))));
		inputSampleR += (dR[count+1-((count+1 > 999)?1000:0)] * (offset-floor(offset)));
		
		gcount--;
		//end Flutter
		
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

void Flutter2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double flutDepth = pow(A,4)*overallscale*90;
	if (flutDepth > 498.0) flutDepth = 498.0;
	double flutFrequency = (0.02*pow(B,3))/overallscale;
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Flutter
		if (gcount < 0 || gcount > 999) gcount = 999;
		
		dL[gcount] = inputSampleL;
		int count = gcount;
		double offset = flutDepth + (flutDepth * sin(sweepL));
		sweepL += nextmaxL * flutFrequency;
		if (sweepL > (M_PI*2.0)) {sweepL -= M_PI*2.0; nextmaxL = 0.24 + (fpdL / (double)UINT32_MAX * 0.74);}
		count += (int)floor(offset);
		inputSampleL = (dL[count-((count > 999)?1000:0)] * (1-(offset-floor(offset))));
		inputSampleL += (dL[count+1-((count+1 > 999)?1000:0)] * (offset-floor(offset)));
		
		dR[gcount] = inputSampleR;
		count = gcount;
		offset = flutDepth + (flutDepth * sin(sweepR));
		sweepR += nextmaxR * flutFrequency;
		if (sweepR > (M_PI*2.0)) {sweepR -= M_PI*2.0; nextmaxR = 0.24 + (fpdR / (double)UINT32_MAX * 0.74);}
		count += (int)floor(offset);
		inputSampleR = (dR[count-((count > 999)?1000:0)] * (1-(offset-floor(offset))));
		inputSampleR += (dR[count+1-((count+1 > 999)?1000:0)] * (offset-floor(offset)));
		
		gcount--;
		//end Flutter
		
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
