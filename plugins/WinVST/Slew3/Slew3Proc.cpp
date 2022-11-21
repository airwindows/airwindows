/* ========================================
 *  Slew3 - Slew3.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Slew3_H
#include "Slew3.h"
#endif

void Slew3::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();

	double threshold = pow((1-A),4)/overallscale;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//begin L
		double clamp = (lastSampleBL - lastSampleCL) * 0.381966011250105;
		clamp -= (lastSampleAL - lastSampleBL) * 0.6180339887498948482045;
		clamp += inputSampleL - lastSampleAL; //regular slew clamping added
		
		lastSampleCL = lastSampleBL;
		lastSampleBL = lastSampleAL;
		lastSampleAL = inputSampleL; //now our output relates off lastSampleB
		
		if (clamp > threshold)
			inputSampleL = lastSampleBL + threshold;
		if (-clamp > threshold)
			inputSampleL = lastSampleBL - threshold;

		lastSampleAL = (lastSampleAL*0.381966011250105)+(inputSampleL*0.6180339887498948482045); //split the difference between raw and smoothed for buffer
		//end L
		
		//begin R
		clamp = (lastSampleBR - lastSampleCR) * 0.381966011250105;
		clamp -= (lastSampleAR - lastSampleBR) * 0.6180339887498948482045;
		clamp += inputSampleR - lastSampleAR; //regular slew clamping added
		
		lastSampleCR = lastSampleBR;
		lastSampleBR = lastSampleAR;
		lastSampleAR = inputSampleR; //now our output relates off lastSampleB
		
		if (clamp > threshold)
			inputSampleR = lastSampleBR + threshold;
		if (-clamp > threshold)
			inputSampleR = lastSampleBR - threshold;
		
		lastSampleAR = (lastSampleAR*0.381966011250105)+(inputSampleR*0.6180339887498948482045); //split the difference between raw and smoothed for buffer
		//end R
		
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

void Slew3::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double threshold = pow((1-A),4)/overallscale;
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//begin L
		double clamp = (lastSampleBL - lastSampleCL) * 0.381966011250105;
		clamp -= (lastSampleAL - lastSampleBL) * 0.6180339887498948482045;
		clamp += inputSampleL - lastSampleAL; //regular slew clamping added
		
		lastSampleCL = lastSampleBL;
		lastSampleBL = lastSampleAL;
		lastSampleAL = inputSampleL; //now our output relates off lastSampleB
		
		if (clamp > threshold)
			inputSampleL = lastSampleBL + threshold;
		if (-clamp > threshold)
			inputSampleL = lastSampleBL - threshold;
		
		lastSampleAL = (lastSampleAL*0.381966011250105)+(inputSampleL*0.6180339887498948482045); //split the difference between raw and smoothed for buffer
		//end L
		
		//begin R
		clamp = (lastSampleBR - lastSampleCR) * 0.381966011250105;
		clamp -= (lastSampleAR - lastSampleBR) * 0.6180339887498948482045;
		clamp += inputSampleR - lastSampleAR; //regular slew clamping added
		
		lastSampleCR = lastSampleBR;
		lastSampleBR = lastSampleAR;
		lastSampleAR = inputSampleR; //now our output relates off lastSampleB
		
		if (clamp > threshold)
			inputSampleR = lastSampleBR + threshold;
		if (-clamp > threshold)
			inputSampleR = lastSampleBR - threshold;
		
		lastSampleAR = (lastSampleAR*0.381966011250105)+(inputSampleR*0.6180339887498948482045); //split the difference between raw and smoothed for buffer
		//end R
		
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
