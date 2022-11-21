/* ========================================
 *  Lowpass2 - Lowpass2.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Lowpass2_H
#include "Lowpass2.h"
#endif

void Lowpass2::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double iirAmount = A;
	double tight = (B*2.0)-1.0;
	if (iirAmount < 0.0000001) iirAmount = 0.0000001;
	if (iirAmount > 1) iirAmount = 1;
	if (tight < 0.0) tight *= 0.5;
	double offsetL;
	double offsetR;
	double aWet = 1.0;
	double bWet = 1.0;
	double cWet = 1.0;
	double dWet = C*4.0;
	//four-stage wet/dry control using progressive stages that bypass when not engaged
	if (dWet < 1.0) {aWet = dWet; bWet = 0.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 2.0) {bWet = dWet - 1.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 3.0) {cWet = dWet - 2.0; dWet = 0.0;}
	else {dWet -= 3.0;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	double wet = D;
	
	while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (tight > 0) offsetL = (1 - tight) + (fabs(inputSampleL)*tight);
		else offsetL = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offsetL < 0.0000001) offsetL = 0.0000001; if (offsetL > 1) offsetL = 1;

		if (tight > 0) offsetR = (1 - tight) + (fabs(inputSampleR)*tight);
		else offsetR = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offsetR < 0.0000001) offsetR = 0.0000001; if (offsetR > 1) offsetR = 1;
		
		if (fpFlip)
		{
			if (aWet > 0.0) {
				iirSampleAL = (iirSampleAL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleAL * aWet) + (inputSampleL * (1.0-aWet));

				iirSampleAR = (iirSampleAR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleAR * aWet) + (inputSampleR * (1.0-aWet));
			}
			if (bWet > 0.0) {
				iirSampleCL = (iirSampleCL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleCL * bWet) + (inputSampleL * (1.0-bWet));

				iirSampleCR = (iirSampleCR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleCR * bWet) + (inputSampleR * (1.0-bWet));
			}
			if (cWet > 0.0) {
				iirSampleEL = (iirSampleEL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleEL * cWet) + (inputSampleL * (1.0-cWet));

				iirSampleER = (iirSampleER * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleER * cWet) + (inputSampleR * (1.0-cWet));
			}
			if (dWet > 0.0) {
				iirSampleGL = (iirSampleGL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleGL * dWet) + (inputSampleL * (1.0-dWet));

				iirSampleGR = (iirSampleGR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleGR * dWet) + (inputSampleR * (1.0-dWet));
			}
		}
		else
		{
			if (aWet > 0.0) {
				iirSampleBL = (iirSampleBL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleBL * aWet) + (inputSampleL * (1.0-aWet));

				iirSampleBR = (iirSampleBR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleBR * aWet) + (inputSampleR * (1.0-aWet));
			}
			if (bWet > 0.0) {
				iirSampleDL = (iirSampleDL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleDL * bWet) + (inputSampleL * (1.0-bWet));

				iirSampleDR = (iirSampleDR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleDR * bWet) + (inputSampleR * (1.0-bWet));
			}
			if (cWet > 0.0) {
				iirSampleFL = (iirSampleFL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleFL * cWet) + (inputSampleL * (1.0-cWet));

				iirSampleFR = (iirSampleFR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleFR * cWet) + (inputSampleR * (1.0-cWet));
			}
			if (dWet > 0.0) {
				iirSampleHL = (iirSampleHL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleHL * dWet) + (inputSampleL * (1.0-dWet));

				iirSampleHR = (iirSampleHR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleHR * dWet) + (inputSampleR * (1.0-dWet));
			}
		}
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		fpFlip = !fpFlip;
		
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

void Lowpass2::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double iirAmount = A;
	double tight = (B*2.0)-1.0;
	iirAmount += fabs(tight);
	if (iirAmount < 0.0000001) iirAmount = 0.0000001;
	if (iirAmount > 1) iirAmount = 1;
	if (tight < 0.0) tight *= 0.5;
	double offsetL;
	double offsetR;
	double aWet = 1.0;
	double bWet = 1.0;
	double cWet = 1.0;
	double dWet = C*4.0;
	//four-stage wet/dry control using progressive stages that bypass when not engaged
	if (dWet < 1.0) {aWet = dWet; bWet = 0.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 2.0) {bWet = dWet - 1.0; cWet = 0.0; dWet = 0.0;}
	else if (dWet < 3.0) {cWet = dWet - 2.0; dWet = 0.0;}
	else {dWet -= 3.0;}
	//this is one way to make a little set of dry/wet stages that are successively added to the
	//output as the control is turned up. Each one independently goes from 0-1 and stays at 1
	//beyond that point: this is a way to progressively add a 'black box' sound processing
	//which lets you fall through to simpler processing at lower settings.
	double wet = D;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (tight > 0) offsetL = (1 - tight) + (fabs(inputSampleL)*tight);
		else offsetL = (1 + tight) + ((1-fabs(inputSampleL))*tight);
		if (offsetL < 0.0000001) offsetL = 0.0000001; if (offsetL > 1) offsetL = 1;
		
		if (tight > 0) offsetR = (1 - tight) + (fabs(inputSampleR)*tight);
		else offsetR = (1 + tight) + ((1-fabs(inputSampleR))*tight);
		if (offsetR < 0.0000001) offsetR = 0.0000001; if (offsetR > 1) offsetR = 1;
		
		if (fpFlip)
		{
			if (aWet > 0.0) {
				iirSampleAL = (iirSampleAL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleAL * aWet) + (inputSampleL * (1.0-aWet));
				
				iirSampleAR = (iirSampleAR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleAR * aWet) + (inputSampleR * (1.0-aWet));
			}
			if (bWet > 0.0) {
				iirSampleCL = (iirSampleCL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleCL * bWet) + (inputSampleL * (1.0-bWet));
				
				iirSampleCR = (iirSampleCR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleCR * bWet) + (inputSampleR * (1.0-bWet));
			}
			if (cWet > 0.0) {
				iirSampleEL = (iirSampleEL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleEL * cWet) + (inputSampleL * (1.0-cWet));
				
				iirSampleER = (iirSampleER * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleER * cWet) + (inputSampleR * (1.0-cWet));
			}
			if (dWet > 0.0) {
				iirSampleGL = (iirSampleGL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleGL * dWet) + (inputSampleL * (1.0-dWet));
				
				iirSampleGR = (iirSampleGR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleGR * dWet) + (inputSampleR * (1.0-dWet));
			}
		}
		else
		{
			if (aWet > 0.0) {
				iirSampleBL = (iirSampleBL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleBL * aWet) + (inputSampleL * (1.0-aWet));
				
				iirSampleBR = (iirSampleBR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleBR * aWet) + (inputSampleR * (1.0-aWet));
			}
			if (bWet > 0.0) {
				iirSampleDL = (iirSampleDL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleDL * bWet) + (inputSampleL * (1.0-bWet));
				
				iirSampleDR = (iirSampleDR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleDR * bWet) + (inputSampleR * (1.0-bWet));
			}
			if (cWet > 0.0) {
				iirSampleFL = (iirSampleFL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleFL * cWet) + (inputSampleL * (1.0-cWet));
				
				iirSampleFR = (iirSampleFR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleFR * cWet) + (inputSampleR * (1.0-cWet));
			}
			if (dWet > 0.0) {
				iirSampleHL = (iirSampleHL * (1 - (offsetL * iirAmount))) + (inputSampleL * (offsetL * iirAmount));
				inputSampleL = (iirSampleHL * dWet) + (inputSampleL * (1.0-dWet));
				
				iirSampleHR = (iirSampleHR * (1 - (offsetR * iirAmount))) + (inputSampleR * (offsetR * iirAmount));
				inputSampleR = (iirSampleHR * dWet) + (inputSampleR * (1.0-dWet));
			}
		}
		
		if (wet !=1.0) {
			inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
			inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		}
		//Dry/Wet control, defaults to the last slider
		fpFlip = !fpFlip;
		
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
