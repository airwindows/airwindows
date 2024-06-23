/* ========================================
 *  DeRez3 - DeRez3.h
 *  Copyright (c) airwindows, Airwindows uses the MIT license
 * ======================================== */

#ifndef __DeRez3_H
#include "DeRez3.h"
#endif

void DeRez3::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames) 
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	rezA = rezB;
	rezB = pow(A,3.0)/overallscale;
	bitA = bitB;
	bitB = (B*15.0)+1.0;
	wetA = wetB;
	wetB = C*2.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double rez = (rezA*temp)+(rezB*(1.0-temp));
		double bit = (bitA*temp)+(bitB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
		if (rez < 0.0005) rez = 0.0005;
		double bitFactor = pow(2.0,bit);
		double dry = 2.0 - wet;
		if (wet > 1.0) wet = 1.0;
		if (wet < 0.0) wet = 0.0;
		if (dry > 1.0) dry = 1.0;
		if (dry < 0.0) dry = 0.0;
		//this bitcrush makes 50% full dry AND full wet, not crossfaded.
		//that's so it can be on tracks without cutting back dry channel when adjusted
		
		inputSampleL *= bitFactor;		
		inputSampleL = floor(inputSampleL+(0.5/bitFactor));
		inputSampleL /= bitFactor;
		inputSampleR *= bitFactor;		
		inputSampleR = floor(inputSampleR+(0.5/bitFactor));
		inputSampleR /= bitFactor;
		
		bez[bez_cycle] += rez;
		bez[bez_SampL] += (inputSampleL * rez);
		bez[bez_SampR] += (inputSampleR * rez);
		if (bez[bez_cycle] > 1.0) {
			bez[bez_cycle] -= 1.0;
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double CBL = (bez[bez_CL]*(1.0-bez[bez_cycle]))+(bez[bez_BL]*bez[bez_cycle]);
		double CBR = (bez[bez_CR]*(1.0-bez[bez_cycle]))+(bez[bez_BR]*bez[bez_cycle]);
		double BAL = (bez[bez_BL]*(1.0-bez[bez_cycle]))+(bez[bez_AL]*bez[bez_cycle]);
		double BAR = (bez[bez_BR]*(1.0-bez[bez_cycle]))+(bez[bez_AR]*bez[bez_cycle]);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-bez[bez_cycle]))+(BAL*bez[bez_cycle]))*0.5;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-bez[bez_cycle]))+(BAR*bez[bez_cycle]))*0.5;
		
		inputSampleL = (wet*CBAL)+(dry*drySampleL);
		inputSampleR = (wet*CBAR)+(dry*drySampleR);
		
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

void DeRez3::processDoubleReplacing(double **inputs, double **outputs, VstInt32 sampleFrames) 
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];

	VstInt32 inFramesToProcess = sampleFrames; //vst doesn't give us this as a separate variable so we'll make it
	double overallscale = 1.0;
	overallscale /= 44100.0;
	overallscale *= getSampleRate();
	
	rezA = rezB;
	rezB = pow(A,3.0)/overallscale;
	bitA = bitB;
	bitB = (B*15.0)+1.0;
	wetA = wetB;
	wetB = C*2.0;
	
    while (--sampleFrames >= 0)
    {
		double inputSampleL = *in1;
		double inputSampleR = *in2;
		if (fabs(inputSampleL)<1.18e-23) inputSampleL = fpdL * 1.18e-17;
		if (fabs(inputSampleR)<1.18e-23) inputSampleR = fpdR * 1.18e-17;
		double drySampleL = inputSampleL;
		double drySampleR = inputSampleR;
		
		double temp = (double)sampleFrames/inFramesToProcess;
		double rez = (rezA*temp)+(rezB*(1.0-temp));
		double bit = (bitA*temp)+(bitB*(1.0-temp));
		double wet = (wetA*temp)+(wetB*(1.0-temp));
		if (rez < 0.0005) rez = 0.0005;
		double bitFactor = pow(2.0,bit);
		double dry = 2.0 - wet;
		if (wet > 1.0) wet = 1.0;
		if (wet < 0.0) wet = 0.0;
		if (dry > 1.0) dry = 1.0;
		if (dry < 0.0) dry = 0.0;
		//this bitcrush makes 50% full dry AND full wet, not crossfaded.
		//that's so it can be on tracks without cutting back dry channel when adjusted
		
		inputSampleL *= bitFactor;		
		inputSampleL = floor(inputSampleL+(0.5/bitFactor));
		inputSampleL /= bitFactor;
		inputSampleR *= bitFactor;		
		inputSampleR = floor(inputSampleR+(0.5/bitFactor));
		inputSampleR /= bitFactor;
		
		bez[bez_cycle] += rez;
		bez[bez_SampL] += (inputSampleL * rez);
		bez[bez_SampR] += (inputSampleR * rez);
		if (bez[bez_cycle] > 1.0) {
			bez[bez_cycle] -= 1.0;
			bez[bez_CL] = bez[bez_BL];
			bez[bez_BL] = bez[bez_AL];
			bez[bez_AL] = inputSampleL;
			bez[bez_SampL] = 0.0;
			bez[bez_CR] = bez[bez_BR];
			bez[bez_BR] = bez[bez_AR];
			bez[bez_AR] = inputSampleR;
			bez[bez_SampR] = 0.0;
		}
		double CBL = (bez[bez_CL]*(1.0-bez[bez_cycle]))+(bez[bez_BL]*bez[bez_cycle]);
		double CBR = (bez[bez_CR]*(1.0-bez[bez_cycle]))+(bez[bez_BR]*bez[bez_cycle]);
		double BAL = (bez[bez_BL]*(1.0-bez[bez_cycle]))+(bez[bez_AL]*bez[bez_cycle]);
		double BAR = (bez[bez_BR]*(1.0-bez[bez_cycle]))+(bez[bez_AR]*bez[bez_cycle]);
		double CBAL = (bez[bez_BL]+(CBL*(1.0-bez[bez_cycle]))+(BAL*bez[bez_cycle]))*0.5;
		double CBAR = (bez[bez_BR]+(CBR*(1.0-bez[bez_cycle]))+(BAR*bez[bez_cycle]))*0.5;
		
		inputSampleL = (wet*CBAL)+(dry*drySampleL);
		inputSampleR = (wet*CBAR)+(dry*drySampleR);
		
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
