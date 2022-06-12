/* ========================================
 *  Console8BussOut - Console8BussOut.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __Console8BussOut_H
#include "Console8BussOut.h"
#endif

void Console8BussOut::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	inTrimA = inTrimB; inTrimB = A*2.0;
	//0.5 is unity gain, and we can attenuate to silence or boost slightly over 12dB
	//into softclipping overdrive.
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		
		double position = (double)sampleFrames/inFramesToProcess;
		double inTrim = (inTrimA*position)+(inTrimB*(1.0-position));
		//input trim smoothed to cut out zipper noise
		inputSampleL *= inTrim;
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633; if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		inputSampleR *= inTrim;
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633; if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//Console8 gain stage clips at exactly 1.0 post-sin()
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
		inputSampleL *= inTrim; inputSampleR *= inTrim;
		//the final output fader, before ClipOnly2 and dithering
		
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
		
		inputSampleL *= 8388608.0; inputSampleR *= 8388608.0; //0-1 is now one bit, now we dither
		double correctionL = 0;
		double correctionR = 0;
		if (flip) {
			NSOddL = (NSOddL * 0.9999999999) + prevShapeL;
			NSEvenL = (NSEvenL * 0.9999999999) - prevShapeL;
			correctionL = NSOddL;
			NSOddR = (NSOddR * 0.9999999999) + prevShapeR;
			NSEvenR = (NSEvenR * 0.9999999999) - prevShapeR;
			correctionR = NSOddR;
		} else {
			NSOddL = (NSOddL * 0.9999999999) - prevShapeL;
			NSEvenL = (NSEvenL * 0.9999999999) + prevShapeL;
			correctionL = NSEvenL;
			NSOddR = (NSOddR * 0.9999999999) - prevShapeR;
			NSEvenR = (NSEvenR * 0.9999999999) + prevShapeR;
			correctionR = NSEvenR;
		}
		double shapedSampleL = inputSampleL+correctionL;
		int quantAL = floor(shapedSampleL);
		int quantBL = floor(shapedSampleL+1.0);
		double expectedSlewL = 0;
		double shapedSampleR = inputSampleR+correctionR;
		int quantAR = floor(shapedSampleR);
		int quantBR = floor(shapedSampleR+1.0);
		double expectedSlewR = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlewL += (darkSampleL[x+1] - darkSampleL[x]);
			expectedSlewR += (darkSampleR[x+1] - darkSampleR[x]);
		}
		expectedSlewL /= depth; expectedSlewR /= depth; //we have an average of all recent slews
		double testAL = fabs((darkSampleL[0] - quantAL) - expectedSlewL);
		double testBL = fabs((darkSampleL[0] - quantBL) - expectedSlewL);
		if (testAL < testBL) inputSampleL = quantAL; else inputSampleL = quantBL;
		double testAR = fabs((darkSampleR[0] - quantAR) - expectedSlewR);
		double testBR = fabs((darkSampleR[0] - quantBR) - expectedSlewR);
		if (testAR < testBR) inputSampleR = quantAR; else inputSampleR = quantBR;
		for(int x = depth; x >=0; x--) {
			darkSampleL[x+1] = darkSampleL[x];
			darkSampleR[x+1] = darkSampleR[x];
		}
		darkSampleL[0] = inputSampleL; darkSampleR[0] = inputSampleR;
		prevShapeL = (floor(shapedSampleL) - inputSampleL)*0.9999999999;
		prevShapeR = (floor(shapedSampleR) - inputSampleR)*0.9999999999;
		flip = !flip;
		inputSampleL /= 8388608.0; inputSampleR /= 8388608.0; //24 bit Ten Nines into Dark
		
		
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

void Console8BussOut::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	inTrimA = inTrimB; inTrimB = A*2.0;
	//0.5 is unity gain, and we can attenuate to silence or boost slightly over 12dB
	//into softclipping overdrive.
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		
		double position = (double)sampleFrames/inFramesToProcess;
		double inTrim = (inTrimA*position)+(inTrimB*(1.0-position));
		//input trim smoothed to cut out zipper noise
		inputSampleL *= inTrim;
		if (inputSampleL > 1.57079633) inputSampleL = 1.57079633; if (inputSampleL < -1.57079633) inputSampleL = -1.57079633;
		inputSampleL = sin(inputSampleL);
		//Console8 gain stage clips at exactly 1.0 post-sin()
		inputSampleR *= inTrim;
		if (inputSampleR > 1.57079633) inputSampleR = 1.57079633; if (inputSampleR < -1.57079633) inputSampleR = -1.57079633;
		inputSampleR = sin(inputSampleR);
		//Console8 gain stage clips at exactly 1.0 post-sin()
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
		inputSampleL *= inTrim; inputSampleR *= inTrim;
		//the final output fader, before ClipOnly2 and dithering
		
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
		
		inputSampleL *= 8388608.0; inputSampleR *= 8388608.0; //0-1 is now one bit, now we dither
		double correctionL = 0;
		double correctionR = 0;
		if (flip) {
			NSOddL = (NSOddL * 0.9999999999) + prevShapeL;
			NSEvenL = (NSEvenL * 0.9999999999) - prevShapeL;
			correctionL = NSOddL;
			NSOddR = (NSOddR * 0.9999999999) + prevShapeR;
			NSEvenR = (NSEvenR * 0.9999999999) - prevShapeR;
			correctionR = NSOddR;
		} else {
			NSOddL = (NSOddL * 0.9999999999) - prevShapeL;
			NSEvenL = (NSEvenL * 0.9999999999) + prevShapeL;
			correctionL = NSEvenL;
			NSOddR = (NSOddR * 0.9999999999) - prevShapeR;
			NSEvenR = (NSEvenR * 0.9999999999) + prevShapeR;
			correctionR = NSEvenR;
		}
		double shapedSampleL = inputSampleL+correctionL;
		int quantAL = floor(shapedSampleL);
		int quantBL = floor(shapedSampleL+1.0);
		double expectedSlewL = 0;
		double shapedSampleR = inputSampleR+correctionR;
		int quantAR = floor(shapedSampleR);
		int quantBR = floor(shapedSampleR+1.0);
		double expectedSlewR = 0;
		for(int x = 0; x < depth; x++) {
			expectedSlewL += (darkSampleL[x+1] - darkSampleL[x]);
			expectedSlewR += (darkSampleR[x+1] - darkSampleR[x]);
		}
		expectedSlewL /= depth; expectedSlewR /= depth; //we have an average of all recent slews
		double testAL = fabs((darkSampleL[0] - quantAL) - expectedSlewL);
		double testBL = fabs((darkSampleL[0] - quantBL) - expectedSlewL);
		if (testAL < testBL) inputSampleL = quantAL; else inputSampleL = quantBL;
		double testAR = fabs((darkSampleR[0] - quantAR) - expectedSlewR);
		double testBR = fabs((darkSampleR[0] - quantBR) - expectedSlewR);
		if (testAR < testBR) inputSampleR = quantAR; else inputSampleR = quantBR;
		for(int x = depth; x >=0; x--) {
			darkSampleL[x+1] = darkSampleL[x];
			darkSampleR[x+1] = darkSampleR[x];
		}
		darkSampleL[0] = inputSampleL; darkSampleR[0] = inputSampleR;
		prevShapeL = (floor(shapedSampleL) - inputSampleL)*0.9999999999;
		prevShapeR = (floor(shapedSampleR) - inputSampleR)*0.9999999999;
		flip = !flip;
		inputSampleL /= 8388608.0; inputSampleR /= 8388608.0; //24 bit Ten Nines into Dark
		
		
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
