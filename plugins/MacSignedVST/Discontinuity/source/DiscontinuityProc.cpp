/* ========================================
 *  Discontinuity - Discontinuity.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Discontinuity_H
#include "Discontinuity.h"
#endif

void Discontinuity::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double topdB = 80.0+(A*40.0);
	
	topdB = 0.000000064 * pow(10.0,topdB/20.0) * overallscale;
	double dBpos;
	double dBi;
	int dBdly;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= topdB; inputSampleR *= topdB;
		if (inputSampleL < -0.999) inputSampleL = -0.999; if (inputSampleL > 0.999) inputSampleL = 0.999;
		if (inputSampleR < -0.999) inputSampleR = -0.999; if (inputSampleR > 0.999) inputSampleR = 0.999;
		//Air Discontinuity A begin
		dBaL[dBaX] = inputSampleL; dBpos = pow((inputSampleL-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleL = dBaL[dBaX-dBdly +((dBaX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBaL[dBaX-dBdly +((dBaX-dBdly < 0)?predelay:0)]*dBi;
		dBaR[dBaX] = inputSampleR; dBpos = pow((inputSampleR-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleR = dBaR[dBaX-dBdly +((dBaX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBaR[dBaX-dBdly +((dBaX-dBdly < 0)?predelay:0)]*dBi;
		dBaX++; if (dBaX < 0 || dBaX > predelay) dBaX = 0;
		//Air Discontinuity A end
		//Air Discontinuity B begin
		dBbL[dBbX] = inputSampleL; dBpos = pow((inputSampleL-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleL = dBbL[dBbX-dBdly +((dBbX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBbL[dBbX-dBdly +((dBbX-dBdly < 0)?predelay:0)]*dBi;
		dBbR[dBbX] = inputSampleR; dBpos = pow((inputSampleR-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleR = dBbR[dBbX-dBdly +((dBbX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBbR[dBbX-dBdly +((dBbX-dBdly < 0)?predelay:0)]*dBi;
		dBbX++; if (dBbX < 0 || dBbX > predelay) dBbX = 0;
		//Air Discontinuity B end
		//Air Discontinuity C begin
		dBcL[dBcX] = inputSampleL; dBpos = pow((inputSampleL-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleL = dBcL[dBcX-dBdly +((dBcX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBcL[dBcX-dBdly +((dBcX-dBdly < 0)?predelay:0)]*dBi;
		dBcR[dBcX] = inputSampleR; dBpos = pow((inputSampleR-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleR = dBcR[dBcX-dBdly +((dBcX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBcR[dBcX-dBdly +((dBcX-dBdly < 0)?predelay:0)]*dBi;
		dBcX++; if (dBcX < 0 || dBcX > predelay) dBcX = 0;
		//Air Discontinuity C end
		//Air Discontinuity D begin
		dBdL[dBdX] = inputSampleL; dBpos = pow((inputSampleL-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleL = dBdL[dBdX-dBdly +((dBdX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBdL[dBdX-dBdly +((dBdX-dBdly < 0)?predelay:0)]*dBi;
		dBdR[dBdX] = inputSampleR; dBpos = pow((inputSampleR-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleR = dBdR[dBdX-dBdly +((dBdX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBdR[dBdX-dBdly +((dBdX-dBdly < 0)?predelay:0)]*dBi;
		dBdX++; if (dBdX < 0 || dBdX > predelay) dBdX = 0;
		//Air Discontinuity D end
		inputSampleL /= topdB; inputSampleR /= topdB;
		
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

void Discontinuity::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double topdB = 80.0+(A*40.0);
	
	topdB = 0.000000064 * pow(10.0,topdB/20.0) * overallscale;
	double dBpos;
	double dBi;
	int dBdly;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		
		inputSampleL *= topdB; inputSampleR *= topdB;
		if (inputSampleL < -0.999) inputSampleL = -0.999; if (inputSampleL > 0.999) inputSampleL = 0.999;
		if (inputSampleR < -0.999) inputSampleR = -0.999; if (inputSampleR > 0.999) inputSampleR = 0.999;
		//Air Discontinuity A begin
		dBaL[dBaX] = inputSampleL; dBpos = pow((inputSampleL-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleL = dBaL[dBaX-dBdly +((dBaX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBaL[dBaX-dBdly +((dBaX-dBdly < 0)?predelay:0)]*dBi;
		dBaR[dBaX] = inputSampleR; dBpos = pow((inputSampleR-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleR = dBaR[dBaX-dBdly +((dBaX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBaR[dBaX-dBdly +((dBaX-dBdly < 0)?predelay:0)]*dBi;
		dBaX++; if (dBaX < 0 || dBaX > predelay) dBaX = 0;
		//Air Discontinuity A end
		//Air Discontinuity B begin
		dBbL[dBbX] = inputSampleL; dBpos = pow((inputSampleL-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleL = dBbL[dBbX-dBdly +((dBbX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBbL[dBbX-dBdly +((dBbX-dBdly < 0)?predelay:0)]*dBi;
		dBbR[dBbX] = inputSampleR; dBpos = pow((inputSampleR-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleR = dBbR[dBbX-dBdly +((dBbX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBbR[dBbX-dBdly +((dBbX-dBdly < 0)?predelay:0)]*dBi;
		dBbX++; if (dBbX < 0 || dBbX > predelay) dBbX = 0;
		//Air Discontinuity B end
		//Air Discontinuity C begin
		dBcL[dBcX] = inputSampleL; dBpos = pow((inputSampleL-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleL = dBcL[dBcX-dBdly +((dBcX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBcL[dBcX-dBdly +((dBcX-dBdly < 0)?predelay:0)]*dBi;
		dBcR[dBcX] = inputSampleR; dBpos = pow((inputSampleR-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleR = dBcR[dBcX-dBdly +((dBcX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBcR[dBcX-dBdly +((dBcX-dBdly < 0)?predelay:0)]*dBi;
		dBcX++; if (dBcX < 0 || dBcX > predelay) dBcX = 0;
		//Air Discontinuity C end
		//Air Discontinuity D begin
		dBdL[dBdX] = inputSampleL; dBpos = pow((inputSampleL-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleL = dBdL[dBdX-dBdly +((dBdX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleL += dBdL[dBdX-dBdly +((dBdX-dBdly < 0)?predelay:0)]*dBi;
		dBdR[dBdX] = inputSampleR; dBpos = pow((inputSampleR-1.0)*0.5,2);
		if (dBpos>1.0) dBpos=1.0; dBdly = floor(dBpos*predelay); dBi = (dBpos*predelay)-dBdly;
		inputSampleR = dBdR[dBdX-dBdly +((dBdX-dBdly < 0)?predelay:0)]*(1.0-dBi);
		dBdly++; inputSampleR += dBdR[dBdX-dBdly +((dBdX-dBdly < 0)?predelay:0)]*dBi;
		dBdX++; if (dBdX < 0 || dBdX > predelay) dBdX = 0;
		//Air Discontinuity D end
		inputSampleL /= topdB; inputSampleR /= topdB;
		
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
