/* ========================================
 *  Elliptical - Elliptical.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Elliptical_H
#include "Elliptical.h"
#endif

void Elliptical::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double rangescale = 0.1 / overallscale;
	double cutoff = pow(A,3);
	double slope = pow(B,3) * 6.0;
	double newA = cutoff * rangescale;
	double newB = newA; //other part of interleaved IIR is the same
	double fpOld = 0.618033988749894848204586; //golden ratio!
	double newC = cutoff * rangescale; //first extra pole is the same
	cutoff = (cutoff * fpOld) + (0.00001 * (1.0-fpOld));
	double newD = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * (1.0-fpOld));
	double newE = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * (1.0-fpOld));
	double newF = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * (1.0-fpOld));
	double newG = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * (1.0-fpOld));
	double newH = cutoff * rangescale;
	//converge toward the unvarying fixed cutoff value
	double polesC = slope; if (slope > 1.0) polesC = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	double polesD = slope; if (slope > 1.0) polesD = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	double polesE = slope; if (slope > 1.0) polesE = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	double polesF = slope; if (slope > 1.0) polesF = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	double polesG = slope; if (slope > 1.0) polesG = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	double polesH = slope; if (slope > 1.0) polesH = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	//each one will either be 0.0, the fractional slope value, or 1
	
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
			iirA = (iirA * (1.0 - newA)) + (temp * newA); temp -= iirA; correction = iirA;
		} else {
			iirB = (iirB * (1.0 - newB)) + (temp * newB); temp -= iirB; correction = iirB;
		}
		iirC = (iirC * (1.0 - newC)) + (temp * newC); temp -= iirC;
		iirD = (iirD * (1.0 - newD)) + (temp * newD); temp -= iirD;
		iirE = (iirE * (1.0 - newE)) + (temp * newE); temp -= iirE;
		iirF = (iirF * (1.0 - newF)) + (temp * newF); temp -= iirF;
		iirG = (iirG * (1.0 - newG)) + (temp * newG); temp -= iirG;
		iirH = (iirH * (1.0 - newH)) + (temp * newH); temp -= iirH;
		//set up all the iir filters in case they are used
		
		if (polesC == 1.0) correction += iirC; if (polesC > 0.0 && polesC < 1.0) correction += (iirC * polesC);
		if (polesD == 1.0) correction += iirD; if (polesD > 0.0 && polesD < 1.0) correction += (iirD * polesD);
		if (polesE == 1.0) correction += iirE; if (polesE > 0.0 && polesE < 1.0) correction += (iirE * polesE);
		if (polesF == 1.0) correction += iirF; if (polesF > 0.0 && polesF < 1.0) correction += (iirF * polesF);
		if (polesG == 1.0) correction += iirG; if (polesG > 0.0 && polesG < 1.0) correction += (iirG * polesG);
		if (polesH == 1.0) correction += iirH; if (polesH > 0.0 && polesH < 1.0) correction += (iirH * polesH);
		//each of these are added directly if they're fully engaged,
		//multiplied by 0-1 if they are the interpolated one, or skipped if they are beyond the interpolated one.
		//the purpose is to do all the math at the floating point exponent nearest to the tiny value in use.
		//also, it's formatted that way to easily substitute the next variable: this could be written as a loop
		//with everything an array value. However, this makes just as much sense for this few poles.
		
		side -= correction;
		fpFlip = !fpFlip;
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		//unassign mid and side
		
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

void Elliptical::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double rangescale = 0.1 / overallscale;
	double cutoff = pow(A,3);
	double slope = pow(B,3) * 6.0;
	double newA = cutoff * rangescale;
	double newB = newA; //other part of interleaved IIR is the same
	double fpOld = 0.618033988749894848204586; //golden ratio!
	double newC = cutoff * rangescale; //first extra pole is the same
	cutoff = (cutoff * fpOld) + (0.00001 * (1.0-fpOld));
	double newD = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * (1.0-fpOld));
	double newE = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * (1.0-fpOld));
	double newF = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * (1.0-fpOld));
	double newG = cutoff * rangescale;
	cutoff = (cutoff * fpOld) + (0.00001 * (1.0-fpOld));
	double newH = cutoff * rangescale;
	//converge toward the unvarying fixed cutoff value
	double polesC = slope; if (slope > 1.0) polesC = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	double polesD = slope; if (slope > 1.0) polesD = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	double polesE = slope; if (slope > 1.0) polesE = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	double polesF = slope; if (slope > 1.0) polesF = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	double polesG = slope; if (slope > 1.0) polesG = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	double polesH = slope; if (slope > 1.0) polesH = 1.0; slope -= 1.0; if (slope < 0.0) slope = 0.0;
	//each one will either be 0.0, the fractional slope value, or 1
	
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
			iirA = (iirA * (1.0 - newA)) + (temp * newA); temp -= iirA; correction = iirA;
		} else {
			iirB = (iirB * (1.0 - newB)) + (temp * newB); temp -= iirB; correction = iirB;
		}
		iirC = (iirC * (1.0 - newC)) + (temp * newC); temp -= iirC;
		iirD = (iirD * (1.0 - newD)) + (temp * newD); temp -= iirD;
		iirE = (iirE * (1.0 - newE)) + (temp * newE); temp -= iirE;
		iirF = (iirF * (1.0 - newF)) + (temp * newF); temp -= iirF;
		iirG = (iirG * (1.0 - newG)) + (temp * newG); temp -= iirG;
		iirH = (iirH * (1.0 - newH)) + (temp * newH); temp -= iirH;
		//set up all the iir filters in case they are used
		
		if (polesC == 1.0) correction += iirC; if (polesC > 0.0 && polesC < 1.0) correction += (iirC * polesC);
		if (polesD == 1.0) correction += iirD; if (polesD > 0.0 && polesD < 1.0) correction += (iirD * polesD);
		if (polesE == 1.0) correction += iirE; if (polesE > 0.0 && polesE < 1.0) correction += (iirE * polesE);
		if (polesF == 1.0) correction += iirF; if (polesF > 0.0 && polesF < 1.0) correction += (iirF * polesF);
		if (polesG == 1.0) correction += iirG; if (polesG > 0.0 && polesG < 1.0) correction += (iirG * polesG);
		if (polesH == 1.0) correction += iirH; if (polesH > 0.0 && polesH < 1.0) correction += (iirH * polesH);
		//each of these are added directly if they're fully engaged,
		//multiplied by 0-1 if they are the interpolated one, or skipped if they are beyond the interpolated one.
		//the purpose is to do all the math at the floating point exponent nearest to the tiny value in use.
		//also, it's formatted that way to easily substitute the next variable: this could be written as a loop
		//with everything an array value. However, this makes just as much sense for this few poles.
		
		side -= correction;
		fpFlip = !fpFlip;
		
		inputSampleL = (mid+side)/2.0;
		inputSampleR = (mid-side)/2.0;
		//unassign mid and side
		
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
