/* ========================================
 *  Console8BussHype - Console8BussHype.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console8BussHype_H
#include "Console8BussHype.h"
#endif

void Console8BussHype::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double iirAmountA = 12.66/getSampleRate();
	//this is our distributed unusual highpass, which is
	//adding subtle harmonics to the really deep stuff to define it
	if (fabs(iirAL)<1.18e-37) iirAL = 0.0;
	if (fabs(iirBL)<1.18e-37) iirBL = 0.0;
	if (fabs(iirAR)<1.18e-37) iirAR = 0.0;
	if (fabs(iirBR)<1.18e-37) iirBR = 0.0;
	//catch denormals early and only check once per buffer
	if (getSampleRate() > 49000.0) hsr = true;
	else hsr = false;
	fix[fix_freq] = 24000.0 / getSampleRate();
    fix[fix_reso] = 0.5;
	double K = tan(M_PI * fix[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fix[fix_reso] + K * K);
	fix[fix_a0] = K * K * norm;
	fix[fix_a1] = 2.0 * fix[fix_a0];
	fix[fix_a2] = fix[fix_a0];
	fix[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fix[fix_b2] = (1.0 - K / fix[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd == 3) cycleEnd = 4;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 4 for 176 or 192k
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		
		iirAL = (iirAL * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
		double iirAmountBL = fabs(iirAL)+0.00001;
		iirBL = (iirBL * (1.0 - iirAmountBL)) + (iirAL * iirAmountBL);
		inputSampleL -= iirBL;
		iirAR = (iirAR * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
		double iirAmountBR = fabs(iirAR)+0.00001;
		iirBR = (iirBR * (1.0 - iirAmountBR)) + (iirAR * iirAmountBR);
		inputSampleR -= iirBR;
		//Console8 highpass
		if (cycleEnd == 4) {
			softL[8] = softL[7]; softL[7] = softL[6]; softL[6] = softL[5]; softL[5] = softL[4];
			softL[4] = softL[3]; softL[3] = softL[2]; softL[2] = softL[1]; softL[1] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[7]; softR[7] = softR[6]; softR[6] = softR[5]; softR[5] = softR[4];
			softR[4] = softR[3]; softR[3] = softR[2]; softR[2] = softR[1]; softR[1] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 2) {
			softL[8] = softL[6]; softL[6] = softL[4];
			softL[4] = softL[2]; softL[2] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[6]; softR[6] = softR[4];
			softR[4] = softR[2]; softR[2] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 1) {
			softL[8] = softL[4];
			softL[4] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[4];
			softR[4] = softR[0];
			softR[0] = inputSampleR;
		}
		softL[9] = ((softL[0]-softL[4])-(softL[4]-softL[8]));
		if (softL[9] < -1.57079633) softL[9] = -1.57079633;
		if (softL[9] > 1.57079633) softL[9] = 1.57079633;
		inputSampleL = softL[8]+(sin(softL[9])*0.61803398);
		softR[9] = ((softR[0]-softR[4])-(softR[4]-softR[8]));
		if (softR[9] < -1.57079633) softR[9] = -1.57079633;
		if (softR[9] > 1.57079633) softR[9] = 1.57079633;
		inputSampleR = softR[8]+(sin(softR[9])*0.61803398);
		//treble softening effect ended up being an aural exciter

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
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL = asin(inputSampleL); //Console8 decode
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR = asin(inputSampleR); //Console8 decode
		
		
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

void Console8BussHype::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double iirAmountA = 12.66/getSampleRate();
	//this is our distributed unusual highpass, which is
	//adding subtle harmonics to the really deep stuff to define it
	if (fabs(iirAL)<1.18e-37) iirAL = 0.0;
	if (fabs(iirBL)<1.18e-37) iirBL = 0.0;
	if (fabs(iirAR)<1.18e-37) iirAR = 0.0;
	if (fabs(iirBR)<1.18e-37) iirBR = 0.0;
	//catch denormals early and only check once per buffer
	if (getSampleRate() > 49000.0) hsr = true;
	else hsr = false;
	fix[fix_freq] = 24000.0 / getSampleRate();
    fix[fix_reso] = 0.5;
	double K = tan(M_PI * fix[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fix[fix_reso] + K * K);
	fix[fix_a0] = K * K * norm;
	fix[fix_a1] = 2.0 * fix[fix_a0];
	fix[fix_a2] = fix[fix_a0];
	fix[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fix[fix_b2] = (1.0 - K / fix[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd == 3) cycleEnd = 4;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 4 for 176 or 192k
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		
		iirAL = (iirAL * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
		double iirAmountBL = fabs(iirAL)+0.00001;
		iirBL = (iirBL * (1.0 - iirAmountBL)) + (iirAL * iirAmountBL);
		inputSampleL -= iirBL;
		iirAR = (iirAR * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
		double iirAmountBR = fabs(iirAR)+0.00001;
		iirBR = (iirBR * (1.0 - iirAmountBR)) + (iirAR * iirAmountBR);
		inputSampleR -= iirBR;
		//Console8 highpass
		if (cycleEnd == 4) {
			softL[8] = softL[7]; softL[7] = softL[6]; softL[6] = softL[5]; softL[5] = softL[4];
			softL[4] = softL[3]; softL[3] = softL[2]; softL[2] = softL[1]; softL[1] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[7]; softR[7] = softR[6]; softR[6] = softR[5]; softR[5] = softR[4];
			softR[4] = softR[3]; softR[3] = softR[2]; softR[2] = softR[1]; softR[1] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 2) {
			softL[8] = softL[6]; softL[6] = softL[4];
			softL[4] = softL[2]; softL[2] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[6]; softR[6] = softR[4];
			softR[4] = softR[2]; softR[2] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 1) {
			softL[8] = softL[4];
			softL[4] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[4];
			softR[4] = softR[0];
			softR[0] = inputSampleR;
		}
		softL[9] = ((softL[0]-softL[4])-(softL[4]-softL[8]));
		if (softL[9] < -1.57079633) softL[9] = -1.57079633;
		if (softL[9] > 1.57079633) softL[9] = 1.57079633;
		inputSampleL = softL[8]+(sin(softL[9])*0.61803398);
		softR[9] = ((softR[0]-softR[4])-(softR[4]-softR[8]));
		if (softR[9] < -1.57079633) softR[9] = -1.57079633;
		if (softR[9] > 1.57079633) softR[9] = 1.57079633;
		inputSampleR = softR[8]+(sin(softR[9])*0.61803398);
		//treble softening effect ended up being an aural exciter

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
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL = asin(inputSampleL); //Console8 decode
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR = asin(inputSampleR); //Console8 decode
		
		
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
