/* ========================================
 *  Disintegrate - Disintegrate.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __Disintegrate_H
#include "Disintegrate.h"
#endif

void Disintegrate::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double refdB = 60.0+(A*80.0);
	double topdB = 0.000000064 * pow(10.0,refdB/20.0) * overallscale;
	int dscBuf = (B*(double)(dscBufMax-1))+1;
	int layers = (C*20.0);
	double f = pow(D,2);
	double boost = 1.0 + (f/(layers+1));
	if (f == 0.0) f = 0.000001;
	double wet = E;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL *= topdB;
		inputSampleR *= topdB;
		
		for (int x = 0; x < layers; x++) {
			inputSampleL *= boost;
			if (inputSampleL < -0.222) inputSampleL = -0.222;
			if (inputSampleL > 0.222) inputSampleL = 0.222;
			dBaL[dBaXL[x]][x] = inputSampleL;
			dBaPosL[x] *= (1.0-f); dBaPosL[x] += (dBaPosBL[x]*f); 
			dBaPosBL[x] *= (1.0-f); dBaPosBL[x] += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*f);
			int dBdly = floor(dBaPosL[x]*dscBuf);
			double dBi = (dBaPosL[x]*dscBuf)-dBdly;
			inputSampleL  = dBaL[dBaXL[x]-dBdly+((dBaXL[x]-dBdly<0)?dscBuf:0)][x] * (1.0-dBi);
			dBdly++; inputSampleL += dBaL[dBaXL[x]-dBdly+((dBaXL[x]-dBdly<0)?dscBuf:0)][x] * dBi;
			dBaXL[x]++; if (dBaXL[x] < 0 || dBaXL[x] >= dscBuf) dBaXL[x] = 0;
		}
		for (int x = 0; x < layers; x++) {
			inputSampleR *= boost;
			if (inputSampleR < -0.222) inputSampleR = -0.222;
			if (inputSampleR > 0.222) inputSampleR = 0.222;
			dBaR[dBaXR[x]][x] = inputSampleR;
			dBaPosR[x] *= (1.0-f); dBaPosR[x] += (dBaPosBR[x]*f); 
			dBaPosBR[x] *= (1.0-f); dBaPosBR[x] += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*f);
			int dBdly = floor(dBaPosR[x]*dscBuf);
			double dBi = (dBaPosR[x]*dscBuf)-dBdly;
			inputSampleR  = dBaR[dBaXR[x]-dBdly+((dBaXR[x]-dBdly<0)?dscBuf:0)][x] * (1.0-dBi);
			dBdly++; inputSampleR += dBaR[dBaXR[x]-dBdly+((dBaXR[x]-dBdly<0)?dscBuf:0)][x] * dBi;
			dBaXR[x]++; if (dBaXR[x] < 0 || dBaXR[x] >= dscBuf) dBaXR[x] = 0;
		} //This is being done this way, rather than all together in one loop, because
		//the hope is that all this repetitive processing on a small group of variables
		//can be more easily cached and optimized if we don't act like they must be done together.
		
		outFilterL *= f; outFilterL += (inputSampleL*(1.0-f)); inputSampleL = outFilterL;
		inputSampleL /= topdB;
		inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
		
		outFilterR *= f; outFilterR += (inputSampleR*(1.0-f)); inputSampleR = outFilterR;
		inputSampleR /= topdB;
		inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		
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

void Disintegrate::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	double refdB = 60.0+(A*80.0);
	double topdB = 0.000000064 * pow(10.0,refdB/20.0) * overallscale;
	int dscBuf = (B*(double)(dscBufMax-1))+1;
	int layers = (C*20.0);
	double f = pow(D,2);
	double boost = 1.0 + (f/(layers+1));
	if (f == 0.0) f = 0.000001;
	double wet = E;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		inputSampleL *= topdB;
		inputSampleR *= topdB;
		
		for (int x = 0; x < layers; x++) {
			inputSampleL *= boost;
			if (inputSampleL < -0.222) inputSampleL = -0.222;
			if (inputSampleL > 0.222) inputSampleL = 0.222;
			dBaL[dBaXL[x]][x] = inputSampleL;
			dBaPosL[x] *= (1.0-f); dBaPosL[x] += (dBaPosBL[x]*f); 
			dBaPosBL[x] *= (1.0-f); dBaPosBL[x] += fabs((inputSampleL*((inputSampleL*0.25)-0.5))*f);
			int dBdly = floor(dBaPosL[x]*dscBuf);
			double dBi = (dBaPosL[x]*dscBuf)-dBdly;
			inputSampleL  = dBaL[dBaXL[x]-dBdly+((dBaXL[x]-dBdly<0)?dscBuf:0)][x] * (1.0-dBi);
			dBdly++; inputSampleL += dBaL[dBaXL[x]-dBdly+((dBaXL[x]-dBdly<0)?dscBuf:0)][x] * dBi;
			dBaXL[x]++; if (dBaXL[x] < 0 || dBaXL[x] >= dscBuf) dBaXL[x] = 0;
		}
		for (int x = 0; x < layers; x++) {
			inputSampleR *= boost;
			if (inputSampleR < -0.222) inputSampleR = -0.222;
			if (inputSampleR > 0.222) inputSampleR = 0.222;
			dBaR[dBaXR[x]][x] = inputSampleR;
			dBaPosR[x] *= (1.0-f); dBaPosR[x] += (dBaPosBR[x]*f); 
			dBaPosBR[x] *= (1.0-f); dBaPosBR[x] += fabs((inputSampleR*((inputSampleR*0.25)-0.5))*f);
			int dBdly = floor(dBaPosR[x]*dscBuf);
			double dBi = (dBaPosR[x]*dscBuf)-dBdly;
			inputSampleR  = dBaR[dBaXR[x]-dBdly+((dBaXR[x]-dBdly<0)?dscBuf:0)][x] * (1.0-dBi);
			dBdly++; inputSampleR += dBaR[dBaXR[x]-dBdly+((dBaXR[x]-dBdly<0)?dscBuf:0)][x] * dBi;
			dBaXR[x]++; if (dBaXR[x] < 0 || dBaXR[x] >= dscBuf) dBaXR[x] = 0;
		} //This is being done this way, rather than all together in one loop, because
		//the hope is that all this repetitive processing on a small group of variables
		//can be more easily cached and optimized if we don't act like they must be done together.
		
		outFilterL *= f; outFilterL += (inputSampleL*(1.0-f)); inputSampleL = outFilterL;
		inputSampleL /= topdB;
		inputSampleL = (inputSampleL * wet) + (drySampleL * (1.0-wet));
		
		outFilterR *= f; outFilterR += (inputSampleR*(1.0-f)); inputSampleR = outFilterR;
		inputSampleR /= topdB;
		inputSampleR = (inputSampleR * wet) + (drySampleR * (1.0-wet));
		
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
