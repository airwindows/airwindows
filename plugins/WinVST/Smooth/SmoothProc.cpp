/* ========================================
 *  Smooth - Smooth.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Smooth_H
#include "Smooth.h"
#endif

void Smooth::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double clamp;
	double chase = pow(A,2);
	double makeup = (1.0+(chase*1.6)) * B;
	double ratio = chase * 24.0;
	chase /= overallscale;
	chase *= 0.083; // set up the ratio of new val to old
	double wet = C;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//left channel
		clamp = fabs(inputSampleL - lastSampleAL);
		clamp = sin(clamp*ratio);
		lastSampleAL = inputSampleL;
		gainAL *= (1.0 - chase);
		gainAL += ((1.0-clamp) * chase);
		if (gainAL > 1.0) gainAL = 1.0;
		if (gainAL < 0.0) gainAL = 0.0;
		if (gainAL != 1.0) inputSampleL *= gainAL;
		
		clamp = fabs(inputSampleL - lastSampleBL);
		clamp = sin(clamp*ratio);
		lastSampleBL = inputSampleL;
		gainBL *= (1.0 - chase);
		gainBL += ((1.0-clamp) * chase);
		if (gainBL > 1.0) gainBL = 1.0;
		if (gainBL < 0.0) gainBL = 0.0;
		if (gainBL != 1.0) inputSampleL *= gainBL;
		
		clamp = fabs(inputSampleL - lastSampleCL);
		clamp = sin(clamp*ratio);
		lastSampleCL = inputSampleL;
		gainCL *= (1.0 - chase);
		gainCL += ((1.0-clamp) * chase);
		if (gainCL > 1.0) gainCL = 1.0;
		if (gainCL < 0.0) gainCL = 0.0;
		if (gainCL != 1.0) inputSampleL *= gainCL;
		
		clamp = fabs(inputSampleL - lastSampleDL);
		clamp = sin(clamp*ratio);
		lastSampleDL = inputSampleL;
		gainDL *= (1.0 - chase);
		gainDL += ((1.0-clamp) * chase);
		if (gainDL > 1.0) gainDL = 1.0;
		if (gainDL < 0.0) gainDL = 0.0;
		if (gainDL != 1.0) inputSampleL *= gainDL;
		
		clamp = fabs(inputSampleL - lastSampleEL);
		clamp = sin(clamp*ratio);
		lastSampleEL = inputSampleL;
		gainEL *= (1.0 - chase);
		gainEL += ((1.0-clamp) * chase);
		if (gainEL > 1.0) gainEL = 1.0;
		if (gainEL < 0.0) gainEL = 0.0;
		if (gainEL != 1.0) inputSampleL *= gainEL;
		//end left channel
		
		//right channel
		clamp = fabs(inputSampleR - lastSampleAR);
		clamp = sin(clamp*ratio);
		lastSampleAR = inputSampleR;
		gainAR *= (1.0 - chase);
		gainAR += ((1.0-clamp) * chase);
		if (gainAR > 1.0) gainAR = 1.0;
		if (gainAR < 0.0) gainAR = 0.0;
		if (gainAR != 1.0) inputSampleR *= gainAR;
		
		clamp = fabs(inputSampleR - lastSampleBR);
		clamp = sin(clamp*ratio);
		lastSampleBR = inputSampleR;
		gainBR *= (1.0 - chase);
		gainBR += ((1.0-clamp) * chase);
		if (gainBR > 1.0) gainBR = 1.0;
		if (gainBR < 0.0) gainBR = 0.0;
		if (gainBR != 1.0) inputSampleR *= gainBR;
		
		clamp = fabs(inputSampleR - lastSampleCR);
		clamp = sin(clamp*ratio);
		lastSampleCR = inputSampleR;
		gainCR *= (1.0 - chase);
		gainCR += ((1.0-clamp) * chase);
		if (gainCR > 1.0) gainCR = 1.0;
		if (gainCR < 0.0) gainCR = 0.0;
		if (gainCR != 1.0) inputSampleR *= gainCR;
		
		clamp = fabs(inputSampleR - lastSampleDR);
		clamp = sin(clamp*ratio);
		lastSampleDR = inputSampleR;
		gainDR *= (1.0 - chase);
		gainDR += ((1.0-clamp) * chase);
		if (gainDR > 1.0) gainDR = 1.0;
		if (gainDR < 0.0) gainDR = 0.0;
		if (gainDR != 1.0) inputSampleR *= gainDR;
		
		clamp = fabs(inputSampleR - lastSampleER);
		clamp = sin(clamp*ratio);
		lastSampleER = inputSampleR;
		gainER *= (1.0 - chase);
		gainER += ((1.0-clamp) * chase);
		if (gainER > 1.0) gainER = 1.0;
		if (gainER < 0.0) gainER = 0.0;
		if (gainER != 1.0) inputSampleR *= gainER;
		//end right channel
		
		if (makeup !=1.0) {
			inputSampleL *= makeup;
			inputSampleR *= makeup;
		}
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0 - wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0 - wet));
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

void Smooth::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double clamp;
	double chase = pow(A,2);
	double makeup = (1.0+(chase*1.6)) * B;
	double ratio = chase * 24.0;
	chase /= overallscale;
	chase *= 0.083; // set up the ratio of new val to old
	double wet = C;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//left channel
		clamp = fabs(inputSampleL - lastSampleAL);
		clamp = sin(clamp*ratio);
		lastSampleAL = inputSampleL;
		gainAL *= (1.0 - chase);
		gainAL += ((1.0-clamp) * chase);
		if (gainAL > 1.0) gainAL = 1.0;
		if (gainAL < 0.0) gainAL = 0.0;
		if (gainAL != 1.0) inputSampleL *= gainAL;
		
		clamp = fabs(inputSampleL - lastSampleBL);
		clamp = sin(clamp*ratio);
		lastSampleBL = inputSampleL;
		gainBL *= (1.0 - chase);
		gainBL += ((1.0-clamp) * chase);
		if (gainBL > 1.0) gainBL = 1.0;
		if (gainBL < 0.0) gainBL = 0.0;
		if (gainBL != 1.0) inputSampleL *= gainBL;
		
		clamp = fabs(inputSampleL - lastSampleCL);
		clamp = sin(clamp*ratio);
		lastSampleCL = inputSampleL;
		gainCL *= (1.0 - chase);
		gainCL += ((1.0-clamp) * chase);
		if (gainCL > 1.0) gainCL = 1.0;
		if (gainCL < 0.0) gainCL = 0.0;
		if (gainCL != 1.0) inputSampleL *= gainCL;
		
		clamp = fabs(inputSampleL - lastSampleDL);
		clamp = sin(clamp*ratio);
		lastSampleDL = inputSampleL;
		gainDL *= (1.0 - chase);
		gainDL += ((1.0-clamp) * chase);
		if (gainDL > 1.0) gainDL = 1.0;
		if (gainDL < 0.0) gainDL = 0.0;
		if (gainDL != 1.0) inputSampleL *= gainDL;
		
		clamp = fabs(inputSampleL - lastSampleEL);
		clamp = sin(clamp*ratio);
		lastSampleEL = inputSampleL;
		gainEL *= (1.0 - chase);
		gainEL += ((1.0-clamp) * chase);
		if (gainEL > 1.0) gainEL = 1.0;
		if (gainEL < 0.0) gainEL = 0.0;
		if (gainEL != 1.0) inputSampleL *= gainEL;
		//end left channel
		
		//right channel
		clamp = fabs(inputSampleR - lastSampleAR);
		clamp = sin(clamp*ratio);
		lastSampleAR = inputSampleR;
		gainAR *= (1.0 - chase);
		gainAR += ((1.0-clamp) * chase);
		if (gainAR > 1.0) gainAR = 1.0;
		if (gainAR < 0.0) gainAR = 0.0;
		if (gainAR != 1.0) inputSampleR *= gainAR;
		
		clamp = fabs(inputSampleR - lastSampleBR);
		clamp = sin(clamp*ratio);
		lastSampleBR = inputSampleR;
		gainBR *= (1.0 - chase);
		gainBR += ((1.0-clamp) * chase);
		if (gainBR > 1.0) gainBR = 1.0;
		if (gainBR < 0.0) gainBR = 0.0;
		if (gainBR != 1.0) inputSampleR *= gainBR;
		
		clamp = fabs(inputSampleR - lastSampleCR);
		clamp = sin(clamp*ratio);
		lastSampleCR = inputSampleR;
		gainCR *= (1.0 - chase);
		gainCR += ((1.0-clamp) * chase);
		if (gainCR > 1.0) gainCR = 1.0;
		if (gainCR < 0.0) gainCR = 0.0;
		if (gainCR != 1.0) inputSampleR *= gainCR;
		
		clamp = fabs(inputSampleR - lastSampleDR);
		clamp = sin(clamp*ratio);
		lastSampleDR = inputSampleR;
		gainDR *= (1.0 - chase);
		gainDR += ((1.0-clamp) * chase);
		if (gainDR > 1.0) gainDR = 1.0;
		if (gainDR < 0.0) gainDR = 0.0;
		if (gainDR != 1.0) inputSampleR *= gainDR;
		
		clamp = fabs(inputSampleR - lastSampleER);
		clamp = sin(clamp*ratio);
		lastSampleER = inputSampleR;
		gainER *= (1.0 - chase);
		gainER += ((1.0-clamp) * chase);
		if (gainER > 1.0) gainER = 1.0;
		if (gainER < 0.0) gainER = 0.0;
		if (gainER != 1.0) inputSampleR *= gainER;
		//end right channel
		
		if (makeup !=1.0) {
			inputSampleL *= makeup;
			inputSampleR *= makeup;
		}
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0 - wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0 - wet));
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
