/* ========================================
 *  Console8LiteBuss - Console8LiteBuss.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console8LiteBuss_H
#include "Console8LiteBuss.h"
#endif

void Console8LiteBuss::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double iirAmountA = 12.66/getSampleRate();
	//this is our distributed unusual highpass, which is
	//adding subtle harmonics to the really deep stuff to define it
	if (fabs(iirAL)<1.18e-37) iirAL = 0.0;
	if (fabs(iirBL)<1.18e-37) iirBL = 0.0;
	if (fabs(iirAR)<1.18e-37) iirAR = 0.0;
	if (fabs(iirBR)<1.18e-37) iirBR = 0.0;
	//catch denormals early and only check once per buffer
	if (getSampleRate() > 49000.0) hsr = true; else hsr = false;
	fix[fix_freq] = 24000.0 / getSampleRate();
    fix[fix_reso] = 0.55495813;
	double K = tan(M_PI * fix[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fix[fix_reso] + K * K);
	fix[fix_a0] = K * K * norm;
	fix[fix_a1] = 2.0 * fix[fix_a0];
	fix[fix_a2] = fix[fix_a0];
	fix[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fix[fix_b2] = (1.0 - K / fix[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd == 3) cycleEnd = 4;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 4 for 176 or 192k
	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
	inTrimA = inTrimB; inTrimB = A*2.0;
	//0.5 is unity gain, and we can attenuate to silence or boost slightly over 12dB
	//into softclip and ADClip in case we need intense loudness bursts on transients.

	fixB[fix_freq] = 24000.0 / getSampleRate();
	fixB[fix_reso] = 0.5;
	K = tan(M_PI * fixB[fix_freq]); //lowpass
	norm = 1.0 / (1.0 + K / fixB[fix_reso] + K * K);
	fixB[fix_a0] = K * K * norm;
	fixB[fix_a1] = 2.0 * fixB[fix_a0];
	fixB[fix_a2] = fixB[fix_a0];
	fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixB[fix_b2] = (1.0 - K / fixB[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter

    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double position = (double)sampleFrames/inFramesToProcess;
		double inTrim = (inTrimA*position)+(inTrimB*(1.0-position));
		//input trim smoothed to cut out zipper noise
		
		iirAL = (iirAL * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
		double iirAmountBL = fabs(iirAL)+0.00001;
		iirBL = (iirBL * (1.0 - iirAmountBL)) + (iirAL * iirAmountBL);
		inputSampleL -= iirBL;
		iirAR = (iirAR * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
		double iirAmountBR = fabs(iirAR)+0.00001;
		iirBR = (iirBR * (1.0 - iirAmountBR)) + (iirAR * iirAmountBR);
		inputSampleR -= iirBR;
		//Console8 highpass
		
		if (cycleEnd == 4) {
			softL[8] = softL[7]; softL[7] = softL[6]; softL[6] = softL[5]; softL[5] = softL[4];
			softL[4] = softL[3]; softL[3] = softL[2]; softL[2] = softL[1]; softL[1] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[7]; softR[7] = softR[6]; softR[6] = softR[5]; softR[5] = softR[4];
			softR[4] = softR[3]; softR[3] = softR[2]; softR[2] = softR[1]; softR[1] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 2) {
			softL[8] = softL[6]; softL[6] = softL[4];
			softL[4] = softL[2]; softL[2] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[6]; softR[6] = softR[4];
			softR[4] = softR[2]; softR[2] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 1) {
			softL[8] = softL[4];
			softL[4] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[4];
			softR[4] = softR[0];
			softR[0] = inputSampleR;
		}
		softL[9] = ((softL[0]-softL[4])-(softL[4]-softL[8]));
		if (softL[9] > 0.56852180) inputSampleL = softL[4]+(fabs(softL[4])*sin(softL[9]-0.56852180)*0.4314782);
		if (-softL[9] > 0.56852180) inputSampleL = softL[4]-(fabs(softL[4])*sin(-softL[9]-0.56852180)*0.4314782);
		//Console8 slew soften: must be clipped or it can generate NAN out of the full system
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633; if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		softR[9] = ((softR[0]-softR[4])-(softR[4]-softR[8]));
		if (softR[9] > 0.56852180) inputSampleR = softR[4]+(fabs(softR[4])*sin(softR[9]-0.56852180)*0.4314782);
		if (-softR[9] > 0.56852180) inputSampleR = softR[4]-(fabs(softR[4])*sin(-softR[9]-0.56852180)*0.4314782);
		//Console8 slew soften: must be clipped or it can generate NAN out of the full system

		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633; if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		if (hsr){
			double outSample = (inputSampleL * fix[fix_a0]) + fix[fix_sL1];
			fix[fix_sL1] = (inputSampleL * fix[fix_a1]) - (outSample * fix[fix_b1]) + fix[fix_sL2];
			fix[fix_sL2] = (inputSampleL * fix[fix_a2]) - (outSample * fix[fix_b2]);
			inputSampleL = outSample;
			outSample = (inputSampleR * fix[fix_a0]) + fix[fix_sR1];
			fix[fix_sR1] = (inputSampleR * fix[fix_a1]) - (outSample * fix[fix_b1]) + fix[fix_sR2];
			fix[fix_sR2] = (inputSampleR * fix[fix_a2]) - (outSample * fix[fix_b2]);
			inputSampleR = outSample;
		} //fixed biquad filtering ultrasonics		
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL = asin(inputSampleL); //Console8 decode
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR = asin(inputSampleR); //Console8 decode
				
		inputSampleL *= inTrim;
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633; if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		inputSampleR *= inTrim;
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633; if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		if (hsr){
			double outSample = (inputSampleL * fixB[fix_a0]) + fixB[fix_sL1];
			fixB[fix_sL1] = (inputSampleL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
			fixB[fix_sL2] = (inputSampleL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
			inputSampleL = outSample;
			outSample = (inputSampleR * fixB[fix_a0]) + fixB[fix_sR1];
			fixB[fix_sR1] = (inputSampleR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
			fixB[fix_sR2] = (inputSampleR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
			inputSampleR = outSample;
		} //fixed biquad filtering ultrasonics
		inputSampleL *= inTrim; inputSampleR *= inTrim;
		//the final output fader, before ClipOnly2
		
		//begin ClipOnly2 stereo as a little, compressed chunk that can be dropped into code
		if (inputSampleL > 4.0) inputSampleL = 4.0; if (inputSampleL < -4.0) inputSampleL = -4.0;
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=0.7058208+(inputSampleL*0.2609148);
			else lastSampleL = 0.2491717+(lastSampleL*0.7390851);
		} wasPosClipL = false;
		if (inputSampleL>0.9549925859) {wasPosClipL=true;inputSampleL=0.7058208+(lastSampleL*0.2609148);}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=-0.7058208+(inputSampleL*0.2609148);
			else lastSampleL=-0.2491717+(lastSampleL*0.7390851);
		} wasNegClipL = false;
		if (inputSampleL<-0.9549925859) {wasNegClipL=true;inputSampleL=-0.7058208+(lastSampleL*0.2609148);}
		intermediateL[spacing] = inputSampleL;
        inputSampleL = lastSampleL; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateL[x-1] = intermediateL[x];
		lastSampleL = intermediateL[0]; //run a little buffer to handle this
		
		if (inputSampleR > 4.0) inputSampleR = 4.0; if (inputSampleR < -4.0) inputSampleR = -4.0;
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=0.7058208+(inputSampleR*0.2609148);
			else lastSampleR = 0.2491717+(lastSampleR*0.7390851);
		} wasPosClipR = false;
		if (inputSampleR>0.9549925859) {wasPosClipR=true;inputSampleR=0.7058208+(lastSampleR*0.2609148);}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=-0.7058208+(inputSampleR*0.2609148);
			else lastSampleR=-0.2491717+(lastSampleR*0.7390851);
		} wasNegClipR = false;
		if (inputSampleR<-0.9549925859) {wasNegClipR=true;inputSampleR=-0.7058208+(lastSampleR*0.2609148);}
		intermediateR[spacing] = inputSampleR;
        inputSampleR = lastSampleR; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateR[x-1] = intermediateR[x];
		lastSampleR = intermediateR[0]; //run a little buffer to handle this
		//end ClipOnly2 stereo as a little, compressed chunk that can be dropped into code
		
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

void Console8LiteBuss::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double iirAmountA = 12.66/getSampleRate();
	//this is our distributed unusual highpass, which is
	//adding subtle harmonics to the really deep stuff to define it
	if (fabs(iirAL)<1.18e-37) iirAL = 0.0;
	if (fabs(iirBL)<1.18e-37) iirBL = 0.0;
	if (fabs(iirAR)<1.18e-37) iirAR = 0.0;
	if (fabs(iirBR)<1.18e-37) iirBR = 0.0;
	//catch denormals early and only check once per buffer
	if (getSampleRate() > 49000.0) hsr = true; else hsr = false;
	fix[fix_freq] = 24000.0 / getSampleRate();
    fix[fix_reso] = 0.55495813;
	double K = tan(M_PI * fix[fix_freq]); //lowpass
	double norm = 1.0 / (1.0 + K / fix[fix_reso] + K * K);
	fix[fix_a0] = K * K * norm;
	fix[fix_a1] = 2.0 * fix[fix_a0];
	fix[fix_a2] = fix[fix_a0];
	fix[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fix[fix_b2] = (1.0 - K / fix[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	cycleEnd = floor(overallscale);
	if (cycleEnd < 1) cycleEnd = 1;
	if (cycleEnd == 3) cycleEnd = 4;
	if (cycleEnd > 4) cycleEnd = 4;
	//this is going to be 2 for 88.1 or 96k, 4 for 176 or 192k
	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
	inTrimA = inTrimB; inTrimB = A*2.0;
	//0.5 is unity gain, and we can attenuate to silence or boost slightly over 12dB
	//into softclip and ADClip in case we need intense loudness bursts on transients.
	
	fixB[fix_freq] = 24000.0 / getSampleRate();
	fixB[fix_reso] = 0.5;
	K = tan(M_PI * fixB[fix_freq]); //lowpass
	norm = 1.0 / (1.0 + K / fixB[fix_reso] + K * K);
	fixB[fix_a0] = K * K * norm;
	fixB[fix_a1] = 2.0 * fixB[fix_a0];
	fixB[fix_a2] = fixB[fix_a0];
	fixB[fix_b1] = 2.0 * (K * K - 1.0) * norm;
	fixB[fix_b2] = (1.0 - K / fixB[fix_reso] + K * K) * norm;
	//this is the fixed biquad distributed anti-aliasing filter
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double position = (double)sampleFrames/inFramesToProcess;
		double inTrim = (inTrimA*position)+(inTrimB*(1.0-position));
		//input trim smoothed to cut out zipper noise
		
		iirAL = (iirAL * (1.0 - iirAmountA)) + (inputSampleL * iirAmountA);
		double iirAmountBL = fabs(iirAL)+0.00001;
		iirBL = (iirBL * (1.0 - iirAmountBL)) + (iirAL * iirAmountBL);
		inputSampleL -= iirBL;
		iirAR = (iirAR * (1.0 - iirAmountA)) + (inputSampleR * iirAmountA);
		double iirAmountBR = fabs(iirAR)+0.00001;
		iirBR = (iirBR * (1.0 - iirAmountBR)) + (iirAR * iirAmountBR);
		inputSampleR -= iirBR;
		//Console8 highpass
		
		if (cycleEnd == 4) {
			softL[8] = softL[7]; softL[7] = softL[6]; softL[6] = softL[5]; softL[5] = softL[4];
			softL[4] = softL[3]; softL[3] = softL[2]; softL[2] = softL[1]; softL[1] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[7]; softR[7] = softR[6]; softR[6] = softR[5]; softR[5] = softR[4];
			softR[4] = softR[3]; softR[3] = softR[2]; softR[2] = softR[1]; softR[1] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 2) {
			softL[8] = softL[6]; softL[6] = softL[4];
			softL[4] = softL[2]; softL[2] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[6]; softR[6] = softR[4];
			softR[4] = softR[2]; softR[2] = softR[0];
			softR[0] = inputSampleR;
		}
		if (cycleEnd == 1) {
			softL[8] = softL[4];
			softL[4] = softL[0];
			softL[0] = inputSampleL;
			softR[8] = softR[4];
			softR[4] = softR[0];
			softR[0] = inputSampleR;
		}
		softL[9] = ((softL[0]-softL[4])-(softL[4]-softL[8]));
		if (softL[9] > 0.56852180) inputSampleL = softL[4]+(fabs(softL[4])*sin(softL[9]-0.56852180)*0.4314782);
		if (-softL[9] > 0.56852180) inputSampleL = softL[4]-(fabs(softL[4])*sin(-softL[9]-0.56852180)*0.4314782);
		//Console8 slew soften: must be clipped or it can generate NAN out of the full system
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633; if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		softR[9] = ((softR[0]-softR[4])-(softR[4]-softR[8]));
		if (softR[9] > 0.56852180) inputSampleR = softR[4]+(fabs(softR[4])*sin(softR[9]-0.56852180)*0.4314782);
		if (-softR[9] > 0.56852180) inputSampleR = softR[4]-(fabs(softR[4])*sin(-softR[9]-0.56852180)*0.4314782);
		//Console8 slew soften: must be clipped or it can generate NAN out of the full system
		
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633; if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		if (hsr){
			double outSample = (inputSampleL * fix[fix_a0]) + fix[fix_sL1];
			fix[fix_sL1] = (inputSampleL * fix[fix_a1]) - (outSample * fix[fix_b1]) + fix[fix_sL2];
			fix[fix_sL2] = (inputSampleL * fix[fix_a2]) - (outSample * fix[fix_b2]);
			inputSampleL = outSample;
			outSample = (inputSampleR * fix[fix_a0]) + fix[fix_sR1];
			fix[fix_sR1] = (inputSampleR * fix[fix_a1]) - (outSample * fix[fix_b1]) + fix[fix_sR2];
			fix[fix_sR2] = (inputSampleR * fix[fix_a2]) - (outSample * fix[fix_b2]);
			inputSampleR = outSample;
		} //fixed biquad filtering ultrasonics		
		if (inputSampleL > 1.0) inputSampleL = 1.0; if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL = asin(inputSampleL); //Console8 decode
		if (inputSampleR > 1.0) inputSampleR = 1.0; if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR = asin(inputSampleR); //Console8 decode
		
		inputSampleL *= inTrim;
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633; if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		inputSampleR *= inTrim;
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633; if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		if (hsr){
			double outSample = (inputSampleL * fixB[fix_a0]) + fixB[fix_sL1];
			fixB[fix_sL1] = (inputSampleL * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sL2];
			fixB[fix_sL2] = (inputSampleL * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
			inputSampleL = outSample;
			outSample = (inputSampleR * fixB[fix_a0]) + fixB[fix_sR1];
			fixB[fix_sR1] = (inputSampleR * fixB[fix_a1]) - (outSample * fixB[fix_b1]) + fixB[fix_sR2];
			fixB[fix_sR2] = (inputSampleR * fixB[fix_a2]) - (outSample * fixB[fix_b2]);
			inputSampleR = outSample;
		} //fixed biquad filtering ultrasonics
		inputSampleL *= inTrim; inputSampleR *= inTrim;
		//the final output fader, before ClipOnly2
		
		//begin ClipOnly2 stereo as a little, compressed chunk that can be dropped into code
		if (inputSampleL > 4.0) inputSampleL = 4.0; if (inputSampleL < -4.0) inputSampleL = -4.0;
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=0.7058208+(inputSampleL*0.2609148);
			else lastSampleL = 0.2491717+(lastSampleL*0.7390851);
		} wasPosClipL = false;
		if (inputSampleL>0.9549925859) {wasPosClipL=true;inputSampleL=0.7058208+(lastSampleL*0.2609148);}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=-0.7058208+(inputSampleL*0.2609148);
			else lastSampleL=-0.2491717+(lastSampleL*0.7390851);
		} wasNegClipL = false;
		if (inputSampleL<-0.9549925859) {wasNegClipL=true;inputSampleL=-0.7058208+(lastSampleL*0.2609148);}
		intermediateL[spacing] = inputSampleL;
        inputSampleL = lastSampleL; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateL[x-1] = intermediateL[x];
		lastSampleL = intermediateL[0]; //run a little buffer to handle this
		
		if (inputSampleR > 4.0) inputSampleR = 4.0; if (inputSampleR < -4.0) inputSampleR = -4.0;
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=0.7058208+(inputSampleR*0.2609148);
			else lastSampleR = 0.2491717+(lastSampleR*0.7390851);
		} wasPosClipR = false;
		if (inputSampleR>0.9549925859) {wasPosClipR=true;inputSampleR=0.7058208+(lastSampleR*0.2609148);}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=-0.7058208+(inputSampleR*0.2609148);
			else lastSampleR=-0.2491717+(lastSampleR*0.7390851);
		} wasNegClipR = false;
		if (inputSampleR<-0.9549925859) {wasNegClipR=true;inputSampleR=-0.7058208+(lastSampleR*0.2609148);}
		intermediateR[spacing] = inputSampleR;
        inputSampleR = lastSampleR; //Latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) intermediateR[x-1] = intermediateR[x];
		lastSampleR = intermediateR[0]; //run a little buffer to handle this
		//end ClipOnly2 stereo as a little, compressed chunk that can be dropped into code
		
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
