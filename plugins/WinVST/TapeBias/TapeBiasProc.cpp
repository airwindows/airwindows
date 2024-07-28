/* ========================================
 *  TapeBias - TapeBias.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __TapeBias_H
#include "TapeBias.h"
#endif

void TapeBias::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double bias = (A*2.0)-1.0;
	double underBias = (pow(bias,4)*0.25)/overallscale;
	double overBias = pow(1.0-bias,3)/overallscale;
	if (bias > 0.0) underBias = 0.0;
	if (bias < 0.0) overBias = 1.0/overallscale;
	
	gslew[threshold9] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold8] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold7] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold6] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold5] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold4] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold3] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold2] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold1] = overBias;
	overBias *= 1.618033988749894848204586;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//start bias routine
		for (int x = 0; x < gslew_total; x += 3) {
			if (underBias > 0.0) {
				double stuck = fabs(inputSampleL - (gslew[x]/0.975)) / underBias;
				if (stuck < 1.0) inputSampleL = (inputSampleL * stuck) + ((gslew[x]/0.975)*(1.0-stuck));
				stuck =  fabs(inputSampleR - (gslew[x+1]/0.975)) / underBias;
				if (stuck < 1.0) inputSampleR = (inputSampleR * stuck) + ((gslew[x+1]/0.975)*(1.0-stuck));
			}
			if ((inputSampleL - gslew[x]) > gslew[x+2]) inputSampleL = gslew[x] + gslew[x+2];
			if (-(inputSampleL - gslew[x]) > gslew[x+2]) inputSampleL = gslew[x] - gslew[x+2];
			gslew[x] = inputSampleL * 0.975;
			if ((inputSampleR - gslew[x+1]) > gslew[x+2]) inputSampleR = gslew[x+1] + gslew[x+2];
			if (-(inputSampleR - gslew[x+1]) > gslew[x+2]) inputSampleR = gslew[x+1] - gslew[x+2];
			gslew[x+1] = inputSampleR * 0.975;
		}
		//end bias routine
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void TapeBias::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double bias = (A*2.0)-1.0;
	double underBias = (pow(bias,4)*0.25)/overallscale;
	double overBias = pow(1.0-bias,3)/overallscale;
	if (bias > 0.0) underBias = 0.0;
	if (bias < 0.0) overBias = 1.0/overallscale;
	
	gslew[threshold9] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold8] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold7] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold6] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold5] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold4] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold3] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold2] = overBias;
	overBias *= 1.618033988749894848204586;
	gslew[threshold1] = overBias;
	overBias *= 1.618033988749894848204586;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		//start bias routine
		for (int x = 0; x < gslew_total; x += 3) {
			if (underBias > 0.0) {
				double stuck = fabs(inputSampleL - (gslew[x]/0.975)) / underBias;
				if (stuck < 1.0) inputSampleL = (inputSampleL * stuck) + ((gslew[x]/0.975)*(1.0-stuck));
				stuck =  fabs(inputSampleR - (gslew[x+1]/0.975)) / underBias;
				if (stuck < 1.0) inputSampleR = (inputSampleR * stuck) + ((gslew[x+1]/0.975)*(1.0-stuck));
			}
			if ((inputSampleL - gslew[x]) > gslew[x+2]) inputSampleL = gslew[x] + gslew[x+2];
			if (-(inputSampleL - gslew[x]) > gslew[x+2]) inputSampleL = gslew[x] - gslew[x+2];
			gslew[x] = inputSampleL * 0.975;
			if ((inputSampleR - gslew[x+1]) > gslew[x+2]) inputSampleR = gslew[x+1] + gslew[x+2];
			if (-(inputSampleR - gslew[x+1]) > gslew[x+2]) inputSampleR = gslew[x+1] - gslew[x+2];
			gslew[x+1] = inputSampleR * 0.975;
		}
		//end bias routine
		
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
