/* ========================================
 *  Console8SubOut - Console8SubOut.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console8SubOut_H
#include "Console8SubOut.h"
#endif

void Console8SubOut::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	inTrimA = inTrimB; inTrimB = A*2.0;
	//0.5 is unity gain, and we can attenuate to silence or boost slightly over 12dB
	//into softclipping overdrive.
    if (getSampleRate() > 49000.0) hsr = true; else hsr = false;
    fix[fix_freq] = 24000.0 / getSampleRate();
    fix[fix_reso] = 1.20361562;
    double K = tan(M_PI * fix[fix_freq]); //lowpass
    double norm = 1.0 / (1.0 + K / fix[fix_reso] + K * K);
    fix[fix_a0] = K * K * norm;
    fix[fix_a1] = 2.0 * fix[fix_a0];
    fix[fix_a2] = fix[fix_a0];
    fix[fix_b1] = 2.0 * (K * K - 1.0) * norm;
    fix[fix_b2] = (1.0 - K / fix[fix_reso] + K * K) * norm;
    //this is the fixed biquad distributed anti-aliasing filter

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		
		double position = (double)sampleFrames/inFramesToProcess;
		double inTrim = (inTrimA*position)+(inTrimB*(1.0-position));
		//input trim smoothed to cut out zipper noise
		inputSampleL *= inTrim;
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633; if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		inputSampleR *= inTrim;
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633; if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		if (hsr){
			double outSample = (inputSampleL * fix[fix_a0]) + fix[fix_sL1];
			fix[fix_sL1] = (inputSampleL * fix[fix_a1]) - (outSample * fix[fix_b1]) + fix[fix_sL2];
			fix[fix_sL2] = (inputSampleL * fix[fix_a2]) - (outSample * fix[fix_b2]);
			inputSampleL = outSample;
			outSample = (inputSampleR * fix[fix_a0]) + fix[fix_sR1];
			fix[fix_sR1] = (inputSampleR * fix[fix_a1]) - (outSample * fix[fix_b1]) + fix[fix_sR2];
			fix[fix_sR2] = (inputSampleR * fix[fix_a2]) - (outSample * fix[fix_b2]);
			inputSampleR = outSample;
		} //fixed biquad filtering ultrasonics
		inputSampleL *= inTrim;
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633; if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		inputSampleR *= inTrim;
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633; if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		
		
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

void Console8SubOut::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	inTrimA = inTrimB; inTrimB = A*2.0;
	//0.5 is unity gain, and we can attenuate to silence or boost slightly over 12dB
	//into softclipping overdrive.
    if (getSampleRate() > 49000.0) hsr = true; else hsr = false;
    fix[fix_freq] = 24000.0 / getSampleRate();
    fix[fix_reso] = 1.20361562;
    double K = tan(M_PI * fix[fix_freq]); //lowpass
    double norm = 1.0 / (1.0 + K / fix[fix_reso] + K * K);
    fix[fix_a0] = K * K * norm;
    fix[fix_a1] = 2.0 * fix[fix_a0];
    fix[fix_a2] = fix[fix_a0];
    fix[fix_b1] = 2.0 * (K * K - 1.0) * norm;
    fix[fix_b2] = (1.0 - K / fix[fix_reso] + K * K) * norm;
    //this is the fixed biquad distributed anti-aliasing filter

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		
		double position = (double)sampleFrames/inFramesToProcess;
		double inTrim = (inTrimA*position)+(inTrimB*(1.0-position));
		//input trim smoothed to cut out zipper noise
		inputSampleL *= inTrim;
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633; if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		inputSampleR *= inTrim;
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633; if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		if (hsr){
			double outSample = (inputSampleL * fix[fix_a0]) + fix[fix_sL1];
			fix[fix_sL1] = (inputSampleL * fix[fix_a1]) - (outSample * fix[fix_b1]) + fix[fix_sL2];
			fix[fix_sL2] = (inputSampleL * fix[fix_a2]) - (outSample * fix[fix_b2]);
			inputSampleL = outSample;
			outSample = (inputSampleR * fix[fix_a0]) + fix[fix_sR1];
			fix[fix_sR1] = (inputSampleR * fix[fix_a1]) - (outSample * fix[fix_b1]) + fix[fix_sR2];
			fix[fix_sR2] = (inputSampleR * fix[fix_a2]) - (outSample * fix[fix_b2]);
			inputSampleR = outSample;
		} //fixed biquad filtering ultrasonics
		inputSampleL *= inTrim;
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633; if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		inputSampleR *= inTrim;
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633; if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		
		
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
