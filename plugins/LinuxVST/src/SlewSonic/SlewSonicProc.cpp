/* ========================================
 *  SlewSonic - SlewSonic.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __SlewSonic_H
#include "SlewSonic.h"
#endif

void SlewSonic::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double trim = 2.302585092994045684017991; //natural logarithm of 10
	double freq = ((A*20000.0)+5000.0) / getSampleRate();
	if (freq > 0.499) freq = 0.499;
	biquadD[0] = biquadC[0] = biquadB[0] = biquadA[0] = freq;
	biquadA[1] = 2.24697960; //tenth order Butterworth out of five biquads
	biquadB[1] = 0.80193774;
	biquadC[1] = 0.55495813;
	biquadD[1] = 0.5;
	
	double K = tan(M_PI * biquadA[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadA[0]);
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = K * K * norm;
	biquadB[3] = 2.0 * biquadB[2];
	biquadB[4] = biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;
	
	K = tan(M_PI * biquadC[0]);
	norm = 1.0 / (1.0 + K / biquadC[1] + K * K);
	biquadC[2] = K * K * norm;
	biquadC[3] = 2.0 * biquadC[2];
	biquadC[4] = biquadC[2];
	biquadC[5] = 2.0 * (K * K - 1.0) * norm;
	biquadC[6] = (1.0 - K / biquadC[1] + K * K) * norm;
	
	K = tan(M_PI * biquadD[0]);
	norm = 1.0 / (1.0 + K / biquadD[1] + K * K);
	biquadD[2] = K * K * norm;
	biquadD[3] = 2.0 * biquadD[2];
	biquadD[4] = biquadD[2];
	biquadD[5] = 2.0 * (K * K - 1.0) * norm;
	biquadD[6] = (1.0 - K / biquadD[1] + K * K) * norm;
	
	double aWet = 0.0;
	double bWet = 0.0;
	double cWet = B * 3.0;
	//eight-stage wet/dry control using progressive stages that bypass when not engaged
	if (cWet < 1.0) {aWet = cWet; cWet = 0.0;}
	else if (cWet < 2.0) {bWet = cWet - 1.0; aWet = 1.0; cWet = 0.0;}
	else {cWet -= 2.0; bWet = aWet = 1.0;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		double dryStageAL = 0.0;
		double dryStageBL = 0.0;
		double dryStageAR = 0.0;
		double dryStageBR = 0.0;
		double dryFinalBiquadL = 0.0;
		double dryFinalBiquadR = 0.0;
		double outSampleL = 0.0;
		double outSampleR = 0.0;
		
		if (aWet > 0.0) {
			outSampleL = biquadA[2]*inputSampleL+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
			biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; inputSampleL = outSampleL; biquadA[10] = biquadA[9]; biquadA[9] = inputSampleL; //DF1 left
			outSampleR = biquadA[2]*inputSampleR+biquadA[3]*biquadA[11]+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
			biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; inputSampleR = outSampleR; biquadA[14] = biquadA[13]; biquadA[13] = inputSampleR; //DF1 right
			outSampleL = (inputSampleL - lastSampleAL)*trim;
			lastSampleAL = inputSampleL; inputSampleL = outSampleL;
			dryStageAL = inputSampleL = (inputSampleL * aWet) + (drySampleL * (1.0-aWet));
			//first stage always runs, dry/wet between raw signal and this
			outSampleR = (inputSampleR - lastSampleAR)*trim;
			lastSampleAR = inputSampleR; inputSampleR = outSampleR;
			dryStageAR = inputSampleR = (inputSampleR * aWet) + (drySampleR * (1.0-aWet));
			//first stage always runs, dry/wet between raw signal and this
		}
		
		if (bWet > 0.0) {
			outSampleL = biquadB[2]*inputSampleL+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; inputSampleL = outSampleL; biquadB[10] = biquadB[9]; biquadB[9] = inputSampleL; //DF1 left
			outSampleR = biquadB[2]*inputSampleR+biquadB[3]*biquadB[11]+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
			biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; inputSampleR = outSampleR; biquadB[14] = biquadB[13]; biquadB[13] = inputSampleR; //DF1 right
			outSampleL = (inputSampleL - lastSampleBL)*trim;
			lastSampleBL = inputSampleL; inputSampleL = outSampleL;
			dryStageBL = inputSampleL = (inputSampleL * bWet) + (dryStageAL * (1.0-bWet));
			//second stage adds upon first stage, crossfade between them
			outSampleR = (inputSampleR - lastSampleBR)*trim;
			lastSampleBR = inputSampleR; inputSampleR = outSampleR;
			dryStageBR = inputSampleR = (inputSampleR * bWet) + (dryStageAR * (1.0-bWet));
			//second stage adds upon first stage, crossfade between them
		}
		
		if (cWet > 0.0) {
			outSampleL = biquadC[2]*inputSampleL+biquadC[3]*biquadC[7]+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
			biquadC[8] = biquadC[7]; biquadC[7] = inputSampleL; inputSampleL = outSampleL; biquadC[10] = biquadC[9]; biquadC[9] = inputSampleL; //DF1 left
			outSampleR = biquadC[2]*inputSampleR+biquadC[3]*biquadC[11]+biquadC[4]*biquadC[12]-biquadC[5]*biquadC[13]-biquadC[6]*biquadC[14];
			biquadC[12] = biquadC[11]; biquadC[11] = inputSampleR; inputSampleR = outSampleR; biquadC[14] = biquadC[13]; biquadC[13] = inputSampleR; //DF1 right
			outSampleL = (inputSampleL - lastSampleCL)*trim;
			lastSampleCL = inputSampleL; inputSampleL = outSampleL;
			inputSampleL = (inputSampleL * cWet) + (dryStageBL * (1.0-cWet));
			//third stage adds upon second stage, crossfade between them
			outSampleR = (inputSampleR - lastSampleCR)*trim;
			lastSampleCR = inputSampleR; inputSampleR = outSampleR;
			inputSampleR = (inputSampleR * cWet) + (dryStageBR * (1.0-cWet));
			//third stage adds upon second stage, crossfade between them
		}
		
		
		if (aWet > 0.0) {
			dryFinalBiquadL = inputSampleL;
			dryFinalBiquadR = inputSampleR;
			outSampleL = biquadD[2]*inputSampleL+biquadD[3]*biquadD[7]+biquadD[4]*biquadD[8]-biquadD[5]*biquadD[9]-biquadD[6]*biquadD[10];
			biquadD[8] = biquadD[7]; biquadD[7] = inputSampleL; inputSampleL = outSampleL; biquadD[10] = biquadD[9]; biquadD[9] = inputSampleL; //DF1 left
			outSampleR = biquadD[2]*inputSampleR+biquadD[3]*biquadD[11]+biquadD[4]*biquadD[12]-biquadD[5]*biquadD[13]-biquadD[6]*biquadD[14];
			biquadD[12] = biquadD[11]; biquadD[11] = inputSampleR; inputSampleR = outSampleR; biquadD[14] = biquadD[13]; biquadD[13] = inputSampleR; //DF1 right
			//final post-slew-only stage always runs, minimum of one stage on dry and two on single slew-only
			inputSampleL = (inputSampleL * aWet) + (dryFinalBiquadL * (1.0-aWet));
			inputSampleR = (inputSampleR * aWet) + (dryFinalBiquadR * (1.0-aWet));
		}
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
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

void SlewSonic::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double trim = 2.302585092994045684017991; //natural logarithm of 10
	double freq = ((A*20000.0)+5000.0) / getSampleRate();
	if (freq > 0.499) freq = 0.499;
	biquadD[0] = biquadC[0] = biquadB[0] = biquadA[0] = freq;
	biquadA[1] = 2.24697960; //tenth order Butterworth out of five biquads
	biquadB[1] = 0.80193774;
	biquadC[1] = 0.55495813;
	biquadD[1] = 0.5;
	
	double K = tan(M_PI * biquadA[0]); //lowpass
	double norm = 1.0 / (1.0 + K / biquadA[1] + K * K);
	biquadA[2] = K * K * norm;
	biquadA[3] = 2.0 * biquadA[2];
	biquadA[4] = biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / biquadA[1] + K * K) * norm;
	
	K = tan(M_PI * biquadA[0]);
	norm = 1.0 / (1.0 + K / biquadB[1] + K * K);
	biquadB[2] = K * K * norm;
	biquadB[3] = 2.0 * biquadB[2];
	biquadB[4] = biquadB[2];
	biquadB[5] = 2.0 * (K * K - 1.0) * norm;
	biquadB[6] = (1.0 - K / biquadB[1] + K * K) * norm;
	
	K = tan(M_PI * biquadC[0]);
	norm = 1.0 / (1.0 + K / biquadC[1] + K * K);
	biquadC[2] = K * K * norm;
	biquadC[3] = 2.0 * biquadC[2];
	biquadC[4] = biquadC[2];
	biquadC[5] = 2.0 * (K * K - 1.0) * norm;
	biquadC[6] = (1.0 - K / biquadC[1] + K * K) * norm;
	
	K = tan(M_PI * biquadD[0]);
	norm = 1.0 / (1.0 + K / biquadD[1] + K * K);
	biquadD[2] = K * K * norm;
	biquadD[3] = 2.0 * biquadD[2];
	biquadD[4] = biquadD[2];
	biquadD[5] = 2.0 * (K * K - 1.0) * norm;
	biquadD[6] = (1.0 - K / biquadD[1] + K * K) * norm;
	
	double aWet = 0.0;
	double bWet = 0.0;
	double cWet = B * 3.0;
	//eight-stage wet/dry control using progressive stages that bypass when not engaged
	if (cWet < 1.0) {aWet = cWet; cWet = 0.0;}
	else if (cWet < 2.0) {bWet = cWet - 1.0; aWet = 1.0; cWet = 0.0;}
	else {cWet -= 2.0; bWet = aWet = 1.0;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		double dryStageAL = 0.0;
		double dryStageBL = 0.0;
		double dryStageAR = 0.0;
		double dryStageBR = 0.0;
		double dryFinalBiquadL = 0.0;
		double dryFinalBiquadR = 0.0;
		double outSampleL = 0.0;
		double outSampleR = 0.0;
		
		if (aWet > 0.0) {
			outSampleL = biquadA[2]*inputSampleL+biquadA[3]*biquadA[7]+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
			biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; inputSampleL = outSampleL; biquadA[10] = biquadA[9]; biquadA[9] = inputSampleL; //DF1 left
			outSampleR = biquadA[2]*inputSampleR+biquadA[3]*biquadA[11]+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
			biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; inputSampleR = outSampleR; biquadA[14] = biquadA[13]; biquadA[13] = inputSampleR; //DF1 right
			outSampleL = (inputSampleL - lastSampleAL)*trim;
			lastSampleAL = inputSampleL; inputSampleL = outSampleL;
			dryStageAL = inputSampleL = (inputSampleL * aWet) + (drySampleL * (1.0-aWet));
			//first stage always runs, dry/wet between raw signal and this
			outSampleR = (inputSampleR - lastSampleAR)*trim;
			lastSampleAR = inputSampleR; inputSampleR = outSampleR;
			dryStageAR = inputSampleR = (inputSampleR * aWet) + (drySampleR * (1.0-aWet));
			//first stage always runs, dry/wet between raw signal and this
		}
		
		if (bWet > 0.0) {
			outSampleL = biquadB[2]*inputSampleL+biquadB[3]*biquadB[7]+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; inputSampleL = outSampleL; biquadB[10] = biquadB[9]; biquadB[9] = inputSampleL; //DF1 left
			outSampleR = biquadB[2]*inputSampleR+biquadB[3]*biquadB[11]+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
			biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; inputSampleR = outSampleR; biquadB[14] = biquadB[13]; biquadB[13] = inputSampleR; //DF1 right
			outSampleL = (inputSampleL - lastSampleBL)*trim;
			lastSampleBL = inputSampleL; inputSampleL = outSampleL;
			dryStageBL = inputSampleL = (inputSampleL * bWet) + (dryStageAL * (1.0-bWet));
			//second stage adds upon first stage, crossfade between them
			outSampleR = (inputSampleR - lastSampleBR)*trim;
			lastSampleBR = inputSampleR; inputSampleR = outSampleR;
			dryStageBR = inputSampleR = (inputSampleR * bWet) + (dryStageAR * (1.0-bWet));
			//second stage adds upon first stage, crossfade between them
		}
		
		if (cWet > 0.0) {
			outSampleL = biquadC[2]*inputSampleL+biquadC[3]*biquadC[7]+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
			biquadC[8] = biquadC[7]; biquadC[7] = inputSampleL; inputSampleL = outSampleL; biquadC[10] = biquadC[9]; biquadC[9] = inputSampleL; //DF1 left
			outSampleR = biquadC[2]*inputSampleR+biquadC[3]*biquadC[11]+biquadC[4]*biquadC[12]-biquadC[5]*biquadC[13]-biquadC[6]*biquadC[14];
			biquadC[12] = biquadC[11]; biquadC[11] = inputSampleR; inputSampleR = outSampleR; biquadC[14] = biquadC[13]; biquadC[13] = inputSampleR; //DF1 right
			outSampleL = (inputSampleL - lastSampleCL)*trim;
			lastSampleCL = inputSampleL; inputSampleL = outSampleL;
			inputSampleL = (inputSampleL * cWet) + (dryStageBL * (1.0-cWet));
			//third stage adds upon second stage, crossfade between them
			outSampleR = (inputSampleR - lastSampleCR)*trim;
			lastSampleCR = inputSampleR; inputSampleR = outSampleR;
			inputSampleR = (inputSampleR * cWet) + (dryStageBR * (1.0-cWet));
			//third stage adds upon second stage, crossfade between them
		}
		
		
		if (aWet > 0.0) {
			dryFinalBiquadL = inputSampleL;
			dryFinalBiquadR = inputSampleR;
			outSampleL = biquadD[2]*inputSampleL+biquadD[3]*biquadD[7]+biquadD[4]*biquadD[8]-biquadD[5]*biquadD[9]-biquadD[6]*biquadD[10];
			biquadD[8] = biquadD[7]; biquadD[7] = inputSampleL; inputSampleL = outSampleL; biquadD[10] = biquadD[9]; biquadD[9] = inputSampleL; //DF1 left
			outSampleR = biquadD[2]*inputSampleR+biquadD[3]*biquadD[11]+biquadD[4]*biquadD[12]-biquadD[5]*biquadD[13]-biquadD[6]*biquadD[14];
			biquadD[12] = biquadD[11]; biquadD[11] = inputSampleR; inputSampleR = outSampleR; biquadD[14] = biquadD[13]; biquadD[13] = inputSampleR; //DF1 right
			//final post-slew-only stage always runs, minimum of one stage on dry and two on single slew-only
			inputSampleL = (inputSampleL * aWet) + (dryFinalBiquadL * (1.0-aWet));
			inputSampleR = (inputSampleR * aWet) + (dryFinalBiquadR * (1.0-aWet));
		}
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		
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
