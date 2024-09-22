/* ========================================
 *  ConsoleXPre - ConsoleXPre.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleXPre_H
#include "ConsoleXPre.h"
#endif

void ConsoleXPre::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	highpass[hilp_freq] = ((HIP*330.0)+20.0)/getSampleRate();
	bool highpassEngage = true; if (HIP == 0.0) highpassEngage = false;
	
	lowpass[hilp_freq] = ((pow(1.0-LOP,2)*17000.0)+3000.0)/getSampleRate();
	bool lowpassEngage = true; if (LOP == 0.0) lowpassEngage = false;
	
	double K = tan(M_PI * highpass[hilp_freq]); //highpass
	double norm = 1.0 / (1.0 + K / 1.93185165 + K * K);
	highpass[hilp_a0] = norm;
	highpass[hilp_a1] = -2.0 * highpass[hilp_a0];
	highpass[hilp_b1] = 2.0 * (K * K - 1.0) * norm;
	highpass[hilp_b2] = (1.0 - K / 1.93185165 + K * K) * norm;
	norm = 1.0 / (1.0 + K / 0.70710678 + K * K);
	highpass[hilp_c0] = norm;
	highpass[hilp_c1] = -2.0 * highpass[hilp_c0];
	highpass[hilp_d1] = 2.0 * (K * K - 1.0) * norm;
	highpass[hilp_d2] = (1.0 - K / 0.70710678 + K * K) * norm;
	norm = 1.0 / (1.0 + K / 0.51763809 + K * K);
	highpass[hilp_e0] = norm;
	highpass[hilp_e1] = -2.0 * highpass[hilp_e0];
	highpass[hilp_f1] = 2.0 * (K * K - 1.0) * norm;
	highpass[hilp_f2] = (1.0 - K / 0.51763809 + K * K) * norm;
	
	K = tan(M_PI * lowpass[hilp_freq]); //lowpass
	norm = 1.0 / (1.0 + K / 1.93185165 + K * K);
	lowpass[hilp_a0] = K * K * norm;
	lowpass[hilp_a1] = 2.0 * lowpass[hilp_a0];
	lowpass[hilp_b1] = 2.0 * (K * K - 1.0) * norm;
	lowpass[hilp_b2] = (1.0 - K / 1.93185165 + K * K) * norm;
	norm = 1.0 / (1.0 + K / 0.70710678 + K * K);
	lowpass[hilp_c0] = K * K * norm;
	lowpass[hilp_c1] = 2.0 * lowpass[hilp_c0];
	lowpass[hilp_d1] = 2.0 * (K * K - 1.0) * norm;
	lowpass[hilp_d2] = (1.0 - K / 0.70710678 + K * K) * norm;
	norm = 1.0 / (1.0 + K / 0.51763809 + K * K);
	lowpass[hilp_e0] = K * K * norm;
	lowpass[hilp_e1] = 2.0 * lowpass[hilp_e0];
	lowpass[hilp_f1] = 2.0 * (K * K - 1.0) * norm;
	lowpass[hilp_f2] = (1.0 - K / 0.51763809 + K * K) * norm;
	
	airGainA = airGainB; airGainB = AIR *2.0;
	fireGainA = fireGainB; fireGainB = FIR *2.0;
	stoneGainA = stoneGainB; stoneGainB = STO *2.0;
	//simple three band to adjust
	double kalmanRange = 1.0-(pow(RNG,2)/overallscale);
	//crossover frequency between mid/bass
	
	double compFThresh = pow(FCT,4);
	double compSThresh = pow(SCT,4);
	double compFRatio = 1.0-pow(1.0-FCR,2);
	double compSRatio = 1.0-pow(1.0-SCR,2);
	double compFAttack = 1.0/(((pow(FCA,3)*5000.0)+500.0)*overallscale);
	double compSAttack = 1.0/(((pow(SCA,3)*5000.0)+500.0)*overallscale);
	double compFRelease = 1.0/(((pow(FCL,5)*50000.0)+500.0)*overallscale);
	double compSRelease = 1.0/(((pow(SCL,5)*50000.0)+500.0)*overallscale);
	double gateFThresh = pow(FGT,4);
	double gateSThresh = pow(SGT,4);
	double gateFRatio = 1.0-pow(1.0-FGR,2);
	double gateSRatio = 1.0-pow(1.0-SGR,2);
	double gateFSustain = M_PI_2 * pow(FGS+1.0,4.0);
	double gateSSustain = M_PI_2 * pow(SGS+1.0,4.0);
	double gateFRelease = 1.0/(((pow(FGL,5)*500000.0)+500.0)*overallscale);
	double gateSRelease = 1.0/(((pow(SGL,5)*500000.0)+500.0)*overallscale);
	
	high[biqs_freq] = (((pow(TRF,3)*14500.0)+1500.0)/getSampleRate());
	if (high[biqs_freq] < 0.0001) high[biqs_freq] = 0.0001;
	high[biqs_nonlin] = TRG;
	high[biqs_level] = (high[biqs_nonlin]*2.0)-1.0;
	if (high[biqs_level] > 0.0) high[biqs_level] *= 2.0;
	high[biqs_reso] = ((0.5+(high[biqs_nonlin]*0.5)+sqrt(high[biqs_freq]))-(1.0-pow(1.0-TRR,2.0)))+0.5+(high[biqs_nonlin]*0.5);
	K = tan(M_PI * high[biqs_freq]);
	norm = 1.0 / (1.0 + K / (high[biqs_reso]*1.93185165) + K * K);
	high[biqs_a0] = K / (high[biqs_reso]*1.93185165) * norm;
	high[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_b2] = (1.0 - K / (high[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.70710678) + K * K);
	high[biqs_c0] = K / (high[biqs_reso]*0.70710678) * norm;
	high[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_d2] = (1.0 - K / (high[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.51763809) + K * K);
	high[biqs_e0] = K / (high[biqs_reso]*0.51763809) * norm;
	high[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_f2] = (1.0 - K / (high[biqs_reso]*0.51763809) + K * K) * norm;
	//high
	
	hmid[biqs_freq] = (((pow(HMF,3)*6400.0)+600.0)/getSampleRate());
	if (hmid[biqs_freq] < 0.0001) hmid[biqs_freq] = 0.0001;
	hmid[biqs_nonlin] = HMG;
	hmid[biqs_level] = (hmid[biqs_nonlin]*2.0)-1.0;
	if (hmid[biqs_level] > 0.0) hmid[biqs_level] *= 2.0;
	hmid[biqs_reso] = ((0.5+(hmid[biqs_nonlin]*0.5)+sqrt(hmid[biqs_freq]))-(1.0-pow(1.0-HMR,2.0)))+0.5+(hmid[biqs_nonlin]*0.5);
	K = tan(M_PI * hmid[biqs_freq]);
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*1.93185165) + K * K);
	hmid[biqs_a0] = K / (hmid[biqs_reso]*1.93185165) * norm;
	hmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_b2] = (1.0 - K / (hmid[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.70710678) + K * K);
	hmid[biqs_c0] = K / (hmid[biqs_reso]*0.70710678) * norm;
	hmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_d2] = (1.0 - K / (hmid[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.51763809) + K * K);
	hmid[biqs_e0] = K / (hmid[biqs_reso]*0.51763809) * norm;
	hmid[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_f2] = (1.0 - K / (hmid[biqs_reso]*0.51763809) + K * K) * norm;
	//hmid
	
	lmid[biqs_freq] = (((pow(LMF,3)*2200.0)+200.0)/getSampleRate());
	if (lmid[biqs_freq] < 0.0001) lmid[biqs_freq] = 0.0001;
	lmid[biqs_nonlin] = LMG;
	lmid[biqs_level] = (lmid[biqs_nonlin]*2.0)-1.0;
	if (lmid[biqs_level] > 0.0) lmid[biqs_level] *= 2.0;
	lmid[biqs_reso] = ((0.5+(lmid[biqs_nonlin]*0.5)+sqrt(lmid[biqs_freq]))-(1.0-pow(1.0-LMR,2.0)))+0.5+(lmid[biqs_nonlin]*0.5);
	K = tan(M_PI * lmid[biqs_freq]);
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*1.93185165) + K * K);
	lmid[biqs_a0] = K / (lmid[biqs_reso]*1.93185165) * norm;
	lmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_b2] = (1.0 - K / (lmid[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.70710678) + K * K);
	lmid[biqs_c0] = K / (lmid[biqs_reso]*0.70710678) * norm;
	lmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_d2] = (1.0 - K / (lmid[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.51763809) + K * K);
	lmid[biqs_e0] = K / (lmid[biqs_reso]*0.51763809) * norm;
	lmid[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_f2] = (1.0 - K / (lmid[biqs_reso]*0.51763809) + K * K) * norm;
	//lmid
	
	bass[biqs_freq] = (((pow(BSF,3)*570.0)+30.0)/getSampleRate());
	if (bass[biqs_freq] < 0.0001) bass[biqs_freq] = 0.0001;
	bass[biqs_nonlin] = BSG;
	bass[biqs_level] = (bass[biqs_nonlin]*2.0)-1.0;
	if (bass[biqs_level] > 0.0) bass[biqs_level] *= 2.0;
	bass[biqs_reso] = ((0.5+(bass[biqs_nonlin]*0.5)+sqrt(bass[biqs_freq]))-(1.0-pow(1.0-BSR,2.0)))+0.5+(bass[biqs_nonlin]*0.5);
	K = tan(M_PI * bass[biqs_freq]);
	norm = 1.0 / (1.0 + K / (bass[biqs_reso]*1.93185165) + K * K);
	bass[biqs_a0] = K / (bass[biqs_reso]*1.93185165) * norm;
	bass[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	bass[biqs_b2] = (1.0 - K / (bass[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (bass[biqs_reso]*0.70710678) + K * K);
	bass[biqs_c0] = K / (bass[biqs_reso]*0.70710678) * norm;
	bass[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	bass[biqs_d2] = (1.0 - K / (bass[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (bass[biqs_reso]*0.51763809) + K * K);
	bass[biqs_e0] = K / (bass[biqs_reso]*0.51763809) * norm;
	bass[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
	bass[biqs_f2] = (1.0 - K / (bass[biqs_reso]*0.51763809) + K * K) * norm;
	//bass
	
	double refdB = (DSC*70.0)+70.0;
	double topdB = 0.000000075 * pow(10.0,refdB/20.0) * overallscale;
	
	panA = panB; panB = PAN*1.57079633;
	inTrimA = inTrimB; inTrimB = FAD*2.0;
	
    while (--sampleFrames >= 0)
    {
		long double inputSampleL = *in1;
		long double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (highpassEngage) { //distributed Highpass
			highpass[hilp_temp] = (inputSampleL*highpass[hilp_a0])+highpass[hilp_aL1];
			highpass[hilp_aL1] = (inputSampleL*highpass[hilp_a1])-(highpass[hilp_temp]*highpass[hilp_b1])+highpass[hilp_aL2];
			highpass[hilp_aL2] = (inputSampleL*highpass[hilp_a0])-(highpass[hilp_temp]*highpass[hilp_b2]); inputSampleL = highpass[hilp_temp];
			highpass[hilp_temp] = (inputSampleR*highpass[hilp_a0])+highpass[hilp_aR1];
			highpass[hilp_aR1] = (inputSampleR*highpass[hilp_a1])-(highpass[hilp_temp]*highpass[hilp_b1])+highpass[hilp_aR2];
			highpass[hilp_aR2] = (inputSampleR*highpass[hilp_a0])-(highpass[hilp_temp]*highpass[hilp_b2]); inputSampleR = highpass[hilp_temp];
		} else highpass[hilp_aR1] = highpass[hilp_aR2] = highpass[hilp_aL1] = highpass[hilp_aL2] = 0.0;
		if (lowpassEngage) { //distributed Lowpass
			lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_a0])+lowpass[hilp_aL1];
			lowpass[hilp_aL1] = (inputSampleL*lowpass[hilp_a1])-(lowpass[hilp_temp]*lowpass[hilp_b1])+lowpass[hilp_aL2];
			lowpass[hilp_aL2] = (inputSampleL*lowpass[hilp_a0])-(lowpass[hilp_temp]*lowpass[hilp_b2]); inputSampleL = lowpass[hilp_temp];
			lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_a0])+lowpass[hilp_aR1];
			lowpass[hilp_aR1] = (inputSampleR*lowpass[hilp_a1])-(lowpass[hilp_temp]*lowpass[hilp_b1])+lowpass[hilp_aR2];
			lowpass[hilp_aR2] = (inputSampleR*lowpass[hilp_a0])-(lowpass[hilp_temp]*lowpass[hilp_b2]); inputSampleR = lowpass[hilp_temp];
		} else lowpass[hilp_aR1] = lowpass[hilp_aR2] = lowpass[hilp_aL1] = lowpass[hilp_aL2] = 0.0;
		//first Highpass/Lowpass blocks aliasing before the nonlinearity of ConsoleXBuss and Parametric
		
		//get all Parametric bands before any other processing is done
		//begin Stacked Biquad With Reversed Neutron Flow L
		high[biqs_outL] = inputSampleL * fabs(high[biqs_level]);
		high[biqs_dis] = fabs(high[biqs_a0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_aL1];
		high[biqs_aL1] = high[biqs_aL2] - (high[biqs_temp]*high[biqs_b1]);
		high[biqs_aL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_b2]);
		high[biqs_outL] = high[biqs_temp];
		high[biqs_dis] = fabs(high[biqs_c0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_cL1];
		high[biqs_cL1] = high[biqs_cL2] - (high[biqs_temp]*high[biqs_d1]);
		high[biqs_cL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_d2]);
		high[biqs_outL] = high[biqs_temp];
		high[biqs_dis] = fabs(high[biqs_e0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_eL1];
		high[biqs_eL1] = high[biqs_eL2] - (high[biqs_temp]*high[biqs_f1]);
		high[biqs_eL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_f2]);
		high[biqs_outL] = high[biqs_temp]; high[biqs_outL] *= high[biqs_level];
		if (high[biqs_level] > 1.0) high[biqs_outL] *= high[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		hmid[biqs_outL] = inputSampleL * fabs(hmid[biqs_level]);
		hmid[biqs_dis] = fabs(hmid[biqs_a0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_aL1];
		hmid[biqs_aL1] = hmid[biqs_aL2] - (hmid[biqs_temp]*hmid[biqs_b1]);
		hmid[biqs_aL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_b2]);
		hmid[biqs_outL] = hmid[biqs_temp];
		hmid[biqs_dis] = fabs(hmid[biqs_c0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_cL1];
		hmid[biqs_cL1] = hmid[biqs_cL2] - (hmid[biqs_temp]*hmid[biqs_d1]);
		hmid[biqs_cL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_d2]);
		hmid[biqs_outL] = hmid[biqs_temp];
		hmid[biqs_dis] = fabs(hmid[biqs_e0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_eL1];
		hmid[biqs_eL1] = hmid[biqs_eL2] - (hmid[biqs_temp]*hmid[biqs_f1]);
		hmid[biqs_eL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_f2]);
		hmid[biqs_outL] = hmid[biqs_temp]; hmid[biqs_outL] *= hmid[biqs_level];
		if (hmid[biqs_level] > 1.0) hmid[biqs_outL] *= hmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		lmid[biqs_outL] = inputSampleL * fabs(lmid[biqs_level]);
		lmid[biqs_dis] = fabs(lmid[biqs_a0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_aL1];
		lmid[biqs_aL1] = lmid[biqs_aL2] - (lmid[biqs_temp]*lmid[biqs_b1]);
		lmid[biqs_aL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_b2]);
		lmid[biqs_outL] = lmid[biqs_temp];
		lmid[biqs_dis] = fabs(lmid[biqs_c0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_cL1];
		lmid[biqs_cL1] = lmid[biqs_cL2] - (lmid[biqs_temp]*lmid[biqs_d1]);
		lmid[biqs_cL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_d2]);
		lmid[biqs_outL] = lmid[biqs_temp];
		lmid[biqs_dis] = fabs(lmid[biqs_e0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_eL1];
		lmid[biqs_eL1] = lmid[biqs_eL2] - (lmid[biqs_temp]*lmid[biqs_f1]);
		lmid[biqs_eL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_f2]);
		lmid[biqs_outL] = lmid[biqs_temp]; lmid[biqs_outL] *= lmid[biqs_level];
		if (lmid[biqs_level] > 1.0) lmid[biqs_outL] *= lmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		bass[biqs_outL] = inputSampleL * fabs(bass[biqs_level]);
		bass[biqs_dis] = fabs(bass[biqs_a0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_aL1];
		bass[biqs_aL1] = bass[biqs_aL2] - (bass[biqs_temp]*bass[biqs_b1]);
		bass[biqs_aL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_b2]);
		bass[biqs_outL] = bass[biqs_temp];
		bass[biqs_dis] = fabs(bass[biqs_c0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_cL1];
		bass[biqs_cL1] = bass[biqs_cL2] - (bass[biqs_temp]*bass[biqs_d1]);
		bass[biqs_cL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_d2]);
		bass[biqs_outL] = bass[biqs_temp];
		bass[biqs_dis] = fabs(bass[biqs_e0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_eL1];
		bass[biqs_eL1] = bass[biqs_eL2] - (bass[biqs_temp]*bass[biqs_f1]);
		bass[biqs_eL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_f2]);
		bass[biqs_outL] = bass[biqs_temp]; bass[biqs_outL] *= bass[biqs_level];
		if (bass[biqs_level] > 1.0) bass[biqs_outL] *= bass[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		high[biqs_outR] = inputSampleR * fabs(high[biqs_level]);
		high[biqs_dis] = fabs(high[biqs_a0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_aR1];
		high[biqs_aR1] = high[biqs_aR2] - (high[biqs_temp]*high[biqs_b1]);
		high[biqs_aR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_b2]);
		high[biqs_outR] = high[biqs_temp];
		high[biqs_dis] = fabs(high[biqs_c0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_cR1];
		high[biqs_cR1] = high[biqs_cR2] - (high[biqs_temp]*high[biqs_d1]);
		high[biqs_cR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_d2]);
		high[biqs_outR] = high[biqs_temp];
		high[biqs_dis] = fabs(high[biqs_e0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_eR1];
		high[biqs_eR1] = high[biqs_eR2] - (high[biqs_temp]*high[biqs_f1]);
		high[biqs_eR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_f2]);
		high[biqs_outR] = high[biqs_temp]; high[biqs_outR] *= high[biqs_level];
		if (high[biqs_level] > 1.0) high[biqs_outR] *= high[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		hmid[biqs_outR] = inputSampleR * fabs(hmid[biqs_level]);
		hmid[biqs_dis] = fabs(hmid[biqs_a0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_aR1];
		hmid[biqs_aR1] = hmid[biqs_aR2] - (hmid[biqs_temp]*hmid[biqs_b1]);
		hmid[biqs_aR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_b2]);
		hmid[biqs_outR] = hmid[biqs_temp];
		hmid[biqs_dis] = fabs(hmid[biqs_c0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_cR1];
		hmid[biqs_cR1] = hmid[biqs_cR2] - (hmid[biqs_temp]*hmid[biqs_d1]);
		hmid[biqs_cR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_d2]);
		hmid[biqs_outR] = hmid[biqs_temp];
		hmid[biqs_dis] = fabs(hmid[biqs_e0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_eR1];
		hmid[biqs_eR1] = hmid[biqs_eR2] - (hmid[biqs_temp]*hmid[biqs_f1]);
		hmid[biqs_eR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_f2]);
		hmid[biqs_outR] = hmid[biqs_temp]; hmid[biqs_outR] *= hmid[biqs_level];
		if (hmid[biqs_level] > 1.0) hmid[biqs_outR] *= hmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		lmid[biqs_outR] = inputSampleR * fabs(lmid[biqs_level]);
		lmid[biqs_dis] = fabs(lmid[biqs_a0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_aR1];
		lmid[biqs_aR1] = lmid[biqs_aR2] - (lmid[biqs_temp]*lmid[biqs_b1]);
		lmid[biqs_aR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_b2]);
		lmid[biqs_outR] = lmid[biqs_temp];
		lmid[biqs_dis] = fabs(lmid[biqs_c0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_cR1];
		lmid[biqs_cR1] = lmid[biqs_cR2] - (lmid[biqs_temp]*lmid[biqs_d1]);
		lmid[biqs_cR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_d2]);
		lmid[biqs_outR] = lmid[biqs_temp];
		lmid[biqs_dis] = fabs(lmid[biqs_e0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_eR1];
		lmid[biqs_eR1] = lmid[biqs_eR2] - (lmid[biqs_temp]*lmid[biqs_f1]);
		lmid[biqs_eR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_f2]);
		lmid[biqs_outR] = lmid[biqs_temp]; lmid[biqs_outR] *= lmid[biqs_level];
		if (lmid[biqs_level] > 1.0) lmid[biqs_outR] *= lmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		bass[biqs_outR] = inputSampleR * fabs(bass[biqs_level]);
		bass[biqs_dis] = fabs(bass[biqs_a0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_aR1];
		bass[biqs_aR1] = bass[biqs_aR2] - (bass[biqs_temp]*bass[biqs_b1]);
		bass[biqs_aR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_b2]);
		bass[biqs_outR] = bass[biqs_temp];
		bass[biqs_dis] = fabs(bass[biqs_c0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_cR1];
		bass[biqs_cR1] = bass[biqs_cR2] - (bass[biqs_temp]*bass[biqs_d1]);
		bass[biqs_cR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_d2]);
		bass[biqs_outR] = bass[biqs_temp];
		bass[biqs_dis] = fabs(bass[biqs_e0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_eR1];
		bass[biqs_eR1] = bass[biqs_eR2] - (bass[biqs_temp]*bass[biqs_f1]);
		bass[biqs_eR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_f2]);
		bass[biqs_outR] = bass[biqs_temp]; bass[biqs_outR] *= bass[biqs_level];
		if (bass[biqs_level] > 1.0) bass[biqs_outR] *= bass[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double gainR = (panA*temp)+(panB*(1.0-temp));
		double gainL = 1.57079633-gainR;
		gainR = sin(gainR); gainL = sin(gainL);
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		if (gain > 1.0) gain *= gain; else gain = 1.0-pow(1.0-gain,2);
		gain *= 0.763932022500211;
		double airGain = (airGainA*temp)+(airGainB*(1.0-temp));
		if (airGain > 1.0) airGain *= airGain; else airGain = 1.0-pow(1.0-airGain,2);
		double fireGain = (fireGainA*temp)+(fireGainB*(1.0-temp));
		if (fireGain > 1.0) fireGain *= fireGain; else fireGain = 1.0-pow(1.0-fireGain,2);
		double firePad = fireGain; if (firePad > 1.0) firePad = 1.0;
		double stoneGain = (stoneGainA*temp)+(stoneGainB*(1.0-temp));
		if (stoneGain > 1.0) stoneGain *= stoneGain; else stoneGain = 1.0-pow(1.0-stoneGain,2);
		double stonePad = stoneGain; if (stonePad > 1.0) stonePad = 1.0;
		//set up smoothed gain controls
		
		//begin Air3L
		double drySampleL = inputSampleL;
		air[pvSL4] = air[pvAL4] - air[pvAL3]; air[pvSL3] = air[pvAL3] - air[pvAL2];
		air[pvSL2] = air[pvAL2] - air[pvAL1]; air[pvSL1] = air[pvAL1] - inputSampleL;
		air[accSL3] = air[pvSL4] - air[pvSL3]; air[accSL2] = air[pvSL3] - air[pvSL2];
		air[accSL1] = air[pvSL2] - air[pvSL1];
		air[acc2SL2] = air[accSL3] - air[accSL2]; air[acc2SL1] = air[accSL2] - air[accSL1];		
		air[outAL] = -(air[pvAL1] + air[pvSL3] + air[acc2SL2] - ((air[acc2SL2] + air[acc2SL1])*0.5));
		air[gainAL] *= 0.5; air[gainAL] += fabs(drySampleL-air[outAL])*0.5;
		if (air[gainAL] > 0.3*sqrt(overallscale)) air[gainAL] = 0.3*sqrt(overallscale);
		air[pvAL4] = air[pvAL3]; air[pvAL3] = air[pvAL2];
		air[pvAL2] = air[pvAL1]; air[pvAL1] = (air[gainAL] * air[outAL]) + drySampleL;
		double fireL = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
		temp = (fireL + air[gndavgL])*0.5; air[gndavgL] = fireL; fireL = temp;
		double airL = (drySampleL-fireL)*airGain;
		inputSampleL = fireL;
		//end Air3L
		//begin Air3R
		double drySampleR = inputSampleR;
		air[pvSR4] = air[pvAR4] - air[pvAR3]; air[pvSR3] = air[pvAR3] - air[pvAR2];
		air[pvSR2] = air[pvAR2] - air[pvAR1]; air[pvSR1] = air[pvAR1] - inputSampleR;
		air[accSR3] = air[pvSR4] - air[pvSR3]; air[accSR2] = air[pvSR3] - air[pvSR2];
		air[accSR1] = air[pvSR2] - air[pvSR1];
		air[acc2SR2] = air[accSR3] - air[accSR2]; air[acc2SR1] = air[accSR2] - air[accSR1];		
		air[outAR] = -(air[pvAR1] + air[pvSR3] + air[acc2SR2] - ((air[acc2SR2] + air[acc2SR1])*0.5));
		air[gainAR] *= 0.5; air[gainAR] += fabs(drySampleR-air[outAR])*0.5;
		if (air[gainAR] > 0.3*sqrt(overallscale)) air[gainAR] = 0.3*sqrt(overallscale);
		air[pvAR4] = air[pvAR3]; air[pvAR3] = air[pvAR2];
		air[pvAR2] = air[pvAR1]; air[pvAR1] = (air[gainAR] * air[outAR]) + drySampleR;
		double fireR = drySampleR - ((air[outAR]*0.5)+(drySampleR*(0.457-(0.017*overallscale))));
		temp = (fireR + air[gndavgR])*0.5; air[gndavgR] = fireR; fireR = temp;
		double airR = (drySampleR-fireR)*airGain;
		inputSampleR = fireR;
		//end Air3R
		//begin KalmanL
		temp = inputSampleL = inputSampleL*(1.0-kalmanRange)*0.777;
		inputSampleL *= (1.0-kalmanRange);
		//set up gain levels to control the beast
		kal[prevSlewL3] += kal[prevSampL3] - kal[prevSampL2]; kal[prevSlewL3] *= 0.5;
		kal[prevSlewL2] += kal[prevSampL2] - kal[prevSampL1]; kal[prevSlewL2] *= 0.5;
		kal[prevSlewL1] += kal[prevSampL1] - inputSampleL; kal[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kal[accSlewL2] += kal[prevSlewL3] - kal[prevSlewL2]; kal[accSlewL2] *= 0.5;
		kal[accSlewL1] += kal[prevSlewL2] - kal[prevSlewL1]; kal[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kal[accSlewL3] += (kal[accSlewL2] - kal[accSlewL1]); kal[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kal[kalOutL] += kal[prevSampL1] + kal[prevSlewL2] + kal[accSlewL3]; kal[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kal[kalGainL] += fabs(temp-kal[kalOutL])*kalmanRange*8.0; kal[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kal[kalGainL] > kalmanRange*0.5) kal[kalGainL] = kalmanRange*0.5;
		//attempts to avoid explosions
		kal[kalOutL] += (temp*(1.0-(0.68+(kalmanRange*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kal[prevSampL3] = kal[prevSampL2]; kal[prevSampL2] = kal[prevSampL1];
		kal[prevSampL1] = (kal[kalGainL] * kal[kalOutL]) + ((1.0-kal[kalGainL])*temp);
		//feed the chain of previous samples
		if (kal[prevSampL1] > 1.0) kal[prevSampL1] = 1.0; if (kal[prevSampL1] < -1.0) kal[prevSampL1] = -1.0;
		double stoneL = kal[kalOutL]*0.777;
		fireL -= stoneL;
		//end KalmanL
		//begin KalmanR
		temp = inputSampleR = inputSampleR*(1.0-kalmanRange)*0.777;
		inputSampleR *= (1.0-kalmanRange);
		//set up gain levels to control the beast
		kal[prevSlewR3] += kal[prevSampR3] - kal[prevSampR2]; kal[prevSlewR3] *= 0.5;
		kal[prevSlewR2] += kal[prevSampR2] - kal[prevSampR1]; kal[prevSlewR2] *= 0.5;
		kal[prevSlewR1] += kal[prevSampR1] - inputSampleR; kal[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kal[accSlewR2] += kal[prevSlewR3] - kal[prevSlewR2]; kal[accSlewR2] *= 0.5;
		kal[accSlewR1] += kal[prevSlewR2] - kal[prevSlewR1]; kal[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kal[accSlewR3] += (kal[accSlewR2] - kal[accSlewR1]); kal[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kal[kalOutR] += kal[prevSampR1] + kal[prevSlewR2] + kal[accSlewR3]; kal[kalOutR] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kal[kalGainR] += fabs(temp-kal[kalOutR])*kalmanRange*8.0; kal[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kal[kalGainR] > kalmanRange*0.5) kal[kalGainR] = kalmanRange*0.5;
		//attempts to avoid explosions
		kal[kalOutR] += (temp*(1.0-(0.68+(kalmanRange*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kal[prevSampR3] = kal[prevSampR2]; kal[prevSampR2] = kal[prevSampR1];
		kal[prevSampR1] = (kal[kalGainR] * kal[kalOutR]) + ((1.0-kal[kalGainR])*temp);
		//feed the chain of previous samples
		if (kal[prevSampR1] > 1.0) kal[prevSampR1] = 1.0; if (kal[prevSampR1] < -1.0) kal[prevSampR1] = -1.0;
		double stoneR = kal[kalOutR]*0.777;
		fireR -= stoneR;
		//end KalmanR
		//fire dynamics
		if (fabs(fireL) > compFThresh) { //compression L
			fireCompL -= (fireCompL * compFAttack);
			fireCompL += ((compFThresh / fabs(fireL))*compFAttack);
		} else fireCompL = (fireCompL*(1.0-compFRelease))+compFRelease;
		if (fabs(fireR) > compFThresh) { //compression R
			fireCompR -= (fireCompR * compFAttack);
			fireCompR += ((compFThresh / fabs(fireR))*compFAttack);
		} else fireCompR = (fireCompR*(1.0-compFRelease))+compFRelease;
		if (fireCompL > fireCompR) fireCompL -= (fireCompL * compFAttack);
		if (fireCompR > fireCompL) fireCompR -= (fireCompR * compFAttack);
		if (fabs(fireL) > gateFThresh) fireGate = gateFSustain;
		else if (fabs(fireR) > gateFThresh) fireGate = gateFSustain;
		else fireGate *= (1.0-gateFRelease);
		if (fireGate < 0.0) fireGate = 0.0;
		fireCompL = fmax(fmin(fireCompL,1.0),0.0);
		fireCompR = fmax(fmin(fireCompR,1.0),0.0);
		fireL *= (((1.0-compFRatio)*firePad)+(fireCompL*compFRatio*fireGain));
		fireR *= (((1.0-compFRatio)*firePad)+(fireCompR*compFRatio*fireGain));
		if (fireGate < M_PI_2) {
			temp = ((1.0-gateFRatio)+(sin(fireGate)*gateFRatio));
			airL *= temp;
			airR *= temp;
			fireL *= temp;
			fireR *= temp;
			high[biqs_outL] *= temp;
			high[biqs_outR] *= temp;
			hmid[biqs_outL] *= temp; //if Fire gating, gate Air, high and hmid
			hmid[biqs_outR] *= temp; //note that we aren't compressing these
		}
		//stone dynamics
		if (fabs(stoneL) > compSThresh) { //compression L
			stoneCompL -= (stoneCompL * compSAttack);
			stoneCompL += ((compSThresh / fabs(stoneL))*compSAttack);
		} else stoneCompL = (stoneCompL*(1.0-compSRelease))+compSRelease;
		if (fabs(stoneR) > compSThresh) { //compression R
			stoneCompR -= (stoneCompR * compSAttack);
			stoneCompR += ((compSThresh / fabs(stoneR))*compSAttack);
		} else stoneCompR = (stoneCompR*(1.0-compSRelease))+compSRelease;
		if (stoneCompL > stoneCompR) stoneCompL -= (stoneCompL * compSAttack);
		if (stoneCompR > stoneCompL) stoneCompR -= (stoneCompR * compSAttack);
		if (fabs(stoneL) > gateSThresh) stoneGate = gateSSustain;
		else if (fabs(stoneR) > gateSThresh) stoneGate = gateSSustain;
		else stoneGate *= (1.0-gateSRelease);
		if (stoneGate < 0.0) stoneGate = 0.0;
		stoneCompL = fmax(fmin(stoneCompL,1.0),0.0);
		stoneCompR = fmax(fmin(stoneCompR,1.0),0.0);
		stoneL *= (((1.0-compSRatio)*stonePad)+(stoneCompL*compSRatio*stoneGain));
		stoneR *= (((1.0-compSRatio)*stonePad)+(stoneCompR*compSRatio*stoneGain));
		if (stoneGate < M_PI_2) {
			temp = ((1.0-gateSRatio)+(sin(stoneGate)*gateSRatio));
			stoneL *= temp;
			stoneR *= temp;
			lmid[biqs_outL] *= temp;
			lmid[biqs_outR] *= temp;
			bass[biqs_outL] *= temp; //if Stone gating, gate lmid and bass
			bass[biqs_outR] *= temp; //note that we aren't compressing these
		}
		inputSampleL = stoneL + fireL + airL;
		inputSampleR = stoneR + fireR + airR;
		//create Stonefire output
		
		if (highpassEngage) { //distributed Highpass
			highpass[hilp_temp] = (inputSampleL*highpass[hilp_c0])+highpass[hilp_cL1];
			highpass[hilp_cL1] = (inputSampleL*highpass[hilp_c1])-(highpass[hilp_temp]*highpass[hilp_d1])+highpass[hilp_cL2];
			highpass[hilp_cL2] = (inputSampleL*highpass[hilp_c0])-(highpass[hilp_temp]*highpass[hilp_d2]); inputSampleL = highpass[hilp_temp];
			highpass[hilp_temp] = (inputSampleR*highpass[hilp_c0])+highpass[hilp_cR1];
			highpass[hilp_cR1] = (inputSampleR*highpass[hilp_c1])-(highpass[hilp_temp]*highpass[hilp_d1])+highpass[hilp_cR2];
			highpass[hilp_cR2] = (inputSampleR*highpass[hilp_c0])-(highpass[hilp_temp]*highpass[hilp_d2]); inputSampleR = highpass[hilp_temp];
		} else highpass[hilp_cR1] = highpass[hilp_cR2] = highpass[hilp_cL1] = highpass[hilp_cL2] = 0.0;
		if (lowpassEngage) { //distributed Lowpass
			lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_c0])+lowpass[hilp_cL1];
			lowpass[hilp_cL1] = (inputSampleL*lowpass[hilp_c1])-(lowpass[hilp_temp]*lowpass[hilp_d1])+lowpass[hilp_cL2];
			lowpass[hilp_cL2] = (inputSampleL*lowpass[hilp_c0])-(lowpass[hilp_temp]*lowpass[hilp_d2]); inputSampleL = lowpass[hilp_temp];
			lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_c0])+lowpass[hilp_cR1];
			lowpass[hilp_cR1] = (inputSampleR*lowpass[hilp_c1])-(lowpass[hilp_temp]*lowpass[hilp_d1])+lowpass[hilp_cR2];
			lowpass[hilp_cR2] = (inputSampleR*lowpass[hilp_c0])-(lowpass[hilp_temp]*lowpass[hilp_d2]); inputSampleR = lowpass[hilp_temp];
		} else lowpass[hilp_cR1] = lowpass[hilp_cR2] = lowpass[hilp_cL1] = lowpass[hilp_cL2] = 0.0;
		//another stage of Highpass/Lowpass before bringing in the parametric bands
		
		inputSampleL += (high[biqs_outL] + hmid[biqs_outL] + lmid[biqs_outL] + bass[biqs_outL]);
		inputSampleR += (high[biqs_outR] + hmid[biqs_outR] + lmid[biqs_outR] + bass[biqs_outR]);
		//add parametric boosts or cuts: clean as possible for maximal rawness and sonority		
		
		inputSampleL = inputSampleL * gainL * gain;
		inputSampleR = inputSampleR * gainR * gain;
		//applies pan section, and smoothed fader gain
		
		inputSampleL *= topdB;
		if (inputSampleL < -0.222) inputSampleL = -0.222; if (inputSampleL > 0.222) inputSampleL = 0.222;
		dBaL[dBaXL] = inputSampleL; dBaPosL *= 0.5; dBaPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
		int dBdly = floor(dBaPosL*dscBuf);
		double dBi = (dBaPosL*dscBuf)-dBdly;
		inputSampleL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
		inputSampleL /= topdB;		
		inputSampleR *= topdB;
		if (inputSampleR < -0.222) inputSampleR = -0.222; if (inputSampleR > 0.222) inputSampleR = 0.222;
		dBaR[dBaXR] = inputSampleR; dBaPosR *= 0.5; dBaPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBdly = floor(dBaPosR*dscBuf);
		dBi = (dBaPosR*dscBuf)-dBdly;
		inputSampleR = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
		inputSampleR /= topdB;		
		//top dB processing for distributed discontinuity modeling air nonlinearity		
				
		if (highpassEngage) { //distributed Highpass
			highpass[hilp_temp] = (inputSampleL*highpass[hilp_e0])+highpass[hilp_eL1];
			highpass[hilp_eL1] = (inputSampleL*highpass[hilp_e1])-(highpass[hilp_temp]*highpass[hilp_f1])+highpass[hilp_eL2];
			highpass[hilp_eL2] = (inputSampleL*highpass[hilp_e0])-(highpass[hilp_temp]*highpass[hilp_f2]); inputSampleL = highpass[hilp_temp];
			highpass[hilp_temp] = (inputSampleR*highpass[hilp_e0])+highpass[hilp_eR1];
			highpass[hilp_eR1] = (inputSampleR*highpass[hilp_e1])-(highpass[hilp_temp]*highpass[hilp_f1])+highpass[hilp_eR2];
			highpass[hilp_eR2] = (inputSampleR*highpass[hilp_e0])-(highpass[hilp_temp]*highpass[hilp_f2]); inputSampleR = highpass[hilp_temp];
		} else highpass[hilp_eR1] = highpass[hilp_eR2] = highpass[hilp_eL1] = highpass[hilp_eL2] = 0.0;
		if (lowpassEngage) { //distributed Lowpass
			lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_e0])+lowpass[hilp_eL1];
			lowpass[hilp_eL1] = (inputSampleL*lowpass[hilp_e1])-(lowpass[hilp_temp]*lowpass[hilp_f1])+lowpass[hilp_eL2];
			lowpass[hilp_eL2] = (inputSampleL*lowpass[hilp_e0])-(lowpass[hilp_temp]*lowpass[hilp_f2]); inputSampleL = lowpass[hilp_temp];
			lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_e0])+lowpass[hilp_eR1];
			lowpass[hilp_eR1] = (inputSampleR*lowpass[hilp_e1])-(lowpass[hilp_temp]*lowpass[hilp_f1])+lowpass[hilp_eR2];
			lowpass[hilp_eR2] = (inputSampleR*lowpass[hilp_e0])-(lowpass[hilp_temp]*lowpass[hilp_f2]); inputSampleR = lowpass[hilp_temp];
		} else lowpass[hilp_eR1] = lowpass[hilp_eR2] = lowpass[hilp_eL1] = lowpass[hilp_eL2] = 0.0;		
		//final Highpass/Lowpass continues to address aliasing
		//final stacked biquad section is the softest Q for smoothness
		
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

void ConsoleXPre::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	highpass[hilp_freq] = ((HIP*330.0)+20.0)/getSampleRate();
	bool highpassEngage = true; if (HIP == 0.0) highpassEngage = false;
	
	lowpass[hilp_freq] = ((pow(1.0-LOP,2)*17000.0)+3000.0)/getSampleRate();
	bool lowpassEngage = true; if (LOP == 0.0) lowpassEngage = false;
	
	double K = tan(M_PI * highpass[hilp_freq]); //highpass
	double norm = 1.0 / (1.0 + K / 1.93185165 + K * K);
	highpass[hilp_a0] = norm;
	highpass[hilp_a1] = -2.0 * highpass[hilp_a0];
	highpass[hilp_b1] = 2.0 * (K * K - 1.0) * norm;
	highpass[hilp_b2] = (1.0 - K / 1.93185165 + K * K) * norm;
	norm = 1.0 / (1.0 + K / 0.70710678 + K * K);
	highpass[hilp_c0] = norm;
	highpass[hilp_c1] = -2.0 * highpass[hilp_c0];
	highpass[hilp_d1] = 2.0 * (K * K - 1.0) * norm;
	highpass[hilp_d2] = (1.0 - K / 0.70710678 + K * K) * norm;
	norm = 1.0 / (1.0 + K / 0.51763809 + K * K);
	highpass[hilp_e0] = norm;
	highpass[hilp_e1] = -2.0 * highpass[hilp_e0];
	highpass[hilp_f1] = 2.0 * (K * K - 1.0) * norm;
	highpass[hilp_f2] = (1.0 - K / 0.51763809 + K * K) * norm;
	
	K = tan(M_PI * lowpass[hilp_freq]); //lowpass
	norm = 1.0 / (1.0 + K / 1.93185165 + K * K);
	lowpass[hilp_a0] = K * K * norm;
	lowpass[hilp_a1] = 2.0 * lowpass[hilp_a0];
	lowpass[hilp_b1] = 2.0 * (K * K - 1.0) * norm;
	lowpass[hilp_b2] = (1.0 - K / 1.93185165 + K * K) * norm;
	norm = 1.0 / (1.0 + K / 0.70710678 + K * K);
	lowpass[hilp_c0] = K * K * norm;
	lowpass[hilp_c1] = 2.0 * lowpass[hilp_c0];
	lowpass[hilp_d1] = 2.0 * (K * K - 1.0) * norm;
	lowpass[hilp_d2] = (1.0 - K / 0.70710678 + K * K) * norm;
	norm = 1.0 / (1.0 + K / 0.51763809 + K * K);
	lowpass[hilp_e0] = K * K * norm;
	lowpass[hilp_e1] = 2.0 * lowpass[hilp_e0];
	lowpass[hilp_f1] = 2.0 * (K * K - 1.0) * norm;
	lowpass[hilp_f2] = (1.0 - K / 0.51763809 + K * K) * norm;
	
	airGainA = airGainB; airGainB = AIR *2.0;
	fireGainA = fireGainB; fireGainB = FIR *2.0;
	stoneGainA = stoneGainB; stoneGainB = STO *2.0;
	//simple three band to adjust
	double kalmanRange = 1.0-(pow(RNG,2)/overallscale);
	//crossover frequency between mid/bass
	
	double compFThresh = pow(FCT,4);
	double compSThresh = pow(SCT,4);
	double compFRatio = 1.0-pow(1.0-FCR,2);
	double compSRatio = 1.0-pow(1.0-SCR,2);
	double compFAttack = 1.0/(((pow(FCA,3)*5000.0)+500.0)*overallscale);
	double compSAttack = 1.0/(((pow(SCA,3)*5000.0)+500.0)*overallscale);
	double compFRelease = 1.0/(((pow(FCL,5)*50000.0)+500.0)*overallscale);
	double compSRelease = 1.0/(((pow(SCL,5)*50000.0)+500.0)*overallscale);
	double gateFThresh = pow(FGT,4);
	double gateSThresh = pow(SGT,4);
	double gateFRatio = 1.0-pow(1.0-FGR,2);
	double gateSRatio = 1.0-pow(1.0-SGR,2);
	double gateFSustain = M_PI_2 * pow(FGS+1.0,4.0);
	double gateSSustain = M_PI_2 * pow(SGS+1.0,4.0);
	double gateFRelease = 1.0/(((pow(FGL,5)*500000.0)+500.0)*overallscale);
	double gateSRelease = 1.0/(((pow(SGL,5)*500000.0)+500.0)*overallscale);
	
	high[biqs_freq] = (((pow(TRF,3)*14500.0)+1500.0)/getSampleRate());
	if (high[biqs_freq] < 0.0001) high[biqs_freq] = 0.0001;
	high[biqs_nonlin] = TRG;
	high[biqs_level] = (high[biqs_nonlin]*2.0)-1.0;
	if (high[biqs_level] > 0.0) high[biqs_level] *= 2.0;
	high[biqs_reso] = ((0.5+(high[biqs_nonlin]*0.5)+sqrt(high[biqs_freq]))-(1.0-pow(1.0-TRR,2.0)))+0.5+(high[biqs_nonlin]*0.5);
	K = tan(M_PI * high[biqs_freq]);
	norm = 1.0 / (1.0 + K / (high[biqs_reso]*1.93185165) + K * K);
	high[biqs_a0] = K / (high[biqs_reso]*1.93185165) * norm;
	high[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_b2] = (1.0 - K / (high[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.70710678) + K * K);
	high[biqs_c0] = K / (high[biqs_reso]*0.70710678) * norm;
	high[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_d2] = (1.0 - K / (high[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (high[biqs_reso]*0.51763809) + K * K);
	high[biqs_e0] = K / (high[biqs_reso]*0.51763809) * norm;
	high[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
	high[biqs_f2] = (1.0 - K / (high[biqs_reso]*0.51763809) + K * K) * norm;
	//high
	
	hmid[biqs_freq] = (((pow(HMF,3)*6400.0)+600.0)/getSampleRate());
	if (hmid[biqs_freq] < 0.0001) hmid[biqs_freq] = 0.0001;
	hmid[biqs_nonlin] = HMG;
	hmid[biqs_level] = (hmid[biqs_nonlin]*2.0)-1.0;
	if (hmid[biqs_level] > 0.0) hmid[biqs_level] *= 2.0;
	hmid[biqs_reso] = ((0.5+(hmid[biqs_nonlin]*0.5)+sqrt(hmid[biqs_freq]))-(1.0-pow(1.0-HMR,2.0)))+0.5+(hmid[biqs_nonlin]*0.5);
	K = tan(M_PI * hmid[biqs_freq]);
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*1.93185165) + K * K);
	hmid[biqs_a0] = K / (hmid[biqs_reso]*1.93185165) * norm;
	hmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_b2] = (1.0 - K / (hmid[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.70710678) + K * K);
	hmid[biqs_c0] = K / (hmid[biqs_reso]*0.70710678) * norm;
	hmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_d2] = (1.0 - K / (hmid[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (hmid[biqs_reso]*0.51763809) + K * K);
	hmid[biqs_e0] = K / (hmid[biqs_reso]*0.51763809) * norm;
	hmid[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
	hmid[biqs_f2] = (1.0 - K / (hmid[biqs_reso]*0.51763809) + K * K) * norm;
	//hmid
	
	lmid[biqs_freq] = (((pow(LMF,3)*2200.0)+200.0)/getSampleRate());
	if (lmid[biqs_freq] < 0.0001) lmid[biqs_freq] = 0.0001;
	lmid[biqs_nonlin] = LMG;
	lmid[biqs_level] = (lmid[biqs_nonlin]*2.0)-1.0;
	if (lmid[biqs_level] > 0.0) lmid[biqs_level] *= 2.0;
	lmid[biqs_reso] = ((0.5+(lmid[biqs_nonlin]*0.5)+sqrt(lmid[biqs_freq]))-(1.0-pow(1.0-LMR,2.0)))+0.5+(lmid[biqs_nonlin]*0.5);
	K = tan(M_PI * lmid[biqs_freq]);
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*1.93185165) + K * K);
	lmid[biqs_a0] = K / (lmid[biqs_reso]*1.93185165) * norm;
	lmid[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_b2] = (1.0 - K / (lmid[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.70710678) + K * K);
	lmid[biqs_c0] = K / (lmid[biqs_reso]*0.70710678) * norm;
	lmid[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_d2] = (1.0 - K / (lmid[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (lmid[biqs_reso]*0.51763809) + K * K);
	lmid[biqs_e0] = K / (lmid[biqs_reso]*0.51763809) * norm;
	lmid[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
	lmid[biqs_f2] = (1.0 - K / (lmid[biqs_reso]*0.51763809) + K * K) * norm;
	//lmid
	
	bass[biqs_freq] = (((pow(BSF,3)*570.0)+30.0)/getSampleRate());
	if (bass[biqs_freq] < 0.0001) bass[biqs_freq] = 0.0001;
	bass[biqs_nonlin] = BSG;
	bass[biqs_level] = (bass[biqs_nonlin]*2.0)-1.0;
	if (bass[biqs_level] > 0.0) bass[biqs_level] *= 2.0;
	bass[biqs_reso] = ((0.5+(bass[biqs_nonlin]*0.5)+sqrt(bass[biqs_freq]))-(1.0-pow(1.0-BSR,2.0)))+0.5+(bass[biqs_nonlin]*0.5);
	K = tan(M_PI * bass[biqs_freq]);
	norm = 1.0 / (1.0 + K / (bass[biqs_reso]*1.93185165) + K * K);
	bass[biqs_a0] = K / (bass[biqs_reso]*1.93185165) * norm;
	bass[biqs_b1] = 2.0 * (K * K - 1.0) * norm;
	bass[biqs_b2] = (1.0 - K / (bass[biqs_reso]*1.93185165) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (bass[biqs_reso]*0.70710678) + K * K);
	bass[biqs_c0] = K / (bass[biqs_reso]*0.70710678) * norm;
	bass[biqs_d1] = 2.0 * (K * K - 1.0) * norm;
	bass[biqs_d2] = (1.0 - K / (bass[biqs_reso]*0.70710678) + K * K) * norm;
	norm = 1.0 / (1.0 + K / (bass[biqs_reso]*0.51763809) + K * K);
	bass[biqs_e0] = K / (bass[biqs_reso]*0.51763809) * norm;
	bass[biqs_f1] = 2.0 * (K * K - 1.0) * norm;
	bass[biqs_f2] = (1.0 - K / (bass[biqs_reso]*0.51763809) + K * K) * norm;
	//bass
	
	double refdB = (DSC*70.0)+70.0;
	double topdB = 0.000000075 * pow(10.0,refdB/20.0) * overallscale;
	
	panA = panB; panB = PAN*1.57079633;
	inTrimA = inTrimB; inTrimB = FAD*2.0;
	
    while (--sampleFrames >= 0)
    {
		long double inputSampleL = *in1;
		long double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		if (highpassEngage) { //distributed Highpass
			highpass[hilp_temp] = (inputSampleL*highpass[hilp_a0])+highpass[hilp_aL1];
			highpass[hilp_aL1] = (inputSampleL*highpass[hilp_a1])-(highpass[hilp_temp]*highpass[hilp_b1])+highpass[hilp_aL2];
			highpass[hilp_aL2] = (inputSampleL*highpass[hilp_a0])-(highpass[hilp_temp]*highpass[hilp_b2]); inputSampleL = highpass[hilp_temp];
			highpass[hilp_temp] = (inputSampleR*highpass[hilp_a0])+highpass[hilp_aR1];
			highpass[hilp_aR1] = (inputSampleR*highpass[hilp_a1])-(highpass[hilp_temp]*highpass[hilp_b1])+highpass[hilp_aR2];
			highpass[hilp_aR2] = (inputSampleR*highpass[hilp_a0])-(highpass[hilp_temp]*highpass[hilp_b2]); inputSampleR = highpass[hilp_temp];
		} else highpass[hilp_aR1] = highpass[hilp_aR2] = highpass[hilp_aL1] = highpass[hilp_aL2] = 0.0;
		if (lowpassEngage) { //distributed Lowpass
			lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_a0])+lowpass[hilp_aL1];
			lowpass[hilp_aL1] = (inputSampleL*lowpass[hilp_a1])-(lowpass[hilp_temp]*lowpass[hilp_b1])+lowpass[hilp_aL2];
			lowpass[hilp_aL2] = (inputSampleL*lowpass[hilp_a0])-(lowpass[hilp_temp]*lowpass[hilp_b2]); inputSampleL = lowpass[hilp_temp];
			lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_a0])+lowpass[hilp_aR1];
			lowpass[hilp_aR1] = (inputSampleR*lowpass[hilp_a1])-(lowpass[hilp_temp]*lowpass[hilp_b1])+lowpass[hilp_aR2];
			lowpass[hilp_aR2] = (inputSampleR*lowpass[hilp_a0])-(lowpass[hilp_temp]*lowpass[hilp_b2]); inputSampleR = lowpass[hilp_temp];
		} else lowpass[hilp_aR1] = lowpass[hilp_aR2] = lowpass[hilp_aL1] = lowpass[hilp_aL2] = 0.0;
		//first Highpass/Lowpass blocks aliasing before the nonlinearity of ConsoleXBuss and Parametric
		
		//get all Parametric bands before any other processing is done
		//begin Stacked Biquad With Reversed Neutron Flow L
		high[biqs_outL] = inputSampleL * fabs(high[biqs_level]);
		high[biqs_dis] = fabs(high[biqs_a0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_aL1];
		high[biqs_aL1] = high[biqs_aL2] - (high[biqs_temp]*high[biqs_b1]);
		high[biqs_aL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_b2]);
		high[biqs_outL] = high[biqs_temp];
		high[biqs_dis] = fabs(high[biqs_c0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_cL1];
		high[biqs_cL1] = high[biqs_cL2] - (high[biqs_temp]*high[biqs_d1]);
		high[biqs_cL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_d2]);
		high[biqs_outL] = high[biqs_temp];
		high[biqs_dis] = fabs(high[biqs_e0] * (1.0+(high[biqs_outL]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outL] * high[biqs_dis]) + high[biqs_eL1];
		high[biqs_eL1] = high[biqs_eL2] - (high[biqs_temp]*high[biqs_f1]);
		high[biqs_eL2] = (high[biqs_outL] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_f2]);
		high[biqs_outL] = high[biqs_temp]; high[biqs_outL] *= high[biqs_level];
		if (high[biqs_level] > 1.0) high[biqs_outL] *= high[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		hmid[biqs_outL] = inputSampleL * fabs(hmid[biqs_level]);
		hmid[biqs_dis] = fabs(hmid[biqs_a0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_aL1];
		hmid[biqs_aL1] = hmid[biqs_aL2] - (hmid[biqs_temp]*hmid[biqs_b1]);
		hmid[biqs_aL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_b2]);
		hmid[biqs_outL] = hmid[biqs_temp];
		hmid[biqs_dis] = fabs(hmid[biqs_c0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_cL1];
		hmid[biqs_cL1] = hmid[biqs_cL2] - (hmid[biqs_temp]*hmid[biqs_d1]);
		hmid[biqs_cL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_d2]);
		hmid[biqs_outL] = hmid[biqs_temp];
		hmid[biqs_dis] = fabs(hmid[biqs_e0] * (1.0+(hmid[biqs_outL]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outL] * hmid[biqs_dis]) + hmid[biqs_eL1];
		hmid[biqs_eL1] = hmid[biqs_eL2] - (hmid[biqs_temp]*hmid[biqs_f1]);
		hmid[biqs_eL2] = (hmid[biqs_outL] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_f2]);
		hmid[biqs_outL] = hmid[biqs_temp]; hmid[biqs_outL] *= hmid[biqs_level];
		if (hmid[biqs_level] > 1.0) hmid[biqs_outL] *= hmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		lmid[biqs_outL] = inputSampleL * fabs(lmid[biqs_level]);
		lmid[biqs_dis] = fabs(lmid[biqs_a0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_aL1];
		lmid[biqs_aL1] = lmid[biqs_aL2] - (lmid[biqs_temp]*lmid[biqs_b1]);
		lmid[biqs_aL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_b2]);
		lmid[biqs_outL] = lmid[biqs_temp];
		lmid[biqs_dis] = fabs(lmid[biqs_c0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_cL1];
		lmid[biqs_cL1] = lmid[biqs_cL2] - (lmid[biqs_temp]*lmid[biqs_d1]);
		lmid[biqs_cL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_d2]);
		lmid[biqs_outL] = lmid[biqs_temp];
		lmid[biqs_dis] = fabs(lmid[biqs_e0] * (1.0+(lmid[biqs_outL]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outL] * lmid[biqs_dis]) + lmid[biqs_eL1];
		lmid[biqs_eL1] = lmid[biqs_eL2] - (lmid[biqs_temp]*lmid[biqs_f1]);
		lmid[biqs_eL2] = (lmid[biqs_outL] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_f2]);
		lmid[biqs_outL] = lmid[biqs_temp]; lmid[biqs_outL] *= lmid[biqs_level];
		if (lmid[biqs_level] > 1.0) lmid[biqs_outL] *= lmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow L
		bass[biqs_outL] = inputSampleL * fabs(bass[biqs_level]);
		bass[biqs_dis] = fabs(bass[biqs_a0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_aL1];
		bass[biqs_aL1] = bass[biqs_aL2] - (bass[biqs_temp]*bass[biqs_b1]);
		bass[biqs_aL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_b2]);
		bass[biqs_outL] = bass[biqs_temp];
		bass[biqs_dis] = fabs(bass[biqs_c0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_cL1];
		bass[biqs_cL1] = bass[biqs_cL2] - (bass[biqs_temp]*bass[biqs_d1]);
		bass[biqs_cL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_d2]);
		bass[biqs_outL] = bass[biqs_temp];
		bass[biqs_dis] = fabs(bass[biqs_e0] * (1.0+(bass[biqs_outL]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outL] * bass[biqs_dis]) + bass[biqs_eL1];
		bass[biqs_eL1] = bass[biqs_eL2] - (bass[biqs_temp]*bass[biqs_f1]);
		bass[biqs_eL2] = (bass[biqs_outL] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_f2]);
		bass[biqs_outL] = bass[biqs_temp]; bass[biqs_outL] *= bass[biqs_level];
		if (bass[biqs_level] > 1.0) bass[biqs_outL] *= bass[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow L
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		high[biqs_outR] = inputSampleR * fabs(high[biqs_level]);
		high[biqs_dis] = fabs(high[biqs_a0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_aR1];
		high[biqs_aR1] = high[biqs_aR2] - (high[biqs_temp]*high[biqs_b1]);
		high[biqs_aR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_b2]);
		high[biqs_outR] = high[biqs_temp];
		high[biqs_dis] = fabs(high[biqs_c0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_cR1];
		high[biqs_cR1] = high[biqs_cR2] - (high[biqs_temp]*high[biqs_d1]);
		high[biqs_cR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_d2]);
		high[biqs_outR] = high[biqs_temp];
		high[biqs_dis] = fabs(high[biqs_e0] * (1.0+(high[biqs_outR]*high[biqs_nonlin])));
		if (high[biqs_dis] > 1.0) high[biqs_dis] = 1.0;
		high[biqs_temp] = (high[biqs_outR] * high[biqs_dis]) + high[biqs_eR1];
		high[biqs_eR1] = high[biqs_eR2] - (high[biqs_temp]*high[biqs_f1]);
		high[biqs_eR2] = (high[biqs_outR] * -high[biqs_dis]) - (high[biqs_temp]*high[biqs_f2]);
		high[biqs_outR] = high[biqs_temp]; high[biqs_outR] *= high[biqs_level];
		if (high[biqs_level] > 1.0) high[biqs_outR] *= high[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		hmid[biqs_outR] = inputSampleR * fabs(hmid[biqs_level]);
		hmid[biqs_dis] = fabs(hmid[biqs_a0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_aR1];
		hmid[biqs_aR1] = hmid[biqs_aR2] - (hmid[biqs_temp]*hmid[biqs_b1]);
		hmid[biqs_aR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_b2]);
		hmid[biqs_outR] = hmid[biqs_temp];
		hmid[biqs_dis] = fabs(hmid[biqs_c0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_cR1];
		hmid[biqs_cR1] = hmid[biqs_cR2] - (hmid[biqs_temp]*hmid[biqs_d1]);
		hmid[biqs_cR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_d2]);
		hmid[biqs_outR] = hmid[biqs_temp];
		hmid[biqs_dis] = fabs(hmid[biqs_e0] * (1.0+(hmid[biqs_outR]*hmid[biqs_nonlin])));
		if (hmid[biqs_dis] > 1.0) hmid[biqs_dis] = 1.0;
		hmid[biqs_temp] = (hmid[biqs_outR] * hmid[biqs_dis]) + hmid[biqs_eR1];
		hmid[biqs_eR1] = hmid[biqs_eR2] - (hmid[biqs_temp]*hmid[biqs_f1]);
		hmid[biqs_eR2] = (hmid[biqs_outR] * -hmid[biqs_dis]) - (hmid[biqs_temp]*hmid[biqs_f2]);
		hmid[biqs_outR] = hmid[biqs_temp]; hmid[biqs_outR] *= hmid[biqs_level];
		if (hmid[biqs_level] > 1.0) hmid[biqs_outR] *= hmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		lmid[biqs_outR] = inputSampleR * fabs(lmid[biqs_level]);
		lmid[biqs_dis] = fabs(lmid[biqs_a0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_aR1];
		lmid[biqs_aR1] = lmid[biqs_aR2] - (lmid[biqs_temp]*lmid[biqs_b1]);
		lmid[biqs_aR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_b2]);
		lmid[biqs_outR] = lmid[biqs_temp];
		lmid[biqs_dis] = fabs(lmid[biqs_c0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_cR1];
		lmid[biqs_cR1] = lmid[biqs_cR2] - (lmid[biqs_temp]*lmid[biqs_d1]);
		lmid[biqs_cR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_d2]);
		lmid[biqs_outR] = lmid[biqs_temp];
		lmid[biqs_dis] = fabs(lmid[biqs_e0] * (1.0+(lmid[biqs_outR]*lmid[biqs_nonlin])));
		if (lmid[biqs_dis] > 1.0) lmid[biqs_dis] = 1.0;
		lmid[biqs_temp] = (lmid[biqs_outR] * lmid[biqs_dis]) + lmid[biqs_eR1];
		lmid[biqs_eR1] = lmid[biqs_eR2] - (lmid[biqs_temp]*lmid[biqs_f1]);
		lmid[biqs_eR2] = (lmid[biqs_outR] * -lmid[biqs_dis]) - (lmid[biqs_temp]*lmid[biqs_f2]);
		lmid[biqs_outR] = lmid[biqs_temp]; lmid[biqs_outR] *= lmid[biqs_level];
		if (lmid[biqs_level] > 1.0) lmid[biqs_outR] *= lmid[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		//begin Stacked Biquad With Reversed Neutron Flow R
		bass[biqs_outR] = inputSampleR * fabs(bass[biqs_level]);
		bass[biqs_dis] = fabs(bass[biqs_a0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_aR1];
		bass[biqs_aR1] = bass[biqs_aR2] - (bass[biqs_temp]*bass[biqs_b1]);
		bass[biqs_aR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_b2]);
		bass[biqs_outR] = bass[biqs_temp];
		bass[biqs_dis] = fabs(bass[biqs_c0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_cR1];
		bass[biqs_cR1] = bass[biqs_cR2] - (bass[biqs_temp]*bass[biqs_d1]);
		bass[biqs_cR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_d2]);
		bass[biqs_outR] = bass[biqs_temp];
		bass[biqs_dis] = fabs(bass[biqs_e0] * (1.0+(bass[biqs_outR]*bass[biqs_nonlin])));
		if (bass[biqs_dis] > 1.0) bass[biqs_dis] = 1.0;
		bass[biqs_temp] = (bass[biqs_outR] * bass[biqs_dis]) + bass[biqs_eR1];
		bass[biqs_eR1] = bass[biqs_eR2] - (bass[biqs_temp]*bass[biqs_f1]);
		bass[biqs_eR2] = (bass[biqs_outR] * -bass[biqs_dis]) - (bass[biqs_temp]*bass[biqs_f2]);
		bass[biqs_outR] = bass[biqs_temp]; bass[biqs_outR] *= bass[biqs_level];
		if (bass[biqs_level] > 1.0) bass[biqs_outR] *= bass[biqs_level];
		//end Stacked Biquad With Reversed Neutron Flow R
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double gainR = (panA*temp)+(panB*(1.0-temp));
		double gainL = 1.57079633-gainR;
		gainR = sin(gainR); gainL = sin(gainL);
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		if (gain > 1.0) gain *= gain; else gain = 1.0-pow(1.0-gain,2);
		gain *= 0.763932022500211;
		double airGain = (airGainA*temp)+(airGainB*(1.0-temp));
		if (airGain > 1.0) airGain *= airGain; else airGain = 1.0-pow(1.0-airGain,2);
		double fireGain = (fireGainA*temp)+(fireGainB*(1.0-temp));
		if (fireGain > 1.0) fireGain *= fireGain; else fireGain = 1.0-pow(1.0-fireGain,2);
		double firePad = fireGain; if (firePad > 1.0) firePad = 1.0;
		double stoneGain = (stoneGainA*temp)+(stoneGainB*(1.0-temp));
		if (stoneGain > 1.0) stoneGain *= stoneGain; else stoneGain = 1.0-pow(1.0-stoneGain,2);
		double stonePad = stoneGain; if (stonePad > 1.0) stonePad = 1.0;
		//set up smoothed gain controls
		
		//begin Air3L
		double drySampleL = inputSampleL;
		air[pvSL4] = air[pvAL4] - air[pvAL3]; air[pvSL3] = air[pvAL3] - air[pvAL2];
		air[pvSL2] = air[pvAL2] - air[pvAL1]; air[pvSL1] = air[pvAL1] - inputSampleL;
		air[accSL3] = air[pvSL4] - air[pvSL3]; air[accSL2] = air[pvSL3] - air[pvSL2];
		air[accSL1] = air[pvSL2] - air[pvSL1];
		air[acc2SL2] = air[accSL3] - air[accSL2]; air[acc2SL1] = air[accSL2] - air[accSL1];		
		air[outAL] = -(air[pvAL1] + air[pvSL3] + air[acc2SL2] - ((air[acc2SL2] + air[acc2SL1])*0.5));
		air[gainAL] *= 0.5; air[gainAL] += fabs(drySampleL-air[outAL])*0.5;
		if (air[gainAL] > 0.3*sqrt(overallscale)) air[gainAL] = 0.3*sqrt(overallscale);
		air[pvAL4] = air[pvAL3]; air[pvAL3] = air[pvAL2];
		air[pvAL2] = air[pvAL1]; air[pvAL1] = (air[gainAL] * air[outAL]) + drySampleL;
		double fireL = drySampleL - ((air[outAL]*0.5)+(drySampleL*(0.457-(0.017*overallscale))));
		temp = (fireL + air[gndavgL])*0.5; air[gndavgL] = fireL; fireL = temp;
		double airL = (drySampleL-fireL)*airGain;
		inputSampleL = fireL;
		//end Air3L
		//begin Air3R
		double drySampleR = inputSampleR;
		air[pvSR4] = air[pvAR4] - air[pvAR3]; air[pvSR3] = air[pvAR3] - air[pvAR2];
		air[pvSR2] = air[pvAR2] - air[pvAR1]; air[pvSR1] = air[pvAR1] - inputSampleR;
		air[accSR3] = air[pvSR4] - air[pvSR3]; air[accSR2] = air[pvSR3] - air[pvSR2];
		air[accSR1] = air[pvSR2] - air[pvSR1];
		air[acc2SR2] = air[accSR3] - air[accSR2]; air[acc2SR1] = air[accSR2] - air[accSR1];		
		air[outAR] = -(air[pvAR1] + air[pvSR3] + air[acc2SR2] - ((air[acc2SR2] + air[acc2SR1])*0.5));
		air[gainAR] *= 0.5; air[gainAR] += fabs(drySampleR-air[outAR])*0.5;
		if (air[gainAR] > 0.3*sqrt(overallscale)) air[gainAR] = 0.3*sqrt(overallscale);
		air[pvAR4] = air[pvAR3]; air[pvAR3] = air[pvAR2];
		air[pvAR2] = air[pvAR1]; air[pvAR1] = (air[gainAR] * air[outAR]) + drySampleR;
		double fireR = drySampleR - ((air[outAR]*0.5)+(drySampleR*(0.457-(0.017*overallscale))));
		temp = (fireR + air[gndavgR])*0.5; air[gndavgR] = fireR; fireR = temp;
		double airR = (drySampleR-fireR)*airGain;
		inputSampleR = fireR;
		//end Air3R
		//begin KalmanL
		temp = inputSampleL = inputSampleL*(1.0-kalmanRange)*0.777;
		inputSampleL *= (1.0-kalmanRange);
		//set up gain levels to control the beast
		kal[prevSlewL3] += kal[prevSampL3] - kal[prevSampL2]; kal[prevSlewL3] *= 0.5;
		kal[prevSlewL2] += kal[prevSampL2] - kal[prevSampL1]; kal[prevSlewL2] *= 0.5;
		kal[prevSlewL1] += kal[prevSampL1] - inputSampleL; kal[prevSlewL1] *= 0.5;
		//make slews from each set of samples used
		kal[accSlewL2] += kal[prevSlewL3] - kal[prevSlewL2]; kal[accSlewL2] *= 0.5;
		kal[accSlewL1] += kal[prevSlewL2] - kal[prevSlewL1]; kal[accSlewL1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kal[accSlewL3] += (kal[accSlewL2] - kal[accSlewL1]); kal[accSlewL3] *= 0.5;
		//entering the abyss, what even is this
		kal[kalOutL] += kal[prevSampL1] + kal[prevSlewL2] + kal[accSlewL3]; kal[kalOutL] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kal[kalGainL] += fabs(temp-kal[kalOutL])*kalmanRange*8.0; kal[kalGainL] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kal[kalGainL] > kalmanRange*0.5) kal[kalGainL] = kalmanRange*0.5;
		//attempts to avoid explosions
		kal[kalOutL] += (temp*(1.0-(0.68+(kalmanRange*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kal[prevSampL3] = kal[prevSampL2]; kal[prevSampL2] = kal[prevSampL1];
		kal[prevSampL1] = (kal[kalGainL] * kal[kalOutL]) + ((1.0-kal[kalGainL])*temp);
		//feed the chain of previous samples
		if (kal[prevSampL1] > 1.0) kal[prevSampL1] = 1.0; if (kal[prevSampL1] < -1.0) kal[prevSampL1] = -1.0;
		double stoneL = kal[kalOutL]*0.777;
		fireL -= stoneL;
		//end KalmanL
		//begin KalmanR
		temp = inputSampleR = inputSampleR*(1.0-kalmanRange)*0.777;
		inputSampleR *= (1.0-kalmanRange);
		//set up gain levels to control the beast
		kal[prevSlewR3] += kal[prevSampR3] - kal[prevSampR2]; kal[prevSlewR3] *= 0.5;
		kal[prevSlewR2] += kal[prevSampR2] - kal[prevSampR1]; kal[prevSlewR2] *= 0.5;
		kal[prevSlewR1] += kal[prevSampR1] - inputSampleR; kal[prevSlewR1] *= 0.5;
		//make slews from each set of samples used
		kal[accSlewR2] += kal[prevSlewR3] - kal[prevSlewR2]; kal[accSlewR2] *= 0.5;
		kal[accSlewR1] += kal[prevSlewR2] - kal[prevSlewR1]; kal[accSlewR1] *= 0.5;
		//differences between slews: rate of change of rate of change
		kal[accSlewR3] += (kal[accSlewR2] - kal[accSlewR1]); kal[accSlewR3] *= 0.5;
		//entering the abyss, what even is this
		kal[kalOutR] += kal[prevSampR1] + kal[prevSlewR2] + kal[accSlewR3]; kal[kalOutR] *= 0.5;
		//resynthesizing predicted result (all iir smoothed)
		kal[kalGainR] += fabs(temp-kal[kalOutR])*kalmanRange*8.0; kal[kalGainR] *= 0.5;
		//madness takes its toll. Kalman Gain: how much dry to retain
		if (kal[kalGainR] > kalmanRange*0.5) kal[kalGainR] = kalmanRange*0.5;
		//attempts to avoid explosions
		kal[kalOutR] += (temp*(1.0-(0.68+(kalmanRange*0.157))));	
		//this is for tuning a really complete cancellation up around Nyquist
		kal[prevSampR3] = kal[prevSampR2]; kal[prevSampR2] = kal[prevSampR1];
		kal[prevSampR1] = (kal[kalGainR] * kal[kalOutR]) + ((1.0-kal[kalGainR])*temp);
		//feed the chain of previous samples
		if (kal[prevSampR1] > 1.0) kal[prevSampR1] = 1.0; if (kal[prevSampR1] < -1.0) kal[prevSampR1] = -1.0;
		double stoneR = kal[kalOutR]*0.777;
		fireR -= stoneR;
		//end KalmanR
		//fire dynamics
		if (fabs(fireL) > compFThresh) { //compression L
			fireCompL -= (fireCompL * compFAttack);
			fireCompL += ((compFThresh / fabs(fireL))*compFAttack);
		} else fireCompL = (fireCompL*(1.0-compFRelease))+compFRelease;
		if (fabs(fireR) > compFThresh) { //compression R
			fireCompR -= (fireCompR * compFAttack);
			fireCompR += ((compFThresh / fabs(fireR))*compFAttack);
		} else fireCompR = (fireCompR*(1.0-compFRelease))+compFRelease;
		if (fireCompL > fireCompR) fireCompL -= (fireCompL * compFAttack);
		if (fireCompR > fireCompL) fireCompR -= (fireCompR * compFAttack);
		if (fabs(fireL) > gateFThresh) fireGate = gateFSustain;
		else if (fabs(fireR) > gateFThresh) fireGate = gateFSustain;
		else fireGate *= (1.0-gateFRelease);
		if (fireGate < 0.0) fireGate = 0.0;
		fireCompL = fmax(fmin(fireCompL,1.0),0.0);
		fireCompR = fmax(fmin(fireCompR,1.0),0.0);
		fireL *= (((1.0-compFRatio)*firePad)+(fireCompL*compFRatio*fireGain));
		fireR *= (((1.0-compFRatio)*firePad)+(fireCompR*compFRatio*fireGain));
		if (fireGate < M_PI_2) {
			temp = ((1.0-gateFRatio)+(sin(fireGate)*gateFRatio));
			airL *= temp;
			airR *= temp;
			fireL *= temp;
			fireR *= temp;
			high[biqs_outL] *= temp;
			high[biqs_outR] *= temp;
			hmid[biqs_outL] *= temp; //if Fire gating, gate Air, high and hmid
			hmid[biqs_outR] *= temp; //note that we aren't compressing these
		}
		//stone dynamics
		if (fabs(stoneL) > compSThresh) { //compression L
			stoneCompL -= (stoneCompL * compSAttack);
			stoneCompL += ((compSThresh / fabs(stoneL))*compSAttack);
		} else stoneCompL = (stoneCompL*(1.0-compSRelease))+compSRelease;
		if (fabs(stoneR) > compSThresh) { //compression R
			stoneCompR -= (stoneCompR * compSAttack);
			stoneCompR += ((compSThresh / fabs(stoneR))*compSAttack);
		} else stoneCompR = (stoneCompR*(1.0-compSRelease))+compSRelease;
		if (stoneCompL > stoneCompR) stoneCompL -= (stoneCompL * compSAttack);
		if (stoneCompR > stoneCompL) stoneCompR -= (stoneCompR * compSAttack);
		if (fabs(stoneL) > gateSThresh) stoneGate = gateSSustain;
		else if (fabs(stoneR) > gateSThresh) stoneGate = gateSSustain;
		else stoneGate *= (1.0-gateSRelease);
		if (stoneGate < 0.0) stoneGate = 0.0;
		stoneCompL = fmax(fmin(stoneCompL,1.0),0.0);
		stoneCompR = fmax(fmin(stoneCompR,1.0),0.0);
		stoneL *= (((1.0-compSRatio)*stonePad)+(stoneCompL*compSRatio*stoneGain));
		stoneR *= (((1.0-compSRatio)*stonePad)+(stoneCompR*compSRatio*stoneGain));
		if (stoneGate < M_PI_2) {
			temp = ((1.0-gateSRatio)+(sin(stoneGate)*gateSRatio));
			stoneL *= temp;
			stoneR *= temp;
			lmid[biqs_outL] *= temp;
			lmid[biqs_outR] *= temp;
			bass[biqs_outL] *= temp; //if Stone gating, gate lmid and bass
			bass[biqs_outR] *= temp; //note that we aren't compressing these
		}
		inputSampleL = stoneL + fireL + airL;
		inputSampleR = stoneR + fireR + airR;
		//create Stonefire output
		
		if (highpassEngage) { //distributed Highpass
			highpass[hilp_temp] = (inputSampleL*highpass[hilp_c0])+highpass[hilp_cL1];
			highpass[hilp_cL1] = (inputSampleL*highpass[hilp_c1])-(highpass[hilp_temp]*highpass[hilp_d1])+highpass[hilp_cL2];
			highpass[hilp_cL2] = (inputSampleL*highpass[hilp_c0])-(highpass[hilp_temp]*highpass[hilp_d2]); inputSampleL = highpass[hilp_temp];
			highpass[hilp_temp] = (inputSampleR*highpass[hilp_c0])+highpass[hilp_cR1];
			highpass[hilp_cR1] = (inputSampleR*highpass[hilp_c1])-(highpass[hilp_temp]*highpass[hilp_d1])+highpass[hilp_cR2];
			highpass[hilp_cR2] = (inputSampleR*highpass[hilp_c0])-(highpass[hilp_temp]*highpass[hilp_d2]); inputSampleR = highpass[hilp_temp];
		} else highpass[hilp_cR1] = highpass[hilp_cR2] = highpass[hilp_cL1] = highpass[hilp_cL2] = 0.0;
		if (lowpassEngage) { //distributed Lowpass
			lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_c0])+lowpass[hilp_cL1];
			lowpass[hilp_cL1] = (inputSampleL*lowpass[hilp_c1])-(lowpass[hilp_temp]*lowpass[hilp_d1])+lowpass[hilp_cL2];
			lowpass[hilp_cL2] = (inputSampleL*lowpass[hilp_c0])-(lowpass[hilp_temp]*lowpass[hilp_d2]); inputSampleL = lowpass[hilp_temp];
			lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_c0])+lowpass[hilp_cR1];
			lowpass[hilp_cR1] = (inputSampleR*lowpass[hilp_c1])-(lowpass[hilp_temp]*lowpass[hilp_d1])+lowpass[hilp_cR2];
			lowpass[hilp_cR2] = (inputSampleR*lowpass[hilp_c0])-(lowpass[hilp_temp]*lowpass[hilp_d2]); inputSampleR = lowpass[hilp_temp];
		} else lowpass[hilp_cR1] = lowpass[hilp_cR2] = lowpass[hilp_cL1] = lowpass[hilp_cL2] = 0.0;
		//another stage of Highpass/Lowpass before bringing in the parametric bands
		
		inputSampleL += (high[biqs_outL] + hmid[biqs_outL] + lmid[biqs_outL] + bass[biqs_outL]);
		inputSampleR += (high[biqs_outR] + hmid[biqs_outR] + lmid[biqs_outR] + bass[biqs_outR]);
		//add parametric boosts or cuts: clean as possible for maximal rawness and sonority		
		
		inputSampleL = inputSampleL * gainL * gain;
		inputSampleR = inputSampleR * gainR * gain;
		//applies pan section, and smoothed fader gain
		
		inputSampleL *= topdB;
		if (inputSampleL < -0.222) inputSampleL = -0.222; if (inputSampleL > 0.222) inputSampleL = 0.222;
		dBaL[dBaXL] = inputSampleL; dBaPosL *= 0.5; dBaPosL += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*0.5);
		int dBdly = floor(dBaPosL*dscBuf);
		double dBi = (dBaPosL*dscBuf)-dBdly;
		inputSampleL = dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBaL[dBaXL-dBdly +((dBaXL-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXL++; if (dBaXL < 0 || dBaXL >= dscBuf) dBaXL = 0;
		inputSampleL /= topdB;		
		inputSampleR *= topdB;
		if (inputSampleR < -0.222) inputSampleR = -0.222; if (inputSampleR > 0.222) inputSampleR = 0.222;
		dBaR[dBaXR] = inputSampleR; dBaPosR *= 0.5; dBaPosR += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*0.5);
		dBdly = floor(dBaPosR*dscBuf);
		dBi = (dBaPosR*dscBuf)-dBdly;
		inputSampleR = dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBaR[dBaXR-dBdly +((dBaXR-dBdly < 0)?dscBuf:0)]*dBi;
		dBaXR++; if (dBaXR < 0 || dBaXR >= dscBuf) dBaXR = 0;
		inputSampleR /= topdB;		
		//top dB processing for distributed discontinuity modeling air nonlinearity		
				
		if (highpassEngage) { //distributed Highpass
			highpass[hilp_temp] = (inputSampleL*highpass[hilp_e0])+highpass[hilp_eL1];
			highpass[hilp_eL1] = (inputSampleL*highpass[hilp_e1])-(highpass[hilp_temp]*highpass[hilp_f1])+highpass[hilp_eL2];
			highpass[hilp_eL2] = (inputSampleL*highpass[hilp_e0])-(highpass[hilp_temp]*highpass[hilp_f2]); inputSampleL = highpass[hilp_temp];
			highpass[hilp_temp] = (inputSampleR*highpass[hilp_e0])+highpass[hilp_eR1];
			highpass[hilp_eR1] = (inputSampleR*highpass[hilp_e1])-(highpass[hilp_temp]*highpass[hilp_f1])+highpass[hilp_eR2];
			highpass[hilp_eR2] = (inputSampleR*highpass[hilp_e0])-(highpass[hilp_temp]*highpass[hilp_f2]); inputSampleR = highpass[hilp_temp];
		} else highpass[hilp_eR1] = highpass[hilp_eR2] = highpass[hilp_eL1] = highpass[hilp_eL2] = 0.0;
		if (lowpassEngage) { //distributed Lowpass
			lowpass[hilp_temp] = (inputSampleL*lowpass[hilp_e0])+lowpass[hilp_eL1];
			lowpass[hilp_eL1] = (inputSampleL*lowpass[hilp_e1])-(lowpass[hilp_temp]*lowpass[hilp_f1])+lowpass[hilp_eL2];
			lowpass[hilp_eL2] = (inputSampleL*lowpass[hilp_e0])-(lowpass[hilp_temp]*lowpass[hilp_f2]); inputSampleL = lowpass[hilp_temp];
			lowpass[hilp_temp] = (inputSampleR*lowpass[hilp_e0])+lowpass[hilp_eR1];
			lowpass[hilp_eR1] = (inputSampleR*lowpass[hilp_e1])-(lowpass[hilp_temp]*lowpass[hilp_f1])+lowpass[hilp_eR2];
			lowpass[hilp_eR2] = (inputSampleR*lowpass[hilp_e0])-(lowpass[hilp_temp]*lowpass[hilp_f2]); inputSampleR = lowpass[hilp_temp];
		} else lowpass[hilp_eR1] = lowpass[hilp_eR2] = lowpass[hilp_eL1] = lowpass[hilp_eL2] = 0.0;		
		//final Highpass/Lowpass continues to address aliasing
		//final stacked biquad section is the softest Q for smoothness
		
		//begin 64 bit stereo floating point dither
		int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 1.1e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
