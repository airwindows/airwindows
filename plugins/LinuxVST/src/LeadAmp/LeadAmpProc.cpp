/* ========================================
 *  LeadAmp - LeadAmp.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#include "LeadAmp.h"
#endif

void LeadAmp::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double bassfill = A;
	double startlevel = bassfill;
	double samplerate = getSampleRate();
	double basstrim = bassfill / 10.0;
	double toneEQ = (B / samplerate)*22050.0;
	double EQ = (basstrim / samplerate)*22050.0;
	double outputlevel = C;
	double wet = D;
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check		
	
	double bleed = outputlevel/16.0;
	double bassfactor = 1.0-(basstrim*basstrim);
	double BEQ = (bleed / samplerate)*22050.0;
	int diagonal = (int)(0.000861678*samplerate);
	if (diagonal > 127) diagonal = 127;
	int side = (int)(diagonal/1.4142135623730951);
	int down = (side + diagonal)/2;
	//now we've got down, side and diagonal as offsets and we also use three successive samples upfront
	
	double cutoff = (15000.0+(B*10000.0)) / getSampleRate();
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
		
		double basscutL = 0.99;
		//we're going to be shifting this as the stages progress
		double inputlevelL = startlevel;
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleAL = (iirSampleAL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleAL*basscutL);
		//highpass
		double bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		//overdrive
		bridgerectifier = (smoothAL + inputSampleL);
		smoothAL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		double basscutR = 0.99;
		//we're going to be shifting this as the stages progress
		double inputlevelR = startlevel;
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleAR = (iirSampleAR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleAR*basscutR);
		//highpass
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		//overdrive
		bridgerectifier = (smoothAR + inputSampleR);
		smoothAR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSampleL * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSampleL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSampleL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixB[fix_a0]) + fixB[fix_sR1];
		fixB[fix_sR1] = (inputSampleR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
		fixB[fix_sR2] = (inputSampleR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleBL = (iirSampleBL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleBL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothBL + inputSampleL);
		smoothBL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleBR = (iirSampleBR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleBR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothBR + inputSampleR);
		smoothBR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSampleL * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSampleL * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSampleL * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixC[fix_a0]) + fixC[fix_sR1];
		fixC[fix_sR1] = (inputSampleR * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sR2];
		fixC[fix_sR2] = (inputSampleR * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleCL = (iirSampleCL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleCL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothCL + inputSampleL);
		smoothCL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleCR = (iirSampleCR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleCR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothCR + inputSampleR);
		smoothCR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleDL = (iirSampleDL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleDL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothDL + inputSampleL);
		smoothDL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleDR = (iirSampleDR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleDR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothDR + inputSampleR);
		smoothDR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSampleL * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSampleL * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSampleL * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixD[fix_a0]) + fixD[fix_sR1];
		fixD[fix_sR1] = (inputSampleR * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sR2];
		fixD[fix_sR2] = (inputSampleR * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleEL = (iirSampleEL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleEL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothEL + inputSampleL);
		smoothEL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleER = (iirSampleER * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleER*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothER + inputSampleR);
		smoothER = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleFL = (iirSampleFL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleFL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothFL + inputSampleL);
		smoothFL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleFR = (iirSampleFR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleFR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothFR + inputSampleR);
		smoothFR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSampleL * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSampleL * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSampleL * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixE[fix_a0]) + fixE[fix_sR1];
		fixE[fix_sR1] = (inputSampleR * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sR2];
		fixE[fix_sR2] = (inputSampleR * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleGL = (iirSampleGL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleGL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothGL + inputSampleL);
		smoothGL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleGR = (iirSampleGR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleGR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothGR + inputSampleR);
		smoothGR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleHL = (iirSampleHL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleHL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothHL + inputSampleL);
		smoothHL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleHR = (iirSampleHR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleHR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothHR + inputSampleR);
		smoothHR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSampleL * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSampleL * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSampleL * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixF[fix_a0]) + fixF[fix_sR1];
		fixF[fix_sR1] = (inputSampleR * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sR2];
		fixF[fix_sR2] = (inputSampleR * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleIL = (iirSampleIL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleIL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothIL + inputSampleL);
		smoothIL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleIR = (iirSampleIR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleIR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothIR + inputSampleR);
		smoothIR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleJL = (iirSampleJL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleJL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothJL + inputSampleL);
		smoothJL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleJR = (iirSampleJR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleJR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothJR + inputSampleR);
		smoothJR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleKL = (iirSampleKL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleKL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothKL + inputSampleL);
		smoothKL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleKR = (iirSampleKR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleKR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothKR + inputSampleR);
		smoothKR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		iirLowpassL = (iirLowpassL * (1.0 - toneEQ)) + (inputSampleL * toneEQ);
		inputSampleL = iirLowpassL;
		//lowpass. The only one of this type.
		
		iirLowpassR = (iirLowpassR * (1.0 - toneEQ)) + (inputSampleR * toneEQ);
		inputSampleR = iirLowpassR;
		//lowpass. The only one of this type.
		
		iirSpkAL = (iirSpkAL * (1.0 -  BEQ)) + (inputSampleL * BEQ);
		//extra lowpass for 4*12" speakers
		
		iirSpkAR = (iirSpkAR * (1.0 -  BEQ)) + (inputSampleR * BEQ);
		//extra lowpass for 4*12" speakers

		if (count < 0 || count > 128) {count = 128;}
		double resultBL = 0.0;
		double resultBR = 0.0;
		if (flip)
		{
			OddL[count+128] = OddL[count] = iirSpkAL;
			resultBL = (OddL[count+down] + OddL[count+side] + OddL[count+diagonal]);
			OddR[count+128] = OddR[count] = iirSpkAR;
			resultBR = (OddR[count+down] + OddR[count+side] + OddR[count+diagonal]);
		}
		else
		{
			EvenL[count+128] = EvenL[count] = iirSpkAL;
			resultBL = (EvenL[count+down] + EvenL[count+side] + EvenL[count+diagonal]);
			EvenR[count+128] = EvenR[count] = iirSpkAR;
			resultBR = (EvenR[count+down] + EvenR[count+side] + EvenR[count+diagonal]);
		}
		count--;
		
		iirSpkBL = (iirSpkBL * (1.0 - BEQ)) + (resultBL * BEQ);
		inputSampleL += (iirSpkBL*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSpkBR = (iirSpkBR * (1.0 - BEQ)) + (resultBR * BEQ);
		inputSampleR += (iirSpkBR*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSubL = (iirSubL * (1.0 - BEQ)) + (inputSampleL * BEQ);
		inputSampleL += (iirSubL * bassfill);
		
		iirSubR = (iirSubR * (1.0 - BEQ)) + (inputSampleR * BEQ);
		inputSampleR += (iirSubR * bassfill);
		
		bridgerectifier = fabs(inputSampleL*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		
		bridgerectifier = fabs(inputSampleR*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		
		double randy = ((double(fpdL)/UINT32_MAX)*0.084);
		inputSampleL = ((inputSampleL*(1.0-randy))+(storeSampleL*randy))*outputlevel;
		storeSampleL = inputSampleL;

		randy = ((double(fpdR)/UINT32_MAX)*0.084);
		inputSampleR = ((inputSampleR*(1.0-randy))+(storeSampleR*randy))*outputlevel;
		storeSampleR = inputSampleR;

		flip = !flip;
		
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
			
			bL[85] = bL[84]; bL[84] = bL[83]; bL[83] = bL[82]; bL[82] = bL[81]; bL[81] = bL[80]; bL[80] = bL[79]; 
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
			inputSampleL += (bL[1] * (1.30406584776167445  - (0.01410622186823351*fabs(bL[1]))));
			inputSampleL += (bL[2] * (1.09350974154373559  + (0.34478044709202327*fabs(bL[2]))));
			inputSampleL += (bL[3] * (0.52285510059938256  + (0.84225842837363574*fabs(bL[3]))));
			inputSampleL -= (bL[4] * (0.00018126260714707  - (1.02446537989058117*fabs(bL[4]))));
			inputSampleL -= (bL[5] * (0.34943699771860115  - (0.84094709567790016*fabs(bL[5]))));
			inputSampleL -= (bL[6] * (0.53068048407937285  - (0.49231169327705593*fabs(bL[6]))));
			inputSampleL -= (bL[7] * (0.48631669406792399  - (0.08965111766223610*fabs(bL[7]))));
			inputSampleL -= (bL[8] * (0.28099201947014130  + (0.23921137841068607*fabs(bL[8]))));
			inputSampleL -= (bL[9] * (0.10333290012666248  + (0.35058962687321482*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.06605032198166226  + (0.23447405567823365*fabs(bL[10]))));
			inputSampleL -= (bL[11] * (0.10485808661261729  + (0.05025985449763527*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.13231190973014911  - (0.05484648240248013*fabs(bL[12]))));
			inputSampleL -= (bL[13] * (0.12926184767180304  - (0.04054223744746116*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.13802696739839460  + (0.01876754906568237*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.16548980700926913  + (0.06772130758771169*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.14469310965751475  + (0.10590928840978781*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.07838457396093310  + (0.13120101199677947*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.05123031606187391  + (0.13883400806512292*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.08906103481939850  + (0.07840461228402337*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.13939265522625241  + (0.01194366471800457*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.14957600717294034  + (0.07687598594361914*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.14112708654047090  + (0.20118461131186977*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.14961020766492997  + (0.30005716443826147*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.16130382224652270  + (0.40459872030013055*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.15679868471080052  + (0.47292767226083465*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.16456530552807727  + (0.45182121471666481*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.16852385701909278  + (0.38272684270752266*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.13317562760966850  + (0.28829580273670768*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.09396196532150952  + (0.18886898332071317*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.10133496956734221  + (0.11158788414137354*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.16097596389376778  + (0.02621299102374547*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.21419006394821866  - (0.03585678078834797*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.21273234570555244  - (0.02574469802924526*fabs(bL[33]))));
			inputSampleL -= (bL[34] * (0.16934948798707830  + (0.01354331184333835*fabs(bL[34]))));
			inputSampleL -= (bL[35] * (0.11970436472852493  + (0.04242183865883427*fabs(bL[35]))));
			inputSampleL -= (bL[36] * (0.09329023656747724  + (0.06890873292358397*fabs(bL[36]))));
			inputSampleL -= (bL[37] * (0.10255328436608116  + (0.11482972519137427*fabs(bL[37]))));
			inputSampleL -= (bL[38] * (0.13883223352796811  + (0.18016014431438840*fabs(bL[38]))));
			inputSampleL -= (bL[39] * (0.16532844286979087  + (0.24521957638633446*fabs(bL[39]))));
			inputSampleL -= (bL[40] * (0.16254607738965438  + (0.25669472097572482*fabs(bL[40]))));
			inputSampleL -= (bL[41] * (0.15353207135544752  + (0.15048064682912729*fabs(bL[41]))));
			inputSampleL -= (bL[42] * (0.13039046390746015  - (0.00200335414623601*fabs(bL[42]))));
			inputSampleL -= (bL[43] * (0.06707245032180627  - (0.06498125592578702*fabs(bL[43]))));
			inputSampleL += (bL[44] * (0.01427326441869788  + (0.01940451360783622*fabs(bL[44]))));
			inputSampleL += (bL[45] * (0.06151238306578224  - (0.07335755969763329*fabs(bL[45]))));
			inputSampleL += (bL[46] * (0.04685840498892526  - (0.14258849371688248*fabs(bL[46]))));
			inputSampleL -= (bL[47] * (0.00950136304466093  + (0.14379354707665129*fabs(bL[47]))));
			inputSampleL -= (bL[48] * (0.06245771575493557  + (0.07639718586346110*fabs(bL[48]))));
			inputSampleL -= (bL[49] * (0.07159593175777741  - (0.00595536565276915*fabs(bL[49]))));
			inputSampleL -= (bL[50] * (0.03167929390245019  - (0.03856769526301793*fabs(bL[50]))));
			inputSampleL += (bL[51] * (0.01890898565110766  + (0.00760539424271147*fabs(bL[51]))));
			inputSampleL += (bL[52] * (0.04926161137832240  - (0.06411014430053390*fabs(bL[52]))));
			inputSampleL += (bL[53] * (0.05768814623421683  - (0.15068618173358578*fabs(bL[53]))));
			inputSampleL += (bL[54] * (0.06144258297076708  - (0.21200636329120301*fabs(bL[54]))));
			inputSampleL += (bL[55] * (0.06348341960185613  - (0.19620269813094307*fabs(bL[55]))));
			inputSampleL += (bL[56] * (0.04877736350310589  - (0.11864999881200111*fabs(bL[56]))));
			inputSampleL += (bL[57] * (0.01010950997574472  - (0.02630070679113791*fabs(bL[57]))));
			inputSampleL -= (bL[58] * (0.02929178864801191  - (0.04439260202207482*fabs(bL[58]))));
			inputSampleL -= (bL[59] * (0.03484517126321562  - (0.04508635396034735*fabs(bL[59]))));
			inputSampleL -= (bL[60] * (0.00547176780437610  - (0.00205637806941426*fabs(bL[60]))));
			inputSampleL += (bL[61] * (0.02278296865283977  - (0.00063732526427685*fabs(bL[61]))));
			inputSampleL += (bL[62] * (0.02688982591366477  + (0.05333738901586284*fabs(bL[62]))));
			inputSampleL += (bL[63] * (0.01942012754957055  + (0.10942832669749143*fabs(bL[63]))));
			inputSampleL += (bL[64] * (0.01572585258756565  + (0.11189204189054594*fabs(bL[64]))));
			inputSampleL += (bL[65] * (0.01490550715016034  + (0.04449822818925343*fabs(bL[65]))));
			inputSampleL += (bL[66] * (0.01715683226376727  - (0.06944648050933899*fabs(bL[66]))));
			inputSampleL += (bL[67] * (0.02822659878011318  - (0.17843652160132820*fabs(bL[67]))));
			inputSampleL += (bL[68] * (0.03758307610456144  - (0.21986013433664692*fabs(bL[68]))));
			inputSampleL += (bL[69] * (0.03275008021608433  - (0.15869878676112170*fabs(bL[69]))));
			inputSampleL += (bL[70] * (0.01855749786752354  - (0.02337224995718105*fabs(bL[70]))));
			inputSampleL += (bL[71] * (0.00217095395782931  + (0.10971764224593601*fabs(bL[71]))));
			inputSampleL -= (bL[72] * (0.01851381451105007  - (0.17214910008793413*fabs(bL[72]))));
			inputSampleL -= (bL[73] * (0.04722574936345419  - (0.14341588977845254*fabs(bL[73]))));
			inputSampleL -= (bL[74] * (0.07151540514482006  - (0.04684695724814321*fabs(bL[74]))));
			inputSampleL -= (bL[75] * (0.06827195484995092  + (0.07022207121861397*fabs(bL[75]))));
			inputSampleL -= (bL[76] * (0.03290227240464227  + (0.16328400808152735*fabs(bL[76]))));
			inputSampleL += (bL[77] * (0.01043861198275382  - (0.20184486126076279*fabs(bL[77]))));
			inputSampleL += (bL[78] * (0.03236563559476477  - (0.17125821306380920*fabs(bL[78]))));
			inputSampleL += (bL[79] * (0.02040121529932702  - (0.09103660189829657*fabs(bL[79]))));
			inputSampleL -= (bL[80] * (0.00509649513318102  + (0.01170360991547489*fabs(bL[80]))));
			inputSampleL -= (bL[81] * (0.01388353426600228  - (0.03588955538451771*fabs(bL[81]))));
			inputSampleL -= (bL[82] * (0.00523671715033842  - (0.07068798057534148*fabs(bL[82]))));
			inputSampleL += (bL[83] * (0.00665852487721137  + (0.11666210640054926*fabs(bL[83]))));
			inputSampleL += (bL[84] * (0.01593540832939290  + (0.15844892856402149*fabs(bL[84]))));
			inputSampleL += (bL[85] * (0.02080509201836796  + (0.17186274420065850*fabs(bL[85]))));
			
			temp = (inputSampleL + smoothCabBL)/3.0;
			smoothCabBL = inputSampleL;
			inputSampleL = temp/4.0;
			
			randy = ((double(fpdL)/UINT32_MAX)*0.079);
			drySampleL = ((((inputSampleL*(1.0-randy))+(lastCabSampleL*randy))*wet)+(drySampleL*(1.0-wet)))*outputlevel;
			lastCabSampleL = inputSampleL;
			inputSampleL = drySampleL; //cab L
			
			
			temp = (inputSampleR + smoothCabAR)/3.0;
			smoothCabAR = inputSampleR;
			inputSampleR = temp;
			
			bR[85] = bR[84]; bR[84] = bR[83]; bR[83] = bR[82]; bR[82] = bR[81]; bR[81] = bR[80]; bR[80] = bR[79]; 
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
			inputSampleR += (bR[1] * (1.30406584776167445  - (0.01410622186823351*fabs(bR[1]))));
			inputSampleR += (bR[2] * (1.09350974154373559  + (0.34478044709202327*fabs(bR[2]))));
			inputSampleR += (bR[3] * (0.52285510059938256  + (0.84225842837363574*fabs(bR[3]))));
			inputSampleR -= (bR[4] * (0.00018126260714707  - (1.02446537989058117*fabs(bR[4]))));
			inputSampleR -= (bR[5] * (0.34943699771860115  - (0.84094709567790016*fabs(bR[5]))));
			inputSampleR -= (bR[6] * (0.53068048407937285  - (0.49231169327705593*fabs(bR[6]))));
			inputSampleR -= (bR[7] * (0.48631669406792399  - (0.08965111766223610*fabs(bR[7]))));
			inputSampleR -= (bR[8] * (0.28099201947014130  + (0.23921137841068607*fabs(bR[8]))));
			inputSampleR -= (bR[9] * (0.10333290012666248  + (0.35058962687321482*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.06605032198166226  + (0.23447405567823365*fabs(bR[10]))));
			inputSampleR -= (bR[11] * (0.10485808661261729  + (0.05025985449763527*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.13231190973014911  - (0.05484648240248013*fabs(bR[12]))));
			inputSampleR -= (bR[13] * (0.12926184767180304  - (0.04054223744746116*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.13802696739839460  + (0.01876754906568237*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.16548980700926913  + (0.06772130758771169*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.14469310965751475  + (0.10590928840978781*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.07838457396093310  + (0.13120101199677947*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.05123031606187391  + (0.13883400806512292*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.08906103481939850  + (0.07840461228402337*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.13939265522625241  + (0.01194366471800457*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.14957600717294034  + (0.07687598594361914*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.14112708654047090  + (0.20118461131186977*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.14961020766492997  + (0.30005716443826147*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.16130382224652270  + (0.40459872030013055*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.15679868471080052  + (0.47292767226083465*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.16456530552807727  + (0.45182121471666481*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.16852385701909278  + (0.38272684270752266*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.13317562760966850  + (0.28829580273670768*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.09396196532150952  + (0.18886898332071317*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.10133496956734221  + (0.11158788414137354*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.16097596389376778  + (0.02621299102374547*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.21419006394821866  - (0.03585678078834797*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.21273234570555244  - (0.02574469802924526*fabs(bR[33]))));
			inputSampleR -= (bR[34] * (0.16934948798707830  + (0.01354331184333835*fabs(bR[34]))));
			inputSampleR -= (bR[35] * (0.11970436472852493  + (0.04242183865883427*fabs(bR[35]))));
			inputSampleR -= (bR[36] * (0.09329023656747724  + (0.06890873292358397*fabs(bR[36]))));
			inputSampleR -= (bR[37] * (0.10255328436608116  + (0.11482972519137427*fabs(bR[37]))));
			inputSampleR -= (bR[38] * (0.13883223352796811  + (0.18016014431438840*fabs(bR[38]))));
			inputSampleR -= (bR[39] * (0.16532844286979087  + (0.24521957638633446*fabs(bR[39]))));
			inputSampleR -= (bR[40] * (0.16254607738965438  + (0.25669472097572482*fabs(bR[40]))));
			inputSampleR -= (bR[41] * (0.15353207135544752  + (0.15048064682912729*fabs(bR[41]))));
			inputSampleR -= (bR[42] * (0.13039046390746015  - (0.00200335414623601*fabs(bR[42]))));
			inputSampleR -= (bR[43] * (0.06707245032180627  - (0.06498125592578702*fabs(bR[43]))));
			inputSampleR += (bR[44] * (0.01427326441869788  + (0.01940451360783622*fabs(bR[44]))));
			inputSampleR += (bR[45] * (0.06151238306578224  - (0.07335755969763329*fabs(bR[45]))));
			inputSampleR += (bR[46] * (0.04685840498892526  - (0.14258849371688248*fabs(bR[46]))));
			inputSampleR -= (bR[47] * (0.00950136304466093  + (0.14379354707665129*fabs(bR[47]))));
			inputSampleR -= (bR[48] * (0.06245771575493557  + (0.07639718586346110*fabs(bR[48]))));
			inputSampleR -= (bR[49] * (0.07159593175777741  - (0.00595536565276915*fabs(bR[49]))));
			inputSampleR -= (bR[50] * (0.03167929390245019  - (0.03856769526301793*fabs(bR[50]))));
			inputSampleR += (bR[51] * (0.01890898565110766  + (0.00760539424271147*fabs(bR[51]))));
			inputSampleR += (bR[52] * (0.04926161137832240  - (0.06411014430053390*fabs(bR[52]))));
			inputSampleR += (bR[53] * (0.05768814623421683  - (0.15068618173358578*fabs(bR[53]))));
			inputSampleR += (bR[54] * (0.06144258297076708  - (0.21200636329120301*fabs(bR[54]))));
			inputSampleR += (bR[55] * (0.06348341960185613  - (0.19620269813094307*fabs(bR[55]))));
			inputSampleR += (bR[56] * (0.04877736350310589  - (0.11864999881200111*fabs(bR[56]))));
			inputSampleR += (bR[57] * (0.01010950997574472  - (0.02630070679113791*fabs(bR[57]))));
			inputSampleR -= (bR[58] * (0.02929178864801191  - (0.04439260202207482*fabs(bR[58]))));
			inputSampleR -= (bR[59] * (0.03484517126321562  - (0.04508635396034735*fabs(bR[59]))));
			inputSampleR -= (bR[60] * (0.00547176780437610  - (0.00205637806941426*fabs(bR[60]))));
			inputSampleR += (bR[61] * (0.02278296865283977  - (0.00063732526427685*fabs(bR[61]))));
			inputSampleR += (bR[62] * (0.02688982591366477  + (0.05333738901586284*fabs(bR[62]))));
			inputSampleR += (bR[63] * (0.01942012754957055  + (0.10942832669749143*fabs(bR[63]))));
			inputSampleR += (bR[64] * (0.01572585258756565  + (0.11189204189054594*fabs(bR[64]))));
			inputSampleR += (bR[65] * (0.01490550715016034  + (0.04449822818925343*fabs(bR[65]))));
			inputSampleR += (bR[66] * (0.01715683226376727  - (0.06944648050933899*fabs(bR[66]))));
			inputSampleR += (bR[67] * (0.02822659878011318  - (0.17843652160132820*fabs(bR[67]))));
			inputSampleR += (bR[68] * (0.03758307610456144  - (0.21986013433664692*fabs(bR[68]))));
			inputSampleR += (bR[69] * (0.03275008021608433  - (0.15869878676112170*fabs(bR[69]))));
			inputSampleR += (bR[70] * (0.01855749786752354  - (0.02337224995718105*fabs(bR[70]))));
			inputSampleR += (bR[71] * (0.00217095395782931  + (0.10971764224593601*fabs(bR[71]))));
			inputSampleR -= (bR[72] * (0.01851381451105007  - (0.17214910008793413*fabs(bR[72]))));
			inputSampleR -= (bR[73] * (0.04722574936345419  - (0.14341588977845254*fabs(bR[73]))));
			inputSampleR -= (bR[74] * (0.07151540514482006  - (0.04684695724814321*fabs(bR[74]))));
			inputSampleR -= (bR[75] * (0.06827195484995092  + (0.07022207121861397*fabs(bR[75]))));
			inputSampleR -= (bR[76] * (0.03290227240464227  + (0.16328400808152735*fabs(bR[76]))));
			inputSampleR += (bR[77] * (0.01043861198275382  - (0.20184486126076279*fabs(bR[77]))));
			inputSampleR += (bR[78] * (0.03236563559476477  - (0.17125821306380920*fabs(bR[78]))));
			inputSampleR += (bR[79] * (0.02040121529932702  - (0.09103660189829657*fabs(bR[79]))));
			inputSampleR -= (bR[80] * (0.00509649513318102  + (0.01170360991547489*fabs(bR[80]))));
			inputSampleR -= (bR[81] * (0.01388353426600228  - (0.03588955538451771*fabs(bR[81]))));
			inputSampleR -= (bR[82] * (0.00523671715033842  - (0.07068798057534148*fabs(bR[82]))));
			inputSampleR += (bR[83] * (0.00665852487721137  + (0.11666210640054926*fabs(bR[83]))));
			inputSampleR += (bR[84] * (0.01593540832939290  + (0.15844892856402149*fabs(bR[84]))));
			inputSampleR += (bR[85] * (0.02080509201836796  + (0.17186274420065850*fabs(bR[85]))));
			
			temp = (inputSampleR + smoothCabBR)/3.0;
			smoothCabBR = inputSampleR;
			inputSampleR = temp/4.0;
			
			
			randy = ((double(fpdR)/UINT32_MAX)*0.079);
			drySampleR = ((((inputSampleR*(1.0-randy))+(lastCabSampleR*randy))*wet)+(drySampleR*(1.0-wet)))*outputlevel;
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

void LeadAmp::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double bassfill = A;
	double startlevel = bassfill;
	double samplerate = getSampleRate();
	double basstrim = bassfill / 10.0;
	double toneEQ = (B / samplerate)*22050.0;
	double EQ = (basstrim / samplerate)*22050.0;
	double outputlevel = C;
	double wet = D;
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	int cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 3 for silly people, 4 for 176 or 192k
	if (cycle > cycleEnd-1) cycle = cycleEnd-1; //sanity check		
	
	double bleed = outputlevel/16.0;
	double bassfactor = 1.0-(basstrim*basstrim);
	double BEQ = (bleed / samplerate)*22050.0;
	int diagonal = (int)(0.000861678*samplerate);
	if (diagonal > 127) diagonal = 127;
	int side = (int)(diagonal/1.4142135623730951);
	int down = (side + diagonal)/2;
	//now we've got down, side and diagonal as offsets and we also use three successive samples upfront
	
	double cutoff = (15000.0+(B*10000.0)) / getSampleRate();
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
		
		double basscutL = 0.99;
		//we're going to be shifting this as the stages progress
		double inputlevelL = startlevel;
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleAL = (iirSampleAL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleAL*basscutL);
		//highpass
		double bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		//overdrive
		bridgerectifier = (smoothAL + inputSampleL);
		smoothAL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		double basscutR = 0.99;
		//we're going to be shifting this as the stages progress
		double inputlevelR = startlevel;
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleAR = (iirSampleAR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleAR*basscutR);
		//highpass
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		//overdrive
		bridgerectifier = (smoothAR + inputSampleR);
		smoothAR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSampleL * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSampleL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSampleL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixB[fix_a0]) + fixB[fix_sR1];
		fixB[fix_sR1] = (inputSampleR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
		fixB[fix_sR2] = (inputSampleR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleBL = (iirSampleBL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleBL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothBL + inputSampleL);
		smoothBL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleBR = (iirSampleBR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleBR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothBR + inputSampleR);
		smoothBR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSampleL * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSampleL * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSampleL * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixC[fix_a0]) + fixC[fix_sR1];
		fixC[fix_sR1] = (inputSampleR * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sR2];
		fixC[fix_sR2] = (inputSampleR * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleCL = (iirSampleCL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleCL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothCL + inputSampleL);
		smoothCL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleCR = (iirSampleCR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleCR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothCR + inputSampleR);
		smoothCR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleDL = (iirSampleDL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleDL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothDL + inputSampleL);
		smoothDL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleDR = (iirSampleDR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleDR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothDR + inputSampleR);
		smoothDR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSampleL * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSampleL * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSampleL * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixD[fix_a0]) + fixD[fix_sR1];
		fixD[fix_sR1] = (inputSampleR * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sR2];
		fixD[fix_sR2] = (inputSampleR * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleEL = (iirSampleEL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleEL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothEL + inputSampleL);
		smoothEL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleER = (iirSampleER * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleER*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothER + inputSampleR);
		smoothER = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleFL = (iirSampleFL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleFL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothFL + inputSampleL);
		smoothFL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleFR = (iirSampleFR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleFR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothFR + inputSampleR);
		smoothFR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSampleL * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSampleL * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSampleL * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixE[fix_a0]) + fixE[fix_sR1];
		fixE[fix_sR1] = (inputSampleR * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sR2];
		fixE[fix_sR2] = (inputSampleR * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleGL = (iirSampleGL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleGL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothGL + inputSampleL);
		smoothGL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleGR = (iirSampleGR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleGR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothGR + inputSampleR);
		smoothGR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleHL = (iirSampleHL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleHL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothHL + inputSampleL);
		smoothHL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleHR = (iirSampleHR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleHR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothHR + inputSampleR);
		smoothHR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		outSample = (inputSampleL * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSampleL * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSampleL * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixF[fix_a0]) + fixF[fix_sR1];
		fixF[fix_sR1] = (inputSampleR * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sR2];
		fixF[fix_sR2] = (inputSampleR * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleIL = (iirSampleIL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleIL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothIL + inputSampleL);
		smoothIL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleIR = (iirSampleIR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleIR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothIR + inputSampleR);
		smoothIR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleJL = (iirSampleJL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleJL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothJL + inputSampleL);
		smoothJL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleJR = (iirSampleJR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleJR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothJR + inputSampleR);
		smoothJR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleL *= inputlevelL;
		inputlevelL = ((inputlevelL * 7.0)+1.0)/8.0;
		iirSampleKL = (iirSampleKL * (1.0 - EQ)) + (inputSampleL * EQ);
		basscutL *= bassfactor;
		inputSampleL = inputSampleL - (iirSampleKL*basscutL);
		//highpass
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633;
		if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//overdrive
		bridgerectifier = (smoothKL + inputSampleL);
		smoothKL = inputSampleL;
		inputSampleL = bridgerectifier;
		//two-sample averaging lowpass
		
		inputSampleR *= inputlevelR;
		inputlevelR = ((inputlevelR * 7.0)+1.0)/8.0;
		iirSampleKR = (iirSampleKR * (1.0 - EQ)) + (inputSampleR * EQ);
		basscutR *= bassfactor;
		inputSampleR = inputSampleR - (iirSampleKR*basscutR);
		//highpass
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633;
		if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//overdrive
		bridgerectifier = (smoothKR + inputSampleR);
		smoothKR = inputSampleR;
		inputSampleR = bridgerectifier;
		//two-sample averaging lowpass
		
		iirLowpassL = (iirLowpassL * (1.0 - toneEQ)) + (inputSampleL * toneEQ);
		inputSampleL = iirLowpassL;
		//lowpass. The only one of this type.
		
		iirLowpassR = (iirLowpassR * (1.0 - toneEQ)) + (inputSampleR * toneEQ);
		inputSampleR = iirLowpassR;
		//lowpass. The only one of this type.
		
		iirSpkAL = (iirSpkAL * (1.0 -  BEQ)) + (inputSampleL * BEQ);
		//extra lowpass for 4*12" speakers
		
		iirSpkAR = (iirSpkAR * (1.0 -  BEQ)) + (inputSampleR * BEQ);
		//extra lowpass for 4*12" speakers
		
		if (count < 0 || count > 128) {count = 128;}
		double resultBL = 0.0;
		double resultBR = 0.0;
		if (flip)
		{
			OddL[count+128] = OddL[count] = iirSpkAL;
			resultBL = (OddL[count+down] + OddL[count+side] + OddL[count+diagonal]);
			OddR[count+128] = OddR[count] = iirSpkAR;
			resultBR = (OddR[count+down] + OddR[count+side] + OddR[count+diagonal]);
		}
		else
		{
			EvenL[count+128] = EvenL[count] = iirSpkAL;
			resultBL = (EvenL[count+down] + EvenL[count+side] + EvenL[count+diagonal]);
			EvenR[count+128] = EvenR[count] = iirSpkAR;
			resultBR = (EvenR[count+down] + EvenR[count+side] + EvenR[count+diagonal]);
		}
		count--;
		
		iirSpkBL = (iirSpkBL * (1.0 - BEQ)) + (resultBL * BEQ);
		inputSampleL += (iirSpkBL*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSpkBR = (iirSpkBR * (1.0 - BEQ)) + (resultBR * BEQ);
		inputSampleR += (iirSpkBR*bleed);
		//extra lowpass for 4*12" speakers
		
		iirSubL = (iirSubL * (1.0 - BEQ)) + (inputSampleL * BEQ);
		inputSampleL += (iirSubL * bassfill);
		
		iirSubR = (iirSubR * (1.0 - BEQ)) + (inputSampleR * BEQ);
		inputSampleR += (iirSubR * bassfill);
		
		bridgerectifier = fabs(inputSampleL*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		
		bridgerectifier = fabs(inputSampleR*outputlevel);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		
		double randy = ((double(fpdL)/UINT32_MAX)*0.084);
		inputSampleL = ((inputSampleL*(1.0-randy))+(storeSampleL*randy))*outputlevel;
		storeSampleL = inputSampleL;
		
		randy = ((double(fpdR)/UINT32_MAX)*0.084);
		inputSampleR = ((inputSampleR*(1.0-randy))+(storeSampleR*randy))*outputlevel;
		storeSampleR = inputSampleR;
		
		flip = !flip;
		
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
			
			bL[85] = bL[84]; bL[84] = bL[83]; bL[83] = bL[82]; bL[82] = bL[81]; bL[81] = bL[80]; bL[80] = bL[79]; 
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
			inputSampleL += (bL[1] * (1.30406584776167445  - (0.01410622186823351*fabs(bL[1]))));
			inputSampleL += (bL[2] * (1.09350974154373559  + (0.34478044709202327*fabs(bL[2]))));
			inputSampleL += (bL[3] * (0.52285510059938256  + (0.84225842837363574*fabs(bL[3]))));
			inputSampleL -= (bL[4] * (0.00018126260714707  - (1.02446537989058117*fabs(bL[4]))));
			inputSampleL -= (bL[5] * (0.34943699771860115  - (0.84094709567790016*fabs(bL[5]))));
			inputSampleL -= (bL[6] * (0.53068048407937285  - (0.49231169327705593*fabs(bL[6]))));
			inputSampleL -= (bL[7] * (0.48631669406792399  - (0.08965111766223610*fabs(bL[7]))));
			inputSampleL -= (bL[8] * (0.28099201947014130  + (0.23921137841068607*fabs(bL[8]))));
			inputSampleL -= (bL[9] * (0.10333290012666248  + (0.35058962687321482*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.06605032198166226  + (0.23447405567823365*fabs(bL[10]))));
			inputSampleL -= (bL[11] * (0.10485808661261729  + (0.05025985449763527*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (0.13231190973014911  - (0.05484648240248013*fabs(bL[12]))));
			inputSampleL -= (bL[13] * (0.12926184767180304  - (0.04054223744746116*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.13802696739839460  + (0.01876754906568237*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.16548980700926913  + (0.06772130758771169*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.14469310965751475  + (0.10590928840978781*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.07838457396093310  + (0.13120101199677947*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.05123031606187391  + (0.13883400806512292*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.08906103481939850  + (0.07840461228402337*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.13939265522625241  + (0.01194366471800457*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.14957600717294034  + (0.07687598594361914*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.14112708654047090  + (0.20118461131186977*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.14961020766492997  + (0.30005716443826147*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.16130382224652270  + (0.40459872030013055*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.15679868471080052  + (0.47292767226083465*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.16456530552807727  + (0.45182121471666481*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.16852385701909278  + (0.38272684270752266*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.13317562760966850  + (0.28829580273670768*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.09396196532150952  + (0.18886898332071317*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.10133496956734221  + (0.11158788414137354*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.16097596389376778  + (0.02621299102374547*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.21419006394821866  - (0.03585678078834797*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.21273234570555244  - (0.02574469802924526*fabs(bL[33]))));
			inputSampleL -= (bL[34] * (0.16934948798707830  + (0.01354331184333835*fabs(bL[34]))));
			inputSampleL -= (bL[35] * (0.11970436472852493  + (0.04242183865883427*fabs(bL[35]))));
			inputSampleL -= (bL[36] * (0.09329023656747724  + (0.06890873292358397*fabs(bL[36]))));
			inputSampleL -= (bL[37] * (0.10255328436608116  + (0.11482972519137427*fabs(bL[37]))));
			inputSampleL -= (bL[38] * (0.13883223352796811  + (0.18016014431438840*fabs(bL[38]))));
			inputSampleL -= (bL[39] * (0.16532844286979087  + (0.24521957638633446*fabs(bL[39]))));
			inputSampleL -= (bL[40] * (0.16254607738965438  + (0.25669472097572482*fabs(bL[40]))));
			inputSampleL -= (bL[41] * (0.15353207135544752  + (0.15048064682912729*fabs(bL[41]))));
			inputSampleL -= (bL[42] * (0.13039046390746015  - (0.00200335414623601*fabs(bL[42]))));
			inputSampleL -= (bL[43] * (0.06707245032180627  - (0.06498125592578702*fabs(bL[43]))));
			inputSampleL += (bL[44] * (0.01427326441869788  + (0.01940451360783622*fabs(bL[44]))));
			inputSampleL += (bL[45] * (0.06151238306578224  - (0.07335755969763329*fabs(bL[45]))));
			inputSampleL += (bL[46] * (0.04685840498892526  - (0.14258849371688248*fabs(bL[46]))));
			inputSampleL -= (bL[47] * (0.00950136304466093  + (0.14379354707665129*fabs(bL[47]))));
			inputSampleL -= (bL[48] * (0.06245771575493557  + (0.07639718586346110*fabs(bL[48]))));
			inputSampleL -= (bL[49] * (0.07159593175777741  - (0.00595536565276915*fabs(bL[49]))));
			inputSampleL -= (bL[50] * (0.03167929390245019  - (0.03856769526301793*fabs(bL[50]))));
			inputSampleL += (bL[51] * (0.01890898565110766  + (0.00760539424271147*fabs(bL[51]))));
			inputSampleL += (bL[52] * (0.04926161137832240  - (0.06411014430053390*fabs(bL[52]))));
			inputSampleL += (bL[53] * (0.05768814623421683  - (0.15068618173358578*fabs(bL[53]))));
			inputSampleL += (bL[54] * (0.06144258297076708  - (0.21200636329120301*fabs(bL[54]))));
			inputSampleL += (bL[55] * (0.06348341960185613  - (0.19620269813094307*fabs(bL[55]))));
			inputSampleL += (bL[56] * (0.04877736350310589  - (0.11864999881200111*fabs(bL[56]))));
			inputSampleL += (bL[57] * (0.01010950997574472  - (0.02630070679113791*fabs(bL[57]))));
			inputSampleL -= (bL[58] * (0.02929178864801191  - (0.04439260202207482*fabs(bL[58]))));
			inputSampleL -= (bL[59] * (0.03484517126321562  - (0.04508635396034735*fabs(bL[59]))));
			inputSampleL -= (bL[60] * (0.00547176780437610  - (0.00205637806941426*fabs(bL[60]))));
			inputSampleL += (bL[61] * (0.02278296865283977  - (0.00063732526427685*fabs(bL[61]))));
			inputSampleL += (bL[62] * (0.02688982591366477  + (0.05333738901586284*fabs(bL[62]))));
			inputSampleL += (bL[63] * (0.01942012754957055  + (0.10942832669749143*fabs(bL[63]))));
			inputSampleL += (bL[64] * (0.01572585258756565  + (0.11189204189054594*fabs(bL[64]))));
			inputSampleL += (bL[65] * (0.01490550715016034  + (0.04449822818925343*fabs(bL[65]))));
			inputSampleL += (bL[66] * (0.01715683226376727  - (0.06944648050933899*fabs(bL[66]))));
			inputSampleL += (bL[67] * (0.02822659878011318  - (0.17843652160132820*fabs(bL[67]))));
			inputSampleL += (bL[68] * (0.03758307610456144  - (0.21986013433664692*fabs(bL[68]))));
			inputSampleL += (bL[69] * (0.03275008021608433  - (0.15869878676112170*fabs(bL[69]))));
			inputSampleL += (bL[70] * (0.01855749786752354  - (0.02337224995718105*fabs(bL[70]))));
			inputSampleL += (bL[71] * (0.00217095395782931  + (0.10971764224593601*fabs(bL[71]))));
			inputSampleL -= (bL[72] * (0.01851381451105007  - (0.17214910008793413*fabs(bL[72]))));
			inputSampleL -= (bL[73] * (0.04722574936345419  - (0.14341588977845254*fabs(bL[73]))));
			inputSampleL -= (bL[74] * (0.07151540514482006  - (0.04684695724814321*fabs(bL[74]))));
			inputSampleL -= (bL[75] * (0.06827195484995092  + (0.07022207121861397*fabs(bL[75]))));
			inputSampleL -= (bL[76] * (0.03290227240464227  + (0.16328400808152735*fabs(bL[76]))));
			inputSampleL += (bL[77] * (0.01043861198275382  - (0.20184486126076279*fabs(bL[77]))));
			inputSampleL += (bL[78] * (0.03236563559476477  - (0.17125821306380920*fabs(bL[78]))));
			inputSampleL += (bL[79] * (0.02040121529932702  - (0.09103660189829657*fabs(bL[79]))));
			inputSampleL -= (bL[80] * (0.00509649513318102  + (0.01170360991547489*fabs(bL[80]))));
			inputSampleL -= (bL[81] * (0.01388353426600228  - (0.03588955538451771*fabs(bL[81]))));
			inputSampleL -= (bL[82] * (0.00523671715033842  - (0.07068798057534148*fabs(bL[82]))));
			inputSampleL += (bL[83] * (0.00665852487721137  + (0.11666210640054926*fabs(bL[83]))));
			inputSampleL += (bL[84] * (0.01593540832939290  + (0.15844892856402149*fabs(bL[84]))));
			inputSampleL += (bL[85] * (0.02080509201836796  + (0.17186274420065850*fabs(bL[85]))));
			
			temp = (inputSampleL + smoothCabBL)/3.0;
			smoothCabBL = inputSampleL;
			inputSampleL = temp/4.0;
			
			randy = ((double(fpdL)/UINT32_MAX)*0.079);
			drySampleL = ((((inputSampleL*(1.0-randy))+(lastCabSampleL*randy))*wet)+(drySampleL*(1.0-wet)))*outputlevel;
			lastCabSampleL = inputSampleL;
			inputSampleL = drySampleL; //cab L
			
			
			temp = (inputSampleR + smoothCabAR)/3.0;
			smoothCabAR = inputSampleR;
			inputSampleR = temp;
			
			bR[85] = bR[84]; bR[84] = bR[83]; bR[83] = bR[82]; bR[82] = bR[81]; bR[81] = bR[80]; bR[80] = bR[79]; 
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
			inputSampleR += (bR[1] * (1.30406584776167445  - (0.01410622186823351*fabs(bR[1]))));
			inputSampleR += (bR[2] * (1.09350974154373559  + (0.34478044709202327*fabs(bR[2]))));
			inputSampleR += (bR[3] * (0.52285510059938256  + (0.84225842837363574*fabs(bR[3]))));
			inputSampleR -= (bR[4] * (0.00018126260714707  - (1.02446537989058117*fabs(bR[4]))));
			inputSampleR -= (bR[5] * (0.34943699771860115  - (0.84094709567790016*fabs(bR[5]))));
			inputSampleR -= (bR[6] * (0.53068048407937285  - (0.49231169327705593*fabs(bR[6]))));
			inputSampleR -= (bR[7] * (0.48631669406792399  - (0.08965111766223610*fabs(bR[7]))));
			inputSampleR -= (bR[8] * (0.28099201947014130  + (0.23921137841068607*fabs(bR[8]))));
			inputSampleR -= (bR[9] * (0.10333290012666248  + (0.35058962687321482*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.06605032198166226  + (0.23447405567823365*fabs(bR[10]))));
			inputSampleR -= (bR[11] * (0.10485808661261729  + (0.05025985449763527*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (0.13231190973014911  - (0.05484648240248013*fabs(bR[12]))));
			inputSampleR -= (bR[13] * (0.12926184767180304  - (0.04054223744746116*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.13802696739839460  + (0.01876754906568237*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.16548980700926913  + (0.06772130758771169*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.14469310965751475  + (0.10590928840978781*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.07838457396093310  + (0.13120101199677947*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.05123031606187391  + (0.13883400806512292*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.08906103481939850  + (0.07840461228402337*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.13939265522625241  + (0.01194366471800457*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.14957600717294034  + (0.07687598594361914*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.14112708654047090  + (0.20118461131186977*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.14961020766492997  + (0.30005716443826147*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.16130382224652270  + (0.40459872030013055*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.15679868471080052  + (0.47292767226083465*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.16456530552807727  + (0.45182121471666481*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.16852385701909278  + (0.38272684270752266*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.13317562760966850  + (0.28829580273670768*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.09396196532150952  + (0.18886898332071317*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.10133496956734221  + (0.11158788414137354*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.16097596389376778  + (0.02621299102374547*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.21419006394821866  - (0.03585678078834797*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.21273234570555244  - (0.02574469802924526*fabs(bR[33]))));
			inputSampleR -= (bR[34] * (0.16934948798707830  + (0.01354331184333835*fabs(bR[34]))));
			inputSampleR -= (bR[35] * (0.11970436472852493  + (0.04242183865883427*fabs(bR[35]))));
			inputSampleR -= (bR[36] * (0.09329023656747724  + (0.06890873292358397*fabs(bR[36]))));
			inputSampleR -= (bR[37] * (0.10255328436608116  + (0.11482972519137427*fabs(bR[37]))));
			inputSampleR -= (bR[38] * (0.13883223352796811  + (0.18016014431438840*fabs(bR[38]))));
			inputSampleR -= (bR[39] * (0.16532844286979087  + (0.24521957638633446*fabs(bR[39]))));
			inputSampleR -= (bR[40] * (0.16254607738965438  + (0.25669472097572482*fabs(bR[40]))));
			inputSampleR -= (bR[41] * (0.15353207135544752  + (0.15048064682912729*fabs(bR[41]))));
			inputSampleR -= (bR[42] * (0.13039046390746015  - (0.00200335414623601*fabs(bR[42]))));
			inputSampleR -= (bR[43] * (0.06707245032180627  - (0.06498125592578702*fabs(bR[43]))));
			inputSampleR += (bR[44] * (0.01427326441869788  + (0.01940451360783622*fabs(bR[44]))));
			inputSampleR += (bR[45] * (0.06151238306578224  - (0.07335755969763329*fabs(bR[45]))));
			inputSampleR += (bR[46] * (0.04685840498892526  - (0.14258849371688248*fabs(bR[46]))));
			inputSampleR -= (bR[47] * (0.00950136304466093  + (0.14379354707665129*fabs(bR[47]))));
			inputSampleR -= (bR[48] * (0.06245771575493557  + (0.07639718586346110*fabs(bR[48]))));
			inputSampleR -= (bR[49] * (0.07159593175777741  - (0.00595536565276915*fabs(bR[49]))));
			inputSampleR -= (bR[50] * (0.03167929390245019  - (0.03856769526301793*fabs(bR[50]))));
			inputSampleR += (bR[51] * (0.01890898565110766  + (0.00760539424271147*fabs(bR[51]))));
			inputSampleR += (bR[52] * (0.04926161137832240  - (0.06411014430053390*fabs(bR[52]))));
			inputSampleR += (bR[53] * (0.05768814623421683  - (0.15068618173358578*fabs(bR[53]))));
			inputSampleR += (bR[54] * (0.06144258297076708  - (0.21200636329120301*fabs(bR[54]))));
			inputSampleR += (bR[55] * (0.06348341960185613  - (0.19620269813094307*fabs(bR[55]))));
			inputSampleR += (bR[56] * (0.04877736350310589  - (0.11864999881200111*fabs(bR[56]))));
			inputSampleR += (bR[57] * (0.01010950997574472  - (0.02630070679113791*fabs(bR[57]))));
			inputSampleR -= (bR[58] * (0.02929178864801191  - (0.04439260202207482*fabs(bR[58]))));
			inputSampleR -= (bR[59] * (0.03484517126321562  - (0.04508635396034735*fabs(bR[59]))));
			inputSampleR -= (bR[60] * (0.00547176780437610  - (0.00205637806941426*fabs(bR[60]))));
			inputSampleR += (bR[61] * (0.02278296865283977  - (0.00063732526427685*fabs(bR[61]))));
			inputSampleR += (bR[62] * (0.02688982591366477  + (0.05333738901586284*fabs(bR[62]))));
			inputSampleR += (bR[63] * (0.01942012754957055  + (0.10942832669749143*fabs(bR[63]))));
			inputSampleR += (bR[64] * (0.01572585258756565  + (0.11189204189054594*fabs(bR[64]))));
			inputSampleR += (bR[65] * (0.01490550715016034  + (0.04449822818925343*fabs(bR[65]))));
			inputSampleR += (bR[66] * (0.01715683226376727  - (0.06944648050933899*fabs(bR[66]))));
			inputSampleR += (bR[67] * (0.02822659878011318  - (0.17843652160132820*fabs(bR[67]))));
			inputSampleR += (bR[68] * (0.03758307610456144  - (0.21986013433664692*fabs(bR[68]))));
			inputSampleR += (bR[69] * (0.03275008021608433  - (0.15869878676112170*fabs(bR[69]))));
			inputSampleR += (bR[70] * (0.01855749786752354  - (0.02337224995718105*fabs(bR[70]))));
			inputSampleR += (bR[71] * (0.00217095395782931  + (0.10971764224593601*fabs(bR[71]))));
			inputSampleR -= (bR[72] * (0.01851381451105007  - (0.17214910008793413*fabs(bR[72]))));
			inputSampleR -= (bR[73] * (0.04722574936345419  - (0.14341588977845254*fabs(bR[73]))));
			inputSampleR -= (bR[74] * (0.07151540514482006  - (0.04684695724814321*fabs(bR[74]))));
			inputSampleR -= (bR[75] * (0.06827195484995092  + (0.07022207121861397*fabs(bR[75]))));
			inputSampleR -= (bR[76] * (0.03290227240464227  + (0.16328400808152735*fabs(bR[76]))));
			inputSampleR += (bR[77] * (0.01043861198275382  - (0.20184486126076279*fabs(bR[77]))));
			inputSampleR += (bR[78] * (0.03236563559476477  - (0.17125821306380920*fabs(bR[78]))));
			inputSampleR += (bR[79] * (0.02040121529932702  - (0.09103660189829657*fabs(bR[79]))));
			inputSampleR -= (bR[80] * (0.00509649513318102  + (0.01170360991547489*fabs(bR[80]))));
			inputSampleR -= (bR[81] * (0.01388353426600228  - (0.03588955538451771*fabs(bR[81]))));
			inputSampleR -= (bR[82] * (0.00523671715033842  - (0.07068798057534148*fabs(bR[82]))));
			inputSampleR += (bR[83] * (0.00665852487721137  + (0.11666210640054926*fabs(bR[83]))));
			inputSampleR += (bR[84] * (0.01593540832939290  + (0.15844892856402149*fabs(bR[84]))));
			inputSampleR += (bR[85] * (0.02080509201836796  + (0.17186274420065850*fabs(bR[85]))));
			
			temp = (inputSampleR + smoothCabBR)/3.0;
			smoothCabBR = inputSampleR;
			inputSampleR = temp/4.0;
			
			
			randy = ((double(fpdR)/UINT32_MAX)*0.079);
			drySampleR = ((((inputSampleR*(1.0-randy))+(lastCabSampleR*randy))*wet)+(drySampleR*(1.0-wet)))*outputlevel;
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
