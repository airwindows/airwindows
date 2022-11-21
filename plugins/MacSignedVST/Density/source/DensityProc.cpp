/* ========================================
 *  Density - Density.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Density_H
#include "Density.h"
#endif

void Density::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double density = (A*5.0)-1.0;
	double iirAmount = pow(B,3)/overallscale;
	double output = C;
	double wet = D;
	double dry = 1.0-wet;
	double bridgerectifier;
	double out = fabs(density);
	density = density * fabs(density);
	double count;

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

		if (fpFlip)
		{
			iirSampleAL = (iirSampleAL * (1.0 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL -= iirSampleAL;
			iirSampleAR = (iirSampleAR * (1.0 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR -= iirSampleAR;
		}
		else
		{
			iirSampleBL = (iirSampleBL * (1.0 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL -= iirSampleBL;
			iirSampleBR = (iirSampleBR * (1.0 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR -= iirSampleBR;
		}
		//highpass section
		fpFlip = !fpFlip;
		
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
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1.0-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1.0-out))-(bridgerectifier*out);
		//blend according to density control
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		if (wet < 1.0) {
			inputSampleL = (drySampleL * dry)+(inputSampleL * wet);
			inputSampleR = (drySampleR * dry)+(inputSampleR * wet);
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

void Density::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	double density = (A*5.0)-1.0;
	double iirAmount = pow(B,3)/overallscale;
	double output = C;
	double wet = D;
	double dry = 1.0-wet;
	double bridgerectifier;
	double out = fabs(density);
	density = density * fabs(density);
	double count;
	
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

		if (fpFlip)
		{
			iirSampleAL = (iirSampleAL * (1.0 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL -= iirSampleAL;
			iirSampleAR = (iirSampleAR * (1.0 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR -= iirSampleAR;
		}
		else
		{
			iirSampleBL = (iirSampleBL * (1.0 - iirAmount)) + (inputSampleL * iirAmount);
			inputSampleL -= iirSampleBL;
			iirSampleBR = (iirSampleBR * (1.0 - iirAmount)) + (inputSampleR * iirAmount);
			inputSampleR -= iirSampleBR;
		}
		//highpass section
		fpFlip = !fpFlip;
		
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
		if (inputSampleR > 0) inputSampleR = (inputSampleR*(1.0-out))+(bridgerectifier*out);
		else inputSampleR = (inputSampleR*(1.0-out))-(bridgerectifier*out);
		//blend according to density control
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		if (wet < 1.0) {
			inputSampleL = (drySampleL * dry)+(inputSampleL * wet);
			inputSampleR = (drySampleR * dry)+(inputSampleR * wet);
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