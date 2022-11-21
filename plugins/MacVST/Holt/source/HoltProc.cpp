/* ========================================
 *  Holt - Holt.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Holt_H
#include "Holt.h"
#endif

void Holt::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double alpha = pow(A,4)+0.00001;
	if (alpha > 1.0) alpha = 1.0;
	double beta = (alpha * pow(B,2))+0.00001;
	alpha += ((1.0-beta)*pow(A,3)); //correct for droop in frequency
	if (alpha > 1.0) alpha = 1.0;
	
	double trend;
	double forecast; //defining these here because we're copying the routine four times
	
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
	
	double gain = D;	
	double wet = E;	
    
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		if (aWet > 0.0) {
			trend = (beta * (inputSampleL - previousSampleAL) + ((0.999-beta) * previousTrendAL));
			forecast = previousSampleAL + previousTrendAL;
			inputSampleL = (alpha * inputSampleL) + ((0.999-alpha) * forecast);
			previousSampleAL = inputSampleL; previousTrendAL = trend;
			inputSampleL = (inputSampleL * aWet) + (drySampleL * (1.0-aWet));

			trend = (beta * (inputSampleR - previousSampleAR) + ((0.999-beta) * previousTrendAR));
			forecast = previousSampleAR + previousTrendAR;
			inputSampleR = (alpha * inputSampleR) + ((0.999-alpha) * forecast);
			previousSampleAR = inputSampleR; previousTrendAR = trend;
			inputSampleR = (inputSampleR * aWet) + (drySampleR * (1.0-aWet));
		}
		
		if (bWet > 0.0) {
			trend = (beta * (inputSampleL - previousSampleBL) + ((0.999-beta) * previousTrendBL));
			forecast = previousSampleBL + previousTrendBL;
			inputSampleL = (alpha * inputSampleL) + ((0.999-alpha) * forecast);
			previousSampleBL = inputSampleL; previousTrendBL = trend;
			inputSampleL = (inputSampleL * bWet) + (previousSampleAL * (1.0-bWet));

			trend = (beta * (inputSampleR - previousSampleBR) + ((0.999-beta) * previousTrendBR));
			forecast = previousSampleBR + previousTrendBR;
			inputSampleR = (alpha * inputSampleR) + ((0.999-alpha) * forecast);
			previousSampleBR = inputSampleR; previousTrendBR = trend;
			inputSampleR = (inputSampleR * bWet) + (previousSampleAR * (1.0-bWet));
		}
		
		if (cWet > 0.0) {
			trend = (beta * (inputSampleL - previousSampleCL) + ((0.999-beta) * previousTrendCL));
			forecast = previousSampleCL + previousTrendCL;
			inputSampleL = (alpha * inputSampleL) + ((0.999-alpha) * forecast);
			previousSampleCL = inputSampleL; previousTrendCL = trend;
			inputSampleL = (inputSampleL * cWet) + (previousSampleBL * (1.0-cWet));

			trend = (beta * (inputSampleR - previousSampleCR) + ((0.999-beta) * previousTrendCR));
			forecast = previousSampleCR + previousTrendCR;
			inputSampleR = (alpha * inputSampleR) + ((0.999-alpha) * forecast);
			previousSampleCR = inputSampleR; previousTrendCR = trend;
			inputSampleR = (inputSampleR * cWet) + (previousSampleBR * (1.0-cWet));
		}
		
		if (dWet > 0.0) {
			trend = (beta * (inputSampleL - previousSampleDL) + ((0.999-beta) * previousTrendDL));
			forecast = previousSampleDL + previousTrendDL;
			inputSampleL = (alpha * inputSampleL) + ((0.999-alpha) * forecast);
			previousSampleDL = inputSampleL; previousTrendDL = trend;
			inputSampleL = (inputSampleL * dWet) + (previousSampleCL * (1.0-dWet));

			trend = (beta * (inputSampleR - previousSampleDR) + ((0.999-beta) * previousTrendDR));
			forecast = previousSampleDR + previousTrendDR;
			inputSampleR = (alpha * inputSampleR) + ((0.999-alpha) * forecast);
			previousSampleDR = inputSampleR; previousTrendDR = trend;
			inputSampleR = (inputSampleR * dWet) + (previousSampleCR * (1.0-dWet));
		}
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		}
		
		//clip to 1.2533141373155 to reach maximum output
		if (inputSampleL > 1.2533141373155) inputSampleL = 1.2533141373155;
		if (inputSampleL < -1.2533141373155) inputSampleL = -1.2533141373155;
		if (inputSampleR > 1.2533141373155) inputSampleR = 1.2533141373155;
		if (inputSampleR < -1.2533141373155) inputSampleR = -1.2533141373155;
		inputSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		inputSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		
		if (wet < 1.0) {
			inputSampleL = (inputSampleL*wet)+(drySampleL*(1.0-wet));
			inputSampleR = (inputSampleR*wet)+(drySampleR*(1.0-wet));
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

void Holt::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double alpha = pow(A,4)+0.00001;
	if (alpha > 1.0) alpha = 1.0;
	double beta = (alpha * pow(B,2))+0.00001;
	alpha += ((1.0-beta)*pow(A,3)); //correct for droop in frequency
	if (alpha > 1.0) alpha = 1.0;
	
	double trend;
	double forecast; //defining these here because we're copying the routine four times
	
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
	
	double gain = D;	
	double wet = E;	
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;

		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;

		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;

		if (aWet > 0.0) {
			trend = (beta * (inputSampleL - previousSampleAL) + ((0.999-beta) * previousTrendAL));
			forecast = previousSampleAL + previousTrendAL;
			inputSampleL = (alpha * inputSampleL) + ((0.999-alpha) * forecast);
			previousSampleAL = inputSampleL; previousTrendAL = trend;
			inputSampleL = (inputSampleL * aWet) + (drySampleL * (1.0-aWet));
			
			trend = (beta * (inputSampleR - previousSampleAR) + ((0.999-beta) * previousTrendAR));
			forecast = previousSampleAR + previousTrendAR;
			inputSampleR = (alpha * inputSampleR) + ((0.999-alpha) * forecast);
			previousSampleAR = inputSampleR; previousTrendAR = trend;
			inputSampleR = (inputSampleR * aWet) + (drySampleR * (1.0-aWet));
		}
		
		if (bWet > 0.0) {
			trend = (beta * (inputSampleL - previousSampleBL) + ((0.999-beta) * previousTrendBL));
			forecast = previousSampleBL + previousTrendBL;
			inputSampleL = (alpha * inputSampleL) + ((0.999-alpha) * forecast);
			previousSampleBL = inputSampleL; previousTrendBL = trend;
			inputSampleL = (inputSampleL * bWet) + (previousSampleAL * (1.0-bWet));
			
			trend = (beta * (inputSampleR - previousSampleBR) + ((0.999-beta) * previousTrendBR));
			forecast = previousSampleBR + previousTrendBR;
			inputSampleR = (alpha * inputSampleR) + ((0.999-alpha) * forecast);
			previousSampleBR = inputSampleR; previousTrendBR = trend;
			inputSampleR = (inputSampleR * bWet) + (previousSampleAR * (1.0-bWet));
		}
		
		if (cWet > 0.0) {
			trend = (beta * (inputSampleL - previousSampleCL) + ((0.999-beta) * previousTrendCL));
			forecast = previousSampleCL + previousTrendCL;
			inputSampleL = (alpha * inputSampleL) + ((0.999-alpha) * forecast);
			previousSampleCL = inputSampleL; previousTrendCL = trend;
			inputSampleL = (inputSampleL * cWet) + (previousSampleBL * (1.0-cWet));
			
			trend = (beta * (inputSampleR - previousSampleCR) + ((0.999-beta) * previousTrendCR));
			forecast = previousSampleCR + previousTrendCR;
			inputSampleR = (alpha * inputSampleR) + ((0.999-alpha) * forecast);
			previousSampleCR = inputSampleR; previousTrendCR = trend;
			inputSampleR = (inputSampleR * cWet) + (previousSampleBR * (1.0-cWet));
		}
		
		if (dWet > 0.0) {
			trend = (beta * (inputSampleL - previousSampleDL) + ((0.999-beta) * previousTrendDL));
			forecast = previousSampleDL + previousTrendDL;
			inputSampleL = (alpha * inputSampleL) + ((0.999-alpha) * forecast);
			previousSampleDL = inputSampleL; previousTrendDL = trend;
			inputSampleL = (inputSampleL * dWet) + (previousSampleCL * (1.0-dWet));
			
			trend = (beta * (inputSampleR - previousSampleDR) + ((0.999-beta) * previousTrendDR));
			forecast = previousSampleDR + previousTrendDR;
			inputSampleR = (alpha * inputSampleR) + ((0.999-alpha) * forecast);
			previousSampleDR = inputSampleR; previousTrendDR = trend;
			inputSampleR = (inputSampleR * dWet) + (previousSampleCR * (1.0-dWet));
		}
		
		if (gain < 1.0) {
			inputSampleL *= gain;
			inputSampleR *= gain;
		}
		
		//clip to 1.2533141373155 to reach maximum output
		if (inputSampleL > 1.2533141373155) inputSampleL = 1.2533141373155;
		if (inputSampleL < -1.2533141373155) inputSampleL = -1.2533141373155;
		if (inputSampleR > 1.2533141373155) inputSampleR = 1.2533141373155;
		if (inputSampleR < -1.2533141373155) inputSampleR = -1.2533141373155;
		inputSampleL = sin(inputSampleL * fabs(inputSampleL)) / ((fabs(inputSampleL) == 0.0) ?1:fabs(inputSampleL));
		inputSampleR = sin(inputSampleR * fabs(inputSampleR)) / ((fabs(inputSampleR) == 0.0) ?1:fabs(inputSampleR));
		
		if (wet < 1.0) {
			inputSampleL = (inputSampleL*wet)+(drySampleL*(1.0-wet));
			inputSampleR = (inputSampleR*wet)+(drySampleR*(1.0-wet));
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
