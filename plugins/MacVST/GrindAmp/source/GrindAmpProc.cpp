/* ========================================
 *  GrindAmp - GrindAmp.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#include "GrindAmp.h"
#endif

void GrindAmp::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check		
	
	double inputlevel = pow(A,2);
	double samplerate = getSampleRate();
	double trimEQ = 1.1-B;
	double toneEQ = trimEQ/1.2;
	trimEQ /= 50.0;
	trimEQ += 0.165;
	double EQ = ((trimEQ-(toneEQ/6.1)) / samplerate)*22050.0;
	double BEQ = ((trimEQ+(toneEQ/2.1)) / samplerate)*22050.0;
	double outputlevel = C;
	double wet = D;
	double bassdrive = 1.57079633*(2.5-toneEQ);
	
	double cutoff = (18000.0+(B*1000.0)) / getSampleRate();
	if (cutoff > 0.49) cutoff = 0.49; //don't crash if run at 44.1k
	if (cutoff < 0.001) cutoff = 0.001; //or if cutoff's too low
	
	fixF[fix_freq] = fixE[fix_freq] = fixD[fix_freq] = fixC[fix_freq] = fixB[fix_freq] = fixA[fix_freq] = cutoff;
	
    fixA[fix_reso] = 4.46570214;
	fixB[fix_reso] = 1.51387132;
	fixC[fix_reso] = 0.93979296;
	fixD[fix_reso] = 0.70710678;
	fixE[fix_reso] = 0.52972649;
	fixF[fix_reso] = 0.50316379;
	
	double K = tan(M_PI * fixA[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fixA[fix_reso] + K * K);
	fixA[fix_a0] = K * K * norm;
	fixA[fix_a1] = 2.0 * fixA[fix_a0];
	fixA[fix_a2] = fixA[fix_a0];
	fixA[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixA[fix_b2] = (1.0 - K / fixA[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixB[fix_freq]);
	norm = 1.0 / (1.0 + K / fixB[fix_reso] + K * K);
	fixB[fix_a0] = K * K * norm;
	fixB[fix_a1] = 2.0 * fixB[fix_a0];
	fixB[fix_a2] = fixB[fix_a0];
	fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixB[fix_b2] = (1.0 - K / fixB[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixC[fix_freq]);
	norm = 1.0 / (1.0 + K / fixC[fix_reso] + K * K);
	fixC[fix_a0] = K * K * norm;
	fixC[fix_a1] = 2.0 * fixC[fix_a0];
	fixC[fix_a2] = fixC[fix_a0];
	fixC[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixC[fix_b2] = (1.0 - K / fixC[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixD[fix_freq]);
	norm = 1.0 / (1.0 + K / fixD[fix_reso] + K * K);
	fixD[fix_a0] = K * K * norm;
	fixD[fix_a1] = 2.0 * fixD[fix_a0];
	fixD[fix_a2] = fixD[fix_a0];
	fixD[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixD[fix_b2] = (1.0 - K / fixD[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixE[fix_freq]);
	norm = 1.0 / (1.0 + K / fixE[fix_reso] + K * K);
	fixE[fix_a0] = K * K * norm;
	fixE[fix_a1] = 2.0 * fixE[fix_a0];
	fixE[fix_a2] = fixE[fix_a0];
	fixE[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixE[fix_b2] = (1.0 - K / fixE[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixF[fix_freq]);
	norm = 1.0 / (1.0 + K / fixF[fix_reso] + K * K);
	fixF[fix_a0] = K * K * norm;
	fixF[fix_a1] = 2.0 * fixF[fix_a0];
	fixF[fix_a2] = fixF[fix_a0];
	fixF[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixF[fix_b2] = (1.0 - K / fixF[fix_reso] + K * K) * norm;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double outSample = (inputSampleL * fixA[fix_a0]) + fixA[fix_sL1];
		fixA[fix_sL1] = (inputSampleL * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sL2];
		fixA[fix_sL2] = (inputSampleL * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixA[fix_a0]) + fixA[fix_sR1];
		fixA[fix_sR1] = (inputSampleR * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sR2];
		fixA[fix_sR2] = (inputSampleR * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		iirSampleAL = (iirSampleAL * (1.0 - EQ)) + (inputSampleL * EQ);
		inputSampleL = inputSampleL - (iirSampleAL*0.92);
		//highpass
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		double bridgerectifier = fabs(inputSampleL);
		double inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothAL + (secondAL*inverse) + (thirdAL*bridgerectifier) + inputSampleL);
		thirdAL = secondAL;
		secondAL = smoothAL;
		smoothAL = inputSampleL;
		double basscatchL = inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		inputSampleR *= inputlevel;
		iirSampleAR = (iirSampleAR * (1.0 - EQ)) + (inputSampleR * EQ);
		inputSampleR = inputSampleR - (iirSampleAR*0.92);
		//highpass
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		bridgerectifier = fabs(inputSampleR);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothAR + (secondAR*inverse) + (thirdAR*bridgerectifier) + inputSampleR);
		thirdAR = secondAR;
		secondAR = smoothAR;
		smoothAR = inputSampleR;
		double basscatchR = inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSampleL * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSampleL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSampleL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixB[fix_a0]) + fixB[fix_sR1];
		fixB[fix_sR1] = (inputSampleR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
		fixB[fix_sR2] = (inputSampleR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		iirSampleBL = (iirSampleBL * (1.0 - EQ)) + (inputSampleL * EQ);
		inputSampleL = inputSampleL - (iirSampleBL*0.79);
		//highpass
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		bridgerectifier = fabs(inputSampleL);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothBL + (secondBL*inverse) + (thirdBL*bridgerectifier) + inputSampleL);
		thirdBL = secondBL;
		secondBL = smoothBL;
		smoothBL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		inputSampleR *= inputlevel;
		iirSampleBR = (iirSampleBR * (1.0 - EQ)) + (inputSampleR * EQ);
		inputSampleR = inputSampleR - (iirSampleBR*0.79);
		//highpass
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		bridgerectifier = fabs(inputSampleR);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothBR + (secondBR*inverse) + (thirdBR*bridgerectifier) + inputSampleR);
		thirdBR = secondBR;
		secondBR = smoothBR;
		smoothBR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleCL = (iirSampleCL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleCL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothCL + (secondCL*inverse) + (thirdCL*bridgerectifier) + inputSampleL);
		thirdCL = secondCL;
		secondCL = smoothCL;
		smoothCL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass

		iirSampleCR = (iirSampleCR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleCR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothCR + (secondCR*inverse) + (thirdCR*bridgerectifier) + inputSampleR);
		thirdCR = secondCR;
		secondCR = smoothCR;
		smoothCR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSampleL * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSampleL * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSampleL * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixC[fix_a0]) + fixC[fix_sR1];
		fixC[fix_sR1] = (inputSampleR * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sR2];
		fixC[fix_sR2] = (inputSampleR * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleDL = (iirSampleDL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleDL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothDL + (secondDL*inverse) + (thirdDL*bridgerectifier) + inputSampleL);
		thirdDL = secondDL;
		secondDL = smoothDL;
		smoothDL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleDR = (iirSampleDR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleDR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothDR + (secondDR*inverse) + (thirdDR*bridgerectifier) + inputSampleR);
		thirdDR = secondDR;
		secondDR = smoothDR;
		smoothDR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSampleL * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSampleL * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSampleL * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixD[fix_a0]) + fixD[fix_sR1];
		fixD[fix_sR1] = (inputSampleR * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sR2];
		fixD[fix_sR2] = (inputSampleR * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleEL = (iirSampleEL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleEL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothEL + (secondEL*inverse) + (thirdEL*bridgerectifier) + inputSampleL);
		thirdEL = secondEL;
		secondEL = smoothEL;
		smoothEL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleER = (iirSampleER * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleER*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothER + (secondER*inverse) + (thirdER*bridgerectifier) + inputSampleR);
		thirdER = secondER;
		secondER = smoothER;
		smoothER = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleFL = (iirSampleFL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleFL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothFL + (secondFL*inverse) + (thirdFL*bridgerectifier) + inputSampleL);
		thirdFL = secondFL;
		secondFL = smoothFL;
		smoothFL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleFR = (iirSampleFR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleFR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothFR + (secondFR*inverse) + (thirdFR*bridgerectifier) + inputSampleR);
		thirdFR = secondFR;
		secondFR = smoothFR;
		smoothFR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSampleL * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSampleL * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSampleL * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixE[fix_a0]) + fixE[fix_sR1];
		fixE[fix_sR1] = (inputSampleR * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sR2];
		fixE[fix_sR2] = (inputSampleR * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleGL = (iirSampleGL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleGL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothGL + (secondGL*inverse) + (thirdGL*bridgerectifier) + inputSampleL);
		thirdGL = secondGL;
		secondGL = smoothGL;
		smoothGL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleGR = (iirSampleGR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleGR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothGR + (secondGR*inverse) + (thirdGR*bridgerectifier) + inputSampleR);
		thirdGR = secondGR;
		secondGR = smoothGR;
		smoothGR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleHL = (iirSampleHL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleHL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothHL + (secondHL*inverse) + (thirdHL*bridgerectifier) + inputSampleL);
		thirdHL = secondHL;
		secondHL = smoothHL;
		smoothHL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleHR = (iirSampleHR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleHR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothHR + (secondHR*inverse) + (thirdHR*bridgerectifier) + inputSampleR);
		thirdHR = secondHR;
		secondHR = smoothHR;
		smoothHR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSampleL * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSampleL * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSampleL * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixF[fix_a0]) + fixF[fix_sR1];
		fixF[fix_sR1] = (inputSampleR * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sR2];
		fixF[fix_sR2] = (inputSampleR * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleIL = (iirSampleIL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleIL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothIL + (secondIL*inverse) + (thirdIL*bridgerectifier) + inputSampleL);
		thirdIL = secondIL;
		secondIL = smoothIL;
		smoothIL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		bridgerectifier = fabs(inputSampleL);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothJL + (secondJL*inverse) + (thirdJL*bridgerectifier) + inputSampleL);
		thirdJL = secondJL;
		secondJL = smoothJL;
		smoothJL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		bridgerectifier = fabs(inputSampleL);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothKL + (secondKL*inverse) + (thirdKL*bridgerectifier) + inputSampleL);
		thirdKL = secondKL;
		secondKL = smoothKL;
		smoothKL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleIR = (iirSampleIR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleIR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothIR + (secondIR*inverse) + (thirdIR*bridgerectifier) + inputSampleR);
		thirdIR = secondIR;
		secondIR = smoothIR;
		smoothIR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		bridgerectifier = fabs(inputSampleR);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothJR + (secondJR*inverse) + (thirdJR*bridgerectifier) + inputSampleR);
		thirdJR = secondJR;
		secondJR = smoothJR;
		smoothJR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		bridgerectifier = fabs(inputSampleR);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothKR + (secondKR*inverse) + (thirdKR*bridgerectifier) + inputSampleR);
		thirdKR = secondKR;
		secondKR = smoothKR;
		smoothKR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		basscatchL /= 2.0;
		inputSampleL = (inputSampleL*toneEQ)+basscatchL;
		//extra lowpass for 4*12" speakers		
		basscatchR /= 2.0;
		inputSampleR = (inputSampleR*toneEQ)+basscatchR;
		//extra lowpass for 4*12" speakers		
		
		bridgerectifier = fabs(inputSampleL*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		inputSampleL += basscatchL;
		//split bass between overdrive and clean
		inputSampleL /= (1.0+toneEQ);
		
		bridgerectifier = fabs(inputSampleR*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		inputSampleR += basscatchR;
		//split bass between overdrive and clean
		inputSampleR /= (1.0+toneEQ);
		
		double randy = ((double(fpdL)/UINT32_MAX)*0.061);
		inputSampleL = ((inputSampleL*(1-randy))+(storeSampleL*randy))*outputlevel;
		storeSampleL = inputSampleL;
		
		randy = ((double(fpdR)/UINT32_MAX)*0.061);
		inputSampleR = ((inputSampleR*(1-randy))+(storeSampleR*randy))*outputlevel;
		storeSampleR = inputSampleR;
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		//amp

		cycle++;
		if (cycle == cycleEnd) {
			
			double temp = (inputSampleL + smoothCabAL)/3.0;
			smoothCabAL = inputSampleL;
			inputSampleL = temp;
			
			bL[83] = bL[82]; bL[82] = bL[81]; bL[81] = bL[80]; bL[80] = bL[79]; 
			bL[79] = bL[78]; bL[78] = bL[77]; bL[77] = bL[76]; bL[76] = bL[75]; bL[75] = bL[74]; bL[74] = bL[73]; bL[73] = bL[72]; bL[72] = bL[71]; 
			bL[71] = bL[70]; bL[70] = bL[69]; bL[69] = bL[68]; bL[68] = bL[67]; bL[67] = bL[66]; bL[66] = bL[65]; bL[65] = bL[64]; bL[64] = bL[63]; 
			bL[63] = bL[62]; bL[62] = bL[61]; bL[61] = bL[60]; bL[60] = bL[59]; bL[59] = bL[58]; bL[58] = bL[57]; bL[57] = bL[56]; bL[56] = bL[55]; 
			bL[55] = bL[54]; bL[54] = bL[53]; bL[53] = bL[52]; bL[52] = bL[51]; bL[51] = bL[50]; bL[50] = bL[49]; bL[49] = bL[48]; bL[48] = bL[47]; 
			bL[47] = bL[46]; bL[46] = bL[45]; bL[45] = bL[44]; bL[44] = bL[43]; bL[43] = bL[42]; bL[42] = bL[41]; bL[41] = bL[40]; bL[40] = bL[39]; 
			bL[39] = bL[38]; bL[38] = bL[37]; bL[37] = bL[36]; bL[36] = bL[35]; bL[35] = bL[34]; bL[34] = bL[33]; bL[33] = bL[32]; bL[32] = bL[31]; 
			bL[31] = bL[30]; bL[30] = bL[29]; bL[29] = bL[28]; bL[28] = bL[27]; bL[27] = bL[26]; bL[26] = bL[25]; bL[25] = bL[24]; bL[24] = bL[23]; 
			bL[23] = bL[22]; bL[22] = bL[21]; bL[21] = bL[20]; bL[20] = bL[19]; bL[19] = bL[18]; bL[18] = bL[17]; bL[17] = bL[16]; bL[16] = bL[15]; 
			bL[15] = bL[14]; bL[14] = bL[13]; bL[13] = bL[12]; bL[12] = bL[11]; bL[11] = bL[10]; bL[10] = bL[9]; bL[9] = bL[8]; bL[8] = bL[7]; 
			bL[7] = bL[6]; bL[6] = bL[5]; bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1]; bL[1] = bL[0]; bL[0] = inputSampleL;
			inputSampleL += (bL[1] * (1.29550481610475132  + (0.19713872057074355*fabs(bL[1]))));
			inputSampleL += (bL[2] * (1.42302569895462616  + (0.30599505521284787*fabs(bL[2]))));
			inputSampleL += (bL[3] * (1.28728195804197565  + (0.23168333460446133*fabs(bL[3]))));
			inputSampleL += (bL[4] * (0.88553784290822690  + (0.14263256172918892*fabs(bL[4]))));
			inputSampleL += (bL[5] * (0.37129054918432319  + (0.00150040944205920*fabs(bL[5]))));
			inputSampleL -= (bL[6] * (0.12150959412556320  + (0.32776273620569107*fabs(bL[6]))));
			inputSampleL -= (bL[7] * (0.44900065463203775  + (0.74101214925298819*fabs(bL[7]))));
			inputSampleL -= (bL[8] * (0.54058781908186482  + (1.07821707459008387*fabs(bL[8]))));
			inputSampleL -= (bL[9] * (0.49361966401791391  + (1.23540109014850508*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.39819495093078133  + (1.11247213730917749*fabs(bL[10]))));
			inputSampleL -= (bL[11] * (0.31379279985435521  + (0.80330360359638298*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.30744359242808555  + (0.42132528876858205*fabs(bL[12]))));
			inputSampleL -= (bL[13] * (0.33943170284673974  + (0.09183418349389982*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.33838775119286391  - (0.06453051658561271*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.30682305697961665  - (0.09549380253249232*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.23408741339295336  - (0.08083404732361277*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.10411746814025019  + (0.00253651281245780*fabs(bL[17]))));
			inputSampleL += (bL[18] * (0.00133623776084696  - (0.04447267870865820*fabs(bL[18]))));
			inputSampleL += (bL[19] * (0.02461903992114161  + (0.07530671732655550*fabs(bL[19]))));
			inputSampleL += (bL[20] * (0.02086715842475373  + (0.22795860236804899*fabs(bL[20]))));
			inputSampleL += (bL[21] * (0.02761433637100917  + (0.26108320417844094*fabs(bL[21]))));
			inputSampleL += (bL[22] * (0.04475285369162533  + (0.19160705011061663*fabs(bL[22]))));
			inputSampleL += (bL[23] * (0.09447338372862381  + (0.03681550508743799*fabs(bL[23]))));
			inputSampleL += (bL[24] * (0.13445890343722280  - (0.13713036462146147*fabs(bL[24]))));
			inputSampleL += (bL[25] * (0.13872868945088121  - (0.22401242373298191*fabs(bL[25]))));
			inputSampleL += (bL[26] * (0.14915650097434549  - (0.26718804981526367*fabs(bL[26]))));
			inputSampleL += (bL[27] * (0.12766643217091783  - (0.27745664795660430*fabs(bL[27]))));
			inputSampleL += (bL[28] * (0.03675849788393101  - (0.18338278173550679*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.06307306864232835  + (0.06089480869040766*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.14947389348962944  + (0.04642103054798480*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.25235266566401526  + (0.08423062596460507*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.33496344048679683  + (0.09808328256677995*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.36590030482175445  + (0.10622650888958179*fabs(bL[33]))));
			inputSampleL -= (bL[34] * (0.35015197011464372  + (0.08982043516016047*fabs(bL[34]))));
			inputSampleL -= (bL[35] * (0.26808437585665090  + (0.00735561860229533*fabs(bL[35]))));
			inputSampleL -= (bL[36] * (0.11624318543291220  - (0.07142484314510467*fabs(bL[36]))));
			inputSampleL += (bL[37] * (0.05617084165377551  + (0.11785854050350089*fabs(bL[37]))));
			inputSampleL += (bL[38] * (0.20540028692589385  + (0.20479174663329586*fabs(bL[38]))));
			inputSampleL += (bL[39] * (0.30455415003043818  + (0.29074864580096849*fabs(bL[39]))));
			inputSampleL += (bL[40] * (0.33810750937829476  + (0.29182307921316802*fabs(bL[40]))));
			inputSampleL += (bL[41] * (0.31936133365277430  + (0.26535537727394987*fabs(bL[41]))));
			inputSampleL += (bL[42] * (0.27388548321981876  + (0.19735049990538350*fabs(bL[42]))));
			inputSampleL += (bL[43] * (0.21454597517994098  + (0.06415909270247236*fabs(bL[43]))));
			inputSampleL += (bL[44] * (0.15001045817707717  - (0.03831118543404573*fabs(bL[44]))));
			inputSampleL += (bL[45] * (0.07283437284653138  - (0.09281952429543777*fabs(bL[45]))));
			inputSampleL -= (bL[46] * (0.03917872184241358  + (0.14306291461398810*fabs(bL[46]))));
			inputSampleL -= (bL[47] * (0.16695932032148642  + (0.19138995946950504*fabs(bL[47]))));
			inputSampleL -= (bL[48] * (0.27055854466909462  + (0.22531296466343192*fabs(bL[48]))));
			inputSampleL -= (bL[49] * (0.33256357307578271  + (0.23305840475692102*fabs(bL[49]))));
			inputSampleL -= (bL[50] * (0.33459770116834442  + (0.24091822618917569*fabs(bL[50]))));
			inputSampleL -= (bL[51] * (0.27156687236338090  + (0.24062938573512443*fabs(bL[51]))));
			inputSampleL -= (bL[52] * (0.17197093288412094  + (0.19083085091993421*fabs(bL[52]))));
			inputSampleL -= (bL[53] * (0.06738628195910543  + (0.10268609751019808*fabs(bL[53]))));
			inputSampleL += (bL[54] * (0.00222429218204290  + (0.01439664435720548*fabs(bL[54]))));
			inputSampleL += (bL[55] * (0.01346992803494091  + (0.15947137113534526*fabs(bL[55]))));
			inputSampleL -= (bL[56] * (0.02038911881377448  - (0.26763170752416160*fabs(bL[56]))));
			inputSampleL -= (bL[57] * (0.08233579178189687  - (0.29415931086406055*fabs(bL[57]))));
			inputSampleL -= (bL[58] * (0.15447855089824883  - (0.26489186990840807*fabs(bL[58]))));
			inputSampleL -= (bL[59] * (0.20518281113362655  - (0.16135382257522859*fabs(bL[59]))));
			inputSampleL -= (bL[60] * (0.22244686050232007  + (0.00847180390247432*fabs(bL[60]))));
			inputSampleL -= (bL[61] * (0.21849243134998034  + (0.14460595245753741*fabs(bL[61]))));
			inputSampleL -= (bL[62] * (0.20256105734574054  + (0.18932793221831667*fabs(bL[62]))));
			inputSampleL -= (bL[63] * (0.18604070054295399  + (0.17250665610927965*fabs(bL[63]))));
			inputSampleL -= (bL[64] * (0.17222844322058231  + (0.12992472027850357*fabs(bL[64]))));
			inputSampleL -= (bL[65] * (0.14447856616566443  + (0.09089219002147308*fabs(bL[65]))));
			inputSampleL -= (bL[66] * (0.10385520794251019  + (0.08600465834570559*fabs(bL[66]))));
			inputSampleL -= (bL[67] * (0.07124435678265063  + (0.09071532210549428*fabs(bL[67]))));
			inputSampleL -= (bL[68] * (0.05216857461197572  + (0.06794061706070262*fabs(bL[68]))));
			inputSampleL -= (bL[69] * (0.05235381920184123  + (0.02818101717909346*fabs(bL[69]))));
			inputSampleL -= (bL[70] * (0.07569701245553526  - (0.00634228544764946*fabs(bL[70]))));
			inputSampleL -= (bL[71] * (0.10320125382718826  - (0.02751486906644141*fabs(bL[71]))));
			inputSampleL -= (bL[72] * (0.12122120969079088  - (0.05434007312178933*fabs(bL[72]))));
			inputSampleL -= (bL[73] * (0.13438969117200902  - (0.09135218559713874*fabs(bL[73]))));
			inputSampleL -= (bL[74] * (0.13534390437529981  - (0.10437672041458675*fabs(bL[74]))));
			inputSampleL -= (bL[75] * (0.11424128854188388  - (0.08693450726462598*fabs(bL[75]))));
			inputSampleL -= (bL[76] * (0.08166894518596159  - (0.06949989431475120*fabs(bL[76]))));
			inputSampleL -= (bL[77] * (0.04293976378555305  - (0.05718625137421843*fabs(bL[77]))));
			inputSampleL += (bL[78] * (0.00933076320644409  + (0.01728285211520138*fabs(bL[78]))));
			inputSampleL += (bL[79] * (0.06450430362918153  - (0.02492994833691022*fabs(bL[79]))));
			inputSampleL += (bL[80] * (0.10187400687649277  - (0.03578455940532403*fabs(bL[80]))));
			inputSampleL += (bL[81] * (0.11039763294094571  - (0.03995523517573508*fabs(bL[81]))));
			inputSampleL += (bL[82] * (0.08557960776024547  - (0.03482514309492527*fabs(bL[82]))));
			inputSampleL += (bL[83] * (0.02730881850805332  - (0.00514750108411127*fabs(bL[83]))));
			
			temp = (inputSampleL + smoothCabBL)/3.0;
			smoothCabBL = inputSampleL;
			inputSampleL = temp/4.0;
			
			randy = ((double(fpdL)/UINT32_MAX)*0.044);
			drySampleL = ((((inputSampleL*(1-randy))+(lastCabSampleL*randy))*wet)+(drySampleL*(1.0-wet)))*outputlevel;
			lastCabSampleL = inputSampleL;
			inputSampleL = drySampleL; //cab L
			
			temp = (inputSampleR + smoothCabAR)/3.0;
			smoothCabAR = inputSampleR;
			inputSampleR = temp;
			
			bR[83] = bR[82]; bR[82] = bR[81]; bR[81] = bR[80]; bR[80] = bR[79]; 
			bR[79] = bR[78]; bR[78] = bR[77]; bR[77] = bR[76]; bR[76] = bR[75]; bR[75] = bR[74]; bR[74] = bR[73]; bR[73] = bR[72]; bR[72] = bR[71]; 
			bR[71] = bR[70]; bR[70] = bR[69]; bR[69] = bR[68]; bR[68] = bR[67]; bR[67] = bR[66]; bR[66] = bR[65]; bR[65] = bR[64]; bR[64] = bR[63]; 
			bR[63] = bR[62]; bR[62] = bR[61]; bR[61] = bR[60]; bR[60] = bR[59]; bR[59] = bR[58]; bR[58] = bR[57]; bR[57] = bR[56]; bR[56] = bR[55]; 
			bR[55] = bR[54]; bR[54] = bR[53]; bR[53] = bR[52]; bR[52] = bR[51]; bR[51] = bR[50]; bR[50] = bR[49]; bR[49] = bR[48]; bR[48] = bR[47]; 
			bR[47] = bR[46]; bR[46] = bR[45]; bR[45] = bR[44]; bR[44] = bR[43]; bR[43] = bR[42]; bR[42] = bR[41]; bR[41] = bR[40]; bR[40] = bR[39]; 
			bR[39] = bR[38]; bR[38] = bR[37]; bR[37] = bR[36]; bR[36] = bR[35]; bR[35] = bR[34]; bR[34] = bR[33]; bR[33] = bR[32]; bR[32] = bR[31]; 
			bR[31] = bR[30]; bR[30] = bR[29]; bR[29] = bR[28]; bR[28] = bR[27]; bR[27] = bR[26]; bR[26] = bR[25]; bR[25] = bR[24]; bR[24] = bR[23]; 
			bR[23] = bR[22]; bR[22] = bR[21]; bR[21] = bR[20]; bR[20] = bR[19]; bR[19] = bR[18]; bR[18] = bR[17]; bR[17] = bR[16]; bR[16] = bR[15]; 
			bR[15] = bR[14]; bR[14] = bR[13]; bR[13] = bR[12]; bR[12] = bR[11]; bR[11] = bR[10]; bR[10] = bR[9]; bR[9] = bR[8]; bR[8] = bR[7]; 
			bR[7] = bR[6]; bR[6] = bR[5]; bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1]; bR[1] = bR[0]; bR[0] = inputSampleR;
			inputSampleR += (bR[1] * (1.29550481610475132  + (0.19713872057074355*fabs(bR[1]))));
			inputSampleR += (bR[2] * (1.42302569895462616  + (0.30599505521284787*fabs(bR[2]))));
			inputSampleR += (bR[3] * (1.28728195804197565  + (0.23168333460446133*fabs(bR[3]))));
			inputSampleR += (bR[4] * (0.88553784290822690  + (0.14263256172918892*fabs(bR[4]))));
			inputSampleR += (bR[5] * (0.37129054918432319  + (0.00150040944205920*fabs(bR[5]))));
			inputSampleR -= (bR[6] * (0.12150959412556320  + (0.32776273620569107*fabs(bR[6]))));
			inputSampleR -= (bR[7] * (0.44900065463203775  + (0.74101214925298819*fabs(bR[7]))));
			inputSampleR -= (bR[8] * (0.54058781908186482  + (1.07821707459008387*fabs(bR[8]))));
			inputSampleR -= (bR[9] * (0.49361966401791391  + (1.23540109014850508*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.39819495093078133  + (1.11247213730917749*fabs(bR[10]))));
			inputSampleR -= (bR[11] * (0.31379279985435521  + (0.80330360359638298*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.30744359242808555  + (0.42132528876858205*fabs(bR[12]))));
			inputSampleR -= (bR[13] * (0.33943170284673974  + (0.09183418349389982*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.33838775119286391  - (0.06453051658561271*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.30682305697961665  - (0.09549380253249232*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.23408741339295336  - (0.08083404732361277*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.10411746814025019  + (0.00253651281245780*fabs(bR[17]))));
			inputSampleR += (bR[18] * (0.00133623776084696  - (0.04447267870865820*fabs(bR[18]))));
			inputSampleR += (bR[19] * (0.02461903992114161  + (0.07530671732655550*fabs(bR[19]))));
			inputSampleR += (bR[20] * (0.02086715842475373  + (0.22795860236804899*fabs(bR[20]))));
			inputSampleR += (bR[21] * (0.02761433637100917  + (0.26108320417844094*fabs(bR[21]))));
			inputSampleR += (bR[22] * (0.04475285369162533  + (0.19160705011061663*fabs(bR[22]))));
			inputSampleR += (bR[23] * (0.09447338372862381  + (0.03681550508743799*fabs(bR[23]))));
			inputSampleR += (bR[24] * (0.13445890343722280  - (0.13713036462146147*fabs(bR[24]))));
			inputSampleR += (bR[25] * (0.13872868945088121  - (0.22401242373298191*fabs(bR[25]))));
			inputSampleR += (bR[26] * (0.14915650097434549  - (0.26718804981526367*fabs(bR[26]))));
			inputSampleR += (bR[27] * (0.12766643217091783  - (0.27745664795660430*fabs(bR[27]))));
			inputSampleR += (bR[28] * (0.03675849788393101  - (0.18338278173550679*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.06307306864232835  + (0.06089480869040766*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.14947389348962944  + (0.04642103054798480*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.25235266566401526  + (0.08423062596460507*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.33496344048679683  + (0.09808328256677995*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.36590030482175445  + (0.10622650888958179*fabs(bR[33]))));
			inputSampleR -= (bR[34] * (0.35015197011464372  + (0.08982043516016047*fabs(bR[34]))));
			inputSampleR -= (bR[35] * (0.26808437585665090  + (0.00735561860229533*fabs(bR[35]))));
			inputSampleR -= (bR[36] * (0.11624318543291220  - (0.07142484314510467*fabs(bR[36]))));
			inputSampleR += (bR[37] * (0.05617084165377551  + (0.11785854050350089*fabs(bR[37]))));
			inputSampleR += (bR[38] * (0.20540028692589385  + (0.20479174663329586*fabs(bR[38]))));
			inputSampleR += (bR[39] * (0.30455415003043818  + (0.29074864580096849*fabs(bR[39]))));
			inputSampleR += (bR[40] * (0.33810750937829476  + (0.29182307921316802*fabs(bR[40]))));
			inputSampleR += (bR[41] * (0.31936133365277430  + (0.26535537727394987*fabs(bR[41]))));
			inputSampleR += (bR[42] * (0.27388548321981876  + (0.19735049990538350*fabs(bR[42]))));
			inputSampleR += (bR[43] * (0.21454597517994098  + (0.06415909270247236*fabs(bR[43]))));
			inputSampleR += (bR[44] * (0.15001045817707717  - (0.03831118543404573*fabs(bR[44]))));
			inputSampleR += (bR[45] * (0.07283437284653138  - (0.09281952429543777*fabs(bR[45]))));
			inputSampleR -= (bR[46] * (0.03917872184241358  + (0.14306291461398810*fabs(bR[46]))));
			inputSampleR -= (bR[47] * (0.16695932032148642  + (0.19138995946950504*fabs(bR[47]))));
			inputSampleR -= (bR[48] * (0.27055854466909462  + (0.22531296466343192*fabs(bR[48]))));
			inputSampleR -= (bR[49] * (0.33256357307578271  + (0.23305840475692102*fabs(bR[49]))));
			inputSampleR -= (bR[50] * (0.33459770116834442  + (0.24091822618917569*fabs(bR[50]))));
			inputSampleR -= (bR[51] * (0.27156687236338090  + (0.24062938573512443*fabs(bR[51]))));
			inputSampleR -= (bR[52] * (0.17197093288412094  + (0.19083085091993421*fabs(bR[52]))));
			inputSampleR -= (bR[53] * (0.06738628195910543  + (0.10268609751019808*fabs(bR[53]))));
			inputSampleR += (bR[54] * (0.00222429218204290  + (0.01439664435720548*fabs(bR[54]))));
			inputSampleR += (bR[55] * (0.01346992803494091  + (0.15947137113534526*fabs(bR[55]))));
			inputSampleR -= (bR[56] * (0.02038911881377448  - (0.26763170752416160*fabs(bR[56]))));
			inputSampleR -= (bR[57] * (0.08233579178189687  - (0.29415931086406055*fabs(bR[57]))));
			inputSampleR -= (bR[58] * (0.15447855089824883  - (0.26489186990840807*fabs(bR[58]))));
			inputSampleR -= (bR[59] * (0.20518281113362655  - (0.16135382257522859*fabs(bR[59]))));
			inputSampleR -= (bR[60] * (0.22244686050232007  + (0.00847180390247432*fabs(bR[60]))));
			inputSampleR -= (bR[61] * (0.21849243134998034  + (0.14460595245753741*fabs(bR[61]))));
			inputSampleR -= (bR[62] * (0.20256105734574054  + (0.18932793221831667*fabs(bR[62]))));
			inputSampleR -= (bR[63] * (0.18604070054295399  + (0.17250665610927965*fabs(bR[63]))));
			inputSampleR -= (bR[64] * (0.17222844322058231  + (0.12992472027850357*fabs(bR[64]))));
			inputSampleR -= (bR[65] * (0.14447856616566443  + (0.09089219002147308*fabs(bR[65]))));
			inputSampleR -= (bR[66] * (0.10385520794251019  + (0.08600465834570559*fabs(bR[66]))));
			inputSampleR -= (bR[67] * (0.07124435678265063  + (0.09071532210549428*fabs(bR[67]))));
			inputSampleR -= (bR[68] * (0.05216857461197572  + (0.06794061706070262*fabs(bR[68]))));
			inputSampleR -= (bR[69] * (0.05235381920184123  + (0.02818101717909346*fabs(bR[69]))));
			inputSampleR -= (bR[70] * (0.07569701245553526  - (0.00634228544764946*fabs(bR[70]))));
			inputSampleR -= (bR[71] * (0.10320125382718826  - (0.02751486906644141*fabs(bR[71]))));
			inputSampleR -= (bR[72] * (0.12122120969079088  - (0.05434007312178933*fabs(bR[72]))));
			inputSampleR -= (bR[73] * (0.13438969117200902  - (0.09135218559713874*fabs(bR[73]))));
			inputSampleR -= (bR[74] * (0.13534390437529981  - (0.10437672041458675*fabs(bR[74]))));
			inputSampleR -= (bR[75] * (0.11424128854188388  - (0.08693450726462598*fabs(bR[75]))));
			inputSampleR -= (bR[76] * (0.08166894518596159  - (0.06949989431475120*fabs(bR[76]))));
			inputSampleR -= (bR[77] * (0.04293976378555305  - (0.05718625137421843*fabs(bR[77]))));
			inputSampleR += (bR[78] * (0.00933076320644409  + (0.01728285211520138*fabs(bR[78]))));
			inputSampleR += (bR[79] * (0.06450430362918153  - (0.02492994833691022*fabs(bR[79]))));
			inputSampleR += (bR[80] * (0.10187400687649277  - (0.03578455940532403*fabs(bR[80]))));
			inputSampleR += (bR[81] * (0.11039763294094571  - (0.03995523517573508*fabs(bR[81]))));
			inputSampleR += (bR[82] * (0.08557960776024547  - (0.03482514309492527*fabs(bR[82]))));
			inputSampleR += (bR[83] * (0.02730881850805332  - (0.00514750108411127*fabs(bR[83]))));
			
			temp = (inputSampleR + smoothCabBR)/3.0;
			smoothCabBR = inputSampleR;
			inputSampleR = temp/4.0;
			
			randy = ((double(fpdR)/UINT32_MAX)*0.04);
			drySampleR = ((((inputSampleR*(1-randy))+(lastCabSampleR*randy))*wet)+(drySampleR*(1.0-wet)))*outputlevel;
			lastCabSampleR = inputSampleR;
			inputSampleR = drySampleR; //cab
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		switch (cycleEnd) //multi-pole average using lastRef[] variables
		{
			case 4:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[7])*0.5;
				lastRefL[7] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[7])*0.5;
				lastRefR[7] = lastRefR[8]; //continue, do not break
			case 3:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[6])*0.5;
				lastRefL[6] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[6])*0.5;
				lastRefR[6] = lastRefR[8]; //continue, do not break
			case 2:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[5])*0.5;
				lastRefL[5] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[5])*0.5;
				lastRefR[5] = lastRefR[8]; //continue, do not break
			case 1:
				break; //no further averaging
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void GrindAmp::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check		
	
	double inputlevel = pow(A,2);
	double samplerate = getSampleRate();
	double trimEQ = 1.1-B;
	double toneEQ = trimEQ/1.2;
	trimEQ /= 50.0;
	trimEQ += 0.165;
	double EQ = ((trimEQ-(toneEQ/6.1)) / samplerate)*22050.0;
	double BEQ = ((trimEQ+(toneEQ/2.1)) / samplerate)*22050.0;
	double outputlevel = C;
	double wet = D;
	double bassdrive = 1.57079633*(2.5-toneEQ);
	
	double cutoff = (18000.0+(B*1000.0)) / getSampleRate();
	if (cutoff > 0.49) cutoff = 0.49; //don't crash if run at 44.1k
	if (cutoff < 0.001) cutoff = 0.001; //or if cutoff's too low
	
	fixF[fix_freq] = fixE[fix_freq] = fixD[fix_freq] = fixC[fix_freq] = fixB[fix_freq] = fixA[fix_freq] = cutoff;
	
    fixA[fix_reso] = 4.46570214;
	fixB[fix_reso] = 1.51387132;
	fixC[fix_reso] = 0.93979296;
	fixD[fix_reso] = 0.70710678;
	fixE[fix_reso] = 0.52972649;
	fixF[fix_reso] = 0.50316379;
	
	double K = tan(M_PI * fixA[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fixA[fix_reso] + K * K);
	fixA[fix_a0] = K * K * norm;
	fixA[fix_a1] = 2.0 * fixA[fix_a0];
	fixA[fix_a2] = fixA[fix_a0];
	fixA[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixA[fix_b2] = (1.0 - K / fixA[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixB[fix_freq]);
	norm = 1.0 / (1.0 + K / fixB[fix_reso] + K * K);
	fixB[fix_a0] = K * K * norm;
	fixB[fix_a1] = 2.0 * fixB[fix_a0];
	fixB[fix_a2] = fixB[fix_a0];
	fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixB[fix_b2] = (1.0 - K / fixB[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixC[fix_freq]);
	norm = 1.0 / (1.0 + K / fixC[fix_reso] + K * K);
	fixC[fix_a0] = K * K * norm;
	fixC[fix_a1] = 2.0 * fixC[fix_a0];
	fixC[fix_a2] = fixC[fix_a0];
	fixC[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixC[fix_b2] = (1.0 - K / fixC[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixD[fix_freq]);
	norm = 1.0 / (1.0 + K / fixD[fix_reso] + K * K);
	fixD[fix_a0] = K * K * norm;
	fixD[fix_a1] = 2.0 * fixD[fix_a0];
	fixD[fix_a2] = fixD[fix_a0];
	fixD[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixD[fix_b2] = (1.0 - K / fixD[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixE[fix_freq]);
	norm = 1.0 / (1.0 + K / fixE[fix_reso] + K * K);
	fixE[fix_a0] = K * K * norm;
	fixE[fix_a1] = 2.0 * fixE[fix_a0];
	fixE[fix_a2] = fixE[fix_a0];
	fixE[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixE[fix_b2] = (1.0 - K / fixE[fix_reso] + K * K) * norm;
	
	K = tan(M_PI * fixF[fix_freq]);
	norm = 1.0 / (1.0 + K / fixF[fix_reso] + K * K);
	fixF[fix_a0] = K * K * norm;
	fixF[fix_a1] = 2.0 * fixF[fix_a0];
	fixF[fix_a2] = fixF[fix_a0];
	fixF[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixF[fix_b2] = (1.0 - K / fixF[fix_reso] + K * K) * norm;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double outSample = (inputSampleL * fixA[fix_a0]) + fixA[fix_sL1];
		fixA[fix_sL1] = (inputSampleL * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sL2];
		fixA[fix_sL2] = (inputSampleL * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixA[fix_a0]) + fixA[fix_sR1];
		fixA[fix_sR1] = (inputSampleR * fixA[fix_a1]) - (outSample * fixA[fix_b1]) + fixA[fix_sR2];
		fixA[fix_sR2] = (inputSampleR * fixA[fix_a2]) - (outSample * fixA[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		iirSampleAL = (iirSampleAL * (1.0 - EQ)) + (inputSampleL * EQ);
		inputSampleL = inputSampleL - (iirSampleAL*0.92);
		//highpass
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		double bridgerectifier = fabs(inputSampleL);
		double inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothAL + (secondAL*inverse) + (thirdAL*bridgerectifier) + inputSampleL);
		thirdAL = secondAL;
		secondAL = smoothAL;
		smoothAL = inputSampleL;
		double basscatchL = inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		inputSampleR *= inputlevel;
		iirSampleAR = (iirSampleAR * (1.0 - EQ)) + (inputSampleR * EQ);
		inputSampleR = inputSampleR - (iirSampleAR*0.92);
		//highpass
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		bridgerectifier = fabs(inputSampleR);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothAR + (secondAR*inverse) + (thirdAR*bridgerectifier) + inputSampleR);
		thirdAR = secondAR;
		secondAR = smoothAR;
		smoothAR = inputSampleR;
		double basscatchR = inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSampleL * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSampleL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSampleL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixB[fix_a0]) + fixB[fix_sR1];
		fixB[fix_sR1] = (inputSampleR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
		fixB[fix_sR2] = (inputSampleR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		iirSampleBL = (iirSampleBL * (1.0 - EQ)) + (inputSampleL * EQ);
		inputSampleL = inputSampleL - (iirSampleBL*0.79);
		//highpass
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		bridgerectifier = fabs(inputSampleL);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothBL + (secondBL*inverse) + (thirdBL*bridgerectifier) + inputSampleL);
		thirdBL = secondBL;
		secondBL = smoothBL;
		smoothBL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		inputSampleR *= inputlevel;
		iirSampleBR = (iirSampleBR * (1.0 - EQ)) + (inputSampleR * EQ);
		inputSampleR = inputSampleR - (iirSampleBR*0.79);
		//highpass
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		bridgerectifier = fabs(inputSampleR);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothBR + (secondBR*inverse) + (thirdBR*bridgerectifier) + inputSampleR);
		thirdBR = secondBR;
		secondBR = smoothBR;
		smoothBR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleCL = (iirSampleCL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleCL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothCL + (secondCL*inverse) + (thirdCL*bridgerectifier) + inputSampleL);
		thirdCL = secondCL;
		secondCL = smoothCL;
		smoothCL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleCR = (iirSampleCR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleCR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothCR + (secondCR*inverse) + (thirdCR*bridgerectifier) + inputSampleR);
		thirdCR = secondCR;
		secondCR = smoothCR;
		smoothCR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSampleL * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSampleL * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSampleL * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixC[fix_a0]) + fixC[fix_sR1];
		fixC[fix_sR1] = (inputSampleR * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sR2];
		fixC[fix_sR2] = (inputSampleR * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleDL = (iirSampleDL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleDL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothDL + (secondDL*inverse) + (thirdDL*bridgerectifier) + inputSampleL);
		thirdDL = secondDL;
		secondDL = smoothDL;
		smoothDL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleDR = (iirSampleDR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleDR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothDR + (secondDR*inverse) + (thirdDR*bridgerectifier) + inputSampleR);
		thirdDR = secondDR;
		secondDR = smoothDR;
		smoothDR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSampleL * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSampleL * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSampleL * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixD[fix_a0]) + fixD[fix_sR1];
		fixD[fix_sR1] = (inputSampleR * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sR2];
		fixD[fix_sR2] = (inputSampleR * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleEL = (iirSampleEL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleEL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothEL + (secondEL*inverse) + (thirdEL*bridgerectifier) + inputSampleL);
		thirdEL = secondEL;
		secondEL = smoothEL;
		smoothEL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleER = (iirSampleER * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleER*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothER + (secondER*inverse) + (thirdER*bridgerectifier) + inputSampleR);
		thirdER = secondER;
		secondER = smoothER;
		smoothER = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleFL = (iirSampleFL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleFL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothFL + (secondFL*inverse) + (thirdFL*bridgerectifier) + inputSampleL);
		thirdFL = secondFL;
		secondFL = smoothFL;
		smoothFL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleFR = (iirSampleFR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleFR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothFR + (secondFR*inverse) + (thirdFR*bridgerectifier) + inputSampleR);
		thirdFR = secondFR;
		secondFR = smoothFR;
		smoothFR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSampleL * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSampleL * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSampleL * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixE[fix_a0]) + fixE[fix_sR1];
		fixE[fix_sR1] = (inputSampleR * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sR2];
		fixE[fix_sR2] = (inputSampleR * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleGL = (iirSampleGL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleGL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothGL + (secondGL*inverse) + (thirdGL*bridgerectifier) + inputSampleL);
		thirdGL = secondGL;
		secondGL = smoothGL;
		smoothGL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleGR = (iirSampleGR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleGR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothGR + (secondGR*inverse) + (thirdGR*bridgerectifier) + inputSampleR);
		thirdGR = secondGR;
		secondGR = smoothGR;
		smoothGR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleHL = (iirSampleHL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleHL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothHL + (secondHL*inverse) + (thirdHL*bridgerectifier) + inputSampleL);
		thirdHL = secondHL;
		secondHL = smoothHL;
		smoothHL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleHR = (iirSampleHR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleHR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothHR + (secondHR*inverse) + (thirdHR*bridgerectifier) + inputSampleR);
		thirdHR = secondHR;
		secondHR = smoothHR;
		smoothHR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		outSample = (inputSampleL * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSampleL * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSampleL * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixF[fix_a0]) + fixF[fix_sR1];
		fixF[fix_sR1] = (inputSampleR * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sR2];
		fixF[fix_sR2] = (inputSampleR * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		iirSampleIL = (iirSampleIL * (1.0 - BEQ)) + (basscatchL * BEQ);
		basscatchL = iirSampleIL*bassdrive;
		bridgerectifier = fabs(basscatchL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchL > 0.0) basscatchL = bridgerectifier;
		else basscatchL = -bridgerectifier;
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothIL + (secondIL*inverse) + (thirdIL*bridgerectifier) + inputSampleL);
		thirdIL = secondIL;
		secondIL = smoothIL;
		smoothIL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		bridgerectifier = fabs(inputSampleL);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothJL + (secondJL*inverse) + (thirdJL*bridgerectifier) + inputSampleL);
		thirdJL = secondJL;
		secondJL = smoothJL;
		smoothJL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		bridgerectifier = fabs(inputSampleL);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothKL + (secondKL*inverse) + (thirdKL*bridgerectifier) + inputSampleL);
		thirdKL = secondKL;
		secondKL = smoothKL;
		smoothKL = inputSampleL;
		inputSampleL = bridgerectifier;
		//three-sample averaging lowpass
		
		iirSampleIR = (iirSampleIR * (1.0 - BEQ)) + (basscatchR * BEQ);
		basscatchR = iirSampleIR*bassdrive;
		bridgerectifier = fabs(basscatchR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (basscatchR > 0.0) basscatchR = bridgerectifier;
		else basscatchR = -bridgerectifier;
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		//overdrive
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothIR + (secondIR*inverse) + (thirdIR*bridgerectifier) + inputSampleR);
		thirdIR = secondIR;
		secondIR = smoothIR;
		smoothIR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		bridgerectifier = fabs(inputSampleR);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothJR + (secondJR*inverse) + (thirdJR*bridgerectifier) + inputSampleR);
		thirdJR = secondJR;
		secondJR = smoothJR;
		smoothJR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		bridgerectifier = fabs(inputSampleR);
		inverse = (bridgerectifier+1.0)/2.0;
		bridgerectifier = (smoothKR + (secondKR*inverse) + (thirdKR*bridgerectifier) + inputSampleR);
		thirdKR = secondKR;
		secondKR = smoothKR;
		smoothKR = inputSampleR;
		inputSampleR = bridgerectifier;
		//three-sample averaging lowpass
		
		basscatchL /= 2.0;
		inputSampleL = (inputSampleL*toneEQ)+basscatchL;
		//extra lowpass for 4*12" speakers		
		basscatchR /= 2.0;
		inputSampleR = (inputSampleR*toneEQ)+basscatchR;
		//extra lowpass for 4*12" speakers		
		
		bridgerectifier = fabs(inputSampleL*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		inputSampleL += basscatchL;
		//split bass between overdrive and clean
		inputSampleL /= (1.0+toneEQ);
		
		bridgerectifier = fabs(inputSampleR*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		inputSampleR += basscatchR;
		//split bass between overdrive and clean
		inputSampleR /= (1.0+toneEQ);
		
		double randy = ((double(fpdL)/UINT32_MAX)*0.061);
		inputSampleL = ((inputSampleL*(1-randy))+(storeSampleL*randy))*outputlevel;
		storeSampleL = inputSampleL;
		
		randy = ((double(fpdR)/UINT32_MAX)*0.061);
		inputSampleR = ((inputSampleR*(1-randy))+(storeSampleR*randy))*outputlevel;
		storeSampleR = inputSampleR;
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		//amp
		
		cycle++;
		if (cycle == cycleEnd) {
			
			double temp = (inputSampleL + smoothCabAL)/3.0;
			smoothCabAL = inputSampleL;
			inputSampleL = temp;
			
			bL[83] = bL[82]; bL[82] = bL[81]; bL[81] = bL[80]; bL[80] = bL[79]; 
			bL[79] = bL[78]; bL[78] = bL[77]; bL[77] = bL[76]; bL[76] = bL[75]; bL[75] = bL[74]; bL[74] = bL[73]; bL[73] = bL[72]; bL[72] = bL[71]; 
			bL[71] = bL[70]; bL[70] = bL[69]; bL[69] = bL[68]; bL[68] = bL[67]; bL[67] = bL[66]; bL[66] = bL[65]; bL[65] = bL[64]; bL[64] = bL[63]; 
			bL[63] = bL[62]; bL[62] = bL[61]; bL[61] = bL[60]; bL[60] = bL[59]; bL[59] = bL[58]; bL[58] = bL[57]; bL[57] = bL[56]; bL[56] = bL[55]; 
			bL[55] = bL[54]; bL[54] = bL[53]; bL[53] = bL[52]; bL[52] = bL[51]; bL[51] = bL[50]; bL[50] = bL[49]; bL[49] = bL[48]; bL[48] = bL[47]; 
			bL[47] = bL[46]; bL[46] = bL[45]; bL[45] = bL[44]; bL[44] = bL[43]; bL[43] = bL[42]; bL[42] = bL[41]; bL[41] = bL[40]; bL[40] = bL[39]; 
			bL[39] = bL[38]; bL[38] = bL[37]; bL[37] = bL[36]; bL[36] = bL[35]; bL[35] = bL[34]; bL[34] = bL[33]; bL[33] = bL[32]; bL[32] = bL[31]; 
			bL[31] = bL[30]; bL[30] = bL[29]; bL[29] = bL[28]; bL[28] = bL[27]; bL[27] = bL[26]; bL[26] = bL[25]; bL[25] = bL[24]; bL[24] = bL[23]; 
			bL[23] = bL[22]; bL[22] = bL[21]; bL[21] = bL[20]; bL[20] = bL[19]; bL[19] = bL[18]; bL[18] = bL[17]; bL[17] = bL[16]; bL[16] = bL[15]; 
			bL[15] = bL[14]; bL[14] = bL[13]; bL[13] = bL[12]; bL[12] = bL[11]; bL[11] = bL[10]; bL[10] = bL[9]; bL[9] = bL[8]; bL[8] = bL[7]; 
			bL[7] = bL[6]; bL[6] = bL[5]; bL[5] = bL[4]; bL[4] = bL[3]; bL[3] = bL[2]; bL[2] = bL[1]; bL[1] = bL[0]; bL[0] = inputSampleL;
			inputSampleL += (bL[1] * (1.29550481610475132  + (0.19713872057074355*fabs(bL[1]))));
			inputSampleL += (bL[2] * (1.42302569895462616  + (0.30599505521284787*fabs(bL[2]))));
			inputSampleL += (bL[3] * (1.28728195804197565  + (0.23168333460446133*fabs(bL[3]))));
			inputSampleL += (bL[4] * (0.88553784290822690  + (0.14263256172918892*fabs(bL[4]))));
			inputSampleL += (bL[5] * (0.37129054918432319  + (0.00150040944205920*fabs(bL[5]))));
			inputSampleL -= (bL[6] * (0.12150959412556320  + (0.32776273620569107*fabs(bL[6]))));
			inputSampleL -= (bL[7] * (0.44900065463203775  + (0.74101214925298819*fabs(bL[7]))));
			inputSampleL -= (bL[8] * (0.54058781908186482  + (1.07821707459008387*fabs(bL[8]))));
			inputSampleL -= (bL[9] * (0.49361966401791391  + (1.23540109014850508*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.39819495093078133  + (1.11247213730917749*fabs(bL[10]))));
			inputSampleL -= (bL[11] * (0.31379279985435521  + (0.80330360359638298*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.30744359242808555  + (0.42132528876858205*fabs(bL[12]))));
			inputSampleL -= (bL[13] * (0.33943170284673974  + (0.09183418349389982*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.33838775119286391  - (0.06453051658561271*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.30682305697961665  - (0.09549380253249232*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.23408741339295336  - (0.08083404732361277*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.10411746814025019  + (0.00253651281245780*fabs(bL[17]))));
			inputSampleL += (bL[18] * (0.00133623776084696  - (0.04447267870865820*fabs(bL[18]))));
			inputSampleL += (bL[19] * (0.02461903992114161  + (0.07530671732655550*fabs(bL[19]))));
			inputSampleL += (bL[20] * (0.02086715842475373  + (0.22795860236804899*fabs(bL[20]))));
			inputSampleL += (bL[21] * (0.02761433637100917  + (0.26108320417844094*fabs(bL[21]))));
			inputSampleL += (bL[22] * (0.04475285369162533  + (0.19160705011061663*fabs(bL[22]))));
			inputSampleL += (bL[23] * (0.09447338372862381  + (0.03681550508743799*fabs(bL[23]))));
			inputSampleL += (bL[24] * (0.13445890343722280  - (0.13713036462146147*fabs(bL[24]))));
			inputSampleL += (bL[25] * (0.13872868945088121  - (0.22401242373298191*fabs(bL[25]))));
			inputSampleL += (bL[26] * (0.14915650097434549  - (0.26718804981526367*fabs(bL[26]))));
			inputSampleL += (bL[27] * (0.12766643217091783  - (0.27745664795660430*fabs(bL[27]))));
			inputSampleL += (bL[28] * (0.03675849788393101  - (0.18338278173550679*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.06307306864232835  + (0.06089480869040766*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.14947389348962944  + (0.04642103054798480*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.25235266566401526  + (0.08423062596460507*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.33496344048679683  + (0.09808328256677995*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.36590030482175445  + (0.10622650888958179*fabs(bL[33]))));
			inputSampleL -= (bL[34] * (0.35015197011464372  + (0.08982043516016047*fabs(bL[34]))));
			inputSampleL -= (bL[35] * (0.26808437585665090  + (0.00735561860229533*fabs(bL[35]))));
			inputSampleL -= (bL[36] * (0.11624318543291220  - (0.07142484314510467*fabs(bL[36]))));
			inputSampleL += (bL[37] * (0.05617084165377551  + (0.11785854050350089*fabs(bL[37]))));
			inputSampleL += (bL[38] * (0.20540028692589385  + (0.20479174663329586*fabs(bL[38]))));
			inputSampleL += (bL[39] * (0.30455415003043818  + (0.29074864580096849*fabs(bL[39]))));
			inputSampleL += (bL[40] * (0.33810750937829476  + (0.29182307921316802*fabs(bL[40]))));
			inputSampleL += (bL[41] * (0.31936133365277430  + (0.26535537727394987*fabs(bL[41]))));
			inputSampleL += (bL[42] * (0.27388548321981876  + (0.19735049990538350*fabs(bL[42]))));
			inputSampleL += (bL[43] * (0.21454597517994098  + (0.06415909270247236*fabs(bL[43]))));
			inputSampleL += (bL[44] * (0.15001045817707717  - (0.03831118543404573*fabs(bL[44]))));
			inputSampleL += (bL[45] * (0.07283437284653138  - (0.09281952429543777*fabs(bL[45]))));
			inputSampleL -= (bL[46] * (0.03917872184241358  + (0.14306291461398810*fabs(bL[46]))));
			inputSampleL -= (bL[47] * (0.16695932032148642  + (0.19138995946950504*fabs(bL[47]))));
			inputSampleL -= (bL[48] * (0.27055854466909462  + (0.22531296466343192*fabs(bL[48]))));
			inputSampleL -= (bL[49] * (0.33256357307578271  + (0.23305840475692102*fabs(bL[49]))));
			inputSampleL -= (bL[50] * (0.33459770116834442  + (0.24091822618917569*fabs(bL[50]))));
			inputSampleL -= (bL[51] * (0.27156687236338090  + (0.24062938573512443*fabs(bL[51]))));
			inputSampleL -= (bL[52] * (0.17197093288412094  + (0.19083085091993421*fabs(bL[52]))));
			inputSampleL -= (bL[53] * (0.06738628195910543  + (0.10268609751019808*fabs(bL[53]))));
			inputSampleL += (bL[54] * (0.00222429218204290  + (0.01439664435720548*fabs(bL[54]))));
			inputSampleL += (bL[55] * (0.01346992803494091  + (0.15947137113534526*fabs(bL[55]))));
			inputSampleL -= (bL[56] * (0.02038911881377448  - (0.26763170752416160*fabs(bL[56]))));
			inputSampleL -= (bL[57] * (0.08233579178189687  - (0.29415931086406055*fabs(bL[57]))));
			inputSampleL -= (bL[58] * (0.15447855089824883  - (0.26489186990840807*fabs(bL[58]))));
			inputSampleL -= (bL[59] * (0.20518281113362655  - (0.16135382257522859*fabs(bL[59]))));
			inputSampleL -= (bL[60] * (0.22244686050232007  + (0.00847180390247432*fabs(bL[60]))));
			inputSampleL -= (bL[61] * (0.21849243134998034  + (0.14460595245753741*fabs(bL[61]))));
			inputSampleL -= (bL[62] * (0.20256105734574054  + (0.18932793221831667*fabs(bL[62]))));
			inputSampleL -= (bL[63] * (0.18604070054295399  + (0.17250665610927965*fabs(bL[63]))));
			inputSampleL -= (bL[64] * (0.17222844322058231  + (0.12992472027850357*fabs(bL[64]))));
			inputSampleL -= (bL[65] * (0.14447856616566443  + (0.09089219002147308*fabs(bL[65]))));
			inputSampleL -= (bL[66] * (0.10385520794251019  + (0.08600465834570559*fabs(bL[66]))));
			inputSampleL -= (bL[67] * (0.07124435678265063  + (0.09071532210549428*fabs(bL[67]))));
			inputSampleL -= (bL[68] * (0.05216857461197572  + (0.06794061706070262*fabs(bL[68]))));
			inputSampleL -= (bL[69] * (0.05235381920184123  + (0.02818101717909346*fabs(bL[69]))));
			inputSampleL -= (bL[70] * (0.07569701245553526  - (0.00634228544764946*fabs(bL[70]))));
			inputSampleL -= (bL[71] * (0.10320125382718826  - (0.02751486906644141*fabs(bL[71]))));
			inputSampleL -= (bL[72] * (0.12122120969079088  - (0.05434007312178933*fabs(bL[72]))));
			inputSampleL -= (bL[73] * (0.13438969117200902  - (0.09135218559713874*fabs(bL[73]))));
			inputSampleL -= (bL[74] * (0.13534390437529981  - (0.10437672041458675*fabs(bL[74]))));
			inputSampleL -= (bL[75] * (0.11424128854188388  - (0.08693450726462598*fabs(bL[75]))));
			inputSampleL -= (bL[76] * (0.08166894518596159  - (0.06949989431475120*fabs(bL[76]))));
			inputSampleL -= (bL[77] * (0.04293976378555305  - (0.05718625137421843*fabs(bL[77]))));
			inputSampleL += (bL[78] * (0.00933076320644409  + (0.01728285211520138*fabs(bL[78]))));
			inputSampleL += (bL[79] * (0.06450430362918153  - (0.02492994833691022*fabs(bL[79]))));
			inputSampleL += (bL[80] * (0.10187400687649277  - (0.03578455940532403*fabs(bL[80]))));
			inputSampleL += (bL[81] * (0.11039763294094571  - (0.03995523517573508*fabs(bL[81]))));
			inputSampleL += (bL[82] * (0.08557960776024547  - (0.03482514309492527*fabs(bL[82]))));
			inputSampleL += (bL[83] * (0.02730881850805332  - (0.00514750108411127*fabs(bL[83]))));
			
			temp = (inputSampleL + smoothCabBL)/3.0;
			smoothCabBL = inputSampleL;
			inputSampleL = temp/4.0;
			
			randy = ((double(fpdL)/UINT32_MAX)*0.044);
			drySampleL = ((((inputSampleL*(1-randy))+(lastCabSampleL*randy))*wet)+(drySampleL*(1.0-wet)))*outputlevel;
			lastCabSampleL = inputSampleL;
			inputSampleL = drySampleL; //cab L
			
			temp = (inputSampleR + smoothCabAR)/3.0;
			smoothCabAR = inputSampleR;
			inputSampleR = temp;
			
			bR[83] = bR[82]; bR[82] = bR[81]; bR[81] = bR[80]; bR[80] = bR[79]; 
			bR[79] = bR[78]; bR[78] = bR[77]; bR[77] = bR[76]; bR[76] = bR[75]; bR[75] = bR[74]; bR[74] = bR[73]; bR[73] = bR[72]; bR[72] = bR[71]; 
			bR[71] = bR[70]; bR[70] = bR[69]; bR[69] = bR[68]; bR[68] = bR[67]; bR[67] = bR[66]; bR[66] = bR[65]; bR[65] = bR[64]; bR[64] = bR[63]; 
			bR[63] = bR[62]; bR[62] = bR[61]; bR[61] = bR[60]; bR[60] = bR[59]; bR[59] = bR[58]; bR[58] = bR[57]; bR[57] = bR[56]; bR[56] = bR[55]; 
			bR[55] = bR[54]; bR[54] = bR[53]; bR[53] = bR[52]; bR[52] = bR[51]; bR[51] = bR[50]; bR[50] = bR[49]; bR[49] = bR[48]; bR[48] = bR[47]; 
			bR[47] = bR[46]; bR[46] = bR[45]; bR[45] = bR[44]; bR[44] = bR[43]; bR[43] = bR[42]; bR[42] = bR[41]; bR[41] = bR[40]; bR[40] = bR[39]; 
			bR[39] = bR[38]; bR[38] = bR[37]; bR[37] = bR[36]; bR[36] = bR[35]; bR[35] = bR[34]; bR[34] = bR[33]; bR[33] = bR[32]; bR[32] = bR[31]; 
			bR[31] = bR[30]; bR[30] = bR[29]; bR[29] = bR[28]; bR[28] = bR[27]; bR[27] = bR[26]; bR[26] = bR[25]; bR[25] = bR[24]; bR[24] = bR[23]; 
			bR[23] = bR[22]; bR[22] = bR[21]; bR[21] = bR[20]; bR[20] = bR[19]; bR[19] = bR[18]; bR[18] = bR[17]; bR[17] = bR[16]; bR[16] = bR[15]; 
			bR[15] = bR[14]; bR[14] = bR[13]; bR[13] = bR[12]; bR[12] = bR[11]; bR[11] = bR[10]; bR[10] = bR[9]; bR[9] = bR[8]; bR[8] = bR[7]; 
			bR[7] = bR[6]; bR[6] = bR[5]; bR[5] = bR[4]; bR[4] = bR[3]; bR[3] = bR[2]; bR[2] = bR[1]; bR[1] = bR[0]; bR[0] = inputSampleR;
			inputSampleR += (bR[1] * (1.29550481610475132  + (0.19713872057074355*fabs(bR[1]))));
			inputSampleR += (bR[2] * (1.42302569895462616  + (0.30599505521284787*fabs(bR[2]))));
			inputSampleR += (bR[3] * (1.28728195804197565  + (0.23168333460446133*fabs(bR[3]))));
			inputSampleR += (bR[4] * (0.88553784290822690  + (0.14263256172918892*fabs(bR[4]))));
			inputSampleR += (bR[5] * (0.37129054918432319  + (0.00150040944205920*fabs(bR[5]))));
			inputSampleR -= (bR[6] * (0.12150959412556320  + (0.32776273620569107*fabs(bR[6]))));
			inputSampleR -= (bR[7] * (0.44900065463203775  + (0.74101214925298819*fabs(bR[7]))));
			inputSampleR -= (bR[8] * (0.54058781908186482  + (1.07821707459008387*fabs(bR[8]))));
			inputSampleR -= (bR[9] * (0.49361966401791391  + (1.23540109014850508*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.39819495093078133  + (1.11247213730917749*fabs(bR[10]))));
			inputSampleR -= (bR[11] * (0.31379279985435521  + (0.80330360359638298*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.30744359242808555  + (0.42132528876858205*fabs(bR[12]))));
			inputSampleR -= (bR[13] * (0.33943170284673974  + (0.09183418349389982*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.33838775119286391  - (0.06453051658561271*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.30682305697961665  - (0.09549380253249232*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.23408741339295336  - (0.08083404732361277*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.10411746814025019  + (0.00253651281245780*fabs(bR[17]))));
			inputSampleR += (bR[18] * (0.00133623776084696  - (0.04447267870865820*fabs(bR[18]))));
			inputSampleR += (bR[19] * (0.02461903992114161  + (0.07530671732655550*fabs(bR[19]))));
			inputSampleR += (bR[20] * (0.02086715842475373  + (0.22795860236804899*fabs(bR[20]))));
			inputSampleR += (bR[21] * (0.02761433637100917  + (0.26108320417844094*fabs(bR[21]))));
			inputSampleR += (bR[22] * (0.04475285369162533  + (0.19160705011061663*fabs(bR[22]))));
			inputSampleR += (bR[23] * (0.09447338372862381  + (0.03681550508743799*fabs(bR[23]))));
			inputSampleR += (bR[24] * (0.13445890343722280  - (0.13713036462146147*fabs(bR[24]))));
			inputSampleR += (bR[25] * (0.13872868945088121  - (0.22401242373298191*fabs(bR[25]))));
			inputSampleR += (bR[26] * (0.14915650097434549  - (0.26718804981526367*fabs(bR[26]))));
			inputSampleR += (bR[27] * (0.12766643217091783  - (0.27745664795660430*fabs(bR[27]))));
			inputSampleR += (bR[28] * (0.03675849788393101  - (0.18338278173550679*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.06307306864232835  + (0.06089480869040766*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.14947389348962944  + (0.04642103054798480*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.25235266566401526  + (0.08423062596460507*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.33496344048679683  + (0.09808328256677995*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.36590030482175445  + (0.10622650888958179*fabs(bR[33]))));
			inputSampleR -= (bR[34] * (0.35015197011464372  + (0.08982043516016047*fabs(bR[34]))));
			inputSampleR -= (bR[35] * (0.26808437585665090  + (0.00735561860229533*fabs(bR[35]))));
			inputSampleR -= (bR[36] * (0.11624318543291220  - (0.07142484314510467*fabs(bR[36]))));
			inputSampleR += (bR[37] * (0.05617084165377551  + (0.11785854050350089*fabs(bR[37]))));
			inputSampleR += (bR[38] * (0.20540028692589385  + (0.20479174663329586*fabs(bR[38]))));
			inputSampleR += (bR[39] * (0.30455415003043818  + (0.29074864580096849*fabs(bR[39]))));
			inputSampleR += (bR[40] * (0.33810750937829476  + (0.29182307921316802*fabs(bR[40]))));
			inputSampleR += (bR[41] * (0.31936133365277430  + (0.26535537727394987*fabs(bR[41]))));
			inputSampleR += (bR[42] * (0.27388548321981876  + (0.19735049990538350*fabs(bR[42]))));
			inputSampleR += (bR[43] * (0.21454597517994098  + (0.06415909270247236*fabs(bR[43]))));
			inputSampleR += (bR[44] * (0.15001045817707717  - (0.03831118543404573*fabs(bR[44]))));
			inputSampleR += (bR[45] * (0.07283437284653138  - (0.09281952429543777*fabs(bR[45]))));
			inputSampleR -= (bR[46] * (0.03917872184241358  + (0.14306291461398810*fabs(bR[46]))));
			inputSampleR -= (bR[47] * (0.16695932032148642  + (0.19138995946950504*fabs(bR[47]))));
			inputSampleR -= (bR[48] * (0.27055854466909462  + (0.22531296466343192*fabs(bR[48]))));
			inputSampleR -= (bR[49] * (0.33256357307578271  + (0.23305840475692102*fabs(bR[49]))));
			inputSampleR -= (bR[50] * (0.33459770116834442  + (0.24091822618917569*fabs(bR[50]))));
			inputSampleR -= (bR[51] * (0.27156687236338090  + (0.24062938573512443*fabs(bR[51]))));
			inputSampleR -= (bR[52] * (0.17197093288412094  + (0.19083085091993421*fabs(bR[52]))));
			inputSampleR -= (bR[53] * (0.06738628195910543  + (0.10268609751019808*fabs(bR[53]))));
			inputSampleR += (bR[54] * (0.00222429218204290  + (0.01439664435720548*fabs(bR[54]))));
			inputSampleR += (bR[55] * (0.01346992803494091  + (0.15947137113534526*fabs(bR[55]))));
			inputSampleR -= (bR[56] * (0.02038911881377448  - (0.26763170752416160*fabs(bR[56]))));
			inputSampleR -= (bR[57] * (0.08233579178189687  - (0.29415931086406055*fabs(bR[57]))));
			inputSampleR -= (bR[58] * (0.15447855089824883  - (0.26489186990840807*fabs(bR[58]))));
			inputSampleR -= (bR[59] * (0.20518281113362655  - (0.16135382257522859*fabs(bR[59]))));
			inputSampleR -= (bR[60] * (0.22244686050232007  + (0.00847180390247432*fabs(bR[60]))));
			inputSampleR -= (bR[61] * (0.21849243134998034  + (0.14460595245753741*fabs(bR[61]))));
			inputSampleR -= (bR[62] * (0.20256105734574054  + (0.18932793221831667*fabs(bR[62]))));
			inputSampleR -= (bR[63] * (0.18604070054295399  + (0.17250665610927965*fabs(bR[63]))));
			inputSampleR -= (bR[64] * (0.17222844322058231  + (0.12992472027850357*fabs(bR[64]))));
			inputSampleR -= (bR[65] * (0.14447856616566443  + (0.09089219002147308*fabs(bR[65]))));
			inputSampleR -= (bR[66] * (0.10385520794251019  + (0.08600465834570559*fabs(bR[66]))));
			inputSampleR -= (bR[67] * (0.07124435678265063  + (0.09071532210549428*fabs(bR[67]))));
			inputSampleR -= (bR[68] * (0.05216857461197572  + (0.06794061706070262*fabs(bR[68]))));
			inputSampleR -= (bR[69] * (0.05235381920184123  + (0.02818101717909346*fabs(bR[69]))));
			inputSampleR -= (bR[70] * (0.07569701245553526  - (0.00634228544764946*fabs(bR[70]))));
			inputSampleR -= (bR[71] * (0.10320125382718826  - (0.02751486906644141*fabs(bR[71]))));
			inputSampleR -= (bR[72] * (0.12122120969079088  - (0.05434007312178933*fabs(bR[72]))));
			inputSampleR -= (bR[73] * (0.13438969117200902  - (0.09135218559713874*fabs(bR[73]))));
			inputSampleR -= (bR[74] * (0.13534390437529981  - (0.10437672041458675*fabs(bR[74]))));
			inputSampleR -= (bR[75] * (0.11424128854188388  - (0.08693450726462598*fabs(bR[75]))));
			inputSampleR -= (bR[76] * (0.08166894518596159  - (0.06949989431475120*fabs(bR[76]))));
			inputSampleR -= (bR[77] * (0.04293976378555305  - (0.05718625137421843*fabs(bR[77]))));
			inputSampleR += (bR[78] * (0.00933076320644409  + (0.01728285211520138*fabs(bR[78]))));
			inputSampleR += (bR[79] * (0.06450430362918153  - (0.02492994833691022*fabs(bR[79]))));
			inputSampleR += (bR[80] * (0.10187400687649277  - (0.03578455940532403*fabs(bR[80]))));
			inputSampleR += (bR[81] * (0.11039763294094571  - (0.03995523517573508*fabs(bR[81]))));
			inputSampleR += (bR[82] * (0.08557960776024547  - (0.03482514309492527*fabs(bR[82]))));
			inputSampleR += (bR[83] * (0.02730881850805332  - (0.00514750108411127*fabs(bR[83]))));
			
			temp = (inputSampleR + smoothCabBR)/3.0;
			smoothCabBR = inputSampleR;
			inputSampleR = temp/4.0;
			
			randy = ((double(fpdR)/UINT32_MAX)*0.04);
			drySampleR = ((((inputSampleR*(1-randy))+(lastCabSampleR*randy))*wet)+(drySampleR*(1.0-wet)))*outputlevel;
			lastCabSampleR = inputSampleR;
			inputSampleR = drySampleR; //cab
			
			if (cycleEnd == 4) {
				lastRefL[0] = lastRefL[4]; //start from previous last
				lastRefL[2] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[1] = (lastRefL[0] + lastRefL[2])/2; //one quarter
				lastRefL[3] = (lastRefL[2] + inputSampleL)/2; //three quarters
				lastRefL[4] = inputSampleL; //full
				lastRefR[0] = lastRefR[4]; //start from previous last
				lastRefR[2] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[1] = (lastRefR[0] + lastRefR[2])/2; //one quarter
				lastRefR[3] = (lastRefR[2] + inputSampleR)/2; //three quarters
				lastRefR[4] = inputSampleR; //full
			}
			if (cycleEnd == 3) {
				lastRefL[0] = lastRefL[3]; //start from previous last
				lastRefL[2] = (lastRefL[0]+lastRefL[0]+inputSampleL)/3; //third
				lastRefL[1] = (lastRefL[0]+inputSampleL+inputSampleL)/3; //two thirds
				lastRefL[3] = inputSampleL; //full
				lastRefR[0] = lastRefR[3]; //start from previous last
				lastRefR[2] = (lastRefR[0]+lastRefR[0]+inputSampleR)/3; //third
				lastRefR[1] = (lastRefR[0]+inputSampleR+inputSampleR)/3; //two thirds
				lastRefR[3] = inputSampleR; //full
			}
			if (cycleEnd == 2) {
				lastRefL[0] = lastRefL[2]; //start from previous last
				lastRefL[1] = (lastRefL[0] + inputSampleL)/2; //half
				lastRefL[2] = inputSampleL; //full
				lastRefR[0] = lastRefR[2]; //start from previous last
				lastRefR[1] = (lastRefR[0] + inputSampleR)/2; //half
				lastRefR[2] = inputSampleR; //full
			}
			if (cycleEnd == 1) {
				lastRefL[0] = inputSampleL;
				lastRefR[0] = inputSampleR;
			}
			cycle = 0; //reset
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
		} else {
			inputSampleL = lastRefL[cycle];
			inputSampleR = lastRefR[cycle];
			//we are going through our references now
		}
		switch (cycleEnd) //multi-pole average using lastRef[] variables
		{
			case 4:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[7])*0.5;
				lastRefL[7] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[7])*0.5;
				lastRefR[7] = lastRefR[8]; //continue, do not break
			case 3:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[6])*0.5;
				lastRefL[6] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[6])*0.5;
				lastRefR[6] = lastRefR[8]; //continue, do not break
			case 2:
				lastRefL[8] = inputSampleL; inputSampleL = (inputSampleL+lastRefL[5])*0.5;
				lastRefL[5] = lastRefL[8]; //continue, do not break
				lastRefR[8] = inputSampleR; inputSampleR = (inputSampleR+lastRefR[5])*0.5;
				lastRefR[5] = lastRefR[8]; //continue, do not break
			case 1:
				break; //no further averaging
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
