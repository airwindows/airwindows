/* ========================================
 *  Console4Buss - Console4Buss.h
 *  Created 8/12/11 by SPIAdmin 
 *  Copyright (c) 2011 __MyCompanyName__, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Console4Buss_H
#include "Console4Buss.h"
#endif

void Console4Buss::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double inputSampleL;
	double inputSampleR;
	double half;
	double falf;
	double slewcompensation;
	if (settingchase != gain) {
		chasespeed *= 2.0;
		settingchase = gain;
	}
	if (chasespeed > 2500.0) chasespeed = 2500.0;
	if (gainchase < 0.0) gainchase = gain;
	
	
    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		chasespeed *= 0.9999;
		chasespeed -= 0.01;
		if (chasespeed < 350.0) chasespeed = 350.0;
		//we have our chase speed compensated for recent fader activity
		
		gainchase = (((gainchase*chasespeed)+gain)/(chasespeed+1.0));
		//gainchase is chasing the target, as a simple multiply gain factor
		
		if (1.0 != gainchase) {inputSampleL *= gainchase; inputSampleR *= gainchase;}
		//done with trim control
		
		half = inputSampleL;
		falf = fabs(half);
		half *= falf;
		half *= falf;
		slewcompensation = fabs(inputSampleL - lastSampleL) * overallscale;
		//magnify effect at high sample rate so it will still register when inter-sample changes
		//are very small at high rates.
		if (slewcompensation > 1.0) slewcompensation = 1.0;
		//let's not invert the effect: maximum application is to cancel out half entirely
		half *= (1.0 - slewcompensation);
		//apply it
		lastSampleL = inputSampleL;
		inputSampleL += half;
		//this is the inverse processing for Console: boosts but not so much if there's slew.
		//is this too subtle an effect?

		
		half = inputSampleR;
		falf = fabs(half);
		half *= falf;
		half *= falf;
		slewcompensation = fabs(inputSampleR - lastSampleR) * overallscale;
		//magnify effect at high sample rate so it will still register when inter-sample changes
		//are very small at high rates.
		if (slewcompensation > 1.0) slewcompensation = 1.0;
		//let's not invert the effect: maximum application is to cancel out half entirely
		half *= (1.0 - slewcompensation);
		//apply it
		lastSampleR = inputSampleR;
		inputSampleR += half;
		//this is the inverse processing for Console: boosts but not so much if there's slew.
		//is this too subtle an effect?
		
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Console4Buss::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double inputSampleL;
	double inputSampleR;    
	double half;
	double falf;
	double slewcompensation;
	if (settingchase != gain) {
		chasespeed *= 2.0;
		settingchase = gain;
	}
	if (chasespeed > 2500.0) chasespeed = 2500.0;
	if (gainchase < 0.0) gainchase = gain;


    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		chasespeed *= 0.9999;
		chasespeed -= 0.01;
		if (chasespeed < 350.0) chasespeed = 350.0;
		//we have our chase speed compensated for recent fader activity
		
		gainchase = (((gainchase*chasespeed)+gain)/(chasespeed+1.0));
		//gainchase is chasing the target, as a simple multiply gain factor
		
		if (1.0 != gainchase) {inputSampleL *= gainchase; inputSampleR *= gainchase;}
		//done with trim control
		
		half = inputSampleL;
		falf = fabs(half);
		half *= falf;
		half *= falf;
		slewcompensation = fabs(inputSampleL - lastSampleL) * overallscale;
		//magnify effect at high sample rate so it will still register when inter-sample changes
		//are very small at high rates.
		if (slewcompensation > 1.0) slewcompensation = 1.0;
		//let's not invert the effect: maximum application is to cancel out half entirely
		half *= (1.0 - slewcompensation);
		//apply it
		lastSampleL = inputSampleL;
		inputSampleL += half;
		//this is the inverse processing for Console: boosts but not so much if there's slew.
		//is this too subtle an effect?
		
		
		half = inputSampleR;
		falf = fabs(half);
		half *= falf;
		half *= falf;
		slewcompensation = fabs(inputSampleR - lastSampleR) * overallscale;
		//magnify effect at high sample rate so it will still register when inter-sample changes
		//are very small at high rates.
		if (slewcompensation > 1.0) slewcompensation = 1.0;
		//let's not invert the effect: maximum application is to cancel out half entirely
		half *= (1.0 - slewcompensation);
		//apply it
		lastSampleR = inputSampleR;
		inputSampleR += half;
		//this is the inverse processing for Console: boosts but not so much if there's slew.
		//is this too subtle an effect?

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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}