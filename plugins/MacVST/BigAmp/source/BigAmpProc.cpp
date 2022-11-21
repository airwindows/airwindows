/* ========================================
 *  BigAmp - BigAmp.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Gain_H
#include "BigAmp.h"
#endif

void BigAmp::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double bassfill = A;
	double basstrim = B;
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
	double inputlevel = bassfill*3.0;
	
	double samplerate = getSampleRate();
	double EQ = (B/samplerate)*22050.0;
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
		
		double skewL = (inputSampleL - lastSampleL);
		lastSampleL = inputSampleL;
		//skew will be direction/angle
		double bridgerectifier = fabs(skewL);
		if (bridgerectifier > 3.1415926) bridgerectifier = 3.1415926;
		//for skew we want it to go to zero effect again, so we use full range of the sine
		bridgerectifier = sin(bridgerectifier);
		if (skewL > 0) skewL = bridgerectifier;
		else skewL = -bridgerectifier;
		//skew is now sined and clamped and then re-amplified again
		skewL *= inputSampleL;
		skewL = (skewL+(skewL*basstrim))/2.0;
		inputSampleL *= basstrim;
		
		double skewR = (inputSampleR - lastSampleR);
		lastSampleR = inputSampleR;
		//skew will be direction/angle
		bridgerectifier = fabs(skewR);
		if (bridgerectifier > 3.1415926) bridgerectifier = 3.1415926;
		//for skew we want it to go to zero effect again, so we use full range of the sine
		bridgerectifier = sin(bridgerectifier);
		if (skewR > 0) skewR = bridgerectifier;
		else skewR = -bridgerectifier;
		//skew is now sined and clamped and then re-amplified again
		skewR *= inputSampleR;
		skewR = (skewR+(skewR*basstrim))/2.0;
		inputSampleR *= basstrim;
		double basscut = basstrim;
		//we're going to be shifting this as the stages progress
		
		outSample = (inputSampleL * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSampleL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSampleL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixB[fix_a0]) + fixB[fix_sR1];
		fixB[fix_sR1] = (inputSampleR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
		fixB[fix_sR2] = (inputSampleR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		double offsetL = (1.0 - EQ) + (fabs(inputSampleL)*EQ);
		if (offsetL < 0.0) offsetL = 0.0;
		if (offsetL > 1.0) offsetL = 1.0;
		iirSampleAL = (iirSampleAL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleAL*basscut);
		//highpass
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//overdrive
		iirSampleBL = (iirSampleBL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleBL*basscut);
		//highpass. Use offset from before gain stage
		//finished first gain stage
		
		inputSampleR *= inputlevel;
		double offsetR = (1.0 - EQ) + (fabs(inputSampleR)*EQ);
		if (offsetR < 0.0) offsetR = 0.0;
		if (offsetR > 1.0) offsetR = 1.0;
		iirSampleAR = (iirSampleAR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleAR*basscut);
		//highpass
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//overdrive
		iirSampleBR = (iirSampleBR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleBR*basscut);
		basscut /= 2.0;
		//highpass. Use offset from before gain stage
		//finished first gain stage
		
		outSample = (inputSampleL * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSampleL * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSampleL * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixC[fix_a0]) + fixC[fix_sR1];
		fixC[fix_sR1] = (inputSampleR * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sR2];
		fixC[fix_sR2] = (inputSampleR * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		skewL /= 2.0;
		offsetL = (1.0 + offsetL) / 2.0;
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//overdrive
		iirSampleCL = (iirSampleCL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleCL*basscut);
		//highpass.
		iirSampleDL = (iirSampleDL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = iirSampleDL;
		//lowpass. Use offset from before gain stage
		
		inputSampleR *= inputlevel;
		offsetR = (1.0 + offsetR) / 2.0;
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//overdrive
		iirSampleCR = (iirSampleCR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleCR*basscut);
		basscut /= 2.0;
		//highpass.
		iirSampleDR = (iirSampleDR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = iirSampleDR;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSampleL * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSampleL * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSampleL * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixD[fix_a0]) + fixD[fix_sR1];
		fixD[fix_sR1] = (inputSampleR * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sR2];
		fixD[fix_sR2] = (inputSampleR * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		skewL /= 2.0;
		offsetL = (1.0 + offsetL) / 2.0;
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//overdrive
		iirSampleEL = (iirSampleEL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleEL*basscut);
		//we don't need to do basscut again, that was the last one
		//highpass.
		iirSampleFL = (iirSampleFL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = iirSampleFL;
		//lowpass. Use offset from before gain stage
		
		inputSampleR *= inputlevel;
		skewR /= 2.0;
		offsetR = (1.0 + offsetR) / 2.0;
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//overdrive
		iirSampleER = (iirSampleER * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleER*basscut);
		//we don't need to do basscut again, that was the last one
		//highpass.
		iirSampleFR = (iirSampleFR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = iirSampleFR;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSampleL * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSampleL * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSampleL * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixE[fix_a0]) + fixE[fix_sR1];
		fixE[fix_sR1] = (inputSampleR * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sR2];
		fixE[fix_sR2] = (inputSampleR * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		skewL /= 2.0;
		offsetL= (1.0 + offsetL) / 2.0;
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//output stage has less gain, no highpass, straight lowpass
		iirSampleGL = (iirSampleGL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = iirSampleGL;
		//lowpass. Use offset from before gain stage
		iirSampleHL = (iirSampleHL * (1.0 - (offsetL * BEQ))) + (inputSampleL * (offsetL * BEQ));
		//extra lowpass for 4*12" speakers
		
		inputSampleR *= inputlevel;
		skewR /= 2.0;
		offsetR = (1.0 + offsetR) / 2.0;
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//output stage has less gain, no highpass, straight lowpass
		iirSampleGR = (iirSampleGR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = iirSampleGR;
		//lowpass. Use offset from before gain stage
		iirSampleHR = (iirSampleHR * (1.0 - (offsetR * BEQ))) + (inputSampleR * (offsetR * BEQ));
		//extra lowpass for 4*12" speakers
		
		
		if (count < 0 || count > 128) count = 128;
		double resultBL = 0.0;
		double resultBR = 0.0;
		if (flip)
		{
			OddL[count+128] = OddL[count] = iirSampleHL;
			resultBL = (OddL[count+down] + OddL[count+side] + OddL[count+diagonal]);
			OddR[count+128] = OddR[count] = iirSampleHR;
			resultBR = (OddR[count+down] + OddR[count+side] + OddR[count+diagonal]);
		} else {
			EvenL[count+128] = EvenL[count] = iirSampleHL;
			resultBL = (EvenL[count+down] + EvenL[count+side] + EvenL[count+diagonal]);
			EvenR[count+128] = EvenR[count] = iirSampleHR;
			resultBR = (EvenR[count+down] + EvenR[count+side] + EvenR[count+diagonal]);
		}
		count--;
		
		iirSampleIL = (iirSampleIL * (1.0 - (offsetL * BEQ))) + (resultBL * (offsetL * BEQ));
		inputSampleL += (iirSampleIL*bleed);
		//extra lowpass for 4*12" speakers
		iirSampleJL = (iirSampleJL * (1.0 - (offsetL * BEQ))) + (inputSampleL * (offsetL * BEQ));
		inputSampleL += (iirSampleJL * bassfill);
		inputSampleL = sin(inputSampleL*outputlevel);
		double randy = ((double(fpdL)/UINT32_MAX)*0.04);
		inputSampleL = ((inputSampleL*(1-randy))+(storeSampleL*randy))*outputlevel;
		storeSampleL = inputSampleL;
		
		iirSampleIR = (iirSampleIR * (1.0 - (offsetR * BEQ))) + (resultBR * (offsetR * BEQ));
		inputSampleR += (iirSampleIR*bleed);
		//extra lowpass for 4*12" speakers
		iirSampleJR = (iirSampleJR * (1.0 - (offsetR * BEQ))) + (inputSampleR * (offsetR * BEQ));
		inputSampleR += (iirSampleJR * bassfill);
		inputSampleR = sin(inputSampleR*outputlevel);
		randy = ((double(fpdR)/UINT32_MAX)*0.04);
		inputSampleR = ((inputSampleR*(1-randy))+(storeSampleR*randy))*outputlevel;
		storeSampleR = inputSampleR;
		
		outSample = (inputSampleL * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSampleL * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSampleL * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixF[fix_a0]) + fixF[fix_sR1];
		fixF[fix_sR1] = (inputSampleR * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sR2];
		fixF[fix_sR2] = (inputSampleR * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
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
			
			bL[81] = bL[80]; bL[80] = bL[79]; 
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
			
			inputSampleL += (bL[1] * (1.35472031405494242  + (0.00220914099195157*fabs(bL[1]))));
			inputSampleL += (bL[2] * (1.63534207755253003  - (0.11406232654509685*fabs(bL[2]))));
			inputSampleL += (bL[3] * (1.82334575691525869  - (0.42647194712964054*fabs(bL[3]))));
			inputSampleL += (bL[4] * (1.86156386235405868  - (0.76744187887586590*fabs(bL[4]))));
			inputSampleL += (bL[5] * (1.67332739338852599  - (0.95161997324293013*fabs(bL[5]))));
			inputSampleL += (bL[6] * (1.25054130794899021  - (0.98410433514572859*fabs(bL[6]))));
			inputSampleL += (bL[7] * (0.70049121047281737  - (0.87375612110718992*fabs(bL[7]))));
			inputSampleL += (bL[8] * (0.15291791448081560  - (0.61195266024519046*fabs(bL[8]))));
			inputSampleL -= (bL[9] * (0.37301992914152693  + (0.16755422915252094*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.76568539228498433  - (0.28554435228965386*fabs(bL[10]))));
			inputSampleL -= (bL[11] * (0.95726568749937369  - (0.61659719162806048*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (1.01273552193911032  - (0.81827288407943954*fabs(bL[12]))));
			inputSampleL -= (bL[13] * (0.93920108117234447  - (0.80077111864205874*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.79831898832953974  - (0.65814750339694406*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.64200088100452313  - (0.46135833001232618*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.48807302802822128  - (0.15506178974799034*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.36545171501947982  + (0.16126103769376721*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.31469581455759105  + (0.32250870039053953*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.36893534817945800  + (0.25409418897237473*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.41092557722975687  + (0.13114730488878301*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.38584044480710594  + (0.06825323739722661*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.33378434007178670  + (0.04144255489164217*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.26144203061699706  + (0.06031313105098152*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.25818342000920502  + (0.03642289242586355*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.28096018498822661  + (0.00976973667327174*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.25845682019095384  + (0.02749015358080831*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.26655607865953096  - (0.00329839675455690*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.30590085026938518  - (0.07375043215328811*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.32875683916470899  - (0.12454134857516502*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.38166643180506560  - (0.19973911428609989*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.49068186937289598  - (0.34785166842136384*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.60274753867622777  - (0.48685038872711034*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.65944678627090636  - (0.49844657885975518*fabs(bL[33]))));
			inputSampleL -= (bL[34] * (0.64488955808717285  - (0.40514406499806987*fabs(bL[34]))));
			inputSampleL -= (bL[35] * (0.55818730353434354  - (0.28029870614987346*fabs(bL[35]))));
			inputSampleL -= (bL[36] * (0.43110859113387556  - (0.15373504582939335*fabs(bL[36]))));
			inputSampleL -= (bL[37] * (0.37726894966096269  - (0.11570983506028532*fabs(bL[37]))));
			inputSampleL -= (bL[38] * (0.39953242355200935  - (0.17879231130484088*fabs(bL[38]))));
			inputSampleL -= (bL[39] * (0.36726676379100875  - (0.22013553023983223*fabs(bL[39]))));
			inputSampleL -= (bL[40] * (0.27187029469227386  - (0.18461171768478427*fabs(bL[40]))));
			inputSampleL -= (bL[41] * (0.21109334552321635  - (0.14497481318083569*fabs(bL[41]))));
			inputSampleL -= (bL[42] * (0.19808797405293213  - (0.14916579928186940*fabs(bL[42]))));
			inputSampleL -= (bL[43] * (0.16287926785495671  - (0.15146098461120627*fabs(bL[43]))));
			inputSampleL -= (bL[44] * (0.11086621477163359  - (0.13182973443924018*fabs(bL[44]))));
			inputSampleL -= (bL[45] * (0.07531043236890560  - (0.08062172796472888*fabs(bL[45]))));
			inputSampleL -= (bL[46] * (0.01747364473230771  + (0.02201865873632456*fabs(bL[46]))));
			inputSampleL += (bL[47] * (0.03080279125662693  - (0.08721756240972101*fabs(bL[47]))));
			inputSampleL += (bL[48] * (0.02354148659185142  - (0.06376361763053796*fabs(bL[48]))));
			inputSampleL -= (bL[49] * (0.02835772372098715  + (0.00589978513642627*fabs(bL[49]))));
			inputSampleL -= (bL[50] * (0.08983370744565244  - (0.02350960427706536*fabs(bL[50]))));
			inputSampleL -= (bL[51] * (0.14148947620055380  - (0.03329826628693369*fabs(bL[51]))));
			inputSampleL -= (bL[52] * (0.17576502674572581  - (0.06507546651241880*fabs(bL[52]))));
			inputSampleL -= (bL[53] * (0.17168865666573860  - (0.07734801128437317*fabs(bL[53]))));
			inputSampleL -= (bL[54] * (0.14107027738292105  - (0.03136459344220402*fabs(bL[54]))));
			inputSampleL -= (bL[55] * (0.12287163395380074  + (0.01933408169185258*fabs(bL[55]))));
			inputSampleL -= (bL[56] * (0.12276622398112971  + (0.01983508766241737*fabs(bL[56]))));
			inputSampleL -= (bL[57] * (0.12349721440213673  - (0.01111031415304768*fabs(bL[57]))));
			inputSampleL -= (bL[58] * (0.08649454142716655  + (0.02252815645513927*fabs(bL[58]))));
			inputSampleL -= (bL[59] * (0.00953083685474757  + (0.13778878548343007*fabs(bL[59]))));
			inputSampleL += (bL[60] * (0.06045983158868478  - (0.23966318224935096*fabs(bL[60]))));
			inputSampleL += (bL[61] * (0.09053229817093242  - (0.27190119941572544*fabs(bL[61]))));
			inputSampleL += (bL[62] * (0.08112662178843048  - (0.22456862606452327*fabs(bL[62]))));
			inputSampleL += (bL[63] * (0.07503525686243730  - (0.14330154410548213*fabs(bL[63]))));
			inputSampleL += (bL[64] * (0.07372595404399729  - (0.06185193766408734*fabs(bL[64]))));
			inputSampleL += (bL[65] * (0.06073789200080433  + (0.01261857435786178*fabs(bL[65]))));
			inputSampleL += (bL[66] * (0.04616712695742254  + (0.05851771967084609*fabs(bL[66]))));
			inputSampleL += (bL[67] * (0.01036235510345900  + (0.08286534414423796*fabs(bL[67]))));
			inputSampleL -= (bL[68] * (0.03708389413229191  - (0.06695282381039531*fabs(bL[68]))));
			inputSampleL -= (bL[69] * (0.07092204876981217  - (0.01915829199112784*fabs(bL[69]))));
			inputSampleL -= (bL[70] * (0.09443579589460312  + (0.01210082455316246*fabs(bL[70]))));
			inputSampleL -= (bL[71] * (0.07824038577769601  + (0.06121988546065113*fabs(bL[71]))));
			inputSampleL -= (bL[72] * (0.00854730633079399  + (0.14468518752295506*fabs(bL[72]))));
			inputSampleL += (bL[73] * (0.06845589924191028  - (0.18902431382592944*fabs(bL[73]))));
			inputSampleL += (bL[74] * (0.10351569998375465  - (0.13204443060279647*fabs(bL[74]))));
			inputSampleL += (bL[75] * (0.10513368758532179  - (0.02993199294485649*fabs(bL[75]))));
			inputSampleL += (bL[76] * (0.08896978950235003  + (0.04074499273825906*fabs(bL[76]))));
			inputSampleL += (bL[77] * (0.03697537734050980  + (0.09217751130846838*fabs(bL[77]))));
			inputSampleL -= (bL[78] * (0.04014322441280276  - (0.14062297149365666*fabs(bL[78]))));
			inputSampleL -= (bL[79] * (0.10505934581398618  - (0.16988861157275814*fabs(bL[79]))));
			inputSampleL -= (bL[80] * (0.13937661651676272  - (0.15083294570551492*fabs(bL[80]))));
			inputSampleL -= (bL[81] * (0.13183458845108439  - (0.06657454442471208*fabs(bL[81]))));
			
			temp = (inputSampleL + smoothCabBL)/3.0;
			smoothCabBL = inputSampleL;
			inputSampleL = temp/4.0;
			
			randy = ((double(fpdL)/UINT32_MAX)*0.05);
			drySampleL = ((((inputSampleL*(1-randy))+(lastCabSampleL*randy))*wet)+(drySampleL*(1.0-wet)))*outputlevel;
			lastCabSampleL = inputSampleL;
			inputSampleL = drySampleL; //cab L
			
			
			temp = (inputSampleR + smoothCabAR)/3.0;
			smoothCabAR = inputSampleR;
			inputSampleR = temp;
			
			bR[81] = bR[80]; bR[80] = bR[79]; 
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
			
			inputSampleR += (bR[1] * (1.35472031405494242  + (0.00220914099195157*fabs(bR[1]))));
			inputSampleR += (bR[2] * (1.63534207755253003  - (0.11406232654509685*fabs(bR[2]))));
			inputSampleR += (bR[3] * (1.82334575691525869  - (0.42647194712964054*fabs(bR[3]))));
			inputSampleR += (bR[4] * (1.86156386235405868  - (0.76744187887586590*fabs(bR[4]))));
			inputSampleR += (bR[5] * (1.67332739338852599  - (0.95161997324293013*fabs(bR[5]))));
			inputSampleR += (bR[6] * (1.25054130794899021  - (0.98410433514572859*fabs(bR[6]))));
			inputSampleR += (bR[7] * (0.70049121047281737  - (0.87375612110718992*fabs(bR[7]))));
			inputSampleR += (bR[8] * (0.15291791448081560  - (0.61195266024519046*fabs(bR[8]))));
			inputSampleR -= (bR[9] * (0.37301992914152693  + (0.16755422915252094*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.76568539228498433  - (0.28554435228965386*fabs(bR[10]))));
			inputSampleR -= (bR[11] * (0.95726568749937369  - (0.61659719162806048*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (1.01273552193911032  - (0.81827288407943954*fabs(bR[12]))));
			inputSampleR -= (bR[13] * (0.93920108117234447  - (0.80077111864205874*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.79831898832953974  - (0.65814750339694406*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.64200088100452313  - (0.46135833001232618*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.48807302802822128  - (0.15506178974799034*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.36545171501947982  + (0.16126103769376721*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.31469581455759105  + (0.32250870039053953*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.36893534817945800  + (0.25409418897237473*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.41092557722975687  + (0.13114730488878301*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.38584044480710594  + (0.06825323739722661*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.33378434007178670  + (0.04144255489164217*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.26144203061699706  + (0.06031313105098152*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.25818342000920502  + (0.03642289242586355*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.28096018498822661  + (0.00976973667327174*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.25845682019095384  + (0.02749015358080831*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.26655607865953096  - (0.00329839675455690*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.30590085026938518  - (0.07375043215328811*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.32875683916470899  - (0.12454134857516502*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.38166643180506560  - (0.19973911428609989*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.49068186937289598  - (0.34785166842136384*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.60274753867622777  - (0.48685038872711034*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.65944678627090636  - (0.49844657885975518*fabs(bR[33]))));
			inputSampleR -= (bR[34] * (0.64488955808717285  - (0.40514406499806987*fabs(bR[34]))));
			inputSampleR -= (bR[35] * (0.55818730353434354  - (0.28029870614987346*fabs(bR[35]))));
			inputSampleR -= (bR[36] * (0.43110859113387556  - (0.15373504582939335*fabs(bR[36]))));
			inputSampleR -= (bR[37] * (0.37726894966096269  - (0.11570983506028532*fabs(bR[37]))));
			inputSampleR -= (bR[38] * (0.39953242355200935  - (0.17879231130484088*fabs(bR[38]))));
			inputSampleR -= (bR[39] * (0.36726676379100875  - (0.22013553023983223*fabs(bR[39]))));
			inputSampleR -= (bR[40] * (0.27187029469227386  - (0.18461171768478427*fabs(bR[40]))));
			inputSampleR -= (bR[41] * (0.21109334552321635  - (0.14497481318083569*fabs(bR[41]))));
			inputSampleR -= (bR[42] * (0.19808797405293213  - (0.14916579928186940*fabs(bR[42]))));
			inputSampleR -= (bR[43] * (0.16287926785495671  - (0.15146098461120627*fabs(bR[43]))));
			inputSampleR -= (bR[44] * (0.11086621477163359  - (0.13182973443924018*fabs(bR[44]))));
			inputSampleR -= (bR[45] * (0.07531043236890560  - (0.08062172796472888*fabs(bR[45]))));
			inputSampleR -= (bR[46] * (0.01747364473230771  + (0.02201865873632456*fabs(bR[46]))));
			inputSampleR += (bR[47] * (0.03080279125662693  - (0.08721756240972101*fabs(bR[47]))));
			inputSampleR += (bR[48] * (0.02354148659185142  - (0.06376361763053796*fabs(bR[48]))));
			inputSampleR -= (bR[49] * (0.02835772372098715  + (0.00589978513642627*fabs(bR[49]))));
			inputSampleR -= (bR[50] * (0.08983370744565244  - (0.02350960427706536*fabs(bR[50]))));
			inputSampleR -= (bR[51] * (0.14148947620055380  - (0.03329826628693369*fabs(bR[51]))));
			inputSampleR -= (bR[52] * (0.17576502674572581  - (0.06507546651241880*fabs(bR[52]))));
			inputSampleR -= (bR[53] * (0.17168865666573860  - (0.07734801128437317*fabs(bR[53]))));
			inputSampleR -= (bR[54] * (0.14107027738292105  - (0.03136459344220402*fabs(bR[54]))));
			inputSampleR -= (bR[55] * (0.12287163395380074  + (0.01933408169185258*fabs(bR[55]))));
			inputSampleR -= (bR[56] * (0.12276622398112971  + (0.01983508766241737*fabs(bR[56]))));
			inputSampleR -= (bR[57] * (0.12349721440213673  - (0.01111031415304768*fabs(bR[57]))));
			inputSampleR -= (bR[58] * (0.08649454142716655  + (0.02252815645513927*fabs(bR[58]))));
			inputSampleR -= (bR[59] * (0.00953083685474757  + (0.13778878548343007*fabs(bR[59]))));
			inputSampleR += (bR[60] * (0.06045983158868478  - (0.23966318224935096*fabs(bR[60]))));
			inputSampleR += (bR[61] * (0.09053229817093242  - (0.27190119941572544*fabs(bR[61]))));
			inputSampleR += (bR[62] * (0.08112662178843048  - (0.22456862606452327*fabs(bR[62]))));
			inputSampleR += (bR[63] * (0.07503525686243730  - (0.14330154410548213*fabs(bR[63]))));
			inputSampleR += (bR[64] * (0.07372595404399729  - (0.06185193766408734*fabs(bR[64]))));
			inputSampleR += (bR[65] * (0.06073789200080433  + (0.01261857435786178*fabs(bR[65]))));
			inputSampleR += (bR[66] * (0.04616712695742254  + (0.05851771967084609*fabs(bR[66]))));
			inputSampleR += (bR[67] * (0.01036235510345900  + (0.08286534414423796*fabs(bR[67]))));
			inputSampleR -= (bR[68] * (0.03708389413229191  - (0.06695282381039531*fabs(bR[68]))));
			inputSampleR -= (bR[69] * (0.07092204876981217  - (0.01915829199112784*fabs(bR[69]))));
			inputSampleR -= (bR[70] * (0.09443579589460312  + (0.01210082455316246*fabs(bR[70]))));
			inputSampleR -= (bR[71] * (0.07824038577769601  + (0.06121988546065113*fabs(bR[71]))));
			inputSampleR -= (bR[72] * (0.00854730633079399  + (0.14468518752295506*fabs(bR[72]))));
			inputSampleR += (bR[73] * (0.06845589924191028  - (0.18902431382592944*fabs(bR[73]))));
			inputSampleR += (bR[74] * (0.10351569998375465  - (0.13204443060279647*fabs(bR[74]))));
			inputSampleR += (bR[75] * (0.10513368758532179  - (0.02993199294485649*fabs(bR[75]))));
			inputSampleR += (bR[76] * (0.08896978950235003  + (0.04074499273825906*fabs(bR[76]))));
			inputSampleR += (bR[77] * (0.03697537734050980  + (0.09217751130846838*fabs(bR[77]))));
			inputSampleR -= (bR[78] * (0.04014322441280276  - (0.14062297149365666*fabs(bR[78]))));
			inputSampleR -= (bR[79] * (0.10505934581398618  - (0.16988861157275814*fabs(bR[79]))));
			inputSampleR -= (bR[80] * (0.13937661651676272  - (0.15083294570551492*fabs(bR[80]))));
			inputSampleR -= (bR[81] * (0.13183458845108439  - (0.06657454442471208*fabs(bR[81]))));
			
			temp = (inputSampleR + smoothCabBR)/3.0;
			smoothCabBR = inputSampleR;
			inputSampleR = temp/4.0;
			
			
			randy = ((double(fpdR)/UINT32_MAX)*0.05);
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

void BigAmp::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double bassfill = A;
	double basstrim = B;
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
	double inputlevel = bassfill*3.0;
	
	double samplerate = getSampleRate();
	double EQ = (B/samplerate)*22050.0;
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
		
		double skewL = (inputSampleL - lastSampleL);
		lastSampleL = inputSampleL;
		//skew will be direction/angle
		double bridgerectifier = fabs(skewL);
		if (bridgerectifier > 3.1415926) bridgerectifier = 3.1415926;
		//for skew we want it to go to zero effect again, so we use full range of the sine
		bridgerectifier = sin(bridgerectifier);
		if (skewL > 0) skewL = bridgerectifier;
		else skewL = -bridgerectifier;
		//skew is now sined and clamped and then re-amplified again
		skewL *= inputSampleL;
		skewL = (skewL+(skewL*basstrim))/2.0;
		inputSampleL *= basstrim;
		
		double skewR = (inputSampleR - lastSampleR);
		lastSampleR = inputSampleR;
		//skew will be direction/angle
		bridgerectifier = fabs(skewR);
		if (bridgerectifier > 3.1415926) bridgerectifier = 3.1415926;
		//for skew we want it to go to zero effect again, so we use full range of the sine
		bridgerectifier = sin(bridgerectifier);
		if (skewR > 0) skewR = bridgerectifier;
		else skewR = -bridgerectifier;
		//skew is now sined and clamped and then re-amplified again
		skewR *= inputSampleR;
		skewR = (skewR+(skewR*basstrim))/2.0;
		inputSampleR *= basstrim;
		double basscut = basstrim;
		//we're going to be shifting this as the stages progress
		
		outSample = (inputSampleL * fixB[fix_a0]) + fixB[fix_sL1];
		fixB[fix_sL1] = (inputSampleL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
		fixB[fix_sL2] = (inputSampleL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixB[fix_a0]) + fixB[fix_sR1];
		fixB[fix_sR1] = (inputSampleR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
		fixB[fix_sR2] = (inputSampleR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		double offsetL = (1.0 - EQ) + (fabs(inputSampleL)*EQ);
		if (offsetL < 0.0) offsetL = 0.0;
		if (offsetL > 1.0) offsetL = 1.0;
		iirSampleAL = (iirSampleAL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleAL*basscut);
		//highpass
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//overdrive
		iirSampleBL = (iirSampleBL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleBL*basscut);
		//highpass. Use offset from before gain stage
		//finished first gain stage
		
		inputSampleR *= inputlevel;
		double offsetR = (1.0 - EQ) + (fabs(inputSampleR)*EQ);
		if (offsetR < 0.0) offsetR = 0.0;
		if (offsetR > 1.0) offsetR = 1.0;
		iirSampleAR = (iirSampleAR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleAR*basscut);
		//highpass
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//overdrive
		iirSampleBR = (iirSampleBR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleBR*basscut);
		basscut /= 2.0;
		//highpass. Use offset from before gain stage
		//finished first gain stage
		
		outSample = (inputSampleL * fixC[fix_a0]) + fixC[fix_sL1];
		fixC[fix_sL1] = (inputSampleL * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sL2];
		fixC[fix_sL2] = (inputSampleL * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixC[fix_a0]) + fixC[fix_sR1];
		fixC[fix_sR1] = (inputSampleR * fixC[fix_a1]) - (outSample * fixC[fix_b1]) + fixC[fix_sR2];
		fixC[fix_sR2] = (inputSampleR * fixC[fix_a2]) - (outSample * fixC[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		skewL /= 2.0;
		offsetL = (1.0 + offsetL) / 2.0;
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//overdrive
		iirSampleCL = (iirSampleCL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleCL*basscut);
		//highpass.
		iirSampleDL = (iirSampleDL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = iirSampleDL;
		//lowpass. Use offset from before gain stage
		
		inputSampleR *= inputlevel;
		offsetR = (1.0 + offsetR) / 2.0;
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//overdrive
		iirSampleCR = (iirSampleCR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleCR*basscut);
		basscut /= 2.0;
		//highpass.
		iirSampleDR = (iirSampleDR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = iirSampleDR;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSampleL * fixD[fix_a0]) + fixD[fix_sL1];
		fixD[fix_sL1] = (inputSampleL * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sL2];
		fixD[fix_sL2] = (inputSampleL * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixD[fix_a0]) + fixD[fix_sR1];
		fixD[fix_sR1] = (inputSampleR * fixD[fix_a1]) - (outSample * fixD[fix_b1]) + fixD[fix_sR2];
		fixD[fix_sR2] = (inputSampleR * fixD[fix_a2]) - (outSample * fixD[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		skewL /= 2.0;
		offsetL = (1.0 + offsetL) / 2.0;
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//overdrive
		iirSampleEL = (iirSampleEL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = inputSampleL - (iirSampleEL*basscut);
		//we don't need to do basscut again, that was the last one
		//highpass.
		iirSampleFL = (iirSampleFL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = iirSampleFL;
		//lowpass. Use offset from before gain stage
		
		inputSampleR *= inputlevel;
		skewR /= 2.0;
		offsetR = (1.0 + offsetR) / 2.0;
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = (sin(bridgerectifier) * 1.57079633) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//overdrive
		iirSampleER = (iirSampleER * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = inputSampleR - (iirSampleER*basscut);
		//we don't need to do basscut again, that was the last one
		//highpass.
		iirSampleFR = (iirSampleFR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = iirSampleFR;
		//lowpass. Use offset from before gain stage
		
		outSample = (inputSampleL * fixE[fix_a0]) + fixE[fix_sL1];
		fixE[fix_sL1] = (inputSampleL * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sL2];
		fixE[fix_sL2] = (inputSampleL * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixE[fix_a0]) + fixE[fix_sR1];
		fixE[fix_sR1] = (inputSampleR * fixE[fix_a1]) - (outSample * fixE[fix_b1]) + fixE[fix_sR2];
		fixE[fix_sR2] = (inputSampleR * fixE[fix_a2]) - (outSample * fixE[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
		inputSampleL *= inputlevel;
		skewL /= 2.0;
		offsetL= (1.0 + offsetL) / 2.0;
		bridgerectifier = fabs(inputSampleL) + skewL;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(-0.57079633+skewL))+(bridgerectifier*(1.57079633+skewL));
		else inputSampleL = (inputSampleL*(-0.57079633+skewL))-(bridgerectifier*(1.57079633+skewL));
		//output stage has less gain, no highpass, straight lowpass
		iirSampleGL = (iirSampleGL * (1.0 - (offsetL * EQ))) + (inputSampleL * (offsetL * EQ));
		inputSampleL = iirSampleGL;
		//lowpass. Use offset from before gain stage
		iirSampleHL = (iirSampleHL * (1.0 - (offsetL * BEQ))) + (inputSampleL * (offsetL * BEQ));
		//extra lowpass for 4*12" speakers
		
		inputSampleR *= inputlevel;
		skewR /= 2.0;
		offsetR = (1.0 + offsetR) / 2.0;
		bridgerectifier = fabs(inputSampleR) + skewR;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = sin(bridgerectifier) * 1.57079633;
		if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(-0.57079633+skewR))+(bridgerectifier*(1.57079633+skewR));
		else inputSampleR = (inputSampleR*(-0.57079633+skewR))-(bridgerectifier*(1.57079633+skewR));
		//output stage has less gain, no highpass, straight lowpass
		iirSampleGR = (iirSampleGR * (1.0 - (offsetR * EQ))) + (inputSampleR * (offsetR * EQ));
		inputSampleR = iirSampleGR;
		//lowpass. Use offset from before gain stage
		iirSampleHR = (iirSampleHR * (1.0 - (offsetR * BEQ))) + (inputSampleR * (offsetR * BEQ));
		//extra lowpass for 4*12" speakers
		
		
		if (count < 0 || count > 128) count = 128;
		double resultBL = 0.0;
		double resultBR = 0.0;
		if (flip)
		{
			OddL[count+128] = OddL[count] = iirSampleHL;
			resultBL = (OddL[count+down] + OddL[count+side] + OddL[count+diagonal]);
			OddR[count+128] = OddR[count] = iirSampleHR;
			resultBR = (OddR[count+down] + OddR[count+side] + OddR[count+diagonal]);
		} else {
			EvenL[count+128] = EvenL[count] = iirSampleHL;
			resultBL = (EvenL[count+down] + EvenL[count+side] + EvenL[count+diagonal]);
			EvenR[count+128] = EvenR[count] = iirSampleHR;
			resultBR = (EvenR[count+down] + EvenR[count+side] + EvenR[count+diagonal]);
		}
		count--;
		
		iirSampleIL = (iirSampleIL * (1.0 - (offsetL * BEQ))) + (resultBL * (offsetL * BEQ));
		inputSampleL += (iirSampleIL*bleed);
		//extra lowpass for 4*12" speakers
		iirSampleJL = (iirSampleJL * (1.0 - (offsetL * BEQ))) + (inputSampleL * (offsetL * BEQ));
		inputSampleL += (iirSampleJL * bassfill);
		inputSampleL = sin(inputSampleL*outputlevel);
		double randy = ((double(fpdL)/UINT32_MAX)*0.04);
		inputSampleL = ((inputSampleL*(1-randy))+(storeSampleL*randy))*outputlevel;
		storeSampleL = inputSampleL;
		
		iirSampleIR = (iirSampleIR * (1.0 - (offsetR * BEQ))) + (resultBR * (offsetR * BEQ));
		inputSampleR += (iirSampleIR*bleed);
		//extra lowpass for 4*12" speakers
		iirSampleJR = (iirSampleJR * (1.0 - (offsetR * BEQ))) + (inputSampleR * (offsetR * BEQ));
		inputSampleR += (iirSampleJR * bassfill);
		inputSampleR = sin(inputSampleR*outputlevel);
		randy = ((double(fpdR)/UINT32_MAX)*0.04);
		inputSampleR = ((inputSampleR*(1-randy))+(storeSampleR*randy))*outputlevel;
		storeSampleR = inputSampleR;
		
		outSample = (inputSampleL * fixF[fix_a0]) + fixF[fix_sL1];
		fixF[fix_sL1] = (inputSampleL * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sL2];
		fixF[fix_sL2] = (inputSampleL * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleL = outSample; //fixed biquad filtering ultrasonics
		outSample = (inputSampleR * fixF[fix_a0]) + fixF[fix_sR1];
		fixF[fix_sR1] = (inputSampleR * fixF[fix_a1]) - (outSample * fixF[fix_b1]) + fixF[fix_sR2];
		fixF[fix_sR2] = (inputSampleR * fixF[fix_a2]) - (outSample * fixF[fix_b2]);
		inputSampleR = outSample; //fixed biquad filtering ultrasonics
		
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
			
			bL[81] = bL[80]; bL[80] = bL[79]; 
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
			
			inputSampleL += (bL[1] * (1.35472031405494242  + (0.00220914099195157*fabs(bL[1]))));
			inputSampleL += (bL[2] * (1.63534207755253003  - (0.11406232654509685*fabs(bL[2]))));
			inputSampleL += (bL[3] * (1.82334575691525869  - (0.42647194712964054*fabs(bL[3]))));
			inputSampleL += (bL[4] * (1.86156386235405868  - (0.76744187887586590*fabs(bL[4]))));
			inputSampleL += (bL[5] * (1.67332739338852599  - (0.95161997324293013*fabs(bL[5]))));
			inputSampleL += (bL[6] * (1.25054130794899021  - (0.98410433514572859*fabs(bL[6]))));
			inputSampleL += (bL[7] * (0.70049121047281737  - (0.87375612110718992*fabs(bL[7]))));
			inputSampleL += (bL[8] * (0.15291791448081560  - (0.61195266024519046*fabs(bL[8]))));
			inputSampleL -= (bL[9] * (0.37301992914152693  + (0.16755422915252094*fabs(bL[9]))));
			inputSampleL -= (bL[10] * (0.76568539228498433  - (0.28554435228965386*fabs(bL[10]))));
			inputSampleL -= (bL[11] * (0.95726568749937369  - (0.61659719162806048*fabs(bL[11]))));
			inputSampleL -= (bL[12] * (1.01273552193911032  - (0.81827288407943954*fabs(bL[12]))));
			inputSampleL -= (bL[13] * (0.93920108117234447  - (0.80077111864205874*fabs(bL[13]))));
			inputSampleL -= (bL[14] * (0.79831898832953974  - (0.65814750339694406*fabs(bL[14]))));
			inputSampleL -= (bL[15] * (0.64200088100452313  - (0.46135833001232618*fabs(bL[15]))));
			inputSampleL -= (bL[16] * (0.48807302802822128  - (0.15506178974799034*fabs(bL[16]))));
			inputSampleL -= (bL[17] * (0.36545171501947982  + (0.16126103769376721*fabs(bL[17]))));
			inputSampleL -= (bL[18] * (0.31469581455759105  + (0.32250870039053953*fabs(bL[18]))));
			inputSampleL -= (bL[19] * (0.36893534817945800  + (0.25409418897237473*fabs(bL[19]))));
			inputSampleL -= (bL[20] * (0.41092557722975687  + (0.13114730488878301*fabs(bL[20]))));
			inputSampleL -= (bL[21] * (0.38584044480710594  + (0.06825323739722661*fabs(bL[21]))));
			inputSampleL -= (bL[22] * (0.33378434007178670  + (0.04144255489164217*fabs(bL[22]))));
			inputSampleL -= (bL[23] * (0.26144203061699706  + (0.06031313105098152*fabs(bL[23]))));
			inputSampleL -= (bL[24] * (0.25818342000920502  + (0.03642289242586355*fabs(bL[24]))));
			inputSampleL -= (bL[25] * (0.28096018498822661  + (0.00976973667327174*fabs(bL[25]))));
			inputSampleL -= (bL[26] * (0.25845682019095384  + (0.02749015358080831*fabs(bL[26]))));
			inputSampleL -= (bL[27] * (0.26655607865953096  - (0.00329839675455690*fabs(bL[27]))));
			inputSampleL -= (bL[28] * (0.30590085026938518  - (0.07375043215328811*fabs(bL[28]))));
			inputSampleL -= (bL[29] * (0.32875683916470899  - (0.12454134857516502*fabs(bL[29]))));
			inputSampleL -= (bL[30] * (0.38166643180506560  - (0.19973911428609989*fabs(bL[30]))));
			inputSampleL -= (bL[31] * (0.49068186937289598  - (0.34785166842136384*fabs(bL[31]))));
			inputSampleL -= (bL[32] * (0.60274753867622777  - (0.48685038872711034*fabs(bL[32]))));
			inputSampleL -= (bL[33] * (0.65944678627090636  - (0.49844657885975518*fabs(bL[33]))));
			inputSampleL -= (bL[34] * (0.64488955808717285  - (0.40514406499806987*fabs(bL[34]))));
			inputSampleL -= (bL[35] * (0.55818730353434354  - (0.28029870614987346*fabs(bL[35]))));
			inputSampleL -= (bL[36] * (0.43110859113387556  - (0.15373504582939335*fabs(bL[36]))));
			inputSampleL -= (bL[37] * (0.37726894966096269  - (0.11570983506028532*fabs(bL[37]))));
			inputSampleL -= (bL[38] * (0.39953242355200935  - (0.17879231130484088*fabs(bL[38]))));
			inputSampleL -= (bL[39] * (0.36726676379100875  - (0.22013553023983223*fabs(bL[39]))));
			inputSampleL -= (bL[40] * (0.27187029469227386  - (0.18461171768478427*fabs(bL[40]))));
			inputSampleL -= (bL[41] * (0.21109334552321635  - (0.14497481318083569*fabs(bL[41]))));
			inputSampleL -= (bL[42] * (0.19808797405293213  - (0.14916579928186940*fabs(bL[42]))));
			inputSampleL -= (bL[43] * (0.16287926785495671  - (0.15146098461120627*fabs(bL[43]))));
			inputSampleL -= (bL[44] * (0.11086621477163359  - (0.13182973443924018*fabs(bL[44]))));
			inputSampleL -= (bL[45] * (0.07531043236890560  - (0.08062172796472888*fabs(bL[45]))));
			inputSampleL -= (bL[46] * (0.01747364473230771  + (0.02201865873632456*fabs(bL[46]))));
			inputSampleL += (bL[47] * (0.03080279125662693  - (0.08721756240972101*fabs(bL[47]))));
			inputSampleL += (bL[48] * (0.02354148659185142  - (0.06376361763053796*fabs(bL[48]))));
			inputSampleL -= (bL[49] * (0.02835772372098715  + (0.00589978513642627*fabs(bL[49]))));
			inputSampleL -= (bL[50] * (0.08983370744565244  - (0.02350960427706536*fabs(bL[50]))));
			inputSampleL -= (bL[51] * (0.14148947620055380  - (0.03329826628693369*fabs(bL[51]))));
			inputSampleL -= (bL[52] * (0.17576502674572581  - (0.06507546651241880*fabs(bL[52]))));
			inputSampleL -= (bL[53] * (0.17168865666573860  - (0.07734801128437317*fabs(bL[53]))));
			inputSampleL -= (bL[54] * (0.14107027738292105  - (0.03136459344220402*fabs(bL[54]))));
			inputSampleL -= (bL[55] * (0.12287163395380074  + (0.01933408169185258*fabs(bL[55]))));
			inputSampleL -= (bL[56] * (0.12276622398112971  + (0.01983508766241737*fabs(bL[56]))));
			inputSampleL -= (bL[57] * (0.12349721440213673  - (0.01111031415304768*fabs(bL[57]))));
			inputSampleL -= (bL[58] * (0.08649454142716655  + (0.02252815645513927*fabs(bL[58]))));
			inputSampleL -= (bL[59] * (0.00953083685474757  + (0.13778878548343007*fabs(bL[59]))));
			inputSampleL += (bL[60] * (0.06045983158868478  - (0.23966318224935096*fabs(bL[60]))));
			inputSampleL += (bL[61] * (0.09053229817093242  - (0.27190119941572544*fabs(bL[61]))));
			inputSampleL += (bL[62] * (0.08112662178843048  - (0.22456862606452327*fabs(bL[62]))));
			inputSampleL += (bL[63] * (0.07503525686243730  - (0.14330154410548213*fabs(bL[63]))));
			inputSampleL += (bL[64] * (0.07372595404399729  - (0.06185193766408734*fabs(bL[64]))));
			inputSampleL += (bL[65] * (0.06073789200080433  + (0.01261857435786178*fabs(bL[65]))));
			inputSampleL += (bL[66] * (0.04616712695742254  + (0.05851771967084609*fabs(bL[66]))));
			inputSampleL += (bL[67] * (0.01036235510345900  + (0.08286534414423796*fabs(bL[67]))));
			inputSampleL -= (bL[68] * (0.03708389413229191  - (0.06695282381039531*fabs(bL[68]))));
			inputSampleL -= (bL[69] * (0.07092204876981217  - (0.01915829199112784*fabs(bL[69]))));
			inputSampleL -= (bL[70] * (0.09443579589460312  + (0.01210082455316246*fabs(bL[70]))));
			inputSampleL -= (bL[71] * (0.07824038577769601  + (0.06121988546065113*fabs(bL[71]))));
			inputSampleL -= (bL[72] * (0.00854730633079399  + (0.14468518752295506*fabs(bL[72]))));
			inputSampleL += (bL[73] * (0.06845589924191028  - (0.18902431382592944*fabs(bL[73]))));
			inputSampleL += (bL[74] * (0.10351569998375465  - (0.13204443060279647*fabs(bL[74]))));
			inputSampleL += (bL[75] * (0.10513368758532179  - (0.02993199294485649*fabs(bL[75]))));
			inputSampleL += (bL[76] * (0.08896978950235003  + (0.04074499273825906*fabs(bL[76]))));
			inputSampleL += (bL[77] * (0.03697537734050980  + (0.09217751130846838*fabs(bL[77]))));
			inputSampleL -= (bL[78] * (0.04014322441280276  - (0.14062297149365666*fabs(bL[78]))));
			inputSampleL -= (bL[79] * (0.10505934581398618  - (0.16988861157275814*fabs(bL[79]))));
			inputSampleL -= (bL[80] * (0.13937661651676272  - (0.15083294570551492*fabs(bL[80]))));
			inputSampleL -= (bL[81] * (0.13183458845108439  - (0.06657454442471208*fabs(bL[81]))));
			
			temp = (inputSampleL + smoothCabBL)/3.0;
			smoothCabBL = inputSampleL;
			inputSampleL = temp/4.0;
			
			randy = ((double(fpdL)/UINT32_MAX)*0.05);
			drySampleL = ((((inputSampleL*(1-randy))+(lastCabSampleL*randy))*wet)+(drySampleL*(1.0-wet)))*outputlevel;
			lastCabSampleL = inputSampleL;
			inputSampleL = drySampleL; //cab L
			
			
			temp = (inputSampleR + smoothCabAR)/3.0;
			smoothCabAR = inputSampleR;
			inputSampleR = temp;
			
			bR[81] = bR[80]; bR[80] = bR[79]; 
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
			
			inputSampleR += (bR[1] * (1.35472031405494242  + (0.00220914099195157*fabs(bR[1]))));
			inputSampleR += (bR[2] * (1.63534207755253003  - (0.11406232654509685*fabs(bR[2]))));
			inputSampleR += (bR[3] * (1.82334575691525869  - (0.42647194712964054*fabs(bR[3]))));
			inputSampleR += (bR[4] * (1.86156386235405868  - (0.76744187887586590*fabs(bR[4]))));
			inputSampleR += (bR[5] * (1.67332739338852599  - (0.95161997324293013*fabs(bR[5]))));
			inputSampleR += (bR[6] * (1.25054130794899021  - (0.98410433514572859*fabs(bR[6]))));
			inputSampleR += (bR[7] * (0.70049121047281737  - (0.87375612110718992*fabs(bR[7]))));
			inputSampleR += (bR[8] * (0.15291791448081560  - (0.61195266024519046*fabs(bR[8]))));
			inputSampleR -= (bR[9] * (0.37301992914152693  + (0.16755422915252094*fabs(bR[9]))));
			inputSampleR -= (bR[10] * (0.76568539228498433  - (0.28554435228965386*fabs(bR[10]))));
			inputSampleR -= (bR[11] * (0.95726568749937369  - (0.61659719162806048*fabs(bR[11]))));
			inputSampleR -= (bR[12] * (1.01273552193911032  - (0.81827288407943954*fabs(bR[12]))));
			inputSampleR -= (bR[13] * (0.93920108117234447  - (0.80077111864205874*fabs(bR[13]))));
			inputSampleR -= (bR[14] * (0.79831898832953974  - (0.65814750339694406*fabs(bR[14]))));
			inputSampleR -= (bR[15] * (0.64200088100452313  - (0.46135833001232618*fabs(bR[15]))));
			inputSampleR -= (bR[16] * (0.48807302802822128  - (0.15506178974799034*fabs(bR[16]))));
			inputSampleR -= (bR[17] * (0.36545171501947982  + (0.16126103769376721*fabs(bR[17]))));
			inputSampleR -= (bR[18] * (0.31469581455759105  + (0.32250870039053953*fabs(bR[18]))));
			inputSampleR -= (bR[19] * (0.36893534817945800  + (0.25409418897237473*fabs(bR[19]))));
			inputSampleR -= (bR[20] * (0.41092557722975687  + (0.13114730488878301*fabs(bR[20]))));
			inputSampleR -= (bR[21] * (0.38584044480710594  + (0.06825323739722661*fabs(bR[21]))));
			inputSampleR -= (bR[22] * (0.33378434007178670  + (0.04144255489164217*fabs(bR[22]))));
			inputSampleR -= (bR[23] * (0.26144203061699706  + (0.06031313105098152*fabs(bR[23]))));
			inputSampleR -= (bR[24] * (0.25818342000920502  + (0.03642289242586355*fabs(bR[24]))));
			inputSampleR -= (bR[25] * (0.28096018498822661  + (0.00976973667327174*fabs(bR[25]))));
			inputSampleR -= (bR[26] * (0.25845682019095384  + (0.02749015358080831*fabs(bR[26]))));
			inputSampleR -= (bR[27] * (0.26655607865953096  - (0.00329839675455690*fabs(bR[27]))));
			inputSampleR -= (bR[28] * (0.30590085026938518  - (0.07375043215328811*fabs(bR[28]))));
			inputSampleR -= (bR[29] * (0.32875683916470899  - (0.12454134857516502*fabs(bR[29]))));
			inputSampleR -= (bR[30] * (0.38166643180506560  - (0.19973911428609989*fabs(bR[30]))));
			inputSampleR -= (bR[31] * (0.49068186937289598  - (0.34785166842136384*fabs(bR[31]))));
			inputSampleR -= (bR[32] * (0.60274753867622777  - (0.48685038872711034*fabs(bR[32]))));
			inputSampleR -= (bR[33] * (0.65944678627090636  - (0.49844657885975518*fabs(bR[33]))));
			inputSampleR -= (bR[34] * (0.64488955808717285  - (0.40514406499806987*fabs(bR[34]))));
			inputSampleR -= (bR[35] * (0.55818730353434354  - (0.28029870614987346*fabs(bR[35]))));
			inputSampleR -= (bR[36] * (0.43110859113387556  - (0.15373504582939335*fabs(bR[36]))));
			inputSampleR -= (bR[37] * (0.37726894966096269  - (0.11570983506028532*fabs(bR[37]))));
			inputSampleR -= (bR[38] * (0.39953242355200935  - (0.17879231130484088*fabs(bR[38]))));
			inputSampleR -= (bR[39] * (0.36726676379100875  - (0.22013553023983223*fabs(bR[39]))));
			inputSampleR -= (bR[40] * (0.27187029469227386  - (0.18461171768478427*fabs(bR[40]))));
			inputSampleR -= (bR[41] * (0.21109334552321635  - (0.14497481318083569*fabs(bR[41]))));
			inputSampleR -= (bR[42] * (0.19808797405293213  - (0.14916579928186940*fabs(bR[42]))));
			inputSampleR -= (bR[43] * (0.16287926785495671  - (0.15146098461120627*fabs(bR[43]))));
			inputSampleR -= (bR[44] * (0.11086621477163359  - (0.13182973443924018*fabs(bR[44]))));
			inputSampleR -= (bR[45] * (0.07531043236890560  - (0.08062172796472888*fabs(bR[45]))));
			inputSampleR -= (bR[46] * (0.01747364473230771  + (0.02201865873632456*fabs(bR[46]))));
			inputSampleR += (bR[47] * (0.03080279125662693  - (0.08721756240972101*fabs(bR[47]))));
			inputSampleR += (bR[48] * (0.02354148659185142  - (0.06376361763053796*fabs(bR[48]))));
			inputSampleR -= (bR[49] * (0.02835772372098715  + (0.00589978513642627*fabs(bR[49]))));
			inputSampleR -= (bR[50] * (0.08983370744565244  - (0.02350960427706536*fabs(bR[50]))));
			inputSampleR -= (bR[51] * (0.14148947620055380  - (0.03329826628693369*fabs(bR[51]))));
			inputSampleR -= (bR[52] * (0.17576502674572581  - (0.06507546651241880*fabs(bR[52]))));
			inputSampleR -= (bR[53] * (0.17168865666573860  - (0.07734801128437317*fabs(bR[53]))));
			inputSampleR -= (bR[54] * (0.14107027738292105  - (0.03136459344220402*fabs(bR[54]))));
			inputSampleR -= (bR[55] * (0.12287163395380074  + (0.01933408169185258*fabs(bR[55]))));
			inputSampleR -= (bR[56] * (0.12276622398112971  + (0.01983508766241737*fabs(bR[56]))));
			inputSampleR -= (bR[57] * (0.12349721440213673  - (0.01111031415304768*fabs(bR[57]))));
			inputSampleR -= (bR[58] * (0.08649454142716655  + (0.02252815645513927*fabs(bR[58]))));
			inputSampleR -= (bR[59] * (0.00953083685474757  + (0.13778878548343007*fabs(bR[59]))));
			inputSampleR += (bR[60] * (0.06045983158868478  - (0.23966318224935096*fabs(bR[60]))));
			inputSampleR += (bR[61] * (0.09053229817093242  - (0.27190119941572544*fabs(bR[61]))));
			inputSampleR += (bR[62] * (0.08112662178843048  - (0.22456862606452327*fabs(bR[62]))));
			inputSampleR += (bR[63] * (0.07503525686243730  - (0.14330154410548213*fabs(bR[63]))));
			inputSampleR += (bR[64] * (0.07372595404399729  - (0.06185193766408734*fabs(bR[64]))));
			inputSampleR += (bR[65] * (0.06073789200080433  + (0.01261857435786178*fabs(bR[65]))));
			inputSampleR += (bR[66] * (0.04616712695742254  + (0.05851771967084609*fabs(bR[66]))));
			inputSampleR += (bR[67] * (0.01036235510345900  + (0.08286534414423796*fabs(bR[67]))));
			inputSampleR -= (bR[68] * (0.03708389413229191  - (0.06695282381039531*fabs(bR[68]))));
			inputSampleR -= (bR[69] * (0.07092204876981217  - (0.01915829199112784*fabs(bR[69]))));
			inputSampleR -= (bR[70] * (0.09443579589460312  + (0.01210082455316246*fabs(bR[70]))));
			inputSampleR -= (bR[71] * (0.07824038577769601  + (0.06121988546065113*fabs(bR[71]))));
			inputSampleR -= (bR[72] * (0.00854730633079399  + (0.14468518752295506*fabs(bR[72]))));
			inputSampleR += (bR[73] * (0.06845589924191028  - (0.18902431382592944*fabs(bR[73]))));
			inputSampleR += (bR[74] * (0.10351569998375465  - (0.13204443060279647*fabs(bR[74]))));
			inputSampleR += (bR[75] * (0.10513368758532179  - (0.02993199294485649*fabs(bR[75]))));
			inputSampleR += (bR[76] * (0.08896978950235003  + (0.04074499273825906*fabs(bR[76]))));
			inputSampleR += (bR[77] * (0.03697537734050980  + (0.09217751130846838*fabs(bR[77]))));
			inputSampleR -= (bR[78] * (0.04014322441280276  - (0.14062297149365666*fabs(bR[78]))));
			inputSampleR -= (bR[79] * (0.10505934581398618  - (0.16988861157275814*fabs(bR[79]))));
			inputSampleR -= (bR[80] * (0.13937661651676272  - (0.15083294570551492*fabs(bR[80]))));
			inputSampleR -= (bR[81] * (0.13183458845108439  - (0.06657454442471208*fabs(bR[81]))));
			
			temp = (inputSampleR + smoothCabBR)/3.0;
			smoothCabBR = inputSampleR;
			inputSampleR = temp/4.0;
			
			
			randy = ((double(fpdR)/UINT32_MAX)*0.05);
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
