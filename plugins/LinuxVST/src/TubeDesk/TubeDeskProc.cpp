/* ========================================
 *  TubeDesk - TubeDesk.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TubeDesk_H
#include "TubeDesk.h"
#endif

void TubeDesk::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double intensity = 0.4384938;
	double depthA = 549.0;
	int offsetA = (int)(depthA * overallscale);
	if (offsetA < 1) offsetA = 1;
	if (offsetA > 2440) offsetA = 2440;
	
	double clamp;
	double thickness;
	double out;
	double gain = 0.5;
	double slewgain = 0.128;	
	double prevslew = 0.105;
	double balanceB = 0.0001;
	slewgain *= overallscale;
	prevslew *= overallscale;
	balanceB /= overallscale;
	double balanceA = 1.0 - balanceB;
	double slew;
	double bridgerectifier;
	double combSample;
	
	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		
		if (gcount < 0 || gcount > 2450) {gcount = 2450;}
		
		//begin L
		dL[gcount+2450] = dL[gcount] = fabs(inputSampleL)*intensity;
		controlL += (dL[gcount] / offsetA);
		controlL -= (dL[gcount+offsetA] / offsetA);
		controlL -= 0.000001;
		clamp = 1;
		if (controlL < 0) {controlL = 0;}
		if (controlL > 1) {clamp -= (controlL - 1); controlL = 1;}
		if (clamp < 0.5) {clamp = 0.5;}
		//control = 0 to 1
		thickness = ((1.0 - controlL) * 2.0) - 1.0;
		out = fabs(thickness);		
		bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (thickness > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-out))+(bridgerectifier*out);
		else inputSampleL = (inputSampleL*(1-out))-(bridgerectifier*out);
		//blend according to density control
		inputSampleL *= clamp;
		slew = inputSampleL - lastSampleL;
		lastSampleL = inputSampleL;
		//Set up direct reference for slew
		bridgerectifier = fabs(slew*slewgain);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (slew > 0) slew = bridgerectifier/slewgain;
		else slew = -(bridgerectifier/slewgain);
		inputSampleL = (lastOutSampleL*balanceA) + (lastSampleL*balanceB) + slew;
		//go from last slewed, but include some raw values
		lastOutSampleL = inputSampleL;
		//Set up slewed reference
		combSample = fabs(drySampleL*lastSampleL);
		if (combSample > 1.0) combSample = 1.0;
		//bailout for very high input gains
		inputSampleL -= (lastSlewL * combSample * prevslew);
		lastSlewL = slew;
		//slew interaction with previous slew
		inputSampleL *= gain;
		bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		//drive section
		inputSampleL /= gain;
		//end of Desk section
		//end L

		//begin R
		dR[gcount+2450] = dR[gcount] = fabs(inputSampleR)*intensity;
		controlR += (dR[gcount] / offsetA);
		controlR -= (dR[gcount+offsetA] / offsetA);
		controlR -= 0.000001;
		clamp = 1;
		if (controlR < 0) {controlR = 0;}
		if (controlR > 1) {clamp -= (controlR - 1); controlR = 1;}
		if (clamp < 0.5) {clamp = 0.5;}
		//control = 0 to 1
		thickness = ((1.0 - controlR) * 2.0) - 1.0;
		out = fabs(thickness);		
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (thickness > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1-out))-(bridgerectifier*out);
		//blend according to density control
		inputSampleR *= clamp;
		slew = inputSampleR - lastSampleR;
		lastSampleR = inputSampleR;
		//Set up direct reference for slew
		bridgerectifier = fabs(slew*slewgain);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (slew > 0) slew = bridgerectifier/slewgain;
		else slew = -(bridgerectifier/slewgain);
		inputSampleR = (lastOutSampleR*balanceA) + (lastSampleR*balanceB) + slew;
		//go from last slewed, but include some raw values
		lastOutSampleR = inputSampleR;
		//Set up slewed reference
		combSample = fabs(drySampleR*lastSampleR);
		if (combSample > 1.0) combSample = 1.0;
		//bailout for very high input gains
		inputSampleR -= (lastSlewR * combSample * prevslew);
		lastSlewR = slew;
		//slew interaction with previous slew
		inputSampleR *= gain;
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		//drive section
		inputSampleR /= gain;
		//end of Desk section
		//end R
		
		gcount--;
				
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

void TubeDesk::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double intensity = 0.4384938;
	double depthA = 549.0;
	int offsetA = (int)(depthA * overallscale);
	if (offsetA < 1) offsetA = 1;
	if (offsetA > 2440) offsetA = 2440;
	
	double clamp;
	double thickness;
	double out;
	double gain = 0.5;
	double slewgain = 0.128;	
	double prevslew = 0.105;
	double balanceB = 0.0001;
	slewgain *= overallscale;
	prevslew *= overallscale;
	balanceB /= overallscale;
	double balanceA = 1.0 - balanceB;
	double slew;
	double bridgerectifier;
	double combSample;
	
	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		
		if (gcount < 0 || gcount > 2450) {gcount = 2450;}
		
		//begin L
		dL[gcount+2450] = dL[gcount] = fabs(inputSampleL)*intensity;
		controlL += (dL[gcount] / offsetA);
		controlL -= (dL[gcount+offsetA] / offsetA);
		controlL -= 0.000001;
		clamp = 1;
		if (controlL < 0) {controlL = 0;}
		if (controlL > 1) {clamp -= (controlL - 1); controlL = 1;}
		if (clamp < 0.5) {clamp = 0.5;}
		//control = 0 to 1
		thickness = ((1.0 - controlL) * 2.0) - 1.0;
		out = fabs(thickness);		
		bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (thickness > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-out))+(bridgerectifier*out);
		else inputSampleL = (inputSampleL*(1-out))-(bridgerectifier*out);
		//blend according to density control
		inputSampleL *= clamp;
		slew = inputSampleL - lastSampleL;
		lastSampleL = inputSampleL;
		//Set up direct reference for slew
		bridgerectifier = fabs(slew*slewgain);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (slew > 0) slew = bridgerectifier/slewgain;
		else slew = -(bridgerectifier/slewgain);
		inputSampleL = (lastOutSampleL*balanceA) + (lastSampleL*balanceB) + slew;
		//go from last slewed, but include some raw values
		lastOutSampleL = inputSampleL;
		//Set up slewed reference
		combSample = fabs(drySampleL*lastSampleL);
		if (combSample > 1.0) combSample = 1.0;
		//bailout for very high input gains
		inputSampleL -= (lastSlewL * combSample * prevslew);
		lastSlewL = slew;
		//slew interaction with previous slew
		inputSampleL *= gain;
		bridgerectifier = fabs(inputSampleL);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (inputSampleL > 0) inputSampleL = bridgerectifier;
		else inputSampleL = -bridgerectifier;
		//drive section
		inputSampleL /= gain;
		//end of Desk section
		//end L
		
		//begin R
		dR[gcount+2450] = dR[gcount] = fabs(inputSampleR)*intensity;
		controlR += (dR[gcount] / offsetA);
		controlR -= (dR[gcount+offsetA] / offsetA);
		controlR -= 0.000001;
		clamp = 1;
		if (controlR < 0) {controlR = 0;}
		if (controlR > 1) {clamp -= (controlR - 1); controlR = 1;}
		if (clamp < 0.5) {clamp = 0.5;}
		//control = 0 to 1
		thickness = ((1.0 - controlR) * 2.0) - 1.0;
		out = fabs(thickness);		
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (thickness > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1-out))-(bridgerectifier*out);
		//blend according to density control
		inputSampleR *= clamp;
		slew = inputSampleR - lastSampleR;
		lastSampleR = inputSampleR;
		//Set up direct reference for slew
		bridgerectifier = fabs(slew*slewgain);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (slew > 0) slew = bridgerectifier/slewgain;
		else slew = -(bridgerectifier/slewgain);
		inputSampleR = (lastOutSampleR*balanceA) + (lastSampleR*balanceB) + slew;
		//go from last slewed, but include some raw values
		lastOutSampleR = inputSampleR;
		//Set up slewed reference
		combSample = fabs(drySampleR*lastSampleR);
		if (combSample > 1.0) combSample = 1.0;
		//bailout for very high input gains
		inputSampleR -= (lastSlewR * combSample * prevslew);
		lastSlewR = slew;
		//slew interaction with previous slew
		inputSampleR *= gain;
		bridgerectifier = fabs(inputSampleR);
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.0;
		else bridgerectifier = sin(bridgerectifier);
		if (inputSampleR > 0) inputSampleR = bridgerectifier;
		else inputSampleR = -bridgerectifier;
		//drive section
		inputSampleR /= gain;
		//end of Desk section
		//end R
		
		gcount--;
		
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