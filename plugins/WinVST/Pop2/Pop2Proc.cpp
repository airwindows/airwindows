/* ========================================
 *  Pop2 - Pop2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Pop2_H
#include "Pop2.h"
#endif

void Pop2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
	double threshold = 1.0 - ((1.0-pow(1.0-A,2))*0.9);
	double attack = ((pow(B,4)*100000.0)+10.0)*overallscale;
	double release = ((pow(C,5)*2000000.0)+20.0)*overallscale;
	double maxRelease = release * 4.0;
	double muPreGain = 1.0/threshold;
	double muMakeupGain = sqrt(1.0 / threshold)*D;
	double wet = E;
	//compressor section
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin compressor section
		inputSampleL *= muPreGain;
		inputSampleR *= muPreGain;
		//adjust coefficients for L
		if (flip) {
			if (fabs(inputSampleL) > threshold) {
				muVaryL = threshold / fabs(inputSampleL);
				muAttackL = sqrt(fabs(muSpeedAL));
				muCoefficientAL = muCoefficientAL * (muAttackL-1.0);
				if (muVaryL < threshold) muCoefficientAL = muCoefficientAL + threshold;
				else muCoefficientAL = muCoefficientAL + muVaryL;
				muCoefficientAL = muCoefficientAL / muAttackL;
				muNewSpeedL = muSpeedAL * (muSpeedAL-1.0);
				muNewSpeedL = muNewSpeedL + release;
				muSpeedAL = muNewSpeedL / muSpeedAL;
				if (muSpeedAL > maxRelease) muSpeedAL = maxRelease;
			} else {
				muCoefficientAL = muCoefficientAL * ((muSpeedAL * muSpeedAL)-1.0);
				muCoefficientAL = muCoefficientAL + 1.0;
				muCoefficientAL = muCoefficientAL / (muSpeedAL * muSpeedAL);
				muNewSpeedL = muSpeedAL * (muSpeedAL-1.0);
				muNewSpeedL = muNewSpeedL + attack;
				muSpeedAL = muNewSpeedL / muSpeedAL;}
		} else {
			if (fabs(inputSampleL) > threshold) {
				muVaryL = threshold / fabs(inputSampleL);
				muAttackL = sqrt(fabs(muSpeedBL));
				muCoefficientBL = muCoefficientBL * (muAttackL-1);
				if (muVaryL < threshold) muCoefficientBL = muCoefficientBL + threshold;
				else muCoefficientBL = muCoefficientBL + muVaryL;
				muCoefficientBL = muCoefficientBL / muAttackL;
				muNewSpeedL = muSpeedBL * (muSpeedBL-1.0);
				muNewSpeedL = muNewSpeedL + release;
				muSpeedBL = muNewSpeedL / muSpeedBL;
				if (muSpeedBL > maxRelease) muSpeedBL = maxRelease;
			} else {
				muCoefficientBL = muCoefficientBL * ((muSpeedBL * muSpeedBL)-1.0);
				muCoefficientBL = muCoefficientBL + 1.0;
				muCoefficientBL = muCoefficientBL / (muSpeedBL * muSpeedBL);
				muNewSpeedL = muSpeedBL * (muSpeedBL-1.0);
				muNewSpeedL = muNewSpeedL + attack;
				muSpeedBL = muNewSpeedL / muSpeedBL;
			}
		}
		//got coefficients, adjusted speeds for L
		
		//adjust coefficients for R
		if (flip) {
			if (fabs(inputSampleR) > threshold) {
				muVaryR = threshold / fabs(inputSampleR);
				muAttackR = sqrt(fabs(muSpeedAR));
				muCoefficientAR = muCoefficientAR * (muAttackR-1.0);
				if (muVaryR < threshold) muCoefficientAR = muCoefficientAR + threshold;
				else muCoefficientAR = muCoefficientAR + muVaryR;
				muCoefficientAR = muCoefficientAR / muAttackR;
				muNewSpeedR = muSpeedAR * (muSpeedAR-1.0);
				muNewSpeedR = muNewSpeedR + release;
				muSpeedAR = muNewSpeedR / muSpeedAR;
				if (muSpeedAR > maxRelease) muSpeedAR = maxRelease;
			} else {
				muCoefficientAR = muCoefficientAR * ((muSpeedAR * muSpeedAR)-1.0);
				muCoefficientAR = muCoefficientAR + 1.0;
				muCoefficientAR = muCoefficientAR / (muSpeedAR * muSpeedAR);
				muNewSpeedR = muSpeedAR * (muSpeedAR-1.0);
				muNewSpeedR = muNewSpeedR + attack;
				muSpeedAR = muNewSpeedR / muSpeedAR;
			}
		} else {
			if (fabs(inputSampleR) > threshold) {
				muVaryR = threshold / fabs(inputSampleR);
				muAttackR = sqrt(fabs(muSpeedBR));
				muCoefficientBR = muCoefficientBR * (muAttackR-1);
				if (muVaryR < threshold) muCoefficientBR = muCoefficientBR + threshold;
				else muCoefficientBR = muCoefficientBR + muVaryR;
				muCoefficientBR = muCoefficientBR / muAttackR;
				muNewSpeedR = muSpeedBR * (muSpeedBR-1.0);
				muNewSpeedR = muNewSpeedR + release;
				muSpeedBR = muNewSpeedR / muSpeedBR;
				if (muSpeedBR > maxRelease) muSpeedBR = maxRelease;
			} else {
				muCoefficientBR = muCoefficientBR * ((muSpeedBR * muSpeedBR)-1.0);
				muCoefficientBR = muCoefficientBR + 1.0;
				muCoefficientBR = muCoefficientBR / (muSpeedBR * muSpeedBR);
				muNewSpeedR = muSpeedBR * (muSpeedBR-1.0);
				muNewSpeedR = muNewSpeedR + attack;
				muSpeedBR = muNewSpeedR / muSpeedBR;
			}
		}
		//got coefficients, adjusted speeds for R
		
		if (flip) {
			inputSampleL *= pow(muCoefficientAL,2);
			inputSampleR *= pow(muCoefficientAR,2);
		} else {
			inputSampleL *= pow(muCoefficientBL,2);
			inputSampleR *= pow(muCoefficientBR,2);
		}
		inputSampleL *= muMakeupGain;
		inputSampleR *= muMakeupGain;
		flip = !flip;
		//end compressor section
		
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
		
		if (wet<1.0) {
			inputSampleL = (drySampleL*(1.0-wet))+(inputSampleL*wet);
			inputSampleR = (drySampleR*(1.0-wet))+(inputSampleR*wet);
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

void Pop2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int spacing = floor(overallscale); //should give us working basic scaling, usually 2 or 4
	if (spacing < 1) spacing = 1; if (spacing > 16) spacing = 16;
	
	double threshold = 1.0 - ((1.0-pow(1.0-A,2))*0.9);
	double attack = ((pow(B,4)*100000.0)+10.0)*overallscale;
	double release = ((pow(C,5)*2000000.0)+20.0)*overallscale;
	double maxRelease = release * 4.0;
	double muPreGain = 1.0/threshold;
	double muMakeupGain = sqrt(1.0 / threshold)*D;
	double wet = E;
	//compressor section
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		//begin compressor section
		inputSampleL *= muPreGain;
		inputSampleR *= muPreGain;
		//adjust coefficients for L
		if (flip) {
			if (fabs(inputSampleL) > threshold) {
				muVaryL = threshold / fabs(inputSampleL);
				muAttackL = sqrt(fabs(muSpeedAL));
				muCoefficientAL = muCoefficientAL * (muAttackL-1.0);
				if (muVaryL < threshold) muCoefficientAL = muCoefficientAL + threshold;
				else muCoefficientAL = muCoefficientAL + muVaryL;
				muCoefficientAL = muCoefficientAL / muAttackL;
				muNewSpeedL = muSpeedAL * (muSpeedAL-1.0);
				muNewSpeedL = muNewSpeedL + release;
				muSpeedAL = muNewSpeedL / muSpeedAL;
				if (muSpeedAL > maxRelease) muSpeedAL = maxRelease;
			} else {
				muCoefficientAL = muCoefficientAL * ((muSpeedAL * muSpeedAL)-1.0);
				muCoefficientAL = muCoefficientAL + 1.0;
				muCoefficientAL = muCoefficientAL / (muSpeedAL * muSpeedAL);
				muNewSpeedL = muSpeedAL * (muSpeedAL-1.0);
				muNewSpeedL = muNewSpeedL + attack;
				muSpeedAL = muNewSpeedL / muSpeedAL;}
		} else {
			if (fabs(inputSampleL) > threshold) {
				muVaryL = threshold / fabs(inputSampleL);
				muAttackL = sqrt(fabs(muSpeedBL));
				muCoefficientBL = muCoefficientBL * (muAttackL-1);
				if (muVaryL < threshold) muCoefficientBL = muCoefficientBL + threshold;
				else muCoefficientBL = muCoefficientBL + muVaryL;
				muCoefficientBL = muCoefficientBL / muAttackL;
				muNewSpeedL = muSpeedBL * (muSpeedBL-1.0);
				muNewSpeedL = muNewSpeedL + release;
				muSpeedBL = muNewSpeedL / muSpeedBL;
				if (muSpeedBL > maxRelease) muSpeedBL = maxRelease;
			} else {
				muCoefficientBL = muCoefficientBL * ((muSpeedBL * muSpeedBL)-1.0);
				muCoefficientBL = muCoefficientBL + 1.0;
				muCoefficientBL = muCoefficientBL / (muSpeedBL * muSpeedBL);
				muNewSpeedL = muSpeedBL * (muSpeedBL-1.0);
				muNewSpeedL = muNewSpeedL + attack;
				muSpeedBL = muNewSpeedL / muSpeedBL;
			}
		}
		//got coefficients, adjusted speeds for L
		
		//adjust coefficients for R
		if (flip) {
			if (fabs(inputSampleR) > threshold) {
				muVaryR = threshold / fabs(inputSampleR);
				muAttackR = sqrt(fabs(muSpeedAR));
				muCoefficientAR = muCoefficientAR * (muAttackR-1.0);
				if (muVaryR < threshold) muCoefficientAR = muCoefficientAR + threshold;
				else muCoefficientAR = muCoefficientAR + muVaryR;
				muCoefficientAR = muCoefficientAR / muAttackR;
				muNewSpeedR = muSpeedAR * (muSpeedAR-1.0);
				muNewSpeedR = muNewSpeedR + release;
				muSpeedAR = muNewSpeedR / muSpeedAR;
				if (muSpeedAR > maxRelease) muSpeedAR = maxRelease;
			} else {
				muCoefficientAR = muCoefficientAR * ((muSpeedAR * muSpeedAR)-1.0);
				muCoefficientAR = muCoefficientAR + 1.0;
				muCoefficientAR = muCoefficientAR / (muSpeedAR * muSpeedAR);
				muNewSpeedR = muSpeedAR * (muSpeedAR-1.0);
				muNewSpeedR = muNewSpeedR + attack;
				muSpeedAR = muNewSpeedR / muSpeedAR;
			}
		} else {
			if (fabs(inputSampleR) > threshold) {
				muVaryR = threshold / fabs(inputSampleR);
				muAttackR = sqrt(fabs(muSpeedBR));
				muCoefficientBR = muCoefficientBR * (muAttackR-1);
				if (muVaryR < threshold) muCoefficientBR = muCoefficientBR + threshold;
				else muCoefficientBR = muCoefficientBR + muVaryR;
				muCoefficientBR = muCoefficientBR / muAttackR;
				muNewSpeedR = muSpeedBR * (muSpeedBR-1.0);
				muNewSpeedR = muNewSpeedR + release;
				muSpeedBR = muNewSpeedR / muSpeedBR;
				if (muSpeedBR > maxRelease) muSpeedBR = maxRelease;
			} else {
				muCoefficientBR = muCoefficientBR * ((muSpeedBR * muSpeedBR)-1.0);
				muCoefficientBR = muCoefficientBR + 1.0;
				muCoefficientBR = muCoefficientBR / (muSpeedBR * muSpeedBR);
				muNewSpeedR = muSpeedBR * (muSpeedBR-1.0);
				muNewSpeedR = muNewSpeedR + attack;
				muSpeedBR = muNewSpeedR / muSpeedBR;
			}
		}
		//got coefficients, adjusted speeds for R
		
		if (flip) {
			inputSampleL *= pow(muCoefficientAL,2);
			inputSampleR *= pow(muCoefficientAR,2);
		} else {
			inputSampleL *= pow(muCoefficientBL,2);
			inputSampleR *= pow(muCoefficientBR,2);
		}
		inputSampleL *= muMakeupGain;
		inputSampleR *= muMakeupGain;
		flip = !flip;
		//end compressor section
		
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
		
		if (wet<1.0) {
			inputSampleL = (drySampleL*(1.0-wet))+(inputSampleL*wet);
			inputSampleR = (drySampleR*(1.0-wet))+(inputSampleR*wet);
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
