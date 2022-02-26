/* ========================================
 *  Channel6 - Channel6.h
 *  Copyright (c) 2016 airwindows, All rights reserved
 * ======================================== */

#ifndef __Channel6_H
#include "Channel6.h"
#endif

void Channel6::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();	
	const double localiirAmount = iirAmount / overallscale;
	const double localthreshold = threshold / overallscale;
	const double density = pow(drive,2); //this doesn't relate to the plugins Density and Drive much
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		
		if (fabs(inputSampleL)<1.18e-37) inputSampleL = fpd * 1.18e-37;
		if (fabs(inputSampleR)<1.18e-37) inputSampleR = fpd * 1.18e-37;

		
		if (flip)
		{
			iirSampleLA = (iirSampleLA * (1 - localiirAmount)) + (inputSampleL * localiirAmount);
			inputSampleL = inputSampleL - iirSampleLA;
			iirSampleRA = (iirSampleRA * (1 - localiirAmount)) + (inputSampleR * localiirAmount);
			inputSampleR = inputSampleR - iirSampleRA;
		}
		else
		{
			iirSampleLB = (iirSampleLB * (1 - localiirAmount)) + (inputSampleL * localiirAmount);
			inputSampleL = inputSampleL - iirSampleLB;
			iirSampleRB = (iirSampleRB * (1 - localiirAmount)) + (inputSampleR * localiirAmount);
			inputSampleR = inputSampleR - iirSampleRB;
		}
		//highpass section
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;

		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		
		double distSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		inputSampleL = (drySampleL*(1-density))+(distSampleL*density);
		//drive section
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		
		double distSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		inputSampleR = (drySampleR*(1-density))+(distSampleR*density);
		//drive section
		
		double clamp = inputSampleL - lastSampleL;
		if (clamp > localthreshold)
			inputSampleL = lastSampleL + localthreshold;
		if (-clamp > localthreshold)
			inputSampleL = lastSampleL - localthreshold;
		lastSampleL = inputSampleL;
		
		clamp = inputSampleR - lastSampleR;
		if (clamp > localthreshold)
			inputSampleR = lastSampleR + localthreshold;
		if (-clamp > localthreshold)
			inputSampleR = lastSampleR - localthreshold;
		lastSampleR = inputSampleR;
		//slew section
		flip = !flip;
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		
		//begin 32 bit stereo floating point dither
		int expon; frexpf((float)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		frexpf((float)inputSampleR, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleR += static_cast<int32_t>(fpd) * 5.960464655174751e-36L * pow(2,expon+62);
		//end 32 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Channel6::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();	
	const double localiirAmount = iirAmount / overallscale;
	const double localthreshold = threshold / overallscale;
	const double density = pow(drive,2); //this doesn't relate to the plugins Density and Drive much
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		
		if (fabs(inputSampleL)<1.18e-43) inputSampleL = fpd * 1.18e-43;
		if (fabs(inputSampleR)<1.18e-43) inputSampleR = fpd * 1.18e-43;

		
		if (flip)
		{
			iirSampleLA = (iirSampleLA * (1 - localiirAmount)) + (inputSampleL * localiirAmount);
			inputSampleL = inputSampleL - iirSampleLA;
			iirSampleRA = (iirSampleRA * (1 - localiirAmount)) + (inputSampleR * localiirAmount);
			inputSampleR = inputSampleR - iirSampleRA;
		}
		else
		{
			iirSampleLB = (iirSampleLB * (1 - localiirAmount)) + (inputSampleL * localiirAmount);
			inputSampleL = inputSampleL - iirSampleLB;
			iirSampleRB = (iirSampleRB * (1 - localiirAmount)) + (inputSampleR * localiirAmount);
			inputSampleR = inputSampleR - iirSampleRB;
		}
		//highpass section
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		inputSampleL *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		
		double distSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		inputSampleL = (drySampleL*(1-density))+(distSampleL*density);
		//drive section
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		inputSampleR *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output
		
		double distSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		inputSampleR = (drySampleR*(1-density))+(distSampleR*density);
		//drive section
		
		double clamp = inputSampleL - lastSampleL;
		if (clamp > localthreshold)
			inputSampleL = lastSampleL + localthreshold;
		if (-clamp > localthreshold)
			inputSampleL = lastSampleL - localthreshold;
		lastSampleL = inputSampleL;
		
		clamp = inputSampleR - lastSampleR;
		if (clamp > localthreshold)
			inputSampleR = lastSampleR + localthreshold;
		if (-clamp > localthreshold)
			inputSampleR = lastSampleR - localthreshold;
		lastSampleR = inputSampleR;
		//slew section
		flip = !flip;
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
		}
		
		//begin 64 bit stereo floating point dither
		int expon; frexp((double)inputSampleL, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleL += static_cast<int32_t>(fpd) * 1.110223024625156e-44L * pow(2,expon+62);
		frexp((double)inputSampleR, &expon);
		fpd ^= fpd << 13; fpd ^= fpd >> 17; fpd ^= fpd << 5;
		inputSampleR += static_cast<int32_t>(fpd) * 1.110223024625156e-44L * pow(2,expon+62);
		//end 64 bit stereo floating point dither
		
		*out1 = inputSampleL;
		*out2 = inputSampleR;
		
		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
