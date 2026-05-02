/* ========================================
 *  ADClip9 - ADClip9.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __ADClip9_H
#include "ADClip9.h"
#endif

void ADClip9::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
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
	
	double inputGain = pow(10.0,(A*18.0)/20.0);
	double match = pow(10.0,(B*18.0)/20.0);
	double noiseLevel = 1.0-pow(1.0-C,2.0);
	double ceiling = 1.0-pow(1.0-D,2.0);
	int mode = (int) (E*2.999)+1;
	    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		switch (mode)
		{
			case 1: inputSampleL *= inputGain; inputSampleR *= inputGain; break; //Boost
			case 2: inputSampleL *= match; inputSampleR *= match; break; //Match
			case 3: inputSampleL *= inputGain; inputSampleR *= inputGain; break; //ClipOnly
		}
		//this is our output mode switch, showing the effects
		
		double overshootL = lastDryL;
		lastDryL = inputSampleL;
		double overshootR = lastDryR;
		lastDryR = inputSampleR;
		
		//begin ClipOnly3 as a little, compressed chunk that can be dropped into code
		double noise = 1.0-((double(fpdL)/UINT32_MAX)*(1.0-noiseLevel));//0.076
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=(noiseLevel*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = ceiling;
		} wasPosClipL = false;
		if (inputSampleL>noiseLevel) {wasPosClipL=true;inputSampleL=(noiseLevel*noise)+(lastSampleL*(1.0-noise));}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=(-noiseLevel*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = -ceiling;
		} wasNegClipL = false;
		if (inputSampleL<-noiseLevel) {wasNegClipL=true;inputSampleL=(-noiseLevel*noise)+(lastSampleL*(1.0-noise));}
		slewL[spacing*2] = fabs(lastSampleL-inputSampleL);
		for (int x = spacing*2; x > 0; x--) slewL[x-1] = slewL[x];
		intermediateL[spacing] = inputSampleL; inputSampleL = lastSampleL;
		//latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateL[x-1] = intermediateL[x];} lastSampleL = intermediateL[0];
		if (wasPosClipL || wasNegClipL) {
			for (int x = spacing; x > 0; x--) lastSampleL += intermediateL[x];
			lastSampleL /= spacing;
		} double finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewL[x]) finalSlew = slewL[x];
		double postclip = ceiling / (1.0+(finalSlew*1.3986013));
		if (inputSampleL > postclip) inputSampleL = postclip; if (inputSampleL < -postclip) inputSampleL = -postclip;

		noise = 1.0-((double(fpdR)/UINT32_MAX)*(1.0-noiseLevel));//0.076
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=(noiseLevel*noise)+(inputSampleR*(1.0-noise));
			else lastSampleR = ceiling;
		} wasPosClipR = false;
		if (inputSampleR>noiseLevel) {wasPosClipR=true;inputSampleR=(noiseLevel*noise)+(lastSampleR*(1.0-noise));}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=(-noiseLevel*noise)+(inputSampleR*(1.0-noise));
			else lastSampleR = -ceiling;
		} wasNegClipR = false;
		if (inputSampleR<-noiseLevel) {wasNegClipR=true;inputSampleR=(-noiseLevel*noise)+(lastSampleR*(1.0-noise));}
		slewR[spacing*2] = fabs(lastSampleR-inputSampleR);
		for (int x = spacing*2; x > 0; x--) slewR[x-1] = slewR[x];
		intermediateR[spacing] = inputSampleR; inputSampleR = lastSampleR;
		//latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateR[x-1] = intermediateR[x];} lastSampleR = intermediateR[0];
		if (wasPosClipR || wasNegClipR) {
			for (int x = spacing; x > 0; x--) lastSampleR += intermediateR[x];
			lastSampleR /= spacing;
		} finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewR[x]) finalSlew = slewR[x];
		postclip = ceiling / (1.0+(finalSlew*1.3986013));
		if (inputSampleR > postclip) inputSampleR = postclip; if (inputSampleR < -postclip) inputSampleR = -postclip;
		//end ClipOnly3 as a little, compressed chunk that can be dropped into code
		
		switch (mode)
		{
			case 1: break; //Boost
			case 2: inputSampleL /= match; inputSampleR /= match; break; //Match
			case 3: inputSampleL = (inputSampleL-overshootL)/inputGain; inputSampleR = (inputSampleR-overshootR)/inputGain; break; //Clip Only
		}
		//this is our output mode switch, showing the effects
		
		//begin 32 bit stereo floating point dither
		//int expon; frexpf((float)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//frexpf((float)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 32 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void ADClip9::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
	
	double inputGain = pow(10.0,(A*18.0)/20.0);
	double match = pow(10.0,(B*18.0)/20.0);
	double noiseLevel = 1.0-pow(1.0-C,2.0);
	double ceiling = 1.0-pow(1.0-D,2.0);
	int mode = (int) (E*2.999)+1;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		switch (mode)
		{
			case 1: inputSampleL *= inputGain; inputSampleR *= inputGain; break; //Boost
			case 2: inputSampleL *= match; inputSampleR *= match; break; //Match
			case 3: inputSampleL *= inputGain; inputSampleR *= inputGain; break; //ClipOnly
		}
		//this is our output mode switch, showing the effects
		
		double overshootL = lastDryL;
		lastDryL = inputSampleL;
		double overshootR = lastDryR;
		lastDryR = inputSampleR;
		
		//begin ClipOnly3 as a little, compressed chunk that can be dropped into code
		double noise = 1.0-((double(fpdL)/UINT32_MAX)*(1.0-noiseLevel));//0.076
		if (wasPosClipL == true) { //current will be over
			if (inputSampleL<lastSampleL) lastSampleL=(noiseLevel*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = ceiling;
		} wasPosClipL = false;
		if (inputSampleL>noiseLevel) {wasPosClipL=true;inputSampleL=(noiseLevel*noise)+(lastSampleL*(1.0-noise));}
		if (wasNegClipL == true) { //current will be -over
			if (inputSampleL > lastSampleL) lastSampleL=(-noiseLevel*noise)+(inputSampleL*(1.0-noise));
			else lastSampleL = -ceiling;
		} wasNegClipL = false;
		if (inputSampleL<-noiseLevel) {wasNegClipL=true;inputSampleL=(-noiseLevel*noise)+(lastSampleL*(1.0-noise));}
		slewL[spacing*2] = fabs(lastSampleL-inputSampleL);
		for (int x = spacing*2; x > 0; x--) slewL[x-1] = slewL[x];
		intermediateL[spacing] = inputSampleL; inputSampleL = lastSampleL;
		//latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateL[x-1] = intermediateL[x];} lastSampleL = intermediateL[0];
		if (wasPosClipL || wasNegClipL) {
			for (int x = spacing; x > 0; x--) lastSampleL += intermediateL[x];
			lastSampleL /= spacing;
		} double finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewL[x]) finalSlew = slewL[x];
		double postclip = ceiling / (1.0+(finalSlew*1.3986013));
		if (inputSampleL > postclip) inputSampleL = postclip; if (inputSampleL < -postclip) inputSampleL = -postclip;
		
		noise = 1.0-((double(fpdR)/UINT32_MAX)*(1.0-noiseLevel));//0.076
		if (wasPosClipR == true) { //current will be over
			if (inputSampleR<lastSampleR) lastSampleR=(noiseLevel*noise)+(inputSampleR*(1.0-noise));
			else lastSampleR = ceiling;
		} wasPosClipR = false;
		if (inputSampleR>noiseLevel) {wasPosClipR=true;inputSampleR=(noiseLevel*noise)+(lastSampleR*(1.0-noise));}
		if (wasNegClipR == true) { //current will be -over
			if (inputSampleR > lastSampleR) lastSampleR=(-noiseLevel*noise)+(inputSampleR*(1.0-noise));
			else lastSampleR = -ceiling;
		} wasNegClipR = false;
		if (inputSampleR<-noiseLevel) {wasNegClipR=true;inputSampleR=(-noiseLevel*noise)+(lastSampleR*(1.0-noise));}
		slewR[spacing*2] = fabs(lastSampleR-inputSampleR);
		for (int x = spacing*2; x > 0; x--) slewR[x-1] = slewR[x];
		intermediateR[spacing] = inputSampleR; inputSampleR = lastSampleR;
		//latency is however many samples equals one 44.1k sample
		for (int x = spacing; x > 0; x--) {intermediateR[x-1] = intermediateR[x];} lastSampleR = intermediateR[0];
		if (wasPosClipR || wasNegClipR) {
			for (int x = spacing; x > 0; x--) lastSampleR += intermediateR[x];
			lastSampleR /= spacing;
		} finalSlew = 0.0;
		for (int x = spacing*2; x >= 0; x--) if (finalSlew < slewR[x]) finalSlew = slewR[x];
		postclip = ceiling / (1.0+(finalSlew*1.3986013));
		if (inputSampleR > postclip) inputSampleR = postclip; if (inputSampleR < -postclip) inputSampleR = -postclip;
		//end ClipOnly3 as a little, compressed chunk that can be dropped into code
		
		switch (mode)
		{
			case 1: break; //Boost
			case 2: inputSampleL /= match; inputSampleR /= match; break; //Match
			case 3: inputSampleL = (inputSampleL-overshootL)/inputGain; inputSampleR = (inputSampleR-overshootR)/inputGain; break; //Clip Only
		}
		//this is our output mode switch, showing the effects
		
		//begin 64 bit stereo floating point dither
		//int expon; frexp((double)inputSampleL, &expon);
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		//inputSampleL += ((double(fpdL)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//frexp((double)inputSampleR, &expon);
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		if (fpdL-fpdR < 1073741824 || fpdR-fpdL < 1073741824) {
			fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;}
		//inputSampleR += ((double(fpdR)-uint32_t(0x7fffffff)) * 3.553e-44l * pow(2,expon+62));
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
