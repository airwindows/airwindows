/* ========================================
 *  NodeDither - NodeDither.h
 *  Copyright (c) 2016 airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __NodeDither_H
#include "NodeDither.h"
#endif

void NodeDither::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int offsetA = (int)((A*100) * overallscale);
	if (offsetA < 1) offsetA = 1;
	if (offsetA > 2440) offsetA = 2440;
	
	int phase = floor(B*1.999);
	//0 default is out of phase, 1 is in phase
	
	double inputSampleL;
	double inputSampleR;
	double currentDitherL;
	double currentDitherR;
	    
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= 8388608.0;
		inputSampleR *= 8388608.0;
		//0-1 is now one bit, now we dither

		if (gcount < 0 || gcount > 2450) {gcount = 2450;}
		
		currentDitherL = (double(fpdL)/UINT32_MAX);
		inputSampleL += currentDitherL;

		currentDitherR = (double(fpdR)/UINT32_MAX);
		inputSampleR += currentDitherR;

		if (phase == 1) {
			inputSampleL -= 1.0;
			inputSampleL += dL[gcount+offsetA];
			inputSampleR -= 1.0;
			inputSampleR += dR[gcount+offsetA];
		} else {
			inputSampleL -= dL[gcount+offsetA];
			inputSampleR -= dR[gcount+offsetA];
		}
		//in phase means adding, otherwise we subtract
		
		
		inputSampleL = floor(inputSampleL);
		inputSampleR = floor(inputSampleR);
		dL[gcount+2450] = dL[gcount] = currentDitherL;
		dR[gcount+2450] = dR[gcount] = currentDitherR;
		
		gcount--;
		
		inputSampleL /= 8388608.0;
		inputSampleR /= 8388608.0;
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//pseudorandom number updater

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}

void NodeDither::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	int offsetA = (int)((A*100) * overallscale);
	if (offsetA < 1) offsetA = 1;
	if (offsetA > 2440) offsetA = 2440;
	
	int phase = floor(B*1.999);
	//0 default is out of phase, 1 is in phase
	
	double inputSampleL;
	double inputSampleR;
	double currentDitherL;
	double currentDitherR;
	
    while (--sampleFrames >= 0)
    {
		inputSampleL = *in1;
		inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= 8388608.0;
		inputSampleR *= 8388608.0;
		//0-1 is now one bit, now we dither
		
		if (gcount < 0 || gcount > 2450) {gcount = 2450;}
		
		currentDitherL = (double(fpdL)/UINT32_MAX);
		inputSampleL += currentDitherL;
		
		currentDitherR = (double(fpdR)/UINT32_MAX);
		inputSampleR += currentDitherR;
		
		if (phase == 1) {
			inputSampleL -= 1.0;
			inputSampleL += dL[gcount+offsetA];
			inputSampleR -= 1.0;
			inputSampleR += dR[gcount+offsetA];
		} else {
			inputSampleL -= dL[gcount+offsetA];
			inputSampleR -= dR[gcount+offsetA];
		}
		//in phase means adding, otherwise we subtract
		
		
		inputSampleL = floor(inputSampleL);
		inputSampleR = floor(inputSampleR);
		dL[gcount+2450] = dL[gcount] = currentDitherL;
		dR[gcount+2450] = dR[gcount] = currentDitherR;
		
		gcount--;
		
		inputSampleL /= 8388608.0;
		inputSampleR /= 8388608.0;
		
		fpdL ^= fpdL << 13; fpdL ^= fpdL >> 17; fpdL ^= fpdL << 5;
		fpdR ^= fpdR << 13; fpdR ^= fpdR >> 17; fpdR ^= fpdR << 5;
		//pseudorandom number updater

		*out1 = inputSampleL;
		*out2 = inputSampleR;

		*in1++;
		*in2++;
		*out1++;
		*out2++;
    }
}