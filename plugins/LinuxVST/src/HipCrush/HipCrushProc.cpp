/* ========================================
 *  HipCrush - HipCrush.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __HipCrush_H
#include "HipCrush.h"
#endif

void HipCrush::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	high[biqs_freq] = (((pow(TRF,3)*16000.0)+1000.0)/getSampleRate());
	if (high[biqs_freq] < 0.0001) high[biqs_freq] = 0.0001;
	high[biqs_bit] = (TRB*2.0)-1.0;
	high[biqs_level] = (1.0-pow(1.0-TRG,2.0))*1.618033988749894848204586;
	high[biqs_reso] = pow(TRG+0.618033988749894848204586,2.0);
	double K = tan(M_PI * high[biqs_freq]);
	double norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.618033988749894848204586) + K * K);
	high[biqs_a0] = K / (high[biqs_reso]*0.618033988749894848204586) * norm;
	high[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_b2] = (1.0 - K / (high[biqs_reso]*0.618033988749894848204586) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (high[biqs_reso]*1.618033988749894848204586) + K * K);
	high[biqs_c0] = K / (high[biqs_reso]*1.618033988749894848204586) * norm;
	high[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_d2] = (1.0 - K / (high[biqs_reso]*1.618033988749894848204586) + K * K) * norm;
	//high
	
	hmid[biqs_freq] = (((pow(HMF,3)*7000.0)+300.0)/getSampleRate());
	if (hmid[biqs_freq] < 0.0001) hmid[biqs_freq] = 0.0001;
	hmid[biqs_bit] = (HMB*2.0)-1.0;
	hmid[biqs_level] = (1.0-pow(1.0-HMG,2.0))*1.618033988749894848204586;
	hmid[biqs_reso] = pow(HMG+0.618033988749894848204586,2.0);
	K = tan(M_PI * hmid[biqs_freq]);
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.618033988749894848204586) + K * K);
	hmid[biqs_a0] = K / (hmid[biqs_reso]*0.618033988749894848204586) * norm;
	hmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_b2] = (1.0 - K / (hmid[biqs_reso]*0.618033988749894848204586) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*1.618033988749894848204586) + K * K);
	hmid[biqs_c0] = K / (hmid[biqs_reso]*1.618033988749894848204586) * norm;
	hmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_d2] = (1.0 - K / (hmid[biqs_reso]*1.618033988749894848204586) + K * K) * norm;
	//hmid
	
	lmid[biqs_freq] = (((pow(LMF,3)*3000.0)+20.0)/getSampleRate());
	if (lmid[biqs_freq] < 0.00001) lmid[biqs_freq] = 0.00001;
	lmid[biqs_bit] = (LMB*2.0)-1.0;
	lmid[biqs_level] = (1.0-pow(1.0-LMG,2.0))*1.618033988749894848204586;
	lmid[biqs_reso] = pow(LMG+0.618033988749894848204586,2.0);
	K = tan(M_PI * lmid[biqs_freq]);
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.618033988749894848204586) + K * K);
	lmid[biqs_a0] = K / (lmid[biqs_reso]*0.618033988749894848204586) * norm;
	lmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_b2] = (1.0 - K / (lmid[biqs_reso]*0.618033988749894848204586) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*1.618033988749894848204586) + K * K);
	lmid[biqs_c0] = K / (lmid[biqs_reso]*1.618033988749894848204586) * norm;
	lmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_d2] = (1.0 - K / (lmid[biqs_reso]*1.618033988749894848204586) + K * K) * norm;
	//lmid
	
	double wet = DW;
		
	while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		high[biqs_outL] = inputSampleL * fabs(high[biqs_level]);
		high[biqs_temp] = (high[biqs_outL] * high[biqs_a0]) + high[biqs_aL1];
		high[biqs_aL1] = high[biqs_aL2] - (high[biqs_temp]*high[biqs_b1]);
		high[biqs_aL2] = (high[biqs_outL] * -high[biqs_a0]) - (high[biqs_temp]*high[biqs_b2]);
		high[biqs_outL] = high[biqs_temp];
		if (high[biqs_bit] != 0.0) {
			double bitFactor = high[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			high[biqs_outL] *= bitFactor;		
			high[biqs_outL] = floor(high[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
			high[biqs_outL] /= bitFactor;
		}
		high[biqs_temp] = (high[biqs_outL] * high[biqs_c0]) + high[biqs_cL1];
		high[biqs_cL1] = high[biqs_cL2] - (high[biqs_temp]*high[biqs_d1]);
		high[biqs_cL2] = (high[biqs_outL] * -high[biqs_c0]) - (high[biqs_temp]*high[biqs_d2]);
		high[biqs_outL] = high[biqs_temp];
		high[biqs_outL] *= high[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		hmid[biqs_outL] = inputSampleL * fabs(hmid[biqs_level]);
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_a0]) + hmid[biqs_aL1];
		hmid[biqs_aL1] = hmid[biqs_aL2] - (hmid[biqs_temp]*hmid[biqs_b1]);
		hmid[biqs_aL2] = (hmid[biqs_outL] * -hmid[biqs_a0]) - (hmid[biqs_temp]*hmid[biqs_b2]);
		hmid[biqs_outL] = hmid[biqs_temp];
		if (hmid[biqs_bit] != 0.0) {
			double bitFactor = hmid[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			hmid[biqs_outL] *= bitFactor;		
			hmid[biqs_outL] = floor(hmid[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
			hmid[biqs_outL] /= bitFactor;
		}
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_c0]) + hmid[biqs_cL1];
		hmid[biqs_cL1] = hmid[biqs_cL2] - (hmid[biqs_temp]*hmid[biqs_d1]);
		hmid[biqs_cL2] = (hmid[biqs_outL] * -hmid[biqs_c0]) - (hmid[biqs_temp]*hmid[biqs_d2]);
		hmid[biqs_outL] = hmid[biqs_temp];
		hmid[biqs_outL] *= hmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		lmid[biqs_outL] = inputSampleL * fabs(lmid[biqs_level]);
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_a0]) + lmid[biqs_aL1];
		lmid[biqs_aL1] = lmid[biqs_aL2] - (lmid[biqs_temp]*lmid[biqs_b1]);
		lmid[biqs_aL2] = (lmid[biqs_outL] * -lmid[biqs_a0]) - (lmid[biqs_temp]*lmid[biqs_b2]);
		lmid[biqs_outL] = lmid[biqs_temp];
		if (lmid[biqs_bit] != 0.0) {
			double bitFactor = lmid[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			lmid[biqs_outL] *= bitFactor;		
			lmid[biqs_outL] = floor(lmid[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
			lmid[biqs_outL] /= bitFactor;
		}
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_c0]) + lmid[biqs_cL1];
		lmid[biqs_cL1] = lmid[biqs_cL2] - (lmid[biqs_temp]*lmid[biqs_d1]);
		lmid[biqs_cL2] = (lmid[biqs_outL] * -lmid[biqs_c0]) - (lmid[biqs_temp]*lmid[biqs_d2]);
		lmid[biqs_outL] = lmid[biqs_temp];
		lmid[biqs_outL] *= lmid[biqs_level];
		double parametricL = high[biqs_outL] + hmid[biqs_outL] + lmid[biqs_outL];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		high[biqs_outR] = inputSampleR * fabs(high[biqs_level]);
		high[biqs_temp] = (high[biqs_outR] * high[biqs_a0]) + high[biqs_aR1];
		high[biqs_aR1] = high[biqs_aR2] - (high[biqs_temp]*high[biqs_b1]);
		high[biqs_aR2] = (high[biqs_outR] * -high[biqs_a0]) - (high[biqs_temp]*high[biqs_b2]);
		high[biqs_outR] = high[biqs_temp];
		if (high[biqs_bit] != 0.0) {
			double bitFactor = high[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			high[biqs_outR] *= bitFactor;		
			high[biqs_outR] = floor(high[biqs_outR]+(crushGate?0.5/bitFactor:0.0));
			high[biqs_outR] /= bitFactor;
		}
		high[biqs_temp] = (high[biqs_outR] * high[biqs_c0]) + high[biqs_cR1];
		high[biqs_cR1] = high[biqs_cR2] - (high[biqs_temp]*high[biqs_d1]);
		high[biqs_cR2] = (high[biqs_outR] * -high[biqs_c0]) - (high[biqs_temp]*high[biqs_d2]);
		high[biqs_outR] = high[biqs_temp];
		high[biqs_outR] *= high[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		hmid[biqs_outR] = inputSampleR * fabs(hmid[biqs_level]);
		hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_a0]) + hmid[biqs_aR1];
		hmid[biqs_aR1] = hmid[biqs_aR2] - (hmid[biqs_temp]*hmid[biqs_b1]);
		hmid[biqs_aR2] = (hmid[biqs_outR] * -hmid[biqs_a0]) - (hmid[biqs_temp]*hmid[biqs_b2]);
		hmid[biqs_outR] = hmid[biqs_temp];
		if (hmid[biqs_bit] != 0.0) {
			double bitFactor = hmid[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			hmid[biqs_outR] *= bitFactor;		
			hmid[biqs_outR] = floor(hmid[biqs_outR]+(crushGate?0.5/bitFactor:0.0));
			hmid[biqs_outR] /= bitFactor;
		}
		hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_c0]) + hmid[biqs_cR1];
		hmid[biqs_cR1] = hmid[biqs_cR2] - (hmid[biqs_temp]*hmid[biqs_d1]);
		hmid[biqs_cR2] = (hmid[biqs_outR] * -hmid[biqs_c0]) - (hmid[biqs_temp]*hmid[biqs_d2]);
		hmid[biqs_outR] = hmid[biqs_temp];
		hmid[biqs_outR] *= hmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		lmid[biqs_outR] = inputSampleR * fabs(lmid[biqs_level]);
		lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_a0]) + lmid[biqs_aR1];
		lmid[biqs_aR1] = lmid[biqs_aR2] - (lmid[biqs_temp]*lmid[biqs_b1]);
		lmid[biqs_aR2] = (lmid[biqs_outR] * -lmid[biqs_a0]) - (lmid[biqs_temp]*lmid[biqs_b2]);
		lmid[biqs_outR] = lmid[biqs_temp];
		if (lmid[biqs_bit] != 0.0) {
			double bitFactor = lmid[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			lmid[biqs_outR] *= bitFactor;		
			lmid[biqs_outR] = floor(lmid[biqs_outR]+(crushGate?0.5/bitFactor:0.0));
			lmid[biqs_outR] /= bitFactor;
		}
		lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_c0]) + lmid[biqs_cR1];
		lmid[biqs_cR1] = lmid[biqs_cR2] - (lmid[biqs_temp]*lmid[biqs_d1]);
		lmid[biqs_cR2] = (lmid[biqs_outR] * -lmid[biqs_c0]) - (lmid[biqs_temp]*lmid[biqs_d2]);
		lmid[biqs_outR] = lmid[biqs_temp];
		lmid[biqs_outR] *= lmid[biqs_level];
		double parametricR = high[biqs_outR] + hmid[biqs_outR] + lmid[biqs_outR];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		inputSampleL = (drySampleL * (1.0-wet)) + (parametricL * wet);
		inputSampleR = (drySampleR * (1.0-wet)) + (parametricR * wet);
		
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

void HipCrush::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	high[biqs_freq] = (((pow(TRF,3)*16000.0)+1000.0)/getSampleRate());
	if (high[biqs_freq] < 0.0001) high[biqs_freq] = 0.0001;
	high[biqs_bit] = (TRB*2.0)-1.0;
	high[biqs_level] = (1.0-pow(1.0-TRG,2.0))*1.618033988749894848204586;
	high[biqs_reso] = pow(TRG+0.618033988749894848204586,2.0);
	double K = tan(M_PI * high[biqs_freq]);
	double norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.618033988749894848204586) + K * K);
	high[biqs_a0] = K / (high[biqs_reso]*0.618033988749894848204586) * norm;
	high[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_b2] = (1.0 - K / (high[biqs_reso]*0.618033988749894848204586) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (high[biqs_reso]*1.618033988749894848204586) + K * K);
	high[biqs_c0] = K / (high[biqs_reso]*1.618033988749894848204586) * norm;
	high[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_d2] = (1.0 - K / (high[biqs_reso]*1.618033988749894848204586) + K * K) * norm;
	//high
	
	hmid[biqs_freq] = (((pow(HMF,3)*7000.0)+300.0)/getSampleRate());
	if (hmid[biqs_freq] < 0.0001) hmid[biqs_freq] = 0.0001;
	hmid[biqs_bit] = (HMB*2.0)-1.0;
	hmid[biqs_level] = (1.0-pow(1.0-HMG,2.0))*1.618033988749894848204586;
	hmid[biqs_reso] = pow(HMG+0.618033988749894848204586,2.0);
	K = tan(M_PI * hmid[biqs_freq]);
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.618033988749894848204586) + K * K);
	hmid[biqs_a0] = K / (hmid[biqs_reso]*0.618033988749894848204586) * norm;
	hmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_b2] = (1.0 - K / (hmid[biqs_reso]*0.618033988749894848204586) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*1.618033988749894848204586) + K * K);
	hmid[biqs_c0] = K / (hmid[biqs_reso]*1.618033988749894848204586) * norm;
	hmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_d2] = (1.0 - K / (hmid[biqs_reso]*1.618033988749894848204586) + K * K) * norm;
	//hmid
	
	lmid[biqs_freq] = (((pow(LMF,3)*3000.0)+20.0)/getSampleRate());
	if (lmid[biqs_freq] < 0.00001) lmid[biqs_freq] = 0.00001;
	lmid[biqs_bit] = (LMB*2.0)-1.0;
	lmid[biqs_level] = (1.0-pow(1.0-LMG,2.0))*1.618033988749894848204586;
	lmid[biqs_reso] = pow(LMG+0.618033988749894848204586,2.0);
	K = tan(M_PI * lmid[biqs_freq]);
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.618033988749894848204586) + K * K);
	lmid[biqs_a0] = K / (lmid[biqs_reso]*0.618033988749894848204586) * norm;
	lmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_b2] = (1.0 - K / (lmid[biqs_reso]*0.618033988749894848204586) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*1.618033988749894848204586) + K * K);
	lmid[biqs_c0] = K / (lmid[biqs_reso]*1.618033988749894848204586) * norm;
	lmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_d2] = (1.0 - K / (lmid[biqs_reso]*1.618033988749894848204586) + K * K) * norm;
	//lmid
	
	double wet = DW;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		high[biqs_outL] = inputSampleL * fabs(high[biqs_level]);
		high[biqs_temp] = (high[biqs_outL] * high[biqs_a0]) + high[biqs_aL1];
		high[biqs_aL1] = high[biqs_aL2] - (high[biqs_temp]*high[biqs_b1]);
		high[biqs_aL2] = (high[biqs_outL] * -high[biqs_a0]) - (high[biqs_temp]*high[biqs_b2]);
		high[biqs_outL] = high[biqs_temp];
		if (high[biqs_bit] != 0.0) {
			double bitFactor = high[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			high[biqs_outL] *= bitFactor;		
			high[biqs_outL] = floor(high[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
			high[biqs_outL] /= bitFactor;
		}
		high[biqs_temp] = (high[biqs_outL] * high[biqs_c0]) + high[biqs_cL1];
		high[biqs_cL1] = high[biqs_cL2] - (high[biqs_temp]*high[biqs_d1]);
		high[biqs_cL2] = (high[biqs_outL] * -high[biqs_c0]) - (high[biqs_temp]*high[biqs_d2]);
		high[biqs_outL] = high[biqs_temp];
		high[biqs_outL] *= high[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		hmid[biqs_outL] = inputSampleL * fabs(hmid[biqs_level]);
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_a0]) + hmid[biqs_aL1];
		hmid[biqs_aL1] = hmid[biqs_aL2] - (hmid[biqs_temp]*hmid[biqs_b1]);
		hmid[biqs_aL2] = (hmid[biqs_outL] * -hmid[biqs_a0]) - (hmid[biqs_temp]*hmid[biqs_b2]);
		hmid[biqs_outL] = hmid[biqs_temp];
		if (hmid[biqs_bit] != 0.0) {
			double bitFactor = hmid[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			hmid[biqs_outL] *= bitFactor;		
			hmid[biqs_outL] = floor(hmid[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
			hmid[biqs_outL] /= bitFactor;
		}
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_c0]) + hmid[biqs_cL1];
		hmid[biqs_cL1] = hmid[biqs_cL2] - (hmid[biqs_temp]*hmid[biqs_d1]);
		hmid[biqs_cL2] = (hmid[biqs_outL] * -hmid[biqs_c0]) - (hmid[biqs_temp]*hmid[biqs_d2]);
		hmid[biqs_outL] = hmid[biqs_temp];
		hmid[biqs_outL] *= hmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		lmid[biqs_outL] = inputSampleL * fabs(lmid[biqs_level]);
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_a0]) + lmid[biqs_aL1];
		lmid[biqs_aL1] = lmid[biqs_aL2] - (lmid[biqs_temp]*lmid[biqs_b1]);
		lmid[biqs_aL2] = (lmid[biqs_outL] * -lmid[biqs_a0]) - (lmid[biqs_temp]*lmid[biqs_b2]);
		lmid[biqs_outL] = lmid[biqs_temp];
		if (lmid[biqs_bit] != 0.0) {
			double bitFactor = lmid[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			lmid[biqs_outL] *= bitFactor;		
			lmid[biqs_outL] = floor(lmid[biqs_outL]+(crushGate?0.5/bitFactor:0.0));
			lmid[biqs_outL] /= bitFactor;
		}
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_c0]) + lmid[biqs_cL1];
		lmid[biqs_cL1] = lmid[biqs_cL2] - (lmid[biqs_temp]*lmid[biqs_d1]);
		lmid[biqs_cL2] = (lmid[biqs_outL] * -lmid[biqs_c0]) - (lmid[biqs_temp]*lmid[biqs_d2]);
		lmid[biqs_outL] = lmid[biqs_temp];
		lmid[biqs_outL] *= lmid[biqs_level];
		double parametricL = high[biqs_outL] + hmid[biqs_outL] + lmid[biqs_outL];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		high[biqs_outR] = inputSampleR * fabs(high[biqs_level]);
		high[biqs_temp] = (high[biqs_outR] * high[biqs_a0]) + high[biqs_aR1];
		high[biqs_aR1] = high[biqs_aR2] - (high[biqs_temp]*high[biqs_b1]);
		high[biqs_aR2] = (high[biqs_outR] * -high[biqs_a0]) - (high[biqs_temp]*high[biqs_b2]);
		high[biqs_outR] = high[biqs_temp];
		if (high[biqs_bit] != 0.0) {
			double bitFactor = high[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			high[biqs_outR] *= bitFactor;		
			high[biqs_outR] = floor(high[biqs_outR]+(crushGate?0.5/bitFactor:0.0));
			high[biqs_outR] /= bitFactor;
		}
		high[biqs_temp] = (high[biqs_outR] * high[biqs_c0]) + high[biqs_cR1];
		high[biqs_cR1] = high[biqs_cR2] - (high[biqs_temp]*high[biqs_d1]);
		high[biqs_cR2] = (high[biqs_outR] * -high[biqs_c0]) - (high[biqs_temp]*high[biqs_d2]);
		high[biqs_outR] = high[biqs_temp];
		high[biqs_outR] *= high[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		hmid[biqs_outR] = inputSampleR * fabs(hmid[biqs_level]);
		hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_a0]) + hmid[biqs_aR1];
		hmid[biqs_aR1] = hmid[biqs_aR2] - (hmid[biqs_temp]*hmid[biqs_b1]);
		hmid[biqs_aR2] = (hmid[biqs_outR] * -hmid[biqs_a0]) - (hmid[biqs_temp]*hmid[biqs_b2]);
		hmid[biqs_outR] = hmid[biqs_temp];
		if (hmid[biqs_bit] != 0.0) {
			double bitFactor = hmid[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			hmid[biqs_outR] *= bitFactor;		
			hmid[biqs_outR] = floor(hmid[biqs_outR]+(crushGate?0.5/bitFactor:0.0));
			hmid[biqs_outR] /= bitFactor;
		}
		hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_c0]) + hmid[biqs_cR1];
		hmid[biqs_cR1] = hmid[biqs_cR2] - (hmid[biqs_temp]*hmid[biqs_d1]);
		hmid[biqs_cR2] = (hmid[biqs_outR] * -hmid[biqs_c0]) - (hmid[biqs_temp]*hmid[biqs_d2]);
		hmid[biqs_outR] = hmid[biqs_temp];
		hmid[biqs_outR] *= hmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		lmid[biqs_outR] = inputSampleR * fabs(lmid[biqs_level]);
		lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_a0]) + lmid[biqs_aR1];
		lmid[biqs_aR1] = lmid[biqs_aR2] - (lmid[biqs_temp]*lmid[biqs_b1]);
		lmid[biqs_aR2] = (lmid[biqs_outR] * -lmid[biqs_a0]) - (lmid[biqs_temp]*lmid[biqs_b2]);
		lmid[biqs_outR] = lmid[biqs_temp];
		if (lmid[biqs_bit] != 0.0) {
			double bitFactor = lmid[biqs_bit];
			bool crushGate = (bitFactor < 0.0);
			bitFactor = pow(2.0,fmin(fmax((1.0-fabs(bitFactor))*16.0,0.5),16.0));
			lmid[biqs_outR] *= bitFactor;		
			lmid[biqs_outR] = floor(lmid[biqs_outR]+(crushGate?0.5/bitFactor:0.0));
			lmid[biqs_outR] /= bitFactor;
		}
		lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_c0]) + lmid[biqs_cR1];
		lmid[biqs_cR1] = lmid[biqs_cR2] - (lmid[biqs_temp]*lmid[biqs_d1]);
		lmid[biqs_cR2] = (lmid[biqs_outR] * -lmid[biqs_c0]) - (lmid[biqs_temp]*lmid[biqs_d2]);
		lmid[biqs_outR] = lmid[biqs_temp];
		lmid[biqs_outR] *= lmid[biqs_level];
		double parametricR = high[biqs_outR] + hmid[biqs_outR] + lmid[biqs_outR];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		inputSampleL = (drySampleL * (1.0-wet)) + (parametricL * wet);
		inputSampleR = (drySampleR * (1.0-wet)) + (parametricR * wet);
		
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
