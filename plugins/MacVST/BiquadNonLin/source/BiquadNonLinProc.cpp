/* ========================================
 *  BiquadNonLin - BiquadNonLin.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BiquadNonLin_H
#include "BiquadNonLin.h"
#endif

void BiquadNonLin::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int type = 0;
	if (A > 0.5) type = 1;
	
	biquad[biq_freq] = ((B*B*B*0.9999)+0.0001)*0.499;
	if (biquad[biq_freq] < 0.0001) biquad[biq_freq] = 0.0001;
	
    biquad[biq_reso] = (C*C*C*29.99)+0.01;
	if (biquad[biq_reso] < 0.0001) biquad[biq_reso] = 0.0001;
	
	double nonLin = pow(D,2);
	//if you're using a 0.5 for a lowpass fixed frequency, value is 0.25
	
	double wet = (E*2.0)-1.0;
	
	//biquad contains these values:
	//[0] is frequency: 0.000001 to 0.499999 is near-zero to near-Nyquist
	//[1] is resonance, 0.7071 is Butterworth. Also can't be zero
	//[2] is a0 but you need distinct ones for additional biquad instances so it's here
	//[3] is a1 but you need distinct ones for additional biquad instances so it's here
	//[4] is a2 but you need distinct ones for additional biquad instances so it's here
	//[5] is b1 but you need distinct ones for additional biquad instances so it's here
	//[6] is b2 but you need distinct ones for additional biquad instances so it's here
	//[7] is LEFT stored delayed sample (freq and res are stored so you can move them sample by sample)
	//[8] is LEFT stored delayed sample (you have to include the coefficient making code if you do that)
	//[9] is RIGHT stored delayed sample (freq and res are stored so you can move them sample by sample)
	//[10] is RIGHT stored delayed sample (you have to include the coefficient making code if you do that)
	
	//to build a dedicated filter, rename 'biquad' to whatever the new filter is, then
	//put this code either within the sample buffer (for smoothly modulating freq or res)
	//or in this 'read the controls' area (for letting you change freq and res with controls)
	//or in 'reset' if the freq and res are absolutely fixed (use GetSampleRate to define freq)
	
	biquad[biq_aA0] = biquad[biq_aB0];
	biquad[biq_aA1] = biquad[biq_aB1];
	biquad[biq_aA2] = biquad[biq_aB2];
	biquad[biq_bA1] = biquad[biq_bB1];
	biquad[biq_bA2] = biquad[biq_bB2];
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.
	
	if (type == 0) { //lowpass
		double K = tan(M_PI * biquad[biq_freq]);
		double norm = 1.0 / (1.0 + K / biquad[biq_reso] + K * K);
		biquad[biq_aB0] = K * K * norm;
		biquad[biq_aB1] = 2.0 * biquad[biq_aB0];
		biquad[biq_aB2] = biquad[biq_aB0];
		biquad[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
		biquad[biq_bB2] = (1.0 - K / biquad[biq_reso] + K * K) * norm;
	}
	
	if (type == 1) { //bandpass
		double K = tan(M_PI * biquad[biq_freq]);
		double norm = 1.0 / (1.0 + K / biquad[biq_reso] + K * K);
		biquad[biq_aB0] = K / biquad[biq_reso] * norm;
		biquad[biq_aB1] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
		biquad[biq_aB2] = -biquad[biq_aB0];
		biquad[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
		biquad[biq_bB2] = (1.0 - K / biquad[biq_reso] + K * K) * norm;
	}
	
	if (biquad[biq_aA0] == 0.0) { // if we have just started, start directly with raw info
		biquad[biq_aA0] = biquad[biq_aB0];
		biquad[biq_aA1] = biquad[biq_aB1];
		biquad[biq_aA2] = biquad[biq_aB2];
		biquad[biq_bA1] = biquad[biq_bB1];
		biquad[biq_bA2] = biquad[biq_bB2];
	}
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double buf = (double)sampleFrames/inFramesToProcess;
		biquad[biq_a0] = (biquad[biq_aA0]*buf)+(biquad[biq_aB0]*(1.0-buf));
		biquad[biq_a1] = (biquad[biq_aA1]*buf)+(biquad[biq_aB1]*(1.0-buf));
		biquad[biq_a2] = (biquad[biq_aA2]*buf)+(biquad[biq_aB2]*(1.0-buf));
		biquad[biq_b1] = (biquad[biq_bA1]*buf)+(biquad[biq_bB1]*(1.0-buf));
		biquad[biq_b2] = (biquad[biq_bA2]*buf)+(biquad[biq_bB2]*(1.0-buf));
		
		double dia0 = fabs(biquad[biq_a0]*(1.0+(inputSampleL*nonLin))); if (dia0 > 1.0) dia0 = 1.0;
		double dia2 = dia0; //if lowpass, use this in both places
		if (type == 1) dia2 = -dia2;
		//if bandpass, you must reverse polarity
		double tempSample = (inputSampleL * dia0) + biquad[biq_sL1];
		biquad[biq_sL1] = (inputSampleL * biquad[biq_a1]) - (tempSample * biquad[biq_b1]) + biquad[biq_sL2];
		biquad[biq_sL2] = (inputSampleL * dia2) - (tempSample * biquad[biq_b2]);
		inputSampleL = tempSample;
		inputSampleL *= wet;
		if (wet > 0.0) {
			inputSampleL += (drySampleL*(1.0-wet));
		} else {
			inputSampleL += drySampleL;
		} //inv/dry/wet lets us turn LP into HP and band into notch
		
		dia0 = fabs(biquad[biq_a0]*(1.0+(inputSampleR*nonLin))); if (dia0 > 1.0) dia0 = 1.0;
		dia2 = dia0; //if lowpass, use this in both places
		if (type == 1) dia2 = -dia2;
		//if bandpass, you must reverse polarity
		tempSample = (inputSampleR * dia0) + biquad[biq_sR1];
		biquad[biq_sR1] = (inputSampleR * biquad[biq_a1]) - (tempSample * biquad[biq_b1]) + biquad[biq_sR2];
		biquad[biq_sR2] = (inputSampleR * dia2) - (tempSample * biquad[biq_b2]);
		inputSampleR = tempSample;
		inputSampleR *= wet;
		if (wet > 0.0) {
			inputSampleR += (drySampleR*(1.0-wet));
		} else {
			inputSampleR += drySampleR;
		} //inv/dry/wet lets us turn RP into HP and band into notch
		
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

void BiquadNonLin::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int type = 0;
	if (A > 0.5) type = 1;
	
	biquad[biq_freq] = ((B*B*B*0.9999)+0.0001)*0.499;
	if (biquad[biq_freq] < 0.0001) biquad[biq_freq] = 0.0001;
	
    biquad[biq_reso] = (C*C*C*29.99)+0.01;
	if (biquad[biq_reso] < 0.0001) biquad[biq_reso] = 0.0001;
	
	double nonLin = pow(D,2);
	//if you're using a 0.5 for a lowpass fixed frequency, value is 0.25
	
	double wet = (E*2.0)-1.0;
	
	//biquad contains these values:
	//[0] is frequency: 0.000001 to 0.499999 is near-zero to near-Nyquist
	//[1] is resonance, 0.7071 is Butterworth. Also can't be zero
	//[2] is a0 but you need distinct ones for additional biquad instances so it's here
	//[3] is a1 but you need distinct ones for additional biquad instances so it's here
	//[4] is a2 but you need distinct ones for additional biquad instances so it's here
	//[5] is b1 but you need distinct ones for additional biquad instances so it's here
	//[6] is b2 but you need distinct ones for additional biquad instances so it's here
	//[7] is LEFT stored delayed sample (freq and res are stored so you can move them sample by sample)
	//[8] is LEFT stored delayed sample (you have to include the coefficient making code if you do that)
	//[9] is RIGHT stored delayed sample (freq and res are stored so you can move them sample by sample)
	//[10] is RIGHT stored delayed sample (you have to include the coefficient making code if you do that)
	
	//to build a dedicated filter, rename 'biquad' to whatever the new filter is, then
	//put this code either within the sample buffer (for smoothly modulating freq or res)
	//or in this 'read the controls' area (for letting you change freq and res with controls)
	//or in 'reset' if the freq and res are absolutely fixed (use GetSampleRate to define freq)
	
	biquad[biq_aA0] = biquad[biq_aB0];
	biquad[biq_aA1] = biquad[biq_aB1];
	biquad[biq_aA2] = biquad[biq_aB2];
	biquad[biq_bA1] = biquad[biq_bB1];
	biquad[biq_bA2] = biquad[biq_bB2];
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.
	
	if (type == 0) { //lowpass
		double K = tan(M_PI * biquad[biq_freq]);
		double norm = 1.0 / (1.0 + K / biquad[biq_reso] + K * K);
		biquad[biq_aB0] = K * K * norm;
		biquad[biq_aB1] = 2.0 * biquad[biq_aB0];
		biquad[biq_aB2] = biquad[biq_aB0];
		biquad[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
		biquad[biq_bB2] = (1.0 - K / biquad[biq_reso] + K * K) * norm;
	}
	
	if (type == 1) { //bandpass
		double K = tan(M_PI * biquad[biq_freq]);
		double norm = 1.0 / (1.0 + K / biquad[biq_reso] + K * K);
		biquad[biq_aB0] = K / biquad[biq_reso] * norm;
		biquad[biq_aB1] = 0.0; //bandpass can simplify the biquad kernel: leave out this multiply
		biquad[biq_aB2] = -biquad[biq_aB0];
		biquad[biq_bB1] = 2.0 * (K * K - 1.0) * norm;
		biquad[biq_bB2] = (1.0 - K / biquad[biq_reso] + K * K) * norm;
	}
	
	if (biquad[biq_aA0] == 0.0) { // if we have just started, start directly with raw info
		biquad[biq_aA0] = biquad[biq_aB0];
		biquad[biq_aA1] = biquad[biq_aB1];
		biquad[biq_aA2] = biquad[biq_aB2];
		biquad[biq_bA1] = biquad[biq_bB1];
		biquad[biq_bA2] = biquad[biq_bB2];
	}
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double buf = (double)sampleFrames/inFramesToProcess;
		biquad[biq_a0] = (biquad[biq_aA0]*buf)+(biquad[biq_aB0]*(1.0-buf));
		biquad[biq_a1] = (biquad[biq_aA1]*buf)+(biquad[biq_aB1]*(1.0-buf));
		biquad[biq_a2] = (biquad[biq_aA2]*buf)+(biquad[biq_aB2]*(1.0-buf));
		biquad[biq_b1] = (biquad[biq_bA1]*buf)+(biquad[biq_bB1]*(1.0-buf));
		biquad[biq_b2] = (biquad[biq_bA2]*buf)+(biquad[biq_bB2]*(1.0-buf));
		
		double dia0 = fabs(biquad[biq_a0]*(1.0+(inputSampleL*nonLin))); if (dia0 > 1.0) dia0 = 1.0;
		double dia2 = dia0; //if lowpass, use this in both places
		if (type == 1) dia2 = -dia2;
		//if bandpass, you must reverse polarity
		double tempSample = (inputSampleL * dia0) + biquad[biq_sL1];
		biquad[biq_sL1] = (inputSampleL * biquad[biq_a1]) - (tempSample * biquad[biq_b1]) + biquad[biq_sL2];
		biquad[biq_sL2] = (inputSampleL * dia2) - (tempSample * biquad[biq_b2]);
		inputSampleL = tempSample;
		inputSampleL *= wet;
		if (wet > 0.0) {
			inputSampleL += (drySampleL*(1.0-wet));
		} else {
			inputSampleL += drySampleL;
		} //inv/dry/wet lets us turn LP into HP and band into notch
		
		dia0 = fabs(biquad[biq_a0]*(1.0+(inputSampleR*nonLin))); if (dia0 > 1.0) dia0 = 1.0;
		dia2 = dia0; //if lowpass, use this in both places
		if (type == 1) dia2 = -dia2;
		//if bandpass, you must reverse polarity
		tempSample = (inputSampleR * dia0) + biquad[biq_sR1];
		biquad[biq_sR1] = (inputSampleR * biquad[biq_a1]) - (tempSample * biquad[biq_b1]) + biquad[biq_sR2];
		biquad[biq_sR2] = (inputSampleR * dia2) - (tempSample * biquad[biq_b2]);
		inputSampleR = tempSample;
		inputSampleR *= wet;
		if (wet > 0.0) {
			inputSampleR += (drySampleR*(1.0-wet));
		} else {
			inputSampleR += drySampleR;
		} //inv/dry/wet lets us turn RP into HP and band into notch
		
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
