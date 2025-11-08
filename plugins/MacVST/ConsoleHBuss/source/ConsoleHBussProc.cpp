/* ========================================
 *  ConsoleHBuss - ConsoleHBuss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ConsoleHBuss_H
#include "ConsoleHBuss.h"
#endif

void ConsoleHBuss::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale*2.0);
	if (spacing < 2) spacing = 2; if (spacing > 32) spacing = 32;
	
	double trebleGain = (HIG-0.5)*2.0;
	trebleGain = 1.0+(trebleGain*fabs(trebleGain)*fabs(trebleGain));
	double midGain = (MID-0.5)*2.0;
	midGain = 1.0+(midGain*fabs(midGain)*fabs(midGain));
	double bassGain = (LOW-0.5)*2.0;
	bassGain = 1.0+(bassGain*fabs(bassGain)*fabs(bassGain));
	//separate from filtering stage, this is amplitude, centered on 1.0 unity gain
	
	//SmoothEQ3 is how to get 3rd order steepness at very low CPU.
	//because sample rate varies, you could also vary the crossovers
	//you can't vary Q because math is simplified to take advantage of
	//how the accurate Q value for this filter is always exactly 1.0.
	highFast[biq_freq] = (4000.0/getSampleRate());
	double omega = 2.0*M_PI*(4000.0/getSampleRate()); //mid-high crossover freq
	double biqK = 2.0 - cos(omega);
	double highCoef = -sqrt(biqK*biqK - 1.0) + biqK;
	lowFast[biq_freq] = (200.0/getSampleRate());
	omega = 2.0*M_PI*(200.0/getSampleRate()); //low-mid crossover freq
	biqK = 2.0 - cos(omega);
	double lowCoef = -sqrt(biqK*biqK - 1.0) + biqK;
	//exponential IIR filter as part of an accurate 3rd order Butterworth filter 
	biqK = tan(M_PI * highFast[biq_freq]);
	double norm = 1.0 / (1.0 + biqK + biqK*biqK);
	highFast[biq_a0] = biqK * biqK * norm;
	highFast[biq_a1] = 2.0 * highFast[biq_a0];
	highFast[biq_a2] = highFast[biq_a0];
	highFast[biq_b1] = 2.0 * (biqK*biqK - 1.0) * norm;
	highFast[biq_b2] = (1.0 - biqK + biqK*biqK) * norm;
	biqK = tan(M_PI * lowFast[biq_freq]);
	norm = 1.0 / (1.0 + biqK + biqK*biqK);
	lowFast[biq_a0] = biqK * biqK * norm;
	lowFast[biq_a1] = 2.0 * lowFast[biq_a0];
	lowFast[biq_a2] = lowFast[biq_a0];
	lowFast[biq_b1] = 2.0 * (biqK*biqK - 1.0) * norm;
	lowFast[biq_b2] = (1.0 - biqK + biqK*biqK) * norm;
	//custom biquad setup with Q = 1.0 gets to omit some divides
	//SmoothEQ3
		
	double bezCThresh = pow(1.0-THR, 6.0) * 8.0;
	double bezRez = pow(1.0-THR, 12.360679774997898) / overallscale;
	double sloRez = pow(1.0-THR,10.0) / overallscale;
	sloRez = fmin(fmax(sloRez,0.00001),1.0);
	bezRez = fmin(fmax(bezRez,0.00001),1.0);
	//Dynamics2
	
	highpass[hilp_freq] = ((pow(HIP,3)*24000.0)+10.0)/getSampleRate();
	if (highpass[hilp_freq] > 0.495) highpass[hilp_freq] = 0.495;
	bool highpassEngage = true; if (HIP == 0.0) highpassEngage = false;
	
	lowpass[hilp_freq] = ((pow(LOP,3)*24000.0)+10.0)/getSampleRate();
	if (lowpass[hilp_freq] > 0.495) lowpass[hilp_freq] = 0.495;
	bool lowpassEngage = true; if (LOP == 0.0) lowpassEngage = false;
	
	highpass[hilp_aA0] = highpass[hilp_aB0];
	highpass[hilp_aA1] = highpass[hilp_aB1];
	highpass[hilp_bA1] = highpass[hilp_bB1];
	highpass[hilp_bA2] = highpass[hilp_bB2];
	highpass[hilp_cA0] = highpass[hilp_cB0];
	highpass[hilp_cA1] = highpass[hilp_cB1];
	highpass[hilp_dA1] = highpass[hilp_dB1];
	highpass[hilp_dA2] = highpass[hilp_dB2];
	highpass[hilp_eA0] = highpass[hilp_eB0];
	highpass[hilp_eA1] = highpass[hilp_eB1];
	highpass[hilp_fA1] = highpass[hilp_fB1];
	highpass[hilp_fA2] = highpass[hilp_fB2];
	lowpass[hilp_aA0] = lowpass[hilp_aB0];
	lowpass[hilp_aA1] = lowpass[hilp_aB1];
	lowpass[hilp_bA1] = lowpass[hilp_bB1];
	lowpass[hilp_bA2] = lowpass[hilp_bB2];
	lowpass[hilp_cA0] = lowpass[hilp_cB0];
	lowpass[hilp_cA1] = lowpass[hilp_cB1];
	lowpass[hilp_dA1] = lowpass[hilp_dB1];
	lowpass[hilp_dA2] = lowpass[hilp_dB2];
	lowpass[hilp_eA0] = lowpass[hilp_eB0];
	lowpass[hilp_eA1] = lowpass[hilp_eB1];
	lowpass[hilp_fA1] = lowpass[hilp_fB1];
	lowpass[hilp_fA2] = lowpass[hilp_fB2];
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.
	//On the buss, highpass and lowpass are isolators meant to be moved,
	//so they are interpolated where the channels are not
	
	biqK = tan(M_PI * highpass[hilp_freq]); //highpass
	norm = 1.0 / (1.0 + biqK / 1.93185165 + biqK * biqK);
	highpass[hilp_aB0] = norm;
	highpass[hilp_aB1] = -2.0 * highpass[hilp_aB0];
	highpass[hilp_bB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highpass[hilp_bB2] = (1.0 - biqK / 1.93185165 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.70710678 + biqK * biqK);
	highpass[hilp_cB0] = norm;
	highpass[hilp_cB1] = -2.0 * highpass[hilp_cB0];
	highpass[hilp_dB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highpass[hilp_dB2] = (1.0 - biqK / 0.70710678 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.51763809 + biqK * biqK);
	highpass[hilp_eB0] = norm;
	highpass[hilp_eB1] = -2.0 * highpass[hilp_eB0];
	highpass[hilp_fB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highpass[hilp_fB2] = (1.0 - biqK / 0.51763809 + biqK * biqK) * norm;
	
	biqK = tan(M_PI * lowpass[hilp_freq]); //lowpass
	norm = 1.0 / (1.0 + biqK / 1.93185165 + biqK * biqK);
	lowpass[hilp_aB0] = biqK * biqK * norm;
	lowpass[hilp_aB1] = 2.0 * lowpass[hilp_aB0];
	lowpass[hilp_bB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowpass[hilp_bB2] = (1.0 - biqK / 1.93185165 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.70710678 + biqK * biqK);
	lowpass[hilp_cB0] = biqK * biqK * norm;
	lowpass[hilp_cB1] = 2.0 * lowpass[hilp_cB0];
	lowpass[hilp_dB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowpass[hilp_dB2] = (1.0 - biqK / 0.70710678 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.51763809 + biqK * biqK);
	lowpass[hilp_eB0] = biqK * biqK * norm;
	lowpass[hilp_eB1] = 2.0 * lowpass[hilp_eB0];
	lowpass[hilp_fB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowpass[hilp_fB2] = (1.0 - biqK / 0.51763809 + biqK * biqK) * norm;
	
	if (highpass[hilp_aA0] == 0.0) { // if we have just started, start directly with raw info
		highpass[hilp_aA0] = highpass[hilp_aB0];
		highpass[hilp_aA1] = highpass[hilp_aB1];
		highpass[hilp_bA1] = highpass[hilp_bB1];
		highpass[hilp_bA2] = highpass[hilp_bB2];
		highpass[hilp_cA0] = highpass[hilp_cB0];
		highpass[hilp_cA1] = highpass[hilp_cB1];
		highpass[hilp_dA1] = highpass[hilp_dB1];
		highpass[hilp_dA2] = highpass[hilp_dB2];
		highpass[hilp_eA0] = highpass[hilp_eB0];
		highpass[hilp_eA1] = highpass[hilp_eB1];
		highpass[hilp_fA1] = highpass[hilp_fB1];
		highpass[hilp_fA2] = highpass[hilp_fB2];
		lowpass[hilp_aA0] = lowpass[hilp_aB0];
		lowpass[hilp_aA1] = lowpass[hilp_aB1];
		lowpass[hilp_bA1] = lowpass[hilp_bB1];
		lowpass[hilp_bA2] = lowpass[hilp_bB2];
		lowpass[hilp_cA0] = lowpass[hilp_cB0];
		lowpass[hilp_cA1] = lowpass[hilp_cB1];
		lowpass[hilp_dA1] = lowpass[hilp_dB1];
		lowpass[hilp_dA2] = lowpass[hilp_dB2];
		lowpass[hilp_eA0] = lowpass[hilp_eB0];
		lowpass[hilp_eA1] = lowpass[hilp_eB1];
		lowpass[hilp_fA1] = lowpass[hilp_fB1];
		lowpass[hilp_fA2] = lowpass[hilp_fB2];
	}
	
	panA = panB; panB = PAN*1.57079633;
	inTrimA = inTrimB; inTrimB = FAD*2.0;
	//Console
	
	while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		const double temp = (double)sampleFrames/inFramesToProcess;
		highpass[hilp_a0] = (highpass[hilp_aA0]*temp)+(highpass[hilp_aB0]*(1.0-temp));
		highpass[hilp_a1] = (highpass[hilp_aA1]*temp)+(highpass[hilp_aB1]*(1.0-temp));
		highpass[hilp_b1] = (highpass[hilp_bA1]*temp)+(highpass[hilp_bB1]*(1.0-temp));
		highpass[hilp_b2] = (highpass[hilp_bA2]*temp)+(highpass[hilp_bB2]*(1.0-temp));
		highpass[hilp_c0] = (highpass[hilp_cA0]*temp)+(highpass[hilp_cB0]*(1.0-temp));
		highpass[hilp_c1] = (highpass[hilp_cA1]*temp)+(highpass[hilp_cB1]*(1.0-temp));
		highpass[hilp_d1] = (highpass[hilp_dA1]*temp)+(highpass[hilp_dB1]*(1.0-temp));
		highpass[hilp_d2] = (highpass[hilp_dA2]*temp)+(highpass[hilp_dB2]*(1.0-temp));
		highpass[hilp_e0] = (highpass[hilp_eA0]*temp)+(highpass[hilp_eB0]*(1.0-temp));
		highpass[hilp_e1] = (highpass[hilp_eA1]*temp)+(highpass[hilp_eB1]*(1.0-temp));
		highpass[hilp_f1] = (highpass[hilp_fA1]*temp)+(highpass[hilp_fB1]*(1.0-temp));
		highpass[hilp_f2] = (highpass[hilp_fA2]*temp)+(highpass[hilp_fB2]*(1.0-temp));
		lowpass[hilp_a0] = (lowpass[hilp_aA0]*temp)+(lowpass[hilp_aB0]*(1.0-temp));
		lowpass[hilp_a1] = (lowpass[hilp_aA1]*temp)+(lowpass[hilp_aB1]*(1.0-temp));
		lowpass[hilp_b1] = (lowpass[hilp_bA1]*temp)+(lowpass[hilp_bB1]*(1.0-temp));
		lowpass[hilp_b2] = (lowpass[hilp_bA2]*temp)+(lowpass[hilp_bB2]*(1.0-temp));
		lowpass[hilp_c0] = (lowpass[hilp_cA0]*temp)+(lowpass[hilp_cB0]*(1.0-temp));
		lowpass[hilp_c1] = (lowpass[hilp_cA1]*temp)+(lowpass[hilp_cB1]*(1.0-temp));
		lowpass[hilp_d1] = (lowpass[hilp_dA1]*temp)+(lowpass[hilp_dB1]*(1.0-temp));
		lowpass[hilp_d2] = (lowpass[hilp_dA2]*temp)+(lowpass[hilp_dB2]*(1.0-temp));
		lowpass[hilp_e0] = (lowpass[hilp_eA0]*temp)+(lowpass[hilp_eB0]*(1.0-temp));
		lowpass[hilp_e1] = (lowpass[hilp_eA1]*temp)+(lowpass[hilp_eB1]*(1.0-temp));
		lowpass[hilp_f1] = (lowpass[hilp_fA1]*temp)+(lowpass[hilp_fB1]*(1.0-temp));
		lowpass[hilp_f2] = (lowpass[hilp_fA2]*temp)+(lowpass[hilp_fB2]*(1.0-temp));
		
		
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
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 0.6180339887498949));
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 0.6180339887498949));
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 0.6180339887498949));
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 0.6180339887498949));
		
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
		
		double trebleFastL = inputSampleL;		
		double outSample = (trebleFastL * highFast[biq_a0]) + highFast[biq_sL1];
		highFast[biq_sL1] = (trebleFastL * highFast[biq_a1]) - (outSample * highFast[biq_b1]) + highFast[biq_sL2];
		highFast[biq_sL2] = (trebleFastL * highFast[biq_a2]) - (outSample * highFast[biq_b2]);
		double midFastL = outSample; trebleFastL -= midFastL;
		outSample = (midFastL * lowFast[biq_a0]) + lowFast[biq_sL1];
		lowFast[biq_sL1] = (midFastL * lowFast[biq_a1]) - (outSample * lowFast[biq_b1]) + lowFast[biq_sL2];
		lowFast[biq_sL2] = (midFastL * lowFast[biq_a2]) - (outSample * lowFast[biq_b2]);
		double bassFastL = outSample; midFastL -= bassFastL;
		trebleFastL = (bassFastL*bassGain) + (midFastL*midGain) + (trebleFastL*trebleGain);
		//first stage of two crossovers is biquad of exactly 1.0 Q
		highFastLIIR = (highFastLIIR*highCoef) + (trebleFastL*(1.0-highCoef));
		midFastL = highFastLIIR; trebleFastL -= midFastL;
		lowFastLIIR = (lowFastLIIR*lowCoef) + (midFastL*(1.0-lowCoef));
		bassFastL = lowFastLIIR; midFastL -= bassFastL;
		inputSampleL = (bassFastL*bassGain) + (midFastL*midGain) + (trebleFastL*trebleGain);		
		//second stage of two crossovers is the exponential filters
		//this produces a slightly steeper Butterworth filter very cheaply
		
		double trebleFastR = inputSampleR;		
		outSample = (trebleFastR * highFast[biq_a0]) + highFast[biq_sR1];
		highFast[biq_sR1] = (trebleFastR * highFast[biq_a1]) - (outSample * highFast[biq_b1]) + highFast[biq_sR2];
		highFast[biq_sR2] = (trebleFastR * highFast[biq_a2]) - (outSample * highFast[biq_b2]);
		double midFastR = outSample; trebleFastR -= midFastR;
		outSample = (midFastR * lowFast[biq_a0]) + lowFast[biq_sR1];
		lowFast[biq_sR1] = (midFastR * lowFast[biq_a1]) - (outSample * lowFast[biq_b1]) + lowFast[biq_sR2];
		lowFast[biq_sR2] = (midFastR * lowFast[biq_a2]) - (outSample * lowFast[biq_b2]);
		double bassFastR = outSample; midFastR -= bassFastR;
		trebleFastR = (bassFastR*bassGain) + (midFastR*midGain) + (trebleFastR*trebleGain);
		//first stage of two crossovers is biquad of exactly 1.0 Q
		highFastRIIR = (highFastRIIR*highCoef) + (trebleFastR*(1.0-highCoef));
		midFastR = highFastRIIR; trebleFastR -= midFastR;
		lowFastRIIR = (lowFastRIIR*lowCoef) + (midFastR*(1.0-lowCoef));
		bassFastR = lowFastRIIR; midFastR -= bassFastR;
		inputSampleR = (bassFastR*bassGain) + (midFastR*midGain) + (trebleFastR*trebleGain);		
		//second stage of two crossovers is the exponential filters
		//this produces a slightly steeper Butterworth filter very cheaply
		//SmoothEQ3
				
		if (bezCThresh > 0.0) {
			inputSampleL *= ((bezCThresh*0.5)+1.0);
			inputSampleR *= ((bezCThresh*0.5)+1.0);
		}
		
		bezCompF[bez_cycle] += bezRez;
		bezCompF[bez_SampL] += (fabs(inputSampleL) * bezRez);
		bezCompF[bez_SampR] += (fabs(inputSampleR) * bezRez);
		bezMaxF = fmax(bezMaxF,fmax(fabs(inputSampleL),fabs(inputSampleR)));
		
		if (bezCompF[bez_cycle] > 1.0) {
			bezCompF[bez_cycle] -= 1.0;
			bezCompF[bez_CL] = bezCompF[bez_BL];
			bezCompF[bez_BL] = bezCompF[bez_AL];
			bezCompF[bez_AL] = bezCompF[bez_SampL];
			bezCompF[bez_SampL] = 0.0;
			bezCompF[bez_CR] = bezCompF[bez_BR];
			bezCompF[bez_BR] = bezCompF[bez_AR];
			bezCompF[bez_AR] = bezCompF[bez_SampR];
			bezCompF[bez_SampR] = 0.0;
			bezMaxF = 0.0;
		}
		bezCompS[bez_cycle] += sloRez;
		bezCompS[bez_SampL] += (fabs(inputSampleL) * sloRez); //note: SampL is a control voltage
		bezCompS[bez_SampR] += (fabs(inputSampleR) * sloRez); //note: SampR is a control voltage
		if (bezCompS[bez_cycle] > 1.0) {
			bezCompS[bez_cycle] -= 1.0;
			bezCompS[bez_CL] = bezCompS[bez_BL];
			bezCompS[bez_BL] = bezCompS[bez_AL];
			bezCompS[bez_AL] = bezCompS[bez_SampL];
			bezCompS[bez_SampL] = 0.0;
			bezCompS[bez_CR] = bezCompS[bez_BR];
			bezCompS[bez_BR] = bezCompS[bez_AR];
			bezCompS[bez_AR] = bezCompS[bez_SampR];
			bezCompS[bez_SampR] = 0.0;
		}
		double CBFL = (bezCompF[bez_CL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BL]*bezCompF[bez_cycle]);
		double BAFL = (bezCompF[bez_BL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AL]*bezCompF[bez_cycle]);
		double CBAFL = (bezCompF[bez_BL]+(CBFL*(1.0-bezCompF[bez_cycle]))+(BAFL*bezCompF[bez_cycle]))*0.5;
		double CBSL = (bezCompS[bez_CL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BL]*bezCompS[bez_cycle]);
		double BASL = (bezCompS[bez_BL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AL]*bezCompS[bez_cycle]);
		double CBASL = (bezCompS[bez_BL]+(CBSL*(1.0-bezCompS[bez_cycle]))+(BASL*bezCompS[bez_cycle]))*0.5;
		double CBAMax = fmax(CBASL,CBAFL); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		double CBAFade = ((CBASL*-CBAMax)+(CBAFL*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleL *= 1.0-(fmin(((CBASL*(1.0-CBAFade))+(CBAFL*CBAFade))*bezCThresh,1.0));
		
		double CBFR = (bezCompF[bez_CR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BR]*bezCompF[bez_cycle]);
		double BAFR = (bezCompF[bez_BR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AR]*bezCompF[bez_cycle]);
		double CBAFR = (bezCompF[bez_BR]+(CBFR*(1.0-bezCompF[bez_cycle]))+(BAFR*bezCompF[bez_cycle]))*0.5;
		double CBSR = (bezCompS[bez_CR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BR]*bezCompS[bez_cycle]);
		double BASR = (bezCompS[bez_BR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AR]*bezCompS[bez_cycle]);
		double CBASR = (bezCompS[bez_BR]+(CBSR*(1.0-bezCompS[bez_cycle]))+(BASR*bezCompS[bez_cycle]))*0.5;
		CBAMax = fmax(CBASR,CBAFR); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		CBAFade = ((CBASR*-CBAMax)+(CBAFR*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleR *= 1.0-(fmin(((CBASR*(1.0-CBAFade))+(CBAFR*CBAFade))*bezCThresh,1.0));
		//Dynamics2
		
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

		double gainR = (panA*temp)+(panB*(1.0-temp));
		double gainL = 1.57079633-gainR;
		gainR = sin(gainR); gainL = sin(gainL);
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		if (gain > 1.0) gain *= gain;
		if (gain < 1.0) gain = 1.0-pow(1.0-gain,2);
		gain *= 2.0;
		
		inputSampleL = inputSampleL * gainL * gain;
		inputSampleR = inputSampleR * gainR * gain;
		//applies pan section, and smoothed fader gain
		
		double darkSampleL = inputSampleL;
		double darkSampleR = inputSampleR;
		if (avgPos > 31) avgPos = 0;
		if (spacing > 31) {
			avg32L[avgPos] = darkSampleL; avg32R[avgPos] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 32; x++) {darkSampleL += avg32L[x]; darkSampleR += avg32R[x];}
			darkSampleL /= 32.0; darkSampleR /= 32.0;
		} if (spacing > 15) {
			avg16L[avgPos%16] = darkSampleL; avg16R[avgPos%16] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 16; x++) {darkSampleL += avg16L[x]; darkSampleR += avg16R[x];}
			darkSampleL /= 16.0; darkSampleR /= 16.0;
		} if (spacing > 7) {
			avg8L[avgPos%8] = darkSampleL; avg8R[avgPos%8] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 8; x++) {darkSampleL += avg8L[x]; darkSampleR += avg8R[x];}
			darkSampleL /= 8.0; darkSampleR /= 8.0;
		} if (spacing > 3) {
			avg4L[avgPos%4] = darkSampleL; avg4R[avgPos%4] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 4; x++) {darkSampleL += avg4L[x]; darkSampleR += avg4R[x];}
			darkSampleL /= 4.0; darkSampleR /= 4.0;
		} if (spacing > 1) {
			avg2L[avgPos%2] = darkSampleL; avg2R[avgPos%2] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 2; x++) {darkSampleL += avg2L[x]; darkSampleR += avg2R[x];}
			darkSampleL /= 2.0; darkSampleR /= 2.0; 
		} avgPos++;
		lastSlewL += fabs(lastSlewpleL-inputSampleL); lastSlewpleL = inputSampleL;
		double avgSlewL = fmin(lastSlewL,1.0);
		lastSlewL = fmax(lastSlewL*0.78,2.39996322972865332223);
		lastSlewR += fabs(lastSlewpleR-inputSampleR); lastSlewpleR = inputSampleR;
		double avgSlewR = fmin(lastSlewR,1.0);
		lastSlewR = fmax(lastSlewR*0.78,2.39996322972865332223); //look up Golden Angle, it's cool
		inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
		inputSampleR = (inputSampleR*(1.0-avgSlewR)) + (darkSampleR*avgSlewR);
		
		//begin TapeHack section
		inputSampleL = fmax(fmin(inputSampleL,2.305929007734908),-2.305929007734908);
		double addtwo = inputSampleL * inputSampleL;
		double empower = inputSampleL * addtwo; // inputSampleL to the third power
		inputSampleL -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleL += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleL -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleL += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleL -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		inputSampleL *= 0.92;
		//end TapeHack section
		
		//begin TapeHack section
		inputSampleR = fmax(fmin(inputSampleR,2.305929007734908),-2.305929007734908);
		addtwo = inputSampleR * inputSampleR;
		empower = inputSampleR * addtwo; // inputSampleR to the third power
		inputSampleR -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleR += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleR -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleR += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleR -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		inputSampleR *= 0.92;
		//end TapeHack section		
		
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

void ConsoleHBuss::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int spacing = floor(overallscale*2.0);
	if (spacing < 2) spacing = 2; if (spacing > 32) spacing = 32;
	
	double trebleGain = (HIG-0.5)*2.0;
	trebleGain = 1.0+(trebleGain*fabs(trebleGain)*fabs(trebleGain));
	double midGain = (MID-0.5)*2.0;
	midGain = 1.0+(midGain*fabs(midGain)*fabs(midGain));
	double bassGain = (LOW-0.5)*2.0;
	bassGain = 1.0+(bassGain*fabs(bassGain)*fabs(bassGain));
	//separate from filtering stage, this is amplitude, centered on 1.0 unity gain
	
	//SmoothEQ3 is how to get 3rd order steepness at very low CPU.
	//because sample rate varies, you could also vary the crossovers
	//you can't vary Q because math is simplified to take advantage of
	//how the accurate Q value for this filter is always exactly 1.0.
	highFast[biq_freq] = (4000.0/getSampleRate());
	double omega = 2.0*M_PI*(4000.0/getSampleRate()); //mid-high crossover freq
	double biqK = 2.0 - cos(omega);
	double highCoef = -sqrt(biqK*biqK - 1.0) + biqK;
	lowFast[biq_freq] = (200.0/getSampleRate());
	omega = 2.0*M_PI*(200.0/getSampleRate()); //low-mid crossover freq
	biqK = 2.0 - cos(omega);
	double lowCoef = -sqrt(biqK*biqK - 1.0) + biqK;
	//exponential IIR filter as part of an accurate 3rd order Butterworth filter 
	biqK = tan(M_PI * highFast[biq_freq]);
	double norm = 1.0 / (1.0 + biqK + biqK*biqK);
	highFast[biq_a0] = biqK * biqK * norm;
	highFast[biq_a1] = 2.0 * highFast[biq_a0];
	highFast[biq_a2] = highFast[biq_a0];
	highFast[biq_b1] = 2.0 * (biqK*biqK - 1.0) * norm;
	highFast[biq_b2] = (1.0 - biqK + biqK*biqK) * norm;
	biqK = tan(M_PI * lowFast[biq_freq]);
	norm = 1.0 / (1.0 + biqK + biqK*biqK);
	lowFast[biq_a0] = biqK * biqK * norm;
	lowFast[biq_a1] = 2.0 * lowFast[biq_a0];
	lowFast[biq_a2] = lowFast[biq_a0];
	lowFast[biq_b1] = 2.0 * (biqK*biqK - 1.0) * norm;
	lowFast[biq_b2] = (1.0 - biqK + biqK*biqK) * norm;
	//custom biquad setup with Q = 1.0 gets to omit some divides
	//SmoothEQ3
	
	double bezCThresh = pow(1.0-THR, 6.0) * 8.0;
	double bezRez = pow(1.0-THR, 12.360679774997898) / overallscale;
	double sloRez = pow(1.0-THR,10.0) / overallscale;
	sloRez = fmin(fmax(sloRez,0.00001),1.0);
	bezRez = fmin(fmax(bezRez,0.00001),1.0);
	//Dynamics2
	
	highpass[hilp_freq] = ((pow(HIP,3)*24000.0)+10.0)/getSampleRate();
	if (highpass[hilp_freq] > 0.495) highpass[hilp_freq] = 0.495;
	bool highpassEngage = true; if (HIP == 0.0) highpassEngage = false;
	
	lowpass[hilp_freq] = ((pow(LOP,3)*24000.0)+10.0)/getSampleRate();
	if (lowpass[hilp_freq] > 0.495) lowpass[hilp_freq] = 0.495;
	bool lowpassEngage = true; if (LOP == 0.0) lowpassEngage = false;
	
	highpass[hilp_aA0] = highpass[hilp_aB0];
	highpass[hilp_aA1] = highpass[hilp_aB1];
	highpass[hilp_bA1] = highpass[hilp_bB1];
	highpass[hilp_bA2] = highpass[hilp_bB2];
	highpass[hilp_cA0] = highpass[hilp_cB0];
	highpass[hilp_cA1] = highpass[hilp_cB1];
	highpass[hilp_dA1] = highpass[hilp_dB1];
	highpass[hilp_dA2] = highpass[hilp_dB2];
	highpass[hilp_eA0] = highpass[hilp_eB0];
	highpass[hilp_eA1] = highpass[hilp_eB1];
	highpass[hilp_fA1] = highpass[hilp_fB1];
	highpass[hilp_fA2] = highpass[hilp_fB2];
	lowpass[hilp_aA0] = lowpass[hilp_aB0];
	lowpass[hilp_aA1] = lowpass[hilp_aB1];
	lowpass[hilp_bA1] = lowpass[hilp_bB1];
	lowpass[hilp_bA2] = lowpass[hilp_bB2];
	lowpass[hilp_cA0] = lowpass[hilp_cB0];
	lowpass[hilp_cA1] = lowpass[hilp_cB1];
	lowpass[hilp_dA1] = lowpass[hilp_dB1];
	lowpass[hilp_dA2] = lowpass[hilp_dB2];
	lowpass[hilp_eA0] = lowpass[hilp_eB0];
	lowpass[hilp_eA1] = lowpass[hilp_eB1];
	lowpass[hilp_fA1] = lowpass[hilp_fB1];
	lowpass[hilp_fA2] = lowpass[hilp_fB2];
	//previous run through the buffer is still in the filter, so we move it
	//to the A section and now it's the new starting point.
	//On the buss, highpass and lowpass are isolators meant to be moved,
	//so they are interpolated where the channels are not
	
	biqK = tan(M_PI * highpass[hilp_freq]); //highpass
	norm = 1.0 / (1.0 + biqK / 1.93185165 + biqK * biqK);
	highpass[hilp_aB0] = norm;
	highpass[hilp_aB1] = -2.0 * highpass[hilp_aB0];
	highpass[hilp_bB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highpass[hilp_bB2] = (1.0 - biqK / 1.93185165 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.70710678 + biqK * biqK);
	highpass[hilp_cB0] = norm;
	highpass[hilp_cB1] = -2.0 * highpass[hilp_cB0];
	highpass[hilp_dB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highpass[hilp_dB2] = (1.0 - biqK / 0.70710678 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.51763809 + biqK * biqK);
	highpass[hilp_eB0] = norm;
	highpass[hilp_eB1] = -2.0 * highpass[hilp_eB0];
	highpass[hilp_fB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	highpass[hilp_fB2] = (1.0 - biqK / 0.51763809 + biqK * biqK) * norm;
	
	biqK = tan(M_PI * lowpass[hilp_freq]); //lowpass
	norm = 1.0 / (1.0 + biqK / 1.93185165 + biqK * biqK);
	lowpass[hilp_aB0] = biqK * biqK * norm;
	lowpass[hilp_aB1] = 2.0 * lowpass[hilp_aB0];
	lowpass[hilp_bB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowpass[hilp_bB2] = (1.0 - biqK / 1.93185165 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.70710678 + biqK * biqK);
	lowpass[hilp_cB0] = biqK * biqK * norm;
	lowpass[hilp_cB1] = 2.0 * lowpass[hilp_cB0];
	lowpass[hilp_dB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowpass[hilp_dB2] = (1.0 - biqK / 0.70710678 + biqK * biqK) * norm;
	norm = 1.0 / (1.0 + biqK / 0.51763809 + biqK * biqK);
	lowpass[hilp_eB0] = biqK * biqK * norm;
	lowpass[hilp_eB1] = 2.0 * lowpass[hilp_eB0];
	lowpass[hilp_fB1] = 2.0 * (biqK * biqK - 1.0) * norm;
	lowpass[hilp_fB2] = (1.0 - biqK / 0.51763809 + biqK * biqK) * norm;
	
	if (highpass[hilp_aA0] == 0.0) { // if we have just started, start directly with raw info
		highpass[hilp_aA0] = highpass[hilp_aB0];
		highpass[hilp_aA1] = highpass[hilp_aB1];
		highpass[hilp_bA1] = highpass[hilp_bB1];
		highpass[hilp_bA2] = highpass[hilp_bB2];
		highpass[hilp_cA0] = highpass[hilp_cB0];
		highpass[hilp_cA1] = highpass[hilp_cB1];
		highpass[hilp_dA1] = highpass[hilp_dB1];
		highpass[hilp_dA2] = highpass[hilp_dB2];
		highpass[hilp_eA0] = highpass[hilp_eB0];
		highpass[hilp_eA1] = highpass[hilp_eB1];
		highpass[hilp_fA1] = highpass[hilp_fB1];
		highpass[hilp_fA2] = highpass[hilp_fB2];
		lowpass[hilp_aA0] = lowpass[hilp_aB0];
		lowpass[hilp_aA1] = lowpass[hilp_aB1];
		lowpass[hilp_bA1] = lowpass[hilp_bB1];
		lowpass[hilp_bA2] = lowpass[hilp_bB2];
		lowpass[hilp_cA0] = lowpass[hilp_cB0];
		lowpass[hilp_cA1] = lowpass[hilp_cB1];
		lowpass[hilp_dA1] = lowpass[hilp_dB1];
		lowpass[hilp_dA2] = lowpass[hilp_dB2];
		lowpass[hilp_eA0] = lowpass[hilp_eB0];
		lowpass[hilp_eA1] = lowpass[hilp_eB1];
		lowpass[hilp_fA1] = lowpass[hilp_fB1];
		lowpass[hilp_fA2] = lowpass[hilp_fB2];
	}
	
	panA = panB; panB = PAN*1.57079633;
	inTrimA = inTrimB; inTrimB = FAD*2.0;
	//Console
	
	while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		const double temp = (double)sampleFrames/inFramesToProcess;
		highpass[hilp_a0] = (highpass[hilp_aA0]*temp)+(highpass[hilp_aB0]*(1.0-temp));
		highpass[hilp_a1] = (highpass[hilp_aA1]*temp)+(highpass[hilp_aB1]*(1.0-temp));
		highpass[hilp_b1] = (highpass[hilp_bA1]*temp)+(highpass[hilp_bB1]*(1.0-temp));
		highpass[hilp_b2] = (highpass[hilp_bA2]*temp)+(highpass[hilp_bB2]*(1.0-temp));
		highpass[hilp_c0] = (highpass[hilp_cA0]*temp)+(highpass[hilp_cB0]*(1.0-temp));
		highpass[hilp_c1] = (highpass[hilp_cA1]*temp)+(highpass[hilp_cB1]*(1.0-temp));
		highpass[hilp_d1] = (highpass[hilp_dA1]*temp)+(highpass[hilp_dB1]*(1.0-temp));
		highpass[hilp_d2] = (highpass[hilp_dA2]*temp)+(highpass[hilp_dB2]*(1.0-temp));
		highpass[hilp_e0] = (highpass[hilp_eA0]*temp)+(highpass[hilp_eB0]*(1.0-temp));
		highpass[hilp_e1] = (highpass[hilp_eA1]*temp)+(highpass[hilp_eB1]*(1.0-temp));
		highpass[hilp_f1] = (highpass[hilp_fA1]*temp)+(highpass[hilp_fB1]*(1.0-temp));
		highpass[hilp_f2] = (highpass[hilp_fA2]*temp)+(highpass[hilp_fB2]*(1.0-temp));
		lowpass[hilp_a0] = (lowpass[hilp_aA0]*temp)+(lowpass[hilp_aB0]*(1.0-temp));
		lowpass[hilp_a1] = (lowpass[hilp_aA1]*temp)+(lowpass[hilp_aB1]*(1.0-temp));
		lowpass[hilp_b1] = (lowpass[hilp_bA1]*temp)+(lowpass[hilp_bB1]*(1.0-temp));
		lowpass[hilp_b2] = (lowpass[hilp_bA2]*temp)+(lowpass[hilp_bB2]*(1.0-temp));
		lowpass[hilp_c0] = (lowpass[hilp_cA0]*temp)+(lowpass[hilp_cB0]*(1.0-temp));
		lowpass[hilp_c1] = (lowpass[hilp_cA1]*temp)+(lowpass[hilp_cB1]*(1.0-temp));
		lowpass[hilp_d1] = (lowpass[hilp_dA1]*temp)+(lowpass[hilp_dB1]*(1.0-temp));
		lowpass[hilp_d2] = (lowpass[hilp_dA2]*temp)+(lowpass[hilp_dB2]*(1.0-temp));
		lowpass[hilp_e0] = (lowpass[hilp_eA0]*temp)+(lowpass[hilp_eB0]*(1.0-temp));
		lowpass[hilp_e1] = (lowpass[hilp_eA1]*temp)+(lowpass[hilp_eB1]*(1.0-temp));
		lowpass[hilp_f1] = (lowpass[hilp_fA1]*temp)+(lowpass[hilp_fB1]*(1.0-temp));
		lowpass[hilp_f2] = (lowpass[hilp_fA2]*temp)+(lowpass[hilp_fB2]*(1.0-temp));
		
		
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
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		else if (inputSampleL > 0.0) inputSampleL = -expm1((log1p(-inputSampleL) * 0.6180339887498949));
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		else if (inputSampleL < 0.0) inputSampleL = expm1((log1p(inputSampleL) * 0.6180339887498949));
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		else if (inputSampleR > 0.0) inputSampleR = -expm1((log1p(-inputSampleR) * 0.6180339887498949));
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		else if (inputSampleR < 0.0) inputSampleR = expm1((log1p(inputSampleR) * 0.6180339887498949));
		
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
		
		double trebleFastL = inputSampleL;		
		double outSample = (trebleFastL * highFast[biq_a0]) + highFast[biq_sL1];
		highFast[biq_sL1] = (trebleFastL * highFast[biq_a1]) - (outSample * highFast[biq_b1]) + highFast[biq_sL2];
		highFast[biq_sL2] = (trebleFastL * highFast[biq_a2]) - (outSample * highFast[biq_b2]);
		double midFastL = outSample; trebleFastL -= midFastL;
		outSample = (midFastL * lowFast[biq_a0]) + lowFast[biq_sL1];
		lowFast[biq_sL1] = (midFastL * lowFast[biq_a1]) - (outSample * lowFast[biq_b1]) + lowFast[biq_sL2];
		lowFast[biq_sL2] = (midFastL * lowFast[biq_a2]) - (outSample * lowFast[biq_b2]);
		double bassFastL = outSample; midFastL -= bassFastL;
		trebleFastL = (bassFastL*bassGain) + (midFastL*midGain) + (trebleFastL*trebleGain);
		//first stage of two crossovers is biquad of exactly 1.0 Q
		highFastLIIR = (highFastLIIR*highCoef) + (trebleFastL*(1.0-highCoef));
		midFastL = highFastLIIR; trebleFastL -= midFastL;
		lowFastLIIR = (lowFastLIIR*lowCoef) + (midFastL*(1.0-lowCoef));
		bassFastL = lowFastLIIR; midFastL -= bassFastL;
		inputSampleL = (bassFastL*bassGain) + (midFastL*midGain) + (trebleFastL*trebleGain);		
		//second stage of two crossovers is the exponential filters
		//this produces a slightly steeper Butterworth filter very cheaply
		
		double trebleFastR = inputSampleR;		
		outSample = (trebleFastR * highFast[biq_a0]) + highFast[biq_sR1];
		highFast[biq_sR1] = (trebleFastR * highFast[biq_a1]) - (outSample * highFast[biq_b1]) + highFast[biq_sR2];
		highFast[biq_sR2] = (trebleFastR * highFast[biq_a2]) - (outSample * highFast[biq_b2]);
		double midFastR = outSample; trebleFastR -= midFastR;
		outSample = (midFastR * lowFast[biq_a0]) + lowFast[biq_sR1];
		lowFast[biq_sR1] = (midFastR * lowFast[biq_a1]) - (outSample * lowFast[biq_b1]) + lowFast[biq_sR2];
		lowFast[biq_sR2] = (midFastR * lowFast[biq_a2]) - (outSample * lowFast[biq_b2]);
		double bassFastR = outSample; midFastR -= bassFastR;
		trebleFastR = (bassFastR*bassGain) + (midFastR*midGain) + (trebleFastR*trebleGain);
		//first stage of two crossovers is biquad of exactly 1.0 Q
		highFastRIIR = (highFastRIIR*highCoef) + (trebleFastR*(1.0-highCoef));
		midFastR = highFastRIIR; trebleFastR -= midFastR;
		lowFastRIIR = (lowFastRIIR*lowCoef) + (midFastR*(1.0-lowCoef));
		bassFastR = lowFastRIIR; midFastR -= bassFastR;
		inputSampleR = (bassFastR*bassGain) + (midFastR*midGain) + (trebleFastR*trebleGain);		
		//second stage of two crossovers is the exponential filters
		//this produces a slightly steeper Butterworth filter very cheaply
		//SmoothEQ3
		
		if (bezCThresh > 0.0) {
			inputSampleL *= ((bezCThresh*0.5)+1.0);
			inputSampleR *= ((bezCThresh*0.5)+1.0);
		}
		
		bezCompF[bez_cycle] += bezRez;
		bezCompF[bez_SampL] += (fabs(inputSampleL) * bezRez);
		bezCompF[bez_SampR] += (fabs(inputSampleR) * bezRez);
		bezMaxF = fmax(bezMaxF,fmax(fabs(inputSampleL),fabs(inputSampleR)));
		
		if (bezCompF[bez_cycle] > 1.0) {
			bezCompF[bez_cycle] -= 1.0;
			bezCompF[bez_CL] = bezCompF[bez_BL];
			bezCompF[bez_BL] = bezCompF[bez_AL];
			bezCompF[bez_AL] = bezCompF[bez_SampL];
			bezCompF[bez_SampL] = 0.0;
			bezCompF[bez_CR] = bezCompF[bez_BR];
			bezCompF[bez_BR] = bezCompF[bez_AR];
			bezCompF[bez_AR] = bezCompF[bez_SampR];
			bezCompF[bez_SampR] = 0.0;
			bezMaxF = 0.0;
		}
		bezCompS[bez_cycle] += sloRez;
		bezCompS[bez_SampL] += (fabs(inputSampleL) * sloRez); //note: SampL is a control voltage
		bezCompS[bez_SampR] += (fabs(inputSampleR) * sloRez); //note: SampR is a control voltage
		if (bezCompS[bez_cycle] > 1.0) {
			bezCompS[bez_cycle] -= 1.0;
			bezCompS[bez_CL] = bezCompS[bez_BL];
			bezCompS[bez_BL] = bezCompS[bez_AL];
			bezCompS[bez_AL] = bezCompS[bez_SampL];
			bezCompS[bez_SampL] = 0.0;
			bezCompS[bez_CR] = bezCompS[bez_BR];
			bezCompS[bez_BR] = bezCompS[bez_AR];
			bezCompS[bez_AR] = bezCompS[bez_SampR];
			bezCompS[bez_SampR] = 0.0;
		}
		double CBFL = (bezCompF[bez_CL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BL]*bezCompF[bez_cycle]);
		double BAFL = (bezCompF[bez_BL]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AL]*bezCompF[bez_cycle]);
		double CBAFL = (bezCompF[bez_BL]+(CBFL*(1.0-bezCompF[bez_cycle]))+(BAFL*bezCompF[bez_cycle]))*0.5;
		double CBSL = (bezCompS[bez_CL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BL]*bezCompS[bez_cycle]);
		double BASL = (bezCompS[bez_BL]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AL]*bezCompS[bez_cycle]);
		double CBASL = (bezCompS[bez_BL]+(CBSL*(1.0-bezCompS[bez_cycle]))+(BASL*bezCompS[bez_cycle]))*0.5;
		double CBAMax = fmax(CBASL,CBAFL); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		double CBAFade = ((CBASL*-CBAMax)+(CBAFL*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleL *= 1.0-(fmin(((CBASL*(1.0-CBAFade))+(CBAFL*CBAFade))*bezCThresh,1.0));
		
		double CBFR = (bezCompF[bez_CR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_BR]*bezCompF[bez_cycle]);
		double BAFR = (bezCompF[bez_BR]*(1.0-bezCompF[bez_cycle]))+(bezCompF[bez_AR]*bezCompF[bez_cycle]);
		double CBAFR = (bezCompF[bez_BR]+(CBFR*(1.0-bezCompF[bez_cycle]))+(BAFR*bezCompF[bez_cycle]))*0.5;
		double CBSR = (bezCompS[bez_CR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_BR]*bezCompS[bez_cycle]);
		double BASR = (bezCompS[bez_BR]*(1.0-bezCompS[bez_cycle]))+(bezCompS[bez_AR]*bezCompS[bez_cycle]);
		double CBASR = (bezCompS[bez_BR]+(CBSR*(1.0-bezCompS[bez_cycle]))+(BASR*bezCompS[bez_cycle]))*0.5;
		CBAMax = fmax(CBASR,CBAFR); if (CBAMax > 0.0) CBAMax = 1.0/CBAMax;
		CBAFade = ((CBASR*-CBAMax)+(CBAFR*CBAMax)+1.0)*0.5;
		if (bezCThresh > 0.0) inputSampleR *= 1.0-(fmin(((CBASR*(1.0-CBAFade))+(CBAFR*CBAFade))*bezCThresh,1.0));
		//Dynamics2
		
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
		
		double gainR = (panA*temp)+(panB*(1.0-temp));
		double gainL = 1.57079633-gainR;
		gainR = sin(gainR); gainL = sin(gainL);
		double gain = (inTrimA*temp)+(inTrimB*(1.0-temp));
		if (gain > 1.0) gain *= gain;
		if (gain < 1.0) gain = 1.0-pow(1.0-gain,2);
		gain *= 2.0;
		
		inputSampleL = inputSampleL * gainL * gain;
		inputSampleR = inputSampleR * gainR * gain;
		//applies pan section, and smoothed fader gain
		
		double darkSampleL = inputSampleL;
		double darkSampleR = inputSampleR;
		if (avgPos > 31) avgPos = 0;
		if (spacing > 31) {
			avg32L[avgPos] = darkSampleL; avg32R[avgPos] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 32; x++) {darkSampleL += avg32L[x]; darkSampleR += avg32R[x];}
			darkSampleL /= 32.0; darkSampleR /= 32.0;
		} if (spacing > 15) {
			avg16L[avgPos%16] = darkSampleL; avg16R[avgPos%16] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 16; x++) {darkSampleL += avg16L[x]; darkSampleR += avg16R[x];}
			darkSampleL /= 16.0; darkSampleR /= 16.0;
		} if (spacing > 7) {
			avg8L[avgPos%8] = darkSampleL; avg8R[avgPos%8] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 8; x++) {darkSampleL += avg8L[x]; darkSampleR += avg8R[x];}
			darkSampleL /= 8.0; darkSampleR /= 8.0;
		} if (spacing > 3) {
			avg4L[avgPos%4] = darkSampleL; avg4R[avgPos%4] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 4; x++) {darkSampleL += avg4L[x]; darkSampleR += avg4R[x];}
			darkSampleL /= 4.0; darkSampleR /= 4.0;
		} if (spacing > 1) {
			avg2L[avgPos%2] = darkSampleL; avg2R[avgPos%2] = darkSampleR;
			darkSampleL = 0.0; darkSampleR = 0.0;
			for (int x = 0; x < 2; x++) {darkSampleL += avg2L[x]; darkSampleR += avg2R[x];}
			darkSampleL /= 2.0; darkSampleR /= 2.0; 
		} avgPos++;
		lastSlewL += fabs(lastSlewpleL-inputSampleL); lastSlewpleL = inputSampleL;
		double avgSlewL = fmin(lastSlewL,1.0);
		lastSlewL = fmax(lastSlewL*0.78,2.39996322972865332223);
		lastSlewR += fabs(lastSlewpleR-inputSampleR); lastSlewpleR = inputSampleR;
		double avgSlewR = fmin(lastSlewR,1.0);
		lastSlewR = fmax(lastSlewR*0.78,2.39996322972865332223); //look up Golden Angle, it's cool
		inputSampleL = (inputSampleL*(1.0-avgSlewL)) + (darkSampleL*avgSlewL);
		inputSampleR = (inputSampleR*(1.0-avgSlewR)) + (darkSampleR*avgSlewR);
		
		//begin TapeHack section
		inputSampleL = fmax(fmin(inputSampleL,2.305929007734908),-2.305929007734908);
		double addtwo = inputSampleL * inputSampleL;
		double empower = inputSampleL * addtwo; // inputSampleL to the third power
		inputSampleL -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleL += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleL -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleL += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleL -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		inputSampleL *= 0.92;
		//end TapeHack section
		
		//begin TapeHack section
		inputSampleR = fmax(fmin(inputSampleR,2.305929007734908),-2.305929007734908);
		addtwo = inputSampleR * inputSampleR;
		empower = inputSampleR * addtwo; // inputSampleR to the third power
		inputSampleR -= (empower / 6.0);
		empower *= addtwo; // to the fifth power
		inputSampleR += (empower / 69.0);
		empower *= addtwo; //seventh
		inputSampleR -= (empower / 2530.08);
		empower *= addtwo; //ninth
		inputSampleR += (empower / 224985.6);
		empower *= addtwo; //eleventh
		inputSampleR -= (empower / 9979200.0f);
		//this is a degenerate form of a Taylor Series to approximate sin()
		inputSampleR *= 0.92;
		//end TapeHack section		
		
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
