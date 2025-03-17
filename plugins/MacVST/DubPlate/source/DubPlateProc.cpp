/* ========================================
 *  DubPlate - DubPlate.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DubPlate_H
#include "DubPlate.h"
#endif

void DubPlate::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double rangescale = 0.1 / overallscale;
	double iirSide = 0.287496 * rangescale;
	double iirMid = 0.20123 * rangescale;
	double threshSinewA = 0.15 / overallscale;
	double threshSinewB = 0.127 / overallscale;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double mid = inputSampleL + inputSampleR;
		double side = inputSampleL - inputSampleR;
		//assign mid and side.Between these sections, you can do mid/side processing
		double temp = side;
		double correction;
		
		if (fpFlip) {
			iirA = (iirA * (1.0 - iirSide)) + (temp * iirSide); temp -= iirA; correction = iirA;
		} else {
			iirB = (iirB * (1.0 - iirSide)) + (temp * iirSide); temp -= iirB; correction = iirB;
		}
		iirC = (iirC * (1.0 - iirSide)) + (temp * iirSide); temp -= iirC; correction += (iirC * 0.162);
		
		side -= sin(correction);
		fpFlip = !fpFlip;
		
		iirD = (iirD * (1.0 - iirMid)) + (mid * iirMid);
		mid -= sin(iirD);
		//gonna cut those lows a hair
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		//unassign mid and side
		
		temp = inputSampleL;
		double sinew = threshSinewA * cos(lastSinewAL*lastSinewAL);
		if (inputSampleL - lastSinewAL > sinew) temp = lastSinewAL + sinew;
		if (-(inputSampleL - lastSinewAL) > sinew) temp = lastSinewAL - sinew;
		lastSinewAL = temp;
		if (lastSinewAL > 1.0) lastSinewAL = 1.0;
		if (lastSinewAL < -1.0) lastSinewAL = -1.0;
		inputSampleL = (inputSampleL * 0.5)+(lastSinewAL * 0.5);
		
		sinew = threshSinewB * cos(lastSinewBL*lastSinewBL);
		if (inputSampleL - lastSinewBL > sinew) temp = lastSinewBL + sinew;
		if (-(inputSampleL - lastSinewBL) > sinew) temp = lastSinewBL - sinew;
		lastSinewBL = temp;
		if (lastSinewBL > 1.0) lastSinewBL = 1.0;
		if (lastSinewBL < -1.0) lastSinewBL = -1.0;
		inputSampleL = (inputSampleL * 0.414)+(lastSinewBL * 0.586);
		
		temp = inputSampleR;
		sinew = threshSinewA * cos(lastSinewAR*lastSinewAR);
		if (inputSampleR - lastSinewAR > sinew) temp = lastSinewAR + sinew;
		if (-(inputSampleR - lastSinewAR) > sinew) temp = lastSinewAR - sinew;
		lastSinewAR = temp;
		if (lastSinewAR > 1.0) lastSinewAR = 1.0;
		if (lastSinewAR < -1.0) lastSinewAR = -1.0;
		inputSampleR = (inputSampleR * 0.5)+(lastSinewAR * 0.5);
		
		sinew = threshSinewB * cos(lastSinewBR*lastSinewBR);
		if (inputSampleR - lastSinewBR > sinew) temp = lastSinewBR + sinew;
		if (-(inputSampleR - lastSinewBR) > sinew) temp = lastSinewBR - sinew;
		lastSinewBR = temp;
		if (lastSinewBR > 1.0) lastSinewBR = 1.0;
		if (lastSinewBR < -1.0) lastSinewBR = -1.0;
		inputSampleR = (inputSampleR * 0.414)+(lastSinewBR * 0.586);
		//run Sinew to stop excess slews, two layers to make it more audible
		
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

void DubPlate::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double rangescale = 0.1 / overallscale;
	double iirSide = 0.287496 * rangescale;
	double iirMid = 0.20123 * rangescale;
	double threshSinewA = 0.15 / overallscale;
	double threshSinewB = 0.127 / overallscale;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		double mid = inputSampleL + inputSampleR;
		double side = inputSampleL - inputSampleR;
		//assign mid and side.Between these sections, you can do mid/side processing
		double temp = side;
		double correction;
		
		if (fpFlip) {
			iirA = (iirA * (1.0 - iirSide)) + (temp * iirSide); temp -= iirA; correction = iirA;
		} else {
			iirB = (iirB * (1.0 - iirSide)) + (temp * iirSide); temp -= iirB; correction = iirB;
		}
		iirC = (iirC * (1.0 - iirSide)) + (temp * iirSide); temp -= iirC; correction += (iirC * 0.162);
		
		side -= sin(correction);
		fpFlip = !fpFlip;
		
		iirD = (iirD * (1.0 - iirMid)) + (mid * iirMid);
		mid -= sin(iirD);
		//gonna cut those lows a hair
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		//unassign mid and side
		
		temp = inputSampleL;
		double sinew = threshSinewA * cos(lastSinewAL*lastSinewAL);
		if (inputSampleL - lastSinewAL > sinew) temp = lastSinewAL + sinew;
		if (-(inputSampleL - lastSinewAL) > sinew) temp = lastSinewAL - sinew;
		lastSinewAL = temp;
		if (lastSinewAL > 1.0) lastSinewAL = 1.0;
		if (lastSinewAL < -1.0) lastSinewAL = -1.0;
		inputSampleL = (inputSampleL * 0.5)+(lastSinewAL * 0.5);
		
		sinew = threshSinewB * cos(lastSinewBL*lastSinewBL);
		if (inputSampleL - lastSinewBL > sinew) temp = lastSinewBL + sinew;
		if (-(inputSampleL - lastSinewBL) > sinew) temp = lastSinewBL - sinew;
		lastSinewBL = temp;
		if (lastSinewBL > 1.0) lastSinewBL = 1.0;
		if (lastSinewBL < -1.0) lastSinewBL = -1.0;
		inputSampleL = (inputSampleL * 0.414)+(lastSinewBL * 0.586);
		
		temp = inputSampleR;
		sinew = threshSinewA * cos(lastSinewAR*lastSinewAR);
		if (inputSampleR - lastSinewAR > sinew) temp = lastSinewAR + sinew;
		if (-(inputSampleR - lastSinewAR) > sinew) temp = lastSinewAR - sinew;
		lastSinewAR = temp;
		if (lastSinewAR > 1.0) lastSinewAR = 1.0;
		if (lastSinewAR < -1.0) lastSinewAR = -1.0;
		inputSampleR = (inputSampleR * 0.5)+(lastSinewAR * 0.5);
		
		sinew = threshSinewB * cos(lastSinewBR*lastSinewBR);
		if (inputSampleR - lastSinewBR > sinew) temp = lastSinewBR + sinew;
		if (-(inputSampleR - lastSinewBR) > sinew) temp = lastSinewBR - sinew;
		lastSinewBR = temp;
		if (lastSinewBR > 1.0) lastSinewBR = 1.0;
		if (lastSinewBR < -1.0) lastSinewBR = -1.0;
		inputSampleR = (inputSampleR * 0.414)+(lastSinewBR * 0.586);
		//run Sinew to stop excess slews, two layers to make it more audible
		
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
