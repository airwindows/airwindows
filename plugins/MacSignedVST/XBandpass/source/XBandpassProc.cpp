/* ========================================
 *  XBandpass - XBandpass.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __XBandpass_H
#include "XBandpass.h"
#endif

void XBandpass::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double gain = pow(A+0.5,4);
	biquadA[0] = (pow(B,4)*20000.0)/getSampleRate();
	if (biquadA[0] < 0.00005) biquadA[0] = 0.00005;
	
	double compensation = sqrt(biquadA[0])*6.4;
	double clipFactor = 0.768+(biquadA[0]*C*37.0);
	
	double K = tan(M_PI * biquadA[0]);
	double norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquadA[2] = K / 0.7071 * norm;
	//biquadA[3] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
	biquadA[4] = -biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / 0.7071 + K * K) * norm;
	
	for (int x = 0; x < 7; x++) {biquad[x] = biquadD[x] = biquadC[x] = biquadB[x] = biquadA[x];}
	
	double aWet = 1.0;
	double bWet = 1.0;
	double cWet = 1.0;
	double dWet = C*4.0;
	double wet = D;
	
	//four-stage wet/dry control using progressive stages that bypass when not engaged
	if (dWet < 1.0) {aWet = dWet; bWet = 0.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 2.0) {bWet = dWet - 1.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 3.0) {cWet = dWet - 2.0; dWet = 0.0;}
	else {dWet -= 3.0;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	double outSample = 0.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (gain != 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		}
		
		double nukeLevelL = inputSampleL;
		double nukeLevelR = inputSampleR;
		
		inputSampleL *= clipFactor;
		outSample = biquad[2]*inputSampleL+biquad[4]*biquad[8]-biquad[5]*biquad[9]-biquad[6]*biquad[10];
		biquad[8] = biquad[7]; biquad[7] = inputSampleL; biquad[10] = biquad[9];
		if (outSample > 1.57079633) outSample = 1.57079633;
		if (outSample < -1.57079633) outSample = -1.57079633;
		biquad[9] = sin(outSample); //DF1 left
		inputSampleL = outSample / compensation; nukeLevelL = inputSampleL;
		
		inputSampleR *= clipFactor;
		outSample = biquad[2]*inputSampleR+biquad[4]*biquad[12]-biquad[5]*biquad[13]-biquad[6]*biquad[14];
		biquad[12] = biquad[11]; biquad[11] = inputSampleR; biquad[14] = biquad[13];
		if (outSample > 1.57079633) outSample = 1.57079633;
		if (outSample < -1.57079633) outSample = -1.57079633;
		biquad[13] = sin(outSample); //DF1 right
		inputSampleR = outSample / compensation; nukeLevelR = inputSampleR;
		
		if (aWet > 0.0) {
			inputSampleL *= clipFactor;
			outSample = biquadA[2]*inputSampleL+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
			biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; biquadA[10] = biquadA[9];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadA[9] = sin(outSample); //DF1 left
			inputSampleL = outSample / compensation; inputSampleL = (inputSampleL * aWet) + (nukeLevelL * (1.0-aWet));
			nukeLevelL = inputSampleL;
			
			inputSampleR *= clipFactor;
			outSample = biquadA[2]*inputSampleR+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
			biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; biquadA[14] = biquadA[13];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadA[13] = sin(outSample); //DF1 right
			inputSampleR = outSample / compensation; inputSampleR = (inputSampleR * aWet) + (nukeLevelR * (1.0-aWet));
			nukeLevelR = inputSampleR;
		}
		if (bWet > 0.0) {
			inputSampleL *= clipFactor;
			outSample = biquadB[2]*inputSampleL+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; biquadB[10] = biquadB[9];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadB[9] = sin(outSample); //DF1 left
			inputSampleL = outSample / compensation; inputSampleL = (inputSampleL * bWet) + (nukeLevelL * (1.0-bWet));
			nukeLevelL = inputSampleL;
			
			inputSampleR *= clipFactor;
			outSample = biquadB[2]*inputSampleR+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
			biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; biquadB[14] = biquadB[13];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadB[13] = sin(outSample); //DF1 right
			inputSampleR = outSample / compensation; inputSampleR = (inputSampleR * bWet) + (nukeLevelR * (1.0-bWet));
			nukeLevelR = inputSampleR;
		}
		if (cWet > 0.0) {
			inputSampleL *= clipFactor;
			outSample = biquadC[2]*inputSampleL+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
			biquadC[8] = biquadC[7]; biquadC[7] = inputSampleL; biquadC[10] = biquadC[9];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadC[9] = sin(outSample); //DF1 left
			inputSampleL = outSample / compensation; inputSampleL = (inputSampleL * cWet) + (nukeLevelL * (1.0-cWet));
			nukeLevelL = inputSampleL;
			
			inputSampleR *= clipFactor;
			outSample = biquadC[2]*inputSampleR+biquadC[4]*biquadC[12]-biquadC[5]*biquadC[13]-biquadC[6]*biquadC[14];
			biquadC[12] = biquadC[11]; biquadC[11] = inputSampleR; biquadC[14] = biquadC[13];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadC[13] = sin(outSample); //DF1 right
			inputSampleR = outSample / compensation; inputSampleR = (inputSampleR * cWet) + (nukeLevelR * (1.0-cWet));
			nukeLevelR = inputSampleR;
		}
		if (dWet > 0.0) {
			inputSampleL *= clipFactor;
			outSample = biquadD[2]*inputSampleL+biquadD[4]*biquadD[8]-biquadD[5]*biquadD[9]-biquadD[6]*biquadD[10];
			biquadD[8] = biquadD[7]; biquadD[7] = inputSampleL; biquadD[10] = biquadD[9];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadD[9] = sin(outSample); //DF1 left
			inputSampleL = outSample / compensation; inputSampleL = (inputSampleL * dWet) + (nukeLevelL * (1.0-dWet));
			nukeLevelL = inputSampleL;
			
			inputSampleR *= clipFactor;
			outSample = biquadD[2]*inputSampleR+biquadD[4]*biquadD[12]-biquadD[5]*biquadD[13]-biquadD[6]*biquadD[14];
			biquadD[12] = biquadD[11]; biquadD[11] = inputSampleR; biquadD[14] = biquadD[13];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadD[13] = sin(outSample); //DF1 right
			inputSampleR = outSample / compensation; inputSampleR = (inputSampleR * dWet) + (nukeLevelR * (1.0-dWet));
			nukeLevelR = inputSampleR;
		}
		
		if (wet < 1.0) {
			inputSampleL = (drySampleL * (1.0-wet))+(inputSampleL * wet);
			inputSampleR = (drySampleR * (1.0-wet))+(inputSampleR * wet);
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

void XBandpass::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double gain = pow(A+0.5,4);
	biquadA[0] = (pow(B,4)*20000.0)/getSampleRate();
	if (biquadA[0] < 0.00005) biquadA[0] = 0.00005;
	
	double compensation = sqrt(biquadA[0])*6.4;
	double clipFactor = 0.768+(biquadA[0]*C*37.0);
	
	double K = tan(M_PI * biquadA[0]);
	double norm = 1.0 / (1.0 + K / 0.7071 + K * K);
	biquadA[2] = K / 0.7071 * norm;
	//biquadA[3] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
	biquadA[4] = -biquadA[2];
	biquadA[5] = 2.0 * (K * K - 1.0) * norm;
	biquadA[6] = (1.0 - K / 0.7071 + K * K) * norm;
	
	for (int x = 0; x < 7; x++) {biquad[x] = biquadD[x] = biquadC[x] = biquadB[x] = biquadA[x];}
	
	double aWet = 1.0;
	double bWet = 1.0;
	double cWet = 1.0;
	double dWet = C*4.0;
	double wet = D;
	
	//four-stage wet/dry control using progressive stages that bypass when not engaged
	if (dWet < 1.0) {aWet = dWet; bWet = 0.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 2.0) {bWet = dWet - 1.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 3.0) {cWet = dWet - 2.0; dWet = 0.0;}
	else {dWet -= 3.0;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	double outSample = 0.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (gain != 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		}
		
		double nukeLevelL = inputSampleL;
		double nukeLevelR = inputSampleR;
		
		inputSampleL *= clipFactor;
		outSample = biquad[2]*inputSampleL+biquad[4]*biquad[8]-biquad[5]*biquad[9]-biquad[6]*biquad[10];
		biquad[8] = biquad[7]; biquad[7] = inputSampleL; biquad[10] = biquad[9];
		if (outSample > 1.57079633) outSample = 1.57079633;
		if (outSample < -1.57079633) outSample = -1.57079633;
		biquad[9] = sin(outSample); //DF1 left
		inputSampleL = outSample / compensation; nukeLevelL = inputSampleL;
		
		inputSampleR *= clipFactor;
		outSample = biquad[2]*inputSampleR+biquad[4]*biquad[12]-biquad[5]*biquad[13]-biquad[6]*biquad[14];
		biquad[12] = biquad[11]; biquad[11] = inputSampleR; biquad[14] = biquad[13];
		if (outSample > 1.57079633) outSample = 1.57079633;
		if (outSample < -1.57079633) outSample = -1.57079633;
		biquad[13] = sin(outSample); //DF1 right
		inputSampleR = outSample / compensation; nukeLevelR = inputSampleR;
		
		if (aWet > 0.0) {
			inputSampleL *= clipFactor;
			outSample = biquadA[2]*inputSampleL+biquadA[4]*biquadA[8]-biquadA[5]*biquadA[9]-biquadA[6]*biquadA[10];
			biquadA[8] = biquadA[7]; biquadA[7] = inputSampleL; biquadA[10] = biquadA[9];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadA[9] = sin(outSample); //DF1 left
			inputSampleL = outSample / compensation; inputSampleL = (inputSampleL * aWet) + (nukeLevelL * (1.0-aWet));
			nukeLevelL = inputSampleL;
			
			inputSampleR *= clipFactor;
			outSample = biquadA[2]*inputSampleR+biquadA[4]*biquadA[12]-biquadA[5]*biquadA[13]-biquadA[6]*biquadA[14];
			biquadA[12] = biquadA[11]; biquadA[11] = inputSampleR; biquadA[14] = biquadA[13];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadA[13] = sin(outSample); //DF1 right
			inputSampleR = outSample / compensation; inputSampleR = (inputSampleR * aWet) + (nukeLevelR * (1.0-aWet));
			nukeLevelR = inputSampleR;
		}
		if (bWet > 0.0) {
			inputSampleL *= clipFactor;
			outSample = biquadB[2]*inputSampleL+biquadB[4]*biquadB[8]-biquadB[5]*biquadB[9]-biquadB[6]*biquadB[10];
			biquadB[8] = biquadB[7]; biquadB[7] = inputSampleL; biquadB[10] = biquadB[9];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadB[9] = sin(outSample); //DF1 left
			inputSampleL = outSample / compensation; inputSampleL = (inputSampleL * bWet) + (nukeLevelL * (1.0-bWet));
			nukeLevelL = inputSampleL;
			
			inputSampleR *= clipFactor;
			outSample = biquadB[2]*inputSampleR+biquadB[4]*biquadB[12]-biquadB[5]*biquadB[13]-biquadB[6]*biquadB[14];
			biquadB[12] = biquadB[11]; biquadB[11] = inputSampleR; biquadB[14] = biquadB[13];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadB[13] = sin(outSample); //DF1 right
			inputSampleR = outSample / compensation; inputSampleR = (inputSampleR * bWet) + (nukeLevelR * (1.0-bWet));
			nukeLevelR = inputSampleR;
		}
		if (cWet > 0.0) {
			inputSampleL *= clipFactor;
			outSample = biquadC[2]*inputSampleL+biquadC[4]*biquadC[8]-biquadC[5]*biquadC[9]-biquadC[6]*biquadC[10];
			biquadC[8] = biquadC[7]; biquadC[7] = inputSampleL; biquadC[10] = biquadC[9];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadC[9] = sin(outSample); //DF1 left
			inputSampleL = outSample / compensation; inputSampleL = (inputSampleL * cWet) + (nukeLevelL * (1.0-cWet));
			nukeLevelL = inputSampleL;
			
			inputSampleR *= clipFactor;
			outSample = biquadC[2]*inputSampleR+biquadC[4]*biquadC[12]-biquadC[5]*biquadC[13]-biquadC[6]*biquadC[14];
			biquadC[12] = biquadC[11]; biquadC[11] = inputSampleR; biquadC[14] = biquadC[13];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadC[13] = sin(outSample); //DF1 right
			inputSampleR = outSample / compensation; inputSampleR = (inputSampleR * cWet) + (nukeLevelR * (1.0-cWet));
			nukeLevelR = inputSampleR;
		}
		if (dWet > 0.0) {
			inputSampleL *= clipFactor;
			outSample = biquadD[2]*inputSampleL+biquadD[4]*biquadD[8]-biquadD[5]*biquadD[9]-biquadD[6]*biquadD[10];
			biquadD[8] = biquadD[7]; biquadD[7] = inputSampleL; biquadD[10] = biquadD[9];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadD[9] = sin(outSample); //DF1 left
			inputSampleL = outSample / compensation; inputSampleL = (inputSampleL * dWet) + (nukeLevelL * (1.0-dWet));
			nukeLevelL = inputSampleL;
			
			inputSampleR *= clipFactor;
			outSample = biquadD[2]*inputSampleR+biquadD[4]*biquadD[12]-biquadD[5]*biquadD[13]-biquadD[6]*biquadD[14];
			biquadD[12] = biquadD[11]; biquadD[11] = inputSampleR; biquadD[14] = biquadD[13];
			if (outSample > 1.57079633) outSample = 1.57079633;
			if (outSample < -1.57079633) outSample = -1.57079633;
			biquadD[13] = sin(outSample); //DF1 right
			inputSampleR = outSample / compensation; inputSampleR = (inputSampleR * dWet) + (nukeLevelR * (1.0-dWet));
			nukeLevelR = inputSampleR;
		}
		
		if (wet < 1.0) {
			inputSampleL = (drySampleL * (1.0-wet))+(inputSampleL * wet);
			inputSampleR = (drySampleR * (1.0-wet))+(inputSampleR * wet);
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
