/* ========================================
 *  Zoom - Zoom.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Zoom_H
#include "Zoom.h"
#endif

void Zoom::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double zoom = (A*2.0)-1.0;
	double zoomStages = (fabs(zoom)*4.0)+1.0;
	for (int count = 0; count < sqrt(zoomStages); count++) zoom *= fabs(zoom);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		for (int count = 0; count < zoomStages; count++) {
			if (zoom > 0.0) {
				double closer = inputSampleL * 1.57079633;
				if (closer > 1.57079633) closer = 1.57079633;
				if (closer < -1.57079633) closer = -1.57079633;
				inputSampleL = (inputSampleL*(1.0-zoom))+(sin(closer)*zoom);
				closer = inputSampleR * 1.57079633;
				if (closer > 1.57079633) closer = 1.57079633;
				if (closer < -1.57079633) closer = -1.57079633;
				inputSampleR = (inputSampleR*(1.0-zoom))+(sin(closer)*zoom);
			} //zooming in will make the body of the sound louder: it's just Density
			if (zoom < 0.0) {
				double farther = fabs(inputSampleL) * 1.57079633;
				if (farther > 1.57079633) farther = 1.0;
				else farther = 1.0-cos(farther);
				if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(1.0+zoom))-(farther*zoom*1.57079633);
				if (inputSampleL < 0.0) inputSampleL = (inputSampleL*(1.0+zoom))+(farther*zoom*1.57079633);			
				farther = fabs(inputSampleR) * 1.57079633;
				if (farther > 1.57079633) farther = 1.0;
				else farther = 1.0-cos(farther);
				if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(1.0+zoom))-(farther*zoom*1.57079633);
				if (inputSampleR < 0.0) inputSampleR = (inputSampleR*(1.0+zoom))+(farther*zoom*1.57079633);			
			} //zooming out boosts the hottest peaks but cuts back softer stuff
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}

void Zoom::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double zoom = (A*2.0)-1.0;
	double zoomStages = (fabs(zoom)*4.0)+1.0;
	for (int count = 0; count < sqrt(zoomStages); count++) zoom *= fabs(zoom);
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		for (int count = 0; count < zoomStages; count++) {
			if (zoom > 0.0) {
				double closer = inputSampleL * 1.57079633;
				if (closer > 1.57079633) closer = 1.57079633;
				if (closer < -1.57079633) closer = -1.57079633;
				inputSampleL = (inputSampleL*(1.0-zoom))+(sin(closer)*zoom);
				closer = inputSampleR * 1.57079633;
				if (closer > 1.57079633) closer = 1.57079633;
				if (closer < -1.57079633) closer = -1.57079633;
				inputSampleR = (inputSampleR*(1.0-zoom))+(sin(closer)*zoom);
			} //zooming in will make the body of the sound louder: it's just Density
			if (zoom < 0.0) {
				double farther = fabs(inputSampleL) * 1.57079633;
				if (farther > 1.57079633) farther = 1.0;
				else farther = 1.0-cos(farther);
				if (inputSampleL > 0.0) inputSampleL = (inputSampleL*(1.0+zoom))-(farther*zoom*1.57079633);
				if (inputSampleL < 0.0) inputSampleL = (inputSampleL*(1.0+zoom))+(farther*zoom*1.57079633);			
				farther = fabs(inputSampleR) * 1.57079633;
				if (farther > 1.57079633) farther = 1.0;
				else farther = 1.0-cos(farther);
				if (inputSampleR > 0.0) inputSampleR = (inputSampleR*(1.0+zoom))-(farther*zoom*1.57079633);
				if (inputSampleR < 0.0) inputSampleR = (inputSampleR*(1.0+zoom))+(farther*zoom*1.57079633);			
			} //zooming out boosts the hottest peaks but cuts back softer stuff
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

		in1++;
		in2++;
		out1++;
		out2++;
    }
}
