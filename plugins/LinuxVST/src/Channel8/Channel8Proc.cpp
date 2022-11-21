/* ========================================
 *  Channel8 - Channel8.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Channel8_H
#include "Channel8.h"
#endif

void Channel8::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();	
	double localiirAmount = iirAmount / overallscale;
	double localthreshold = threshold; //we've learned not to try and adjust threshold for sample rate
	double density = drive*2.0; //0-2
	double phattity = density - 1.0;
	if (density > 1.0) density = 1.0; //max out at full wet for Spiral aspect
	if (phattity < 0.0) phattity = 0.0; //
	double nonLin = 5.0-density; //number is smaller for more intense, larger for more subtle
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double dielectricScaleL = fabs(2.0-((inputSampleL+nonLin)/nonLin));
		double dielectricScaleR = fabs(2.0-((inputSampleR+nonLin)/nonLin));

		if (flip)
		{
			iirSampleLA = (iirSampleLA * (1.0 - (localiirAmount * dielectricScaleL))) + (inputSampleL * localiirAmount * dielectricScaleL);
			inputSampleL = inputSampleL - iirSampleLA;
			iirSampleRA = (iirSampleRA * (1.0 - (localiirAmount * dielectricScaleR))) + (inputSampleR * localiirAmount * dielectricScaleR);
			inputSampleR = inputSampleR - iirSampleRA;
		}
		else
		{
			iirSampleLB = (iirSampleLB * (1.0 - (localiirAmount * dielectricScaleL))) + (inputSampleL * localiirAmount * dielectricScaleL);
			inputSampleL = inputSampleL - iirSampleLB;
			iirSampleRB = (iirSampleRB * (1.0 - (localiirAmount * dielectricScaleR))) + (inputSampleR * localiirAmount * dielectricScaleR);
			inputSampleR = inputSampleR - iirSampleRB;
		}
		//highpass section
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		double phatSampleL = sin(inputSampleL * 1.57079633);
		inputSampleL *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output, or 1.57079633 for pure sine 'phat' version
		
		double distSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		
		inputSampleL = distSampleL; //purest form is full Spiral
		if (density < 1.0) inputSampleL = (drySampleL*(1-density))+(distSampleL*density); //fade Spiral aspect
		if (phattity > 0.0) inputSampleL = (inputSampleL*(1-phattity))+(phatSampleL*phattity); //apply original Density on top
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		double phatSampleR = sin(inputSampleR * 1.57079633);
		inputSampleR *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output, or 1.57079633 for pure sine 'phat' version
		
		double distSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		
		inputSampleR = distSampleR; //purest form is full Spiral
		if (density < 1.0) inputSampleR = (drySampleR*(1-density))+(distSampleR*density); //fade Spiral aspect
		if (phattity > 0.0) inputSampleR = (inputSampleR*(1-phattity))+(phatSampleR*phattity); //apply original Density on top
		
		//begin L
		double clamp = (lastSampleBL - lastSampleCL) * 0.381966011250105;
		clamp -= (lastSampleAL - lastSampleBL) * 0.6180339887498948482045;
		clamp += inputSampleL - lastSampleAL; //regular slew clamping added
		
		lastSampleCL = lastSampleBL;
		lastSampleBL = lastSampleAL;
		lastSampleAL = inputSampleL; //now our output relates off lastSampleB
		
		if (clamp > localthreshold)
			inputSampleL = lastSampleBL + localthreshold;
		if (-clamp > localthreshold)
			inputSampleL = lastSampleBL - localthreshold;
		
		lastSampleAL = (lastSampleAL*0.381966011250105)+(inputSampleL*0.6180339887498948482045); //split the difference between raw and smoothed for buffer
		//end L
		
		//begin R
		clamp = (lastSampleBR - lastSampleCR) * 0.381966011250105;
		clamp -= (lastSampleAR - lastSampleBR) * 0.6180339887498948482045;
		clamp += inputSampleR - lastSampleAR; //regular slew clamping added
		
		lastSampleCR = lastSampleBR;
		lastSampleBR = lastSampleAR;
		lastSampleAR = inputSampleR; //now our output relates off lastSampleB
		
		if (clamp > localthreshold)
			inputSampleR = lastSampleBR + localthreshold;
		if (-clamp > localthreshold)
			inputSampleR = lastSampleBR - localthreshold;
		
		lastSampleAR = (lastSampleAR*0.381966011250105)+(inputSampleR*0.6180339887498948482045); //split the difference between raw and smoothed for buffer
		//end R
		
		flip = !flip;
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void Channel8::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();	
	double localiirAmount = iirAmount / overallscale;
	double localthreshold = threshold; //we've learned not to try and adjust threshold for sample rate
	double density = drive*2.0; //0-2
	double phattity = density - 1.0;
	if (density > 1.0) density = 1.0; //max out at full wet for Spiral aspect
	if (phattity < 0.0) phattity = 0.0; //
	double nonLin = 5.0-density; //number is smaller for more intense, larger for more subtle
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double dielectricScaleL = fabs(2.0-((inputSampleL+nonLin)/nonLin));
		double dielectricScaleR = fabs(2.0-((inputSampleR+nonLin)/nonLin));
		
		if (flip)
		{
			iirSampleLA = (iirSampleLA * (1.0 - (localiirAmount * dielectricScaleL))) + (inputSampleL * localiirAmount * dielectricScaleL);
			inputSampleL = inputSampleL - iirSampleLA;
			iirSampleRA = (iirSampleRA * (1.0 - (localiirAmount * dielectricScaleR))) + (inputSampleR * localiirAmount * dielectricScaleR);
			inputSampleR = inputSampleR - iirSampleRA;
		}
		else
		{
			iirSampleLB = (iirSampleLB * (1.0 - (localiirAmount * dielectricScaleL))) + (inputSampleL * localiirAmount * dielectricScaleL);
			inputSampleL = inputSampleL - iirSampleLB;
			iirSampleRB = (iirSampleRB * (1.0 - (localiirAmount * dielectricScaleR))) + (inputSampleR * localiirAmount * dielectricScaleR);
			inputSampleR = inputSampleR - iirSampleRB;
		}
		//highpass section
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (inputSampleL > 1.0) inputSampleL = 1.0;
		if (inputSampleL < -1.0) inputSampleL = -1.0;
		double phatSampleL = sin(inputSampleL * 1.57079633);
		inputSampleL *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output, or 1.57079633 for pure sine 'phat' version
		
		double distSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		
		inputSampleL = distSampleL; //purest form is full Spiral
		if (density < 1.0) inputSampleL = (drySampleL*(1-density))+(distSampleL*density); //fade Spiral aspect
		if (phattity > 0.0) inputSampleL = (inputSampleL*(1-phattity))+(phatSampleL*phattity); //apply original Density on top
		
		if (inputSampleR > 1.0) inputSampleR = 1.0;
		if (inputSampleR < -1.0) inputSampleR = -1.0;
		double phatSampleR = sin(inputSampleR * 1.57079633);
		inputSampleR *= 1.2533141373155;
		//clip to 1.2533141373155 to reach maximum output, or 1.57079633 for pure sine 'phat' version
		
		double distSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		
		inputSampleR = distSampleR; //purest form is full Spiral
		if (density < 1.0) inputSampleR = (drySampleR*(1-density))+(distSampleR*density); //fade Spiral aspect
		if (phattity > 0.0) inputSampleR = (inputSampleR*(1-phattity))+(phatSampleR*phattity); //apply original Density on top
		
		//begin L
		double clamp = (lastSampleBL - lastSampleCL) * 0.381966011250105;
		clamp -= (lastSampleAL - lastSampleBL) * 0.6180339887498948482045;
		clamp += inputSampleL - lastSampleAL; //regular slew clamping added
		
		lastSampleCL = lastSampleBL;
		lastSampleBL = lastSampleAL;
		lastSampleAL = inputSampleL; //now our output relates off lastSampleB
		
		if (clamp > localthreshold)
			inputSampleL = lastSampleBL + localthreshold;
		if (-clamp > localthreshold)
			inputSampleL = lastSampleBL - localthreshold;
		
		lastSampleAL = (lastSampleAL*0.381966011250105)+(inputSampleL*0.6180339887498948482045); //split the difference between raw and smoothed for buffer
		//end L
		
		//begin R
		clamp = (lastSampleBR - lastSampleCR) * 0.381966011250105;
		clamp -= (lastSampleAR - lastSampleBR) * 0.6180339887498948482045;
		clamp += inputSampleR - lastSampleAR; //regular slew clamping added
		
		lastSampleCR = lastSampleBR;
		lastSampleBR = lastSampleAR;
		lastSampleAR = inputSampleR; //now our output relates off lastSampleB
		
		if (clamp > localthreshold)
			inputSampleR = lastSampleBR + localthreshold;
		if (-clamp > localthreshold)
			inputSampleR = lastSampleBR - localthreshold;
		
		lastSampleAR = (lastSampleAR*0.381966011250105)+(inputSampleR*0.6180339887498948482045); //split the difference between raw and smoothed for buffer
		//end R
		
		flip = !flip;
		
		if (output < 1.0) {
			inputSampleL *= output;
			inputSampleR *= output;
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

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}
