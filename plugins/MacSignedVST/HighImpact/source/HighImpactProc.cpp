/* ========================================
 *  HighImpact - HighImpact.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __HighImpact_H
#include "HighImpact.h"
#endif

void HighImpact::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];


	double inputSampleL;
	double inputSampleR;
	double drySampleL;
	double drySampleR;
	
	double density = A*5.0;
	double out = density / 5.0;
	double sustain = 1.0 - (1.0/(1.0 + (density*A)));
	double bridgerectifier;
	double count;
	double output = B;
	double wet = C;
	double dry = 1.0-wet;	
	double clamp;
	double threshold = (1.25 - out);
    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;

		count = density;
		while (count > 1.0)
		{
			bridgerectifier = fabs(inputSampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
			else inputSampleL = -bridgerectifier;
			
			bridgerectifier = fabs(inputSampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
			else inputSampleR = -bridgerectifier;
			
			count = count - 1.0;
		}
		//we have now accounted for any really high density settings.
		
		while (out > 1.0) out = out - 1.0;
		
		bridgerectifier = fabs(inputSampleL)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-out))+(bridgerectifier*out);
		else inputSampleL = (inputSampleL*(1-out))-(bridgerectifier*out);
		//blend according to density control

		bridgerectifier = fabs(inputSampleR)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1-out))-(bridgerectifier*out);
		//blend according to density control
		
		
		//done first density. Next, sustain-reducer
		bridgerectifier = fabs(inputSampleL)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = 1-cos(bridgerectifier);
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-sustain))+(bridgerectifier*sustain);
		else inputSampleL = (inputSampleL*(1-sustain))-(bridgerectifier*sustain);
		//done sustain removing, converted to Slew inputs

		//done first density. Next, sustain-reducer
		bridgerectifier = fabs(inputSampleR)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = 1-cos(bridgerectifier);
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-sustain))+(bridgerectifier*sustain);
		else inputSampleR = (inputSampleR*(1-sustain))-(bridgerectifier*sustain);
		//done sustain removing, converted to Slew inputs
		
		clamp = inputSampleL - lastSampleL;
		if (clamp > threshold)
			inputSampleL = lastSampleL + threshold;
		if (-clamp > threshold)
			inputSampleL = lastSampleL - threshold;
		lastSampleL = inputSampleL;

		clamp = inputSampleR - lastSampleR;
		if (clamp > threshold)
			inputSampleR = lastSampleR + threshold;
		if (-clamp > threshold)
			inputSampleR = lastSampleR - threshold;
		lastSampleR = inputSampleR;
		
		if (output < 1.0) {inputSampleL *= output; inputSampleR *= output;}
		if (wet < 1.0) {
			inputSampleL = (drySampleL * dry)+(inputSampleL*wet);
			inputSampleR = (drySampleR * dry)+(inputSampleR*wet);
		}
		//nice little output stage template: if we have another scale of floating point
		//number, we really don't want to meaninglessly multiply that by 1.0.

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

void HighImpact::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
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
	double drySampleL;
	double drySampleR;
	
	double density = A*5.0;
	double out = density / 5.0;
	double sustain = 1.0 - (1.0/(1.0 + (density*A)));
	double bridgerectifier;
	double count;
	double output = B;
	double wet = C;
	double dry = 1.0-wet;	
	double clamp;
	double threshold = (1.25 - out);
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		drySampleL = inputSampleL;
		drySampleR = inputSampleR;
		
		count = density;
		while (count > 1.0)
		{
			bridgerectifier = fabs(inputSampleL)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleL > 0.0) inputSampleL = bridgerectifier;
			else inputSampleL = -bridgerectifier;
			
			bridgerectifier = fabs(inputSampleR)*1.57079633;
			if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
			//max value for sine function
			bridgerectifier = sin(bridgerectifier);
			if (inputSampleR > 0.0) inputSampleR = bridgerectifier;
			else inputSampleR = -bridgerectifier;
			
			count = count - 1.0;
		}
		//we have now accounted for any really high density settings.
		
		while (out > 1.0) out = out - 1.0;
		
		bridgerectifier = fabs(inputSampleL)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-out))+(bridgerectifier*out);
		else inputSampleL = (inputSampleL*(1-out))-(bridgerectifier*out);
		//blend according to density control
		
		bridgerectifier = fabs(inputSampleR)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		//max value for sine function
		if (density > 0) bridgerectifier = sin(bridgerectifier);
		else bridgerectifier = 1-cos(bridgerectifier);
		//produce either boosted or starved version
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1-out))-(bridgerectifier*out);
		//blend according to density control
		
		
		//done first density. Next, sustain-reducer
		bridgerectifier = fabs(inputSampleL)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = 1-cos(bridgerectifier);
		if (inputSampleL > 0) inputSampleL = (inputSampleL*(1-sustain))+(bridgerectifier*sustain);
		else inputSampleL = (inputSampleL*(1-sustain))-(bridgerectifier*sustain);
		//done sustain removing, converted to Slew inputs
		
		//done first density. Next, sustain-reducer
		bridgerectifier = fabs(inputSampleR)*1.57079633;
		if (bridgerectifier > 1.57079633) bridgerectifier = 1.57079633;
		bridgerectifier = 1-cos(bridgerectifier);
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1-sustain))+(bridgerectifier*sustain);
		else inputSampleR = (inputSampleR*(1-sustain))-(bridgerectifier*sustain);
		//done sustain removing, converted to Slew inputs
		
		clamp = inputSampleL - lastSampleL;
		if (clamp > threshold)
			inputSampleL = lastSampleL + threshold;
		if (-clamp > threshold)
			inputSampleL = lastSampleL - threshold;
		lastSampleL = inputSampleL;
		
		clamp = inputSampleR - lastSampleR;
		if (clamp > threshold)
			inputSampleR = lastSampleR + threshold;
		if (-clamp > threshold)
			inputSampleR = lastSampleR - threshold;
		lastSampleR = inputSampleR;
		
		if (output < 1.0) {inputSampleL *= output; inputSampleR *= output;}
		if (wet < 1.0) {
			inputSampleL = (drySampleL * dry)+(inputSampleL*wet);
			inputSampleR = (drySampleR * dry)+(inputSampleR*wet);
		}
		//nice little output stage template: if we have another scale of floating point
		//number, we really don't want to meaninglessly multiply that by 1.0.
		
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