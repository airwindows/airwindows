/* ========================================
 *  BiquadStack - BiquadStack.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __BiquadStack_H
#include "BiquadStack.h"
#endif

void BiquadStack::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	biqs[biqs_levelA] = biqs[biqs_levelB];
	biqs[biqs_aA0] = biqs[biqs_aB0];
	biqs[biqs_bA1] = biqs[biqs_bB1];
	biqs[biqs_bA2] = biqs[biqs_bB2];
	biqs[biqs_cA0] = biqs[biqs_cB0];
	biqs[biqs_dA1] = biqs[biqs_dB1];
	biqs[biqs_dA2] = biqs[biqs_dB2];
	biqs[biqs_eA0] = biqs[biqs_eB0];
	biqs[biqs_fA1] = biqs[biqs_fB1];
	biqs[biqs_fA2] = biqs[biqs_fB2];
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.
	
	biqs[biqs_freq] = (((pow(A,4)*19980.0)+20.0)/getSampleRate());	
	biqs[biqs_nonlin] = B;
	biqs[biqs_levelB] = (biqs[biqs_nonlin]*2.0)-1.0;
	if (biqs[biqs_levelB] > 0.0) biqs[biqs_levelB] *= 2.0;
	biqs[biqs_reso] = ((0.5+(biqs[biqs_nonlin]*0.5)+sqrt(biqs[biqs_freq]))-(1.0-pow(1.0-C,2.0)))+0.5+(biqs[biqs_nonlin]*0.5);
	
	double K = tan(M_PI * biqs[biqs_freq]);
	double norm = 1.0 / (1.0 + K / (biqs[biqs_reso]*1.93185165) + K * K);
	biqs[biqs_aB0] = K / (biqs[biqs_reso]*1.93185165) * norm;
	biqs[biqs_bB1] = 2.0 * (K * K - 1.0) * norm;
	biqs[biqs_bB2] = (1.0 - K / (biqs[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (biqs[biqs_reso]*0.70710678) + K * K);
	biqs[biqs_cB0] = K / (biqs[biqs_reso]*0.70710678) * norm;
	biqs[biqs_dB1] = 2.0 * (K * K - 1.0) * norm;
	biqs[biqs_dB2] = (1.0 - K / (biqs[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (biqs[biqs_reso]*0.51763809) + K * K);
	biqs[biqs_eB0] = K / (biqs[biqs_reso]*0.51763809) * norm;
	biqs[biqs_fB1] = 2.0 * (K * K - 1.0) * norm;
	biqs[biqs_fB2] = (1.0 - K / (biqs[biqs_reso]*0.51763809) + K * K) * norm;
	
	if (biqs[biqs_aA0] == 0.0) { // if we have just started, start directly with raw info
		biqs[biqs_levelA] = biqs[biqs_levelB];
		biqs[biqs_aA0] = biqs[biqs_aB0];
		biqs[biqs_bA1] = biqs[biqs_bB1];
		biqs[biqs_bA2] = biqs[biqs_bB2];
		biqs[biqs_cA0] = biqs[biqs_cB0];
		biqs[biqs_dA1] = biqs[biqs_dB1];
		biqs[biqs_dA2] = biqs[biqs_dB2];
		biqs[biqs_eA0] = biqs[biqs_eB0];
		biqs[biqs_fA1] = biqs[biqs_fB1];
		biqs[biqs_fA2] = biqs[biqs_fB2];
	}
		
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double buf = (double)sampleFrames/inFramesToProcess;
		biqs[biqs_level] = (biqs[biqs_levelA]*buf)+(biqs[biqs_levelB]*(1.0-buf));
		biqs[biqs_a0] = (biqs[biqs_aA0]*buf)+(biqs[biqs_aB0]*(1.0-buf));
		biqs[biqs_b1] = (biqs[biqs_bA1]*buf)+(biqs[biqs_bB1]*(1.0-buf));
		biqs[biqs_b2] = (biqs[biqs_bA2]*buf)+(biqs[biqs_bB2]*(1.0-buf));
		biqs[biqs_c0] = (biqs[biqs_cA0]*buf)+(biqs[biqs_cB0]*(1.0-buf));
		biqs[biqs_d1] = (biqs[biqs_dA1]*buf)+(biqs[biqs_dB1]*(1.0-buf));
		biqs[biqs_d2] = (biqs[biqs_dA2]*buf)+(biqs[biqs_dB2]*(1.0-buf));
		biqs[biqs_e0] = (biqs[biqs_eA0]*buf)+(biqs[biqs_eB0]*(1.0-buf));
		biqs[biqs_f1] = (biqs[biqs_fA1]*buf)+(biqs[biqs_fB1]*(1.0-buf));
		biqs[biqs_f2] = (biqs[biqs_fA2]*buf)+(biqs[biqs_fB2]*(1.0-buf));
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		biqs[biqs_outL] = inputSampleL * fabs(biqs[biqs_level]);
		biqs[biqs_dis] = fabs(biqs[biqs_a0] * (1.0+(biqs[biqs_outL]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outL] * biqs[biqs_dis]) + biqs[biqs_aL1];
		biqs[biqs_aL1] = biqs[biqs_aL2] - (biqs[biqs_temp]*biqs[biqs_b1]);
		biqs[biqs_aL2] = (biqs[biqs_outL] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_b2]);
		biqs[biqs_outL] = biqs[biqs_temp];
		biqs[biqs_dis] = fabs(biqs[biqs_c0] * (1.0+(biqs[biqs_outL]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outL] * biqs[biqs_dis]) + biqs[biqs_cL1];
		biqs[biqs_cL1] = biqs[biqs_cL2] - (biqs[biqs_temp]*biqs[biqs_d1]);
		biqs[biqs_cL2] = (biqs[biqs_outL] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_d2]);
		biqs[biqs_outL] = biqs[biqs_temp];
		biqs[biqs_dis] = fabs(biqs[biqs_e0] * (1.0+(biqs[biqs_outL]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outL] * biqs[biqs_dis]) + biqs[biqs_eL1];
		biqs[biqs_eL1] = biqs[biqs_eL2] - (biqs[biqs_temp]*biqs[biqs_f1]);
		biqs[biqs_eL2] = (biqs[biqs_outL] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_f2]);
		biqs[biqs_outL] = biqs[biqs_temp];
		biqs[biqs_outL] *= biqs[biqs_level];
		if (biqs[biqs_level] > 1.0) biqs[biqs_outL] *= biqs[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		biqs[biqs_outR] = inputSampleR * fabs(biqs[biqs_level]);
		biqs[biqs_dis] = fabs(biqs[biqs_a0] * (1.0+(biqs[biqs_outR]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outR] * biqs[biqs_dis]) + biqs[biqs_aR1];
		biqs[biqs_aR1] = biqs[biqs_aR2] - (biqs[biqs_temp]*biqs[biqs_b1]);
		biqs[biqs_aR2] = (biqs[biqs_outR] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_b2]);
		biqs[biqs_outR] = biqs[biqs_temp];
		biqs[biqs_dis] = fabs(biqs[biqs_c0] * (1.0+(biqs[biqs_outR]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outR] * biqs[biqs_dis]) + biqs[biqs_cR1];
		biqs[biqs_cR1] = biqs[biqs_cR2] - (biqs[biqs_temp]*biqs[biqs_d1]);
		biqs[biqs_cR2] = (biqs[biqs_outR] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_d2]);
		biqs[biqs_outR] = biqs[biqs_temp];
		biqs[biqs_dis] = fabs(biqs[biqs_e0] * (1.0+(biqs[biqs_outR]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outR] * biqs[biqs_dis]) + biqs[biqs_eR1];
		biqs[biqs_eR1] = biqs[biqs_eR2] - (biqs[biqs_temp]*biqs[biqs_f1]);
		biqs[biqs_eR2] = (biqs[biqs_outR] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_f2]);
		biqs[biqs_outR] = biqs[biqs_temp];
		biqs[biqs_outR] *= biqs[biqs_level];
		if (biqs[biqs_level] > 1.0) biqs[biqs_outR] *= biqs[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		inputSampleL += biqs[biqs_outL]; //purely a parallel filter stage here
		inputSampleR += biqs[biqs_outR]; //purely a parallel filter stage here
		
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

void BiquadStack::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	biqs[biqs_levelA] = biqs[biqs_levelB];
	biqs[biqs_aA0] = biqs[biqs_aB0];
	biqs[biqs_bA1] = biqs[biqs_bB1];
	biqs[biqs_bA2] = biqs[biqs_bB2];
	biqs[biqs_cA0] = biqs[biqs_cB0];
	biqs[biqs_dA1] = biqs[biqs_dB1];
	biqs[biqs_dA2] = biqs[biqs_dB2];
	biqs[biqs_eA0] = biqs[biqs_eB0];
	biqs[biqs_fA1] = biqs[biqs_fB1];
	biqs[biqs_fA2] = biqs[biqs_fB2];
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.
	
	biqs[biqs_freq] = (((pow(A,4)*19980.0)+20.0)/getSampleRate());	
	biqs[biqs_nonlin] = B;
	biqs[biqs_levelB] = (biqs[biqs_nonlin]*2.0)-1.0;
	if (biqs[biqs_levelB] > 0.0) biqs[biqs_levelB] *= 2.0;
	biqs[biqs_reso] = ((0.5+(biqs[biqs_nonlin]*0.5)+sqrt(biqs[biqs_freq]))-(1.0-pow(1.0-C,2.0)))+0.5+(biqs[biqs_nonlin]*0.5);
	
	double K = tan(M_PI * biqs[biqs_freq]);
	double norm = 1.0 / (1.0 + K / (biqs[biqs_reso]*1.93185165) + K * K);
	biqs[biqs_aB0] = K / (biqs[biqs_reso]*1.93185165) * norm;
	biqs[biqs_bB1] = 2.0 * (K * K - 1.0) * norm;
	biqs[biqs_bB2] = (1.0 - K / (biqs[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (biqs[biqs_reso]*0.70710678) + K * K);
	biqs[biqs_cB0] = K / (biqs[biqs_reso]*0.70710678) * norm;
	biqs[biqs_dB1] = 2.0 * (K * K - 1.0) * norm;
	biqs[biqs_dB2] = (1.0 - K / (biqs[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (biqs[biqs_reso]*0.51763809) + K * K);
	biqs[biqs_eB0] = K / (biqs[biqs_reso]*0.51763809) * norm;
	biqs[biqs_fB1] = 2.0 * (K * K - 1.0) * norm;
	biqs[biqs_fB2] = (1.0 - K / (biqs[biqs_reso]*0.51763809) + K * K) * norm;
	
	if (biqs[biqs_aA0] == 0.0) { // if we have just started, start directly with raw info
		biqs[biqs_levelA] = biqs[biqs_levelB];
		biqs[biqs_aA0] = biqs[biqs_aB0];
		biqs[biqs_bA1] = biqs[biqs_bB1];
		biqs[biqs_bA2] = biqs[biqs_bB2];
		biqs[biqs_cA0] = biqs[biqs_cB0];
		biqs[biqs_dA1] = biqs[biqs_dB1];
		biqs[biqs_dA2] = biqs[biqs_dB2];
		biqs[biqs_eA0] = biqs[biqs_eB0];
		biqs[biqs_fA1] = biqs[biqs_fB1];
		biqs[biqs_fA2] = biqs[biqs_fB2];
	}
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double buf = (double)sampleFrames/inFramesToProcess;
		biqs[biqs_level] = (biqs[biqs_levelA]*buf)+(biqs[biqs_levelB]*(1.0-buf));
		biqs[biqs_a0] = (biqs[biqs_aA0]*buf)+(biqs[biqs_aB0]*(1.0-buf));
		biqs[biqs_b1] = (biqs[biqs_bA1]*buf)+(biqs[biqs_bB1]*(1.0-buf));
		biqs[biqs_b2] = (biqs[biqs_bA2]*buf)+(biqs[biqs_bB2]*(1.0-buf));
		biqs[biqs_c0] = (biqs[biqs_cA0]*buf)+(biqs[biqs_cB0]*(1.0-buf));
		biqs[biqs_d1] = (biqs[biqs_dA1]*buf)+(biqs[biqs_dB1]*(1.0-buf));
		biqs[biqs_d2] = (biqs[biqs_dA2]*buf)+(biqs[biqs_dB2]*(1.0-buf));
		biqs[biqs_e0] = (biqs[biqs_eA0]*buf)+(biqs[biqs_eB0]*(1.0-buf));
		biqs[biqs_f1] = (biqs[biqs_fA1]*buf)+(biqs[biqs_fB1]*(1.0-buf));
		biqs[biqs_f2] = (biqs[biqs_fA2]*buf)+(biqs[biqs_fB2]*(1.0-buf));
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		biqs[biqs_outL] = inputSampleL * fabs(biqs[biqs_level]);
		biqs[biqs_dis] = fabs(biqs[biqs_a0] * (1.0+(biqs[biqs_outL]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outL] * biqs[biqs_dis]) + biqs[biqs_aL1];
		biqs[biqs_aL1] = biqs[biqs_aL2] - (biqs[biqs_temp]*biqs[biqs_b1]);
		biqs[biqs_aL2] = (biqs[biqs_outL] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_b2]);
		biqs[biqs_outL] = biqs[biqs_temp];
		biqs[biqs_dis] = fabs(biqs[biqs_c0] * (1.0+(biqs[biqs_outL]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outL] * biqs[biqs_dis]) + biqs[biqs_cL1];
		biqs[biqs_cL1] = biqs[biqs_cL2] - (biqs[biqs_temp]*biqs[biqs_d1]);
		biqs[biqs_cL2] = (biqs[biqs_outL] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_d2]);
		biqs[biqs_outL] = biqs[biqs_temp];
		biqs[biqs_dis] = fabs(biqs[biqs_e0] * (1.0+(biqs[biqs_outL]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outL] * biqs[biqs_dis]) + biqs[biqs_eL1];
		biqs[biqs_eL1] = biqs[biqs_eL2] - (biqs[biqs_temp]*biqs[biqs_f1]);
		biqs[biqs_eL2] = (biqs[biqs_outL] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_f2]);
		biqs[biqs_outL] = biqs[biqs_temp];
		biqs[biqs_outL] *= biqs[biqs_level];
		if (biqs[biqs_level] > 1.0) biqs[biqs_outL] *= biqs[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		biqs[biqs_outR] = inputSampleR * fabs(biqs[biqs_level]);
		biqs[biqs_dis] = fabs(biqs[biqs_a0] * (1.0+(biqs[biqs_outR]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outR] * biqs[biqs_dis]) + biqs[biqs_aR1];
		biqs[biqs_aR1] = biqs[biqs_aR2] - (biqs[biqs_temp]*biqs[biqs_b1]);
		biqs[biqs_aR2] = (biqs[biqs_outR] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_b2]);
		biqs[biqs_outR] = biqs[biqs_temp];
		biqs[biqs_dis] = fabs(biqs[biqs_c0] * (1.0+(biqs[biqs_outR]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outR] * biqs[biqs_dis]) + biqs[biqs_cR1];
		biqs[biqs_cR1] = biqs[biqs_cR2] - (biqs[biqs_temp]*biqs[biqs_d1]);
		biqs[biqs_cR2] = (biqs[biqs_outR] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_d2]);
		biqs[biqs_outR] = biqs[biqs_temp];
		biqs[biqs_dis] = fabs(biqs[biqs_e0] * (1.0+(biqs[biqs_outR]*biqs[biqs_nonlin])));
		if (biqs[biqs_dis] > 1.0) biqs[biqs_dis] = 1.0;
		biqs[biqs_temp] = (biqs[biqs_outR] * biqs[biqs_dis]) + biqs[biqs_eR1];
		biqs[biqs_eR1] = biqs[biqs_eR2] - (biqs[biqs_temp]*biqs[biqs_f1]);
		biqs[biqs_eR2] = (biqs[biqs_outR] * -biqs[biqs_dis]) - (biqs[biqs_temp]*biqs[biqs_f2]);
		biqs[biqs_outR] = biqs[biqs_temp];
		biqs[biqs_outR] *= biqs[biqs_level];
		if (biqs[biqs_level] > 1.0) biqs[biqs_outR] *= biqs[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		inputSampleL += biqs[biqs_outL]; //purely a parallel filter stage here
		inputSampleR += biqs[biqs_outR]; //purely a parallel filter stage here
		
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
